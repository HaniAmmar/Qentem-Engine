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

template <typename Value_, typename Char_T_>
struct HAItem {
    HAItem *        Position;
    ULong           Hash;
    HAItem *        Next;
    String<Char_T_> Key;
    Value_          Value;
};

template <typename Value_, typename Char_T_>
class HArray {
    using HAItem_T = HAItem<Value_, Char_T_>;

  public:
    HArray() = default;

    explicit HArray(SizeT size) {
        if (size != 0) {
            setCapacity(algineSize(size));
            allocate();
        }
    }

    ~HArray() {
        if (Storage() != nullptr) {
            Memory::Destruct(Storage(), End());
            deallocate(Storage());
        }
    }

    HArray(HArray &&h_arr) noexcept
        : storage_(h_arr.storage_), index_(h_arr.Size()),
          capacity_(h_arr.Capacity()) {
        h_arr.clearStorage();
        h_arr.setSize(0);
        h_arr.setCapacity(0);
    }

    HArray(const HArray &h_arr) { copyArray(h_arr); }

    HArray &operator=(HArray &&h_arr) noexcept {
        if (this != &h_arr) {
            if (Storage() != nullptr) {
                Memory::Destruct(Storage(), End());
                deallocate(Storage());
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
        SizeT n_size = (Size() + h_arr.Size());

        if (n_size > Capacity()) {
            n_size = algineSize(n_size);
            resize(n_size);
        }

        HAItem_T *      src_item = h_arr.Storage();
        const HAItem_T *end      = h_arr.End();

        while (src_item != end) {
            if (src_item->Hash != 0) {
                HAItem_T **item = find(src_item->Key.First(),
                                       src_item->Key.Length(), src_item->Hash);

                if (*item == nullptr) {
                    construct(item,
                              static_cast<String<Char_T_> &&>(src_item->Key),
                              src_item->Hash);
                } else {
                    src_item->Key.~String<Char_T_>();
                }

                (*item)->Value = static_cast<Value_ &&>(src_item->Value);
            }

            ++src_item;
        }

        h_arr.deallocate(h_arr.Storage());
        h_arr.clearStorage();
        h_arr.setCapacity(0);
        h_arr.setSize(0);
    }

    void operator+=(const HArray &h_arr) {
        SizeT n_size = (Size() + h_arr.Size());

        if (n_size > Capacity()) {
            n_size = algineSize(n_size);
            resize(n_size);
        }

        HAItem_T *      src_item = h_arr.Storage();
        const HAItem_T *end      = h_arr.End();

        while (src_item != end) {
            if (src_item->Hash != 0) {
                HAItem_T **item = find(src_item->Key.First(),
                                       src_item->Key.Length(), src_item->Hash);

                if (*item == nullptr) {
                    construct(item, String<Char_T_>(src_item->Key),
                              src_item->Hash);
                }

                (*item)->Value = Value_(src_item->Value);
            }

            ++src_item;
        }
    }

    Value_ &operator[](const Char_T_ *key) {
        if (Size() == Capacity()) {
            grow();
        }

        SizeT       len  = StringUtils::Count(key);
        const ULong hash = Hash(key, len);
        HAItem_T ** item = find(key, len, hash);

        if (*item == nullptr) {
            construct(item, String<Char_T_>(key, len), hash);
        }

        return (*item)->Value;
    }

    Value_ &operator[](String<Char_T_> &&key) {
        if (Size() == Capacity()) {
            grow();
        }

        const ULong hash = Hash(key.First(), key.Length());
        HAItem_T ** item = find(key.First(), key.Length(), hash);

        if (*item == nullptr) {
            construct(item, static_cast<String<Char_T_> &&>(key), hash);
        }

        return (*item)->Value;
    }

    Value_ &operator[](const String<Char_T_> &key) {
        if (Size() == Capacity()) {
            grow();
        }

        const ULong hash = Hash(key.First(), key.Length());
        HAItem_T ** item = find(key.First(), key.Length(), hash);

        if (*item == nullptr) {
            construct(item, String<Char_T_>(key), hash);
        }

        return (*item)->Value;
    }

    void Insert(String<Char_T_> &&key, Value_ &&val) {
        if (Size() == Capacity()) {
            grow();
        }

        const ULong hash = Hash(key.First(), key.Length());
        HAItem_T ** item = find(key.First(), key.Length(), hash);

        if (*item == nullptr) {
            *item = (Storage() + Size());
            Memory::ConstructValues(*item, hash, nullptr,
                                    static_cast<String<Char_T_> &&>(key),
                                    static_cast<Value_ &&>(val));
            ++index_;
            return;
        }

        (*item)->Value = static_cast<Value_ &&>(val);
    }

    Value_ *GetValue(SizeT index) const noexcept {
        if (index < Size()) {
            HAItem_T *item = (Storage() + index);

            if (item->Hash != 0) {
                return &(item->Value);
            }
        }

        return nullptr;
    }

    const String<Char_T_> *GetKey(SizeT index) const noexcept {
        if (index < Size()) {
            const HAItem_T *item = (Storage() + index);

            if (item->Hash != 0) {
                return &(item->Key);
            }
        }

        return nullptr;
    }

    const HAItem_T *GetItem(SizeT index) const noexcept {
        if (index < Size()) {
            const HAItem_T *item = (Storage() + index);

            if (item->Hash != 0) {
                return item;
            }
        }

        return nullptr;
    }

    const HAItem_T *GetItem(const String<Char_T_> &key) const noexcept {
        // You can get the index of the item using (*GetItem() - *First())
        return (*(
            find(key.First(), key.Length(), Hash(key.First(), key.Length()))));
    }

    Value_ *Find(const Char_T_ *key, SizeT length) const noexcept {
        if (Size() != 0) {
            HAItem_T *item = *(find(key, length, Hash(key, length)));

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
        remove(key, length, Hash(key, length));
    }

    inline void Remove(const Char_T_ *key) const noexcept {
        Remove(key, StringUtils::Count(key));
    }

    inline void Remove(const String<Char_T_> &key) const noexcept {
        remove(key.First(), key.Length(), Hash(key.First(), key.Length()));
    }

    void RemoveIndex(SizeT index) const noexcept {
        if (index < Size()) {
            const HAItem_T &item = Storage()[index];

            if (item.Hash != 0) {
                remove(item.Key.First(), item.Key.Length(), item.Hash);
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
            const ULong hash_from = Hash(from.First(), from.Length());
            HAItem_T ** left_item = getItem(hash_from);
            HAItem_T ** before    = left_item;

            while (((*left_item) != nullptr) &&
                   (((*left_item)->Hash != hash_from) ||
                    ((*left_item)->Key.Length() != from.Length()) ||
                    !(StringUtils::IsEqual((*left_item)->Key.First(),
                                           from.First(), from.Length())))) {
                before    = left_item; // Store the previous item
                left_item = &((*left_item)->Next);
            }

            if ((*left_item) != nullptr) {
                const ULong hash_to    = Hash(to.First(), to.Length());
                HAItem_T ** right_item = find(to.First(), to.Length(), hash_to);

                if ((*right_item) == nullptr) {
                    (*right_item)       = (*left_item);
                    (*right_item)->Key  = static_cast<String<Char_T_> &&>(to);
                    (*right_item)->Hash = hash_to;

                    // See remove() for the next part
                    if ((*before) >= (*right_item)) {
                        (*before) = (*right_item)->Next;
                    } else {
                        (*left_item) = (*right_item)->Next;
                    }

                    (*right_item)->Next = nullptr;

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
        if (Storage() != nullptr) {
            Memory::Destruct(Storage(), End());
            deallocate(Storage());
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

    // Remove excess storage.
    void Compress() {
        if (Size() != 0) {
            SizeT n_size = 0;

            const HAItem_T *item = First();
            const HAItem_T *end  = End();

            while (item != end) {
                // Find the actual number of items.
                if (item->Hash != 0) {
                    ++n_size;
                }

                ++item;
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

    static ULong Hash(const Char_T_ *key, SizeT length) noexcept {
        ULong hash = 0;

        if (key != nullptr) {
            constexpr ULong hash_start = 7;
            constexpr SizeT base_start = 33;

            hash         = hash_start;
            SizeT base   = base_start;
            SizeT offset = 0;

            while (offset != length) {
                const UInt num = static_cast<UInt>(key[offset]);
                ++offset;
                base <<= 2U;
                base += offset;
                hash += (base * (offset << 1U) * num);

                if (offset != length) {
                    hash *= (length * offset);
                    base += offset;
                    --length;
                    hash += static_cast<ULong>(key[length]);
                }
            }
        }

        return hash;
    }

    inline SizeT           Size() const noexcept { return index_; }
    inline SizeT           Capacity() const noexcept { return capacity_; }
    inline HAItem_T *      Storage() const noexcept { return storage_; }
    inline bool            IsEmpty() const noexcept { return (Size() == 0); }
    inline bool            IsNotEmpty() const noexcept { return !(IsEmpty()); }
    inline const HAItem_T *First() const noexcept { return Storage(); }
    inline const HAItem_T *End() const noexcept { return (First() + Size()); }
    inline const HAItem_T *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - 1));
        }

        return nullptr;
    }

    //////////// Private ////////////

  private:
    void setStorage(HAItem_T *new_storage) noexcept { storage_ = new_storage; }
    void allocate() {
        setStorage(Memory::Allocate<HAItem_T>(Capacity()));

        for (size_t i = 0; i < Capacity(); i++) {
            (Storage()[i]).Position = nullptr;
        }
    }
    void  deallocate(HAItem_T *old_storage) { Memory::Deallocate(old_storage); }
    void  clearStorage() noexcept { setStorage(nullptr); }
    void  setSize(SizeT new_size) noexcept { index_ = new_size; }
    void  setCapacity(SizeT new_capacity) noexcept { capacity_ = new_capacity; }
    SizeT getBase() const noexcept { return (Capacity() - 1); }

    void grow() {
        SizeT n_size = ((Capacity() != 0) ? (Capacity() << 1U) : 1);
        resize(n_size);
    }

    SizeT algineSize(SizeT n_size) noexcept {
        SizeT size = (SizeT{1} << Platform::CLZ(n_size));

        if (size < n_size) {
            size <<= 1U;
        }

        return size;
    }

    HAItem_T **getItem(ULong hash) const noexcept {
        return &(Storage()[hash & getBase()].Position);
    }

    HAItem_T **find(const Char_T_ *key, SizeT length,
                    ULong hash) const noexcept {
        HAItem_T **item = getItem(hash);

        while ((*item != nullptr) &&
               (((*item)->Hash != hash) || ((*item)->Key.Length() != length) ||
                !(StringUtils::IsEqual((*item)->Key.First(), key, length)))) {
            item = &((*item)->Next);
        }

        return item;
    }

    inline void construct(HAItem_T **item, String<Char_T_> &&key,
                          ULong hash) noexcept {
        *item = (Storage() + Size());
        Memory::ConstructValues(*item, (*item)->Position, hash, nullptr,
                                static_cast<String<Char_T_> &&>(key), Value_());
        ++index_;
    }

    void remove(const Char_T_ *key, SizeT length, ULong hash) const noexcept {
        if (Size() != 0) {
            HAItem_T **item   = getItem(hash);
            HAItem_T **before = item;

            while (
                (*item != nullptr) &&
                (((*item)->Hash != hash) || ((*item)->Key.Length() != length) ||
                 !(StringUtils::IsEqual((*item)->Key.First(), key, length)))) {
                before = item; // Store the previous item
                item   = &((*item)->Next);
            }

            if (*item != nullptr) {
                HAItem_T *c_item = (*item); // Current item
                c_item->Key      = String<Char_T_>();
                c_item->Value    = Value_();
                c_item->Hash     = 0;

                if ((*before) >= (*item)) {
                    /*
                     * If "before" inserted after "item"
                     * (e.g., deleting items from 0 to n).
                     */
                    (*before) = (*item)->Next;
                } else {
                    /*
                     * If "before" inserted before "item"
                     * (e.g., deleting items from n to 0).
                     */
                    *item = (*item)->Next;
                }

                c_item->Next = nullptr;
            }
        }
    }

    void resize(SizeT new_size) {
        setCapacity(new_size);
        HAItem_T *      src      = Storage();
        HAItem_T *      src_item = Storage();
        const HAItem_T *end      = End();

        allocate();

        HAItem_T *des_item = Storage();

        while (src_item != end) {
            if (src_item->Hash != 0) {
                Memory::ConstructValues(
                    des_item, nullptr, src_item->Hash, nullptr,
                    static_cast<String<Char_T_> &&>(src_item->Key),
                    static_cast<Value_ &&>(src_item->Value));
                ++des_item;
            }

            ++src_item;
        }

        setSize(static_cast<SizeT>(des_item - Storage()));
        deallocate(src);
        generateHash();
    }

    void generateHash() const noexcept {
        HAItem_T *      item = Storage();
        const HAItem_T *end  = End();
        const SizeT     base = getBase();

        while (item != end) {
            HAItem_T **position =
                &((Storage() + (item->Hash & base))->Position);

            while ((*position) != nullptr) {
                position = &((*position)->Next);
            }

            (*position) = item;
            ++item;
        }
    }

    QENTEM_NOINLINE void copyArray(const HArray &h_arr) {
        // The function Reset() has to be called before this.
        if (h_arr.Size() != 0) {
            setCapacity(algineSize(h_arr.Size()));
            allocate();

            HAItem_T *      des_item = Storage();
            const HAItem_T *src_item = h_arr.First();
            const HAItem_T *end      = h_arr.End();

            do {
                if (src_item->Hash != 0) {
                    Memory::ConstructValues(des_item, nullptr, src_item->Hash,
                                            nullptr, src_item->Key,
                                            src_item->Value);
                    ++des_item;
                }
            } while (++src_item != end);

            setSize(static_cast<SizeT>(des_item - Storage()));
            generateHash();
        }
    }

    HAItem_T *storage_{nullptr};
    SizeT     index_{0};
    SizeT     capacity_{0};
};

} // namespace Qentem

#endif
