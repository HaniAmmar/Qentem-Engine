/**
 * @file HArray.hpp
 * @brief Defines the ordered hash-array container for Qentem Engine.
 *
 * A data structure that looks like an ordered array but behaves like a hash table.
 * Elements can be accessed both by their index (array-style) and by key (hash-style).
 *
 * - Preserves insertion order and uses a single contiguous memory block.
 * - No extra allocations on collisions: the initial size determines capacity.
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
        Value = Memory::Move(item.Value);
        Memory::Dispose(&(item.Key));
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
        Memory::Initialize(&Value);
    }
};

/**
 * @brief Ordered associative array (hash table) for Qentem Engine.
 *
 * HArray combines the features of an ordered array and a hash table. It preserves
 * insertion order and provides fast lookups by key, supporting both array-style
 * access by index and hash-style access by key. All storage is contiguous, and
 * capacity is managed internally with minimal allocations.
 *
 * This container is ideal for use cases requiring direct lookup and fast iteration,
 * such as symbol tables, configuration data, or high-performance scripting engines.
 *
 * @tparam Key_T   The key type (must provide .First() and .Length()).
 * @tparam Value_T The value type to store for each key.
 */
template <typename Key_T, typename Value_T>
struct HArray : public StringHashTable<Key_T, HAItem_T<Key_T, Value_T>> {
    /**
     * @brief Hash table item type storing key-value pairs.
     */
    using HItem = HAItem_T<Key_T, Value_T>;

    /**
     * @brief The parent type (string-adapted hash table).
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

    using BaseT::find;
    using BaseT::Size;
    using BaseT::Storage;
    using BaseT::tryInsert;

    /**
     * @brief Gets (or inserts) a value by raw character key and length.
     *
     * Looks up the value associated with the given key; if not found,
     * a new entry is inserted with default value.
     *
     * @param key    Pointer to the character array key.
     * @param length Number of characters in the key.
     * @return Reference to the value.
     */
    inline Value_T &Get(const Char_T *key, const SizeT length) {
        HItem *item = tryInsert(key, length);
        return item->Value;
    }

    /**
     * @brief Array subscript operator by character key (null-terminated).
     *
     * Looks up or inserts a value for the given C-string key.
     * Equivalent to Get(key, StringUtils::Count(key)).
     *
     * @param key Pointer to null-terminated character array.
     * @return Reference to the value.
     */
    inline Value_T &operator[](const Char_T *key) {
        return Get(key, StringUtils::Count(key));
    }

    /**
     * @brief Gets (or inserts) a value by key object.
     *
     * Looks up the value for the provided key object. If not present,
     * inserts a new entry with default value.
     *
     * @param key The key object.
     * @return Reference to the value.
     */
    inline Value_T &Get(const Key_T &key) {
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
    inline Value_T &Get(Key_T &&key) {
        HItem *item = tryInsert(Memory::Move(key));
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
    inline Value_T &operator[](Key_T &&key) {
        HItem *item = tryInsert(Memory::Move(key));
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
    inline Value_T &operator[](const Key_T &key) {
        return Get(key);
    }

    /**
     * @brief Inserts or updates a value by raw character key.
     *
     * If the key exists, assigns the value. If not, inserts a new item.
     *
     * @param key    Pointer to character array key.
     * @param length Number of characters in the key.
     * @param value  Value to move into the entry.
     */
    inline void Insert(const Char_T *key, const SizeT length, Value_T &&value) {
        HItem *item = tryInsert(key, length);
        item->Value = Memory::Move(value);
    }

    /**
     * @brief Inserts or updates a value by key object (copy).
     *
     * If the key exists, assigns the value. If not, inserts a new item.
     *
     * @param key   The key object.
     * @param value Value to copy into the entry.
     */
    inline void Insert(const Key_T &key, const Value_T &value) {
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
    inline void Insert(const Key_T &key, Value_T &&value) {
        HItem *item = tryInsert(key);
        item->Value = Memory::Move(value);
    }

    /**
     * @brief Inserts or updates a value by moved key (copy value).
     *
     * If the key exists, assigns the value. If not, inserts a new item.
     *
     * @param key   The key object to move.
     * @param value Value to copy into the entry.
     */
    inline void Insert(Key_T &&key, const Value_T &value) {
        HItem *item = tryInsert(Memory::Move(key));
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
    inline void Insert(Key_T &&key, Value_T &&value) {
        HItem *item = tryInsert(Memory::Move(key));
        item->Value = Memory::Move(value);
    }

    /**
     * @brief Gets a pointer to the value for a given raw key, length, and precomputed hash.
     *
     * Performs a lookup using the provided key, length, and hash value.
     * Returns nullptr if the key is not found.
     *
     * @param key    Pointer to the character array key.
     * @param length Number of characters in the key.
     * @param hash   Precomputed hash for the key.
     * @return Pointer to the value, or nullptr if not found.
     */
    inline Value_T *GetValue(const Char_T *key, const SizeT length, const SizeT hash) const noexcept {
        if (Size() != 0) {
            SizeT *index;
            HItem *item = find(index, key, length, hash);

            if (item != nullptr) {
                return &(item->Value);
            }
        }
        return nullptr;
    }

    /**
     * @brief Gets a pointer to the value for a given raw key and length.
     *
     * Computes the hash internally and performs a lookup.
     * Returns nullptr if the key is not found.
     *
     * @param key    Pointer to the character array key.
     * @param length Number of characters in the key.
     * @return Pointer to the value, or nullptr if not found.
     */
    inline Value_T *GetValue(const Char_T *key, const SizeT length) const noexcept {
        return GetValue(key, length, StringUtils::Hash(key, length));
    }

    /**
     * @brief Gets a pointer to the value for a given key object.
     *
     * Returns nullptr if the key is not found.
     *
     * @param key The key object.
     * @return Pointer to the value, or nullptr if not found.
     */
    inline Value_T *GetValue(const Key_T &key) const noexcept {
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
     * @brief Gets a pointer to the value at the specified index.
     *
     * Index refers to the ordered position in the items array, not the hash table bucket.
     * Returns nullptr if index is out of bounds or the entry is empty.
     *
     * @param index The item index.
     * @return Pointer to the value, or nullptr if not found.
     */
    inline Value_T *GetValue(const SizeT index) const noexcept {
        HItem *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Value);
        }
        return nullptr;
    }
};

} // namespace Qentem

#endif
