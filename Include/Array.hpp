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
template <typename Type_T_, bool>
struct ArrayData;

template <typename Type_T_>
struct ArrayData<Type_T_, false> {
    // Little-Endian
    ArrayData() noexcept = default;

    ArrayData(ArrayData &&src) noexcept : Index{src.Index}, Capacity{src.Capacity}, Storage{Memory::Move(src.Storage)} {
        src.Index    = SizeT{0};
        src.Capacity = SizeT{0};
    }

    ArrayData(const ArrayData &)            = delete;
    ArrayData &operator=(ArrayData &&)      = delete;
    ArrayData &operator=(const ArrayData &) = delete;

    ~ArrayData() {
        Type_T_ *storage = Storage.GetPointer();
        Memory::Dispose(storage, (storage + Index));
        Memory::Deallocate(storage);
    }

    explicit ArrayData(SizeT size) noexcept : Capacity{size} {
    }

    SizeT             Index{0};
    SizeT             Capacity{0};
    QPointer<Type_T_> Storage{};
};

template <typename Type_T_>
struct ArrayData<Type_T_, true> {
    // Big-Endian
    ArrayData() noexcept = default;

    ArrayData(ArrayData &&src) noexcept : Storage{Memory::Move(src.Storage)}, Index{src.Index}, Capacity{src.Capacity} {
        src.Index    = SizeT{0};
        src.Capacity = SizeT{0};
    }

    explicit ArrayData(SizeT size) noexcept : Capacity{size} {
    }

    ArrayData(const ArrayData &)            = delete;
    ArrayData &operator=(ArrayData &&)      = delete;
    ArrayData &operator=(const ArrayData &) = delete;

    ~ArrayData() {
        Type_T_ *storage = Storage.GetPointer();
        Memory::Dispose(storage, (storage + Index));
        Memory::Deallocate(storage);
    }

    QPointer<Type_T_> Storage{};
    SizeT             Index{0};
    SizeT             Capacity{0};
};

template <typename Type_T_>
class Array {
  public:
    Array() noexcept            = default;
    Array(Array &&src) noexcept = default;
    ~Array()                    = default;

    explicit Array(SizeT size, bool initialize = false) : data_{size} {
        if (size != SizeT{0}) {
            Type_T_ *current = allocate();

            if (initialize) {
                Memory::Initialize(current, (current + size));
                setSize(size);
            }
        }
    }

    Array(const Array &src) : data_{src.Size()} {
        copyArray(src);
    }

    Array &operator=(Array &&src) noexcept {
        if (this != &src) {
            Type_T_    *storage = Storage();
            const SizeT size    = Size();

            setSize(src.Size());
            setCapacity(src.Capacity());
            src.setSize(SizeT{0});
            src.setCapacity(SizeT{0});
            data_.Storage.MovePointerOnly(src.data_.Storage);

            // Just in case the copied array is not a child array.
            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(storage);
        }

        return *this;
    }

    Array &operator=(const Array &src) {
        if (this != &src) {
            Type_T_    *storage = Storage();
            const SizeT size    = Size();

            setStorage(nullptr);
            setSize(SizeT{0});
            setCapacity(src.Size());
            copyArray(src);

            // Just in case the copied array is not a child array.
            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(storage);
        }

        return *this;
    }

    void operator+=(Array &&src) {
        if (Capacity() == SizeT{0}) {
            setSize(src.Size());
            setCapacity(src.Capacity());
            setStorage(src.Storage());
        } else {
            const SizeT n_size = (Size() + src.Size());

            if (n_size > Capacity()) {
                resize(n_size);
            }

            if (src.IsNotEmpty()) {
                Memory::Copy<sizeof(Type_T_)>((Storage() + Size()), src.First(), src.Size() * sizeof(Type_T_));
            }

            setSize(n_size);
            Memory::Deallocate(src.Storage());
        }

        src.setSize(SizeT{0});
        src.setCapacity(SizeT{0});
        src.clearStorage();
    }

    void operator+=(const Array &src) {
        const SizeT n_size = (Size() + src.Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }

        data_.Index += src.Size();
        Type_T_       *storage  = Storage();
        const Type_T_ *src_item = src.First();
        const Type_T_ *src_end  = (src_item + src.Size());

        while (src_item < src_end) {
            Memory::Initialize(storage, *src_item);
            ++storage;
            ++src_item;
        }
    }

    void operator+=(Type_T_ &&item) {
        if (Size() == Capacity()) {
            if (Capacity() == SizeT{0}) {
                setCapacity(1U);
            }

            resize(Capacity() * SizeT{2});
        }

        Memory::Initialize((Storage() + Size()), Memory::Move(item));
        ++data_.Index;
    }

    inline void operator+=(const Type_T_ &item) {
        *this += Memory::Move(Type_T_{item});
    }

    inline void Insert(Array &&src) {
        *this += Memory::Move(src);
    }

    inline void Insert(const Array &src) {
        *this += src;
    }

    inline Type_T_ &Insert(Type_T_ &&item) {
        const SizeT size = Size();

        *this += Memory::Move(item);

        return Storage()[size];
    }

    inline Type_T_ &Insert(const Type_T_ &item) {
        const SizeT size = Size();

        *this += Memory::Move(Type_T_{item});

        return Storage()[size];
    }

    void Clear() noexcept {
        Type_T_ *storage = Storage();
        Memory::Dispose(storage, (storage + Size()));
        setSize(SizeT{0});
    }

    void Reset() noexcept {
        Type_T_ *storage = Storage();
        Memory::Dispose(storage, (storage + Size()));
        Memory::Deallocate(storage);
        clearStorage();
        setSize(SizeT{0});
        setCapacity(SizeT{0});
    }

    Type_T_ *Detach() noexcept {
        setSize(SizeT{0});
        setCapacity(SizeT{0});
        Type_T_ *tmp = Storage();
        clearStorage();

        return tmp;
    }

    void Reserve(SizeT size) {
        Reset();

        if (size != SizeT{0}) {
            setCapacity(size);
            allocate();
        }
    }

    void Resize(SizeT new_size) {
        if (new_size != SizeT{0}) {
            if (Size() > new_size) {
                // Shrink
                Type_T_ *storage = Storage();
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
            Type_T_ *current = Storage();
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

    void Swap(Type_T_ &item1, Type_T_ &item2) noexcept {
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

    inline Type_T_ *Storage() const noexcept {
        return data_.Storage.GetPointer();
    }

    inline SizeT Size() const noexcept {
        return data_.Index;
    }

    inline SizeT Capacity() const noexcept {
        return data_.Capacity;
    }

    inline const Type_T_ *First() const noexcept {
        return Storage();
    }

    inline Type_T_ *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - SizeT{1}));
        }

        return nullptr;
    }

    inline unsigned char GetHighByte() const noexcept {
        return data_.Storage.GetHighByte();
    }

    inline void SetHighByte(unsigned char byte) noexcept {
        data_.Storage.SetHighByte(byte);
    }

    inline unsigned char GetLowByte() const noexcept {
        return data_.Storage.GetLowByte();
    }

    inline void SetLowByte(unsigned char byte) noexcept {
        data_.Storage.SetLowByte(byte);
    }

    inline const Type_T_ *End() const noexcept {
        return (First() + Size());
    }

    inline bool IsEmpty() const noexcept {
        return (Size() == SizeT{0});
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // For STL
    inline const Type_T_ *begin() const noexcept {
        return First();
    }

    inline const Type_T_ *end() const noexcept {
        return End();
    }

    inline Type_T_ *begin() noexcept {
        return Storage();
    }

    inline Type_T_ *end() noexcept {
        return (Storage() + Size());
    }

    //////////// Private ////////////

  private:
    void setStorage(Type_T_ *ptr) noexcept {
        data_.Storage.SetPointer(ptr);
    }

    Type_T_ *allocate() {
        Type_T_ *new_storage = Memory::Allocate<Type_T_>(Capacity());
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
        Type_T_ *src = Storage();
        setCapacity(new_size);
        Type_T_ *des = allocate();

        if (IsNotEmpty()) {
            Memory::Copy<sizeof(Type_T_)>(des, src, (Size() * sizeof(Type_T_)));
        }

        Memory::Deallocate(src);
    }

    void copyArray(const Array &src) {
        if (Capacity() != SizeT{0}) {
            setSize(src.Size());
            Type_T_       *storage  = allocate();
            const Type_T_ *src_item = src.First();
            const Type_T_ *src_end  = (src_item + src.Size());

            while (src_item < src_end) {
                Memory::Initialize(storage, *src_item);
                ++storage;
                ++src_item;
            }
        }
    }

    ArrayData<Type_T_, Config::IsBigEndian> data_{};
};

} // namespace Qentem

#endif
