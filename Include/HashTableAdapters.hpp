/**
 * @file HashTableAdapters.hpp
 * @brief Provides key-type adapters for Qentem Engine hash tables.
 *
 * This header defines convenience wrappers for HashTable, allowing easy use with
 * string-like keys (via StringHashTable) and number keys (via NumberHashTable).
 * Each adapter exposes extra overloads and checks to ensure correct usage.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_HASH_TABLE_ADAPTERS_H
#define QENTEM_HASH_TABLE_ADAPTERS_H

#include "QTraits.hpp"
#include "HashTable.hpp"
#include "QNumber.hpp"
#include "StringUtils.hpp"
#include "StringView.hpp"

namespace Qentem {

/**
 * @brief Number key utilities for Qentem hash tables.
 *
 * Provides static methods for hashing and comparing numeric keys in the context
 * of hash table operations. Designed to be used as a KeyUtils policy for hash tables
 * that use integral (or other numeric) types as keys.
 *
 * @tparam Key_T Numeric key type (e.g., int, unsigned, etc.). Must support equality comparison.
 *
 * Example usage:
 *   using Table = HashTable<unsigned int, NumberKeyUtils_T<unsigned int>, ...>;
 */
template <typename Key_T>
struct NumberKeyUtils_T {
    /**
     * @brief Computes a non-zero hash value for numeric keys.
     *
     * For numeric types, this function ensures that all keys—*even zero*—are hashed
     * to a nonzero value, thus preserving the integrity of the hash table's
     * empty-slot convention (zero indicates an empty slot).
     *
     * - For any key whose integer representation is nonzero, returns the value as-is.
     * - For zero-valued keys:
     *   - If the key type is narrower than the hash type, the bits are shifted up,
     *     and the lowest bit is set to ensure a nonzero result.
     *   - Otherwise, returns all bits set (~0) to avoid ambiguity with empty slots.
     *
     * @param key Numeric key to hash.
     * @return Nonzero hash value for the given key.
     */
    QENTEM_INLINE static SizeT Hash(const Key_T &key) noexcept {
        constexpr SizeT32 key_size = sizeof(SizeT);
        constexpr SizeT32 n_size   = sizeof(Key_T);

        using QNumberTypeT = typename QNumberAutoTypeT<Key_T, key_size>::QNumberType_T;

        auto        q_key = QNumberTypeT{key}.Natural;
        const SizeT hash  = static_cast<SizeT>(q_key);

        if (hash != 0) {
            return hash;
        }

        if QENTEM_CONST_EXPRESSION (key_size > n_size) {
            return static_cast<SizeT>(SizeT(q_key >> ((key_size - n_size) * 8U)) | SizeT{1});
        } else {
            return ~SizeT{0};
        }
    }

    /**
     * @brief Compares two numeric keys for equality.
     *
     * @param hash1 Hash value of the first key (unused).
     * @param hash2 Hash value of the second key (unused).
     * @param key1  The first numeric key.
     * @param key2  The second numeric key.
     * @return True if the keys are equal, false otherwise.
     */
    QENTEM_INLINE static bool IsEqual(SizeT, SizeT, const Key_T &key1, const Key_T &key2) noexcept {
        return (key1 == key2);
    }
};

/**
 * @brief String key utilities for Qentem hash tables.
 *
 * This struct provides static methods for hashing and comparing string-like keys
 * in the context of hash table operations. It is designed to be used as a key utility
 * (KeyUtils) template parameter for HashTable and its adapters. Supports hashing
 * from both a string object and raw character pointer, as well as equality checks
 * between string keys and raw character data.
 *
 * @tparam Key_T The string key type, which must provide CharType, First(), Length(), and IsEqual().
 */
template <typename Key_T>
struct StringKeyUtils_T {
    /**
     * @brief Character type associated with the key.
     */
    using Char_T = typename Key_T::CharType;

    /**
     * @brief Computes a hash for a raw character array.
     *
     * @param str   Pointer to the character array.
     * @param length Number of characters to hash.
     * @return The computed hash value.
     */
    QENTEM_INLINE static SizeT Hash(const Char_T *str, SizeT length) {
        return StringUtils::Hash(str, length);
    }

    /**
     * @brief Computes a hash for a string key object.
     *
     * Delegates to the raw pointer hash using the key's First() and Length().
     *
     * @param key The string key object.
     * @return The computed hash value.
     */
    QENTEM_INLINE static SizeT Hash(const Key_T &key) {
        return Hash(key.First(), key.Length());
    }

    /**
     * @brief Compares two string key objects for equality.
     *
     * Checks both hash values and, if matching, compares the keys.
     *
     * @param hash1 Hash value of the first key.
     * @param hash2 Hash value of the second key.
     * @param key1  First string key object.
     * @param key2  Second string key object.
     * @return True if both hashes and keys match, false otherwise.
     */
    template <typename KeyType_T>
    QENTEM_INLINE static bool IsEqual(SizeT hash1, SizeT hash2, const KeyType_T &key1, const Key_T &key2) {
        return ((hash1 == hash2) && key1.IsEqual(key2.First(), key2.Length()));
    }
};

// -------------- String Adapter ------------------
/**
 * @brief String-keyed hash table adapter for Qentem Engine.
 *
 * This adapter provides a hash table with string-like keys, exposing overloads for
 * raw character pointers and length parameters, in addition to object-based key APIs.
 * All key handling (hashing, comparison) is delegated to StringKeyUtils_T, ensuring
 * consistent behavior and maximum efficiency for string lookup.
 *
 * This struct inherits all core hash table functionality from the generic HashTable base,
 * and is intended for use with keys that implement CharType, First(), Length(), and IsEqual().
 *
 * @tparam Key_T       Key type. Must be copyable, movable, and support equality and less-than.
 * @tparam KeyUtils_T  Policy for key hashing and equality. Must provide static:
 *                       - SizeT Hash(const Key_T&)
 *                       - bool IsEqual(SizeT, SizeT, const Key_T&, const Key_T&)
 * @tparam HItem_T     Storage type. Must have SizeT Hash, SizeT Next, Key_T Key, plus Clear(), MoveDoublecat(), etc.
 */
template <typename StringKey_T, typename HItem_T>
struct StringHashTable : public HashTable<StringKey_T, StringKeyUtils_T<StringKey_T>, HItem_T> {
    /**
     * @brief Character type associated with the string key.
     */
    using Char_T = typename StringKey_T::CharType;

    /**
     * @brief Key utilities trait for string hashing and comparison.
     */
    using KeyUtilsT = StringKeyUtils_T<StringKey_T>;

    /**
     * @brief Base hash table type.
     */
    using BaseT = HashTable<StringKey_T, KeyUtilsT, HItem_T>;

    /**
     * @brief Inherit all constructors from the base hash table.
     */
    using BaseT::BaseT;
    // using BaseT::...; // c++17
    // (Other 'using BaseT::...' lines follow in the full struct.)

    using BaseT::ActualSize;
    using BaseT::begin;
    using BaseT::Capacity;
    using BaseT::Clear;
    using BaseT::Compress;
    using BaseT::End;
    using BaseT::end;
    using BaseT::Expect;
    using BaseT::First;
    using BaseT::GetIndex;
    using BaseT::GetItem;
    using BaseT::GetItemAt;
    using BaseT::GetKeyAt;
    using BaseT::Has;
    using BaseT::Insert;
    using BaseT::IsEmpty;
    using BaseT::IsNotEmpty;
    using BaseT::Last;
    using BaseT::Remove;
    using BaseT::RemoveAt;
    using BaseT::Rename;
    using BaseT::Reserve;
    using BaseT::Reset;
    using BaseT::Resize;
    using BaseT::Size;
    using BaseT::Sort;
    using BaseT::Storage;

    using BaseT::clearStorage;
    using BaseT::copyTable;
    using BaseT::expand;
    using BaseT::find;
    using BaseT::generateHash;
    using BaseT::getBase;
    using BaseT::hashAndFind;
    using BaseT::insert;
    using BaseT::remove;
    using BaseT::reserve;
    using BaseT::reserveOnly;
    using BaseT::setCapacity;
    using BaseT::setSize;
    using BaseT::setStorage;
    using BaseT::tryInsert;

    /**
     * @brief Inserts a key into the hash table from a raw character array.
     *
     * Convenience overload for inserting a key without constructing a key object.
     * If the key does not already exist, it will be created and initialized.
     *
     * @param str    Pointer to the character array representing the key.
     * @param length Number of characters to use from the array.
     */
    inline void Insert(const Char_T *str, const SizeT length) {
        tryInsert(str, length);
    }

    /**
     * @brief Checks if a key, given as a raw character array, exists in the table.
     *
     * @param str    Pointer to the character array to look up.
     * @param length Number of characters to use from the array.
     * @return True if the key is present in the table, false otherwise.
     */
    inline bool Has(const Char_T *str, const SizeT length) const noexcept {
        if (IsNotEmpty()) {
            SizeT index;
            return (find(index, str, length) != nullptr);
        }

        return false;
    }

    /**
     * @brief Finds an item by key (raw character array, hash specified).
     *
     * @param str    Pointer to the character array representing the key.
     * @param length Number of characters to use from the array.
     * @param hash   Hash value to use for the lookup.
     * @return Pointer to the item if found, nullptr otherwise.
     */
    inline HItem_T *GetItem(const Char_T *str, const SizeT length, const SizeT hash) noexcept {
        if (IsNotEmpty()) {
            SizeT *index;
            return find(index, str, length, hash);
        }

        return nullptr;
    }

    /**
     * @brief Finds an item by key (raw character array, hash specified).
     *
     * @param str    Pointer to the character array representing the key.
     * @param length Number of characters to use from the array.
     * @param hash   Hash value to use for the lookup.
     * @return Const pointer to the item if found, nullptr otherwise.
     */
    inline const HItem_T *GetItem(const Char_T *str, const SizeT length, const SizeT hash) const noexcept {
        if (IsNotEmpty()) {
            SizeT index;
            return find(index, str, length, hash);
        }

        return nullptr;
    }

    /**
     * @brief Finds an item by key (raw character array, hash computed automatically).
     *
     * @param str    Pointer to the character array representing the key.
     * @param length Number of characters to use from the array.
     * @return Pointer to the item if found, nullptr otherwise.
     */
    inline const HItem_T *GetItem(const Char_T *str, const SizeT length) noexcept {
        return GetItem(str, length, KeyUtilsT::Hash(str, length));
    }

    /**
     * @brief Finds an item by key (raw character array, hash computed automatically).
     *
     * @param str    Pointer to the character array representing the key.
     * @param length Number of characters to use from the array.
     * @return Const pointer to the item if found, nullptr otherwise.
     */
    inline const HItem_T *GetItem(const Char_T *str, const SizeT length) const noexcept {
        return GetItem(str, length, KeyUtilsT::Hash(str, length));
    }

    /**
     * @brief Retrieves the internal index of a key given as a raw character array.
     *
     * @param index  Output parameter that will receive the index if found.
     * @param str    Pointer to the character array representing the key.
     * @param length Number of characters to use from the array.
     * @return True if the key was found and index is set, false otherwise.
     */
    inline bool GetIndex(SizeT &index, const Char_T *str, const SizeT length) const noexcept {
        if (IsNotEmpty()) {
            find(index, str, length);
            return (index != Capacity());
        }

        return false;
    }

    /**
     * @brief Removes a key from the table, given as a raw character array.
     *
     * @param str    Pointer to the character array representing the key.
     * @param length Number of characters to use from the array.
     */
    inline void Remove(const Char_T *str, SizeT length) noexcept {
        remove(str, length);
    }

    /**
     * @brief Removes a null-terminated string key from the table.
     *
     * Convenience overload; computes the length of the string automatically.
     *
     * @param str Pointer to the null-terminated character array representing the key.
     */
    inline void Remove(const Char_T *str) noexcept {
        Remove(str, StringUtils::Count(str));
    }

  protected:
    /**
     * @brief Finds an item in the hash table by a raw character key and hash value.
     *
     * Performs a lookup for an entry matching the given key and hash value.
     * Updates the index pointer to the hash table slot or chain position where the item was found,
     * or where a new item should be inserted if not found.
     *
     * @param[out] index  Output reference to a pointer to the hash table slot; updated during lookup.
     * @param str         Pointer to the character array representing the key to search for.
     * @param length      Number of characters in the key array.
     * @param hash        Precomputed hash value of the key.
     * @return Pointer to the found item, or nullptr if not found.
     *
     * @note If the item is not found, @p index is set to the appropriate insertion point in the chain.
     */
    HItem_T *find(SizeT *&index, const Char_T *str, const SizeT length, const SizeT hash) noexcept {
        return find(index, StringView<Char_T>{str, length}, hash);
    }

    /**
     * @brief Finds a const item in the hash table by a raw character key and hash value.
     *
     * Performs a const lookup for an entry matching the given key and hash value.
     * Updates the index reference to the slot or chain position found (or for insertion if not found).
     *
     * @param[out] index  Output reference to a slot index; updated during lookup.
     * @param str         Pointer to the character array representing the key to search for.
     * @param length      Number of characters in the key array.
     * @param hash        Precomputed hash value of the key.
     * @return Const pointer to the found item, or nullptr if not found.
     */
    const HItem_T *find(SizeT &index, const Char_T *str, const SizeT length, const SizeT hash) const noexcept {
        return find(index, StringView<Char_T>{str, length}, hash);
    }

    /**
     * @brief Finds an item in the hash table by a raw character key (hash computed automatically).
     *
     * A convenient overload: computes the hash from the provided key and length, then performs the lookup.
     *
     * @param[out] index Output reference to a pointer to the hash table slot; updated during lookup.
     * @param str       Pointer to the character array representing the key to search for.
     * @param length    Number of characters in the key array.
     * @return Pointer to the found item, or nullptr if not found.
     */
    inline HItem_T *find(SizeT *&index, const Char_T *str, const SizeT length) noexcept {
        return find(index, str, length, KeyUtilsT::Hash(str, length));
    }

    /**
     * @brief Finds a const item in the hash table by a raw character key (hash computed automatically).
     *
     * Computes the hash from the key and length, then performs a const lookup.
     *
     * @param[out] index Output reference to a slot index; updated during lookup.
     * @param str       Pointer to the character array representing the key to search for.
     * @param length    Number of characters in the key array.
     * @return Const pointer to the found item, or nullptr if not found.
     */
    inline const HItem_T *find(SizeT &index, const Char_T *str, const SizeT length) const noexcept {
        return find(index, str, length, KeyUtilsT::Hash(str, length));
    }

    /**
     * @brief Finds or inserts a key by raw character pointer and length.
     *
     * If the key is already present, returns a pointer to the existing item.
     * Otherwise, inserts a new item with the provided key, initializing its value,
     * and returns a pointer to the new item.
     *
     * If the container is at capacity, triggers an expansion before insertion.
     *
     * @param str    Pointer to the character array representing the key.
     * @param length Number of characters in the key array.
     * @return Pointer to the found or newly inserted item.
     */
    inline HItem_T *tryInsert(const Char_T *str, const SizeT length) noexcept {
        if (Size() == Capacity()) {
            expand(Capacity() * SizeT{2});
        }

        const SizeT hash = KeyUtilsT::Hash(str, length);
        SizeT      *index;
        HItem_T    *item = find(index, str, length, hash);

        if (item == nullptr) {
            item = insert(index, StringKey_T{str, length}, hash);
            item->InitValue();
        }

        return item;
    }

    /**
     * @brief Removes an item from the hash table by raw character key.
     *
     * Finds and removes the item with the specified key (character pointer and length), if present.
     * If the table is empty or the key is not found, the function has no effect.
     *
     * @param str    Pointer to the character array representing the key to remove.
     * @param length Number of characters in the key array.
     */
    void remove(const Char_T *str, const SizeT length) noexcept {
        if (IsNotEmpty()) {
            SizeT   *index;
            HItem_T *item = find(index, str, length);

            remove(index, item);
        }
    }
};

/**
 * @brief Selects the appropriate hash table adapter (string or number) at compile time.
 *
 * This set of templates chooses between StringHashTable and NumberHashTable based
 * on whether the key type is a recognized numeric type, using QTraits::IsNumber.
 *
 * Usage: Use AutoHashTable<Key_T, HItem_T> as a base class for associative containers.
 *
 * @tparam Key_T   The key type.
 * @tparam HItem_T The hash table item type (must provide Key, Hash, Next, Clear, etc).
 * @tparam is_number (internal) SFINAE: true for number types, false for string types.
 */

// Primary template: does not define Type (will select via partial specialization)
template <typename Key_T, typename HItem_T, bool = QTraits::IsNumber<Key_T>::value>
struct HashTableSelector;

// Specialization for non-numeric (string-like) keys
/**
 * @brief Specialization: selects StringHashTable for non-numeric key types.
 */
template <typename Key_T, typename HItem_T>
struct HashTableSelector<Key_T, HItem_T, false> {
    /// Type alias for string-keyed hash table.
    using Type = StringHashTable<Key_T, HItem_T>;
};

// Specialization for numeric keys
/**
 * @brief Specialization: selects NumberHashTable for numeric key types.
 */
template <typename Key_T, typename HItem_T>
struct HashTableSelector<Key_T, HItem_T, true> {
    /// Type alias for number-keyed hash table.
    using Type = HashTable<Key_T, NumberKeyUtils_T<Key_T>, HItem_T>;
};

/**
 * @brief Type alias for automatic hash table selection.
 *
 * Resolves to StringHashTable if Key_T is not a recognized number type,
 * or NumberHashTable if Key_T is a recognized number type (per QTraits::IsNumber).
 *
 * Example:
 *   using Table = AutoHashTable<MyKeyType, MyItemType>;
 */
template <typename Key_T, typename HItem_T>
using AutoHashTable = typename HashTableSelector<Key_T, HItem_T>::Type;

} // namespace Qentem

#endif
