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
            allocate(Capacity());
        }
    }

    Array(Array &&arr) noexcept
        : storage_(arr.Storage()), index_(arr.Size()),
          capacity_(arr.Capacity()) {
        arr.clearStorage();
        arr.setSize(0);
        arr.setCapacity(0);
    }

    Array(const Array &arr) : capacity_(arr.Size()) {
        if (Capacity() != 0) {
            allocate(Capacity());

            do {
                Memory::Construct((Storage() + Size()), arr.Storage()[Size()]);
                ++index_;
            } while (Size() != Capacity());
        }
    }

    ~Array() {
        if (Storage() != nullptr) {
            Memory::Destruct(Storage(), (Storage() + Size()));
            Memory::Deallocate(Storage());
        }
    }

    Type_ &operator[](SizeT index) const {
        if (index < Size()) {
            return Storage()[index];
        }

        throw 1; // Index out of range
    }

    Array &operator=(Array &&arr) noexcept {
        if (this != &arr) {
            Memory::Destruct(Storage(), (Storage() + Size()));
            Memory::Deallocate(Storage());

            setStorage(arr.Storage());
            setSize(arr.Size());
            setCapacity(arr.Capacity());

            arr.clearStorage();
            arr.setSize(0);
            arr.setCapacity(0);
        }

        return *this;
    }

    Array &operator=(const Array &arr) {
        if (this != &arr) {
            Reserve(arr.Size());

            while (Size() != Capacity()) {
                Memory::Construct((Storage() + Size()), arr.Storage()[Size()]);
                ++index_;
            }
        }

        return *this;
    }

    void operator+=(Array &&arr) {
        if (Capacity() == 0) {
            // If the array hasn't allocated any memory, then there is no need
            // for rest.
            setStorage(arr.Storage());
            setSize(arr.Size());
            setCapacity(arr.Capacity());
        } else {
            const SizeT n_size = (Size() + arr.Size());

            if (n_size > Capacity()) {
                resize(n_size);
            }

            if (arr.IsNotEmpty()) {
                Memory::Copy((Storage() + Size()), arr.Storage(),
                             arr.Size() * sizeof(Type_));
            }

            setSize(n_size);
            Memory::Deallocate(arr.Storage());
        }

        arr.clearStorage();
        arr.setSize(0);
        arr.setCapacity(0);
    }

    void operator+=(const Array &arr) { copyArray(arr); }

    void operator+=(Type_ &&item) {
        if (Size() == Capacity()) {
            if (Capacity() == 0) {
                setCapacity(1);
            }

            resize(Capacity() << 1U);
        }

        Memory::Construct((Storage() + Size()), static_cast<Type_ &&>(item));
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
        Memory::Destruct(Storage(), (Storage() + Size()));
        Memory::Deallocate(Storage());

        clearStorage();
        setSize(0);
        setCapacity(0);
    }

    void Clear() noexcept {
        Memory::Destruct(Storage(), (Storage() + Size()));
        setSize(0);
    }

    Type_ *Eject() noexcept {
        Type_ *tmp = Storage();
        clearStorage();
        setSize(0);
        setCapacity(0);

        return tmp;
    }

    void Reserve(SizeT size) {
        if (Storage() != nullptr) {
            Reset();
        }

        if (size != 0) {
            setCapacity(size);
            allocate(Capacity());
        }
    }

    void Resize(SizeT new_size) {
        if (new_size == 0) {
            Reset();
            return;
        }

        if (Size() > new_size) {
            // Shrink
            Memory::Destruct((Storage() + new_size), (Storage() + Size()));
            setSize(new_size);
        }

        resize(new_size);
    }

    inline void Expect(SizeT size) {
        const SizeT n_size = (size + Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }
    }

    void Compress() {
        // Remove excess storage;
        if (Size() != Capacity()) {
            Resize(Size());
        }
    }

    void GoBackTo(SizeT index) noexcept {
        if (index < Size()) {
            Memory::Destruct((Storage() + index), (Storage() + Size()));
            setSize(index);
        }
    }

    void ResizeAndInitialize(SizeT size) {
        Resize(size);

        if (size > Size()) {
            Memory::Construct((Storage() + Size()), (Storage() + size),
                              Type_());
        }

        setSize(Capacity());
    }

    inline Type_ *      First() const noexcept { return Storage(); }
    inline Type_ *      Storage() const noexcept { return storage_; }
    inline SizeT        Size() const noexcept { return index_; }
    inline SizeT        Capacity() const noexcept { return capacity_; }
    inline const Type_ *End() const noexcept { return (Storage() + Size()); }
    inline bool         IsEmpty() const noexcept { return (Size() == 0); }
    inline bool         IsNotEmpty() const noexcept { return !(IsEmpty()); }

    inline const Type_ *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - 1));
        }

        return nullptr;
    }

    //////////// Private ////////////

  private:
    void setStorage(Type_ *new_storage) noexcept { storage_ = new_storage; }
    void setCapacity(SizeT new_capacity) noexcept { capacity_ = new_capacity; }
    void setSize(SizeT new_size) noexcept { index_ = new_size; }

    void clearStorage() noexcept { storage_ = nullptr; }
    void allocate(SizeT new_size) {
        setStorage(Memory::Allocate<Type_>(new_size));
    }

    void resize(SizeT new_size) {
        Type_ *tmp = Storage();
        setCapacity(new_size);
        allocate(new_size);

        if (IsNotEmpty()) {
            Memory::Copy(Storage(), tmp, (Size() * sizeof(Type_)));
        }

        Memory::Deallocate(tmp);
    }

    QENTEM_NOINLINE void copyArray(const Array &arr) {
        const SizeT n_size = (Size() + arr.Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }

        SizeT n = 0;

        while (n != arr.Size()) {
            Memory::Construct((Storage() + Size()), arr.Storage()[n]);
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
