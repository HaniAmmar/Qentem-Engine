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
 * @date    2025
 * @license MIT
 */

#ifndef QENTEM_RESERVER_HPP
#define QENTEM_RESERVER_HPP

#include "LiteArray.hpp"
#include "CPUHelper.hpp"
#include "MemoryBlock.hpp"

#ifdef QENTEM_ENABLE_MEMORY_RECORD
#include "MemoryRecord.hpp"
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
template <SizeT32       Alignment_T = QENTEM_RESERVER_DEFAULT_ALIGNMENT,
          SystemIntType BlockSize_T = QENTEM_RESERVER_BLOCK_SIZE>
struct ReserverCore {
    /// Alias for the managed memory block type, bound to the default alignment.
    using MemoryBlockT = MemoryBlock<QENTEM_RESERVER_DEFAULT_ALIGNMENT>;

    /// Maximum representable system integer value (used for limit checks or sentinels).
    static constexpr SystemIntType MAX_SYSTEM_INT_TYPE = ~SystemIntType{0};

    /// Alignment mask used to round up sizes to the nearest boundary.
    static constexpr SystemIntType DEFAULT_ALIGNMENT_M1 = static_cast<SystemIntType>(Alignment_T - 1U);

    /// Inverse alignment mask for fast alignment rounding via bitwise operations.
    static constexpr SystemIntType DEFAULT_ALIGNMENT_N = ~DEFAULT_ALIGNMENT_M1;

    /// Native width of SystemIntType in bits — typically 32 or 64.
    static constexpr SizeT32 BIT_WIDTH = (sizeof(SystemIntType) * 8U);

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
    ReserverCore() noexcept {
        static_assert(Alignment_T >= sizeof(void *),
                      "Alignment_T must be at least the size of a pointer to ensure correct placement.");
    }

    ~ReserverCore() noexcept {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
        for (auto &block : blocks_) {
            MemoryRecord::ReleasedBlock(block.Capacity());
        }

        // for (auto &block : exhausted_blocks_) {
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
     *   1. The caller must precompute a rounded-up byte size using `RoundUpSize<T>()`.
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
     * @note The size must already be aligned. Use `RoundUpSize<T>()` before calling.
     */
    template <SizeT32 CustomAlignment_T = Alignment_T>
    QENTEM_INLINE void *Reserve(SystemIntType size) {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
        // Optionally track memory usage for debugging/statistics.
        MemoryRecord::Reserved(size);
#endif

        ///////////////////////////////////////////////////////////
        // Phase 1: Try to serve from existing active blocks.

        const SystemIntType chunks = (size >> MemoryBlockT::DefaultAlignmentBit());

        for (auto &current_block : blocks_) {
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

        MemoryBlockT *new_block_ptr;

        // Phase 2a: The block can serve more than this request — add it to active pool.
        if (size < new_block.UsableSize()) {
            new_block_ptr = &(blocks_.Insert(QUtility::Move(new_block)));
            new_block_ptr->ClearTable(); // Reset internal region tracking.

            void *ptr = reserveFirstFit<CustomAlignment_T>(new_block_ptr, chunks);
            new_block_ptr->DecreaseAvailable(size);

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
    bool Release(void *ptr, SystemIntType size) {
        // Phase 1: Search within active blocks.
        for (auto &block : blocks_) {
            if ((ptr >= block.Data()) && (ptr < block.End())) {
                block.IncreaseAvailable(size);
                block.ReleaseRegion(ptr, (size >> block.DefaultAlignmentBit()));

                // If block became fully free and isn't the leading one, retire it.
                if ((&block != blocks_.First()) && block.IsEmpty()) {
                    releaseBlock(&block);
                }

#ifdef QENTEM_ENABLE_MEMORY_RECORD
                MemoryRecord::Released(size);
#endif
                return true;
            }
        }

        // Phase 2: Search within exhausted (detached) blocks.
        for (auto &block : exhausted_blocks_) {
            if ((ptr >= block.Base()) && (ptr < block.End())) {
                if (size < block.UsableSize()) {
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
                    reattachBlock(&block);
                }

#ifdef QENTEM_ENABLE_MEMORY_RECORD
                MemoryRecord::Released(size);
#endif
                return true;
            }
        }

        // Unknown origin — pointer does not belong to any known region.
        return false;
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
    bool IsEmpty() const noexcept {
        // Scan all active blocks. Any used memory voids emptiness.
        for (const auto &block : blocks_) {
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
    void Reset() noexcept {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
        for (const auto &block : blocks_) {
            MemoryRecord::ReleasedBlock(block.Capacity());
        }

        for (const auto &block : exhausted_blocks_) {
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
    SizeT TotalBlocks() const noexcept {
        return (blocks_.Size() + exhausted_blocks_.Size());
    }

  private:
    /**
     * @brief Attempts to find the first suitable free region within a memory block.
     *
     * This function scans the internal bitfield table of a MemoryBlock to locate the
     * first contiguous sequence of unset bits (`0`) large enough to fulfill the
     * requested `chunks`. The result is then adjusted to satisfy the requested alignment.
     *
     * @tparam CustomAlignment_T Optional override for alignment in bytes.
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
    void *reserveFirstFit(MemoryBlockT *block, SystemIntType chunks) noexcept {
        constexpr SystemIntType alignment_m1 = static_cast<SystemIntType>(CustomAlignment_T - 1U);
        constexpr SystemIntType alignment_n  = ~alignment_m1;
        constexpr SizeT32       bit_width_m1 = (BIT_WIDTH - 1U);

        // -----------------------------------------------------------------------------
        // NOTE: This section is deliberately *undocumented* to deter misuse, imitation,
        //       or uncredited replication. Any attempt to extract logic without deep
        //       comprehension will fail or mislead.
        //
        // You are welcome to use this system — but you *will* credit the architect.
        // -----------------------------------------------------------------------------

        SystemIntType      *table       = static_cast<SystemIntType *>(block->Base());
        const SystemIntType table_size  = block->TableSize();
        SystemIntType       table_index = block->GetRefNextIndex();
        SystemIntType       start_index = block->GetRefNextIndex();
        SystemIntType       start_bit   = 0;
        SystemIntType       region_size = 0;

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

            SystemIntType shifted = 0;
            SystemIntType current = table[table_index];

            while (current != 0) {
                SizeT32 available = (bit_width_m1 - Platform::FindLastBit(current));
                region_size += available;

                if (region_size >= chunks) {
                    const SystemIntType bit_index = (start_bit + (BIT_WIDTH * start_index));

                    if QENTEM_CONST_EXPRESSION (CustomAlignment_T <= Alignment_T) {
                        return block->ReserveRegion(bit_index, chunks);
                    } else {
                        const SystemIntType raw_index =
                            (block->DataAlignment() + (bit_index << MemoryBlockT::DefaultAlignmentBit()));
                        const SystemIntType aligned_index = ((raw_index + alignment_m1) & alignment_n);
                        const SystemIntType alignment_shift_count =
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
                const SystemIntType bit_index = (start_bit + (BIT_WIDTH * start_index));

                if QENTEM_CONST_EXPRESSION (CustomAlignment_T <= Alignment_T) {
                    return block->ReserveRegion(bit_index, chunks);
                } else {
                    const SystemIntType raw_index =
                        (block->DataAlignment() + (bit_index << MemoryBlockT::DefaultAlignmentBit()));
                    const SystemIntType aligned_index = ((raw_index + alignment_m1) & alignment_n);
                    const SystemIntType alignment_shift_count =
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

    static constexpr SystemIntType DEFAULT_ALIGNMENT_M1 =
        static_cast<SystemIntType>(QENTEM_RESERVER_DEFAULT_ALIGNMENT - 1U);
    static constexpr SystemIntType DEFAULT_ALIGNMENT_N = ~DEFAULT_ALIGNMENT_M1;

    template <typename Type_T>
    QENTEM_INLINE static void RoundUpSize(SystemIntType &size) noexcept {
        size *= sizeof(Type_T);
        // Round the size up to the alignment boundary.
        size += DEFAULT_ALIGNMENT_M1;
        size &= DEFAULT_ALIGNMENT_N;
    }

    /**
     * @brief Reserves a raw memory region that has already been size-aligned.
     *
     * This function assumes the given `size` has already been rounded up using
     * `RoundUpSize<Type_T>()` and returns an untyped memory region with the requested alignment.
     * It is intended for internal use or advanced callers who manage size and alignment manually.
     *
     * @tparam CustomAlignment_T Desired alignment for the memory region (defaults to engine constant).
     * @param size Size in bytes, already aligned to `CustomAlignment_T`.
     * @return Pointer to aligned memory region, or nullptr if reservation failed.
     *
     * @see Reserve()
     * @see RoundUpSize()
     */
    template <SizeT32 CustomAlignment_T = QENTEM_RESERVER_DEFAULT_ALIGNMENT>
    QENTEM_INLINE static void *ReserveRoundedUpBytes(SystemIntType size) noexcept {
        return GetCurrentInstance().Reserve<CustomAlignment_T>(size);
    }

    /**
     * @brief Reserves a region for objects of the given type.
     *
     * This function selects the arena tied to the current CPU core and requests space
     * sufficient to contain the specified number of elements with the given alignment.
     * It automatically multiplies `size` by sizeof(Type_T) and rounds up to alignment.
     *
     * @tparam Type_T The type of object to be constructed within the memory region.
     * @param size    Number of elements desired.
     * @param alignment Alignment in bytes. Defaults to engine alignment constant.
     * @return Pointer to reserved region, or nullptr if unavailable.
     */
    template <typename Type_T, SizeT32 CustomAlignment_T = QENTEM_RESERVER_DEFAULT_ALIGNMENT>
    QENTEM_INLINE static Type_T *Reserve(SystemIntType size) noexcept {
        RoundUpSize<Type_T>(size);
        return static_cast<Type_T *>(ReserveRoundedUpBytes<CustomAlignment_T>(size));
    }

    /**
     * @brief Releases a previously reserved memory region back to the system.
     *
     * Assumes the `size` has already been rounded up via `RoundUpSize`. This method
     * attempts to return the region to the arena it originated from. If that fails,
     * and the platform supports multiple arenas (Linux/Windows), it will attempt to
     * return the region to sibling arenas in the `getReservers()` list.
     *
     * If the platform is single-core or non-symmetric (e.g., microcontroller),
     * the region is returned to the sole `ReserverCore` instance.
     *
     * @param ptr   Pointer to the memory to release.
     * @param size  Aligned size in bytes (must match reservation size).
     *
     * @see RoundUpSize()
     * @see getReservers()
     */
    QENTEM_INLINE static void ReleaseRoundedUpBytes(void *ptr, SystemIntType size) noexcept {
        if (ptr != nullptr) {
            Core &instance = GetCurrentInstance();

#if defined(__linux__) || defined(_WIN32)
            // Prefer returning to the current arena.
            if (instance.Release(ptr, size)) {
                return;
            }

            // Attempt return to a sibling arena (cross-core recovery).
            LiteArray<Core> &reservers = getReservers();

            for (auto &reserver : reservers) {
                if ((&reserver != &instance) && reserver.Release(ptr, size)) {
                    return;
                }
            }
#else
            // Single-arena fallback path.
            instance.Release(ptr, size);
#endif
        }
    }

    /**
     * @brief Returns a previously reserved region to the system allocator.
     *
     * Adjusts the given `size` to match alignment requirements via `RoundUpSize()`,
     * then forwards the call to `ReleaseRoundedUpBytes()`, which handles the
     * actual memory return logic.
     *
     * @tparam Type_T The type of the original contents stored in the region.
     * @param ptr     Pointer to the memory region to release.
     * @param size    Number of elements of Type_T originally requested.
     *
     * @see RoundUpSize()
     * @see ReleaseRoundedUpBytes()
     */
    template <typename Type_T>
    QENTEM_INLINE static void Release(Type_T *ptr, SystemIntType size) noexcept {
        RoundUpSize<Type_T>(size);
        ReleaseRoundedUpBytes(ptr, size);
    }


    /**
     * @brief Empties the current thread's arena and reclaims all reserved memory regions.
     *
     * Should be used with care—this severs all outstanding reservations in the current core’s pool.
     */
    QENTEM_INLINE static void Reset() noexcept {
        GetCurrentInstance().Reset();
    }

    /**
     * @brief Checks whether the current core's arena has any remaining regions in use.
     *
     * @return True if all tracked regions have been returned or released.
     */
    QENTEM_INLINE static bool IsEmpty() noexcept {
        return GetCurrentInstance().IsEmpty();
    }

    /**
     * @brief Provides access to the arena associated with the calling thread’s core.
     *
     * Each logical CPU is tied to one arena to ensure spatial locality and avoid contention.
     *
     * @return Reference to the active core’s memory management unit.
     */
    QENTEM_INLINE static Core &GetCurrentInstance() noexcept {
        static thread_local Core &instance = getReserver(CPUHelper::GetCurrentCore());
        return instance;
    }

  private:
    /**
     * @brief Constructs all core-local arenas based on detected hardware topology.
     *
     * The number of cores is queried at runtime, and each arena is initialized independently.
     */
    QENTEM_INLINE static LiteArray<Core> initReservers() noexcept {
        return LiteArray<Core>{CPUHelper::GetCoreCount(), true};
    }

    /**
     * @brief Retrieves the arena corresponding to a given core index.
     *
     * @param core_id Logical CPU index as reported by the system.
     * @return Reference to the corresponding ReserverCore.
     */
    QENTEM_INLINE static Core &getReserver(int core_id) noexcept {
#if defined(__linux__) || defined(_WIN32)
        return getReservers().Storage()[core_id];
#else
        (void)core_id;
        static Core reserver{};
        return reserver;
#endif
    }

#if defined(__linux__) || defined(_WIN32)
    /**
     * @brief Grants access to the complete set of ReserverCore instances.
     */
    QENTEM_INLINE static LiteArray<ReserverCore<>> &getReservers() noexcept {
        static LiteArray<ReserverCore<>> reservers{initReservers()};
        return reservers;
    }
#endif
};

} // namespace Qentem

#endif // QENTEM_RESERVER_HPP
