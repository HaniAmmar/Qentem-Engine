/**
 * @file MemoryBlock.hpp
 * @brief Fixed-size memory block with metadata-free allocations and bitfield-based tracking.
 *
 * Provides a contiguous memory region subdivided into fixed-size aligned chunks.
 * Allocation state is tracked through a compact bitfield table stored within the
 * block, allowing allocation and release operations without per-allocation metadata.
 *
 * MemoryBlock serves as the foundation of ReserverCore and is designed for
 * high-performance allocation workloads where predictable memory layout,
 * alignment control, and efficient region reuse are required.
 *
 * @tparam Alignment_T Minimum allocation alignment and chunk size in bytes.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT
 */

#ifndef QENTEM_MEMORY_BLOCK_HPP
#define QENTEM_MEMORY_BLOCK_HPP

#include "Qentem/SystemMemory.hpp"
#include "Qentem/Platform.hpp"

#ifdef QENTEM_ENABLE_MEMORY_RECORD
#include "Qentem/MemoryRecord.hpp"
#endif

namespace Qentem {

template <SizeT32 Alignment_T>
struct MemoryBlock {
    static constexpr SystemLong MAX_SYSTEM_INT_TYPE = ~SystemLong{0};
    static constexpr SizeT32    BITS_IN_CHAR_SHIFT  = 3U;
    static constexpr SizeT32    PTR_SIZE            = sizeof(void *);
    static constexpr SizeT32    BIT_WIDTH           = (PTR_SIZE << BITS_IN_CHAR_SHIFT);
    static constexpr SizeT32    PTR_SIZE_SHIFT      = (PTR_SIZE == 8U ? 3 : 2);
    static constexpr SystemLong ALIGNMENT_MASK      = static_cast<SystemLong>(Alignment_T - 1U);
    static constexpr SystemLong ALIGNMENT_MASK_INV  = ~ALIGNMENT_MASK;

    QENTEM_INLINE MemoryBlock() noexcept        = default;
    MemoryBlock(const MemoryBlock &)            = delete;
    MemoryBlock &operator=(const MemoryBlock &) = delete;

    /**
     * @brief Constructs a memory block with at least the requested capacity.
     *
     * Reserves a contiguous memory region from the operating system and prepares it
     * for chunk-based allocation tracking. The requested capacity is rounded up to
     * at least one system page and aligned to a page boundary when larger sizes are
     * requested.
     *
     * During initialization:
     * - Space is reserved for the allocation bitfield table.
     * - The usable allocation area is aligned to `Alignment_T`.
     * - Regions occupied by the table and alignment padding are excluded from
     *   the usable allocation space.
     * - Internal tracking values are calculated for fast region reservation
     *   and release operations.
     *
     * The resulting block contains a compact bitfield table followed by an
     * aligned usable memory region. No per-allocation metadata is stored within
     * user allocations.
     *
     * @param capacity Minimum number of bytes to reserve. The final block size
     *                 may be increased to satisfy page-size requirements.
     */
    explicit MemoryBlock(SystemLong capacity) noexcept : capacity_{capacity} {
        static_assert((Alignment_T > 0) && ((Alignment_T & (Alignment_T - 1)) == 0),
                      "Alignment_T must be power-of-two");

        if (capacity_ > SystemMemory::GetPageSize()) {
            capacity_ = SystemMemory::AlignToPageSize(capacity_);
        } else {
            capacity_ = SystemMemory::GetPageSize();
        }

        base_ = SystemMemory::Reserve(capacity_);

        table_size_ = capacity_;
        table_size_ >>= DefaultAlignmentBit();
        table_size_ >>= BITS_IN_CHAR_SHIFT;

        table_mask_shift_ = BIT_WIDTH;

        if (table_size_ < PTR_SIZE) {
            table_mask_shift_ = (static_cast<SizeT32>(capacity_ >> DefaultAlignmentBit()));
            table_size_       = PTR_SIZE;
        }

        const SystemLong usable_base_raw     = reinterpret_cast<SystemLong>((static_cast<char *>(base_) + table_size_));
        const SystemLong aligned_usable_base = ((usable_base_raw + ALIGNMENT_MASK) & ALIGNMENT_MASK_INV);
        const SystemLong unusable =
            (((table_size_ + (aligned_usable_base - usable_base_raw)) + ALIGNMENT_MASK) & ALIGNMENT_MASK_INV);
        data_ = reinterpret_cast<char *>(aligned_usable_base);

        table_size_ >>= PTR_SIZE_SHIFT;

        const SizeT32 unusable_bits    = static_cast<SizeT32>(unusable >> DefaultAlignmentBit());
        const SizeT32 unusable_indices = (unusable_bits >> TableBitShift());

        table_size_ -= unusable_indices;
        table_mask_shift_ -= (unusable_bits - (unusable_indices << TableBitShift()));

        usable_size_ = (capacity_ - unusable);
        available_   = usable_size_;

#ifdef QENTEM_ENABLE_MEMORY_RECORD
        MemoryRecord::ReservedBlock(capacity_);
#endif
    }

    QENTEM_INLINE ~MemoryBlock() noexcept {
        release();
    }

    QENTEM_INLINE MemoryBlock(MemoryBlock &&src) noexcept
        : base_{src.base_}, data_{src.data_}, usable_size_{src.usable_size_}, available_{src.available_},
          next_index_{src.next_index_}, table_size_{src.table_size_}, capacity_{src.capacity_},
          table_mask_shift_{src.table_mask_shift_} {
        src.base_ = nullptr;
    }

    QENTEM_INLINE MemoryBlock &operator=(MemoryBlock &&src) noexcept {
        if (this != &src) {
            release();

            base_     = src.base_;
            src.base_ = nullptr;

            data_             = src.data_;
            usable_size_      = src.usable_size_;
            available_        = src.available_;
            next_index_       = src.next_index_;
            table_size_       = src.table_size_;
            capacity_         = src.capacity_;
            table_mask_shift_ = src.table_mask_shift_;
        }

        return *this;
    }

    /**
     * @brief Returns the base address of the memory block.
     *
     * This is the beginning of the reserved memory region and includes both
     * the allocation bitfield table and the usable allocation area.
     *
     * The returned pointer represents the entire block and may point to
     * allocator-managed data. It is not generally suitable for normal
     * allocation requests.
     *
     * @return Pointer to the beginning of the memory block.
     *
     * @see Data
     */
    QENTEM_INLINE void *Base() noexcept {
        return base_;
    }

    /**
     * @brief Returns the start of the usable allocation area.
     *
     * This pointer marks the first byte that may be returned by allocation
     * requests. It is aligned according to `Alignment_T` and always resides
     * after the allocation bitfield table and any required alignment padding.
     *
     * @return Pointer to the first allocatable byte of the block.
     *
     * @see Base
     */
    QENTEM_INLINE void *Data() noexcept {
        return data_;
    }

    QENTEM_INLINE const void *Data() const noexcept {
        return data_;
    }

    /**
     * @brief Returns log2(Alignment_T).
     *
     * Converts the allocation alignment into its corresponding bit shift.
     * This allows divisions and multiplications by the alignment size to
     * be performed using bitwise operations.
     *
     * Examples:
     * - Alignment_T = 8   -> returns 3
     * - Alignment_T = 16  -> returns 4
     * - Alignment_T = 64  -> returns 6
     *
     * @return Base-2 logarithm of Alignment_T.
     */
    QENTEM_INLINE static SizeT32 DefaultAlignmentBit() noexcept {
        return Platform::FindFirstBitConstexpr(Alignment_T);
    }

    /**
     * @brief Returns a reference to the allocation search hint.
     *
     * This index records the bitmap position from which the next allocation
     * search should begin, reducing repeated scans of fully occupied regions.
     *
     * @return Reference to the next bitmap index used during allocation searches.
     */
    QENTEM_INLINE SystemLong &GetRefNextIndex() noexcept {
        return next_index_;
    }

    /**
     * @brief Returns the number of bitmap entries used by this block.
     *
     * The returned value represents the size of the allocation tracking
     * table in units of SystemLong entries.
     *
     * @return Number of bitmap entries in the allocation table.
     */
    QENTEM_INLINE SystemLong TableSize() const noexcept {
        return table_size_;
    }

    /**
     * @brief Returns log2(BIT_WIDTH).
     *
     * Converts the number of bits stored in a bitmap entry into the shift
     * value used to translate between bit counts and bitmap table indices.
     *
     * Examples:
     * - BIT_WIDTH = 32 -> returns 5
     * - BIT_WIDTH = 64 -> returns 6
     *
     * @return Shift value corresponding to BIT_WIDTH.
     */
    QENTEM_INLINE static constexpr SizeT32 TableBitShift() noexcept {
        return Platform::FindFirstBitConstexpr(BIT_WIDTH);
    }

    QENTEM_INLINE SystemLong Capacity() const noexcept {
        return capacity_;
    }

    QENTEM_INLINE bool IsEmpty() const noexcept {
        return (available_ == usable_size_);
    }

    QENTEM_INLINE SystemLong Available() const noexcept {
        return available_;
    }

    QENTEM_INLINE SystemLong UsableSize() const noexcept {
        return usable_size_;
    }

    QENTEM_INLINE void IncreaseAvailable(SystemLong size) noexcept {
        available_ += size;
    }

    QENTEM_INLINE void DecreaseAvailable(SystemLong size) noexcept {
        available_ -= size;
    }

    QENTEM_INLINE const void *End() const noexcept {
        return (static_cast<const char *>(base_) + capacity_);
    }

    /**
     * @brief Clears the allocation table and resets all bits to 0 (free).
     *
     * Final table entry is masked to prevent allocation beyond usable capacity,
     * accounting for padding and unusable tail regions.
     */
    void ClearTable() noexcept {
        SystemLong      *table         = static_cast<SystemLong *>(base_);
        const SystemLong table_size_m1 = (table_size_ - SystemLong{1});
        const SystemLong table_mask =
            ((table_mask_shift_ != BIT_WIDTH) ? (MAX_SYSTEM_INT_TYPE >> table_mask_shift_) : 0);

        {
            SystemLong index = 0;
            while (index < table_size_m1) {
                table[index] = 0;
                ++index;
            }
        }

        // Prevent allocation beyond usable capacity in the last table entry
        table[table_size_m1] = table_mask;
    }

    void ReserveRegion(SystemLong bit_index, SystemLong chunks) {
        SystemLong *table       = static_cast<SystemLong *>(Base());
        SystemLong  table_index = (bit_index >> TableBitShift());

        bit_index -= (table_index << TableBitShift());
        SystemLong mask = MAX_SYSTEM_INT_TYPE;
        mask <<= ((chunks < BIT_WIDTH) ? (BIT_WIDTH - chunks) : 0);
        mask >>= bit_index;
        table[table_index] |= mask;

        chunks += bit_index;

        while (chunks > BIT_WIDTH) {
            chunks -= BIT_WIDTH;
            mask = MAX_SYSTEM_INT_TYPE;
            mask <<= ((chunks < BIT_WIDTH) ? (BIT_WIDTH - chunks) : 0);
            ++table_index;
            table[table_index] |= mask;
        }

        next_index_ += static_cast<SystemLong>(table[table_index] == MAX_SYSTEM_INT_TYPE);
        next_index_ = ((table_index != table_size_) ? table_index : 0);
    }

    void ReserveRegion(SystemLong table_index, SystemLong bit_index, SystemLong chunks) noexcept {
        SystemLong *table = static_cast<SystemLong *>(Base());
        SystemLong  mask  = MAX_SYSTEM_INT_TYPE;
        mask <<= ((chunks < BIT_WIDTH) ? (BIT_WIDTH - chunks) : 0);
        mask >>= bit_index;
        table[table_index] |= mask;

        chunks += bit_index;

        while (chunks > BIT_WIDTH) {
            chunks -= BIT_WIDTH;
            mask = MAX_SYSTEM_INT_TYPE;
            mask <<= ((chunks < BIT_WIDTH) ? (BIT_WIDTH - chunks) : 0);
            ++table_index;
            table[table_index] |= mask;
        }

        next_index_ += static_cast<SystemLong>(table[table_index] == MAX_SYSTEM_INT_TYPE);
        next_index_ = ((table_index != table_size_) ? table_index : 0);
    }

    void ReleaseRegion(void *ptr, SystemLong chunks) noexcept {
        SystemLong *table = static_cast<SystemLong *>(Base());
        SystemLong  table_index;
        SystemLong  bit_index;

        DecodeBitmapPosition(static_cast<SystemLong>(static_cast<char *>(ptr) - static_cast<char *>(Data())),
                             table_index, bit_index);

        SystemLong mask = MAX_SYSTEM_INT_TYPE;
        mask <<= ((chunks < BIT_WIDTH) ? (BIT_WIDTH - chunks) : 0);
        mask >>= bit_index;

#if defined(QENTEM_DEBUG) && !defined(_WIN32)
        if ((table[table_index] & mask) != mask) {
            __builtin_trap(); // Double free
        }
#endif
        table[table_index] &= ~mask;

        chunks += bit_index;

        next_index_ = ((table_index <= next_index_) ? table_index : next_index_);

        while (chunks > BIT_WIDTH) {
            chunks -= BIT_WIDTH;
            mask = MAX_SYSTEM_INT_TYPE;
            mask <<= ((chunks < BIT_WIDTH) ? (BIT_WIDTH - chunks) : 0);
            ++table_index;

#if defined(QENTEM_DEBUG) && !defined(_WIN32)
            if ((table[table_index] & mask) != mask) {
                __builtin_trap(); // Double free
            }
#endif
            table[table_index] &= ~mask;
        }
    }

    /**
     * @brief Decodes a relative pointer offset into its corresponding bitmap coordinates.
     *
     * This function calculates the exact bit-level location within the allocation bitmap
     * where a memory region begins. The pointer offset is first aligned to the allocation
     * unit size, then split into:
     * - `table_index`: the index of the 64-bit word in the bitmap table.
     * - `bit_index`: the bit offset within that word.
     *
     * @param ptr_int     The relative pointer offset (i.e., `ptr - block.Data()`).
     * @param table_index [out] Index into the bitmap table (each entry is a 64-bit word).
     * @param bit_index   [out] Bit offset within the selected word.
     */
    QENTEM_INLINE static void DecodeBitmapPosition(SystemLong ptr_int, SystemLong &table_index, SystemLong &bit_index) {
        // Convert byte offset into allocation unit index.
        bit_index = (ptr_int >> DefaultAlignmentBit());

        // Divide bit index by BIT_WIDTH (i.e., 64) to find table word index.
        table_index = (bit_index >> TableBitShift());

        // Compute residual bit position within the selected 64-bit word.
        bit_index -= (table_index << TableBitShift());
    }

  private:
    QENTEM_INLINE void release() {
        if (base_ != nullptr) {
            SystemMemory::Release(base_, capacity_);

#ifdef QENTEM_ENABLE_MEMORY_RECORD
            MemoryRecord::ReleasedBlock(capacity_);
#endif

            base_ = nullptr;
        }
    }

    void      *base_{nullptr};
    void      *data_{nullptr};
    SystemLong usable_size_{0};
    SystemLong available_{0};
    SystemLong next_index_{0};
    SystemLong table_size_{0};
    SystemLong capacity_{0};
    SizeT32    table_mask_shift_{0};
};

} // namespace Qentem

#endif // QENTEM_MEMORY_BLOCK_HPP
