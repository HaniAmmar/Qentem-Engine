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
    SizeT           Position; // To maintain the order of the items.
    SizeT           Next;
    SizeT           Hash;
    String<Char_T_> Key;
    Value_          Value;
};

template <typename Value_, typename Char_T_>
class HArray {
    using HAItem_T_ = HAItem<Value_, Char_T_>;

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

    HArray(HArray &&src) noexcept
        : index_(src.Size()), capacity_(src.Capacity()) {
        storage_.Set(static_cast<QPointer<HAItem_T_> &&>(src.storage_));
        src.setSize(0);
        src.setCapacity(0);
    }

    HArray(const HArray &src) { copyArray(src); }

    HArray &operator=(HArray &&src) noexcept {
        if (this != &src) {
            HAItem_T_ *current = Storage();

            if (current != nullptr) {
                Memory::Destruct(current, End());
                deallocate(current);
            }

            setSize(src.Size());
            setCapacity(src.Capacity());
            storage_ = static_cast<QPointer<HAItem_T_> &&>(src.storage_);

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
        const SizeT n_size = (Size() + src.Size());

        if (n_size > Capacity()) {
            resize(algineSize(n_size));
        }

        for (HAItem_T_ *item = src.Storage(), *end = (item + src.Size());
             item != end; item++) {
            if (item->Hash != 0) {
                SizeT *    index;
                HAItem_T_ *des_item = find(index, item->Key.First(),
                                           item->Key.Length(), item->Hash);

                if (des_item == nullptr) {
                    des_item = insert(
                        index, static_cast<String<Char_T_> &&>(item->Key),
                        item->Hash);
                } else {
                    item->Key.~String<Char_T_>();
                }

                des_item->Value = static_cast<Value_ &&>(item->Value);
            }
        }

        src.setCapacity(0);
        src.setSize(0);
        src.deallocate(src.Storage());
        src.storage_.Reset();
    }

    void operator+=(const HArray &src) {
        const SizeT n_size = (Size() + src.Size());

        if (n_size > Capacity()) {
            resize(algineSize(n_size));
        }

        for (const HAItem_T_ *item = src.First(), *end = item + src.Size();
             item != end; item++) {
            if (item->Hash != 0) {
                SizeT *    index;
                HAItem_T_ *des_item = find(index, item->Key.First(),
                                           item->Key.Length(), item->Hash);

                if (des_item == nullptr) {
                    des_item =
                        insert(index, String<Char_T_>(item->Key), item->Hash);
                }

                des_item->Value = Value_(item->Value);
            }
        }
    }

    Value_ &operator[](const Char_T_ *key) {
        if (Size() == Capacity()) {
            grow();
        }

        const SizeT len  = StringUtils::Count(key);
        const SizeT hash = StringUtils::Hash(key, len);
        SizeT *     index;
        HAItem_T_ * item = find(index, key, len, hash);

        if (item == nullptr) {
            item = insert(index, String<Char_T_>(key, len), hash);
        }

        return item->Value;
    }

    Value_ &operator[](String<Char_T_> &&key) {
        if (Size() == Capacity()) {
            grow();
        }

        const SizeT hash = StringUtils::Hash(key.First(), key.Length());
        SizeT *     index;
        HAItem_T_ * item = find(index, key.First(), key.Length(), hash);

        if (item == nullptr) {
            item = insert(index, static_cast<String<Char_T_> &&>(key), hash);
        }

        return item->Value;
    }

    Value_ &operator[](const String<Char_T_> &key) {
        if (Size() == Capacity()) {
            grow();
        }

        const SizeT hash = StringUtils::Hash(key.First(), key.Length());
        SizeT *     index;
        HAItem_T_ * item = find(index, key.First(), key.Length(), hash);

        if (item == nullptr) {
            item = insert(index, String<Char_T_>(key), hash);
        }

        return item->Value;
    }

    void Insert(String<Char_T_> &&key, Value_ &&val) {
        if (Size() == Capacity()) {
            grow();
        }

        const SizeT hash = StringUtils::Hash(key.First(), key.Length());
        SizeT *     index;
        HAItem_T_ * item = find(index, key.First(), key.Length(), hash);

        if (item == nullptr) {
            item = insert(index, static_cast<String<Char_T_> &&>(key), hash);
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

    const String<Char_T_> *GetKey(SizeT index) const noexcept {
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

    const HAItem_T_ *GetItem(const String<Char_T_> &key) const noexcept {
        SizeT *index;
        return find(index, key.First(), key.Length(),
                    StringUtils::Hash(key.First(), key.Length()));
    }

    Value_ *Find(const Char_T_ *key, SizeT length) const noexcept {
        if (Size() != 0) {
            SizeT *    index;
            HAItem_T_ *item =
                find(index, key, length, StringUtils::Hash(key, length));

            if (item != nullptr) {
                return &(item->Value);
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
    bool Rename(const String<Char_T_> &from,
                String<Char_T_> &&     to) const noexcept {
        if (Size() != 0) {
            SizeT *left_index;
            find(left_index, from.First(), from.Length(),
                 StringUtils::Hash(from.First(), from.Length()));

            if (*left_index != 0) {
                const SizeT to_hash =
                    StringUtils::Hash(to.First(), to.Length());

                SizeT *right_index;
                find(right_index, to.First(), to.Length(), to_hash);

                if (*right_index == 0) {
                    const SizeT offset = ((*left_index) - 1);
                    HAItem_T_ * item   = (Storage() + offset);
                    *right_index       = *left_index;
                    *left_index        = item->Next;
                    item->Hash         = to_hash;
                    item->Next         = 0;

                    item->Key = static_cast<String<Char_T_> &&>(to);
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
            SizeT n_size = 0;

            for (const HAItem_T_ *item = Storage(), *end = (item + Size());
                 item != end; item++) {
                // Find the actual number of items.
                n_size += (item->Hash > 0);
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

    inline HAItem_T_ *Storage() const noexcept { return storage_.GetPointer(); }
    inline SizeT      Size() const noexcept { return index_; }
    inline SizeT      Capacity() const noexcept { return capacity_; }
    inline bool       IsEmpty() const noexcept { return (Size() == 0); }
    inline bool       IsNotEmpty() const noexcept { return !(IsEmpty()); }
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
    void setStorage(HAItem_T_ *ptr) noexcept { storage_.SetPointer(ptr); }
    void clearPosition() noexcept {
        for (HAItem_T_ *item = Storage(), *end = (item + Capacity());
             item != end; item++) {
            item->Position = 0;
        }
    }

    HAItem_T_ *allocate() {
        HAItem_T_ *new_storage = Memory::Allocate<HAItem_T_>(Capacity());
        setStorage(new_storage);
        clearPosition();

        return new_storage;
    }

    void deallocate(HAItem_T_ *old_storage) { Memory::Deallocate(old_storage); }
    void clearStorage() noexcept { setStorage(nullptr); }
    void setSize(SizeT new_size) noexcept { index_ = new_size; }
    void setCapacity(SizeT new_capacity) noexcept { capacity_ = new_capacity; }
    SizeT getBase() const noexcept { return (Capacity() - 1); }
    void  grow() { resize(((Capacity() != 0) ? (Capacity() << 1U) : 1)); }

    SizeT algineSize(SizeT n_size) noexcept {
        const SizeT size = (SizeT{1} << Platform::CLZ(n_size));

        if (size < n_size) {
            return (size << 1U);
        }

        return size;
    }

    HAItem_T_ *find(SizeT *&index, const Char_T_ *key, SizeT length,
                    SizeT hash) const noexcept {
        HAItem_T_ *src = Storage();
        index          = &((src + (hash & getBase()))->Position);

        while (*index != 0) {
            HAItem_T_ *item = &(src[(*index) - 1]);

            if (((item->Hash == hash) && item->Key.IsEqual(key, length))) {
                return item;
            }

            index = &(item->Next);
        }

        return nullptr;
    }

    inline HAItem_T_ *insert(SizeT *index, String<Char_T_> &&key,
                             SizeT hash) noexcept {
        HAItem_T_ *item = (Storage() + Size());
        item->Next      = 0;
        item->Hash      = hash;

        ++index_;
        *index = Size();

        Memory::Construct(&(item->Key), static_cast<String<Char_T_> &&>(key));
        Memory::Construct(&(item->Value), Value_());

        return item;
    }

    void remove(const Char_T_ *key, SizeT length, SizeT hash) const noexcept {
        if (Size() != 0) {
            SizeT *    index;
            HAItem_T_ *item = find(index, key, length, hash);

            if (item != nullptr) {
                *index     = item->Next;
                item->Hash = 0;
                item->Next = 0;

                item->Key   = String<Char_T_>();
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

            HAItem_T_ *des_src  = allocate();
            HAItem_T_ *des_item = des_src;

            do {
                if (src_item->Hash != 0) {
                    des_item->Hash = src_item->Hash;
                    Memory::Construct(&(des_item->Key), src_item->Key);
                    Memory::Construct(&(des_item->Value), src_item->Value);
                    ++des_item;
                }

                ++src_item;
            } while (src_item != end);

            setSize(static_cast<SizeT>(des_item - des_src));
            generateHash();
        }
    }

    void resize(SizeT new_size) {
        HAItem_T_ *src = Storage();

        setCapacity(new_size);

        HAItem_T_ *des      = allocate();
        HAItem_T_ *des_item = des;

        for (HAItem_T_ *item = src, *end = (item + Size()); item != end;
             item++) {
            if (item->Hash != 0) {
                des_item->Hash = item->Hash;
                Memory::Construct(&(des_item->Key),
                                  static_cast<String<Char_T_> &&>(item->Key));
                Memory::Construct(&(des_item->Value),
                                  static_cast<Value_ &&>(item->Value));
                ++des_item;
            }
        }

        setSize(static_cast<SizeT>(des_item - des));
        deallocate(src);
        generateHash();
    }

    void generateHash() const noexcept {
        SizeT       i    = 1;
        const SizeT base = getBase();
        HAItem_T_ * src  = Storage();

        for (HAItem_T_ *item = src, *end = (item + Size()); item != end;
             item++, i++) {
            SizeT *index = &((src + (item->Hash & base))->Position);

            while (*index != 0) {
                index = &(src[(*index) - 1].Next);
            }

            *index     = i;
            item->Next = 0;
        }
    }

#ifndef QENTEM_BIG_ENDIAN
    SizeT               index_{0};
    SizeT               capacity_{0};
    QPointer<HAItem_T_> storage_{};
#else
    QPointer<HAItem_T_> storage_{};
    SizeT               index_{0};
    SizeT               capacity_{0};
#endif
};

} // namespace Qentem

#endif
