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

#ifndef QENTEM_COMMON_H_
#define QENTEM_COMMON_H_

namespace Qentem {

using NullType = decltype(nullptr);
using SizeT8   = unsigned char;
using SizeT16  = unsigned short;
using SizeT32  = unsigned int;
using SizeT64  = unsigned long long;
using SizeT64I = long long;

#ifndef QENTEM_SIZE_T
#define QENTEM_SIZE_T
using SizeT = SizeT32;
#endif

// #define QENTEM_ENABLE_FLOAT_16 1
// #define QENTEM_ENABLE_BFLOAT_16 1

#ifndef QENTEM_DOUBLE_PRECISION
#define QENTEM_DOUBLE_PRECISION 15U
#endif

#ifndef QENTEM_TEMPLATE_PRECISION
#define QENTEM_TEMPLATE_PRECISION 3U
#endif

struct Config {
    static constexpr SizeT32 DoublePrecision{QENTEM_DOUBLE_PRECISION};
    static constexpr SizeT32 TemplatePrecision{QENTEM_TEMPLATE_PRECISION};
    static constexpr SizeT32 PointerSize{sizeof(void *)};
    static constexpr bool    Is64bit{PointerSize == SizeT32{8}};

    ///////////////////////////////////////////////
#ifndef QENTEM_BIG_ENDIAN
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define QENTEM_BIG_ENDIAN 1
#endif
#endif
///////////////////////////////////////////////
#if defined(QENTEM_BIG_ENDIAN) && (QENTEM_BIG_ENDIAN == 1)
    static constexpr bool IsBigEndian{true};
#else
    static constexpr bool IsBigEndian{false};
#endif
///////////////////////////////////////////////
#ifndef QENTEM_POINTER_TAGGING
#define QENTEM_POINTER_TAGGING 1
#endif

#ifndef QENTEM_SSO
#define QENTEM_SSO 1
#endif
///////////////////////////////////////////////
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
    static constexpr bool PointerTagging{Is64bit};
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
    static constexpr bool ShortStringOptimization{PointerTagging && (sizeof(SizeT) >= 4U)};
#else
    static constexpr bool ShortStringOptimization{false};
#undef QENTEM_SSO
#endif
#else
    static constexpr bool PointerTagging{false};
    static constexpr bool ShortStringOptimization{false};
#undef QENTEM_POINTER_TAGGING
#endif

    ///////////////////////////////////////////////

#ifndef QENTEM_AUTO_ESCAPE_HTML
#define QENTEM_AUTO_ESCAPE_HTML 1
#endif
///////////////////////////////////////////////
#if defined(QENTEM_AUTO_ESCAPE_HTML) && (QENTEM_AUTO_ESCAPE_HTML == 1)
    static constexpr bool AutoEscapeHTML{true};
#else
    static constexpr bool AutoEscapeHTML{false};
#endif
///////////////////////////////////////////////
#ifndef QENTEM_AVX2
#define QENTEM_AVX2 0
#endif

#ifndef QENTEM_SSE2
#define QENTEM_SSE2 0
#endif

#ifndef __AVX2__
#undef QENTEM_AVX2
#endif

#ifndef __SSE2__
#undef QENTEM_SSE2
#endif

#if (QENTEM_AVX2 == 1) || (QENTEM_SSE2 == 1) || (QENTEM_MSIMD128 == 1)
    static constexpr bool IsSIMDEnabled{true};
#else
    static constexpr bool IsSIMDEnabled{false};
#endif
};
///////////////////////////////////////////////

#ifdef _MSC_VER
#define QENTEM_NOINLINE __declspec(noinline)
#define QENTEM_INLINE __forceinline
#define QENTEM_MAYBE_UNUSED
#else
#define QENTEM_NOINLINE __attribute__((noinline))
#define QENTEM_INLINE __attribute__((always_inline))
#define QENTEM_MAYBE_UNUSED __attribute__((unused))
#endif
///////////////////////////////////////////////
#if defined(QENTEM_ENABLE_FLOAT_16) && (QENTEM_ENABLE_FLOAT_16 == 1)
// float16_t, _Float16
using FloatT16 = decltype(0.0F16);
#endif

#if defined(QENTEM_ENABLE_BFLOAT_16) && (QENTEM_ENABLE_BFLOAT_16 == 1)
// bfloat16_t
using BFloatT16 = decltype(0.0BF16);
#endif
///////////////////////////////////////////////
template <SizeT32>
struct SystemIntTypeT {};

template <>
struct SystemIntTypeT<8U> {
    using NumberType_ = SizeT64;
};

template <>
struct SystemIntTypeT<4U> {
    using NumberType_ = SizeT32;
};

using SystemIntType = typename SystemIntTypeT<Config::PointerSize>::NumberType_;

enum class QNumberType : unsigned char {
    NotANumber = 0,
    Real       = 1, // double
    Natural    = 2, // unsigned
    Integer    = 3  // signed
};
///////////////////////////////////////////////
template <typename Type_>
static constexpr bool IsFloat() {
    return (Type_(0.5) != 0);
}

template <typename Type_>
static constexpr bool IsUnsigned() {
    return (Type_(-1) > 0);
}
///////////////////////////////////////////////
union QNumber64 {
    QNumber64() noexcept                             = default;
    QNumber64(QNumber64 &&) noexcept                 = default;
    QNumber64(const QNumber64 &) noexcept            = default;
    QNumber64 &operator=(QNumber64 &&) noexcept      = default;
    QNumber64 &operator=(const QNumber64 &) noexcept = default;
    ~QNumber64() noexcept                            = default;

    template <typename Number_T_>
    explicit QNumber64(Number_T_ num) noexcept {
        if (IsFloat<Number_T_>()) {
            Real = double(num);
        } else if (IsUnsigned<Number_T_>()) {
            Natural = SizeT64(num);
        } else {
            Integer = SizeT64I(num);
        }
    }

    SizeT64  Natural{0};
    SizeT64I Integer;
    double   Real;
};

union QNumber32 {
    QNumber32() noexcept                             = default;
    QNumber32(QNumber32 &&) noexcept                 = default;
    QNumber32(const QNumber32 &) noexcept            = default;
    QNumber32 &operator=(QNumber32 &&) noexcept      = default;
    QNumber32 &operator=(const QNumber32 &) noexcept = default;
    ~QNumber32() noexcept                            = default;

    template <typename Number_T_>
    explicit QNumber32(Number_T_ num) noexcept {
        if (IsFloat<Number_T_>()) {
            Real = float(num);
        } else if (IsUnsigned<Number_T_>()) {
            Natural = SizeT32(num);
        } else {
            Integer = int(num);
        }
    }

    SizeT32 Natural{0};
    int     Integer;
    float   Real;
};

template <typename Number_T_>
union QNumber16 {
    QNumber16() noexcept                             = default;
    QNumber16(QNumber16 &&) noexcept                 = default;
    QNumber16(const QNumber16 &) noexcept            = default;
    QNumber16 &operator=(QNumber16 &&) noexcept      = default;
    QNumber16 &operator=(const QNumber16 &) noexcept = default;
    ~QNumber16() noexcept                            = default;

    explicit QNumber16(Number_T_ num) noexcept {
        if (IsFloat<Number_T_>()) {
            Real = Number_T_(num);
        } else if (IsUnsigned<Number_T_>()) {
            Natural = SizeT16(num);
        } else {
            Integer = short(num);
        }
    }

    SizeT16   Natural{0};
    short     Integer;
    Number_T_ Real; // float16 or whatever.
};

union QNumber8 {
    QNumber8() noexcept                            = default;
    QNumber8(QNumber8 &&) noexcept                 = default;
    QNumber8(const QNumber8 &) noexcept            = default;
    QNumber8 &operator=(QNumber8 &&) noexcept      = default;
    QNumber8 &operator=(const QNumber8 &) noexcept = default;
    ~QNumber8() noexcept                           = default;

    template <typename Number_T_>
    explicit QNumber8(Number_T_ num) noexcept {
        if (IsUnsigned<Number_T_>()) {
            Natural = (unsigned char)(num);
        } else {
            Integer = char(num);
        }
    }

    unsigned char Natural{0};
    char          Integer;
};
///////////////////////////////////////////////
template <typename Number_T_, SizeT32>
struct QNumberAutoTypeT {};

template <typename Number_T_>
struct QNumberAutoTypeT<Number_T_, 1U> {
    using QNumberType_T = QNumber8;
};

template <typename Number_T_>
struct QNumberAutoTypeT<Number_T_, 2U> {
    using QNumberType_T = QNumber16<Number_T_>;
};

template <typename Number_T_>
struct QNumberAutoTypeT<Number_T_, 4U> {
    using QNumberType_T = QNumber32;
};

template <typename Number_T_>
struct QNumberAutoTypeT<Number_T_, 8U> {
    using QNumberType_T = QNumber64;
};

} // namespace Qentem

#endif
