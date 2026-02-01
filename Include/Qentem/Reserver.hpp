/**
 * @file Reserver.hpp
 * @brief High-performance memory reservation system with zero internal metadata.
 *
 * This module implements Qentem's low-level allocator — Reserver — a deterministic,
 * reusable memory region manager built to replace traditional heap mechanisms
 * with precise, high-efficiency block control.
 *
 * Each memory block is divided and tracked via compact bit tables, avoiding
 * per-region metadata and ensuring tight memory boundaries. Allocations are aligned,
 * coalescible, and free of runtime headers. All tracking is external and
 * architecture-aware, making it ideal for systems with strict performance constraints.
 *
 * Reserver is designed for per-core use, with one instance per logical CPU to avoid
 * contention and preserve cache locality. Memory requests are routed to the current
 * core's arena, with optional cross-core fallback for safe reclamation. The system
 * supports NUMA-friendly placement and configurable block sizes for tuning.
 *
 * Features:
 *  - Zero internal metadata: allocations remain clean and unencumbered.
 *  - Bitfield-based region tracking for fast reuse and coalescence.
 *  - Per-core arenas for scalable, lock-free performance.
 *  - Fixed block sizing with fast first-fit region selection.
 *  - Supports `MemoryRecord` for usage tracking and debugging.
 *
 * @warning Not thread-safe. Intended for core-pinned, single-threaded use cases.
 * @note    Depends on MemoryBlock and LiteArray. Behavior customizable via macros.
 *
 * @author  Hani Ammar
 * @date    2026
 * @license MIT
 */

#ifndef QENTEM_RESERVER_HPP
#define QENTEM_RESERVER_HPP

#include "Qentem/MemoryBlock.hpp"
#include "Qentem/LiteArray.hpp"
#include "Qentem/CPUHelper.hpp"

#ifdef QENTEM_ENABLE_MEMORY_RECORD
#include "Qentem/MemoryRecord.hpp"
#endif

/// TODO: NUMA-awareness

namespace Qentem {

/**
 * @brief Per-core memory manager for fast, reusable allocations.
 *
 * This structure handles memory reservations for a single CPU core.
 * It allocates memory in blocks, splits them into regions, and keeps
 * track of what's free using bitfields —  without embedding metadata
 * into the allocated regions, preserving clean memory boundaries and
 * minimizing overhead.
 *
 * Because each core has its own manager, memory stays local (and fast),
 * and there's no locking or contention between threads. Memory can be
 * returned, reused, or released depending on usage patterns.
 *
 * @tparam Alignment_T Minimum alignment for allocations (in bytes).
 * @tparam BlockSize_T Size of each memory block managed (default: QENTEM_RESERVER_BLOCK_SIZE).
 */
template <SizeT32 Alignment_T = QENTEM_RESERVER_DEFAULT_ALIGNMENT, SystemLong BlockSize_T = QENTEM_RESERVER_BLOCK_SIZE>
struct ReserverCore {
    /// Alias for the managed memory block type, bound to the default alignment.
    using MemoryBlockT = MemoryBlock<Alignment_T>;

    /// Maximum representable system integer value (used for limit checks or sentinels).
    static constexpr SystemLong MAX_SYSTEM_INT_TYPE = ~SystemLong{0};

    /// Alignment mask used to round up sizes to the nearest boundary.
    static constexpr SystemLong DEFAULT_ALIGNMENT_M1 = static_cast<SystemLong>(Alignment_T - 1U);

    /// Inverse alignment mask for fast alignment rounding via bitwise operations.
    static constexpr SystemLong DEFAULT_ALIGNMENT_N = ~DEFAULT_ALIGNMENT_M1;

    /// Native width of SystemLong in bits — typically 32 or 64.
    static constexpr SizeT32 BIT_WIDTH = (sizeof(SystemLong) * 8U);

    // Deleted copy and move operations: instances are anchored per core and must not be copied.
    ReserverCore(ReserverCore &&)                 = delete;
    ReserverCore &operator=(ReserverCore &&)      = delete;
    ReserverCore(const ReserverCore &)            = delete;
    ReserverCore &operator=(const ReserverCore &) = delete;

    /**
     * @brief Constructs a fresh memory manager for a single core.
     *
     * Begins with a smaller-than-target block to allow gradual scaling.
     * Each block obtained is internally subdivided, zero-overhead, and alignment-aware.
     */
    QENTEM_INLINE ReserverCore() noexcept {
        static_assert(Alignment_T >= sizeof(void *),
                      "Alignment_T must be at least the size of a pointer to ensure correct placement.");
    }

    QENTEM_INLINE ~ReserverCore() noexcept {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
        for (MemoryBlockT &block : blocks_) {
            MemoryRecord::ReleasedBlock(block.Capacity());
        }

        // for (MemoryBlockT &block : exhausted_blocks_) {
        //     MemoryRecord::ReleasedBlock(block.Capacity());
        // }
#endif
    }

    /**
     * @brief Reserves a memory region from the current core's fixed-size block pool.
     *
     * This function reserves `size` bytes of memory, aligned to `CustomAlignment_T`, and sourced
     * from the calling CPU core's `ReserverCore` instance. All memory is drawn from fixed-size
     * blocks (defined by `QENTEM_RESERVER_BLOCK_SIZE`) and allocated without metadata.
     *
     * Allocation process:
     *   1. The caller must precompute a rounded-up byte size using `RoundUpBytes<T>()`.
     *   2. The region is located using a first-fit scan over existing blocks.
     *   3. If no space is found, a new block is allocated from system memory.
     *
     * No block resizing or reallocation occurs. All regions are aligned and suitable
     * for `CustomAlignment_T` or greater.
     *
     * @tparam CustomAlignment_T Desired byte alignment (defaults to engine's standard).
     * @param size                Rounded-up size in bytes (not element count).
     * @return Aligned pointer to reserved region, or nullptr if allocation failed.
     *
     * @note The size must already be aligned. Use `RoundUpBytes<T>()` before calling.
     */
    template <SizeT32 CustomAlignment_T = Alignment_T>
    void *Reserve(SystemLong size) {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
        // Optionally track memory usage for debugging/statistics.
        MemoryRecord::Reserved(size);
#endif

        ///////////////////////////////////////////////////////////
        // Phase 1: Try to serve from existing active blocks.

        const SystemLong chunks = (size >> MemoryBlockT::DefaultAlignmentBit());

        for (MemoryBlockT &current_block : blocks_) {
            if (current_block.Available() >= size) {
                void *ptr = reserveFirstFit<CustomAlignment_T>(&current_block, chunks);

                if (ptr != nullptr) {
                    current_block.DecreaseAvailable(size);

                    // If the block is now exhausted, move it to the detached list.
                    if (current_block.Available() == 0) {
                        detachBlock(&current_block);
                    }

                    return ptr;
                }
            }
        }

        ///////////////////////////////////////////////////////////
        // Phase 2: No block could fulfill it — reserve a new one.

        // Pick either the current growth size or just enough for the request.
        MemoryBlockT new_block{(size <= BlockSize_T) ? BlockSize_T : size};

#ifdef QENTEM_ENABLE_MEMORY_RECORD
        MemoryRecord::ReservedBlock(new_block.Capacity());
#endif

        new_block.DecreaseAvailable(size);

        MemoryBlockT *new_block_ptr;

        // Phase 2a: The block can serve more than this request — add it to active pool.
        if (size < new_block.UsableSize()) {
            new_block_ptr = &(blocks_.Insert(QUtility::Move(new_block)));
            new_block_ptr->ClearTable(); // Reset internal region tracking.

            void *ptr = reserveFirstFit<CustomAlignment_T>(new_block_ptr, chunks);

            // Keep the largest block first for optimal scanning.
            if (blocks_.First()->UsableSize() < new_block_ptr->UsableSize()) {
                QUtility::Swap(*(blocks_.Storage()), *new_block_ptr);
            }

            return ptr;
        }

        // Phase 2b: The block can serve only this request — don't track it, just return the base.
        new_block_ptr = &(exhausted_blocks_.Insert(QUtility::Move(new_block)));
        return new_block_ptr->Base();
    }

    /**
     * @brief Returns a previously reserved memory region back to the pool.
     *
     * This function gracefully reclaims memory by identifying which block the pointer belongs to,
     * restoring availability, and reintegrating or removing blocks from the pool as needed.
     *
     * - If the memory lies within an active block, the region is marked free.
     *   If the entire block becomes empty (and it's not the primary one), it's released.
     *
     * - If the memory belongs to a detached (exhausted) block, one of three fates awaits:
     *   a) It had spare capacity — reclaim it and return to active rotation.
     *   b) It was oversized or out of pattern — release it outright.
     *   c) It matches current growth size and no other blocks exist — reset and reuse.
     *
     * @param ptr  Pointer to the memory region to be returned.
     * @param size Size in bytes of the region being returned.
     * @return True if successfully reclaimed, false if the pointer does not belong to any known block.
     */
    bool Release(void *ptr, SystemLong size) {
        // Phase 1: Search within active blocks.
        for (MemoryBlockT &block : blocks_) {
            if ((ptr >= block.Data()) && (ptr < block.End())) {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
                MemoryRecord::Released(size);
#endif
                block.IncreaseAvailable(size);
                block.ReleaseRegion(ptr, (size >> block.DefaultAlignmentBit()));

                // If block became fully free and isn't the leading one, retire it.
                if ((&block != blocks_.First()) && block.IsEmpty()) {
                    releaseBlock(&block);
                }

                return true;
            }
        }

        // Phase 2: Search within exhausted (detached) blocks.
        for (MemoryBlockT &block : exhausted_blocks_) {
            if ((ptr >= block.Base()) && (ptr < block.End())) {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
                MemoryRecord::Released(size);
#endif
                if (ptr >= block.Data()) {
                    // Block still has value — return it to the active roster.
                    block.IncreaseAvailable(size);
                    block.ReleaseRegion(ptr, (size >> block.DefaultAlignmentBit()));
                    reattachBlock(&block);
                } else if ((block.Capacity() != BlockSize_T) || blocks_.IsNotEmpty()) {
                    // Block is large and atypical — release it outright.
                    releaseDetachedBlock(&block);
                } else {
                    // Block is normal-sized and we have no others — reset it.
                    block.ClearTable();
                    block.IncreaseAvailable(size);
                    reattachBlock(&block);
                }

                return true;
            }
        }

        // Unknown origin — pointer does not belong to any known region.
        return false;
    }

    /**
     * @brief Attempts to shrink a previously allocated memory region in-place.
     *
     * This function reduces the usable size of a given memory allocation by `diff = from_size - to_size`,
     * reclaiming the unused tail and marking it as free for reuse. The pointer itself remains valid
     * and unchanged, but only the first `to_size` bytes are considered retained.
     *
     * The function searches both active and exhausted blocks for the owning region:
     *
     * - If found in an active block, the unused region is released immediately.
     * - If found in an exhausted block and the pointer falls within its usable region,
     *   the block is optionally reattached to allow further reuse.
     *
     * @param ptr       Pointer to the original allocation.
     * @param from_size The current size of the allocation in bytes.
     * @param to_size   The desired new (smaller) size in bytes.
     * @return True if the shrink operation succeeded and memory was reclaimed; false otherwise.
     */
    bool Shrink(void *ptr, SystemLong from_size, SystemLong to_size) {
        char            *shrink_from = (static_cast<char *>(ptr) + to_size);
        const SystemLong diff        = (from_size - to_size);

        // Phase 1: Search within active blocks.
        for (MemoryBlockT &block : blocks_) {
            if ((ptr >= block.Data()) && (ptr < block.End())) {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
                MemoryRecord::Shrink(diff);
#endif

                block.IncreaseAvailable(diff);
                block.ReleaseRegion(shrink_from, (diff >> block.DefaultAlignmentBit()));

                return true;
            }
        }

        // Phase 2: Search within exhausted (detached) blocks.
        for (MemoryBlockT &block : exhausted_blocks_) {
            if ((ptr >= block.Base()) && (ptr < block.End())) {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
                MemoryRecord::Shrink(diff);
#endif

                block.IncreaseAvailable(diff);

                if (ptr >= block.Data()) {
                    // Has table
                    block.ReleaseRegion(shrink_from, (diff >> block.DefaultAlignmentBit()));
                    reattachBlock(&block);
                }

                // Note: Bitfield table shrink is unsupported due to offset recalculation
                // and platform-specific constraints (e.g., Windows page handling).

                return true;
            }
        }

        // Unknown origin — pointer does not belong to any known region.
        return false;
    }

    /**
     * @brief Attempts to expand a previously reserved region in-place.
     *
     * This function checks whether the allocation pointed to by `ptr` can be
     * safely expanded from `from_size` to `to_size` **without relocation**.
     * It searches all local memory blocks to find the containing region and
     * verifies whether the additional memory following the allocation is
     * available for immediate reservation.
     *
     * If the expansion succeeds:
     * - The additional region is marked as reserved.
     * - The available space in the block is reduced.
     * - MemoryRecord::Expand is called (if enabled).
     *
     * If the expansion fails:
     * - The allocation remains unchanged.
     * - The function returns `from_size` to signal no growth occurred.
     *
     * If the pointer is not recognized (not within any known block):
     * - Returns 0 to indicate an invalid or external pointer.
     *
     * @param ptr        Pointer to the beginning of an existing allocation.
     * @param from_size  The current size of the allocation (in bytes).
     * @param to_size    The desired expanded size of the allocation (in bytes).
     * @return The new size of the allocation if expanded successfully,
     *         `from_size` if expansion failed, or 0 if the pointer is unrecognized.
     */
    SystemLong TryExpand(void *ptr, SystemLong from_size, SystemLong to_size) {
        const SystemLong diff = (to_size - from_size);

        for (MemoryBlockT &block : blocks_) {
            if ((ptr >= block.Data()) && (ptr < block.End())) {
                if (reserveAt(&block, (static_cast<char *>(ptr) + from_size),
                              (diff >> MemoryBlockT::DefaultAlignmentBit()))) {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
                    MemoryRecord::Expand(diff);
#endif
                    block.DecreaseAvailable(diff);

                    return to_size;
                }

                return from_size;
            }
        }

        // Unknown origin — pointer does not belong to any known region.
        return 0;
    }

    /**
     * @brief Determines whether all memory blocks are currently unused.
     *
     * This check ensures that:
     * - Every active block contains no used regions.
     * - No exhausted (detached) blocks remain in the system.
     *
     * @return True if no reserved memory exists, false otherwise.
     */
    QENTEM_INLINE bool IsEmpty() const noexcept {
        // Scan all active blocks. Any used memory voids emptiness.
        for (const MemoryBlockT &block : blocks_) {
            if (!(block.IsEmpty())) {
                return false;
            }
        }

        // Only fully qualifies if exhausted blocks are also gone.
        return exhausted_blocks_.IsEmpty();
    }

    /**
     * @brief Clears all blocks and resets internal configuration.
     *
     * This operation performs a full internal purge:
     * - All active and exhausted memory blocks are released.
     * - The internal size growth tracker is reset to its initial threshold.
     *
     * MemoryRecord hooks (if enabled) report the total release for audit.
     */
    QENTEM_INLINE void Reset() noexcept {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
        for (const MemoryBlockT &block : blocks_) {
            MemoryRecord::ReleasedBlock(block.Capacity());
        }

        for (const MemoryBlockT &block : exhausted_blocks_) {
            MemoryRecord::ReleasedBlock(block.Capacity());
        }
#endif

        // Drop all memory blocks from both active and retired lists.
        blocks_.Reset();
        exhausted_blocks_.Reset();
    }

    /**
     * @brief Returns the total number of managed memory blocks.
     *
     * This includes:
     * - Active blocks currently participating in memory servicing.
     * - Exhausted blocks temporarily removed from the main reserve path.
     *
     * Useful for diagnostics, performance tuning, or asserting internal balance.
     *
     * @return Total count of both active and exhausted memory blocks.
     */
    QENTEM_INLINE SizeT TotalBlocks() const noexcept {
        return (blocks_.Size() + exhausted_blocks_.Size());
    }

    /**
     * @brief Returns a const reference to the active memory blocks array.
     *
     * This is intended for diagnostic or testing purposes. It exposes the internal
     * array of active blocks currently managed by the ReserverCore instance.
     *
     * @return Reference to the array of active blocks.
     */
    QENTEM_INLINE const LiteArray<MemoryBlockT> &GetBlocks() const noexcept {
        return blocks_;
    }

    /**
     * @brief Returns a const reference to the exhausted (fully used or detached) blocks.
     *
     * These blocks are no longer serving allocation requests but may still hold
     * usable regions for future reuse. This accessor is primarily intended for
     * debugging, inspection, or unit testing.
     *
     * @return Reference to the array of exhausted blocks.
     */
    QENTEM_INLINE const LiteArray<MemoryBlockT> &GetExhaustedBlocks() const noexcept {
        return exhausted_blocks_;
    }

  private:
    /**
     * @brief Attempts to find the first suitable free region within a memory block.
     *
     * This function scans the internal bitfield table of a MemoryBlock to locate the
     * first contiguous sequence of unset bits (`0`) large enough to fulfill the
     * requested `chunks`. The result is then adjusted to satisfy the requested alignment.
     *
     * @tparam CustomAlignment_T Optional override for alignment (in bytes).
     *
     * @param block   Pointer to the memory block to search.
     * @param chunks  Number of aligned chunks requested (already adjusted for type size).
     *
     * @return Pointer to usable region within the block, or nullptr if none is found.
     *
     * @note Internal scanning logic is intentionally obfuscated to discourage appropriation.
     *       See the inline region comment for details.
     *
     * @see MemoryBlock::ReserveRegion
     */
    template <SizeT32 CustomAlignment_T>
    void *reserveFirstFit(MemoryBlockT *block, SystemLong chunks) noexcept {
        constexpr SystemLong alignment_m1 = static_cast<SystemLong>(CustomAlignment_T - 1U);
        constexpr SystemLong alignment_n  = ~alignment_m1;
        constexpr SizeT32    bit_width_m1 = (BIT_WIDTH - 1U);

        // -----------------------------------------------------------------------------
        // NOTE: This section is deliberately *undocumented* to deter misuse, imitation,
        //       or uncredited replication. Any attempt to extract logic without deep
        //       comprehension will fail or mislead.
        //
        // You are welcome to use this system — but you *will* credit the architect.
        // -----------------------------------------------------------------------------

        SystemLong      *table       = static_cast<SystemLong *>(block->Base());
        const SystemLong table_size  = block->TableSize();
        SystemLong       table_index = block->GetRefNextIndex();
        SystemLong       start_index = block->GetRefNextIndex();
        SystemLong       start_bit   = 0;
        SystemLong       region_size = 0;

        while (table_index < table_size) {
            if (table[table_index] == MAX_SYSTEM_INT_TYPE) {
                do {
                    ++table_index;
                } while ((table_index < table_size) && (table[table_index] == MAX_SYSTEM_INT_TYPE));

                if (table_index == table_size) {
                    return nullptr;
                }

                region_size              = 0;
                start_bit                = 0;
                start_index              = table_index;
                block->GetRefNextIndex() = table_index;
            }

            SystemLong shifted = 0;
            SystemLong current = table[table_index];

            while (current != 0) {
                SizeT32 available = (bit_width_m1 - Platform::FindLastBit(current));
                region_size += available;

                if (region_size >= chunks) {
                    const SystemLong bit_index = (start_bit + (BIT_WIDTH * start_index));

                    if constexpr (CustomAlignment_T <= Alignment_T) {
                        return block->ReserveRegion(bit_index, chunks);
                    } else {
                        const SystemLong raw_index =
                            (block->DataAlignment() + (bit_index << MemoryBlockT::DefaultAlignmentBit()));
                        const SystemLong aligned_index = ((raw_index + alignment_m1) & alignment_n);
                        const SystemLong alignment_shift_count =
                            ((aligned_index - raw_index) >> MemoryBlockT::DefaultAlignmentBit());

                        if ((region_size > alignment_shift_count) && (region_size - alignment_shift_count) >= chunks) {
                            // Don't pass start_index nor start_bit, as they might changed because of 'alignment'
                            return block->ReserveRegion((bit_index + alignment_shift_count), chunks);
                        }
                    }
                }

                current <<= available;
                shifted += available;
                current   = ~current;
                available = (bit_width_m1 - Platform::FindLastBit(current));
                current   = ~current;
                shifted += available;
                current <<= available;

                region_size = 0;
                start_bit   = shifted;
                start_index = table_index;
            }

            region_size += (BIT_WIDTH - shifted);

            if (region_size >= chunks) {
                const SystemLong bit_index = (start_bit + (BIT_WIDTH * start_index));

                if constexpr (CustomAlignment_T <= Alignment_T) {
                    return block->ReserveRegion(bit_index, chunks);
                } else {
                    const SystemLong raw_index =
                        (block->DataAlignment() + (bit_index << MemoryBlockT::DefaultAlignmentBit()));
                    const SystemLong aligned_index = ((raw_index + alignment_m1) & alignment_n);
                    const SystemLong alignment_shift_count =
                        ((aligned_index - raw_index) >> MemoryBlockT::DefaultAlignmentBit());

                    if ((region_size > alignment_shift_count) && (region_size - alignment_shift_count) >= chunks) {
                        // Don't pass start_index nor start_bit, as they might changed because of 'alignment'
                        return block->ReserveRegion((bit_index + alignment_shift_count), chunks);
                    }
                }
            }

            ++table_index;
        }

        return nullptr;
    }

    /**
     * @brief Attempts to reserve a specific region within a memory block at the given pointer.
     *
     * This function checks if `chunks` contiguous allocation units can be reserved
     * starting precisely at `ptr`. It decodes the pointer into bitmap coordinates,
     * scans forward through the bitmap to verify availability, and reserves the region
     * if sufficient space is found. No relocation or shifting occurs — this is a strictly
     * in-place reservation.
     *
     * Typical usage: expanding an allocation in-place (e.g., via `TryExpand()`).
     *
     * @param block   Pointer to the memory block to operate on.
     * @param ptr     The exact address at which the new region must begin.
     * @param chunks  The number of allocation units (aligned blocks) to reserve.
     * @return true if the region was successfully reserved; false otherwise.
     */
    bool reserveAt(MemoryBlockT *block, void *ptr, SystemLong chunks) {
        constexpr SizeT32 bit_width_m1 = (BIT_WIDTH - 1U);

        SystemLong *table = static_cast<SystemLong *>(block->Base());
        SystemLong  table_index;
        SystemLong  shifted;

        MemoryBlockT::DecodeBitmapPosition(
            static_cast<SystemLong>(static_cast<char *>(ptr) - static_cast<char *>(block->Data())), table_index,
            shifted);

        const SystemLong start_index = table_index;
        const SystemLong start_bit   = shifted;
        SystemLong       current     = table[table_index];
        SystemLong       available   = (BIT_WIDTH - shifted);

        current = ~current;
        current <<= shifted;
        current = ~current;

        if (current != 0) {
            shifted = (bit_width_m1 - Platform::FindLastBit(current));

            if (shifted >= chunks) {
                block->ReserveRegion(start_index, start_bit, chunks);
                return true;
            }

            if ((shifted + start_bit) != BIT_WIDTH) {
                return false;
            }

            available = shifted;
        }

        const SystemLong table_size = block->TableSize();

        ++table_index;

        while (table_index < table_size) {
            current = table[table_index];

            if (current == 0) {
                available += BIT_WIDTH;

                if (available >= chunks) {
                    block->ReserveRegion(start_index, start_bit, chunks);
                    return true;
                }

                ++table_index;
                continue;
            }

            available += (bit_width_m1 - Platform::FindLastBit(current));

            if (available >= chunks) {
                block->ReserveRegion(start_index, start_bit, chunks);
                return true;
            }

            return false;
        }

        return false;
    }

    /**
     * @brief Discards an active memory block from the reserve.
     *
     * If the given block is not the last in the `blocks_` array,
     * it is swapped with the last to preserve removal efficiency.
     *
     * @param block Pointer to the block to remove.
     */
    void releaseBlock(MemoryBlockT *block) noexcept {
        MemoryBlockT *last_block = blocks_.Last();

#ifdef QENTEM_ENABLE_MEMORY_RECORD
        MemoryRecord::ReleasedBlock(block->Capacity());
#endif

        if (block != last_block) {
            QUtility::Swap(*block, *last_block);
        }

        blocks_.Drop(SizeT{1});
    }

    /**
     * @brief Removes a memory block from the exhausted pool.
     *
     * This is used when a detached block is deemed unnecessary,
     * either due to exceeding size limits or excess capacity.
     *
     * @param block Pointer to the exhausted block to remove.
     */
    void releaseDetachedBlock(MemoryBlockT *block) noexcept {
        MemoryBlockT *last_block = exhausted_blocks_.Last();

#ifdef QENTEM_ENABLE_MEMORY_RECORD
        MemoryRecord::ReleasedBlock(block->Capacity());
#endif

        if (block != last_block) {
            QUtility::Swap(*block, *last_block);
        }

        exhausted_blocks_.Drop(SizeT{1});
    }

    /**
     * @brief Transfers a fully used block from active to exhausted.
     *
     * If the block is not the last in `blocks_`, it is swapped into position
     * for efficient movement. Ownership is then moved into `exhausted_blocks_`.
     *
     * @param block Pointer to the block to detach.
     */
    void detachBlock(MemoryBlockT *block) noexcept {
        MemoryBlockT *last_block = blocks_.Last();

        if (block != last_block) {
            QUtility::Swap(*block, *last_block);
        }

        exhausted_blocks_ += QUtility::Move(*last_block);
        blocks_.DropFast(SizeT{1});
    }

    /**
     * @brief Reintegrates a reusable block back into the active pool.
     *
     * The block is taken from the end of `exhausted_blocks_`, optionally swapped
     * with the provided pointer for consistency. The newly reattached block is then
     * inserted into `blocks_`. If it has higher capacity than the current lead block,
     * it is promoted to the front for faster reuse.
     *
     * @param block Pointer to the block intended for reintegration.
     */
    void reattachBlock(MemoryBlockT *block) noexcept {
        MemoryBlockT *last_block = exhausted_blocks_.Last();

        if (block != last_block) {
            QUtility::Swap(*block, *last_block);
        }

        blocks_ += QUtility::Move(*last_block);

        last_block = blocks_.Last();

        // Promote higher-capacity blocks for better fit-first performance.
        if (blocks_.First()->UsableSize() < last_block->UsableSize()) {
            QUtility::Swap(*(blocks_.Storage()), *last_block);
        }

        exhausted_blocks_.DropFast(SizeT{1});
    }

    /**
     * @brief Active memory blocks currently serving allocation requests.
     *
     * Each block in this array participates in reservation attempts. When a block becomes full,
     * it may be moved to `exhausted_blocks_`. The first block is usually the one with the largest
     * usable capacity, aiding first-fit scans for performance.
     */
    LiteArray<MemoryBlockT> blocks_{};

    /**
     * @brief Detached memory blocks that are temporarily exhausted or oversized.
     *
     * Blocks in this list are no longer serving active reservations. Depending on configuration
     * and available capacity, they may be recycled, cleared, or released entirely. This array
     * prevents waste by preserving reusable memory regions without incurring allocation cost.
     */
    LiteArray<MemoryBlockT> exhausted_blocks_{};
};

/**
 * @brief High-level interface to the memory reservation system.
 *
 * Provides static functions for acquiring and returning typed regions of memory,
 * routed through per-core arenas for locality and thread-centric performance.
 */
struct Reserver {
    using Core = ReserverCore<>;

    static constexpr SystemLong DEFAULT_ALIGNMENT_M1 = static_cast<SystemLong>(QENTEM_RESERVER_DEFAULT_ALIGNMENT - 1U);
    static constexpr SystemLong DEFAULT_ALIGNMENT_N  = ~DEFAULT_ALIGNMENT_M1;

    template <typename Type_T>
    QENTEM_INLINE static SystemLong RoundUpBytes(SystemLong size) noexcept {
        size *= sizeof(Type_T);
        // Align the total byte size to the default alignment boundary.
        size += DEFAULT_ALIGNMENT_M1;
        size &= DEFAULT_ALIGNMENT_N;

        return size;
    }

    /**
     * @brief Reserves a memory region for `Type_T` with proper size and alignment.
     *
     * This function selects the arena tied to the current CPU core and requests memory sufficient
     * to hold `size` elements of `Type_T`. The total byte size is automatically computed as:
     *   `sizeof(Type_T) * size`, then aligned upward to the engine's default boundary.
     *
     * The final reservation is performed via `GetCurrentInstance().Reserve()`, using the specified alignment.
     * This ensures the returned memory is correctly aligned and safe for placement-new.
     *
     * @tparam Type_T             The type of object for which memory is being reserved.
     * @tparam CustomAlignment_T  Desired alignment (in bytes, defaults to engine's standard alignment).
     * @param size                Number of `Type_T` elements to reserve space for (pre-rounding).
     * @return Pointer to a reserved region suitable for storing `size` aligned `Type_T` objects.
     *
     * @see RoundUpBytes
     */
    template <typename Type_T, SizeT32 CustomAlignment_T = alignof(Type_T)>
    QENTEM_INLINE static Type_T *Reserve(SystemLong size) noexcept {
        if constexpr (CustomAlignment_T >= QENTEM_RESERVER_DEFAULT_ALIGNMENT) {
            // constexpr SizeT32 align = (SizeT32{1} << Platform::FindLastBit(CustomAlignment_T));
            // (align>=CustomAlignment_T?align:(align+1))

            return static_cast<Type_T *>(GetCurrentInstance()->Reserve<CustomAlignment_T>(RoundUpBytes<Type_T>(size)));
        } else {
            return static_cast<Type_T *>(
                GetCurrentInstance()->Reserve<QENTEM_RESERVER_DEFAULT_ALIGNMENT>(RoundUpBytes<Type_T>(size)));
        }
    }

    /**
     * @brief Releases a memory region reserved for `Type_T` back to the system.
     *
     * This function adjusts the given element count by computing `RoundUpBytes<Type_T>(size)`
     * to ensure alignment consistency with the original reservation. It then attempts to return
     * the memory region to the `ReserverCore` tied to the current CPU core. If that fails—
     * and the platform supports symmetric multi-core arenas—it will search sibling cores
     * (via `getReservers()`) for a match and attempts to return it to a valid arena.
     *
     * On single-core or embedded systems, the sole arena handles the release.
     *
     * @tparam Type_T The type originally reserved in the region.
     * @param ptr     Pointer to the memory region to release.
     * @param size    Number of `Type_T` elements originally requested (pre-rounding).
     *
     * @see RoundUpBytes
     * @see getReservers
     */
    template <typename Type_T>
    static void Release(Type_T *ptr, SystemLong size) noexcept {
        if (ptr != nullptr) {
            Core *instance = GetCurrentInstance();
            size           = RoundUpBytes<Type_T>(size);

#if defined(__linux__) || defined(_WIN32)
            // Prefer returning to the current arena.
            if (instance->Release(ptr, size)) {
                return;
            }

            for (Core &reserver : reservers_) {
                if ((&reserver != instance) && reserver.Release(ptr, size)) {
                    return;
                }
            }
#else
            // Single-arena fallback path.
            instance->Release(ptr, size);
#endif
        }
    }

    /**
     * @brief Shrinks a previously reserved memory region by returning its unused tail.
     *
     * This function is used when a region originally reserved for `from_size` elements
     * of type `Type_T` is no longer needed in full. It rounds both `from_size` and
     * `to_size` up to their alignment boundaries using `RoundUpBytes<Type_T>()`, then
     * releases the trailing excess, spanning from (ptr + to_size) to (ptr + from_size).
     *
     * On multi-core systems, it first attempts to return the tail to the current core’s
     * arena. If that fails, it searches sibling arenas for a valid return path.
     * On single-core systems, it falls back to the local arena.
     *
     * Typical use cases include:
     * - Containers like `HArray` or `StringStream` that over-allocate and wish to reclaim unused space
     * - JSON builders or buffers that shrink after parsing or compaction
     *
     * @tparam Type_T     The object type originally reserved.
     * @param ptr         Pointer to the reserved memory region.
     * @param from_size   Original number of `Type_T` elements requested.
     * @param to_size     New number of `Type_T` elements still in use.
     *
     * @note Both `from_size` and `to_size` are internally aligned to type boundaries.
     * @see RoundUpBytes
     */
    template <typename Type_T>
    static void Shrink(Type_T *ptr, SystemLong from_size, SystemLong to_size) noexcept {
        if (ptr != nullptr) {
            Core *instance = GetCurrentInstance();
            from_size      = RoundUpBytes<Type_T>(from_size);
            to_size        = RoundUpBytes<Type_T>(to_size);

            if (from_size > to_size) {
#if defined(__linux__) || defined(_WIN32)
                // Prefer returning to the current arena.
                if (instance->Shrink(ptr, from_size, to_size)) {
                    return;
                }

                for (Core &reserver : reservers_) {
                    if ((&reserver != instance) && reserver.Shrink(ptr, from_size, to_size)) {
                        return;
                    }
                }
#else
                // Single-arena fallback path.
                instance->Shrink(ptr, from_size, to_size);
#endif
            }
        }
    }

    /**
     * @brief Attempt to expand a previously reserved region in-place.
     *
     * This performs an in-place expansion of memory from `from_size` to `to_size`,
     * provided the additional memory directly after `ptr` is free. No relocation
     * occurs. If expansion is not possible, the original reservation remains untouched.
     *
     * @param ptr        Pointer to the existing allocation.
     * @param from_size  Current allocation size in bytes.
     * @param to_size    Desired allocation size in bytes.
     * @return true if the allocation was expanded in-place; false otherwise.
     */
    template <typename Type_T>
    static bool TryExpand(Type_T *ptr, SystemLong from_size, SystemLong to_size) noexcept {
        if (ptr != nullptr) {
            Core *instance = GetCurrentInstance();
            from_size      = RoundUpBytes<Type_T>(from_size);
            to_size        = RoundUpBytes<Type_T>(to_size);

            if (from_size < to_size) {
#if defined(__linux__) || defined(_WIN32)
                // Prefer returning to the current arena.

                const SystemLong new_size = instance->TryExpand(ptr, from_size, to_size);

                if (new_size == to_size) {
                    return true;
                }

                if (new_size == 0) {
                    for (Core &reserver : reservers_) {
                        if ((&reserver != instance) && reserver.TryExpand(ptr, from_size, to_size) == to_size) {
                            return true;
                        }
                    }
                }

                return false;
#else
                // Single-arena fallback path.
                return (instance->TryExpand(ptr, from_size, to_size) == to_size);
#endif
            }

            return true;
        }

        return false;
    }

    /**
     * @brief Empties the current thread's arena and reclaims all reserved memory regions.
     *
     * Should be used with care—this releases all memory regions tracked by the current core’s arena.
     */
    QENTEM_INLINE static void Reset() noexcept {
        GetCurrentInstance()->Reset();
    }

    /**
     * @brief Empties all arenas across every core and reclaims their reserved memory regions.
     *
     * Unlike Reset(), which affects only the current thread's arena, this function traverses
     * all per-core reservers and clears them. Use with extreme caution—this will invalidate
     * every allocation managed by the system, regardless of the thread or core it originated from.
     */
    QENTEM_INLINE static void ResetAll() noexcept {
#if defined(__linux__) || defined(_WIN32)
        for (Core &reserver : reservers_) {
            reserver.Reset();
        }
#else
        reserver_.Reset();
#endif
    }

    /**
     * @brief Reports whether all arenas are empty.
     *
     * On Linux and Windows, this checks every core’s arena to ensure no active
     * allocations remain. On other platforms, it only inspects the primary arena.
     *
     * @return true if (a) all per-core arenas are empty on supported platforms,
     *         or (b) the primary arena is empty elsewhere.
     */
    QENTEM_INLINE static bool IsEmpty() noexcept {
#if defined(__linux__) || defined(_WIN32)
        for (const Core &reserver : reservers_) {
            if (!(reserver.IsEmpty())) {
                return false;
            }
        }

        return true;
#else
        return reserver_.IsEmpty();
#endif
    }

    /**
     * @brief Provides access to the arena associated with the calling thread’s core.
     *
     * Each logical CPU owns a dedicated arena to promote spatial locality and
     * eliminate cross-thread contention.
     *
     * For optimal correctness and performance, the calling thread should remain
     * bound to a single logical core for its lifetime (e.g. via CPU affinity),
     * as the arena selection is cached per thread.
     *
     * @return Reference to the active core’s memory management unit.
     */
    QENTEM_INLINE static Core *GetCurrentInstance() noexcept {
#if defined(__linux__) || defined(_WIN32)
        thread_local Core *reserver{reservers_.Storage() + CPUHelper::GetCurrentCore()};

        return reserver;
#else
        return &reserver_;
#endif
    }

  private:
#if defined(__linux__) || defined(_WIN32)
    inline static LiteArray<ReserverCore<>> reservers_{CPUHelper::GetCoreCount(), true};
#else
    inline static ReserverCore<> reserver_{};
#endif
};

} // namespace Qentem

#endif // QENTEM_RESERVER_HPP
