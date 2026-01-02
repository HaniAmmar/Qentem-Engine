/**
 * @file DigitUtils.hpp
 * @brief Provides advanced numeric constants and digit-processing utilities for Qentem Engine.
 *
 * This header defines internal constants and helper functions for fast, precise
 * numeric operations, including floating-point bit manipulation, power-of-five lookups,
 * and low-level digit processing. DigitUtils.hpp enables high-performance routines for
 * parsing, conversion, and mathematical operations, with special support for double and
 * integer types.
 *
 * The advanced constants and algorithms within this file are designed to maximize both
 * speed and correctness, and may challenge your understanding of floating-point math.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_DIGIT_UTILS_H
#define QENTEM_DIGIT_UTILS_H

#include "QCommon.hpp"

namespace Qentem {
namespace DigitUtils {

template <typename, SizeT32>
struct DigitString {};

// char
template <typename Char_T>
struct DigitString<Char_T, 1U> {
    static constexpr const Char_T *Infinity         = "inf";
    static constexpr const Char_T *NotANumber       = "nan";
    static constexpr SizeT         InfinityLength   = 3U;
    static constexpr SizeT         NotANumberLength = 3U;
    static constexpr const Char_T *Zeros            = "0000000000000000000";
    static constexpr SizeT32       ZerosLength      = 19U;
};

// char16_t
template <typename Char_T>
struct DigitString<Char_T, 2U> {
    static constexpr const Char_T *Infinity         = u"inf";
    static constexpr const Char_T *NotANumber       = u"nan";
    static constexpr SizeT         InfinityLength   = 3U;
    static constexpr SizeT         NotANumberLength = 3U;
    static constexpr const Char_T *Zeros            = u"0000000000000000000";
    static constexpr SizeT32       ZerosLength      = 19U;
};

// char32_t
template <typename Char_T>
struct DigitString<Char_T, 4U> {
    static constexpr const Char_T *Infinity         = U"inf";
    static constexpr const Char_T *NotANumber       = U"nan";
    static constexpr SizeT         InfinityLength   = 3U;
    static constexpr SizeT         NotANumberLength = 3U;
    static constexpr const Char_T *Zeros            = U"0000000000000000000";
    static constexpr SizeT32       ZerosLength      = 19U;
};

// wchar_t size = 4
template <>
struct DigitString<wchar_t, 4U> {
    static constexpr const wchar_t *Infinity         = L"inf";
    static constexpr const wchar_t *NotANumber       = L"nan";
    static constexpr SizeT          InfinityLength   = 3U;
    static constexpr SizeT          NotANumberLength = 3U;
    static constexpr const wchar_t *Zeros            = L"0000000000000000000";
    static constexpr SizeT32        ZerosLength      = 19U;
};

// wchar_t size = 2
template <>
struct DigitString<wchar_t, 2U> {
    static constexpr const wchar_t *Infinity         = L"inf";
    static constexpr const wchar_t *NotANumber       = L"nan";
    static constexpr SizeT          InfinityLength   = 3U;
    static constexpr SizeT          NotANumberLength = 3U;
    static constexpr const wchar_t *Zeros            = L"0000000000000000000";
    static constexpr SizeT32        ZerosLength      = 19U;
};

struct DigitChar {
    static constexpr char Zero     = '0';
    static constexpr char One      = '1';
    static constexpr char Five     = '5';
    static constexpr char Seven    = '7';
    static constexpr char Nine     = '9';
    static constexpr char E        = 'e';
    static constexpr char UE       = 'E';
    static constexpr char Dot      = '.';
    static constexpr char Positive = '+';
    static constexpr char Negative = '-';
    static constexpr char UA       = 'A';
    static constexpr char UF       = 'F';
    static constexpr char A        = 'a';
    static constexpr char F        = 'f';
    static constexpr char UW       = 'W';
    static constexpr char X        = 'x';
    static constexpr char UX       = 'X';
};
///////////////////////////////////////////////////
static const char DigitTable1[] = {"000102030405060708091011121314151617181920212223242526272829"
                                   "303132333435363738394041424344454647484950515253545556575859"
                                   "606162636465666768697071727374757677787980818283848586878889"
                                   "90919293949596979899"};
static const char DigitTable2[] = {"0123456789"};
///////////////////////////////////////////////////
template <SizeT32>
struct DigitConst {};

// uint16_t
template <>
struct DigitConst<2U> {
    static constexpr SizeT32 MaxShift           = 16U;
    static constexpr SizeT32 MaxPowerOfFive     = 6U;
    static constexpr SizeT32 MaxPowerOfTen      = 4U;
    static constexpr SizeT32 MaxPowerOfTenValue = 10000U;

    inline static constexpr SizeT16 PowerOfFive[] = {1U, 5U, 25U, 125U, 625U, 3125U, 15625U};

    inline static constexpr SizeT16 PowerOfOneOverFive[] = {

        // 2^N/5
        1U,     52429U, 41944U, 33555U,
        53688U, 42950U, 34360U

    };

    inline static constexpr SizeT16 PowerOfOneOverFiveShift[] = {

        // 2^N/5
        0U, 2U,  4U, 6U,
        9U, 11U, 13U

    };
};

// uint32_t
template <>
struct DigitConst<4U> {
    static constexpr SizeT32 MaxShift           = 32U;
    static constexpr SizeT32 MaxPowerOfFive     = 13U;
    static constexpr SizeT32 MaxPowerOfTen      = 9U;
    static constexpr SizeT32 MaxPowerOfTenValue = 1000000000U;

    inline static constexpr SizeT32 PowerOfFive[] = {
        // clang-format off
            1U,       5U,        25U,        125U,       625U,
            3125U,    15625U,    78125U,     390625U,    1953125U,
            9765625U, 48828125U, 244140625U, 1220703125U
        // clang-format on
    };

    inline static constexpr SizeT32 PowerOfOneOverFive[] = {
        // 2^N/5
        // clang-format off
            1U,          3435973837U, 2748779070U, 2199023256U, 3518437209U, 2814749768U, 2251799814U, 3602879702U,
            2882303762U, 2305843010U, 3689348815U, 2951479052U, 2361183242U, 3777893187U
        // clang-format on
    };

    inline static constexpr SizeT32 PowerOfOneOverFiveShift[] = {
        // 2^N/5
        // clang-format off
            0U,  2U,  4U,  6U,  9U,  11U, 13U, 16U,
            18U, 20U, 23U, 25U, 27U, 30U
        // clang-format on
    };
};

// uint64_t
template <>
struct DigitConst<8U> {
    static constexpr SizeT32 MaxShift           = 64U;
    static constexpr SizeT32 MaxPowerOfFive     = 27U;
    static constexpr SizeT32 MaxPowerOfTen      = 19U;
    static constexpr SizeT64 MaxPowerOfTenValue = 10000000000000000000ULL;

    inline static constexpr SizeT64 PowerOfFive[] = {
        // clang-format off
            1ULL,5ULL,25ULL,125ULL,625ULL,3125ULL,15625ULL,78125ULL,390625ULL,1953125ULL,
            9765625ULL,48828125ULL,244140625ULL,1220703125ULL,6103515625ULL,30517578125ULL,
            152587890625ULL,762939453125ULL,3814697265625ULL,19073486328125ULL,95367431640625ULL,
            476837158203125ULL,2384185791015625ULL,11920928955078125ULL,59604644775390625ULL,
            298023223876953125ULL,1490116119384765625ULL,7450580596923828125ULL
        // clang-format on
    };

    inline static constexpr SizeT64 PowerOfOneOverFive[] = {
        // 2^N/5
        // clang-format off
            1ULL,
            14757395258967641293ULL, 11805916207174113035ULL, 9444732965739290428ULL,
            15111572745182864684ULL, 12089258196146291748ULL, 9671406556917033398ULL,
            15474250491067253437ULL, 12379400392853802749ULL, 9903520314283042200ULL,
            15845632502852867519ULL, 12676506002282294015ULL, 10141204801825835212ULL,
            16225927682921336340ULL, 12980742146337069072ULL, 10384593717069655258ULL,
            16615349947311448412ULL, 13292279957849158730ULL, 10633823966279326984ULL,
            17014118346046923174ULL, 13611294676837538539ULL, 10889035741470030831ULL,
            17422457186352049330ULL, 13937965749081639464ULL, 11150372599265311571ULL,
            17840596158824498514ULL, 14272476927059598811ULL, 11417981541647679048ULL
        // clang-format on
    };

    inline static constexpr SizeT32 PowerOfOneOverFiveShift[] = {
        // 2^N/5
        // clang-format off
            0U,  2U,  4U,  6U,  9U,  11U, 13U, 16U, 18U, 20U,
            23U, 25U, 27U, 30U, 32U, 34U, 37U, 39U, 41U, 44U,
            46U, 48U, 51U, 53U, 55U, 58U, 60U, 62U
        // clang-format on
    };
};
///////////////////////////////////////////////////
template <typename, SizeT32>
struct RealNumberInfo {};

template <typename Number_T>
struct RealNumberInfo<Number_T, 1U> {
    // Dummy
    static constexpr SizeT32 Bias         = 1U;
    static constexpr SizeT32 ExponentSize = 1U;
    static constexpr SizeT32 MantissaSize = 1U;
    static constexpr SizeT16 SignMask     = 1U;
    static constexpr SizeT16 ExponentMask = 1U;
    static constexpr SizeT16 MantissaMask = 1U;
    static constexpr SizeT16 LeadingBit   = 1U;
    static constexpr SizeT32 MaxCut       = 1U;
};

#if defined(QENTEM_ENABLE_BFLOAT_16) && (QENTEM_ENABLE_BFLOAT_16 == 1)
// bfloat16_t
template <>
struct RealNumberInfo<BFloatT16, 2U> {
    static constexpr SizeT32 Bias         = 127U;
    static constexpr SizeT32 ExponentSize = 8U;
    static constexpr SizeT32 MantissaSize = 7U;
    static constexpr SizeT16 SignMask     = 0x8000U;
    static constexpr SizeT16 ExponentMask = 0x7F80U;
    static constexpr SizeT16 MantissaMask = 0x007F;
    static constexpr SizeT16 LeadingBit   = 0x80;
    static constexpr SizeT32 MaxCut       = 0U;
};
#endif

// float16_t, _Float16
template <typename Number_T>
struct RealNumberInfo<Number_T, 2U> {
    static constexpr SizeT32 Bias         = 15U;
    static constexpr SizeT32 ExponentSize = 5U;
    static constexpr SizeT32 MantissaSize = 10U;
    static constexpr SizeT16 SignMask     = 0x8000U;
    static constexpr SizeT16 ExponentMask = 0x7C00U;
    static constexpr SizeT16 MantissaMask = 0x3FFU;
    static constexpr SizeT16 LeadingBit   = 0x400U;
    static constexpr SizeT32 MaxCut       = 0U;
};

// float32
template <typename Number_T>
struct RealNumberInfo<Number_T, 4U> {
    static constexpr SizeT32 Bias         = 127U;
    static constexpr SizeT32 ExponentSize = 8U;
    static constexpr SizeT32 MantissaSize = 23U;
    static constexpr SizeT32 SignMask     = 0x80000000U;
    static constexpr SizeT32 ExponentMask = 0x7F800000U;
    static constexpr SizeT32 MantissaMask = 0x7FFFFFU;
    static constexpr SizeT32 LeadingBit   = 0x800000U;
    static constexpr SizeT32 MaxCut       = 30U;
};

// double
template <typename Number_T>
struct RealNumberInfo<Number_T, 8U> {
    static constexpr SizeT32 Bias         = 1023U;
    static constexpr SizeT32 ExponentSize = 11U;
    static constexpr SizeT32 MantissaSize = 52U;
    static constexpr SizeT64 SignMask     = 0x8000000000000000ULL;
    static constexpr SizeT64 ExponentMask = 0x7FF0000000000000ULL;
    static constexpr SizeT64 MantissaMask = 0xFFFFFFFFFFFFFULL;
    static constexpr SizeT64 LeadingBit   = 0x10000000000000ULL;
    static constexpr SizeT32 MaxCut       = 300U;
};

#if defined(QENTEM_ENABLE_FLOAT_128) && (QENTEM_ENABLE_FLOAT_128 == 1)
// float128_t, _Float128 (IEEE 754 binary128, Quad Precision)
template <typename Number_T>
struct RealNumberInfo<Number_T, 16U> {
    static constexpr SizeT32  Bias         = 16383U;
    static constexpr SizeT32  ExponentSize = 15U;
    static constexpr SizeT32  MantissaSize = 112U;
    static constexpr SizeT128 SignMask     = 0x80000000000000000000000000000000ULL;
    static constexpr SizeT128 ExponentMask = 0x7FFF0000000000000000000000000000ULL;
    static constexpr SizeT128 MantissaMask = 0x0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFULL;
    static constexpr SizeT128 LeadingBit   = 0x00010000000000000000000000000000ULL;
    static constexpr SizeT32  MaxCut       = 300U;
};
#endif

} // namespace DigitUtils
} // namespace Qentem

#endif
