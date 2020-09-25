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

#ifndef QENTEM_ARRAY_H_
#define QENTEM_ARRAY_H_

namespace Qentem {

/*
 * Simple resizable array.
 */
template <typename Type_>
class Array {
  public:
    Array() = default;

    Array(Array &&arr) noexcept
        : index_(arr.index_), capacity_(arr.capacity_), storage_(arr.storage_) {
        arr.index_    = 0;
        arr.capacity_ = 0;
        arr.storage_  = nullptr;
    }

    Array(const Array &arr) : capacity_(arr.index_) {
        if (capacity_ != 0) {
            storage_ = HAllocator::Allocate<Type_>(capacity_);

            do {
                HAllocator::Construct(
                    (storage_ + index_),
                    static_cast<Type_ &&>(Type_(arr.storage_[index_])));
                ++index_;
            } while (index_ != capacity_);
        }
    }

    explicit Array(ULong size) : capacity_(size) {
        if (size != 0) {
            storage_ = HAllocator::Allocate<Type_>(capacity_);
        }
    }

    ~Array() {
        HAllocator::Destruct(storage_, (storage_ + index_));
        HAllocator::Deallocate(storage_);
    }

    Type_ &operator[](ULong index) const {
        if (index < index_) {
            return storage_[index];
        }

        throw 1; // Index out of range
    }

    Array &operator=(Array &&arr) noexcept {
        if (this != &arr) {
            HAllocator::Destruct(storage_, (storage_ + index_));
            HAllocator::Deallocate(storage_);

            index_    = arr.index_;
            capacity_ = arr.capacity_;
            storage_  = arr.storage_;

            arr.index_    = 0;
            arr.capacity_ = 0;
            arr.storage_  = nullptr;
        }

        return *this;
    }

    Array &operator=(const Array &arr) {
        if (this != &arr) {
            SetCapacity(arr.index_);

            while (index_ != capacity_) {
                HAllocator::Construct(
                    (storage_ + index_),
                    static_cast<Type_ &&>(Type_(arr.storage_[index_])));
                ++index_;
            }
        }

        return *this;
    }

    void operator+=(Array &&arr) {
        if (capacity_ == 0) {
            // If the array hasn't allocated any memory, then there is no need
            // for the rest.
            index_    = arr.index_;
            capacity_ = arr.capacity_;
            storage_  = arr.storage_;
        } else {
            const ULong n_size = (index_ + arr.index_);

            if (n_size > capacity_) {
                resize(n_size);
            }

            Memory::Copy<Type_>((storage_ + index_), arr.storage_,
                                arr.index_ * sizeof(Type_));
            index_ = n_size;
            HAllocator::Deallocate(arr.storage_);
        }

        arr.index_    = 0;
        arr.capacity_ = 0;
        arr.storage_  = nullptr;
    }

    void operator+=(const Array &arr) {
        const ULong n_size = (index_ + arr.index_);

        if (n_size > capacity_) {
            resize(n_size);
        }

        ULong n = 0;

        while (n != arr.index_) {
            HAllocator::Construct<Type_>(
                (storage_ + index_),
                static_cast<Type_ &&>(Type_(arr.storage_[n])));
            ++index_;
            ++n;
        }
    }

    void operator+=(Type_ &&item) {
        if (index_ == capacity_) {
            if (capacity_ == 0) {
                capacity_ = 1;
            }

            resize(capacity_ << 1U);
        }

        HAllocator::Construct((storage_ + index_), static_cast<Type_ &&>(item));
        ++index_;
    }

    inline void operator+=(const Type_ &item) {
        *this += static_cast<Type_ &&>(Type_(item));
    }

    inline Array &Insert(Array &&arr) {
        *this += static_cast<Array &&>(arr);
        return *this;
    }

    inline Array &Insert(const Array &arr) {
        *this += arr;
        return *this;
    }

    inline Array &Insert(Type_ &&item) {
        *this += static_cast<Type_ &&>(item);
        return *this;
    }

    inline Array &Insert(const Type_ &item) {
        *this += item;
        return *this;
    }

    void Reset() noexcept {
        HAllocator::Destruct(storage_, (storage_ + index_));
        HAllocator::Deallocate(storage_);

        index_    = 0;
        capacity_ = 0;
        storage_  = nullptr;
    }

    // Reset just the size
    void Clear() noexcept {
        HAllocator::Destruct(storage_, (storage_ + index_));
        index_ = 0;
    }

    Type_ *Eject() noexcept {
        index_     = 0;
        capacity_  = 0;
        Type_ *tmp = storage_;
        storage_   = nullptr;

        return tmp;
    }

    void SetCapacity(ULong size) {
        Reset();

        if (size != 0) {
            capacity_ = size;
            storage_  = HAllocator::Allocate<Type_>(capacity_);
        }
    }

    void Resize(ULong new_size) {
        if (new_size == 0) {
            Reset();
            return;
        }

        resize(new_size);
    }

    inline void Expect(ULong size) {
        const ULong n_size = (size + index_);

        if (n_size > capacity_) {
            resize(n_size);
        }
    }

    void Compress() {
        // Remove excess storage;
        if (index_ != capacity_) {
            Resize(index_);
        }
    }

    void GoBackTo(ULong index) noexcept {
        if (index < index_) {
            HAllocator::Destruct((storage_ + index), (storage_ + index_));
            index_ = index;
        }
    }

    void ResizeAndInitialize(ULong size) {
        Resize(size);

        if (size > index_) {
            HAllocator::Construct((storage_ + index_), (storage_ + size),
                                  Type_());
        }

        index_ = capacity_;
    }

    inline ULong Size() const noexcept {
        return index_;
    }

    inline ULong Capacity() const noexcept {
        return capacity_;
    }

    inline Type_ *First() const noexcept {
        return storage_;
    }

    inline const Type_ *Last() const noexcept {
        if (storage_ != nullptr) {
            return (storage_ + (index_ - 1));
        }

        return nullptr;
    }

    inline const Type_ *End() const noexcept {
        if (storage_ != nullptr) {
            return (storage_ + index_);
        }

        return nullptr;
    }

    //////////// Private ////////////

  private:
    void resize(ULong new_size) {
        Type_ *tmp = storage_;
        storage_   = HAllocator::Allocate<Type_>(new_size);

        if (index_ > new_size) {
            // Shrink
            HAllocator::Destruct((storage_ + new_size), (storage_ + index_));
            index_ = new_size;
        }

        Memory::Copy<Type_>(storage_, tmp, (index_ * sizeof(Type_)));
        HAllocator::Deallocate(tmp);
        capacity_ = new_size;
    }

    ULong  index_{0};
    ULong  capacity_{0};
    Type_ *storage_{nullptr};
};

} // namespace Qentem

#endif
