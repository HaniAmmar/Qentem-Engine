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

#ifndef _QENTEM_ARRAY_H
#define _QENTEM_ARRAY_H

namespace Qentem {
/*
 * Simple resizable array, with pointer tagging.
 */
template <typename _Type_T, bool>
struct ArrayData;

template <typename _Type_T>
struct ArrayData<_Type_T, false> {
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
        _Type_T *storage = Storage.GetPointer();
        Memory::Dispose(storage, (storage + Index));
        Memory::Deallocate(storage);
    }

    explicit ArrayData(SizeT size) noexcept : Capacity{size} {
    }

    SizeT             Index{0};
    SizeT             Capacity{0};
    QPointer<_Type_T> Storage{};
};

template <typename _Type_T>
struct ArrayData<_Type_T, true> {
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
        _Type_T *storage = Storage.GetPointer();
        Memory::Dispose(storage, (storage + Index));
        Memory::Deallocate(storage);
    }

    QPointer<_Type_T> Storage{};
    SizeT             Index{0};
    SizeT             Capacity{0};
};

template <typename _Type_T>
class Array {
  public:
    Array() noexcept            = default;
    Array(Array &&src) noexcept = default;
    ~Array()                    = default;

    explicit Array(SizeT size, bool initialize = false) : _data{size} {
        if (size != SizeT{0}) {
            _Type_T *current = allocate();

            if (initialize) {
                Memory::Initialize(current, (current + size));
                setSize(size);
            }
        }
    }

    Array(const Array &src) : _data{src.Size()} {
        copyArray(src);
    }

    Array &operator=(Array &&src) noexcept {
        if (this != &src) {
            _Type_T    *storage = Storage();
            const SizeT size    = Size();

            setSize(src.Size());
            setCapacity(src.Capacity());
            src.setSize(SizeT{0});
            src.setCapacity(SizeT{0});
            _data.Storage.MovePointerOnly(src._data.Storage);

            // Just in case the copied array is not a child array.
            Memory::Dispose(storage, (storage + size));
            Memory::Deallocate(storage);
        }

        return *this;
    }

    Array &operator=(const Array &src) {
        if (this != &src) {
            _Type_T    *storage = Storage();
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
                Memory::Copy<sizeof(_Type_T)>((Storage() + Size()), src.First(), src.Size() * sizeof(_Type_T));
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

        _data.Index += src.Size();
        _Type_T       *storage  = Storage();
        const _Type_T *src_item = src.First();
        const _Type_T *src_end  = (src_item + src.Size());

        while (src_item < src_end) {
            Memory::Initialize(storage, *src_item);
            ++storage;
            ++src_item;
        }
    }

    void operator+=(_Type_T &&item) {
        if (Size() == Capacity()) {
            if (Capacity() == SizeT{0}) {
                setCapacity(1U);
            }

            resize(Capacity() * SizeT{2});
        }

        Memory::Initialize((Storage() + Size()), Memory::Move(item));
        ++_data.Index;
    }

    inline void operator+=(const _Type_T &item) {
        *this += Memory::Move(_Type_T{item});
    }

    inline void Insert(Array &&src) {
        *this += Memory::Move(src);
    }

    inline void Insert(const Array &src) {
        *this += src;
    }

    inline _Type_T &Insert(_Type_T &&item) {
        const SizeT size = Size();

        *this += Memory::Move(item);

        return Storage()[size];
    }

    inline _Type_T &Insert(const _Type_T &item) {
        const SizeT size = Size();

        *this += Memory::Move(_Type_T{item});

        return Storage()[size];
    }

    void Clear() noexcept {
        _Type_T *storage = Storage();
        Memory::Dispose(storage, (storage + Size()));
        setSize(SizeT{0});
    }

    void Reset() noexcept {
        _Type_T *storage = Storage();
        Memory::Dispose(storage, (storage + Size()));
        Memory::Deallocate(storage);
        clearStorage();
        setSize(SizeT{0});
        setCapacity(SizeT{0});
    }

    _Type_T *Detach() noexcept {
        setSize(SizeT{0});
        setCapacity(SizeT{0});
        _Type_T *tmp = Storage();
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
                _Type_T *storage = Storage();
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
            _Type_T *current = Storage();
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

    void Swap(_Type_T &item1, _Type_T &item2) noexcept {
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

    inline _Type_T *Storage() const noexcept {
        return _data.Storage.GetPointer();
    }

    inline SizeT Size() const noexcept {
        return _data.Index;
    }

    inline SizeT Capacity() const noexcept {
        return _data.Capacity;
    }

    inline const _Type_T *First() const noexcept {
        return Storage();
    }

    inline _Type_T *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - SizeT{1}));
        }

        return nullptr;
    }

    inline SizeT8 GetHighByte() const noexcept {
        return _data.Storage.GetHighByte();
    }

    inline void SetHighByte(SizeT8 byte) noexcept {
        _data.Storage.SetHighByte(byte);
    }

    inline SizeT8 GetLowByte() const noexcept {
        return _data.Storage.GetLowByte();
    }

    inline void SetLowByte(SizeT8 byte) noexcept {
        _data.Storage.SetLowByte(byte);
    }

    inline const _Type_T *End() const noexcept {
        return (First() + Size());
    }

    inline bool IsEmpty() const noexcept {
        return (Size() == SizeT{0});
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // For STL
    inline const _Type_T *begin() const noexcept {
        return First();
    }

    inline const _Type_T *end() const noexcept {
        return End();
    }

    inline _Type_T *begin() noexcept {
        return Storage();
    }

    inline _Type_T *end() noexcept {
        return (Storage() + Size());
    }

    //////////// Private ////////////

  private:
    void setStorage(_Type_T *ptr) noexcept {
        _data.Storage.SetPointer(ptr);
    }

    _Type_T *allocate() {
        _Type_T *new_storage = Memory::Allocate<_Type_T>(Capacity());
        setStorage(new_storage);
        return new_storage;
    }

    void clearStorage() noexcept {
        _data.Storage.Reset();
    }

    void setSize(SizeT new_size) noexcept {
        _data.Index = new_size;
    }

    void setCapacity(SizeT new_capacity) noexcept {
        _data.Capacity = new_capacity;
    }

    void resize(SizeT new_size) {
        _Type_T *src = Storage();
        setCapacity(new_size);
        _Type_T *des = allocate();

        if (IsNotEmpty()) {
            Memory::Copy<sizeof(_Type_T)>(des, src, (Size() * sizeof(_Type_T)));
        }

        Memory::Deallocate(src);
    }

    void copyArray(const Array &src) {
        if (Capacity() != SizeT{0}) {
            setSize(src.Size());
            _Type_T       *storage  = allocate();
            const _Type_T *src_item = src.First();
            const _Type_T *src_end  = (src_item + src.Size());

            while (src_item < src_end) {
                Memory::Initialize(storage, *src_item);
                ++storage;
                ++src_item;
            }
        }
    }

    ArrayData<_Type_T, Config::IsBigEndian> _data{};
};

} // namespace Qentem

#endif
