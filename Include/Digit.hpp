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

#ifndef QENTEM_DIGIT_H
#define QENTEM_DIGIT_H

#include "QNumber.hpp"
#include "BigInt.hpp"
#include "DigitUtils.hpp"

namespace Qentem {
/*
 * For converting numbers from and to strings.
 */
struct Digit {
    // Default: same as std::defaultfloat.
    // Fixed: same as std::fixed.
    // SemiFixed: same as std::fixed, but do not insert zeros.
    // Scientific: not implemented.
    enum struct RealFormatType : SizeT8 { Default = 0, Fixed = 1, SemiFixed = 2, Scientific = 3 };

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

        if QENTEM_CONST_EXPRESSION (IsFloat<Number_T>()) {
            realToString<Number_T>(stream, QNumberType_T{number}.Natural, format);
        } else {
            constexpr SizeT32 max_number_of_digits = (((n_size * 8U * 30103U) / 100000) + 1U);
            Char_T            storage[max_number_of_digits];

            if (!IsUnsigned<Number_T>()) {
                if (number < Number_T{0}) {
                    qn.Integer = -qn.Integer;
                    stream += DigitUtils::DigitChar::Negative;
                }
            }

            if QENTEM_CONST_EXPRESSION (Reverse_V_T) {
                stream.Write(&(storage[0]), intToString<true>(&(storage[0]), qn.Natural));
            } else {
                const SizeT offset = intToString(&(storage[max_number_of_digits]), qn.Natural);
                stream.Write(&(storage[max_number_of_digits - offset]), offset);
            }
        }
    }
    /////////////////////////////////////////////////////////////////
    template <typename Number_T, typename Char_T, typename SizeT_Type>
    static Number_T HexStringToNumber(const Char_T *value, SizeT_Type &offset, const SizeT_Type end_offset) noexcept {
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
    inline static QNumberType StringToNumber(QNumber64 &number, const Char_T *content, SizeT length) noexcept {
        SizeT offset{0};
        return stringToNumber(number, content, offset, length);
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
                        }

                        if ((digit >= DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
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
                        number.Natural |= 0x8000000000000000LL;
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
                        (fraction_only ? (e_p10_power + (SizeT32(start_offset - dot_offset) - 1U))
                                       : (has_dot ? (SizeT32(offset - dot_offset) - 1U) : 0));

                    // const SizeT32 e_n10_power =
                    //     (fraction_only
                    //          ? ((tmp_offset - start_offset) + (SizeT32(start_offset - dot_offset) - 1U))
                    //          : (has_dot ? (SizeT32(offset - dot_offset) - 1U) : 0));

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

                            default: {
                                keep_going = false;
                            }
                        }
                    }
                    ///////////////////////////////////////
                    SizeT32 e_extra_p10_power = 0;

                    if (!fraction_only && (start_offset != offset)) {
                        if (!has_dot) {
                            e_extra_p10_power =
                                SizeT32((exp_offset == 0) ? (offset - start_offset) : (exp_offset - start_offset));
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
                    number.Natural |= 0x8000000000000000LL;
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
        BigInt<UNumber_T, 256U> b_int{number};
        //////////////////////////////////////////////////////////////
        SizeT32 shifted{exponent};
        //////////////////////////////////////////////////////////////
        shifted += 64U;
        b_int <<= 64U;
        //////////////////////////////////////////////////////////////
        constexpr bool is_size_8 = (sizeof(UNumber_T) == 8U);
        if QENTEM_CONST_EXPRESSION (is_size_8) {
            while (exponent >= DigitConst::MaxPowerOfFive) {
                // 2**126 = 85070591730234615865843651857942052864
                // 5**27 = 7450580596923828125 (MaxPowerOfFive)
                // 2**126 / 5**27 = 11417981541647679048.4
                // 126-64=62; See 2**126 and 64 shift

                b_int *= DigitConst::GetPowerOfOneOverFive(DigitConst::MaxPowerOfFive);
                b_int >>= DigitConst::MaxShift;
                shifted += DigitConst::GetPowerOfOneOverFiveShift(DigitConst::MaxPowerOfFive);
                exponent -= DigitConst::MaxPowerOfFive;
            }

            if (exponent != 0) {
                b_int *= DigitConst::GetPowerOfOneOverFive(exponent);
                b_int >>= DigitConst::MaxShift;
                shifted += DigitConst::GetPowerOfOneOverFiveShift(exponent);
            }
        } else {
            while (exponent >= DigitConst::MaxPowerOfFive) {
                b_int <<= DigitConst::MaxShift;
                b_int /= DigitConst::GetPowerOfFive(DigitConst::MaxPowerOfFive);
                shifted += DigitConst::MaxShift;
                exponent -= DigitConst::MaxPowerOfFive;
            }

            if (exponent != 0) {
                b_int <<= DigitConst::MaxShift;
                b_int /= DigitConst::GetPowerOfFive(exponent);
                shifted += DigitConst::MaxShift;
            }
        }
        //////////////////////////////////////////////////////////////
        SizeT32       exp = DigitUtils::RealNumberInfo<double, 8U>::Bias; // double only
        const SizeT32 bit = b_int.FindLastBit();

        // if (bit <= 52U) {
        //     number = SizeT64(b_int);
        //     number <<= (53U - bit);
        // } else {
        b_int >>= (bit - SizeT32{53});
        number = SizeT64(b_int);
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
        BigInt<UNumber_T, 256U> b_int{number};
        //////////////////////////////////////////////////////////////
        constexpr SizeT32 b_int_limit{2U};
        SizeT32           shifted{exponent};
        //////////////////////////////////////////////////////////////
        while (exponent >= DigitConst::MaxPowerOfFive) {
            b_int *= DigitConst::GetPowerOfFive(DigitConst::MaxPowerOfFive);

            if (b_int.Index() > b_int_limit) {
                b_int >>= DigitConst::MaxShift;
                shifted += DigitConst::MaxShift;
            }

            exponent -= DigitConst::MaxPowerOfFive;
        }

        if (exponent != 0) {
            b_int *= DigitConst::GetPowerOfFive(exponent);
        }
        //////////////////////////////////////////////////////////////
        const SizeT32 bit = b_int.FindLastBit();

        if (bit <= 52U) {
            number = SizeT64(b_int);
            number <<= (52U - bit);
        } else {
            b_int >>= (bit - 53U);
            number = SizeT64(b_int);
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

        if QENTEM_CONST_EXPRESSION (!Reverse_V_T) {
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
        constexpr SizeT32 number_size = sizeof(Number_T);

        using Info_T = DigitUtils::RealNumberInfo<Float_T, number_size>;
        // 4.9406564584124654e-324 needs about 1216 bits to store all its digits.
        using BigIntSys  = BigInt<SystemIntType, ((Info_T::Bias + 1U) + (number_size * 8U * 3U))>;
        using DigitConst = DigitUtils::DigitConst<BigIntSys::SizeOfType()>;

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

                BigIntSys b_int{mantissa};
                /////////////////////////////////////
                const SizeT32 first_shift      = Platform::FindFirstBit(mantissa);
                const int     exponent         = (int)((bias >> Info_T::MantissaSize) - Info_T::Bias);
                const bool    is_positive_exp  = (exponent >= 0);
                const SizeT32 positive_exp     = SizeT32(is_positive_exp ? exponent : -exponent);
                const SizeT32 first_bit        = (Info_T::MantissaSize - first_shift);
                const SizeT32 exp_actual_value = (positive_exp + ((bias == Number_T{0}) * first_bit));
                const SizeT32 digits           = (((exp_actual_value * 30103U) / 100000) + 1U);
                SizeT32       fraction_length  = 0;
                const bool    fixed =
                    ((format.Type == RealFormatType::SemiFixed) | (format.Type == RealFormatType::Fixed));
                const bool extra_digits = ((digits > format.Precision) & !fixed);
                const bool big_offset   = (positive_exp >= first_bit);
                const bool no_fraction  = (is_positive_exp & (big_offset | extra_digits));
                bool       round_up     = false;
                /////////////////////////////////////
                if (no_fraction) {
                    const SizeT32 drop    = ((!extra_digits) ? 0 : (digits - (format.Precision + 1U)));
                    const SizeT32 m_shift = (Info_T::MantissaSize + drop);

                    if (m_shift < positive_exp) {
                        b_int <<= (positive_exp - m_shift);
                    } else {
                        b_int >>= (m_shift - positive_exp);
                    }

                    if (drop != 0) {
                        round_up = true;
                        bigIntDropDigits(b_int, drop);
                    }
                } else {
                    SizeT32 shift   = 0;
                    SizeT32 needed  = 0;
                    fraction_length = first_bit;

                    if (is_positive_exp) {
                        fraction_length -= positive_exp;

                        if (fixed) {
                            needed = format.Precision;
                        } else {
                            needed = (format.Precision - digits);
                        }
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

                    b_int >>= first_shift;

                    SizeT32 times = fraction_length;

                    if (times >= DigitConst::MaxPowerOfFive) {
                        const SizeT32 max_index = (format.Precision < Info_T::MaxCut)
                                                      ? ((format.Precision / DigitConst::MaxPowerOfTen) + 2U)
                                                      : b_int.MaxIndex();

                        do {
                            b_int *= DigitConst::GetPowerOfFive(DigitConst::MaxPowerOfFive);

                            if ((b_int.Index() >= max_index) && (shift >= DigitConst::MaxShift)) {
                                b_int >>= DigitConst::MaxShift;
                                shift -= DigitConst::MaxShift;
                            }

                            times -= DigitConst::MaxPowerOfFive;
                        } while (times >= DigitConst::MaxPowerOfFive);
                    }

                    if (times != 0) {
                        b_int *= DigitConst::GetPowerOfFive(times);
                    }

                    b_int >>= shift;
                }

                const SizeT start_at = stream.Length();
                bigIntToString(stream, b_int);

                switch (format.Type) {
                    case RealFormatType::SemiFixed: {
                        formatStringNumberFixed<false>(stream, start_at, format.Precision, fraction_length, round_up);
                        break;
                    }

                    case RealFormatType::Fixed: {
                        formatStringNumberFixed<true>(stream, start_at, format.Precision, fraction_length, round_up);
                        break;
                    }

                    default: {
                        formatStringNumberDefault(stream, start_at, format.Precision, digits, fraction_length,
                                                  is_positive_exp, round_up);
                    }
                }

            } else {
                stream += DigitUtils::DigitChar::Zero;

                if (format.Type == RealFormatType::Fixed) {
                    stream += DigitUtils::DigitChar::Dot;
                    insertZerosLarge(stream, format.Precision);
                }
            }
        } else {
            using Char_T           = typename Stream_T::CharType;
            constexpr SizeT32 size = sizeof(Char_T);
            using DigitString      = DigitUtils::DigitString<Char_T, size>;

            if ((number & Info_T::MantissaMask) == Number_T{0}) {
                if ((number & Info_T::SignMask) != Number_T{0}) {
                    stream += DigitUtils::DigitChar::Negative;
                }

                stream.Write(DigitString::Infinity, DigitString::InfinityLength);
            } else {
                stream.Write(DigitString::NotANumber, DigitString::NotANumberLength);
            }
        }
    }

    template <typename Stream_T>
    static void insertPowerOfTen(Stream_T &stream, SizeT power_of_ten, bool positive) {
        stream += DigitUtils::DigitChar::E;
        stream += (positive ? DigitUtils::DigitChar::Positive : DigitUtils::DigitChar::Negative);

        if (power_of_ten < SizeT{10}) {
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

    template <typename Stream_T>
    inline static void insertZerosLarge(Stream_T &stream, SizeT32 length) {
        using Char_T           = typename Stream_T::CharType;
        constexpr SizeT32 size = sizeof(Char_T);
        using DigitString      = DigitUtils::DigitString<Char_T, size>;

        while (length > DigitString::ZerosLength) {
            stream.Write(DigitString::Zeros, SizeT(DigitString::ZerosLength));
            length -= DigitString::ZerosLength;
        }

        stream.Write(DigitString::Zeros, SizeT(length));
    }

    template <typename Stream_T, typename BigInt_T>
    static void bigIntToString(Stream_T &stream, BigInt_T &b_int) {
        using DigitConst = DigitUtils::DigitConst<BigInt_T::SizeOfType()>;

        while (b_int.IsBig()) {
            const SizeT length = stream.Length();
            NumberToString<true>(stream, b_int.Divide(DigitConst::MaxPowerOfTenValue));

            // dividing '1000000000000000000' by '1000000000' yield zeros remainder
            insertZeros(stream, (DigitConst::MaxPowerOfTen - SizeT(stream.Length() - length)));
        }

        if (b_int.NotZero()) {
            NumberToString<true>(stream, b_int.Number());
        }
    }

    template <typename BigInt_T>
    inline static void bigIntDropDigits(BigInt_T &b_int, SizeT32 drop) noexcept {
        using DigitConst = DigitUtils::DigitConst<BigInt_T::SizeOfType()>;

        while (drop >= DigitConst::MaxPowerOfFive) {
            b_int /= DigitConst::GetPowerOfFive(DigitConst::MaxPowerOfFive);
            drop -= DigitConst::MaxPowerOfFive;
        }

        if (drop != 0) {
            b_int /= DigitConst::GetPowerOfFive(drop);
        }
    }

    template <typename Stream_T>
    static void formatStringNumberDefault(Stream_T &stream, const SizeT started_at, const SizeT32 precision,
                                          const SizeT32 calculated_digits, SizeT32 fraction_length,
                                          const bool is_positive_exp, const bool round_up) {
        using Char_T                = typename Stream_T::CharType;
        Char_T     *storage         = stream.Storage();
        const SizeT number_length   = (stream.Length() - started_at);
        SizeT       index           = started_at;
        SizeT       power           = SizeT{0};
        bool        power_increased = false;
        /////////////////////////////////////////////////////
        if (number_length > precision) {
            --index;
            index += SizeT(number_length - precision);

            roundStringNumber(stream, index, power_increased, round_up);

            if (is_positive_exp) {
                const SizeT diff = SizeT(
                    ((number_length - fraction_length) +
                     ((calculated_digits <= precision) ? SizeT{0} : (calculated_digits - (precision + SizeT{1})))) -
                    SizeT(!power_increased));

                if (diff >= precision) {
                    Char_T       *number = (storage + index);
                    const Char_T *last   = stream.Last();

                    while ((number < last) && (*number == DigitUtils::DigitChar::Zero)) {
                        ++number;
                        ++index;
                    }

                    power           = diff;
                    fraction_length = SizeT{0};
                }
            }
        }

        if (fraction_length != 0) {
            Char_T       *number        = (storage + index);
            const Char_T *last          = stream.Last();
            const SizeT   dot_index     = SizeT(started_at + fraction_length);
            const bool    fraction_only = (number_length <= fraction_length);

            while ((number < last) && (*number == DigitUtils::DigitChar::Zero)) {
                ++number;
                ++index;
            }

            if (fraction_only) {
                const SizeT diff =
                    SizeT((fraction_length > number_length) ? (fraction_length - number_length) : SizeT{0});

                if (!power_increased) {
                    if (diff < SizeT{4}) {
                        insertZeros(stream, diff);
                        stream += DigitUtils::DigitChar::Dot;
                        stream += DigitUtils::DigitChar::Zero;
                    } else {
                        power = diff;
                        ++power;
                    }
                } else if ((diff != SizeT{0}) && (diff < SizeT{5})) {
                    insertZeros(stream, (diff - SizeT{1}));
                    stream += DigitUtils::DigitChar::Dot;
                    stream += DigitUtils::DigitChar::Zero;
                } else {
                    power = diff;
                }
            } else if (index < dot_index) {
                stream.InsertAt(DigitUtils::DigitChar::Dot, dot_index);
            } else if (power_increased) {
                SizeT zeros = SizeT(number_length - fraction_length);

                do {
                    --index;
                    storage[index] = DigitUtils::DigitChar::Zero;
                } while (--zeros != 0);
            }
        }

        stream.Reverse(started_at);
        stream.StepBack(index - started_at);

        if (power != SizeT{0}) {
            stream.InsertAt(DigitUtils::DigitChar::Dot, (started_at + SizeT{1}));
            insertPowerOfTen(stream, power, is_positive_exp);
        }
    }

    template <bool Fixed_T, typename Stream_T>
    static void formatStringNumberFixed(Stream_T &stream, const SizeT started_at, const SizeT32 precision,
                                        const SizeT32 fraction_length, const bool round_up) {
        using Char_T              = typename Stream_T::CharType;
        Char_T     *storage       = stream.Storage();
        const SizeT number_length = (stream.Length() - started_at);
        SizeT       index         = started_at;
        const SizeT dot_index     = SizeT(started_at + fraction_length);
        SizeT32     diff          = ((fraction_length > number_length) ? SizeT32(fraction_length - number_length) : 0);
        bool        power_increased = false;
        const bool  fraction_only   = (number_length <= fraction_length);
        /////////////////////////////////////////////////////
        if (fraction_length != 0) {
            if (diff <= precision) {
                if (fraction_length > precision) {
                    index += SizeT(fraction_length - (precision + SizeT{1}));
                    roundStringNumber(stream, index, power_increased, (round_up | (diff != 0)));

                    Char_T       *number = (storage + index);
                    const Char_T *last   = stream.Last();

                    while ((number < last) && (*number == DigitUtils::DigitChar::Zero)) {
                        ++number;
                        ++index;
                    }
                }

                if (fraction_only) {
                    if ((index < stream.Length()) || power_increased) {
                        if (diff != 0) {
                            if (power_increased) {
                                index          = (index - SizeT(index == stream.Length()));
                                storage[index] = DigitUtils::DigitChar::One;
                            }

                            diff -= SizeT32(power_increased);
                            insertZerosLarge(stream, diff);
                            stream += DigitUtils::DigitChar::Dot;
                            stream += DigitUtils::DigitChar::Zero;
                        } else if (!power_increased) {
                            stream += DigitUtils::DigitChar::Dot;
                            stream += DigitUtils::DigitChar::Zero;
                        }
                    } else {
                        --index;
                        storage[index] = DigitUtils::DigitChar::Zero;
                    }
                } else if (index < dot_index) {
                    stream.InsertAt(DigitUtils::DigitChar::Dot, dot_index);
                } else if (power_increased) {
                    SizeT zeros = SizeT(number_length - fraction_length);

                    do {
                        --index;
                        storage[index] = DigitUtils::DigitChar::Zero;
                    } while (--zeros != 0);
                }
            } else {
                index          = index + (number_length - SizeT{1});
                storage[index] = DigitUtils::DigitChar::Zero;
            }
        }

        stream.Reverse(started_at);
        stream.StepBack(index - started_at);

        if QENTEM_CONST_EXPRESSION (Fixed_T) {
            if ((dot_index == index) || ((stream.Length() - started_at) == SizeT{1}) ||
                (!fraction_only && power_increased)) {
                stream += DigitUtils::DigitChar::Dot;
                insertZerosLarge(stream, precision);
            } else if (fraction_only) {
                insertZerosLarge(
                    stream, SizeT32(precision -
                                    SizeT32(stream.Length() - (started_at + SizeT{2})))); // 2 is the length of '0.'.
            } else {
                insertZerosLarge(stream, SizeT32(precision - (dot_index - index)));
            }
        }
    }

    template <typename Stream_T>
    static void roundStringNumber(Stream_T &stream, SizeT &index, bool &power_increased, bool round_up) noexcept {
        using Char_T = typename Stream_T::CharType;

        const Char_T *last   = stream.Last();
        Char_T       *number = (stream.Storage() + index);

        ++index;

        const bool round =
            (((*number > DigitUtils::DigitChar::Five) ||
              ((*number == DigitUtils::DigitChar::Five) &&
               (round_up || ((SizeT32(stream.First()[index] - DigitUtils::DigitChar::Zero) & 1U) == 1U)))));

        if (round) {
            ++number;

            while ((number < last) && (*number == DigitUtils::DigitChar::Nine)) {
                ++index;
                ++number;
            }

            if ((number > last) || (*number == DigitUtils::DigitChar::Nine)) {
                power_increased         = true;
                stream.Storage()[index] = DigitUtils::DigitChar::One;
            } else {
                ++(*number);
            }
        }
    }
};

} // namespace Qentem

#endif
