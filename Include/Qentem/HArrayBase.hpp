/**
 * @file HArrayBase.hpp
 * @brief Core ordered hash-array container for Qentem Engine.
 *
 * HArrayBase is a contiguous associative container that combines the ordered
 * traversal of an array with hash-based key lookup. Elements are stored in
 * insertion order within a single memory block while supporting efficient
 * access by both index and key.
 *
 * The container uses an integrated hash structure that preserves ordering
 * without requiring separate storage for iteration. Capacity growth is
 * controlled at compile time and memory management is delegated to a
 * configurable backend.
 *
 * Key features:
 *  - Preserves insertion order.
 *  - Contiguous storage for cache-friendly iteration.
 *  - Supports both index-based and key-based access.
 *  - Pluggable memory backend through a provider interface.
 *  - Compile-time growth policy for predictable expansion behavior.
 *  - Stores elements and hash metadata within a unified structure.
 *  - Rebuilds hash state when resized, discarding deleted entries.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright license MIT
 */

#ifndef QENTEM_H_ARRAY_BASE_H
#define QENTEM_H_ARRAY_BASE_H

#include "Qentem/HashTableAdapters.hpp"

namespace Qentem {

/**
 * @brief Key-value item used by HArray containers.
 *
 * HAItem_T extends HTableItem_T with a value member, providing the
 * storage representation used by ordered hash-array containers.
 *
 * @tparam Key_T
 *         The key type.
 * @tparam Number_T
 *         Numeric type used for capacities, indexes,
 *         hash values, and internal bookkeeping.
 * @tparam Value_T
 *         The value type associated with the key.
 */
template <typename Key_T, typename Number_T, typename Value_T>
struct HAItem_T : public HTableItem_T<Key_T, Number_T> {
    /**
     * @brief Base item type for chaining and key management.
     */
    using BaseT = HTableItem_T<Key_T, Number_T>;
    using BaseT::BaseT;
    using BaseT::Hash;
    using BaseT::Key;

    /**
     * @brief The value associated with this key.
     */
    Value_T Value{};

    /**
     * @brief Clears both the key and value, resetting the item.
     *
     * Sets key and value to their default-constructed states.
     */
    QENTEM_INLINE void Clear() noexcept {
        Key   = Key_T{};
        Value = Value_T{};
    }

    /**
     * @brief Moves the value from another item, and disposes of its key.
     *
     * Used during item replacement or collision resolution.
     *
     * @param item Source item to move from.
     */
    QENTEM_INLINE void MoveDoublecat(HAItem_T &item) noexcept {
        Value = QUtility::Move(item.Value);
        MemoryUtils::Destruct(&(item.Key));
    }

    /**
     * @brief Copies the value from another item.
     *
     * @param item Source item to copy from.
     */
    QENTEM_INLINE void CopyValue(HAItem_T const &item) noexcept {
        Value = item.Value;
    }

    /**
     * @brief Initializes key, value, and hash fields in-place.
     *
     * This method intentionally leaves linkage fields (`Position`, `Next`)
     * untouched to preserve chain integrity during item insertion or move.
     *
     * Should only be used when writing to uninitialized or cleared item slots.
     */
    QENTEM_INLINE void Construct(const HAItem_T &item) noexcept {
        Hash = item.Hash;
        MemoryUtils::Construct(&Key, item.Key);
        MemoryUtils::Construct(&Value, item.Value);
    }

    QENTEM_INLINE void Construct(HAItem_T &&item) noexcept {
        Hash = item.Hash;
        MemoryUtils::Construct(&Key, QUtility::Move(item.Key));
        MemoryUtils::Construct(&Value, QUtility::Move(item.Value));
    }

    /**
     * @brief Initializes the value member in uninitialized storage.
     *
     * Uses placement-new or custom Qentem initialization logic.
     */
    QENTEM_INLINE void InitValue() noexcept {
        MemoryUtils::Construct(&Value);
    }
};

/**
 * @brief Ordered associative array with contiguous storage and hash-based lookup.
 *
 * HArrayBase combines the ordered traversal of an array with the fast lookup
 * characteristics of a hash table. Elements are stored in insertion order
 * within a single contiguous memory block while supporting efficient access
 * by both index and key.
 *
 * The container uses a configurable numeric type for capacities, indexes,
 * hash values, and internal bookkeeping, allowing its storage limits and
 * hash width to be selected independently of SizeT. This enables support
 * for larger containers, pointer-sized identifiers, and application-specific
 * numeric key types without changing the underlying implementation.
 *
 * Capacity growth is controlled at compile time through an expansion
 * multiplier, providing predictable reallocation behavior with no runtime
 * overhead. Memory management is delegated to a configurable backend.
 *
 * @tparam Key_T
 *         The key type.
 * @tparam Number_T
 *         Numeric type used for capacities, indexes,
 *         hash values, and internal bookkeeping.
 * @tparam Value_T
 *         The value type associated with each key.
 * @tparam Expansion_Multiplier_T
 *         Compile-time capacity growth factor used during reallocation.
 * @tparam MemoryProvider_T
 *         Memory backend used for storage management.
 */
template <typename Key_T, typename Number_T, typename Value_T, Number_T Expansion_Multiplier_T,
          typename MemoryProvider_T>
struct HArrayBase : public AutoHashTable<Key_T, Number_T, HAItem_T<Key_T, Number_T, Value_T>, Expansion_Multiplier_T,
                                         MemoryProvider_T> {
    using NumberT = Number_T;

    /**
     * @brief Hash table item type storing key-value pairs.
     */
    using HItem = HAItem_T<Key_T, Number_T, Value_T>;

    /**
     * @brief The parent type (string-adapted hash table).
     */
    using BaseT = AutoHashTable<Key_T, Number_T, HItem, Expansion_Multiplier_T, MemoryProvider_T>;

    /**
     * @brief Inherit constructors from BaseT.
     */
    using BaseT::BaseT;
    using BaseT::Capacity;
    using BaseT::find;
    using BaseT::First;
    using BaseT::Size;
    using BaseT::Storage;
    using BaseT::tryInsert;

    QENTEM_INLINE HArrayBase() noexcept = default;
    /**
     * @brief Gets (or inserts) a value by key object.
     *
     * Looks up the value for the provided key object. If not present,
     * inserts a new entry with default value.
     *
     * @param key The key object.
     * @return Reference to the value.
     */
    QENTEM_INLINE Value_T &Get(const Key_T &key) {
        HItem *item = tryInsert(key);
        return item->Value;
    }

    /**
     * @brief Gets (or inserts) a value by moved key object.
     *
     * Moves the key into the container if not present.
     *
     * @param key The key object to move.
     * @return Reference to the value.
     */
    QENTEM_INLINE Value_T &Get(Key_T &&key) {
        HItem *item = tryInsert(QUtility::Move(key));
        return item->Value;
    }

    /**
     * @brief Array subscript operator by moved key object.
     *
     * Moves the key into the container if not present.
     *
     * @param key The key object to move.
     * @return Reference to the value.
     */
    QENTEM_INLINE Value_T &operator[](Key_T &&key) {
        HItem *item = tryInsert(QUtility::Move(key));
        return item->Value;
    }

    /**
     * @brief Array subscript operator by key object.
     *
     * Looks up or inserts a value for the given key.
     *
     * @param key The key object.
     * @return Reference to the value.
     */
    QENTEM_INLINE Value_T &operator[](const Key_T &key) {
        HItem *item = tryInsert(key);
        return item->Value;
    }

    /**
     * @brief Inserts or updates a value by key object (copy).
     *
     * If the key exists, assigns the value. If not, inserts a new item.
     *
     * @param key   The key object.
     * @param value Value to copy into the entry.
     */
    QENTEM_INLINE void Insert(const Key_T &key, const Value_T &value) {
        HItem *item = tryInsert(key);
        item->Value = Value_T{value};
    }

    /**
     * @brief Inserts or updates a value by key object (move value).
     *
     * If the key exists, assigns the value. If not, inserts a new item.
     *
     * @param key   The key object.
     * @param value Value to move into the entry.
     */
    QENTEM_INLINE void Insert(const Key_T &key, Value_T &&value) {
        HItem *item = tryInsert(key);
        item->Value = QUtility::Move(value);
    }

    /**
     * @brief Inserts or updates a value by moved key (copy value).
     *
     * If the key exists, assigns the value. If not, inserts a new item.
     *
     * @param key   The key object to move.
     * @param value Value to copy into the entry.
     */
    QENTEM_INLINE void Insert(Key_T &&key, const Value_T &value) {
        HItem *item = tryInsert(QUtility::Move(key));
        item->Value = Value_T{value};
    }

    /**
     * @brief Inserts or updates a value by moved key (move value).
     *
     * If the key exists, assigns the value. If not, inserts a new item.
     *
     * @param key   The key object to move.
     * @param value Value to move into the entry.
     */
    QENTEM_INLINE void Insert(Key_T &&key, Value_T &&value) {
        HItem *item = tryInsert(QUtility::Move(key));
        item->Value = QUtility::Move(value);
    }

    /**
     * @brief Gets a pointer to the value for a given key object.
     *
     * Returns nullptr if the key is not found.
     *
     * @param key The key object.
     * @return Pointer to the value, or nullptr if not found.
     */
    QENTEM_INLINE Value_T *GetValue(const Key_T &key) noexcept {
        if (Size() != 0) {
            NumberT *index;
            HItem   *item = find(index, key);

            if (item != nullptr) {
                return &(item->Value);
            }
        }

        return nullptr;
    }

    /**
     * @brief Gets a pointer to the value for a given key object using a precomputed hash.
     *
     * Returns nullptr if the key is not found.
     *
     * @note StringUtils::Hash() is constexpr and can compute the hash at compile time.
     *       If the key is constant, using this overload is recommended to avoid
     *       recomputing the hash at runtime.
     *
     * @param key The key object.
     * @param hash The precomputed hash value of the key.
     * @return Pointer to the value, or nullptr if not found.
     */
    QENTEM_INLINE Value_T *GetValue(const Key_T &key, const NumberT hash) noexcept {
        if (Size() != 0) {
            NumberT *index;
            HItem   *item = find(index, key, hash);

            if (item != nullptr) {
                return &(item->Value);
            }
        }

        return nullptr;
    }

    /**
     * @brief Gets a const pointer to the value for a given key object.
     *
     * Returns nullptr if the key is not found.
     *
     * @param key The key object.
     * @return Const pointer to the value, or nullptr if not found.
     */
    QENTEM_INLINE const Value_T *GetValue(const Key_T &key) const noexcept {
        if (Size() != 0) {
            NumberT      index;
            const HItem *item = find(index, key);

            if (item != nullptr) {
                return &(item->Value);
            }
        }

        return nullptr;
    }

    /**
     * @brief Gets a const pointer to the value for a given key object using a precomputed hash.
     *
     * Returns nullptr if the key is not found.
     *
     * @note StringUtils::Hash() is constexpr and can compute the hash at compile time.
     *       If the key is constant, using this overload is recommended to avoid
     *       recomputing the hash at runtime.
     *
     * @param key The key object.
     * @param hash The precomputed hash value of the key.
     * @return Const pointer to the value, or nullptr if not found.
     */
    QENTEM_INLINE const Value_T *GetValue(const Key_T &key, const NumberT hash) const noexcept {
        if (Size() != 0) {
            NumberT      index;
            const HItem *item = find(index, key, hash);

            if (item != nullptr) {
                return &(item->Value);
            }
        }

        return nullptr;
    }

    /**
     * @brief Gets a pointer to the value at the specified index.
     *
     * Index refers to the ordered position in the items array, not the hash table bucket.
     * Returns nullptr if index is out of bounds or the entry is empty.
     *
     * @param index The item index.
     * @return Pointer to the value, or nullptr if not found.
     */
    QENTEM_INLINE Value_T *GetValueAt(const NumberT index) noexcept {
        HItem *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Value);
        }

        return nullptr;
    }

    /**
     * @brief Gets a const pointer to the value at the specified index.
     *
     * Index refers to the ordered position in the items array, not the hash table bucket.
     * Returns nullptr if index is out of bounds or the entry is empty.
     *
     * @param index The item index.
     * @return Const pointer to the value, or nullptr if not found.
     */
    QENTEM_INLINE const Value_T *GetValueAt(const NumberT index) const noexcept {
        const HItem *src = First();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Value);
        }

        return nullptr;
    }
};

/**
 * @brief String-oriented ordered hash-array container.
 *
 * HArrayStrings extends HArrayBase with convenience overloads and lookup
 * operations optimized for string-like keys. It preserves insertion order,
 * provides both hash-style and array-style access, and supports lookups
 * using key objects or raw character sequences without requiring temporary
 * key construction.
 *
 * The container inherits the storage layout, hashing infrastructure, and
 * growth behavior of HArrayBase while adding string-specific insertion,
 * retrieval, and comparison helpers.
 *
 * @tparam Key_T
 *         String key type. Must provide CharType, First(), Length(),
 *         and IsEqual().
 * @tparam Number_T
 *         Numeric type used for capacities, indexes,
 *         hash values, and internal bookkeeping.
 * @tparam Value_T
 *         The value type associated with each key.
 * @tparam Expansion_Multiplier_T
 *         Compile-time capacity growth factor used during reallocation.
 * @tparam MemoryProvider_T
 *         Memory backend used for storage management.
 */
template <typename Key_T, typename Number_T, typename Value_T, Number_T Expansion_Multiplier_T,
          typename MemoryProvider_T>
struct HArrayStrings : public HArrayBase<Key_T, Number_T, Value_T, Expansion_Multiplier_T, MemoryProvider_T> {
    using NumberT = Number_T;

    /**
     * @brief Hash table item type storing key-value pairs.
     */
    using HItem = HAItem_T<Key_T, Number_T, Value_T>;

    /**
     * @brief The parent type (string-adapted hash table).
     */
    using BaseT = HArrayBase<Key_T, Number_T, Value_T, Expansion_Multiplier_T, MemoryProvider_T>;
    /**
     * @brief Character type for the key.
     */
    using Char_T = typename Key_T::CharType;

    /**
     * @brief Inherit constructors from BaseT.
     */
    using BaseT::BaseT;

    using BaseT::Get;
    using BaseT::GetValue;
    using BaseT::Insert;
    using BaseT::operator[];
    using BaseT::Capacity;
    using BaseT::First;
    using BaseT::Size;
    using BaseT::Storage;

    using BaseT::find;
    using BaseT::tryInsert;

    QENTEM_INLINE HArrayStrings() noexcept = default;

    /**
     * @brief Gets (or inserts) a value by raw character key and length.
     *
     * Looks up the value associated with the given key. If the key is not found,
     * a new entry is inserted with a default-constructed value, and a reference is returned.
     *
     * @param str    Pointer to the character array key.
     * @param length Number of characters in the key.
     * @return Reference to the value associated with the key.
     */
    QENTEM_INLINE Value_T &Get(const Char_T *str, const NumberT length) {
        HItem *item = tryInsert(str, length);
        return item->Value;
    }

    /**
     * @brief Array subscript operator by character key (null-terminated).
     *
     * Looks up or inserts a value for the given C-string key.
     * Equivalent to Get(str, StringUtils::Count(str)).
     * If the key does not exist, a new entry is inserted with a default-constructed value.
     *
     * @param str Pointer to null-terminated character array.
     * @return Reference to the value associated with the key.
     */
    QENTEM_INLINE Value_T &operator[](const Char_T *str) {
        return Get(str, StringUtils::Count(str));
    }

    /**
     * @brief Inserts or updates a value by raw character key.
     *
     * If the key exists, assigns (overwrites) the value. If not, inserts a new item.
     * The value is moved into the table entry.
     *
     * @param str    Pointer to character array key.
     * @param length Number of characters in the key.
     * @param value  Value to move into the entry.
     */
    QENTEM_INLINE void Insert(const Char_T *str, const NumberT length, Value_T &&value) {
        HItem *item = tryInsert(str, length); // Insert new or find existing entry by key
        item->Value = QUtility::Move(value);  // Move-assign value into the entry
    }

    /**
     * @brief Looks up the value associated with the given key string, length, and precomputed hash.
     *
     * Performs a lookup using the provided character key, length, and hash value.
     * Returns nullptr if the key is not found or if the table is empty.
     *
     * @param str    Pointer to the character array key.
     * @param length Length of the key string.
     * @param hash   Precomputed hash value for the key.
     * @return Pointer to the associated value if found; nullptr if not found or table is empty.
     */
    QENTEM_INLINE Value_T *GetValue(const Char_T *str, const NumberT length, const NumberT hash) noexcept {
        if (Size() != 0) {
            NumberT *index;
            HItem   *item = find(index, str, length, hash);

            if (item != nullptr) {
                return &(item->Value);
            }
        }

        return nullptr;
    }

    /**
     * @brief Looks up the value associated with the given key string, length, and precomputed hash.
     *
     * Performs a lookup using the provided character key, length, and hash value.
     * Returns nullptr if the key is not found or if the table is empty.
     *
     * @param str   Pointer to the character array key.
     * @param length Length of the key string.
     * @param hash  Precomputed hash value for the key.
     * @return Const pointer to the associated value if found, nullptr if not found or table is empty.
     */
    QENTEM_INLINE const Value_T *GetValue(const Char_T *str, const NumberT length, const NumberT hash) const noexcept {
        // Only attempt lookup if the table contains at least one item
        if (Size() != 0) {
            // Find the index of the entry matching the key and hash
            NumberT      index;
            const HItem *item = find(index, str, length, hash);

            // If a valid index was found (entry exists)
            if (item != nullptr) {
                // Return a pointer to the associated value
                return &(item->Value);
            }
        }

        // Return nullptr if not found or table is empty
        return nullptr;
    }

    /**
     * @brief Gets a pointer to the value for a given raw key and length.
     *
     * Computes the hash of the key internally and performs a lookup.
     * Returns nullptr if the key is not found.
     *
     * @param str    Pointer to the character array key.
     * @param length Number of characters in the key.
     * @return Pointer to the value if found; nullptr if not found.
     */
    QENTEM_INLINE Value_T *GetValue(const Char_T *str, const NumberT length) noexcept {
        return GetValue(str, length, StringUtils::Hash<Char_T, NumberT>(str, length));
    }

    /**
     * @brief Gets a const pointer to the value for a given raw key and length.
     *
     * Computes the hash of the key internally and performs a lookup.
     * Returns nullptr if the key is not found.
     *
     * @param str    Pointer to the character array key.
     * @param length Number of characters in the key.
     * @return Const pointer to the value if found; nullptr if not found.
     */
    QENTEM_INLINE const Value_T *GetValue(const Char_T *str, const NumberT length) const noexcept {
        return GetValue(str, length, StringUtils::Hash<Char_T, NumberT>(str, length));
    }
};

/**
 * @brief Type selector for HArray containers.
 *
 * HArraySelector chooses the appropriate HArray implementation based on the
 * key type while preserving the configured numeric storage width.
 *
 * If Key_T is a numeric type, HArrayBase is selected to provide optimized
 * numeric hashing and lookup operations. Otherwise, HArrayStrings is selected,
 * enabling string-oriented lookup and raw string overloads.
 *
 * Number_T defines the numeric type used internally for capacities, indexes,
 * hash values, and related bookkeeping. This allows HArray instances to be
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
 * @tparam Value_T
 *         The value type.
 * @tparam Expansion_Multiplier_T
 *         Compile-time capacity growth factor propagated to the selected
 *         implementation.
 * @tparam MemoryProvider_T
 *         Memory backend used by the selected container implementation.
 * @tparam IsNum
 *         Implementation detail indicating whether Key_T is numeric.
 */
template <typename Key_T, typename Number_T, typename Value_T, Number_T Expansion_Multiplier_T,
          typename MemoryProvider_T, bool = QTraits::IsNumber<Key_T>::value>
struct HArraySelector;

// Specialization for non-number keys: string-oriented version.
template <typename Key_T, typename Number_T, typename Value_T, Number_T Expansion_Multiplier_T,
          typename MemoryProvider_T>
struct HArraySelector<Key_T, Number_T, Value_T, Expansion_Multiplier_T, MemoryProvider_T, false> {
    using Type = HArrayStrings<Key_T, Number_T, Value_T, Expansion_Multiplier_T, MemoryProvider_T>;
};

// Specialization for number keys: base version (numeric optimized).
template <typename Key_T, typename Number_T, typename Value_T, Number_T Expansion_Multiplier_T,
          typename MemoryProvider_T>
struct HArraySelector<Key_T, Number_T, Value_T, Expansion_Multiplier_T, MemoryProvider_T, true> {
    using Type = HArrayBase<Key_T, Number_T, Value_T, Expansion_Multiplier_T, MemoryProvider_T>;
};

} // namespace Qentem

#endif
