/**
 * @file QCommon.hpp
 * @brief Core configuration, type aliases, and platform traits for Qentem Engine.
 *
 * This header defines architecture-specific constants, fixed-width integer aliases,
 * platform macros, endianness detection, SIMD flags, and utility constants that are
 * shared across all Qentem modules.
 *
 * @author Hani Ammar
 * @date 2026
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

using SystemIntTypes = SystemIntTypeT<sizeof(void *)>;

#ifndef QENTEM_SIZE_T
#define QENTEM_SIZE_T SystemIntTypes::SizeType
#endif

using SizeT       = QENTEM_SIZE_T;
using SystemLong  = SystemIntTypes::NumberType;
using SystemLongI = SystemIntTypes::NumberTypeI;

///////////////////////////////////////////////////////////////
//                    Pointer Casting                        //
///////////////////////////////////////////////////////////////

template <typename Type_T>
union PtrCast_T {
    Type_T    *Pointer;
    SystemLong Number;
};

///////////////////////////////////////////////////////////////
//                  Reserver Configuration                  //
///////////////////////////////////////////////////////////////
/// Fallback page size (4KiB) used if system page size cannot be queried.
#ifndef QENTEM_FALLBACK_SYSTEM_PAGE_SIZE
#define QENTEM_FALLBACK_SYSTEM_PAGE_SIZE 4096
#endif

/**
 * @brief Enables fallback memory handling for unsupported or freestanding platforms.
 *
 * Define this macro to bypass platform-specific memory APIs (e.g., mmap, VirtualAlloc)
 * and use standard heap-based functions (e.g., malloc) instead.
 *
 * When defined, page size defaults to QENTEM_FALLBACK_SYSTEM_PAGE_SIZE.
 */
// #define QENTEM_SYSTEM_MEMORY_FALLBACK ///< Define to disable mmap/VirtualAlloc use

/**
 * @brief Default memory alignment applied to all reserve requests unless overridden.
 *
 * The alignment is set to (sizeof(void*) * 2), ensuring compatibility with
 * all fundamental types:
 *   - 16 bytes on 64-bit systems
 *   - 8 bytes on 32-bit systems
 *
 * This ensures compatibility with all fundamental types and most SIMD types on modern platforms,
 * including safe support for `double`, `long long`, and `max_align_t` on x86_64.
 */
#ifndef QENTEM_RESERVER_DEFAULT_ALIGNMENT
#define QENTEM_RESERVER_DEFAULT_ALIGNMENT (sizeof(void *) * 2)
#endif

/**
 * @brief Fixed block size used by the Reserver memory allocator.
 *
 * All allocations are drawn from fixed-size memory blocks of this size.
 * Blocks are never resized or grown; memory expansion occurs only via the
 * addition of new blocks of the same size.
 *
 * This design ensures:
 *   - Predictable memory layout
 *   - Improved cache behavior
 *   - Easier fragmentation control
 *
 * Recommended sizes:
 *   - 64 KiB for constrained embedded systems
 *   - 128–1024 KiB for desktop or server-class deployments
 */
#ifndef QENTEM_RESERVER_BLOCK_SIZE
#define QENTEM_RESERVER_BLOCK_SIZE (256U * 1024U) // 256 KiB
#endif

///////////////////////////////////////////////////////////////
//              Configuration Macros (Overridable)           //
///////////////////////////////////////////////////////////////
#ifndef QENTEM_MAX_CPU_CORES
#define QENTEM_MAX_CPU_CORES 128
#endif

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

} // namespace Qentem

#endif // QENTEM_COMMON_H
