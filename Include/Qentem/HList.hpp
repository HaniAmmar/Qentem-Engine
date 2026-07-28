/**
 * @file HList.hpp
 * @brief Ordered key container with pluggable memory backend.
 *
 * HList is an ordered associative container that stores unique keys without
 * associated values. Keys are maintained in insertion order while supporting
 * efficient hash-based lookup and contiguous storage for cache-friendly
 * iteration.
 *
 * The container automatically selects the most appropriate implementation
 * for the key type and exposes a unified interface for both string and
 * numeric keys. Capacity growth behavior is defined at compile time through
 * an expansion multiplier, allowing predictable reallocation semantics with
 * zero runtime overhead.
 *
 * @copyright Copyright (c) 2026 Hani Ammar
 * @license MIT
 */

#ifndef QENTEM_H_LIST_H
#define QENTEM_H_LIST_H

#include "Qentem/HListBase.hpp"
#include "Qentem/Reserver.hpp"

namespace Qentem {

// HList memory backend that forwards allocation operations to Reserver.
struct HListReserverBackend {
    template <typename HItem_T, typename Number_T>
    QENTEM_INLINE static HItem_T *Reserve(Number_T &capacity) {
        return Reserver::Reserve<HItem_T>(capacity);
    }

    template <typename HItem_T, typename Number_T>
    QENTEM_INLINE static void Release(HItem_T *storage, Number_T capacity) {
        Reserver::Release(storage, capacity);
    }

    template <typename HItem_T, typename Number_T>
    QENTEM_INLINE static bool Shrink(HItem_T *storage, Number_T from_size, Number_T to_size) noexcept {
        return Reserver::Shrink<HItem_T>(storage, from_size, to_size);
    }

    template <typename HItem_T, typename Number_T>
    QENTEM_INLINE static bool TryExpand(HItem_T *storage, Number_T from_size, Number_T to_size) noexcept {
        return Reserver::TryExpand(storage, from_size, to_size);
    }
};

/**
 * @brief Ordered key container for unique keys.
 *
 * HList stores unique keys in insertion order while providing efficient
 * hash-based lookup. The implementation is selected automatically based
 * on the key type.
 *
 * @tparam Key_T
 *         The key type.
 * @tparam Expansion_Multiplier_T
 *         Compile-time capacity growth factor.
 * @tparam MemoryProvider_T
 *         Memory backend used for storage management.
 */
template <typename Key_T, SizeT Expansion_Multiplier_T = 2, typename MemoryProvider_T = HListReserverBackend>
using HList = typename HListSelector<Key_T, SizeT, Expansion_Multiplier_T, MemoryProvider_T>::Type;

} // namespace Qentem

#endif
