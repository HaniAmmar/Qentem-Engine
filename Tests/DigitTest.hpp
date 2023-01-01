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

static int TestHexConv() {
    SizeT       number;
    const char *hex = nullptr;

    number = HexStringToNumber(hex);
    EQ_VALUE(number, 0, "number");

    hex    = "0000";
    number = HexStringToNumber(hex);
    EQ_VALUE(number, 0, "number");

    hex    = "AAAA";
    number = HexStringToNumber(hex);
    EQ_VALUE(number, 43690, "number");

    hex    = "000A";
    number = HexStringToNumber(hex);
    EQ_VALUE(number, 10, "number");

    hex    = "A000";
    number = HexStringToNumber(hex);
    EQ_VALUE(number, 40960, "number");

    hex    = "ABCDEF";
    number = HexStringToNumber(hex);
    EQ_VALUE(number, 11259375, "number");

    hex    = "FEDCBA";
    number = HexStringToNumber(hex);
    EQ_VALUE(number, 16702650, "number");

    hex    = "abcdef";
    number = HexStringToNumber(hex);
    EQ_VALUE(number, 11259375, "number");

    hex    = "fedcba";
    number = HexStringToNumber(hex);
    EQ_VALUE(number, 16702650, "number");

    hex    = "12345";
    number = HexStringToNumber(hex);
    EQ_VALUE(number, 74565, "number");

    hex    = "6789";
    number = HexStringToNumber(hex);
    EQ_VALUE(number, 26505, "number");

    hex    = "FFFF";
    number = HexStringToNumber(hex);
    EQ_VALUE(number, 65535, "number");

    hex    = "9999";
    number = HexStringToNumber(hex);
    EQ_VALUE(number, 39321, "number");

    END_SUB_TEST;
}

template <typename Number_T_>
bool StringToNumberCount(Number_T_ &num, const char *str) noexcept {
    return DigitC::StringToNumber(num, str, StringUtils::Count(str));
}

static int TestStringToNumber1() {
    SizeT       number = 0;
    const char *str    = "";
    bool        valid;

    str   = "";
    valid = StringToNumberCount(number, str);
    EQ_FALSE(valid, "valid");
    EQ_VALUE(number, 0, "number");

    str   = "0";
    valid = StringToNumberCount(number, str);
    EQ_TRUE(valid, "valid");
    EQ_VALUE(number, 0, str);

    str   = "1";
    valid = StringToNumberCount(number, str);
    EQ_TRUE(valid, "valid");
    EQ_VALUE(number, 1, str);

    str   = "1000000";
    valid = StringToNumberCount(number, str);
    EQ_TRUE(valid, "valid");
    EQ_VALUE(number, 1000000, str);

    str   = "1010101";
    valid = StringToNumberCount(number, str);
    EQ_TRUE(valid, "valid");
    EQ_VALUE(number, 1010101, str);

    str   = "9999999";
    valid = StringToNumberCount(number, str);
    EQ_TRUE(valid, "valid");
    EQ_VALUE(number, 9999999, str);

    str   = "123456789";
    valid = StringToNumberCount(number, str);
    EQ_TRUE(valid, "valid");
    EQ_VALUE(number, 123456789, str);

    END_SUB_TEST;
}

static int TestStringToNumber2() {
    int           number_int    = 0;
    unsigned int  number_uint   = 0;
    long          number_long   = 0;
    unsigned long number_ulong  = 0;
    float         number_float  = 0;
    double        number_double = 0;

    const char *str = nullptr;
    bool        valid;

    valid = StringToNumberCount(number_int, str);
    EQ_VALUE(number_int, 0, "number");
    EQ_FALSE(valid, "valid");

    valid = StringToNumberCount(number_uint, str);
    EQ_VALUE(number_int, 0, "number");
    EQ_FALSE(valid, "valid");

    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_int, 0, "number");
    EQ_FALSE(valid, "valid");

    valid = StringToNumberCount(number_ulong, str);
    EQ_VALUE(number_ulong, 0, "number");
    EQ_FALSE(valid, "valid");

    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1-e";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1+e";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1-e1";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1+e1";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1--e1";
    valid = StringToNumberCount(number_ulong, str);
    EQ_VALUE(number_ulong, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1++e1";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1e-";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1e+";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1e--1";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1e++1";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1ee1";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1E";
    valid = StringToNumberCount(number_uint, str);
    EQ_VALUE(number_uint, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "E1";
    valid = StringToNumberCount(number_uint, str);
    EQ_VALUE(number_uint, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1-E";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1+E";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1-E1";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1+E1";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1--E1";
    valid = StringToNumberCount(number_ulong, str);
    EQ_VALUE(number_ulong, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1++E1";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1E-";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1E+";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1E--1";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1E++1";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1EE1";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = ".";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    // .0 is valid

    str   = "0.";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = ".0.0";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "0.0.";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "0..0";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "123.";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = ".123.123";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "123.123.";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "123..123";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "12y3";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1x23";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "w123";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "+";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "-";
    valid = StringToNumberCount(number_float, str);
    EQ_VALUE(number_float, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "+";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "-";
    valid = StringToNumberCount(number_long, str);
    EQ_VALUE(number_long, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "500000000000000000X000.000000000000";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 0, "number");
    EQ_FALSE(valid, "valid");

    END_SUB_TEST;
}

static int TestStringToNumber3() {
    int          number_int    = 0;
    unsigned int number_uint   = 0;
    double       number_double = 0;

    const char *str = nullptr;
    bool        valid;

    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "-w1";
    valid = StringToNumberCount(number_int, str);
    EQ_VALUE(number_int, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "+w1";
    valid = StringToNumberCount(number_int, str);
    EQ_VALUE(number_int, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "+01";
    valid = StringToNumberCount(number_int, str);
    EQ_VALUE(number_int, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "-01";
    valid = StringToNumberCount(number_int, str);
    EQ_VALUE(number_int, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "01";
    valid = StringToNumberCount(number_int, str);
    EQ_VALUE(number_int, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "1e";
    valid = StringToNumberCount(number_uint, str);
    EQ_VALUE(number_uint, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "e1";
    valid = StringToNumberCount(number_uint, str);
    EQ_VALUE(number_uint, 0, "number");
    EQ_FALSE(valid, "valid");

    str   = "0";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 0, "number");
    EQ_TRUE(valid, "valid");

    str   = "-1";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, -1, "number");
    EQ_TRUE(valid, "valid");

    str   = "+1";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 1, "number");
    EQ_TRUE(valid, "valid");

    str   = "+1";
    valid = StringToNumberCount(number_uint, str);
    EQ_VALUE(number_uint, 1, "number");
    EQ_TRUE(valid, "valid");

    str   = "-1";
    valid = StringToNumberCount(number_int, str);
    EQ_VALUE(number_int, -1, "number");
    EQ_TRUE(valid, "valid");

    str   = "1";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 1, "number");
    EQ_TRUE(valid, "valid");

    str   = "1.0";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 1, "number");
    EQ_TRUE(valid, "valid");

    str   = "1.1";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 1.1, "number");
    EQ_TRUE(valid, "valid");

    str   = "123456789";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 123456789, "number");
    EQ_TRUE(valid, "valid");

    str   = "1.123456789";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 1.123456789, "number");
    EQ_TRUE(valid, "valid");

    str   = "123456789.123456789";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 123456789.123456789, "number");
    EQ_TRUE(valid, "valid");

    str   = "1e10";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 1e10, "number");
    EQ_TRUE(valid, "valid");

    str   = "1E10";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 1e10, "number");
    EQ_TRUE(valid, "valid");

    str   = "1e+10";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 1e10, "number");
    EQ_TRUE(valid, "valid");

    str   = "1E+10";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 1e10, "number");
    EQ_TRUE(valid, "valid");

    str   = "5000000000000000000000000000000000";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 5e33, "number");
    EQ_TRUE(valid, "valid");

    str   = "5000000000000000000000.000000000000";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 5e21, "number");
    EQ_TRUE(valid, "valid");

    str   = "1E5";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 1e5, "number");
    EQ_TRUE(valid, "valid");

    str   = "1e-10";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 1e-10, "number");
    EQ_TRUE(valid, "valid");

    str   = "1E-10";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 1e-10, "number");
    EQ_TRUE(valid, "valid");

    str   = "0.3";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 0.3, "number");
    EQ_TRUE(valid, "valid");

    str   = "0.2";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 0.2, "number");
    EQ_TRUE(valid, "valid");

    str   = "0.1";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 0.1, "number");
    EQ_TRUE(valid, "valid");

    str   = "2.3";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 2.3, "number");
    EQ_TRUE(valid, "valid");

    str   = "3.2";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 3.2, "number");
    EQ_TRUE(valid, "valid");

    str   = "5.1";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, 5.1, "number");
    EQ_TRUE(valid, "valid");

    str   = "-1";
    valid = StringToNumberCount(number_double, str);
    EQ_VALUE(number_double, -1, "number");
    EQ_TRUE(valid, "valid");

    END_SUB_TEST;
}

static int TestStringToNumber4() {
    const char *str    = nullptr;
    double      number = 0;
    bool        valid;

    str   = "-1.0";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -1, "number");
    EQ_TRUE(valid, "valid");

    str   = "-1.1";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -1.1, "number");
    EQ_TRUE(valid, "valid");

    str   = "-123456789";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -123456789, "number");
    EQ_TRUE(valid, "valid");

    str   = "-1.123456789";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -1.123456789, "number");
    EQ_TRUE(valid, "valid");

    str   = "-123456789.123456789";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -123456789.123456789, "number");
    EQ_TRUE(valid, "valid");

    str   = "-1e10";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -1e10, "number");
    EQ_TRUE(valid, "valid");

    str   = "-1E10";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -1e10, "number");
    EQ_TRUE(valid, "valid");

    str   = "-1e+10";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -1e10, "number");
    EQ_TRUE(valid, "valid");

    str   = "-1E+10";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -1e10, "number");
    EQ_TRUE(valid, "valid");

    str   = "-1e-10";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -1e-10, "number");
    EQ_TRUE(valid, "valid");

    str   = "-1E-10";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -1e-10, "number");
    EQ_TRUE(valid, "valid");

    str   = "-0.3";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -0.3, "number");
    EQ_TRUE(valid, "valid");

    str   = "-0.2";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -0.2, "number");
    EQ_TRUE(valid, "valid");

    str   = "-0.1";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -0.1, "number");
    EQ_TRUE(valid, "valid");

    str   = "-2.3";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -2.3, "number");
    EQ_TRUE(valid, "valid");

    str   = "-3.2";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -3.2, "number");
    EQ_TRUE(valid, "valid");

    str   = "-5.1";
    valid = StringToNumberCount(number, str);
    EQ_VALUE(number, -5.1, "number");
    EQ_TRUE(valid, "valid");

    END_SUB_TEST;
}

static int TestNumberToString1() {
    int           number_int;
    unsigned int  number_uint;
    long          number_long;
    unsigned long number_ulong;
    float         number_float;
    double        number_double;

    number_int = -1;
    EQ_VALUE(DigitC::NumberToString(number_int), "-1", "return");

    number_int = 1;
    EQ_VALUE(DigitC::NumberToString(number_int), "1", "return");

    number_long = -100000;
    EQ_VALUE(DigitC::NumberToString(number_long), "-100000", "return");

    number_long = 100000;
    EQ_VALUE(DigitC::NumberToString(number_long), "100000", "return");

    number_uint = 0;
    EQ_VALUE(DigitC::NumberToString(number_uint), "0", "return");

    number_ulong = 1;
    EQ_VALUE(DigitC::NumberToString(number_ulong), "1", "return");

    number_ulong = 4;
    EQ_VALUE(DigitC::NumberToString(number_ulong), "4", "return");

    number_int = -7;
    EQ_VALUE(DigitC::NumberToString(number_int), "-7", "return");

    number_uint = 10;
    EQ_VALUE(DigitC::NumberToString(number_uint), "10", "return");

    number_ulong = 3;
    EQ_VALUE(DigitC::NumberToString(number_ulong), "3", "return");

    number_float = 1;
    EQ_VALUE(DigitC::NumberToString(number_float), "1", "return");

    number_double = 0;
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 0, 0), "0", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 1, 0), ".0", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 1, 0), "0.0", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 3, 0, 0), "000", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 5, 0), ".00000", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 2, 2, 0), "00.00", "return");

    number_double = 1;
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 0, 0), "1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 0, 0), "1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 1, 0), "1.0", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 1, 0), "1.0", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 3, 0, 0), "001", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 5, 0), "1.00000", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 2, 2, 0), "01.00", "return");

    number_double = 15;
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 0, 0), "15", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 0, 0), "15", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 1, 0), "15.0", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 1, 0), "15.0", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 3, 0, 0), "015", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 5, 0), "15.00000", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 2, 2, 0), "15.00", "return");

    number_double = 12345;
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 0, 0), "12345", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 0, 0), "12345", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 1, 0), "12345.0", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 1, 0), "12345.0", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 3, 0, 0), "12345", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 5, 0), "12345.00000", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 2, 2, 0), "12345.00", "return");

    number_double = 1.1;
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 0, 0), "1.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 0, 0), "1.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 1, 0), "1.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 1, 0), "1.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 3, 0, 0), "001.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 5, 0), "1.10000", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 2, 2, 0), "01.10", "return");

    number_double = 100.5;
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 0, 0), "100.5", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 0, 0), "100.5", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 1, 0), "100.5", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 1, 0), "100.5", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 3, 0, 0), "100.5", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 5, 0), "100.50000", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 2, 2, 0), "100.50", "return");

    number_double = 1.123456;
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 0, 1), "1.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 0, 1), "1.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 1, 1), "1.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 1, 1), "1.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 3, 0, 1), "001.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 5, 1), "1.10000", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 2, 2, 1), "01.10", "return");

    number_double = 80.123456;
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 0, 1), "80.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 0, 1), "80.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 1, 1), "80.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 1, 1), "80.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 3, 0, 1), "080.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 5, 1), "80.10000", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 2, 2, 1), "80.10", "return");

    number_double = 12345.123456;
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 0, 1), "12345.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 0, 1), "12345.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 1, 1), "12345.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 1, 1), "12345.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 3, 0, 1), "12345.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 5, 1), "12345.10000", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 2, 2, 1), "12345.10", "return");

    number_double = 1.123456;
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 0, 1), "1.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 0, 1), "1.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 1, 1), "1.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 1, 1), "1.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 3, 0, 1), "001.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 5, 1), "1.10000", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 2, 2, 1), "01.10", "return");

    number_double = 100.123456;
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 0, 1), "100.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 0, 1), "100.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 1, 1), "100.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 1, 1), "100.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 3, 0, 1), "100.1", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 5, 1), "100.10000", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 2, 2, 1), "100.10", "return");

    number_double = 100.523456;
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 0, 1), "100.5", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 0, 1), "100.5", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 1, 1), "100.5", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 1, 1, 1), "100.5", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 3, 0, 1), "100.5", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 0, 5, 1), "100.50000", "return");
    EQ_VALUE(DigitC::NumberToString(number_double, 2, 2, 1), "100.50", "return");

    END_SUB_TEST;
}

static int TestNumberToString2() {
    double number = 1.123456;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 2), "1.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 2), "1.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 2), "1.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 1, 2), "1.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 0, 2), "001.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 5, 2), "1.12000", "return");
    EQ_VALUE(DigitC::NumberToString(number, 2, 2, 2), "01.12", "return");

    number = 80.123456;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 2), "80.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 2), "80.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 2), "80.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 1, 2), "80.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 0, 2), "080.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 5, 2), "80.12000", "return");
    EQ_VALUE(DigitC::NumberToString(number, 2, 2, 2), "80.12", "return");

    number = 12345.123456;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 2), "12345.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 2), "12345.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 2), "12345.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 1, 2), "12345.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 0, 2), "12345.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 5, 2), "12345.12000", "return");
    EQ_VALUE(DigitC::NumberToString(number, 2, 2, 2), "12345.12", "return");

    number = 1.123456;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 2), "1.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 2), "1.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 2), "1.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 1, 2), "1.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 0, 2), "001.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 5, 2), "1.12000", "return");
    EQ_VALUE(DigitC::NumberToString(number, 2, 2, 2), "01.12", "return");

    number = 100.123456;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 2), "100.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 2), "100.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 2), "100.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 1, 2), "100.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 0, 2), "100.12", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 5, 2), "100.12000", "return");
    EQ_VALUE(DigitC::NumberToString(number, 2, 2, 2), "100.12", "return");

    number = 100.523456;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 2), "100.52", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 2), "100.52", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 2), "100.52", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 1, 2), "100.52", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 0, 2), "100.52", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 5, 2), "100.52000", "return");
    EQ_VALUE(DigitC::NumberToString(number, 2, 2, 2), "100.52", "return");

    number = 100.1234;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 5), "100.1234", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 5), "100.1234", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 5), "100.1234", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 1, 5), "100.1234", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 0, 5), "100.1234", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 5, 5), "100.12340", "return");
    EQ_VALUE(DigitC::NumberToString(number, 2, 2, 5), "100.1234", "return");
    END_SUB_TEST;
}

static int TestNumberToString3() {
    double number;

    number = 5.15;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 3), "5.15", "return");

    number = 5.55;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 2), "5.55", "return");

    // number = 5.55; // NOTE: Fix
    // EQ_VALUE(DigitC::NumberToString(number, 0, 0, 1), "5.6",
    // "return");

    number = 5.57;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 1), "5.6", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 1), "5.6", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 2, 1), "5.60", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 3, 1), "005.600", "return");

    number = 0.99;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 1), "1", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 1), "1.0", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 2, 1), "1.00", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 3, 1), "001.000", "return");

    number = 0.99;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 2), ".99", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 2), ".99", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 2, 2), ".99", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 3, 2), "000.990", "return");

    number = 0.99;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 5), ".99", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 5), ".99", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 2, 5), ".99", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 3, 5), "000.990", "return");

    number = 5.99;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 1), "6", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 1), "6.0", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 2, 1), "6.00", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 3, 1), "006.000", "return");

    number = 9.99;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 1), "10", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 1), "10.0", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 2, 1), "10.00", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 3, 1), "010.000", "return");

    number = 90.99;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 1), "91", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 1), "91.0", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 2, 1), "91.00", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 3, 1), "091.000", "return");

    number = 99.99;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 1), "100", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 1, 1), "100.0", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 2, 1), "100.00", "return");
    EQ_VALUE(DigitC::NumberToString(number, 3, 3, 1), "100.000", "return");

    number = 99.0099;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 1), "99", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 2), "99.01", "return");
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 3), "99.01", "return");

    number = 99.0009;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 3), "99.001", "return");

    number = 456789.0029;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 3), "456789.003", "return");

    number = -456789.0024;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 3), "-456789.002", "return");

    EQ_VALUE(DigitC::NumberToString(18446744073709551615ULL), "18446744073709551615", "return");

    EQ_VALUE(DigitC::NumberToString(-9223372036854775807LL), "-9223372036854775807", "return");

    EQ_VALUE(DigitC::NumberToString(9223372036854775807LL), "9223372036854775807", "return");
    END_SUB_TEST;
}

static int TestNumberToString4() {
    double number = -123456;
    EQ_VALUE(DigitC::NumberToString(number, 30, 0, 0), "-000000000000000123456", "return");

    number = -2;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 0), "-2", "return");

    number = -3.1;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 0), "-3.1", "return");

    number = -3.1;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 0), "-3.1", "return");

    number = -22.87;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 2), "-22.87", "return");

    number = -55.0055;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 10), "-55.0055", "return");

    number = -0.123455678987455;
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 0), "-0.123455678987455", "return");

    number = -0.123455678987452;
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 0), "-0.123455678987452", "return");
    number = 0.999999;
    EQ_VALUE(DigitC::NumberToString(number, 0, 0, 5), "1", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 6), "0.999999", "return");
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 10), "0.999999", "return");

    number = 0.123e-11;
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 0), "1.23e-12", "return");

    number = -2.00000000000099;
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 14), "-2.00000000000099", "return");

    number = 3.9999999999999;
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 14), "3.9999999999999", "return");

    number = 99.1005099;
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 10), "99.1005099", "return");

    number = 99.1005099;
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 7), "99.1005099", "return");

    number = 871.080055555;
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 9), "871.080055555", "return");

    number = 0.00056599999999999999;
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 3), "1e-3", "return");

    EQ_VALUE(DigitC::NumberToString(number), "5.66e-4", "return");

    number = 3.99999999999909;
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 0), "3.99999999999909", "return");

    number = 2.00000000000015;
    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 14), "2.00000000000015", "return");

    EQ_VALUE(DigitC::NumberToString(number, 1, 0, 14), "2.00000000000015", "return");

    EQ_VALUE(DigitC::NumberToString(7.0, 1, 3, 3), "7.000", "return");

    EQ_VALUE(DigitC::NumberToString(-7.0, 1, 3, 3), "-7.000", "return");

    EQ_VALUE(DigitC::NumberToString(8.0, 3, 3, 3), "008.000", "return");

    EQ_VALUE(DigitC::NumberToString(-8.0, 3, 3, 3), "-008.000", "return");

    EQ_VALUE(DigitC::NumberToString(7.1, 1, 3, 3), "7.100", "return");

    EQ_VALUE(DigitC::NumberToString(-7.1, 1, 3, 3), "-7.100", "return");

    EQ_VALUE(DigitC::NumberToString(8.1, 3, 3, 3), "008.100", "return");

    EQ_VALUE(DigitC::NumberToString(-8.1, 3, 3, 3), "-008.100", "return");

    EQ_VALUE(DigitC::NumberToString(0.93002099999999999, 1, 3, 3), "0.930", "return");

    EQ_VALUE(DigitC::NumberToString(-9223372036854775807.0, 1), "-9223372036854775808", "return");

    EQ_VALUE(DigitC::NumberToString(-0.9223372036854775, 1, 1), "-0.9223372036854775", "return");
    EQ_VALUE(DigitC::NumberToString(-92233.0, 10), "-0000092233", "return");

    EQ_VALUE(DigitC::NumberToString(-92233.0, 1), "-92233", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775, 1), "0.9223372036854775", "return");

    EQ_VALUE(DigitC::NumberToString(0.9999, 1, 1, 3), "1.0", "return");
    EQ_VALUE(DigitC::NumberToString(-0.9999, 1, 1, 3), "-1.0", "return");

    EQ_VALUE(DigitC::NumberToString(0.9999, 3, 1, 3), "001.0", "return");
    EQ_VALUE(DigitC::NumberToString(-0.9999, 3, 1, 3), "-001.0", "return");

    EQ_VALUE(DigitC::NumberToString(0.9999, 3, 3, 3), "001.000", "return");
    EQ_VALUE(DigitC::NumberToString(-0.9999, 3, 3, 3), "-001.000", "return");
    EQ_VALUE(DigitC::NumberToString(0.9, 1, 1), "0.9", "return");
    EQ_VALUE(DigitC::NumberToString(-0.9, 1, 1), "-0.9", "return");

    EQ_VALUE(DigitC::NumberToString(0.9999, 1, 1), "0.9999", "return");
    EQ_VALUE(DigitC::NumberToString(-0.9999, 1, 1), "-0.9999", "return");

    EQ_VALUE(DigitC::NumberToString(12345678912345.345, 1, 1, 2), "12345678912345.35", "return");

    EQ_VALUE(DigitC::NumberToString(45.12345678912345, 1, 1, 14), "45.12345678912345", "return");

    EQ_VALUE(DigitC::NumberToString(45.12345678912345, 1, 1, 3), "45.123", "return");

    EQ_VALUE(DigitC::NumberToString(-10.9999, 1, 1, 0), "-10.9999", "return");
    EQ_VALUE(DigitC::NumberToString(10.9999, 1, 1, 0), "10.9999", "return");

    EQ_VALUE(DigitC::NumberToString(-10.9999, 1, 1, 2), "-11.0", "return");
    EQ_VALUE(DigitC::NumberToString(10.9999, 1, 1, 2), "11.0", "return");

    END_SUB_TEST;
}

static int TestNumberToString5() {
    EQ_VALUE(DigitC::NumberToString(1e308), "1e308", "return");

    EQ_VALUE(DigitC::NumberToString(1e-308), "1e-308", "return");

    EQ_VALUE(DigitC::NumberToString(15e100), "1.5e101", "return");

    EQ_VALUE(DigitC::NumberToString(1.6866831148613157e308), "1.686683114861316e308", "return");

    EQ_VALUE(DigitC::NumberToString(99999999999999999999.0), "1e20", "return");

    EQ_VALUE(DigitC::NumberToString(0.000000000000000009), "9e-18", "return");

    EQ_VALUE(DigitC::NumberToString(0.000000000009), "9e-12", "return");

    EQ_VALUE(DigitC::NumberToString(-0.000000000009), "-9e-12", "return");

    EQ_VALUE(DigitC::NumberToString(0.100009), "0.100009", "return");

    EQ_VALUE(DigitC::NumberToString(0.100009, 0), ".100009", "return");

    EQ_VALUE(DigitC::NumberToString(0.000009), "9e-6", "return");

    EQ_VALUE(DigitC::NumberToString(1.000009), "1.000009", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 1), "0.9", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 2), "0.92", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 3), "0.922", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 4), "0.9223", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 5), "0.92234", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 6), "0.922337", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 7), "0.9223372", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 8), "0.9223372", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 9), "0.922337204", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 10), "0.9223372037", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 11), "0.92233720369", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 12), "0.922337203685", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 13), "0.9223372036855", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 14), "0.92233720368548", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 15), "0.922337203685478", "return");

    EQ_VALUE(DigitC::NumberToString(0.9223372036854775807, 1, 1, 16), "0.9223372036854776", "return");

    END_SUB_TEST;
}

static int RunDigitTests() {
    STARTING_TEST("Digit.hpp");

    START_TEST("Hex Conversion Test", TestHexConv);

    START_TEST("StringToNumber Test 1", TestStringToNumber1);
    START_TEST("StringToNumber Test 2", TestStringToNumber2);
    START_TEST("StringToNumber Test 3", TestStringToNumber3);
    START_TEST("StringToNumber Test 4", TestStringToNumber4);

    START_TEST("NumberToString Test 1", TestNumberToString1);
    START_TEST("NumberToString Test 2", TestNumberToString2);
    START_TEST("NumberToString Test 3", TestNumberToString3);
    START_TEST("NumberToString Test 4", TestNumberToString4);
    START_TEST("NumberToString Test 5", TestNumberToString5);

    // TODO: Add NumberToStringStream test

    END_TEST("Digit.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
