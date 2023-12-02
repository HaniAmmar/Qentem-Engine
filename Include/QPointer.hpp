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

#ifndef QENTEM_Q_POINTER_H_
#define QENTEM_Q_POINTER_H_

namespace Qentem {

template <typename Type_T_>
union PointerNumber {
    inline explicit PointerNumber(Type_T_ *pointer) noexcept : Pointer{pointer} {
    }

    inline explicit PointerNumber(unsigned long long number) noexcept : Number{number} {
    }

    Type_T_           *Pointer;
    unsigned long long Number{0};
};

template <typename Type_T_, bool>
struct QPointerData;

template <typename Type_T_>
struct QPointerData<Type_T_, true> {
    // With Tag
    QPointerData() = default;

    inline explicit QPointerData(Type_T_ *pointer) noexcept : Pointer{pointer} {
    }

    inline void SetPointer(Type_T_ *pointer) noexcept {
        PointerNumber<Type_T_> pn{pointer};
        PtrNumber &= 0xFFFF000000000000ULL;
        pn.Pointer = pointer;
        pn.Number &= 0xFFFFFFFFFFFFULL;
        PtrNumber |= pn.Number;
    }

    inline Type_T_ *GetPointer() const noexcept {
        return PointerNumber<Type_T_>{PtrNumber & 0xFFFFFFFFFFFFULL}.Pointer;
    }

    inline void MovePointerOnly(QPointerData &src) noexcept {
        PtrNumber &= 0xFFFF000000000000ULL;
        PtrNumber |= (src.PtrNumber & 0xFFFFFFFFFFFFULL);
    }

    inline void SetHighByte(unsigned char byte) noexcept {
        unsigned long long byte64 = byte;
        PtrNumber &= 0xFFFFFFFFFFFFFFULL;
        byte64 <<= 56U;
        PtrNumber |= byte64;
    }

    inline unsigned char GetHighByte() const noexcept {
        unsigned long long byte64 = PtrNumber;
        byte64 &= 0xFF00000000000000ULL;
        byte64 >>= 56U;
        return (unsigned char)(byte64);
    }

    inline void SetLowByte(unsigned char byte) noexcept {
        unsigned long long byte64 = byte;
        PtrNumber &= 0xFF00FFFFFFFFFFFFULL;
        byte64 <<= 48U;
        PtrNumber |= byte64;
    }
    inline unsigned char GetLowByte() const noexcept {
        unsigned long long byte64 = PtrNumber;
        byte64 &= 0x00FF000000000000ULL;
        byte64 >>= 48U;
        return (unsigned char)(byte64);
    }

    union {
        Type_T_           *Pointer{nullptr};
        unsigned long long PtrNumber;
    };
};

template <typename Type_T_>
struct QPointerData<Type_T_, false> {
    // Without Tag
    QPointerData() = default;

    inline explicit QPointerData(Type_T_ *pointer) noexcept : Pointer{pointer} {
    }

    inline void SetPointer(Type_T_ *pointer) noexcept {
        Pointer = pointer;
    }

    inline Type_T_ *GetPointer() const noexcept {
        return Pointer;
    }

    inline void MovePointerOnly(QPointerData &src) noexcept {
        Pointer = src.Pointer;
    }

    inline void SetHighByte(unsigned char byte) noexcept {
        (void)byte;
    }

    inline unsigned char GetHighByte() const noexcept {
        return 0;
    }

    inline void SetLowByte(unsigned char byte) noexcept {
        (void)byte;
    }

    inline unsigned char GetLowByte() const noexcept {
        return 0;
    }

    Type_T_ *Pointer{nullptr};
};

template <typename Type_T_>
class QPointer {
  public:
    QPointer()                               = default;
    ~QPointer()                              = default;
    QPointer(const QPointer &src)            = delete;
    QPointer &operator=(const QPointer &src) = delete;

    inline explicit QPointer(Type_T_ *pointer) noexcept : data_{pointer} {
    }

    inline QPointer(QPointer &&src) noexcept : data_{src.data_.Pointer} {
        src.data_.Pointer = nullptr;
    }

    inline QPointer &operator=(QPointer &&src) noexcept {
        if (this != &src) {
            data_.Pointer     = src.data_.Pointer;
            src.data_.Pointer = nullptr;
        }

        return *this;
    }

    inline void SetPointer(Type_T_ *pointer) noexcept {
        data_.SetPointer(pointer);
    }

    inline Type_T_ *GetPointer() const noexcept {
        return data_.GetPointer();
    }

    inline void MovePointerOnly(QPointer &src) noexcept {
        data_.MovePointerOnly(src.data_);
        src.data_.Pointer = nullptr;
    }

    inline void SetHighByte(unsigned char byte) noexcept {
        data_.SetHighByte(byte);
    }

    inline void SetLowByte(unsigned char byte) noexcept {
        data_.SetLowByte(byte);
    }

    inline unsigned char GetLowByte() const noexcept {
        return data_.GetLowByte();
    }

    inline unsigned char GetHighByte() const noexcept {
        return data_.GetHighByte();
    }

    inline void Reset() noexcept {
        data_.Pointer = nullptr;
    }

  private:
    QPointerData<Type_T_, Config::PointerTagging> data_;
};

} // namespace Qentem

#endif
