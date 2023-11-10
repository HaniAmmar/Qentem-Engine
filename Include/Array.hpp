/*
 * Copyright (c) 2023 Hani Ammar
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
#include "QPointer.hpp"

#ifndef QENTEM_ARRAY_H_
#define QENTEM_ARRAY_H_

namespace Qentem {

/*
 * Simple resizable array, with pointer tagging.
 */
template <typename Type_>
class Array {
  public:
    Array() = default;

    explicit Array(SizeT size) : capacity_{size} {
        if (size != 0) {
            allocate();
        }
    }

    Array(Array &&src) noexcept
        : index_{src.Size()}, capacity_{src.Capacity()}, storage_{static_cast<QPointer<Type_> &&>(src.storage_)} {
        src.setSize(0);
        src.setCapacity(0);
    }

    Array(const Array &src) : capacity_{src.Size()} {
        copyArray(src);
    }

    ~Array() {
        Type_ *storage = Storage();
        Memory::Dispose(storage, (storage + Size()));
        Memory::Deallocate(storage);
    }

    Array &operator=(Array &&src) noexcept {
        if (this != &src) {
            Type_      *storage = Storage();
            const SizeT size    = Size();

            setSize(src.Size());
            setCapacity(src.Capacity());
            src.setSize(0);
            src.setCapacity(0);
            storage_.MovePointerOnly(src.storage_);

            // Just in case the copied array is not a child array.
            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(storage);
        }

        return *this;
    }

    Array &operator=(const Array &src) {
        if (this != &src) {
            Type_      *storage = Storage();
            const SizeT size    = Size();

            setStorage(nullptr);
            setSize(0);
            setCapacity(src.Size());
            copyArray(src);

            // Just in case the copied array is not a child array.
            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(storage);
        }

        return *this;
    }

    void operator+=(Array &&src) {
        if (Capacity() == 0) {
            setSize(src.Size());
            setCapacity(src.Capacity());
            setStorage(src.Storage());
        } else {
            const SizeT n_size = (Size() + src.Size());

            if (n_size > Capacity()) {
                resize(n_size);
            }

            if (src.IsNotEmpty()) {
                Memory::Copy((Storage() + Size()), src.First(), src.Size() * sizeof(Type_));
            }

            setSize(n_size);
            Memory::Deallocate(src.Storage());
        }

        src.setSize(0);
        src.setCapacity(0);
        src.clearStorage();
    }

    void operator+=(const Array &src) {
        const SizeT n_size = (Size() + src.Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }

        index_ += src.Size();
        Type_       *storage  = Storage();
        const Type_ *src_item = src.First();
        const Type_ *src_end  = (src_item + src.Size());

        while (src_item < src_end) {
            Memory::Initialize(storage, *src_item);
            ++storage;
            ++src_item;
        }
    }

    void operator+=(Type_ &&item) {
        if (Size() == Capacity()) {
            if (Capacity() == 0) {
                setCapacity(1U);
            }

            resize(Capacity() * SizeT{2});
        }

        Memory::Initialize((Storage() + Size()), static_cast<Type_ &&>(item));
        ++index_;
    }

    inline void operator+=(const Type_ &item) {
        *this += static_cast<Type_ &&>(Type_{item});
    }

    inline void Insert(Array &&src) {
        *this += static_cast<Array &&>(src);
    }

    inline void Insert(const Array &src) {
        *this += static_cast<const Array &>(src);
    }

    inline void Insert(Type_ &&item) {
        *this += static_cast<Type_ &&>(item);
    }

    inline void Insert(const Type_ &item) {
        *this += static_cast<Type_ &&>(Type_{item});
    }

    inline Type_ &InsertGet(Type_ &&item) {
        *this += static_cast<Type_ &&>(item);
        return *(Storage() + (Size() - 1));
    }

    void Clear() noexcept {
        Type_ *storage = Storage();
        Memory::Dispose(storage, (storage + Size()));
        setSize(0);
    }

    void Reset() noexcept {
        Type_ *storage = Storage();
        Memory::Dispose(storage, (storage + Size()));
        Memory::Deallocate(storage);
        clearStorage();
        setSize(0);
        setCapacity(0);
    }

    Type_ *Detach() noexcept {
        setSize(0);
        setCapacity(0);
        Type_ *tmp = Storage();
        clearStorage();

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
        if (new_size != 0) {
            if (Size() > new_size) {
                // Shrink
                Type_ *storage = Storage();
                Memory::Dispose((storage + new_size), (storage + Size()));
                setSize(new_size);
            }

            resize(new_size);
            return;
        }

        Reset();
    }

    inline void Expect(SizeT size) {
        const SizeT n_size = (size + Size());

        if (n_size > Capacity()) {
            resize(Memory::AlignSize(n_size));
        }
    }

    void Swap(Type_ &item1, Type_ &item2) noexcept {
        Memory::Swap(item1, item2);
    }

    // Set ascend to (false) for descend (ascend: 1,2,3; descend: 3,2,1 )
    void Sort(bool ascend = true) noexcept {
        Memory::QuickSort<Type_, SizeT>::Sort(Storage(), 0, Size(), ascend);
    }

    void Compress() {
        // Remove excess storage;
        Resize(Size());
    }

    void ResizeAndInitialize(SizeT new_size) {
        Resize(new_size);

        if (new_size > Size()) {
            Type_ *current = Storage();
            Memory::Initialize((current + Size()), (current + new_size));
        }

        setSize(Capacity());
    }

    inline Type_ *Storage() const noexcept {
        return storage_.GetPointer();
    }

    inline SizeT Size() const noexcept {
        return index_;
    }

    inline SizeT Capacity() const noexcept {
        return capacity_;
    }

    inline const Type_ *First() const noexcept {
        return Storage();
    }

    inline const Type_ *End() const noexcept {
        return (First() + Size());
    }

    inline bool IsEmpty() const noexcept {
        return (Size() == 0);
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // For STL
    inline const Type_ *begin() const noexcept {
        return First();
    }

    inline const Type_ *end() const noexcept {
        return End();
    }

    inline Type_ *begin() noexcept {
        return Storage();
    }

    inline Type_ *end() noexcept {
        return (Storage() + Size());
    }

    inline Type_ *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - 1));
        }

        return nullptr;
    }

    //////////// Private ////////////

  private:
    void setStorage(Type_ *ptr) noexcept {
        storage_.SetPointer(ptr);
    }

    Type_ *allocate() {
        Type_ *new_storage = Memory::Allocate<Type_>(Capacity());
        setStorage(new_storage);
        return new_storage;
    }

    void clearStorage() noexcept {
        storage_.Reset();
    }

    void setSize(SizeT new_size) noexcept {
        index_ = new_size;
    }

    void setCapacity(SizeT new_capacity) noexcept {
        capacity_ = new_capacity;
    }

    void resize(SizeT new_size) {
        Type_ *src = Storage();
        setCapacity(new_size);
        Type_ *des = allocate();

        if (IsNotEmpty()) {
            Memory::Copy(des, src, (Size() * sizeof(Type_)));
        }

        Memory::Deallocate(src);
    }

    void copyArray(const Array &src) {
        if (Capacity() != 0) {
            setSize(src.Size());
            Type_       *storage  = allocate();
            const Type_ *src_item = src.First();
            const Type_ *src_end  = (src_item + src.Size());

            while (src_item < src_end) {
                Memory::Initialize(storage, *src_item);
                ++storage;
                ++src_item;
            }
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
