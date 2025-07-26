/**
 * @file HArray.hpp
 * @brief Defines the ordered hash-array container for Qentem Engine.
 *
 * A data structure that looks like an ordered array but behaves like a hash table.
 * Elements can be accessed both by their index (array-style) and by key (hash-style).
 *
 * - Preserves insertion order and uses a single contiguous memory block.
 * - No extra reserves on collisions: the initial size determines capacity.
 * - When resized, deleted items are dropped and the hash base is rebuilt/reset.
 *
 * Ideal for scenarios needing fast iteration and direct lookup, combining the
 * best aspects of arrays and hash tables.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_HARRAY_H
#define QENTEM_HARRAY_H

#include "HashTableAdapters.hpp"

namespace Qentem {

/**
 * @brief Hash table item for key-value associative containers (HArray).
 *
 * Extends the base item type with a value member, supporting assignment,
 * move semantics, and initialization. Used as the primary storage for
 * Qentem's HArray container, providing both key and value.
 *
 * @tparam Key_T   The key type.
 * @tparam Value_T The value type.
 */
template <typename Key_T, typename Value_T>
struct HAItem_T : public HTableItem_T<Key_T> {
    /**
     * @brief Base item type for chaining and key management.
     */
    using BaseT = HTableItem_T<Key_T>;
    using BaseT::BaseT;
    using BaseT::Key;

    /**
     * @brief The value associated with this key.
     */
    Value_T Value;

    /**
     * @brief Clears both the key and value, resetting the item.
     *
     * Sets key and value to their default-constructed states.
     */
    QENTEM_INLINE void Clear() {
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
    QENTEM_INLINE void MoveDoublecat(HAItem_T &item) {
        Value = QUtility::Move(item.Value);
        MemoryUtils::Dispose(&(item.Key));
    }

    /**
     * @brief Copies the value from another item.
     *
     * @param item Source item to copy from.
     */
    QENTEM_INLINE void CopyValue(HAItem_T const &item) {
        Value = item.Value;
    }

    /**
     * @brief Initializes the value member in uninitialized storage.
     *
     * Uses placement-new or custom Qentem initialization logic.
     */
    QENTEM_INLINE void InitValue() {
        MemoryUtils::Initialize(&Value);
    }
};

/**
 * @brief Ordered, type-adaptive associative array (hash table) for Qentem Engine.
 *
 * HArrayBase combines the features of an ordered array and a hash table, preserving
 * insertion order while enabling fast lookups by key. It adapts internally to the key type:
 * string keys use an efficient string hashing policy, and number keys use direct hashing,
 * with all storage provided in a single contiguous block and minimal memory.
 *
 * @tparam Key_T   The key type. Supports both string types and plain integral types.
 *                 The correct hash policy is selected automatically.
 * @tparam Value_T The value type to store for each key.
 */
template <typename Key_T, typename Value_T>
struct HArrayBase : public AutoHashTable<Key_T, HAItem_T<Key_T, Value_T>> {
    /**
     * @brief Hash table item type storing key-value pairs.
     */
    using HItem = HAItem_T<Key_T, Value_T>;

    /**
     * @brief The parent type (string-adapted hash table).
     */
    using BaseT = AutoHashTable<Key_T, HItem>;

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
            SizeT *index;
            HItem *item = find(index, key);

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
            SizeT        index;
            const HItem *item = find(index, key);

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
    QENTEM_INLINE Value_T *GetValueAt(const SizeT index) noexcept {
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
    QENTEM_INLINE const Value_T *GetValueAt(const SizeT index) const noexcept {
        const HItem *src = First();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Value);
        }

        return nullptr;
    }
};

/**
 * @brief Specialized ordered hash-array for string keys in Qentem Engine.
 *
 * HArrayStrings is a type adapter for string-like keys, building on HArrayBase
 * with policies and overloads optimized for string lookup. It preserves insertion
 * order, allows fast hash-style and array-style access, and provides convenience
 * overloads for C-string (null-terminated) keys as well as key objects.
 *
 * - Optimized for string keys: supports hashing and equality from character arrays.
 * - Provides operator[] and Get/Insert methods for raw strings and key objects.
 * - Automatically selected by HArray if the key type is recognized as a string type.
 *
 * @tparam Key_T   String key type (must provide CharType, .First(), .Length(), .IsEqual()).
 * @tparam Value_T The value type stored for each key.
 */
template <typename Key_T, typename Value_T>
struct HArrayStrings : public HArrayBase<Key_T, Value_T> {
    /**
     * @brief Hash table item type storing key-value pairs.
     */
    using HItem = HAItem_T<Key_T, Value_T>;

    /**
     * @brief The parent type (string-adapted hash table).
     */
    using BaseT = HArrayBase<Key_T, Value_T>;
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
    QENTEM_INLINE Value_T &Get(const Char_T *str, const SizeT length) {
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
    QENTEM_INLINE void Insert(const Char_T *str, const SizeT length, Value_T &&value) {
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
    QENTEM_INLINE Value_T *GetValue(const Char_T *str, const SizeT length, const SizeT hash) noexcept {
        if (Size() != 0) {
            SizeT *index;
            HItem *item = find(index, str, length, hash);

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
    QENTEM_INLINE const Value_T *GetValue(const Char_T *str, const SizeT length, const SizeT hash) const noexcept {
        // Only attempt lookup if the table contains at least one item
        if (Size() != 0) {
            // Find the index of the entry matching the key and hash
            SizeT        index;
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
    QENTEM_INLINE Value_T *GetValue(const Char_T *str, const SizeT length) noexcept {
        return GetValue(str, length, StringUtils::Hash(str, length));
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
    QENTEM_INLINE const Value_T *GetValue(const Char_T *str, const SizeT length) const noexcept {
        return GetValue(str, length, StringUtils::Hash(str, length));
    }
};

/**
 * @brief Type selector for HArray containers.
 *
 * HArraySelector chooses the correct HArray implementation based on the key type:
 *  - If Key_T is a number type (as determined by QTraits::IsNumber), use HArrayBase (fast numeric logic).
 *  - Otherwise, use HArrayStrings (enables raw string pointer overloads).
 *
 * @tparam Key_T   The key type.
 * @tparam Value_T The value type.
 * @tparam IsNum   (Implementation detail) Whether the key is a number.
 */
template <typename Key_T, typename Value_T, bool = QTraits::IsNumber<Key_T>::value>
struct HArraySelector;

// Specialization for non-number keys: string-oriented version.
template <typename Key_T, typename Value_T>
struct HArraySelector<Key_T, Value_T, false> {
    using Type = HArrayStrings<Key_T, Value_T>;
};

// Specialization for number keys: base version (numeric optimized).
template <typename Key_T, typename Value_T>
struct HArraySelector<Key_T, Value_T, true> {
    using Type = HArrayBase<Key_T, Value_T>;
};

/**
 * @brief Ordered associative array that adapts to string or number keys.
 *
 * HArray automatically chooses the most efficient implementation for the key type,
 * ensuring the best combination of performance and convenience, with a single API.
 *
 * @tparam Key_T   The key type (string or number).
 * @tparam Value_T The value type.
 */
template <typename Key_T, typename Value_T>
using HArray = typename HArraySelector<Key_T, Value_T>::Type;

} // namespace Qentem

#endif
