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

#ifndef QENTEM_HASH_TABLE_H
#define QENTEM_HASH_TABLE_H

#include "Memory.hpp"
#include "StringUtils.hpp"

namespace Qentem {

template <typename, typename>
struct HArray;

template <typename>
struct HList;
///////////////////////////////////////////////
// This is just the base for HArray and HList.

template <typename Key_T, typename HItem>
struct HashTable {
    using Char_T = typename Key_T::CharType;

    HashTable() noexcept = default;

    explicit HashTable(const SizeT size) {
        if (size != 0) {
            allocate(size);
        }
    }

    HashTable(HashTable &&src) noexcept : hashTable_{src.hashTable_}, index_{src.index_}, capacity_{src.capacity_} {
        src.setSize(0);
        src.setCapacity(0);
        src.clearHashTable();
    }

    HashTable(const HashTable &src) {
        copyTableWithHash(src);
    }

    ~HashTable() {
        SizeT *hashTable = getHashTable();
        HItem *storage   = Storage();
        Memory::Dispose(storage, (storage + Size()));
        Memory::Deallocate(hashTable);
    }

    HashTable &operator=(HashTable &&src) noexcept {
        if (this != &src) {
            SizeT      *ht      = getHashTable();
            HItem      *storage = Storage();
            const SizeT size    = Size();

            setHashTable(src.getHashTable());
            setSize(src.Size());
            setCapacity(src.Capacity());

            src.clearHashTable();
            src.setSize(0);
            src.setCapacity(0);

            // Just in case the copied array is not a child array, do this last.
            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(ht);
        }

        return *this;
    }

    HashTable &operator=(const HashTable &src) {
        if (this != &src) {
            SizeT      *ht      = getHashTable();
            HItem      *storage = Storage();
            const SizeT size    = Size();

            clearHashTable();
            setSize(0);
            setCapacity(0);

            copyTable(src);

            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(ht);
        }

        return *this;
    }

    bool Has(const Char_T *key, const SizeT length) const noexcept {
        if (IsNotEmpty()) {
            const SizeT hash = StringUtils::Hash(key, length);
            SizeT      *index;
            HItem      *item = find(index, key, length, hash);

            return (item != nullptr);
        }

        return false;
    }

    bool Has(const Key_T &key) const noexcept {
        return Has(key.First(), key.Length());
    }

    const Key_T *GetKey(const SizeT index) const noexcept {
        const HItem *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return &((src + index)->Key);
        }

        return nullptr;
    }

    const HItem *GetItem(const Char_T *key, const SizeT length, const SizeT hash) const noexcept {
        if (IsNotEmpty()) {
            SizeT       *index;
            const HItem *item = find(index, key, length, hash);

            if (item != nullptr) {
                return item;
            }
        }

        return nullptr;
    }

    inline const HItem *GetItem(const Key_T &key) const noexcept {
        return GetItem(key.First(), key.Length(), StringUtils::Hash(key.First(), key.Length()));
    }

    const HItem *GetItem(const SizeT index) const noexcept {
        const HItem *src = Storage();

        if ((index < Size()) && ((src + index)->Hash != 0)) {
            return (src + index);
        }

        return nullptr;
    }

    bool GetKeyIndex(SizeT &index, const Char_T *str, const SizeT length) const noexcept {
        if (IsNotEmpty()) {
            SizeT *tmp;

            if (find(tmp, str, length, StringUtils::Hash(str, length)) != nullptr) {
                index = *tmp;
                --index;

                return true;
            }
        }

        return false;
    }

    bool GetKeyIndex(SizeT &index, const Key_T &key) const noexcept {
        return GetKeyIndex(index, key.First(), key.Length());
    }

    inline void Remove(const Char_T *key) const noexcept {
        Remove(key, StringUtils::Count(key));
    }

    inline void Remove(const Char_T *key, SizeT length) const noexcept {
        remove(key, length, StringUtils::Hash(key, length));
    }

    inline void Remove(const Key_T &key) const noexcept {
        remove(key.First(), key.Length(), StringUtils::Hash(key.First(), key.Length()));
    }

    void RemoveIndex(const SizeT index) const noexcept {
        if (index < Size()) {
            const HItem *item = (Storage() + index);

            if (item->Hash != 0) {
                remove(item->Key.First(), item->Key.Length(), item->Hash);
            }
        }
    }

    /*
     * This function renames a key to a nonexisting one without changing the
     * order of its item, and returns true if successful.
     */
    bool Rename(const Key_T &from, Key_T &&to) const noexcept {
        if (IsNotEmpty()) {
            SizeT *left_index;
            SizeT *right_index;

            find(left_index, from.First(), from.Length(), StringUtils::Hash(from.First(), from.Length()));

            if (*left_index != 0) {
                const SizeT to_hash = StringUtils::Hash(to.First(), to.Length());

                find(right_index, to.First(), to.Length(), to_hash);

                if (*right_index == 0) {
                    SizeT index = *left_index;
                    --index;

                    HItem *item  = (Storage() + index);
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
        return Rename(from, Key_T{to});
    }

    void Reserve(SizeT size) {
        Reset();

        if (size != 0) {
            allocate(size);
        }
    }

    void Clear() noexcept {
        if (IsNotEmpty()) {
            constexpr SizeT32 size    = sizeof(SizeT);
            SizeT            *ht      = getHashTable();
            HItem            *storage = Storage();
            Memory::SetToZero(ht, (size * Capacity()));
            Memory::Dispose(storage, (storage + Size()));
            setSize(0);
        }
    }

    void Reset() noexcept {
        if (Capacity() != 0) {
            HItem *storage = Storage();

            Memory::Dispose(storage, (storage + Size()));
            Memory::Deallocate(getHashTable());

            clearHashTable();
            setSize(0);
            setCapacity(0);
        }
    }

    void Resize(const SizeT new_size) {
        if (new_size == 0) {
            Reset();
            return;
        }

        if (Size() > new_size) {
            // Shrink
            HItem *storage = Storage();
            Memory::Dispose((storage + new_size), (storage + Size()));

            setSize(new_size);
        }

        resize(new_size);
    }

    void Expect(const SizeT count) {
        const SizeT new_size = (count + Size());

        if (new_size > Capacity()) {
            resize(new_size);
        }
    }

    // Set ascend to (false) for descend (ascend: 1,2,3; descend: 3,2,1 )
    void Sort(const bool ascend = true) noexcept {
        constexpr SizeT32 size = sizeof(SizeT);

        if (ascend) {
            Memory::Sort<true>(Storage(), SizeT{0}, Size());
        } else {
            Memory::Sort<false>(Storage(), SizeT{0}, Size());
        }

        Memory::SetToZero(getHashTable(), (size * Capacity()));
        generateHash();
    }

    // Removes excess storage.
    void Compress() {
        const SizeT size = ActualSize();

        if (size != 0) {
            if (size < Size()) {
                resize(size);
            }

            return;
        }

        Reset();
    }

    // Returns the actual number of items.
    SizeT ActualSize() const noexcept {
        const HItem *item = Storage();
        const HItem *end  = (item + Size());
        SizeT        size = 0;

        while (item < end) {
            if (item->Hash != 0) {
                ++size;
            }

            ++item;
        }

        return size;
    }

    inline SizeT Size() const noexcept {
        return index_;
    }

    inline SizeT Capacity() const noexcept {
        return capacity_;
    }

    inline HItem *Storage() const noexcept {
        return Memory::ChangePointer<HItem>(getHashTable() + Capacity());
    }

    inline const HItem *First() const noexcept {
        return Storage();
    }

    inline HItem *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - SizeT{1}));
        }

        return nullptr;
    }

    inline const HItem *End() const noexcept {
        return (First() + Size());
    }

    inline bool IsEmpty() const noexcept {
        return (Size() == 0);
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // For STL
    inline const HItem *begin() const noexcept {
        return First();
    }

    inline const HItem *end() const noexcept {
        return End();
    }

    inline HItem *begin() noexcept {
        return Storage();
    }

    inline HItem *end() noexcept {
        return (Storage() + Size());
    }

    //////////// Private ////////////
  private:
    inline SizeT getBase() const noexcept {
        SizeT base = Capacity();
        --base;

        return base;
    }

    inline HItem *allocate(SizeT capacity) {
        constexpr SizeT32 size = sizeof(SizeT);

        SizeT *ht = allocateOnly(capacity);

        Memory::SetToZero(ht, (size * Capacity()));

        return Memory::ChangePointer<HItem>(ht + Capacity());
    }

    inline SizeT *allocateOnly(SizeT capacity) {
        constexpr SizeT32 size     = sizeof(SizeT);
        constexpr SizeT   size_sum = SizeT{size + sizeof(HItem)};

        // Making sure 'size' is not an odd number.
        capacity += (capacity & SizeT{1});
        capacity = Memory::AlignSize(capacity);

        setCapacity(capacity);

        SizeT *ht = Memory::ChangePointer<SizeT>(Memory::Allocate<char>((size_sum * capacity)));

        setHashTable(ht);

        return ht;
    }

    inline void setHashTable(SizeT *ptr) noexcept {
        hashTable_ = ptr;
    }

    inline SizeT *getHashTable() const noexcept {
        return hashTable_;
    }

    inline void clearHashTable() noexcept {
        hashTable_ = nullptr;
    }

    inline void setSize(const SizeT new_size) noexcept {
        index_ = new_size;
    }

    inline void setCapacity(const SizeT capacity) noexcept {
        capacity_ = capacity;
    }

    inline void expand() {
        resize((SizeT(Capacity() == 0) + Capacity()) * SizeT{2});
    }

    HItem *insert(SizeT *index, Key_T &&key, const SizeT hash) noexcept {
        HItem *item = (Storage() + Size());
        ++index_;
        *index = Size();

        item->Hash = hash;
        item->Next = 0;

        Memory::Initialize(&(item->Key), Memory::Move(key));

        return item;
    }

    void remove(const Char_T *key, const SizeT length, const SizeT hash) const noexcept {
        if (IsNotEmpty()) {
            SizeT *index;
            HItem *item = find(index, key, length, hash);

            if (item != nullptr) {
                *index     = item->Next;
                item->Next = 0;
                item->Hash = 0;

                item->Clear();
            }
        }
    }

    void copyTable(const HashTable &src) {
        if (src.IsNotEmpty()) {
            const HItem *src_item = src.First();
            const HItem *src_end  = (src_item + src.Size());
            HItem       *storage  = allocate(src.Size());
            SizeT        index{0};

            do {
                if (src_item->Hash != 0) {
                    Memory::Initialize(storage, *src_item);
                    ++storage;
                    ++index;
                }

                ++src_item;
            } while (src_item < src_end);

            setSize(index);
            generateHash();
        }
    }

    void copyTableWithHash(const HashTable &src) {
        constexpr SizeT32 size = sizeof(SizeT);

        if (src.IsNotEmpty()) {
            setCapacity(src.Capacity());

            const HItem *src_item = src.First();
            const HItem *src_end  = (src_item + src.Size());
            const SizeT *src_ht   = src.getHashTable();
            SizeT       *ht       = allocateOnly(Capacity());
            HItem       *storage  = Memory::ChangePointer<HItem>(ht + Capacity());
            SizeT        index{0};

            Memory::Copy(ht, src_ht, (size * Capacity()));

            do {
                if (src_item->Hash != 0) {
                    Memory::Initialize(storage, *src_item);
                    ++storage;
                    ++index;
                }

                ++src_item;
            } while (src_item < src_end);

            setSize(index);
        }
    }

    void resize(const SizeT new_size) {
        SizeT       *ht      = getHashTable();
        HItem       *item    = Storage();
        const HItem *end     = (item + Size());
        HItem       *storage = allocate(new_size);

        setSize(0);

        while (item < end) {
            if (item->Hash != 0) {
                Memory::Initialize(storage, Memory::Move(*item));
                ++storage;
                ++index_;
            }

            ++item;
        }

        Memory::Deallocate(ht);
        generateHash();
    }

    HItem *find(SizeT *&index, const Char_T *key, const SizeT length, const SizeT hash) const noexcept {
        SizeT *ht = getHashTable();

        HItem *storage = Memory::ChangePointer<HItem>(ht + Capacity());
        HItem *item;
        index = (ht + (hash & getBase()));

        while (*index != 0) {
            item = (storage + *index);
            --item;

            if ((item->Hash == hash) && item->Key.IsEqual(key, length)) {
                return item;
            }

            index = &(item->Next);
        }

        return nullptr;
    }

    void generateHash() const noexcept {
        SizeT       *ht   = getHashTable();
        HItem       *src  = Storage();
        HItem       *item = src;
        const HItem *end  = (item + Size());
        SizeT       *index;
        SizeT        i    = SizeT{1};
        const SizeT  base = getBase();

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

    template <typename, typename>
    friend struct HArray;

    template <typename>
    friend struct HList;

    SizeT *hashTable_{nullptr};
    SizeT  index_{0};
    SizeT  capacity_{0};
};

} // namespace Qentem

#endif
