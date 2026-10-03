/**
 * @file LiteArray.hpp
 * @brief Dynamic array with a page-based SystemMemory backend.
 *
 * LiteArray is a page-backed dynamic array that stores elements in contiguous
 * memory with minimal allocation overhead. It is designed for low-level use
 * cases where predictable memory behavior and a lightweight storage backend
 * are preferred over advanced allocator features.
 *
 * Storage is allocated directly through SystemMemory and sized according to
 * the system page size. On supported platforms, unused pages can be released
 * when shrinking, while Linux can attempt to expand the existing mapping
 * in place.
 *
 * @copyright Copyright (c) 2026 Hani Ammar
 * @license MIT
 */

#ifndef QENTEM_LITE_ARRAY_H
#define QENTEM_LITE_ARRAY_H

#include "Qentem/ArrayBase.hpp"
#include "Qentem/SystemMemory.hpp"

namespace Qentem {

// Array memory backend that forwards allocation operations to SystemMemory.
struct ArrayPageBackend {
    template <typename Type_T>
    QENTEM_INLINE static Type_T *Reserve(SizeT &capacity) {
        SizeT capacity_bytes = (capacity * sizeof(Type_T));

#ifndef QENTEM_SYSTEM_MEMORY_FALLBACK
        if (capacity_bytes > SystemMemory::GetPageSize()) {
            capacity_bytes = SystemMemory::AlignToPageSize(capacity_bytes);
        } else {
            capacity_bytes = static_cast<SizeT>(SystemMemory::GetPageSize());
        }

        capacity = (capacity_bytes / sizeof(Type_T));
#endif

        return static_cast<Type_T *>(SystemMemory::Reserve(capacity_bytes));
    }

    template <typename Type_T>
    QENTEM_INLINE static void Release(Type_T *storage, SizeT capacity) {
        if (storage != nullptr) {
            const SizeT size_bytes = SystemMemory::AlignToPageSize<SizeT>(capacity * sizeof(Type_T));

            SystemMemory::Release(storage, static_cast<SystemLong>(size_bytes));
        }
    }

    template <typename Type_T>
    QENTEM_INLINE static bool Shrink(Type_T *storage, SizeT from_size, SizeT &to_size) noexcept {
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK) && !defined(_WIN32)
        if (storage != nullptr) {
            const SizeT from_size_bytes = SystemMemory::AlignToPageSize<SizeT>(from_size * sizeof(Type_T));
            const SizeT to_size_bytes   = SystemMemory::AlignToPageSize<SizeT>(to_size * sizeof(Type_T));

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

    template <typename Type_T>
    QENTEM_INLINE static bool TryExpand(Type_T *storage, SizeT from_size, SizeT &to_size) noexcept {
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK) && defined(__linux__)
        if (storage != nullptr) {
            const SizeT from_size_bytes = SystemMemory::AlignToPageSize<SizeT>(from_size * sizeof(Type_T));
            const SizeT to_size_bytes   = SystemMemory::AlignToPageSize<SizeT>(to_size * sizeof(Type_T));

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

template <typename Type_T, SizeT Expansion_Multiplier_T = 2, typename MemoryProvider_T = ArrayPageBackend>
struct LiteArray : public ArrayBase<Type_T, Expansion_Multiplier_T, MemoryProvider_T> {
    using BaseT = ArrayBase<Type_T, Expansion_Multiplier_T, MemoryProvider_T>;
    using BaseT::BaseT;
};

} // namespace Qentem

#endif
