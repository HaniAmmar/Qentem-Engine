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

#ifndef QENTEM_DIGIT_H_
#define QENTEM_DIGIT_H_

/*
 * For converting numbers from and to strings.
 */
namespace Qentem {
struct Digit {
    enum class RealFormatType : unsigned char { Default = 0, Fixed = 1, Scientific = 2 };

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
    template <bool Reverse_V_ = false, typename Stream_T_, typename Number_T_>
    inline static void NumberToString(Stream_T_ &stream, Number_T_ number) {
        constexpr SizeT32 n_size = sizeof(Number_T_);
        using Char_T_            = typename Stream_T_::CharType;
        using QNumberType_T      = typename QNumberAutoTypeT<Number_T_, n_size>::QNumberType_T;

        constexpr SizeT32 max_number_of_digits = (((n_size * 8U * 30103U) / 100000U) + 1U);
        Char_T_           storage[max_number_of_digits];

        QNumberType_T qn{number};

        if QENTEM_CONSTEXPR (!IsUnsigned<Number_T_>()) {
            if (number < Number_T_{0}) {
                qn.Integer = -qn.Integer;
                stream += DigitUtils::DigitChars::NegativeChar;
            }
        }

        if QENTEM_CONSTEXPR (Reverse_V_) {
            stream.Write(&(storage[0U]), intToString<true>(&(storage[0U]), qn.Natural));
        } else {
            const SizeT offset = intToString(&(storage[max_number_of_digits]), qn.Natural);
            stream.Write(&(storage[max_number_of_digits - offset]), offset);
        }
    }

    template <typename Stream_T_, typename Number_T_>
    inline static void NumberToString(Stream_T_ &stream, Number_T_ number, RealFormatInfo format) {
        constexpr SizeT32 n_size = sizeof(Number_T_);
        using QNumberType_T      = typename QNumberAutoTypeT<Number_T_, n_size>::QNumberType_T;

        realToString<Number_T_>(stream, QNumberType_T{number}.Natural, format);
    }
    /////////////////////////////////////////////////////////////////
    template <typename Char_T_>
    static SizeT32 HexStringToNumber(const Char_T_ *value, const SizeT length) noexcept {
        using Uint_Char_T = SizeT32;

        SizeT32 number{0};
        SizeT32 offset{0};

        while (offset < length) {
            const Char_T_ digit = value[offset];
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
    template <typename Number_T_, typename Char_T_>
    static void FastStringToNumber(Number_T_ &number, const Char_T_ *content, SizeT length) noexcept {
        SizeT offset = 0;
        number       = 0;

        if (offset < length) {
            number += Number_T_(content[offset]);
            number -= Number_T_{DigitUtils::DigitChars::ZeroChar};
            ++offset;

            while (offset < length) {
                number *= Number_T_{10};
                number += Number_T_(content[offset]);
                number -= Number_T_{DigitUtils::DigitChars::ZeroChar};
                ++offset;
            }
        }
    }

    template <typename Char_T_>
    inline static QNumberType StringToNumber(QNumber64 &number, const Char_T_ *content, SizeT &offset,
                                             SizeT end_offset) noexcept {
        return stringToNumber(number, content, offset, end_offset);
    }

    template <typename Char_T_>
    inline static QNumberType StringToNumber(QNumber64 &number, const Char_T_ *content, SizeT end_offset) noexcept {
        SizeT offset{0};
        return stringToNumber(number, content, offset, end_offset);
    }

    //////////// Private ////////////
  private:
    template <typename Char_T_>
    static QNumberType stringToNumber(QNumber64 &number, const Char_T_ *content, SizeT &offset,
                                      SizeT end_offset) noexcept {
        using Number_T = decltype(number.Natural);

        constexpr SizeT max_length = SizeT{19};
        number.Natural             = 0;

        if (offset < end_offset) {
            SizeT   dot_offset   = 0;
            SizeT   exp_offset   = 0;
            SizeT   start_offset = 0;
            SizeT   tmp_offset;
            Char_T_ digit         = content[offset];
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
    template <typename Number_T_>
    static void powerOfNegativeTen(Number_T_ &number, SizeT32 exponent) noexcept {
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
        if QENTEM_CONSTEXPR (sizeof(UNumber_T) == 8U) {
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

            number += (number & Number_T_{1});
            number >>= 1U;
            exp += (number > Number_T_{0x1FFFFFFFFFFFFF});
        } else {
            shifted -= bit;

            if (exp > shifted) {
                exp -= shifted;
                number += (number & Number_T_{1});
                number >>= 1U;
                exp += (number > Number_T_{0x1FFFFFFFFFFFFF});
            } else {
                shifted -= exp;
                ++shifted;
                number >>= shifted;
                number += (number & Number_T_{1});
                number >>= 1U;
                exp = (number > Number_T_{0xFFFFFFFFFFFFF});
            }
        }

        number &= 0xFFFFFFFFFFFFFULL;
        number |= (SizeT64(exp) << 52U);
    }
    /////////////////////////////////////////
    template <typename Number_T_>
    static void powerOfPositiveTen(Number_T_ &number, SizeT32 exponent) noexcept {
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
            number += (number & Number_T_{1});
            number >>= 1U;
            shifted += SizeT32(number > Number_T_{0x1FFFFFFFFFFFFF});
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
    template <typename Char_T_>
    static bool parseExponent(const Char_T_ *content, SizeT32 &exponent, bool &is_negative_exp, SizeT &offset,
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
                        const Char_T_ digit = content[offset];

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
    template <bool Reverse_V_ = false, typename Char_T_, typename Number_T_>
    static SizeT intToString(Char_T_ *storage, Number_T_ number) noexcept {
        const Char_T_ *str = storage;

        if QENTEM_CONSTEXPR (!Reverse_V_) {
            while (number >= Number_T_{10}) {
                const SizeT index = (SizeT(number % Number_T_{100}) * SizeT{2});
                number /= Number_T_{100};

                --storage;
                *storage = Char_T_(DigitUtils::DigitTable1[index + SizeT{1}]);
                --storage;
                *storage = Char_T_(DigitUtils::DigitTable1[index]);
            }

            if ((number != Number_T_{0}) || (str == storage)) {
                --storage;
                *storage = Char_T_(DigitUtils::DigitTable2[number]);
            }

            return SizeT(str - storage);
        } else {
            while (number >= Number_T_{10}) {
                const SizeT index = (SizeT(number % Number_T_{100}) * SizeT{2});
                number /= Number_T_{100};

                *storage = Char_T_(DigitUtils::DigitTable1[index + SizeT{1}]);
                ++storage;
                *storage = Char_T_(DigitUtils::DigitTable1[index]);
                ++storage;
            }

            if ((number != Number_T_{0}) || (str == storage)) {
                *storage = Char_T_(DigitUtils::DigitTable2[number]);
                ++storage;
            }

            return SizeT(storage - str);
        }
    }
    /////////////////////////////////////////
    template <typename Float_T_, typename Stream_T_, typename Number_T_>
    static void realToString(Stream_T_ &stream, const Number_T_ number, const RealFormatInfo format) {
        using Char_T_ = typename Stream_T_::CharType;
        using Info_T  = DigitUtils::RealNumberInfo<Float_T_, sizeof(number)>;

        const Number_T_ bias = (number & Info_T::ExponentMask);

        if (bias != Info_T::ExponentMask) {
            if ((number & Info_T::SignMask) != Number_T_{0}) {
                stream += DigitUtils::DigitChars::NegativeChar;
            }

            Number_T_ mantissa = (number & Info_T::MantissaMask);

            if ((mantissa != Number_T_{0}) || (bias != Number_T_{0})) {
                if (bias != Number_T_{0}) {
                    mantissa |= Info_T::LeadingBit;
                } else {
                    mantissa <<= 1U;
                }

                BigInt<SystemIntType, ((Info_T::Bias + 1U) + (sizeof(Number_T_) * 8U * 3U))> bint{mantissa};
                using DigitLimit = DigitUtils::DigitLimit<bint.SizeOfType()>;
                /////////////////////////////////////
                const SizeT32 first_shift      = Platform::FindFirstBit(mantissa);
                const int     exponent         = (int)((bias >> Info_T::MantissaSize) - Info_T::Bias);
                const bool    is_positive_exp  = (exponent >= 0);
                const SizeT32 positive_exp     = SizeT32(is_positive_exp ? exponent : -exponent);
                const SizeT32 first_bit        = (Info_T::MantissaSize - first_shift);
                const SizeT32 exp_actual_value = (positive_exp + ((bias == Number_T_{0}) * first_bit));
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
            constexpr SizeT32 size = sizeof(Char_T_);

            if ((number & Info_T::MantissaMask) == Number_T_{0}) {
                if ((number & Info_T::SignMask) != Number_T_{0}) {
                    stream += DigitUtils::DigitChars::NegativeChar;
                }

                // stream.Write(DigitUtils::DigitStrings<Char_T_, size>::InfinityString,
                //              StringUtils::ConstCount(DigitUtils::DigitStrings<Char_T_, size>::InfinityString));

                stream.Write(DigitUtils::DigitStrings<Char_T_, size>::InfinityString, SizeT{3});
            } else {
                // stream.Write(DigitUtils::DigitStrings<Char_T_, size>::NANString,
                //              StringUtils::ConstCount(DigitUtils::DigitStrings<Char_T_, size>::NANString));

                stream.Write(DigitUtils::DigitStrings<Char_T_, size>::NANString, SizeT{3});
            }
        }
    }

    template <typename Stream_T_, typename Number_T_>
    static void insertPowerOfTen(Stream_T_ &stream, Number_T_ power_of_ten, bool positive) {
        stream += DigitUtils::DigitChars::E_Char;
        stream += (positive ? DigitUtils::DigitChars::PositiveChar : DigitUtils::DigitChars::NegativeChar);

        if (power_of_ten < Number_T_{10}) {
            // e+01,e+09
            stream += DigitUtils::DigitChars::ZeroChar;
        }

        NumberToString(stream, power_of_ten);
    }

    template <typename Stream_T_>
    inline static void insertZeros(Stream_T_ &stream, const SizeT length) {
        using Char_T_          = typename Stream_T_::CharType;
        constexpr SizeT32 size = sizeof(Char_T_);
        stream.Write(DigitUtils::DigitStrings<Char_T_, size>::ZeroesString, length);
    }

    template <typename Stream_T_, typename BigInt_T_>
    static void bigIntToString(Stream_T_ &stream, BigInt_T_ &bint) {
        using DigitLimit = DigitUtils::DigitLimit<BigInt_T_::SizeOfType()>;

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

    template <typename BigInt_T_>
    inline static void bigIntDropDigits(BigInt_T_ &bint, SizeT32 drop) noexcept {
        using DigitLimit = DigitUtils::DigitLimit<BigInt_T_::SizeOfType()>;

        while (drop >= DigitLimit::MaxPowerOfFive) {
            bint /= DigitLimit::GetPowerOfFive(DigitLimit::MaxPowerOfFive);
            drop -= DigitLimit::MaxPowerOfFive;
        }

        if (drop != 0U) {
            bint /= DigitLimit::GetPowerOfFive(drop);
        }
    }

    template <typename Stream_T_>
    static void formatStringNumber(Stream_T_ &stream, const SizeT started_at, const SizeT32 precision,
                                   const SizeT32 calculated_digits, SizeT32 fraction_length, const bool is_positive_exp,
                                   const bool round_up) {
        using Char_T_             = typename Stream_T_::CharType;
        const SizeT stream_length = (stream.Length() - started_at);
        SizeT       index         = started_at;
        SizeT       power;
        SizeT       length;
        const SizeT precision_plus_one = SizeT(precision + SizeT{1});

        if (is_positive_exp) {
            length =
                SizeT(stream_length +
                      ((calculated_digits > precision_plus_one) ? (calculated_digits - precision_plus_one) : SizeT{0}) -
                      fraction_length);
            power = (length - SizeT{1});
        } else {
            length = stream_length;
            power =
                (((fraction_length > stream_length) ? SizeT(fraction_length - stream_length) : SizeT{0}) + SizeT{1});
        }

        if (stream_length > precision_plus_one) {
            index += (stream_length - precision_plus_one);
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
                        SizeT power_index = power;

                        do {
                            --index;
                            stream.Storage()[index] = DigitUtils::DigitChars::ZeroChar;
                        } while (--power_index != SizeT{0});
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
        Char_T_       *number = (stream.Storage() + index);
        const Char_T_ *last   = stream.Last();

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
            stream.InsertAt(DigitUtils::DigitChars::DotChar, (stream.Length() - 1U));
        }

        stream.Reverse(started_at);

        if (index != started_at) {
            stream.StepBack(index - started_at);
        }

        if (display_exp) {
            insertPowerOfTen(stream, power, is_positive_exp);
        }
    }

    template <typename Stream_T_>
    static void roundStringNumber(Stream_T_ &stream, SizeT &index, bool &is_power_increased, bool round_up) noexcept {
        using Char_T_ = typename Stream_T_::CharType;

        const Char_T_ *last   = stream.Last();
        Char_T_       *number = (stream.Storage() + index);

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
