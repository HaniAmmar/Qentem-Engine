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

#include "Digit.hpp"

#ifndef QENTEM_DIGIT_TESTS_H_
#define QENTEM_DIGIT_TESTS_H_

namespace Qentem {
namespace Test {

using DigitC = Digit<char>;

static SizeT HexStringToNumber(const char *str) noexcept {
    return Digit<char>::HexStringToNumber(str, Qentem::StringUtils::Count(str));
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

template <typename Number_T_>
bool StringToNumberCount(Number_T_ &num, const char *str) noexcept {
    return DigitC::StringToNumber(num, str, StringUtils::Count(str));
}

static void TestStringToNumber1(TestHelper &helper) {
    SizeT       number = 0;
    const char *str    = "";
    bool        valid;

    str   = "";
    valid = StringToNumberCount(number, str);
    helper.EqualsFalse(valid, "valid", __LINE__);
    helper.Equal(number, 0U, "number", __LINE__);

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

    const char *str = "";
    bool        valid;

    valid = StringToNumberCount(number_int, str);
    helper.Equal(number_int, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    valid = StringToNumberCount(number_uint, str);
    helper.Equal(number_int, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_int, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    valid = StringToNumberCount(number_ulong, str);
    helper.Equal(number_ulong, 0U, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1-e";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1+e";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1-e1";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1+e1";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1--e1";
    valid = StringToNumberCount(number_ulong, str);
    helper.Equal(number_ulong, 0U, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1++e1";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e-";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e+";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e--1";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e++1";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1ee1";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 0.0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E";
    valid = StringToNumberCount(number_uint, str);
    helper.Equal(number_uint, 0U, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "E1";
    valid = StringToNumberCount(number_uint, str);
    helper.Equal(number_uint, 0U, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1-E";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1+E";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1-E1";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1+E1";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1--E1";
    valid = StringToNumberCount(number_ulong, str);
    helper.Equal(number_ulong, 0U, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1++E1";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E-";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E+";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E--1";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1E++1";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1EE1";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 0.0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = ".";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    // .0 is valid

    str   = "0.";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = ".0.0";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "0.0.";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "0..0";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "123.";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = ".123.123";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "123.123.";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "123..123";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "12y3";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1x23";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "w123";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-";
    valid = StringToNumberCount(number_float, str);
    helper.Equal(number_float, 0.0F, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-";
    valid = StringToNumberCount(number_long, str);
    helper.Equal(number_long, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "500000000000000000X000.000000000000";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 0.0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);
}

static void TestStringToNumber3(TestHelper &helper) {
    int          number_int    = 0;
    unsigned int number_uint   = 0;
    double       number_double = 0;

    const char *str = "";
    bool        valid;

    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 0.0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-w1";
    valid = StringToNumberCount(number_int, str);
    helper.Equal(number_int, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+w1";
    valid = StringToNumberCount(number_int, str);
    helper.Equal(number_int, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "+01";
    valid = StringToNumberCount(number_int, str);
    helper.Equal(number_int, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "-01";
    valid = StringToNumberCount(number_int, str);
    helper.Equal(number_int, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "01";
    valid = StringToNumberCount(number_int, str);
    helper.Equal(number_int, 0, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "1e";
    valid = StringToNumberCount(number_uint, str);
    helper.Equal(number_uint, 0U, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "e1";
    valid = StringToNumberCount(number_uint, str);
    helper.Equal(number_uint, 0U, "number", __LINE__);
    helper.EqualsFalse(valid, "valid", __LINE__);

    str   = "0";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 0.0, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-1";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, -1.0, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "+1";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 1.0, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "+1";
    valid = StringToNumberCount(number_uint, str);
    helper.Equal(number_uint, 1U, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-1";
    valid = StringToNumberCount(number_int, str);
    helper.Equal(number_int, -1, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "1";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 1.0, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "1.0";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 1.0, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "1.1";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 1.1, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "123456789";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 123456789.0, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "1.123456789";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 1.123456789, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "123456789.123456789";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 123456789.123456789, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "1e10";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 1e10, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "1E10";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 1e10, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "1e+10";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 1e10, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "1E+10";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 1e10, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "5000000000000000000000000000000000";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 5e33, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "5000000000000000000000.000000000000";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 5e21, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "1E5";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 1e5, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "1e-10";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 1e-10, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "1E-10";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 1e-10, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "0.3";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 0.3, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "0.2";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 0.2, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "0.1";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 0.1, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "2.3";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 2.3, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "3.2";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 3.2, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "5.1";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, 5.1, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-1";
    valid = StringToNumberCount(number_double, str);
    helper.Equal(number_double, -1, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);
}

static void TestStringToNumber4(TestHelper &helper) {
    const char *str    = nullptr;
    double      number = 0;
    bool        valid;

    str   = "-1.0";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -1, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-1.1";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -1.1, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-123456789";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -123456789.0, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-1.123456789";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -1.123456789, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-123456789.123456789";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -123456789.123456789, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-1e10";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -1e10, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-1E10";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -1e10, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-1e+10";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -1e10, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-1E+10";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -1e10, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-1e-10";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -1e-10, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-1E-10";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -1e-10, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-0.3";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -0.3, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-0.2";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -0.2, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-0.1";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -0.1, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-2.3";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -2.3, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-3.2";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -3.2, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);

    str   = "-5.1";
    valid = StringToNumberCount(number, str);
    helper.Equal(number, -5.1, "number", __LINE__);
    helper.EqualsTrue(valid, "valid", __LINE__);
}

static void TestNumberToString1(TestHelper &helper) {
    int           number_int;
    unsigned int  number_uint;
    long          number_long;
    unsigned long number_ulong;
    float         number_float;
    double        number_double;

    number_int = -1;
    helper.Equal(DigitC::NumberToString(number_int), "-1", "return", __LINE__);

    number_int = 1;
    helper.Equal(DigitC::NumberToString(number_int), "1", "return", __LINE__);

    number_long = -100000;
    helper.Equal(DigitC::NumberToString(number_long), "-100000", "return", __LINE__);

    number_long = 100000;
    helper.Equal(DigitC::NumberToString(number_long), "100000", "return", __LINE__);

    number_uint = 0;
    helper.Equal(DigitC::NumberToString(number_uint), "0", "return", __LINE__);

    number_ulong = 1;
    helper.Equal(DigitC::NumberToString(number_ulong), "1", "return", __LINE__);

    number_ulong = 4;
    helper.Equal(DigitC::NumberToString(number_ulong), "4", "return", __LINE__);

    number_int = -7;
    helper.Equal(DigitC::NumberToString(number_int), "-7", "return", __LINE__);

    number_uint = 10;
    helper.Equal(DigitC::NumberToString(number_uint), "10", "return", __LINE__);

    number_ulong = 3;
    helper.Equal(DigitC::NumberToString(number_ulong), "3", "return", __LINE__);

    number_float = 1;
    helper.Equal(DigitC::NumberToString(number_float), "1", "return", __LINE__);

    number_double = 0;
    helper.Equal(DigitC::NumberToString(number_double, 1U, 0U, 0U), "0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 1U, 0U), ".0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 1U, 0U), "0.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 3U, 0U, 0U), "000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 5U, 0U), ".00000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 2U, 2U, 0U), "00.00", "return", __LINE__);

    number_double = 1;
    helper.Equal(DigitC::NumberToString(number_double, 0U, 0U, 0U), "1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 0U, 0U), "1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 1U, 0U), "1.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 1U, 0U), "1.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 3U, 0U, 0U), "001", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 5U, 0U), "1.00000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 2U, 2U, 0U), "01.00", "return", __LINE__);

    number_double = 15;
    helper.Equal(DigitC::NumberToString(number_double, 0U, 0U, 0U), "15", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 0U, 0U), "15", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 1U, 0U), "15.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 1U, 0U), "15.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 3U, 0U, 0U), "015", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 5U, 0U), "15.00000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 2U, 2U, 0U), "15.00", "return", __LINE__);

    number_double = 12345;
    helper.Equal(DigitC::NumberToString(number_double, 0U, 0U, 0U), "12345", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 0U, 0U), "12345", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 1U, 0U), "12345.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 1U, 0U), "12345.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 3U, 0U, 0U), "12345", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 5U, 0U), "12345.00000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 2U, 2U, 0U), "12345.00", "return", __LINE__);

    number_double = 1.1;
    helper.Equal(DigitC::NumberToString(number_double, 0U, 0U, 0U), "1.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 0U, 0U), "1.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 1U, 0U), "1.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 1U, 0U), "1.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 3U, 0U, 0U), "001.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 5U, 0U), "1.10000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 2U, 2U, 0U), "01.10", "return", __LINE__);

    number_double = 100.5;
    helper.Equal(DigitC::NumberToString(number_double, 0U, 0U, 0U), "100.5", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 0U, 0U), "100.5", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 1U, 0U), "100.5", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 1U, 0U), "100.5", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 3U, 0U, 0U), "100.5", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 5U, 0U), "100.50000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 2U, 2U, 0U), "100.50", "return", __LINE__);

    number_double = 1.123456;
    helper.Equal(DigitC::NumberToString(number_double, 0U, 0U, 1U), "1.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 0U, 1U), "1.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 1U, 1U), "1.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 1U, 1U), "1.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 3U, 0U, 1U), "001.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 5U, 1U), "1.10000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 2U, 2U, 1U), "01.10", "return", __LINE__);

    number_double = 80.123456;
    helper.Equal(DigitC::NumberToString(number_double, 0U, 0U, 1U), "80.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 0U, 1U), "80.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 1U, 1U), "80.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 1U, 1U), "80.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 3U, 0U, 1U), "080.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 5U, 1U), "80.10000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 2U, 2U, 1U), "80.10", "return", __LINE__);

    number_double = 12345.123456;
    helper.Equal(DigitC::NumberToString(number_double, 0U, 0U, 1U), "12345.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 0U, 1U), "12345.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 1U, 1U), "12345.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 1U, 1U), "12345.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 3U, 0U, 1U), "12345.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 5U, 1U), "12345.10000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 2U, 2U, 1U), "12345.10", "return", __LINE__);

    number_double = 1.123456;
    helper.Equal(DigitC::NumberToString(number_double, 0U, 0U, 1U), "1.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 0U, 1U), "1.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 1U, 1U), "1.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 1U, 1U), "1.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 3U, 0U, 1U), "001.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 5U, 1U), "1.10000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 2U, 2U, 1U), "01.10", "return", __LINE__);

    number_double = 100.123456;
    helper.Equal(DigitC::NumberToString(number_double, 0U, 0U, 1U), "100.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 0U, 1U), "100.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 1U, 1U), "100.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 1U, 1U), "100.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 3U, 0U, 1U), "100.1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 5U, 1U), "100.10000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 2U, 2U, 1U), "100.10", "return", __LINE__);

    number_double = 100.523456;
    helper.Equal(DigitC::NumberToString(number_double, 0U, 0U, 1U), "100.5", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 0U, 1U), "100.5", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 1U, 1U), "100.5", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 1U, 1U, 1U), "100.5", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 3U, 0U, 1U), "100.5", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 0U, 5U, 1U), "100.50000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number_double, 2U, 2U, 1U), "100.50", "return", __LINE__);
}

static void TestNumberToString2(TestHelper &helper) {
    double number = 1.123456;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 2U), "1.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 2U), "1.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 2U), "1.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 1U, 2U), "1.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 0U, 2U), "001.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 5U, 2U), "1.12000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 2U, 2U, 2U), "01.12", "return", __LINE__);

    number = 80.123456;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 2U), "80.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 2U), "80.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 2U), "80.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 1U, 2U), "80.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 0U, 2U), "080.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 5U, 2U), "80.12000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 2U, 2U, 2U), "80.12", "return", __LINE__);

    number = 12345.123456;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 2U), "12345.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 2U), "12345.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 2U), "12345.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 1U, 2U), "12345.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 0U, 2U), "12345.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 5U, 2U), "12345.12000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 2U, 2U, 2U), "12345.12", "return", __LINE__);

    number = 1.123456;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 2U), "1.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 2U), "1.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 2U), "1.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 1U, 2U), "1.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 0U, 2U), "001.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 5U, 2U), "1.12000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 2U, 2U, 2U), "01.12", "return", __LINE__);

    number = 100.123456;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 2U), "100.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 2U), "100.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 2U), "100.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 1U, 2U), "100.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 0U, 2U), "100.12", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 5U, 2U), "100.12000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 2U, 2U, 2U), "100.12", "return", __LINE__);

    number = 100.523456;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 2U), "100.52", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 2U), "100.52", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 2U), "100.52", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 1U, 2U), "100.52", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 0U, 2U), "100.52", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 5U, 2U), "100.52000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 2U, 2U, 2U), "100.52", "return", __LINE__);

    number = 100.1234;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 5U), "100.1234", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 5U), "100.1234", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 5U), "100.1234", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 1U, 5U), "100.1234", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 0U, 5U), "100.1234", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 5U, 5U), "100.12340", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 2U, 2U, 5U), "100.1234", "return", __LINE__);
}

static void TestNumberToString3(TestHelper &helper) {
    double number;

    number = 5.15;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 3U), "5.15", "return", __LINE__);

    number = 5.55;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 2U), "5.55", "return", __LINE__);

    // number = 5.55; // NOTE: Fix
    // EQ_VALUE(DigitC::NumberToString(number, 0U, 0U, 1U), "5.6",
    // "return");

    number = 5.57;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 1U), "5.6", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 1U), "5.6", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 2U, 1U), "5.60", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 3U, 1U), "005.600", "return", __LINE__);

    number = 0.99;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 1U), "1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 1U), "1.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 2U, 1U), "1.00", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 3U, 1U), "001.000", "return", __LINE__);

    number = 0.99;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 2U), ".99", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 2U), ".99", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 2U, 2U), ".99", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 3U, 2U), "000.990", "return", __LINE__);

    number = 0.99;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 5U), ".99", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 5U), ".99", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 2U, 5U), ".99", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 3U, 5U), "000.990", "return", __LINE__);

    number = 5.99;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 1U), "6", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 1U), "6.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 2U, 1U), "6.00", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 3U, 1U), "006.000", "return", __LINE__);

    number = 9.99;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 1U), "10", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 1U), "10.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 2U, 1U), "10.00", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 3U, 1U), "010.000", "return", __LINE__);

    number = 90.99;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 1U), "91", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 1U), "91.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 2U, 1U), "91.00", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 3U, 1U), "091.000", "return", __LINE__);

    number = 99.99;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 1U), "100", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 1U, 1U), "100.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 2U, 1U), "100.00", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 3U, 3U, 1U), "100.000", "return", __LINE__);

    number = 99.0099;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 1U), "99", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 2U), "99.01", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 3U), "99.01", "return", __LINE__);

    number = 99.0009;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 3U), "99.001", "return", __LINE__);

    number = 456789.0029;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 3U), "456789.003", "return", __LINE__);

    number = -456789.0024;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 3U), "-456789.002", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(18446744073709551615ULL), "18446744073709551615", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(-9223372036854775807LL), "-9223372036854775807", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(9223372036854775807LL), "9223372036854775807", "return", __LINE__);
}

static void TestNumberToString4(TestHelper &helper) {
    double number = -123456;
    helper.Equal(DigitC::NumberToString(number, 30U, 0U, 0U), "-000000000000000123456", "return", __LINE__);

    number = -2;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 0U), "-2", "return", __LINE__);

    number = -3.1;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 0U), "-3.1", "return", __LINE__);

    number = -3.1;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 0U), "-3.1", "return", __LINE__);

    number = -22.87;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 2U), "-22.87", "return", __LINE__);

    number = -55.0055;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 10U), "-55.0055", "return", __LINE__);

    number = -0.123455678987455;
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 0U), "-0.123455678987455", "return", __LINE__);

    number = -0.123455678987452;
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 0U), "-0.123455678987452", "return", __LINE__);
    number = 0.999999;
    helper.Equal(DigitC::NumberToString(number, 0U, 0U, 5U), "1", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 6U), "0.999999", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 10U), "0.999999", "return", __LINE__);

    number = 0.123e-11;
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 0U), "1.23e-12", "return", __LINE__);

    number = -2.00000000000099;
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 14U), "-2.00000000000099", "return", __LINE__);

    number = 3.9999999999999;
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 14U), "3.9999999999999", "return", __LINE__);

    number = 99.1005099;
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 10U), "99.1005099", "return", __LINE__);

    number = 99.1005099;
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 7U), "99.1005099", "return", __LINE__);

    number = 871.080055555;
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 9U), "871.080055555", "return", __LINE__);

    number = 0.00056599999999999999;
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 3U), "1e-3", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(number), "5.66e-4", "return", __LINE__);

    number = 3.99999999999909;
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 0U), "3.99999999999909", "return", __LINE__);

    number = 2.00000000000015;
    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 14U), "2.00000000000015", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(number, 1U, 0U, 14U), "2.00000000000015", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(7.0, 1U, 3U, 3U), "7.000", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(-7.0, 1U, 3U, 3U), "-7.000", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(8.0, 3U, 3U, 3U), "008.000", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(-8.0, 3U, 3U, 3U), "-008.000", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(7.1, 1U, 3U, 3U), "7.100", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(-7.1, 1U, 3U, 3U), "-7.100", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(8.1, 3U, 3U, 3U), "008.100", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(-8.1, 3U, 3U, 3U), "-008.100", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.93002099999999999, 1U, 3U, 3U), "0.930", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(-9223372036854775807.0, 1U), "-9223372036854775808", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(-0.9223372036854775, 1U, 1U), "-0.9223372036854775", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(-92233.0, 10U), "-0000092233", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(-92233.0, 1U), "-92233", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775, 1U), "0.9223372036854775", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9999, 1U, 1U, 3U), "1.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(-0.9999, 1U, 1U, 3U), "-1.0", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9999, 3U, 1U, 3U), "001.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(-0.9999, 3U, 1U, 3U), "-001.0", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9999, 3U, 3U, 3U), "001.000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(-0.9999, 3U, 3U, 3U), "-001.000", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(0.9, 1U, 1U), "0.9", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(-0.9, 1U, 1U), "-0.9", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9999, 1U, 1U), "0.9999", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(-0.9999, 1U, 1U), "-0.9999", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(12345678912345.345, 1U, 1U, 2U), "12345678912345.35", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(45.12345678912345, 1U, 1U, 14U), "45.12345678912345", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(45.12345678912345, 1U, 1U, 3U), "45.123", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(-10.9999, 1U, 1U, 0U), "-10.9999", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(10.9999, 1U, 1U, 0U), "10.9999", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(-10.9999, 1U, 1U, 2U), "-11.0", "return", __LINE__);
    helper.Equal(DigitC::NumberToString(10.9999, 1U, 1U, 2U), "11.0", "return", __LINE__);
}

static void TestNumberToString5(TestHelper &helper) {
    helper.Equal(DigitC::NumberToString(1e308), "1e308", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(1e-308), "1e-308", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(15e100), "1.5e101", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(1.6866831148613157e308), "1.686683114861316e308", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(99999999999999999999.0), "1e20", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.000000000000000009), "9e-18", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.000000000009), "9e-12", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(-0.000000000009), "-9e-12", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.100009), "0.100009", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.100009, 0U), ".100009", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.000009), "9e-6", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(1.000009), "1.000009", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 1U), "0.9", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 2U), "0.92", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 3U), "0.922", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 4U), "0.9223", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 5U), "0.92234", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 6U), "0.922337", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 7U), "0.9223372", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 8U), "0.9223372", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 9U), "0.922337204", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 10U), "0.9223372037", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 11U), "0.92233720369", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 12U), "0.922337203685", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 13U), "0.9223372036855", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 14U), "0.92233720368548", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 15U), "0.922337203685478", "return", __LINE__);

    helper.Equal(DigitC::NumberToString(0.9223372036854775807, 1U, 1U, 16U), "0.9223372036854776", "return", __LINE__);
}

static int RunDigitTests() {
    TestHelper helper{"Digit.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("Hex Conversion Test", TestHexConv);

    helper.Test("StringToNumber Test 1", TestStringToNumber1);
    helper.Test("StringToNumber Test 2", TestStringToNumber2);
    helper.Test("StringToNumber Test 3", TestStringToNumber3);
    helper.Test("StringToNumber Test 4", TestStringToNumber4);

    helper.Test("NumberToString Test 1", TestNumberToString1);
    helper.Test("NumberToString Test 2", TestNumberToString2);
    helper.Test("NumberToString Test 3", TestNumberToString3);
    helper.Test("NumberToString Test 4", TestNumberToString4);
    helper.Test("NumberToString Test 5", TestNumberToString5);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
