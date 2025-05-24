/*
 * Copyright (c) 2024 Hani Ammar
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

#ifndef QENTEM_ARRAY_H
#define QENTEM_ARRAY_H

#include "Memory.hpp"

namespace Qentem {
/*
 * Simple resizable array,
 */

template <typename Type_T>
struct Array {
    Array() noexcept = default;

    Array(Array &&src) noexcept : storage_{src.storage_}, index_{src.index_}, capacity_{src.capacity_} {
        src.clearStorage();
        src.setSize(0);
        src.setCapacity(0);
    }

    explicit Array(SizeT size, bool initialize = false) : capacity_{size} {
        if (size != 0) {
            Type_T *current = allocate();

            if (initialize) {
                Memory::Initialize(current, (current + size));
                setSize(size);
            }
        }
    }

    Array(const Array &src) : capacity_{src.Size()} {
        if (src.IsNotEmpty()) {
            setSize(src.Size());
            copyArray(src);
        }
    }

    ~Array() {
        Memory::Dispose(Storage(), End());
        Memory::Deallocate(Storage());
    }

    Array &operator=(Array &&src) noexcept {
        if (this != &src) {
            Type_T     *storage = Storage();
            const SizeT size    = Size();

            setStorage(src.Storage());
            setSize(src.Size());
            setCapacity(src.Capacity());

            src.clearStorage();
            src.setSize(0);
            src.setCapacity(0);

            // Just in case the copied array is not a child array, do this last.
            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(storage);
        }

        return *this;
    }

    Array &operator=(const Array &src) {
        if (this != &src) {
            Type_T     *storage = Storage();
            const SizeT size    = Size();

            clearStorage();
            setSize(src.Size());
            setCapacity(src.Size());

            if (IsNotEmpty()) {
                copyArray(src);
            }

            // Just in case the copied array is not a child array.
            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(storage);
        }

        return *this;
    }

    void operator+=(Array &&src) {
        if (Capacity() == 0) {
            setStorage(src.Storage());
            setSize(src.Size());
            setCapacity(src.Capacity());
        } else {
            constexpr SizeT32 type_size = sizeof(Type_T);
            const SizeT       n_size    = (Size() + src.Size());

            if (n_size > Capacity()) {
                resize(n_size);
            }

            Memory::Copy((Storage() + Size()), src.Storage(), (src.Size() * type_size));
            Memory::Deallocate(src.Storage());
            setSize(n_size);
        }

        src.clearStorage();
        src.setSize(0);
        src.setCapacity(0);
    }

    void operator+=(const Array &src) {
        const SizeT n_size = (Size() + src.Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }

        index_ += src.Size();

        Type_T       *storage  = Storage();
        const Type_T *src_item = src.First();
        const Type_T *src_end  = (src_item + src.Size());

        while (src_item < src_end) {
            Memory::Initialize(storage, *src_item);
            ++storage;
            ++src_item;
        }
    }

    void operator+=(Type_T &&item) {
        if (Size() == Capacity()) {
            resize((Capacity() | (Capacity() == 0)) * SizeT{2});
        }

        Memory::Initialize((Storage() + Size()), Memory::Move(item));
        ++index_;
    }

    inline void operator+=(const Type_T &item) {
        if (Size() == Capacity()) {
            resize((Capacity() | (Capacity() == 0)) * SizeT{2});
        }

        Memory::Initialize((Storage() + Size()), item);
        ++index_;
    }

    inline void Insert(Array &&src) {
        *this += Memory::Move(src);
    }

    inline void Insert(const Array &src) {
        *this += src;
    }

    inline Type_T &Insert(Type_T &&item) {
        const SizeT size = Size();

        *this += Memory::Move(item);

        return Storage()[size];
    }

    inline Type_T &Insert(const Type_T &item) {
        const SizeT size = Size();

        *this += item;

        return Storage()[size];
    }

    void Clear() noexcept {
        Memory::Dispose(Storage(), End());

        setSize(0);
    }

    void Reset() noexcept {
        Memory::Dispose(Storage(), End());
        Memory::Deallocate(Storage());

        clearStorage();
        setSize(0);
        setCapacity(0);
    }

    Type_T *Detach() noexcept {
        Type_T *tmp = Storage();

        clearStorage();
        setSize(0);
        setCapacity(0);

        return tmp;
    }

    void Reserve(const SizeT size, bool initialize = false) {
        Reset();

        if (size != 0) {
            setCapacity(size);
            Type_T *current = allocate();

            if (initialize) {
                Memory::Initialize(current, (current + size));
                setSize(size);
            }
        }
    }

    void Resize(SizeT new_size) {
        if (new_size != 0) {
            if (Size() > new_size) {
                // Shrink
                Memory::Dispose((Storage() + new_size), End());
                setSize(new_size);
            }

            resize(new_size);
            return;
        }

        Reset();
    }

    void ResizeWithDefaultInit(SizeT new_size) {
        Resize(new_size);

        if (new_size > Size()) {
            Type_T *current = Storage();
            Memory::Initialize((current + Size()), (current + new_size));
        }

        setSize(Capacity());
    }

    inline void Expect(SizeT size) {
        const SizeT n_size = (size + Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }
    }

    void Swap(Type_T &item1, Type_T &item2) noexcept {
        Memory::Swap(item1, item2);
    }

    // Set ascend to (false) for descend (ascend: 1,2,3; descend: 3,2,1 )
    void Sort(bool ascend = true) noexcept {
        if (ascend) {
            Memory::Sort<true>(Storage(), SizeT{0}, Size());
        } else {
            Memory::Sort<false>(Storage(), SizeT{0}, Size());
        }
    }

    /**
     * @brief Shrinks the internal buffer to match the current element count.
     *
     * If the allocated capacity exceeds the number of stored elements,
     * this method reallocates the buffer to the exact size, copies
     * all elements into the new storage and frees the excess memory.
     */
    void Compress() {
        if (Size() < Capacity()) {
            Resize(Size());
        }
    }

    inline void Drop(const SizeT size) noexcept {
        if (size <= Size()) {
            const SizeT new_size = (Size() - size);

            Memory::Dispose((Storage() + new_size), End());
            setSize(new_size);
        }
    }

    inline Type_T *Storage() const noexcept {
        return storage_;
    }

    inline SizeT Size() const noexcept {
        return index_;
    }

    inline SizeT Capacity() const noexcept {
        return capacity_;
    }

    inline const Type_T *First() const noexcept {
        return Storage();
    }

    inline Type_T *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - SizeT{1}));
        }

        return nullptr;
    }

    inline const Type_T *End() const noexcept {
        return (First() + Size());
    }

    inline bool IsEmpty() const noexcept {
        return (Size() == 0);
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // For STL
    inline const Type_T *begin() const noexcept {
        return First();
    }

    inline const Type_T *end() const noexcept {
        return End();
    }

    inline Type_T *begin() noexcept {
        return Storage();
    }

    inline Type_T *end() noexcept {
        return (Storage() + Size());
    }

    //////////// Private ////////////

  private:
    void setStorage(Type_T *ptr) noexcept {
        storage_ = ptr;
    }

    Type_T *allocate() {
        Type_T *new_storage = Memory::Allocate<Type_T>(Capacity());
        setStorage(new_storage);
        return new_storage;
    }

    void clearStorage() noexcept {
        storage_ = nullptr;
    }

    void setSize(SizeT new_size) noexcept {
        index_ = new_size;
    }

    void setCapacity(SizeT new_capacity) noexcept {
        capacity_ = new_capacity;
    }

    void resize(SizeT new_capacity) {
        constexpr SizeT32 type_size = sizeof(Type_T);
        Type_T           *src       = Storage();

        setCapacity(new_capacity);

        Type_T *des = allocate();
        Memory::Copy(des, src, (Size() * type_size));
        Memory::Deallocate(src);
    }

    void copyArray(const Array &src) {
        Type_T       *new_storage = allocate();
        const Type_T *src_item    = src.First();
        const Type_T *src_end     = (src_item + src.Size());

        while (src_item < src_end) {
            Memory::Initialize(new_storage, *src_item);
            ++new_storage;
            ++src_item;
        }
    }

    Type_T *storage_{nullptr};
    SizeT   index_{0};
    SizeT   capacity_{0};
};

} // namespace Qentem

#endif
