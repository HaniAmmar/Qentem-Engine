/**
 * @file QNumber.hpp
 * @brief Provides number utilities and conversions for Qentem Engine.
 *
 * This header defines functions and types for numeric manipulation, parsing,
 * formatting, and conversion, supporting a wide range of integer and floating-point
 * operations throughout the Qentem Engine library.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_Q_NUMBER_H
#define QENTEM_Q_NUMBER_H

#include "QCommon.hpp"

namespace Qentem {

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
        if QENTEM_CONST_EXPRESSION (IsFloat<Number_T>()) {
            Real = double(num);
        } else if QENTEM_CONST_EXPRESSION (IsUnsigned<Number_T>()) {
            Natural = SizeT64(num);
        } else {
            Integer = SizeT64I(num);
        }
    }

    inline QNumber64 &operator=(const SizeT64 num) noexcept {
        Natural = num;
        return *this;
    }

    inline QNumber64 &operator=(const SizeT64I num) noexcept {
        Integer = num;
        return *this;
    }

    inline QNumber64 &operator=(const double num) noexcept {
        Real = num;
        return *this;
    }

    template <typename Number_T>
    inline QNumber64 &operator=(const Number_T num) noexcept {
        if QENTEM_CONST_EXPRESSION (IsFloat<Number_T>()) {
            Real = double(num);
        } else if QENTEM_CONST_EXPRESSION (IsUnsigned<Number_T>()) {
            Natural = SizeT64(num);
        } else {
            Integer = SizeT64I(num);
        }

        return *this;
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
        if QENTEM_CONST_EXPRESSION (IsFloat<Number_T>()) {
            Real = float(num);
        } else if QENTEM_CONST_EXPRESSION (IsUnsigned<Number_T>()) {
            Natural = SizeT32(num);
        } else {
            Integer = SizeT32I(num);
        }
    }

    inline QNumber32 &operator=(const SizeT32 num) noexcept {
        Natural = num;
        return *this;
    }

    inline QNumber32 &operator=(const SizeT32I num) noexcept {
        Integer = num;
        return *this;
    }

    inline QNumber32 &operator=(const float num) noexcept {
        Real = num;
        return *this;
    }

    template <typename Number_T>
    inline QNumber32 &operator=(const Number_T num) noexcept {
        if QENTEM_CONST_EXPRESSION (IsFloat<Number_T>()) {
            Real = float(num);
        } else if QENTEM_CONST_EXPRESSION (IsUnsigned<Number_T>()) {
            Natural = SizeT32(num);
        } else {
            Integer = SizeT32I(num);
        }

        return *this;
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

    inline explicit QNumber16(const SizeT16 num) noexcept : Natural{num} {
    }

    inline explicit QNumber16(const short num) noexcept : Integer{num} {
    }

    template <typename Number2_T>
    inline explicit QNumber16(const Number2_T num) noexcept {
#if (defined(QENTEM_ENABLE_FLOAT_16) && (QENTEM_ENABLE_FLOAT_16 == 1)) ||                                              \
    (defined(QENTEM_ENABLE_BFLOAT_16) && (QENTEM_ENABLE_BFLOAT_16 == 1))
        if QENTEM_CONST_EXPRESSION (IsFloat<Number2_T>()) {
            Real = num;
        } else
#endif
            if QENTEM_CONST_EXPRESSION (IsUnsigned<Number2_T>()) {
            Natural = SizeT16(num);
        } else {
            Integer = short(num);
        }
    }

    inline QNumber16 &operator=(const SizeT16 num) noexcept {
        Natural = num;
        return *this;
    }

    inline QNumber16 &operator=(const short num) noexcept {
        Integer = num;
        return *this;
    }

    template <typename Number2_T>
    inline QNumber16 &operator=(const Number2_T num) noexcept {
#if (defined(QENTEM_ENABLE_FLOAT_16) && (QENTEM_ENABLE_FLOAT_16 == 1)) ||                                              \
    (defined(QENTEM_ENABLE_BFLOAT_16) && (QENTEM_ENABLE_BFLOAT_16 == 1))
        if QENTEM_CONST_EXPRESSION (IsFloat<Number2_T>()) {
            Real = num;
        } else
#endif
            if QENTEM_CONST_EXPRESSION (IsUnsigned<Number2_T>()) {
            Natural = SizeT16(num);
        } else {
            Integer = short(num);
        }

        return *this;
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

    template <typename Number_T>
    inline explicit QNumber8(const Number_T num) noexcept {
        if QENTEM_CONST_EXPRESSION (IsUnsigned<Number_T>()) {
            Natural = SizeT8(num);
        } else {
            Integer = SizeT8I(num);
        }
    }

    template <typename Number_T>
    inline QNumber8 &operator=(const Number_T num) noexcept {
        if QENTEM_CONST_EXPRESSION (IsUnsigned<Number_T>()) {
            Natural = SizeT8(num);
        } else {
            Integer = SizeT8I(num);
        }

        return *this;
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
