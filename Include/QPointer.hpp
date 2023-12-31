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

#ifndef QENTEM_Q_POINTER_H
#define QENTEM_Q_POINTER_H

#include "Common.hpp"

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

template <typename Type_T, bool>
struct QPointerData;

template <typename Type_T>
struct QPointerData<Type_T, true> {
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
struct QPointerData<Type_T, false> {
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

    inline explicit QPointer(Type_T *pointer) noexcept : data_{pointer} {
    }

    inline QPointer &operator=(QPointer &&src) noexcept {
        if (this != &src) {
            data_.Pointer     = src.data_.Pointer;
            src.data_.Pointer = nullptr;
        }

        return *this;
    }

    inline void SetPointer(Type_T *pointer) noexcept {
        data_.SetPointer(pointer);
    }

    inline Type_T *GetPointer() const noexcept {
        return data_.GetPointer();
    }

    inline void MovePointerOnly(QPointer &src) noexcept {
        data_.MovePointerOnly(src.data_);
    }

    inline void SetHighByte(SizeT8 byte) noexcept {
        data_.SetHighByte(byte);
    }

    inline void SetLowByte(SizeT8 byte) noexcept {
        data_.SetLowByte(byte);
    }

    inline SizeT8 GetLowByte() const noexcept {
        return data_.GetLowByte();
    }

    inline SizeT8 GetHighByte() const noexcept {
        return data_.GetHighByte();
    }

    inline void Reset() noexcept {
        data_.Pointer = nullptr;
    }

  private:
    QPointerData<Type_T, Config::PointerTagging> data_;
};

} // namespace Qentem

#endif
