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

#include "Common.hpp"

#ifndef QENTEM_Q_POINTER_H
#define QENTEM_Q_POINTER_H

namespace Qentem {

template <typename Type_T>
union PointerNumber {
    inline explicit PointerNumber(Type_T *pointer) noexcept : Pointer{pointer} {
    }

    inline explicit PointerNumber(SystemIntType number) noexcept : Number{number} {
    }

    Type_T       *Pointer;
    SystemIntType Number{0};
};

template <typename Type_T, bool Is64Bit, bool>
struct QPointerData;

template <typename Type_T>
struct QPointerData<Type_T, true, true> {
    // With Tag
    QPointerData() noexcept  = default;
    ~QPointerData() noexcept = default;

    QPointerData(const QPointerData &)            = delete;
    QPointerData &operator=(QPointerData &&)      = delete;
    QPointerData &operator=(const QPointerData &) = delete;

    inline QPointerData(QPointerData &&src) noexcept : Pointer{src.Pointer} {
        src.Pointer = nullptr;
    }

    inline explicit QPointerData(Type_T *pointer) noexcept : Pointer{pointer} {
    }

    inline void SetPointer(Type_T *pointer) noexcept {
        PointerNumber<Type_T> pn{pointer};
        PtrNumber &= 0xFFFF000000000000ULL;
        pn.Pointer = pointer;
        pn.Number &= 0xFFFFFFFFFFFFULL;
        PtrNumber |= pn.Number;
    }

    inline Type_T *GetPointer() const noexcept {
        return PointerNumber<Type_T>{PtrNumber & 0xFFFFFFFFFFFFULL}.Pointer;
    }

    inline void MovePointerOnly(QPointerData &src) noexcept {
        PtrNumber &= 0xFFFF000000000000ULL;
        PtrNumber |= (src.PtrNumber & 0xFFFFFFFFFFFFULL);
        src.PtrNumber = 0;
    }

    inline void SetHighByte(SizeT8 byte) noexcept {
        SizeT64 byte64 = byte;
        PtrNumber &= 0xFFFFFFFFFFFFFFULL;
        byte64 <<= 56U;
        PtrNumber |= byte64;
    }

    inline SizeT8 GetHighByte() const noexcept {
        SizeT64 byte64 = PtrNumber;
        byte64 &= 0xFF00000000000000ULL;
        byte64 >>= 56U;
        return SizeT8(byte64);
    }

    inline void SetLowByte(SizeT8 byte) noexcept {
        SizeT64 byte64 = byte;
        PtrNumber &= 0xFF00FFFFFFFFFFFFULL;
        byte64 <<= 48U;
        PtrNumber |= byte64;
    }

    inline SizeT8 GetLowByte() const noexcept {
        SizeT64 byte64 = PtrNumber;
        byte64 &= 0x00FF000000000000ULL;
        byte64 >>= 48U;
        return SizeT8(byte64);
    }

    union {
        Type_T       *Pointer{nullptr};
        SystemIntType PtrNumber;
    };
};

template <typename Type_T>
struct QPointerData<Type_T, false, true> {
    // 32-bit with Tag, devices with less than 16 megabytes of memory
    QPointerData() noexcept  = default;
    ~QPointerData() noexcept = default;

    QPointerData(const QPointerData &)            = delete;
    QPointerData &operator=(QPointerData &&)      = delete;
    QPointerData &operator=(const QPointerData &) = delete;

    inline QPointerData(QPointerData &&src) noexcept : Pointer{src.Pointer} {
        src.Pointer = nullptr;
    }

    inline explicit QPointerData(Type_T *pointer) noexcept : Pointer{pointer} {
    }

    inline void SetPointer(Type_T *pointer) noexcept {
        PointerNumber<Type_T> pn{pointer};
        PtrNumber &= 0xFF000000U;
        pn.Pointer = pointer;
        pn.Number &= 0xFFFFFFU;
        PtrNumber |= pn.Number;
    }

    inline Type_T *GetPointer() const noexcept {
        return PointerNumber<Type_T>{PtrNumber & 0xFFFFFFU}.Pointer;
    }

    inline void MovePointerOnly(QPointerData &src) noexcept {
        PtrNumber &= 0xFF000000U;
        PtrNumber |= (src.PtrNumber & 0xFFFFFFU);
        src.PtrNumber = 0;
    }

    inline void SetHighByte(SizeT8 byte) noexcept {
        SizeT64 byte64 = byte;
        PtrNumber &= 0xFFFFFFU;
        byte64 <<= 24U;
        PtrNumber |= byte64;
    }

    inline SizeT8 GetHighByte() const noexcept {
        SizeT64 byte64 = PtrNumber;
        byte64 &= 0xFF000000U;
        byte64 >>= 24U;
        return SizeT8(byte64);
    }

    inline void SetLowByte(SizeT8) noexcept {
    }

    inline SizeT8 GetLowByte() const noexcept {
        return 0;
    }

    union {
        Type_T       *Pointer{nullptr};
        SystemIntType PtrNumber;
    };
};

template <typename Type_T, bool Is64Bit>
struct QPointerData<Type_T, Is64Bit, false> {
    // Without Tag
    QPointerData() noexcept  = default;
    ~QPointerData() noexcept = default;

    QPointerData(const QPointerData &)            = delete;
    QPointerData &operator=(QPointerData &&)      = delete;
    QPointerData &operator=(const QPointerData &) = delete;

    inline QPointerData(QPointerData &&src) noexcept : Pointer{src.Pointer} {
        src.Pointer = nullptr;
    }

    inline explicit QPointerData(Type_T *pointer) noexcept : Pointer{pointer} {
    }

    inline void SetPointer(Type_T *pointer) noexcept {
        Pointer = pointer;
    }

    inline Type_T *GetPointer() const noexcept {
        return Pointer;
    }

    inline void MovePointerOnly(QPointerData &src) noexcept {
        Pointer     = src.Pointer;
        src.Pointer = nullptr;
    }

    inline void SetHighByte(SizeT8) noexcept {
    }

    inline SizeT8 GetHighByte() const noexcept {
        return 0;
    }

    inline void SetLowByte(SizeT8) noexcept {
    }

    inline SizeT8 GetLowByte() const noexcept {
        return 0;
    }

    Type_T *Pointer{nullptr};
};

template <typename Type_T>
struct QPointer {
    QPointer() noexcept                      = default;
    inline QPointer(QPointer &&src) noexcept = default;
    ~QPointer() noexcept                     = default;
    QPointer(const QPointer &src)            = delete;
    QPointer &operator=(const QPointer &src) = delete;

    inline explicit QPointer(Type_T *pointer) noexcept : _data{pointer} {
    }

    inline QPointer &operator=(QPointer &&src) noexcept {
        if (this != &src) {
            _data.Pointer     = src._data.Pointer;
            src._data.Pointer = nullptr;
        }

        return *this;
    }

    inline void SetPointer(Type_T *pointer) noexcept {
        _data.SetPointer(pointer);
    }

    inline Type_T *GetPointer() const noexcept {
        return _data.GetPointer();
    }

    inline void MovePointerOnly(QPointer &src) noexcept {
        _data.MovePointerOnly(src._data);
    }

    inline void SetHighByte(SizeT8 byte) noexcept {
        _data.SetHighByte(byte);
    }

    inline void SetLowByte(SizeT8 byte) noexcept {
        _data.SetLowByte(byte);
    }

    inline SizeT8 GetLowByte() const noexcept {
        return _data.GetLowByte();
    }

    inline SizeT8 GetHighByte() const noexcept {
        return _data.GetHighByte();
    }

    inline void Reset() noexcept {
        _data.Pointer = nullptr;
    }

  private:
    QPointerData<Type_T, Config::Is64bit, Config::PointerTagging> _data;
};

} // namespace Qentem

#endif
