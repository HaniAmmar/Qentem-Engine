/**
 * @file MemoryBlock.hpp
 * @brief Fixed-size memory block with zero-overhead region tracking via bitfield.
 *
 * Provides a low-level, page-aligned memory region that is subdivided into
 * fixed-size aligned chunks. Allocation state is tracked using a compact
 * bitfield stored at the beginning of the block. Suitable for use in
 * high-performance memory managers.
 *
 * This component is header-only, non-STL, and designed for embedded or
 * high-throughput systems. No internal dynamic allocation is performed.
 *
 * @tparam Alignment_T The minimum alignment (and chunk size) in bytes.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT
 */

#ifndef QENTEM_MEMORY_BLOCK_HPP
#define QENTEM_MEMORY_BLOCK_HPP

#include "Qentem/SystemMemory.hpp"
#include "Qentem/Platform.hpp"

namespace Qentem {

/**
 * @brief Memory block with bitfield-based region tracking and strict alignment.
 *
 * Allocates a contiguous memory region aligned to system page boundaries and subdivides it
 * into fixed-size chunks tracked via a bitfield table. Provides zero-metadata allocation
 * and release via region reservation logic.
 *
 * @tparam Alignment_T Chunk alignment in bytes. Must be power of two.
 */
template <SizeT32 Alignment_T>
struct MemoryBlock {
    static constexpr SystemLong BITS_IN_CHAR        = 8;
    static constexpr SystemLong MAX_SYSTEM_INT_TYPE = ~SystemLong{0};
    static constexpr SizeT32    PTR_SIZE            = sizeof(SystemLong);
    static constexpr SizeT32    BIT_WIDTH           = (PTR_SIZE * 8U);
    static constexpr SystemLong ALIGNMENT_M1        = static_cast<SystemLong>(Alignment_T - 1U);
    static constexpr SystemLong ALIGNMENT_N         = ~ALIGNMENT_M1;

    QENTEM_INLINE MemoryBlock() noexcept        = default;
    MemoryBlock(const MemoryBlock &)            = delete;
    MemoryBlock &operator=(const MemoryBlock &) = delete;

    /**
     * @brief Constructs a memory block with at least the given capacity.
     *
     * Reserves a page-aligned memory region from the system. Internally aligns the
     * start of usable memory to the specified chunk alignment and initializes the
     * bitfield table for tracking chunk usage. If the requested capacity is less than
     * a system page, it will be rounded up. Otherwise, it is rounded up to the nearest page.
     *
     * @param capacity Minimum number of bytes to reserve (may be rounded up).
     */
    explicit MemoryBlock(SystemLong capacity) noexcept : capacity_{capacity} {
        static_assert((Alignment_T > 0) && ((Alignment_T & (Alignment_T - 1)) == 0),
                      "Alignment_T must be power-of-two");

        if (capacity_ > SystemMemory::GetPageSize()) {
            capacity_ = SystemMemory::AlignToPageSize(capacity_);
        } else {
            capacity_ = SystemMemory::GetPageSize();
        }

#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
        capacity_ += page_size; // Insure correct alignment
#endif

        base_raw_ = SystemMemory::Reserve(capacity_);

#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
        {
            const SystemLong aligned_address = ((reinterpret_cast<SystemLong>(base_raw_) + ALIGNMENT_M1) & ALIGNMENT_N);
            base_                            = reinterpret_cast<void *>(aligned_address);
        }
#endif

        table_size_ = (capacity_ / (Alignment_T * BITS_IN_CHAR));

        table_mask_shift_ = BIT_WIDTH;

        if (table_size_ < PTR_SIZE) {
            table_mask_shift_ = (static_cast<SizeT32>(capacity_ >> DefaultAlignmentBit()));
            table_size_       = PTR_SIZE;
        }

        const SystemLong usable_base_raw     = reinterpret_cast<SystemLong>((static_cast<char *>(base_) + table_size_));
        const SystemLong aligned_usable_base = ((usable_base_raw + ALIGNMENT_M1) & ALIGNMENT_N);
        const SystemLong unusable =
            (((table_size_ + (aligned_usable_base - usable_base_raw)) + ALIGNMENT_M1) & ALIGNMENT_N);

        table_size_ /= PTR_SIZE;

        SizeT32 unusable_bits    = static_cast<SizeT32>(unusable >> DefaultAlignmentBit());
        SizeT32 unusable_indices = (unusable_bits / BIT_WIDTH);

        table_size_ -= unusable_indices;
        table_mask_shift_ -= (unusable_bits - (unusable_indices * BIT_WIDTH));

        data_ = reinterpret_cast<char *>(aligned_usable_base);

        // Compute actual alignment of the usable region (lowest power-of-two divisor)
        data_alignment_ = (1U << Platform::FindFirstBit(aligned_usable_base));

        usable_size_ = (capacity_ - unusable);
        available_   = usable_size_;
    }

    QENTEM_INLINE ~MemoryBlock() noexcept {
        release();
    }

    QENTEM_INLINE MemoryBlock(MemoryBlock &&src) noexcept
        : base_raw_{src.base_raw_}, data_{src.data_}, usable_size_{src.usable_size_}, available_{src.available_},
          next_index_{src.next_index_}, table_size_{src.table_size_}, data_alignment_{src.data_alignment_},
          table_mask_shift_{src.table_mask_shift_}, capacity_{src.capacity_} {
#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
        base_ = src.base_;
#endif

        src.base_raw_ = nullptr;
    }

    QENTEM_INLINE MemoryBlock &operator=(MemoryBlock &&src) noexcept {
        if (this != &src) {
            release();

            base_raw_ = src.base_raw_;
#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
            base_ = src.base_;
#endif
            data_             = src.data_;
            usable_size_      = src.usable_size_;
            available_        = src.available_;
            next_index_       = src.next_index_;
            table_size_       = src.table_size_;
            data_alignment_   = src.data_alignment_;
            table_mask_shift_ = src.table_mask_shift_;
            capacity_         = src.capacity_;

            src.base_raw_ = nullptr;
        }

        return *this;
    }

    /**
     * @brief Returns the base address of the memory block.
     *
     * This is the start of the reserved region, including both the bitfield table
     * and the aligned usable memory.
     *
     * @return Pointer to the base of the reserved memory region.
     */
    QENTEM_INLINE void *Base() noexcept {
        return base_;
    }

    /**
     * @brief Returns the start of the usable, aligned memory region.
     *
     * This pointer is aligned according to the chunk alignment and follows the
     * allocation bitfield.
     *
     * @return Aligned pointer to the first usable byte of memory.
     */
    QENTEM_INLINE void *Data() noexcept {
        return data_;
    }

    /**
     * @brief Returns the actual alignment of the usable data pointer.
     *
     * This reflects the largest power-of-two divisor of the `Data()` address,
     * computed from the least significant set bit. It represents the true alignment
     * of the memory region, which may be equal to or greater than the requested alignment.
     *
     * @return Alignment (in bytes) of the usable memory region.
     */
    QENTEM_INLINE SizeT32 DataAlignment() noexcept {
        return data_alignment_;
    }

    /**
     * @brief Returns the shift amount corresponding to the chunk alignment.
     *
     * For example, if Alignment_T is 16, returns 4.
     */
    QENTEM_INLINE static constexpr SizeT32 DefaultAlignmentBit() noexcept {
        return Platform::FindFirstBit(Alignment_T);
    }

    QENTEM_INLINE const void *End() const noexcept {
        return (static_cast<const char *>(base_) + capacity_);
    }

    QENTEM_INLINE SystemLong &GetRefNextIndex() noexcept {
        return next_index_;
    }

    QENTEM_INLINE SystemLong TableSize() const noexcept {
        return table_size_;
    }

    QENTEM_INLINE static constexpr SizeT32 TableFirstBit() noexcept {
        // BIT_WIDTH = bits in one entry (e.g., 64); result = log2(BIT_WIDTH) = 6
        return Platform::FindFirstBit(BIT_WIDTH);
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

    /**
     * @brief Reserves a range of contiguous aligned chunks.
     *
     * Marks bits in the allocation table as used. Returns a pointer to the aligned region
     * corresponding to the given bit index and chunk size.
     *
     * @param bit_index Starting chunk index.
     * @param chunks Number of chunks to reserve.
     * @return Aligned pointer to reserved memory region.
     */
    void *ReserveRegion(SystemLong bit_index, SystemLong chunks) {
        SystemLong *table       = static_cast<SystemLong *>(Base());
        SystemLong  table_index = (bit_index >> TableFirstBit());

        void *ptr = (static_cast<char *>(Data()) + (bit_index << DefaultAlignmentBit()));
        bit_index -= (table_index << TableFirstBit());
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

        next_index_ += SystemLong(table[table_index] == MAX_SYSTEM_INT_TYPE);
        next_index_ = ((table_index != table_size_) ? table_index : 0);

        return ptr;
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

        next_index_ += SystemLong(table[table_index] == MAX_SYSTEM_INT_TYPE);
        next_index_ = ((table_index != table_size_) ? table_index : 0);
    }

    /**
     * @brief Releases a previously reserved memory region.
     *
     * Clears the corresponding bits in the allocation table, marking the specified
     * chunk range as free. The region must exactly match a prior reservation.
     *
     * @param ptr    Pointer returned by ReserveRegion().
     * @param chunks Number of chunks originally reserved.
     */
    void ReleaseRegion(void *ptr, SystemLong chunks) noexcept {
        SystemLong *table = static_cast<SystemLong *>(Base());
        SystemLong  table_index;
        SystemLong  bit_index;

        DecodeBitmapPosition(static_cast<SystemLong>(static_cast<char *>(ptr) - static_cast<char *>(Data())),
                             table_index, bit_index);

        SystemLong mask = MAX_SYSTEM_INT_TYPE;
        mask <<= ((chunks < BIT_WIDTH) ? (BIT_WIDTH - chunks) : 0);
        mask >>= bit_index;
        table[table_index] &= ~mask;

        chunks += bit_index;

        next_index_ = ((table_index <= next_index_) ? table_index : next_index_);

        while (chunks > BIT_WIDTH) {
            chunks -= BIT_WIDTH;
            mask = MAX_SYSTEM_INT_TYPE;
            mask <<= ((chunks < BIT_WIDTH) ? (BIT_WIDTH - chunks) : 0);
            ++table_index;
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
        table_index = (bit_index >> TableFirstBit());

        // Compute residual bit position within the selected 64-bit word.
        bit_index -= (table_index << TableFirstBit());
    }

  private:
    QENTEM_INLINE void release() {
        if (base_raw_ != nullptr) {
            SystemMemory::Release(base_raw_, capacity_);
        }
    }

#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
    void *base_raw_{nullptr};
    void *base_{nullptr};
#else
    union {
        void *base_raw_{nullptr};
        void *base_;
    };
#endif

    void      *data_{nullptr};
    SystemLong usable_size_{0};
    SystemLong available_{0};
    SystemLong next_index_{0};
    SystemLong table_size_{0};
    SizeT32    data_alignment_{0};
    SizeT32    table_mask_shift_{0};
    SystemLong capacity_{0};
};

} // namespace Qentem

#endif // QENTEM_MEMORY_BLOCK_HPP
