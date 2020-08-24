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

#include "Digit.hpp"
#include "TestHelper.hpp"

#ifndef QENTEM_DIGITTEST_H_
#define QENTEM_DIGITTEST_H_

namespace Qentem {
namespace Test {

static int TestHexConv() {
    ULong       number;
    const char *hex = nullptr;

    number = Digit::HexStringToNumber(hex);
    SHOULD_EQUAL_VALUE(number, 0, "number");

    hex    = "0000";
    number = Digit::HexStringToNumber(hex);
    SHOULD_EQUAL_VALUE(number, 0, "number");

    hex    = "AAAA";
    number = Digit::HexStringToNumber(hex);
    SHOULD_EQUAL_VALUE(number, 43690, "number");

    hex    = "000A";
    number = Digit::HexStringToNumber(hex);
    SHOULD_EQUAL_VALUE(number, 10, "number");

    hex    = "A000";
    number = Digit::HexStringToNumber(hex);
    SHOULD_EQUAL_VALUE(number, 40960, "number");

    hex    = "ABCDEF";
    number = Digit::HexStringToNumber(hex);
    SHOULD_EQUAL_VALUE(number, 11259375, "number");

    hex    = "FEDCBA";
    number = Digit::HexStringToNumber(hex);
    SHOULD_EQUAL_VALUE(number, 16702650, "number");

    hex    = "abcdef";
    number = Digit::HexStringToNumber(hex);
    SHOULD_EQUAL_VALUE(number, 11259375, "number");

    hex    = "fedcba";
    number = Digit::HexStringToNumber(hex);
    SHOULD_EQUAL_VALUE(number, 16702650, "number");

    hex    = "12345";
    number = Digit::HexStringToNumber(hex);
    SHOULD_EQUAL_VALUE(number, 74565, "number");

    hex    = "6789";
    number = Digit::HexStringToNumber(hex);
    SHOULD_EQUAL_VALUE(number, 26505, "number");

    hex    = "FFFF";
    number = Digit::HexStringToNumber(hex);
    SHOULD_EQUAL_VALUE(number, 65535, "number");

    hex    = "9999";
    number = Digit::HexStringToNumber(hex);
    SHOULD_EQUAL_VALUE(number, 39321, "number");

    END_SUB_TEST;
}

static int TestStringToNumber1() {
    UInt        number;
    const char *str = "";
    bool        valid;

    str   = "";
    valid = Digit::StringToNumber(number, str);
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");
    SHOULD_EQUAL_VALUE(number, 0, "number");

    str   = "0";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_TRUE(valid, "valid");
    SHOULD_EQUAL_VALUE(number, 0, str);

    str   = "1";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_TRUE(valid, "valid");
    SHOULD_EQUAL_VALUE(number, 1, str);

    str   = "1000000";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_TRUE(valid, "valid");
    SHOULD_EQUAL_VALUE(number, 1000000, str);

    str   = "1010101";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_TRUE(valid, "valid");
    SHOULD_EQUAL_VALUE(number, 1010101, str);

    str   = "9999999";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_TRUE(valid, "valid");
    SHOULD_EQUAL_VALUE(number, 9999999, str);

    str   = "123456789";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_TRUE(valid, "valid");
    SHOULD_EQUAL_VALUE(number, 123456789, str);

    END_SUB_TEST;
}

static int TestStringToNumber2() {
    int           number_int;
    unsigned int  number_uint;
    long          number_long;
    unsigned long number_ulong;
    float         number_float;
    double        number_double;

    const char *str = nullptr;
    bool        valid;

    valid = Digit::StringToNumber(number_int, str);
    SHOULD_EQUAL_VALUE(number_int, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    valid = Digit::StringToNumber(number_uint, str);
    SHOULD_EQUAL_VALUE(number_int, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_int, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    valid = Digit::StringToNumber(number_ulong, str);
    SHOULD_EQUAL_VALUE(number_ulong, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1-e";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1+e";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1-e1";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1+e1";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1--e1";
    valid = Digit::StringToNumber(number_ulong, str);
    SHOULD_EQUAL_VALUE(number_ulong, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1++e1";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1e-";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1e+";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1e--1";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1e++1";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1ee1";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1E";
    valid = Digit::StringToNumber(number_uint, str);
    SHOULD_EQUAL_VALUE(number_uint, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "E1";
    valid = Digit::StringToNumber(number_uint, str);
    SHOULD_EQUAL_VALUE(number_uint, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1-E";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1+E";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1-E1";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1+E1";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1--E1";
    valid = Digit::StringToNumber(number_ulong, str);
    SHOULD_EQUAL_VALUE(number_ulong, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1++E1";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1E-";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1E+";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1E--1";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1E++1";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1EE1";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = ".";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    // .0 is valid

    str   = "0.";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = ".0.0";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "0.0.";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "0..0";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "123.";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = ".123.123";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "123.123.";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "123..123";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "12y3";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1x23";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "w123";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "+";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "-";
    valid = Digit::StringToNumber(number_float, str);
    SHOULD_EQUAL_VALUE(number_float, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "+";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "-";
    valid = Digit::StringToNumber(number_long, str);
    SHOULD_EQUAL_VALUE(number_long, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "500000000000000000X000.000000000000";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    END_SUB_TEST;
}

static int TestStringToNumber3() {
    int          number_int;
    unsigned int number_uint;
    double       number_double;

    const char *str = nullptr;
    bool        valid;

    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "-w1";
    valid = Digit::StringToNumber(number_int, str);
    SHOULD_EQUAL_VALUE(number_int, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "+w1";
    valid = Digit::StringToNumber(number_int, str);
    SHOULD_EQUAL_VALUE(number_int, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "+01";
    valid = Digit::StringToNumber(number_int, str);
    SHOULD_EQUAL_VALUE(number_int, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "-01";
    valid = Digit::StringToNumber(number_int, str);
    SHOULD_EQUAL_VALUE(number_int, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "01";
    valid = Digit::StringToNumber(number_int, str);
    SHOULD_EQUAL_VALUE(number_int, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "1e";
    valid = Digit::StringToNumber(number_uint, str);
    SHOULD_EQUAL_VALUE(number_uint, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "e1";
    valid = Digit::StringToNumber(number_uint, str);
    SHOULD_EQUAL_VALUE(number_uint, 0, "number");
    SHOULD_NOT_EQUAL_TRUE(valid, "valid");

    str   = "0";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 0, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-1";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, -1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "+1";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "+1";
    valid = Digit::StringToNumber(number_uint, str);
    SHOULD_EQUAL_VALUE(number_uint, 1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-1";
    valid = Digit::StringToNumber(number_int, str);
    SHOULD_EQUAL_VALUE(number_int, -1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "1";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "1.0";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "1.1";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 1.1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "123456789";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 123456789, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "1.123456789";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 1.123456789, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "123456789.123456789";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 123456789.123456789, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "1e10";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 1e10, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "1E10";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 1e10, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "1e+10";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 1e10, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "1E+10";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 1e10, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "5000000000000000000000000000000000";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 5e33, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "5000000000000000000000.000000000000";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 5e21, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "1E5";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 1e5, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "1e-10";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 1e-10, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "1E-10";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 1e-10, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "0.3";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 0.3, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "0.2";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 0.2, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "0.1";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 0.1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "2.3";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 2.3, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "3.2";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 3.2, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "5.1";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, 5.1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-1";
    valid = Digit::StringToNumber(number_double, str);
    SHOULD_EQUAL_VALUE(number_double, -1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    END_SUB_TEST;
}

static int TestStringToNumber4() {
    const char *str = nullptr;
    double      number;
    bool        valid;

    str   = "-1.0";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-1.1";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -1.1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-123456789";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -123456789, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-1.123456789";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -1.123456789, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-123456789.123456789";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -123456789.123456789, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-1e10";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -1e10, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-1E10";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -1e10, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-1e+10";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -1e10, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-1E+10";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -1e10, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-1e-10";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -1e-10, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-1E-10";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -1e-10, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-0.3";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -0.3, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-0.2";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -0.2, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-0.1";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -0.1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-2.3";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -2.3, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-3.2";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -3.2, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

    str   = "-5.1";
    valid = Digit::StringToNumber(number, str);
    SHOULD_EQUAL_VALUE(number, -5.1, "number");
    SHOULD_EQUAL_TRUE(valid, "valid");

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
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_int), "-1", "return");

    number_int = 1;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_int), "1", "return");

    number_long = -100000;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_long), "-100000", "return");

    number_long = 100000;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_long), "100000", "return");

    number_uint = 0;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_uint, 0), "", "return");

    number_ulong = 1;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_ulong, 10), "0000000001",
                       "return");

    number_uint = 10;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_uint, 2), "10", "return");

    number_ulong = 3;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_ulong, 5), "00003",
                       "return");

    number_ulong = 3;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_ulong, 5), "00003",
                       "return");

    number_float = 1;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_float, 1), "1", "return");

    number_double = 0;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 0, 0), "",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 0, 0), "0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 1, 0), ".0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 1, 0), "0.0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 3, 0, 0), "000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 5, 0), ".00000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 2, 2, 0), "00.00",
                       "return");

    number_double = 1;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 0, 0), "1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 0, 0), "1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 1, 0), "1.0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 1, 0), "1.0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 3, 0, 0), "001",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 5, 0), "1.00000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 2, 2, 0), "01.00",
                       "return");

    number_double = 15;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 0, 0), "15",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 0, 0), "15",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 1, 0), "15.0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 1, 0), "15.0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 3, 0, 0), "015",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 5, 0),
                       "15.00000", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 2, 2, 0), "15.00",
                       "return");

    number_double = 12345;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 0, 0), "12345",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 0, 0), "12345",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 1, 0), "12345.0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 1, 0), "12345.0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 3, 0, 0), "12345",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 5, 0),
                       "12345.00000", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 2, 2, 0),
                       "12345.00", "return");

    number_double = 1.1;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 0, 0), "1.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 0, 0), "1.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 1, 0), "1.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 1, 0), "1.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 3, 0, 0), "001.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 5, 0), "1.10000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 2, 2, 0), "01.10",
                       "return");

    number_double = 100.5;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 0, 0), "100.5",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 0, 0), "100.5",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 1, 0), "100.5",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 1, 0), "100.5",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 3, 0, 0), "100.5",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 5, 0),
                       "100.50000", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 2, 2, 0), "100.50",
                       "return");

    number_double = 1.123456;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 0, 1), "1.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 0, 1), "1.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 1, 1), "1.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 1, 1), "1.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 3, 0, 1), "001.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 5, 1), "1.10000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 2, 2, 1), "01.10",
                       "return");

    number_double = 80.123456;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 0, 1), "80.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 0, 1), "80.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 1, 1), "80.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 1, 1), "80.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 3, 0, 1), "080.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 5, 1),
                       "80.10000", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 2, 2, 1), "80.10",
                       "return");

    number_double = 12345.123456;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 0, 1), "12345.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 0, 1), "12345.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 1, 1), "12345.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 1, 1), "12345.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 3, 0, 1), "12345.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 5, 1),
                       "12345.10000", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 2, 2, 1),
                       "12345.10", "return");

    number_double = 1.123456;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 0, 1), "1.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 0, 1), "1.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 1, 1), "1.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 1, 1), "1.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 3, 0, 1), "001.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 5, 1), "1.10000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 2, 2, 1), "01.10",
                       "return");

    number_double = 100.123456;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 0, 1), "100.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 0, 1), "100.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 1, 1), "100.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 1, 1), "100.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 3, 0, 1), "100.1",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 5, 1),
                       "100.10000", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 2, 2, 1), "100.10",
                       "return");

    number_double = 100.523456;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 0, 1), "100.5",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 0, 1), "100.5",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 1, 1), "100.5",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 1, 1, 1), "100.5",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 3, 0, 1), "100.5",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 0, 5, 1),
                       "100.50000", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number_double, 2, 2, 1), "100.50",
                       "return");

    END_SUB_TEST;
}

static int TestNumberToString2() {
    double number = 1.123456;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 2), "1.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 2), "1.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 2), "1.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 1, 2), "1.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 0, 2), "001.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 5, 2), "1.12000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 2, 2, 2), "01.12",
                       "return");

    number = 80.123456;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 2), "80.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 2), "80.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 2), "80.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 1, 2), "80.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 0, 2), "080.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 5, 2), "80.12000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 2, 2, 2), "80.12",
                       "return");

    number = 12345.123456;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 2), "12345.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 2), "12345.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 2), "12345.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 1, 2), "12345.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 0, 2), "12345.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 5, 2), "12345.12000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 2, 2, 2), "12345.12",
                       "return");

    number = 1.123456;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 2), "1.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 2), "1.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 2), "1.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 1, 2), "1.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 0, 2), "001.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 5, 2), "1.12000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 2, 2, 2), "01.12",
                       "return");

    number = 100.123456;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 2), "100.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 2), "100.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 2), "100.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 1, 2), "100.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 0, 2), "100.12",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 5, 2), "100.12000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 2, 2, 2), "100.12",
                       "return");

    number = 100.523456;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 2), "100.52",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 2), "100.52",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 2), "100.52",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 1, 2), "100.52",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 0, 2), "100.52",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 5, 2), "100.52000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 2, 2, 2), "100.52",
                       "return");

    number = 100.1234;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 5), "100.1234",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 5), "100.1234",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 5), "100.1234",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 1, 5), "100.1234",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 0, 5), "100.1234",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 5, 5), "100.12340",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 2, 2, 5), "100.1234",
                       "return");
    END_SUB_TEST;
}

static int TestNumberToString3() {
    double number;

    number = 5.15;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 3), "5.15",
                       "return");

    number = 5.55;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 2), "5.55",
                       "return");

    // number = 5.55; // TODO: Fix
    // SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 1), "5.6",
    // "return");

    number = 5.57;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 1), "5.6", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 1), "5.6", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 2, 1), "5.60",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 3, 1), "005.600",
                       "return");

    number = 0.99;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 1), "1", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 1), "1.0", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 2, 1), "1.00",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 3, 1), "001.000",
                       "return");

    number = 0.99;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 2), ".99", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 2), ".99", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 2, 2), ".99", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 3, 2), "000.990",
                       "return");

    number = 0.99;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 5), ".99", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 5), ".99", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 2, 5), ".99", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 3, 5), "000.990",
                       "return");

    number = 5.99;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 1), "6", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 1), "6.0", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 2, 1), "6.00",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 3, 1), "006.000",
                       "return");

    number = 9.99;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 1), "10", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 1), "10.0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 2, 1), "10.00",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 3, 1), "010.000",
                       "return");

    number = 90.99;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 1), "91", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 1), "91.0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 2, 1), "91.00",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 3, 1), "091.000",
                       "return");

    number = 99.99;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 1), "100", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 1, 1), "100.0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 2, 1), "100.00",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 3, 3, 1), "100.000",
                       "return");

    number = 99.0099;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 1), "99", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 2), "99.01",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 3), "99.01",
                       "return");

    number = 99.0009;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 3), "99.001",
                       "return");

    number = 456789.0029;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 3), "456789.003",
                       "return");

    number = -456789.0024;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 3), "-456789.002",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(18446744073709551615ULL),
                       "18446744073709551615", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(-9223372036854775807LL),
                       "-9223372036854775807", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(9223372036854775807LL),
                       "9223372036854775807", "return");
    END_SUB_TEST;
}

static int TestNumberToString4() {
    double number = -123456;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 30, 0, 0),
                       "-000000000000000000000000123456", "return");

    number = -2;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 0), "-2", "return");

    number = -3.1;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 0), "-3.1",
                       "return");

    number = -3.1;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 0), "-3.1",
                       "return");

    number = -22.87;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 2), "-22.87",
                       "return");

    number = -55.0055;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 10), "-55.0055",
                       "return");

    number = -0.123455678987455;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 0),
                       "-0.123455678987455", "return");

    number = -0.123455678987452;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 0),
                       "-0.123455678987452", "return");
    number = 0.999999;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 0, 0, 5), "1", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 6), "0.999999",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 10), "0.999999",
                       "return");

    number = 0.123e-11;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 0), "1.23e-12",
                       "return");

    number = -2.00000000000099;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 14),
                       "-2.00000000000099", "return");

    number = 3.9999999999999;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 14),
                       "3.9999999999999", "return");

    number = 99.1005099;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 10), "99.1005099",
                       "return");

    number = 99.1005099;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 7), "99.1005099",
                       "return");

    number = 871.080055555;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 9), "871.080055555",
                       "return");

    number = 0.00056599999999999999;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 3), "1e-3",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(number), "5.66e-4", "return");

    number = 3.99999999999909;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 0),
                       "3.99999999999909", "return");

    number = 2.00000000000015;
    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 14),
                       "2.00000000000015", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(number, 1, 0, 14),
                       "2.00000000000015", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(7.0, 1, 3, 3), "7.000", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(-7.0, 1, 3, 3), "-7.000",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(8.0, 3, 3, 3), "008.000",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(-8.0, 3, 3, 3), "-008.000",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(7.1, 1, 3, 3), "7.100", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(-7.1, 1, 3, 3), "-7.100",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(8.1, 3, 3, 3), "008.100",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(-8.1, 3, 3, 3), "-008.100",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.93002099999999999, 1, 3, 3),
                       "0.930", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(-9223372036854775807.0, 1),
                       "-9223372036854775808", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(-0.9223372036854775, 1, 1),
                       "-0.9223372036854775", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(-92233.0, 10), "-0000092233",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(-92233.0, 1), "-92233", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775, 1),
                       "0.9223372036854775", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9999, 1, 1, 3), "1.0", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(-0.9999, 1, 1, 3), "-1.0",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9999, 3, 1, 3), "001.0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(-0.9999, 3, 1, 3), "-001.0",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9999, 3, 3, 3), "001.000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(-0.9999, 3, 3, 3), "-001.000",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9, 1, 1), "0.9", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(-0.9, 1, 1), "-0.9", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9999, 1, 1), "0.9999", "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(-0.9999, 1, 1), "-0.9999",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(12345678912345.345, 1, 1, 2),
                       "12345678912345.35", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(45.12345678912345, 1, 1, 14),
                       "45.12345678912345", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(45.12345678912345, 1, 1, 3),
                       "45.123", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(-10.9999, 1, 1, 0), "-10.9999",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(10.9999, 1, 1, 0), "10.9999",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(-10.9999, 1, 1, 2), "-11.0",
                       "return");
    SHOULD_EQUAL_VALUE(Digit::NumberToString(10.9999, 1, 1, 2), "11.0",
                       "return");

    END_SUB_TEST;
}

static int TestNumberToString5() {
    SHOULD_EQUAL_VALUE(Digit::NumberToString(1e308), "1e308", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(1e-308), "1e-308", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(15e100), "1.5e101", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(99999999999999999999.0), "1e20",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.000000000000000009), "9e-18",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.000000000009), "9e-12",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(-0.000000000009), "-9e-12",
                       "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.100009), "0.100009", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.100009, 0), ".100009", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.000009), "9e-6", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(1.000009), "1.000009", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 1),
                       "0.9", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 2),
                       "0.92", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 3),
                       "0.922", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 4),
                       "0.9223", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 5),
                       "0.92234", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 6),
                       "0.922337", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 7),
                       "0.9223372", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 8),
                       "0.9223372", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 9),
                       "0.922337204", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 10),
                       "0.9223372037", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 11),
                       "0.92233720369", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 12),
                       "0.922337203685", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 13),
                       "0.9223372036855", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 14),
                       "0.92233720368548", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 15),
                       "0.922337203685478", "return");

    SHOULD_EQUAL_VALUE(Digit::NumberToString(0.9223372036854775807, 1, 1, 16),
                       "0.9223372036854776", "return");

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

    END_TEST("Digit.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
