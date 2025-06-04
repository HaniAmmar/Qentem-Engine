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

#include "HashTableAdapters.hpp"

namespace Qentem {

/**
 * @brief Hash table item for key-only associative containers (HList).
 *
 * This struct represents a single entry in an HList container, storing only a key,
 * hash, and chain link for collision resolution. Provides basic value initialization,
 * move, and copy semantics suitable for use in ordered, unique-key sets.
 *
 * @tparam Key_T The key type, which must implement the required interface (operators, etc.).
 */
template <typename Key_T>
struct HLItem_T : public HTableItem_T<Key_T> {
    /**
     * @brief Base table item type for code reuse.
     */
    using BaseT = HTableItem_T<Key_T>;
    using BaseT::BaseT;

    /**
     * @brief Moves and disposes of the key from another item.
     *
     * Used during hash table reorganization or item replacement.
     * Disposes of the key in the source item.
     *
     * @param item The source item to move/dispose from.
     */
    QENTEM_INLINE void MoveDoublecat(HLItem_T &item) {
        Memory::Dispose(&(item.Key));
    }

    /**
     * @brief No-op copy for key-only items (required for interface).
     *
     * @param Unused Source item.
     */
    constexpr void CopyValue(HLItem_T const &) {
    }

    /**
     * @brief No-op initialization for key-only items (required for interface).
     */
    constexpr void InitValue() {
    }
};

/**
 * @brief Ordered, unique key list container (HList) for Qentem Engine.
 *
 * HList is an associative container that maintains a unique, ordered list of keys
 * (with no associated values). Useful for tracking the presence and order of string
 * keys, such as in sets, indexes, or metadata fields. Inherits hashing and storage
 * behavior from StringHashTable.
 *
 * @tparam Key_T The key type (must provide .First(), .Length(), and equality operators).
 */
template <typename Key_T>
struct HList : public StringHashTable<Key_T, HLItem_T<Key_T>> {
    /**
     * @brief Hash table item type storing only the key.
     */
    using HItem = HLItem_T<Key_T>;

    /**
     * @brief The parent type (string-adapted hash table for keys).
     */
    using BaseT = StringHashTable<Key_T, HItem>;

    /**
     * @brief Character type for the key.
     */
    using Char_T = typename Key_T::CharType;

    /**
     * @brief Inherit constructors from BaseT.
     */
    using BaseT::BaseT;
};

} // namespace Qentem

#endif
