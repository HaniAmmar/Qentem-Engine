/**
 * @file LiteHArray.hpp
 * @brief Ordered hash-array container with page-sized system memory backend.
 *
 * LiteHArray is an ordered associative container that combines array-style
 * iteration and indexed access with hash-based key lookup. Elements are stored
 * in insertion order within a single contiguous memory block while supporting
 * efficient key-based retrieval.
 *
 * Unlike HArray, LiteHArray uses SystemMemory directly for storage allocation.
 * Allocations are backed by system memory pages, with capacity rounded to the
 * required page boundary and at least one page allocated for nonzero capacity.
 * This provides a lightweight memory backend with predictable allocation
 * behavior and minimal allocator overhead.
 *
 * On supported POSIX systems, the backend can release unused pages when
 * shrinking. On Linux, it can also attempt to expand the existing mapping
 * in place using `mremap()`. When an in-place resize is unavailable or fails,
 * the container can fall back to its normal reallocation path.
 *
 * @copyright Copyright (c) 2026 Hani Ammar
 * @license MIT
 */

#ifndef QENTEM_LITE_H_ARRAY_H
#define QENTEM_LITE_H_ARRAY_H

#include "Qentem/HArrayBase.hpp"
#include "Qentem/SystemMemory.hpp"

namespace Qentem {

/**
 * @brief Memory backend for LiteHArray using SystemMemory.
 *
 * LiteHArrayReserverBackend forwards storage allocation and release directly
 * to SystemMemory. Allocations are page-sized when necessary and page-aligned
 * for requests larger than a single system memory page.
 *
 * Unlike the regular HArrayReserverBackend, this backend uses page-based
 * system memory and can release unused pages during shrinking. On Linux,
 * it can also attempt in-place expansion through `mremap()`.
 */
struct LiteHArrayReserverBackend {
    template <typename Type_T, typename Number_T>
    QENTEM_INLINE static Type_T *Reserve(Number_T &capacity) {
        Number_T capacity_bytes = (capacity * sizeof(Type_T));

#ifndef QENTEM_SYSTEM_MEMORY_FALLBACK
        if (capacity_bytes > SystemMemory::GetPageSize()) {
            capacity_bytes = SystemMemory::AlignToPageSize(capacity_bytes);
        } else {
            capacity_bytes = static_cast<Number_T>(SystemMemory::GetPageSize());
        }

        capacity = (capacity_bytes / sizeof(Type_T));
#endif

        return static_cast<Type_T *>(SystemMemory::Reserve(capacity_bytes));
    }

    template <typename Type_T, typename Number_T>
    QENTEM_INLINE static void Release(Type_T *storage, Number_T capacity) {
        if (storage != nullptr) {
            const Number_T size_bytes = SystemMemory::AlignToPageSize<Number_T>(capacity * sizeof(Type_T));

            SystemMemory::Release(storage, static_cast<SystemLong>(size_bytes));
        }
    }

    template <typename Type_T, typename Number_T>
    QENTEM_INLINE static bool Shrink(Type_T *storage, Number_T from_size, Number_T &to_size) noexcept {
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK) && !defined(_WIN32)
        if (storage != nullptr) {
            const Number_T from_size_bytes = SystemMemory::AlignToPageSize<Number_T>(from_size * sizeof(Type_T));
            const Number_T to_size_bytes   = SystemMemory::AlignToPageSize<Number_T>(to_size * sizeof(Type_T));

            if ((to_size_bytes < from_size_bytes) &&
                SystemMemory::ReleasePages((reinterpret_cast<char *>(storage) + to_size_bytes),
                                           static_cast<SystemLong>(from_size_bytes - to_size_bytes))) {
                to_size = (to_size_bytes / sizeof(Type_T));

                return true;
            }
        }
#else
        (void)storage;
        (void)from_size;
        (void)to_size;
#endif
        return false;
    }

    template <typename Type_T, typename Number_T>
    QENTEM_INLINE static bool TryExpand(Type_T *storage, Number_T from_size, Number_T &to_size) noexcept {
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK) && defined(__linux__)
        if (storage != nullptr) {
            const Number_T from_size_bytes = SystemMemory::AlignToPageSize<Number_T>(from_size * sizeof(Type_T));
            const Number_T to_size_bytes   = SystemMemory::AlignToPageSize<Number_T>(to_size * sizeof(Type_T));

            if ((to_size_bytes > from_size_bytes) &&
                SystemMemory::ExpandPages(storage, from_size_bytes, to_size_bytes)) {
                to_size = (to_size_bytes / sizeof(Type_T));

                return true;
            }
        }
#else
        (void)storage;
        (void)from_size;
        (void)to_size;
#endif
        return false;
    }
};

/**
 * @brief Lightweight ordered associative array using system memory storage.
 *
 * LiteHArray provides the same ordered hash-array interface as HArray while
 * using SystemMemory as its storage backend. It is intended for low-level
 * runtime use where a lightweight allocation strategy is preferred over the
 * more advanced behavior provided by Reserver.
 *
 * @tparam Key_T The key type.
 * @tparam Value_T The value type.
 * @tparam Expansion_Multiplier_T Compile-time capacity growth factor.
 * @tparam MemoryProvider_T Memory backend used for storage management.
 */
template <typename Key_T, typename Value_T, SizeT Expansion_Multiplier_T = 2,
          typename MemoryProvider_T = LiteHArrayReserverBackend>
using LiteHArray = typename HArraySelector<Key_T, SizeT, Value_T, Expansion_Multiplier_T, MemoryProvider_T>::Type;

} // namespace Qentem

#endif
