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
#include "QNumber.hpp"
#include "BigInt.hpp"
#include "DigitUtils.hpp"
#include "StringUtils.hpp"

#ifndef QENTEM_DIGIT_H
#define QENTEM_DIGIT_H

/*
 * For converting numbers from and to strings.
 */
namespace Qentem {
struct Digit {
    enum struct RealFormatType : SizeT8 { Default = 0, Fixed = 1, Scientific = 2 };

    struct RealFormatInfo {
        RealFormatInfo() noexcept {
        }

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
    template <bool Reverse_V_T = false, typename Stream_T, typename Number_T>
    inline static void NumberToString(Stream_T &stream, Number_T number,
                                      const RealFormatInfo format = RealFormatInfo{}) {
        constexpr SizeT32 n_size = sizeof(Number_T);
        using Char_T             = typename Stream_T::CharType;
        using QNumberType_T      = typename QNumberAutoTypeT<Number_T, n_size>::QNumberType_T;

        QNumberType_T qn{number};

        if (IsFloat<Number_T>()) {
            realToString<Number_T>(stream, QNumberType_T{number}.Natural, format);
        } else {
            constexpr SizeT32 max_number_of_digits = (((n_size * 8U * 30103U) / 100000U) + 1U);
            Char_T            storage[max_number_of_digits];

            if (!IsUnsigned<Number_T>()) {
                if (number < Number_T{0}) {
                    qn.Integer = -qn.Integer;
                    stream += DigitUtils::DigitChar::Negative;
                }
            }

            if (Reverse_V_T) {
                stream.Write(&(storage[0U]), intToString<true>(&(storage[0U]), qn.Natural));
            } else {
                const SizeT offset = intToString(&(storage[max_number_of_digits]), qn.Natural);
                stream.Write(&(storage[max_number_of_digits - offset]), offset);
            }
        }
    }
    /////////////////////////////////////////////////////////////////
    template <typename Number_T, typename Char_T>
    static Number_T HexStringToNumber(const Char_T *value, SizeT &offset, const SizeT end_offset) noexcept {
        Number_T number{0};

        while (offset < end_offset) {
            const Char_T digit = value[offset];

            if ((digit >= DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                // 0-9
                number <<= 4U;
                ++offset;
                number |= Number_T(digit - DigitUtils::DigitChar::Zero);
                continue;
            }

            if ((digit >= DigitUtils::DigitChar::UA) && (digit <= DigitUtils::DigitChar::UF)) {
                // A-F
                number <<= 4U;
                ++offset;
                number |= Number_T(digit - DigitUtils::DigitChar::Seven);
                continue;
            }

            if ((digit >= DigitUtils::DigitChar::A) && (digit <= DigitUtils::DigitChar::F)) {
                // a-f
                number <<= 4U;
                ++offset;
                number |= Number_T(digit - DigitUtils::DigitChar::UW);
                continue;
            }

            break;
        }

        return number;
    }

    template <typename Number_T, typename Char_T>
    static Number_T HexStringToNumber(const Char_T *value, const SizeT length) noexcept {
        SizeT offset{0};
        return HexStringToNumber<Number_T>(value, offset, length);
    }
    /////////////////////////////////////////////////////////////////
    template <typename Number_T, typename Char_T>
    static void FastStringToNumber(Number_T &number, const Char_T *content, SizeT length) noexcept {
        SizeT offset = 0;
        number       = 0;

        if (offset < length) {
            number += Number_T(content[offset]);
            number -= Number_T{DigitUtils::DigitChar::Zero};
            ++offset;

            while (offset < length) {
                number *= Number_T{10};
                number += Number_T(content[offset]);
                number -= Number_T{DigitUtils::DigitChar::Zero};
                ++offset;
            }
        }
    }

    template <typename Char_T>
    inline static QNumberType StringToNumber(QNumber64 &number, const Char_T *content, SizeT &offset,
                                             SizeT end_offset) noexcept {
        return stringToNumber(number, content, offset, end_offset);
    }

    template <typename Char_T>
    inline static QNumberType StringToNumber(QNumber64 &number, const Char_T *content, SizeT end_offset) noexcept {
        SizeT offset{0};
        return stringToNumber(number, content, offset, end_offset);
    }

    //////////// Private ////////////
  private:
    template <typename Char_T>
    static QNumberType stringToNumber(QNumber64 &number, const Char_T *content, SizeT &offset,
                                      SizeT end_offset) noexcept {
        using Number_T = decltype(number.Natural);

        constexpr SizeT max_length = SizeT{19};
        number.Natural             = 0;

        if (offset < end_offset) {
            SizeT  dot_offset   = 0;
            SizeT  exp_offset   = 0;
            SizeT  start_offset = 0;
            SizeT  tmp_offset;
            Char_T digit         = content[offset];
            bool   is_negative   = false;
            bool   is_real       = false;
            bool   has_dot       = false;
            bool   fraction_only = false;
            ///////////////////////////////////////////////////////////
            if (digit == DigitUtils::DigitChar::Negative) {
                ++offset;
                is_negative = true;
            } else if (digit == DigitUtils::DigitChar::Positive) {
                ++offset;
            }

            tmp_offset = offset;
            ///////////////////////////////////////////////////////////
            if (offset < end_offset) {
                digit = content[offset];

                if ((digit > DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                    // 123456789
                    number.Natural += Number_T(digit);
                    number.Natural -= Number_T{DigitUtils::DigitChar::Zero};
                    tmp_offset   = (((end_offset - offset) < max_length) ? end_offset : (offset + max_length));
                    start_offset = offset;
                    ++offset;
                } else if ((digit == DigitUtils::DigitChar::Zero) || (digit == DigitUtils::DigitChar::Dot)) {
                    if ((digit == DigitUtils::DigitChar::Zero) && (offset + SizeT{1}) < end_offset) {
                        ++offset;
                        digit = content[offset];

                        if ((digit == DigitUtils::DigitChar::X) || (digit == DigitUtils::DigitChar::UX)) {
                            // Hex
                            ++offset;

                            number.Natural = HexStringToNumber<decltype(number.Natural)>(content, offset, end_offset);
                            return QNumberType::Natural;
                        } else if ((digit >= DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                            // 0000xxxx
                            return QNumberType::NotANumber; // Leading zero.
                        }
                    }

                    if (digit == DigitUtils::DigitChar::Dot) {
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

                            if (digit == DigitUtils::DigitChar::Zero) {
                                ++offset;
                                continue;
                            }

                            break;
                        }
                        ///////////////////////////////////////////////////////////
                        if ((start_offset == offset) && (dot_offset == tmp_offset) &&
                            ((digit < DigitUtils::DigitChar::Zero) || (digit > DigitUtils::DigitChar::Nine))) {
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

                        if ((digit >= DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                            number.Natural *= Number_T{10};
                            number.Natural += Number_T(digit);
                            number.Natural -= Number_T{DigitUtils::DigitChar::Zero};
                            ++offset;
                            continue;
                        }

                        break;
                    }

                    if (digit == DigitUtils::DigitChar::Dot) {
                        if (!has_dot) {
                            dot_offset = offset;
                            ++offset;
                            is_real = true;
                            has_dot = true;

                            if (offset < max_end_offset) {
                                digit = content[offset];

                                if ((digit > DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                                    continue;
                                }

                                if ((digit == DigitUtils::DigitChar::Zero) && (offset + 1U) < max_end_offset) {
                                    digit = content[offset + SizeT{1}];

                                    if ((digit >= DigitUtils::DigitChar::Zero) &&
                                        (digit <= DigitUtils::DigitChar::Nine)) {
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
                        case DigitUtils::DigitChar::Dot:
                        case DigitUtils::DigitChar::E:
                        case DigitUtils::DigitChar::UE: {
                            is_real = true;
                            break;
                        }

                        default: {
                            if ((digit >= DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                                if ((number.Natural > 0x1999999999999999ULL) ||
                                    ((number.Natural == 0x1999999999999999ULL) &&
                                     (digit > DigitUtils::DigitChar::Five))) {
                                    is_real = true;
                                    break;
                                }

                                number.Natural *= Number_T{10};
                                number.Natural += Number_T(digit);
                                number.Natural -= Number_T{DigitUtils::DigitChar::Zero};
                                ++offset;
                                ++tmp_offset;

                                if (offset < end_offset) {
                                    digit = content[offset];

                                    switch (digit) {
                                        case DigitUtils::DigitChar::Dot:
                                        case DigitUtils::DigitChar::E:
                                        case DigitUtils::DigitChar::UE: {
                                            is_real = true;
                                            break;
                                        }

                                        default: {
                                            if ((digit >= DigitUtils::DigitChar::Zero) &&
                                                (digit <= DigitUtils::DigitChar::Nine)) {
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

                        if ((digit >= DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                            ++offset;
                            keep_going = (offset < end_offset);
                            continue;
                        }

                        switch (content[offset]) {
                            case DigitUtils::DigitChar::Dot: {
                                if (!has_dot) {
                                    dot_offset = offset;
                                    ++offset;
                                    has_dot    = true;
                                    keep_going = (offset < end_offset);
                                    continue;
                                }

                                return QNumberType::NotANumber;
                            }

                            case DigitUtils::DigitChar::E:
                            case DigitUtils::DigitChar::UE: {
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
    template <typename Number_T>
    static void powerOfNegativeTen(Number_T &number, SizeT32 exponent) noexcept {
        using UNumber_T  = SizeT64;
        using DigitConst = DigitUtils::DigitConst<sizeof(UNumber_T)>;
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
            while (exponent >= DigitConst::MaxPowerOfFive) {
                // 2**126 = 85070591730234615865843651857942052864
                // 5**27 = 7450580596923828125 (MaxPowerOfFive)
                // 2**126 / 5**27 = 11417981541647679048.4
                // 126-64=62; See 2**126 and 64 shift

                bint *= DigitConst::GetPowerOfOneOverFive(DigitConst::MaxPowerOfFive);
                bint >>= DigitConst::MaxShift;
                shifted += DigitConst::GetPowerOfOneOverFiveShift(DigitConst::MaxPowerOfFive);
                exponent -= DigitConst::MaxPowerOfFive;
            }

            if (exponent != SizeT32{0}) {
                bint *= DigitConst::GetPowerOfOneOverFive(exponent);
                bint >>= DigitConst::MaxShift;
                shifted += DigitConst::GetPowerOfOneOverFiveShift(exponent);
            }
        } else {
            while (exponent >= DigitConst::MaxPowerOfFive) {
                bint <<= DigitConst::MaxShift;
                bint /= DigitConst::GetPowerOfFive(DigitConst::MaxPowerOfFive);
                shifted += DigitConst::MaxShift;
                exponent -= DigitConst::MaxPowerOfFive;
            }

            if (exponent != SizeT32{0}) {
                bint <<= DigitConst::MaxShift;
                bint /= DigitConst::GetPowerOfFive(exponent);
                shifted += DigitConst::MaxShift;
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

            number += (number & Number_T{1});
            number >>= 1U;
            exp += (number > Number_T{0x1FFFFFFFFFFFFF});
        } else {
            shifted -= bit;

            if (exp > shifted) {
                exp -= shifted;
                number += (number & Number_T{1});
                number >>= 1U;
                exp += (number > Number_T{0x1FFFFFFFFFFFFF});
            } else {
                shifted -= exp;
                ++shifted;
                number >>= shifted;
                number += (number & Number_T{1});
                number >>= 1U;
                exp = (number > Number_T{0xFFFFFFFFFFFFF});
            }
        }

        number &= 0xFFFFFFFFFFFFFULL;
        number |= (SizeT64(exp) << 52U);
    }
    /////////////////////////////////////////
    template <typename Number_T>
    static void powerOfPositiveTen(Number_T &number, SizeT32 exponent) noexcept {
        using UNumber_T  = SystemIntType;
        using DigitConst = DigitUtils::DigitConst<sizeof(UNumber_T)>;
        //////////////////////////////////////////////////////////////
        BigInt<UNumber_T, 192U> bint{number};
        //////////////////////////////////////////////////////////////
        constexpr SizeT32 bint_limit{2U};
        SizeT32           shifted{exponent};
        //////////////////////////////////////////////////////////////
        while (exponent >= DigitConst::MaxPowerOfFive) {
            bint *= DigitConst::GetPowerOfFive(DigitConst::MaxPowerOfFive);

            if (bint.Index() > bint_limit) {
                bint >>= DigitConst::MaxShift;
                shifted += DigitConst::MaxShift;
            }

            exponent -= DigitConst::MaxPowerOfFive;
        }

        if (exponent != SizeT32{0}) {
            bint *= DigitConst::GetPowerOfFive(exponent);
        }
        //////////////////////////////////////////////////////////////
        const SizeT32 bit = bint.FindLastBit();

        if (bit <= 52U) {
            number = SizeT64(bint);
            number <<= (52U - bit);
        } else {
            bint >>= (bit - 53U);
            number = SizeT64(bint);
            number += (number & Number_T{1});
            number >>= 1U;
            shifted += SizeT32(number > Number_T{0x1FFFFFFFFFFFFF});
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
    template <typename Char_T>
    static bool parseExponent(const Char_T *content, SizeT32 &exponent, bool &is_negative_exp, SizeT &offset,
                              const SizeT end_offset) noexcept {
        bool sign_set{false};

        while (offset < end_offset) {
            switch (content[offset]) {
                case DigitUtils::DigitChar::Positive: {
                    if (!sign_set) {
                        ++offset;
                        sign_set = true;
                        break;
                    }

                    return false;
                }
                case DigitUtils::DigitChar::Negative: {
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
                        const Char_T digit = content[offset];

                        if ((digit >= DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                            exponent *= SizeT32{10};
                            exponent += SizeT32(digit - DigitUtils::DigitChar::Zero);
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
    template <bool Reverse_V_T = false, typename Char_T, typename Number_T>
    static SizeT intToString(Char_T *storage, Number_T number) noexcept {
        const Char_T *str = storage;

        if (!Reverse_V_T) {
            while (number >= Number_T{10}) {
                const SizeT index = (SizeT(number % Number_T{100}) * SizeT{2});
                number /= Number_T{100};

                --storage;
                *storage = Char_T(DigitUtils::DigitTable1[index + SizeT{1}]);
                --storage;
                *storage = Char_T(DigitUtils::DigitTable1[index]);
            }

            if ((number != Number_T{0}) || (str == storage)) {
                --storage;
                *storage = Char_T(DigitUtils::DigitTable2[number]);
            }

            return SizeT(str - storage);
        } else {
            while (number >= Number_T{10}) {
                const SizeT index = (SizeT(number % Number_T{100}) * SizeT{2});
                number /= Number_T{100};

                *storage = Char_T(DigitUtils::DigitTable1[index + SizeT{1}]);
                ++storage;
                *storage = Char_T(DigitUtils::DigitTable1[index]);
                ++storage;
            }

            if ((number != Number_T{0}) || (str == storage)) {
                *storage = Char_T(DigitUtils::DigitTable2[number]);
                ++storage;
            }

            return SizeT(storage - str);
        }
    }
    /////////////////////////////////////////
    template <typename Float_T, typename Stream_T, typename Number_T>
    static void realToString(Stream_T &stream, const Number_T number, const RealFormatInfo format) {
        using Char_T = typename Stream_T::CharType;
        using Info_T = DigitUtils::RealNumberInfo<Float_T, sizeof(number)>;

        const Number_T bias = (number & Info_T::ExponentMask);

        if (bias != Info_T::ExponentMask) {
            if ((number & Info_T::SignMask) != Number_T{0}) {
                stream += DigitUtils::DigitChar::Negative;
            }

            Number_T mantissa = (number & Info_T::MantissaMask);

            if ((mantissa != Number_T{0}) || (bias != Number_T{0})) {
                if (bias != Number_T{0}) {
                    mantissa |= Info_T::LeadingBit;
                } else {
                    mantissa <<= 1U;
                }

                BigInt<SystemIntType, ((Info_T::Bias + 1U) + (sizeof(Number_T) * 8U * 3U))> bint{mantissa};
                using DigitConst = DigitUtils::DigitConst<bint.SizeOfType()>;
                /////////////////////////////////////
                const SizeT32 first_shift      = Platform::FindFirstBit(mantissa);
                const int     exponent         = (int)((bias >> Info_T::MantissaSize) - Info_T::Bias);
                const bool    is_positive_exp  = (exponent >= 0);
                const SizeT32 positive_exp     = SizeT32(is_positive_exp ? exponent : -exponent);
                const SizeT32 first_bit        = (Info_T::MantissaSize - first_shift);
                const SizeT32 exp_actual_value = (positive_exp + ((bias == Number_T{0}) * first_bit));
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

                    if (times >= DigitConst::MaxPowerOfFive) {
                        const SizeT32 max_index = (format.Precision < Info_T::MaxCut)
                                                      ? ((format.Precision / DigitConst::MaxPowerOfTen) + SizeT32{2})
                                                      : bint.MaxIndex();

                        do {
                            bint *= DigitConst::GetPowerOfFive(DigitConst::MaxPowerOfFive);

                            if ((bint.Index() >= max_index) && (shift >= DigitConst::MaxShift)) {
                                bint >>= DigitConst::MaxShift;
                                shift -= DigitConst::MaxShift;
                            }

                            times -= DigitConst::MaxPowerOfFive;
                        } while (times >= DigitConst::MaxPowerOfFive);
                    }

                    if (times != SizeT32{0}) {
                        bint *= DigitConst::GetPowerOfFive(times);
                    }

                    bint >>= shift;
                }

                const SizeT start_at = stream.Length();
                bigIntToString(stream, bint);

                // Only 'Default' is implemented
                formatStringNumber(stream, start_at, format.Precision, digits, fraction_length, is_positive_exp,
                                   round_up);
            } else {
                stream += DigitUtils::DigitChar::Zero;
            }
        } else {
            constexpr SizeT32 size = sizeof(Char_T);

            if ((number & Info_T::MantissaMask) == Number_T{0}) {
                if ((number & Info_T::SignMask) != Number_T{0}) {
                    stream += DigitUtils::DigitChar::Negative;
                }

                // stream.Write(DigitUtils::DigitString<Char_T, size>::Infinity,
                //              StringUtils::ConstCount(DigitUtils::DigitString<Char_T, size>::Infinity));

                stream.Write(DigitUtils::DigitString<Char_T, size>::Infinity, SizeT{3});
            } else {
                // stream.Write(DigitUtils::DigitString<Char_T, size>::NAN,
                //              StringUtils::ConstCount(DigitUtils::DigitString<Char_T, size>::NAN));

                stream.Write(DigitUtils::DigitString<Char_T, size>::NAN, SizeT{3});
            }
        }
    }

    template <typename Stream_T, typename Number_T>
    static void insertPowerOfTen(Stream_T &stream, Number_T power_of_ten, bool positive) {
        stream += DigitUtils::DigitChar::E;
        stream += (positive ? DigitUtils::DigitChar::Positive : DigitUtils::DigitChar::Negative);

        if (power_of_ten < Number_T{10}) {
            // e+01,e+09
            stream += DigitUtils::DigitChar::Zero;
        }

        NumberToString(stream, power_of_ten);
    }

    template <typename Stream_T>
    inline static void insertZeros(Stream_T &stream, const SizeT length) {
        using Char_T           = typename Stream_T::CharType;
        constexpr SizeT32 size = sizeof(Char_T);
        stream.Write(DigitUtils::DigitString<Char_T, size>::Zeros, length);
    }

    template <typename Stream_T, typename BigInt_T>
    static void bigIntToString(Stream_T &stream, BigInt_T &bint) {
        using DigitConst = DigitUtils::DigitConst<BigInt_T::SizeOfType()>;

        while (bint.IsBig()) {
            const SizeT length = stream.Length();
            NumberToString<true>(stream, bint.Divide(DigitConst::MaxPowerOfTenValue));

            // dividing '1000000000000000000' by '1000000000' yield zeros remainder
            insertZeros(stream, (DigitConst::MaxPowerOfTen - SizeT(stream.Length() - length)));
        }

        if (bint.NotZero()) {
            NumberToString<true>(stream, bint.Number());
        }
    }

    template <typename BigInt_T>
    inline static void bigIntDropDigits(BigInt_T &bint, SizeT32 drop) noexcept {
        using DigitConst = DigitUtils::DigitConst<BigInt_T::SizeOfType()>;

        while (drop >= DigitConst::MaxPowerOfFive) {
            bint /= DigitConst::GetPowerOfFive(DigitConst::MaxPowerOfFive);
            drop -= DigitConst::MaxPowerOfFive;
        }

        if (drop != 0U) {
            bint /= DigitConst::GetPowerOfFive(drop);
        }
    }

    template <typename Stream_T>
    static void formatStringNumber(Stream_T &stream, const SizeT started_at, const SizeT32 precision,
                                   const SizeT32 calculated_digits, SizeT32 fraction_length, const bool is_positive_exp,
                                   const bool round_up) {
        using Char_T                     = typename Stream_T::CharType;
        const SizeT   stream_length      = (stream.Length() - started_at);
        SizeT         index              = started_at;
        const SizeT32 precision_plus_one = (precision + SizeT32{1});
        SizeT32       length;
        SizeT32       power;

        if (is_positive_exp) {
            length = SizeT32(
                stream_length +
                ((calculated_digits > precision_plus_one) ? (calculated_digits - precision_plus_one) : SizeT{0}) -
                fraction_length);
            power = (length - SizeT32{1});
        } else {
            length = SizeT32(stream_length);
            power  = SizeT32(((fraction_length > stream_length) ? (fraction_length - stream_length) : SizeT32{0}) +
                             SizeT32{1});
        }

        if (stream_length > precision_plus_one) {
            index += SizeT(stream_length - precision_plus_one);
        }

        if (stream_length > precision) {
            bool is_power_increased = false;
            roundStringNumber(stream, index, is_power_increased, round_up);

            if (is_power_increased) {
                stream.Storage()[index] = DigitUtils::DigitChar::One;

                if (is_positive_exp) {
                    fraction_length = SizeT32{0};
                    ++power;

                    if (power < precision) {
                        SizeT32 power_index = power;

                        do {
                            --index;
                            stream.Storage()[index] = DigitUtils::DigitChar::Zero;
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
        Char_T       *number = (stream.Storage() + index);
        const Char_T *last   = stream.Last();

        if (display_exp || (fraction_length != SizeT32{0})) {
            while ((number < last) && (*number == DigitUtils::DigitChar::Zero)) {
                ++number;
                ++index;
            }
        }

        const SizeT index2 = (index - started_at);

        if (!display_exp) {
            if ((fraction_length != SizeT32{0}) && (fraction_length > index2)) {
                if (!is_positive_exp && (power != SizeT{0})) {
                    insertZeros(stream, SizeT(power - SizeT{1}));
                    stream += DigitUtils::DigitChar::Dot;
                    stream += DigitUtils::DigitChar::Zero;
                } else if ((stream_length - SizeT{1}) != index2) {
                    stream.InsertAt(DigitUtils::DigitChar::Dot, SizeT(fraction_length + started_at));
                }
            }
        } else if ((stream_length - SizeT{1}) != index2) {
            stream.InsertAt(DigitUtils::DigitChar::Dot, (stream.Length() - SizeT{1}));
        }

        stream.Reverse(started_at);

        if (index != started_at) {
            stream.StepBack(index - started_at);
        }

        if (display_exp) {
            insertPowerOfTen(stream, power, is_positive_exp);
        }
    }

    template <typename Stream_T>
    static void roundStringNumber(Stream_T &stream, SizeT &index, bool &is_power_increased, bool round_up) noexcept {
        using Char_T = typename Stream_T::CharType;

        const Char_T *last   = stream.Last();
        Char_T       *number = (stream.Storage() + index);

        ++index;

        const bool round =
            ((number < last) && ((*number > DigitUtils::DigitChar::Five) ||
                                 ((*number == DigitUtils::DigitChar::Five) &&
                                  (round_up || ((SizeT32(stream.Storage()[index] - DigitUtils::DigitChar::Zero) &
                                                 SizeT32{1}) == SizeT32{1})))));

        if (round) {
            while ((++number < last) && (*number == DigitUtils::DigitChar::Nine)) {
                ++index;
            }

            if ((number != last) || (*number != DigitUtils::DigitChar::Nine)) {
                ++(*number);
            } else {
                is_power_increased = true;
            }
        }
    }
};

} // namespace Qentem

#endif
