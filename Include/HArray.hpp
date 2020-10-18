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
    String<Char_T_> Key;
    Value_          Value;
};

template <typename Value_, typename Char_T_>
class HArray {
    using HAItem_T_ = HAItem<Value_, Char_T_>;

    struct HashTable_ {
        SizeT   Position; // To maintain the order of the items.
        SizeT   Next;
        ULSizeT Hash;
    };

  public:
    HArray() = default;

    explicit HArray(SizeT size) {
        if (size != 0) {
            setCapacity(algineSize(size));
            allocate();
        }
    }

    ~HArray() {
        HAItem_T_ *current = Storage();

        if (current != nullptr) {
            Memory::Destruct(current, End());
            deallocate(current);
        }
    }

    HArray(HArray &&h_arr) noexcept
        : storage_(h_arr.Storage()), index_(h_arr.Size()),
          capacity_(h_arr.Capacity()) {
        h_arr.clearStorage();
        h_arr.setSize(0);
        h_arr.setCapacity(0);
    }

    HArray(const HArray &h_arr) { copyArray(h_arr); }

    HArray &operator=(HArray &&h_arr) noexcept {
        if (this != &h_arr) {
            HAItem_T_ *current = Storage();

            if (current != nullptr) {
                Memory::Destruct(current, End());
                deallocate(current);
            }

            setStorage(h_arr.Storage());
            setSize(h_arr.Size());
            setCapacity(h_arr.Capacity());

            h_arr.clearStorage();
            h_arr.setSize(0);
            h_arr.setCapacity(0);
        }

        return *this;
    }

    HArray &operator=(const HArray &h_arr) {
        if (this != &h_arr) {
            Reset();
            copyArray(h_arr);
        }

        return *this;
    }

    void operator+=(HArray &&h_arr) {
        const SizeT n_size = (Size() + h_arr.Size());

        if (n_size > Capacity()) {
            resize(algineSize(n_size));
        }

        HAItem_T_ *       des      = Storage();
        HAItem_T_ *       src_item = h_arr.Storage();
        const HashTable_ *src_ht   = h_arr.getHashTable();
        const HAItem_T_ * end      = h_arr.End();

        while (src_item != end) {
            if (src_ht->Hash != 0) {
                SizeT *index;
                find(index, src_item->Key.First(), src_item->Key.Length(),
                     src_ht->Hash);

                if (*index == 0) {
                    insert(index,
                           static_cast<String<Char_T_> &&>(src_item->Key),
                           src_ht->Hash);
                } else {
                    src_item->Key.~String<Char_T_>();
                }

                des[(*index) - 1].Value =
                    static_cast<Value_ &&>(src_item->Value);
            }

            ++src_item;
            ++src_ht;
        }

        h_arr.deallocate(h_arr.Storage());
        h_arr.clearStorage();
        h_arr.setCapacity(0);
        h_arr.setSize(0);
    }

    void operator+=(const HArray &h_arr) {
        const SizeT n_size = (Size() + h_arr.Size());

        if (n_size > Capacity()) {
            resize(algineSize(n_size));
        }

        HAItem_T_ *       des      = Storage();
        const HAItem_T_ * src_item = h_arr.Storage();
        const HashTable_ *src_ht   = h_arr.getHashTable();
        const HAItem_T_ * end      = h_arr.End();

        while (src_item != end) {
            if (src_ht->Hash != 0) {
                SizeT *index;
                find(index, src_item->Key.First(), src_item->Key.Length(),
                     src_ht->Hash);

                if (*index == 0) {
                    insert(index, String<Char_T_>(src_item->Key), src_ht->Hash);
                }

                des[(*index) - 1].Value = Value_(src_item->Value);
            }

            ++src_item;
            ++src_ht;
        }
    }

    Value_ &operator[](const Char_T_ *key) {
        if (Size() == Capacity()) {
            grow();
        }

        SizeT         len  = StringUtils::Count(key);
        const ULSizeT hash = StringUtils::Hash(key, len);
        SizeT *       index;
        find(index, key, len, hash);

        if ((*index) == 0) {
            insert(index, String<Char_T_>(key, len), hash);
        }

        return Storage()[(*index) - 1].Value;
    }

    Value_ &operator[](String<Char_T_> &&key) {
        if (Size() == Capacity()) {
            grow();
        }

        const ULSizeT hash = StringUtils::Hash(key.First(), key.Length());
        SizeT *       index;
        find(index, key.First(), key.Length(), hash);

        if ((*index) == 0) {
            insert(index, static_cast<String<Char_T_> &&>(key), hash);
        }

        return Storage()[(*index) - 1].Value;
    }

    Value_ &operator[](const String<Char_T_> &key) {
        if (Size() == Capacity()) {
            grow();
        }

        const ULSizeT hash = StringUtils::Hash(key.First(), key.Length());
        SizeT *       index;
        find(index, key.First(), key.Length(), hash);

        if ((*index) == 0) {
            insert(index, String<Char_T_>(key), hash);
        }

        return Storage()[(*index) - 1].Value;
    }

    void Insert(String<Char_T_> &&key, Value_ &&val) {
        if (Size() == Capacity()) {
            grow();
        }

        const ULSizeT hash = StringUtils::Hash(key.First(), key.Length());
        SizeT *       index;
        find(index, key.First(), key.Length(), hash);

        if ((*index) == 0) {
            insert(index, static_cast<String<Char_T_> &&>(key), hash);
        }

        Storage()[(*index) - 1].Value = static_cast<Value_ &&>(val);
    }

    Value_ *GetValue(SizeT index) const noexcept {
        if ((index < Size()) && ((getHashTable() + index)->Hash != 0)) {
            return &((Storage() + index)->Value);
        }

        return nullptr;
    }

    const String<Char_T_> *GetKey(SizeT index) const noexcept {
        if ((index < Size()) && ((getHashTable() + index)->Hash != 0)) {
            return &((Storage() + index)->Key);
        }

        return nullptr;
    }

    const HAItem_T_ *GetItem(SizeT index) const noexcept {
        if ((index < Size()) && ((getHashTable() + index)->Hash != 0)) {
            return (Storage() + index);
        }

        return nullptr;
    }

    const HAItem_T_ *GetItem(const String<Char_T_> &key) const noexcept {
        SizeT *index;
        find(index, key.First(), key.Length(),
             StringUtils::Hash(key.First(), key.Length()));

        if ((*index) != 0) {
            return (Storage() + ((*index) - 1));
        }

        return nullptr;
    }

    Value_ *Find(const Char_T_ *key, SizeT length) const noexcept {
        if (Size() != 0) {
            SizeT *index;
            find(index, key, length, StringUtils::Hash(key, length));

            if ((*index) != 0) {
                return &((Storage()[(*index) - 1]).Value);
            }
        }

        return nullptr;
    }

    inline Value_ *Find(const String<Char_T_> &key) const noexcept {
        return Find(key.First(), key.Length());
    }

    inline void Remove(const Char_T_ *key, SizeT length) const noexcept {
        remove(key, length, StringUtils::Hash(key, length));
    }

    inline void Remove(const Char_T_ *key) const noexcept {
        Remove(key, StringUtils::Count(key));
    }

    inline void Remove(const String<Char_T_> &key) const noexcept {
        remove(key.First(), key.Length(),
               StringUtils::Hash(key.First(), key.Length()));
    }

    void RemoveIndex(SizeT index) const noexcept {
        if (index < Size()) {
            const HashTable_ *ht = (getHashTable() + index);

            if (ht->Hash != 0) {
                const HAItem_T_ &item = Storage()[index];
                remove(item.Key.First(), item.Key.Length(), ht->Hash);
            }
        }
    }

    /*
     * This function renames a key to a nonexisting one without changing the
     * order of the item, and returns true if successful.
     */
    bool Rename(const String<Char_T_> &from,
                String<Char_T_> &&     to) const noexcept {
        if (Size() != 0) {
            SizeT *left_index;
            find(left_index, from.First(), from.Length(),
                 StringUtils::Hash(from.First(), from.Length()));

            if (*left_index != 0) {
                const ULSizeT to_hash =
                    StringUtils::Hash(to.First(), to.Length());

                SizeT *right_index;
                find(right_index, to.First(), to.Length(), to_hash);

                if (*right_index == 0) {
                    const SizeT offset = ((*left_index) - 1);
                    HashTable_ *ht     = (getHashTable() + offset);
                    *right_index       = *left_index;
                    *left_index        = ht->Next;
                    ht->Hash           = to_hash;
                    ht->Next           = 0;

                    HAItem_T_ *item = (Storage() + offset);
                    item->Key       = static_cast<String<Char_T_> &&>(to);
                    return true;
                }
            }
        }

        return false;
    }

    bool Rename(const String<Char_T_> &from,
                const String<Char_T_> &to) const noexcept {
        return Rename(from, String<Char_T_>(to));
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
            deallocate(current);
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

    // Removes excess storage.
    void Compress() {
        if (Size() != 0) {
            const HashTable_ *ht  = getHashTable();
            const HashTable_ *end = (ht + Size());

            SizeT n_size = 0;

            while (ht != end) {
                // Find the actual number of items.
                if (ht->Hash != 0) {
                    ++n_size;
                }

                ++ht;
            }

            if (n_size == 0) {
                Reset();
                return;
            }

            const SizeT n_cap = algineSize(n_size);

            if ((n_size < Size()) || (n_cap < Capacity())) {
                resize(n_cap);
            }
        }
    }

#ifdef QENTEM_POINTER_TAGGING
    inline HAItem_T_ *Storage() const noexcept {
        return reinterpret_cast<HAItem_T_ *>(int_storage_ & 0xFFFFFFFFFFFFULL);
    }
#else
    inline HAItem_T_ *Storage() const noexcept { return storage_; }
#endif

    inline SizeT            Size() const noexcept { return index_; }
    inline SizeT            Capacity() const noexcept { return capacity_; }
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
    void setStorage(HAItem_T_ *new_storage) noexcept {
#ifdef QENTEM_POINTER_TAGGING
        int_storage_ &= 0xFFFF000000000000ULL; // Preserve the tag
        int_storage_ |= reinterpret_cast<unsigned long long>(
            new_storage); // Restore the tag
#else
        storage_ = new_storage;
#endif
    }

    void clearPosition() noexcept {
        HashTable_ *      item = getHashTable();
        const HashTable_ *end  = (item + Capacity());

        while (item != end) {
            item->Position = 0;
            ++item;
        }
    }

    void allocate() {
        /*--------------------------------*/
        /*| Hash Table      | Item       |*/
        /*| Pos, Next, Hash | Key, Value |*/
        /*--------------------------------*/

        const SizeT size = static_cast<SizeT>(
            (sizeof(HAItem_T_) + sizeof(HashTable_)) * Capacity());

        setStorage(reinterpret_cast<HAItem_T_ *>(Memory::Allocate<char>(size)));
        clearPosition();
    }

    void deallocate(HAItem_T_ *old_storage) { Memory::Deallocate(old_storage); }
    void clearStorage() noexcept { setStorage(nullptr); }
    void setSize(SizeT new_size) noexcept { index_ = new_size; }
    void setCapacity(SizeT new_capacity) noexcept { capacity_ = new_capacity; }
    SizeT       getBase() const noexcept { return (Capacity() - 1); }
    HashTable_ *getHashTable() const noexcept {
        return reinterpret_cast<HashTable_ *>(Storage() + Capacity());
    }

    void grow() { resize(((Capacity() != 0) ? (Capacity() << 1U) : 1)); }

    SizeT algineSize(SizeT n_size) noexcept {
        const SizeT size = (SizeT{1} << Platform::CLZ(n_size));

        if (size < n_size) {
            return (size << 1U);
        }

        return size;
    }

    SizeT *getPosition(ULSizeT hash) const noexcept {
        return &(getHashTable()[hash & getBase()].Position);
    }

    void find(SizeT *&index, const Char_T_ *key, SizeT length,
              ULSizeT hash) const noexcept {
        index                = getPosition(hash);
        const HAItem_T_ *src = Storage();
        HashTable_ *     ht  = getHashTable();

        while (*index != 0) {
            HashTable_ *item       = &(ht[(*index) - 1]);
            SizeT *     next_index = &(item->Next); // To avoid cache misses.

            if (((item->Hash == hash) &&
                 (src + ((*index) - 1))->Key.IsEqual(key, length))) {
                return;
            }

            index = next_index;
        }
    }

    inline void insert(SizeT *index, String<Char_T_> &&key,
                       ULSizeT hash) noexcept {
        HashTable_ *ht = (getHashTable() + Size());
        ht->Next       = 0;
        ht->Hash       = hash;

        HAItem_T_ *item = (Storage() + Size());
        ++index_;
        *index = Size();

        Memory::Construct(&(item->Key), static_cast<String<Char_T_> &&>(key));
        Memory::Construct(&(item->Value), Value_());
    }

    void remove(const Char_T_ *key, SizeT length, ULSizeT hash) const noexcept {
        if (Size() != 0) {
            SizeT *index;
            find(index, key, length, hash);

            if (*index != 0) {
                const SizeT offset = ((*index) - 1);
                HashTable_ *ht     = (getHashTable() + offset);
                *index             = ht->Next;
                ht->Hash           = 0;
                ht->Next           = 0;

                HAItem_T_ *item = (Storage() + offset);
                item->Key       = String<Char_T_>();
                item->Value     = Value_();
            }
        }
    }

    QENTEM_NOINLINE void copyArray(const HArray &h_arr) {
        // The function Reset() has to be called before this.
        if (h_arr.Size() != 0) {
            const HAItem_T_ * src_item = h_arr.First();
            const HAItem_T_ * end      = h_arr.End();
            const HashTable_ *src_ht   = h_arr.getHashTable();

            setCapacity(algineSize(h_arr.Size()));
            allocate();

            HAItem_T_ * des_item = Storage();
            HashTable_ *des_ht   = getHashTable();

            do {
                if (src_ht->Hash != 0) {
                    des_ht->Hash = src_ht->Hash;
                    Memory::Construct(&(des_item->Key), src_item->Key);
                    Memory::Construct(&(des_item->Value), src_item->Value);
                    ++des_item;
                    ++des_ht;
                }

                ++src_item;
                ++src_ht;
            } while (src_item != end);

            setSize(static_cast<SizeT>(des_item - Storage()));
            generateHash();
        }
    }

    void resize(SizeT new_size) {
        HAItem_T_ *       current  = Storage();
        HAItem_T_ *       src_item = current;
        const HashTable_ *src_ht   = getHashTable();
        const HAItem_T_ * end      = End();

        setCapacity(new_size);
        allocate();

        HAItem_T_ * des_item = Storage();
        HashTable_ *des_ht   = getHashTable();

        while (src_item != end) {
            if (src_ht->Hash != 0) {
                des_ht->Hash = src_ht->Hash;
                Memory::Construct(
                    &(des_item->Key),
                    static_cast<String<Char_T_> &&>(src_item->Key));
                Memory::Construct(&(des_item->Value),
                                  static_cast<Value_ &&>(src_item->Value));
                ++des_ht;
                ++des_item;
            }

            ++src_ht;
            ++src_item;
        }

        setSize(static_cast<SizeT>(des_item - Storage()));
        deallocate(current);
        generateHash();
    }

    void generateHash() const noexcept {
        SizeT             i    = 0;
        const SizeT       base = getBase();
        HashTable_ *      src  = getHashTable();
        HashTable_ *      item = src;
        const HashTable_ *end  = (getHashTable() + Size());

        while (item != end) {
            SizeT *index = &((src + (item->Hash & base))->Position);

            while (*index != 0) {
                index = &(src[(*index) - 1].Next);
            }

            ++i;
            *index     = i;
            item->Next = 0;
            ++item;
        }
    }

#ifdef QENTEM_POINTER_TAGGING
    union {
        unsigned long long int_storage_;
        HAItem_T_ *        storage_{nullptr};
    };
#else
    HAItem_T_ *storage_{nullptr};
#endif

    SizeT index_{0};
    SizeT capacity_{0};
};

} // namespace Qentem

#endif
