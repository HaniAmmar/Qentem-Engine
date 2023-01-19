/*
 * Copyright (c) 2020 Hani Ammar
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

#include "String.hpp"

#ifndef QENTEM_DIGIT_H_
#define QENTEM_DIGIT_H_

namespace Qentem {

/*
 * For converting numbers from and to strings.
 */

template <typename Char_T_>
struct Digit {
    template <typename Stream_T_>
    inline static void NumberToString(Stream_T_ &stream, unsigned long long number) {
        unsignedIntToString(stream, number);
    }

    template <typename Stream_T_>
    inline static void NumberToString(Stream_T_ &stream, long long number) {
        signedIntToString(stream, number);
    }

    template <typename Number_T_>
    inline static String<Char_T_> NumberToString(Number_T_ number) {
        String<Char_T_> str;
        NumberToString(str, number);
        return str;
    }

    template <typename Stream_T_>
    inline static void NumberToString(Stream_T_ &stream, unsigned short number) {
        NumberToString(stream, static_cast<unsigned long long>(number));
    }

    template <typename Stream_T_>
    inline static void NumberToString(Stream_T_ &stream, unsigned int number) {
        NumberToString(stream, static_cast<unsigned long long>(number));
    }

    template <typename Stream_T_>
    inline static void NumberToString(Stream_T_ &stream, unsigned long number) {
        NumberToString(stream, static_cast<unsigned long long>(number));
    }

    template <typename Stream_T_>
    inline static void NumberToString(Stream_T_ &stream, short number) {
        NumberToString(stream, static_cast<long long>(number));
    }

    template <typename Stream_T_>
    inline static void NumberToString(Stream_T_ &stream, int number) {
        NumberToString(stream, static_cast<long long>(number));
    }

    template <typename Stream_T_>
    inline static void NumberToString(Stream_T_ &stream, long number) {
        NumberToString(stream, static_cast<long long>(number));
    }

    /*
     * "min" is the minimum digits to the left of the pieried. Zeros will be
     * added if the digits on the left are less than "min". "r_min" is the same
     * as "min" but for the right side.
     */
    inline static String<Char_T_> NumberToString(double number, SizeT min = 1, SizeT r_min = 0,
                                                 unsigned int precision = 0) {
        String<Char_T_> str;
        doubleToString(str, number, min, r_min, precision);
        return str;
    }

    inline static String<Char_T_> NumberToString(float f_number, SizeT min = 1, SizeT r_min = 0,
                                                 unsigned int precision = 0) {
        String<Char_T_> str;
        doubleToString(str, static_cast<double>(f_number), min, r_min, precision);
        return str;
    }

    template <typename Stream_T_>
    inline static void NumberToString(Stream_T_ &stream, double number, SizeT min = 1, SizeT r_min = 0,
                                      unsigned int precision = 0) {
        doubleToString(stream, number, min, r_min, precision);
    }

    template <typename Stream_T_>
    inline static void NumberToString(Stream_T_ &stream, float f_number, SizeT min = 1, SizeT r_min = 0,
                                      unsigned int precision = 0) {
        doubleToString(stream, static_cast<double>(f_number), min, r_min, precision);
    }

    /////////////////////////////////////////////////////////////////

    static unsigned int HexStringToNumber(const Char_T_ *value, const SizeT length) noexcept {
        unsigned int number = 0;
        unsigned int offset = 0;

        while (offset < length) {
            const Char_T_ digit = value[offset];
            number <<= 4U;

            if ((digit >= DigitChars::OneChar) && (digit <= DigitChars::NineChar)) {
                // No need for 0
                number |= (static_cast<unsigned int>(digit - DigitChars::ZeroChar));       // 1-9
            } else if ((digit >= DigitChars::UA_Char) && (digit <= DigitChars::UF_Char)) { // A-F
                number |= (static_cast<unsigned int>(digit - DigitChars::SevenChar));
            } else if ((digit >= DigitChars::A_Char) && (digit <= DigitChars::F_Char)) { // a-f
                number |= (static_cast<unsigned int>(digit - DigitChars::UW_Char));
            }

            ++offset;
        }

        return number;
    }

    /////////////////////////////////////////////////////////////////

    template <typename Number_T>
    static void FastStringToNumber(Number_T &number, const Char_T_ *content, SizeT length) noexcept {
        SizeT offset = 0;
        number       = 0;

        if (offset < length) {
            number += (static_cast<Number_T>(content[offset] - DigitChars::ZeroChar));
            ++offset;

            while (offset < length) {
                number *= 10ULL;
                number += (static_cast<Number_T>(content[offset] - DigitChars::ZeroChar));
                ++offset;
            }
        }
    }

    static unsigned int StringToNumber(QNumber &number, const Char_T_ *content, SizeT &offset,
                                       SizeT end_offset) noexcept {
        SizeT        o_offset = offset;
        SizeT        max_end_offset;
        unsigned int flags    = 0;
        unsigned int exponent = 0;
        number.Natural        = 0;

        if (offset < end_offset) {
            Char_T_ digit = content[offset];

            switch (digit) {
                case DigitChars::NegativeChar: {
                    ++offset;

                    if (offset < end_offset) {
                        flags |= stringToNumberFlags::Negative;
                        digit = content[offset];
                        break;
                    }

                    return 0U;
                }

                case DigitChars::PositiveChar: {
                    ++offset;

                    if (offset < end_offset) {
                        digit = content[offset];
                        break;
                    }

                    return 0U;
                }

                default: {
                }
            }

            if ((end_offset - offset) < 19U) {
                max_end_offset = end_offset;
            } else {
                max_end_offset = offset + 19U;

                if ((flags & stringToNumberFlags::Negative) != 0) {
                    --max_end_offset;
                }

                flags |= stringToNumberFlags::OverFlow;
            }

            if (offset < max_end_offset) {
                if ((digit == DigitChars::ZeroChar) && ((max_end_offset - offset) > 1)) {
                    digit = content[offset + 1];

                    if ((digit >= DigitChars::ZeroChar) && (digit <= DigitChars::NineChar)) {
                        return 0U; // Leading zero.
                    }
                }

                while (true) {
                    while (offset < max_end_offset) {
                        digit = content[offset];

                        if ((digit >= DigitChars::ZeroChar) && (digit <= DigitChars::NineChar)) {
                            number.Natural *= 10ULL;
                            number.Natural += (static_cast<unsigned long long>(digit - DigitChars::ZeroChar));
                            ++offset;
                            continue;
                        }

                        break;
                    }

                    switch (digit) {
                        case DigitChars::DotChar: {
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

                        case DigitChars::UE_Char:
                        case DigitChars::E_Char: {
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

    QENTEM_NOINLINE static int exponentToNumber(unsigned int &exponent, unsigned int &flags, const Char_T_ *content,
                                                SizeT &offset, SizeT end_offset) noexcept {
        flags |= stringToNumberFlags::Real;
        flags |= stringToNumberFlags::MergedExponent;

        ++offset;
        Char_T_ digit = content[offset];

        switch (digit) {
            case DigitChars::NegativeChar: {
                flags |= stringToNumberFlags::NegativeExponent;
                ++offset;
                break;
            }

            case DigitChars::PositiveChar: {
                ++offset;
                break;
            }

            default: {
                if ((digit < DigitChars::ZeroChar) && (digit > DigitChars::NineChar)) {
                    return false;
                }
            }
        }

        const SizeT  sci_offset   = offset;
        unsigned int sci_exponent = 0U;

        while (offset < end_offset) {
            digit = content[offset];

            if ((digit >= DigitChars::ZeroChar) && (digit <= DigitChars::NineChar)) {
                sci_exponent *= 10ULL;
                sci_exponent += (static_cast<unsigned int>(digit - DigitChars::ZeroChar));
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

                if ((digit >= DigitChars::ZeroChar) && (digit <= DigitChars::NineChar)) {
                    ++offset;
                    continue;
                }

                break;
            } while (offset < end_offset);

            switch (digit) {
                case DigitChars::DotChar: {
                    exponent_diff = (offset - last_offset);
                    is_diff_set   = true;

                    ++offset;
                    continue;
                }

                case DigitChars::E_Char:
                case DigitChars::UE_Char: {
                    if ((flags & stringToNumberFlags::Real) == 0U) {
                        flags |= stringToNumberFlags::Real;

                        if (!is_diff_set) {
                            exponent_diff = (offset - last_offset);
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
                        exponent_diff = (offset - last_offset);
                    }

                    keep_on = false;
                }
            }
        }

        if ((flags & stringToNumberFlags::Real) == 0U) {
            if ((exponent_diff == 0U) || ((exponent_diff == 1U) && (number.Natural < 0x2000000000000000))) {
                digit                  = content[last_offset];
                unsigned long long tmp = number.Natural;

                if ((digit >= DigitChars::ZeroChar) && (digit <= DigitChars::NineChar)) {
                    tmp *= 10U;
                    tmp += (static_cast<unsigned long long>(digit - DigitChars::ZeroChar));

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
                        exponent = (exponent_diff - exponent);
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

            if ((flags & stringToNumberFlags::Negative) != 0) {
                number.Real = -number.Real;
            }

            if (number.Natural != 9218868437227405312) {
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
        while (exponent >= 100) {
            number *= 1e100;
            exponent -= 100;
        }

        while (exponent >= 10) {
            number *= 1e10;
            exponent -= 10;
        }

        while (exponent > 0) {
            number *= 10;
            --exponent;
        }
    }

    static void powerOfNegativeTen(double &number, unsigned int exponent) {
        while (exponent >= 100) {
            number /= 1e100;
            exponent -= 100;
        }

        while (exponent >= 10) {
            number /= 1e10;
            exponent -= 10;
        }

        while (exponent > 0) {
            number /= 10;
            --exponent;
        }
    }

    QENTEM_NOINLINE static void intToString(Char_T_ *storage, SizeT &offset, unsigned long long number) {
        using NumberT_ = unsigned long long;

        static constexpr char table[] = {"000102030405060708091011121314151617181920212223242526272829"
                                         "303132333435363738394041424344454647484950515253545556575859"
                                         "606162636465666768697071727374757677787980818283848586878889"
                                         "90919293949596979899"};

        while (number >= NumberT_{100}) {
            offset -= SizeT{2};
            const SizeT index = (static_cast<SizeT>(number % NumberT_{100}) * SizeT{2});
            number /= NumberT_{100};

            storage[offset]            = static_cast<Char_T_>(table[index]);
            storage[offset + SizeT{1}] = static_cast<Char_T_>(table[index + SizeT{1}]);
        }

        if (number < NumberT_{10}) {
            --offset;
            storage[offset] = static_cast<Char_T_>(number) + DigitChars::ZeroChar;
        } else {
            offset -= SizeT{2};
            const SizeT index          = (static_cast<SizeT>(number) * SizeT{2});
            storage[offset]            = static_cast<Char_T_>(table[index]);
            storage[offset + SizeT{1}] = static_cast<Char_T_>(table[index + SizeT{1}]);
        }
    }

    template <typename Stream_T_>
    inline static void unsignedIntToString(Stream_T_ &stream, unsigned long long number) {
        Char_T_ storage[Config::IntMaxLength];
        SizeT   offset = Config::IntMaxLength;

        /*
         *   18446744073709551615 MAX unsigned long long   20
         *  -9223372036854775807  MAX long long            19 + (-|+) = 20
         *
         *   4294967295           MAX unsigned int         10
         *  -2147483647           MAX int                  10 + (-|+) = 11
         *
         *   65535                MAX unsigned short       5
         *  -32767                MAX short                5 + (-|+) = 6
         */

        intToString(&(storage[0]), offset, static_cast<unsigned long long>(number));
        stream.Write(&(storage[offset]), (Config::IntMaxLength - offset));
    }

    template <typename Stream_T_>
    inline static void signedIntToString(Stream_T_ &stream, long long number) {
        Char_T_ storage[Config::IntMaxLength];
        SizeT   offset = Config::IntMaxLength;

        /*
         *   18446744073709551615 MAX unsigned long long   20
         *  -9223372036854775807  MAX long long            19 + (-|+) = 20
         *
         *   4294967295           MAX unsigned int         10
         *  -2147483647           MAX int                  10 + (-|+) = 11
         *
         *   65535                MAX unsigned short       5
         *  -32767                MAX short                5 + (-|+) = 6
         */

        bool negative = false;

        if (number < 0) {
            number   = -number;
            negative = true;
        }

        intToString(&(storage[0]), offset, static_cast<unsigned long long>(number));

        if (negative) {
            --offset;
            storage[offset] = DigitChars::NegativeChar;
        }

        stream.Write(&(storage[offset]), (Config::IntMaxLength - offset));
    }

    template <typename String_T_>
    QENTEM_NOINLINE static void doubleToString(String_T_ &dstring, double number, SizeT min, SizeT r_min,
                                               SizeT precision) {
        constexpr SizeT max_end_offset = (Config::FloatMaxLength - 1U);

        Char_T_            tmp[max_end_offset];
        Char_T_            tmp2[Config::FloatMaxLength];
        unsigned long long fraction        = 0;
        SizeT              end_offset      = max_end_offset;
        SizeT              fraction_length = 0;
        SizeT              offset          = 0;
        int                exponent        = 0;
        const bool         negative        = (number < 0);
        const bool         no_exponent     = (r_min != 0);

        if (negative) {
            number = -number;
            ++offset;
        }

        if ((number > 1E19) || ((number != 0) && (number < 1E-17))) {
            extractExponent(number, exponent);
            r_min     = 0;
            precision = 15;
        }

        unsigned long long left_number = static_cast<unsigned long long>(number);
        unsigned long long tmp_number  = left_number;

        if (tmp_number != 0) {
            intToString(&(tmp[0]), end_offset, tmp_number);
        }

        SizeT left_length = (max_end_offset - end_offset);

        if (end_offset != 0) { // Full
            number -= static_cast<double>(left_number);

            if (number != 0) {
                SizeT precision2 = 17;

                if (left_length != 0) {
                    --precision2;
                    precision2 -= left_length;
                }

                if ((precision == 0U) || (precision > precision2)) {
                    precision = precision2;
                }

                fraction = extractFraction(number, precision);

                if ((precision < 17) && ((fraction % 10) > 4)) {
                    fraction /= 10;
                    ++fraction;
                } else {
                    fraction /= 10;
                }

                // Removing all zeros from the fraction.
                while ((precision != 0) && ((fraction % 10) == 0U)) {
                    fraction /= 10;
                    --precision;
                }

                if ((precision == 0U) && (fraction == 1)) {
                    fraction = 0;
                    ++left_number;
                    end_offset = max_end_offset;

                    intToString(&(tmp[0]), end_offset, left_number);

                    left_length = (max_end_offset - end_offset);
                }

                fraction_length = precision;
            }
        }

        if ((fraction_length != 0) || no_exponent) {
            if (r_min > fraction_length) {
                r_min -= fraction_length;
            } else {
                r_min = 0;
            }

            ++fraction_length; // One for DotChar
        }

        while (end_offset < max_end_offset) {
            tmp2[offset] = tmp[end_offset];
            ++offset;
            ++end_offset;
        }

        if (fraction_length != 0) {
            end_offset = --fraction_length;

            if (end_offset != 0) {
                intToString(&(tmp[0]), end_offset, fraction);
            }

            if (((end_offset == 0U) && (exponent == 0U)) || (left_length != 0) || no_exponent) {
                SizeT offset2 = end_offset;

                tmp2[offset] = DigitChars::DotChar;
                ++offset;

                while (end_offset != 0) {
                    tmp2[offset] = DigitChars::ZeroChar;
                    ++offset;
                    --end_offset;
                }

                while (offset2 < fraction_length) {
                    tmp2[offset] = tmp[offset2];
                    ++offset;
                    ++offset2;
                }

                while (r_min != 0) {
                    tmp2[offset] = DigitChars::ZeroChar;
                    ++offset;
                    --r_min;
                }
            } else {
                offset = (negative) ? 1 : 0;
                --exponent;
                ++left_length;
                exponent -= static_cast<int>(end_offset);

                tmp2[offset] = tmp[end_offset];
                ++offset;
                ++end_offset;

                if (end_offset != fraction_length) {
                    tmp2[offset] = DigitChars::DotChar;
                    ++offset;

                    do {
                        tmp2[offset] = tmp[end_offset];
                        ++offset;
                        ++end_offset;
                    } while (end_offset < fraction_length);
                }
            }
        }

        if (exponent != 0) {
            tmp2[offset] = DigitChars::E_Char;
            ++offset;

            if (exponent < 0) {
                exponent     = -exponent;
                tmp2[offset] = DigitChars::NegativeChar;
                ++offset;
            }

            end_offset = 4;

            intToString(&(tmp[0]), end_offset, static_cast<unsigned long long>(exponent));

            while (end_offset < 4) {
                tmp2[offset] = tmp[end_offset];
                ++offset;
                ++end_offset;
            }
        }

        end_offset = offset;
        offset     = 0;

        if (min >= left_length) {
            min -= left_length;
            end_offset += min;

            if (end_offset > Config::FloatMaxLength) {
                min += Config::FloatMaxLength;
                min        = static_cast<unsigned int>(min - end_offset);
                end_offset = Config::FloatMaxLength;
            }
        } else {
            min = 0;
        }

        Char_T_     *str     = getCharForNumber(dstring, end_offset);
        unsigned int offset2 = 0;

        if (negative) {
            str[0] = DigitChars::NegativeChar;
            ++offset;
            ++offset2;
        }

        while (min != 0) {
            str[offset] = DigitChars::ZeroChar;
            ++offset;
            --min;
        }

        while (offset < end_offset) {
            str[offset] = tmp2[offset2];
            ++offset;
            ++offset2;
        }
    }

    inline static Char_T_ *getCharForNumber(String<Char_T_> &dstring, SizeT length) {
        dstring = String<Char_T_>{length};
        return dstring.Storage();
    }

    template <typename Stream_T_>
    inline static Char_T_ *getCharForNumber(Stream_T_ &string, SizeT length) {
        return string.Buffer(length);
    }

    QENTEM_NOINLINE static void extractExponent(double &number, int &exponent) noexcept {
        if (number > 1E19) {
            do {
                if (number > 1E99) {
                    exponent += 100;
                    number /= 1E100;
                } else if (number > 1E9) {
                    exponent += 10;
                    number /= 1E10;
                } else {
                    ++exponent;
                    number /= 10;
                }
            } while (number > 9);
        } else {
            do {
                if (number < 1E-99) {
                    exponent -= 100;
                    number *= 1E100;
                } else if (number < 1E-9) {
                    exponent -= 10;
                    number *= 1E10;
                } else {
                    --exponent;
                    number *= 10;
                }
            } while (number < 0.9);
        }
    }

    static unsigned long long extractFraction(double number, SizeT precision) noexcept {
        static constexpr double mul[] = {1,    1E2,  1E3,  1E4,  1E5,  1E6,  1E7,  1E8, 1E9,
                                         1E10, 1E11, 1E12, 1E13, 1E14, 1E15, 1E16, 1E17};

        if (precision < 17U) {
            return static_cast<unsigned long long>(number * mul[precision]);
        }

        return static_cast<unsigned long long>(number * 1E18);
    }

  private:
    class DigitChars {
      public:
        static constexpr Char_T_ ZeroChar     = '0';
        static constexpr Char_T_ OneChar      = '1';
        static constexpr Char_T_ FiveChar     = '5';
        static constexpr Char_T_ SevenChar    = '7';
        static constexpr Char_T_ NineChar     = '9';
        static constexpr Char_T_ E_Char       = 'e';
        static constexpr Char_T_ UE_Char      = 'E';
        static constexpr Char_T_ DotChar      = '.';
        static constexpr Char_T_ PositiveChar = '+';
        static constexpr Char_T_ NegativeChar = '-';
        static constexpr Char_T_ UA_Char      = 'A';
        static constexpr Char_T_ UF_Char      = 'F';
        static constexpr Char_T_ A_Char       = 'a';
        static constexpr Char_T_ F_Char       = 'f';
        static constexpr Char_T_ UW_Char      = 'W';
    };
};

} // namespace Qentem

#endif
