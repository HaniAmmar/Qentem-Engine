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

#ifndef QENTEM_COMMON_H
#define QENTEM_COMMON_H

namespace Qentem {

using NullType = decltype(nullptr);
using SizeT8   = unsigned char;
using SizeT8I  = signed char;
using SizeT16  = unsigned short;
using SizeT32  = unsigned int;
using SizeT32I = int;
using SizeT64  = unsigned long long;
using SizeT64I = long long;
///////////////////////////////////////////////
template <SizeT32>
struct SystemIntTypeT {};

template <>
struct SystemIntTypeT<8U> {
    // 64-bit
    using NumberType  = SizeT64;
    using NumberTypeI = SizeT64I;
    using SizeType    = SizeT32;
};

template <>
struct SystemIntTypeT<4U> {
    // 32-bit
    using NumberType  = SizeT32;
    using NumberTypeI = SizeT32I;
    using SizeType    = SizeT32;
};

using SystemIntType_All = SystemIntTypeT<sizeof(void *)>;
///////////////////////////////////////////////
#ifndef QENTEM_SIZE_T
#define QENTEM_SIZE_T SystemIntType_All::SizeType
#endif

using SizeT          = QENTEM_SIZE_T;
using SystemIntType  = SystemIntType_All::NumberType;
using SystemIntTypeI = SystemIntType_All::NumberTypeI;
///////////////////////////////////////////////
#ifndef QENTEM_ENABLE_FLOAT_16
// Requires c++23
#define QENTEM_ENABLE_FLOAT_16 0
#endif

#ifndef QENTEM_ENABLE_BFLOAT_16
// Requires c++23
#define QENTEM_ENABLE_BFLOAT_16 0
#endif

#ifndef QENTEM_DOUBLE_PRECISION
#define QENTEM_DOUBLE_PRECISION 15U
#endif

#ifndef QENTEM_TEMPLATE_PRECISION
#define QENTEM_TEMPLATE_PRECISION 2U
#endif

#ifndef QENTEM_TEMPLATE_DOUBLE_FORMAT
// Default, Fixed, SemiFixed
#define QENTEM_TEMPLATE_DOUBLE_FORMAT Digit::RealFormatType::SemiFixed
#endif
///////////////////////////////////////////////
struct Config {
    static constexpr SizeT32 DoublePrecision{QENTEM_DOUBLE_PRECISION};
    static constexpr SizeT32 TemplatePrecision{QENTEM_TEMPLATE_PRECISION};
    static constexpr SizeT32 PointerSize{sizeof(void *)};
    static constexpr bool    Is64bit{PointerSize == 8U};

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
#define QENTEM_SIMD_ENABLED 1
#else
    static constexpr bool IsSIMDEnabled{false};
#undef QENTEM_SIMD_ENABLED
#endif
};
///////////////////////////////////////////////
#if defined(QENTEM_ENABLE_BFLOAT_16) && (QENTEM_ENABLE_BFLOAT_16 == 1)
// bfloat16_t
using BFloatT16 = decltype(0.0BF16);
#endif
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

#if (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)) || (defined(__cplusplus) && (__cplusplus >= 201703L))
#define QENTEM_CONST_EXPRESSION constexpr
#define QENTEM_FALL_THROUGH [[fallthrough]]
#else
#define QENTEM_CONST_EXPRESSION
#define QENTEM_FALL_THROUGH
#endif
} // namespace Qentem

#endif
