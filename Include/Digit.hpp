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

#include "BigInt.hpp"
#include "DigitUtils.hpp"
#include "StringUtils.hpp"

#ifndef _QENTEM_DIGIT_H
#define _QENTEM_DIGIT_H

/*
 * For converting numbers from and to strings.
 */
namespace Qentem {
struct Digit {
    enum class RealFormatType : SizeT8 { Default = 0, Fixed = 1, Scientific = 2 };

    struct RealFormatInfo {
        RealFormatInfo() noexcept = default;

        RealFormatInfo(SizeT32 precision) noexcept : Precision{precision} {
        }

        RealFormatInfo(RealFormatType type) noexcept : Type{type} {
        }

        RealFormatInfo(SizeT32 precision, RealFormatType type) noexcept : Precision{precision}, Type{type} {
        }

        RealFormatInfo &operator=(SizeT32 precision) noexcept {
            Precision = precision;
            return *this;
        }

        RealFormatInfo &operator=(RealFormatType type) noexcept {
            Type = type;
            return *this;
        }

        SizeT32        Precision{6U};
        RealFormatType Type{RealFormatType::Default};
    };
    /////////////////////////////////////////////////////////////////
    template <bool _Reverse_V = false, typename _Stream_T, typename _Number_T>
    inline static void NumberToString(_Stream_T &stream, _Number_T number) {
        constexpr SizeT32 n_size = sizeof(_Number_T);
        using _Char_T            = typename _Stream_T::CharType;
        using QNumberType_T      = typename QNumberAutoTypeT<_Number_T, n_size>::QNumberType_T;

        constexpr SizeT32 max_number_of_digits = (((n_size * 8U * 30103U) / 100000U) + 1U);
        _Char_T           storage[max_number_of_digits];

        QNumberType_T qn{number};

        if (!IsUnsigned<_Number_T>()) {
            if (number < _Number_T{0}) {
                qn.Integer = -qn.Integer;
                stream += DigitUtils::DigitChars::NegativeChar;
            }
        }

        if (_Reverse_V) {
            stream.Write(&(storage[0U]), intToString<true>(&(storage[0U]), qn.Natural));
        } else {
            const SizeT offset = intToString(&(storage[max_number_of_digits]), qn.Natural);
            stream.Write(&(storage[max_number_of_digits - offset]), offset);
        }
    }

    template <typename _Stream_T, typename _Number_T>
    inline static void NumberToString(_Stream_T &stream, _Number_T number, RealFormatInfo format) {
        constexpr SizeT32 n_size = sizeof(_Number_T);
        using QNumberType_T      = typename QNumberAutoTypeT<_Number_T, n_size>::QNumberType_T;

        realToString<_Number_T>(stream, QNumberType_T{number}.Natural, format);
    }
    /////////////////////////////////////////////////////////////////
    template <typename _Char_T>
    static SizeT32 HexStringToNumber(const _Char_T *value, const SizeT length) noexcept {
        using Uint_Char_T = SizeT32;

        SizeT32 number{0};
        SizeT32 offset{0};

        while (offset < length) {
            const _Char_T digit = value[offset];
            number <<= 4U;

            if ((digit >= DigitUtils::DigitChars::OneChar) && (digit <= DigitUtils::DigitChars::NineChar)) {
                // No need for 0
                number |= Uint_Char_T(digit - DigitUtils::DigitChars::ZeroChar); // 1-9
            } else if ((digit >= DigitUtils::DigitChars::UA_Char) &&
                       (digit <= DigitUtils::DigitChars::UF_Char)) { // A-F
                number |= Uint_Char_T(digit - DigitUtils::DigitChars::SevenChar);
            } else if ((digit >= DigitUtils::DigitChars::A_Char) && (digit <= DigitUtils::DigitChars::F_Char)) { // a-f
                number |= Uint_Char_T(digit - DigitUtils::DigitChars::UW_Char);
            }

            ++offset;
        }

        return number;
    }
    /////////////////////////////////////////////////////////////////
    template <typename _Number_T, typename _Char_T>
    static void FastStringToNumber(_Number_T &number, const _Char_T *content, SizeT length) noexcept {
        SizeT offset = 0;
        number       = 0;

        if (offset < length) {
            number += _Number_T(content[offset]);
            number -= _Number_T{DigitUtils::DigitChars::ZeroChar};
            ++offset;

            while (offset < length) {
                number *= _Number_T{10};
                number += _Number_T(content[offset]);
                number -= _Number_T{DigitUtils::DigitChars::ZeroChar};
                ++offset;
            }
        }
    }

    template <typename _Char_T>
    inline static QNumberType StringToNumber(QNumber64 &number, const _Char_T *content, SizeT &offset,
                                             SizeT end_offset) noexcept {
        return stringToNumber(number, content, offset, end_offset);
    }

    template <typename _Char_T>
    inline static QNumberType StringToNumber(QNumber64 &number, const _Char_T *content, SizeT end_offset) noexcept {
        SizeT offset{0};
        return stringToNumber(number, content, offset, end_offset);
    }

    //////////// Private ////////////
  private:
    template <typename _Char_T>
    static QNumberType stringToNumber(QNumber64 &number, const _Char_T *content, SizeT &offset,
                                      SizeT end_offset) noexcept {
        using Number_T = decltype(number.Natural);

        constexpr SizeT max_length = SizeT{19};
        number.Natural             = 0;

        if (offset < end_offset) {
            SizeT   dot_offset   = 0;
            SizeT   exp_offset   = 0;
            SizeT   start_offset = 0;
            SizeT   tmp_offset;
            _Char_T digit         = content[offset];
            bool    is_negative   = false;
            bool    is_real       = false;
            bool    has_dot       = false;
            bool    fraction_only = false;
            ///////////////////////////////////////////////////////////
            if (digit == DigitUtils::DigitChars::NegativeChar) {
                ++offset;
                is_negative = true;
            } else if (digit == DigitUtils::DigitChars::PositiveChar) {
                ++offset;
            }

            tmp_offset = offset;
            ///////////////////////////////////////////////////////////
            if (offset < end_offset) {
                digit = content[offset];

                if ((digit > DigitUtils::DigitChars::ZeroChar) && (digit <= DigitUtils::DigitChars::NineChar)) {
                    // 123456789
                    number.Natural += Number_T(digit);
                    number.Natural -= Number_T{DigitUtils::DigitChars::ZeroChar};
                    tmp_offset   = (((end_offset - offset) < max_length) ? end_offset : (offset + max_length));
                    start_offset = offset;
                    ++offset;
                } else if ((digit == DigitUtils::DigitChars::ZeroChar) || (digit == DigitUtils::DigitChars::DotChar)) {
                    if ((digit == DigitUtils::DigitChars::ZeroChar) && (offset + SizeT{1}) < end_offset) {
                        ++offset;
                        digit = content[offset];

                        if ((digit >= DigitUtils::DigitChars::ZeroChar) &&
                            (digit <= DigitUtils::DigitChars::NineChar)) {
                            // 0000xxxx
                            return QNumberType::NotANumber; // Leading zero.
                        }
                    }

                    if (digit == DigitUtils::DigitChars::DotChar) {
                        // 0.xxxxxxxx
                        dot_offset = offset;
                        ++offset;
                        start_offset  = offset;
                        is_real       = true;
                        has_dot       = true;
                        fraction_only = true;

                        while (offset < end_offset) {
                            // 0.000000000x
                            digit = content[offset];

                            if (digit == DigitUtils::DigitChars::ZeroChar) {
                                ++offset;
                                continue;
                            }

                            break;
                        }
                        ///////////////////////////////////////////////////////////
                        if ((start_offset == offset) && (dot_offset == tmp_offset) &&
                            ((digit < DigitUtils::DigitChars::ZeroChar) ||
                             (digit > DigitUtils::DigitChars::NineChar))) {
                            return QNumberType::NotANumber; // Just a dot.
                        }
                        ///////////////////////////////////////////////////////////
                        start_offset = offset;
                    }

                    tmp_offset = (((end_offset - offset) < max_length) ? end_offset : (offset + max_length));
                } else {
                    return QNumberType::NotANumber;
                }
                ///////////////////////////////////////////////////////////
                const SizeT max_end_offset = tmp_offset;
                ///////////////////////////////////////////////////////////
                while (offset < end_offset) {
                    while (offset < max_end_offset) {
                        digit = content[offset];

                        if ((digit >= DigitUtils::DigitChars::ZeroChar) &&
                            (digit <= DigitUtils::DigitChars::NineChar)) {
                            number.Natural *= Number_T{10};
                            number.Natural += Number_T(digit);
                            number.Natural -= Number_T{DigitUtils::DigitChars::ZeroChar};
                            ++offset;
                            continue;
                        }

                        break;
                    }

                    if (digit == DigitUtils::DigitChars::DotChar) {
                        if (!has_dot) {
                            dot_offset = offset;
                            ++offset;
                            is_real = true;
                            has_dot = true;

                            if (offset < max_end_offset) {
                                digit = content[offset];

                                if ((digit > DigitUtils::DigitChars::ZeroChar) &&
                                    (digit <= DigitUtils::DigitChars::NineChar)) {
                                    continue;
                                }

                                if ((digit == DigitUtils::DigitChars::ZeroChar) && (offset + 1U) < max_end_offset) {
                                    digit = content[offset + SizeT{1}];

                                    if ((digit >= DigitUtils::DigitChars::ZeroChar) &&
                                        (digit <= DigitUtils::DigitChars::NineChar)) {
                                        continue;
                                    }
                                }
                            }

                            // Just zero at the end.
                            break;
                        }

                        return QNumberType::NotANumber; // x.x.x..x
                    }

                    break;
                }
                ///////////////////////////////////////////////////////////
                tmp_offset = offset;
                ///////////////////////////////////////////////////////////
                if (!is_real && (offset < end_offset)) {
                    digit = content[offset];

                    switch (digit) {
                        case DigitUtils::DigitChars::DotChar:
                        case DigitUtils::DigitChars::E_Char:
                        case DigitUtils::DigitChars::UE_Char: {
                            is_real = true;
                            break;
                        }

                        default: {
                            if ((digit >= DigitUtils::DigitChars::ZeroChar) &&
                                (digit <= DigitUtils::DigitChars::NineChar)) {
                                if ((number.Natural > 0x1999999999999999ULL) ||
                                    ((number.Natural == 0x1999999999999999ULL) &&
                                     (digit > DigitUtils::DigitChars::FiveChar))) {
                                    is_real = true;
                                    break;
                                }

                                number.Natural *= Number_T{10};
                                number.Natural += Number_T(digit);
                                number.Natural -= Number_T{DigitUtils::DigitChars::ZeroChar};
                                ++offset;
                                ++tmp_offset;

                                if (offset < end_offset) {
                                    digit = content[offset];

                                    switch (digit) {
                                        case DigitUtils::DigitChars::DotChar:
                                        case DigitUtils::DigitChars::E_Char:
                                        case DigitUtils::DigitChars::UE_Char: {
                                            is_real = true;
                                            break;
                                        }

                                        default: {
                                            if ((digit >= DigitUtils::DigitChars::ZeroChar) &&
                                                (digit <= DigitUtils::DigitChars::NineChar)) {
                                                is_real = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                ///////////////////////////////////////////////////////////
                if (!is_real) {
                    if (!is_negative) {
                        return QNumberType::Natural;
                    }

                    if (number.Natural == Number_T{0}) {
                        number.Natural |= 0x8000000000000000ULL;
                        return QNumberType::Real;
                    }

                    if (number.Natural <= 0x7FFFFFFFFFFFFFFFULL) {
                        number.Integer = -number.Integer;
                        return QNumberType::Integer;
                    }
                }
                ///////////////////////////////////////////////////////////
                if (number.Natural != Number_T{0}) {
                    const SizeT32 e_p10_power =
                        (SizeT32(tmp_offset - start_offset) - SizeT32(!fraction_only && has_dot));

                    const SizeT32 e_n10_power =
                        (fraction_only ? (e_p10_power + (SizeT32(start_offset - dot_offset) - SizeT32{1}))
                                       : (has_dot ? (SizeT32(offset - dot_offset) - SizeT32{1}) : SizeT32{0}));

                    // const SizeT32 e_n10_power =
                    //     (fraction_only
                    //          ? ((tmp_offset - start_offset) + (SizeT32(start_offset - dot_offset) - 1U))
                    //          : (has_dot ? (SizeT32(offset - dot_offset) - 1U) : 0U));

                    SizeT32 exponent        = 0;
                    bool    is_negative_exp = false;
                    bool    keep_going      = (offset < end_offset);
                    ///////////////////////////////////////////////////////////
                    tmp_offset   = dot_offset;
                    start_offset = offset;
                    ///////////////////////////////////////////////////////////
                    while (keep_going) {
                        digit = content[offset];

                        if ((digit >= DigitUtils::DigitChars::ZeroChar) &&
                            (digit <= DigitUtils::DigitChars::NineChar)) {
                            ++offset;
                            keep_going = (offset < end_offset);
                            continue;
                        }

                        switch (content[offset]) {
                            case DigitUtils::DigitChars::DotChar: {
                                if (!has_dot) {
                                    dot_offset = offset;
                                    ++offset;
                                    has_dot    = true;
                                    keep_going = (offset < end_offset);
                                    continue;
                                }

                                return QNumberType::NotANumber;
                            }

                            case DigitUtils::DigitChars::E_Char:
                            case DigitUtils::DigitChars::UE_Char: {
                                exp_offset = offset;
                                ++offset;

                                if (parseExponent(content, exponent, is_negative_exp, offset, end_offset)) {
                                    keep_going = false;
                                    continue;
                                }

                                return QNumberType::NotANumber;
                            }

                            default:
                                keep_going = false;
                                break;
                        }
                    }
                    ///////////////////////////////////////
                    SizeT32 e_extra_p10_power = 0;

                    if (!fraction_only && (start_offset != offset)) {
                        if (!has_dot) {
                            e_extra_p10_power = SizeT32((exp_offset == SizeT32{0}) ? (offset - start_offset)
                                                                                   : (exp_offset - start_offset));
                        } else if (dot_offset != tmp_offset) {
                            e_extra_p10_power = SizeT32(dot_offset - start_offset);
                        }

                        if (!is_negative_exp) {
                            exponent += e_extra_p10_power;
                        } else if (exponent <= e_extra_p10_power) {
                            exponent        = (e_extra_p10_power - exponent);
                            is_negative_exp = false;
                        } else {
                            exponent -= e_extra_p10_power;
                        }
                    }
                    ///////////////////////////////////////
                    if (is_negative_exp) {
                        exponent += e_n10_power;

                    } else if (exponent >= e_n10_power) {
                        exponent -= e_n10_power;
                    } else {
                        exponent        = (e_n10_power - exponent);
                        is_negative_exp = true;
                    }

                    if ((is_negative_exp && (exponent > e_p10_power) && ((exponent - e_p10_power) > SizeT32{324})) ||
                        (!is_negative_exp && ((exponent + e_p10_power) > SizeT32{309}))) {
                        return QNumberType::NotANumber;
                    }

                    if (is_negative_exp) {
                        powerOfNegativeTen(number.Natural, exponent);
                    } else {
                        powerOfPositiveTen(number.Natural, exponent);
                    }
                }
                ///////////////////////////////////////
                if (is_negative) {
                    number.Natural |= 0x8000000000000000ULL;
                }

                return QNumberType::Real;
            }
        }

        return QNumberType::NotANumber;
    }
    /////////////////////////////////////////
    template <typename _Number_T>
    static void powerOfNegativeTen(_Number_T &number, SizeT32 exponent) noexcept {
        using UNumber_T  = SizeT64;
        using DigitLimit = DigitUtils::DigitLimit<sizeof(UNumber_T)>;
        //////////////////////////////////////////////////////////////
        BigInt<UNumber_T, 192U> bint{number};
        //////////////////////////////////////////////////////////////
        SizeT32 shifted{exponent};
        //////////////////////////////////////////////////////////////
        shifted += 64U;
        bint <<= 64U;
        //////////////////////////////////////////////////////////////
        constexpr bool is_size_8 = (sizeof(UNumber_T) == 8U);
        if (is_size_8) {
            while (exponent >= DigitLimit::MaxPowerOfFive) {
                // 2**126 = 85070591730234615865843651857942052864
                // 5**27 = 7450580596923828125 (MaxPowerOfFive)
                // 2**126 / 5**27 = 11417981541647679048.4
                // 126-64=62; See 2**126 and 64 shift

                bint *= DigitLimit::GetPowerOfOneOverFive(DigitLimit::MaxPowerOfFive);
                bint >>= DigitLimit::MaxShift;
                shifted += DigitLimit::GetPowerOfOneOverFiveShift(DigitLimit::MaxPowerOfFive);
                exponent -= DigitLimit::MaxPowerOfFive;
            }

            if (exponent != SizeT32{0}) {
                bint *= DigitLimit::GetPowerOfOneOverFive(exponent);
                bint >>= DigitLimit::MaxShift;
                shifted += DigitLimit::GetPowerOfOneOverFiveShift(exponent);
            }
        } else {
            while (exponent >= DigitLimit::MaxPowerOfFive) {
                bint <<= DigitLimit::MaxShift;
                bint /= DigitLimit::GetPowerOfFive(DigitLimit::MaxPowerOfFive);
                shifted += DigitLimit::MaxShift;
                exponent -= DigitLimit::MaxPowerOfFive;
            }

            if (exponent != SizeT32{0}) {
                bint <<= DigitLimit::MaxShift;
                bint /= DigitLimit::GetPowerOfFive(exponent);
                shifted += DigitLimit::MaxShift;
            }
        }
        //////////////////////////////////////////////////////////////
        SizeT32       exp = DigitUtils::RealNumberInfo<double, 8U>::Bias; // double only
        const SizeT32 bit = bint.FindLastBit();

        // if (bit <= 52U) {
        //     number = SizeT64(bint);
        //     number <<= (53U - bit);
        // } else {
        bint >>= (bit - SizeT32{53});
        number = SizeT64(bint);
        // }
        //////////////////////////////////////////////////////////////
        if (shifted <= bit) {
            shifted = (bit - shifted);
            exp += shifted;

            number += (number & _Number_T{1});
            number >>= 1U;
            exp += (number > _Number_T{0x1FFFFFFFFFFFFF});
        } else {
            shifted -= bit;

            if (exp > shifted) {
                exp -= shifted;
                number += (number & _Number_T{1});
                number >>= 1U;
                exp += (number > _Number_T{0x1FFFFFFFFFFFFF});
            } else {
                shifted -= exp;
                ++shifted;
                number >>= shifted;
                number += (number & _Number_T{1});
                number >>= 1U;
                exp = (number > _Number_T{0xFFFFFFFFFFFFF});
            }
        }

        number &= 0xFFFFFFFFFFFFFULL;
        number |= (SizeT64(exp) << 52U);
    }
    /////////////////////////////////////////
    template <typename _Number_T>
    static void powerOfPositiveTen(_Number_T &number, SizeT32 exponent) noexcept {
        using UNumber_T  = SystemIntType;
        using DigitLimit = DigitUtils::DigitLimit<sizeof(UNumber_T)>;
        //////////////////////////////////////////////////////////////
        BigInt<UNumber_T, 192U> bint{number};
        //////////////////////////////////////////////////////////////
        constexpr SizeT32 bint_limit{2U};
        SizeT32           shifted{exponent};
        //////////////////////////////////////////////////////////////
        while (exponent >= DigitLimit::MaxPowerOfFive) {
            bint *= DigitLimit::GetPowerOfFive(DigitLimit::MaxPowerOfFive);

            if (bint.Index() > bint_limit) {
                bint >>= DigitLimit::MaxShift;
                shifted += DigitLimit::MaxShift;
            }

            exponent -= DigitLimit::MaxPowerOfFive;
        }

        if (exponent != SizeT32{0}) {
            bint *= DigitLimit::GetPowerOfFive(exponent);
        }
        //////////////////////////////////////////////////////////////
        const SizeT32 bit = bint.FindLastBit();

        if (bit <= 52U) {
            number = SizeT64(bint);
            number <<= (52U - bit);
        } else {
            bint >>= (bit - 53U);
            number = SizeT64(bint);
            number += (number & _Number_T{1});
            number >>= 1U;
            shifted += SizeT32(number > _Number_T{0x1FFFFFFFFFFFFF});
        }
        //////////////////////////////////////////////////////////////
        SizeT64 exp = DigitUtils::RealNumberInfo<double, 8U>::Bias; // double only
        exp += bit;
        exp += shifted;
        exp <<= 52U;
        number &= 0xFFFFFFFFFFFFFULL;
        number |= exp;
    }
    /////////////////////////////////////////
    template <typename _Char_T>
    static bool parseExponent(const _Char_T *content, SizeT32 &exponent, bool &is_negative_exp, SizeT &offset,
                              const SizeT end_offset) noexcept {
        bool sign_set{false};

        while (offset < end_offset) {
            switch (content[offset]) {
                case DigitUtils::DigitChars::PositiveChar: {
                    if (!sign_set) {
                        ++offset;
                        sign_set = true;
                        break;
                    }

                    return false;
                }
                case DigitUtils::DigitChars::NegativeChar: {
                    if (!sign_set) {
                        ++offset;
                        is_negative_exp = true;
                        sign_set        = true;
                        break;
                    }

                    return false;
                }

                default: {
                    const SizeT o_offset = offset;

                    while (offset < end_offset) {
                        const _Char_T digit = content[offset];

                        if ((digit >= DigitUtils::DigitChars::ZeroChar) &&
                            (digit <= DigitUtils::DigitChars::NineChar)) {
                            exponent *= SizeT32{10};
                            exponent += SizeT32(digit - DigitUtils::DigitChars::ZeroChar);
                            ++offset;
                            continue;
                        }

                        break;
                    }

                    return (o_offset != offset);
                }
            }
        }

        return false;
    }
    /////////////////////////////////////////
    template <bool _Reverse_V = false, typename _Char_T, typename _Number_T>
    static SizeT intToString(_Char_T *storage, _Number_T number) noexcept {
        const _Char_T *str = storage;

        if (!_Reverse_V) {
            while (number >= _Number_T{10}) {
                const SizeT index = (SizeT(number % _Number_T{100}) * SizeT{2});
                number /= _Number_T{100};

                --storage;
                *storage = _Char_T(DigitUtils::DigitTable1[index + SizeT{1}]);
                --storage;
                *storage = _Char_T(DigitUtils::DigitTable1[index]);
            }

            if ((number != _Number_T{0}) || (str == storage)) {
                --storage;
                *storage = _Char_T(DigitUtils::DigitTable2[number]);
            }

            return SizeT(str - storage);
        } else {
            while (number >= _Number_T{10}) {
                const SizeT index = (SizeT(number % _Number_T{100}) * SizeT{2});
                number /= _Number_T{100};

                *storage = _Char_T(DigitUtils::DigitTable1[index + SizeT{1}]);
                ++storage;
                *storage = _Char_T(DigitUtils::DigitTable1[index]);
                ++storage;
            }

            if ((number != _Number_T{0}) || (str == storage)) {
                *storage = _Char_T(DigitUtils::DigitTable2[number]);
                ++storage;
            }

            return SizeT(storage - str);
        }
    }
    /////////////////////////////////////////
    template <typename _Float_T, typename _Stream_T, typename _Number_T>
    static void realToString(_Stream_T &stream, const _Number_T number, const RealFormatInfo format) {
        using _Char_T = typename _Stream_T::CharType;
        using Info_T  = DigitUtils::RealNumberInfo<_Float_T, sizeof(number)>;

        const _Number_T bias = (number & Info_T::ExponentMask);

        if (bias != Info_T::ExponentMask) {
            if ((number & Info_T::SignMask) != _Number_T{0}) {
                stream += DigitUtils::DigitChars::NegativeChar;
            }

            _Number_T mantissa = (number & Info_T::MantissaMask);

            if ((mantissa != _Number_T{0}) || (bias != _Number_T{0})) {
                if (bias != _Number_T{0}) {
                    mantissa |= Info_T::LeadingBit;
                } else {
                    mantissa <<= 1U;
                }

                BigInt<SystemIntType, ((Info_T::Bias + 1U) + (sizeof(_Number_T) * 8U * 3U))> bint{mantissa};
                using DigitLimit = DigitUtils::DigitLimit<bint.SizeOfType()>;
                /////////////////////////////////////
                const SizeT32 first_shift      = Platform::FindFirstBit(mantissa);
                const int     exponent         = (int)((bias >> Info_T::MantissaSize) - Info_T::Bias);
                const bool    is_positive_exp  = (exponent >= 0);
                const SizeT32 positive_exp     = SizeT32(is_positive_exp ? exponent : -exponent);
                const SizeT32 first_bit        = (Info_T::MantissaSize - first_shift);
                const SizeT32 exp_actual_value = (positive_exp + ((bias == _Number_T{0}) * first_bit));
                const SizeT32 digits           = (((exp_actual_value * 30103U) / 100000U) + 1U);
                SizeT32       fraction_length  = 0;
                const bool    extra_digits     = (digits > format.Precision);
                bool          round_up         = false;
                const bool    big_offset       = (positive_exp >= first_bit);
                const bool    no_fraction      = (is_positive_exp && (big_offset || extra_digits));
                /////////////////////////////////////
                if (no_fraction) {
                    const SizeT32 drop    = ((!extra_digits) ? SizeT32{0} : (digits - (format.Precision + SizeT32{1})));
                    const SizeT32 m_shift = (Info_T::MantissaSize + drop);

                    if (m_shift < positive_exp) {
                        bint <<= (positive_exp - m_shift);
                    } else {
                        bint >>= (m_shift - positive_exp);
                    }

                    if (drop != 0U) {
                        round_up = true;
                        bigIntDropDigits(bint, drop);
                    }
                } else {
                    SizeT32 shift   = 0U;
                    SizeT32 needed  = 0U;
                    fraction_length = first_bit;

                    if (is_positive_exp) {
                        fraction_length -= positive_exp;
                        needed = (format.Precision - digits);
                    } else {
                        fraction_length += positive_exp;
                        needed = (digits + format.Precision);
                    }

                    ++needed; // For rounding.

                    if (fraction_length > needed) {
                        shift           = (fraction_length - needed);
                        fraction_length = needed;
                        round_up        = true;
                    }

                    bint >>= first_shift;

                    SizeT32 times = fraction_length;

                    if (times >= DigitLimit::MaxPowerOfFive) {
                        const SizeT32 max_index = (format.Precision < Info_T::MaxCut)
                                                      ? ((format.Precision / DigitLimit::MaxPowerOfTen) + SizeT32{2})
                                                      : bint.MaxIndex();

                        do {
                            bint *= DigitLimit::GetPowerOfFive(DigitLimit::MaxPowerOfFive);

                            if ((bint.Index() >= max_index) && (shift >= DigitLimit::MaxShift)) {
                                bint >>= DigitLimit::MaxShift;
                                shift -= DigitLimit::MaxShift;
                            }

                            times -= DigitLimit::MaxPowerOfFive;
                        } while (times >= DigitLimit::MaxPowerOfFive);
                    }

                    if (times != SizeT32{0}) {
                        bint *= DigitLimit::GetPowerOfFive(times);
                    }

                    bint >>= shift;
                }

                const SizeT start_at = stream.Length();
                bigIntToString(stream, bint);

                // Only 'Default' is implemented
                formatStringNumber(stream, start_at, format.Precision, digits, fraction_length, is_positive_exp,
                                   round_up);
            } else {
                stream += DigitUtils::DigitChars::ZeroChar;
            }
        } else {
            constexpr SizeT32 size = sizeof(_Char_T);

            if ((number & Info_T::MantissaMask) == _Number_T{0}) {
                if ((number & Info_T::SignMask) != _Number_T{0}) {
                    stream += DigitUtils::DigitChars::NegativeChar;
                }

                // stream.Write(DigitUtils::DigitStrings<_Char_T, size>::InfinityString,
                //              StringUtils::ConstCount(DigitUtils::DigitStrings<_Char_T, size>::InfinityString));

                stream.Write(DigitUtils::DigitStrings<_Char_T, size>::InfinityString, SizeT{3});
            } else {
                // stream.Write(DigitUtils::DigitStrings<_Char_T, size>::NANString,
                //              StringUtils::ConstCount(DigitUtils::DigitStrings<_Char_T, size>::NANString));

                stream.Write(DigitUtils::DigitStrings<_Char_T, size>::NANString, SizeT{3});
            }
        }
    }

    template <typename _Stream_T, typename _Number_T>
    static void insertPowerOfTen(_Stream_T &stream, _Number_T power_of_ten, bool positive) {
        stream += DigitUtils::DigitChars::E_Char;
        stream += (positive ? DigitUtils::DigitChars::PositiveChar : DigitUtils::DigitChars::NegativeChar);

        if (power_of_ten < _Number_T{10}) {
            // e+01,e+09
            stream += DigitUtils::DigitChars::ZeroChar;
        }

        NumberToString(stream, power_of_ten);
    }

    template <typename _Stream_T>
    inline static void insertZeros(_Stream_T &stream, const SizeT length) {
        using _Char_T          = typename _Stream_T::CharType;
        constexpr SizeT32 size = sizeof(_Char_T);
        stream.Write(DigitUtils::DigitStrings<_Char_T, size>::ZeroesString, length);
    }

    template <typename _Stream_T, typename _BigInt_T>
    static void bigIntToString(_Stream_T &stream, _BigInt_T &bint) {
        using DigitLimit = DigitUtils::DigitLimit<_BigInt_T::SizeOfType()>;

        while (bint.IsBig()) {
            const SizeT length = stream.Length();
            NumberToString<true>(stream, bint.Divide(DigitLimit::MaxPowerOfTenValue));

            // dividing '1000000000000000000' by '1000000000' yield zeros remainder
            insertZeros(stream, (DigitLimit::MaxPowerOfTen - SizeT(stream.Length() - length)));
        }

        if (bint.NotZero()) {
            NumberToString<true>(stream, bint.Number());
        }
    }

    template <typename _BigInt_T>
    inline static void bigIntDropDigits(_BigInt_T &bint, SizeT32 drop) noexcept {
        using DigitLimit = DigitUtils::DigitLimit<_BigInt_T::SizeOfType()>;

        while (drop >= DigitLimit::MaxPowerOfFive) {
            bint /= DigitLimit::GetPowerOfFive(DigitLimit::MaxPowerOfFive);
            drop -= DigitLimit::MaxPowerOfFive;
        }

        if (drop != 0U) {
            bint /= DigitLimit::GetPowerOfFive(drop);
        }
    }

    template <typename _Stream_T>
    static void formatStringNumber(_Stream_T &stream, const SizeT started_at, const SizeT32 precision,
                                   const SizeT32 calculated_digits, SizeT32 fraction_length, const bool is_positive_exp,
                                   const bool round_up) {
        using _Char_T                    = typename _Stream_T::CharType;
        const SizeT   stream_length      = (stream.Length() - started_at);
        SizeT         index              = started_at;
        const SizeT32 precision_plus_one = (precision + SizeT32{1});
        SizeT32       length;
        SizeT32       power;

        if (is_positive_exp) {
            length = (stream_length +
                      ((calculated_digits > precision_plus_one) ? (calculated_digits - precision_plus_one) : SizeT{0}) -
                      fraction_length);
            power  = (length - SizeT32{1});
        } else {
            length = stream_length;
            power = (((fraction_length > stream_length) ? (fraction_length - stream_length) : SizeT32{0}) + SizeT32{1});
        }

        if (stream_length > precision_plus_one) {
            index += SizeT(stream_length - precision_plus_one);
        }

        if (stream_length > precision) {
            bool is_power_increased = false;
            roundStringNumber(stream, index, is_power_increased, round_up);

            if (is_power_increased) {
                stream.Storage()[index] = DigitUtils::DigitChars::OneChar;

                if (is_positive_exp) {
                    fraction_length = SizeT32{0};
                    ++power;

                    if (power < precision) {
                        SizeT32 power_index = power;

                        do {
                            --index;
                            stream.Storage()[index] = DigitUtils::DigitChars::ZeroChar;
                        } while (--power_index != SizeT32{0});
                    }
                } else {
                    --power;
                }
            }
        }
        /////////////////////////////////////////////////////
        const bool display_exp =
            ((is_positive_exp && ((power + SizeT{1}) > precision)) || (!is_positive_exp && (power > SizeT{4})));
        /////////////////////////////////////////////////////
        _Char_T       *number = (stream.Storage() + index);
        const _Char_T *last   = stream.Last();

        if (display_exp || (fraction_length != SizeT32{0})) {
            while ((number < last) && (*number == DigitUtils::DigitChars::ZeroChar)) {
                ++number;
                ++index;
            }
        }

        const SizeT index2 = (index - started_at);

        if (!display_exp) {
            if ((fraction_length != SizeT32{0}) && (fraction_length > index2)) {
                if (!is_positive_exp && (power != SizeT{0})) {
                    insertZeros(stream, SizeT(power - SizeT{1}));
                    stream += DigitUtils::DigitChars::DotChar;
                    stream += DigitUtils::DigitChars::ZeroChar;
                } else if ((stream_length - SizeT{1}) != index2) {
                    stream.InsertAt(DigitUtils::DigitChars::DotChar, SizeT(fraction_length + started_at));
                }
            }
        } else if ((stream_length - SizeT{1}) != index2) {
            stream.InsertAt(DigitUtils::DigitChars::DotChar, (stream.Length() - SizeT{1}));
        }

        stream.Reverse(started_at);

        if (index != started_at) {
            stream.StepBack(index - started_at);
        }

        if (display_exp) {
            insertPowerOfTen(stream, power, is_positive_exp);
        }
    }

    template <typename _Stream_T>
    static void roundStringNumber(_Stream_T &stream, SizeT &index, bool &is_power_increased, bool round_up) noexcept {
        using _Char_T = typename _Stream_T::CharType;

        const _Char_T *last   = stream.Last();
        _Char_T       *number = (stream.Storage() + index);

        ++index;

        const bool round =
            ((number < last) && ((*number > DigitUtils::DigitChars::FiveChar) ||
                                 ((*number == DigitUtils::DigitChars::FiveChar) &&
                                  (round_up || ((SizeT32(stream.Storage()[index] - DigitUtils::DigitChars::ZeroChar) &
                                                 SizeT32{1}) == SizeT32{1})))));

        if (round) {
            while ((++number < last) && (*number == DigitUtils::DigitChars::NineChar)) {
                ++index;
            }

            if ((number != last) || (*number != DigitUtils::DigitChars::NineChar)) {
                ++(*number);
            } else {
                is_power_increased = true;
            }
        }
    }
};

} // namespace Qentem

#endif
