/**
 * @file LiteHList.hpp
 * @brief Ordered key container with a page-based SystemMemory backend.
 *
 * LiteHList is an ordered associative container that stores unique keys without
 * associated values. Keys are maintained in insertion order while supporting
 * efficient hash-based lookup and contiguous storage for cache-friendly
 * iteration.
 *
 * Unlike HList, LiteHList uses SystemMemory directly for storage allocation.
 * Storage is backed by system memory pages, with capacity rounded to the
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

#ifndef QENTEM_LITE_H_LIST_H
#define QENTEM_LITE_H_LIST_H

#include "Qentem/HListBase.hpp"
#include "Qentem/SystemMemory.hpp"

namespace Qentem {

/**
 * @brief Memory backend for LiteHList using SystemMemory.
 *
 * LiteHListReserverBackend forwards storage allocation and release directly
 * to SystemMemory. Allocations are page-sized when necessary and page-aligned
 * for requests larger than a single system memory page.
 *
 * The backend can release unused pages during shrinking on supported POSIX
 * systems and can attempt in-place expansion through `mremap()` on Linux.
 */
struct LiteHListReserverBackend {
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
 * @brief Lightweight ordered key container using system memory storage.
 *
 * LiteHList provides the same ordered key-container interface as HList while
 * using SystemMemory as its storage backend. It is intended for low-level
 * runtime use where a lightweight allocation strategy is preferred over the
 * more advanced behavior provided by Reserver.
 *
 * @tparam Key_T The key type.
 * @tparam Expansion_Multiplier_T Compile-time capacity growth factor.
 * @tparam MemoryProvider_T Memory backend used for storage management.
 */
template <typename Key_T, SizeT Expansion_Multiplier_T = 2, typename MemoryProvider_T = LiteHListReserverBackend>
using LiteHList = typename HListSelector<Key_T, SizeT, Expansion_Multiplier_T, MemoryProvider_T>::Type;

} // namespace Qentem

#endif
