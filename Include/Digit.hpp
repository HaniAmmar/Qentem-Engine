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

#ifndef QENTEM_DIGIT_H_
#define QENTEM_DIGIT_H_

/*
 * For converting numbers from and to strings.
 */
namespace Qentem {
struct Digit {
    template <bool Reverse_V_ = false, typename Stream_T_, typename Number_T_>
    inline static void NumberToString(Stream_T_ &stream, Number_T_ number) {
        using Char_T_ = typename Stream_T_::CharType;

        static constexpr unsigned int max_number_of_digits = (((sizeof(Number_T_) * 8U * 30103U) / 100000U) + 1U);
        // static constexpr bool         is_signed            = ((Number_T_{0} - 1) < 0);

        if (number < 0) {
            number *= (Number_T_{0} - 1);
            stream += DigitUtils::DigitChars::NegativeChar;
        }

        Char_T_ storage[max_number_of_digits];
        SizeT   offset = max_number_of_digits;

        RawIntToString<Reverse_V_>(&(storage[0]), offset, number);

        if (!Reverse_V_) {
            stream.Write(&(storage[offset]), (max_number_of_digits - offset));
        } else {
            stream.Write(&(storage[0U]), offset);
        }
    }

    template <bool Reverse_V_ = false, typename Char_T_, typename Number_T_>
    static void RawIntToString(Char_T_ *storage, SizeT &offset, Number_T_ number) {
        intToString<Reverse_V_>(storage, offset, number);
    }

    template <typename Stream_T_>
    inline static void NumberToString(Stream_T_ &stream, double number, unsigned int precision = 15U) {
        realToString(stream, number, precision);
    }

    template <typename Stream_T_>
    inline static void NumberToString(Stream_T_ &stream, float f_number, unsigned int precision = 15U) {
        realToString(stream, static_cast<double>(f_number), precision);
    }
    /////////////////////////////////////////////////////////////////
    template <typename Char_T_>
    static unsigned int HexStringToNumber(const Char_T_ *value, const SizeT length) noexcept {
        unsigned int number = 0;
        unsigned int offset = 0;

        while (offset < length) {
            const Char_T_ digit = value[offset];
            number <<= 4U;

            if ((digit >= DigitUtils::DigitChars::OneChar) && (digit <= DigitUtils::DigitChars::NineChar)) {
                // No need for 0
                number |= (static_cast<unsigned int>(digit - DigitUtils::DigitChars::ZeroChar)); // 1-9
            } else if ((digit >= DigitUtils::DigitChars::UA_Char) &&
                       (digit <= DigitUtils::DigitChars::UF_Char)) { // A-F
                number |= (static_cast<unsigned int>(digit - DigitUtils::DigitChars::SevenChar));
            } else if ((digit >= DigitUtils::DigitChars::A_Char) && (digit <= DigitUtils::DigitChars::F_Char)) { // a-f
                number |= (static_cast<unsigned int>(digit - DigitUtils::DigitChars::UW_Char));
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
            number += (static_cast<Number_T_>(content[offset] - DigitUtils::DigitChars::ZeroChar));
            ++offset;

            while (offset < length) {
                number *= Number_T_{10};
                number += (static_cast<Number_T_>(content[offset] - DigitUtils::DigitChars::ZeroChar));
                ++offset;
            }
        }
    }

    template <typename Char_T_>
    static unsigned int StringToNumber(QNumber &number, const Char_T_ *content, SizeT &offset,
                                       SizeT end_offset) noexcept {
        SizeT        o_offset = offset;
        SizeT        max_end_offset;
        unsigned int flags    = 0;
        unsigned int exponent = 0;
        number.Natural        = 0;

        if (offset < end_offset) {
            Char_T_ digit = content[offset];

            if (digit == DigitUtils::DigitChars::NegativeChar) {
                ++offset;
                flags |= stringToNumberFlags::Negative;

            } else if (digit == DigitUtils::DigitChars::PositiveChar) {
                ++offset;
            }

            if ((end_offset - offset) < SizeT{19}) {
                max_end_offset = end_offset;
            } else {
                max_end_offset = (offset + SizeT{19});

                if ((flags & stringToNumberFlags::Negative) != 0) {
                    --max_end_offset;
                }

                flags |= stringToNumberFlags::OverFlow;
            }

            if (offset < max_end_offset) {
                digit = content[offset];

                if ((digit == DigitUtils::DigitChars::ZeroChar) && ((max_end_offset - offset) > SizeT{1})) {
                    digit = content[offset + SizeT{1}];

                    if ((digit >= DigitUtils::DigitChars::ZeroChar) && (digit <= DigitUtils::DigitChars::NineChar)) {
                        return 0U; // Leading zero.
                    }
                }

                while (true) {
                    while (offset < max_end_offset) {
                        digit = content[offset];

                        if ((digit >= DigitUtils::DigitChars::ZeroChar) &&
                            (digit <= DigitUtils::DigitChars::NineChar)) {
                            number.Natural *= 10ULL;
                            number.Natural +=
                                (static_cast<unsigned long long>(digit - DigitUtils::DigitChars::ZeroChar));
                            ++offset;
                            continue;
                        }

                        break;
                    }

                    switch (digit) {
                        case DigitUtils::DigitChars::DotChar: {
                            if ((flags & stringToNumberFlags::Real) == 0U) {
                                flags |= stringToNumberFlags::Real;
                                ++offset;
                                ++o_offset;

                                exponent = static_cast<unsigned int>(offset);

                                if (offset == max_end_offset) {
                                    digit = 0;
                                }

                                continue;
                            }

                            return 0;
                        }

                        case DigitUtils::DigitChars::UE_Char:
                        case DigitUtils::DigitChars::E_Char: {
                            if (number.Natural != 0) {
                                if ((flags & stringToNumberFlags::Real) != 0U) {
                                    exponent = (static_cast<unsigned int>(offset) - exponent);
                                }

                                if ((offset < end_offset) &&
                                    exponentToNumber(exponent, flags, content, offset, end_offset)) {
                                    digit          = 0;
                                    max_end_offset = offset;
                                    continue;
                                }
                            }

                            return 0;
                        }

                        default: {
                            if (o_offset != offset) {
                                if ((offset < max_end_offset) || ((flags & stringToNumberFlags::OverFlow) == 0U)) {
                                    if ((flags & stringToNumberFlags::Real) == 0U) {
                                        if ((flags & stringToNumberFlags::Negative) == 0U) {
                                            return 1U;
                                        }

                                        if (number.Integer != 0) {
                                            number.Integer = -number.Integer;
                                            return 2U;
                                        }
                                    }

                                    number.Real = static_cast<double>(number.Natural);

                                    if ((flags & stringToNumberFlags::MergedExponent) == 0U) {
                                        exponent = (static_cast<unsigned int>(offset) - exponent);
                                        flags |= stringToNumberFlags::NegativeExponent;
                                    }

                                    if (((flags & stringToNumberFlags::NegativeExponent) != 0U)) {
                                        powerOfNegativeTen(number.Real, exponent);
                                    } else {
                                        powerOfTen(number.Real, exponent);
                                    }

                                    if ((flags & stringToNumberFlags::Negative) != 0) {
                                        number.Real = -number.Real;
                                    }

                                    return 3U;
                                }

                                return roundNumber(number, exponent, flags, content, offset, end_offset);
                            }

                            return 0U;
                        }
                    }
                }
            }
        }

        return 0U;
    }

  private:
    //////////// Private ////////////

    struct stringToNumberFlags {
        static constexpr unsigned int Real             = 1U;
        static constexpr unsigned int Negative         = 2U;
        static constexpr unsigned int NegativeExponent = 4U;
        static constexpr unsigned int MergedExponent   = 8U;
        static constexpr unsigned int OverFlow         = 16U;
    };

    template <bool Reverse_V_ = false, typename Char_T_, typename Number_T_>
    QENTEM_NOINLINE static void intToString(Char_T_ *storage, SizeT &offset, Number_T_ number) {
        if (!Reverse_V_) {
            const SizeT o_offset = offset;

            while (number >= Number_T_{10}) {
                const SizeT index = (static_cast<SizeT>(number % Number_T_{100}) * SizeT{2});
                number /= Number_T_{100};

                --offset;
                storage[offset] = static_cast<Char_T_>(DigitUtils::DigitTable[index + SizeT{1}]);
                --offset;
                storage[offset] = static_cast<Char_T_>(DigitUtils::DigitTable[index]);
            }

            if ((number != 0) || (offset == o_offset)) {
                --offset;
                storage[offset] = (static_cast<Char_T_>(number) + DigitUtils::DigitChars::ZeroChar);
            }
        } else {
            offset = 0;

            while (number >= Number_T_{10}) {
                const SizeT index = (static_cast<SizeT>(number % Number_T_{100}) * SizeT{2});
                number /= Number_T_{100};

                storage[offset] = static_cast<Char_T_>(DigitUtils::DigitTable[index + SizeT{1}]);
                ++offset;
                storage[offset] = static_cast<Char_T_>(DigitUtils::DigitTable[index]);
                ++offset;
            }

            if ((number != 0) || (offset == SizeT{0})) {
                storage[offset] = (static_cast<Char_T_>(number) + DigitUtils::DigitChars::ZeroChar);
                ++offset;
            }
        }
    }

    template <typename Char_T_>
    QENTEM_NOINLINE static int exponentToNumber(unsigned int &exponent, unsigned int &flags, const Char_T_ *content,
                                                SizeT &offset, SizeT end_offset) noexcept {
        flags |= stringToNumberFlags::Real;
        flags |= stringToNumberFlags::MergedExponent;

        ++offset;
        Char_T_ digit = content[offset];

        switch (digit) {
            case DigitUtils::DigitChars::NegativeChar: {
                flags |= stringToNumberFlags::NegativeExponent;
                ++offset;
                break;
            }

            case DigitUtils::DigitChars::PositiveChar: {
                ++offset;
                break;
            }

            default: {
                if ((digit < DigitUtils::DigitChars::ZeroChar) || (digit > DigitUtils::DigitChars::NineChar)) {
                    return false;
                }
            }
        }

        const SizeT  sci_offset   = offset;
        unsigned int sci_exponent = 0U;

        while (offset < end_offset) {
            digit = content[offset];

            if ((digit >= DigitUtils::DigitChars::ZeroChar) && (digit <= DigitUtils::DigitChars::NineChar)) {
                sci_exponent *= 10ULL;
                sci_exponent += (static_cast<unsigned int>(digit - DigitUtils::DigitChars::ZeroChar));
                ++offset;
                continue;
            }

            break;
        }

        if ((flags & stringToNumberFlags::NegativeExponent) == 0U) {
            if (exponent <= sci_exponent) {
                exponent = (sci_exponent - exponent);
            } else {
                exponent = (exponent - sci_exponent);
                flags |= stringToNumberFlags::NegativeExponent;
            }
        } else {
            exponent += sci_exponent;
        }

        return (sci_offset != offset);
    }

    template <typename Char_T_>
    QENTEM_NOINLINE static unsigned int roundNumber(QNumber &number, unsigned int &exponent, unsigned int &flags,
                                                    const Char_T_ *content, SizeT &offset, SizeT end_offset) {
        const SizeT  last_offset   = offset;
        unsigned int exponent_diff = 0U;
        Char_T_      digit         = 0;
        bool         keep_on       = true;
        bool         is_diff_set   = false;

        while ((offset < end_offset) && keep_on) {
            do {
                digit = content[offset];

                if ((digit >= DigitUtils::DigitChars::ZeroChar) && (digit <= DigitUtils::DigitChars::NineChar)) {
                    ++offset;
                    continue;
                }

                break;
            } while (offset < end_offset);

            switch (digit) {
                case DigitUtils::DigitChars::DotChar: {
                    exponent_diff = static_cast<unsigned int>(offset - last_offset);
                    is_diff_set   = true;

                    ++offset;
                    continue;
                }

                case DigitUtils::DigitChars::E_Char:
                case DigitUtils::DigitChars::UE_Char: {
                    if ((flags & stringToNumberFlags::Real) == 0U) {
                        flags |= stringToNumberFlags::Real;

                        if (!is_diff_set) {
                            exponent_diff = static_cast<unsigned int>(offset - last_offset);
                        }
                    } else {
                        exponent = (static_cast<unsigned int>(last_offset) - exponent);
                    }

                    if ((offset < end_offset) && exponentToNumber(exponent, flags, content, offset, end_offset)) {
                        keep_on = false;
                        continue;
                    }

                    return 0U;
                }

                default: {
                    if (((flags & stringToNumberFlags::Real) == 0U) && !is_diff_set) {
                        exponent_diff = static_cast<unsigned int>(offset - last_offset);
                    }

                    keep_on = false;
                }
            }
        }

        if ((flags & stringToNumberFlags::Real) == 0U) {
            if ((exponent_diff == 0U) || ((exponent_diff == 1U) && (number.Natural < 0x2000000000000000ULL))) {
                digit                  = content[last_offset];
                unsigned long long tmp = number.Natural;

                if ((digit >= DigitUtils::DigitChars::ZeroChar) && (digit <= DigitUtils::DigitChars::NineChar)) {
                    tmp *= 10U;
                    tmp += (static_cast<unsigned long long>(digit - DigitUtils::DigitChars::ZeroChar));

                    if ((flags & stringToNumberFlags::Negative) != 0) {
                        tmp &= 0x7FFFFFFFFFFFFFFFULL;
                    }

                    if (tmp > number.Natural) {
                        number.Natural = tmp;
                    } else {
                        flags |= stringToNumberFlags::Real;
                    }
                }
            } else {
                flags |= stringToNumberFlags::Real;
            }
        }

        if ((flags & stringToNumberFlags::Real) != 0U) {
            if (exponent != 0 && ((flags & stringToNumberFlags::MergedExponent) == 0U)) {
                exponent = (static_cast<unsigned int>(last_offset) - exponent);
                flags |= stringToNumberFlags::NegativeExponent;
            }

            if (exponent_diff != 0) {
                if ((flags & stringToNumberFlags::NegativeExponent) == 0U) {
                    exponent += exponent_diff;
                } else {
                    if (exponent < exponent_diff) {
                        exponent = static_cast<unsigned int>(exponent_diff - exponent);
                        flags ^= stringToNumberFlags::NegativeExponent;
                    } else {
                        exponent -= exponent_diff;
                    }
                }
            }

            number.Real = static_cast<double>(number.Natural);

            if (((flags & stringToNumberFlags::NegativeExponent) != 0U)) {
                powerOfNegativeTen(number.Real, exponent);
            } else {
                powerOfTen(number.Real, exponent);
            }

            if ((flags & stringToNumberFlags::Negative) != 0U) {
                number.Real = -number.Real;
            }

            if (number.Natural != 9218868437227405312ULL) {
                return 3U;
            }

            return 0U;
        }

        if ((flags & stringToNumberFlags::Negative) == 0U) {
            return 1U;
        }

        if (number.Integer != 0) {
            number.Integer = -number.Integer;
            return 2U;
        }

        return 0U;
    }

    static void powerOfTen(double &number, unsigned int exponent) {
        while (exponent >= 100U) {
            number *= 1e100;
            exponent -= 100U;
        }

        while (exponent >= 10U) {
            number *= 1e10;
            exponent -= 10U;
        }

        while (exponent > 0U) {
            number *= 10U;
            --exponent;
        }
    }

    static void powerOfNegativeTen(double &number, unsigned int exponent) {
        while (exponent >= 100U) {
            number /= 1e100;
            exponent -= 100U;
        }

        while (exponent >= 10U) {
            number /= 1e10;
            exponent -= 10U;
        }

        while (exponent > 0U) {
            number /= 10U;
            --exponent;
        }
    }

    // ------------------------------------------------------
    // struct RealFormatType {
    // static constexpr unsigned int Default = 0U;
    // static constexpr unsigned int Fixed      = 1U;
    // static constexpr unsigned int Scientific = 2U;
    // };

    // struct RealFormatInfo {
    //     RealFormatInfo() noexcept : Precision{6U}, Type{RealFormatType::Default} {
    //     }

    //     RealFormatInfo(unsigned int precision, unsigned int type) noexcept : Precision{precision}, Type{type} {
    //     }

    //     unsigned int Precision;
    //     unsigned int Type;
    // };

    template <typename Stream_T_, typename Number_T_>
    static void realToString(Stream_T_ &stream, const Number_T_ number, const unsigned int precision) noexcept {
        using Char_T_ = typename Stream_T_::CharType;
        using Info_T  = DigitUtils::RealNumberInfo<Number_T_>;
        const Info_T info{number};

        using UNumber_T = decltype(info.NaturalNumber);
        BigInt<UNumber_T, ((Info_T::Bias + 1U) + (sizeof(UNumber_T) * 8U * 3))> b_int{info.NaturalNumber &
                                                                                      Info_T::MantissaMask};

        const UNumber_T bias = (info.NaturalNumber & Info_T::ExponentMask);

        if (bias != Info_T::ExponentMask) {
            UNumber_T &mantissa = b_int.GetBucket(0U);

            if (info.NaturalNumber & Info_T::SignMask) {
                stream += DigitUtils::DigitChars::NegativeChar;
            }

            if ((mantissa != UNumber_T{0}) || (bias != UNumber_T{0})) {
                if (bias != UNumber_T{0}) {
                    mantissa |= Info_T::LeadingBit;
                } else {
                    mantissa <<= 1U;
                }
                /////////////////////////////////////
                const unsigned int first_shift      = Platform::CTZ(mantissa);
                const int          exponent         = static_cast<int>((bias >> Info_T::MantissaSize) - Info_T::Bias);
                const bool         is_positive_exp  = (exponent >= 0);
                const unsigned int positive_exp     = static_cast<unsigned int>(is_positive_exp ? exponent : -exponent);
                const unsigned int first_bit        = (Info_T::MantissaSize - first_shift);
                const unsigned int exp_actual_value = (positive_exp + ((bias == UNumber_T{0}) * first_bit));
                const unsigned int digits           = (((exp_actual_value * 30103U) / 100000U) + 1U);
                unsigned int       fraction_length  = 0;
                const bool         extra_digits     = (digits > precision);
                bool               round_up         = false;
                const bool         big_offset       = (positive_exp >= first_bit);
                const bool         no_fraction      = (is_positive_exp && (big_offset || extra_digits));
                /////////////////////////////////////
                if (no_fraction) {
                    const unsigned int drop    = (!extra_digits) ? 0 : (digits - (precision + 1U));
                    const unsigned int m_shift = (Info_T::MantissaSize + drop);

                    if (m_shift < positive_exp) {
                        b_int <<= (positive_exp - m_shift);
                    } else {
                        mantissa >>= (m_shift - positive_exp);
                    }

                    if (drop != 0U) {
                        round_up = true;
                        bigIntDropDigits<Number_T_>(b_int, drop);
                    }
                } else {
                    unsigned int shift  = 0U;
                    unsigned int needed = 0U;
                    fraction_length     = first_bit;

                    if (is_positive_exp) {
                        fraction_length -= positive_exp;

                        if (extra_digits) {
                            needed = (digits - precision);
                        } else {
                            needed = (precision - digits);
                        }
                    } else {
                        fraction_length += positive_exp;

                        needed = (digits + precision);
                    }

                    ++needed; // For rounding.

                    if (fraction_length > needed) {
                        shift           = (fraction_length - needed);
                        fraction_length = needed;
                        round_up        = true;
                    }

                    mantissa >>= first_shift;

                    unsigned int times = fraction_length;

                    if (times >= Info_T::MaxPowerOfFiveDrop) {
                        const unsigned int max_index = (precision < Info_T::MaxCut)
                                                           ? ((precision / Info_T::PowerOfTenDigits) + 2U)
                                                           : b_int.MaxIndex;

                        do {
                            b_int.MultiplyBy(Info_T::GetPowerOfFive(Info_T::MaxPowerOfFiveDrop));
                            times -= Info_T::MaxPowerOfFiveDrop;

                            if ((b_int.GetIndex() >= max_index) && (shift >= Info_T::MaxPowerOfFiveShift)) {
                                b_int.ShiftRight(Info_T::MaxPowerOfFiveShift);
                                shift -= Info_T::MaxPowerOfFiveShift;
                            }
                        } while (times >= Info_T::MaxPowerOfFiveDrop);
                    }

                    if (times != 0U) {
                        b_int.MultiplyBy(Info_T::GetPowerOfFive(times));
                    }

                    b_int.ShiftRight(shift);
                }

                const SizeT start_at = stream.Length();
                bigIntToString<Number_T_>(stream, b_int);
                formatStringNumber(stream, start_at, precision, digits, fraction_length, is_positive_exp, round_up);
            } else {
                stream += DigitUtils::DigitChars::ZeroChar;
            }
        } else {
            static constexpr int size = static_cast<int>(sizeof(Char_T_));

            if ((info.NaturalNumber & Info_T::MantissaMask) == UNumber_T{0}) {
                if (info.NaturalNumber & Info_T::SignMask) {
                    stream += DigitUtils::DigitChars::NegativeChar;
                }

                stream.Write(DigitUtils::DigitStrings<Char_T_, size>::InfinityString, SizeT{3});
            } else {
                stream.Write(DigitUtils::DigitStrings<Char_T_, size>::NANString, SizeT{3});
            }
        }
    }

    template <typename Stream_T_, typename Number_T_>
    static void insertPowerOfTen(Stream_T_ &stream, Number_T_ power_of_ten, bool positive) {
        stream += DigitUtils::DigitChars::E_Char;
        stream += (positive ? DigitUtils::DigitChars::PositiveChar : DigitUtils::DigitChars::NegativeChar);

        if (power_of_ten < 10U) {
            // e+01,e+09
            stream += DigitUtils::DigitChars::ZeroChar;
        }

        NumberToString(stream, power_of_ten);
    }

    template <typename Stream_T_, typename Number_T_>
    static void insertZeros(Stream_T_ &stream, const Number_T_ length) noexcept {
        using Char_T_             = typename Stream_T_::CharType;
        static constexpr int size = static_cast<int>(sizeof(Char_T_));
        stream.Write(DigitUtils::DigitStrings<Char_T_, size>::ZeroesString, length);
    }

    template <typename Number_T_, typename Stream_T_, typename BigInt_T_>
    static void bigIntToString(Stream_T_ &stream, BigInt_T_ &b_int) noexcept {
        using Info_T = DigitUtils::RealNumberInfo<Number_T_>;

        while (b_int.IsBig()) {
            const SizeT length = stream.Length();
            NumberToString<true>(stream, b_int.DivideBy(Info_T::MaxPowerOfTenValue));

            // dividing '1000000000000000000' by '1000000000' yield zeros remainder
            insertZeros(stream, (Info_T::PowerOfTenDigits - (stream.Length() - length)));
        }

        if (b_int.NotZero()) {
            NumberToString<true>(stream, b_int.GetNumber());
        }
    }

    template <typename Number_T_, typename BigInt_T_>
    static void bigIntDropDigits(BigInt_T_ &b_int, unsigned int drop) noexcept {
        using Info_T = DigitUtils::RealNumberInfo<Number_T_>;

        while (drop >= Info_T::MaxPowerOfFiveDrop) {
            b_int.DivideBy(Info_T::GetPowerOfFive(Info_T::MaxPowerOfFiveDrop));
            drop -= Info_T::MaxPowerOfFiveDrop;
        }

        if (drop != 0U) {
            b_int.DivideBy(Info_T::GetPowerOfFive(drop));
        }
    }

    template <typename Stream_T_>
    static void formatStringNumber(Stream_T_ &stream, const SizeT started_at, const unsigned int precision,
                                   const unsigned int calculated_digits, unsigned int fraction_length,
                                   const bool is_positive_exp, const bool round_up) noexcept {
        using Char_T_                    = typename Stream_T_::CharType;
        const SizeT        stream_length = (stream.Length() - started_at);
        SizeT              index         = started_at;
        SizeT              power;
        SizeT              length;
        const unsigned int precision_plus_one = (precision + 1U);

        if (is_positive_exp) {
            length = stream_length +
                     ((calculated_digits > precision_plus_one) ? (calculated_digits - precision_plus_one) : 0) -
                     fraction_length;
            power = (length - SizeT{1});
        } else {
            length = stream_length;
            power  = (((fraction_length > stream_length) ? (fraction_length - stream_length) : SizeT{0}) + SizeT{1});
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
                    fraction_length = 0;
                    ++power;

                    if (power < precision) {
                        SizeT tmp = power;

                        do {
                            --index;
                            stream.Storage()[index] = DigitUtils::DigitChars::ZeroChar;
                        } while (--tmp != 0);
                    }
                } else {
                    --power;
                }
            }
        }
        /////////////////////////////////////////////////////
        const bool display_exp =
            ((is_positive_exp && ((power + 1U) > precision)) || (!is_positive_exp && (power > 4U)));
        /////////////////////////////////////////////////////
        Char_T_       *number = (stream.Storage() + index);
        const Char_T_ *last   = stream.Last();

        if (display_exp || (fraction_length != 0)) {
            while ((number < last) && (*number == DigitUtils::DigitChars::ZeroChar)) {
                ++number;
                ++index;
            }
        }

        const auto index2 = (index - started_at);

        if (!display_exp) {
            if ((fraction_length != 0) && (fraction_length > index2)) {
                if (!is_positive_exp && (power != 0)) {
                    insertZeros(stream, (power - 1U));
                    stream += DigitUtils::DigitChars::DotChar;
                    stream += DigitUtils::DigitChars::ZeroChar;
                } else if ((stream_length - SizeT{1}) != index2) {
                    stream.InsertAt(DigitUtils::DigitChars::DotChar, (fraction_length + started_at));
                }
            }
        } else if ((stream_length - SizeT{1}) != index2) {
            stream.InsertAt(DigitUtils::DigitChars::DotChar, (stream.Length() - 1U));
        }

        stream.Reverse(started_at);
        stream.StepBack(index - started_at);

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
            ((number < last) &&
             ((*number > DigitUtils::DigitChars::FiveChar) ||
              ((*number == DigitUtils::DigitChars::FiveChar) &&
               (round_up ||
                (static_cast<unsigned int>(stream.Storage()[index] - DigitUtils::DigitChars::ZeroChar) & 1U)))));

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
