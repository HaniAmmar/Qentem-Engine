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

#ifndef QENTEM_H_LIST_H
#define QENTEM_H_LIST_H

#include "HashTable.hpp"

namespace Qentem {

template <typename Key_T>
struct HLItem_T {
    SizeT Hash;
    SizeT Next;
    Key_T Key;

    inline bool operator<(const HLItem_T &item) const noexcept {
        return (Key < item.Key);
    }

    inline bool operator>(const HLItem_T &item) const noexcept {
        return (Key > item.Key);
    }

    inline bool operator<=(const HLItem_T &item) const noexcept {
        return (Key <= item.Key);
    }

    inline bool operator>=(const HLItem_T &item) const noexcept {
        return (Key >= item.Key);
    }

    inline bool operator==(const HLItem_T &item) const noexcept {
        return (Key == item.Key);
    }

    void Clear() {
        Key = Key_T{};
    }
};

///////////////////////////////////////////////
template <typename Key_T>
struct HList : public HashTable<Key_T, HLItem_T<Key_T>> {
    using HItem  = HLItem_T<Key_T>;
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

    void operator+=(HList &&src) {
        const SizeT  n_size   = (Size() + src.Size());
        HItem       *src_item = src.Storage();
        const HItem *src_end  = (src_item + src.Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }

        while (src_item < src_end) {
            if (src_item->Hash != SizeT{0}) {
                SizeT *index;
                HItem *storage_item = find(index, src_item->Key.First(), src_item->Key.Length(), src_item->Hash);

                if (storage_item == nullptr) {
                    insert(index, Memory::Move(src_item->Key), src_item->Hash);
                } else {
                    Memory::Dispose(&(src_item->Key));
                }
            }

            ++src_item;
        }

        Memory::Deallocate(src.getHashTable());
        src.clearHashTable();
        src.setCapacity(SizeT{0});
        src.setSize(SizeT{0});
    }

    void operator+=(const HList &src) {
        const SizeT  n_size   = (Size() + src.Size());
        const HItem *src_item = src.First();
        const HItem *src_end  = src_item + src.Size();

        if (n_size > Capacity()) {
            resize(n_size);
        }

        while (src_item < src_end) {
            if (src_item->Hash != SizeT{0}) {
                SizeT *index;
                HItem *storage_item = find(index, src_item->Key.First(), src_item->Key.Length(), src_item->Hash);

                if (storage_item == nullptr) {
                    storage_item = insert(index, Key_T{src_item->Key}, src_item->Hash);
                }
            }

            ++src_item;
        }
    }

    void Insert(const Char_T *key, const SizeT length) {
        Insert(Key_T{key, length});
    }

    void Insert(const Key_T &key) {
        Insert(Key_T{key});
    }

    void Insert(Key_T &&key) {
        if (Size() == Capacity()) {
            expand();
        }

        const SizeT hash = StringUtils::Hash(key.First(), key.Length());
        SizeT      *index;
        HItem      *item = find(index, key.First(), key.Length(), hash);

        if (item == nullptr) {
            item = insert(index, Memory::Move(key), hash);
        }
    }
};

} // namespace Qentem

#endif
