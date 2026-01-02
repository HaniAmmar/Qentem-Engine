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
 * @date 2026
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
    using BaseT::Hash;
    using BaseT::Key;

    /**
     * @brief Clears both the key and value, resetting the item.
     *
     * Sets key and value to their default-constructed states.
     */
    QENTEM_INLINE void Clear() {
        Key = Key_T{};
    }

    /**
     * @brief Moves and disposes of the key from another item.
     *
     * Used during hash table reorganization or item replacement.
     * Disposes of the key in the source item.
     *
     * @param item The source item to move/dispose from.
     */
    QENTEM_INLINE void MoveDoublecat(HLItem_T &item) {
        MemoryUtils::Destruct(&(item.Key));
    }

    /**
     * @brief No-op copy for key-only items (required for interface).
     *
     * @param Unused Source item.
     */
    QENTEM_INLINE void CopyValue(HLItem_T const &) const {
    }

    /**
     * @brief Initializes key, and hash fields in-place.
     *
     * This method intentionally leaves linkage fields (`Position`, `Next`)
     * untouched to preserve chain integrity during item insertion or move.
     *
     * Should only be used when writing to uninitialized or cleared item slots.
     */
    QENTEM_INLINE void Construct(const HLItem_T &item) {
        Hash = item.Hash;
        MemoryUtils::Construct(&Key, item.Key);
    }

    QENTEM_INLINE void Construct(HLItem_T &&item) {
        Hash = item.Hash;
        MemoryUtils::Construct(&Key, QUtility::Move(item.Key));
    }

    /**
     * @brief No-op initialization for key-only items (required for interface).
     */
    QENTEM_INLINE void InitValue() const {
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
 * @tparam Expansion_Multiplier_T
 *                 Compile-time capacity growth factor used by the underlying
 *                 HArray implementation. Defaults to exponential growth.
 */
template <typename Key_T, SizeT Expansion_Multiplier_T = 2>
struct HList : public AutoHashTable<Key_T, HLItem_T<Key_T>, Expansion_Multiplier_T> {
    /**
     * @brief Hash table item type storing only the key.
     */
    using HItem = HLItem_T<Key_T>;

    /**
     * @brief The parent type (string-adapted hash table for keys).
     */
    using BaseT = AutoHashTable<Key_T, HItem, Expansion_Multiplier_T>;

    /**
     * @brief Inherit constructors from BaseT.
     */
    using BaseT::BaseT;
    using BaseT::tryInsert;

    /**
     * @brief Array subscript operator by moved key object.
     *
     * Moves the key into the container if not present.
     *
     * @param key The key object to move.
     */
    QENTEM_INLINE void operator[](Key_T &&key) {
        tryInsert(QUtility::Move(key));
    }

    /**
     * @brief Array subscript operator by key object.
     *
     * Looks up or inserts a value for the given key.
     *
     * @param key The key object.
     */
    QENTEM_INLINE void operator[](const Key_T &key) {
        tryInsert(key);
    }
};

} // namespace Qentem

#endif
