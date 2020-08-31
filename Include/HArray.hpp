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

#include "Memory.hpp"
#include "String.hpp"

#ifndef QENTEM_HARRAY_H_
#define QENTEM_HARRAY_H_

namespace Qentem {

template <typename Value_>
struct HAItem {
    HAItem *Next;
    String  Key;
    Value_  Value;
    ULong   Hash;
};

// A fork of https://github.com/HaniAmmar/HArray

/*
 * For less memory usage in exchange for 15-20% slower lookup, define
 * HARRAY_DOUBLE_SIZE_HASH_TABLE_ 0
 */
#ifndef HARRAY_DOUBLE_SIZE_HASH_TABLE_
#define HARRAY_DOUBLE_SIZE_HASH_TABLE_ 1
#endif

template <typename Value_>
class HArray {
    using HAItem_T = HAItem<Value_>;

  public:
    HArray() = default;

    explicit HArray(ULong size) : capacity_(size) {
        set_base_(size);
        hash_table_ = HAllocator::AllocateClear<HAItem_T *>(base_ + 1U);
        storage_    = HAllocator::Allocate<HAItem_T>(size);
    }

    ~HArray() {
        HAllocator::Deallocate(hash_table_);
        HAllocator::Destruct(storage_, (storage_ + size_));
        HAllocator::Deallocate(storage_);
    }

    HArray(HArray &&h_arr) noexcept
        : capacity_(h_arr.capacity_), base_(h_arr.base_), size_(h_arr.size_),
          hash_table_(h_arr.hash_table_), storage_(h_arr.storage_) {
        h_arr.capacity_   = 0;
        h_arr.base_       = 0;
        h_arr.size_       = 0;
        h_arr.hash_table_ = nullptr;
        h_arr.storage_    = nullptr;
    }

    HArray(const HArray &h_arr) {
        copy_array_(h_arr);
    }

    HArray &operator=(HArray &&h_arr) noexcept {
        if (this != &h_arr) {
            HAllocator::Deallocate(hash_table_);
            HAllocator::Destruct(storage_, (storage_ + size_));
            HAllocator::Deallocate(storage_);

            capacity_ = h_arr.capacity_;
            base_     = h_arr.base_;
            size_     = h_arr.size_;

            h_arr.capacity_ = 0;
            h_arr.base_     = 0;
            h_arr.size_     = 0;

            hash_table_       = h_arr.hash_table_;
            storage_          = h_arr.storage_;
            h_arr.hash_table_ = nullptr;
            h_arr.storage_    = nullptr;
        }

        return *this;
    }

    HArray &operator=(const HArray &h_arr) {
        if (this != &h_arr) {
            Clear();
            copy_array_(h_arr);
        }

        return *this;
    }

    void operator+=(HArray &&h_arr) {
        const ULong n_size = (size_ + h_arr.size_);

        if (n_size > capacity_) {
            HAllocator::Deallocate(hash_table_);
            set_base_(n_size);
            hash_table_ = HAllocator::AllocateClear<HAItem_T *>(base_ + 1U);
            resize_(n_size);
        }

        HAItem_T *      src_item = h_arr.storage_;
        const HAItem_T *end      = (h_arr.storage_ + h_arr.size_);

        while (src_item != end) {
            if (src_item->Hash != 0) {
                HAItem_T **item = find_(src_item->Key.Char(),
                                        src_item->Key.Length(), src_item->Hash);

                if ((*item) == nullptr) {
                    (*item) = (storage_ + size_);
                    insert_(static_cast<String &&>(src_item->Key),
                            src_item->Hash);
                } else {
                    src_item->Key.~String();
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
        h_arr.size_     = 0;
    }

    void operator+=(const HArray &h_arr_c) {
        const ULong n_size = (size_ + h_arr_c.size_);

        if (n_size > capacity_) {
            HAllocator::Deallocate(hash_table_);
            set_base_(n_size);
            hash_table_ = HAllocator::AllocateClear<HAItem_T *>(base_ + 1U);
            resize_(n_size);
        }

        HAItem_T *      c_item = h_arr_c.storage_;
        const HAItem_T *end    = (h_arr_c.storage_ + h_arr_c.size_);

        while (c_item != end) {
            if (c_item->Hash != 0) {
                HAItem_T **item = find_(c_item->Key.Char(),
                                        c_item->Key.Length(), c_item->Hash);

                if ((*item) == nullptr) {
                    (*item) = (storage_ + size_);
                    insert_(String(c_item->Key), c_item->Hash);
                }

                (*item)->Value = Value_(c_item->Value);
            }

            ++c_item;
        }
    }

    Value_ &operator[](String &&key) {
        if (key.Char() != nullptr) {
            const ULong hash = Hash(key.Char(), key.Length());

            if (capacity_ == 0) {
                init_storage_();
            }

            HAItem_T **item = find_(key.Char(), key.Length(), hash);

            if ((*item) == nullptr) {
                if (size_ == capacity_) {
                    grow_();
                    item = find_(key.Char(), key.Length(), hash);
                }

                (*item) = (storage_ + size_);
                insert_(static_cast<String &&>(key), hash);
            }

            return (*item)->Value;
        }

        throw 2; // Null key
    }

    Value_ &Add(const char *key, ULong length) {
        if (key != nullptr) {
            const ULong hash = Hash(key, length);

            if (capacity_ == 0) {
                init_storage_();
            }

            HAItem_T **item = find_(key, length, hash);

            if ((*item) == nullptr) {
                if (size_ == capacity_) {
                    grow_();
                    item = find_(key, length, hash);
                }

                (*item) = (storage_ + size_);
                insert_(String(key), hash);
            }

            return (*item)->Value;
        }

        throw 2; // Null key
    }

    inline Value_ &operator[](const String &key) {
        if (key.Char() != nullptr) {
            const ULong hash = Hash(key.Char(), key.Length());

            if (capacity_ == 0) {
                init_storage_();
            }

            HAItem_T **item = find_(key.Char(), key.Length(), hash);

            if ((*item) == nullptr) {
                if (size_ == capacity_) {
                    grow_();
                    item = find_(key.Char(), key.Length(), hash);
                }

                (*item) = (storage_ + size_);
                insert_(String(key), hash);
            }

            return (*item)->Value;
        }

        throw 2; // Null key
    }

    Value_ *GetValue(ULong index) {
        if (index < size_) {
            HAItem_T *item = (storage_ + index);

            if (item->Hash != 0) {
                return &(item->Value);
            }
        }

        return nullptr;
    }

    const String *GetKey(ULong index) const {
        if (index < size_) {
            const HAItem_T *item = (storage_ + index);

            if (item->Hash != 0) {
                return &(item->Key);
            }
        }

        return nullptr;
    }

    const HAItem_T *GetItem(ULong index) const {
        if (index < size_) {
            const HAItem_T *item = (storage_ + index);

            if (item->Hash != 0) {
                return item;
            }
        }

        return nullptr;
    }

    const HAItem_T *GetItem(const String &key) {
        // You can get the index of the item using (*GetItem() - *Storage())
        return (
            *(find_(key.Char(), key.Length(), Hash(key.Char(), key.Length()))));
    }

    Value_ *Find(const char *key, ULong length) {
        if (capacity_ != 0) {
            HAItem_T *item = *(find_(key, length, Hash(key, length)));

            if (item != nullptr) {
                return &(item->Value);
            }
        }

        return nullptr;
    }

    inline Value_ *Find(const String &key) {
        return Find(key.Char(), key.Length());
    }

    inline void Delete(const char *key, ULong lengthy) {
        delete_(key, lengthy, Hash(key, lengthy));
    }

    inline void Delete(const String &key) {
        Delete(key.Char(), key.Length());
    }

    void DeleteIndex(ULong index) {
        if (index < size_) {
            const HAItem_T &item = storage_[index];

            if (item.Hash != 0) {
                delete_(item.Key.Char(), item.Key.Length(), item.Hash);
            }
        }
    }

    /*
     * This function renames a key to a nonexisting one without changing the
     * order of the item, and returns true if successful.
     */
    bool Rename(const String &from, String &&to) {
        if (capacity_ != 0) {
            HAItem_T **left_item =
                (hash_table_ + (Hash(from.Char(), from.Length()) & base_));
            HAItem_T **before = left_item;

            while (((*left_item) != nullptr) &&
                   (((*left_item)->Key.Length() != from.Length()) ||
                    !(Memory::Compare((*left_item)->Key.Char(), from.Char(),
                                      from.Length())))) {
                before    = left_item; // Store the previous item
                left_item = &((*left_item)->Next);
            }

            if ((*left_item) != nullptr) {
                const ULong hash       = Hash(to.Char(), to.Length());
                HAItem_T ** right_item = find_(to.Char(), to.Length(), hash);

                if ((*right_item) == nullptr) {
                    (*right_item)       = (*left_item);
                    (*right_item)->Key  = static_cast<String &&>(to);
                    (*right_item)->Hash = hash;

                    // See delete_() for the next part
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

    bool Rename(const String &from, const String &to) {
        return Rename(from, String(to));
    }

    void SetCapacity(ULong size) {
        Clear();

        if (size != 0) {
            set_base_(size);
            hash_table_ = HAllocator::AllocateClear<HAItem_T *>(base_ + 1U);

            capacity_ = size;
            storage_  = HAllocator::Allocate<HAItem_T>(size);
        }
    }

    void Clear() {
        HAllocator::Deallocate(hash_table_);
        HAllocator::Destruct(storage_, (storage_ + size_));
        HAllocator::Deallocate(storage_);
        hash_table_ = nullptr;
        storage_    = nullptr;

        capacity_ = 0;
        base_     = 0;
        size_     = 0;
    }

    // Resize to more or to less, and not zero, use Clear() instead.
    void Resize(ULong new_size) {
        if (new_size == 0) {
            Clear();
            return;
        }

        if (size_ > new_size) {
            // Shrink
            HAllocator::Destruct((storage_ + new_size), (storage_ + size_));
            size_ = new_size;
        }

        HAllocator::Deallocate(hash_table_);
        set_base_(new_size);
        hash_table_ = HAllocator::AllocateClear<HAItem_T *>(base_ + 1U);
        resize_(new_size);
    }

    // Remove excess storage.
    void Compress() {
        if (size_ != 0) {
            ULong n_size = 0;

            const HAItem_T *item = storage_;
            const HAItem_T *end  = (storage_ + size_);

            while (item != end) {
                // Find the actual number of items.
                if (item->Hash != 0) {
                    ++n_size;
                }

                ++item;
            }

            if (n_size == 0) {
                Clear();
            } else if (n_size != capacity_) {
                const ULong old_base = base_;
                set_base_(n_size);

                if (old_base != base_) {
                    HAllocator::Deallocate(hash_table_);
                    hash_table_ =
                        HAllocator::AllocateClear<HAItem_T *>(base_ + 1U);
                } else {
                    /*
                     * Clear the hash table if there is no need to reallocate
                     * it.
                     */
                    Memory::SetToZero(hash_table_,
                                      ((base_ + 1U) * sizeof(void *)));
                }

                resize_(n_size);
            }
        }
    }

    static ULong Hash(const char *key, ULong length) noexcept {
        ULong hash = 0;

        if (key != nullptr) {
            constexpr ULong hash_start = 7;
            constexpr ULong base_start = 33;

            hash         = hash_start;
            ULong base   = base_start;
            ULong offset = 0;

            while (offset != length) {
                const UInt num = static_cast<unsigned char>(key[offset]);
                ++offset;
                base <<= 2U;
                base += offset;
                hash += (base * (offset << 1U) * num);

                if (offset != length) {
                    hash *= (length * offset);
                    base += offset;
                    --length;
                    hash += static_cast<unsigned char>(key[length]);
                }
            }
        }

        return hash;
    }

    inline ULong Size() const noexcept {
        return size_;
    }

    inline ULong Capacity() const noexcept {
        return capacity_;
    }

    inline const HAItem_T *Storage() const noexcept {
        return storage_;
    }

    //////////// Private ////////////

  private:
    void grow_() {
        ++base_;
        base_ <<= 1U;
        --base_;

        HAllocator::Deallocate(hash_table_);
        hash_table_ = HAllocator::AllocateClear<HAItem_T *>(base_ + 1U);
        resize_(capacity_ << 1U);
    }

    void init_storage_() {
#if HARRAY_DOUBLE_SIZE_HASH_TABLE_ == 1
        base_ = 1;
#else
        base_ = 0;
#endif
        capacity_   = 1;
        hash_table_ = HAllocator::AllocateClear<HAItem_T *>(base_ + 1U);
        storage_    = HAllocator::Allocate<HAItem_T>(capacity_);
    }

    void set_base_(ULong n_size) noexcept {
        base_ = (ULong(1) << Q_CLZL(n_size));

        if (base_ < n_size) {
            base_ <<= 1U;
        }

#if HARRAY_DOUBLE_SIZE_HASH_TABLE_ == 1
        base_ <<= 1U;
#endif
        --base_;
    }

    HAItem_T **find_(const char *key, ULong length, ULong hash) {
        HAItem_T **item = (hash_table_ + (hash & base_));

        while (((*item) != nullptr) &&
               (((*item)->Key.Length() != length) ||
                !(Memory::Compare((*item)->Key.Char(), key, length)))) {
            item = &((*item)->Next);
        }

        return item;
    }

    inline void insert_(String &&key, ULong hash) {
        HAllocator::Construct(
            (storage_ + size_),
            HAItem_T{nullptr, static_cast<String &&>(key), Value_(), hash});
        ++size_;
    }

    void delete_(const char *key, ULong length, ULong hash) {
        if (capacity_ != 0) {
            HAItem_T **item   = (hash_table_ + (hash & base_));
            HAItem_T **before = item;

            while (((*item) != nullptr) &&
                   (((*item)->Key.Length() != length) ||
                    !(Memory::Compare((*item)->Key.Char(), key, length)))) {
                before = item; // Store the previous item
                item   = &((*item)->Next);
            }

            if ((*item) != nullptr) {
                HAItem_T *c_item = (*item); // Current item
                c_item->Key      = String();
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

    void resize_(ULong new_size) {
        HAItem_T *src = storage_;
        storage_      = HAllocator::Allocate<HAItem_T>(new_size);

        HAItem_T *      des_item = storage_;
        HAItem_T *      src_item = src;
        const HAItem_T *end      = (src + size_);

        while (src_item != end) {
            if (src_item->Hash != 0) {
                HAllocator::Construct(
                    des_item,
                    HAItem_T{nullptr, static_cast<String &&>(src_item->Key),
                             static_cast<Value_ &&>(src_item->Value),
                             src_item->Hash});
                ++des_item;
            }

            ++src_item;
        }

        size_     = static_cast<ULong>(des_item - storage_);
        capacity_ = new_size;

        HAllocator::Deallocate(src);
        generate_hash_();
    }

    void generate_hash_() {
        // hash_table_ should be null before calling this function.
        const HAItem_T *end  = (storage_ + size_);
        HAItem_T *      item = storage_;

        while (item != end) {
            HAItem_T **position = (hash_table_ + (item->Hash & base_));

            while ((*position) != nullptr) {
                position = &((*position)->Next);
            }

            (*position) = item;
            ++item;
        }
    }

    void copy_array_(const HArray &h_arr) {
        // The function Clear() should be called before this.
        if (h_arr.size_ != 0) {
            capacity_ = h_arr.size_;
            storage_  = HAllocator::Allocate<HAItem_T>(capacity_);

            const HAItem_T *end      = h_arr.storage_ + capacity_;
            const HAItem_T *src_item = h_arr.storage_;
            HAItem_T *      des_item = (storage_ + size_);

            do {
                if (src_item->Hash != 0) {
                    HAllocator::Construct(
                        des_item, HAItem_T{nullptr, src_item->Key,
                                           src_item->Value, src_item->Hash});
                    ++des_item;
                }
            } while (++src_item != end);

            size_ = static_cast<ULong>(des_item - storage_);

            set_base_(capacity_);
            hash_table_ = HAllocator::AllocateClear<HAItem_T *>(base_ + 1U);
            generate_hash_();
        }
    }

    ULong      capacity_{0};
    ULong      base_{0};
    ULong      size_{0};
    HAItem_T **hash_table_{nullptr};
    HAItem_T * storage_{nullptr};
};

} // namespace Qentem

#endif
