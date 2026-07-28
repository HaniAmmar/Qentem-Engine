/**
 * @file HashTableAdapters.hpp
 * @brief Hash table adapters and key utilities.
 *
 * Provides specialized hash table implementations and key utility policies
 * for string and numeric key types. These adapters extend the generic
 * HashTable interface with key-specific hashing, comparison, and lookup
 * operations while preserving a common API and storage model.
 *
 * The selected implementation can be configured with a custom numeric type
 * for capacities, indexes, hash values, and internal bookkeeping, allowing
 * hash tables to operate independently of SizeT when required.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_HASH_TABLE_ADAPTERS_H
#define QENTEM_HASH_TABLE_ADAPTERS_H

#include "Qentem/QTraits.hpp"
#include "Qentem/HashTable.hpp"
#include "Qentem/QNumber.hpp"
#include "Qentem/StringView.hpp"

namespace Qentem {

/**
 * @brief Numeric key utilities for hash table operations.
 *
 * NumberKeyUtils_T provides hashing and comparison functions for numeric
 * keys used by HashTable and related containers. It is intended for
 * integral, enumeration, pointer-sized, and other numeric key types
 * where hashing can be performed directly from the key value.
 *
 * Hash values are represented using Number_T, allowing the hash width
 * and internal numeric representation to be configured independently
 * of SizeT.
 *
 * @tparam Key_T
 *         Numeric key type. Must support equality comparison.
 * @tparam Number_T
 *         Numeric type used for capacities, indexes,
 *         hash values, and internal bookkeeping.
 */
template <typename Key_T, typename Number_T>
struct NumberKeyUtils_T {
    using NumberT = Number_T;

    /**
     * @brief Computes a non-zero hash value for numeric keys.
     *
     * Numeric keys are hashed directly from their underlying value. Since
     * zero is reserved internally to represent an empty hash slot, a
     * zero-valued key is mapped to 1 to guarantee a non-zero hash result.
     *
     * @param key
     *         Numeric key to hash.
     *
     * @return
     *         Non-zero hash value for the given key.
     */
    QENTEM_INLINE static NumberT Hash(const Key_T &key) noexcept {
        constexpr SizeT32 key_size = sizeof(Key_T);

        using QNumberTypeT = typename QNumberAutoTypeT<Key_T, key_size>::QNumberType_T;

        auto q_key = QNumberTypeT{key}.Natural;

        return (q_key != 0) ? static_cast<NumberT>(q_key) : NumberT{1};
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
    QENTEM_INLINE static bool IsEqual(NumberT, NumberT, const Key_T &key1, const Key_T &key2) noexcept {
        return (key1 == key2);
    }
};

/**
 * @brief String key utilities for hash table operations.
 *
 * StringKeyUtils_T provides hashing and comparison functions for string-like
 * keys used by HashTable and related containers. It supports operations on
 * both key objects and raw character sequences, allowing efficient lookup
 * without requiring temporary key construction.
 *
 * Hash values are represented using Number_T, enabling the hash width to be
 * configured independently of SizeT.
 *
 * @tparam Key_T
 *         String key type. Must provide CharType, First(), Length(),
 *         and IsEqual().
 * @tparam Number_T
 *         Numeric type used for capacities, indexes,
 *         hash values, and internal bookkeeping.
 */
template <typename Key_T, typename Number_T>
struct StringKeyUtils_T {
    using NumberT = Number_T;

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
    QENTEM_INLINE static NumberT Hash(const Char_T *str, NumberT length) {
        return StringUtils::Hash<Char_T, NumberT>(str, length);
    }

    /**
     * @brief Computes a hash for a string key object.
     *
     * Delegates to the raw pointer hash using the key's First() and Length().
     *
     * @param key The string key object.
     * @return The computed hash value.
     */
    QENTEM_INLINE static NumberT Hash(const Key_T &key) {
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
    QENTEM_INLINE static bool IsEqual(NumberT hash1, NumberT hash2, const KeyType_T &key1, const Key_T &key2) {
        return ((hash1 == hash2) && key1.IsEqual(key2.First(), key2.Length()));
    }
};

/**
 * @brief Hash table specialized for string-like keys.
 *
 * StringHashTable extends HashTable with convenience overloads for
 * character sequences and string objects while preserving the storage
 * layout, growth behavior, and memory management of the underlying
 * hash table implementation.
 *
 * Hashing and key comparison are delegated to StringKeyUtils_T,
 * providing efficient lookup from both key objects and raw character
 * sequences without requiring temporary key construction.
 *
 * The container uses Number_T for capacities, indexes, hash values,
 * and internal bookkeeping, allowing its numeric width to be configured
 * independently of SizeT.
 *
 * @tparam StringKey_T
 *         String key type. Must provide CharType, First(), Length(),
 *         and IsEqual().
 * @tparam Number_T
 *         Numeric type used for capacities, indexes,
 *         hash values, and internal bookkeeping.
 * @tparam HItem_T
 *         Hash table item type.
 * @tparam Expansion_Multiplier_T
 *         Compile-time capacity growth factor used during reallocation.
 * @tparam MemoryProvider_T
 *         Memory backend used for storage management.
 */
template <typename StringKey_T, typename Number_T, typename HItem_T, Number_T Expansion_Multiplier_T,
          typename MemoryProvider_T>
struct StringHashTable : public HashTable<StringKey_T, Number_T, StringKeyUtils_T<StringKey_T, Number_T>, HItem_T,
                                          Expansion_Multiplier_T, MemoryProvider_T> {
    using NumberT = Number_T;

    /**
     * @brief Character type associated with the string key.
     */
    using Char_T = typename StringKey_T::CharType;

    /**
     * @brief Key utilities trait for string hashing and comparison.
     */
    using KeyUtilsT = StringKeyUtils_T<StringKey_T, Number_T>;

    /**
     * @brief Base hash table type.
     */
    using BaseT = HashTable<StringKey_T, Number_T, KeyUtilsT, HItem_T, Expansion_Multiplier_T, MemoryProvider_T>;

    /**
     * @brief Inherit all constructors from the base hash table.
     */
    using BaseT::BaseT;

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

    QENTEM_INLINE StringHashTable() noexcept = default;
    /**
     * @brief Inserts a key into the hash table from a raw character array.
     *
     * Convenience overload for inserting a key without constructing a key object.
     * If the key does not already exist, it will be created and initialized.
     *
     * @param str    Pointer to the character array representing the key.
     * @param length Number of characters to use from the array.
     */
    QENTEM_INLINE void Insert(const Char_T *str, const NumberT length) {
        tryInsert(str, length);
    }

    /**
     * @brief Checks if a key, given as a raw character array, exists in the table.
     *
     * @param str    Pointer to the character array to look up.
     * @param length Number of characters to use from the array.
     * @return True if the key is present in the table, false otherwise.
     */
    QENTEM_INLINE bool Has(const Char_T *str, const NumberT length) const noexcept {
        if (IsNotEmpty()) {
            NumberT index;
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
    QENTEM_INLINE HItem_T *GetItem(const Char_T *str, const NumberT length, const NumberT hash) noexcept {
        if (IsNotEmpty()) {
            NumberT *index;
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
    QENTEM_INLINE const HItem_T *GetItem(const Char_T *str, const NumberT length, const NumberT hash) const noexcept {
        if (IsNotEmpty()) {
            NumberT index;
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
    QENTEM_INLINE const HItem_T *GetItem(const Char_T *str, const NumberT length) noexcept {
        return GetItem(str, length, KeyUtilsT::Hash(str, length));
    }

    /**
     * @brief Finds an item by key (raw character array, hash computed automatically).
     *
     * @param str    Pointer to the character array representing the key.
     * @param length Number of characters to use from the array.
     * @return Const pointer to the item if found, nullptr otherwise.
     */
    QENTEM_INLINE const HItem_T *GetItem(const Char_T *str, const NumberT length) const noexcept {
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
    QENTEM_INLINE bool GetIndex(NumberT &index, const Char_T *str, const NumberT length) const noexcept {
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
    QENTEM_INLINE void Remove(const Char_T *str, NumberT length) noexcept {
        remove(str, length);
    }

    /**
     * @brief Removes a null-terminated string key from the table.
     *
     * Convenience overload; computes the length of the string automatically.
     *
     * @param str Pointer to the null-terminated character array representing the key.
     */
    QENTEM_INLINE void Remove(const Char_T *str) noexcept {
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
    QENTEM_INLINE HItem_T *find(NumberT *&index, const Char_T *str, const NumberT length, const NumberT hash) noexcept {
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
    QENTEM_INLINE const HItem_T *find(NumberT &index, const Char_T *str, const NumberT length,
                                      const NumberT hash) const noexcept {
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
    QENTEM_INLINE HItem_T *find(NumberT *&index, const Char_T *str, const NumberT length) noexcept {
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
    QENTEM_INLINE const HItem_T *find(NumberT &index, const Char_T *str, const NumberT length) const noexcept {
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
    QENTEM_INLINE HItem_T *tryInsert(const Char_T *str, const NumberT length) noexcept {
        if (Size() == Capacity()) {
            expand(Capacity() * NumberT{2});
        }

        const NumberT hash = KeyUtilsT::Hash(str, length);
        NumberT      *index;
        HItem_T      *item = find(index, str, length, hash);

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
    QENTEM_INLINE void remove(const Char_T *str, const NumberT length) noexcept {
        if (IsNotEmpty()) {
            NumberT *index;
            HItem_T *item = find(index, str, length);

            remove(index, item);
        }
    }
};

/**
 * @brief Selects the appropriate hash table implementation at compile time.
 *
 * HashTableSelector chooses between NumberHashTable and StringHashTable
 * based on the key type. Numeric keys use NumberHashTable, while all
 * other key types use StringHashTable.
 *
 * The selected implementation preserves the configured numeric width,
 * growth policy, and memory backend, ensuring consistent behavior across
 * all specializations with no runtime overhead.
 *
 * @tparam Key_T
 *         The key type.
 * @tparam Number_T
 *         Numeric type used for capacities, indexes,
 *         hash values, and internal bookkeeping.
 * @tparam HItem_T
 *         The hash table item type.
 * @tparam Expansion_Multiplier_T
 *         Compile-time capacity growth factor propagated to the selected
 *         hash table implementation.
 * @tparam MemoryProvider_T
 *         Memory backend used for storage management.
 * @tparam is_number
 *         Implementation detail indicating whether Key_T is numeric.
 */
template <typename Key_T, typename Number_T, typename HItem_T, Number_T Expansion_Multiplier_T,
          typename MemoryProvider_T, bool = QTraits::IsNumber<Key_T>::value>
struct HashTableSelector;

// Specialization for non-numeric (string-like) keys
/**
 * @brief Specialization: selects StringHashTable for non-numeric key types.
 */
template <typename Key_T, typename Number_T, typename HItem_T, Number_T Expansion_Multiplier_T,
          typename MemoryProvider_T>
struct HashTableSelector<Key_T, Number_T, HItem_T, Expansion_Multiplier_T, MemoryProvider_T, false> {
    /// Type alias for string-keyed hash table.
    using Type = StringHashTable<Key_T, Number_T, HItem_T, Expansion_Multiplier_T, MemoryProvider_T>;
};

// Specialization for numeric keys
/**
 * @brief Specialization: selects NumberHashTable for numeric key types.
 */
template <typename Key_T, typename Number_T, typename HItem_T, Number_T Expansion_Multiplier_T,
          typename MemoryProvider_T>
struct HashTableSelector<Key_T, Number_T, HItem_T, Expansion_Multiplier_T, MemoryProvider_T, true> {
    /// Type alias for number-keyed hash table.
    using Type = HashTable<Key_T, Number_T, NumberKeyUtils_T<Key_T, Number_T>, HItem_T, Expansion_Multiplier_T,
                           MemoryProvider_T>;
};

/**
 * @brief Type alias for automatic hash table selection.
 *
 * AutoHashTable selects the most appropriate hash table implementation
 * for the specified key type. Numeric keys use NumberHashTable, while
 * all other key types use StringHashTable.
 *
 * The selected implementation uses Number_T for capacities, indexes,
 * hash values, and internal bookkeeping, allowing the table's numeric
 * width to be configured independently of SizeT.
 *
 * Capacity growth behavior is controlled at compile time through the
 * expansion multiplier and is forwarded unchanged to the selected
 * implementation, introducing no runtime overhead.
 *
 * @tparam Key_T
 *         The key type.
 * @tparam Number_T
 *         Numeric type used for capacities, indexes,
 *         hash values, and internal bookkeeping.
 * @tparam HItem_T
 *         The hash table item type.
 * @tparam Expansion_Multiplier_T
 *         Compile-time capacity growth factor used by the selected
 *         hash table implementation.
 * @tparam MemoryProvider_T
 *         Memory backend used for storage management.
 */
template <typename Key_T, typename Number_T, typename HItem_T, Number_T Expansion_Multiplier_T,
          typename MemoryProvider_T>
using AutoHashTable =
    typename HashTableSelector<Key_T, Number_T, HItem_T, Expansion_Multiplier_T, MemoryProvider_T>::Type;

} // namespace Qentem

#endif
