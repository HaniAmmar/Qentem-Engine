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

#ifndef QENTEM_DIGIT_UTILS_H_
#define QENTEM_DIGIT_UTILS_H_

namespace Qentem::DigitUtils {

template <typename, SizeT32>
struct DigitStrings {};

// char
template <typename Char_T_>
struct DigitStrings<Char_T_, 1U> {
    static constexpr const Char_T_ *InfinityString = "inf";
    static constexpr const Char_T_ *NANString      = "nan";
    static constexpr const Char_T_ *ZeroesString   = "0000000000000000000";
};

// char16_t
template <typename Char_T_>
struct DigitStrings<Char_T_, 2U> {
    static constexpr const Char_T_ *InfinityString = u"inf";
    static constexpr const Char_T_ *NANString      = u"nan";
    static constexpr const Char_T_ *ZeroesString   = u"0000000000000000000";
};

// char32_t
template <typename Char_T_>
struct DigitStrings<Char_T_, 4U> {
    static constexpr const Char_T_ *InfinityString = U"inf";
    static constexpr const Char_T_ *NANString      = U"nan";
    static constexpr const Char_T_ *ZeroesString   = U"0000000000000000000";
};

// wchar_t size = 4
template <>
struct DigitStrings<wchar_t, 4U> {
    static constexpr const wchar_t *InfinityString = L"inf";
    static constexpr const wchar_t *NANString      = L"nan";
    static constexpr const wchar_t *ZeroesString   = L"0000000000000000000";
};

// wchar_t size = 2
template <>
struct DigitStrings<wchar_t, 2U> {
    static constexpr const wchar_t *InfinityString = L"inf";
    static constexpr const wchar_t *NANString      = L"nan";
    static constexpr const wchar_t *ZeroesString   = L"0000000000000000000";
};

struct DigitChars {
    static constexpr char ZeroChar     = '0';
    static constexpr char OneChar      = '1';
    static constexpr char FiveChar     = '5';
    static constexpr char SevenChar    = '7';
    static constexpr char NineChar     = '9';
    static constexpr char E_Char       = 'e';
    static constexpr char UE_Char      = 'E';
    static constexpr char DotChar      = '.';
    static constexpr char PositiveChar = '+';
    static constexpr char NegativeChar = '-';
    static constexpr char UA_Char      = 'A';
    static constexpr char UF_Char      = 'F';
    static constexpr char A_Char       = 'a';
    static constexpr char F_Char       = 'f';
    static constexpr char UW_Char      = 'W';
};
///////////////////////////////////////////////////
static const char DigitTable1[] = {"000102030405060708091011121314151617181920212223242526272829"
                                   "303132333435363738394041424344454647484950515253545556575859"
                                   "606162636465666768697071727374757677787980818283848586878889"
                                   "90919293949596979899"};
static const char DigitTable2[] = {"0123456789"};
///////////////////////////////////////////////////
template <SizeT32>
struct DigitLimit {};

// uint16_t
template <>
struct DigitLimit<2U> {
    static constexpr SizeT32 MaxPowerOfFiveShift = 16U;
    static constexpr SizeT32 MaxPowerOfFive      = 6U;
    static constexpr SizeT32 MaxPowerOfTenDigits = 4U;

    static constexpr unsigned short PowerOfFive[] = {1U, 5U, 25U, 125U, 625U, 3125U, 15625U};
    static constexpr unsigned short PowerOfTen[]  = {1U, 10U, 100U, 1000U, 10000U};
};

// uint32_t
template <>
struct DigitLimit<4U> {
    static constexpr SizeT32 MaxPowerOfFiveShift = 32U;
    static constexpr SizeT32 MaxPowerOfFive      = 13U;
    static constexpr SizeT32 MaxPowerOfTenDigits = 9U;

    static constexpr SizeT32 PowerOfFive[] = {1U,     5U,      25U,      125U,     625U,      3125U,      15625U,
                                              78125U, 390625U, 1953125U, 9765625U, 48828125U, 244140625U, 1220703125U};

    static constexpr SizeT32 PowerOfTen[] = {1U,      10U,      100U,      1000U,      10000U,
                                             100000U, 1000000U, 10000000U, 100000000U, 1000000000U};
};

// uint64_t
template <>
struct DigitLimit<8U> {
    static constexpr SizeT32 MaxPowerOfFiveShift = 64U;
    static constexpr SizeT32 MaxPowerOfFive      = 27U;
    static constexpr SizeT32 MaxPowerOfTenDigits = 19U;

    static constexpr SizeT64 PowerOfFive[] = {
        // clang-format off
        1ULL,5ULL,25ULL,125ULL,625ULL,3125ULL,15625ULL,78125ULL,390625ULL,1953125ULL,
        9765625ULL,48828125ULL,244140625ULL,1220703125ULL,6103515625ULL,30517578125ULL,
        152587890625ULL,762939453125ULL,3814697265625ULL,19073486328125ULL,95367431640625ULL,
        476837158203125ULL,2384185791015625ULL,11920928955078125ULL,59604644775390625ULL,
        298023223876953125ULL,1490116119384765625ULL,7450580596923828125ULL
        // clang-format on
    };

    static constexpr SizeT64 PowerOfTen[] = {
        // clang-format off
        1ULL,
        10ULL,
        100ULL,
        1000ULL,
        10000ULL,
        100000ULL,
        1000000ULL,
        10000000ULL,
        100000000ULL,
        1000000000ULL,
        10000000000ULL,
        100000000000ULL,
        1000000000000ULL,
        10000000000000ULL,
        100000000000000ULL,
        1000000000000000ULL,
        10000000000000000ULL,
        100000000000000000ULL,
        1000000000000000000ULL,
        10000000000000000000ULL
        // clang-format on
    };
};
///////////////////////////////////////////////////
template <typename, SizeT32>
struct RealNumberInfo {};

// double
template <>
struct RealNumberInfo<double, 8U> {
    static constexpr SizeT32 Bias         = 1023U;
    static constexpr SizeT32 ExponentSize = 11U;
    static constexpr SizeT32 MantissaSize = 52U;
    static constexpr SizeT64 SignMask     = 0x8000000000000000ULL;
    static constexpr SizeT64 ExponentMask = 0x7FF0000000000000ULL;
    static constexpr SizeT64 MantissaMask = 0xFFFFFFFFFFFFFULL;
    static constexpr SizeT64 LeadingBit   = 0x10000000000000ULL;
    static constexpr SizeT32 MaxCut       = 300U;
};

// float32
template <>
struct RealNumberInfo<float, 4U> {
    static constexpr SizeT32 Bias         = 127U;
    static constexpr SizeT32 ExponentSize = 8U;
    static constexpr SizeT32 MantissaSize = 23U;
    static constexpr SizeT32 SignMask     = 0x80000000U;
    static constexpr SizeT32 ExponentMask = 0x7F800000U;
    static constexpr SizeT32 MantissaMask = 0x7FFFFFU;
    static constexpr SizeT32 LeadingBit   = 0x800000U;
    static constexpr SizeT32 MaxCut       = 30U;
};

#if defined(QENTEM_ENABLE_FLOAT_16) && (QENTEM_ENABLE_FLOAT_16 == 1)
// float16_t, _Float16
template <>
struct RealNumberInfo<float16_t, 2U> {
    static constexpr SizeT32        Bias         = 15U;
    static constexpr SizeT32        ExponentSize = 5U;
    static constexpr SizeT32        MantissaSize = 10U;
    static constexpr unsigned short SignMask     = 0x8000U;
    static constexpr unsigned short ExponentMask = 0x7C00U;
    static constexpr unsigned short MantissaMask = 0x3FFU;
    static constexpr unsigned short LeadingBit   = 0x400U;
    static constexpr SizeT32        MaxCut       = 0U;
};
#endif

#if defined(QENTEM_ENABLE_BFLOAT_16) && (QENTEM_ENABLE_BFLOAT_16 == 1)
// // bfloat16_t
template <>
struct RealNumberInfo<bfloat16_t, 2U> {
    static constexpr SizeT32        Bias         = 127U;
    static constexpr SizeT32        ExponentSize = 8U;
    static constexpr SizeT32        MantissaSize = 7U;
    static constexpr unsigned short SignMask     = 0x8000U;
    static constexpr unsigned short ExponentMask = 0x7F80U;
    static constexpr unsigned short MantissaMask = 0x7FU;
    static constexpr unsigned short LeadingBit   = 0x80U;
    static constexpr SizeT32        MaxCut       = 0U;
};
#endif

} // namespace Qentem::DigitUtils

#endif
