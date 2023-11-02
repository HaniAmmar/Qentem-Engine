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

#include "TestHelper.hpp"

#include "StringStream.hpp"
#include "Digit.hpp"

#ifndef QENTEM_DIGIT_TESTS_H_
#define QENTEM_DIGIT_TESTS_H_

namespace Qentem {
namespace Test {

static SizeT HexStringToNumber(const char *str) noexcept {
    return Digit<char>::HexStringToNumber(str, Qentem::StringUtils::Count(str));
}

template <typename Number_T_>
static void IntToStreamEqual(TestHelper &helper, StringStream<char> &stream, Number_T_ number, const char *expected,
                             const char *name, unsigned long line) {
    Digit<char>::NumberToString(stream, number);
    helper.Equal(stream, expected, name, line);
    stream.Clear();
}

template <typename Number_T_>
static void RealToStreamEqual(TestHelper &helper, StringStream<char> &stream, Number_T_ number, unsigned int precision,
                              const char *expected, const char *name, unsigned long line) {
    Digit<char>::NumberToString(stream, number, precision);
    helper.Equal(stream, expected, name, line);
    stream.Clear();
}

template <typename Number_T_>
static bool StringToNumberCount(Number_T_ &num, const char *str) noexcept {
    QNumber number;
    SizeT   offset = 0;
    SizeT   length = StringUtils::Count(str);

    switch (Digit<char>::StringToNumber(number, str, offset, length)) {
        case 1: {
            num = static_cast<Number_T_>(number.Natural);
            break;
        }

        case 2: {
            num = static_cast<Number_T_>(number.Integer);
            break;
        }

        case 3: {
            num = static_cast<Number_T_>(number.Real);
            break;
        }

        default:
            return false;
    };

    if (offset >= length) {
        return true;
    }

    return false;
}

static void TestStringToNumber1(TestHelper &helper) {
    SizeT       number = 0;
    const char *str    = "";
    bool        valid;

    str   = "";
    valid = StringToNumberCount(number, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "0";
    valid = StringToNumberCount(number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number, 0U, str, __LINE__);

    str   = "1";
    valid = StringToNumberCount(number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number, 1U, str, __LINE__);

    str   = "1000000";
    valid = StringToNumberCount(number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number, 1000000U, str, __LINE__);

    str   = "1010101";
    valid = StringToNumberCount(number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number, 1010101U, str, __LINE__);

    str   = "9999999";
    valid = StringToNumberCount(number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number, 9999999U, str, __LINE__);

    str   = "123456789";
    valid = StringToNumberCount(number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
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
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1+e";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1-e1";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1+e1";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1--e1";
    valid = StringToNumberCount(number_ulong, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1++e1";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e-";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e+";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e--1";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e++1";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1ee1";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E";
    valid = StringToNumberCount(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "E1";
    valid = StringToNumberCount(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1-E";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1+E";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1-E1";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1+E1";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1--E1";
    valid = StringToNumberCount(number_ulong, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1++E1";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E-";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E+";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E--1";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E++1";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1EE1";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = ".";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = ".0.0";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "0.0.";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "0..0";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = ".123.123";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "123.123.";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "123..123";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "12y3";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1x23";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "w123";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-";
    valid = StringToNumberCount(number_float, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e+";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e-";
    valid = StringToNumberCount(number_long, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-w1";
    valid = StringToNumberCount(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+w1";
    valid = StringToNumberCount(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+01";
    valid = StringToNumberCount(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-01";
    valid = StringToNumberCount(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "01";
    valid = StringToNumberCount(number_int, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e";
    valid = StringToNumberCount(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "e1";
    valid = StringToNumberCount(number_uint, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "500000000000000000X000.000000000000";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsFalse(valid, "valid", __LINE__);

    // str   = "1e-99999999999";
    // valid = StringToNumberCount(number_double, str);
    // helper.EqualsFalse(valid, "valid", __LINE__);
}

static void TestStringToNumber3(TestHelper &helper) {
    int          number_int    = 0;
    unsigned int number_uint   = 0;
    double       number_double = 0;

    const char *str;
    bool        valid;

    str   = "-1";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, -1.0, "number", __LINE__);

    str   = "+1";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1.0, "number", __LINE__);

    str   = "+1";
    valid = StringToNumberCount(number_uint, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_uint, 1U, "number", __LINE__);

    str   = "-1";
    valid = StringToNumberCount(number_int, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_int, -1, "number", __LINE__);

    str   = "1";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1.0, "number", __LINE__);

    str   = "1.0";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1.0, "number", __LINE__);

    str   = "1.00001";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1.00001, "number", __LINE__);

    str   = "1.1";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1.1, "number", __LINE__);

    str   = "123456789";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 123456789.0, "number", __LINE__);

    str   = "1.48828125";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1.48828125, "number", __LINE__);

    str   = "1e10";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1e10, "number", __LINE__);

    str   = "1E10";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1e10, "number", __LINE__);

    str   = "1e+10";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1e10, "number", __LINE__);

    str   = "1E+10";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1e10, "number", __LINE__);

    str   = "1E5";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 1e5, "number", __LINE__);

    str   = "0.3";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 0.3, "number", __LINE__);

    str   = "0.2";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 0.2, "number", __LINE__);

    str   = "0.1";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 0.1, "number", __LINE__);

    str   = "2.3";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 2.3, "number", __LINE__);

    str   = "3.2";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 3.2, "number", __LINE__);

    str   = "5.1";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 5.1, "number", __LINE__);

    str   = "-1";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, -1, "number", __LINE__);

    str   = "123.";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 123.0, "number", __LINE__);

    str   = ".0";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 0.0, "number", __LINE__);

    str   = "0.";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 0.0, "number", __LINE__);

    str   = "123456789.123456789";
    valid = StringToNumberCount(number_double, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(number_double, 123456789.123456789, "number", __LINE__);
}

static void TestStringToNumber4(TestHelper &helper) {
    const char        *str        = nullptr;
    double             d_number   = 0;
    long long          ll_number  = 0;
    unsigned long long ull_number = 0;
    bool               valid;

    str   = "-1.0";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1, "number", __LINE__);

    str   = "-1.1";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1.1, "number", __LINE__);

    str   = "-123456789";
    valid = StringToNumberCount(ll_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(ll_number, -123456789, "number", __LINE__);

    str   = "-1.48828125";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1.48828125, "number", __LINE__);

    str   = "-1e10";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e10, "number", __LINE__);

    str   = "-1E10";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e10, "d_number", __LINE__);

    str   = "-1e+10";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e10, "d_number", __LINE__);

    str   = "-1e+9";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e9, "d_number", __LINE__);

    str   = "-1E+10";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e10, "d_number", __LINE__);

    str   = "-1e00";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1, "d_number", __LINE__);

    str   = "-1e-00";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1, "d_number", __LINE__);

    str   = "-1e+00";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1, "d_number", __LINE__);

    str   = "1e-00";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1, "d_number", __LINE__);

    str   = "1e+00";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1, "d_number", __LINE__);

    str   = "1e-5";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-5, "d_number", __LINE__);

    str   = "1e5";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e5, "d_number", __LINE__);

    str   = "1e-10";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-10, "d_number", __LINE__);

    str   = "1e+9";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e9, "d_number", __LINE__);

    str   = "-1e-10";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e-10, "d_number", __LINE__);

    str   = "1E-10";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1e-10, "d_number", __LINE__);

    str   = "-1E-10";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -1e-10, "d_number", __LINE__);

    str   = "-0.3";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -0.3, "d_number", __LINE__);

    str   = "-0.2";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -0.2, "d_number", __LINE__);

    str   = "-0.1";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -0.1, "d_number", __LINE__);

    str   = "-2.3";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -2.3, "d_number", __LINE__);

    str   = "-3.2";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -3.2, "d_number", __LINE__);

    str   = "-5.1";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -5.1, "d_number", __LINE__);

    str   = "-0";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -0.0, "d_number", __LINE__);

    str   = "-0.0";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -0.0, "d_number", __LINE__);

    str   = "-123456789.12345679";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -123456789.123456789, "number", __LINE__);

    str   = "18446744073709551615";
    valid = StringToNumberCount(ull_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(ull_number, 18446744073709551615ULL, "number", __LINE__);

    str   = "18446744073709551616";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 18446744073709551616.0, "number", __LINE__);

    str   = "9223372036854775807";
    valid = StringToNumberCount(ll_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(ll_number, 9223372036854775807LL, "number", __LINE__);

    str   = "-9223372036854775807";
    valid = StringToNumberCount(ll_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(ll_number, -9223372036854775807LL, "number", __LINE__);

    str   = "-9223372036854775808";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -9223372036854775808.0, "number", __LINE__);

    str   = "-9223372036854775808.0";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, -9223372036854775808.0, "number", __LINE__);

    str   = "5000000000000000000000000000000000";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5e33, "number", __LINE__);

    str   = "5000000000000000000000.000000000000";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5e21, "number", __LINE__);

    str   = "5000000000000000000000000000000000e-15";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 5000000000000000000.0, "number", __LINE__);

    // str   = "5000000000000000000000e100";
    // valid = StringToNumberCount(d_number, str);
    // helper.EqualsTrue(valid, "valid", __LINE__);
    // helper.Equal(d_number, 5.0e121, "number", __LINE__);

    // str   = "100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    // valid = StringToNumberCount(d_number, str);
    // helper.EqualsTrue(valid, "valid", __LINE__);
    // helper.Equal(d_number, 1e92, "number", __LINE__);

    //     str   = "200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    // valid = StringToNumberCount(d_number, str);
    // helper.EqualsTrue(valid, "valid", __LINE__);
    // helper.Equal(d_number, 1e92, "number", __LINE__);

    // str   = "999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999";
    // valid = StringToNumberCount(d_number, str);
    // helper.EqualsTrue(valid, "valid", __LINE__);
    // helper.Equal(d_number, 1e93, "number", __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 3.123456789123456812, "number", __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789e+10";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 31234567891.23456955, "number", __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789e-10";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 3.1234567891234559e-10, "number", __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789e100";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 3.1234567891234563e+100, "number", __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789e-100";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 3.1234567891234566e-100, "number", __LINE__);

    str =
        "3.123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789e-308";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 3.123456789123456856e-308, "number", __LINE__);

    str =
        "1.7976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157e+300";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1.7976931348623165e+300, "number", __LINE__);

    str   = "1.7976931348623157e+300";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 1.7976931348623157e+300, "number", __LINE__);

    str   = "1.7976931348623157e+1";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 17.976931348623154, "number", __LINE__);

    str   = "4856312379865454687821527487362783273278723787e-7";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 4.856312379865455e+38, "number", __LINE__);

    str   = "4856312379865454687821527487362783273278723787e+7";
    valid = StringToNumberCount(d_number, str);
    helper.EqualsTrue(valid, "valid", __LINE__);
    helper.Equal(d_number, 4.856312379865455e+52, "number", __LINE__);

    // str   = "1.0e308";
    // valid = StringToNumberCount(d_number, str);
    // helper.EqualsTrue(valid, "valid", __LINE__);
    // helper.Equal(d_number, 1.0e308, "d_number", __LINE__);

    // str   = "-1.0e308";
    // valid = StringToNumberCount(d_number, str);
    // helper.EqualsTrue(valid, "valid", __LINE__);
    // helper.Equal(d_number, -1.0e308, "d_number", __LINE__);
}

static void TestStringToNumber5(TestHelper &helper) {
    const char  *content = nullptr;
    QNumber      number;
    unsigned int type   = 0;
    SizeT        offset = 0;

    content = "0]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 1U, "type", __LINE__);
    helper.Equal(number.Natural, 0U, "number", __LINE__);

    offset  = 0;
    content = ".0]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 3U, "type", __LINE__);
    helper.Equal(number.Real, 0.0, "number", __LINE__);

    offset  = 0;
    content = "0.]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 3U, "type", __LINE__);
    helper.Equal(number.Real, 0.0, "number", __LINE__);

    offset  = 0;
    content = "0.0]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 3U, "type", __LINE__);
    helper.Equal(number.Real, -0.0, "number", __LINE__);

    offset  = 0;
    content = "-0]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 3U, "type", __LINE__);
    helper.Equal(number.Real, -0.0, "number", __LINE__);

    offset  = 0;
    content = "-.0]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 3U, "type", __LINE__);
    helper.Equal(number.Real, -0.0, "number", __LINE__);

    offset  = 0;
    content = "-0.]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 3U, "type", __LINE__);
    helper.Equal(number.Real, -0.0, "number", __LINE__);

    offset  = 0;
    content = "-0.0]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 3U, "type", __LINE__);
    helper.Equal(number.Real, -0.0, "number", __LINE__);

    offset  = 0;
    content = "1]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 1U, "type", __LINE__);
    helper.Equal(number.Natural, 1U, "number", __LINE__);

    offset  = 0;
    content = ".2]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 3U, "type", __LINE__);
    helper.Equal(number.Real, 0.2, "number", __LINE__);

    offset  = 0;
    content = "3.]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 3U, "type", __LINE__);
    helper.Equal(number.Real, 3.0, "number", __LINE__);

    offset  = 0;
    content = "4.5]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 3U, "type", __LINE__);
    helper.Equal(number.Real, 4.5, "number", __LINE__);

    offset  = 0;
    content = "-1]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 2U, "type", __LINE__);
    helper.Equal(number.Integer, -1, "number", __LINE__);

    offset  = 0;
    content = "-.2]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 3U, "type", __LINE__);
    helper.Equal(number.Real, -0.2, "number", __LINE__);

    offset  = 0;
    content = "-3.]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 3U, "type", __LINE__);
    helper.Equal(number.Real, -3.0, "number", __LINE__);

    offset  = 0;
    content = "-4.5]";
    type    = Digit<char>::StringToNumber(number, content, offset, StringUtils::Count(content));
    helper.Equal(type, 3U, "type", __LINE__);
    helper.Equal(number.Real, -4.5, "number", __LINE__);
}

static void TestNumberToString1(TestHelper &helper) {
    StringStream<char> stream;
    int                number_int;
    unsigned int       number_uint;
    long long          number_long;
    unsigned long long number_ulong;
    float              number_float;
    double             number_double;

    number_int = -1;
    IntToStreamEqual(helper, stream, number_int, "-1", "return", __LINE__);

    number_int = 1;
    IntToStreamEqual(helper, stream, number_int, "1", "return", __LINE__);

    number_long = -100000;
    IntToStreamEqual(helper, stream, number_long, "-100000", "return", __LINE__);

    number_long = 100000;
    IntToStreamEqual(helper, stream, number_long, "100000", "return", __LINE__);

    number_uint = 0;
    IntToStreamEqual(helper, stream, number_uint, "0", "return", __LINE__);

    number_ulong = 1;
    IntToStreamEqual(helper, stream, number_ulong, "1", "return", __LINE__);

    number_ulong = 4;
    IntToStreamEqual(helper, stream, number_ulong, "4", "return", __LINE__);

    number_int = -7;
    IntToStreamEqual(helper, stream, number_int, "-7", "return", __LINE__);

    number_uint = 10;
    IntToStreamEqual(helper, stream, number_uint, "10", "return", __LINE__);

    number_ulong = 3;
    IntToStreamEqual(helper, stream, number_ulong, "3", "return", __LINE__);

    number_long = 9223372036854775807LL;
    IntToStreamEqual(helper, stream, number_long, "9223372036854775807", "return", __LINE__);

    number_long = -9223372036854775807LL;
    IntToStreamEqual(helper, stream, number_long, "-9223372036854775807", "return", __LINE__);

    number_ulong = 18446744073709551615ULL;
    IntToStreamEqual(helper, stream, number_ulong, "18446744073709551615", "return", __LINE__);

    number_float = 1;
    RealToStreamEqual(helper, stream, number_float, 0U, "1", "return", __LINE__);

    number_double = 0;
    RealToStreamEqual(helper, stream, number_double, 0U, "0", "return", __LINE__);

    number_double = 1;
    RealToStreamEqual(helper, stream, number_double, 0U, "1", "return", __LINE__);

    number_double = 15;
    RealToStreamEqual(helper, stream, number_double, 0U, "15", "return", __LINE__);

    number_double = 12345;
    RealToStreamEqual(helper, stream, number_double, 0U, "12345", "return", __LINE__);

    number_double = 1.1;
    RealToStreamEqual(helper, stream, number_double, 0U, "1.1", "return", __LINE__);

    number_double = 100.5;
    RealToStreamEqual(helper, stream, number_double, 0U, "100.5", "return", __LINE__);

    number_double = 1.123456;
    RealToStreamEqual(helper, stream, number_double, 1U, "1.1", "return", __LINE__);

    number_double = 80.123456;
    RealToStreamEqual(helper, stream, number_double, 1U, "80.1", "return", __LINE__);

    number_double = 12345.123456;
    RealToStreamEqual(helper, stream, number_double, 1U, "12345.1", "return", __LINE__);

    number_double = 1.123456;
    RealToStreamEqual(helper, stream, number_double, 1U, "1.1", "return", __LINE__);

    number_double = 100.123456;
    RealToStreamEqual(helper, stream, number_double, 1U, "100.1", "return", __LINE__);

    number_double = 100.523456;
    RealToStreamEqual(helper, stream, number_double, 1U, "100.5", "return", __LINE__);
}

static void TestNumberToString2(TestHelper &helper) {
    StringStream<char> stream;

    double number = 1.123456;
    RealToStreamEqual(helper, stream, number, 2U, "1.12", "return", __LINE__);

    number = 80.123456;
    RealToStreamEqual(helper, stream, number, 2U, "80.12", "return", __LINE__);

    number = 12345.123456;
    RealToStreamEqual(helper, stream, number, 2U, "12345.12", "return", __LINE__);

    number = 1.123456;
    RealToStreamEqual(helper, stream, number, 2U, "1.12", "return", __LINE__);

    number = 100.123456;
    RealToStreamEqual(helper, stream, number, 2U, "100.12", "return", __LINE__);

    number = 100.523456;
    RealToStreamEqual(helper, stream, number, 2U, "100.52", "return", __LINE__);

    number = 100.1234;
    RealToStreamEqual(helper, stream, number, 5U, "100.1234", "return", __LINE__);
}

static void TestNumberToString3(TestHelper &helper) {
    StringStream<char> stream;
    double             number;

    number = 5.15;
    RealToStreamEqual(helper, stream, number, 3U, "5.15", "return", __LINE__);

    number = 5.55;
    RealToStreamEqual(helper, stream, number, 2U, "5.55", "return", __LINE__);

    number = 5.55; // NOTE: Fix
    // RealToStreamEqual(helper, stream, number, 1U, "5.6", "return", __LINE__);

    number = 5.57;
    RealToStreamEqual(helper, stream, number, 1U, "5.6", "return", __LINE__);

    number = 0.99;
    RealToStreamEqual(helper, stream, number, 1U, "1", "return", __LINE__);

    number = 0.99;
    RealToStreamEqual(helper, stream, number, 2U, "0.99", "return", __LINE__);

    number = 0.99;
    RealToStreamEqual(helper, stream, number, 5U, "0.99", "return", __LINE__);

    number = 5.99;
    RealToStreamEqual(helper, stream, number, 1U, "6", "return", __LINE__);

    number = 9.99;
    RealToStreamEqual(helper, stream, number, 1U, "10", "return", __LINE__);

    number = 90.99;
    RealToStreamEqual(helper, stream, number, 1U, "91", "return", __LINE__);

    number = 99.99;
    RealToStreamEqual(helper, stream, number, 1U, "100", "return", __LINE__);

    number = 99.0099;
    RealToStreamEqual(helper, stream, number, 1U, "99", "return", __LINE__);
    RealToStreamEqual(helper, stream, number, 2U, "99.01", "return", __LINE__);
    RealToStreamEqual(helper, stream, number, 3U, "99.01", "return", __LINE__);

    number = 99.0009;
    RealToStreamEqual(helper, stream, number, 3U, "99.001", "return", __LINE__);

    number = 456789.0029;
    RealToStreamEqual(helper, stream, number, 3U, "456789.003", "return", __LINE__);

    number = -456789.0024;
    RealToStreamEqual(helper, stream, number, 3U, "-456789.002", "return", __LINE__);
}

static void TestNumberToString4(TestHelper &helper) {
    StringStream<char> stream;

    double number = -2;
    RealToStreamEqual(helper, stream, number, 0U, "-2", "return", __LINE__);

    number = -3.1;
    RealToStreamEqual(helper, stream, number, 0U, "-3.1", "return", __LINE__);

    number = -3.1;
    RealToStreamEqual(helper, stream, number, 0U, "-3.1", "return", __LINE__);

    number = -22.87;
    RealToStreamEqual(helper, stream, number, 2U, "-22.87", "return", __LINE__);

    number = -55.0055;
    RealToStreamEqual(helper, stream, number, 10U, "-55.0055", "return", __LINE__);

    number = -0.123455678987455;
    RealToStreamEqual(helper, stream, number, 0U, "-0.123455678987455", "return", __LINE__);

    number = -0.123455678987452;
    RealToStreamEqual(helper, stream, number, 0U, "-0.123455678987452", "return", __LINE__);
    number = 0.999999;
    RealToStreamEqual(helper, stream, number, 5U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, number, 6U, "0.999999", "return", __LINE__);
    RealToStreamEqual(helper, stream, number, 10U, "0.999999", "return", __LINE__);

    number = 0.123e-11;
    RealToStreamEqual(helper, stream, number, 0U, "1.23e-12", "return", __LINE__);

    number = -2.00000000000099;
    RealToStreamEqual(helper, stream, number, 14U, "-2.00000000000099", "return", __LINE__);

    number = 3.9999999999999;
    RealToStreamEqual(helper, stream, number, 14U, "3.9999999999999", "return", __LINE__);

    number = 99.1005099;
    RealToStreamEqual(helper, stream, number, 10U, "99.1005099", "return", __LINE__);

    number = 99.1005099;
    RealToStreamEqual(helper, stream, number, 7U, "99.1005099", "return", __LINE__);

    number = 871.080055555;
    RealToStreamEqual(helper, stream, number, 9U, "871.080055555", "return", __LINE__);

    number = 0.00056599999999999999;
    RealToStreamEqual(helper, stream, number, 3U, "1e-3", "return", __LINE__);

    RealToStreamEqual(helper, stream, number, 0U, "5.66e-4", "return", __LINE__);

    number = 3.99999999999909;
    RealToStreamEqual(helper, stream, number, 0U, "3.99999999999909", "return", __LINE__);

    number = 2.00000000000015;
    RealToStreamEqual(helper, stream, number, 14U, "2.00000000000015", "return", __LINE__);

    RealToStreamEqual(helper, stream, number, 14U, "2.00000000000015", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.93002099999999999, 3U, "0.93", "return", __LINE__);

    RealToStreamEqual(helper, stream, -9223372036854775807.0, 1U, "-9223372036854775808", "return", __LINE__);

    RealToStreamEqual(helper, stream, -0.9223372036854775, 20U, "-0.9223372036854775", "return", __LINE__);

    RealToStreamEqual(helper, stream, -92233.0, 0U, "-92233", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775, 20U, "0.9223372036854775", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9999, 3U, "1", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.9999, 3U, "-1", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9, 1U, "0.9", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.9, 1U, "-0.9", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9999, 4U, "0.9999", "return", __LINE__);
    RealToStreamEqual(helper, stream, -0.9999, 4U, "-0.9999", "return", __LINE__);

    RealToStreamEqual(helper, stream, 12345678912345.345, 2U, "12345678912345.35", "return", __LINE__);

    RealToStreamEqual(helper, stream, 45.12345678912345, 14U, "45.12345678912345", "return", __LINE__);

    RealToStreamEqual(helper, stream, 45.12345678912345, 3U, "45.123", "return", __LINE__);

    RealToStreamEqual(helper, stream, -10.9999, 0U, "-10.9999", "return", __LINE__);
    RealToStreamEqual(helper, stream, 10.9999, 0U, "10.9999", "return", __LINE__);

    RealToStreamEqual(helper, stream, -10.9999, 2U, "-11", "return", __LINE__);
    RealToStreamEqual(helper, stream, 10.9999, 2U, "11", "return", __LINE__);
}

static void TestNumberToString5(TestHelper &helper) {
    StringStream<char> stream;

    RealToStreamEqual(helper, stream, 1e308, 0U, "1e308", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1e-308, 0U, "1e-308", "return", __LINE__);

    RealToStreamEqual(helper, stream, 15e100, 0U, "1.5e101", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.6866831148613157e308, 0U, "1.686683114861316e308", "return", __LINE__);

    RealToStreamEqual(helper, stream, 99999999999999999999.0, 0U, "1e20", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.000000000000000009, 0U, "9e-18", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.000000000009, 0U, "9e-12", "return", __LINE__);

    RealToStreamEqual(helper, stream, -0.000000000009, 0U, "-9e-12", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.100009, 0U, "0.100009", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.000009, 0U, "9e-6", "return", __LINE__);

    RealToStreamEqual(helper, stream, 1.000009, 0U, "1.000009", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 1U, "0.9", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 2U, "0.92", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 3U, "0.922", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 4U, "0.9223", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 5U, "0.92234", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 6U, "0.922337", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 7U, "0.9223372", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 8U, "0.9223372", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 9U, "0.922337204", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 10U, "0.9223372037", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 11U, "0.92233720369", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 12U, "0.922337203685", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 13U, "0.9223372036855", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 14U, "0.92233720368548", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 15U, "0.922337203685478", "return", __LINE__);

    RealToStreamEqual(helper, stream, 0.9223372036854775807, 16U, "0.9223372036854776", "return", __LINE__);
}

static void TestHexConv(TestHelper &helper) {
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

static int RunDigitTests() {
    TestHelper helper{"Digit.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("StringToNumber Test 1", TestStringToNumber1);
    helper.Test("StringToNumber Test 2", TestStringToNumber2);
    helper.Test("StringToNumber Test 3", TestStringToNumber3);
    helper.Test("StringToNumber Test 4", TestStringToNumber4);
    helper.Test("StringToNumber Test 5", TestStringToNumber5);

    helper.Test("NumberToString Test 1", TestNumberToString1);
    helper.Test("NumberToString Test 2", TestNumberToString2);
    helper.Test("NumberToString Test 3", TestNumberToString3);
    helper.Test("NumberToString Test 4", TestNumberToString4);
    helper.Test("NumberToString Test 5", TestNumberToString5);

    helper.Test("Hex Conversion Test", TestHexConv);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
