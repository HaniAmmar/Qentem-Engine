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
 * @date 2025
 * @copyright MIT
 */

#ifndef QENTEM_MEMORY_BLOCK_HPP
#define QENTEM_MEMORY_BLOCK_HPP

#include "SystemMemory.hpp"
#include "Platform.hpp"

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
    static constexpr SystemIntType BITS_IN_CHAR        = 8;
    static constexpr SystemIntType MAX_SYSTEM_INT_TYPE = ~SystemIntType{0};
    static constexpr SizeT32       PTR_SIZE            = sizeof(SystemIntType);
    static constexpr SizeT32       BIT_WIDTH           = (PTR_SIZE * 8U);
    static constexpr SystemIntType ALIGNMENT_M1        = static_cast<SystemIntType>(Alignment_T - 1U);
    static constexpr SystemIntType ALIGNMENT_N         = ~ALIGNMENT_M1;

    MemoryBlock() noexcept                      = default;
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
    QENTEM_INLINE explicit MemoryBlock(SystemIntType capacity) noexcept : capacity_{capacity} {
        static_assert((Alignment_T > 0) && ((Alignment_T & (Alignment_T - 1)) == 0),
                      "Alignment_T must be power-of-two");

        static const SystemIntType page_size = SystemMemory::PageSize();

        if (capacity_ > page_size) {
            // Round up to next page boundary
            capacity_ += (page_size - 1U);
            capacity_ &= ~(page_size - 1U);
        } else {
            capacity_ = page_size;
        }

#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
        capacity_ += page_size; // Insure correct alignment
#endif

        base_raw_ = SystemMemory::Reserve(capacity_);

#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
        {
            const SystemIntType aligned_address =
                ((reinterpret_cast<SystemIntType>(base_raw_) + ALIGNMENT_M1) & ALIGNMENT_N);
            base_ = reinterpret_cast<void *>(aligned_address);
        }
#endif

        table_size_ = (capacity_ / (Alignment_T * BITS_IN_CHAR));

        table_mask_shift_ = BIT_WIDTH;

        if (table_size_ < PTR_SIZE) {
            table_mask_shift_ = (static_cast<SizeT32>(capacity_ >> DefaultAlignmentBit()));
            table_size_       = PTR_SIZE;
        }

        const SystemIntType usable_base_raw =
            reinterpret_cast<SystemIntType>((static_cast<char *>(base_) + table_size_));
        const SystemIntType aligned_usable_base = ((usable_base_raw + ALIGNMENT_M1) & ALIGNMENT_N);
        const SystemIntType unusable =
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
    QENTEM_INLINE static QENTEM_CONST_EXPRESSION SizeT32 DefaultAlignmentBit() noexcept {
        return Platform::FindFirstBit(Alignment_T);
    }

    QENTEM_INLINE const void *End() const noexcept {
        return (static_cast<const char *>(base_) + capacity_);
    }

    QENTEM_INLINE SystemIntType &GetRefNextIndex() noexcept {
        return next_index_;
    }

    QENTEM_INLINE SystemIntType TableSize() const noexcept {
        return table_size_;
    }

    QENTEM_INLINE static constexpr SizeT32 TableFirstBit() noexcept {
        // BIT_WIDTH = bits in one entry (e.g., 64); result = log2(BIT_WIDTH) = 6
        return Platform::FindFirstBit(BIT_WIDTH);
    }

    QENTEM_INLINE SystemIntType Capacity() const noexcept {
        return capacity_;
    }

    QENTEM_INLINE bool IsEmpty() const noexcept {
        return (available_ == usable_size_);
    }

    QENTEM_INLINE SystemIntType Available() const noexcept {
        return available_;
    }

    QENTEM_INLINE SystemIntType UsableSize() const noexcept {
        return usable_size_;
    }

    QENTEM_INLINE void IncreaseAvailable(SystemIntType size) noexcept {
        available_ += size;
    }

    QENTEM_INLINE void DecreaseAvailable(SystemIntType size) noexcept {
        available_ -= size;
    }

    /**
     * @brief Clears the allocation table and resets all bits to 0 (free).
     *
     * Final table entry is masked to prevent allocation beyond usable capacity,
     * accounting for padding and unusable tail regions.
     */
    QENTEM_INLINE void ClearTable() noexcept {
        SystemIntType      *table         = static_cast<SystemIntType *>(base_);
        const SystemIntType table_size_m1 = (table_size_ - SystemIntType{1});
        const SystemIntType table_mask =
            ((table_mask_shift_ != BIT_WIDTH) ? (MAX_SYSTEM_INT_TYPE >> table_mask_shift_) : 0);

        {
            SystemIntType index = 0;
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
    void *ReserveRegion(SystemIntType bit_index, SystemIntType chunks) {
        SystemIntType *table       = static_cast<SystemIntType *>(Base());
        SystemIntType  table_index = (bit_index >> TableFirstBit());

        void *ptr = (static_cast<char *>(Data()) + (bit_index << DefaultAlignmentBit()));
        bit_index -= (table_index << TableFirstBit());
        SystemIntType mask = MAX_SYSTEM_INT_TYPE;
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

        // next_index_ *= static_cast<SystemIntType>(table_index != (table_size_ - SystemIntType{1}));

        return ptr;
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
    void ReleaseRegion(void *ptr, SystemIntType chunks) noexcept {
        SystemIntType *table    = static_cast<SystemIntType *>(Base());
        SystemIntType bit_index = (static_cast<SystemIntType>(static_cast<char *>(ptr) - static_cast<char *>(Data())) >>
                                   DefaultAlignmentBit());

        SystemIntType table_index = (bit_index >> TableFirstBit());
        bit_index -= (table_index << TableFirstBit());

        SystemIntType mask = MAX_SYSTEM_INT_TYPE;
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

  private:
    void release() {
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

    void         *data_{nullptr};
    SystemIntType usable_size_{0};
    SystemIntType available_{0};
    SystemIntType next_index_{0};
    SystemIntType table_size_{0};
    SizeT32       data_alignment_{0};
    SizeT32       table_mask_shift_{0};
    SystemIntType capacity_{0};
};

} // namespace Qentem

#endif // QENTEM_MEMORY_BLOCK_HPP
