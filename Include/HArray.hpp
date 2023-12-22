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

#ifndef QENTEM_HARRAY_H
#define QENTEM_HARRAY_H

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

template <typename Value_T, typename Char_T>
struct HAItem_T {
    SizeT          Hash;
    SizeT          Next;
    String<Char_T> Key;
    Value_T        Value;

    inline bool operator<(const HAItem_T &item) const noexcept {
        return (Key < item.Key);
    }

    inline bool operator>(const HAItem_T &item) const noexcept {
        return (Key > item.Key);
    }
};
///////////////////////////////////////////////
template <typename, bool BigEndian>
struct HArrayData;

template <typename HAItem_T>
struct HArrayData<HAItem_T, false> {
    // Little-Endian
    HArrayData() noexcept = default;

    HArrayData(HArrayData &&src) noexcept
        : Index{src.Index}, Capacity{src.Capacity}, HashTable{Memory::Move(src.HashTable)} {
        src.Capacity = 0;
        src.Index    = 0;
    }

    HArrayData(const HArrayData &)            = delete;
    HArrayData &operator=(HArrayData &&)      = delete;
    HArrayData &operator=(const HArrayData &) = delete;

    inline SizeT *GetHashTable() const noexcept {
        return HashTable.GetPointer();
    }

    inline HAItem_T *GetStorage(SizeT *hashTable) const noexcept {
        return Memory::ChangePointer<HAItem_T>(hashTable + Capacity);
    }

    ~HArrayData() {
        SizeT    *hashTable = GetHashTable();
        HAItem_T *storage   = GetStorage(hashTable);
        Memory::Dispose(storage, (storage + Index));
        Memory::Deallocate(hashTable);
    }

    SizeT           Index{0};
    SizeT           Capacity{0};
    QPointer<SizeT> HashTable{};
};

template <typename HAItem_T>
struct HArrayData<HAItem_T, true> {
    // Big-Endian
    HArrayData() noexcept = default;

    HArrayData(HArrayData &&src) noexcept
        : HashTable{Memory::Move(src.HashTable)}, Capacity{src.Capacity}, Index{src.Index} {
        src.Index    = 0;
        src.Capacity = 0;
    }

    HArrayData(const HArrayData &)            = delete;
    HArrayData &operator=(HArrayData &&)      = delete;
    HArrayData &operator=(const HArrayData &) = delete;

    inline SizeT *GetHashTable() const noexcept {
        return HashTable.GetPointer();
    }

    inline HAItem_T *GetStorage(SizeT *hashTable) const noexcept {
        return Memory::ChangePointer<HAItem_T>(hashTable + Capacity);
    }

    ~HArrayData() {
        SizeT    *hashTable = GetHashTable();
        HAItem_T *storage   = GetStorage(hashTable);
        Memory::Dispose(storage, (storage + Index));
        Memory::Deallocate(hashTable);
    }

    QPointer<SizeT> HashTable{};
    SizeT           Capacity{0};
    SizeT           Index{0};
};
///////////////////////////////////////////////
template <typename Value_T, typename Char_T>
struct HArray {
    using HAItem = HAItem_T<Value_T, Char_T>;
    using Key_T  = String<Char_T>;

    HArray() noexcept             = default;
    HArray(HArray &&src) noexcept = default;
    ~HArray()                     = default;

    explicit HArray(SizeT size) {
        if (size != SizeT{0}) {
            allocate(size);
        }
    }

    HArray(const HArray &src) {
        copyTable(src);
    }

    HArray &operator=(HArray &&src) noexcept {
        if (this != &src) {
            HAItem     *storage = Storage();
            SizeT      *ht      = _data.GetHashTable();
            const SizeT size    = Size();

            _data.HashTable.MovePointerOnly(src._data.HashTable);
            setCapacity(src.Capacity());
            setSize(src.Size());
            src.setSize(SizeT{0});
            src.setCapacity(SizeT{0});

            // Just in case the copied array is not a child array, do this last.
            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(ht);
        }

        return *this;
    }

    HArray &operator=(const HArray &src) {
        if (this != &src) {
            HAItem     *storage = Storage();
            SizeT      *ht      = _data.GetHashTable();
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
        const SizeT   n_size   = (Size() + src.Size());
        HAItem       *src_item = src.Storage();
        const HAItem *src_end  = (src_item + src.Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }

        while (src_item < src_end) {
            if (src_item->Hash != SizeT{0}) {
                SizeT      *index;
                const SizeT length       = src_item->Key.Length();
                HAItem     *storage_item = find(index, src_item->Key.Storage(length), length, src_item->Hash);

                if (storage_item == nullptr) {
                    storage_item = insert(index, Memory::Move(src_item->Key), src_item->Hash);
                } else {
                    Memory::Dispose(&(src_item->Key));
                }

                storage_item->Value = Memory::Move(src_item->Value);
            }

            ++src_item;
        }

        Memory::Deallocate(src._data.GetHashTable());
        src.clearHashTable();
        src.setCapacity(SizeT{0});
        src.setSize(SizeT{0});
    }

    void operator+=(const HArray &src) {
        const SizeT   n_size   = (Size() + src.Size());
        const HAItem *src_item = src.First();
        const HAItem *src_end  = src_item + src.Size();

        if (n_size > Capacity()) {
            resize(n_size);
        }

        while (src_item < src_end) {
            if (src_item->Hash != SizeT{0}) {
                const SizeT length = src_item->Key.Length();
                SizeT      *index;
                HAItem     *storage_item = find(index, src_item->Key.Storage(length), length, src_item->Hash);

                if (storage_item == nullptr) {
                    storage_item = insert(index, Key_T(src_item->Key), src_item->Hash);
                }

                storage_item->Value = Value_T(src_item->Value);
            }

            ++src_item;
        }
    }

    Value_T &operator[](const Char_T *key) {
        return Get(key, StringUtils::Count(key));
    }

    Value_T &operator[](Key_T &&key) {
        if (Size() == Capacity()) {
            expand();
        }

        const SizeT   len = key.Length();
        const Char_T *str = key.Storage(len);

        const SizeT hash = StringUtils::Hash(str, len);
        SizeT      *index;
        HAItem     *item = find(index, str, len, hash);

        if (item != nullptr) {
            return item->Value;
        }

        return insert(index, Memory::Move(key), hash)->Value;
    }

    Value_T &operator[](const Key_T &key) {
        if (Size() == Capacity()) {
            expand();
        }

        const SizeT   len = key.Length();
        const Char_T *str = key.Storage(len);

        const SizeT hash = StringUtils::Hash(str, len);
        SizeT      *index;
        HAItem     *item = find(index, str, len, hash);

        if (item != nullptr) {
            return item->Value;
        }

        return insert(index, Key_T(key), hash)->Value;
    }

    Value_T &Get(const Char_T *key, const SizeT len) {
        if (Size() == Capacity()) {
            expand();
        }

        const SizeT hash = StringUtils::Hash(key, len);
        SizeT      *index;
        HAItem     *item = find(index, key, len, hash);

        if (item != nullptr) {
            return item->Value;
        }

        return insert(index, Key_T(key, len), hash)->Value;
    }

    void Insert(Key_T &&key, Value_T &&val) {
        if (Size() == Capacity()) {
            expand();
        }

        const SizeT   len = key.Length();
        const Char_T *str = key.Storage(len);

        const SizeT hash = StringUtils::Hash(str, len);
        SizeT      *index;
        HAItem     *item = find(index, str, len, hash);

        if (item == nullptr) {
            item = insert(index, Memory::Move(key), hash);
        }

        item->Value = Memory::Move(val);
    }

    inline Value_T *GetValue(const Key_T &key) const noexcept {
        const SizeT len = key.Length();
        return GetValue(key.Storage(len), len);
    }

    Value_T *GetValue(const SizeT index) const noexcept {
        HAItem *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != SizeT{0})) {
            return &((src + index)->Value);
        }

        return nullptr;
    }

    Value_T *GetValue(const Char_T *key, const SizeT length) const noexcept {
        return GetValue(key, length, StringUtils::Hash(key, length));
    }

    Value_T *GetValue(const Char_T *key, const SizeT length, const SizeT hash) const noexcept {
        SizeT  *index;
        HAItem *item = find(index, key, length, hash);

        if (item != nullptr) {
            return &(item->Value);
        }

        return nullptr;
    }

    const Key_T *GetKey(const SizeT index) const noexcept {
        const HAItem *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != SizeT{0})) {
            return &((src + index)->Key);
        }

        return nullptr;
    }

    inline const HAItem *GetItem(const Key_T &key) const noexcept {
        const SizeT   len = key.Length();
        const Char_T *str = key.Storage(len);

        return GetItem(str, len, StringUtils::Hash(str, len));
    }

    const HAItem *GetItem(const Char_T *key, const SizeT length, const SizeT hash) const noexcept {
        SizeT        *index;
        const HAItem *item = find(index, key, length, hash);

        if (item != nullptr) {
            return item;
        }

        return nullptr;
    }

    const HAItem *GetItem(const SizeT index) const noexcept {
        const HAItem *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != SizeT{0})) {
            return (src + index);
        }

        return nullptr;
    }

    bool GetKeyIndex(SizeT &index, const Char_T *str, const SizeT len) const noexcept {
        SizeT *sub_index;
        if (find(sub_index, str, len, StringUtils::Hash(str, len)) != nullptr) {
            index = *sub_index;
            --index;

            return true;
        }

        return false;
    }

    inline void Remove(const Char_T *key) const noexcept {
        Remove(key, StringUtils::Count(key));
    }

    inline void Remove(const Char_T *key, SizeT length) const noexcept {
        remove(key, length, StringUtils::Hash(key, length));
    }

    inline void Remove(const Key_T &key) const noexcept {
        const SizeT   len = key.Length();
        const Char_T *str = key.Storage(len);

        remove(str, len, StringUtils::Hash(str, len));
    }

    void RemoveIndex(const SizeT index) const noexcept {
        if (index < Size()) {
            const HAItem *item = (Storage() + index);

            if (item->Hash != SizeT{0}) {
                const SizeT len = item->Key.Length();
                remove(item->Key.Storage(len), len, item->Hash);
            }
        }
    }

    /*
     * This function renames a key to a nonexisting one without changing the
     * order of the item, and returns true if successful.
     */
    bool Rename(const Key_T &from, Key_T &&to) const noexcept {
        if (Size() != SizeT{0}) {
            HAItem       *item;
            const SizeT   from_len = from.Length();
            SizeT         to_len;
            const Char_T *from_str = from.Storage(from_len);
            const Char_T *to_str;
            SizeT        *left_index;
            SizeT        *right_index;
            SizeT         to_hash;
            SizeT         offset;

            find(left_index, from_str, from_len, StringUtils::Hash(from_str, from_len));

            if (*left_index != SizeT{0}) {
                to_len = to.Length();
                to_str = to.Storage(to_len);

                to_hash = StringUtils::Hash(to_str, to_len);

                find(right_index, to_str, to_len, to_hash);

                if (*right_index == SizeT{0}) {
                    offset = *left_index;
                    --offset;

                    item         = (Storage() + offset);
                    *right_index = *left_index;
                    *left_index  = item->Next;
                    item->Next   = SizeT{0};
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

        if (size != SizeT{0}) {
            allocate(size);
        }
    }

    void Reset() noexcept {
        HAItem *storage = Storage();
        Memory::Dispose(storage, (storage + Size()));
        Memory::Deallocate(_data.GetHashTable());
        clearHashTable();
        setCapacity(SizeT{0});
        setSize(SizeT{0});
    }

    void Resize(const SizeT new_size) {
        if (new_size == SizeT{0}) {
            Reset();
            return;
        }

        if (Size() > new_size) {
            // Shrink
            HAItem *storage = Storage();
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

        Memory::SetToZero(_data.GetHashTable(), (size * Capacity()));
        generateHash();
    }

    // Removes excess storage.
    void Compress() {
        const SizeT size = ActualSize();

        if (size != SizeT{0}) {
            if (size < Size()) {
                resize(Memory::AlignSize(size));
            }

            return;
        }

        Reset();
    }

    // Returns the actual number of items.
    SizeT ActualSize() const noexcept {
        const HAItem *item = Storage();
        const HAItem *end  = (item + Size());
        SizeT         size = 0;

        while (item < end) {
            if (item->Hash != SizeT{0}) {
                ++size;
            }

            ++item;
        }

        return size;
    }

    inline SizeT Size() const noexcept {
        return _data.Index;
    }

    inline SizeT Capacity() const noexcept {
        return _data.Capacity;
    }

    inline HAItem *Storage() const noexcept {
        return _data.GetStorage(_data.GetHashTable());
    }

    inline const HAItem *First() const noexcept {
        return Storage();
    }

    inline HAItem *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - SizeT{1}));
        }

        return nullptr;
    }

    inline const HAItem *End() const noexcept {
        return (First() + Size());
    }

    inline bool IsEmpty() const noexcept {
        return (Size() == SizeT{0});
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    inline SizeT8 GetHighByte() const noexcept {
        return _data.HashTable.GetHighByte();
    }

    inline void SetHighByte(SizeT8 byte) noexcept {
        _data.HashTable.SetHighByte(byte);
    }

    inline SizeT8 GetLowByte() const noexcept {
        return _data.HashTable.GetLowByte();
    }

    inline void SetLowByte(SizeT8 byte) noexcept {
        _data.HashTable.SetLowByte(byte);
    }

    // For STL
    inline const HAItem *begin() const noexcept {
        return First();
    }

    inline const HAItem *end() const noexcept {
        return End();
    }

    inline HAItem *begin() noexcept {
        return Storage();
    }

    inline HAItem *end() noexcept {
        return (Storage() + Size());
    }

    //////////// Private ////////////

  private:
    inline SizeT getBase() const noexcept {
        SizeT base = Capacity();
        --base;

        return base;
    }

    inline void setHashTable(SizeT *ptr) noexcept {
        _data.HashTable.SetPointer(ptr);
    }

    HAItem *allocate(SizeT new_capacity) {
        constexpr SizeT32 size     = sizeof(SizeT);
        constexpr SizeT   size_sum = SizeT{size + sizeof(HAItem)};

        if (new_capacity < SizeT{2}) {
            new_capacity = SizeT{2};
        }

        new_capacity = Memory::AlignSize(new_capacity);
        setCapacity(new_capacity);
        SizeT *ht = Memory::ChangePointer<SizeT>(Memory::Allocate<char>((size_sum * new_capacity)));
        setHashTable(ht);
        Memory::SetToZero(ht, (size * new_capacity));

        return Memory::ChangePointer<HAItem>(ht + Capacity());
    }

    inline void clearHashTable() noexcept {
        _data.HashTable.Reset();
    }

    inline void setSize(const SizeT new_size) noexcept {
        _data.Index = new_size;
    }

    inline void setCapacity(const SizeT new_capacity) noexcept {
        _data.Capacity = new_capacity;
    }

    inline void expand() {
        resize(((Capacity() != SizeT{0}) ? (Capacity() * SizeT{2}) : SizeT{2}));
    }

    HAItem *insert(SizeT *index, Key_T &&key, const SizeT hash) noexcept {
        HAItem *item = (Storage() + Size());
        ++(_data.Index);
        *index = Size();

        item->Next = SizeT{0};
        item->Hash = hash;
        Memory::Initialize(&(item->Key), Memory::Move(key));
        Memory::Initialize(&(item->Value));

        return item;
    }

    void remove(const Char_T *key, const SizeT length, const SizeT hash) const noexcept {
        if (Size() != SizeT{0}) {
            SizeT  *index;
            HAItem *item = find(index, key, length, hash);

            if (item != nullptr) {
                *index     = item->Next;
                item->Next = SizeT{0};
                item->Hash = SizeT{0};

                item->Key   = Key_T{};
                item->Value = Value_T{};
            }
        }
    }

    void copyTable(const HArray &src) {
        if (src.Size() != SizeT{0}) {
            const HAItem *src_item = src.First();
            const HAItem *src_end  = (src_item + src.Size());
            HAItem       *storage  = allocate(src.Size());
            SizeT         len      = 0;

            do {
                if (src_item->Hash != SizeT{0}) {
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
        SizeT        *ht      = _data.GetHashTable();
        HAItem       *src     = Storage();
        HAItem       *storage = allocate(new_size);
        HAItem       *item    = src;
        const HAItem *end     = (item + Size());
        _data.Index           = SizeT{0};

        while (item < end) {
            if (item->Hash != SizeT{0}) {
                Memory::Initialize(storage, Memory::Move(*item));
                ++storage;
                ++(_data.Index);
            }

            ++item;
        }

        Memory::Deallocate(ht);
        generateHash();
    }

    HAItem *find(SizeT *&index, const Char_T *key, const SizeT length, const SizeT hash) const noexcept {
        SizeT  *ht      = _data.GetHashTable();
        HAItem *storage = Memory::ChangePointer<HAItem>(ht + Capacity());
        HAItem *item;
        index = (ht + (hash & getBase()));

        while (*index != SizeT{0}) {
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
        SizeT        *ht   = _data.GetHashTable();
        HAItem       *src  = Storage();
        HAItem       *item = src;
        const HAItem *end  = (item + Size());
        SizeT        *index;
        SizeT         i    = SizeT{1};
        const SizeT   base = getBase();

        while (item < end) {
            item->Next = SizeT{0};
            index      = (ht + (item->Hash & base));

            while (*index != SizeT{0}) {
                index = &((src + (*index - SizeT{1}))->Next);
            }

            *index = i;
            ++i;
            ++item;
        }
    }

    HArrayData<HAItem, Config::IsBigEndian> _data{};
};

} // namespace Qentem

#endif
