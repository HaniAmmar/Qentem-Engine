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
struct HAItem_T_ {
    SizeT           Hash;
    SizeT           Next;
    String<Char_T_> Key;
    Value_          Value;

    inline bool operator<(const HAItem_T_ &item) const noexcept { return (Key < item.Key); }
    inline bool operator>(const HAItem_T_ &item) const noexcept { return (Key > item.Key); }
};

/*|-------------------------------------------|*/
/*|                Hash Table                 |*/
/*|____________________|______________________|*/
/*|     Positions      |       Items          |*/
/*| 0, 1 , 2, ... n-1  | item 0, item 1, ...  |*/
/*|____________________|______________________|*/

template <typename Value_, typename Char_T_>
class HArray {
    using HAItem_ = HAItem_T_<Value_, Char_T_>;
    using Key_    = String<Char_T_>;

  public:
    HArray() = default;

    explicit HArray(SizeT size) {
        if (size != 0) {
            allocate(size);
        }
    }

    ~HArray() {
        HAItem_ *storage = Storage();
        Memory::Dispose(storage, (storage + Size()));
        Memory::Deallocate(getHashTable());
    }

    HArray(HArray &&src) noexcept
        : index_{src.Size()}, capacity_{src.Capacity()}, hashTable_{static_cast<QPointer<SizeT> &&>(src.hashTable_)} {
        src.setSize(0);
        src.setCapacity(0);
    }

    HArray(const HArray &src) { copyTable(src); }

    HArray &operator=(HArray &&src) noexcept {
        if (this != &src) {
            HAItem_    *storage = Storage();
            SizeT      *ht      = getHashTable();
            const SizeT size    = Size();

            setSize(src.Size());
            setCapacity(src.Capacity());
            src.setSize(0);
            src.setCapacity(0);
            hashTable_.MovePointerOnly(src.hashTable_);

            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(ht);
        }

        return *this;
    }

    HArray &operator=(const HArray &src) {
        if (this != &src) {
            HAItem_    *storage = Storage();
            SizeT      *ht      = getHashTable();
            const SizeT size    = Size();

            setHashTable(nullptr);
            setCapacity(0);
            setSize(0);
            copyTable(src);

            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(ht);
        }

        return *this;
    }

    void operator+=(HArray &&src) {
        const SizeT    n_size   = (Size() + src.Size());
        HAItem_       *src_item = src.Storage();
        const HAItem_ *src_end  = (src_item + src.Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }

        while (src_item < src_end) {
            if (src_item->Hash != 0) {
                SizeT   *index;
                HAItem_ *storage_item = find(index, src_item->Key.First(), src_item->Key.Length(), src_item->Hash);

                if (storage_item == nullptr) {
                    storage_item = insert(index, static_cast<Key_ &&>(src_item->Key), src_item->Hash);
                } else {
                    Memory::Dispose(&(src_item->Key));
                }

                storage_item->Value = static_cast<Value_ &&>(src_item->Value);
            }

            ++src_item;
        }

        src.setCapacity(0);
        src.setSize(0);
        Memory::Deallocate(src.getHashTable());
        src.clearHashTable();
    }

    void operator+=(const HArray &src) {
        const SizeT    n_size   = (Size() + src.Size());
        const HAItem_ *src_item = src.First();
        const HAItem_ *src_end  = src_item + src.Size();

        if (n_size > Capacity()) {
            resize(n_size);
        }

        while (src_item < src_end) {
            if (src_item->Hash != 0) {
                SizeT   *index;
                HAItem_ *storage_item = find(index, src_item->Key.First(), src_item->Key.Length(), src_item->Hash);

                if (storage_item == nullptr) {
                    storage_item = insert(index, Key_(src_item->Key), src_item->Hash);
                }

                storage_item->Value = Value_(src_item->Value);
            }

            ++src_item;
        }
    }

    Value_ &operator[](const Char_T_ *key) { return GetOrAdd(key, StringUtils::Count(key)); }

    Value_ &operator[](Key_ &&key) {
        if (Size() == Capacity()) {
            expand();
        }

        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        const SizeT hash = StringUtils::Hash(str, len);
        SizeT      *index;
        HAItem_    *item = find(index, str, len, hash);

        if (item != nullptr) {
            return item->Value;
        }

        return insert(index, static_cast<Key_ &&>(key), hash)->Value;
    }

    Value_ &operator[](const Key_ &key) {
        if (Size() == Capacity()) {
            expand();
        }

        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        const SizeT hash = StringUtils::Hash(str, len);
        SizeT      *index;
        HAItem_    *item = find(index, str, len, hash);

        if (item != nullptr) {
            return item->Value;
        }

        return insert(index, Key_(key), hash)->Value;
    }

    Value_ &GetOrAdd(const Char_T_ *key, const SizeT len) {
        if (Size() == Capacity()) {
            expand();
        }

        const SizeT hash = StringUtils::Hash(key, len);
        SizeT      *index;
        HAItem_    *item = find(index, key, len, hash);

        if (item != nullptr) {
            return item->Value;
        }

        return insert(index, Key_(key, len), hash)->Value;
    }

    void Insert(Key_ &&key, Value_ &&val) {
        if (Size() == Capacity()) {
            expand();
        }

        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        const SizeT hash = StringUtils::Hash(str, len);
        SizeT      *index;
        HAItem_    *item = find(index, str, len, hash);

        if (item == nullptr) {
            item = insert(index, static_cast<Key_ &&>(key), hash);
        }

        item->Value = static_cast<Value_ &&>(val);
    }

    inline Value_ *GetValue(const Key_ &key) const noexcept { return GetValue(key.First(), key.Length()); }

    Value_ *GetValue(const SizeT index) const noexcept {
        HAItem_ *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Value);
        }

        return nullptr;
    }

    Value_ *GetValue(const Char_T_ *key, const SizeT length) const noexcept {
        return GetValue(key, length, StringUtils::Hash(key, length));
    }

    Value_ *GetValue(const Char_T_ *key, const SizeT length, const SizeT hash) const noexcept {
        SizeT   *index;
        HAItem_ *item = find(index, key, length, hash);

        if (item != nullptr) {
            return &(item->Value);
        }

        return nullptr;
    }

    const Key_ *GetKey(const SizeT index) const noexcept {
        const HAItem_ *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Key);
        }

        return nullptr;
    }

    inline const HAItem_ *GetItem(const Key_ &key) const noexcept {
        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        return GetItem(str, len, StringUtils::Hash(str, len));
    }

    const HAItem_ *GetItem(const Char_T_ *key, const SizeT length, const SizeT hash) const noexcept {
        SizeT   *index;
        HAItem_ *item = find(index, key, length, hash);

        if (item != nullptr) {
            return item;
        }

        return nullptr;
    }

    const HAItem_ *GetItem(const SizeT index) const noexcept {
        const HAItem_ *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return (src + index);
        }

        return nullptr;
    }

    bool GetKeyIndex(SizeT &index, const Char_T_ *str, const SizeT len) const noexcept {
        SizeT *sub_index;
        if (find(sub_index, str, len, StringUtils::Hash(str, len)) != nullptr) {
            index = (*sub_index) - 1;

            return true;
        }

        return false;
    }

    inline void Remove(const Char_T_ *key) const noexcept { Remove(key, StringUtils::Count(key)); }

    inline void Remove(const Char_T_ *key, SizeT length) const noexcept {
        remove(key, length, StringUtils::Hash(key, length));
    }

    inline void Remove(const Key_ &key) const noexcept {
        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        remove(str, len, StringUtils::Hash(str, len));
    }

    void RemoveIndex(const SizeT index) const noexcept {
        if (index < Size()) {
            const HAItem_ *item = (Storage() + index);

            if (item->Hash != 0) {
                remove(item->Key.First(), item->Key.Length(), item->Hash);
            }
        }
    }

    /*
     * This function renames a key to a nonexisting one without changing the
     * order of the item, and returns true if successful.
     */
    bool Rename(const Key_ &from, Key_ &&to) const noexcept {
        if (Size() != 0) {
            HAItem_       *item;
            const Char_T_ *from_str = from.First();
            const Char_T_ *to_str;
            SizeT         *left_index;
            SizeT         *right_index;
            const SizeT    from_len = from.Length();
            SizeT          to_len;
            SizeT          to_hash;
            SizeT          offset;

            find(left_index, from_str, from_len, StringUtils::Hash(from_str, from_len));

            if (*left_index != 0) {
                to_str = to.First();
                to_len = to.Length();

                to_hash = StringUtils::Hash(to_str, to_len);

                find(right_index, to_str, to_len, to_hash);

                if (*right_index == 0) {
                    offset       = ((*left_index) - 1);
                    item         = (Storage() + offset);
                    *right_index = *left_index;
                    *left_index  = item->Next;
                    item->Next   = 0;
                    item->Hash   = to_hash;

                    item->Key = static_cast<Key_ &&>(to);
                    return true;
                }
            }
        }

        return false;
    }

    bool Rename(const Key_ &from, const Key_ &to) const { return Rename(from, Key_(to)); }

    void Reserve(SizeT size) {
        if (Storage() != nullptr) {
            Reset();
        }

        if (size != 0) {
            allocate(size);
        }
    }

    void Reset() noexcept {
        HAItem_ *storage = Storage();
        Memory::Dispose(storage, (storage + Size()));
        Memory::Deallocate(getHashTable());
        clearHashTable();
        setCapacity(0);
        setSize(0);
    }

    void Resize(const SizeT new_size) {
        if (new_size == 0) {
            Reset();
            return;
        }

        if (Size() > new_size) {
            // Shrink
            HAItem_ *storage = Storage();
            Memory::Dispose((storage + new_size), (storage + Size()));
            setSize(new_size);
        }

        resize(new_size);
    }

    // Set ascend to (false) for descend (ascend: 1,2,3; descend: 3,2,1 )
    void Sort(const bool ascend = true) noexcept {
        Memory::QuickSort<HAItem_, SizeT>::Sort(Storage(), 0, Size(), ascend);
        Memory::SetToZero(getHashTable(), (sizeof(SizeT) * Capacity()));
        generateHash();
    }

    // Removes excess storage.
    void Compress() {
        const SizeT size = ActualSize();

        if (size == 0) {
            Reset();
        } else {
            const SizeT n_cap = Memory::AligneSize(size);

            if ((size < Size()) || (n_cap < Capacity())) {
                resize(n_cap);
            }
        }
    }

    // Returns the actual number of items.
    SizeT ActualSize() const noexcept {
        const HAItem_ *item = Storage();
        const HAItem_ *end  = (item + Size());
        SizeT          size = 0;

        while (item < end) {
            size += (item->Hash != 0);
            ++item;
        }

        return size;
    }

    inline SizeT          Size() const noexcept { return index_; }
    inline SizeT          Capacity() const noexcept { return capacity_; }
    inline HAItem_       *Storage() const noexcept { return reinterpret_cast<HAItem_ *>(getHashTable() + Capacity()); }
    inline const HAItem_ *First() const noexcept { return Storage(); }
    inline const HAItem_ *End() const noexcept { return (First() + Size()); }
    inline bool           IsEmpty() const noexcept { return (Size() == 0); }
    inline bool           IsNotEmpty() const noexcept { return !(IsEmpty()); }

    // For STL
    inline const HAItem_ *begin() const noexcept { return First(); }
    inline const HAItem_ *end() const noexcept { return End(); }
    inline HAItem_       *begin() noexcept { return Storage(); }
    inline HAItem_       *end() noexcept { return (Storage() + Size()); }

    inline HAItem_ *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - 1));
        }

        return nullptr;
    }

    //////////// Private ////////////

  private:
    SizeT  getBase() const noexcept { return (Capacity() - 1); }
    SizeT *getHashTable() const noexcept { return hashTable_.GetPointer(); }
    void   setHashTable(SizeT *ptr) noexcept { hashTable_.SetPointer(ptr); }

    HAItem_ *allocate(SizeT new_cpacity) {
        new_cpacity = Memory::AligneSize(new_cpacity);
        setCapacity(new_cpacity);

        const SizeT size = ((sizeof(SizeT) + sizeof(HAItem_)) * new_cpacity);
        SizeT      *ht   = reinterpret_cast<SizeT *>(Memory::Allocate<char>(size));

        setHashTable(ht);
        Memory::SetToZero(ht, (sizeof(SizeT) * new_cpacity));

        return reinterpret_cast<HAItem_ *>(ht + Capacity());
    }

    void clearHashTable() noexcept { hashTable_.Reset(); }
    void setSize(const SizeT new_size) noexcept { index_ = new_size; }
    void setCapacity(const SizeT new_capacity) noexcept { capacity_ = new_capacity; }
    void expand() { resize(((Capacity() != 0) ? (Capacity() * SizeT{2}) : SizeT{2})); }

    HAItem_ *find(SizeT *&index, const Char_T_ *key, const SizeT length, const SizeT hash) const noexcept {
        SizeT   *ht      = getHashTable();
        HAItem_ *storage = reinterpret_cast<HAItem_ *>(ht + Capacity());
        HAItem_ *item;
        index     = (ht + (hash & getBase()));
        SizeT tmp = *index;

        while (tmp != 0) {
            item = (storage + (tmp - 1));

            if ((item->Hash == hash) && item->Key.IsEqual(key, length)) {
                return item;
            }

            index = &(item->Next);
            tmp   = *index;
        }

        return nullptr;
    }

    inline HAItem_ *insert(SizeT *index, Key_ &&key, const SizeT hash) noexcept {
        HAItem_ *item = (Storage() + Size());
        ++index_;
        *index = Size();

        item->Next = 0;
        item->Hash = hash;
        Memory::Initialize(&(item->Key), static_cast<Key_ &&>(key));
        Memory::Initialize(&(item->Value), Value_());

        return item;
    }

    void remove(const Char_T_ *key, const SizeT length, const SizeT hash) const noexcept {
        if (Size() != 0) {
            SizeT   *index;
            HAItem_ *item = find(index, key, length, hash);

            if (item != nullptr) {
                *index     = item->Next;
                item->Next = 0;
                item->Hash = 0;

                item->Key   = Key_();
                item->Value = Value_();
            }
        }
    }

    void copyTable(const HArray &src) {
        if (src.Size() != 0) {
            const HAItem_ *src_item     = src.First();
            const HAItem_ *src_end      = (src_item + src.Size());
            HAItem_       *storage_item = allocate(src.Size());
            const HAItem_ *storage_src  = storage_item;

            do {
                if (src_item->Hash != 0) {
                    Memory::Initialize(storage_item, *src_item);
                    ++storage_item;
                }

                ++src_item;
            } while (src_item < src_end);

            setSize(static_cast<SizeT>(storage_item - storage_src));
            generateHash();
        }
    }

    void resize(const SizeT new_size) {
        SizeT         *ht           = getHashTable();
        HAItem_       *src          = Storage();
        HAItem_       *storage      = allocate(new_size);
        HAItem_       *storage_item = storage;
        HAItem_       *item         = src;
        const HAItem_ *end          = (item + Size());

        while (item < end) {
            if (item->Hash != 0) {
                Memory::Initialize(storage_item, static_cast<HAItem_ &&>(*item));
                ++storage_item;
            }

            ++item;
        }

        setSize(static_cast<SizeT>(storage_item - storage));
        Memory::Deallocate(ht);
        generateHash();
    }

    void generateHash() const noexcept {
        SizeT         *ht   = getHashTable();
        HAItem_       *src  = Storage();
        HAItem_       *item = src;
        const HAItem_ *end  = (item + Size());
        SizeT         *index;
        SizeT          i    = 1;
        const SizeT    base = getBase();
        SizeT          tmp;

        while (item < end) {
            item->Next = 0;
            index      = (ht + (item->Hash & base));
            tmp        = *index;

            while (tmp != 0) {
                index = &((src + (tmp - 1))->Next);
                tmp   = *index;
            }

            *index = i;
            ++i;
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
