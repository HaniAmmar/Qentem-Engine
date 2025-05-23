/*
 * Copyright (c) 2024 Hani Ammar
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef QENTEM_HARRAY_H
#define QENTEM_HARRAY_H

#include "HashTable.hpp"

namespace Qentem {
/*
 * A data structure that looks like an ordered array but behaves like a hash
 * table. Any value can be accessed by its index or its name. It preserves
 * the elements' order and uses one memory block. Therefore, it has an initial
 * size, and collisions do not cause new allocations. When resized, it drops
 * deleted items and resets its hash base.
 */

/*|-------------------------------------------|*/
/*|                Hash Table                 |*/
/*|____________________|______________________|*/
/*|     Positions      |       Items          |*/
/*| 0, 1 , 2, ... n-1  | item 0, item 1, ...  |*/
/*|____________________|______________________|*/

template <typename Key_T, typename Value_T>
struct HAItem_T {
    Key_T   Key;
    SizeT   Hash;
    SizeT   Next;
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

    void Clear() {
        Key   = Key_T{};
        Value = Value_T{};
    }
};

template <typename Key_T, typename Value_T>
struct HArray : public HashTable<Key_T, HAItem_T<Key_T, Value_T>> {
    using HItem  = HAItem_T<Key_T, Value_T>;
    using BaseT  = HashTable<Key_T, HItem>;
    using Char_T = typename Key_T::CharType;

    using BaseT::BaseT;

    using BaseT::Capacity;
    using BaseT::clearHashTable;
    using BaseT::expand;
    using BaseT::find;
    using BaseT::getHashTable;
    using BaseT::insert;
    using BaseT::IsNotEmpty;
    using BaseT::resize;
    using BaseT::setCapacity;
    using BaseT::setSize;
    using BaseT::Size;
    using BaseT::Storage;

    void operator+=(HArray &&src) {
        const SizeT  n_size   = (Size() + src.Size());
        HItem       *src_item = src.Storage();
        const HItem *src_end  = (src_item + src.Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }

        while (src_item < src_end) {
            if (src_item->Hash != 0) {
                SizeT *index;
                HItem *storage_item = find(index, src_item->Key.First(), src_item->Key.Length(), src_item->Hash);

                if (storage_item == nullptr) {
                    insert(index, Memory::Move(src_item->Key), src_item->Hash, Memory::Move(src_item->Value));
                } else {
                    storage_item->Value = Memory::Move(src_item->Value);
                    Memory::Dispose(&(src_item->Key));
                }
            }

            ++src_item;
        }

        Memory::Deallocate(src.getHashTable());
        src.clearHashTable();
        src.setSize(0);
        src.setCapacity(0);
    }

    void operator+=(const HArray &src) {
        const SizeT  n_size   = (Size() + src.Size());
        const HItem *src_item = src.First();
        const HItem *src_end  = src_item + src.Size();

        if (n_size > Capacity()) {
            resize(n_size);
        }

        while (src_item < src_end) {
            if (src_item->Hash != 0) {
                SizeT *index;
                HItem *storage_item = find(index, src_item->Key.First(), src_item->Key.Length(), src_item->Hash);

                if (storage_item == nullptr) {
                    storage_item = insert(index, Key_T{src_item->Key}, src_item->Hash);
                    Memory::Initialize(&(storage_item->Value));
                }

                storage_item->Value = src_item->Value;
            }

            ++src_item;
        }
    }

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

    Value_T &operator[](const Char_T *key) {
        return Get(key, StringUtils::Count(key));
    }

    Value_T &operator[](const Key_T &key) {
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

    void Insert(const Key_T &key, Value_T &&value) {
        Insert(Key_T{key}, Memory::Move(value));
    }

    void Insert(Key_T &&key, const Value_T &value) {
        Insert(Memory::Move(key), Value_T{value});
    }

    void Insert(const Key_T &key, const Value_T &value) {
        Insert(Key_T{key}, Value_T{value});
    }

    void Insert(const Char_T *key, const SizeT length, Value_T &&value) {
        Insert(Key_T{key, length}, Memory::Move(value));
    }

    Value_T *GetValue(const Char_T *key, const SizeT length) const noexcept {
        return GetValue(key, length, StringUtils::Hash(key, length));
    }

    inline Value_T *GetValue(const Key_T &key) const noexcept {
        return GetValue(key.First(), key.Length());
    }

    Value_T *GetValue(const SizeT index) noexcept {
        HItem *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Value);
        }

        return nullptr;
    }

    const Value_T *GetValue(const SizeT index) const noexcept {
        const HItem *src = BaseT::First();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Value);
        }

        return nullptr;
    }

    Value_T *GetValue(const Char_T *key, const SizeT length, const SizeT hash) const noexcept {
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
