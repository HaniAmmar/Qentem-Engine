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
#include "Platform.hpp"
#include "QPointer.hpp"

#ifndef QENTEM_ARRAY_H_
#define QENTEM_ARRAY_H_

namespace Qentem {

/*
 * Simple resizable array, with pointer taging.
 */
template <typename Type_>
class Array {
  public:
    Array() = default;

    explicit Array(SizeT size) : capacity_(size) {
        if (size != 0) {
            allocate();
        }
    }

    Array(Array &&src) noexcept
        : index_(src.Size()), capacity_(src.Capacity()) {
        storage_.Set(static_cast<QPointer<Type_> &&>(src.storage_));
        src.setSize(0);
        src.setCapacity(0);
    }

    Array(const Array &src) : capacity_(src.Size()) {
        if (Capacity() != 0) {
            setSize(src.Size());
            Type_ *des = allocate();

            for (const Type_ *item = src.First(), *end = (item + src.Size());
                 item != end; item++, des++) {
                Memory::Construct(des, *item);
            }
        }
    }

    ~Array() {
        Type_ *current = Storage();

        if (current != nullptr) {
            Memory::Destruct(current, End());
            deallocate(current);
        }
    }

    Array &operator=(Array &&src) noexcept {
        if (this != &src) {
            Type_ *current = Storage();

            if (current != nullptr) {
                Memory::Destruct(current, End());
                deallocate(current);
            }

            setSize(src.Size());
            setCapacity(src.Capacity());
            storage_ = static_cast<QPointer<Type_> &&>(src.storage_);

            src.setSize(0);
            src.setCapacity(0);
        }

        return *this;
    }

    Array &operator=(const Array &src) {
        if (this != &src) {
            Reserve(src.Size());
            setSize(src.Size());
            Type_ *des = Storage();

            for (const Type_ *item = src.First(), *end = (item + src.Size());
                 item != end; item++, des++) {
                Memory::Construct(des, *item);
            }
        }

        return *this;
    }

    Type_ &operator[](SizeT index) const {
        if (index < Size()) {
            return Storage()[index];
        }

        throw 1; // Index out of range
    }

    void operator+=(Array &&src) {
        if (Capacity() == 0) {
            // If the array hasn't allocated any memory, then there is no need
            // for rest.
            setSize(src.Size());
            setCapacity(src.Capacity());
            setStorage(src.Storage());
        } else {
            const SizeT n_size = (Size() + src.Size());

            if (n_size > Capacity()) {
                resize(n_size);
            }

            if (src.IsNotEmpty()) {
                Memory::Copy((Storage() + Size()), src.First(),
                             src.Size() * sizeof(Type_));
            }

            setSize(n_size);
            src.deallocate(src.Storage());
        }

        src.setSize(0);
        src.setCapacity(0);
        src.storage_.Reset();
    }

    void operator+=(const Array &src) { copyArray(src); }

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

    inline Array &Insert(Array &&src) {
        *this += static_cast<Array &&>(src);
        return *this;
    }

    inline Array &Insert(const Array &src) {
        copyArray(src);
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
        Type_ *current = Storage();

        if (current != nullptr) {
            Memory::Destruct(current, End());
            deallocate(current);
            clearStorage();

            setSize(0);
            setCapacity(0);
        }
    }

    void Clear() noexcept {
        Memory::Destruct(Storage(), End());
        setSize(0);
    }

    Type_ *Eject() noexcept {
        setSize(0);
        setCapacity(0);
        Type_ *tmp = Storage();
        storage_.Reset();

        return tmp;
    }

    void Reserve(SizeT size) {
        Reset();

        if (size != 0) {
            setCapacity(size);
            allocate();
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

        resize(new_size);
    }

    inline void Expect(SizeT size) {
        const SizeT n_size = (size + Size());

        if (n_size > Capacity()) {
            resize((SizeT{2} << Platform::CLZ(n_size)));
        }
    }

    void Swap(Type_ &item1, Type_ &item2) noexcept {
        Type_ item = static_cast<Type_ &&>(item1);
        item1      = static_cast<Type_ &&>(item2);
        item2      = static_cast<Type_ &&>(item);
    }

    void Compress() {
        // Remove excess storage;
        if (Size() != Capacity()) {
            Resize(Size());
        }
    }

    void GoBackTo(SizeT index) noexcept {
        if (index < Size()) {
            Memory::Destruct((Storage() + index), End());
            setSize(index);
        }
    }

    void ResizeAndInitialize(SizeT new_size) {
        Resize(new_size);

        if (new_size > Size()) {
            Type_ *current = Storage();
            Memory::Construct((current + Size()), (current + new_size),
                              Type_());
        }

        setSize(Capacity());
    }

    inline Type_ *Storage() const noexcept { return storage_.GetPointer(); }
    inline SizeT  Size() const noexcept { return index_; }
    inline SizeT  Capacity() const noexcept { return capacity_; }
    inline const Type_ *First() const noexcept { return Storage(); }
    inline const Type_ *End() const noexcept { return (First() + Size()); }
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
    void   setStorage(Type_ *ptr) noexcept { storage_.SetPointer(ptr); }
    Type_ *allocate() {
        Type_ *new_storage = Memory::Allocate<Type_>(Capacity());
        setStorage(new_storage);
        return new_storage;
    }
    void deallocate(Type_ *old_storage) { Memory::Deallocate(old_storage); }
    void clearStorage() noexcept { setStorage(nullptr); }
    void setSize(SizeT new_size) noexcept { index_ = new_size; }
    void setCapacity(SizeT new_capacity) noexcept { capacity_ = new_capacity; }

    void resize(SizeT new_size) {
        Type_ *src = Storage();
        setCapacity(new_size);
        Type_ *des = allocate();

        if (IsNotEmpty()) {
            Memory::Copy(des, src, (Size() * sizeof(Type_)));
        }

        deallocate(src);
    }

    QENTEM_NOINLINE void copyArray(const Array &src) {
        const SizeT n_size = (Size() + src.Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }

        index_ += src.Size();
        Type_ *des = Storage();

        for (const Type_ *item = src.First(), *end = (item + src.Size());
             item != end; item++, des++) {
            Memory::Construct(des, *item);
        }
    }

#ifndef QENTEM_BIG_ENDIAN
    SizeT           index_{0};
    SizeT           capacity_{0};
    QPointer<Type_> storage_{};
#else
    QPointer<Type_> storage_{};
    SizeT           index_{0};
    SizeT           capacity_{0};
#endif
};

} // namespace Qentem

#endif
