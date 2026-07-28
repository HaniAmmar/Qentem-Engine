/**
 * @file HListBase.hpp
 * @brief Core ordered key container for Qentem Engine.
 *
 * HListBase is an associative container that stores unique keys while
 * preserving insertion order. It combines efficient hash-based lookup
 * with ordered traversal, making it suitable for sets, indexes, and
 * membership tracking where values are not required.
 *
 * The container uses an integrated hash structure that maintains key
 * ordering without requiring separate storage for iteration. Capacity
 * growth is controlled at compile time and memory management is delegated
 * to a configurable backend.
 *
 * Key features:
 *  - Preserves insertion order.
 *  - Efficient hash-based lookup.
 *  - Stores only keys with no associated values.
 *  - Pluggable memory backend through a provider interface.
 *  - Compile-time growth policy for predictable expansion behavior.
 *  - Stores keys and hash metadata within a unified structure.
 *  - Rebuilds hash state when resized, discarding deleted entries.
 *
 * @copyright Copyright (c) 2026 Hani Ammar
 * @license MIT
 */

#ifndef QENTEM_H_LIST_BASE_H
#define QENTEM_H_LIST_BASE_H

#include "Qentem/HashTableAdapters.hpp"

namespace Qentem {

/**
 * @brief Key-only storage item used by HList containers.
 *
 * HLItem_T extends HTableItem_T without adding additional data members,
 * providing the storage representation used by key-only associative
 * containers. Each item stores a key, hash value, and collision-chain
 * link while supporting efficient copy and move operations.
 *
 * @tparam Key_T
 *         The key type.
 * @tparam Number_T
 *         Numeric type used for capacities, indexes,
 *         hash values, and internal bookkeeping.
 */
template <typename Key_T, typename Number_T>
struct HLItem_T : public HTableItem_T<Key_T, Number_T> {
    /**
     * @brief Base table item type for code reuse.
     */
    using BaseT = HTableItem_T<Key_T, Number_T>;
    using BaseT::BaseT;

    using BaseT::Hash;
    using BaseT::Key;

    /**
     * @brief Resets the key, clearing the item.
     *
     * Sets the key to its default-constructed state.
     */
    QENTEM_INLINE void Clear() {
        Key = Key_T{};
    }

    /**
     * @brief Destroys the key stored in another item.
     *
     * Used during hash table reorganization and item replacement
     * when ownership has already been transferred elsewhere.
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
     * @brief Initializes the key and hash fields in-place.
     *
     * Linkage metadata is intentionally left unchanged to preserve
     * hash-chain state during insertion and relocation.
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
 * @brief Ordered key container for unique keys.
 *
 * HListBase stores unique keys while preserving insertion order and
 * providing efficient hash-based lookup. It combines the ordered
 * traversal of a list with the lookup characteristics of a hash table,
 * making it suitable for sets, indexes, and membership tracking.
 *
 * The container uses Number_T for capacities, indexes, hash values,
 * and internal bookkeeping, allowing its numeric width to be configured
 * independently of SizeT.
 *
 * @tparam Key_T
 *         The key type.
 * @tparam Number_T
 *         Numeric type used for capacities, indexes,
 *         hash values, and internal bookkeeping.
 * @tparam Expansion_Multiplier_T
 *         Compile-time capacity growth factor.
 * @tparam MemoryProvider_T
 *         Memory backend used for storage management.
 */
template <typename Key_T, typename Number_T, Number_T Expansion_Multiplier_T, typename MemoryProvider_T>
struct HListBase
    : public AutoHashTable<Key_T, Number_T, HLItem_T<Key_T, Number_T>, Expansion_Multiplier_T, MemoryProvider_T> {
    /**
     * @brief Hash table item type storing only the key.
     */
    using HItem = HLItem_T<Key_T, Number_T>;

    /**
     * @brief The parent type (string-adapted hash table for keys).
     */
    using BaseT = AutoHashTable<Key_T, Number_T, HItem, Expansion_Multiplier_T, MemoryProvider_T>;

    /**
     * @brief Inherit constructors from BaseT.
     */
    using BaseT::BaseT;
    using BaseT::Has;
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

/**
 * @brief String-oriented ordered key container.
 *
 * HListStrings extends HListBase with convenience overloads for
 * string-like keys and raw character sequences. It allows efficient
 * lookup and insertion without requiring temporary key construction.
 *
 * The container preserves insertion order while providing hash-based
 * lookup and inherits its storage, growth policy, and memory backend
 * from HListBase.
 *
 * @tparam Key_T
 *         String key type. Must provide CharType, First(), Length(),
 *         and IsEqual().
 * @tparam Number_T
 *         Numeric type used for capacities, indexes,
 *         hash values, and internal bookkeeping.
 * @tparam Expansion_Multiplier_T
 *         Compile-time capacity growth factor.
 * @tparam MemoryProvider_T
 *         Memory backend used for storage management.
 */
template <typename Key_T, typename Number_T, Number_T Expansion_Multiplier_T, typename MemoryProvider_T>
struct HListStrings : public HListBase<Key_T, Number_T, Expansion_Multiplier_T, MemoryProvider_T> {
    using NumberT = Number_T;

    /****
     * @brief Hash table item type storing keys.
     */
    using HItem = HLItem_T<Key_T, Number_T>;

    /**
     * @brief Base ordered key container type.
     */
    using BaseT = HListBase<Key_T, Number_T, Expansion_Multiplier_T, MemoryProvider_T>;
    /**
     * @brief Character type for the key.
     */
    using Char_T = typename Key_T::CharType;

    /**
     * @brief Inherit constructors from BaseT.
     */
    using BaseT::BaseT;

    using BaseT::Has;
    using BaseT::Insert;
    using BaseT::operator[];
    using BaseT::Capacity;
    using BaseT::First;
    using BaseT::Size;
    using BaseT::Storage;

    using BaseT::tryInsert;

    QENTEM_INLINE HListStrings() noexcept = default;

    /**
     * @brief Inserts a key from a raw character sequence.
     *
     * If the key already exists, no new item is created.
     *
     * @param str
     *         Pointer to the character sequence.
     * @param length
     *         Number of characters in the key.
     */
    QENTEM_INLINE void Insert(const Char_T *str, const NumberT length) {
        HItem *item = tryInsert(str, length); // Insert new or find existing entry by key
    }
};

/**
 * @brief Type selector for HList containers.
 *
 * HListSelector chooses the appropriate HList implementation based on the
 * key type while preserving the configured numeric storage width.
 *
 * If Key_T is a numeric type, HArrayBase is selected to provide optimized
 * numeric hashing and lookup operations. Otherwise, HArrayStrings is selected,
 * enabling string-oriented lookup and raw string overloads.
 *
 * Number_T defines the numeric type used internally for capacities, indexes,
 * hash values, and related bookkeeping. This allows HList instances to be
 * configured independently of SizeT, enabling wider index ranges, larger hash
 * values, pointer-sized keys, or application-specific numeric identifiers.
 *
 * The expansion multiplier is forwarded unchanged to the selected
 * implementation and remains part of the resulting container type.
 *
 * @tparam Key_T
 *         The key type.
 * @tparam Number_T
 *         Numeric type used for capacities, indexes,
 *         hash values, and internal bookkeeping.
 * @tparam Expansion_Multiplier_T
 *         Compile-time capacity growth factor propagated to the selected
 *         implementation.
 * @tparam MemoryProvider_T
 *         Memory backend used by the selected container implementation.
 * @tparam IsNum
 *         Implementation detail indicating whether Key_T is numeric.
 */
template <typename Key_T, typename Number_T, Number_T Expansion_Multiplier_T, typename MemoryProvider_T,
          bool = QTraits::IsNumber<Key_T>::value>
struct HListSelector;

// Specialization for non-number keys: string-oriented version.
template <typename Key_T, typename Number_T, Number_T Expansion_Multiplier_T, typename MemoryProvider_T>
struct HListSelector<Key_T, Number_T, Expansion_Multiplier_T, MemoryProvider_T, false> {
    using Type = HListStrings<Key_T, Number_T, Expansion_Multiplier_T, MemoryProvider_T>;
};

// Specialization for number keys: base version (numeric optimized).
template <typename Key_T, typename Number_T, Number_T Expansion_Multiplier_T, typename MemoryProvider_T>
struct HListSelector<Key_T, Number_T, Expansion_Multiplier_T, MemoryProvider_T, true> {
    using Type = HListBase<Key_T, Number_T, Expansion_Multiplier_T, MemoryProvider_T>;
};

} // namespace Qentem

#endif
