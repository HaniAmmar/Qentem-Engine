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

#include "StringStream.hpp"

#ifndef QENTEM_DIGIT_H_
#define QENTEM_DIGIT_H_

#define QENTEM_DECIMAL_BASE_ 10
#define QENTEM_EXPONENT_MAX_LENGTH_ 5
#define QENTEM_INT_NUMBER_MAX_SIZE_ 20
#define QENTEM_FLOAT_NUMBER_MAX_SIZE_ 22

namespace Qentem {

/*
 * Basic number conversion.
 */

template <typename Char_T_>
class Digit {
  private:
    class DigitChars {
      public:
        static constexpr Char_T_ ZeroChar  = '0';
        static constexpr Char_T_ NineChar  = '9';
        static constexpr Char_T_ SevenChar = '7';
        static constexpr Char_T_ E_Char    = 'e';
        static constexpr Char_T_ UE_Char   = 'E';
        static constexpr Char_T_ DotChar   = '.';
        static constexpr Char_T_ PlusChar  = '+';
        static constexpr Char_T_ MinusChar = '-';
        static constexpr Char_T_ ColonChar = ':';
        static constexpr Char_T_ SlashChar = '/';
        static constexpr Char_T_ UA_Char   = 'A';
        static constexpr Char_T_ UF_Char   = 'F';
        static constexpr Char_T_ A_Char    = 'a';
        static constexpr Char_T_ F_Char    = 'f';
        static constexpr Char_T_ UW_Char   = 'W';
    };

  public:
    static unsigned int HexStringToNumber(const Char_T_ *str, SizeT length) noexcept {
        unsigned int value = 0;

        if (str != nullptr) {
            unsigned int base = 0;

            while (length != 0) {
                --length;

                if ((str[length] > DigitChars::ZeroChar) && (str[length] < DigitChars::ColonChar)) {
                    // No use for 0
                    value += ((static_cast<unsigned int>(str[length]) - DigitChars::ZeroChar) << base);    // 1-9
                } else if ((str[length] >= DigitChars::UA_Char) && (str[length] <= DigitChars::UF_Char)) { // A-F
                    value += ((static_cast<unsigned int>(str[length]) - DigitChars::SevenChar) << base);
                } else if ((str[length] >= DigitChars::A_Char) && (str[length] <= DigitChars::F_Char)) { // a-f
                    value += ((static_cast<unsigned int>(str[length]) - DigitChars::UW_Char) << base);
                }

                base += 4U;
            }
        }

        return value;
    }

    /*
     * "min" is the minimum digits. Zeros will be added to the left side, if the
     * number of digits is less than "mini"
     */
    template <typename Number_T_>
    inline static String<Char_T_> NumberToString(Number_T_ number, unsigned int min = 1) noexcept {
        constexpr bool is_unsigned = (static_cast<Number_T_>(-1) > 0);
        return NumberToStringHelper<Number_T_, is_unsigned>::NumberToString(number, min);
    }

    template <typename Number_T_>
    inline static void NumberToStringStream(StringStream<Char_T_> &ss, Number_T_ number,
                                            unsigned int min = 1) noexcept {
        constexpr bool is_unsigned = (static_cast<Number_T_>(-1) > 0);
        NumberToStringStreamHelper<Number_T_, is_unsigned>::NumberToStringStream(ss, number, min);
    }

    /*
     * "min" is the minimum digits to the left of the pieried. Zeros will be
     * added if the digits on the left are less than "min". "r_min" is the same
     * as "min" but for the right side. "precision" is the precision of the
     * decimal.
     */
    inline static String<Char_T_> NumberToString(double number, unsigned int min = 1, unsigned int r_min = 0,
                                                 unsigned int precision = 0) {
        String<Char_T_> str;
        doubleToString(str, number, min, r_min, precision);
        return str;
    }

    inline static String<Char_T_> NumberToString(float f_number, unsigned int min = 1, unsigned int r_min = 0,
                                                 unsigned int precision = 0) {
        return NumberToString(static_cast<double>(f_number), min, r_min, precision);
    }

    inline static void NumberToStringStream(StringStream<Char_T_> &ss, double number, unsigned int min = 1,
                                            unsigned int r_min = 0, unsigned int precision = 0) {
        doubleToString(ss, number, min, r_min, precision);
    }

    inline static void NumberToStringStream(StringStream<Char_T_> &ss, float f_number, unsigned int min = 1,
                                            unsigned int r_min = 0, unsigned int precision = 0) {
        NumberToStringStream(ss, static_cast<double>(f_number), min, r_min, precision);
    }

    /////////////////////////////////////////////////////////////////

    template <typename Number_T_>
    inline static bool StringToNumber(Number_T_ &number, const Char_T_ *str, SizeT length) noexcept {
        constexpr bool is_unsigned = (static_cast<Number_T_>(-1) > 0);
        constexpr bool is_float    = (static_cast<double>(static_cast<Number_T_>(1.5)) == 1.5);

        return StringToNumberHelper<Number_T_, is_unsigned, is_float>::StringToNumber(number, str, length);
    }

    //////////// Private ////////////

  private:
    template <typename Number_T_, bool IS_UNSIGNED, bool IS_FLOAT>
    class StringToNumberHelper {
      public:
        inline static bool StringToNumber(Number_T_ &number, const Char_T_ *str, SizeT length) noexcept {
            return stringToSignedFloat(number, str, length);
        }
    };

    template <typename Number_T_>
    class StringToNumberHelper<Number_T_, true, false> {
      public:
        inline static bool StringToNumber(Number_T_ &number, const Char_T_ *str, SizeT length) noexcept {
            return stringToUnsignedInt(number, str, length);
        }
    };

    template <typename Number_T_>
    class StringToNumberHelper<Number_T_, false, false> {
      public:
        inline static bool StringToNumber(Number_T_ &number, const Char_T_ *str, SizeT length) noexcept {
            return stringToSignedInt(number, str, length);
        }
    };

    template <typename Number_T_>
    static bool stringToInt(Number_T_ &number, const Char_T_ *str, SizeT length) noexcept {
        Number_T_ postion = 1;

        do {
            --length;
            const Char_T_ c = str[length];

            if ((c > DigitChars::SlashChar) && (c < DigitChars::ColonChar)) {
                number += ((static_cast<Number_T_>(c) - DigitChars::ZeroChar) * postion);
                postion *= QENTEM_DECIMAL_BASE_;
            } else {
                return false;
            }
        } while (length != 0);

        return true;
    }

    template <typename Number_T_>
    static bool stringToUnsignedInt(Number_T_ &number, const Char_T_ *str, SizeT length) noexcept {
        number = 0;

        if (length != 0) {
            if ((length > 1) && (str[0] == DigitChars::ZeroChar)) {
                return false; // No leanding zeros.
            }

            if (str[0] == DigitChars::PlusChar) {
                SizeT offset = 1;
                StringUtils::TrimLeft(str, offset, length);
                length -= offset;

                if (length != 0) {
                    return stringToUnsignedInt(number, &(str[offset]), length);
                }
            } else if (stringToInt(number, str, length)) {
                return true;
            }

            number = 0;
        }

        return false;
    }

    template <typename Number_T_>
    static bool stringToSignedInt(Number_T_ &number, const Char_T_ *s_str, SizeT length) noexcept {
        number = 0;

        if (length != 0) {
            if ((length > 1) && (s_str[0] == DigitChars::ZeroChar)) {
                return false; // No leanding zeros.
            }

            switch (s_str[0]) {
                case DigitChars::MinusChar: {
                    SizeT offset = 1;
                    StringUtils::TrimLeft(s_str, offset, length);
                    length -= offset;

                    if ((length != 0) && stringToSignedInt(number, &(s_str[offset]), length)) {
                        number *= -1;
                        return true;
                    }

                    break;
                }

                case DigitChars::PlusChar: {
                    SizeT offset = 1;
                    StringUtils::TrimLeft(s_str, offset, length);
                    length -= offset;

                    if (length != 0) {
                        return stringToSignedInt(number, &(s_str[offset]), length);
                    }

                    break;
                }

                default: {
                    if (stringToInt(number, s_str, length)) {
                        return true;
                    }
                }
            }

            number = 0;
        }

        return false;
    }

    template <typename Number_T_, bool IS_UNSIGNED>
    class NumberToStringHelper {
      public:
        inline static String<Char_T_> NumberToString(Number_T_ number, unsigned int min = 1) {
            String<Char_T_> str;
            intToString(str, number, min, false);
            return str;
        }
    };

    template <typename Number_T_>
    class NumberToStringHelper<Number_T_, false> {
      public:
        inline static String<Char_T_> NumberToString(Number_T_ number, unsigned int min = 1) {
            String<Char_T_> str;

            if (number < 0) {
                number *= -1;
                intToString(str, number, min, true);
            } else {
                intToString(str, number, min, false);
            }

            return str;
        }
    };

    template <typename Number_T_, bool IS_UNSIGNED>
    class NumberToStringStreamHelper {
      public:
        inline static void NumberToStringStream(StringStream<Char_T_> &ss, Number_T_ number, unsigned int min = 1) {
            intToString(ss, number, min, false);
        }
    };

    template <typename Number_T_>
    class NumberToStringStreamHelper<Number_T_, false> {
      public:
        inline static void NumberToStringStream(StringStream<Char_T_> &ss, Number_T_ number, unsigned int min = 1) {
            if (number < 0) {
                number *= -1;
                intToString(ss, number, min, true);
            } else {
                intToString(ss, number, min, false);
            }
        }
    };

    template <typename Number_T_>
    static bool stringToSignedFloat(Number_T_ &number, const Char_T_ *str, SizeT length) noexcept {
        number = 0;

        if (length != 0) {
            if (((length > 1) && ((str[0] == DigitChars::ZeroChar) && (str[1] != DigitChars::DotChar)))) {
                number = 0;
                return false; // No leanding zeros.
            }

            switch (str[0]) {
                case DigitChars::MinusChar: {
                    SizeT n_offset = 1;
                    StringUtils::TrimLeft(str, n_offset, length);
                    length -= n_offset;

                    if ((length != 0) && stringToSignedFloat(number, &(str[n_offset]), length)) {
                        number *= -1;
                        return true;
                    }

                    break;
                }

                case DigitChars::PlusChar: {
                    SizeT offset = 1;
                    StringUtils::TrimLeft(str, offset, length);
                    length -= offset;

                    if (length != 0) {
                        return stringToSignedFloat(number, &(str[offset]), length);
                    }

                    break;
                }

                default: {
                    int exponent = 0;

                    if (parseExponent(exponent, str, length) && stringToFloat(number, exponent, str, length)) {
                        return true;
                    }
                }
            }

            number = 0;
        }

        return false;
    }

    QENTEM_NOINLINE static bool parseExponent(int &exponent, const Char_T_ *str, SizeT &length) noexcept {
        SizeT offset = (length - 1);

        if (offset != 0) {
            SizeT offset2    = 0;
            SizeT MAX_LENGTH = QENTEM_EXPONENT_MAX_LENGTH_; // e(-|+)xxx
            int   sign       = 0;

            do {
                const Char_T_ c = str[offset];

                if ((c < DigitChars::ZeroChar) || (c > DigitChars::NineChar)) {
                    switch (c) {
                        case DigitChars::DotChar: {
                            return true;
                        }

                        case DigitChars::E_Char:
                        case DigitChars::UE_Char: {
                            if (MAX_LENGTH == QENTEM_EXPONENT_MAX_LENGTH_) {
                                // No number.
                                return false;
                            }

                            offset2    = offset;
                            MAX_LENGTH = 0;
                            continue;
                        }

                        case DigitChars::MinusChar:
                        case DigitChars::PlusChar: {
                            if ((MAX_LENGTH == QENTEM_EXPONENT_MAX_LENGTH_) || (sign != 0)) {
                                // No number, or double sign.
                                return false;
                            }

                            sign = ((c == DigitChars::MinusChar) ? -1 : 1);
                            break;
                        }

                        default: {
                            return false;
                        }
                    }
                }

                --MAX_LENGTH;
                --offset;
            } while ((offset != 0) && (MAX_LENGTH != 0));

            if (offset2 != 0) {
                if (sign != 0) {
                    ++offset2;
                }

                while (++offset2 < length) {
                    exponent *= QENTEM_DECIMAL_BASE_;
                    exponent += static_cast<int>(str[offset2] - DigitChars::ZeroChar);
                }

                if (sign == -1) {
                    exponent *= -1;
                }

                length = offset;
            }
        }

        return true;
    }

    template <typename Number_T_>
    QENTEM_NOINLINE static bool stringToFloat(Number_T_ &number, int exponent, const Char_T_ *str,
                                              SizeT length) noexcept {
        unsigned long long w_number = 0;
        SizeT              len      = ((length < 18) ? length : 17);
        unsigned int       offset   = 0;
        Char_T_            c;
        bool               has_dot = false;

        do {
            c = str[offset];
            ++offset;

            if ((c > DigitChars::SlashChar) && (c < DigitChars::ColonChar)) {
                w_number *= QENTEM_DECIMAL_BASE_;
                w_number += (static_cast<unsigned long long>(c) - DigitChars::ZeroChar);
            } else if (c == DigitChars::DotChar) {
                if (has_dot || (offset == length)) {
                    return false;
                }

                has_dot = true;
                break;
            } else {
                return false;
            }
        } while (offset < len);

        if (has_dot) {
            Number_T_ base = 1;
            len            = ((length < 20) ? length : 19);

            do {
                c = str[offset];

                if ((c > DigitChars::SlashChar) && (c < DigitChars::ColonChar)) {
                    base *= QENTEM_DECIMAL_BASE_;
                    number += (static_cast<Number_T_>(c) - DigitChars::ZeroChar) / base;
                } else {
                    return false;
                }

                ++offset;
            } while (offset < len);
        } else if (offset != length) {
            do {
                c = str[offset];

                if ((c > DigitChars::SlashChar) && (c < DigitChars::ColonChar)) {
                    ++exponent;
                } else if (c == DigitChars::DotChar) {
                    break;
                } else {
                    return false;
                }

                ++offset;
            } while (offset < length);
        }

        number += static_cast<Number_T_>(w_number);

        if (exponent != 0) {
            bool negative = false;

            if (exponent < 0) {
                negative = true;
                exponent *= -1;
            }

            do {
                Number_T_ number2;

                if (exponent > 9) {
                    number2 = 1E10;
                    exponent -= QENTEM_DECIMAL_BASE_;
                } else {
                    number2 = 1E1;
                    --exponent;
                }

                if (negative) {
                    number /= number2;
                } else {
                    number *= number2;
                }
            } while (exponent != 0);
        }

        return true;
    }

    template <typename String_T_, typename Number_T_>
    QENTEM_NOINLINE static void intToString(String_T_ &dstring, Number_T_ number, SizeT min, bool negative) {
        Char_T_ tmp[QENTEM_INT_NUMBER_MAX_SIZE_];
        SizeT   offset = 0;

        /*
         *  18446744073709551615 MAX unsigned long long 20
         *  -9223372036854775807 MAX long long          19 + (-|+) = 20
         *
         *  4294967295           MAX unsigned int       10
         *  -2147483647          MAX int                10 + (-|+) = 11
         *
         *  65535                MAX unsigned short       5
         *  -32767               MAX short                5 + (-|+) = 6
         */

        while (number != 0) {
            tmp[offset] = (Char_T_(number % QENTEM_DECIMAL_BASE_) + DigitChars::ZeroChar);
            ++offset;
            number /= QENTEM_DECIMAL_BASE_;
        }

        if (min > offset) {
            min -= offset;

            if ((offset + min) > QENTEM_INT_NUMBER_MAX_SIZE_) {
                min = QENTEM_INT_NUMBER_MAX_SIZE_ - offset;
            }

            if (negative) {
                --min;
            }

            while (min != 0) {
                tmp[offset] = DigitChars::ZeroChar;
                ++offset;
                --min;
            }
        }

        if (negative) {
            tmp[offset] = DigitChars::MinusChar;
            ++offset;
        }

        Char_T_ *str = getCharForNumber(dstring, offset);

        while (offset != 0) {
            --offset;
            *str = tmp[offset];
            ++str;
        }
    }

    template <typename String_T_>
    QENTEM_NOINLINE static void doubleToString(String_T_ &dstring, double number, unsigned int min, unsigned int r_min,
                                               unsigned int precision) {
        constexpr unsigned int max_length = QENTEM_FLOAT_NUMBER_MAX_SIZE_ - 1;

        Char_T_            tmp[max_length];
        Char_T_            tmp2[QENTEM_FLOAT_NUMBER_MAX_SIZE_];
        unsigned long long fraction        = 0;
        unsigned int       fraction_length = 0;
        unsigned int       end_offset      = max_length;
        unsigned int       offset          = 0;
        int                exponent        = 0;
        const bool         negative        = (number < 0);
        const bool         no_exponent     = (r_min != 0);

        if (negative) {
            number *= -1;
            ++offset;
        }

        if ((number > 1E19) || ((number != 0) && (number < 1E-17))) {
            extractExponent(number, exponent);
            r_min     = 0;
            precision = 15;
        }

        unsigned long long left_number = static_cast<unsigned long long>(number);

        unsigned long long tmp_number = left_number;

        while (tmp_number != 0) {
            --end_offset;
            tmp[end_offset] = (Char_T_(tmp_number % QENTEM_DECIMAL_BASE_) + DigitChars::ZeroChar);
            tmp_number /= QENTEM_DECIMAL_BASE_;
        }

        unsigned int left_length = (max_length - end_offset);

        if (end_offset != 0) { // Full
            number -= static_cast<double>(left_number);

            if (number != 0) {
                unsigned int precision2 = 17;

                if (left_length != 0) {
                    --precision2;
                    precision2 -= left_length;
                }

                if ((precision == 0) || (precision > precision2)) {
                    precision = precision2;
                }

                fraction = extractFraction(number, precision);

                if ((precision < 17) && ((fraction % QENTEM_DECIMAL_BASE_) > 4)) {
                    fraction /= QENTEM_DECIMAL_BASE_;
                    ++fraction;
                } else {
                    fraction /= QENTEM_DECIMAL_BASE_;
                }

                // Removing all zeros from the fraction.
                while ((precision != 0) && ((fraction % QENTEM_DECIMAL_BASE_) == 0)) {
                    fraction /= QENTEM_DECIMAL_BASE_;
                    --precision;
                }

                if ((precision == 0) && (fraction == 1)) {
                    fraction = 0;
                    ++left_number;
                    end_offset = max_length;

                    while (left_number != 0) {
                        --end_offset;
                        tmp[end_offset] = (Char_T_(left_number % QENTEM_DECIMAL_BASE_) + DigitChars::ZeroChar);
                        left_number /= QENTEM_DECIMAL_BASE_;
                    }

                    left_length = (max_length - end_offset);
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

        while (end_offset < max_length) {
            tmp2[offset] = tmp[end_offset];
            ++offset;
            ++end_offset;
        }

        if (fraction_length != 0) {
            end_offset = --fraction_length;

            while (fraction != 0) {
                --end_offset;
                tmp[end_offset] = (Char_T_(fraction % QENTEM_DECIMAL_BASE_) + DigitChars::ZeroChar);
                fraction /= QENTEM_DECIMAL_BASE_;
            }

            if (((end_offset == 0) && (exponent == 0)) || (left_length != 0) || no_exponent) {
                unsigned int offset2 = end_offset;

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
                exponent *= -1;
                tmp2[offset] = DigitChars::MinusChar;
                ++offset;
            }

            end_offset = 4;

            while (exponent != 0) {
                --end_offset;
                tmp[end_offset] =
                    static_cast<Char_T_>(static_cast<Char_T_>(exponent % QENTEM_DECIMAL_BASE_) + DigitChars::ZeroChar);
                exponent /= QENTEM_DECIMAL_BASE_;
            }

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

            if (end_offset > QENTEM_FLOAT_NUMBER_MAX_SIZE_) {
                min += QENTEM_FLOAT_NUMBER_MAX_SIZE_;
                min        = (min - end_offset);
                end_offset = QENTEM_FLOAT_NUMBER_MAX_SIZE_;
            }
        } else {
            min = 0;
        }

        Char_T_     *str     = getCharForNumber(dstring, end_offset);
        unsigned int offset2 = 0;

        if (negative) {
            str[0] = DigitChars::MinusChar;
            ++offset;
            ++offset2;
        }

        while (min != 0) {
            str[offset] = DigitChars::ZeroChar;
            ++offset;
            --min;
        }

        while (offset != end_offset) {
            str[offset] = tmp2[offset2];
            ++offset;
            ++offset2;
        }
    }

    inline static Char_T_ *getCharForNumber(String<Char_T_> &dstring, SizeT length) {
        dstring = String<Char_T_>{length};
        return dstring.Storage();
    }

    inline static Char_T_ *getCharForNumber(StringStream<Char_T_> &dstring, SizeT length) {
        return dstring.Buffer(length);
    }

    QENTEM_NOINLINE static void extractExponent(double &number, int &exponent) noexcept {
        if (number > 1E19) {
            do {
                if (number > 1E99) {
                    exponent += 100;
                    number /= 1E100;
                } else if (number > 1E9) {
                    exponent += QENTEM_DECIMAL_BASE_;
                    number /= 1E10;
                } else {
                    ++exponent;
                    number /= QENTEM_DECIMAL_BASE_;
                }
            } while (number > 9);
        } else {
            do {
                if (number < 1E-99) {
                    exponent -= 100;
                    number *= 1E100;
                } else if (number < 1E-9) {
                    exponent -= QENTEM_DECIMAL_BASE_;
                    number *= 1E10;
                } else {
                    --exponent;
                    number *= QENTEM_DECIMAL_BASE_;
                }
            } while (number < 0.9);
        }
    }

    static unsigned long long extractFraction(double number, unsigned int precision) noexcept {
        static const double mul[] = {1,    1E2,  1E3,  1E4,  1E5,  1E6,  1E7,  1E8, 1E9,
                                     1E10, 1E11, 1E12, 1E13, 1E14, 1E15, 1E16, 1E17};

        if (precision < 17U) {
            return static_cast<unsigned long long>(number * mul[precision]);
        }

        return static_cast<unsigned long long>(number * 1E18);
    }
};

} // namespace Qentem

#endif
