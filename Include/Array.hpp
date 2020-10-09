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

    explicit Array(SizeT size) : capacity_(size) {
        if (size != 0) {
            storage_ = Memory::Allocate<Type_>(capacity_);
        }
    }

    Array(Array &&arr) noexcept
        : storage_(arr.storage_), index_(arr.index_), capacity_(arr.capacity_) {
        arr.storage_  = nullptr;
        arr.index_    = 0;
        arr.capacity_ = 0;
    }

    Array(const Array &arr) : capacity_(arr.index_) {
        if (capacity_ != 0) {
            storage_ = Memory::Allocate<Type_>(capacity_);

            do {
                Memory::Construct((storage_ + index_), arr.storage_[index_]);
                ++index_;
            } while (index_ != capacity_);
        }
    }

    ~Array() {
        if (storage_ != nullptr) {
            Memory::Destruct(storage_, (storage_ + index_));
            Memory::Deallocate(storage_);
        }
    }

    Type_ &operator[](SizeT index) const {
        if (index < index_) {
            return storage_[index];
        }

        throw 1; // Index out of range
    }

    Array &operator=(Array &&arr) noexcept {
        if (this != &arr) {
            Memory::Destruct(storage_, (storage_ + index_));
            Memory::Deallocate(storage_);

            storage_  = arr.storage_;
            index_    = arr.index_;
            capacity_ = arr.capacity_;

            arr.storage_  = nullptr;
            arr.index_    = 0;
            arr.capacity_ = 0;
        }

        return *this;
    }

    Array &operator=(const Array &arr) {
        if (this != &arr) {
            Reserve(arr.index_);

            while (index_ != capacity_) {
                Memory::Construct((storage_ + index_), arr.storage_[index_]);
                ++index_;
            }
        }

        return *this;
    }

    void operator+=(Array &&arr) {
        if (capacity_ == 0) {
            // If the array hasn't allocated any memory, then there is no need
            // for the rest.
            storage_  = arr.storage_;
            index_    = arr.index_;
            capacity_ = arr.capacity_;
        } else {
            const SizeT n_size = (index_ + arr.index_);

            if (n_size > capacity_) {
                resize(n_size);
            }

            if (arr.index_ != 0) {
                Memory::Copy((storage_ + index_), arr.storage_,
                             arr.index_ * sizeof(Type_));
            }

            index_ = n_size;
            Memory::Deallocate(arr.storage_);
        }

        arr.storage_  = nullptr;
        arr.index_    = 0;
        arr.capacity_ = 0;
    }

    void operator+=(const Array &arr) {
        copyArray(arr);
    }

    void operator+=(Type_ &&item) {
        if (index_ == capacity_) {
            if (capacity_ == 0) {
                capacity_ = 1;
            }

            resize(capacity_ << 1U);
        }

        Memory::Construct((storage_ + index_), static_cast<Type_ &&>(item));
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
        copyArray(arr);
        return *this;
    }

    inline Array &Insert(Type_ &&item) {
        *this += static_cast<Type_ &&>(item);
        return *this;
    }

    inline Array &Insert(const Type_ &item) {
        *this += static_cast<Type_ &&>(Type_(item));
        return *this;
    }

    void Reset() noexcept {
        Memory::Destruct(storage_, (storage_ + index_));
        Memory::Deallocate(storage_);

        storage_  = nullptr;
        index_    = 0;
        capacity_ = 0;
    }

    void Clear() noexcept {
        Memory::Destruct(storage_, (storage_ + index_));
        index_ = 0;
    }

    Type_ *Eject() noexcept {
        Type_ *tmp = storage_;
        storage_   = nullptr;
        index_     = 0;
        capacity_  = 0;

        return tmp;
    }

    void Reserve(SizeT size) {
        if (storage_ != nullptr) {
            Reset();
        }

        if (size != 0) {
            capacity_ = size;
            storage_  = Memory::Allocate<Type_>(capacity_);
        }
    }

    void Resize(SizeT new_size) {
        if (new_size == 0) {
            Reset();
            return;
        }

        resize(new_size);
    }

    inline void Expect(SizeT size) {
        const SizeT n_size = (size + index_);

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

    void GoBackTo(SizeT index) noexcept {
        if (index < index_) {
            Memory::Destruct((storage_ + index), (storage_ + index_));
            index_ = index;
        }
    }

    void ResizeAndInitialize(SizeT size) {
        Resize(size);

        if (size > index_) {
            Memory::Construct((storage_ + index_), (storage_ + size), Type_());
        }

        index_ = capacity_;
    }

    inline Type_ *First() const noexcept {
        return storage_;
    }

    inline const Type_ *Last() const noexcept {
        if (index_ != 0) {
            return (storage_ + (index_ - 1));
        }

        return nullptr;
    }

    inline const Type_ *End() const noexcept {
        return (storage_ + index_);
    }

    inline SizeT Size() const noexcept {
        return index_;
    }

    inline SizeT Capacity() const noexcept {
        return capacity_;
    }

    inline bool IsEmpty() const noexcept {
        return (index_ == 0);
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    //////////// Private ////////////

  private:
    void resize(SizeT new_size) {
        capacity_  = new_size;
        Type_ *tmp = storage_;
        storage_   = Memory::Allocate<Type_>(new_size);

        if (index_ > new_size) {
            // Shrink
            Memory::Destruct((storage_ + new_size), (storage_ + index_));
            index_ = new_size;
        }

        if (index_ != 0) {
            Memory::Copy(storage_, tmp, (index_ * sizeof(Type_)));
        }

        Memory::Deallocate(tmp);
    }

    QENTEM_NOINLINE void copyArray(const Array &arr) {
        const SizeT n_size = (index_ + arr.index_);

        if (n_size > capacity_) {
            resize(n_size);
        }

        SizeT n = 0;

        while (n != arr.index_) {
            Memory::Construct((storage_ + index_), arr.storage_[n]);
            ++index_;
            ++n;
        }
    }

    Type_ *storage_{nullptr};
    SizeT  index_{0};
    SizeT  capacity_{0};
};

} // namespace Qentem

#endif
