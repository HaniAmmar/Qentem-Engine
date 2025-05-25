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
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_H_LIST_H
#define QENTEM_H_LIST_H

#include "HashTable.hpp"

namespace Qentem {

template <typename Key_T>
struct HLItem_T {
    Key_T Key;
    SizeT Hash;
    SizeT Next;

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
            if (src_item->Hash != 0) {
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
        src.setSize(0);
        src.setCapacity(0);
    }

    void operator+=(const HList &src) {
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
