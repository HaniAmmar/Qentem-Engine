/**
 * @file Reserver.hpp
 * @brief High-performance memory reservation system with zero internal metadata.
 *
 * This file implements Qentem's low-level allocator, Reserver, a deterministic
 * memory reservation and reuse system designed for high-performance applications
 * that require predictable allocation behavior and minimal overhead.
 *
 * Memory is managed through fixed-size blocks that are divided and tracked using
 * compact external bit tables. Allocations contain no internal headers or
 * per-region metadata, preserving clean memory boundaries and maximizing usable
 * space. Region tracking, reuse, and coalescence are performed entirely outside
 * the allocated memory itself.
 *
 * Reserver is built around thread-local arenas to eliminate synchronization
 * overhead and improve cache locality. Each thread maintains its own independent
 * arena, allowing allocation and release operations to execute without locking
 * while preserving deterministic behavior and efficient memory reuse.
 *
 * Features:
 *  - Zero internal metadata: allocated memory remains clean and unencumbered.
 *  - External bitfield-based region tracking for fast reuse and coalescence.
 *  - Thread-local arenas for scalable, contention-free allocation.
 *  - Fixed block sizing with efficient first-fit region selection.
 *  - Supports `MemoryRecord` for allocation tracking and debugging.
 *
 * @warning Arenas are thread-local and must only be accessed by their owning
 *          thread. Concurrent access to the same arena is unsupported.
 *
 * @note    Depends on MemoryBlock and LiteArray. Behavior is customizable
 *          through compile-time macros.
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

namespace Qentem {

/**
 * @brief Thread-local memory manager for fast, reusable allocations.
 *
 * This structure manages memory reservations within a thread-local arena.
 * Memory is allocated in fixed-size blocks, divided into regions, and tracked
 * using compact bitfields. Allocation tracking is stored externally, avoiding
 * embedded metadata and preserving clean memory boundaries while minimizing
 * overhead.
 *
 * Regions can be reserved, released, expanded, or reused with deterministic
 * behavior and efficient memory locality. The allocator is designed to reduce
 * allocation overhead while maintaining predictable performance characteristics.
 *
 * Each thread owns its own arena, eliminating synchronization overhead and
 * avoiding contention during normal operation. Memory remains local to the
 * owning thread, improving cache utilization and reuse efficiency.
 *
 * @tparam Alignment_T Minimum alignment for allocations, in bytes. Defaults to
 *                     QENTEM_RESERVER_DEFAULT_ALIGNMENT.
 * @tparam BlockSize_T Size of each managed memory block. Defaults to
 *                     QENTEM_RESERVER_BLOCK_SIZE.
 */
template <SizeT32 Alignment_T = QENTEM_RESERVER_DEFAULT_ALIGNMENT, SystemLong BlockSize_T = QENTEM_RESERVER_BLOCK_SIZE>
struct ReserverCore {
    /// Alias for the memory block type managed by this allocator.
    using MemoryBlockT = MemoryBlock<Alignment_T>;

    /// Maximum value representable by SystemLong.
    static constexpr SystemLong MAX_SYSTEM_INT_TYPE = ~SystemLong{0};

    /// Alignment mask used when rounding sizes up to Alignment_T.
    static constexpr SystemLong ALIGNMENT_MASK = static_cast<SystemLong>(Alignment_T - 1U);

    /// Inverse alignment mask used for alignment rounding operations.
    static constexpr SystemLong ALIGNMENT_MASK_INV = ~ALIGNMENT_MASK;

    /// Number of bits in SystemLong (typically 32 or 64).
    static constexpr SizeT32 BIT_WIDTH = (sizeof(SystemLong) * 8U);

    QENTEM_INLINE ReserverCore() noexcept         = delete;
    ReserverCore(ReserverCore &&)                 = delete;
    ReserverCore &operator=(ReserverCore &&)      = delete;
    ReserverCore(const ReserverCore &)            = delete;
    ReserverCore &operator=(const ReserverCore &) = delete;
    QENTEM_INLINE ~ReserverCore() noexcept        = delete;

    static_assert(Alignment_T >= sizeof(void *), "Alignment_T must be at least the size of a pointer.");
    static_assert((Alignment_T & (sizeof(void *) - 1U)) == 0, "Alignment_T must be a multiple of sizeof(void *).");

    /**
     * @brief Rounds an allocation size up to the allocator's default alignment.
     *
     * Converts a count of objects into a total byte size and rounds the result
     * up to the nearest allocator alignment boundary. This ensures that all
     * reserved regions satisfy the allocator's minimum alignment requirements.
     *
     * @tparam Type_T Type being allocated.
     * @param size Number of objects to allocate.
     *
     * @return Total allocation size in bytes, rounded up to the allocator's
     *         default alignment.
     */
    template <typename Type_T>
    QENTEM_INLINE static SystemLong RoundUpBytes(SystemLong size) noexcept {
        size *= sizeof(Type_T);
        size += ALIGNMENT_MASK;
        size &= ALIGNMENT_MASK_INV;

        return size;
    }

    /**
     * @brief Reserves memory for one or more objects of the specified type.
     *
     * Computes the total allocation size for `size` objects of `Type_T`,
     * rounds the result up to the allocator's default alignment boundary,
     * and reserves a suitably aligned memory region.
     *
     * The returned memory is aligned according to `CustomAlignment_T`,
     * making it suitable for constructing objects with placement-new.
     * No objects are constructed by this function.
     *
     * The effective alignment is never less than the allocator's minimum
     * alignment (`Alignment_T`).
     *
     * @tparam Type_T Type of object to reserve memory for.
     * @tparam CustomAlignment_T Desired alignment in bytes. Defaults to
     *         `alignof(Type_T)`.
     *
     * @param size Number of `Type_T` objects to reserve space for.
     *
     * @return Pointer to an aligned memory region capable of storing
     *         `size` objects of type `Type_T`.
     *
     * @see reserveRound
     */
    template <typename Type_T, SizeT32 CustomAlignment_T = alignof(Type_T)>
    QENTEM_NOINLINE static Type_T *Reserve(SystemLong size) noexcept {
        static_assert((CustomAlignment_T > 0) && ((CustomAlignment_T & (CustomAlignment_T - 1)) == 0),
                      "alignment must be power-of-two");
        return static_cast<Type_T *>(reserveRound<Type_T, CustomAlignment_T>(size));
    }

    /**
     * @brief Releases a previously reserved memory region.
     *
     * Converts the original object count into the aligned byte size used during
     * reservation and returns the region to the allocator for reuse.
     *
     * The supplied `size` must match the number of objects originally passed to
     * `Reserve()`. The allocator uses this value to reconstruct the reservation's
     * aligned byte size before releasing the region.
     *
     * A null pointer is ignored.
     *
     * @tparam Type_T Type originally used when reserving the memory region.
     * @param ptr Pointer to the memory region to release.
     * @param size Number of `Type_T` objects originally requested.
     *
     * @see Reserve
     * @see RoundUpBytes
     */
    template <typename Type_T>
    QENTEM_NOINLINE static void Release(Type_T *ptr, SystemLong size) noexcept {
        if (ptr != nullptr) {
            release(ptr, RoundUpBytes<Type_T>(size));
        }
    }

    /**
     * @brief Shrinks a previously reserved memory region.
     *
     * Reduces the size of an existing reservation by releasing the unused tail
     * portion back to the allocator. Both the original size and the target size
     * are converted to aligned byte counts using `RoundUpBytes<Type_T>()` before
     * the shrink operation is performed.
     *
     * If `to_size` is smaller than `from_size`, the region spanning from the new
     * end of the allocation to the original end is returned to the allocator for
     * future reuse. The leading portion of the reservation remains unchanged.
     *
     * Typical use cases include:
     * - Containers that over-allocate capacity and later reclaim unused space.
     * - Buffers that shrink after parsing, serialization, or compaction.
     * - Dynamic data structures that reduce memory usage after growth.
     *
     * @tparam Type_T Type originally used when reserving the memory region.
     * @param ptr Pointer to the reserved memory region.
     * @param from_size Original number of `Type_T` objects reserved.
     * @param to_size New number of `Type_T` objects to retain.
     *
     * @return `true` if the reservation was successfully shrunk;
     *         otherwise `false`.
     *
     * @note Both sizes are internally converted to aligned byte counts before
     *       the shrink operation is performed.
     *
     * @see shrink
     * @see RoundUpBytes
     */
    template <typename Type_T>
    QENTEM_NOINLINE static bool Shrink(Type_T *ptr, SystemLong from_size, SystemLong to_size) noexcept {
        from_size = RoundUpBytes<Type_T>(from_size);
        to_size   = RoundUpBytes<Type_T>(to_size);

        if ((from_size > to_size) && (ptr != nullptr)) {
            return shrink(ptr, from_size, to_size);
        }

        return false;
    }

    /**
     * @brief Attempts to expand a previously reserved region in-place.
     *
     * Increases the size of an existing reservation from `from_size` to
     * `to_size` without relocating the allocation. Both sizes are converted
     * to aligned byte counts using `RoundUpBytes<Type_T>()` before the
     * expansion is attempted.
     *
     * Expansion succeeds only if sufficient free space exists immediately
     * after the current reservation. The allocation remains at the same
     * address and no data is moved.
     *
     * If `to_size` is less than or equal to `from_size`, no expansion is
     * required and the function returns `true`.
     *
     * @tparam Type_T Type originally used when reserving the memory region.
     * @param ptr Pointer to the existing reservation.
     * @param from_size Current number of `Type_T` objects reserved.
     * @param to_size Desired number of `Type_T` objects.
     *
     * @return `true` if the reservation already satisfies the requested size
     *         or was successfully expanded in-place; otherwise `false`.
     *
     * @see tryExpand
     * @see RoundUpBytes
     */
    template <typename Type_T>
    QENTEM_NOINLINE static bool TryExpand(Type_T *ptr, SystemLong from_size, SystemLong to_size) noexcept {
        if (ptr != nullptr) {
            from_size = RoundUpBytes<Type_T>(from_size);
            to_size   = RoundUpBytes<Type_T>(to_size);

            return (from_size >= to_size) || (tryExpand(ptr, from_size, to_size) == to_size);
        }

        return false;
    }

    /**
     * @brief Resets the allocator to its initial state.
     *
     * Releases all managed memory blocks and clears both the active and
     * exhausted block lists. After this operation, the allocator contains
     * no managed blocks and behaves as if it were newly initialized.
     *
     * If memory tracking is enabled, the released memory is reported through
     * the configured MemoryRecord hooks.
     *
     * @note All outstanding reservations become invalid after this call.
     */
    QENTEM_INLINE static void Reset() noexcept {
        // Drop all memory blocks from both active and retired lists.
        active_blocks_.Reset();
        exhausted_blocks_.Reset();
    }

    /**
     * @brief Determines whether the allocator contains no active reservations.
     *
     * This check verifies that:
     * - Every active block contains no reserved regions.
     * - No exhausted blocks remain managed by the allocator.
     *
     * @return `true` if all managed memory is currently free;
     *         otherwise `false`.
     */
    QENTEM_INLINE static bool IsEmpty() noexcept {
        // Scan all active blocks. Any reserved region means the allocator is not empty.
        for (const MemoryBlockT &block : active_blocks_) {
            if (!(block.IsEmpty())) {
                return false;
            }
        }

        return exhausted_blocks_.IsEmpty();
    }

    /**
     * @brief Returns the total number of managed memory blocks.
     *
     * Counts both active blocks and exhausted blocks currently owned by
     * the allocator, regardless of whether they are participating in
     * allocation requests.
     *
     * This function is primarily intended for diagnostics, testing,
     * and allocator state inspection.
     *
     * @return Total number of managed memory blocks.
     */
    QENTEM_INLINE static SizeT TotalBlocks() noexcept {
        return (active_blocks_.Size() + exhausted_blocks_.Size());
    }

    /**
     * @brief Returns the active memory blocks managed by the allocator.
     *
     * Provides read-only access to the collection of active blocks currently
     * participating in allocation requests. This function is primarily intended
     * for diagnostics, debugging, and unit testing.
     *
     * @return Pointer to the array of active memory blocks.
     */
    QENTEM_INLINE static const LiteArray<MemoryBlockT> *GetActiveBlocks() noexcept {
        return &active_blocks_;
    }

    /**
     * @brief Returns the exhausted memory blocks managed by the allocator.
     *
     * Provides read-only access to blocks that are currently excluded from the
     * primary allocation path. This function is primarily intended for
     * diagnostics, debugging, and unit testing.
     *
     * @return Pointer to the array of exhausted memory blocks.
     */
    QENTEM_INLINE static const LiteArray<MemoryBlockT> *GetExhaustedBlocks() noexcept {
        return &exhausted_blocks_;
    }

    /**
     * @brief Allocates a libc-style memory block (malloc).
     *
     * Reserves a memory region large enough to store both the requested
     * allocation size and an internal size header. The header is stored
     * immediately before the returned pointer and is later used by
     * `LibcResize()` and `LibcRelease()`.
     *
     * The returned memory is uninitialized.
     *
     * @param size Number of bytes to allocate.
     *
     * @return Pointer to the allocated memory region, or nullptr on failure.
     *
     * @note An internal size header of type `SystemLong` is stored directly
     *       before the returned pointer.
     *
     * @see LibcResize
     * @see LibcRelease
     */
    QENTEM_NOINLINE static void *LibcReserve(SystemLong size) noexcept {
        SystemLong  r_size = RoundUpBytes<char>(size + sizeof(SystemLong));
        void       *ptr    = reserveNoRound<Alignment_T>(r_size);
        SystemLong *l_ptr  = static_cast<SystemLong *>(ptr);

        *l_ptr = r_size;
        ++l_ptr;

        return l_ptr;
    }

    /**
     * @brief Allocates and zero-initializes a libc-style memory block (calloc).
     *
     * Reserves a memory region large enough to store both the requested
     * allocation and an internal size header. The usable portion of the
     * allocation is then initialized to zero.
     *
     * The header is stored immediately before the returned pointer and is
     * later used by `LibcResize()` and `LibcRelease()`.
     *
     * @param count Number of elements to allocate.
     * @param item_size Size of each element in bytes.
     *
     * @return Pointer to the allocated and zero-initialized memory region,
     *         or nullptr on failure.
     *
     * @note An internal size header of type `SystemLong` is stored directly
     *       before the returned pointer.
     *
     * @see LibcReserve
     * @see LibcResize
     * @see LibcRelease
     */
    QENTEM_NOINLINE static void *LibcReserveClear(SystemLong count, SystemLong item_size) noexcept {
        SystemLong  r_size = RoundUpBytes<char>((count * item_size) + sizeof(SystemLong));
        void       *ptr    = reserveNoRound<Alignment_T>(r_size);
        SystemLong *l_ptr  = static_cast<SystemLong *>(ptr);

        *l_ptr = r_size;
        ++l_ptr;

        r_size -= sizeof(SystemLong);

        if (r_size != 0) {
            MemoryUtils::SetToZeroByType(l_ptr, (r_size / sizeof(SystemLong)));
        }

        return l_ptr;
    }

    /**
     * @brief Releases a libc-style memory block (free).
     *
     * Returns a memory region previously allocated by `LibcReserve()`,
     * `LibcReserveClear()`, or successfully managed through `LibcResize()`.
     *
     * The allocation size is obtained from the internal size header stored
     * immediately before the user-visible memory region.
     *
     * A null pointer is ignored.
     *
     * @param ptr Pointer to the memory region to release.
     *
     * @see LibcReserve
     * @see LibcReserveClear
     * @see LibcResize
     */
    QENTEM_NOINLINE static void LibcRelease(void *ptr) noexcept {
        if (ptr != nullptr) {
            SystemLong *l_ptr = static_cast<SystemLong *>(ptr);

            --l_ptr;

            release(l_ptr, *l_ptr);
        }
    }

    /**
     * @brief Resizes a libc-style memory block (realloc).
     *
     * Attempts to adjust the size of a previously allocated memory region.
     * If the allocation can be expanded or shrunk in place, the original
     * pointer is preserved. Otherwise, a new region is allocated, the
     * existing data is copied, and the old region is released.
     *
     * The original allocation size is obtained from the internal size header
     * stored immediately before the user-visible memory region.
     *
     * If `ptr` is nullptr, this function behaves like `LibcReserve()`.
     * If `new_size` is zero, the allocation is released and nullptr is
     * returned.
     *
     * @param ptr Pointer previously returned by `LibcReserve()` or
     *            `LibcReserveClear()`.
     * @param new_size Desired size in bytes.
     *
     * @return Pointer to the resized memory region, or nullptr when
     *         `new_size` is zero or allocation fails.
     *
     * @see LibcReserve
     * @see LibcReserveClear
     * @see LibcRelease
     */
    QENTEM_NOINLINE static void *LibcResize(void *ptr, SystemLong size) {
        if (ptr != nullptr) {
            SystemLong *l_ptr = static_cast<SystemLong *>(ptr);
            --l_ptr;
            const SystemLong old_size = *l_ptr;

            if (size != 0) {
                SystemLong new_size = (size + sizeof(SystemLong));

                if (new_size > old_size) {
                    if (TryExpand(reinterpret_cast<char *>(l_ptr), old_size, new_size)) {
                        return ptr;
                    }

                    char *new_ptr = static_cast<char *>(LibcReserve(size));
                    MemoryUtils::CopyTo(new_ptr, ptr, (old_size - sizeof(SystemLong)));
                    release(l_ptr, old_size);

                    return new_ptr;
                }

                if (new_size < old_size) {
                    Shrink(reinterpret_cast<char *>(l_ptr), old_size, new_size);
                }

                return ptr;
            }

            release(l_ptr, old_size);

            return nullptr;
        }

        return LibcReserve(size);
    }

  private:
    /**
     * @brief Reserves a memory region from the allocator's managed block pool.
     *
     * Reserves `size` bytes of memory aligned to `CustomAlignment_T`.
     * The supplied size must already be rounded to the allocator's
     * alignment boundary.
     *
     * Allocation proceeds in two phases:
     *   1. Search existing active blocks using a first-fit strategy.
     *   2. If no suitable region is found, allocate a new memory block
     *      and satisfy the request from that block.
     *
     * Blocks that become fully consumed are moved to the exhausted block
     * list and excluded from the primary allocation path.
     *
     * Large allocations that consume an entire block are placed directly
     * into the exhausted block list since they cannot satisfy additional
     * allocation requests.
     *
     * @tparam CustomAlignment_T Required alignment in bytes.
     * @param size Allocation size in bytes. The value must already be
     *             rounded to the allocator's alignment boundary.
     *
     * @return Pointer to the reserved memory region.
     *
     * @note Callers are responsible for rounding sizes before invoking
     *       this function. Use `RoundUpBytes<T>()` when appropriate.
     */
    template <SizeT32 CustomAlignment_T = Alignment_T>
    static void *reserve(SystemLong size) {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
        MemoryRecord::Reserved(size);
#endif

        ///////////////////////////////////////////////////////////
        // Phase 1: Search existing active blocks.

        const SystemLong chunks = (size >> MemoryBlockT::DefaultAlignmentBit());

        for (MemoryBlockT &current_block : active_blocks_) {
            if (current_block.Available() >= size) {
                void *ptr = reserveFirstFit<CustomAlignment_T>(&current_block, chunks);

                if (ptr != nullptr) {
                    current_block.DecreaseAvailable(size);

                    // Move fully consumed blocks to the exhausted list.
                    if (current_block.Available() == 0) {
                        moveToExhaustedBlock(&current_block);
                    }

                    return ptr;
                }
            }
        }

        ///////////////////////////////////////////////////////////
        // Phase 2: No existing block could satisfy the request.

#ifndef QENTEM_SYSTEM_MEMORY_FALLBACK
        const SystemLong adjusted_size = size;
#else
        const SystemLong adjusted_size = (size + (CustomAlignment_T << 1U)); // Ensure correct alignment.
#endif

        // Allocate a new block large enough for the request.
        MemoryBlockT new_block{(adjusted_size <= BlockSize_T) ? BlockSize_T : adjusted_size};

        new_block.DecreaseAvailable(size);

        MemoryBlockT *new_block_ptr;

        // The new block has remaining capacity after this allocation.
        if (size < new_block.UsableSize()) {
            new_block_ptr = &(active_blocks_.Insert(QUtility::Move(new_block)));

            // Initialize region tracking for the newly inserted block.
            new_block_ptr->ClearTable();

            void *ptr = reserveFirstFit<CustomAlignment_T>(new_block_ptr, chunks);

            // Keep the largest active block at the front to improve search efficiency.
            if (active_blocks_.First()->UsableSize() < new_block_ptr->UsableSize()) {
                QUtility::Swap(*(active_blocks_.Storage()), *new_block_ptr);
            }

            return ptr;
        }

        // The request consumes the entire block; place it directly
        // in the exhausted list and return its base address.
        new_block_ptr = &(exhausted_blocks_.Insert(QUtility::Move(new_block)));

        return new_block_ptr->Base();
    }

    /**
     * @brief Reserves a memory region without size rounding.
     *
     * Uses the greater of `CustomAlignment_T` and the allocator's minimum
     * alignment (`Alignment_T`) to ensure the returned region satisfies
     * the allocator's alignment requirements.
     *
     * @tparam CustomAlignment_T Requested alignment in bytes.
     * @param size Allocation size in bytes.
     *
     * @return Pointer to the reserved memory region.
     */
    template <SizeT32 CustomAlignment_T>
    QENTEM_NOINLINE static void *reserveNoRound(SystemLong size) noexcept {
        if constexpr (CustomAlignment_T >= Alignment_T) {
            return reserve<CustomAlignment_T>(size);
        } else {
            return reserve<Alignment_T>(size);
        }
    }

    /**
     * @brief Reserves a memory region after rounding its size.
     *
     * Converts an object count into an aligned byte size using
     * `RoundUpBytes<Type_T>()`, then reserves the resulting memory region
     * using the specified alignment.
     *
     * The effective alignment is the greater of `CustomAlignment_T`
     * and the allocator's minimum alignment (`Alignment_T`).
     *
     * @tparam Type_T Type being allocated.
     * @tparam CustomAlignment_T Requested alignment in bytes.
     * @param size Number of objects to allocate.
     *
     * @return Pointer to the reserved memory region.
     */
    template <typename Type_T, SizeT32 CustomAlignment_T>
    QENTEM_NOINLINE static void *reserveRound(SystemLong size) noexcept {
        return reserveNoRound<CustomAlignment_T>(RoundUpBytes<Type_T>(size));
    }

    /**
     * @brief Releases a previously reserved memory region.
     *
     * Returns a memory region to the allocator and updates the owning block's
     * availability information. The allocator first locates the block that owns
     * the supplied pointer, then performs the appropriate reclamation strategy.
     *
     * Active blocks:
     * - The released region is marked free.
     * - Availability is increased.
     * - Fully empty blocks may be released when other active blocks remain.
     *
     * Exhausted blocks:
     * - Released regions within the usable area restore available space and
     *   return the block to the active block list.
     * - Dedicated oversized blocks may be released entirely.
     * - A standard-sized exhausted block may be reset and reused when no
     *   active blocks remain.
     *
     * @param ptr Pointer to the memory region being released.
     * @param size Size of the region in bytes.
     *
     * @return `true` if the region was successfully released;
     *         otherwise `false`.
     *
     * @warning The pointer must originate from this allocator and must not
     *          have been released previously. Violating these requirements
     *          results in undefined behavior.
     */
    static bool release(void *ptr, SystemLong size) {
        // Phase 1: Search active blocks.
        for (MemoryBlockT &block : active_blocks_) {
            // Regular allocation from the block's usable region.
            if ((ptr >= block.Data()) && (ptr < block.End())) {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
                MemoryRecord::Released(size);
#endif
                block.IncreaseAvailable(size);
                block.ReleaseRegion(ptr, (size >> block.DefaultAlignmentBit()));

                // Release fully empty blocks while preserving at least one active block.
                if ((active_blocks_.Size() != SizeT{1}) && block.IsEmpty()) {
                    releaseBlock(&block);
                }

                return true;
            }
        }

        // Phase 2: Search exhausted blocks.
        for (MemoryBlockT &block : exhausted_blocks_) {
            // Whole-block allocation returned from Base().
            if ((ptr >= block.Base()) && (ptr < block.End())) {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
                MemoryRecord::Released(size);
#endif
                if (ptr >= block.Data()) {
                    // Restore available space and return the block to the active list.
                    block.IncreaseAvailable(size);
                    block.ReleaseRegion(ptr, (size >> block.DefaultAlignmentBit()));
                    moveToActiveBlock(&block);
                } else if ((block.Capacity() != BlockSize_T) || active_blocks_.IsNotEmpty()) {
                    // Release oversized blocks or extra standard blocks.
                    releaseExhaustedBlock(&block);
                } else {
                    // Reuse the last remaining standard-sized block.
                    block.ClearTable();
                    block.IncreaseAvailable(size);
                    moveToActiveBlock(&block);
                }

                return true;
            }
        }

#if defined(QENTEM_DEBUG) && !defined(_WIN32)
        __builtin_trap(); // Pointer released from a different thread than the allocating thread.
#endif

        // Unknown origin — pointer does not belong to any known region.
        return false;
    }

    /**
     * @brief Shrinks an existing allocation in-place.
     *
     * Reduces an allocation from `from_size` bytes to `to_size` bytes by
     * reclaiming the unused tail region. The allocation address remains
     * unchanged and the leading `to_size` bytes remain valid.
     *
     * The allocator locates the block that owns the allocation and returns
     * the released portion to that block's available space.
     *
     * For allocations originating from a block's normal allocation region,
     * the released tail is also returned to the block's region tracking
     * table and the block may be returned to the active block list.
     *
     * Whole-block allocations are handled separately since they do not
     * participate in normal region tracking.
     *
     * @param ptr Pointer to the allocation being shrunk.
     * @param from_size Current allocation size in bytes.
     * @param to_size Desired allocation size in bytes.
     *
     * @return `true` if the allocation was successfully shrunk;
     *         otherwise `false`.
     */
    static bool shrink(void *ptr, SystemLong from_size, SystemLong to_size) {
        char            *ptr_tail = (static_cast<char *>(ptr) + to_size);
        const SystemLong diff     = (from_size - to_size);

        // Phase 1: Search active blocks.
        for (MemoryBlockT &block : active_blocks_) {
            // Allocation from the block's normal allocation region.
            if ((ptr >= block.Data()) && (ptr < block.End())) {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
                MemoryRecord::Shrink(diff);
#endif

                block.IncreaseAvailable(diff);
                block.ReleaseRegion(ptr_tail, (diff >> block.DefaultAlignmentBit()));

                return true;
            }
        }

        // Phase 2: Search exhausted blocks.
        for (MemoryBlockT &block : exhausted_blocks_) {
            // Allocation belongs to this exhausted block.
            if ((ptr >= block.Base()) && (ptr < block.End())) {
#ifdef QENTEM_ENABLE_MEMORY_RECORD
                MemoryRecord::Shrink(diff);
#endif

                block.IncreaseAvailable(diff);

                if (ptr >= block.Data()) {
                    // Normal allocation tracked by the block's region table.
                    block.ReleaseRegion(ptr_tail, (diff >> block.DefaultAlignmentBit()));
                    moveToActiveBlock(&block);
                }

                // Whole-block allocations do not use the region tracking table.
                // Physically shrinking the underlying block would require rebuilding
                // allocator metadata and may involve platform-specific memory management
                // constraints, so only the available-byte count is updated.

                return true;
            }
        }

#if defined(QENTEM_DEBUG) && !defined(_WIN32)
        __builtin_trap(); // Pointer released from a different thread than the allocating thread.
#endif

        // Unknown origin — pointer does not belong to any known region.
        return false;
    }

    /**
     * @brief Attempts to expand an allocation in-place.
     *
     * Tries to increase an existing allocation from `from_size` bytes to
     * `to_size` bytes without relocating the allocation. The allocator
     * locates the active block that owns the allocation and checks whether
     * the region immediately following the allocation is available.
     *
     * If sufficient contiguous space exists, the additional region is
     * reserved and the block's available space is updated accordingly.
     *
     * If expansion is not possible, the allocation remains unchanged.
     *
     * @param ptr Pointer to the beginning of an existing allocation.
     * @param from_size Current allocation size in bytes.
     * @param to_size Desired allocation size in bytes.
     *
     * @return `to_size` if the allocation was successfully expanded,
     *         `from_size` if expansion was not possible, or
     *         `0` if the pointer does not belong to any active block.
     */
    static SystemLong tryExpand(void *ptr, SystemLong from_size, SystemLong to_size) {
        const SystemLong diff = (to_size - from_size);

        for (MemoryBlockT &block : active_blocks_) {
            if ((ptr >= block.Data()) && (ptr < block.End())) {
                // Attempt to reserve the region immediately following the allocation.
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
     * @brief Finds and reserves the first suitable free region.
     *
     * Implements the allocator's internal first-fit reservation strategy.
     *
     * @param block Memory block to search.
     * @param chunks Number of allocation units requested.
     *
     * @return Pointer to the reserved region, or nullptr if no suitable
     *         region exists.
     */
    template <SizeT32 CustomAlignment_T>
    static void *reserveFirstFit(MemoryBlockT *block, const SystemLong chunks) noexcept {
        constexpr SystemLong alignment_mask     = static_cast<SystemLong>(CustomAlignment_T - 1U);
        constexpr SystemLong alignment_mask_inv = ~alignment_mask;
        constexpr SizeT32    bit_width_mask_m1  = (BIT_WIDTH - 1U);

        const SystemLong *table       = static_cast<const SystemLong *>(block->Base());
        const SystemLong  table_size  = block->TableSize();
        SystemLong        table_index = block->GetRefNextIndex();
        SystemLong        start_index = block->GetRefNextIndex();
        SystemLong        start_bit   = 0;
        SystemLong        region_size = 0;

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
                SizeT32 available = (bit_width_mask_m1 - Platform::FindLastBit(current));
                region_size += available;

                if (region_size >= chunks) {
                    const SystemLong bit_index = (start_bit + (BIT_WIDTH * start_index));

                    if constexpr (CustomAlignment_T <= Alignment_T) {
                        block->ReserveRegion(bit_index, chunks);
                        return (static_cast<char *>(block->Data()) +
                                (bit_index << MemoryBlockT::DefaultAlignmentBit()));
                    } else {
                        const SystemLong ptr        = (reinterpret_cast<SystemLong>(block->Data()) +
                                                       (bit_index << MemoryBlockT::DefaultAlignmentBit()));
                        const SystemLong diff       = (((ptr + alignment_mask) & alignment_mask_inv) - ptr);
                        const SystemLong index_diff = (diff >> MemoryBlockT::DefaultAlignmentBit());

                        if (region_size >= index_diff) {
                            region_size -= index_diff;

                            if (region_size >= chunks) {
                                block->ReserveRegion((bit_index + index_diff), chunks);
                                return reinterpret_cast<void *>(ptr + diff);
                            } else {
                                start_bit += index_diff;
                                start_index += (start_bit >> MemoryBlockT::TableBitShift());
                                start_bit &= bit_width_mask_m1;
                            }
                        }
                    }
                }

                current <<= available;
                shifted += available;
                current   = ~current;
                available = (bit_width_mask_m1 - Platform::FindLastBit(current));
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
                    block->ReserveRegion(bit_index, chunks);
                    return (static_cast<char *>(block->Data()) + (bit_index << MemoryBlockT::DefaultAlignmentBit()));
                } else {
                    const SystemLong ptr        = (reinterpret_cast<SystemLong>(block->Data()) +
                                                   (bit_index << MemoryBlockT::DefaultAlignmentBit()));
                    const SystemLong diff       = (((ptr + alignment_mask) & alignment_mask_inv) - ptr);
                    const SystemLong index_diff = (diff >> MemoryBlockT::DefaultAlignmentBit());

                    if (region_size >= index_diff) {
                        region_size -= index_diff;

                        if (region_size >= chunks) {
                            block->ReserveRegion((bit_index + index_diff), chunks);
                            return reinterpret_cast<void *>(ptr + diff);
                        } else {
                            start_bit += index_diff;
                            start_index += (start_bit >> MemoryBlockT::TableBitShift());
                            start_bit &= bit_width_mask_m1;
                        }
                    }
                }
            }

            ++table_index;
        }

        return nullptr;
    }

    /**
     * @brief Attempts to reserve a specific region within a memory block.
     *
     * Verifies that `chunks` contiguous allocation units are available
     * beginning exactly at `ptr`. If the entire range is free, the region
     * is marked as reserved and the function returns `true`.
     *
     * Unlike normal allocation searches, this function does not look for
     * alternative locations. Reservation succeeds only if the requested
     * address and all following chunks are immediately available.
     *
     * This helper is primarily used to support in-place allocation
     * expansion.
     *
     * @param block Pointer to the memory block being examined.
     * @param ptr Exact address at which the reservation must begin.
     * @param chunks Number of contiguous allocation units to reserve.
     *
     * @return `true` if the requested region was reserved;
     *         otherwise `false`.
     */
    static bool reserveAt(MemoryBlockT *block, void *ptr, SystemLong chunks) {
        constexpr SizeT32 bit_width_m1 = (BIT_WIDTH - 1U);

        const SystemLong *table = static_cast<const SystemLong *>(block->Base());
        SystemLong        table_index;
        SystemLong        shifted;

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
     * @brief Releases an active memory block from the allocator.
     *
     * Removes the specified block from the active block list. To keep
     * removal efficient, the block is swapped with the last element
     * before being removed when it is not already the final entry.
     *
     * The underlying memory owned by the block is released when the
     * block object is destroyed during removal.
     *
     * @param block Pointer to the active block to release.
     */
    static void releaseBlock(MemoryBlockT *block) noexcept {
        MemoryBlockT *last_block = active_blocks_.Last();

        // Move the target block to the end for O(1) removal.
        if (block != last_block) {
            QUtility::Swap(*block, *last_block);
        }

        active_blocks_.Drop(SizeT{1});
    }

    /**
     * @brief Releases an exhausted memory block from the allocator.
     *
     * Removes the specified block from the exhausted block list. To keep
     * removal efficient, the block is swapped with the last element
     * before being removed when it is not already the final entry.
     *
     * The underlying memory owned by the block is released when the
     * block object is destroyed during removal.
     *
     * @param block Pointer to the exhausted block to release.
     */
    static void releaseExhaustedBlock(MemoryBlockT *block) noexcept {
        MemoryBlockT *last_block = exhausted_blocks_.Last();

        // Move the target block to the end for O(1) removal.
        if (block != last_block) {
            QUtility::Swap(*block, *last_block);
        }

        exhausted_blocks_.Drop(SizeT{1});
    }

    /**
     * @brief Moves an active block to the exhausted block list.
     *
     * Removes the specified block from the active block list and transfers
     * ownership to the exhausted block list.
     *
     * To keep removal efficient, the block is first swapped with the last
     * active block when necessary, allowing constant-time removal.
     *
     * @param block Pointer to the active block to move.
     */
    static void moveToExhaustedBlock(MemoryBlockT *block) noexcept {
        MemoryBlockT *last_block = active_blocks_.Last();

        if (block != last_block) {
            // Move the target block to the end for O(1) removal.
            QUtility::Swap(*block, *last_block);
        }

        exhausted_blocks_ += QUtility::Move(*last_block);
        active_blocks_.DropFast(SizeT{1});
    }

    /**
     * @brief Moves an exhausted block back to the active block list.
     *
     * Removes the specified block from the exhausted block list and
     * transfers ownership to the active block list.
     *
     * To keep removal efficient, the block is first swapped with the last
     * exhausted block when necessary, allowing constant-time removal.
     *
     * After insertion, the block may be promoted to the front of the
     * active block list if it has a larger usable capacity than the
     * current leading block.
     *
     * @param block Pointer to the exhausted block to move.
     */
    static void moveToActiveBlock(MemoryBlockT *block) noexcept {
        MemoryBlockT *last_block = exhausted_blocks_.Last();

        if (block != last_block) {
            // Move the target block to the end for O(1) removal.
            QUtility::Swap(*block, *last_block);
        }

        active_blocks_ += QUtility::Move(*last_block);

        last_block = active_blocks_.Last();

        // Keep the largest active block at the front to improve search efficiency.
        if (active_blocks_.First()->UsableSize() < last_block->UsableSize()) {
            QUtility::Swap(*(active_blocks_.Storage()), *last_block);
        }

        exhausted_blocks_.DropFast(SizeT{1});
    }

    /**
     * @brief Active memory blocks available for allocation requests.
     *
     * Blocks in this array participate in allocation searches and may
     * satisfy new reservation requests. When a block becomes fully
     * consumed, it is moved to `exhausted_blocks_`.
     *
     * The largest active block is typically kept at the front of the
     * array to improve allocation search efficiency.
     */
    inline static thread_local LiteArray<MemoryBlockT> active_blocks_{};

    /**
     * @brief Exhausted memory blocks currently excluded from allocation searches.
     *
     * These blocks are temporarily removed from the primary allocation
     * path because they have no immediately available space for new
     * reservations or are dedicated to a specific allocation.
     *
     * Depending on allocator state, exhausted blocks may later be
     * reactivated, reset, or released.
     */
    inline static thread_local LiteArray<MemoryBlockT> exhausted_blocks_{};
};

using Reserver = ReserverCore<>;

} // namespace Qentem

#endif // QENTEM_RESERVER_HPP
