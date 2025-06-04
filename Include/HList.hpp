/**
 * @file HList.hpp
 * @brief Defines an ordered key list container for Qentem Engine.
 *
 * This header implements the HList container, an associative structure that
 * stores only keys (without values) and maintains their insertion order.
 * HList provides efficient ordered key storage and iteration, similar to HArray,
 * making it useful for scenarios where unique, ordered keys are required.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_H_LIST_H
#define QENTEM_H_LIST_H

#include "HashTable.hpp"

namespace Qentem {

template <typename Key_T>
struct HLItem_T {
    SizeT Hash;
    SizeT Next;
    Key_T Key;

    inline bool operator<(const HLItem_T &item) const noexcept {
        return (Key < item.Key);
    }

    inline bool operator>(const HLItem_T &item) const noexcept {
        return (Key > item.Key);
    }

    inline bool operator<=(const HLItem_T &item) const noexcept {
        return (Key <= item.Key);
    }

    inline bool operator>=(const HLItem_T &item) const noexcept {
        return (Key >= item.Key);
    }

    inline bool operator==(const HLItem_T &item) const noexcept {
        return (Key == item.Key);
    }

    inline void Clear() {
        Key = Key_T{};
    }

    inline void MoveDoublecat(HLItem_T &item) {
        Memory::Dispose(&(item.Key));
    }

    constexpr void CopyValue(HLItem_T const &) {
    }

    constexpr void InitValue() {
    }
};

///////////////////////////////////////////////
template <typename Key_T>
struct HList : public HashTable<Key_T, HLItem_T<Key_T>> {
    using HItem  = HLItem_T<Key_T>;
    using BaseT  = HashTable<Key_T, HItem>;
    using Char_T = typename Key_T::CharType;

    using BaseT::BaseT;
};

} // namespace Qentem

#endif
