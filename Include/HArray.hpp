/*
 * Copyright (c) 2023 Hani Ammar
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

/*|-------------------------------------------|*/
/*|                Hash Table                 |*/
/*|____________________|______________________|*/
/*|     Positions      |       Items          |*/
/*| 0, 1 , 2, ... n-1  | item 0, item 1, ...  |*/
/*|____________________|______________________|*/

template <typename Value_T_, typename Char_T_>
struct HAItem_T_ {
    SizeT           Hash;
    SizeT           Next;
    String<Char_T_> Key;
    Value_T_        Value;

    inline bool operator<(const HAItem_T_ &item) const noexcept {
        return (Key < item.Key);
    }

    inline bool operator>(const HAItem_T_ &item) const noexcept {
        return (Key > item.Key);
    }
};
///////////////////////////////////////////////
template <typename HAItem_T_, bool BigEndian>
struct HArrayData;

template <typename HAItem_T_>
struct HArrayData<HAItem_T_, false> {
    // Little-Endian
    HArrayData() noexcept = default;

    HArrayData(HArrayData &&src) noexcept
        : Index{src.Index}, Capacity{src.Capacity}, HashTable{Memory::Move(src.HashTable)} {
        src.Index    = 0;
        src.Capacity = 0;
    }

    HArrayData(const HArrayData &)            = delete;
    HArrayData &operator=(HArrayData &&)      = delete;
    HArrayData &operator=(const HArrayData &) = delete;

    inline SizeT *GetHashTable() const noexcept {
        return HashTable.GetPointer();
    }

    inline HAItem_T_ *GetStorage(SizeT *hashTable) const noexcept {
        return Memory::ChangePointer<HAItem_T_>(hashTable + Capacity);
    }

    ~HArrayData() {
        SizeT     *hashTable = GetHashTable();
        HAItem_T_ *storage   = GetStorage(hashTable);
        Memory::Dispose(storage, (storage + Index));
        Memory::Deallocate(hashTable);
    }

    SizeT           Index{0};
    SizeT           Capacity{0};
    QPointer<SizeT> HashTable{};
};

template <typename HAItem_T_>
struct HArrayData<HAItem_T_, true> {
    // Big-Endian
    HArrayData() noexcept = default;

    HArrayData(HArrayData &&src) noexcept
        : HashTable{Memory::Move(src.HashTable)}, Index{src.Index}, Capacity{src.Capacity} {
        src.Index    = 0;
        src.Capacity = 0;
    }

    HArrayData(const HArrayData &)            = delete;
    HArrayData &operator=(HArrayData &&)      = delete;
    HArrayData &operator=(const HArrayData &) = delete;

    inline SizeT *GetHashTable() const noexcept {
        return HashTable.GetPointer();
    }

    inline HAItem_T_ *GetStorage(SizeT *hashTable) const noexcept {
        return Memory::ChangePointer<HAItem_T_>(hashTable + Capacity);
    }

    ~HArrayData() {
        SizeT     *hashTable = GetHashTable();
        HAItem_T_ *storage   = GetStorage(hashTable);
        Memory::Dispose(storage, (storage + Index));
        Memory::Deallocate(hashTable);
    }

    QPointer<SizeT> HashTable{};
    SizeT           Index{0};
    SizeT           Capacity{0};
};
///////////////////////////////////////////////
template <typename Value_T_, typename Char_T_>
class HArray {
    using HAItem_T = HAItem_T_<Value_T_, Char_T_>;
    using Key_T    = String<Char_T_>;

  public:
    HArray() noexcept             = default;
    HArray(HArray &&src) noexcept = default;
    ~HArray()                     = default;

    explicit HArray(SizeT size) {
        if (size != 0) {
            allocate(size);
        }
    }

    HArray(const HArray &src) {
        copyTable(src);
    }

    HArray &operator=(HArray &&src) noexcept {
        if (this != &src) {
            HAItem_T   *storage = Storage();
            SizeT      *ht      = data_.GetHashTable();
            const SizeT size    = Size();

            setSize(src.Size());
            setCapacity(src.Capacity());
            src.setSize(0);
            src.setCapacity(0);
            data_.HashTable.MovePointerOnly(src.data_.HashTable);

            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(ht);
        }

        return *this;
    }

    HArray &operator=(const HArray &src) {
        if (this != &src) {
            HAItem_T   *storage = Storage();
            SizeT      *ht      = data_.GetHashTable();
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
        const SizeT     n_size   = (Size() + src.Size());
        HAItem_T       *src_item = src.Storage();
        const HAItem_T *src_end  = (src_item + src.Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }

        while (src_item < src_end) {
            if (src_item->Hash != 0) {
                SizeT    *index;
                HAItem_T *storage_item = find(index, src_item->Key.First(), src_item->Key.Length(), src_item->Hash);

                if (storage_item == nullptr) {
                    storage_item = insert(index, Memory::Move(src_item->Key), src_item->Hash);
                } else {
                    Memory::Dispose(&(src_item->Key));
                }

                storage_item->Value = Memory::Move(src_item->Value);
            }

            ++src_item;
        }

        src.setCapacity(0);
        src.setSize(0);
        Memory::Deallocate(src.data_.GetHashTable());
        src.clearHashTable();
    }

    void operator+=(const HArray &src) {
        const SizeT     n_size   = (Size() + src.Size());
        const HAItem_T *src_item = src.First();
        const HAItem_T *src_end  = src_item + src.Size();

        if (n_size > Capacity()) {
            resize(n_size);
        }

        while (src_item < src_end) {
            if (src_item->Hash != 0) {
                SizeT    *index;
                HAItem_T *storage_item = find(index, src_item->Key.First(), src_item->Key.Length(), src_item->Hash);

                if (storage_item == nullptr) {
                    storage_item = insert(index, Key_T(src_item->Key), src_item->Hash);
                }

                storage_item->Value = Value_T_(src_item->Value);
            }

            ++src_item;
        }
    }

    Value_T_ &operator[](const Char_T_ *key) {
        return GetOrAdd(key, StringUtils::Count(key));
    }

    Value_T_ &operator[](Key_T &&key) {
        if (Size() == Capacity()) {
            expand();
        }

        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        const SizeT hash = StringUtils::Hash(str, len);
        SizeT      *index;
        HAItem_T   *item = find(index, str, len, hash);

        if (item != nullptr) {
            return item->Value;
        }

        return insert(index, Memory::Move(key), hash)->Value;
    }

    Value_T_ &operator[](const Key_T &key) {
        if (Size() == Capacity()) {
            expand();
        }

        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        const SizeT hash = StringUtils::Hash(str, len);
        SizeT      *index;
        HAItem_T   *item = find(index, str, len, hash);

        if (item != nullptr) {
            return item->Value;
        }

        return insert(index, Key_T(key), hash)->Value;
    }

    Value_T_ &GetOrAdd(const Char_T_ *key, const SizeT len) {
        if (Size() == Capacity()) {
            expand();
        }

        const SizeT hash = StringUtils::Hash(key, len);
        SizeT      *index;
        HAItem_T   *item = find(index, key, len, hash);

        if (item != nullptr) {
            return item->Value;
        }

        return insert(index, Key_T(key, len), hash)->Value;
    }

    void Insert(Key_T &&key, Value_T_ &&val) {
        if (Size() == Capacity()) {
            expand();
        }

        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        const SizeT hash = StringUtils::Hash(str, len);
        SizeT      *index;
        HAItem_T   *item = find(index, str, len, hash);

        if (item == nullptr) {
            item = insert(index, Memory::Move(key), hash);
        }

        item->Value = Memory::Move(val);
    }

    inline Value_T_ *GetValue(const Key_T &key) const noexcept {
        return GetValue(key.First(), key.Length());
    }

    Value_T_ *GetValue(const SizeT index) const noexcept {
        HAItem_T *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Value);
        }

        return nullptr;
    }

    Value_T_ *GetValue(const Char_T_ *key, const SizeT length) const noexcept {
        return GetValue(key, length, StringUtils::Hash(key, length));
    }

    Value_T_ *GetValue(const Char_T_ *key, const SizeT length, const SizeT hash) const noexcept {
        SizeT    *index;
        HAItem_T *item = find(index, key, length, hash);

        if (item != nullptr) {
            return &(item->Value);
        }

        return nullptr;
    }

    const Key_T *GetKey(const SizeT index) const noexcept {
        const HAItem_T *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Key);
        }

        return nullptr;
    }

    inline const HAItem_T *GetItem(const Key_T &key) const noexcept {
        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        return GetItem(str, len, StringUtils::Hash(str, len));
    }

    const HAItem_T *GetItem(const Char_T_ *key, const SizeT length, const SizeT hash) const noexcept {
        SizeT          *index;
        const HAItem_T *item = find(index, key, length, hash);

        if (item != nullptr) {
            return item;
        }

        return nullptr;
    }

    const HAItem_T *GetItem(const SizeT index) const noexcept {
        const HAItem_T *src = Storage();

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

    inline void Remove(const Char_T_ *key) const noexcept {
        Remove(key, StringUtils::Count(key));
    }

    inline void Remove(const Char_T_ *key, SizeT length) const noexcept {
        remove(key, length, StringUtils::Hash(key, length));
    }

    inline void Remove(const Key_T &key) const noexcept {
        const Char_T_ *str = key.First();
        const SizeT    len = key.Length();

        remove(str, len, StringUtils::Hash(str, len));
    }

    void RemoveIndex(const SizeT index) const noexcept {
        if (index < Size()) {
            const HAItem_T *item = (Storage() + index);

            if (item->Hash != 0) {
                remove(item->Key.First(), item->Key.Length(), item->Hash);
            }
        }
    }

    /*
     * This function renames a key to a nonexisting one without changing the
     * order of the item, and returns true if successful.
     */
    bool Rename(const Key_T &from, Key_T &&to) const noexcept {
        if (Size() != 0) {
            HAItem_T      *item;
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

                    item->Key = Memory::Move(to);
                    return true;
                }
            }
        }

        return false;
    }

    bool Rename(const Key_T &from, const Key_T &to) const {
        return Rename(from, Key_T(to));
    }

    void Reserve(SizeT size) {
        if (Storage() != nullptr) {
            Reset();
        }

        if (size != 0) {
            allocate(size);
        }
    }

    void Reset() noexcept {
        HAItem_T *storage = Storage();
        Memory::Dispose(storage, (storage + Size()));
        Memory::Deallocate(data_.GetHashTable());
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
            HAItem_T *storage = Storage();
            Memory::Dispose((storage + new_size), (storage + Size()));
            setSize(new_size);
        }

        resize(new_size);
    }

    // Set ascend to (false) for descend (ascend: 1,2,3; descend: 3,2,1 )
    void Sort(const bool ascend = true) noexcept {
        constexpr SizeT32 size = sizeof(SizeT);

        if (ascend) {
            Memory::Sort<true>(Storage(), SizeT{0}, Size());
        } else {
            Memory::Sort<false>(Storage(), SizeT{0}, Size());
        }

        Memory::SetToZero<size>(data_.GetHashTable(), (size * Capacity()));
        generateHash();
    }

    // Removes excess storage.
    void Compress() {
        const SizeT size = ActualSize();

        if (size != 0) {
            if (size < Size()) {
                resize(Memory::AlignSize(size));
            }

            return;
        }

        Reset();
    }

    // Returns the actual number of items.
    SizeT ActualSize() const noexcept {
        const HAItem_T *item = Storage();
        const HAItem_T *end  = (item + Size());
        SizeT           size = 0;

        while (item < end) {
            size += (item->Hash != 0);
            ++item;
        }

        return size;
    }

    inline SizeT Size() const noexcept {
        return data_.Index;
    }

    inline SizeT Capacity() const noexcept {
        return data_.Capacity;
    }

    inline HAItem_T *Storage() const noexcept {
        return data_.GetStorage(data_.GetHashTable());
    }

    inline const HAItem_T *First() const noexcept {
        return Storage();
    }

    inline const HAItem_T *End() const noexcept {
        return (First() + Size());
    }

    inline bool IsEmpty() const noexcept {
        return (Size() == 0);
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // For STL
    inline const HAItem_T *begin() const noexcept {
        return First();
    }

    inline const HAItem_T *end() const noexcept {
        return End();
    }

    inline HAItem_T *begin() noexcept {
        return Storage();
    }

    inline HAItem_T *end() noexcept {
        return (Storage() + Size());
    }

    inline HAItem_T *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - 1));
        }

        return nullptr;
    }

    //////////// Private ////////////

  private:
    inline SizeT getBase() const noexcept {
        return (Capacity() - 1);
    }

    inline void setHashTable(SizeT *ptr) noexcept {
        data_.HashTable.SetPointer(ptr);
    }

    HAItem_T *allocate(SizeT new_capacity) {
        constexpr SizeT32 size     = sizeof(SizeT);
        constexpr SizeT   size_sum = SizeT{size + sizeof(HAItem_T)};

        if (new_capacity < SizeT{2}) {
            new_capacity = SizeT{2};
        }

        new_capacity = Memory::AlignSize(new_capacity);
        setCapacity(new_capacity);
        SizeT *ht = Memory::ChangePointer<SizeT>(Memory::Allocate<char>((size_sum * new_capacity)));
        setHashTable(ht);
        Memory::SetToZero<size>(ht, (size * new_capacity));

        return Memory::ChangePointer<HAItem_T>(ht + Capacity());
    }

    inline void clearHashTable() noexcept {
        data_.HashTable.Reset();
    }

    inline void setSize(const SizeT new_size) noexcept {
        data_.Index = new_size;
    }

    inline void setCapacity(const SizeT new_capacity) noexcept {
        data_.Capacity = new_capacity;
    }

    inline void expand() {
        resize(((Capacity() != 0) ? (Capacity() * SizeT{2}) : SizeT{2}));
    }

    HAItem_T *insert(SizeT *index, Key_T &&key, const SizeT hash) noexcept {
        HAItem_T *item = (Storage() + Size());
        ++(data_.Index);
        *index = Size();

        item->Next = 0;
        item->Hash = hash;
        Memory::Initialize(&(item->Key), Memory::Move(key));
        Memory::Initialize(&(item->Value));

        return item;
    }

    void remove(const Char_T_ *key, const SizeT length, const SizeT hash) const noexcept {
        if (Size() != 0) {
            SizeT    *index;
            HAItem_T *item = find(index, key, length, hash);

            if (item != nullptr) {
                *index     = item->Next;
                item->Next = 0;
                item->Hash = 0;

                item->Key   = Key_T{};
                item->Value = Value_T_{};
            }
        }
    }

    void copyTable(const HArray &src) {
        if (src.Size() != 0) {
            const HAItem_T *src_item = src.First();
            const HAItem_T *src_end  = (src_item + src.Size());
            HAItem_T       *storage  = allocate(src.Size());
            SizeT           len      = 0;

            do {
                if (src_item->Hash != 0) {
                    Memory::Initialize(storage, *src_item);
                    ++storage;
                    ++len;
                }

                ++src_item;
            } while (src_item < src_end);

            setSize(len);
            generateHash();
        }
    }

    void resize(const SizeT new_size) {
        SizeT          *ht      = data_.GetHashTable();
        HAItem_T       *src     = Storage();
        HAItem_T       *storage = allocate(new_size);
        HAItem_T       *item    = src;
        const HAItem_T *end     = (item + Size());
        SizeT           len     = 0;

        while (item < end) {
            if (item->Hash != 0) {
                Memory::Initialize(storage, Memory::Move(*item));
                ++storage;
                ++len;
            }

            ++item;
        }

        setSize(len);
        Memory::Deallocate(ht);
        generateHash();
    }

    HAItem_T *find(SizeT *&index, const Char_T_ *key, const SizeT length, const SizeT hash) const noexcept {
        SizeT    *ht      = data_.GetHashTable();
        HAItem_T *storage = Memory::ChangePointer<HAItem_T>(ht + Capacity());
        HAItem_T *item;
        index = (ht + (hash & getBase()));

        while (*index != 0) {
            item = (storage + *index);
            --item;

            if ((item->Hash == hash) && item->Key.IsEqual(key, length)) {
                return item;
            }

            index  = &(item->Next);
            *index = *index;
        }

        return nullptr;
    }

    void generateHash() const noexcept {
        SizeT          *ht   = data_.GetHashTable();
        HAItem_T       *src  = Storage();
        HAItem_T       *item = src;
        const HAItem_T *end  = (item + Size());
        SizeT          *index;
        SizeT           i    = 1;
        const SizeT     base = getBase();

        while (item < end) {
            item->Next = 0;
            index      = (ht + (item->Hash & base));

            while (*index != 0) {
                index = &((src + (*index - SizeT{1}))->Next);
            }

            *index = i;
            ++i;
            ++item;
        }
    }

    HArrayData<HAItem_T, Config::IsBigEndian> data_{};
};

} // namespace Qentem

#endif
