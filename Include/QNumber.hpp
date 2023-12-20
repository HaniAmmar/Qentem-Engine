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

#ifndef QENTEM_Q_NUMBER_H
#define QENTEM_Q_NUMBER_H

namespace Qentem {
///////////////////////////////////////////////
enum struct QNumberType : SizeT8 {
    NotANumber = 0, // NAN
    Real       = 1, // double
    Natural    = 2, // unsigned
    Integer    = 3  // signed
};
///////////////////////////////////////////////
template <typename Type_T>
inline static constexpr bool IsFloat() {
    return (Type_T(0.5) != 0);
}

template <typename Type_T>
inline static constexpr bool IsUnsigned() {
    return (Type_T(-1) > 0);
}
///////////////////////////////////////////////
union QNumber64 {
    QNumber64() noexcept                             = default;
    QNumber64(QNumber64 &&) noexcept                 = default;
    QNumber64(const QNumber64 &) noexcept            = default;
    QNumber64 &operator=(QNumber64 &&) noexcept      = default;
    QNumber64 &operator=(const QNumber64 &) noexcept = default;
    ~QNumber64() noexcept                            = default;

    inline explicit QNumber64(const SizeT64 num) noexcept : Natural{num} {
    }

    inline explicit QNumber64(const SizeT64I num) noexcept : Integer{num} {
    }

    inline explicit QNumber64(const double num) noexcept : Real{num} {
    }

    template <typename Number_T>
    inline explicit QNumber64(const Number_T num) noexcept {
        if (IsFloat<Number_T>()) {
            Real = double(num);
        } else if (IsUnsigned<Number_T>()) {
            Natural = SizeT64(num);
        } else {
            Integer = SizeT64I(num);
        }
    }

    inline void operator=(const SizeT64 num) noexcept {
        Natural = num;
    }

    inline void operator=(const SizeT64I num) noexcept {
        Integer = num;
    }

    inline void operator=(const double num) noexcept {
        Real = num;
    }

    template <typename Number_T>
    inline void operator=(const Number_T num) noexcept {
        if (IsFloat<Number_T>()) {
            Real = double(num);
        } else if (IsUnsigned<Number_T>()) {
            Natural = SizeT64(num);
        } else {
            Integer = SizeT64I(num);
        }
    }

    SizeT64  Natural{0};
    SizeT64I Integer;
    double   Real;
};
////////////////////
union QNumber32 {
    QNumber32() noexcept                             = default;
    QNumber32(QNumber32 &&) noexcept                 = default;
    QNumber32(const QNumber32 &) noexcept            = default;
    QNumber32 &operator=(QNumber32 &&) noexcept      = default;
    QNumber32 &operator=(const QNumber32 &) noexcept = default;
    ~QNumber32() noexcept                            = default;

    inline explicit QNumber32(const SizeT32 num) noexcept : Natural{num} {
    }

    inline explicit QNumber32(const SizeT32I num) noexcept : Integer{num} {
    }

    inline explicit QNumber32(const float num) noexcept : Real{num} {
    }

    template <typename Number_T>
    inline explicit QNumber32(const Number_T num) noexcept {
        if (IsFloat<Number_T>()) {
            Real = float(num);
        } else if (IsUnsigned<Number_T>()) {
            Natural = SizeT32(num);
        } else {
            Integer = SizeT32I(num);
        }
    }

    inline void operator=(const SizeT32 num) noexcept {
        Natural = num;
    }

    inline void operator=(const SizeT32I num) noexcept {
        Integer = num;
    }

    inline void operator=(const float num) noexcept {
        Real = num;
    }

    template <typename Number_T>
    inline void operator=(const Number_T num) noexcept {
        if (IsFloat<Number_T>()) {
            Real = float(num);
        } else if (IsUnsigned<Number_T>()) {
            Natural = SizeT32(num);
        } else {
            Integer = SizeT32I(num);
        }
    }

    SizeT32  Natural{0};
    SizeT32I Integer;
    float    Real;
};
////////////////////
template <typename Number_T>
union QNumber16 {
    QNumber16() noexcept                             = default;
    QNumber16(QNumber16 &&) noexcept                 = default;
    QNumber16(const QNumber16 &) noexcept            = default;
    QNumber16 &operator=(QNumber16 &&) noexcept      = default;
    QNumber16 &operator=(const QNumber16 &) noexcept = default;
    ~QNumber16() noexcept                            = default;

    // inline explicit QNumber16(const SizeT16 num) noexcept : Natural{num} {
    // }

    inline explicit QNumber16(const Number_T num) noexcept {
#if (defined(QENTEM_ENABLE_FLOAT_16) && (QENTEM_ENABLE_FLOAT_16 == 1)) ||                                              \
    (defined(QENTEM_ENABLE_BFLOAT_16) && (QENTEM_ENABLE_BFLOAT_16 == 1))
        if (IsFloat<Number_T>()) {
            Real = Number_T(num);
        } else
#endif
            if (IsUnsigned<Number_T>()) {
            Natural = SizeT16(num);
        } else {
            Integer = short(num);
        }
    }

    inline void operator=(const Number_T num) noexcept {
#if (defined(QENTEM_ENABLE_FLOAT_16) && (QENTEM_ENABLE_FLOAT_16 == 1)) ||                                              \
    (defined(QENTEM_ENABLE_BFLOAT_16) && (QENTEM_ENABLE_BFLOAT_16 == 1))
        if (IsFloat<Number_T>()) {
            Real = Number_T(num);
        } else
#endif
            if (IsUnsigned<Number_T>()) {
            Natural = SizeT16(num);
        } else {
            Integer = short(num);
        }
    }

    SizeT16  Natural{0};
    short    Integer;
    Number_T Real; // float16 or whatever.
};
////////////////////
union QNumber8 {
    QNumber8() noexcept                            = default;
    QNumber8(QNumber8 &&) noexcept                 = default;
    QNumber8(const QNumber8 &) noexcept            = default;
    QNumber8 &operator=(QNumber8 &&) noexcept      = default;
    QNumber8 &operator=(const QNumber8 &) noexcept = default;
    ~QNumber8() noexcept                           = default;

    inline explicit QNumber8(const SizeT8 num) noexcept : Natural{num} {
    }

    inline explicit QNumber8(const char num) noexcept : Integer{num} {
    }

    template <typename Number_T>
    inline explicit QNumber8(const Number_T num) noexcept {
        if (IsUnsigned<Number_T>()) {
            Natural = SizeT8(num);
        } else {
            Integer = char(num);
        }
    }

    inline void operator=(const SizeT8 num) noexcept {
        Natural = num;
    }

    inline void operator=(const char num) noexcept {
        Integer = num;
    }

    template <typename Number_T>
    inline void operator=(const Number_T num) noexcept {
        if (IsUnsigned<Number_T>()) {
            Natural = SizeT8(num);
        } else {
            Integer = char(num);
        }
    }

    SizeT8 Natural{0};
    char   Integer;
};
///////////////////////////////////////////////
template <typename Number_T, SizeT32>
struct QNumberAutoTypeT {};

template <typename Number_T>
struct QNumberAutoTypeT<Number_T, 1U> {
    using QNumberType_T = QNumber8;
};

template <typename Number_T>
struct QNumberAutoTypeT<Number_T, 2U> {
    using QNumberType_T = QNumber16<Number_T>;
};

template <typename Number_T>
struct QNumberAutoTypeT<Number_T, 4U> {
    using QNumberType_T = QNumber32;
};

template <typename Number_T>
struct QNumberAutoTypeT<Number_T, 8U> {
    using QNumberType_T = QNumber64;
};

} // namespace Qentem

#endif
