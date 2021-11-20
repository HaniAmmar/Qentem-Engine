/*
 * Copyright (c) 2020 Hani Ammar
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

#include "Platform.hpp"
#include "QPointer.hpp"
#include "String.hpp"

#ifndef QENTEM_HARRAY_H_
#define QENTEM_HARRAY_H_

namespace Qentem {

/*
 * A data structure that looks like an ordered array and behaves like a hash
 * table. Any value can be accessed by its index or its name. It preserves
 * the elements' order and uses one memory block. Therefore, it has an initial
 * size, and collisions do not cause new allocations. When resized, it drops
 * deleted items and resets its hash base. Also, Its pointer is taggable.
 */

template <typename Value_, typename Char_T_>
struct HAItem {
    SizeT           Next;
    SizeT           Hash;
    String<Char_T_> Key;
    Value_          Value;

    inline bool operator<(const HAItem &item) const noexcept {
        return (Key < item.Key);
    }

    inline bool operator>(const HAItem &item) const noexcept {
        return (Key > item.Key);
    }
};

/*|-------------------------------------------|*/
/*|                Hash Table                 |*/
/*|____________________|______________________|*/
/*|     Positions      |       Items          |*/
/*| 0, 1 , 2, ... n-1  | item0, item 1, ...   |*/
/*|____________________|______________________|*/

template <typename Value_, typename Char_T_>
class HArray {
    using HAItem_T_ = HAItem<Value_, Char_T_>;
    using Key_T_    = String<Char_T_>;

  public:
    HArray() = default;

    explicit HArray(SizeT size) {
        if (size != 0) {
            setCapacity(algineSize(size));
            allocate();
        }
    }

    ~HArray() {
        Memory::Destruct(Storage(), End());
        deallocate(getHashTable());
    }

    HArray(HArray &&src) noexcept
        : index_(src.Size()), capacity_(src.Capacity()) {
        hashTable_.Set(static_cast<QPointer<SizeT> &&>(src.hashTable_));
        src.setSize(0);
        src.setCapacity(0);
    }

    HArray(const HArray &src) { copyArray(src); }

    HArray &operator=(HArray &&src) noexcept {
        if (this != &src) {
            Memory::Destruct(Storage(), End());
            deallocate(getHashTable());

            setSize(src.Size());
            setCapacity(src.Capacity());
            hashTable_ = static_cast<QPointer<SizeT> &&>(src.hashTable_);

            src.setSize(0);
            src.setCapacity(0);
        }

        return *this;
    }

    HArray &operator=(const HArray &src) {
        if (this != &src) {
            Reset();
            copyArray(src);
        }

        return *this;
    }

    void operator+=(HArray &&src) {
        const SizeT      n_size = (Size() + src.Size());
        HAItem_T_       *item   = src.Storage();
        const HAItem_T_ *end    = (item + src.Size());

        if (n_size > Capacity()) {
            resize(algineSize(n_size));
        }

        while (item != end) {
            if (item->Hash != 0) {
                SizeT     *index;
                HAItem_T_ *des_item = find(index, item->Key.First(),
                                           item->Key.Length(), item->Hash);

                if (des_item == nullptr) {
                    des_item = insert(index, static_cast<Key_T_ &&>(item->Key),
                                      item->Hash);
                } else {
                    item->Key.~Key_T_();
                }

                des_item->Value = static_cast<Value_ &&>(item->Value);
            }

            ++item;
        }

        src.setCapacity(0);
        src.setSize(0);
        src.deallocate(src.getHashTable());
        src.hashTable_.Reset();
    }

    void operator+=(const HArray &src) {
        const SizeT      n_size = (Size() + src.Size());
        const HAItem_T_ *item   = src.First();
        const HAItem_T_ *end    = item + src.Size();

        if (n_size > Capacity()) {
            resize(algineSize(n_size));
        }

        while (item != end) {
            if (item->Hash != 0) {
                SizeT     *index;
                HAItem_T_ *des_item = find(index, item->Key.First(),
                                           item->Key.Length(), item->Hash);

                if (des_item == nullptr) {
                    des_item = insert(index, Key_T_(item->Key), item->Hash);
                }

                des_item->Value = Value_(item->Value);
            }

            ++item;
        }
    }

    Value_ &operator[](const Char_T_ *key) {
        if (Size() == Capacity()) {
            grow();
        }

        const SizeT len  = StringUtils::Count(key);
        const SizeT hash = StringUtils::Hash(key, len);
        SizeT      *index;
        HAItem_T_  *item = find(index, key, len, hash);

        if (item != nullptr) {
            return item->Value;
        }

        return insert(index, Key_T_(key, len), hash)->Value;
    }

    Value_ &operator[](Key_T_ &&key) {
        if (Size() == Capacity()) {
            grow();
        }

        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        const SizeT hash = StringUtils::Hash(str, len);
        SizeT      *index;
        HAItem_T_  *item = find(index, str, len, hash);

        if (item != nullptr) {
            return item->Value;
        }

        return insert(index, static_cast<Key_T_ &&>(key), hash)->Value;
    }

    Value_ &operator[](const Key_T_ &key) {
        if (Size() == Capacity()) {
            grow();
        }

        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        const SizeT hash = StringUtils::Hash(str, len);
        SizeT      *index;
        HAItem_T_  *item = find(index, str, len, hash);

        if (item != nullptr) {
            return item->Value;
        }

        return insert(index, Key_T_(key), hash)->Value;
    }

    void Insert(Key_T_ &&key, Value_ &&val) {
        if (Size() == Capacity()) {
            grow();
        }

        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        const SizeT hash = StringUtils::Hash(str, len);
        SizeT      *index;
        HAItem_T_  *item = find(index, str, len, hash);

        if (item == nullptr) {
            item = insert(index, static_cast<Key_T_ &&>(key), hash);
        }

        item->Value = static_cast<Value_ &&>(val);
    }

    Value_ *GetValue(SizeT index) const noexcept {
        HAItem_T_ *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Value);
        }

        return nullptr;
    }

    const Key_T_ *GetKey(SizeT index) const noexcept {
        HAItem_T_ *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Key);
        }

        return nullptr;
    }

    const HAItem_T_ *GetItem(SizeT index) const noexcept {
        HAItem_T_ *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return (src + index);
        }

        return nullptr;
    }

    const HAItem_T_ *GetItem(const Key_T_ &key) const noexcept {
        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        SizeT *index;
        return find(index, str, len, StringUtils::Hash(str, len));
    }

    bool GetKeyIndex(SizeT &index, const Char_T_ *str,
                     const SizeT len) const noexcept {
        SizeT *sub_index;
        if (find(sub_index, str, len, StringUtils::Hash(str, len)) != nullptr) {
            index = (*sub_index) - 1;

            return true;
        }

        return false;
    }

    Value_ *Find(const Char_T_ *key, SizeT length) const noexcept {
        if (Size() != 0) {
            SizeT     *index;
            HAItem_T_ *item =
                find(index, key, length, StringUtils::Hash(key, length));

            if (item != nullptr) {
                return &(item->Value);
            }
        }

        return nullptr;
    }

    inline Value_ *Find(const Key_T_ &key) const noexcept {
        return Find(key.First(), key.Length());
    }

    inline void Remove(const Char_T_ *key, SizeT length) const noexcept {
        remove(key, length, StringUtils::Hash(key, length));
    }

    inline void Remove(const Char_T_ *key) const noexcept {
        Remove(key, StringUtils::Count(key));
    }

    inline void Remove(const Key_T_ &key) const noexcept {
        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        remove(str, len, StringUtils::Hash(str, len));
    }

    void RemoveIndex(SizeT index) const noexcept {
        if (index < Size()) {
            const HAItem_T_ *item = (Storage() + index);

            if (item->Hash != 0) {
                remove(item->Key.First(), item->Key.Length(), item->Hash);
            }
        }
    }

    /*
     * This function renames a key to a nonexisting one without changing the
     * order of the item, and returns true if successful.
     */
    bool Rename(const Key_T_ &from, Key_T_ &&to) const noexcept {
        if (Size() != 0) {
            const Char_T_ *from_str = from.First();
            const SizeT    from_len = from.Length();

            SizeT *left_index;
            find(left_index, from_str, from_len,
                 StringUtils::Hash(from_str, from_len));

            if (*left_index != 0) {
                const Char_T_ *to_str = to.First();
                const SizeT    to_len = to.Length();

                const SizeT to_hash = StringUtils::Hash(to_str, to_len);

                SizeT *right_index;
                find(right_index, to_str, to_len, to_hash);

                if (*right_index == 0) {
                    const SizeT offset = ((*left_index) - 1);
                    HAItem_T_  *item   = (Storage() + offset);
                    *right_index       = *left_index;
                    *left_index        = item->Next;
                    item->Next         = 0;
                    item->Hash         = to_hash;

                    item->Key = static_cast<Key_T_ &&>(to);
                    return true;
                }
            }
        }

        return false;
    }

    bool Rename(const Key_T_ &from, const Key_T_ &to) const noexcept {
        return Rename(from, Key_T_(to));
    }

    void Reserve(SizeT size) {
        if (Storage() != nullptr) {
            Reset();
        }

        if (size != 0) {
            setCapacity(algineSize(size));
            allocate();
        }
    }

    void Reset() noexcept {
        HAItem_T_ *current = Storage();

        if (current != nullptr) {
            Memory::Destruct(current, End());
            deallocate(getHashTable());
            clearStorage();
            setCapacity(0);
            setSize(0);
        }
    }

    void Resize(SizeT new_size) {
        if (new_size == 0) {
            Reset();
            return;
        }

        if (Size() > new_size) {
            // Shrink
            Memory::Destruct((Storage() + new_size), End());
            setSize(new_size);
        }

        new_size = algineSize(new_size);
        resize(new_size);
    }

    // Set ascend to (false) for descend (ascend: 1,2,3; descend: 3,2,1 )
    void Sort(bool ascend = true) noexcept {
        Memory::QuickSort<HAItem_T_, SizeT>(Storage(), 0, Size(), ascend);
        Memory::SetToZero(getHashTable(), (sizeof(SizeT) * Capacity()));
        generateHash();
    }

    // Removes excess storage.
    void Compress() {
        const SizeT size = ActualSize();

        if (size == 0) {
            Reset();
        } else {
            const SizeT n_cap = algineSize(size);

            if ((size < Size()) || (n_cap < Capacity())) {
                resize(n_cap);
            }
        }
    }

    // Returns the actual number of items.
    SizeT ActualSize() const noexcept {
        SizeT size = 0;

        const HAItem_T_ *item = Storage();
        const HAItem_T_ *end  = (item + Size());

        while (item != end) {
            size += (item->Hash != 0);
            ++item;
        }

        return size;
    }

    inline SizeT Size() const noexcept { return index_; }
    inline SizeT Capacity() const noexcept { return capacity_; }

    inline HAItem_T_ *Storage() const noexcept {
        return reinterpret_cast<HAItem_T_ *>(getHashTable() + Capacity());
    }

    inline bool             IsEmpty() const noexcept { return (Size() == 0); }
    inline bool             IsNotEmpty() const noexcept { return !(IsEmpty()); }
    inline const HAItem_T_ *First() const noexcept { return Storage(); }
    inline const HAItem_T_ *End() const noexcept { return (First() + Size()); }

    inline const HAItem_T_ *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - 1));
        }

        return nullptr;
    }

    //////////// Private ////////////

  private:
    SizeT  getBase() const noexcept { return (Capacity() - 1); }
    SizeT *getHashTable() const noexcept { return hashTable_.GetPointer(); }
    void   setStorage(SizeT *ptr) noexcept { hashTable_.SetPointer(ptr); }

    HAItem_T_ *allocate() {
        const SizeT size = ((sizeof(SizeT) + sizeof(HAItem_T_)) * Capacity());
        SizeT *ht = reinterpret_cast<SizeT *>(Memory::Allocate<char>(size));
        setStorage(ht);
        Memory::SetToZero(ht, (sizeof(SizeT) * Capacity()));

        return reinterpret_cast<HAItem_T_ *>(ht + Capacity());
    }

    void deallocate(SizeT *hash_table) { Memory::Deallocate(hash_table); }
    void clearStorage() noexcept { setStorage(nullptr); }
    void setSize(SizeT new_size) noexcept { index_ = new_size; }
    void setCapacity(SizeT new_capacity) noexcept { capacity_ = new_capacity; }
    void grow() { resize(((Capacity() != 0) ? (Capacity() << 1U) : 1)); }

    SizeT algineSize(SizeT n_size) noexcept {
        const SizeT size = (SizeT{1} << Platform::CLZ(n_size));

        if (size < n_size) {
            return (size << 1U);
        }

        return size;
    }

    HAItem_T_ *find(SizeT *&index, const Char_T_ *key, SizeT length,
                    SizeT hash) const noexcept {
        SizeT     *ht  = getHashTable();
        HAItem_T_ *src = reinterpret_cast<HAItem_T_ *>(ht + Capacity());
        index          = (ht + (hash & getBase()));

        while (*index != 0) {
            HAItem_T_ *item = (src + (*index) - 1);

            if ((item->Hash == hash) && item->Key.IsEqual(key, length)) {
                return item;
            }

            index = &(item->Next);
        }

        return nullptr;
    }

    inline HAItem_T_ *insert(SizeT *index, Key_T_ &&key, SizeT hash) noexcept {
        HAItem_T_ *item = (Storage() + Size());
        ++index_;
        *index = Size();

        item->Next = 0;
        item->Hash = hash;
        Memory::Construct(&(item->Key), static_cast<Key_T_ &&>(key));
        Memory::Construct(&(item->Value), Value_());

        return item;
    }

    void remove(const Char_T_ *key, SizeT length, SizeT hash) const noexcept {
        if (Size() != 0) {
            SizeT     *index;
            HAItem_T_ *item = find(index, key, length, hash);

            if (item != nullptr) {
                *index     = item->Next;
                item->Next = 0;
                item->Hash = 0;

                item->Key   = Key_T_();
                item->Value = Value_();
            }
        }
    }

    QENTEM_NOINLINE void copyArray(const HArray &src) {
        // The function Reset() has to be called before this.
        if (src.Size() != 0) {
            const HAItem_T_ *src_item = src.First();
            const HAItem_T_ *end      = (src_item + src.Size());

            setCapacity(algineSize(src.Size()));

            HAItem_T_       *des_item = allocate();
            const HAItem_T_ *des_src  = des_item;

            do {
                if (src_item->Hash != 0) {
                    Memory::Construct(des_item, *src_item);
                    ++des_item;
                }

                ++src_item;
            } while (src_item != end);

            setSize(static_cast<SizeT>(des_item - des_src));
            generateHash();
        }
    }

    void resize(SizeT new_size) {
        SizeT     *ht  = getHashTable();
        HAItem_T_ *src = Storage();

        setCapacity(new_size);

        HAItem_T_       *des      = allocate();
        HAItem_T_       *des_item = des;
        HAItem_T_       *item     = src;
        const HAItem_T_ *end      = (item + Size());

        while (item != end) {
            if (item->Hash != 0) {
                Memory::Construct(des_item, static_cast<HAItem_T_ &&>(*item));
                ++des_item;
            }

            ++item;
        }

        setSize(static_cast<SizeT>(des_item - des));
        deallocate(ht);
        generateHash();
    }

    void generateHash() const noexcept {
        SizeT       i    = 1;
        const SizeT base = getBase();
        SizeT      *ht   = getHashTable();
        HAItem_T_  *src  = Storage();
        HAItem_T_  *item = src;

        const HAItem_T_ *end = (item + Size());

        while (item != end) {
            item->Next   = 0;
            SizeT *index = (ht + (item->Hash & base));

            while (*index != 0) {
                index = &((src + ((*index) - 1))->Next);
            }

            *index = i;
            ++i;
            item->Next = 0;
            ++item;
        }
    }

#ifndef QENTEM_BIG_ENDIAN
    SizeT           index_{0};
    SizeT           capacity_{0};
    QPointer<SizeT> hashTable_{};
#else
    QPointer<SizeT> hashTable_{};
    SizeT           index_{0};
    SizeT           capacity_{0};
#endif
};

} // namespace Qentem

#endif
