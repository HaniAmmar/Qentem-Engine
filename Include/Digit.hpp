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

namespace SubDigit {

template <typename Char_T_, ULong S = sizeof(Char_T_)>
struct DigitChars {};

// One byte character.
template <typename Char_T_>
struct DigitChars<Char_T_, 1> {
    static constexpr Char_T_ ZeroChar        = '0';
    static constexpr Char_T_ NineChar        = '9';
    static constexpr Char_T_ SevenChar       = '7';
    static constexpr Char_T_ E_Char          = 'e';
    static constexpr Char_T_ UE_Char         = 'E';
    static constexpr Char_T_ PlusChar        = '+';
    static constexpr Char_T_ MinusChar       = '-';
    static constexpr Char_T_ DotChar         = '.';
    static constexpr Char_T_ ColonChar       = ':';
    static constexpr Char_T_ SlashChar       = '/';
    static constexpr Char_T_ AtChar          = '@'; // A is after @
    static constexpr Char_T_ UG_Char         = 'G'; // F is before G
    static constexpr Char_T_ G_Char          = 'g';
    static constexpr Char_T_ UW_Char         = 'W';
    static constexpr Char_T_ GraveAccentChar = '`'; // a is after `
};

// Two bytes character.
template <typename Char_T_>
struct DigitChars<Char_T_, 2> {
    static constexpr Char_T_ ZeroChar        = u'0';
    static constexpr Char_T_ NineChar        = u'9';
    static constexpr Char_T_ SevenChar       = u'7';
    static constexpr Char_T_ E_Char          = u'e';
    static constexpr Char_T_ UE_Char         = u'E';
    static constexpr Char_T_ PlusChar        = u'+';
    static constexpr Char_T_ MinusChar       = u'-';
    static constexpr Char_T_ DotChar         = u'.';
    static constexpr Char_T_ ColonChar       = u':';
    static constexpr Char_T_ SlashChar       = u'/';
    static constexpr Char_T_ AtChar          = u'@'; // A is after @
    static constexpr Char_T_ UG_Char         = u'G'; // F is before G
    static constexpr Char_T_ G_Char          = u'g';
    static constexpr Char_T_ UW_Char         = u'W';
    static constexpr Char_T_ GraveAccentChar = u'`'; // a is after `
};

// Four bytes character.
template <typename Char_T_>
struct DigitChars<Char_T_, 4> {
    static constexpr Char_T_ ZeroChar        = U'0';
    static constexpr Char_T_ NineChar        = U'9';
    static constexpr Char_T_ SevenChar       = U'7';
    static constexpr Char_T_ E_Char          = U'e';
    static constexpr Char_T_ UE_Char         = U'E';
    static constexpr Char_T_ PlusChar        = U'+';
    static constexpr Char_T_ MinusChar       = U'-';
    static constexpr Char_T_ DotChar         = U'.';
    static constexpr Char_T_ ColonChar       = U':';
    static constexpr Char_T_ SlashChar       = U'/';
    static constexpr Char_T_ AtChar          = U'@'; // A is after @
    static constexpr Char_T_ UG_Char         = U'G'; // F is before G
    static constexpr Char_T_ G_Char          = U'g';
    static constexpr Char_T_ UW_Char         = U'W';
    static constexpr Char_T_ GraveAccentChar = U'`'; // a is after `
};

// wchar_t
template <>
struct DigitChars<wchar_t> {
    static constexpr wchar_t ZeroChar        = L'0';
    static constexpr wchar_t NineChar        = L'9';
    static constexpr wchar_t SevenChar       = L'7';
    static constexpr wchar_t E_Char          = L'e';
    static constexpr wchar_t UE_Char         = L'E';
    static constexpr wchar_t PlusChar        = L'+';
    static constexpr wchar_t MinusChar       = L'-';
    static constexpr wchar_t DotChar         = L'.';
    static constexpr wchar_t ColonChar       = L':';
    static constexpr wchar_t SlashChar       = L'/';
    static constexpr wchar_t AtChar          = L'@'; // A is after @
    static constexpr wchar_t UG_Char         = L'G'; // F is before G
    static constexpr wchar_t G_Char          = L'g';
    static constexpr wchar_t UW_Char         = L'W';
    static constexpr wchar_t GraveAccentChar = L'`'; // a is after `
};

} // namespace SubDigit

template <typename Char_T_>
class Digit {
  public:
    static UInt HexStringToNumber(const Char_T_ *str, SizeT length) noexcept {
        using DigitChars_T_ = SubDigit::DigitChars<Char_T_>;

        UInt value = 0;

        if (str != nullptr) {
            UInt base = 0;

            while (length != 0) {
                --length;

                if ((str[length] > DigitChars_T_::ZeroChar) &&
                    (str[length] < DigitChars_T_::ColonChar)) {
                    // No use for 0
                    value += ((static_cast<UInt>(str[length]) -
                               DigitChars_T_::ZeroChar)
                              << base); // 1-9
                } else if ((str[length] > DigitChars_T_::AtChar) &&
                           (str[length] < DigitChars_T_::UG_Char)) { // A-F
                    value += ((static_cast<UInt>(str[length]) -
                               DigitChars_T_::SevenChar)
                              << base);
                } else if ((str[length] > DigitChars_T_::GraveAccentChar) &&
                           (str[length] < DigitChars_T_::G_Char)) { // a-f
                    value += ((static_cast<UInt>(str[length]) -
                               DigitChars_T_::UW_Char)
                              << base);
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
    inline static String<Char_T_> NumberToString(Number_T_ number,
                                                 UInt      min = 1) noexcept {
        constexpr bool is_unsigned = (static_cast<Number_T_>(-1) > 0);
        return S_NumberToString<Number_T_, is_unsigned>::NumberToString(number,
                                                                        min);
    }

    template <typename Number_T_>
    inline static void NumberToStringStream(StringStream<Char_T_> &ss,
                                            Number_T_              number,
                                            UInt min = 1) noexcept {
        constexpr bool is_unsigned = (static_cast<Number_T_>(-1) > 0);
        S_NumberToStringStream<Number_T_, is_unsigned>::NumberToStringStream(
            ss, number, min);
    }

    /*
     * "min" is the minimum digits to the left of the pieried. Zeros will be
     * added if the digits on the left are less than "min". "r_min" is the same
     * as "min" but for the right side. "precision" is the precision of the
     * decimal.
     */
    inline static String<Char_T_> NumberToString(double number, UInt min = 1,
                                                 UInt r_min     = 0,
                                                 UInt precision = 0) {
        String<Char_T_> str;
        doubleToString(str, number, min, r_min, precision);
        return str;
    }

    inline static String<Char_T_> NumberToString(float f_number, UInt min = 1,
                                                 UInt r_min     = 0,
                                                 UInt precision = 0) {
        return NumberToString(static_cast<double>(f_number), min, r_min,
                              precision);
    }

    inline static void NumberToStringStream(StringStream<Char_T_> &ss,
                                            double number, UInt min = 1,
                                            UInt r_min     = 0,
                                            UInt precision = 0) {
        doubleToString(ss, number, min, r_min, precision);
    }

    inline static void NumberToStringStream(StringStream<Char_T_> &ss,
                                            float f_number, UInt min = 1,
                                            UInt r_min     = 0,
                                            UInt precision = 0) {
        NumberToStringStream(ss, static_cast<double>(f_number), min, r_min,
                             precision);
    }

    /////////////////////////////////////////////////////////////////

    template <typename Number_T_>
    inline static bool StringToNumber(Number_T_ &number, const Char_T_ *str,
                                      SizeT length) noexcept {
        constexpr bool is_unsigned = (static_cast<Number_T_>(-1) > 0);
        constexpr bool is_float =
            (static_cast<double>(static_cast<Number_T_>(1.5)) == 1.5);

        return S_StringToNumber<Number_T_, is_unsigned,
                                is_float>::StringToNumber(number, str, length);
    }

    //////////// Private ////////////

  private:
    template <typename Number_T_, bool IS_UNSIGNED, bool IS_FLOAT>
    struct S_StringToNumber {
        inline static bool StringToNumber(Number_T_ &number, const Char_T_ *str,
                                          SizeT length) noexcept {
            return stringToSignedFloat(number, str, length);
        }
    };

    template <typename Number_T_>
    struct S_StringToNumber<Number_T_, true, false> {
        inline static bool StringToNumber(Number_T_ &number, const Char_T_ *str,
                                          SizeT length) noexcept {
            return stringToUnsignedInt(number, str, length);
        }
    };

    template <typename Number_T_>
    struct S_StringToNumber<Number_T_, false, false> {
        inline static bool StringToNumber(Number_T_ &number, const Char_T_ *str,
                                          SizeT length) noexcept {
            return stringToSignedInt(number, str, length);
        }
    };

    template <typename Number_T_>
    static bool stringToInt(Number_T_ &number, const Char_T_ *str,
                            SizeT length) noexcept {
        using DigitChars_T_ = SubDigit::DigitChars<Char_T_>;

        Number_T_ postion = 1;

        do {
            --length;
            const Char_T_ c = str[length];

            if ((c > DigitChars_T_::SlashChar) &&
                (c < DigitChars_T_::ColonChar)) {
                number +=
                    ((static_cast<Number_T_>(c) - DigitChars_T_::ZeroChar) *
                     postion);
                postion *= QENTEM_DECIMAL_BASE_;
            } else {
                return false;
            }
        } while (length != 0);

        return true;
    }

    template <typename Number_T_>
    static bool stringToUnsignedInt(Number_T_ &number, const Char_T_ *str,
                                    SizeT length) noexcept {
        using DigitChars_T_ = SubDigit::DigitChars<Char_T_>;

        number = 0;

        if ((str != nullptr) && (length != 0)) {
            if ((length > 1) && (str[0] == DigitChars_T_::ZeroChar)) {
                return false; // No leanding zeros.
            }

            if (str[0] == DigitChars_T_::PlusChar) {
                SizeT offset = 1;
                StringUtils::SoftTrim(str, offset, --length);

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
    static bool stringToSignedInt(Number_T_ &number, const Char_T_ *s_str,
                                  SizeT length) noexcept {
        using DigitChars_T_ = SubDigit::DigitChars<Char_T_>;

        number = 0;

        if ((s_str != nullptr) && (length != 0)) {
            if ((length > 1) && (s_str[0] == DigitChars_T_::ZeroChar)) {
                return false; // No leanding zeros.
            }

            switch (s_str[0]) {
                case DigitChars_T_::MinusChar: {
                    SizeT ni_offset = 1;
                    StringUtils::SoftTrim(s_str, ni_offset, --length);

                    if ((length != 0) &&
                        stringToSignedInt(number, &(s_str[ni_offset]),
                                          length)) {
                        number *= -1;
                        return true;
                    }

                    break;
                }

                case DigitChars_T_::PlusChar: {
                    SizeT offset = 1;
                    StringUtils::SoftTrim(s_str, offset, --length);

                    if (length != 0) {
                        return stringToSignedInt(number, &(s_str[offset]),
                                                 length);
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
    struct S_NumberToString {
        inline static String<Char_T_> NumberToString(Number_T_ number,
                                                     UInt      min = 1) {
            String<Char_T_> str;
            intToString(str, number, min, false);
            return str;
        }
    };

    template <typename Number_T_>
    struct S_NumberToString<Number_T_, false> {
        inline static String<Char_T_> NumberToString(Number_T_ number,
                                                     UInt      min = 1) {
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
    struct S_NumberToStringStream {
        inline static void NumberToStringStream(StringStream<Char_T_> &ss,
                                                Number_T_              number,
                                                UInt min = 1) {
            intToString(ss, number, min, false);
        }
    };

    template <typename Number_T_>
    struct S_NumberToStringStream<Number_T_, false> {
        inline static void NumberToStringStream(StringStream<Char_T_> &ss,
                                                Number_T_              number,
                                                UInt min = 1) {
            if (number < 0) {
                number *= -1;
                intToString(ss, number, min, true);
            } else {
                intToString(ss, number, min, false);
            }
        }
    };

    template <typename Number_T_>
    static bool stringToSignedFloat(Number_T_ &number, const Char_T_ *str,
                                    SizeT length) noexcept {
        using DigitChars_T_ = SubDigit::DigitChars<Char_T_>;

        number = 0;

        if ((str != nullptr) && (length != 0)) {
            if (((length > 1) && ((str[0] == DigitChars_T_::ZeroChar) &&
                                  (str[1] != DigitChars_T_::DotChar)))) {
                number = 0;
                return false; // No leanding zeros.
            }

            switch (str[0]) {
                case DigitChars_T_::MinusChar: {
                    SizeT n_offset = 1;
                    StringUtils::SoftTrim(str, n_offset, --length);

                    if ((length != 0) &&
                        stringToSignedFloat(number, &(str[n_offset]), length)) {
                        number *= -1;
                        return true;
                    }

                    break;
                }

                case DigitChars_T_::PlusChar: {
                    SizeT offset = 1;
                    StringUtils::SoftTrim(str, offset, --length);

                    if (length != 0) {
                        return stringToSignedFloat(number, &(str[offset]),
                                                   length);
                    }

                    break;
                }

                default: {
                    int exponent = 0;

                    if (parseExponent(exponent, str, length) &&
                        stringToFloat(number, exponent, str, length)) {
                        return true;
                    }
                }
            }

            number = 0;
        }

        return false;
    }

    QENTEM_NOINLINE static bool parseExponent(int &exponent, const Char_T_ *str,
                                              SizeT &length) noexcept {
        using DigitChars_T_ = SubDigit::DigitChars<Char_T_>;

        SizeT offset = (length - 1);

        if (offset != 0) {
            SizeT offset2    = 0;
            SizeT MAX_LENGTH = QENTEM_EXPONENT_MAX_LENGTH_; // e(-|+)xxx
            int   sign       = 0;

            do {
                const Char_T_ c = str[offset];

                if ((c < DigitChars_T_::ZeroChar) ||
                    (c > DigitChars_T_::NineChar)) {
                    switch (c) {
                        case DigitChars_T_::DotChar: {
                            return true;
                        }

                        case DigitChars_T_::E_Char:
                        case DigitChars_T_::UE_Char: {
                            if (MAX_LENGTH == QENTEM_EXPONENT_MAX_LENGTH_) {
                                // No number.
                                return false;
                            }

                            offset2    = offset;
                            MAX_LENGTH = 0;
                            continue;
                        }

                        case DigitChars_T_::MinusChar:
                        case DigitChars_T_::PlusChar: {
                            if ((MAX_LENGTH == QENTEM_EXPONENT_MAX_LENGTH_) ||
                                (sign != 0)) {
                                // No number, or double sign.
                                return false;
                            }

                            sign = ((c == DigitChars_T_::MinusChar) ? -1 : 1);
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
                    exponent += static_cast<int>(str[offset2] -
                                                 DigitChars_T_::ZeroChar);
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
    QENTEM_NOINLINE static bool stringToFloat(Number_T_ &number, int exponent,
                                              const Char_T_ *str,
                                              SizeT          length) noexcept {
        using DigitChars_T_ = SubDigit::DigitChars<Char_T_>;

        unsigned long long w_number = 0;
        SizeT              len      = ((length < 18) ? length : 17);
        UInt               offset   = 0;
        Char_T_            c;
        bool               has_dot = false;

        do {
            c = str[offset];
            ++offset;

            if ((c > DigitChars_T_::SlashChar) &&
                (c < DigitChars_T_::ColonChar)) {
                w_number *= QENTEM_DECIMAL_BASE_;
                w_number += (static_cast<ULong>(c) - DigitChars_T_::ZeroChar);
            } else if (c == DigitChars_T_::DotChar) {
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

                if ((c > DigitChars_T_::SlashChar) &&
                    (c < DigitChars_T_::ColonChar)) {
                    base *= QENTEM_DECIMAL_BASE_;
                    number +=
                        (static_cast<Number_T_>(c) - DigitChars_T_::ZeroChar) /
                        base;
                } else {
                    return false;
                }

                ++offset;
            } while (offset < len);
        } else if (offset != length) {
            do {
                c = str[offset];

                if ((c > DigitChars_T_::SlashChar) &&
                    (c < DigitChars_T_::ColonChar)) {
                    ++exponent;
                } else if (c == DigitChars_T_::DotChar) {
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
    QENTEM_NOINLINE static void
    intToString(String_T_ &dstring, Number_T_ number, UInt min, bool negative) {
        using DigitChars_T_ = SubDigit::DigitChars<Char_T_>;

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
            tmp[offset] = (Char_T_(number % QENTEM_DECIMAL_BASE_) +
                           DigitChars_T_::ZeroChar);
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
                tmp[offset] = DigitChars_T_::ZeroChar;
                ++offset;
                --min;
            }
        }

        if (negative) {
            tmp[offset] = DigitChars_T_::MinusChar;
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
    QENTEM_NOINLINE static void doubleToString(String_T_ &dstring,
                                               double number, UInt min,
                                               UInt r_min, UInt precision) {
        using DigitChars_T_ = SubDigit::DigitChars<Char_T_>;

        constexpr UInt max_length = QENTEM_FLOAT_NUMBER_MAX_SIZE_ - 1;

        Char_T_            tmp[max_length];
        Char_T_            tmp2[QENTEM_FLOAT_NUMBER_MAX_SIZE_];
        unsigned long long fraction        = 0;
        UInt               fraction_length = 0;
        UInt               end_offset      = max_length;
        UInt               offset          = 0;
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

        unsigned long long left_number =
            static_cast<unsigned long long>(number);

        unsigned long long tmp_number = left_number;

        while (tmp_number != 0) {
            --end_offset;
            tmp[end_offset] = (Char_T_(tmp_number % QENTEM_DECIMAL_BASE_) +
                               DigitChars_T_::ZeroChar);
            tmp_number /= QENTEM_DECIMAL_BASE_;
        }

        UInt left_length = (max_length - end_offset);

        if (end_offset != 0) { // Full
            number -= static_cast<double>(left_number);

            if (number != 0) {
                UInt precision2 = 17;

                if (left_length != 0) {
                    --precision2;
                    precision2 -= left_length;
                }

                if ((precision == 0) || (precision > precision2)) {
                    precision = precision2;
                }

                fraction = extractFraction(number, precision);

                if ((precision < 17) &&
                    ((fraction % QENTEM_DECIMAL_BASE_) > 4)) {
                    fraction /= QENTEM_DECIMAL_BASE_;
                    ++fraction;
                } else {
                    fraction /= QENTEM_DECIMAL_BASE_;
                }

                // Removing all zeros from the fraction.
                while ((precision != 0) &&
                       ((fraction % QENTEM_DECIMAL_BASE_) == 0)) {
                    fraction /= QENTEM_DECIMAL_BASE_;
                    --precision;
                }

                if ((precision == 0) && (fraction == 1)) {
                    fraction = 0;
                    ++left_number;
                    end_offset = max_length;

                    while (left_number != 0) {
                        --end_offset;
                        tmp[end_offset] =
                            (Char_T_(left_number % QENTEM_DECIMAL_BASE_) +
                             DigitChars_T_::ZeroChar);
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
                tmp[end_offset] = (Char_T_(fraction % QENTEM_DECIMAL_BASE_) +
                                   DigitChars_T_::ZeroChar);
                fraction /= QENTEM_DECIMAL_BASE_;
            }

            if (((end_offset == 0) && (exponent == 0)) || (left_length != 0) ||
                no_exponent) {
                UInt offset2 = end_offset;

                tmp2[offset] = DigitChars_T_::DotChar;
                ++offset;

                while (end_offset != 0) {
                    tmp2[offset] = DigitChars_T_::ZeroChar;
                    ++offset;
                    --end_offset;
                }

                while (offset2 < fraction_length) {
                    tmp2[offset] = tmp[offset2];
                    ++offset;
                    ++offset2;
                }

                while (r_min != 0) {
                    tmp2[offset] = DigitChars_T_::ZeroChar;
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
                    tmp2[offset] = DigitChars_T_::DotChar;
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
            tmp2[offset] = DigitChars_T_::E_Char;
            ++offset;

            if (exponent < 0) {
                exponent *= -1;
                tmp2[offset] = DigitChars_T_::MinusChar;
                ++offset;
            }

            end_offset = 4;

            while (exponent != 0) {
                --end_offset;
                tmp[end_offset] = static_cast<Char_T_>(
                    static_cast<Char_T_>(exponent % QENTEM_DECIMAL_BASE_) +
                    DigitChars_T_::ZeroChar);
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

        Char_T_ *str     = getCharForNumber(dstring, end_offset);
        UInt     offset2 = 0;

        if (negative) {
            str[0] = DigitChars_T_::MinusChar;
            ++offset;
            ++offset2;
        }

        while (min != 0) {
            str[offset] = DigitChars_T_::ZeroChar;
            ++offset;
            --min;
        }

        while (offset != end_offset) {
            str[offset] = tmp2[offset2];
            ++offset;
            ++offset2;
        }
    }

    inline static Char_T_ *getCharForNumber(String<Char_T_> &dstring,
                                            SizeT            length) {
        Char_T_ *str = HAllocator::Allocate<Char_T_>(length + 1U);
        str[length]  = 0;
        dstring      = String<Char_T_>{str, length};
        return str;
    }

    inline static Char_T_ *getCharForNumber(StringStream<Char_T_> &dstring,
                                            SizeT                  length) {
        return dstring.Buffer(length);
    }

    QENTEM_NOINLINE static void extractExponent(double &number,
                                                int &   exponent) noexcept {
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

    QENTEM_NOINLINE static unsigned long long
    extractFraction(double number, UInt precision) noexcept {
        switch (precision) {
            case 1: {
                number *= 1E2;
                break;
            }

            case 2: {
                number *= 1E3;
                break;
            }

            case 3: {
                number *= 1E4;
                break;
            }

            case 4: {
                number *= 1E5;
                break;
            }

            case 5: {
                number *= 1E6;
                break;
            }

            case 6: {
                number *= 1E7;
                break;
            }

            case 7: {
                number *= 1E8;
                break;
            }

            case 8: {
                number *= 1E9;
                break;
            }

            case 9: {
                number *= 1E10;
                break;
            }

            case 10: {
                number *= 1E11;
                break;
            }

            case 11: {
                number *= 1E12;
                break;
            }

            case 12: {
                number *= 1E13;
                break;
            }

            case 13: {
                number *= 1E14;
                break;
            }

            case 14: {
                number *= 1E15;
                break;
            }

            case 15: {
                number *= 1E16;
                break;
            }

            case 16: {
                number *= 1E17;
                break;
            }

            default: {
                number *= 1E18;
            }
        }

        return static_cast<unsigned long long>(number);
    }
};

} // namespace Qentem

#endif
