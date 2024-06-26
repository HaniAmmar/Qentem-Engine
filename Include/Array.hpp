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
#include "QPointer.hpp"

namespace Qentem {
/*
 * Simple resizable array, with pointer tagging.
 */
template <typename, bool>
struct ArrayData;

template <typename Type_T>
struct ArrayData<Type_T, false> {
    // Little-Endian
    ArrayData() noexcept = default;

    ArrayData(ArrayData &&src) noexcept : Index{src.Index}, Capacity{src.Capacity}, Storage{Memory::Move(src.Storage)} {
        src.Capacity = SizeT{0};
        src.Index    = SizeT{0};
    }

    explicit ArrayData(SizeT size) noexcept : Capacity{size} {
    }

    ArrayData(const ArrayData &)            = delete;
    ArrayData &operator=(ArrayData &&)      = delete;
    ArrayData &operator=(const ArrayData &) = delete;

    ~ArrayData() {
        Type_T *storage = Storage.GetPointer();
        Memory::Dispose(storage, (storage + Index));
        Memory::Deallocate(storage);
    }

    SizeT            Index{0};
    SizeT            Capacity{0};
    QPointer<Type_T> Storage{};
};

template <typename Type_T>
struct ArrayData<Type_T, true> {
    // Big-Endian
    ArrayData() noexcept = default;

    ArrayData(ArrayData &&src) noexcept : Storage{Memory::Move(src.Storage)}, Capacity{src.Capacity}, Index{src.Index} {
        src.Index    = SizeT{0};
        src.Capacity = SizeT{0};
    }

    explicit ArrayData(SizeT size) noexcept : Capacity{size} {
    }

    ArrayData(const ArrayData &)            = delete;
    ArrayData &operator=(ArrayData &&)      = delete;
    ArrayData &operator=(const ArrayData &) = delete;

    ~ArrayData() {
        Type_T *storage = Storage.GetPointer();
        Memory::Dispose(storage, (storage + Index));
        Memory::Deallocate(storage);
    }

    QPointer<Type_T> Storage{};
    SizeT            Capacity{0};
    SizeT            Index{0};
};

template <typename Type_T>
struct Array {
    Array() noexcept            = default;
    Array(Array &&src) noexcept = default;
    ~Array()                    = default;

    explicit Array(SizeT size, bool initialize = false) : data_{size} {
        if (size != SizeT{0}) {
            Type_T *current = allocate();

            if (initialize) {
                Memory::Initialize(current, (current + size));
                setSize(size);
            }
        }
    }

    Array(const Array &src) : data_{src.Size()} {
        if (src.Size() != SizeT{0}) {
            setSize(src.Size());
            copyArray(src);
        }
    }

    Array &operator=(Array &&src) noexcept {
        if (this != &src) {
            Type_T     *storage = Storage();
            const SizeT size    = Size();

            data_.Storage.MovePointerOnly(src.data_.Storage);
            setCapacity(src.Capacity());
            setSize(src.Size());
            src.setSize(SizeT{0});
            src.setCapacity(SizeT{0});

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

            setStorage(nullptr);
            setCapacity(src.Size());
            setSize(src.Size());

            if (Size() != SizeT{0}) {
                copyArray(src);
            }

            // Just in case the copied array is not a child array.
            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(storage);
        }

        return *this;
    }

    void operator+=(Array &&src) {
        if (Capacity() == SizeT{0}) {
            setStorage(src.Storage());
            setCapacity(src.Capacity());
            setSize(src.Size());
        } else {
            constexpr SizeT32 type_size = sizeof(Type_T);
            const SizeT       n_size    = (Size() + src.Size());

            if (n_size > Capacity()) {
                resize(n_size);
            }

            Type_T *n_storage = src.Storage();
            Memory::Copy((Storage() + Size()), n_storage, (src.Size() * type_size));
            Memory::Deallocate(n_storage);
            setSize(n_size);
        }

        src.clearStorage();
        src.setCapacity(SizeT{0});
        src.setSize(SizeT{0});
    }

    void operator+=(const Array &src) {
        const SizeT n_size = (Size() + src.Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }

        data_.Index += src.Size();
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
            if (Capacity() == SizeT{0}) {
                setCapacity(1U);
            }

            resize(Capacity() * SizeT{2});
        }

        Memory::Initialize((Storage() + Size()), Memory::Move(item));
        ++(data_.Index);
    }

    inline void operator+=(const Type_T &item) {
        if (Size() == Capacity()) {
            if (Capacity() == SizeT{0}) {
                setCapacity(1U);
            }

            resize(Capacity() * SizeT{2});
        }

        Memory::Initialize((Storage() + Size()), item);
        ++(data_.Index);
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
        Type_T *storage = Storage();
        Memory::Dispose(storage, (storage + Size()));
        setSize(SizeT{0});
    }

    void Reset() noexcept {
        Type_T *storage = Storage();
        Memory::Dispose(storage, (storage + Size()));
        Memory::Deallocate(storage);
        clearStorage();
        setCapacity(SizeT{0});
        setSize(SizeT{0});
    }

    Type_T *Detach() noexcept {
        Type_T *tmp = Storage();
        setCapacity(SizeT{0});
        clearStorage();
        setSize(SizeT{0});

        return tmp;
    }

    void Reserve(const SizeT size, bool initialize = false) {
        Reset();

        if (size != SizeT{0}) {
            setCapacity(size);
            Type_T *current = allocate();

            if (initialize) {
                Memory::Initialize(current, (current + size));
                setSize(size);
            }
        }
    }

    void Resize(SizeT new_size) {
        if (new_size != SizeT{0}) {
            if (Size() > new_size) {
                // Shrink
                Type_T *storage = Storage();
                Memory::Dispose((storage + new_size), (storage + Size()));
                setSize(new_size);
            }

            resize(new_size);
            return;
        }

        Reset();
    }

    void ResizeAndInitialize(SizeT new_size) {
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
            resize(Memory::AlignSize(n_size));
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

    void Compress() {
        // Remove excess storage;
        Resize(Size());
    }

    inline Type_T *Storage() const noexcept {
        return data_.Storage.GetPointer();
    }

    inline SizeT Size() const noexcept {
        return data_.Index;
    }

    inline SizeT Capacity() const noexcept {
        return data_.Capacity;
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

    inline void Drop(const SizeT size) noexcept {
        if (size <= Size()) {
            const SizeT new_size = (Size() - size);

            Type_T *storage = Storage();
            Memory::Dispose((storage + new_size), (storage + Size()));
            setSize(new_size);
        }
    }

    inline SizeT8 GetHighByte() const noexcept {
        return data_.Storage.GetHighByte();
    }

    inline void SetHighByte(SizeT8 byte) noexcept {
        data_.Storage.SetHighByte(byte);
    }

    inline SizeT8 GetLowByte() const noexcept {
        return data_.Storage.GetLowByte();
    }

    inline void SetLowByte(SizeT8 byte) noexcept {
        data_.Storage.SetLowByte(byte);
    }

    inline const Type_T *End() const noexcept {
        return (First() + Size());
    }

    inline bool IsEmpty() const noexcept {
        return (Size() == SizeT{0});
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
        data_.Storage.SetPointer(ptr);
    }

    Type_T *allocate() {
        Type_T *new_storage = Memory::Allocate<Type_T>(Capacity());
        setStorage(new_storage);
        return new_storage;
    }

    void clearStorage() noexcept {
        data_.Storage.Reset();
    }

    void setSize(SizeT new_size) noexcept {
        data_.Index = new_size;
    }

    void setCapacity(SizeT new_capacity) noexcept {
        data_.Capacity = new_capacity;
    }

    void resize(SizeT new_size) {
        constexpr SizeT32 type_size = sizeof(Type_T);
        Type_T           *src       = Storage();
        setCapacity(new_size);
        Type_T *des = allocate();
        Memory::Copy(des, src, (Size() * type_size));
        Memory::Deallocate(src);
    }

    void copyArray(const Array &src) {
        Type_T       *storage  = allocate();
        const Type_T *src_item = src.First();
        const Type_T *src_end  = (src_item + src.Size());

        while (src_item < src_end) {
            Memory::Initialize(storage, *src_item);
            ++storage;
            ++src_item;
        }
    }

    ArrayData<Type_T, Config::IsBigEndian> data_{};
};

} // namespace Qentem

#endif
