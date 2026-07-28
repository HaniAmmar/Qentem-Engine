/**
 * @file LiteArray.hpp
 * @brief Dynamic array with page-sized system memory Backend
 *
 * LiteArray is a page-backed dynamic array that stores elements in
 * contiguous memory with minimal allocation overhead. It is designed for cases
 * where resizing support is needed, but advanced allocator behavior is not.
 * Growth and release are handled through the system memory provider, making the
 * container simple, predictable, and suitable for low-level runtime use.
 *
 * @copyright Copyright (c) 2026 Hani Ammar
 * @license MIT
 */

#ifndef QENTEM_LITE_ARRAY_H
#define QENTEM_LITE_ARRAY_H

#include "Qentem/ArrayBase.hpp"
#include "Qentem/SystemMemory.hpp"
#include "Qentem/Platform.hpp"

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
        SystemMemory::Release(storage, (capacity * sizeof(Type_T)));
    }

    template <typename Type_T>
    QENTEM_INLINE constexpr static bool Shrink(Type_T *, SizeT, SizeT) noexcept {
        return false;
    }

    template <typename Type_T>
    QENTEM_INLINE constexpr static bool TryExpand(Type_T *, SizeT, SizeT) noexcept {
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
