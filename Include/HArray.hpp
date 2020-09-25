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
    ULong           Hash;
    HAItem *        Next;
    String<Char_T_> Key;
    Value_          Value;
};

// A fork of https://github.com/HaniAmmar/HArray

/*
 * For less memory usage in exchange for 15-20% slower lookup, define
 * HARRAY_DOUBLE_SIZE_HASH_TABLE_ 0
 */
#ifndef HARRAY_DOUBLE_SIZE_HASH_TABLE_
#define HARRAY_DOUBLE_SIZE_HASH_TABLE_ 1
#endif

template <typename Value_, typename Char_T_>
class HArray {
    using HAItem_T = HAItem<Value_, Char_T_>;

  public:
    HArray() = default;

    explicit HArray(ULong size) : capacity_(size) {
        if (size != 0) {
            setBase(size);
            hash_table_ = HAllocator::AllocatePointers<HAItem_T *>(base_ + 1U);
            storage_    = HAllocator::Allocate<HAItem_T>(size);
        }
    }

    ~HArray() {
        HAllocator::Deallocate(hash_table_);
        HAllocator::Destruct(storage_, (storage_ + index_));
        HAllocator::Deallocate(storage_);
    }

    HArray(HArray &&h_arr) noexcept
        : capacity_(h_arr.capacity_), base_(h_arr.base_), index_(h_arr.index_),
          hash_table_(h_arr.hash_table_), storage_(h_arr.storage_) {
        h_arr.capacity_   = 0;
        h_arr.base_       = 0;
        h_arr.index_      = 0;
        h_arr.hash_table_ = nullptr;
        h_arr.storage_    = nullptr;
    }

    HArray(const HArray &h_arr) {
        copyArray(h_arr);
    }

    HArray &operator=(HArray &&h_arr) noexcept {
        if (this != &h_arr) {
            HAllocator::Deallocate(hash_table_);
            HAllocator::Destruct(storage_, (storage_ + index_));
            HAllocator::Deallocate(storage_);

            capacity_ = h_arr.capacity_;
            base_     = h_arr.base_;
            index_    = h_arr.index_;

            h_arr.capacity_ = 0;
            h_arr.base_     = 0;
            h_arr.index_    = 0;

            hash_table_       = h_arr.hash_table_;
            storage_          = h_arr.storage_;
            h_arr.hash_table_ = nullptr;
            h_arr.storage_    = nullptr;
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
        const ULong n_size = (index_ + h_arr.index_);

        if (n_size > capacity_) {
            HAllocator::Deallocate(hash_table_);
            setBase(n_size);
            hash_table_ = HAllocator::AllocatePointers<HAItem_T *>(base_ + 1U);
            resize(n_size);
        }

        HAItem_T *      src_item = h_arr.storage_;
        const HAItem_T *end      = h_arr.End();

        while (src_item != end) {
            if (src_item->Hash != 0) {
                HAItem_T **item = find(src_item->Key.First(),
                                       src_item->Key.Length(), src_item->Hash);

                if ((*item) == nullptr) {
                    (*item) = (storage_ + index_);
                    insert(static_cast<String<Char_T_> &&>(src_item->Key),
                           src_item->Hash);
                } else {
                    src_item->Key.~String<Char_T_>();
                }

                (*item)->Value = static_cast<Value_ &&>(src_item->Value);
            }

            ++src_item;
        }

        HAllocator::Deallocate(h_arr.hash_table_);
        h_arr.hash_table_ = nullptr;
        HAllocator::Deallocate(h_arr.storage_);
        h_arr.storage_ = nullptr;

        h_arr.capacity_ = 0;
        h_arr.base_     = 0;
        h_arr.index_    = 0;
    }

    void operator+=(const HArray &h_arr) {
        const ULong n_size = (index_ + h_arr.index_);

        if (n_size > capacity_) {
            HAllocator::Deallocate(hash_table_);
            setBase(n_size);
            hash_table_ = HAllocator::AllocatePointers<HAItem_T *>(base_ + 1U);
            resize(n_size);
        }

        HAItem_T *      src_item = h_arr.storage_;
        const HAItem_T *end      = h_arr.End();

        while (src_item != end) {
            if (src_item->Hash != 0) {
                HAItem_T **item = find(src_item->Key.First(),
                                       src_item->Key.Length(), src_item->Hash);

                if ((*item) == nullptr) {
                    (*item) = (storage_ + index_);
                    insert(String<Char_T_>(src_item->Key), src_item->Hash);
                }

                (*item)->Value = Value_(src_item->Value);
            }

            ++src_item;
        }
    }

    Value_ &operator[](String<Char_T_> &&key) {
        const ULong hash = Hash(key.First(), key.Length());

        if (capacity_ == 0) {
            initStorage();
        }

        HAItem_T **item = find(key.First(), key.Length(), hash);

        if ((*item) == nullptr) {
            if (index_ == capacity_) {
                grow();
                item = find(key.First(), key.Length(), hash);
            }

            (*item) = (storage_ + index_);
            insert(static_cast<String<Char_T_> &&>(key), hash);
        }

        return (*item)->Value;
    }

    inline Value_ &operator[](const String<Char_T_> &key) {
        const ULong hash = Hash(key.First(), key.Length());

        if (capacity_ == 0) {
            initStorage();
        }

        HAItem_T **item = find(key.First(), key.Length(), hash);

        if ((*item) == nullptr) {
            if (index_ == capacity_) {
                grow();
                item = find(key.First(), key.Length(), hash);
            }

            (*item) = (storage_ + index_);
            insert(String<Char_T_>(key), hash);
        }

        return (*item)->Value;
    }

    Value_ *GetValue(ULong index) const noexcept {
        if (index < index_) {
            HAItem_T *item = (storage_ + index);

            if (item->Hash != 0) {
                return &(item->Value);
            }
        }

        return nullptr;
    }

    const String<Char_T_> *GetKey(ULong index) const noexcept {
        if (index < index_) {
            const HAItem_T *item = (storage_ + index);

            if (item->Hash != 0) {
                return &(item->Key);
            }
        }

        return nullptr;
    }

    const HAItem_T *GetItem(ULong index) const noexcept {
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

    Value_ *Find(const Char_T_ *key, ULong length) const noexcept {
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

    inline void Remove(const String<Char_T_> &key) noexcept {
        remove(key, Hash(key.First(), key.Length()));
    }

    void RemoveIndex(ULong index) noexcept {
        if (index < index_) {
            const HAItem_T &item = storage_[index];

            if (item.Hash != 0) {
                remove(item.Key, item.Hash);
            }
        }
    }

    /*
     * This function renames a key to a nonexisting one without changing the
     * order of the item, and returns true if successful.
     */
    bool Rename(const String<Char_T_> &from, String<Char_T_> &&to) noexcept {
        if (capacity_ != 0) {
            const ULong hash_from = Hash(from.First(), from.Length());
            HAItem_T ** left_item = (hash_table_ + (hash_from & base_));
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
                const String<Char_T_> &to) noexcept {
        return Rename(from, String<Char_T_>(to));
    }

    void SetCapacity(ULong size) {
        Reset();

        if (size != 0) {
            setBase(size);
            hash_table_ = HAllocator::AllocatePointers<HAItem_T *>(base_ + 1U);

            capacity_ = size;
            storage_  = HAllocator::Allocate<HAItem_T>(size);
        }
    }

    void Reset() noexcept {
        HAllocator::Deallocate(hash_table_);
        HAllocator::Destruct(storage_, (storage_ + index_));
        HAllocator::Deallocate(storage_);
        hash_table_ = nullptr;
        storage_    = nullptr;

        capacity_ = 0;
        base_     = 0;
        index_    = 0;
    }

    // Resize to more or to less, and not zero, use Reset() instead.
    void Resize(ULong new_size) {
        if (new_size == 0) {
            Reset();
            return;
        }

        if (index_ > new_size) {
            // Shrink
            HAllocator::Destruct((storage_ + new_size), (storage_ + index_));
            index_ = new_size;
        }

        HAllocator::Deallocate(hash_table_);
        setBase(new_size);
        hash_table_ = HAllocator::AllocatePointers<HAItem_T *>(base_ + 1U);
        resize(new_size);
    }

    // Remove excess storage.
    void Compress() {
        if (index_ != 0) {
            ULong n_size = 0;

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
            } else if (n_size != capacity_) {
                const ULong old_base = base_;
                setBase(n_size);

                if (old_base != base_) {
                    HAllocator::Deallocate(hash_table_);
                    hash_table_ =
                        HAllocator::AllocatePointers<HAItem_T *>(base_ + 1U);
                } else {
                    /*
                     * Clear the hash table if there is no need to reallocate
                     * it.
                     */
                    Memory::SetToZero<void *>(hash_table_,
                                              ((base_ + 1U) * sizeof(void *)));
                }

                resize(n_size);
            }
        }
    }

    static ULong Hash(const Char_T_ *key, ULong length) noexcept {
        ULong hash = 0;

        if (key != nullptr) {
            constexpr ULong hash_start = 7;
            constexpr ULong base_start = 33;

            hash         = hash_start;
            ULong base   = base_start;
            ULong offset = 0;

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

    inline ULong Size() const noexcept {
        return index_;
    }

    inline ULong Capacity() const noexcept {
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
        ++base_;
        base_ <<= 1U;
        --base_;

        HAllocator::Deallocate(hash_table_);
        hash_table_ = HAllocator::AllocatePointers<HAItem_T *>(base_ + 1U);
        resize(capacity_ << 1U);
    }

    void initStorage() {
#if HARRAY_DOUBLE_SIZE_HASH_TABLE_ == 1
        base_ = 1;
#else
        base_ = 0;
#endif
        capacity_   = 1;
        hash_table_ = HAllocator::AllocatePointers<HAItem_T *>(base_ + 1U);
        storage_    = HAllocator::Allocate<HAItem_T>(capacity_);
    }

    void setBase(ULong n_size) noexcept {
        base_ = (ULong{1} << Platform::CLZL(n_size));

        if (base_ < n_size) {
            base_ <<= 1U;
        }

#if HARRAY_DOUBLE_SIZE_HASH_TABLE_ == 1
        base_ <<= 1U;
#endif
        --base_;
    }

    HAItem_T **find(const Char_T_ *key, ULong length,
                    ULong hash) const noexcept {
        HAItem_T **item = (hash_table_ + (hash & base_));

        while (((*item) != nullptr) &&
               (((*item)->Hash != hash) || ((*item)->Key.Length() != length) ||
                !(StringUtils::IsEqual((*item)->Key.First(), key, length)))) {
            item = &((*item)->Next);
        }

        return item;
    }

    inline void insert(String<Char_T_> &&key, ULong hash) noexcept {
        HAllocator::Construct((storage_ + index_),
                              HAItem_T{hash, nullptr,
                                       static_cast<String<Char_T_> &&>(key),
                                       Value_()});
        ++index_;
    }

    void remove(const String<Char_T_> &key, ULong hash) noexcept {
        if (capacity_ != 0) {
            HAItem_T **item   = (hash_table_ + (hash & base_));
            HAItem_T **before = item;

            while (((*item) != nullptr) &&
                   (((*item)->Hash != hash) || ((*item)->Key != key))) {
                before = item; // Store the previous item
                item   = &((*item)->Next);
            }

            if ((*item) != nullptr) {
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
                    (*item) = (*item)->Next;
                }

                c_item->Next = nullptr;
            }
        }
    }

    void resize(ULong new_size) {
        HAItem_T *src = storage_;
        storage_      = HAllocator::Allocate<HAItem_T>(new_size);

        HAItem_T *      des_item = storage_;
        HAItem_T *      src_item = src;
        const HAItem_T *end      = (src + index_);

        while (src_item != end) {
            if (src_item->Hash != 0) {
                HAllocator::Construct(
                    des_item,
                    HAItem_T{
                        src_item->Hash,
                        nullptr,
                        static_cast<String<Char_T_> &&>(src_item->Key),
                        static_cast<Value_ &&>(src_item->Value),
                    });
                ++des_item;
            }

            ++src_item;
        }

        index_    = static_cast<ULong>(des_item - storage_);
        capacity_ = new_size;

        HAllocator::Deallocate(src);
        generateHash();
    }

    void generateHash() const noexcept {
        HAItem_T *      item = storage_;
        const HAItem_T *end  = End();

        while (item != end) {
            HAItem_T **position = (hash_table_ + (item->Hash & base_));

            while ((*position) != nullptr) {
                position = &((*position)->Next);
            }

            (*position) = item;
            ++item;
        }
    }

    void copyArray(const HArray &h_arr) {
        // The function Reset() should be called before this.
        if (h_arr.index_ != 0) {
            capacity_ = h_arr.index_;
            storage_  = HAllocator::Allocate<HAItem_T>(capacity_);

            const HAItem_T *end      = h_arr.storage_ + capacity_;
            const HAItem_T *src_item = h_arr.storage_;
            HAItem_T *      des_item = (storage_ + index_);

            do {
                if (src_item->Hash != 0) {
                    HAllocator::Construct(
                        des_item, HAItem_T{src_item->Hash, nullptr,
                                           src_item->Key, src_item->Value});
                    ++des_item;
                }
            } while (++src_item != end);

            index_ = static_cast<ULong>(des_item - storage_);

            setBase(capacity_);
            hash_table_ = HAllocator::AllocatePointers<HAItem_T *>(base_ + 1U);
            generateHash();
        }
    }

    ULong      capacity_{0};
    ULong      base_{0};
    ULong      index_{0};
    HAItem_T **hash_table_{nullptr};
    HAItem_T * storage_{nullptr};
};

} // namespace Qentem

#endif
