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

#include "HashTable.hpp"

namespace Qentem {

/*|-------------------------------------------|*/
/*|                Hash Table                 |*/
/*|____________________|______________________|*/
/*|     Positions      |       Items          |*/
/*| 0, 1 , 2, ... n-1  | item 0, item 1, ...  |*/
/*|____________________|______________________|*/

template <typename Key_T, typename Value_T>
struct HAItem_T {
    SizeT   Hash;
    SizeT   Next;
    Key_T   Key;
    Value_T Value;

    inline bool operator<(const HAItem_T &item) const noexcept {
        return (Key < item.Key);
    }

    inline bool operator>(const HAItem_T &item) const noexcept {
        return (Key > item.Key);
    }

    inline bool operator<=(const HAItem_T &item) const noexcept {
        return (Key <= item.Key);
    }

    inline bool operator>=(const HAItem_T &item) const noexcept {
        return (Key >= item.Key);
    }

    inline bool operator==(const HAItem_T &item) const noexcept {
        return (Key == item.Key);
    }

    inline void Clear() {
        Key   = Key_T{};
        Value = Value_T{};
    }

    inline void MoveDoublecat(HAItem_T &item) {
        Value = Memory::Move(item.Value);
        Memory::Dispose(&(item.Key));
    }

    inline void CopyValue(HAItem_T const &item) {
        Value = item.Value;
    }

    inline void InitValue() {
        Memory::Initialize(&Value);
    }
};

template <typename Key_T, typename Value_T>
struct HArray : public HashTable<Key_T, HAItem_T<Key_T, Value_T>> {
    using HItem  = HAItem_T<Key_T, Value_T>;
    using BaseT  = HashTable<Key_T, HItem>;
    using Char_T = typename Key_T::CharType;

    using BaseT::BaseT;

    using BaseT::Capacity;
    using BaseT::expand;
    using BaseT::find;
    using BaseT::insert;
    using BaseT::IsNotEmpty;
    using BaseT::Size;
    using BaseT::Storage;

    Value_T &Get(const Char_T *key, const SizeT length) {
        if (Size() == Capacity()) {
            expand();
        }

        const SizeT hash = StringUtils::Hash(key, length);
        SizeT      *index;
        HItem      *item = find(index, key, length, hash);

        if (item != nullptr) {
            return item->Value;
        }

        item = insert(index, Key_T{key, length}, hash);
        Memory::Initialize(&(item->Value));

        return item->Value;
    }

    inline Value_T &operator[](const Char_T *key) {
        return Get(key, StringUtils::Count(key));
    }

    inline Value_T &operator[](const Key_T &key) {
        return Get(key.First(), key.Length());
    }

    Value_T &operator[](Key_T &&key) {
        if (Size() == Capacity()) {
            expand();
        }

        const SizeT hash = StringUtils::Hash(key.First(), key.Length());
        SizeT      *index;
        HItem      *item = find(index, key.First(), key.Length(), hash);

        if (item == nullptr) {
            item = insert(index, Memory::Move(key), hash);
            Memory::Initialize(&(item->Value));
        }

        return item->Value;
    }

    void Insert(Key_T &&key, Value_T &&value) {
        if (Size() == Capacity()) {
            expand();
        }

        const SizeT hash = StringUtils::Hash(key.First(), key.Length());
        SizeT      *index;
        HItem      *item = find(index, key.First(), key.Length(), hash);

        if (item == nullptr) {
            item = insert(index, Memory::Move(key), hash);
            Memory::Initialize(&(item->Value), Memory::Move(value));
        } else {
            item->Value = Memory::Move(value);
        }
    }

    inline void Insert(const Key_T &key, Value_T &&value) {
        Insert(Key_T{key}, Memory::Move(value));
    }

    inline void Insert(Key_T &&key, const Value_T &value) {
        Insert(Memory::Move(key), Value_T{value});
    }

    inline void Insert(const Key_T &key, const Value_T &value) {
        Insert(Key_T{key}, Value_T{value});
    }

    inline void Insert(const Char_T *key, const SizeT length, Value_T &&value) {
        Insert(Key_T{key, length}, Memory::Move(value));
    }

    inline Value_T *GetValue(const Char_T *key, const SizeT length) const noexcept {
        return GetValue(key, length, StringUtils::Hash(key, length));
    }

    inline Value_T *GetValue(const Key_T &key) const noexcept {
        return GetValue(key.First(), key.Length());
    }

    inline Value_T *GetValue(const SizeT index) const noexcept {
        HItem *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Value);
        }

        return nullptr;
    }

    inline Value_T *GetValue(const Char_T *key, const SizeT length, const SizeT hash) const noexcept {
        if (IsNotEmpty()) {
            SizeT *index;
            HItem *item = find(index, key, length, hash);

            if (item != nullptr) {
                return &(item->Value);
            }
        }

        return nullptr;
    }

  private:
    void insert(SizeT *index, Key_T &&key, const SizeT hash, Value_T &&value) noexcept {
        HItem *item = insert(index, Memory::Move(key), hash);
        Memory::Initialize(&(item->Value), Memory::Move(value));
    }
};

} // namespace Qentem

#endif
