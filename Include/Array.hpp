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
        : size_(arr.size_), capacity_(arr.capacity_), storage_(arr.storage_) {
        arr.size_     = 0;
        arr.capacity_ = 0;
        arr.storage_  = nullptr;
    }

    Array(const Array &arr) : capacity_(arr.size_) {
        if (capacity_ != 0) {
            storage_ = HAllocator::Allocate<Type_>(capacity_);

            do {
                HAllocator::Construct(
                    (storage_ + size_),
                    static_cast<Type_ &&>(Type_(arr.storage_[size_])));
                ++size_;
            } while (size_ < capacity_);
        }
    }

    explicit Array(ULong size) : capacity_(size) {
        if (size > 0) {
            storage_ = HAllocator::Allocate<Type_>(capacity_);
        }
    }

    ~Array() {
        HAllocator::Destruct(storage_, (storage_ + size_));
        HAllocator::Deallocate(storage_);
    }

    Type_ &operator[](ULong index) const {
        if (index < size_) {
            return storage_[index];
        }

        throw 1; // Index out of range
    }

    Array &operator=(Array &&arr) noexcept {
        if (this != &arr) {
            HAllocator::Destruct(storage_, (storage_ + size_));
            HAllocator::Deallocate(storage_);

            size_     = arr.size_;
            capacity_ = arr.capacity_;
            storage_  = arr.storage_;

            arr.size_     = 0;
            arr.capacity_ = 0;
            arr.storage_  = nullptr;
        }

        return (*this);
    }

    Array &operator=(const Array &arr) {
        if (this != &arr) {
            SetCapacity(arr.size_);

            while (size_ != capacity_) {
                HAllocator::Construct(
                    (storage_ + size_),
                    static_cast<Type_ &&>(Type_(arr.storage_[size_])));
                ++size_;
            }
        }

        return (*this);
    }

    void operator+=(Array &&arr) {
        if (capacity_ == 0) {
            // If the array hasn't allocated any memory, then there is no need
            // for the rest.
            size_     = arr.size_;
            capacity_ = arr.capacity_;
            storage_  = arr.storage_;
        } else {
            const ULong n_size = (size_ + arr.size_);

            if (n_size > capacity_) {
                resize_(n_size);
            }

            Memory::Copy((storage_ + size_), arr.storage_,
                         arr.size_ * sizeof(Type_));
            size_ = n_size;
            HAllocator::Deallocate(arr.storage_);
        }

        arr.size_     = 0;
        arr.capacity_ = 0;
        arr.storage_  = nullptr;
    }

    void operator+=(const Array &arr) {
        const ULong n_size = (size_ + arr.size_);

        if (n_size > capacity_) {
            resize_(n_size);
        }

        ULong n = 0;

        while (n != arr.size_) {
            HAllocator::Construct<Type_>(
                (storage_ + size_),
                static_cast<Type_ &&>(Type_(arr.storage_[n])));
            ++size_;
            ++n;
        }
    }

    void operator+=(Type_ &&item) {
        if (size_ == capacity_) {
            if (capacity_ == 0) {
                capacity_ = 1;
            }

            resize_(capacity_ << 1U);
        }

        HAllocator::Construct((storage_ + size_), static_cast<Type_ &&>(item));
        ++size_;
    }

    inline void operator+=(const Type_ &item) {
        (*this) += static_cast<Type_ &&>(Type_(item));
    }

    Array &Add(Array &&arr) {
        (*this) += static_cast<Array &&>(arr);
        return (*this);
    }

    Array &Add(const Array &arr) {
        (*this) += static_cast<Array &&>(Array(arr));
        return (*this);
    }

    Array &Add(Type_ &&item) {
        (*this) += static_cast<Type_ &&>(item);
        return (*this);
    }

    Array &Add(const Type_ &item) {
        (*this) += static_cast<Type_ &&>(Type_(item));
        return (*this);
    }

    void Clear() {
        HAllocator::Destruct(storage_, (storage_ + size_));
        HAllocator::Deallocate(storage_);

        size_     = 0;
        capacity_ = 0;
        storage_  = nullptr;
    }

    // Reset just the size
    void SoftClear() {
        HAllocator::Destruct(storage_, (storage_ + size_));
        size_ = 0;
    }

    Type_ *Eject() noexcept {
        size_      = 0;
        capacity_  = 0;
        Type_ *tmp = storage_;
        storage_   = nullptr;

        return tmp;
    }

    void SetCapacity(ULong size) {
        Clear();

        if (size > 0) {
            capacity_ = size;
            storage_  = HAllocator::Allocate<Type_>(capacity_);
        }
    }

    void Resize(ULong new_size) {
        if (new_size == 0) {
            Clear();
            return;
        }

        resize_(new_size);
    }

    void Compress() {
        // Remove excess storage;
        if (size_ != capacity_) {
            Resize(size_);
        }
    }

    void SoftResize(ULong new_size) {
        if (new_size <= capacity_) {
            if (new_size > size_) {
                HAllocator::Construct((storage_ + size_), (storage_ + new_size),
                                      Type_());
            } else if (new_size < size_) {
                HAllocator::Destruct((storage_ + new_size), (storage_ + size_));
            }

            size_ = new_size;
            return;
        }

        throw 1; // Size too large for soft resizing.
    }

    void ResizeAndInitialize(ULong size) {
        Resize(size);

        if (size > size_) {
            HAllocator::Construct((storage_ + size_), (storage_ + size),
                                  Type_());
        }

        size_ = capacity_;
    }

    inline ULong Size() const noexcept {
        return size_;
    }

    inline ULong Capacity() const noexcept {
        return capacity_;
    }

    inline const Type_ *Storage() const noexcept {
        return storage_;
    }

    inline Type_ *First() const noexcept {
        return storage_;
    }

    //////////// Private ////////////

  private:
    void resize_(ULong new_size) {
        Type_ *tmp = storage_;
        storage_   = HAllocator::Allocate<Type_>(new_size);

        if (size_ > new_size) {
            // Shrink
            HAllocator::Destruct((storage_ + new_size), (storage_ + size_));
            size_ = new_size;
        }

        Memory::Copy(storage_, tmp, (size_ * sizeof(Type_)));
        HAllocator::Deallocate(tmp);
        capacity_ = new_size;
    }

    ULong  size_{0};
    ULong  capacity_{0};
    Type_ *storage_{nullptr};
};

} // namespace Qentem

#endif
