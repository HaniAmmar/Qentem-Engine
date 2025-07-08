/**
 * @file QCommon.hpp
 * @brief Core configuration, type aliases, and platform traits for Qentem Engine.
 *
 * This header defines architecture-specific constants, fixed-width integer aliases,
 * platform macros, endianness detection, SIMD flags, and utility constants that are
 * shared across all Qentem modules.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_COMMON_H
#define QENTEM_COMMON_H

#include "QUtility.hpp"

namespace Qentem {

///////////////////////////////////////////////////////////////
//               Type Width Detection (32/64-bit)            //
///////////////////////////////////////////////////////////////

template <unsigned>
struct SystemIntDefs {};

template <>
struct SystemIntDefs<8U> {
    // 64-bit
    using SizeT32  = unsigned int;
    using SizeT32I = int;

#ifdef _MSC_VER
    using SizeT64  = unsigned long long;
    using SizeT64I = long long;
#else
    using SizeT64  = unsigned long;
    using SizeT64I = long;
#endif
};

template <>
struct SystemIntDefs<4U> {
    // 32-bit
    using SizeT32  = unsigned int;
    using SizeT32I = int;
    using SizeT64  = unsigned long long;
    using SizeT64I = long long;
};

using NullType = decltype(nullptr);

using SizeT8   = unsigned char;
using SizeT8I  = signed char;
using SizeT16  = unsigned short;
using SizeT16I = short;

using SizeT32  = SystemIntDefs<sizeof(void *)>::SizeT32;
using SizeT32I = SystemIntDefs<sizeof(void *)>::SizeT32I;
using SizeT64  = SystemIntDefs<sizeof(void *)>::SizeT64;
using SizeT64I = SystemIntDefs<sizeof(void *)>::SizeT64I;

///////////////////////////////////////////////////////////////
//                   System Integer Type                     //
///////////////////////////////////////////////////////////////

template <SizeT32>
struct SystemIntTypeT {};

template <>
struct SystemIntTypeT<8U> {
    using NumberType  = SizeT64;
    using NumberTypeI = SizeT64I;
    using SizeType    = SizeT32;
};

template <>
struct SystemIntTypeT<4U> {
    using NumberType  = SizeT32;
    using NumberTypeI = SizeT32I;
    using SizeType    = SizeT32;
};

using SystemIntType_All = SystemIntTypeT<sizeof(void *)>;

#ifndef QENTEM_SIZE_T
#define QENTEM_SIZE_T SystemIntType_All::SizeType
#endif

using SizeT          = QENTEM_SIZE_T;
using SystemIntType  = SystemIntType_All::NumberType;
using SystemIntTypeI = SystemIntType_All::NumberTypeI;

///////////////////////////////////////////////////////////////
//                    Pointer Casting                        //
///////////////////////////////////////////////////////////////

template <typename Type_T>
union PtrCast_T {
    Type_T       *Pointer;
    SystemIntType Number;
};

///////////////////////////////////////////////////////////////
//              Configuration Macros (Overridable)           //
///////////////////////////////////////////////////////////////

#ifndef QENTEM_ENABLE_FLOAT_16
#define QENTEM_ENABLE_FLOAT_16 0
#endif

#ifndef QENTEM_ENABLE_BFLOAT_16
#define QENTEM_ENABLE_BFLOAT_16 0
#endif

#ifndef QENTEM_DOUBLE_PRECISION
#define QENTEM_DOUBLE_PRECISION 15U
#endif

#ifndef QENTEM_TEMPLATE_PRECISION
#define QENTEM_TEMPLATE_PRECISION 2U
#endif

#ifndef QENTEM_TEMPLATE_DOUBLE_FORMAT
#define QENTEM_TEMPLATE_DOUBLE_FORMAT Digit::RealFormatType::SemiFixed // Default, Fixed, SemiFixed
#endif

///////////////////////////////////////////////////////////////
//                 Qentem Build Configuration                //
///////////////////////////////////////////////////////////////

struct QentemConfig {
    static constexpr SizeT32 DoublePrecision   = QENTEM_DOUBLE_PRECISION;
    static constexpr SizeT32 TemplatePrecision = QENTEM_TEMPLATE_PRECISION;
    static constexpr SizeT32 PointerSize       = sizeof(void *);
    static constexpr bool    Is64bit           = (PointerSize == 8U);

    ///////////////////////////////////////////////////////////
    //                  Endianness Detection                 //
    ///////////////////////////////////////////////////////////

#ifndef QENTEM_BIG_ENDIAN
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define QENTEM_BIG_ENDIAN 1
#endif
#endif

#if defined(QENTEM_BIG_ENDIAN) && (QENTEM_BIG_ENDIAN == 1)
    static constexpr bool IsBigEndian = true;
#else
    static constexpr bool IsBigEndian = false;
#endif

    ///////////////////////////////////////////////////////////
    //              Template Engine Behavior                 //
    ///////////////////////////////////////////////////////////

#ifndef QENTEM_AUTO_ESCAPE_HTML
#define QENTEM_AUTO_ESCAPE_HTML 1
#endif

#if defined(QENTEM_AUTO_ESCAPE_HTML) && (QENTEM_AUTO_ESCAPE_HTML == 1)
    static constexpr bool AutoEscapeHTML = true;
#else
    static constexpr bool AutoEscapeHTML = false;
#endif

    ///////////////////////////////////////////////////////////
    //                   SIMD Feature Flags                  //
    ///////////////////////////////////////////////////////////

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
    static constexpr bool IsSIMDEnabled = true;
#define QENTEM_SIMD_ENABLED 1
#else
    static constexpr bool IsSIMDEnabled = false;
#undef QENTEM_SIMD_ENABLED
#endif
};

///////////////////////////////////////////////////////////////
//               Extended Floating Types (Optional)          //
///////////////////////////////////////////////////////////////

#if defined(QENTEM_ENABLE_BFLOAT_16) && (QENTEM_ENABLE_BFLOAT_16 == 1)
using BFloatT16 = decltype(0.0BF16); // bfloat16_t, requires C++23
#endif

///////////////////////////////////////////////////////////////
//               Compiler Intrinsic Macros                   //
///////////////////////////////////////////////////////////////

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
#else
#define QENTEM_CONST_EXPRESSION
#endif

} // namespace Qentem

#endif // QENTEM_COMMON_H
