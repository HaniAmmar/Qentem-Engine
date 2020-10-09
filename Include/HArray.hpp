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
    HAItem *        Anchor;
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
            capacity_ = algine_size(size);
            allocate();
        }
    }

    ~HArray() {
        if (storage_ != nullptr) {
            Memory::Destruct(storage_, (storage_ + index_));
            Memory::Deallocate(storage_);
        }
    }

    HArray(HArray &&h_arr) noexcept
        : storage_(h_arr.storage_), index_(h_arr.index_),
          capacity_(h_arr.capacity_) {
        h_arr.storage_  = nullptr;
        h_arr.index_    = 0;
        h_arr.capacity_ = 0;
    }

    HArray(const HArray &h_arr) {
        copyArray(h_arr);
    }

    HArray &operator=(HArray &&h_arr) noexcept {
        if (this != &h_arr) {
            if (storage_ != nullptr) {
                Memory::Destruct(storage_, (storage_ + index_));
                Memory::Deallocate(storage_);
            }

            storage_  = h_arr.storage_;
            index_    = h_arr.index_;
            capacity_ = h_arr.capacity_;

            h_arr.storage_  = nullptr;
            h_arr.index_    = 0;
            h_arr.capacity_ = 0;
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
        SizeT n_size = (index_ + h_arr.index_);

        if (n_size > capacity_) {
            n_size = algine_size(n_size);
            resize(n_size);
        }

        HAItem_T *      src_item = h_arr.storage_;
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

        Memory::Deallocate(h_arr.storage_);
        h_arr.storage_  = nullptr;
        h_arr.capacity_ = 0;
        h_arr.index_    = 0;
    }

    void operator+=(const HArray &h_arr) {
        SizeT n_size = (index_ + h_arr.index_);

        if (n_size > capacity_) {
            n_size = algine_size(n_size);
            resize(n_size);
        }

        HAItem_T *      src_item = h_arr.storage_;
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
        if (index_ == capacity_) {
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
        if (index_ == capacity_) {
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
        if (index_ == capacity_) {
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
        if (index_ == capacity_) {
            grow();
        }

        const ULong hash = Hash(key.First(), key.Length());
        HAItem_T ** item = find(key.First(), key.Length(), hash);

        if (*item == nullptr) {
            *item = (storage_ + index_);
            Memory::ConstructValues(*item, hash, nullptr,
                                    static_cast<String<Char_T_> &&>(key),
                                    static_cast<Value_ &&>(val));
            ++index_;
            return;
        }

        (*item)->Value = static_cast<Value_ &&>(val);
    }

    Value_ *GetValue(SizeT index) const noexcept {
        if (index < index_) {
            HAItem_T *item = (storage_ + index);

            if (item->Hash != 0) {
                return &(item->Value);
            }
        }

        return nullptr;
    }

    const String<Char_T_> *GetKey(SizeT index) const noexcept {
        if (index < index_) {
            const HAItem_T *item = (storage_ + index);

            if (item->Hash != 0) {
                return &(item->Key);
            }
        }

        return nullptr;
    }

    const HAItem_T *GetItem(SizeT index) const noexcept {
        if (index < index_) {
            const HAItem_T *item = (storage_ + index);

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
        if (capacity_ != 0) {
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
        if (index < index_) {
            const HAItem_T &item = storage_[index];

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
        if (capacity_ != 0) {
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
        if (storage_ != nullptr) {
            Reset();
        }

        if (size != 0) {
            capacity_ = algine_size(size);
            allocate();
        }
    }

    void Reset() noexcept {
        if (storage_ != nullptr) {
            Memory::Destruct(storage_, (storage_ + index_));
            Memory::Deallocate(storage_);
            storage_  = nullptr;
            capacity_ = 0;
            index_    = 0;
        }
    }

    void Resize(SizeT new_size) {
        if (new_size == 0) {
            Reset();
            return;
        }

        if (index_ > new_size) {
            // Shrink
            Memory::Destruct((storage_ + new_size), (storage_ + index_));
            index_ = new_size;
        }

        new_size = algine_size(new_size);
        resize(new_size);
    }

    // Remove excess storage.
    void Compress() {
        if (index_ != 0) {
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

            const SizeT n_cap = algine_size(n_size);

            if ((n_size < index_) || (n_cap < capacity_)) {
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

    inline SizeT Size() const noexcept {
        return index_;
    }

    inline SizeT Capacity() const noexcept {
        return capacity_;
    }

    inline const HAItem_T *First() const noexcept {
        return storage_;
    }

    inline const HAItem_T *Last() const noexcept {
        if (storage_ != nullptr) {
            return (storage_ + (index_ - 1));
        }

        return nullptr;
    }

    inline const HAItem_T *End() const noexcept {
        if (storage_ != nullptr) {
            return (storage_ + index_);
        }

        return nullptr;
    }

    //////////// Private ////////////

  private:
    void grow() {
        SizeT n_size = ((capacity_ != 0) ? (capacity_ << 1U) : 1);
        resize(n_size);
    }

    SizeT algine_size(SizeT n_size) noexcept {
        SizeT size = (SizeT{1} << Platform::CLZ(n_size));

        if (size < n_size) {
            size <<= 1U;
        }

        return size;
    }

    HAItem_T **getItem(ULong hash) const noexcept {
        return &((storage_ + (hash & (capacity_ - 1)))->Anchor);
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
        *item = (storage_ + index_);

        Memory::ConstructValues(*item, (*item)->Anchor, hash, nullptr,
                                static_cast<String<Char_T_> &&>(key), Value_());
        ++index_;
    }

    void remove(const Char_T_ *key, SizeT length, ULong hash) const noexcept {
        if (capacity_ != 0) {
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

    void allocate() {
        storage_ = Memory::Allocate<HAItem_T>(capacity_);

        for (size_t i = 0; i < capacity_; i++) {
            storage_[i].Anchor = nullptr;
        }
    }

    void resize(SizeT new_size) {
        capacity_     = new_size;
        HAItem_T *src = storage_;
        allocate();

        HAItem_T *      des_item = storage_;
        HAItem_T *      src_item = src;
        const HAItem_T *end      = (src + index_);

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

        index_ = static_cast<SizeT>(des_item - storage_);

        Memory::Deallocate(src);
        generateHash();
    }

    void generateHash() const noexcept {
        HAItem_T *      item = storage_;
        const HAItem_T *end  = End();
        const SizeT     base = (capacity_ - 1);

        while (item != end) {
            HAItem_T **position = &((storage_ + (item->Hash & base))->Anchor);

            while ((*position) != nullptr) {
                position = &((*position)->Next);
            }

            (*position) = item;
            ++item;
        }
    }

    QENTEM_NOINLINE void copyArray(const HArray &h_arr) {
        // The function Reset() should be called before this.
        if (h_arr.index_ != 0) {
            capacity_ = algine_size(h_arr.index_);
            allocate();

            HAItem_T *      des_item = storage_;
            const HAItem_T *src_item = h_arr.storage_;
            const HAItem_T *end      = (h_arr.storage_ + h_arr.index_);

            do {
                if (src_item->Hash != 0) {
                    Memory::ConstructValues(des_item, nullptr, src_item->Hash,
                                            nullptr, src_item->Key,
                                            src_item->Value);
                    ++des_item;
                }
            } while (++src_item != end);

            index_ = static_cast<SizeT>(des_item - storage_);
            generateHash();
        }
    }

    HAItem_T *storage_{nullptr};
    SizeT     index_{0};
    SizeT     capacity_{0};
};

} // namespace Qentem

#endif
