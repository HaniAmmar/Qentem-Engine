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

#include "TestHelper.hpp"
#include "Digit.hpp"
#include "StringStream.hpp"

// #define QENTEM_COMPARE_DIGIT_WITH_STL_ true
#define QENTEM_TEST_DIGIT_SKIP_NAN_ true
#define QENTEM_TEST_DIGIT_SKIP_INF_ true

#ifdef QENTEM_COMPARE_DIGIT_WITH_STL_
#include <iomanip>
#include <sstream>
#include <string>
#endif

#ifndef QENTEM_DIGIT_TESTS_H_
#define QENTEM_DIGIT_TESTS_H_

namespace Qentem::Test {

static SizeT HexStringToNumber(const char *str) noexcept {
    return Digit::HexStringToNumber(str, Qentem::StringUtils::Count(str));
}

template <typename Number_T_>
QENTEM_NOINLINE static void IntToStreamEqual(TestHelper &helper, StringStream<char> &stream, Number_T_ number,
                                             const char *expected, const char *name, unsigned long line) {
    Digit::NumberToString(stream, number);
    helper.Equal(stream, expected, name, line);
    stream.Clear();
}

template <typename Stream_T_, typename Number_T_>
QENTEM_NOINLINE static void RealToStreamEqual(TestHelper &helper, Stream_T_ &stream, Number_T_ number,
                                              Digit::RealFormatInfo format, const char *expected, const char *name,
                                              unsigned long line) {
    if (!helper.HasError() || helper.IsContinueOnError()) {
#ifdef QENTEM_COMPARE_DIGIT_WITH_STL_
        std::ostringstream out;
        out.precision(format.Precision);
        out << number;

        Digit::NumberToString(stream, number, format);
        helper.Equal(stream, out.str().c_str(), name, line);
        stream.Clear();

        (void)expected;
#else
        Digit::NumberToString(stream, number, format);
        helper.Equal(stream, expected, name, line);
        stream.Clear();
#endif
    }
}

template <typename Number_T_>
static bool StringToNumber(Number_T_ &num, const char *str) noexcept {
    QNumber     number;
    SizeT       offset = 0;
    const SizeT length = StringUtils::Count(str);

    switch (Digit::StringToNumber(number, str, offset, length)) {
        case QNumberType::Natural: {
            num = Number_T_(number.Natural);
            break;
        }

        case QNumberType::Integer: {
            num = Number_T_(number.Integer);
            break;
        }

        case QNumberType::Real: {
            num = Number_T_(number.Real);
            break;
        }

        default: {
            num = 0;
            return false;
        }
    };

    return (offset >= length);
}

static void TestStringToNumber1(TestHelper &helper) {
    unsigned long long number   = 0;
    double             d_number = 0;
    const char        *str;

    str = "";
    helper.EqualsFalse(StringToNumber(number, str), "valid", __LINE__);

    str = "0";
    helper.EqualsTrue(StringToNumber(number, str), "valid", __LINE__);
    helper.Equal(number, 0U, str, __LINE__);

    str = "0.0";
    helper.EqualsTrue(StringToNumber(d_number, str), "valid", __LINE__);
    helper.Equal(d_number, 0.0, str, __LINE__);

    str = "-0";
    helper.EqualsTrue(StringToNumber(d_number, str), "valid", __LINE__);
    helper.Equal(d_number, -0.0, str, __LINE__);

    str = "-0.0";
    helper.EqualsTrue(StringToNumber(d_number, str), "valid", __LINE__);
    helper.Equal(d_number, -0.0, str, __LINE__);

    str = "1";
    helper.EqualsTrue(StringToNumber(number, str), "valid", __LINE__);
    helper.Equal(number, 1U, str, __LINE__);

    str = "1000000";
    helper.EqualsTrue(StringToNumber(number, str), "valid", __LINE__);
    helper.Equal(number, 1000000U, str, __LINE__);

    str = "1010101";
    helper.EqualsTrue(StringToNumber(number, str), "valid", __LINE__);
    helper.Equal(number, 1010101U, str, __LINE__);

    str = "9999999";
    helper.EqualsTrue(StringToNumber(number, str), "valid", __LINE__);
    helper.Equal(number, 9999999U, str, __LINE__);

    str = "123456789";
    helper.EqualsTrue(StringToNumber(number, str), "valid", __LINE__);
    helper.Equal(number, 123456789U, str, __LINE__);
}

static void TestStringToNumber2(TestHelper &helper) {
    int           number_int    = 0;
    unsigned int  number_uint   = 0;
    long          number_long   = 0;
    unsigned long number_ulong  = 0;
    float         number_float  = 0;
    double        number_double = 0;

    const char *str;
    bool        valid;

    str   = "1-e";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1+e";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1-e1";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1+e1";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1--e1";
    valid = StringToNumber(number_ulong, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1++e1";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e-";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e+";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e--1";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e++1";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1ee1";
    valid = StringToNumber(number_double, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E";
    valid = StringToNumber(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "E1";
    valid = StringToNumber(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1-E";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1+E";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1-E1";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1+E1";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1--E1";
    valid = StringToNumber(number_ulong, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1++E1";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E-";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E+";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E--1";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E++1";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1EE1";
    valid = StringToNumber(number_double, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = ".";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "..";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "...";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = ".  ";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "..  ";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "...  ";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+.";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-.";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+..";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+...";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-..";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-...";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = ".0.0";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "0.0.";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "0..0";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "0..0]]]";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = ".123.123";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "123.123.";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "123..123";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "12y3";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1x23";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "w123";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-";
    valid = StringToNumber(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e+";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e-";
    valid = StringToNumber(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-w1";
    valid = StringToNumber(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+w1";
    valid = StringToNumber(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "01";
    valid = StringToNumber(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+01";
    valid = StringToNumber(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-01";
    valid = StringToNumber(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "00001";
    valid = StringToNumber(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+00001";
    valid = StringToNumber(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-00001";
    valid = StringToNumber(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "0123456";
    valid = StringToNumber(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+0123456";
    valid = StringToNumber(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-0123456";
    valid = StringToNumber(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e";
    valid = StringToNumber(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "e1";
    valid = StringToNumber(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "e12";
    valid = StringToNumber(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1-e";
    valid = StringToNumber(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "e-1";
    valid = StringToNumber(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-e12";
    valid = StringToNumber(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1+e";
    valid = StringToNumber(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+e1";
    valid = StringToNumber(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "e+12";
    valid = StringToNumber(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "500000000000000000X000.000000000000";
    valid = StringToNumber(number_double, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e-330";
    valid = StringToNumber(number_double, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e+310";
    valid = StringToNumber(number_double, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e+999";
    valid = StringToNumber(number_double, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e-999";
    valid = StringToNumber(number_double, str);
    helper.EqualsFalse(valid, "valid", __LINE__);
}

static void TestStringToNumber3(TestHelper &helper) {
    const char *content = nullptr;
    QNumber     number;
    QNumberType type   = QNumberType::NotANumber;
    SizeT       offset = 0;

    content = "0]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Natural, "Natural", __LINE__);
    helper.Equal(number.Natural, 0U, "number", __LINE__);

    offset  = 0;
    content = ".0]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Real, "Real", __LINE__);
    helper.Equal(number.Real, 0.0, "number", __LINE__);

    offset  = 0;
    content = "0.]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Real, "Real", __LINE__);
    helper.Equal(number.Real, 0.0, "number", __LINE__);

    offset  = 0;
    content = "0.0]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Real, "Real", __LINE__);
    helper.Equal(number.Real, -0.0, "number", __LINE__);

    offset  = 0;
    content = "-0]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Real, "Real", __LINE__);
    helper.Equal(number.Real, -0.0, "number", __LINE__);

    offset  = 0;
    content = "-.0]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Real, "Real", __LINE__);
    helper.Equal(number.Real, -0.0, "number", __LINE__);

    offset  = 0;
    content = "-0.]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Real, "Real", __LINE__);
    helper.Equal(number.Real, -0.0, "number", __LINE__);

    offset  = 0;
    content = "-0.0]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Real, "Real", __LINE__);
    helper.Equal(number.Real, -0.0, "number", __LINE__);

    offset  = 0;
    content = "1]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Natural, "Natural", __LINE__);
    helper.Equal(number.Natural, 1U, "number", __LINE__);

    offset  = 0;
    content = ".2]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Real, "Real", __LINE__);
    helper.Equal(number.Real, 0.2, "number", __LINE__);

    offset  = 0;
    content = "3.]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Real, "Real", __LINE__);
    helper.Equal(number.Real, 3.0, "number", __LINE__);

    offset  = 0;
    content = "4.5]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Real, "Real", __LINE__);
    helper.Equal(number.Real, 4.5, "number", __LINE__);

    offset  = 0;
    content = "-1]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Integer, "Integer", __LINE__);
    helper.Equal(number.Integer, -1, "number", __LINE__);

    offset  = 0;
    content = "-.2]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Real, "Real", __LINE__);
    helper.Equal(number.Real, -0.2, "number", __LINE__);

    offset  = 0;
    content = "-3.]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Real, "Real", __LINE__);
    helper.Equal(number.Real, -3.0, "number", __LINE__);

    offset  = 0;
    content = "-4.5]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.EqualsTrue(type == QNumberType::Real, "Real", __LINE__);
    helper.Equal(number.Real, -4.5, "number", __LINE__);
}

static void TestStringToNumber4(TestHelper &helper) {
    int          number_int    = 0;
    unsigned int number_uint   = 0;
    double       number_double = 0;

    const char *str;
    bool        valid;

    str   = "-1";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, -1.0, "number", __LINE__);

    str   = "+1";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1.0, "number", __LINE__);

    str   = "+1";
    valid = StringToNumber(number_uint, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_uint, 1U, "number", __LINE__);

    str   = "-1";
    valid = StringToNumber(number_int, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_int, -1, "number", __LINE__);

    str   = "1";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1.0, "number", __LINE__);

    str   = "1.0";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1.0, "number", __LINE__);

    str   = "1.00001";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1.00001, "number", __LINE__);

    str   = "1.1";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1.1, "number", __LINE__);

    str   = "123456789";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 123456789.0, "number", __LINE__);

    str   = "1.48828125";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1.48828125, "number", __LINE__);

    str   = "1e10";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1e10, "number", __LINE__);

    str   = "1E10";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1e10, "number", __LINE__);

    str   = "1e+10";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1e10, "number", __LINE__);

    str   = "1E+10";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1e10, "number", __LINE__);

    str   = "1E5";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1e5, "number", __LINE__);

    str   = "0.3";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 0.3, "number", __LINE__);

    str   = "0.2";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 0.2, "number", __LINE__);

    str   = "0.1";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 0.1, "number", __LINE__);

    str   = "0.5";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 0.5, "number", __LINE__);

    str   = "4.5";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 4.5, "number", __LINE__);

    str   = "2.3";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 2.3, "number", __LINE__);

    str   = "3.2";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 3.2, "number", __LINE__);

    str   = "5.1";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 5.1, "number", __LINE__);

    str   = "-1";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, -1, "number", __LINE__);

    str   = ".123";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, .123, "number", __LINE__);

    str   = "-123.";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, -123., "number", __LINE__);

    str   = "-.123";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, -.123, "number", __LINE__);

    str   = "+123.";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, +123., "number", __LINE__);

    str   = "+.123";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, +.123, "number", __LINE__);

    str   = "123.";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 123., "number", __LINE__);

    str   = ".123e00";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, .123e00, "number", __LINE__);

    str   = "-123.e00";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, -123.e00, "number", __LINE__);

    str   = "-.123e00";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, -.123e00, "number", __LINE__);

    str   = "+123.e00";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, +123.e00, "number", __LINE__);

    str   = "+.123e00";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, +.123e00, "number", __LINE__);

    str = "123.e00 ";
    StringToNumber(number_double, str);
    helper.Equal(number_double, 123.e00, "number", __LINE__);

    str = ".123e00 ";
    StringToNumber(number_double, str);
    helper.Equal(number_double, .123e00, "number", __LINE__);

    str = "-123.e00 ";
    StringToNumber(number_double, str);
    helper.Equal(number_double, -123.e00, "number", __LINE__);

    str = "-.123e00 ";
    StringToNumber(number_double, str);
    helper.Equal(number_double, -.123e00, "number", __LINE__);

    str = "+123.e00 ";
    StringToNumber(number_double, str);
    helper.Equal(number_double, +123.e00, "number", __LINE__);

    str = "+.123e00 ";
    StringToNumber(number_double, str);
    helper.Equal(number_double, +.123e00, "number", __LINE__);

    str = "123.e00 ";
    StringToNumber(number_double, str);
    helper.Equal(number_double, 123.e00, "number", __LINE__);

    str = ".123 ";
    StringToNumber(number_double, str);
    helper.Equal(number_double, .123, "number", __LINE__);

    str = "-123. ";
    StringToNumber(number_double, str);
    helper.Equal(number_double, -123., "number", __LINE__);

    str = "-.123 ";
    StringToNumber(number_double, str);
    helper.Equal(number_double, -.123, "number", __LINE__);

    str = "+123. ";
    StringToNumber(number_double, str);
    helper.Equal(number_double, +123., "number", __LINE__);

    str = "+.123 ";
    StringToNumber(number_double, str);
    helper.Equal(number_double, +.123, "number", __LINE__);

    str   = ".0";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 0.0, "number", __LINE__);

    str   = "0.";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 0.0, "number", __LINE__);

    str   = "123456789.123456789";
    valid = StringToNumber(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 123456789.123456789, "number", __LINE__);
}

static void TestStringToNumber5(TestHelper &helper) {
    const char        *str        = nullptr;
    double             d_number   = 0;
    long long          ll_number  = 0;
    unsigned long long ull_number = 0;
    bool               valid;

    str   = "-1.0";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1, "number", __LINE__);

    str   = "-1.1";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1.1, "number", __LINE__);

    str   = "-123456789";
    valid = StringToNumber(ll_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(ll_number, -123456789, "number", __LINE__);

    str   = "-1.48828125";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1.48828125, "number", __LINE__);

    str   = "-1e10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e10, "number", __LINE__);

    str   = "-1E10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e10, "d_number", __LINE__);

    str   = "-1e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e10, "d_number", __LINE__);

    str   = "-1e+9";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e9, "d_number", __LINE__);

    str   = "-1E+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e10, "d_number", __LINE__);

    str   = "-1e00";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1, "d_number", __LINE__);

    str   = "-1e-00";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1, "d_number", __LINE__);

    str   = "-1e+00";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1, "d_number", __LINE__);

    str   = "1e-00";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1, "d_number", __LINE__);

    str   = "1e+00";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1, "d_number", __LINE__);

    str   = "1e-5";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-5, "d_number", __LINE__);

    str   = "1e5";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e5, "d_number", __LINE__);

    str   = "1e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-10, "d_number", __LINE__);

    str   = "1e+9";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e9, "d_number", __LINE__);

    str   = "-1e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e-10, "d_number", __LINE__);

    str   = "1E-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-10, "d_number", __LINE__);

    str   = "-1E-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e-10, "d_number", __LINE__);

    str   = "-0.3";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -0.3, "d_number", __LINE__);

    str   = "-0.2";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -0.2, "d_number", __LINE__);

    str   = "-0.1";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -0.1, "d_number", __LINE__);

    str   = "-2.3";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -2.3, "d_number", __LINE__);

    str   = "-3.2";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -3.2, "d_number", __LINE__);

    str   = "-4.";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -4., "d_number", __LINE__);

    str   = "-5.1";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -5.1, "d_number", __LINE__);

    str   = "-0";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -0.0, "d_number", __LINE__);

    str   = "-0.0";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -0.0, "d_number", __LINE__);

    str   = "-123456789.12345679";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -123456789.123456789, "number", __LINE__);

    str   = "1844674407370955161";
    valid = StringToNumber(ull_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(ull_number, 1844674407370955161ULL, "number", __LINE__);

    str   = "-1844674407370955161";
    valid = StringToNumber(ll_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(ll_number, -1844674407370955161LL, "number", __LINE__);

    str   = "18446744073709551615";
    valid = StringToNumber(ull_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(ull_number, 18446744073709551615ULL, "number", __LINE__);

    str   = "18446744073709551616";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 18446744073709551616.0, "number", __LINE__);

    str   = "-18446744073709551615";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -18446744073709551615.0, "number", __LINE__);

    str   = "-18446744073709551616";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -18446744073709551616.0, "number", __LINE__);

    str   = "-18446744073709551616e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -18446744073709551616e-10, "number", __LINE__);

    str   = "+18446744073709551616e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, +18446744073709551616e+10, "number", __LINE__);

    str   = "+18446744073709551615e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, +18446744073709551615e+10, "number", __LINE__);

    str   = "-18446744073709551615e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -18446744073709551615e-10, "number", __LINE__);

    str   = "-184467440737095516155";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -184467440737095516155.0, "number", __LINE__);

    str   = "184467440737095516155";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 184467440737095516155.0, "number", __LINE__);

    str   = "9223372036854775807";
    valid = StringToNumber(ll_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(ll_number, 9223372036854775807LL, "number", __LINE__);

    str   = "-9223372036854775807";
    valid = StringToNumber(ll_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(ll_number, -9223372036854775807LL, "number", __LINE__);

    str   = "-9223372036854775808";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -9223372036854775808.0, "number", __LINE__);

    str   = "-9223372036854775808.0";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -9223372036854775808.0, "number", __LINE__);

    str   = "5000000000000000000000000000000000";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5e33, "number", __LINE__);

    str   = "5000000000000000000000.000000000000";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5e21, "number", __LINE__);

    str   = "5000000000000000000000000000000000e-15";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5000000000000000000.0, "number", __LINE__);

    str   = "5000000000000000000000e100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5.000000000000000072e+121, "number", __LINE__);

    str   = "100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e92, "number", __LINE__);

    str   = "200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 2e+92, "number", __LINE__);

    str   = "999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e93, "number", __LINE__);

    str   = "340282366920938463463374607431768211455";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 340282366920938463463374607431768211455.0, "number", __LINE__);

    str   = "115792089237316195423570985008687907853269984665640564039457584007913129639935";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 115792089237316195423570985008687907853269984665640564039457584007913129639935.0, "number",
                 __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 3.123456789123456812, "number", __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 31234567891.23456955, "number", __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 3.123456789123456789123456789123456789123456789123456789123456789e-10, "number", __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789e100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 3.123456789123456643e+100, "number", __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789e-100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 3.1234567891234566e-100, "number", __LINE__);

    str =
        "3.123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789e-308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 3.123456789123456856e-308, "number", __LINE__);

    str =
        "1.7976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157e+300";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1.797693134862315648e+300, "number", __LINE__);

    str =
        "17976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157e+52";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1.797693134862315708e+308, "number", __LINE__);

    str   = "-1.7976931348623157e308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1.7976931348623157e308, "number", __LINE__);

    str   = "1.7976931348623157e+300";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1.797693134862315648e+300, "number", __LINE__);

    str   = "1.7976931348623157e+1";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 17.97693134862315745, "number", __LINE__);

    str   = "4856312379865454687821527487362783273278723787e-7";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 4.856312379865454319e+38, "number", __LINE__);

    str   = "4856312379865454687821527487362783273278723787e+7";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 4.856312379865455113e+52, "number", __LINE__);

    str   = "1.0e308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1.0e308, "d_number", __LINE__);

    str   = "-1.0e308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1.0e308, "d_number", __LINE__);

    str   = "1e308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e308, "d_number", __LINE__);

    str   = "-1e308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e308, "d_number", __LINE__);

    str   = "1.0e-308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 9.999999999999999093e-309, "d_number", __LINE__);

    str   = "-1.0e-308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1.0e-308, "d_number", __LINE__);

    str   = "1e-308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-308, "d_number", __LINE__);

    str   = "-1e-308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e-308, "d_number", __LINE__);

    str   = "4.9406564584124654E-324";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 4.9406564584124654E-324, "d_number", __LINE__);

    str   = "4.940656458412465442e-324";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 4.940656458412465442e-324, "d_number", __LINE__);
}

static void TestStringToNumber6(TestHelper &helper) {
    const char *str      = nullptr;
    double      d_number = 0;
    bool        valid;

    str   = "-12.34";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -12.34, "number", __LINE__);

    str   = "1.00000000";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1, "number", __LINE__);

    str   = "1.05";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1.05, "number", __LINE__);

    str   = "0.25";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.25, "number", __LINE__);

    str   = "1.00005";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1.00005, "number", __LINE__);

    str   = "-0.00025";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -0.00025, "number", __LINE__);

    str   = "0.3743";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.3743, "number", __LINE__);

    str   = "458.3743";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 458.3743, "number", __LINE__);

    str   = "458.3";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 458.3, "number", __LINE__);

    str   = "4.3";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 4.3, "number", __LINE__);

    str   = "1123.0005";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1123.0005, "number", __LINE__);

    str   = "25e-5";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 25e-5, "number", __LINE__);

    str   = "1.000025";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1.000025, "number", __LINE__);

    str   = "10.005e1";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 10.005e1, "number", __LINE__);

    str =
        "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 9.999999999999999666e+117, "number", __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.0000000000000000000000000000000000000000000000001, "number", __LINE__);

    str   = "0.0000000000000000000000005555";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.0000000000000000000000005555, "number", __LINE__);

    str   = "0.0000000000000000000000005555e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.0000000000000000000000005555e-10, "number", __LINE__);

    str   = "0.0000000000000000000000005555e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.0000000000000000000000005555e+10, "number", __LINE__);

    str   = "0.0000000000000000000000005555e+50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.0000000000000000000000005555e+50, "number", __LINE__);

    str   = "0.0000000000000000000000005555e-50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.0000000000000000000000005555e-50, "number", __LINE__);

    str = "0.0000000000000000000000005555   ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 0.0000000000000000000000005555, "number", __LINE__);

    str = "0.0000000000000000000000005555e-10  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 0.0000000000000000000000005555e-10, "number", __LINE__);

    str = "0.0000000000000000000000005555e+10   ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 0.0000000000000000000000005555e+10, "number", __LINE__);

    str = "0.0000000000000000000000005555e+50   ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 0.0000000000000000000000005555e+50, "number", __LINE__);

    str = "0.0000000000000000000000005555e-50   ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 0.0000000000000000000000005555e-50, "number", __LINE__);

    str   = "0.00000005";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.00000005, "number", __LINE__);

    str   = "0.00000005e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.00000005e-10, "number", __LINE__);

    str   = "0.00000005e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.00000005e+10, "number", __LINE__);

    str   = "0.00000005e+50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.00000005e+50, "number", __LINE__);

    str   = "0.00000005e-50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.00000005e-50, "number", __LINE__);

    str = "0.00000005 ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 0.00000005, "number", __LINE__);

    str = "0.00000005e-10 ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 0.00000005e-10, "number", __LINE__);

    str = "0.00000005e+10 ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 0.00000005e+10, "number", __LINE__);

    str = "0.00000005e+50 ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 0.00000005e50, "number", __LINE__);

    str = "0.00000005e-50 ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 0.00000005e-50, "number", __LINE__);

    str   = "50000000000000000.0";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000.0, "number", __LINE__);

    str   = "50000000000000000.0e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000.0e-10, "number", __LINE__);

    str   = "50000000000000000.0e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000.0e+10, "number", __LINE__);

    str   = "50000000000000000.0e+50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000.0e+50, "number", __LINE__);

    str = "50000000000000000.0  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50000000000000000.0, "number", __LINE__);

    str = "50000000000000000.0e-10  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50000000000000000.0e-10, "number", __LINE__);

    str = "50000000000000000.0e+10  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50000000000000000.0e+10, "number", __LINE__);

    str = "50000000000000000.0e+50  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50000000000000000.0e+50, "number", __LINE__);

    str = "50000000000000000.0e+50  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50000000000000000.0e+50, "number", __LINE__);

    str   = "5000000000.0";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5000000000.0, "number", __LINE__);

    str   = "5000000000.0e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5000000000.0e-10, "number", __LINE__);

    str   = "5000000000.0e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5000000000.0e+10, "number", __LINE__);

    str   = "5000000000.0e-50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5000000000.0e-50, "number", __LINE__);

    str   = "5000000000.0e+50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5000000000.0e+50, "number", __LINE__);

    str = "5000000000.0  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 5000000000.0, "number", __LINE__);

    str = "5000000000.0e-10  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 5000000000.0e-10, "number", __LINE__);

    str = "5000000000.0e+10  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 5000000000.0e+10, "number", __LINE__);

    str = "5000000000.0e-50  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 5000000000.0e-50, "number", __LINE__);

    str = "5000000000.0e+50  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 5000000000.0e+50, "number", __LINE__);

    str   = "50000000.0";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000.0, "number", __LINE__);

    str   = "50000000.0e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000.0e-10, "number", __LINE__);

    str   = "50000000.0e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000.0e+10, "number", __LINE__);

    str   = "50000000.0e+50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000.0e+50, "number", __LINE__);

    str   = "50000000.0e-50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000.0e-50, "number", __LINE__);

    str = "50000000.0  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50000000.0, "number", __LINE__);

    str = "50000000.0e-10  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50000000.0e-10, "number", __LINE__);

    str = "50000000.0e+10  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50000000.0e+10, "number", __LINE__);

    str = "50000000.0e+50  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50000000.0e+50, "number", __LINE__);

    str = "50000000.0e-50  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50000000.0e-50, "number", __LINE__);

    str   = "50.0";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50.0, "number", __LINE__);

    str   = "50.0e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50.0e-10, "number", __LINE__);

    str   = "50.0e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50.0e+10, "number", __LINE__);

    str   = "50.0e+50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50.0e+50, "number", __LINE__);

    str   = "50.0e-50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50.0e-50, "number", __LINE__);

    str = "50.0  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50.0, "number", __LINE__);

    str = "50.0e-10  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50.0e-10, "number", __LINE__);

    str = "50.0e+10  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50.0e+10, "number", __LINE__);

    str = "50.0e+50  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50.0e+50, "number", __LINE__);

    str = "50.0e-50  ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 50.0e-50, "number", __LINE__);

    str   = "5.0000000000000000000000000000000";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5.0000000000000000000000000000000, "number", __LINE__);

    str   = "5.0000000000000000000000000000000E10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5.0000000000000000000000000000000E10, "number", __LINE__);

    str   = "5.0000000000000000000000000000000E-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5.0000000000000000000000000000000E-10, "number", __LINE__);

    str   = "5.0000000000000000000000000000000E-50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5.0000000000000000000000000000000E-50, "number", __LINE__);

    str   = "5.0000000000000000000000000000000E+50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5.0000000000000000000000000000000E+50, "number", __LINE__);

    str = "5.0000000000000000000000000000000 ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 5.0000000000000000000000000000000, "number", __LINE__);

    str = "5.0000000000000000000000000000000E10 ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 5.0000000000000000000000000000000E10, "number", __LINE__);

    str = "5.0000000000000000000000000000000E-10 ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 5.0000000000000000000000000000000E-10, "number", __LINE__);

    str = "5.0000000000000000000000000000000E-50 ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 5.0000000000000000000000000000000E-50, "number", __LINE__);

    str = "5.0000000000000000000000000000000E+50 ";
    StringToNumber(d_number, str);
    helper.Equal(d_number, 5.0000000000000000000000000000000E+50, "number", __LINE__);

    str   = "500000000000000.00000000000000000";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 500000000000000.00000000000000000, "number", __LINE__);

    str   = "500000000000000.00000000000000000e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 500000000000000.00000000000000000e+10, "number", __LINE__);

    str   = "500000000000000.00000000000000000e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 500000000000000.00000000000000000e-10, "number", __LINE__);

    str   = "500000000000000.00000000000000000e-50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 500000000000000.00000000000000000e-50, "number", __LINE__);

    str   = "500000000000000.00000000000000000e+50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 500000000000000.00000000000000000e+50, "number", __LINE__);

    str   = "50000000000000000000000000000000.0";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000000000000000000.0, "number", __LINE__);

    str   = "50000000000000000000000000000000.0e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000000000000000000.0e+10, "number", __LINE__);

    str   = "50000000000000000000000000000000.0e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000000000000000000.0e-10, "number", __LINE__);

    str   = "50000000000000000000000000000000.0e-50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000000000000000000.0e-50, "number", __LINE__);

    str   = "50000000000000000000000000000000.0e+50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000000000000000000.0e+50, "number", __LINE__);

    str   = "50000000000000000000000000.000000";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000000000000.000000, "number", __LINE__);

    str   = "50000000000000000000000000.000000e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000000000000.000000e+10, "number", __LINE__);

    str   = "50000000000000000000000000.000000e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000000000000.000000e-10, "number", __LINE__);

    str   = "50000000000000000000000000.000000e-50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000000000000.000000e-50, "number", __LINE__);

    str   = "50000000000000000000000000.000000e+50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000000000000.000000e+50, "number", __LINE__);

    str   = "5000000000000000000000000000000.0";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5000000000000000000000000000000.0, "number", __LINE__);

    str   = "5000000000000000000000000000000.0E10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5000000000000000000000000000000.0E10, "number", __LINE__);

    str   = "5000000000000000000000000000000.0E-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5000000000000000000000000000000.0E-10, "number", __LINE__);

    str   = "5000000000000000000000000000000.0E-60";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5000000000000000000000000000000.0E-60, "number", __LINE__);

    str   = "5000000000000000000000000000000.0E+60";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5000000000000000000000000000000.0E+60, "number", __LINE__);

    str   = "50000000000000000.5";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000000.5, "number", __LINE__);

    str   = "5.0000000050000000000000000000000";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5.0000000050000000000000000000000, "number", __LINE__);

    str   = "500000000.90000000000000000";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 500000000.90000000000000000, "number", __LINE__);

    str   = "50000000000000.90000000000000000";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 50000000000000.90000000000000000, "number", __LINE__);

    str   = "5000000000000000000000000000000000.0";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5000000000000000000000000000000000.0, "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1000000000000000000000000000000000000000000000000.0, "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number,
                 1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000,
                 "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000e10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1000000000000000000000000000000000000000000000000e10, "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1000000000000000000000000000000000000000000000000e+10, "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1000000000000000000000000000000000000000000000000.0e+10, "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0e10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1000000000000000000000000000000000000000000000000.0e10, "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number,
                 1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000e+10,
                 "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1000000000000000000000000000000000000000000000000e-10, "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1000000000000000000000000000000000000000000000000.0e-10, "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number,
                 1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000e-10,
                 "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000e-100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1000000000000000000000000000000000000000000000000e-100, "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0e-100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1000000000000000000000000000000000000000000000000.0e-100, "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000e-100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(
        d_number,
        1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000e-100,
        "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000e100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1000000000000000000000000000000000000000000000000e100, "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0e100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1000000000000000000000000000000000000000000000000.0e100, "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000e+100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1000000000000000000000000000000000000000000000000e+100, "number", __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0e+100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1000000000000000000000000000000000000000000000000.0e+100, "number", __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001e+10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.0000000000000000000000000000000000000000000000001e+10, "number", __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001e10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.0000000000000000000000000000000000000000000000001e10, "number", __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001e+100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.0000000000000000000000000000000000000000000000001e+100, "number", __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001e100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.0000000000000000000000000000000000000000000000001e100, "number", __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.0000000000000000000000000000000000000000000000001e-10, "number", __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001e-100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.0000000000000000000000000000000000000000000000001e-100, "number", __LINE__);
}

static void TestStringToNumber7(TestHelper &helper) {
    const char *str      = nullptr;
    double      d_number = 0;
    bool        valid;

    str   = "5e-1";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5e-1, "number", __LINE__);

    str   = "10e-1";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 10e-1, "number", __LINE__);

    str   = "15e-1";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 15e-1, "number", __LINE__);

    str   = "1.0e-1";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1.0e-1, "number", __LINE__);

    str   = "1.5e-1";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1.5e-1, "number", __LINE__);

    str   = "6.25";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 6.25, "number", __LINE__);

    str   = "62.5";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 62.5, "number", __LINE__);

    str   = "0.625";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.625, "number", __LINE__);

    str   = "0.58963";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 0.58963, "number", __LINE__);

    str   = "10.58963";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 10.58963, "number", __LINE__);

    str   = "1e-10";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-10, "number", __LINE__);

    str   = "1e-50";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-50, "number", __LINE__);

    str   = "1e-100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-100, "number", __LINE__);

    str   = "1e-200";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-200, "number", __LINE__);

    str   = "1e-250";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-250, "number", __LINE__);

    str   = "1e-300";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-300, "number", __LINE__);

    str   = "1e-305";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-305, "number", __LINE__);

    str   = "1e-307";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-307, "number", __LINE__);

    str   = "1e-310";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-310, "number", __LINE__);

    str   = "1e-323";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-323, "number", __LINE__);

    str   = "10e100";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 10e100, "number", __LINE__);

    str   = "1e1";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e1, "number", __LINE__);

    str   = "155e-3";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 155e-3, "number", __LINE__);

    str   = "72057594037927932.0";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 72057594037927932.0, "number", __LINE__);

    str   = "1.78005908680576090884592783737e-307";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1.78005908680576090884592783737e-307, "number", __LINE__);

    str   = "8.90029543402880553236093086933e-308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 8.90029543402880553236093086933e-308, "number", __LINE__);

    str   = "2.22507385850720138309023271733e-308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 2.22507385850720138309023271733e-308, "number", __LINE__);

    str   = "2.22507385850720088902458687609e-308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 2.22507385850720088902458687609e-308, "number", __LINE__);

    str   = "4.94065645841246544176568792868e-324";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 4.94065645841246544176568792868e-324, "number", __LINE__);

    str   = "4.45014771701440227211481959342e-308";
    valid = StringToNumber(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 4.45014771701440227211481959342e-308, "number", __LINE__);
}

static void TestHexStringToNumber(TestHelper &helper) {
    SizeT       number;
    const char *hex = "";

    number = HexStringToNumber(hex);
    helper.Equal(number, 0U, "number", __LINE__);

    hex    = "0000";
    number = HexStringToNumber(hex);
    helper.Equal(number, 0U, "number", __LINE__);

    hex    = "AAAA";
    number = HexStringToNumber(hex);
    helper.Equal(number, 43690U, "number", __LINE__);

    hex    = "000A";
    number = HexStringToNumber(hex);
    helper.Equal(number, 10U, "number", __LINE__);

    hex    = "A000";
    number = HexStringToNumber(hex);
    helper.Equal(number, 40960U, "number", __LINE__);

    hex    = "ABCDEF";
    number = HexStringToNumber(hex);
    helper.Equal(number, 11259375U, "number", __LINE__);

    hex    = "FEDCBA";
    number = HexStringToNumber(hex);
    helper.Equal(number, 16702650U, "number", __LINE__);

    hex    = "abcdef";
    number = HexStringToNumber(hex);
    helper.Equal(number, 11259375U, "number", __LINE__);

    hex    = "fedcba";
    number = HexStringToNumber(hex);
    helper.Equal(number, 16702650U, "number", __LINE__);

    hex    = "12345";
    number = HexStringToNumber(hex);
    helper.Equal(number, 74565U, "number", __LINE__);

    hex    = "6789";
    number = HexStringToNumber(hex);
    helper.Equal(number, 26505U, "number", __LINE__);

    hex    = "FFFF";
    number = HexStringToNumber(hex);
    helper.Equal(number, 65535U, "number", __LINE__);

    hex    = "9999";
    number = HexStringToNumber(hex);
    helper.Equal(number, 39321U, "number", __LINE__);
}

template <typename Stream_T_>
static void TestIntToString1(TestHelper &helper, Stream_T_ &stream) {
    IntToStreamEqual(helper, stream, -1, "-1", "return", __LINE__);
    IntToStreamEqual(helper, stream, 1, "1", "return", __LINE__);
    IntToStreamEqual(helper, stream, -7, "-7", "return", __LINE__);

    IntToStreamEqual(helper, stream, 0U, "0", "return", __LINE__);
    IntToStreamEqual(helper, stream, 10U, "10", "return", __LINE__);

    IntToStreamEqual(helper, stream, -100000L, "-100000", "return", __LINE__);
    IntToStreamEqual(helper, stream, 100000L, "100000", "return", __LINE__);

    IntToStreamEqual(helper, stream, -9223372036854775807LL, "-9223372036854775807", "return", __LINE__);

    IntToStreamEqual(helper, stream, 1ULL, "1", "return", __LINE__);
    IntToStreamEqual(helper, stream, 5000000000000000ULL, "5000000000000000", "return", __LINE__);
    IntToStreamEqual(helper, stream, 4ULL, "4", "return", __LINE__);
    IntToStreamEqual(helper, stream, 3ULL, "3", "return", __LINE__);
    IntToStreamEqual(helper, stream, 9223372036854775807LL, "9223372036854775807", "return", __LINE__);
    IntToStreamEqual(helper, stream, 18446744073709551615ULL, "18446744073709551615", "return", __LINE__);
}

template <typename Stream_T_>
static void TestDoubleToString1(TestHelper &helper, Stream_T_ &stream) {
    RealToStreamEqual(helper, stream, -0.0, 6U, "-0", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0, 6U, "0", "return", __LINE__);
#ifndef QENTEM_TEST_DIGIT_SKIP_INF_
    RealToStreamEqual(helper, stream, (1e+308 + 1e+308), 6U, "inf", "return", __LINE__);
    RealToStreamEqual(helper, stream, (-1e+308 + -1e+308), 6U, "-inf", "return", __LINE__);
#endif

#ifndef QENTEM_TEST_DIGIT_SKIP_NAN_
    RealToStreamEqual(helper, stream, ((1e+308 + 1e+308) / (1e+308 + 1e+308)), 6U, "nan", "return", __LINE__);
    RealToStreamEqual(helper, stream, (0.0 / 0.0), 6U, "nan", "return", __LINE__);
#endif

    RealToStreamEqual(helper, stream, 1.0, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.0, 6U, "2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.0, 6U, "3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4.0, 6U, "4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.0, 6U, "5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 6.0, 6U, "6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.0, 6U, "7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.0, 6U, "8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.0, 6U, "9", "return", __LINE__);
    RealToStreamEqual(helper, stream, 220.0, 6U, "220", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1, 6U, "0.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.2, 6U, "0.2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3, 6U, "0.3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.4, 6U, "0.4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.5, 6U, "0.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.6, 6U, "0.6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.7, 6U, "0.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.8, 6U, "0.8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9, 6U, "0.9", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.09, 6U, "0.09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.009, 6U, "0.009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0009, 6U, "0.0009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00009, 6U, "9e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000009, 6U, "9e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000009, 6U, "9e-07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000009, 6U, "9e-08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000009, 6U, "9e-09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000009, 6U, "9e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000009, 6U, "9e-11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000009, 6U, "9e-12", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000009, 6U, "9e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000000009, 6U, "9e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000000009, 6U, "9e-15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000000009, 6U, "9e-16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.01, 6U, "0.01", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.001, 6U, "0.001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0001, 6U, "0.0001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00001, 6U, "1e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000001, 6U, "1e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000001, 6U, "1e-07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000001, 6U, "1e-08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000001, 6U, "1e-09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000001, 6U, "1e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000001, 6U, "1e-11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000001, 6U, "1e-12", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000001, 6U, "1e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000000001, 6U, "1e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000000001, 6U, "1e-15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000000001, 6U, "1e-16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.02, 6U, "0.02", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.002, 6U, "0.002", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0002, 6U, "0.0002", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00002, 6U, "2e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000002, 6U, "2e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000002, 6U, "2e-07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000002, 6U, "2e-08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000002, 6U, "2e-09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000002, 6U, "2e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000002, 6U, "2e-11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000002, 6U, "2e-12", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000002, 6U, "2e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000000002, 6U, "2e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000000002, 6U, "2e-15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000000002, 6U, "2e-16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.5, 6U, "1.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.05, 6U, "1.05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.005, 6U, "1.005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0005, 6U, "1.0005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00005, 6U, "1.00005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000005, 6U, "1.00001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000005, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000005, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000005, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000005, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000005, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000005, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000005, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000000005, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000000005, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000000005, 6U, "1", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.9, 6U, "1.9", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.09, 6U, "1.09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.009, 6U, "1.009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0009, 6U, "1.0009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00009, 6U, "1.00009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000009, 6U, "1.00001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000009, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000009, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000009, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000009, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000009, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000009, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000009, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000000009, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000000009, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000000009, 6U, "1", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.1, 6U, "1.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.01, 6U, "1.01", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.001, 6U, "1.001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0001, 6U, "1.0001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00001, 6U, "1.00001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000001, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000001, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000001, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000001, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000001, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000001, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000001, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000001, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000000001, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000000001, 6U, "1", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.1000000000000006, 6U, "0.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000000011, 6U, "1.1e-14", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.02, 6U, "0.02", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.002, 6U, "0.002", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0002, 6U, "0.0002", "return", __LINE__);

    RealToStreamEqual(helper, stream, 123456789.0, 6U, "1.23457e+08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2147483647.0, 6U, "2.14748e+09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4294967295.0, 6U, "4.29497e+09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4294967295.625, 6U, "4.29497e+09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2251799813685247.0, 6U, "2.2518e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4503599627370495.0, 6U, "4.5036e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4611686018427387903.0, 6U, "4.61169e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9223372036854775806.0, 6U, "9.22337e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9223372036854775807.0, 6U, "9.22337e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551615.0, 6U, "1.84467e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551616.0, 6U, "1.84467e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 34028236692093846341.0, 6U, "3.40282e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463412.0, 6U, "3.40282e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3402823669209384634633746074317.0, 6U, "3.40282e+30", "return", __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463426481119284341.0, 6U, "3.40282e+32", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3402823669209384634264811192843414.0, 6U, "3.40282e+33", "return", __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463426481119284349108225.0, 6U, "3.40282e+38", "return", __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463463374607431768211455.0, 6U, "3.40282e+38", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3689348814741910333.0, 6U, "3.68935e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 36893488147419103230.0, 6U, "3.68935e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 36893488147419103333.0, 6U, "3.68935e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 368934881474191033333.0, 6U, "3.68935e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9007199254740992.0, 6U, "9.0072e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18014398509481984.0, 6U, "1.80144e+16", "return", __LINE__);
    RealToStreamEqual(helper, stream, 36028797018963968.0, 6U, "3.60288e+16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9000000000000006, 6U, "0.9", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9.999999999999999, 6U, "10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1999999.0, 6U, "2e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999999999999999.0, 6U, "1e+16", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999999999999999999.0, 6U, "1e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999999999999999999.0, 6U, "1e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99999999999999999999.0, 6U, "1e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999999999999999999999999999999999999999.0, 6U, "1e+40", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99e100, 6U, "9.9e+101", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.999, 6U, "0.999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.999999, 6U, "0.999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.999999999, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.999999999999999, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999999, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999999, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999995, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999959, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999996, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999999999959, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999999999999, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999999999999999999999999999, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999999999999999999999999999999999999, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999999e-99, 6U, "1e-99", "return", __LINE__);

    RealToStreamEqual(helper, stream, 10000005.0, 6U, "1e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 10000005.0, 7U, "1e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 10000006.0, 7U, "1.000001e+07", "return", __LINE__);

    RealToStreamEqual(helper, stream, 10000000.0, 6U, "1e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999999.0, 6U, "1e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 19999999.0, 6U, "2e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 29999999.0, 6U, "3e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 39999999.0, 6U, "4e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 49999999.0, 6U, "5e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 59999999.0, 6U, "6e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 69999999.0, 6U, "7e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 79999999.0, 6U, "8e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 89999999.0, 6U, "9e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99999999.0, 6U, "1e+08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999999999.0, 6U, "1e+09", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.10000000, 6U, "0.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.19999999, 6U, "0.2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.29999999, 6U, "0.3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.39999999, 6U, "0.4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.49999999, 6U, "0.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.59999999, 6U, "0.6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.69999999, 6U, "0.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.79999999, 6U, "0.8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.89999999, 6U, "0.9", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.99999999, 6U, "2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.99999999, 6U, "3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.99999999, 6U, "4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4.99999999, 6U, "5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.99999999, 6U, "6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 6.99999999, 6U, "7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.99999999, 6U, "8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.99999999, 6U, "9", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.99999999, 6U, "10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99.99999999, 6U, "100", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999.99999999, 6U, "1000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99999.89999999, 6U, "99999.9", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99999.99999999, 6U, "100000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999999.99999999, 6U, "1e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9e-50, 6U, "9e-50", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1e1, 6U, "10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e2, 6U, "100", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e3, 6U, "1000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e4, 6U, "10000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e5, 6U, "100000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e10, 6U, "1e+10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e13, 6U, "1e+13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e14, 6U, "1e+14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e15, 6U, "1e+15", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1e-1, 6U, "0.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-2, 6U, "0.01", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-3, 6U, "0.001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-4, 6U, "0.0001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-5, 6U, "1e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-10, 6U, "1e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-13, 6U, "1e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-14, 6U, "1e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-15, 6U, "1e-15", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1e-50, 6U, "1e-50", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e+50, 6U, "1e+50", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-100, 6U, "1e-100", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e+100, 6U, "1e+100", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-200, 6U, "1e-200", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e+200, 6U, "1e+200", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-300, 6U, "1e-300", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e+300, 6U, "1e+300", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-307, 6U, "1e-307", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-308, 6U, "1e-308", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e+308, 6U, "1e+308", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-309, 6U, "1e-309", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-310, 6U, "1e-310", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-311, 6U, "1e-311", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.25e-50, 6U, "1.25e-50", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.25e+50, 6U, "1.25e+50", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.125e-50, 6U, "1.125e-50", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.125e+50, 6U, "1.125e+50", "return", __LINE__);

    RealToStreamEqual(helper, stream, 4.9406564584124654e-324, 6U, "4.94066e-324", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.7976931348623157e+308, 6U, "1.79769e+308", "return", __LINE__);
    RealToStreamEqual(helper, stream, -4.9406564584124654e-324, 6U, "-4.94066e-324", "return", __LINE__);
    RealToStreamEqual(helper, stream, -1.7976931348623157e+308, 6U, "-1.79769e+308", "return", __LINE__);

    RealToStreamEqual(helper, stream, -854534.5, 6U, "-854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, -854534.05, 6U, "-854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.5, 6U, "854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.05, 6U, "854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9765625.0, 6U, "-9.76562e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.2765625, 6U, "-0.276562", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9765625.0, 6U, "9.76562e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765645.0, 6U, "9.76564e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765665.0, 6U, "9.76566e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765685.0, 6U, "9.76568e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765605.0, 6U, "9.7656e+06", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9765615.0, 6U, "9.76562e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765635.0, 6U, "9.76564e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765655.0, 6U, "9.76566e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765675.0, 6U, "9.76568e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765695.0, 6U, "9.7657e+06", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9999425.0, 6U, "9.99942e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999445.0, 6U, "9.99944e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999465.0, 6U, "9.99946e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999485.0, 6U, "9.99948e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999405.0, 6U, "9.9994e+06", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9.999925, 6U, "9.99992", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.999945, 6U, "9.99995", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.999965, 6U, "9.99996", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.999985, 6U, "9.99999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.999905, 6U, "9.99991", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.765625, 6U, "1.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1765625, 6U, "0.176563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.2765625, 6U, "0.276562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3765625, 6U, "0.376563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.4765625, 6U, "0.476562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.5765625, 6U, "0.576562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.6765625, 6U, "0.676562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.7765625, 6U, "0.776563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.8765625, 6U, "0.876563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765625, 6U, "0.976562", "return", __LINE__);

    RealToStreamEqual(helper, stream, 2.165625, 6U, "2.16562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.265625, 6U, "2.26562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.365625, 6U, "2.36563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.465625, 6U, "2.46563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.565625, 6U, "2.56562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.665625, 6U, "2.66562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.765625, 6U, "2.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.865625, 6U, "2.86563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.965625, 6U, "2.96563", "return", __LINE__);

    RealToStreamEqual(helper, stream, 5.165625, 6U, "5.16563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.265625, 6U, "5.26562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.365625, 6U, "5.36562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.465625, 6U, "5.46563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.565625, 6U, "5.56562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.665625, 6U, "5.66563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.765625, 6U, "5.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.865625, 6U, "5.86562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.965625, 6U, "5.96563", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9.765615, 6U, "9.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625, 6U, "9.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765635, 6U, "9.76563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765645, 6U, "9.76564", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765655, 6U, "9.76566", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765665, 6U, "9.76567", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765675, 6U, "9.76567", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765685, 6U, "9.76568", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765695, 6U, "9.76569", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765615, 6U, "0.976561", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765625, 6U, "0.976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765635, 6U, "0.976564", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765645, 6U, "0.976564", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765655, 6U, "0.976565", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765665, 6U, "0.976567", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765675, 6U, "0.976568", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765685, 6U, "0.976568", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765695, 6U, "0.976569", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-1, 6U, "0.976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-2, 6U, "0.0976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-3, 6U, "0.00976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-4, 6U, "0.000976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-5, 6U, "9.76563e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-10, 6U, "9.76562e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-13, 6U, "9.76563e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-14, 6U, "9.76563e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-15, 6U, "9.76562e-15", "return", __LINE__);

    RealToStreamEqual(helper, stream, -9765625.0, 6U, "-9.76562e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9765645.0, 6U, "-9.76564e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9765665.0, 6U, "-9.76566e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9765685.0, 6U, "-9.76568e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9765605.0, 6U, "-9.7656e+06", "return", __LINE__);

    RealToStreamEqual(helper, stream, -9765615.0, 6U, "-9.76562e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9765635.0, 6U, "-9.76564e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9765655.0, 6U, "-9.76566e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9765675.0, 6U, "-9.76568e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9765695.0, 6U, "-9.7657e+06", "return", __LINE__);

    RealToStreamEqual(helper, stream, -9999425.0, 6U, "-9.99942e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9999445.0, 6U, "-9.99944e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9999465.0, 6U, "-9.99946e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9999485.0, 6U, "-9.99948e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9999405.0, 6U, "-9.9994e+06", "return", __LINE__);

    RealToStreamEqual(helper, stream, -9.999925, 6U, "-9.99992", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.999945, 6U, "-9.99995", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.999965, 6U, "-9.99996", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.999985, 6U, "-9.99999", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.999905, 6U, "-9.99991", "return", __LINE__);

    RealToStreamEqual(helper, stream, -1.765625, 6U, "-1.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.1765625, 6U, "-0.176563", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.2765625, 6U, "-0.276562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.3765625, 6U, "-0.376563", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.4765625, 6U, "-0.476562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.5765625, 6U, "-0.576562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.6765625, 6U, "-0.676562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.7765625, 6U, "-0.776563", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.8765625, 6U, "-0.876563", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.9765625, 6U, "-0.976562", "return", __LINE__);

    RealToStreamEqual(helper, stream, -2.165625, 6U, "-2.16562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -2.265625, 6U, "-2.26562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -2.365625, 6U, "-2.36563", "return", __LINE__);
    RealToStreamEqual(helper, stream, -2.465625, 6U, "-2.46563", "return", __LINE__);
    RealToStreamEqual(helper, stream, -2.565625, 6U, "-2.56562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -2.665625, 6U, "-2.66562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -2.765625, 6U, "-2.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -2.865625, 6U, "-2.86563", "return", __LINE__);
    RealToStreamEqual(helper, stream, -2.965625, 6U, "-2.96563", "return", __LINE__);

    RealToStreamEqual(helper, stream, -5.165625, 6U, "-5.16563", "return", __LINE__);
    RealToStreamEqual(helper, stream, -5.265625, 6U, "-5.26562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -5.365625, 6U, "-5.36562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -5.465625, 6U, "-5.46563", "return", __LINE__);
    RealToStreamEqual(helper, stream, -5.565625, 6U, "-5.56562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -5.665625, 6U, "-5.66563", "return", __LINE__);
    RealToStreamEqual(helper, stream, -5.765625, 6U, "-5.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -5.865625, 6U, "-5.86562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -5.965625, 6U, "-5.96563", "return", __LINE__);

    RealToStreamEqual(helper, stream, -9.765615, 6U, "-9.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765625, 6U, "-9.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765635, 6U, "-9.76563", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765645, 6U, "-9.76564", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765655, 6U, "-9.76566", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765665, 6U, "-9.76567", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765675, 6U, "-9.76567", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765685, 6U, "-9.76568", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765695, 6U, "-9.76569", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.9765615, 6U, "-0.976561", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.9765625, 6U, "-0.976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.9765635, 6U, "-0.976564", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.9765645, 6U, "-0.976564", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.9765655, 6U, "-0.976565", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.9765665, 6U, "-0.976567", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.9765675, 6U, "-0.976568", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.9765685, 6U, "-0.976568", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.9765695, 6U, "-0.976569", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765625e-1, 6U, "-0.976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765625e-2, 6U, "-0.0976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765625e-3, 6U, "-0.00976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765625e-4, 6U, "-0.000976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765625e-5, 6U, "-9.76563e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765625e-10, 6U, "-9.76562e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765625e-13, 6U, "-9.76563e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765625e-14, 6U, "-9.76563e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9.765625e-15, 6U, "-9.76562e-15", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.00001844674407370955161500, 6U, "1.84467e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0001844674407370955161500, 6U, "0.000184467", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.001844674407370955161500, 6U, "0.00184467", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.001844674407370955161500, 6U, "5.00184", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.01844674407370955161500, 6U, "0.0184467", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1844674407370955161500, 6U, "0.184467", "return", __LINE__);

    RealToStreamEqual(helper, stream, 18446744073709551614.0, 6U, "1.84467e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551620.0, 6U, "1.84467e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 184467440737095516150.0, 6U, "1.84467e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1844674407370955161500.0, 6U, "1.84467e+21", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551615e99, 6U, "1.84467e+118", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551615e100, 6U, "1.84467e+119", "return", __LINE__);
    RealToStreamEqual(helper, stream, 17446744073709551614.0, 6U, "1.74467e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9923372036854775809.0, 6U, "9.92337e+18", "return", __LINE__);

    RealToStreamEqual(helper, stream, 4.76837158203125e-50, 6U, "4.76837e-50", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.384185791015625e-50, 6U, "2.38419e-50", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.05, 6U, "0.05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.005, 6U, "0.005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0005, 6U, "0.0005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00005, 6U, "5e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000005, 6U, "5e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.55, 6U, "1.55", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0055, 6U, "1.0055", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.005999, 6U, "1.006", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.009999, 6U, "1.01", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00055, 6U, "0.00055", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00055, 6U, "1.00055", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.50009, 6U, "0.50009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00055e-100, 6U, "1.00055e-100", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.50009e-100, 6U, "5.0009e-101", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000050000000005, 6U, "5e-11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000005, 6U, "5e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000000000000000000005, 6U, "5e-28", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.3125, 6U, "5.3125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.3, 6U, "5.3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.5, 6U, "5.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.55, 6U, "0.55", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.25, 6U, "0.25", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.75, 6U, "0.75", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.125, 6U, "0.125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.625, 6U, "0.625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3125, 6U, "0.3125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.95367431640625, 6U, "0.953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.298023223876953125, 6U, "0.298023", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1490116119384765625, 6U, "0.149012", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.7450580596923828125, 6U, "0.745058", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.37252902984619140625, 6U, "0.372529", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.186264514923095703125, 6U, "0.186265", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.12345, 6U, "1.12345", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.953674, 6U, "1.95367", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0953674, 6U, "1.09537", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00953674, 6U, "1.00954", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000953674, 6U, "1.00095", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.953674, 6U, "9.95367", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.0953674, 6U, "9.09537", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.00953674, 6U, "9.00954", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.000953674, 6U, "9.00095", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.953674, 6U, "0.953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0953674, 6U, "0.0953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00953674, 6U, "0.00953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000953674, 6U, "0.000953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.123455, 6U, "0.123455", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1123405, 6U, "0.11234", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1123095, 6U, "0.11231", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.78126, 6U, "0.78126", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.300048828125, 6U, "0.300049", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.111, 6U, "0.111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.11111, 6U, "0.11111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1111111111111111, 6U, "0.111111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1111111111111115, 6U, "0.111111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.11111111111111159, 6U, "0.111111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1111111111111116, 6U, "0.111111", "return", __LINE__);

    RealToStreamEqual(helper, stream, 105.625, 6U, "105.625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 25e20, 6U, "2.5e+21", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.5e21, 6U, "2.5e+21", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3814697265625.0, 6U, "3.8147e+12", "return", __LINE__);
    RealToStreamEqual(helper, stream, 381469725.0, 6U, "3.8147e+08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3814697265625, 6U, "0.38147", "return", __LINE__);
    RealToStreamEqual(helper, stream, 10.5, 6U, "10.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 10.0, 6U, "10", "return", __LINE__);

    RealToStreamEqual(helper, stream, 2097151.125, 6U, "2.09715e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2097151.0, 6U, "2.09715e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 16217975.0, 6U, "1.6218e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 32995191.0, 6U, "3.29952e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 444444444444444444444.0, 6U, "4.44444e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 555555555555555555555.0, 6U, "5.55556e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3668262915145728.0, 6U, "3.66826e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7336525830291456.0, 6U, "7.33653e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7336535830291456.0, 6U, "7.33654e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 14673051660582912.0, 6U, "1.46731e+16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1000000.390625, 6U, "1e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1000009.590625, 6U, "1.00001e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1000008.590625, 6U, "1.00001e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.3365258e-10, 6U, "7.33653e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.3365258e-10, 8U, "7.3365258e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3333, 6U, "0.3333", "return", __LINE__);

    RealToStreamEqual(helper, stream, 4554534.0, 6U, "4.55453e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.055, 6U, "854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.0055, 6U, "854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.000055, 6U, "854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.0000005, 14U, "854534.0000005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.0000005, 6U, "8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.000005, 6U, "8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.005, 6U, "8.005", "return", __LINE__);

    RealToStreamEqual(helper, stream, 5.55, 1U, "6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.996, 5U, "9.996", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.996, 4U, "9.996", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.996, 3U, "10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.996, 2U, "10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.996, 1U, "1e+01", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.5, 1U, "0.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.5, 1U, "2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.5, 1U, "2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.5, 1U, "4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4.5, 1U, "4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.25, 1U, "2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.75, 1U, "3", "return", __LINE__);

    RealToStreamEqual(helper, stream, 2.625, 2U, "2.6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.65, 2U, "2.6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.25, 2U, "2.2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.25, 2U, "3.2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.625, 2U, "2.6", "return", __LINE__);

    RealToStreamEqual(helper, stream, 2.635, 2U, "2.6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.35, 2U, "2.4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.35, 2U, "3.4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.635, 2U, "2.6", "return", __LINE__);

    RealToStreamEqual(helper, stream, 91.0, 1U, "9e+01", "return", __LINE__);
    RealToStreamEqual(helper, stream, 95.0, 1U, "1e+02", "return", __LINE__);

    RealToStreamEqual(helper, stream, 95000000.0, 1U, "1e+08", "return", __LINE__);

    RealToStreamEqual(helper, stream, 3.0, 15U, "3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.14, 15U, "3.14", "return", __LINE__);

    RealToStreamEqual(helper, stream, 140737488355327.953674, 15U, "140737488355328", "return", __LINE__);
    RealToStreamEqual(helper, stream, 140737488355327.015625, 15U, "140737488355327", "return", __LINE__);

    RealToStreamEqual(helper, stream, 140737458355327.953674, 15U, "140737458355328", "return", __LINE__);
    RealToStreamEqual(helper, stream, 140737488355327.953674, 6U, "1.40737e+14", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.95367431640625, 20U, "0.95367431640625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.298023223876953125, 20U, "0.298023223876953125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1490116119384765625, 20U, "0.1490116119384765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.7450580596923828125, 20U, "0.7450580596923828125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.37252902984619140625, 20U, "0.37252902984619140625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.186264514923095703125, 20U, "0.18626451492309570312", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.186264514923095703125, 21U, "0.186264514923095703125", "return", __LINE__);

    RealToStreamEqual(helper, stream, 18446744073709551614.0, 20U, "18446744073709551616", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551620.0, 20U, "18446744073709551616", "return", __LINE__);
    RealToStreamEqual(helper, stream, 184467440737095516150.0, 20U, "1.8446744073709551616e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1844674407370955161500.0, 20U, "1.8446744073709551616e+21", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551615e99, 20U, "1.8446744073709551013e+118", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551615e100, 20U, "1.8446744073709551909e+119", "return", __LINE__);
    RealToStreamEqual(helper, stream, 17446744073709551614.0, 21U, "17446744073709551616", "return", __LINE__);
    RealToStreamEqual(helper, stream, 17446744073709551614.0, 20U, "17446744073709551616", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9923372036854775809.0, 21U, "9923372036854775808", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9923372036854775809.0, 20U, "9923372036854775808", "return", __LINE__);

    RealToStreamEqual(helper, stream, 2251799813685247.0, 21U, "2251799813685247", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4503599627370495.0, 21U, "4503599627370495", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4611686018427387903.0, 21U, "4611686018427387904", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9223372036854775806.0, 21U, "9223372036854775808", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9223372036854775807.0, 21U, "9223372036854775808", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551615.0, 21U, "18446744073709551616", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551616.0, 21U, "18446744073709551616", "return", __LINE__);
    RealToStreamEqual(helper, stream, 34028236692093846341.0, 21U, "34028236692093845504", "return", __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463412.0, 21U, "340282366920938487808", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3402823669209384634633746074317.0, 34U, "3402823669209384705829531287552",
                      "return", __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463426481119284341.0, 34U, "340282366920938448064954991902720",
                      "return", __LINE__);
    RealToStreamEqual(helper, stream, 3402823669209384634264811192843414.0, 34U, "3402823669209384912995114146594816",
                      "return", __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463426481119284349108225.0, 39U,
                      "340282366920938463463374607431768211456", "return", __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463463374607431768211455.0, 39U,
                      "340282366920938463463374607431768211456", "return", __LINE__);
}

template <typename Stream_T_>
static void TestDoubleToString2(TestHelper &helper, Stream_T_ &stream) {
    RealToStreamEqual(helper, stream, -0.0, 15U, "-0", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0, 15U, "0", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.0, 15U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.0, 15U, "2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.0, 15U, "3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4.0, 15U, "4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.0, 15U, "5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 6.0, 15U, "6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.0, 15U, "7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.0, 15U, "8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.0, 15U, "9", "return", __LINE__);
    RealToStreamEqual(helper, stream, 220.0, 15U, "220", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1, 15U, "0.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.2, 15U, "0.2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3, 15U, "0.3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.4, 15U, "0.4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.5, 15U, "0.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.6, 15U, "0.6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.7, 15U, "0.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.8, 15U, "0.8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9, 15U, "0.9", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.09, 15U, "0.09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.009, 15U, "0.009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0009, 15U, "0.0009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00009, 15U, "9e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000009, 15U, "9e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000009, 15U, "9e-07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000009, 15U, "9e-08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000009, 15U, "9e-09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000009, 15U, "9e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000009, 15U, "9e-11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000009, 15U, "9e-12", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000009, 15U, "9e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000000009, 15U, "9e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000000009, 15U, "9e-15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000000009, 15U, "9e-16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.01, 15U, "0.01", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.001, 15U, "0.001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0001, 15U, "0.0001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00001, 15U, "1e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000001, 15U, "1e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000001, 15U, "1e-07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000001, 15U, "1e-08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000001, 15U, "1e-09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000001, 15U, "1e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000001, 15U, "1e-11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000001, 15U, "1e-12", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000001, 15U, "1e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000000001, 15U, "1e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000000001, 15U, "1e-15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000000001, 15U, "1e-16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.02, 15U, "0.02", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.002, 15U, "0.002", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0002, 15U, "0.0002", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00002, 15U, "2e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000002, 15U, "2e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000002, 15U, "2e-07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000002, 15U, "2e-08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000002, 15U, "2e-09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000002, 15U, "2e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000002, 15U, "2e-11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000002, 15U, "2e-12", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000002, 15U, "2e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000000002, 15U, "2e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000000002, 15U, "2e-15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000000002, 15U, "2e-16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.5, 15U, "1.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.05, 15U, "1.05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.005, 15U, "1.005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0005, 15U, "1.0005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00005, 15U, "1.00005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000005, 15U, "1.000005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000005, 15U, "1.0000005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000005, 15U, "1.00000005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000005, 15U, "1.000000005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000005, 15U, "1.0000000005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000005, 15U, "1.00000000005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000005, 15U, "1.000000000005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000005, 15U, "1.0000000000005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000000005, 15U, "1.00000000000005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000000005, 15U, "1.00000000000001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000000005, 15U, "1", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.9, 15U, "1.9", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.09, 15U, "1.09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.009, 15U, "1.009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0009, 15U, "1.0009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00009, 15U, "1.00009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000009, 15U, "1.000009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000009, 15U, "1.0000009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000009, 15U, "1.00000009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000009, 15U, "1.000000009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000009, 15U, "1.0000000009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000009, 15U, "1.00000000009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000009, 15U, "1.000000000009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000009, 15U, "1.0000000000009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000000009, 15U, "1.00000000000009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000000009, 15U, "1.00000000000001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000000009, 15U, "1", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.1, 15U, "1.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.01, 15U, "1.01", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.001, 15U, "1.001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0001, 15U, "1.0001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00001, 15U, "1.00001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000001, 15U, "1.000001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000001, 15U, "1.0000001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000001, 15U, "1.00000001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000001, 15U, "1.000000001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000001, 15U, "1.0000000001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000001, 15U, "1.00000000001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000001, 15U, "1.000000000001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000001, 15U, "1.0000000000001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000000001, 15U, "1.00000000000001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000000001, 15U, "1", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.1000000000000006, 15U, "0.100000000000001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000000011, 15U, "1.1e-14", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.02, 15U, "0.02", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.002, 15U, "0.002", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0002, 15U, "0.0002", "return", __LINE__);

    RealToStreamEqual(helper, stream, 123456789.0, 15U, "123456789", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2147483647.0, 15U, "2147483647", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4294967295.0, 15U, "4294967295", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4294967295.625, 15U, "4294967295.625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2251799813685247.0, 15U, "2.25179981368525e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4503599627370495.0, 15U, "4.5035996273705e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4611686018427387903.0, 15U, "4.61168601842739e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9223372036854775806.0, 15U, "9.22337203685478e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9223372036854775807.0, 15U, "9.22337203685478e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551615.0, 15U, "1.84467440737096e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551616.0, 15U, "1.84467440737096e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 34028236692093846341.0, 15U, "3.40282366920938e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463412.0, 15U, "3.40282366920938e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3402823669209384634633746074317.0, 15U, "3.40282366920938e+30", "return",
                      __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463426481119284341.0, 15U, "3.40282366920938e+32", "return",
                      __LINE__);
    RealToStreamEqual(helper, stream, 3402823669209384634264811192843414.0, 15U, "3.40282366920938e+33", "return",
                      __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463426481119284349108225.0, 15U, "3.40282366920938e+38", "return",
                      __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463463374607431768211455.0, 15U, "3.40282366920938e+38", "return",
                      __LINE__);
    RealToStreamEqual(helper, stream, 3689348814741910333.0, 15U, "3.68934881474191e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 36893488147419103230.0, 15U, "3.68934881474191e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 36893488147419103333.0, 15U, "3.68934881474191e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 368934881474191033333.0, 15U, "3.68934881474191e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9007199254740992.0, 15U, "9.00719925474099e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18014398509481984.0, 15U, "1.8014398509482e+16", "return", __LINE__);
    RealToStreamEqual(helper, stream, 36028797018963968.0, 15U, "3.6028797018964e+16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9000000000000006, 15U, "0.900000000000001", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9.999999999999999, 15U, "10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1999999.0, 15U, "1999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999999999999999.0, 15U, "1e+16", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999999999999999999.0, 15U, "1e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999999999999999999.0, 15U, "1e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99999999999999999999.0, 15U, "1e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999999999999999999999999999999999999999.0, 15U, "1e+40", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99e100, 15U, "9.9e+101", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.999999999, 15U, "0.999999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999999, 15U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.999, 15U, "0.999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.999999, 15U, "0.999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.999999999999999, 15U, "0.999999999999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999999, 15U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999995, 15U, "0.999999999999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999959, 15U, "0.999999999999996", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999996, 15U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999999999959, 15U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999999999999, 15U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999999999999999999999999999, 15U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999999999999999999999999999999999999, 15U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999999e-99, 15U, "1e-99", "return", __LINE__);

    RealToStreamEqual(helper, stream, 10000005.0, 15U, "10000005", "return", __LINE__);

    RealToStreamEqual(helper, stream, 10000000.0, 15U, "10000000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999999.0, 15U, "9999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 19999999.0, 15U, "19999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 29999999.0, 15U, "29999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 39999999.0, 15U, "39999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 49999999.0, 15U, "49999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 59999999.0, 15U, "59999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 69999999.0, 15U, "69999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 79999999.0, 15U, "79999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 89999999.0, 15U, "89999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99999999.0, 15U, "99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999999999.0, 15U, "999999999", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.10000000, 15U, "0.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999, 15U, "0.9999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.19999999, 15U, "0.19999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.29999999, 15U, "0.29999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.39999999, 15U, "0.39999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.49999999, 15U, "0.49999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.59999999, 15U, "0.59999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.69999999, 15U, "0.69999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.79999999, 15U, "0.79999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.89999999, 15U, "0.89999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999, 15U, "0.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.99999999, 15U, "1.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.99999999, 15U, "2.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.99999999, 15U, "3.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4.99999999, 15U, "4.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.99999999, 15U, "5.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 6.99999999, 15U, "6.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.99999999, 15U, "7.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.99999999, 15U, "8.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.99999999, 15U, "9.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99.99999999, 15U, "99.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999.99999999, 15U, "999.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99999.89999999, 15U, "99999.89999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99999.99999999, 15U, "99999.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999999.99999999, 15U, "999999.99999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9e-50, 15U, "9e-50", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1e1, 15U, "10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e2, 15U, "100", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e3, 15U, "1000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e4, 15U, "10000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e5, 15U, "100000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e10, 15U, "10000000000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e13, 15U, "10000000000000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e14, 15U, "100000000000000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e15, 15U, "1e+15", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1e-1, 15U, "0.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-2, 15U, "0.01", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-3, 15U, "0.001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-4, 15U, "0.0001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-5, 15U, "1e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-10, 15U, "1e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-13, 15U, "1e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-14, 15U, "1e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-15, 15U, "1e-15", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1e-50, 15U, "1e-50", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e+50, 15U, "1e+50", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-100, 15U, "1e-100", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e+100, 15U, "1e+100", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-200, 15U, "1e-200", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e+200, 15U, "1e+200", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-300, 15U, "1e-300", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e+300, 15U, "1e+300", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-307, 15U, "1e-307", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-308, 15U, "1e-308", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e+308, 15U, "1e+308", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-309, 15U, "1e-309", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-310, 15U, "9.99999999999997e-311", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-311, 15U, "9.99999999999948e-312", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.25e-50, 15U, "1.25e-50", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.25e+50, 15U, "1.25e+50", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.125e-50, 15U, "1.125e-50", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.125e+50, 15U, "1.125e+50", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.40129846e-45, 6U, "1.4013e-45", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.40282347e+38, 6U, "3.40282e+38", "return", __LINE__);
    RealToStreamEqual(helper, stream, -1.40129846e-45, 6U, "-1.4013e-45", "return", __LINE__);
    RealToStreamEqual(helper, stream, -3.40282347e+38, 6U, "-3.40282e+38", "return", __LINE__);

    RealToStreamEqual(helper, stream, 4.9406564584124654e-324, 15U, "4.94065645841247e-324", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.7976931348623157e+308, 15U, "1.79769313486232e+308", "return", __LINE__);
    RealToStreamEqual(helper, stream, -4.9406564584124654e-324, 15U, "-4.94065645841247e-324", "return", __LINE__);
    RealToStreamEqual(helper, stream, -1.7976931348623157e+308, 15U, "-1.79769313486232e+308", "return", __LINE__);

    RealToStreamEqual(helper, stream, -854534.5, 15U, "-854534.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, -854534.05, 15U, "-854534.05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.5, 15U, "854534.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.05, 15U, "854534.05", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9765625.0, 15U, "-9765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.2765625, 15U, "-0.2765625", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9765625.0, 15U, "9765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765645.0, 15U, "9765645", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765665.0, 15U, "9765665", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765685.0, 15U, "9765685", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765605.0, 15U, "9765605", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9765615.0, 15U, "9765615", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765635.0, 15U, "9765635", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765655.0, 15U, "9765655", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765675.0, 15U, "9765675", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765695.0, 15U, "9765695", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9999425.0, 15U, "9999425", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999445.0, 15U, "9999445", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999465.0, 15U, "9999465", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999485.0, 15U, "9999485", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999405.0, 15U, "9999405", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9.999925, 15U, "9.999925", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.999945, 15U, "9.999945", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.999965, 15U, "9.999965", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.999985, 15U, "9.999985", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.999905, 15U, "9.999905", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.765625, 15U, "1.765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1765625, 15U, "0.1765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.2765625, 15U, "0.2765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3765625, 15U, "0.3765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.4765625, 15U, "0.4765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.5765625, 15U, "0.5765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.6765625, 15U, "0.6765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.7765625, 15U, "0.7765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.8765625, 15U, "0.8765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765625, 15U, "0.9765625", "return", __LINE__);

    RealToStreamEqual(helper, stream, 2.165625, 15U, "2.165625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.265625, 15U, "2.265625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.365625, 15U, "2.365625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.465625, 15U, "2.465625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.565625, 15U, "2.565625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.665625, 15U, "2.665625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.765625, 15U, "2.765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.865625, 15U, "2.865625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.965625, 15U, "2.965625", "return", __LINE__);

    RealToStreamEqual(helper, stream, 5.165625, 15U, "5.165625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.265625, 15U, "5.265625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.365625, 15U, "5.365625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.465625, 15U, "5.465625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.565625, 15U, "5.565625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.665625, 15U, "5.665625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.765625, 15U, "5.765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.865625, 15U, "5.865625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.965625, 15U, "5.965625", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9.765615, 15U, "9.765615", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625, 15U, "9.765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765635, 15U, "9.765635", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765645, 15U, "9.765645", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765655, 15U, "9.765655", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765665, 15U, "9.765665", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765675, 15U, "9.765675", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765685, 15U, "9.765685", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765695, 15U, "9.765695", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765615, 15U, "0.9765615", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765625, 15U, "0.9765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765635, 15U, "0.9765635", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765645, 15U, "0.9765645", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765655, 15U, "0.9765655", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765665, 15U, "0.9765665", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765675, 15U, "0.9765675", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765685, 15U, "0.9765685", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765695, 15U, "0.9765695", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-1, 15U, "0.9765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-2, 15U, "0.09765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-3, 15U, "0.009765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-4, 15U, "0.0009765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-5, 15U, "9.765625e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-10, 15U, "9.765625e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-13, 15U, "9.765625e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-14, 15U, "9.765625e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-15, 15U, "9.765625e-15", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.00001844674407370955161500, 15U, "1.84467440737096e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0001844674407370955161500, 15U, "0.000184467440737096", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.001844674407370955161500, 15U, "0.00184467440737096", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.001844674407370955161500, 15U, "5.00184467440737", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.01844674407370955161500, 15U, "0.0184467440737095", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1844674407370955161500, 15U, "0.184467440737096", "return", __LINE__);

    RealToStreamEqual(helper, stream, 18446744073709551614.0, 15U, "1.84467440737096e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551620.0, 15U, "1.84467440737096e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 184467440737095516150.0, 15U, "1.84467440737096e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1844674407370955161500.0, 15U, "1.84467440737096e+21", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551615e99, 15U, "1.84467440737096e+118", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551615e100, 15U, "1.84467440737096e+119", "return", __LINE__);
    RealToStreamEqual(helper, stream, 17446744073709551614.0, 15U, "1.74467440737096e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9923372036854775809.0, 15U, "9.92337203685478e+18", "return", __LINE__);

    RealToStreamEqual(helper, stream, 4.76837158203125e-50, 15U, "4.76837158203125e-50", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.384185791015625e-50, 15U, "2.38418579101563e-50", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.05, 15U, "0.05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.005, 15U, "0.005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0005, 15U, "0.0005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00005, 15U, "5e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000005, 15U, "5e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.55, 15U, "1.55", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0055, 15U, "1.0055", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.005999, 15U, "1.005999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.009999, 15U, "1.009999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00055, 15U, "0.00055", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00055, 15U, "1.00055", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.50009, 15U, "0.50009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00055e-100, 15U, "1.00055e-100", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.50009e-100, 15U, "5.0009e-101", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000050000000005, 15U, "5.0000000005e-11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000005, 15U, "5e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000000000000000000005, 15U, "5e-28", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.3125, 15U, "5.3125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.3, 15U, "5.3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.5, 15U, "5.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.55, 15U, "0.55", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.25, 15U, "0.25", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.75, 15U, "0.75", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.125, 15U, "0.125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.625, 15U, "0.625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3125, 15U, "0.3125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.95367431640625, 15U, "0.95367431640625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.298023223876953125, 15U, "0.298023223876953", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1490116119384765625, 15U, "0.149011611938477", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.7450580596923828125, 15U, "0.745058059692383", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.37252902984619140625, 15U, "0.372529029846191", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.186264514923095703125, 15U, "0.186264514923096", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.12345, 15U, "1.12345", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.953674, 15U, "1.953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0953674, 15U, "1.0953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00953674, 15U, "1.00953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000953674, 15U, "1.000953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.953674, 15U, "9.953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.0953674, 15U, "9.0953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.00953674, 15U, "9.00953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.000953674, 15U, "9.000953674", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.953674, 15U, "0.953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0953674, 15U, "0.0953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00953674, 15U, "0.00953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000953674, 15U, "0.000953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.123455, 15U, "0.123455", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1123405, 15U, "0.1123405", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1123095, 15U, "0.1123095", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.78126, 15U, "0.78126", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.300048828125, 15U, "0.300048828125", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.111, 15U, "0.111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.11111, 15U, "0.11111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1111111111111111, 15U, "0.111111111111111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1111111111111115, 15U, "0.111111111111111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.11111111111111159, 15U, "0.111111111111112", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1111111111111116, 15U, "0.111111111111112", "return", __LINE__);

    RealToStreamEqual(helper, stream, 105.625, 15U, "105.625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 25e20, 15U, "2.5e+21", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.5e21, 15U, "2.5e+21", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3814697265625.0, 15U, "3814697265625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 381469725.0, 15U, "381469725", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3814697265625, 15U, "0.3814697265625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 10.5, 15U, "10.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 10.0, 15U, "10", "return", __LINE__);

    RealToStreamEqual(helper, stream, 2097151.125, 15U, "2097151.125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2097151.0, 15U, "2097151", "return", __LINE__);
    RealToStreamEqual(helper, stream, 16217975.0, 15U, "16217975", "return", __LINE__);
    RealToStreamEqual(helper, stream, 32995191.0, 15U, "32995191", "return", __LINE__);
    RealToStreamEqual(helper, stream, 444444444444444444444.0, 15U, "4.44444444444444e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 555555555555555555555.0, 15U, "5.55555555555556e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3668262915145728.0, 15U, "3.66826291514573e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7336525830291456.0, 15U, "7.33652583029146e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7336535830291456.0, 15U, "7.33653583029146e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 14673051660582912.0, 15U, "1.46730516605829e+16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1000000.390625, 15U, "1000000.390625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1000009.590625, 15U, "1000009.590625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1000008.590625, 15U, "1000008.590625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.3365258e-10, 15U, "7.3365258e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3333, 15U, "0.3333", "return", __LINE__);

    RealToStreamEqual(helper, stream, 4554534.0, 15U, "4554534", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.055, 15U, "854534.055", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.0055, 15U, "854534.0055", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.000055, 15U, "854534.000055", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.0000005, 15U, "8.0000005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.000005, 15U, "8.000005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.005, 15U, "8.005", "return", __LINE__);
}

template <typename Stream_T_>
static void TestDoubleToString3(TestHelper &helper, Stream_T_ &stream) {
    RealToStreamEqual(helper, stream, -1.0, 6U, "-1", "return", __LINE__);

    RealToStreamEqual(helper, stream, 70.0, 6U, "70", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7e+10, 6U, "7e+10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 75e+10, 6U, "7.5e+11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7e+50, 6U, "7e+50", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5000000000000000.25, 30U, "5000000000000000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5000000000000000.0, 6U, "5e+15", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9999998.0, 6U, "1e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999998.9, 6U, "1e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999999999.999999999, 6U, "1e+09", "return", __LINE__);

    RealToStreamEqual(helper, stream, 111.5, 6U, "111.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999.625, 6U, "999.625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e+33, 6U, "1e+33", "return", __LINE__);

    RealToStreamEqual(helper, stream, 18446744073709551615.0, 2U, "1.8e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.18446744073709551615, 2U, "0.18", "return", __LINE__);

    RealToStreamEqual(helper, stream, 10.7, 6U, "10.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 20.7, 6U, "20.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 30.7, 6U, "30.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 40.7, 6U, "40.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 50.7, 6U, "50.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 60.7, 6U, "60.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 70.7, 6U, "70.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 80.7, 6U, "80.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 90.7, 6U, "90.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 50.25, 6U, "50.25", "return", __LINE__);
    RealToStreamEqual(helper, stream, 500000000000000.25, 6U, "5e+14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 500000000000000.25, 18U, "500000000000000.25", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.7, 6U, "7.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.000007, 8U, "7.000007", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.25, 6U, "1.25", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.07, 6U, "0.07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3125, 4U, "0.3125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5e-4, 6U, "0.0005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5e-10, 6U, "5e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5e-10, 10U, "5e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5e-10, 11U, "5e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7e-24, 6U, "7e-24", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.390625, 6U, "0.390625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000390625, 6U, "0.000390625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000390625, 6U, "3.90625e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000390625, 6U, "3.90625e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000009999999, 6U, "1e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 505.3125, 6U, "505.312", "return", __LINE__);
    RealToStreamEqual(helper, stream, 505.003125, 6U, "505.003", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1e+300, 50U, "1.0000000000000000525047602552044202487044685811082e+300", "return",
                      __LINE__);
    RealToStreamEqual(
        helper, stream, 1.7976931348623157e+308, 309U,
        "179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",
        "return", __LINE__);

    RealToStreamEqual(helper, stream, 1e-300, 50U, "1.0000000000000000250590918352087596856961468077037e-300", "return",
                      __LINE__);

    RealToStreamEqual(
        helper, stream, 4.9406564584124654e-324, 324U,
        "4.94065645841246544176568792868221372365059802614324764425585682500675507270208751865299836361635992379796564695445717730926656710355939796398774796010781878126300713190311404527845817167848982103688718636056998730723050006387409153564984387312473397273169615140031715385398074126238565591171026658556686768187039560310624932e-324",
        "return", __LINE__);
}

template <typename Stream_T_>
static void TestFloatToString1(TestHelper &helper, Stream_T_ &stream) {
    RealToStreamEqual(helper, stream, -0.0F, 6U, "-0", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0F, 6U, "0", "return", __LINE__);

#ifndef QENTEM_TEST_DIGIT_SKIP_INF_
    RealToStreamEqual(helper, stream, (3e+38f + 3e+38f), 6U, "inf", "return", __LINE__);
    RealToStreamEqual(helper, stream, (-3e+38f + -3e+38f), 6U, "-inf", "return", __LINE__);
#endif

#ifndef QENTEM_TEST_DIGIT_SKIP_NAN_
    RealToStreamEqual(helper, stream, ((3e+38f + 3e+38f) / (3e+38f + 3e+38f)), 6U, "nan", "return", __LINE__);
    RealToStreamEqual(helper, stream, (0.0f / 0.0f), 6U, "nan", "return", __LINE__);
#endif

    RealToStreamEqual(helper, stream, 1.0F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.0F, 6U, "2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.0F, 6U, "3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4.0F, 6U, "4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.0F, 6U, "5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 6.0F, 6U, "6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.0F, 6U, "7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.0F, 6U, "8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.0F, 6U, "9", "return", __LINE__);
    RealToStreamEqual(helper, stream, 220.0F, 6U, "220", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1F, 6U, "0.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.2F, 6U, "0.2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3F, 6U, "0.3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.4F, 6U, "0.4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.5F, 6U, "0.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.6F, 6U, "0.6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.7F, 6U, "0.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.8F, 6U, "0.8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9F, 6U, "0.9", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.09F, 6U, "0.09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.009F, 6U, "0.009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0009F, 6U, "0.0009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00009F, 6U, "9e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000009F, 6U, "9e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000009F, 6U, "9e-07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000009F, 6U, "9e-08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000009F, 6U, "9e-09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000009F, 6U, "9e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000009F, 6U, "9e-11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000009F, 6U, "9e-12", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000009F, 6U, "9e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000000009F, 6U, "9e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000000009F, 6U, "9e-15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000000009F, 6U, "9e-16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.01F, 6U, "0.01", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.001F, 6U, "0.001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0001F, 6U, "0.0001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00001F, 6U, "1e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000001F, 6U, "1e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000001F, 6U, "1e-07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000001F, 6U, "1e-08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000001F, 6U, "1e-09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000001F, 6U, "1e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000001F, 6U, "1e-11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000001F, 6U, "1e-12", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000001F, 6U, "1e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000000001F, 6U, "1e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000000001F, 6U, "1e-15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000000001F, 6U, "1e-16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.02F, 6U, "0.02", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.002F, 6U, "0.002", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0002F, 6U, "0.0002", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00002F, 6U, "2e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000002F, 6U, "2e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000002F, 6U, "2e-07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000002F, 6U, "2e-08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000002F, 6U, "2e-09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000002F, 6U, "2e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000002F, 6U, "2e-11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000002F, 6U, "2e-12", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000002F, 6U, "2e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000000000002F, 6U, "2e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000000002F, 6U, "2e-15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000000002F, 6U, "2e-16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.5F, 6U, "1.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.05F, 6U, "1.05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.005F, 6U, "1.005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0005F, 6U, "1.0005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00005F, 6U, "1.00005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000005F, 6U, "1.00001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000005F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000005F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000005F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000005F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000005F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000005F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000005F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000000005F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000000005F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000000005F, 6U, "1", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.9F, 6U, "1.9", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.09F, 6U, "1.09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.009F, 6U, "1.009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0009F, 6U, "1.0009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00009F, 6U, "1.00009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000009F, 6U, "1.00001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000009F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000009F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000009F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000009F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000009F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000009F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000009F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000000009F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000000009F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000000009F, 6U, "1", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.1F, 6U, "1.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.01F, 6U, "1.01", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.001F, 6U, "1.001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0001F, 6U, "1.0001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00001F, 6U, "1.00001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000001F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000001F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000001F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000001F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000001F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000001F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000001F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0000000000001F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00000000000001F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000000000000001F, 6U, "1", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.1000000000000006F, 6U, "0.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000000011F, 6U, "1.1e-14", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.02F, 6U, "0.02", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.002F, 6U, "0.002", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0002F, 6U, "0.0002", "return", __LINE__);

    RealToStreamEqual(helper, stream, 123456789.0F, 6U, "1.23457e+08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2147483647.0F, 6U, "2.14748e+09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4294967295.0F, 6U, "4.29497e+09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4294967295.625F, 6U, "4.29497e+09", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2251799813685247.0F, 6U, "2.2518e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4503599627370495.0F, 6U, "4.5036e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4611686018427387903.0F, 6U, "4.61169e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9223372036854775806.0F, 6U, "9.22337e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9223372036854775807.0F, 6U, "9.22337e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551615.0F, 6U, "1.84467e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551616.0F, 6U, "1.84467e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 34028236692093846341.0F, 6U, "3.40282e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463412.0F, 6U, "3.40282e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3402823669209384634633746074317.0F, 6U, "3.40282e+30", "return", __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463426481119284341.0F, 6U, "3.40282e+32", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3402823669209384634264811192843414.0F, 6U, "3.40282e+33", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3689348814741910333.0F, 6U, "3.68935e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 36893488147419103230.0F, 6U, "3.68935e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 36893488147419103333.0F, 6U, "3.68935e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 368934881474191033333.0F, 6U, "3.68935e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9007199254740992.0F, 6U, "9.0072e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18014398509481984.0F, 6U, "1.80144e+16", "return", __LINE__);
    RealToStreamEqual(helper, stream, 36028797018963968.0F, 6U, "3.60288e+16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9000000000000006F, 6U, "0.9", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9.999999999999999F, 6U, "10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1999999.0F, 6U, "2e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999999999999999.0F, 6U, "1e+16", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999999999999999999.0F, 6U, "1e+18", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999999999999999999.0F, 6U, "1e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99999999999999999999.0F, 6U, "1e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99e10F, 6U, "9.9e+11", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.999999999F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999999F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.999F, 6U, "0.999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.999999F, 6U, "0.999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.999999999999999F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999999F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999995F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999959F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999999999996F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999999999959F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999999999999F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999999999999999999999999999F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999999999999999999999999999999999999F, 6U, "1", "return", __LINE__);

    RealToStreamEqual(helper, stream, 10000005.0F, 6U, "1e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 10000005.0F, 7U, "1e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 10000006.0F, 7U, "1.000001e+07", "return", __LINE__);

    RealToStreamEqual(helper, stream, 10000000.0F, 6U, "1e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999999.0F, 6U, "1e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 19999999.0F, 6U, "2e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 29999999.0F, 6U, "3e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 39999999.0F, 6U, "4e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 49999999.0F, 6U, "5e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 59999999.0F, 6U, "6e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 69999999.0F, 6U, "7e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 79999999.0F, 6U, "8e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 89999999.0F, 6U, "9e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99999999.0F, 6U, "1e+08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999999999.0F, 6U, "1e+09", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.10000000F, 6U, "0.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9999999F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.19999999F, 6U, "0.2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.29999999F, 6U, "0.3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.39999999F, 6U, "0.4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.49999999F, 6U, "0.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.59999999F, 6U, "0.6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.69999999F, 6U, "0.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.79999999F, 6U, "0.8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.89999999F, 6U, "0.9", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.99999999F, 6U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.99999999F, 6U, "2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.99999999F, 6U, "3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.99999999F, 6U, "4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4.99999999F, 6U, "5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.99999999F, 6U, "6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 6.99999999F, 6U, "7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.99999999F, 6U, "8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.99999999F, 6U, "9", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.99999999F, 6U, "10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99.99999999F, 6U, "100", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999.99999999F, 6U, "1000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99999.89999999F, 6U, "99999.9", "return", __LINE__);
    RealToStreamEqual(helper, stream, 99999.99999999F, 6U, "100000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999999.99999999F, 6U, "1e+06", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1e1F, 6U, "10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e2F, 6U, "100", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e3F, 6U, "1000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e4F, 6U, "10000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e5F, 6U, "100000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e10F, 6U, "1e+10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e13F, 6U, "1e+13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e14F, 6U, "1e+14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e15F, 6U, "1e+15", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1e-1F, 6U, "0.1", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-2F, 6U, "0.01", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-3F, 6U, "0.001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-4F, 6U, "0.0001", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-5F, 6U, "1e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-10F, 6U, "1e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-13F, 6U, "1e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-14F, 6U, "1e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e-15F, 6U, "1e-15", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.40129846e-45F, 6U, "1.4013e-45", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.40282347e+38F, 6U, "3.40282e+38", "return", __LINE__);
    RealToStreamEqual(helper, stream, -1.40129846e-45F, 6U, "-1.4013e-45", "return", __LINE__);
    RealToStreamEqual(helper, stream, -3.40282347e+38F, 6U, "-3.40282e+38", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.40129846e-45F, 45U, "1.40129846432481707092372958328991613128026194e-45",
                      "return", __LINE__);
    RealToStreamEqual(helper, stream, -3.40282347e+38F, 39U, "-340282346638528859811704183484516925440", "return",
                      __LINE__);
    RealToStreamEqual(helper, stream, -3.40282347e+38F, 38U, "-3.4028234663852885981170418348451692544e+38", "return",
                      __LINE__);

    RealToStreamEqual(helper, stream, -854534.5F, 6U, "-854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, -854534.05F, 6U, "-854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.5F, 6U, "854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.05F, 6U, "854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, -9765625.0F, 6U, "-9.76562e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.2765625F, 6U, "-0.276563", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9765625.0F, 6U, "9.76562e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765645.0F, 6U, "9.76564e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765665.0F, 6U, "9.76566e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765685.0F, 6U, "9.76568e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765605.0F, 6U, "9.7656e+06", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9765615.0F, 6U, "9.76562e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765635.0F, 6U, "9.76564e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765655.0F, 6U, "9.76566e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765675.0F, 6U, "9.76568e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9765695.0F, 6U, "9.7657e+06", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9999425.0F, 6U, "9.99942e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999445.0F, 6U, "9.99944e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999465.0F, 6U, "9.99946e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999485.0F, 6U, "9.99948e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999405.0F, 6U, "9.9994e+06", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9.999925F, 6U, "9.99992", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.999945F, 6U, "9.99994", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.999965F, 6U, "9.99996", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.999985F, 6U, "9.99998", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.999905F, 6U, "9.9999", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.765625F, 6U, "1.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1765625F, 6U, "0.176563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.2765625F, 6U, "0.276563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3765625F, 6U, "0.376563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.4765625F, 6U, "0.476562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.5765625F, 6U, "0.576563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.6765625F, 6U, "0.676562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.7765625F, 6U, "0.776563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.8765625F, 6U, "0.876562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765625F, 6U, "0.976562", "return", __LINE__);

    RealToStreamEqual(helper, stream, 2.165625F, 6U, "2.16563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.265625F, 6U, "2.26562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.365625F, 6U, "2.36562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.465625F, 6U, "2.46563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.565625F, 6U, "2.56562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.665625F, 6U, "2.66563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.765625F, 6U, "2.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.865625F, 6U, "2.86562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.965625F, 6U, "2.96563", "return", __LINE__);

    RealToStreamEqual(helper, stream, 5.165625F, 6U, "5.16563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.265625F, 6U, "5.26562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.365625F, 6U, "5.36562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.465625F, 6U, "5.46562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.565625F, 6U, "5.56563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.665625F, 6U, "5.66563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.765625F, 6U, "5.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.865625F, 6U, "5.86562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.965625F, 6U, "5.96562", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9.765615F, 6U, "9.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625F, 6U, "9.76562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765635F, 6U, "9.76563", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765645F, 6U, "9.76565", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765655F, 6U, "9.76565", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765665F, 6U, "9.76567", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765675F, 6U, "9.76567", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765685F, 6U, "9.76569", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765695F, 6U, "9.76569", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765615F, 6U, "0.976561", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765625F, 6U, "0.976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765635F, 6U, "0.976564", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765645F, 6U, "0.976565", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765655F, 6U, "0.976565", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765665F, 6U, "0.976566", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765675F, 6U, "0.976568", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765685F, 6U, "0.976569", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.9765695F, 6U, "0.976569", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-1F, 6U, "0.976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-2F, 6U, "0.0976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-3F, 6U, "0.00976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-4F, 6U, "0.000976562", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-5F, 6U, "9.76563e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-10F, 6U, "9.76562e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-13F, 6U, "9.76562e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-14F, 6U, "9.76563e-14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.765625e-15F, 6U, "9.76562e-15", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.00001844674407370955161500F, 6U, "1.84467e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0001844674407370955161500F, 6U, "0.000184467", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.001844674407370955161500F, 6U, "0.00184467", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.001844674407370955161500F, 6U, "5.00184", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.01844674407370955161500F, 6U, "0.0184467", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1844674407370955161500F, 6U, "0.184467", "return", __LINE__);

    RealToStreamEqual(helper, stream, 18446744073709551614.0F, 6U, "1.84467e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551620.0F, 6U, "1.84467e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 184467440737095516150.0F, 6U, "1.84467e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1844674407370955161500.0F, 6U, "1.84467e+21", "return", __LINE__);
    RealToStreamEqual(helper, stream, 17446744073709551614.0F, 6U, "1.74467e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9923372036854775809.0F, 6U, "9.92337e+18", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.05F, 6U, "0.05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.005F, 6U, "0.005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0005F, 6U, "0.0005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00005F, 6U, "5e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000005F, 6U, "5e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.55F, 6U, "1.55", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0055F, 6U, "1.0055", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.005999F, 6U, "1.006", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.009999F, 6U, "1.01", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00055F, 6U, "0.00055", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00055F, 6U, "1.00055", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.50009F, 6U, "0.50009", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000000000050000000005F, 6U, "5e-11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000005F, 6U, "5e-13", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000000000000000000000000005F, 6U, "5e-28", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.3125F, 6U, "5.3125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.3F, 6U, "5.3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5.5F, 6U, "5.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.55F, 6U, "0.55", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.25F, 6U, "0.25", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.75F, 6U, "0.75", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.125F, 6U, "0.125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.625F, 6U, "0.625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3125F, 6U, "0.3125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.95367431640625F, 6U, "0.953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.298023223876953125F, 6U, "0.298023", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1490116119384765625F, 6U, "0.149012", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.7450580596923828125F, 6U, "0.745058", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.37252902984619140625F, 6U, "0.372529", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.186264514923095703125F, 6U, "0.186265", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.12345F, 6U, "1.12345", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.953674F, 6U, "1.95367", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.0953674F, 6U, "1.09537", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.00953674F, 6U, "1.00954", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.000953674F, 6U, "1.00095", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.953674F, 6U, "9.95367", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.0953674F, 6U, "9.09537", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.00953674F, 6U, "9.00954", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.000953674F, 6U, "9.00095", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.953674F, 6U, "0.953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0953674F, 6U, "0.0953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00953674F, 6U, "0.00953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000953674F, 6U, "0.000953674", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.123455F, 6U, "0.123455", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1123405F, 6U, "0.112341", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1123095F, 6U, "0.11231", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.78126F, 6U, "0.78126", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.300048828125F, 6U, "0.300049", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.111F, 6U, "0.111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.11111F, 6U, "0.11111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1111111111111111F, 6U, "0.111111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1111111111111115F, 6U, "0.111111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.11111111111111159F, 6U, "0.111111", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1111111111111116F, 6U, "0.111111", "return", __LINE__);

    RealToStreamEqual(helper, stream, 105.625F, 6U, "105.625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 25e20F, 6U, "2.5e+21", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.5e21F, 6U, "2.5e+21", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3814697265625.0F, 6U, "3.8147e+12", "return", __LINE__);
    RealToStreamEqual(helper, stream, 381469725.0F, 6U, "3.8147e+08", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3814697265625F, 6U, "0.38147", "return", __LINE__);
    RealToStreamEqual(helper, stream, 10.5F, 6U, "10.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 10.0F, 6U, "10", "return", __LINE__);

    RealToStreamEqual(helper, stream, 2097151.125F, 6U, "2.09715e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2097151.0F, 6U, "2.09715e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 16217975.0F, 6U, "1.6218e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 32995191.0F, 6U, "3.29952e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 444444444444444444444.0F, 6U, "4.44444e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 555555555555555555555.0F, 6U, "5.55556e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3668262915145728.0F, 6U, "3.66826e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7336525430291456.0F, 6U, "7.33653e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7336525830291456.0F, 6U, "7.33653e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7336535830291456.0F, 6U, "7.33654e+15", "return", __LINE__);
    RealToStreamEqual(helper, stream, 14673051660582912.0F, 6U, "1.46731e+16", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1000000.390625F, 6U, "1e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1000009.590625F, 6U, "1.00001e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1000008.590625F, 6U, "1.00001e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.3365258e-10F, 6U, "7.33653e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.3365258e-10F, 8U, "7.3365258e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3333F, 6U, "0.3333", "return", __LINE__);

    RealToStreamEqual(helper, stream, 4554534.0F, 6U, "4.55453e+06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.055F, 6U, "854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.0055F, 6U, "854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.000055F, 6U, "854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, 854534.0000005F, 14U, "854534", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.0000005F, 6U, "8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.000005F, 6U, "8", "return", __LINE__);
    RealToStreamEqual(helper, stream, 8.005F, 6U, "8.005", "return", __LINE__);

    RealToStreamEqual(helper, stream, 5.55F, 1U, "6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.996F, 5U, "9.996", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.996F, 4U, "9.996", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.996F, 3U, "10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.996F, 2U, "10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9.996F, 1U, "1e+01", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.5F, 1U, "0.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.5F, 1U, "2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.5F, 1U, "2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.5F, 1U, "4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4.5F, 1U, "4", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.25F, 1U, "2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.75F, 1U, "3", "return", __LINE__);

    RealToStreamEqual(helper, stream, 2.625F, 2U, "2.6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.65F, 2U, "2.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.25F, 2U, "2.2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.25F, 2U, "3.2", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.625F, 2U, "2.6", "return", __LINE__);

    RealToStreamEqual(helper, stream, 2.635F, 2U, "2.6", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.35F, 2U, "2.3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.35F, 2U, "3.3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 2.635F, 2U, "2.6", "return", __LINE__);

    RealToStreamEqual(helper, stream, 91.0F, 1U, "9e+01", "return", __LINE__);
    RealToStreamEqual(helper, stream, 95.0F, 1U, "1e+02", "return", __LINE__);

    RealToStreamEqual(helper, stream, 95000000.0F, 1U, "1e+08", "return", __LINE__);

    RealToStreamEqual(helper, stream, 3.0F, 15U, "3", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3.14F, 15U, "3.14000010490417", "return", __LINE__);

    RealToStreamEqual(helper, stream, 140737488355327.953674F, 15U, "140737488355328", "return", __LINE__);
    RealToStreamEqual(helper, stream, 140737488355327.015625F, 15U, "140737488355328", "return", __LINE__);

    RealToStreamEqual(helper, stream, 140737458355327.953674F, 15U, "140737454800896", "return", __LINE__);
    RealToStreamEqual(helper, stream, 140737488355327.953674F, 6U, "1.40737e+14", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.95367431640625F, 20U, "0.95367431640625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.298023223876953125F, 20U, "0.298023223876953125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.1490116119384765625F, 20U, "0.1490116119384765625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.7450580596923828125F, 20U, "0.7450580596923828125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.37252902984619140625F, 20U, "0.37252902984619140625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.186264514923095703125F, 20U, "0.18626451492309570312", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.186264514923095703125F, 21U, "0.186264514923095703125", "return", __LINE__);

    RealToStreamEqual(helper, stream, 18446744073709551614.0F, 20U, "18446744073709551616", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551620.0F, 20U, "18446744073709551616", "return", __LINE__);
    RealToStreamEqual(helper, stream, 184467440737095516150.0F, 20U, "1.8446744073709551616e+20", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1844674407370955161500.0F, 20U, "1.8446744073709551616e+21", "return", __LINE__);
    RealToStreamEqual(helper, stream, 17446744073709551614.0F, 21U, "17446743745805418496", "return", __LINE__);
    RealToStreamEqual(helper, stream, 17446744073709551614.0F, 20U, "17446743745805418496", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9923372036854775809.0F, 21U, "9923371716631855104", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9923372036854775809.0F, 20U, "9923371716631855104", "return", __LINE__);

    RealToStreamEqual(helper, stream, 2251799813685247.0F, 21U, "2251799813685248", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4503599627370495.0F, 21U, "4503599627370496", "return", __LINE__);
    RealToStreamEqual(helper, stream, 4611686018427387903.0F, 21U, "4611686018427387904", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9223372036854775806.0F, 21U, "9223372036854775808", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9223372036854775807.0F, 21U, "9223372036854775808", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551615.0F, 21U, "18446744073709551616", "return", __LINE__);
    RealToStreamEqual(helper, stream, 18446744073709551616.0F, 21U, "18446744073709551616", "return", __LINE__);
    RealToStreamEqual(helper, stream, 34028236692093846341.0F, 21U, "34028235612225536000", "return", __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463412.0F, 21U, "340282382510534426624", "return", __LINE__);
    RealToStreamEqual(helper, stream, 3402823669209384634633746074317.0F, 34U, "3402823648528822565846915219456",
                      "return", __LINE__);
    RealToStreamEqual(helper, stream, 340282366920938463426481119284341.0F, 34U, "340282366061808076199320696651776",
                      "return", __LINE__);
    RealToStreamEqual(helper, stream, 3402823669209384634264811192843414.0F, 34U, "3402823583246828306656939785322496",
                      "return", __LINE__);
}

template <typename Stream_T_>
static void TestFloatToString2(TestHelper &helper, Stream_T_ &stream) {
    RealToStreamEqual(helper, stream, -1.0F, 6U, "-1", "return", __LINE__);

    RealToStreamEqual(helper, stream, 70.0F, 6U, "70", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7e+10F, 6U, "7e+10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 75e+10F, 6U, "7.5e+11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7e+30F, 6U, "7e+30", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5000000000000000.0F, 6U, "5e+15", "return", __LINE__);

    RealToStreamEqual(helper, stream, 9999998.0F, 6U, "1e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 9999998.9F, 6U, "1e+07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999999999.999999999F, 6U, "1e+09", "return", __LINE__);

    RealToStreamEqual(helper, stream, 111.5F, 6U, "111.5", "return", __LINE__);
    RealToStreamEqual(helper, stream, 999.625F, 6U, "999.625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1e+33F, 6U, "1e+33", "return", __LINE__);

    RealToStreamEqual(helper, stream, 18446744073709551615.0F, 2U, "1.8e+19", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.18446744073709551615F, 2U, "0.18", "return", __LINE__);

    RealToStreamEqual(helper, stream, 10.7F, 6U, "10.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 20.7F, 6U, "20.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 30.7F, 6U, "30.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 40.7F, 6U, "40.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 50.7F, 6U, "50.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 60.7F, 6U, "60.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 70.7F, 6U, "70.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 80.7F, 6U, "80.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 90.7F, 6U, "90.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 50.25F, 6U, "50.25", "return", __LINE__);
    RealToStreamEqual(helper, stream, 500000000000000.25F, 6U, "5e+14", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.7F, 6U, "7.7", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7.00007F, 7U, "7.00007", "return", __LINE__);
    RealToStreamEqual(helper, stream, 1.25F, 6U, "1.25", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.07F, 6U, "0.07", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.3125F, 4U, "0.3125", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5e-4F, 6U, "0.0005", "return", __LINE__);
    RealToStreamEqual(helper, stream, 5e-10F, 6U, "5e-10", "return", __LINE__);
    RealToStreamEqual(helper, stream, 7e-24F, 6U, "7e-24", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.390625F, 6U, "0.390625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000390625F, 6U, "0.000390625", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.0000390625F, 6U, "3.90625e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.00000390625F, 6U, "3.90625e-06", "return", __LINE__);
    RealToStreamEqual(helper, stream, 0.000009999999F, 6U, "1e-05", "return", __LINE__);
    RealToStreamEqual(helper, stream, 505.3125F, 6U, "505.312", "return", __LINE__);
    RealToStreamEqual(helper, stream, 505.003125F, 6U, "505.003", "return", __LINE__);
}

static int RunDigitTests() {
    using SStream = StringStream<char>;

    TestHelper helper{"Digit.hpp", __FILE__};

    // helper.ContinueOnError(true);
    helper.PrintGroupName();

    helper.Test("StringToNumber Test 1", TestStringToNumber1);
    helper.Test("StringToNumber Test 2", TestStringToNumber2);
    helper.Test("StringToNumber Test 3", TestStringToNumber3);
    helper.Test("StringToNumber Test 4", TestStringToNumber4);
    helper.Test("StringToNumber Test 5", TestStringToNumber5);
    helper.Test("StringToNumber Test 6", TestStringToNumber6);
    helper.Test("StringToNumber Test 7", TestStringToNumber7);

    helper.Test("HexStringToNumber Test", TestHexStringToNumber);

    SStream stream{64};

    helper.Test("IntToString Test 1", TestIntToString1<SStream>, false, stream);

    helper.Test("DoubleToString Test 1", TestDoubleToString1<SStream>, false, stream);
    helper.Test("DoubleToString Test 2", TestDoubleToString2<SStream>, false, stream);
    helper.Test("DoubleToString Test 3", TestDoubleToString3<SStream>, false, stream);

    helper.Test("FloatToString Test 1", TestFloatToString1<SStream>, false, stream);
    helper.Test("FloatToString Test 2", TestFloatToString2<SStream>, false, stream);

    return helper.EndTests();
}

} // namespace Qentem::Test

#endif
