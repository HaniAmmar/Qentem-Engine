/**
 * @file HArray.hpp
 * @brief Ordered hash-array container with pluggable memory backend.
 *
 * HArray is an ordered associative container that combines array-style
 * iteration and indexed access with hash-based key lookup. Elements are
 * stored in insertion order within a single contiguous memory block while
 * supporting efficient key-based retrieval.
 *
 * The container automatically selects the most appropriate implementation
 * for the key type and exposes a unified interface for both string and
 * numeric keys. Capacity growth behavior is defined at compile time through
 * an expansion multiplier, allowing predictable reallocation semantics with
 * zero runtime overhead.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright license MIT
 */

#ifndef QENTEM_H_ARRAY_H
#define QENTEM_H_ARRAY_H

#include "Qentem/HArrayBase.hpp"
#include "Qentem/Reserver.hpp"

namespace Qentem {

// HArray memory backend that forwards allocation operations to Reserver.
struct HArrayReserverBackend {
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
 * @brief Ordered associative array with array and hash-table semantics.
 *
 * @tparam Key_T The key type.
 * @tparam Value_T The value type.
 * @tparam Expansion_Multiplier_T Compile-time capacity growth factor.
 * @tparam MemoryProvider_T Memory backend used for storage management.
 */
template <typename Key_T, typename Value_T, SizeT Expansion_Multiplier_T = 2,
          typename MemoryProvider_T = HArrayReserverBackend>
using HArray = typename HArraySelector<Key_T, SizeT, Value_T, Expansion_Multiplier_T, MemoryProvider_T>::Type;

} // namespace Qentem

#endif
