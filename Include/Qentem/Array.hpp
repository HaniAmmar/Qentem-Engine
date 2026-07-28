/**
 * @brief Dynamic array with Reserver memory Backend.
 *
 * Array is a resizable, contiguous container for elements of type `Type_T`.
 * It stores elements in a single block of memory for fast indexed access,
 * efficient iteration, and low-overhead growth.
 *
 * @copyright Copyright (c) 2026 Hani Ammar
 * @license MIT
 */

#ifndef QENTEM_ARRAY_H
#define QENTEM_ARRAY_H

#include "Qentem/ArrayBase.hpp"
#include "Qentem/Reserver.hpp"

namespace Qentem {

// Array memory backend that forwards allocation operations to Reserver.
struct ArrayReserverBackend {
    template <typename Type_T>
    QENTEM_INLINE static Type_T *Reserve(SizeT &capacity) {
        return Reserver::Reserve<Type_T>(capacity);
    }

    template <typename Type_T>
    QENTEM_INLINE static void Release(Type_T *storage, SizeT capacity) {
        Reserver::Release(storage, capacity);
    }

    template <typename Type_T>
    QENTEM_INLINE static bool Shrink(Type_T *storage, SizeT from_size, SizeT to_size) noexcept {
        return Reserver::Shrink<Type_T>(storage, from_size, to_size);
    }

    template <typename Type_T>
    QENTEM_INLINE static bool TryExpand(Type_T *storage, SizeT from_size, SizeT to_size) noexcept {
        return Reserver::TryExpand(storage, from_size, to_size);
    }
};

template <typename Type_T, SizeT Expansion_Multiplier_T = 2, typename MemoryProvider_T = ArrayReserverBackend>
struct Array : public ArrayBase<Type_T, Expansion_Multiplier_T, MemoryProvider_T> {
    using BaseT = ArrayBase<Type_T, Expansion_Multiplier_T, MemoryProvider_T>;
    using BaseT::BaseT;
};

} // namespace Qentem

#endif
