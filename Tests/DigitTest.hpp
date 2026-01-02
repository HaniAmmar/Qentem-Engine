/*
 * Copyright (c) 2026 Hani Ammar
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

#ifndef QENTEM_DIGIT_TESTS_H
#define QENTEM_DIGIT_TESTS_H

// #define QENTEM_COMPARE_DIGIT_WITH_STL true

#ifdef QENTEM_COMPARE_DIGIT_WITH_STL
#include <iomanip>
#include <sstream>
#include <string>
#endif

#include "Qentem/QTest.hpp"
#include "Qentem/Digit.hpp"
#include "Qentem/StringStream.hpp"

namespace Qentem {
namespace Test {

static SizeT64 HexStringToNumber(const char *str) noexcept {
    return Digit::HexStringToNumber<SizeT64>(str, Qentem::StringUtils::Count(str));
}

template <typename Number_T>
QENTEM_NOINLINE static void IntToStreamEqual(QTest &test, StringStream<char> &stream, Number_T number,
                                             const char *expected, unsigned long line) {
    Digit::NumberToString(stream, number);
    test.IsEqual(stream, expected, line);
    stream.Clear();
}

template <typename Number_T>
QENTEM_NOINLINE static void RealToStreamEqual(QTest &test, StringStream<char> &stream, Number_T number,
                                              Digit::RealFormatInfo format, const char *expected, unsigned long line) {
    Digit::NumberToString(stream, number, format);

    if (!test.HasError() || test.ContinueOnErrorEnabled()) {
#ifdef QENTEM_COMPARE_DIGIT_WITH_STL
        std::ostringstream out;
        out.precision(format.Precision);
        out << number;

        test.IsEqual(stream, out.str().c_str(), line);

        (void)expected;
#else
        test.IsEqual(stream, expected, line);
#endif

        stream.Clear();
    }
}

template <typename Number_T>
QENTEM_NOINLINE static void RealToStreamEqualFixed(QTest &test, StringStream<char> &stream, Number_T number,
                                                   SizeT32 precision, const char *expected, unsigned long line) {
    Digit::NumberToString(stream, number, Digit::RealFormatInfo{precision, Digit::RealFormatType::Fixed});

    if (!test.HasError() || test.ContinueOnErrorEnabled()) {
#ifdef QENTEM_COMPARE_DIGIT_WITH_STL
        std::ostringstream out;
        out.precision(precision);
        out << std::fixed;
        out << number;

        test.IsEqual(stream, out.str().c_str(), line);

        (void)expected;
#else
        test.IsEqual(stream, expected, line);
#endif

        stream.Clear();
    }
}

template <typename Number_T>
QENTEM_NOINLINE static void RealToStreamEqualSemiFixed(QTest &test, StringStream<char> &stream, Number_T number,
                                                       SizeT32 precision, const char *expected, unsigned long line) {
    constexpr SizeT offset{10};

    stream.SetLength(offset);
    Digit::NumberToString(stream, number, Digit::RealFormatInfo{precision, Digit::RealFormatType::SemiFixed});

    stream.InsertNull();
    const StringView<char> str_number{(stream.First() + offset), SizeT(stream.Length() - offset)};

    if (!test.HasError() || test.ContinueOnErrorEnabled()) {
        test.IsEqual(str_number, expected, line);
        stream.Clear();
    }
}

template <typename Number_T>
QENTEM_NOINLINE static bool StringToNumber(const QTest &test, Number_T &num, const char *str) noexcept {
    QNumber64   number;
    SizeT       offset = 0;
    const SizeT length = StringUtils::Count(str);

    if (!test.HasError() || test.ContinueOnErrorEnabled()) {
        switch (Digit::StringToNumber(number, str, offset, length)) {
            case QNumberType::Natural: {
                num = Number_T(number.Natural);
                break;
            }

            case QNumberType::Integer: {
                num = Number_T(number.Integer);
                break;
            }

            case QNumberType::Real: {
                num = Number_T(number.Real);
                break;
            }

            default: {
                num = 0;
                return false;
            }
        };
    }

    return (offset >= length);
}

static void TestStringToNumber1(QTest &test) {
    SizeT64     number   = 0;
    double      d_number = 0;
    const char *str;

    str = "";
    test.IsFalse(StringToNumber(test, number, str), __LINE__);

    str = "0";
    test.IsTrue(StringToNumber(test, number, str), __LINE__);
    test.IsEqual(number, 0U, __LINE__);

    str = "0.0";
    test.IsTrue(StringToNumber(test, d_number, str), __LINE__);
    test.IsEqual(d_number, 0.0, __LINE__);

    str = "-0";
    test.IsTrue(StringToNumber(test, d_number, str), __LINE__);
    test.IsEqual(d_number, -0.0, __LINE__);

    str = "-0.0";
    test.IsTrue(StringToNumber(test, d_number, str), __LINE__);
    test.IsEqual(d_number, -0.0, __LINE__);

    str = "1";
    test.IsTrue(StringToNumber(test, number, str), __LINE__);
    test.IsEqual(number, 1U, __LINE__);

    str = "1000000";
    test.IsTrue(StringToNumber(test, number, str), __LINE__);
    test.IsEqual(number, 1000000U, __LINE__);

    str = "1010101";
    test.IsTrue(StringToNumber(test, number, str), __LINE__);
    test.IsEqual(number, 1010101U, __LINE__);

    str = "9999999";
    test.IsTrue(StringToNumber(test, number, str), __LINE__);
    test.IsEqual(number, 9999999U, __LINE__);

    str = "123456789";
    test.IsTrue(StringToNumber(test, number, str), __LINE__);
    test.IsEqual(number, 123456789U, __LINE__);
}

static void TestStringToNumber2(QTest &test) {
    int           number_int    = 0;
    SizeT32       number_uint   = 0;
    long          number_long   = 0;
    unsigned long number_ulong  = 0;
    float         number_float  = 0;
    double        number_double = 0;

    const char *str;
    bool        valid;

    str   = "1-e";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1+e";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1-e1";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1+e1";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1--e1";
    valid = StringToNumber(test, number_ulong, str);
    test.IsFalse(valid, __LINE__);

    str   = "1++e1";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "1e-";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1e+";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1e--1";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1e++1";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1ee1";
    valid = StringToNumber(test, number_double, str);
    test.IsFalse(valid, __LINE__);

    str   = "1E";
    valid = StringToNumber(test, number_uint, str);
    test.IsFalse(valid, __LINE__);

    str   = "E1";
    valid = StringToNumber(test, number_uint, str);
    test.IsFalse(valid, __LINE__);

    str   = "1-E";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1+E";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1-E1";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1+E1";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1--E1";
    valid = StringToNumber(test, number_ulong, str);
    test.IsFalse(valid, __LINE__);

    str   = "1++E1";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "1E-";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1E+";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1E--1";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1E++1";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1EE1";
    valid = StringToNumber(test, number_double, str);
    test.IsFalse(valid, __LINE__);

    str   = ".";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "..";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "...";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = ".  ";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "..  ";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "...  ";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "+.";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "-.";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "+..";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "+...";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "-..";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "-...";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = ".0.0";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "0.0.";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "0..0";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "0..0]]]";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = ".123.123";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "123.123.";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "123..123";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "12y3";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "1x23";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "w123";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "+";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "-";
    valid = StringToNumber(test, number_float, str);
    test.IsFalse(valid, __LINE__);

    str   = "+";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "-";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1e";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1e+";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "1e-";
    valid = StringToNumber(test, number_long, str);
    test.IsFalse(valid, __LINE__);

    str   = "-w1";
    valid = StringToNumber(test, number_int, str);
    test.IsFalse(valid, __LINE__);

    str   = "+w1";
    valid = StringToNumber(test, number_int, str);
    test.IsFalse(valid, __LINE__);

    str   = "01";
    valid = StringToNumber(test, number_int, str);
    test.IsFalse(valid, __LINE__);

    str   = "+01";
    valid = StringToNumber(test, number_int, str);
    test.IsFalse(valid, __LINE__);

    str   = "-01";
    valid = StringToNumber(test, number_int, str);
    test.IsFalse(valid, __LINE__);

    str   = "00001";
    valid = StringToNumber(test, number_int, str);
    test.IsFalse(valid, __LINE__);

    str   = "+00001";
    valid = StringToNumber(test, number_int, str);
    test.IsFalse(valid, __LINE__);

    str   = "-00001";
    valid = StringToNumber(test, number_int, str);
    test.IsFalse(valid, __LINE__);

    str   = "0123456";
    valid = StringToNumber(test, number_int, str);
    test.IsFalse(valid, __LINE__);

    str   = "+0123456";
    valid = StringToNumber(test, number_int, str);
    test.IsFalse(valid, __LINE__);

    str   = "-0123456";
    valid = StringToNumber(test, number_int, str);
    test.IsFalse(valid, __LINE__);

    str   = "1e";
    valid = StringToNumber(test, number_uint, str);
    test.IsFalse(valid, __LINE__);

    str   = "e1";
    valid = StringToNumber(test, number_uint, str);
    test.IsFalse(valid, __LINE__);

    str   = "e12";
    valid = StringToNumber(test, number_uint, str);
    test.IsFalse(valid, __LINE__);

    str   = "1-e";
    valid = StringToNumber(test, number_uint, str);
    test.IsFalse(valid, __LINE__);

    str   = "e-1";
    valid = StringToNumber(test, number_uint, str);
    test.IsFalse(valid, __LINE__);

    str   = "-e12";
    valid = StringToNumber(test, number_uint, str);
    test.IsFalse(valid, __LINE__);

    str   = "1+e";
    valid = StringToNumber(test, number_uint, str);
    test.IsFalse(valid, __LINE__);

    str   = "+e1";
    valid = StringToNumber(test, number_uint, str);
    test.IsFalse(valid, __LINE__);

    str   = "e+12";
    valid = StringToNumber(test, number_uint, str);
    test.IsFalse(valid, __LINE__);

    str   = "500000000000000000X000.000000000000";
    valid = StringToNumber(test, number_double, str);
    test.IsFalse(valid, __LINE__);

    str   = "1e-330";
    valid = StringToNumber(test, number_double, str);
    test.IsFalse(valid, __LINE__);

    str   = "1e+310";
    valid = StringToNumber(test, number_double, str);
    test.IsFalse(valid, __LINE__);

    str   = "1e+999";
    valid = StringToNumber(test, number_double, str);
    test.IsFalse(valid, __LINE__);

    str   = "1e-999";
    valid = StringToNumber(test, number_double, str);
    test.IsFalse(valid, __LINE__);
}

static void TestStringToNumber3(QTest &test) {
    const char *content = nullptr;
    QNumber64   number;
    QNumberType type   = QNumberType::NotANumber;
    SizeT       offset = 0;

    content = "0]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Natural, __LINE__);
    test.IsEqual(number.Natural, 0U, __LINE__);

    offset  = 0;
    content = ".0]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Real, __LINE__);
    test.IsEqual(number.Real, 0.0, __LINE__);

    offset  = 0;
    content = "0.]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Real, __LINE__);
    test.IsEqual(number.Real, 0.0, __LINE__);

    offset  = 0;
    content = "0.0]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Real, __LINE__);
    test.IsEqual(number.Real, -0.0, __LINE__);

    offset  = 0;
    content = "-0]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Real, __LINE__);
    test.IsEqual(number.Real, -0.0, __LINE__);

    offset  = 0;
    content = "-.0]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Real, __LINE__);
    test.IsEqual(number.Real, -0.0, __LINE__);

    offset  = 0;
    content = "-0.]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Real, __LINE__);
    test.IsEqual(number.Real, -0.0, __LINE__);

    offset  = 0;
    content = "-0.0]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Real, __LINE__);
    test.IsEqual(number.Real, -0.0, __LINE__);

    offset  = 0;
    content = "1]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Natural, __LINE__);
    test.IsEqual(number.Natural, 1U, __LINE__);

    offset  = 0;
    content = ".2]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Real, __LINE__);
    test.IsEqual(number.Real, 0.2, __LINE__);

    offset  = 0;
    content = "3.]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Real, __LINE__);
    test.IsEqual(number.Real, 3.0, __LINE__);

    offset  = 0;
    content = "4.5]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Real, __LINE__);
    test.IsEqual(number.Real, 4.5, __LINE__);

    offset  = 0;
    content = "-1]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Integer, __LINE__);
    test.IsEqual(number.Integer, -1, __LINE__);

    offset  = 0;
    content = "-.2]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Real, __LINE__);
    test.IsEqual(number.Real, -0.2, __LINE__);

    offset  = 0;
    content = "-3.]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Real, __LINE__);
    test.IsEqual(number.Real, -3.0, __LINE__);

    offset  = 0;
    content = "-4.5]";
    type    = Digit::StringToNumber(number, content, offset, StringUtils::Count(content));
    test.IsTrue(type == QNumberType::Real, __LINE__);
    test.IsEqual(number.Real, -4.5, __LINE__);
}

static void TestStringToNumber4(QTest &test) {
    int     number_int    = 0;
    SizeT32 number_uint   = 0;
    double  number_double = 0;

    const char *str;
    bool        valid;

    str   = "-1";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, -1.0, __LINE__);

    str   = "+1";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 1.0, __LINE__);

    str   = "+1";
    valid = StringToNumber(test, number_uint, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_uint, 1U, __LINE__);

    str   = "-1";
    valid = StringToNumber(test, number_int, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_int, -1, __LINE__);

    str   = "1";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 1.0, __LINE__);

    str   = "1.0";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 1.0, __LINE__);

    str   = "1.00001";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 1.00001, __LINE__);

    str   = "1.1";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 1.1, __LINE__);

    str   = "123456789";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 123456789.0, __LINE__);

    str   = "1.48828125";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 1.48828125, __LINE__);

    str   = "1e10";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 1e10, __LINE__);

    str   = "1E10";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 1e10, __LINE__);

    str   = "1e+10";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 1e10, __LINE__);

    str   = "1E+10";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 1e10, __LINE__);

    str   = "1E5";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 1e5, __LINE__);

    str   = "0.3";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 0.3, __LINE__);

    str   = "0.2";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 0.2, __LINE__);

    str   = "0.1";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 0.1, __LINE__);

    str   = "0.5";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 0.5, __LINE__);

    str   = "4.5";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 4.5, __LINE__);

    str   = "2.3";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 2.3, __LINE__);

    str   = "3.2";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 3.2, __LINE__);

    str   = "5.1";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 5.1, __LINE__);

    str   = "-1";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, -1, __LINE__);

    str   = ".123";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, .123, __LINE__);

    str   = "-123.";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, -123., __LINE__);

    str   = "-.123";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, -.123, __LINE__);

    str   = "+123.";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, +123., __LINE__);

    str   = "+.123";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, +.123, __LINE__);

    str   = "123.";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 123., __LINE__);

    str   = ".123e00";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, .123e00, __LINE__);

    str   = "-123.e00";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, -123.e00, __LINE__);

    str   = "-.123e00";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, -.123e00, __LINE__);

    str   = "+123.e00";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, +123.e00, __LINE__);

    str   = "+.123e00";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, +.123e00, __LINE__);

    str = "123.e00 ";
    StringToNumber(test, number_double, str);
    test.IsEqual(number_double, 123.e00, __LINE__);

    str = ".123e00 ";
    StringToNumber(test, number_double, str);
    test.IsEqual(number_double, .123e00, __LINE__);

    str = "-123.e00 ";
    StringToNumber(test, number_double, str);
    test.IsEqual(number_double, -123.e00, __LINE__);

    str = "-.123e00 ";
    StringToNumber(test, number_double, str);
    test.IsEqual(number_double, -.123e00, __LINE__);

    str = "+123.e00 ";
    StringToNumber(test, number_double, str);
    test.IsEqual(number_double, +123.e00, __LINE__);

    str = "+.123e00 ";
    StringToNumber(test, number_double, str);
    test.IsEqual(number_double, +.123e00, __LINE__);

    str = "123.e00 ";
    StringToNumber(test, number_double, str);
    test.IsEqual(number_double, 123.e00, __LINE__);

    str = ".123 ";
    StringToNumber(test, number_double, str);
    test.IsEqual(number_double, .123, __LINE__);

    str = "-123. ";
    StringToNumber(test, number_double, str);
    test.IsEqual(number_double, -123., __LINE__);

    str = "-.123 ";
    StringToNumber(test, number_double, str);
    test.IsEqual(number_double, -.123, __LINE__);

    str = "+123. ";
    StringToNumber(test, number_double, str);
    test.IsEqual(number_double, +123., __LINE__);

    str = "+.123 ";
    StringToNumber(test, number_double, str);
    test.IsEqual(number_double, +.123, __LINE__);

    str   = ".0";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 0.0, __LINE__);

    str   = "0.";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 0.0, __LINE__);

    str   = "123456789.123456789";
    valid = StringToNumber(test, number_double, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number_double, 123456789.123456789, __LINE__);
}

static void TestStringToNumber5(QTest &test) {
    const char *str        = nullptr;
    double      d_number   = 0;
    SizeT64I    ll_number  = 0;
    SizeT64     ull_number = 0;
    bool        valid;

    str   = "-1.0";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1, __LINE__);

    str   = "-1.1";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1.1, __LINE__);

    str   = "-123456789";
    valid = StringToNumber(test, ll_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(ll_number, -123456789, __LINE__);

    str   = "-1.48828125";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1.48828125, __LINE__);

    str   = "-1e10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1e10, __LINE__);

    str   = "-1E10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1e10, __LINE__);

    str   = "-1e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1e10, __LINE__);

    str   = "-1e+9";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1e9, __LINE__);

    str   = "-1E+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1e10, __LINE__);

    str   = "-1e00";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1, __LINE__);

    str   = "-1e-00";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1, __LINE__);

    str   = "-1e+00";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1, __LINE__);

    str   = "1e-00";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1, __LINE__);

    str   = "1e+00";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1, __LINE__);

    str   = "1e-5";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-5, __LINE__);

    str   = "1e5";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e5, __LINE__);

    str   = "1e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-10, __LINE__);

    str   = "1e+9";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e9, __LINE__);

    str   = "-1e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1e-10, __LINE__);

    str   = "1E-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-10, __LINE__);

    str   = "-1E-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1e-10, __LINE__);

    str   = "-0.3";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -0.3, __LINE__);

    str   = "-0.2";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -0.2, __LINE__);

    str   = "-0.1";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -0.1, __LINE__);

    str   = "-2.3";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -2.3, __LINE__);

    str   = "-3.2";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -3.2, __LINE__);

    str   = "-4.";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -4., __LINE__);

    str   = "-5.1";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -5.1, __LINE__);

    str   = "-0";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -0.0, __LINE__);

    str   = "-0.0";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -0.0, __LINE__);

    str   = "-123456789.12345679";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -123456789.123456789, __LINE__);

    str   = "1844674407370955161";
    valid = StringToNumber(test, ull_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(ull_number, 1844674407370955161ULL, __LINE__);

    str   = "-1844674407370955161";
    valid = StringToNumber(test, ll_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(ll_number, -1844674407370955161LL, __LINE__);

    str   = "18446744073709551615";
    valid = StringToNumber(test, ull_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(ull_number, 18446744073709551615ULL, __LINE__);

    str   = "18446744073709551616";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 18446744073709551616.0, __LINE__);

    str   = "-18446744073709551615";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -18446744073709551615.0, __LINE__);

    str   = "-18446744073709551616";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -18446744073709551616.0, __LINE__);

    str   = "-18446744073709551616e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -18446744073709551616e-10, __LINE__);

    str   = "+18446744073709551616e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, +18446744073709551616e+10, __LINE__);

    str   = "+18446744073709551615e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, +18446744073709551615e+10, __LINE__);

    str   = "-18446744073709551615e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -18446744073709551615e-10, __LINE__);

    str   = "-184467440737095516155";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -184467440737095516155.0, __LINE__);

    str   = "184467440737095516155";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 184467440737095516155.0, __LINE__);

    str   = "9223372036854775807";
    valid = StringToNumber(test, ll_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(ll_number, 9223372036854775807LL, __LINE__);

    str   = "-9223372036854775807";
    valid = StringToNumber(test, ll_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(ll_number, -9223372036854775807LL, __LINE__);

    str   = "-9223372036854775808";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -9223372036854775808.0, __LINE__);

    str   = "-9223372036854775808.0";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -9223372036854775808.0, __LINE__);

    str   = "5000000000000000000000000000000000";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5e33, __LINE__);

    str   = "5000000000000000000000.000000000000";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5e21, __LINE__);

    str   = "5000000000000000000000000000000000e-15";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5000000000000000000.0, __LINE__);

    str   = "5000000000000000000000e100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5.000000000000000072e+121, __LINE__);

    str   = "100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e92, __LINE__);

    str   = "200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 2e+92, __LINE__);

    str   = "999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e93, __LINE__);

    str   = "340282366920938463463374607431768211455";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 340282366920938463463374607431768211455.0, __LINE__);

    str   = "115792089237316195423570985008687907853269984665640564039457584007913129639935";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 115792089237316195423570985008687907853269984665640564039457584007913129639935.0, __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 3.123456789123456812, __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 31234567891.23456955, __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 3.123456789123456789123456789123456789123456789123456789123456789e-10, __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789e100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 3.123456789123456643e+100, __LINE__);

    str   = "3.123456789123456789123456789123456789123456789123456789123456789e-100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 3.1234567891234566e-100, __LINE__);

    str =
        "3.123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789e-308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 3.123456789123456856e-308, __LINE__);

    str =
        "1.7976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157e+300";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1.797693134862315648e+300, __LINE__);

    str =
        "17976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157797693134862315779769313486231577976931348623157e+52";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1.797693134862315708e+308, __LINE__);

    str   = "-1.7976931348623157e308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1.7976931348623157e308, __LINE__);

    str   = "1.7976931348623157e+300";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1.797693134862315648e+300, __LINE__);

    str   = "1.7976931348623157e+1";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 17.97693134862315745, __LINE__);

    str   = "4856312379865454687821527487362783273278723787e-7";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 4.856312379865454319e+38, __LINE__);

    str   = "4856312379865454687821527487362783273278723787e+7";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 4.856312379865455113e+52, __LINE__);

    str   = "1.0e308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1.0e308, __LINE__);

    str   = "-1.0e308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1.0e308, __LINE__);

    str   = "1e308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e308, __LINE__);

    str   = "-1e308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1e308, __LINE__);

    str   = "1.0e-308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 9.999999999999999093e-309, __LINE__);

    str   = "-1.0e-308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1.0e-308, __LINE__);

    str   = "1e-308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-308, __LINE__);

    str   = "-1e-308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -1e-308, __LINE__);

    str   = "4.9406564584124654E-324";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 4.9406564584124654E-324, __LINE__);

    str   = "4.940656458412465442e-324";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 4.940656458412465442e-324, __LINE__);
}

static void TestStringToNumber6(QTest &test) {
    const char *str      = nullptr;
    double      d_number = 0;
    bool        valid;

    str   = "-12.34";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -12.34, __LINE__);

    str   = "1.00000000";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1, __LINE__);

    str   = "1.05";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1.05, __LINE__);

    str   = "0.25";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.25, __LINE__);

    str   = "1.00005";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1.00005, __LINE__);

    str   = "-0.00025";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, -0.00025, __LINE__);

    str   = "0.3743";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.3743, __LINE__);

    str   = "458.3743";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 458.3743, __LINE__);

    str   = "458.3";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 458.3, __LINE__);

    str   = "4.3";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 4.3, __LINE__);

    str   = "1123.0005";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1123.0005, __LINE__);

    str   = "25e-5";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 25e-5, __LINE__);

    str   = "1.000025";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1.000025, __LINE__);

    str   = "10.005e1";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 10.005e1, __LINE__);

    str =
        "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 9.999999999999999666e+117, __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.0000000000000000000000000000000000000000000000001, __LINE__);

    str   = "0.0000000000000000000000005555";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.0000000000000000000000005555, __LINE__);

    str   = "0.0000000000000000000000005555e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.0000000000000000000000005555e-10, __LINE__);

    str   = "0.0000000000000000000000005555e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.0000000000000000000000005555e+10, __LINE__);

    str   = "0.0000000000000000000000005555e+50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.0000000000000000000000005555e+50, __LINE__);

    str   = "0.0000000000000000000000005555e-50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.0000000000000000000000005555e-50, __LINE__);

    str = "0.0000000000000000000000005555   ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 0.0000000000000000000000005555, __LINE__);

    str = "0.0000000000000000000000005555e-10  ";
    QNumber64         d_num;
    const QNumberType type = Digit::StringToNumber(d_num, str, StringUtils::Count(str));
    test.IsTrue((type == QNumberType::Real), __LINE__);
    test.IsEqual(d_num.Real, 0.0000000000000000000000005555e-10, __LINE__);

    str = "0.0000000000000000000000005555e+10   ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 0.0000000000000000000000005555e+10, __LINE__);

    str = "0.0000000000000000000000005555e+50   ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 0.0000000000000000000000005555e+50, __LINE__);

    str = "0.0000000000000000000000005555e-50   ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 0.0000000000000000000000005555e-50, __LINE__);

    str   = "0.00000005";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.00000005, __LINE__);

    str   = "0.00000005e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.00000005e-10, __LINE__);

    str   = "0.00000005e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.00000005e+10, __LINE__);

    str   = "0.00000005e+50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.00000005e+50, __LINE__);

    str   = "0.00000005e-50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.00000005e-50, __LINE__);

    str = "0.00000005 ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 0.00000005, __LINE__);

    str = "0.00000005e-10 ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 0.00000005e-10, __LINE__);

    str = "0.00000005e+10 ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 0.00000005e+10, __LINE__);

    str = "0.00000005e+50 ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 0.00000005e50, __LINE__);

    str = "0.00000005e-50 ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 0.00000005e-50, __LINE__);

    str   = "50000000000000000.0";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000.0, __LINE__);

    str   = "50000000000000000.0e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000.0e-10, __LINE__);

    str   = "50000000000000000.0e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000.0e+10, __LINE__);

    str   = "50000000000000000.0e+50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000.0e+50, __LINE__);

    str = "50000000000000000.0  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50000000000000000.0, __LINE__);

    str = "50000000000000000.0e-10  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50000000000000000.0e-10, __LINE__);

    str = "50000000000000000.0e+10  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50000000000000000.0e+10, __LINE__);

    str = "50000000000000000.0e+50  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50000000000000000.0e+50, __LINE__);

    str = "50000000000000000.0e+50  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50000000000000000.0e+50, __LINE__);

    str   = "5000000000.0";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5000000000.0, __LINE__);

    str   = "5000000000.0e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5000000000.0e-10, __LINE__);

    str   = "5000000000.0e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5000000000.0e+10, __LINE__);

    str   = "5000000000.0e-50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5000000000.0e-50, __LINE__);

    str   = "5000000000.0e+50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5000000000.0e+50, __LINE__);

    str = "5000000000.0  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 5000000000.0, __LINE__);

    str = "5000000000.0e-10  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 5000000000.0e-10, __LINE__);

    str = "5000000000.0e+10  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 5000000000.0e+10, __LINE__);

    str = "5000000000.0e-50  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 5000000000.0e-50, __LINE__);

    str = "5000000000.0e+50  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 5000000000.0e+50, __LINE__);

    str   = "50000000.0";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000.0, __LINE__);

    str   = "50000000.0e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000.0e-10, __LINE__);

    str   = "50000000.0e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000.0e+10, __LINE__);

    str   = "50000000.0e+50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000.0e+50, __LINE__);

    str   = "50000000.0e-50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000.0e-50, __LINE__);

    str = "50000000.0  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50000000.0, __LINE__);

    str = "50000000.0e-10  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50000000.0e-10, __LINE__);

    str = "50000000.0e+10  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50000000.0e+10, __LINE__);

    str = "50000000.0e+50  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50000000.0e+50, __LINE__);

    str = "50000000.0e-50  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50000000.0e-50, __LINE__);

    str   = "50.0";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50.0, __LINE__);

    str   = "50.0e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50.0e-10, __LINE__);

    str   = "50.0e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50.0e+10, __LINE__);

    str   = "50.0e+50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50.0e+50, __LINE__);

    str   = "50.0e-50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50.0e-50, __LINE__);

    str = "50.0  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50.0, __LINE__);

    str = "50.0e-10  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50.0e-10, __LINE__);

    str = "50.0e+10  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50.0e+10, __LINE__);

    str = "50.0e+50  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50.0e+50, __LINE__);

    str = "50.0e-50  ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 50.0e-50, __LINE__);

    str   = "5.0000000000000000000000000000000";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5.0000000000000000000000000000000, __LINE__);

    str   = "5.0000000000000000000000000000000E10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5.0000000000000000000000000000000E10, __LINE__);

    str   = "5.0000000000000000000000000000000E-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5.0000000000000000000000000000000E-10, __LINE__);

    str   = "5.0000000000000000000000000000000E-50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5.0000000000000000000000000000000E-50, __LINE__);

    str   = "5.0000000000000000000000000000000E+50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5.0000000000000000000000000000000E+50, __LINE__);

    str = "5.0000000000000000000000000000000 ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 5.0000000000000000000000000000000, __LINE__);

    str = "5.0000000000000000000000000000000E10 ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 5.0000000000000000000000000000000E10, __LINE__);

    str = "5.0000000000000000000000000000000E-10 ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 5.0000000000000000000000000000000E-10, __LINE__);

    str = "5.0000000000000000000000000000000E-50 ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 5.0000000000000000000000000000000E-50, __LINE__);

    str = "5.0000000000000000000000000000000E+50 ";
    StringToNumber(test, d_number, str);
    test.IsEqual(d_number, 5.0000000000000000000000000000000E+50, __LINE__);

    str   = "500000000000000.00000000000000000";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 500000000000000.00000000000000000, __LINE__);

    str   = "500000000000000.00000000000000000e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 500000000000000.00000000000000000e+10, __LINE__);

    str   = "500000000000000.00000000000000000e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 500000000000000.00000000000000000e-10, __LINE__);

    str   = "500000000000000.00000000000000000e-50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 500000000000000.00000000000000000e-50, __LINE__);

    str   = "500000000000000.00000000000000000e+50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 500000000000000.00000000000000000e+50, __LINE__);

    str   = "50000000000000000000000000000000.0";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000000000000000000.0, __LINE__);

    str   = "50000000000000000000000000000000.0e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000000000000000000.0e+10, __LINE__);

    str   = "50000000000000000000000000000000.0e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000000000000000000.0e-10, __LINE__);

    str   = "50000000000000000000000000000000.0e-50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000000000000000000.0e-50, __LINE__);

    str   = "50000000000000000000000000000000.0e+50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000000000000000000.0e+50, __LINE__);

    str   = "50000000000000000000000000.000000";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000000000000.000000, __LINE__);

    str   = "50000000000000000000000000.000000e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000000000000.000000e+10, __LINE__);

    str   = "50000000000000000000000000.000000e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000000000000.000000e-10, __LINE__);

    str   = "50000000000000000000000000.000000e-50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000000000000.000000e-50, __LINE__);

    str   = "50000000000000000000000000.000000e+50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000000000000.000000e+50, __LINE__);

    str   = "5000000000000000000000000000000.0";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5000000000000000000000000000000.0, __LINE__);

    str   = "5000000000000000000000000000000.0E10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5000000000000000000000000000000.0E10, __LINE__);

    str   = "5000000000000000000000000000000.0E-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5000000000000000000000000000000.0E-10, __LINE__);

    str   = "5000000000000000000000000000000.0E-60";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5000000000000000000000000000000.0E-60, __LINE__);

    str   = "5000000000000000000000000000000.0E+60";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5000000000000000000000000000000.0E+60, __LINE__);

    str   = "50000000000000000.5";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000000.5, __LINE__);

    str   = "5.0000000050000000000000000000000";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5.0000000050000000000000000000000, __LINE__);

    str   = "500000000.90000000000000000";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 500000000.90000000000000000, __LINE__);

    str   = "50000000000000.90000000000000000";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 50000000000000.90000000000000000, __LINE__);

    str   = "5000000000000000000000000000000000.0";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5000000000000000000000000000000000.0, __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1000000000000000000000000000000000000000000000000.0, __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number,
                 1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000,
                 __LINE__);

    str   = "1000000000000000000000000000000000000000000000000e10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1000000000000000000000000000000000000000000000000e10, __LINE__);

    str   = "1000000000000000000000000000000000000000000000000e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1000000000000000000000000000000000000000000000000e+10, __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1000000000000000000000000000000000000000000000000.0e+10, __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0e10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1000000000000000000000000000000000000000000000000.0e10, __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number,
                 1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000e+10,
                 __LINE__);

    str   = "1000000000000000000000000000000000000000000000000e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1000000000000000000000000000000000000000000000000e-10, __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1000000000000000000000000000000000000000000000000.0e-10, __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number,
                 1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000e-10,
                 __LINE__);

    str   = "1000000000000000000000000000000000000000000000000e-100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1000000000000000000000000000000000000000000000000e-100, __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0e-100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1000000000000000000000000000000000000000000000000.0e-100, __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000e-100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(
        d_number,
        1000000000000000000000000000000000000000000000000.000000000000000000000000000000000000000000000000e-100,
        __LINE__);

    str   = "1000000000000000000000000000000000000000000000000e100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1000000000000000000000000000000000000000000000000e100, __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0e100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1000000000000000000000000000000000000000000000000.0e100, __LINE__);

    str   = "1000000000000000000000000000000000000000000000000e+100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1000000000000000000000000000000000000000000000000e+100, __LINE__);

    str   = "1000000000000000000000000000000000000000000000000.0e+100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1000000000000000000000000000000000000000000000000.0e+100, __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001e+10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.0000000000000000000000000000000000000000000000001e+10, __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001e10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.0000000000000000000000000000000000000000000000001e10, __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001e+100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.0000000000000000000000000000000000000000000000001e+100, __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001e100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.0000000000000000000000000000000000000000000000001e100, __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.0000000000000000000000000000000000000000000000001e-10, __LINE__);

    str   = "0.0000000000000000000000000000000000000000000000001e-100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.0000000000000000000000000000000000000000000000001e-100, __LINE__);
}

static void TestStringToNumber7(QTest &test) {
    const char *str      = nullptr;
    double      d_number = 0;
    bool        valid;

    str   = "1234567891234567891.5";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1234567891234567891.5, __LINE__);

    str   = "12345678912345678912.5";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 12345678912345678912.5, __LINE__);

    str   = "5e-1";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 5e-1, __LINE__);

    str   = "10e-1";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 10e-1, __LINE__);

    str   = "15e-1";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 15e-1, __LINE__);

    str   = "1.0e-1";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1.0e-1, __LINE__);

    str   = "1.5e-1";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1.5e-1, __LINE__);

    str   = "6.25";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 6.25, __LINE__);

    str   = "62.5";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 62.5, __LINE__);

    str   = "0.625";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.625, __LINE__);

    str   = "0.58963";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 0.58963, __LINE__);

    str   = "10.58963";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 10.58963, __LINE__);

    str   = "1e-10";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-10, __LINE__);

    str   = "1e-50";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-50, __LINE__);

    str   = "1e-100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-100, __LINE__);

    str   = "1e-200";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-200, __LINE__);

    str   = "1e-250";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-250, __LINE__);

    str   = "1e-300";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-300, __LINE__);

    str   = "1e-305";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-305, __LINE__);

    str   = "1e-307";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-307, __LINE__);

    str   = "1e-310";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-310, __LINE__);

    str   = "1e-323";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e-323, __LINE__);

    str   = "10e100";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 10e100, __LINE__);

    str   = "1e1";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1e1, __LINE__);

    str   = "155e-3";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 155e-3, __LINE__);

    str   = "72057594037927932.0";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 72057594037927932.0, __LINE__);

    str   = "1.78005908680576090884592783737e-307";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 1.78005908680576090884592783737e-307, __LINE__);

    str   = "8.90029543402880553236093086933e-308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 8.90029543402880553236093086933e-308, __LINE__);

    str   = "2.22507385850720138309023271733e-308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 2.22507385850720138309023271733e-308, __LINE__);

    str   = "2.22507385850720088902458687609e-308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 2.22507385850720088902458687609e-308, __LINE__);

    str   = "4.94065645841246544176568792868e-324";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 4.94065645841246544176568792868e-324, __LINE__);

    str   = "4.45014771701440227211481959342e-308";
    valid = StringToNumber(test, d_number, str);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(d_number, 4.45014771701440227211481959342e-308, __LINE__);
}

static void TestHexStringToNumber1(QTest &test) {
    SizeT64     number;
    const char *hex = "";

    number = HexStringToNumber(hex);
    test.IsEqual(number, 0ULL, __LINE__);

    hex    = "0000";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 0ULL, __LINE__);

    hex    = "AAAA";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 43690ULL, __LINE__);

    hex    = "000A";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 10ULL, __LINE__);

    hex    = "A000";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 40960ULL, __LINE__);

    hex    = "ABCDEF";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 11259375ULL, __LINE__);

    hex    = "FEDCBA";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 16702650ULL, __LINE__);

    hex    = "abcdef";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 11259375ULL, __LINE__);

    hex    = "fedcba";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 16702650ULL, __LINE__);

    hex    = "12345";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 74565ULL, __LINE__);

    hex    = "6789";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 26505ULL, __LINE__);

    hex    = "FFFF";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 65535ULL, __LINE__);

    hex    = "9999";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 39321ULL, __LINE__);

    hex    = "FFFFFFFFFFFFFFFF";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 18446744073709551615ULL, __LINE__);

    hex    = "8888888881111111";
    number = HexStringToNumber(hex);
    test.IsEqual(number, 9838263505853157649ULL, __LINE__);
}

static void TestHexStringToNumber2(QTest &test) {
    SizeT64     number;
    const char *hex = "";
    bool        valid;

    valid = StringToNumber(test, number, hex);
    test.IsFalse(valid, __LINE__);
    test.IsEqual(number, 0ULL, __LINE__);

    hex   = "0x0000";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 0ULL, __LINE__);

    hex   = "0xAAAA";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 43690ULL, __LINE__);

    hex   = "0x000A";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 10ULL, __LINE__);

    hex   = "0XA000";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 40960ULL, __LINE__);

    hex   = "0xABCDEF";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 11259375ULL, __LINE__);

    hex   = "0xFEDCBA";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 16702650ULL, __LINE__);

    hex   = "0xabcdef";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 11259375ULL, __LINE__);

    hex   = "0xfedcba";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 16702650ULL, __LINE__);

    hex   = "0X12345";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 74565ULL, __LINE__);

    hex   = "0x6789";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 26505ULL, __LINE__);

    hex   = "0xFFFF";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 65535ULL, __LINE__);

    hex   = "0X9999";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 39321ULL, __LINE__);

    hex   = "0xFFFFFFFFFFFFFFFF";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 18446744073709551615ULL, __LINE__);

    hex   = "0x8888888881111111";
    valid = StringToNumber(test, number, hex);
    test.IsTrue(valid, __LINE__);
    test.IsEqual(number, 9838263505853157649ULL, __LINE__);
}

static void TestIntToString1(QTest &test, StringStream<char> &stream) {
    IntToStreamEqual(test, stream, char{100}, "100", __LINE__);
    IntToStreamEqual(test, stream, SizeT8{255}, "255", __LINE__);
    IntToStreamEqual(test, stream, SizeT8I(-100), "-100", __LINE__);

    IntToStreamEqual(test, stream, SizeT16{65535}, "65535", __LINE__);
    IntToStreamEqual(test, stream, short{-32767}, "-32767", __LINE__);

    IntToStreamEqual(test, stream, -1, "-1", __LINE__);
    IntToStreamEqual(test, stream, 1, "1", __LINE__);
    IntToStreamEqual(test, stream, -7, "-7", __LINE__);

    IntToStreamEqual(test, stream, 0U, "0", __LINE__);
    IntToStreamEqual(test, stream, 10U, "10", __LINE__);

    IntToStreamEqual(test, stream, -100000L, "-100000", __LINE__);
    IntToStreamEqual(test, stream, 100000L, "100000", __LINE__);

    IntToStreamEqual(test, stream, -9223372036854775807LL, "-9223372036854775807", __LINE__);

    IntToStreamEqual(test, stream, 1ULL, "1", __LINE__);
    IntToStreamEqual(test, stream, 5000000000000000ULL, "5000000000000000", __LINE__);
    IntToStreamEqual(test, stream, 4ULL, "4", __LINE__);
    IntToStreamEqual(test, stream, 3ULL, "3", __LINE__);
    IntToStreamEqual(test, stream, 9223372036854775807LL, "9223372036854775807", __LINE__);
    IntToStreamEqual(test, stream, 18446744073709551615ULL, "18446744073709551615", __LINE__);
}

static void TestDoubleToString1(QTest &test, StringStream<char> &stream) {
    RealToStreamEqual(test, stream, -0.0, 6U, "-0", __LINE__);
    RealToStreamEqual(test, stream, 0.0, 6U, "0", __LINE__);

    RealToStreamEqual(test, stream, QNumber64{9218868437227405312ULL}.Real,
                      Digit::RealFormatInfo{6U, Digit::RealFormatType::Default}, "inf", __LINE__);
    RealToStreamEqual(test, stream, QNumber64{18442240474082181120ULL}.Real, Digit::RealFormatType::Default, "-inf",
                      __LINE__);

#ifndef QENTEM_COMPARE_DIGIT_WITH_STL
    RealToStreamEqual(test, stream, QNumber64{18444492273895866368ULL}.Real, 6U, "nan", __LINE__);
#endif

    Digit::RealFormatInfo format{};
    format = Digit::RealFormatType::Default;
    format = 6U;

    RealToStreamEqual(test, stream, 1.0, format, "1", __LINE__);
    RealToStreamEqual(test, stream, 2.0, 6U, "2", __LINE__);
    RealToStreamEqual(test, stream, 3.0, 6U, "3", __LINE__);
    RealToStreamEqual(test, stream, 4.0, 6U, "4", __LINE__);
    RealToStreamEqual(test, stream, 5.0, 6U, "5", __LINE__);
    RealToStreamEqual(test, stream, 6.0, 6U, "6", __LINE__);
    RealToStreamEqual(test, stream, 7.0, 6U, "7", __LINE__);
    RealToStreamEqual(test, stream, 8.0, 6U, "8", __LINE__);
    RealToStreamEqual(test, stream, 9.0, 6U, "9", __LINE__);
    RealToStreamEqual(test, stream, 220.0, 6U, "220", __LINE__);
    RealToStreamEqual(test, stream, 0.1, 6U, "0.1", __LINE__);
    RealToStreamEqual(test, stream, 0.2, 6U, "0.2", __LINE__);
    RealToStreamEqual(test, stream, 0.3, 6U, "0.3", __LINE__);
    RealToStreamEqual(test, stream, 0.4, 6U, "0.4", __LINE__);
    RealToStreamEqual(test, stream, 0.5, 6U, "0.5", __LINE__);
    RealToStreamEqual(test, stream, 0.6, 6U, "0.6", __LINE__);
    RealToStreamEqual(test, stream, 0.7, 6U, "0.7", __LINE__);
    RealToStreamEqual(test, stream, 0.8, 6U, "0.8", __LINE__);
    RealToStreamEqual(test, stream, 0.9, 6U, "0.9", __LINE__);

    RealToStreamEqual(test, stream, 0.09, 6U, "0.09", __LINE__);
    RealToStreamEqual(test, stream, 0.009, 6U, "0.009", __LINE__);
    RealToStreamEqual(test, stream, 0.0009, 6U, "0.0009", __LINE__);
    RealToStreamEqual(test, stream, 0.00009, 6U, "9e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.000009, 6U, "9e-06", __LINE__);
    RealToStreamEqual(test, stream, 0.0000009, 6U, "9e-07", __LINE__);
    RealToStreamEqual(test, stream, 0.00000009, 6U, "9e-08", __LINE__);
    RealToStreamEqual(test, stream, 0.000000009, 6U, "9e-09", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000009, 6U, "9e-10", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000009, 6U, "9e-11", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000009, 6U, "9e-12", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000009, 6U, "9e-13", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000000009, 6U, "9e-14", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000000009, 6U, "9e-15", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000000009, 6U, "9e-16", __LINE__);

    RealToStreamEqual(test, stream, 0.01, 6U, "0.01", __LINE__);
    RealToStreamEqual(test, stream, 0.001, 6U, "0.001", __LINE__);
    RealToStreamEqual(test, stream, 0.0001, 6U, "0.0001", __LINE__);
    RealToStreamEqual(test, stream, 0.00001, 6U, "1e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.000001, 6U, "1e-06", __LINE__);
    RealToStreamEqual(test, stream, 0.0000001, 6U, "1e-07", __LINE__);
    RealToStreamEqual(test, stream, 0.00000001, 6U, "1e-08", __LINE__);
    RealToStreamEqual(test, stream, 0.000000001, 6U, "1e-09", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000001, 6U, "1e-10", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000001, 6U, "1e-11", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000001, 6U, "1e-12", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000001, 6U, "1e-13", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000000001, 6U, "1e-14", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000000001, 6U, "1e-15", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000000001, 6U, "1e-16", __LINE__);

    RealToStreamEqual(test, stream, 0.02, 6U, "0.02", __LINE__);
    RealToStreamEqual(test, stream, 0.002, 6U, "0.002", __LINE__);
    RealToStreamEqual(test, stream, 0.0002, 6U, "0.0002", __LINE__);
    RealToStreamEqual(test, stream, 0.00002, 6U, "2e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.000002, 6U, "2e-06", __LINE__);
    RealToStreamEqual(test, stream, 0.0000002, 6U, "2e-07", __LINE__);
    RealToStreamEqual(test, stream, 0.00000002, 6U, "2e-08", __LINE__);
    RealToStreamEqual(test, stream, 0.000000002, 6U, "2e-09", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000002, 6U, "2e-10", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000002, 6U, "2e-11", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000002, 6U, "2e-12", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000002, 6U, "2e-13", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000000002, 6U, "2e-14", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000000002, 6U, "2e-15", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000000002, 6U, "2e-16", __LINE__);

    RealToStreamEqual(test, stream, 1.5, 6U, "1.5", __LINE__);
    RealToStreamEqual(test, stream, 1.05, 6U, "1.05", __LINE__);
    RealToStreamEqual(test, stream, 1.005, 6U, "1.005", __LINE__);
    RealToStreamEqual(test, stream, 1.0005, 6U, "1.0005", __LINE__);
    RealToStreamEqual(test, stream, 1.00005, 6U, "1.00005", __LINE__);
    RealToStreamEqual(test, stream, 1.000005, 6U, "1.00001", __LINE__);
    RealToStreamEqual(test, stream, 1.0000005, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000005, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000005, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000005, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000005, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000005, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000005, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000000005, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000000005, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000000005, 6U, "1", __LINE__);

    RealToStreamEqual(test, stream, 1.9, 6U, "1.9", __LINE__);
    RealToStreamEqual(test, stream, 1.09, 6U, "1.09", __LINE__);
    RealToStreamEqual(test, stream, 1.009, 6U, "1.009", __LINE__);
    RealToStreamEqual(test, stream, 1.0009, 6U, "1.0009", __LINE__);
    RealToStreamEqual(test, stream, 1.00009, 6U, "1.00009", __LINE__);
    RealToStreamEqual(test, stream, 1.000009, 6U, "1.00001", __LINE__);
    RealToStreamEqual(test, stream, 1.0000009, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000009, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000009, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000009, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000009, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000009, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000009, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000000009, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000000009, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000000009, 6U, "1", __LINE__);

    RealToStreamEqual(test, stream, 1.1, 6U, "1.1", __LINE__);
    RealToStreamEqual(test, stream, 1.01, 6U, "1.01", __LINE__);
    RealToStreamEqual(test, stream, 1.001, 6U, "1.001", __LINE__);
    RealToStreamEqual(test, stream, 1.0001, 6U, "1.0001", __LINE__);
    RealToStreamEqual(test, stream, 1.00001, 6U, "1.00001", __LINE__);
    RealToStreamEqual(test, stream, 1.000001, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000001, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000001, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000001, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000001, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000001, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000001, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000001, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000000001, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000000001, 6U, "1", __LINE__);

    RealToStreamEqual(test, stream, 0.1000000000000006, 6U, "0.1", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000000011, 6U, "1.1e-14", __LINE__);

    RealToStreamEqual(test, stream, 0.02, 6U, "0.02", __LINE__);
    RealToStreamEqual(test, stream, 0.002, 6U, "0.002", __LINE__);
    RealToStreamEqual(test, stream, 0.0002, 6U, "0.0002", __LINE__);

    RealToStreamEqual(test, stream, 123456789.0, 6U, "1.23457e+08", __LINE__);
    RealToStreamEqual(test, stream, 2147483647.0, 6U, "2.14748e+09", __LINE__);
    RealToStreamEqual(test, stream, 4294967295.0, 6U, "4.29497e+09", __LINE__);
    RealToStreamEqual(test, stream, 4294967295.625, 6U, "4.29497e+09", __LINE__);
    RealToStreamEqual(test, stream, 2251799813685247.0, 6U, "2.2518e+15", __LINE__);
    RealToStreamEqual(test, stream, 4503599627370495.0, 6U, "4.5036e+15", __LINE__);
    RealToStreamEqual(test, stream, 4611686018427387903.0, 6U, "4.61169e+18", __LINE__);
    RealToStreamEqual(test, stream, 9223372036854775806.0, 6U, "9.22337e+18", __LINE__);
    RealToStreamEqual(test, stream, 9223372036854775807.0, 6U, "9.22337e+18", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551615.0, 6U, "1.84467e+19", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551616.0, 6U, "1.84467e+19", __LINE__);
    RealToStreamEqual(test, stream, 34028236692093846341.0, 6U, "3.40282e+19", __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463412.0, 6U, "3.40282e+20", __LINE__);
    RealToStreamEqual(test, stream, 3402823669209384634633746074317.0, 6U, "3.40282e+30", __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463426481119284341.0, 6U, "3.40282e+32", __LINE__);
    RealToStreamEqual(test, stream, 3402823669209384634264811192843414.0, 6U, "3.40282e+33", __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463426481119284349108225.0, 6U, "3.40282e+38", __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463463374607431768211455.0, 6U, "3.40282e+38", __LINE__);
    RealToStreamEqual(test, stream, 3689348814741910333.0, 6U, "3.68935e+18", __LINE__);
    RealToStreamEqual(test, stream, 36893488147419103230.0, 6U, "3.68935e+19", __LINE__);
    RealToStreamEqual(test, stream, 36893488147419103333.0, 6U, "3.68935e+19", __LINE__);
    RealToStreamEqual(test, stream, 368934881474191033333.0, 6U, "3.68935e+20", __LINE__);
    RealToStreamEqual(test, stream, 9007199254740992.0, 6U, "9.0072e+15", __LINE__);
    RealToStreamEqual(test, stream, 18014398509481984.0, 6U, "1.80144e+16", __LINE__);
    RealToStreamEqual(test, stream, 36028797018963968.0, 6U, "3.60288e+16", __LINE__);
}

static void TestDoubleToString2(QTest &test, StringStream<char> &stream) {
    RealToStreamEqual(test, stream, 0.9000000000000006, 6U, "0.9", __LINE__);

    RealToStreamEqual(test, stream, 9.999999999999999, 6U, "10", __LINE__);
    RealToStreamEqual(test, stream, 1999999.0, 6U, "2e+06", __LINE__);
    RealToStreamEqual(test, stream, 9999999999999999.0, 6U, "1e+16", __LINE__);
    RealToStreamEqual(test, stream, 999999999999999999.0, 6U, "1e+18", __LINE__);
    RealToStreamEqual(test, stream, 9999999999999999999.0, 6U, "1e+19", __LINE__);
    RealToStreamEqual(test, stream, 99999999999999999999.0, 6U, "1e+20", __LINE__);
    RealToStreamEqual(test, stream, 9999999999999999999999999999999999999999.0, 6U, "1e+40", __LINE__);
    RealToStreamEqual(test, stream, 99e100, 6U, "9.9e+101", __LINE__);

    RealToStreamEqual(test, stream, 599.9999, 6U, "600", __LINE__);
    RealToStreamEqual(test, stream, 6999.999, 6U, "7000", __LINE__);
    RealToStreamEqual(test, stream, 79999.999, 6U, "80000", __LINE__);
    RealToStreamEqual(test, stream, 899999.999, 6U, "900000", __LINE__);

    RealToStreamEqual(test, stream, 0.999, 6U, "0.999", __LINE__);
    RealToStreamEqual(test, stream, 0.999999, 6U, "0.999999", __LINE__);
    RealToStreamEqual(test, stream, 0.999999999, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.999999999999999, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999999, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999999, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999995, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999959, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999996, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999999999959, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999999999999, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999999999999999999999999999, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999999999999999999999999999999999999, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999999e-99, 6U, "1e-99", __LINE__);

    RealToStreamEqual(test, stream, 10000005.0, 6U, "1e+07", __LINE__);
    RealToStreamEqual(test, stream, 10000005.0, 7U, "1e+07", __LINE__);
    RealToStreamEqual(test, stream, 10000006.0, 7U, "1.000001e+07", __LINE__);

    RealToStreamEqual(test, stream, 10000000.0, 6U, "1e+07", __LINE__);
    RealToStreamEqual(test, stream, 9999999.0, 6U, "1e+07", __LINE__);
    RealToStreamEqual(test, stream, 19999999.0, 6U, "2e+07", __LINE__);
    RealToStreamEqual(test, stream, 29999999.0, 6U, "3e+07", __LINE__);
    RealToStreamEqual(test, stream, 39999999.0, 6U, "4e+07", __LINE__);
    RealToStreamEqual(test, stream, 49999999.0, 6U, "5e+07", __LINE__);
    RealToStreamEqual(test, stream, 59999999.0, 6U, "6e+07", __LINE__);
    RealToStreamEqual(test, stream, 69999999.0, 6U, "7e+07", __LINE__);
    RealToStreamEqual(test, stream, 79999999.0, 6U, "8e+07", __LINE__);
    RealToStreamEqual(test, stream, 89999999.0, 6U, "9e+07", __LINE__);
    RealToStreamEqual(test, stream, 99999999.0, 6U, "1e+08", __LINE__);
    RealToStreamEqual(test, stream, 999999999.0, 6U, "1e+09", __LINE__);

    RealToStreamEqual(test, stream, 0.10000000, 6U, "0.1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.19999999, 6U, "0.2", __LINE__);
    RealToStreamEqual(test, stream, 0.29999999, 6U, "0.3", __LINE__);
    RealToStreamEqual(test, stream, 0.39999999, 6U, "0.4", __LINE__);
    RealToStreamEqual(test, stream, 0.49999999, 6U, "0.5", __LINE__);
    RealToStreamEqual(test, stream, 0.59999999, 6U, "0.6", __LINE__);
    RealToStreamEqual(test, stream, 0.69999999, 6U, "0.7", __LINE__);
    RealToStreamEqual(test, stream, 0.79999999, 6U, "0.8", __LINE__);
    RealToStreamEqual(test, stream, 0.89999999, 6U, "0.9", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.99999999, 6U, "2", __LINE__);
    RealToStreamEqual(test, stream, 2.99999999, 6U, "3", __LINE__);
    RealToStreamEqual(test, stream, 3.99999999, 6U, "4", __LINE__);
    RealToStreamEqual(test, stream, 4.99999999, 6U, "5", __LINE__);
    RealToStreamEqual(test, stream, 5.99999999, 6U, "6", __LINE__);
    RealToStreamEqual(test, stream, 6.99999999, 6U, "7", __LINE__);
    RealToStreamEqual(test, stream, 7.99999999, 6U, "8", __LINE__);
    RealToStreamEqual(test, stream, 8.99999999, 6U, "9", __LINE__);
    RealToStreamEqual(test, stream, 9.99999999, 6U, "10", __LINE__);
    RealToStreamEqual(test, stream, 99.99999999, 6U, "100", __LINE__);
    RealToStreamEqual(test, stream, 999.99999999, 6U, "1000", __LINE__);
    RealToStreamEqual(test, stream, 99999.89999999, 6U, "99999.9", __LINE__);
    RealToStreamEqual(test, stream, 99999.99999999, 6U, "100000", __LINE__);
    RealToStreamEqual(test, stream, 999999.99999999, 6U, "1e+06", __LINE__);
    RealToStreamEqual(test, stream, 9e-50, 6U, "9e-50", __LINE__);

    RealToStreamEqual(test, stream, 1e1, 6U, "10", __LINE__);
    RealToStreamEqual(test, stream, 1e2, 6U, "100", __LINE__);
    RealToStreamEqual(test, stream, 1e3, 6U, "1000", __LINE__);
    RealToStreamEqual(test, stream, 1e4, 6U, "10000", __LINE__);
    RealToStreamEqual(test, stream, 1e5, 6U, "100000", __LINE__);
    RealToStreamEqual(test, stream, 1e10, 6U, "1e+10", __LINE__);
    RealToStreamEqual(test, stream, 1e13, 6U, "1e+13", __LINE__);
    RealToStreamEqual(test, stream, 1e14, 6U, "1e+14", __LINE__);
    RealToStreamEqual(test, stream, 1e15, 6U, "1e+15", __LINE__);

    RealToStreamEqual(test, stream, 1e-1, 6U, "0.1", __LINE__);
    RealToStreamEqual(test, stream, 1e-2, 6U, "0.01", __LINE__);
    RealToStreamEqual(test, stream, 1e-3, 6U, "0.001", __LINE__);
    RealToStreamEqual(test, stream, 1e-4, 6U, "0.0001", __LINE__);
    RealToStreamEqual(test, stream, 1e-5, 6U, "1e-05", __LINE__);
    RealToStreamEqual(test, stream, 1e-10, 6U, "1e-10", __LINE__);
    RealToStreamEqual(test, stream, 1e-13, 6U, "1e-13", __LINE__);
    RealToStreamEqual(test, stream, 1e-14, 6U, "1e-14", __LINE__);
    RealToStreamEqual(test, stream, 1e-15, 6U, "1e-15", __LINE__);

    RealToStreamEqual(test, stream, 1e-50, 6U, "1e-50", __LINE__);
    RealToStreamEqual(test, stream, 1e+50, 6U, "1e+50", __LINE__);
    RealToStreamEqual(test, stream, 1e-100, 6U, "1e-100", __LINE__);
    RealToStreamEqual(test, stream, 1e+100, 6U, "1e+100", __LINE__);
    RealToStreamEqual(test, stream, 1e-200, 6U, "1e-200", __LINE__);
    RealToStreamEqual(test, stream, 1e+200, 6U, "1e+200", __LINE__);
    RealToStreamEqual(test, stream, 1e-300, 6U, "1e-300", __LINE__);
    RealToStreamEqual(test, stream, 1e+300, 6U, "1e+300", __LINE__);
    RealToStreamEqual(test, stream, 1e-307, 6U, "1e-307", __LINE__);
    RealToStreamEqual(test, stream, 1e-308, 6U, "1e-308", __LINE__);
    RealToStreamEqual(test, stream, 1e+308, 6U, "1e+308", __LINE__);
    RealToStreamEqual(test, stream, 1e-309, 6U, "1e-309", __LINE__);
    RealToStreamEqual(test, stream, 1e-310, 6U, "1e-310", __LINE__);
    RealToStreamEqual(test, stream, 1e-311, 6U, "1e-311", __LINE__);
    RealToStreamEqual(test, stream, 1.25e-50, 6U, "1.25e-50", __LINE__);
    RealToStreamEqual(test, stream, 1.25e+50, 6U, "1.25e+50", __LINE__);
    RealToStreamEqual(test, stream, 1.125e-50, 6U, "1.125e-50", __LINE__);
    RealToStreamEqual(test, stream, 1.125e+50, 6U, "1.125e+50", __LINE__);

    RealToStreamEqual(test, stream, 4.9406564584124654e-324, 6U, "4.94066e-324", __LINE__);
    RealToStreamEqual(test, stream, 1.7976931348623157e+308, 6U, "1.79769e+308", __LINE__);
    RealToStreamEqual(test, stream, -4.9406564584124654e-324, 6U, "-4.94066e-324", __LINE__);
    RealToStreamEqual(test, stream, -1.7976931348623157e+308, 6U, "-1.79769e+308", __LINE__);

    RealToStreamEqual(test, stream, -854534.5, 6U, "-854534", __LINE__);
    RealToStreamEqual(test, stream, -854534.05, 6U, "-854534", __LINE__);
    RealToStreamEqual(test, stream, 854534.5, 6U, "854534", __LINE__);
    RealToStreamEqual(test, stream, 854534.05, 6U, "854534", __LINE__);
    RealToStreamEqual(test, stream, -0.2765625, 6U, "-0.276562", __LINE__);

    RealToStreamEqual(test, stream, 9765625.0, 6U, "9.76562e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765645.0, 6U, "9.76564e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765665.0, 6U, "9.76566e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765685.0, 6U, "9.76568e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765605.0, 6U, "9.7656e+06", __LINE__);

    RealToStreamEqual(test, stream, 9765615.0, 6U, "9.76562e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765635.0, 6U, "9.76564e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765655.0, 6U, "9.76566e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765675.0, 6U, "9.76568e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765695.0, 6U, "9.7657e+06", __LINE__);

    RealToStreamEqual(test, stream, 9999425.0, 6U, "9.99942e+06", __LINE__);
    RealToStreamEqual(test, stream, 9999445.0, 6U, "9.99944e+06", __LINE__);
    RealToStreamEqual(test, stream, 9999465.0, 6U, "9.99946e+06", __LINE__);
    RealToStreamEqual(test, stream, 9999485.0, 6U, "9.99948e+06", __LINE__);
    RealToStreamEqual(test, stream, 9999405.0, 6U, "9.9994e+06", __LINE__);

    RealToStreamEqual(test, stream, 9.999925, 6U, "9.99992", __LINE__);
    RealToStreamEqual(test, stream, 9.999945, 6U, "9.99995", __LINE__);
    RealToStreamEqual(test, stream, 9.999965, 6U, "9.99996", __LINE__);
    RealToStreamEqual(test, stream, 9.999985, 6U, "9.99999", __LINE__);
    RealToStreamEqual(test, stream, 9.999905, 6U, "9.99991", __LINE__);

    RealToStreamEqual(test, stream, 1.765625, 6U, "1.76562", __LINE__);
    RealToStreamEqual(test, stream, 0.1765625, 6U, "0.176563", __LINE__);
    RealToStreamEqual(test, stream, 0.2765625, 6U, "0.276562", __LINE__);
    RealToStreamEqual(test, stream, 0.3765625, 6U, "0.376563", __LINE__);
    RealToStreamEqual(test, stream, 0.4765625, 6U, "0.476562", __LINE__);
    RealToStreamEqual(test, stream, 0.5765625, 6U, "0.576562", __LINE__);
    RealToStreamEqual(test, stream, 0.6765625, 6U, "0.676562", __LINE__);
    RealToStreamEqual(test, stream, 0.7765625, 6U, "0.776563", __LINE__);
    RealToStreamEqual(test, stream, 0.8765625, 6U, "0.876563", __LINE__);
    RealToStreamEqual(test, stream, 0.9765625, 6U, "0.976562", __LINE__);
}

static void TestDoubleToString3(QTest &test, StringStream<char> &stream) {
    RealToStreamEqual(test, stream, 2.165625, 6U, "2.16562", __LINE__);
    RealToStreamEqual(test, stream, 2.265625, 6U, "2.26562", __LINE__);
    RealToStreamEqual(test, stream, 2.365625, 6U, "2.36563", __LINE__);
    RealToStreamEqual(test, stream, 2.465625, 6U, "2.46563", __LINE__);
    RealToStreamEqual(test, stream, 2.565625, 6U, "2.56562", __LINE__);
    RealToStreamEqual(test, stream, 2.665625, 6U, "2.66562", __LINE__);
    RealToStreamEqual(test, stream, 2.765625, 6U, "2.76562", __LINE__);
    RealToStreamEqual(test, stream, 2.865625, 6U, "2.86563", __LINE__);
    RealToStreamEqual(test, stream, 2.965625, 6U, "2.96563", __LINE__);

    RealToStreamEqual(test, stream, 5.165625, 6U, "5.16563", __LINE__);
    RealToStreamEqual(test, stream, 5.265625, 6U, "5.26562", __LINE__);
    RealToStreamEqual(test, stream, 5.365625, 6U, "5.36562", __LINE__);
    RealToStreamEqual(test, stream, 5.465625, 6U, "5.46563", __LINE__);
    RealToStreamEqual(test, stream, 5.565625, 6U, "5.56562", __LINE__);
    RealToStreamEqual(test, stream, 5.665625, 6U, "5.66563", __LINE__);
    RealToStreamEqual(test, stream, 5.765625, 6U, "5.76562", __LINE__);
    RealToStreamEqual(test, stream, 5.865625, 6U, "5.86562", __LINE__);
    RealToStreamEqual(test, stream, 5.965625, 6U, "5.96563", __LINE__);

    RealToStreamEqual(test, stream, 9.765615, 6U, "9.76562", __LINE__);
    RealToStreamEqual(test, stream, 9.765625, 6U, "9.76562", __LINE__);
    RealToStreamEqual(test, stream, 9.765635, 6U, "9.76563", __LINE__);
    RealToStreamEqual(test, stream, 9.765645, 6U, "9.76564", __LINE__);
    RealToStreamEqual(test, stream, 9.765655, 6U, "9.76566", __LINE__);
    RealToStreamEqual(test, stream, 9.765665, 6U, "9.76567", __LINE__);
    RealToStreamEqual(test, stream, 9.765675, 6U, "9.76567", __LINE__);
    RealToStreamEqual(test, stream, 9.765685, 6U, "9.76568", __LINE__);
    RealToStreamEqual(test, stream, 9.765695, 6U, "9.76569", __LINE__);
    RealToStreamEqual(test, stream, 0.9765615, 6U, "0.976561", __LINE__);
    RealToStreamEqual(test, stream, 0.9765625, 6U, "0.976562", __LINE__);
    RealToStreamEqual(test, stream, 0.9765635, 6U, "0.976564", __LINE__);
    RealToStreamEqual(test, stream, 0.9765645, 6U, "0.976564", __LINE__);
    RealToStreamEqual(test, stream, 0.9765655, 6U, "0.976565", __LINE__);
    RealToStreamEqual(test, stream, 0.9765665, 6U, "0.976567", __LINE__);
    RealToStreamEqual(test, stream, 0.9765675, 6U, "0.976568", __LINE__);
    RealToStreamEqual(test, stream, 0.9765685, 6U, "0.976568", __LINE__);
    RealToStreamEqual(test, stream, 0.9765695, 6U, "0.976569", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-1, 6U, "0.976562", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-2, 6U, "0.0976562", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-3, 6U, "0.00976562", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-4, 6U, "0.000976562", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-5, 6U, "9.76563e-05", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-10, 6U, "9.76562e-10", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-13, 6U, "9.76563e-13", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-14, 6U, "9.76563e-14", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-15, 6U, "9.76562e-15", __LINE__);

    RealToStreamEqual(test, stream, -9765625.0, 6U, "-9.76562e+06", __LINE__);
    RealToStreamEqual(test, stream, -9765645.0, 6U, "-9.76564e+06", __LINE__);
    RealToStreamEqual(test, stream, -9765665.0, 6U, "-9.76566e+06", __LINE__);
    RealToStreamEqual(test, stream, -9765685.0, 6U, "-9.76568e+06", __LINE__);
    RealToStreamEqual(test, stream, -9765605.0, 6U, "-9.7656e+06", __LINE__);

    RealToStreamEqual(test, stream, -9765615.0, 6U, "-9.76562e+06", __LINE__);
    RealToStreamEqual(test, stream, -9765635.0, 6U, "-9.76564e+06", __LINE__);
    RealToStreamEqual(test, stream, -9765655.0, 6U, "-9.76566e+06", __LINE__);
    RealToStreamEqual(test, stream, -9765675.0, 6U, "-9.76568e+06", __LINE__);
    RealToStreamEqual(test, stream, -9765695.0, 6U, "-9.7657e+06", __LINE__);

    RealToStreamEqual(test, stream, -9999425.0, 6U, "-9.99942e+06", __LINE__);
    RealToStreamEqual(test, stream, -9999445.0, 6U, "-9.99944e+06", __LINE__);
    RealToStreamEqual(test, stream, -9999465.0, 6U, "-9.99946e+06", __LINE__);
    RealToStreamEqual(test, stream, -9999485.0, 6U, "-9.99948e+06", __LINE__);
    RealToStreamEqual(test, stream, -9999405.0, 6U, "-9.9994e+06", __LINE__);

    RealToStreamEqual(test, stream, -9.999925, 6U, "-9.99992", __LINE__);
    RealToStreamEqual(test, stream, -9.999945, 6U, "-9.99995", __LINE__);
    RealToStreamEqual(test, stream, -9.999965, 6U, "-9.99996", __LINE__);
    RealToStreamEqual(test, stream, -9.999985, 6U, "-9.99999", __LINE__);
    RealToStreamEqual(test, stream, -9.999905, 6U, "-9.99991", __LINE__);

    RealToStreamEqual(test, stream, -1.765625, 6U, "-1.76562", __LINE__);
    RealToStreamEqual(test, stream, -0.1765625, 6U, "-0.176563", __LINE__);
    RealToStreamEqual(test, stream, -0.2765625, 6U, "-0.276562", __LINE__);
    RealToStreamEqual(test, stream, -0.3765625, 6U, "-0.376563", __LINE__);
    RealToStreamEqual(test, stream, -0.4765625, 6U, "-0.476562", __LINE__);
    RealToStreamEqual(test, stream, -0.5765625, 6U, "-0.576562", __LINE__);
    RealToStreamEqual(test, stream, -0.6765625, 6U, "-0.676562", __LINE__);
    RealToStreamEqual(test, stream, -0.7765625, 6U, "-0.776563", __LINE__);
    RealToStreamEqual(test, stream, -0.8765625, 6U, "-0.876563", __LINE__);
    RealToStreamEqual(test, stream, -0.9765625, 6U, "-0.976562", __LINE__);

    RealToStreamEqual(test, stream, -2.165625, 6U, "-2.16562", __LINE__);
    RealToStreamEqual(test, stream, -2.265625, 6U, "-2.26562", __LINE__);
    RealToStreamEqual(test, stream, -2.365625, 6U, "-2.36563", __LINE__);
    RealToStreamEqual(test, stream, -2.465625, 6U, "-2.46563", __LINE__);
    RealToStreamEqual(test, stream, -2.565625, 6U, "-2.56562", __LINE__);
    RealToStreamEqual(test, stream, -2.665625, 6U, "-2.66562", __LINE__);
    RealToStreamEqual(test, stream, -2.765625, 6U, "-2.76562", __LINE__);
    RealToStreamEqual(test, stream, -2.865625, 6U, "-2.86563", __LINE__);
    RealToStreamEqual(test, stream, -2.965625, 6U, "-2.96563", __LINE__);

    RealToStreamEqual(test, stream, -5.165625, 6U, "-5.16563", __LINE__);
    RealToStreamEqual(test, stream, -5.265625, 6U, "-5.26562", __LINE__);
    RealToStreamEqual(test, stream, -5.365625, 6U, "-5.36562", __LINE__);
    RealToStreamEqual(test, stream, -5.465625, 6U, "-5.46563", __LINE__);
    RealToStreamEqual(test, stream, -5.565625, 6U, "-5.56562", __LINE__);
    RealToStreamEqual(test, stream, -5.665625, 6U, "-5.66563", __LINE__);
    RealToStreamEqual(test, stream, -5.765625, 6U, "-5.76562", __LINE__);
    RealToStreamEqual(test, stream, -5.865625, 6U, "-5.86562", __LINE__);
    RealToStreamEqual(test, stream, -5.965625, 6U, "-5.96563", __LINE__);

    RealToStreamEqual(test, stream, -9.765615, 6U, "-9.76562", __LINE__);
    RealToStreamEqual(test, stream, -9.765625, 6U, "-9.76562", __LINE__);
    RealToStreamEqual(test, stream, -9.765635, 6U, "-9.76563", __LINE__);
    RealToStreamEqual(test, stream, -9.765645, 6U, "-9.76564", __LINE__);
    RealToStreamEqual(test, stream, -9.765655, 6U, "-9.76566", __LINE__);
    RealToStreamEqual(test, stream, -9.765665, 6U, "-9.76567", __LINE__);
    RealToStreamEqual(test, stream, -9.765675, 6U, "-9.76567", __LINE__);
    RealToStreamEqual(test, stream, -9.765685, 6U, "-9.76568", __LINE__);
    RealToStreamEqual(test, stream, -9.765695, 6U, "-9.76569", __LINE__);
    RealToStreamEqual(test, stream, -0.9765615, 6U, "-0.976561", __LINE__);
    RealToStreamEqual(test, stream, -0.9765625, 6U, "-0.976562", __LINE__);
    RealToStreamEqual(test, stream, -0.9765635, 6U, "-0.976564", __LINE__);
    RealToStreamEqual(test, stream, -0.9765645, 6U, "-0.976564", __LINE__);
    RealToStreamEqual(test, stream, -0.9765655, 6U, "-0.976565", __LINE__);
    RealToStreamEqual(test, stream, -0.9765665, 6U, "-0.976567", __LINE__);
    RealToStreamEqual(test, stream, -0.9765675, 6U, "-0.976568", __LINE__);
    RealToStreamEqual(test, stream, -0.9765685, 6U, "-0.976568", __LINE__);
    RealToStreamEqual(test, stream, -0.9765695, 6U, "-0.976569", __LINE__);
    RealToStreamEqual(test, stream, -9.765625e-1, 6U, "-0.976562", __LINE__);
    RealToStreamEqual(test, stream, -9.765625e-2, 6U, "-0.0976562", __LINE__);
    RealToStreamEqual(test, stream, -9.765625e-3, 6U, "-0.00976562", __LINE__);
    RealToStreamEqual(test, stream, -9.765625e-4, 6U, "-0.000976562", __LINE__);
    RealToStreamEqual(test, stream, -9.765625e-5, 6U, "-9.76563e-05", __LINE__);
    RealToStreamEqual(test, stream, -9.765625e-10, 6U, "-9.76562e-10", __LINE__);
    RealToStreamEqual(test, stream, -9.765625e-13, 6U, "-9.76563e-13", __LINE__);
    RealToStreamEqual(test, stream, -9.765625e-14, 6U, "-9.76563e-14", __LINE__);
    RealToStreamEqual(test, stream, -9.765625e-15, 6U, "-9.76562e-15", __LINE__);

    RealToStreamEqual(test, stream, 0.00001844674407370955161500, 6U, "1.84467e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.0001844674407370955161500, 6U, "0.000184467", __LINE__);
    RealToStreamEqual(test, stream, 0.001844674407370955161500, 6U, "0.00184467", __LINE__);
    RealToStreamEqual(test, stream, 5.001844674407370955161500, 6U, "5.00184", __LINE__);
    RealToStreamEqual(test, stream, 0.01844674407370955161500, 6U, "0.0184467", __LINE__);
    RealToStreamEqual(test, stream, 0.1844674407370955161500, 6U, "0.184467", __LINE__);

    RealToStreamEqual(test, stream, 18446744073709551614.0, 6U, "1.84467e+19", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551620.0, 6U, "1.84467e+19", __LINE__);
    RealToStreamEqual(test, stream, 184467440737095516150.0, 6U, "1.84467e+20", __LINE__);
    RealToStreamEqual(test, stream, 1844674407370955161500.0, 6U, "1.84467e+21", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551615e99, 6U, "1.84467e+118", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551615e100, 6U, "1.84467e+119", __LINE__);
    RealToStreamEqual(test, stream, 17446744073709551614.0, 6U, "1.74467e+19", __LINE__);
    RealToStreamEqual(test, stream, 9923372036854775809.0, 6U, "9.92337e+18", __LINE__);

    RealToStreamEqual(test, stream, 4.76837158203125e-50, 6U, "4.76837e-50", __LINE__);
    RealToStreamEqual(test, stream, 2.384185791015625e-50, 6U, "2.38419e-50", __LINE__);

    RealToStreamEqual(test, stream, 0.05, 6U, "0.05", __LINE__);
    RealToStreamEqual(test, stream, 0.005, 6U, "0.005", __LINE__);
    RealToStreamEqual(test, stream, 0.0005, 6U, "0.0005", __LINE__);
    RealToStreamEqual(test, stream, 0.00005, 6U, "5e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.000005, 6U, "5e-06", __LINE__);
    RealToStreamEqual(test, stream, 1.55, 6U, "1.55", __LINE__);
    RealToStreamEqual(test, stream, 1.0055, 6U, "1.0055", __LINE__);
    RealToStreamEqual(test, stream, 1.005999, 6U, "1.006", __LINE__);
    RealToStreamEqual(test, stream, 1.009999, 6U, "1.01", __LINE__);
}

static void TestDoubleToString4(QTest &test, StringStream<char> &stream) {
    RealToStreamEqual(test, stream, 0.00055, 6U, "0.00055", __LINE__);
    RealToStreamEqual(test, stream, 1.00055, 6U, "1.00055", __LINE__);
    RealToStreamEqual(test, stream, 0.50009, 6U, "0.50009", __LINE__);
    RealToStreamEqual(test, stream, 1.00055e-100, 6U, "1.00055e-100", __LINE__);
    RealToStreamEqual(test, stream, 0.50009e-100, 6U, "5.0009e-101", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000050000000005, 6U, "5e-11", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000005, 6U, "5e-13", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000000000000000000005, 6U, "5e-28", __LINE__);
    RealToStreamEqual(test, stream, 5.3125, 6U, "5.3125", __LINE__);
    RealToStreamEqual(test, stream, 5.3, 6U, "5.3", __LINE__);
    RealToStreamEqual(test, stream, 5.5, 6U, "5.5", __LINE__);
    RealToStreamEqual(test, stream, 0.55, 6U, "0.55", __LINE__);
    RealToStreamEqual(test, stream, 0.25, 6U, "0.25", __LINE__);
    RealToStreamEqual(test, stream, 0.75, 6U, "0.75", __LINE__);
    RealToStreamEqual(test, stream, 0.125, 6U, "0.125", __LINE__);
    RealToStreamEqual(test, stream, 0.625, 6U, "0.625", __LINE__);
    RealToStreamEqual(test, stream, 0.3125, 6U, "0.3125", __LINE__);
    RealToStreamEqual(test, stream, 0.95367431640625, 6U, "0.953674", __LINE__);
    RealToStreamEqual(test, stream, 0.298023223876953125, 6U, "0.298023", __LINE__);
    RealToStreamEqual(test, stream, 0.1490116119384765625, 6U, "0.149012", __LINE__);
    RealToStreamEqual(test, stream, 0.7450580596923828125, 6U, "0.745058", __LINE__);
    RealToStreamEqual(test, stream, 0.37252902984619140625, 6U, "0.372529", __LINE__);
    RealToStreamEqual(test, stream, 0.186264514923095703125, 6U, "0.186265", __LINE__);

    RealToStreamEqual(test, stream, 1.12345, 6U, "1.12345", __LINE__);
    RealToStreamEqual(test, stream, 1.953674, 6U, "1.95367", __LINE__);
    RealToStreamEqual(test, stream, 1.0953674, 6U, "1.09537", __LINE__);
    RealToStreamEqual(test, stream, 1.00953674, 6U, "1.00954", __LINE__);
    RealToStreamEqual(test, stream, 1.000953674, 6U, "1.00095", __LINE__);
    RealToStreamEqual(test, stream, 9.953674, 6U, "9.95367", __LINE__);
    RealToStreamEqual(test, stream, 9.0953674, 6U, "9.09537", __LINE__);
    RealToStreamEqual(test, stream, 9.00953674, 6U, "9.00954", __LINE__);
    RealToStreamEqual(test, stream, 9.000953674, 6U, "9.00095", __LINE__);

    RealToStreamEqual(test, stream, 0.953674, 6U, "0.953674", __LINE__);
    RealToStreamEqual(test, stream, 0.0953674, 6U, "0.0953674", __LINE__);
    RealToStreamEqual(test, stream, 0.00953674, 6U, "0.00953674", __LINE__);
    RealToStreamEqual(test, stream, 0.000953674, 6U, "0.000953674", __LINE__);
    RealToStreamEqual(test, stream, 0.123455, 6U, "0.123455", __LINE__);
    RealToStreamEqual(test, stream, 0.1123405, 6U, "0.11234", __LINE__);
    RealToStreamEqual(test, stream, 0.1123095, 6U, "0.11231", __LINE__);
    RealToStreamEqual(test, stream, 0.78126, 6U, "0.78126", __LINE__);
    RealToStreamEqual(test, stream, 0.300048828125, 6U, "0.300049", __LINE__);

    RealToStreamEqual(test, stream, 0.111, 6U, "0.111", __LINE__);
    RealToStreamEqual(test, stream, 0.11111, 6U, "0.11111", __LINE__);
    RealToStreamEqual(test, stream, 0.1111111111111111, 6U, "0.111111", __LINE__);
    RealToStreamEqual(test, stream, 0.1111111111111115, 6U, "0.111111", __LINE__);
    RealToStreamEqual(test, stream, 0.11111111111111159, 6U, "0.111111", __LINE__);
    RealToStreamEqual(test, stream, 0.1111111111111116, 6U, "0.111111", __LINE__);

    RealToStreamEqual(test, stream, 105.625, 6U, "105.625", __LINE__);
    RealToStreamEqual(test, stream, 25e20, 6U, "2.5e+21", __LINE__);
    RealToStreamEqual(test, stream, 2.5e21, 6U, "2.5e+21", __LINE__);
    RealToStreamEqual(test, stream, 3814697265625.0, 6U, "3.8147e+12", __LINE__);
    RealToStreamEqual(test, stream, 381469725.0, 6U, "3.8147e+08", __LINE__);
    RealToStreamEqual(test, stream, 0.3814697265625, 6U, "0.38147", __LINE__);
    RealToStreamEqual(test, stream, 10.5, 6U, "10.5", __LINE__);
    RealToStreamEqual(test, stream, 10.0, 6U, "10", __LINE__);

    RealToStreamEqual(test, stream, 2097151.125, 6U, "2.09715e+06", __LINE__);
    RealToStreamEqual(test, stream, 2097151.0, 6U, "2.09715e+06", __LINE__);
    RealToStreamEqual(test, stream, 16217975.0, 6U, "1.6218e+07", __LINE__);
    RealToStreamEqual(test, stream, 32995191.0, 6U, "3.29952e+07", __LINE__);
    RealToStreamEqual(test, stream, 444444444444444444444.0, 6U, "4.44444e+20", __LINE__);
    RealToStreamEqual(test, stream, 555555555555555555555.0, 6U, "5.55556e+20", __LINE__);
    RealToStreamEqual(test, stream, 3668262915145728.0, 6U, "3.66826e+15", __LINE__);
    RealToStreamEqual(test, stream, 7336525830291456.0, 6U, "7.33653e+15", __LINE__);
    RealToStreamEqual(test, stream, 7336535830291456.0, 6U, "7.33654e+15", __LINE__);
    RealToStreamEqual(test, stream, 14673051660582912.0, 6U, "1.46731e+16", __LINE__);

    RealToStreamEqual(test, stream, 1000000.390625, 6U, "1e+06", __LINE__);
    RealToStreamEqual(test, stream, 1000009.590625, 6U, "1.00001e+06", __LINE__);
    RealToStreamEqual(test, stream, 1000008.590625, 6U, "1.00001e+06", __LINE__);
    RealToStreamEqual(test, stream, 7.3365258e-10, 6U, "7.33653e-10", __LINE__);
    RealToStreamEqual(test, stream, 7.3365258e-10, 8U, "7.3365258e-10", __LINE__);
    RealToStreamEqual(test, stream, 0.3333, 6U, "0.3333", __LINE__);

    RealToStreamEqual(test, stream, 4554534.0, 6U, "4.55453e+06", __LINE__);
    RealToStreamEqual(test, stream, 854534.055, 6U, "854534", __LINE__);
    RealToStreamEqual(test, stream, 854534.0055, 6U, "854534", __LINE__);
    RealToStreamEqual(test, stream, 854534.000055, 6U, "854534", __LINE__);
    RealToStreamEqual(test, stream, 854534.0000005, 14U, "854534.0000005", __LINE__);
    RealToStreamEqual(test, stream, 8.0000005, 6U, "8", __LINE__);
    RealToStreamEqual(test, stream, 8.000005, 6U, "8", __LINE__);
    RealToStreamEqual(test, stream, 8.005, 6U, "8.005", __LINE__);

    RealToStreamEqual(test, stream, 5.55, 1U, "6", __LINE__);
    RealToStreamEqual(test, stream, 9.996, 5U, "9.996", __LINE__);
    RealToStreamEqual(test, stream, 9.996, 4U, "9.996", __LINE__);
    RealToStreamEqual(test, stream, 9.996, 3U, "10", __LINE__);
    RealToStreamEqual(test, stream, 9.996, 2U, "10", __LINE__);
    RealToStreamEqual(test, stream, 9.996, 1U, "1e+01", __LINE__);

    RealToStreamEqual(test, stream, 0.5, 1U, "0.5", __LINE__);
    RealToStreamEqual(test, stream, 1.5, 1U, "2", __LINE__);
    RealToStreamEqual(test, stream, 2.5, 1U, "2", __LINE__);
    RealToStreamEqual(test, stream, 3.5, 1U, "4", __LINE__);
    RealToStreamEqual(test, stream, 4.5, 1U, "4", __LINE__);
    RealToStreamEqual(test, stream, 2.25, 1U, "2", __LINE__);
    RealToStreamEqual(test, stream, 2.75, 1U, "3", __LINE__);

    RealToStreamEqual(test, stream, 2.625, 2U, "2.6", __LINE__);
    RealToStreamEqual(test, stream, 2.65, 2U, "2.6", __LINE__);
    RealToStreamEqual(test, stream, 2.25, 2U, "2.2", __LINE__);
    RealToStreamEqual(test, stream, 3.25, 2U, "3.2", __LINE__);
    RealToStreamEqual(test, stream, 2.625, 2U, "2.6", __LINE__);

    RealToStreamEqual(test, stream, 2.635, 2U, "2.6", __LINE__);
    RealToStreamEqual(test, stream, 2.35, 2U, "2.4", __LINE__);
    RealToStreamEqual(test, stream, 3.35, 2U, "3.4", __LINE__);
    RealToStreamEqual(test, stream, 2.635, 2U, "2.6", __LINE__);

    RealToStreamEqual(test, stream, 91.0, 1U, "9e+01", __LINE__);
    RealToStreamEqual(test, stream, 95.0, 1U, "1e+02", __LINE__);

    RealToStreamEqual(test, stream, 95000000.0, 1U, "1e+08", __LINE__);

    RealToStreamEqual(test, stream, 3.0, 15U, "3", __LINE__);
    RealToStreamEqual(test, stream, 3.14, 15U, "3.14", __LINE__);

    RealToStreamEqual(test, stream, 140737488355327.953674, 15U, "140737488355328", __LINE__);
    RealToStreamEqual(test, stream, 140737488355327.015625, 15U, "140737488355327", __LINE__);

    RealToStreamEqual(test, stream, 140737458355327.953674, 15U, "140737458355328", __LINE__);
    RealToStreamEqual(test, stream, 140737488355327.953674, 6U, "1.40737e+14", __LINE__);

    RealToStreamEqual(test, stream, 0.95367431640625, 20U, "0.95367431640625", __LINE__);
    RealToStreamEqual(test, stream, 0.298023223876953125, 20U, "0.298023223876953125", __LINE__);
    RealToStreamEqual(test, stream, 0.1490116119384765625, 20U, "0.1490116119384765625", __LINE__);
    RealToStreamEqual(test, stream, 0.7450580596923828125, 20U, "0.7450580596923828125", __LINE__);
    RealToStreamEqual(test, stream, 0.37252902984619140625, 20U, "0.37252902984619140625", __LINE__);
    RealToStreamEqual(test, stream, 0.186264514923095703125, 20U, "0.18626451492309570312", __LINE__);
    RealToStreamEqual(test, stream, 0.186264514923095703125, 21U, "0.186264514923095703125", __LINE__);

    RealToStreamEqual(test, stream, 18446744073709551614.0, 20U, "18446744073709551616", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551620.0, 20U, "18446744073709551616", __LINE__);
    RealToStreamEqual(test, stream, 184467440737095516150.0, 20U, "1.8446744073709551616e+20", __LINE__);
    RealToStreamEqual(test, stream, 1844674407370955161500.0, 20U, "1.8446744073709551616e+21", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551615e99, 20U, "1.8446744073709551013e+118", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551615e100, 20U, "1.8446744073709551909e+119", __LINE__);
    RealToStreamEqual(test, stream, 17446744073709551614.0, 21U, "17446744073709551616", __LINE__);
    RealToStreamEqual(test, stream, 17446744073709551614.0, 20U, "17446744073709551616", __LINE__);
    RealToStreamEqual(test, stream, 9923372036854775809.0, 21U, "9923372036854775808", __LINE__);
    RealToStreamEqual(test, stream, 9923372036854775809.0, 20U, "9923372036854775808", __LINE__);

    RealToStreamEqual(test, stream, 2251799813685247.0, 21U, "2251799813685247", __LINE__);
    RealToStreamEqual(test, stream, 4503599627370495.0, 21U, "4503599627370495", __LINE__);
    RealToStreamEqual(test, stream, 4611686018427387903.0, 21U, "4611686018427387904", __LINE__);
    RealToStreamEqual(test, stream, 9223372036854775806.0, 21U, "9223372036854775808", __LINE__);
    RealToStreamEqual(test, stream, 9223372036854775807.0, 21U, "9223372036854775808", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551615.0, 21U, "18446744073709551616", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551616.0, 21U, "18446744073709551616", __LINE__);
    RealToStreamEqual(test, stream, 34028236692093846341.0, 21U, "34028236692093845504", __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463412.0, 21U, "340282366920938487808", __LINE__);
    RealToStreamEqual(test, stream, 3402823669209384634633746074317.0, 34U, "3402823669209384705829531287552",
                      __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463426481119284341.0, 34U, "340282366920938448064954991902720",
                      __LINE__);
    RealToStreamEqual(test, stream, 3402823669209384634264811192843414.0, 34U, "3402823669209384912995114146594816",
                      __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463426481119284349108225.0, 39U,
                      "340282366920938463463374607431768211456", __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463463374607431768211455.0, 39U,
                      "340282366920938463463374607431768211456", __LINE__);
}

static void TestDoubleToString5(QTest &test, StringStream<char> &stream) {
    RealToStreamEqual(test, stream, -0.0, 15U, "-0", __LINE__);
    RealToStreamEqual(test, stream, 0.0, 15U, "0", __LINE__);

    RealToStreamEqual(test, stream, 1.0, 15U, "1", __LINE__);
    RealToStreamEqual(test, stream, 2.0, 15U, "2", __LINE__);
    RealToStreamEqual(test, stream, 3.0, 15U, "3", __LINE__);
    RealToStreamEqual(test, stream, 4.0, 15U, "4", __LINE__);
    RealToStreamEqual(test, stream, 5.0, 15U, "5", __LINE__);
    RealToStreamEqual(test, stream, 6.0, 15U, "6", __LINE__);
    RealToStreamEqual(test, stream, 7.0, 15U, "7", __LINE__);
    RealToStreamEqual(test, stream, 8.0, 15U, "8", __LINE__);
    RealToStreamEqual(test, stream, 9.0, 15U, "9", __LINE__);
    RealToStreamEqual(test, stream, 220.0, 15U, "220", __LINE__);
    RealToStreamEqual(test, stream, 0.1, 15U, "0.1", __LINE__);
    RealToStreamEqual(test, stream, 0.2, 15U, "0.2", __LINE__);
    RealToStreamEqual(test, stream, 0.3, 15U, "0.3", __LINE__);
    RealToStreamEqual(test, stream, 0.4, 15U, "0.4", __LINE__);
    RealToStreamEqual(test, stream, 0.5, 15U, "0.5", __LINE__);
    RealToStreamEqual(test, stream, 0.6, 15U, "0.6", __LINE__);
    RealToStreamEqual(test, stream, 0.7, 15U, "0.7", __LINE__);
    RealToStreamEqual(test, stream, 0.8, 15U, "0.8", __LINE__);
    RealToStreamEqual(test, stream, 0.9, 15U, "0.9", __LINE__);

    RealToStreamEqual(test, stream, 0.09, 15U, "0.09", __LINE__);
    RealToStreamEqual(test, stream, 0.009, 15U, "0.009", __LINE__);
    RealToStreamEqual(test, stream, 0.0009, 15U, "0.0009", __LINE__);
    RealToStreamEqual(test, stream, 0.00009, 15U, "9e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.000009, 15U, "9e-06", __LINE__);
    RealToStreamEqual(test, stream, 0.0000009, 15U, "9e-07", __LINE__);
    RealToStreamEqual(test, stream, 0.00000009, 15U, "9e-08", __LINE__);
    RealToStreamEqual(test, stream, 0.000000009, 15U, "9e-09", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000009, 15U, "9e-10", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000009, 15U, "9e-11", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000009, 15U, "9e-12", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000009, 15U, "9e-13", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000000009, 15U, "9e-14", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000000009, 15U, "9e-15", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000000009, 15U, "9e-16", __LINE__);

    RealToStreamEqual(test, stream, 0.01, 15U, "0.01", __LINE__);
    RealToStreamEqual(test, stream, 0.001, 15U, "0.001", __LINE__);
    RealToStreamEqual(test, stream, 0.0001, 15U, "0.0001", __LINE__);
    RealToStreamEqual(test, stream, 0.00001, 15U, "1e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.000001, 15U, "1e-06", __LINE__);
    RealToStreamEqual(test, stream, 0.0000001, 15U, "1e-07", __LINE__);
    RealToStreamEqual(test, stream, 0.00000001, 15U, "1e-08", __LINE__);
    RealToStreamEqual(test, stream, 0.000000001, 15U, "1e-09", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000001, 15U, "1e-10", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000001, 15U, "1e-11", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000001, 15U, "1e-12", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000001, 15U, "1e-13", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000000001, 15U, "1e-14", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000000001, 15U, "1e-15", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000000001, 15U, "1e-16", __LINE__);

    RealToStreamEqual(test, stream, 0.02, 15U, "0.02", __LINE__);
    RealToStreamEqual(test, stream, 0.002, 15U, "0.002", __LINE__);
    RealToStreamEqual(test, stream, 0.0002, 15U, "0.0002", __LINE__);
    RealToStreamEqual(test, stream, 0.00002, 15U, "2e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.000002, 15U, "2e-06", __LINE__);
    RealToStreamEqual(test, stream, 0.0000002, 15U, "2e-07", __LINE__);
    RealToStreamEqual(test, stream, 0.00000002, 15U, "2e-08", __LINE__);
    RealToStreamEqual(test, stream, 0.000000002, 15U, "2e-09", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000002, 15U, "2e-10", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000002, 15U, "2e-11", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000002, 15U, "2e-12", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000002, 15U, "2e-13", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000000002, 15U, "2e-14", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000000002, 15U, "2e-15", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000000002, 15U, "2e-16", __LINE__);

    RealToStreamEqual(test, stream, 1.5, 15U, "1.5", __LINE__);
    RealToStreamEqual(test, stream, 1.05, 15U, "1.05", __LINE__);
    RealToStreamEqual(test, stream, 1.005, 15U, "1.005", __LINE__);
    RealToStreamEqual(test, stream, 1.0005, 15U, "1.0005", __LINE__);
    RealToStreamEqual(test, stream, 1.00005, 15U, "1.00005", __LINE__);
    RealToStreamEqual(test, stream, 1.000005, 15U, "1.000005", __LINE__);
    RealToStreamEqual(test, stream, 1.0000005, 15U, "1.0000005", __LINE__);
    RealToStreamEqual(test, stream, 1.00000005, 15U, "1.00000005", __LINE__);
    RealToStreamEqual(test, stream, 1.000000005, 15U, "1.000000005", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000005, 15U, "1.0000000005", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000005, 15U, "1.00000000005", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000005, 15U, "1.000000000005", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000005, 15U, "1.0000000000005", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000000005, 15U, "1.00000000000005", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000000005, 15U, "1.00000000000001", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000000005, 15U, "1", __LINE__);

    RealToStreamEqual(test, stream, 1.9, 15U, "1.9", __LINE__);
    RealToStreamEqual(test, stream, 1.09, 15U, "1.09", __LINE__);
    RealToStreamEqual(test, stream, 1.009, 15U, "1.009", __LINE__);
    RealToStreamEqual(test, stream, 1.0009, 15U, "1.0009", __LINE__);
    RealToStreamEqual(test, stream, 1.00009, 15U, "1.00009", __LINE__);
    RealToStreamEqual(test, stream, 1.000009, 15U, "1.000009", __LINE__);
    RealToStreamEqual(test, stream, 1.0000009, 15U, "1.0000009", __LINE__);
    RealToStreamEqual(test, stream, 1.00000009, 15U, "1.00000009", __LINE__);
    RealToStreamEqual(test, stream, 1.000000009, 15U, "1.000000009", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000009, 15U, "1.0000000009", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000009, 15U, "1.00000000009", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000009, 15U, "1.000000000009", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000009, 15U, "1.0000000000009", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000000009, 15U, "1.00000000000009", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000000009, 15U, "1.00000000000001", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000000009, 15U, "1", __LINE__);

    RealToStreamEqual(test, stream, 1.1, 15U, "1.1", __LINE__);
    RealToStreamEqual(test, stream, 1.01, 15U, "1.01", __LINE__);
    RealToStreamEqual(test, stream, 1.001, 15U, "1.001", __LINE__);
    RealToStreamEqual(test, stream, 1.0001, 15U, "1.0001", __LINE__);
    RealToStreamEqual(test, stream, 1.00001, 15U, "1.00001", __LINE__);
    RealToStreamEqual(test, stream, 1.000001, 15U, "1.000001", __LINE__);
    RealToStreamEqual(test, stream, 1.0000001, 15U, "1.0000001", __LINE__);
    RealToStreamEqual(test, stream, 1.00000001, 15U, "1.00000001", __LINE__);
    RealToStreamEqual(test, stream, 1.000000001, 15U, "1.000000001", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000001, 15U, "1.0000000001", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000001, 15U, "1.00000000001", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000001, 15U, "1.000000000001", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000001, 15U, "1.0000000000001", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000000001, 15U, "1.00000000000001", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000000001, 15U, "1", __LINE__);

    RealToStreamEqual(test, stream, 0.1000000000000006, 15U, "0.100000000000001", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000000011, 15U, "1.1e-14", __LINE__);

    RealToStreamEqual(test, stream, 0.02, 15U, "0.02", __LINE__);
    RealToStreamEqual(test, stream, 0.002, 15U, "0.002", __LINE__);
    RealToStreamEqual(test, stream, 0.0002, 15U, "0.0002", __LINE__);

    RealToStreamEqual(test, stream, 123456789.0, 15U, "123456789", __LINE__);
    RealToStreamEqual(test, stream, 2147483647.0, 15U, "2147483647", __LINE__);
    RealToStreamEqual(test, stream, 4294967295.0, 15U, "4294967295", __LINE__);
    RealToStreamEqual(test, stream, 4294967295.625, 15U, "4294967295.625", __LINE__);
    RealToStreamEqual(test, stream, 2251799813685247.0, 15U, "2.25179981368525e+15", __LINE__);
    RealToStreamEqual(test, stream, 4503599627370495.0, 15U, "4.5035996273705e+15", __LINE__);
    RealToStreamEqual(test, stream, 4611686018427387903.0, 15U, "4.61168601842739e+18", __LINE__);
    RealToStreamEqual(test, stream, 9223372036854775806.0, 15U, "9.22337203685478e+18", __LINE__);
    RealToStreamEqual(test, stream, 9223372036854775807.0, 15U, "9.22337203685478e+18", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551615.0, 15U, "1.84467440737096e+19", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551616.0, 15U, "1.84467440737096e+19", __LINE__);
    RealToStreamEqual(test, stream, 34028236692093846341.0, 15U, "3.40282366920938e+19", __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463412.0, 15U, "3.40282366920938e+20", __LINE__);
    RealToStreamEqual(test, stream, 3402823669209384634633746074317.0, 15U, "3.40282366920938e+30", __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463426481119284341.0, 15U, "3.40282366920938e+32", __LINE__);
    RealToStreamEqual(test, stream, 3402823669209384634264811192843414.0, 15U, "3.40282366920938e+33", __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463426481119284349108225.0, 15U, "3.40282366920938e+38", __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463463374607431768211455.0, 15U, "3.40282366920938e+38", __LINE__);
    RealToStreamEqual(test, stream, 3689348814741910333.0, 15U, "3.68934881474191e+18", __LINE__);
    RealToStreamEqual(test, stream, 36893488147419103230.0, 15U, "3.68934881474191e+19", __LINE__);
    RealToStreamEqual(test, stream, 36893488147419103333.0, 15U, "3.68934881474191e+19", __LINE__);
    RealToStreamEqual(test, stream, 368934881474191033333.0, 15U, "3.68934881474191e+20", __LINE__);
    RealToStreamEqual(test, stream, 9007199254740992.0, 15U, "9.00719925474099e+15", __LINE__);
    RealToStreamEqual(test, stream, 18014398509481984.0, 15U, "1.8014398509482e+16", __LINE__);
    RealToStreamEqual(test, stream, 36028797018963968.0, 15U, "3.6028797018964e+16", __LINE__);

    RealToStreamEqual(test, stream, 0.9000000000000006, 15U, "0.900000000000001", __LINE__);
}

static void TestDoubleToString6(QTest &test, StringStream<char> &stream) {
    RealToStreamEqual(test, stream, 9.999999999999999, 15U, "10", __LINE__);
    RealToStreamEqual(test, stream, 1999999.0, 15U, "1999999", __LINE__);
    RealToStreamEqual(test, stream, 9999999999999999.0, 15U, "1e+16", __LINE__);
    RealToStreamEqual(test, stream, 999999999999999999.0, 15U, "1e+18", __LINE__);
    RealToStreamEqual(test, stream, 9999999999999999999.0, 15U, "1e+19", __LINE__);
    RealToStreamEqual(test, stream, 99999999999999999999.0, 15U, "1e+20", __LINE__);
    RealToStreamEqual(test, stream, 9999999999999999999999999999999999999999.0, 15U, "1e+40", __LINE__);
    RealToStreamEqual(test, stream, 99e100, 15U, "9.9e+101", __LINE__);

    RealToStreamEqual(test, stream, 0.999999999, 15U, "0.999999999", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999999, 15U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.999, 15U, "0.999", __LINE__);
    RealToStreamEqual(test, stream, 0.999999, 15U, "0.999999", __LINE__);
    RealToStreamEqual(test, stream, 0.999999999999999, 15U, "0.999999999999999", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999999, 15U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999995, 15U, "0.999999999999999", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999959, 15U, "0.999999999999996", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999996, 15U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999999999959, 15U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999999999999, 15U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999999999999999999999999999, 15U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999999999999999999999999999999999999, 15U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999999e-99, 15U, "1e-99", __LINE__);

    RealToStreamEqual(test, stream, 10000005.0, 15U, "10000005", __LINE__);

    RealToStreamEqual(test, stream, 10000000.0, 15U, "10000000", __LINE__);
    RealToStreamEqual(test, stream, 9999999.0, 15U, "9999999", __LINE__);
    RealToStreamEqual(test, stream, 19999999.0, 15U, "19999999", __LINE__);
    RealToStreamEqual(test, stream, 29999999.0, 15U, "29999999", __LINE__);
    RealToStreamEqual(test, stream, 39999999.0, 15U, "39999999", __LINE__);
    RealToStreamEqual(test, stream, 49999999.0, 15U, "49999999", __LINE__);
    RealToStreamEqual(test, stream, 59999999.0, 15U, "59999999", __LINE__);
    RealToStreamEqual(test, stream, 69999999.0, 15U, "69999999", __LINE__);
    RealToStreamEqual(test, stream, 79999999.0, 15U, "79999999", __LINE__);
    RealToStreamEqual(test, stream, 89999999.0, 15U, "89999999", __LINE__);
    RealToStreamEqual(test, stream, 99999999.0, 15U, "99999999", __LINE__);
    RealToStreamEqual(test, stream, 999999999.0, 15U, "999999999", __LINE__);

    RealToStreamEqual(test, stream, 0.10000000, 15U, "0.1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999, 15U, "0.9999999", __LINE__);
    RealToStreamEqual(test, stream, 0.19999999, 15U, "0.19999999", __LINE__);
    RealToStreamEqual(test, stream, 0.29999999, 15U, "0.29999999", __LINE__);
    RealToStreamEqual(test, stream, 0.39999999, 15U, "0.39999999", __LINE__);
    RealToStreamEqual(test, stream, 0.49999999, 15U, "0.49999999", __LINE__);
    RealToStreamEqual(test, stream, 0.59999999, 15U, "0.59999999", __LINE__);
    RealToStreamEqual(test, stream, 0.69999999, 15U, "0.69999999", __LINE__);
    RealToStreamEqual(test, stream, 0.79999999, 15U, "0.79999999", __LINE__);
    RealToStreamEqual(test, stream, 0.89999999, 15U, "0.89999999", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999, 15U, "0.99999999", __LINE__);
    RealToStreamEqual(test, stream, 1.99999999, 15U, "1.99999999", __LINE__);
    RealToStreamEqual(test, stream, 2.99999999, 15U, "2.99999999", __LINE__);
    RealToStreamEqual(test, stream, 3.99999999, 15U, "3.99999999", __LINE__);
    RealToStreamEqual(test, stream, 4.99999999, 15U, "4.99999999", __LINE__);
    RealToStreamEqual(test, stream, 5.99999999, 15U, "5.99999999", __LINE__);
    RealToStreamEqual(test, stream, 6.99999999, 15U, "6.99999999", __LINE__);
    RealToStreamEqual(test, stream, 7.99999999, 15U, "7.99999999", __LINE__);
    RealToStreamEqual(test, stream, 8.99999999, 15U, "8.99999999", __LINE__);
    RealToStreamEqual(test, stream, 9.99999999, 15U, "9.99999999", __LINE__);
    RealToStreamEqual(test, stream, 99.99999999, 15U, "99.99999999", __LINE__);
    RealToStreamEqual(test, stream, 999.99999999, 15U, "999.99999999", __LINE__);
    RealToStreamEqual(test, stream, 99999.89999999, 15U, "99999.89999999", __LINE__);
    RealToStreamEqual(test, stream, 99999.99999999, 15U, "99999.99999999", __LINE__);
    RealToStreamEqual(test, stream, 999999.99999999, 15U, "999999.99999999", __LINE__);
    RealToStreamEqual(test, stream, 9e-50, 15U, "9e-50", __LINE__);

    RealToStreamEqual(test, stream, 1e1, 15U, "10", __LINE__);
    RealToStreamEqual(test, stream, 1e2, 15U, "100", __LINE__);
    RealToStreamEqual(test, stream, 1e3, 15U, "1000", __LINE__);
    RealToStreamEqual(test, stream, 1e4, 15U, "10000", __LINE__);
    RealToStreamEqual(test, stream, 1e5, 15U, "100000", __LINE__);
    RealToStreamEqual(test, stream, 1e10, 15U, "10000000000", __LINE__);
    RealToStreamEqual(test, stream, 1e13, 15U, "10000000000000", __LINE__);
    RealToStreamEqual(test, stream, 1e14, 15U, "100000000000000", __LINE__);
    RealToStreamEqual(test, stream, 1e15, 15U, "1e+15", __LINE__);

    RealToStreamEqual(test, stream, 1e-1, 15U, "0.1", __LINE__);
    RealToStreamEqual(test, stream, 1e-2, 15U, "0.01", __LINE__);
    RealToStreamEqual(test, stream, 1e-3, 15U, "0.001", __LINE__);
    RealToStreamEqual(test, stream, 1e-4, 15U, "0.0001", __LINE__);
    RealToStreamEqual(test, stream, 1e-5, 15U, "1e-05", __LINE__);
    RealToStreamEqual(test, stream, 1e-10, 15U, "1e-10", __LINE__);
    RealToStreamEqual(test, stream, 1e-13, 15U, "1e-13", __LINE__);
    RealToStreamEqual(test, stream, 1e-14, 15U, "1e-14", __LINE__);
    RealToStreamEqual(test, stream, 1e-15, 15U, "1e-15", __LINE__);

    RealToStreamEqual(test, stream, 1e-50, 15U, "1e-50", __LINE__);
    RealToStreamEqual(test, stream, 1e+50, 15U, "1e+50", __LINE__);
    RealToStreamEqual(test, stream, 1e-100, 15U, "1e-100", __LINE__);
    RealToStreamEqual(test, stream, 1e+100, 15U, "1e+100", __LINE__);
    RealToStreamEqual(test, stream, 1e-200, 15U, "1e-200", __LINE__);
    RealToStreamEqual(test, stream, 1e+200, 15U, "1e+200", __LINE__);
    RealToStreamEqual(test, stream, 1e-300, 15U, "1e-300", __LINE__);
    RealToStreamEqual(test, stream, 1e+300, 15U, "1e+300", __LINE__);
    RealToStreamEqual(test, stream, 1e-307, 15U, "1e-307", __LINE__);
    RealToStreamEqual(test, stream, 1e-308, 15U, "1e-308", __LINE__);
    RealToStreamEqual(test, stream, 1e+308, 15U, "1e+308", __LINE__);
    RealToStreamEqual(test, stream, 1e-309, 15U, "1e-309", __LINE__);
    RealToStreamEqual(test, stream, 1e-310, 15U, "9.99999999999997e-311", __LINE__);
    RealToStreamEqual(test, stream, 1e-311, 15U, "9.99999999999948e-312", __LINE__);
    RealToStreamEqual(test, stream, 1.25e-50, 15U, "1.25e-50", __LINE__);
    RealToStreamEqual(test, stream, 1.25e+50, 15U, "1.25e+50", __LINE__);
    RealToStreamEqual(test, stream, 1.125e-50, 15U, "1.125e-50", __LINE__);
    RealToStreamEqual(test, stream, 1.125e+50, 15U, "1.125e+50", __LINE__);

    RealToStreamEqual(test, stream, 1.40129846e-45, 6U, "1.4013e-45", __LINE__);
    RealToStreamEqual(test, stream, 3.40282347e+38, 6U, "3.40282e+38", __LINE__);
    RealToStreamEqual(test, stream, -1.40129846e-45, 6U, "-1.4013e-45", __LINE__);
    RealToStreamEqual(test, stream, -3.40282347e+38, 6U, "-3.40282e+38", __LINE__);

    RealToStreamEqual(test, stream, 4.9406564584124654e-324, 15U, "4.94065645841247e-324", __LINE__);
    RealToStreamEqual(test, stream, 1.7976931348623157e+308, 15U, "1.79769313486232e+308", __LINE__);
    RealToStreamEqual(test, stream, -4.9406564584124654e-324, 15U, "-4.94065645841247e-324", __LINE__);
    RealToStreamEqual(test, stream, -1.7976931348623157e+308, 15U, "-1.79769313486232e+308", __LINE__);

    RealToStreamEqual(test, stream, -854534.5, 15U, "-854534.5", __LINE__);
    RealToStreamEqual(test, stream, -854534.05, 15U, "-854534.05", __LINE__);
    RealToStreamEqual(test, stream, 854534.5, 15U, "854534.5", __LINE__);
    RealToStreamEqual(test, stream, 854534.05, 15U, "854534.05", __LINE__);
    RealToStreamEqual(test, stream, -9765625.0, 15U, "-9765625", __LINE__);
    RealToStreamEqual(test, stream, -0.2765625, 15U, "-0.2765625", __LINE__);

    RealToStreamEqual(test, stream, 9765625.0, 15U, "9765625", __LINE__);
    RealToStreamEqual(test, stream, 9765645.0, 15U, "9765645", __LINE__);
    RealToStreamEqual(test, stream, 9765665.0, 15U, "9765665", __LINE__);
    RealToStreamEqual(test, stream, 9765685.0, 15U, "9765685", __LINE__);
    RealToStreamEqual(test, stream, 9765605.0, 15U, "9765605", __LINE__);

    RealToStreamEqual(test, stream, 9765615.0, 15U, "9765615", __LINE__);
    RealToStreamEqual(test, stream, 9765635.0, 15U, "9765635", __LINE__);
    RealToStreamEqual(test, stream, 9765655.0, 15U, "9765655", __LINE__);
    RealToStreamEqual(test, stream, 9765675.0, 15U, "9765675", __LINE__);
    RealToStreamEqual(test, stream, 9765695.0, 15U, "9765695", __LINE__);

    RealToStreamEqual(test, stream, 9999425.0, 15U, "9999425", __LINE__);
    RealToStreamEqual(test, stream, 9999445.0, 15U, "9999445", __LINE__);
    RealToStreamEqual(test, stream, 9999465.0, 15U, "9999465", __LINE__);
    RealToStreamEqual(test, stream, 9999485.0, 15U, "9999485", __LINE__);
    RealToStreamEqual(test, stream, 9999405.0, 15U, "9999405", __LINE__);

    RealToStreamEqual(test, stream, 9.999925, 15U, "9.999925", __LINE__);
    RealToStreamEqual(test, stream, 9.999945, 15U, "9.999945", __LINE__);
    RealToStreamEqual(test, stream, 9.999965, 15U, "9.999965", __LINE__);
    RealToStreamEqual(test, stream, 9.999985, 15U, "9.999985", __LINE__);
    RealToStreamEqual(test, stream, 9.999905, 15U, "9.999905", __LINE__);

    RealToStreamEqual(test, stream, 1.765625, 15U, "1.765625", __LINE__);
    RealToStreamEqual(test, stream, 0.1765625, 15U, "0.1765625", __LINE__);
    RealToStreamEqual(test, stream, 0.2765625, 15U, "0.2765625", __LINE__);
    RealToStreamEqual(test, stream, 0.3765625, 15U, "0.3765625", __LINE__);
    RealToStreamEqual(test, stream, 0.4765625, 15U, "0.4765625", __LINE__);
    RealToStreamEqual(test, stream, 0.5765625, 15U, "0.5765625", __LINE__);
    RealToStreamEqual(test, stream, 0.6765625, 15U, "0.6765625", __LINE__);
    RealToStreamEqual(test, stream, 0.7765625, 15U, "0.7765625", __LINE__);
    RealToStreamEqual(test, stream, 0.8765625, 15U, "0.8765625", __LINE__);
    RealToStreamEqual(test, stream, 0.9765625, 15U, "0.9765625", __LINE__);
}

static void TestDoubleToString7(QTest &test, StringStream<char> &stream) {
    RealToStreamEqual(test, stream, 2.165625, 15U, "2.165625", __LINE__);
    RealToStreamEqual(test, stream, 2.265625, 15U, "2.265625", __LINE__);
    RealToStreamEqual(test, stream, 2.365625, 15U, "2.365625", __LINE__);
    RealToStreamEqual(test, stream, 2.465625, 15U, "2.465625", __LINE__);
    RealToStreamEqual(test, stream, 2.565625, 15U, "2.565625", __LINE__);
    RealToStreamEqual(test, stream, 2.665625, 15U, "2.665625", __LINE__);
    RealToStreamEqual(test, stream, 2.765625, 15U, "2.765625", __LINE__);
    RealToStreamEqual(test, stream, 2.865625, 15U, "2.865625", __LINE__);
    RealToStreamEqual(test, stream, 2.965625, 15U, "2.965625", __LINE__);

    RealToStreamEqual(test, stream, 5.165625, 15U, "5.165625", __LINE__);
    RealToStreamEqual(test, stream, 5.265625, 15U, "5.265625", __LINE__);
    RealToStreamEqual(test, stream, 5.365625, 15U, "5.365625", __LINE__);
    RealToStreamEqual(test, stream, 5.465625, 15U, "5.465625", __LINE__);
    RealToStreamEqual(test, stream, 5.565625, 15U, "5.565625", __LINE__);
    RealToStreamEqual(test, stream, 5.665625, 15U, "5.665625", __LINE__);
    RealToStreamEqual(test, stream, 5.765625, 15U, "5.765625", __LINE__);
    RealToStreamEqual(test, stream, 5.865625, 15U, "5.865625", __LINE__);
    RealToStreamEqual(test, stream, 5.965625, 15U, "5.965625", __LINE__);

    RealToStreamEqual(test, stream, 9.765615, 15U, "9.765615", __LINE__);
    RealToStreamEqual(test, stream, 9.765625, 15U, "9.765625", __LINE__);
    RealToStreamEqual(test, stream, 9.765635, 15U, "9.765635", __LINE__);
    RealToStreamEqual(test, stream, 9.765645, 15U, "9.765645", __LINE__);
    RealToStreamEqual(test, stream, 9.765655, 15U, "9.765655", __LINE__);
    RealToStreamEqual(test, stream, 9.765665, 15U, "9.765665", __LINE__);
    RealToStreamEqual(test, stream, 9.765675, 15U, "9.765675", __LINE__);
    RealToStreamEqual(test, stream, 9.765685, 15U, "9.765685", __LINE__);
    RealToStreamEqual(test, stream, 9.765695, 15U, "9.765695", __LINE__);
    RealToStreamEqual(test, stream, 0.9765615, 15U, "0.9765615", __LINE__);
    RealToStreamEqual(test, stream, 0.9765625, 15U, "0.9765625", __LINE__);
    RealToStreamEqual(test, stream, 0.9765635, 15U, "0.9765635", __LINE__);
    RealToStreamEqual(test, stream, 0.9765645, 15U, "0.9765645", __LINE__);
    RealToStreamEqual(test, stream, 0.9765655, 15U, "0.9765655", __LINE__);
    RealToStreamEqual(test, stream, 0.9765665, 15U, "0.9765665", __LINE__);
    RealToStreamEqual(test, stream, 0.9765675, 15U, "0.9765675", __LINE__);
    RealToStreamEqual(test, stream, 0.9765685, 15U, "0.9765685", __LINE__);
    RealToStreamEqual(test, stream, 0.9765695, 15U, "0.9765695", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-1, 15U, "0.9765625", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-2, 15U, "0.09765625", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-3, 15U, "0.009765625", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-4, 15U, "0.0009765625", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-5, 15U, "9.765625e-05", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-10, 15U, "9.765625e-10", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-13, 15U, "9.765625e-13", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-14, 15U, "9.765625e-14", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-15, 15U, "9.765625e-15", __LINE__);

    RealToStreamEqual(test, stream, 0.00001844674407370955161500, 15U, "1.84467440737096e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.0001844674407370955161500, 15U, "0.000184467440737096", __LINE__);
    RealToStreamEqual(test, stream, 0.001844674407370955161500, 15U, "0.00184467440737096", __LINE__);
    RealToStreamEqual(test, stream, 5.001844674407370955161500, 15U, "5.00184467440737", __LINE__);
    RealToStreamEqual(test, stream, 0.01844674407370955161500, 15U, "0.0184467440737095", __LINE__);
    RealToStreamEqual(test, stream, 0.1844674407370955161500, 15U, "0.184467440737096", __LINE__);

    RealToStreamEqual(test, stream, 18446744073709551614.0, 15U, "1.84467440737096e+19", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551620.0, 15U, "1.84467440737096e+19", __LINE__);
    RealToStreamEqual(test, stream, 184467440737095516150.0, 15U, "1.84467440737096e+20", __LINE__);
    RealToStreamEqual(test, stream, 1844674407370955161500.0, 15U, "1.84467440737096e+21", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551615e99, 15U, "1.84467440737096e+118", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551615e100, 15U, "1.84467440737096e+119", __LINE__);
    RealToStreamEqual(test, stream, 17446744073709551614.0, 15U, "1.74467440737096e+19", __LINE__);
    RealToStreamEqual(test, stream, 9923372036854775809.0, 15U, "9.92337203685478e+18", __LINE__);

    RealToStreamEqual(test, stream, 4.76837158203125e-50, 15U, "4.76837158203125e-50", __LINE__);
    RealToStreamEqual(test, stream, 2.384185791015625e-50, 15U, "2.38418579101563e-50", __LINE__);

    RealToStreamEqual(test, stream, 0.05, 15U, "0.05", __LINE__);
    RealToStreamEqual(test, stream, 0.005, 15U, "0.005", __LINE__);
    RealToStreamEqual(test, stream, 0.0005, 15U, "0.0005", __LINE__);
    RealToStreamEqual(test, stream, 0.00005, 15U, "5e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.000005, 15U, "5e-06", __LINE__);
    RealToStreamEqual(test, stream, 1.55, 15U, "1.55", __LINE__);
    RealToStreamEqual(test, stream, 1.0055, 15U, "1.0055", __LINE__);
    RealToStreamEqual(test, stream, 1.005999, 15U, "1.005999", __LINE__);
    RealToStreamEqual(test, stream, 1.009999, 15U, "1.009999", __LINE__);
    RealToStreamEqual(test, stream, 0.00055, 15U, "0.00055", __LINE__);
    RealToStreamEqual(test, stream, 1.00055, 15U, "1.00055", __LINE__);
    RealToStreamEqual(test, stream, 0.50009, 15U, "0.50009", __LINE__);
    RealToStreamEqual(test, stream, 1.00055e-100, 15U, "1.00055e-100", __LINE__);
    RealToStreamEqual(test, stream, 0.50009e-100, 15U, "5.0009e-101", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000050000000005, 15U, "5.0000000005e-11", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000005, 15U, "5e-13", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000000000000000000005, 15U, "5e-28", __LINE__);
    RealToStreamEqual(test, stream, 5.3125, 15U, "5.3125", __LINE__);
    RealToStreamEqual(test, stream, 5.3, 15U, "5.3", __LINE__);
    RealToStreamEqual(test, stream, 5.5, 15U, "5.5", __LINE__);
    RealToStreamEqual(test, stream, 0.55, 15U, "0.55", __LINE__);
    RealToStreamEqual(test, stream, 0.25, 15U, "0.25", __LINE__);
    RealToStreamEqual(test, stream, 0.75, 15U, "0.75", __LINE__);
    RealToStreamEqual(test, stream, 0.125, 15U, "0.125", __LINE__);
    RealToStreamEqual(test, stream, 0.625, 15U, "0.625", __LINE__);
    RealToStreamEqual(test, stream, 0.3125, 15U, "0.3125", __LINE__);
    RealToStreamEqual(test, stream, 0.95367431640625, 15U, "0.95367431640625", __LINE__);
    RealToStreamEqual(test, stream, 0.298023223876953125, 15U, "0.298023223876953", __LINE__);
    RealToStreamEqual(test, stream, 0.1490116119384765625, 15U, "0.149011611938477", __LINE__);
    RealToStreamEqual(test, stream, 0.7450580596923828125, 15U, "0.745058059692383", __LINE__);
    RealToStreamEqual(test, stream, 0.37252902984619140625, 15U, "0.372529029846191", __LINE__);
    RealToStreamEqual(test, stream, 0.186264514923095703125, 15U, "0.186264514923096", __LINE__);

    RealToStreamEqual(test, stream, 1.12345, 15U, "1.12345", __LINE__);
    RealToStreamEqual(test, stream, 1.953674, 15U, "1.953674", __LINE__);
    RealToStreamEqual(test, stream, 1.0953674, 15U, "1.0953674", __LINE__);
    RealToStreamEqual(test, stream, 1.00953674, 15U, "1.00953674", __LINE__);
    RealToStreamEqual(test, stream, 1.000953674, 15U, "1.000953674", __LINE__);
    RealToStreamEqual(test, stream, 9.953674, 15U, "9.953674", __LINE__);
    RealToStreamEqual(test, stream, 9.0953674, 15U, "9.0953674", __LINE__);
    RealToStreamEqual(test, stream, 9.00953674, 15U, "9.00953674", __LINE__);
    RealToStreamEqual(test, stream, 9.000953674, 15U, "9.000953674", __LINE__);

    RealToStreamEqual(test, stream, 0.953674, 15U, "0.953674", __LINE__);
    RealToStreamEqual(test, stream, 0.0953674, 15U, "0.0953674", __LINE__);
    RealToStreamEqual(test, stream, 0.00953674, 15U, "0.00953674", __LINE__);
    RealToStreamEqual(test, stream, 0.000953674, 15U, "0.000953674", __LINE__);
    RealToStreamEqual(test, stream, 0.123455, 15U, "0.123455", __LINE__);
    RealToStreamEqual(test, stream, 0.1123405, 15U, "0.1123405", __LINE__);
    RealToStreamEqual(test, stream, 0.1123095, 15U, "0.1123095", __LINE__);
    RealToStreamEqual(test, stream, 0.78126, 15U, "0.78126", __LINE__);
    RealToStreamEqual(test, stream, 0.300048828125, 15U, "0.300048828125", __LINE__);

    RealToStreamEqual(test, stream, 0.111, 15U, "0.111", __LINE__);
    RealToStreamEqual(test, stream, 0.11111, 15U, "0.11111", __LINE__);
    RealToStreamEqual(test, stream, 0.1111111111111111, 15U, "0.111111111111111", __LINE__);
    RealToStreamEqual(test, stream, 0.1111111111111115, 15U, "0.111111111111111", __LINE__);
    RealToStreamEqual(test, stream, 0.11111111111111159, 15U, "0.111111111111112", __LINE__);
    RealToStreamEqual(test, stream, 0.1111111111111116, 15U, "0.111111111111112", __LINE__);

    RealToStreamEqual(test, stream, 105.625, 15U, "105.625", __LINE__);
    RealToStreamEqual(test, stream, 25e20, 15U, "2.5e+21", __LINE__);
    RealToStreamEqual(test, stream, 2.5e21, 15U, "2.5e+21", __LINE__);
    RealToStreamEqual(test, stream, 3814697265625.0, 15U, "3814697265625", __LINE__);
    RealToStreamEqual(test, stream, 381469725.0, 15U, "381469725", __LINE__);
    RealToStreamEqual(test, stream, 0.3814697265625, 15U, "0.3814697265625", __LINE__);
    RealToStreamEqual(test, stream, 10.5, 15U, "10.5", __LINE__);
    RealToStreamEqual(test, stream, 10.0, 15U, "10", __LINE__);

    RealToStreamEqual(test, stream, 2097151.125, 15U, "2097151.125", __LINE__);
    RealToStreamEqual(test, stream, 2097151.0, 15U, "2097151", __LINE__);
    RealToStreamEqual(test, stream, 16217975.0, 15U, "16217975", __LINE__);
    RealToStreamEqual(test, stream, 32995191.0, 15U, "32995191", __LINE__);
    RealToStreamEqual(test, stream, 444444444444444444444.0, 15U, "4.44444444444444e+20", __LINE__);
    RealToStreamEqual(test, stream, 555555555555555555555.0, 15U, "5.55555555555556e+20", __LINE__);
    RealToStreamEqual(test, stream, 3668262915145728.0, 15U, "3.66826291514573e+15", __LINE__);
    RealToStreamEqual(test, stream, 7336525830291456.0, 15U, "7.33652583029146e+15", __LINE__);
    RealToStreamEqual(test, stream, 7336535830291456.0, 15U, "7.33653583029146e+15", __LINE__);
    RealToStreamEqual(test, stream, 14673051660582912.0, 15U, "1.46730516605829e+16", __LINE__);

    RealToStreamEqual(test, stream, 1000000.390625, 15U, "1000000.390625", __LINE__);
    RealToStreamEqual(test, stream, 1000009.590625, 15U, "1000009.590625", __LINE__);
    RealToStreamEqual(test, stream, 1000008.590625, 15U, "1000008.590625", __LINE__);
    RealToStreamEqual(test, stream, 7.3365258e-10, 15U, "7.3365258e-10", __LINE__);
    RealToStreamEqual(test, stream, 0.3333, 15U, "0.3333", __LINE__);

    RealToStreamEqual(test, stream, 4554534.0, 15U, "4554534", __LINE__);
    RealToStreamEqual(test, stream, 854534.055, 15U, "854534.055", __LINE__);
    RealToStreamEqual(test, stream, 854534.0055, 15U, "854534.0055", __LINE__);
    RealToStreamEqual(test, stream, 854534.000055, 15U, "854534.000055", __LINE__);
    RealToStreamEqual(test, stream, 8.0000005, 15U, "8.0000005", __LINE__);
    RealToStreamEqual(test, stream, 8.000005, 15U, "8.000005", __LINE__);
    RealToStreamEqual(test, stream, 8.005, 15U, "8.005", __LINE__);
}

static void TestDoubleToString8(QTest &test, StringStream<char> &stream) {
    RealToStreamEqual(test, stream, -1.0, 6U, "-1", __LINE__);

    RealToStreamEqual(test, stream, 70.0, 6U, "70", __LINE__);
    RealToStreamEqual(test, stream, 7e+10, 6U, "7e+10", __LINE__);
    RealToStreamEqual(test, stream, 75e+10, 6U, "7.5e+11", __LINE__);
    RealToStreamEqual(test, stream, 7e+50, 6U, "7e+50", __LINE__);
    RealToStreamEqual(test, stream, 5000000000000000.25, 30U, "5000000000000000", __LINE__);
    RealToStreamEqual(test, stream, 5000000000000000.0, 6U, "5e+15", __LINE__);

    RealToStreamEqual(test, stream, 9999998.0, 6U, "1e+07", __LINE__);
    RealToStreamEqual(test, stream, 9999998.9, 6U, "1e+07", __LINE__);
    RealToStreamEqual(test, stream, 999999999.999999999, 6U, "1e+09", __LINE__);

    RealToStreamEqual(test, stream, 111.5, 6U, "111.5", __LINE__);
    RealToStreamEqual(test, stream, 999.625, 6U, "999.625", __LINE__);
    RealToStreamEqual(test, stream, 1e+33, 6U, "1e+33", __LINE__);

    RealToStreamEqual(test, stream, 18446744073709551615.0, 2U, "1.8e+19", __LINE__);
    RealToStreamEqual(test, stream, 0.18446744073709551615, 2U, "0.18", __LINE__);

    RealToStreamEqual(test, stream, 10.7, 6U, "10.7", __LINE__);
    RealToStreamEqual(test, stream, 20.7, 6U, "20.7", __LINE__);
    RealToStreamEqual(test, stream, 30.7, 6U, "30.7", __LINE__);
    RealToStreamEqual(test, stream, 40.7, 6U, "40.7", __LINE__);
    RealToStreamEqual(test, stream, 50.7, 6U, "50.7", __LINE__);
    RealToStreamEqual(test, stream, 60.7, 6U, "60.7", __LINE__);
    RealToStreamEqual(test, stream, 70.7, 6U, "70.7", __LINE__);
    RealToStreamEqual(test, stream, 80.7, 6U, "80.7", __LINE__);
    RealToStreamEqual(test, stream, 90.7, 6U, "90.7", __LINE__);
    RealToStreamEqual(test, stream, 50.25, 6U, "50.25", __LINE__);
    RealToStreamEqual(test, stream, 500000000000000.25, 6U, "5e+14", __LINE__);
    RealToStreamEqual(test, stream, 500000000000000.25, 18U, "500000000000000.25", __LINE__);
    RealToStreamEqual(test, stream, 7.7, 6U, "7.7", __LINE__);
    RealToStreamEqual(test, stream, 7.000007, 8U, "7.000007", __LINE__);
    RealToStreamEqual(test, stream, 1.25, 6U, "1.25", __LINE__);

    RealToStreamEqual(test, stream, 0.07, 6U, "0.07", __LINE__);
    RealToStreamEqual(test, stream, 0.3125, 4U, "0.3125", __LINE__);
    RealToStreamEqual(test, stream, 5e-4, 6U, "0.0005", __LINE__);
    RealToStreamEqual(test, stream, 5e-10, 6U, "5e-10", __LINE__);
    RealToStreamEqual(test, stream, 5e-10, 10U, "5e-10", __LINE__);
    RealToStreamEqual(test, stream, 5e-10, 11U, "5e-10", __LINE__);
    RealToStreamEqual(test, stream, 7e-24, 6U, "7e-24", __LINE__);

    RealToStreamEqual(test, stream, 0.390625, 6U, "0.390625", __LINE__);
    RealToStreamEqual(test, stream, 0.000390625, 6U, "0.000390625", __LINE__);
    RealToStreamEqual(test, stream, 0.0000390625, 6U, "3.90625e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.00000390625, 6U, "3.90625e-06", __LINE__);
    RealToStreamEqual(test, stream, 0.000009999999, 6U, "1e-05", __LINE__);
    RealToStreamEqual(test, stream, 505.3125, 6U, "505.312", __LINE__);
    RealToStreamEqual(test, stream, 505.003125, 6U, "505.003", __LINE__);

    RealToStreamEqual(test, stream, 1e+300, 50U, "1.0000000000000000525047602552044202487044685811082e+300", __LINE__);
    RealToStreamEqual(
        test, stream, 1.7976931348623157e+308, 309U,
        "179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",
        __LINE__);

    RealToStreamEqual(test, stream, 1e-300, 50U, "1.0000000000000000250590918352087596856961468077037e-300", __LINE__);

    RealToStreamEqual(
        test, stream, 4.9406564584124654e-324, 324U,
        "4.94065645841246544176568792868221372365059802614324764425585682500675507270208751865299836361635992379796564695445717730926656710355939796398774796010781878126300713190311404527845817167848982103688718636056998730723050006387409153564984387312473397273169615140031715385398074126238565591171026658556686768187039560310624932e-324",
        __LINE__);
}

static void TestDoubleToStringSemiFixed(QTest &test, StringStream<char> &stream) {
    RealToStreamEqualSemiFixed(test, stream, -0.0, 6U, "-0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0, 6U, "0", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 1.0, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.0, 6U, "2", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 3.0, 6U, "3", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 4.0, 6U, "4", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.0, 6U, "5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 6.0, 6U, "6", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 7.0, 6U, "7", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 8.0, 6U, "8", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.0, 6U, "9", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 220.0, 6U, "220", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.1, 6U, "0.1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.2, 6U, "0.2", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.3, 6U, "0.3", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.4, 6U, "0.4", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.5, 6U, "0.5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.6, 6U, "0.6", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.7, 6U, "0.7", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.8, 6U, "0.8", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9, 6U, "0.9", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.09, 6U, "0.09", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.009, 6U, "0.009", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0009, 6U, "0.0009", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.00009, 6U, "0.00009", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.000009, 6U, "0.000009", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0000009, 6U, "0.000001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.00000009, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.000000009, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0000000009, 6U, "0", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.01, 6U, "0.01", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.001, 6U, "0.001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0001, 6U, "0.0001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.00001, 6U, "0.00001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.000001, 6U, "0.000001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0000001, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.00000001, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.000000001, 6U, "0", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.02, 6U, "0.02", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.002, 6U, "0.002", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0002, 6U, "0.0002", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.00002, 6U, "0.00002", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.000002, 6U, "0.000002", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0000002, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.00000002, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.000000002, 6U, "0", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 1.5, 6U, "1.5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.05, 6U, "1.05", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.005, 6U, "1.005", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0005, 6U, "1.0005", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.00005, 6U, "1.00005", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.000005, 6U, "1.000005", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0000005, 6U, "1.000001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.00000005, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.000000005, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0000000005, 6U, "1", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 1.9, 6U, "1.9", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.09, 6U, "1.09", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.009, 6U, "1.009", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0009, 6U, "1.0009", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.00009, 6U, "1.00009", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.000009, 6U, "1.000009", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0000009, 6U, "1.000001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.00000009, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.000000009, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0000000009, 6U, "1", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 1.1, 6U, "1.1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.01, 6U, "1.01", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.001, 6U, "1.001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0001, 6U, "1.0001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.00001, 6U, "1.00001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.000001, 6U, "1.000001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0000001, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.00000001, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.000000001, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0000000001, 6U, "1", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.1000000000000006, 6U, "0.1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.000000000000011, 6U, "0", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.02, 6U, "0.02", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.002, 6U, "0.002", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0002, 6U, "0.0002", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 123456789.0, 6U, "123456789", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2147483647.0, 6U, "2147483647", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 4294967295.0, 6U, "4294967295", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 4294967295.625, 6U, "4294967295.625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2251799813685247.0, 6U, "2251799813685247", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 4503599627370495.0, 6U, "4503599627370495", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 4611686018427387904.0, 6U, "4611686018427387904", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9223372036854775808.0, 6U, "9223372036854775808", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.9000000000000006, 6U, "0.9", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 9.999999999999999, 6U, "10", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1999999.0, 6U, "1999999", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9999999999999999.0, 6U, "10000000000000000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 999999999999999999.0, 6U, "1000000000000000000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9999999999999999999.0, 6U, "10000000000000000000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 99999999999999999999.0, 6U, "100000000000000000000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9999999999999999999999999999999999999999.0, 6U,
                               "10000000000000000303786028427003666890752", __LINE__);
    RealToStreamEqualSemiFixed(
        test, stream, 99e100, 6U,
        "989999999999999971062477677470550235220096190889648004812994130017827049653182301025734968880029237248",
        __LINE__);

    RealToStreamEqual(test, stream, 599.9999, 6U, "600", __LINE__);
    RealToStreamEqual(test, stream, 6999.999, 6U, "7000", __LINE__);
    RealToStreamEqual(test, stream, 79999.999, 6U, "80000", __LINE__);
    RealToStreamEqual(test, stream, 899999.999, 6U, "900000", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.999, 6U, "0.999", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.999999, 6U, "0.999999", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.999999999, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.999999999, 6U, "10", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 99.999999999, 6U, "100", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 999.999999999, 6U, "1000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.999999999999999, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9999999999999999, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9999999999999999, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9999999999999995, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9999999999999959, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9999999999999996, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.99999999999999959, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.99999999999999999, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.99999999999999999999999999999999, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.99999999999999999999999999999999999999999, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9999999999999999e-99, 6U, "0", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 10000005.0, 6U, "10000005", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 10000005.0, 7U, "10000005", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 10000006.0, 7U, "10000006", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 10000000.0, 6U, "10000000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 69999999.0, 6U, "69999999", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 999999999.0, 6U, "999999999", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.10000000, 6U, "0.1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9999999, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.19999999, 6U, "0.2", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.29999999, 6U, "0.3", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.39999999, 6U, "0.4", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.49999999, 6U, "0.5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.59999999, 6U, "0.6", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.69999999, 6U, "0.7", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.79999999, 6U, "0.8", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.89999999, 6U, "0.9", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.99999999, 6U, "1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.99999999, 6U, "2", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.99999999, 6U, "3", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 3.99999999, 6U, "4", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 4.99999999, 6U, "5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.99999999, 6U, "6", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 6.99999999, 6U, "7", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 7.99999999, 6U, "8", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 8.99999999, 6U, "9", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.99999999, 6U, "10", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 99.99999999, 6U, "100", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 999.99999999, 6U, "1000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 99999.89999999, 6U, "99999.9", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 99999.99999999, 6U, "100000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 999999.99999999, 6U, "1000000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9e-50, 6U, "0", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 1e1, 6U, "10", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e2, 6U, "100", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e3, 6U, "1000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e4, 6U, "10000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e5, 6U, "100000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e10, 6U, "10000000000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e13, 6U, "10000000000000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e15, 6U, "1000000000000000", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 1e-1, 6U, "0.1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e-2, 6U, "0.01", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e-3, 6U, "0.001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e-4, 6U, "0.0001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e-5, 6U, "0.00001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e-10, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e-13, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e-14, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1e-15, 6U, "0", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 1.25e-50, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.125e-50, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.125e+10, 6U, "11250000000", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, -854534.5, 6U, "-854534.5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -854534.05, 6U, "-854534.05", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 854534.5, 6U, "854534.5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 854534.05, 6U, "854534.05", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9765625.9, 6U, "-9765625.9", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.2765625, 6U, "-0.276562", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 97765625.0, 6U, "97765625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 97765645.0, 6U, "97765645", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 97765665.0, 6U, "97765665", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 97765685.0, 6U, "97765685", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 97765605.0, 6U, "97765605", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 9.9999925, 6U, "9.999992", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.9999945, 6U, "9.999994", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.9999965, 6U, "9.999996", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.9999985, 6U, "9.999999", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.9999905, 6U, "9.99999", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 1.7765625, 6U, "1.776563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.1765625, 6U, "0.176563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.2765625, 6U, "0.276562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.3765625, 6U, "0.376563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.4765625, 6U, "0.476562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.5765625, 6U, "0.576562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.6765625, 6U, "0.676562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.7765625, 6U, "0.776563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.8765625, 6U, "0.876563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9765625, 6U, "0.976562", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 2.1765625, 6U, "2.176563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.2765625, 6U, "2.276562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.3765625, 6U, "2.376562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.4765625, 6U, "2.476562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.5765625, 6U, "2.576563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.6765625, 6U, "2.676563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.7765625, 6U, "2.776562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.8765625, 6U, "2.876562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.9765625, 6U, "2.976562", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 5.165625, 6U, "5.165625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.265625, 6U, "5.265625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.365625, 6U, "5.365625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.465625, 6U, "5.465625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.565625, 6U, "5.565625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.665625, 6U, "5.665625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.765625, 6U, "5.765625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.865625, 6U, "5.865625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.965625, 6U, "5.965625", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 9.7765615, 6U, "9.776561", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.7765625, 6U, "9.776563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.7765635, 6U, "9.776563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.7765645, 6U, "9.776564", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.7765655, 6U, "9.776566", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.7765665, 6U, "9.776566", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.7765675, 6U, "9.776568", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.7765685, 6U, "9.776568", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.7765695, 6U, "9.77657", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9765615, 6U, "0.976561", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9765625, 6U, "0.976562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9765635, 6U, "0.976564", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9765645, 6U, "0.976564", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9765655, 6U, "0.976565", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9765665, 6U, "0.976567", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9765675, 6U, "0.976568", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9765685, 6U, "0.976568", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.9765695, 6U, "0.976569", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.765625e-1, 6U, "0.976562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.765625e-2, 6U, "0.097656", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.765625e-3, 6U, "0.009766", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.765625e-4, 6U, "0.000977", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.765625e-5, 6U, "0.000098", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.765625e-10, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.765625e-13, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.765625e-14, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.765625e-15, 6U, "0", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, -9765625.0, 6U, "-9765625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9765695.0, 6U, "-9765695", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9999465.0, 6U, "-9999465", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.999945, 6U, "-9.999945", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, -1.8765625, 6U, "-1.876562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.1765625, 6U, "-0.176563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.2765625, 6U, "-0.276562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.3765625, 6U, "-0.376563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.4765625, 6U, "-0.476562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.5765625, 6U, "-0.576562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.6765625, 6U, "-0.676562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.7765625, 6U, "-0.776563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.8765625, 6U, "-0.876563", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.9765625, 6U, "-0.976562", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, -2.8165625, 6U, "-2.816562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -2.2653625, 6U, "-2.265363", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -2.3653625, 6U, "-2.365362", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -2.4653625, 6U, "-2.465362", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -2.5653625, 6U, "-2.565362", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -2.6653625, 6U, "-2.665363", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -2.7653625, 6U, "-2.765363", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -2.8653625, 6U, "-2.865362", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -2.9653625, 6U, "-2.965362", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, -5.1656325, 6U, "-5.165633", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -5.2656325, 6U, "-5.265632", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -5.3656325, 6U, "-5.365633", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -5.4656325, 6U, "-5.465632", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -5.5656325, 6U, "-5.565633", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -5.6656325, 6U, "-5.665633", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -5.7656325, 6U, "-5.765632", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -5.8656325, 6U, "-5.865633", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -5.9656325, 6U, "-5.965632", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, -9.765615, 6U, "-9.765615", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.765625, 6U, "-9.765625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.765635, 6U, "-9.765635", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.765645, 6U, "-9.765645", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.765655, 6U, "-9.765655", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.765665, 6U, "-9.765665", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.765675, 6U, "-9.765675", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.765685, 6U, "-9.765685", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.765695, 6U, "-9.765695", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.9765615, 6U, "-0.976561", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.9765625, 6U, "-0.976562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.9765635, 6U, "-0.976564", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.9765645, 6U, "-0.976564", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.9765655, 6U, "-0.976565", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.9765665, 6U, "-0.976567", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.9765675, 6U, "-0.976568", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.9765685, 6U, "-0.976568", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -0.9765695, 6U, "-0.976569", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.765625e-1, 6U, "-0.976562", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.765625e-2, 6U, "-0.097656", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.765625e-3, 6U, "-0.009766", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.765625e-4, 6U, "-0.000977", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, -9.765625e-5, 6U, "-0.000098", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.00001844674407370955161500, 6U, "0.000018", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0001844674407370955161500, 6U, "0.000184", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.001844674407370955161500, 6U, "0.001845", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.001844674407370955161500, 6U, "5.001845", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.01844674407370955161500, 6U, "0.018447", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.1844674407370955161500, 6U, "0.184467", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.05, 6U, "0.05", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.005, 6U, "0.005", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0005, 6U, "0.0005", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.00005, 6U, "0.00005", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.000005, 6U, "0.000005", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.55, 6U, "1.55", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0055, 6U, "1.0055", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0059999, 6U, "1.006", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0099999, 6U, "1.01", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.00055, 6U, "0.00055", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.00055, 6U, "1.00055", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.50009, 6U, "0.50009", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.000000000050000000005, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0000000000005, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0000000000000000000000000005, 6U, "0", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.3125, 6U, "5.3125", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.3, 6U, "5.3", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 5.5, 6U, "5.5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.55, 6U, "0.55", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.25, 6U, "0.25", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.75, 6U, "0.75", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.125, 6U, "0.125", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.625, 6U, "0.625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.3125, 6U, "0.3125", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.95367431640625, 6U, "0.953674", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.298023223876953125, 6U, "0.298023", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.1490116119384765625, 6U, "0.149012", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.7450580596923828125, 6U, "0.745058", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.37252902984619140625, 6U, "0.372529", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.186264514923095703125, 6U, "0.186265", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 1.12345, 6U, "1.12345", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.953674, 6U, "1.953674", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0953674, 6U, "1.095367", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.00953674, 6U, "1.009537", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.000953674, 6U, "1.000954", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.953674, 6U, "9.953674", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.0953674, 6U, "9.095367", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.00953674, 6U, "9.009537", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.000953674, 6U, "9.000954", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.953674, 6U, "0.953674", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0953674, 6U, "0.095367", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.00953674, 6U, "0.009537", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.000953674, 6U, "0.000954", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.123455, 6U, "0.123455", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.1123405, 6U, "0.11234", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.1123095, 6U, "0.11231", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.78126, 6U, "0.78126", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.300048828125, 6U, "0.300049", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.111, 6U, "0.111", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.11111, 6U, "0.11111", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.1111111111111111, 6U, "0.111111", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.1111111111111115, 6U, "0.111111", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.11111111111111159, 6U, "0.111111", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.1111111111111116, 6U, "0.111111", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 105.625, 6U, "105.625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 25e20, 6U, "2500000000000000000000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.5e21, 6U, "2500000000000000000000", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 3814697265625.0, 6U, "3814697265625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 381469725.0, 6U, "381469725", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.3814697265625, 6U, "0.38147", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 10.5, 6U, "10.5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.3333, 6U, "0.3333", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 4554534.0, 6U, "4554534", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 854534.055, 6U, "854534.055", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 854534.0055, 6U, "854534.0055", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 854534.000055, 6U, "854534.000055", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 854534.0000005, 14U, "854534.00000050000381", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 8.0000005, 6U, "8.000001", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 8.000005, 6U, "8.000005", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 8.005, 6U, "8.005", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0099999, 6U, "0.01", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0099999, 6U, "1.01", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.0999999, 6U, "0.1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.0999999, 6U, "1.1", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 5.55, 1U, "5.5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.996, 5U, "9.996", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.996, 4U, "9.996", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.996, 3U, "9.996", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.996, 2U, "10", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 9.996, 1U, "10", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.5, 1U, "0.5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 1.5, 1U, "1.5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.5, 1U, "2.5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 3.5, 1U, "3.5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 4.5, 1U, "4.5", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.25, 1U, "2.2", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.75, 1U, "2.8", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.05, 1U, "0.1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.15, 1U, "0.1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.25, 1U, "0.2", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.35, 1U, "0.3", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.45, 1U, "0.5", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 2.626, 2U, "2.63", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.66, 2U, "2.66", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.26, 2U, "2.26", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 3.266, 2U, "3.27", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.626, 2U, "2.63", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 2.635, 2U, "2.63", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.636, 2U, "2.64", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.36, 2U, "2.36", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 3.35, 2U, "3.35", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 2.635, 2U, "2.63", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 95.0, 1U, "95", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 95.11, 1U, "95.1", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 95.99, 1U, "96", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 95000000.0, 1U, "95000000", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 3.0, 15U, "3", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 3.14, 15U, "3.14", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 140737488355327.953674, 15U, "140737488355327.953125", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 140737488355327.015625, 15U, "140737488355327.015625", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 140737458355327.953674, 15U, "140737458355327.953125", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 140737488355327.953674, 6U, "140737488355327.953125", __LINE__);

    RealToStreamEqualSemiFixed(test, stream, 0.95367431640625, 20U, "0.95367431640625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.298023223876953125, 20U, "0.298023223876953125", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.1490116119384765625, 20U, "0.1490116119384765625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.7450580596923828125, 20U, "0.7450580596923828125", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.37252902984619140625, 20U, "0.37252902984619140625", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.186264514923095703125, 20U, "0.18626451492309570312", __LINE__);
    RealToStreamEqualSemiFixed(test, stream, 0.186264514923095703125, 21U, "0.186264514923095703125", __LINE__);
}

static void TestDoubleToStringFixed(QTest &test, StringStream<char> &stream) {
    RealToStreamEqualFixed(test, stream, -0.0, 6U, "-0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0, 6U, "0.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 1.0, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.0, 6U, "2.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 3.0, 6U, "3.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 4.0, 6U, "4.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.0, 6U, "5.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 6.0, 6U, "6.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 7.0, 6U, "7.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 8.0, 6U, "8.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.0, 6U, "9.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 220.0, 6U, "220.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.1, 6U, "0.100000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.2, 6U, "0.200000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.3, 6U, "0.300000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.4, 6U, "0.400000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.5, 6U, "0.500000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.6, 6U, "0.600000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.7, 6U, "0.700000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.8, 6U, "0.800000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9, 6U, "0.900000", __LINE__);

    RealToStreamEqualFixed(test, stream, 1.0, 60U, "1.000000000000000000000000000000000000000000000000000000000000",
                           __LINE__);

    RealToStreamEqualFixed(test, stream, 0.09, 6U, "0.090000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.009, 6U, "0.009000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0009, 6U, "0.000900", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.00009, 6U, "0.000090", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.000009, 6U, "0.000009", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0000009, 6U, "0.000001", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.00000009, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.000000009, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0000000009, 6U, "0.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.01, 6U, "0.010000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.001, 6U, "0.001000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0001, 6U, "0.000100", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.00001, 6U, "0.000010", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.000001, 6U, "0.000001", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0000001, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.00000001, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.000000001, 6U, "0.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.02, 6U, "0.020000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.002, 6U, "0.002000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0002, 6U, "0.000200", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.00002, 6U, "0.000020", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.000002, 6U, "0.000002", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0000002, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.00000002, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.000000002, 6U, "0.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 1.5, 6U, "1.500000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.05, 6U, "1.050000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.005, 6U, "1.005000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0005, 6U, "1.000500", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.00005, 6U, "1.000050", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.000005, 6U, "1.000005", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0000005, 6U, "1.000001", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.00000005, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.000000005, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0000000005, 6U, "1.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 1.9, 6U, "1.900000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.09, 6U, "1.090000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.009, 6U, "1.009000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0009, 6U, "1.000900", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.00009, 6U, "1.000090", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.000009, 6U, "1.000009", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0000009, 6U, "1.000001", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.00000009, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.000000009, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0000000009, 6U, "1.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 1.1, 6U, "1.100000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.01, 6U, "1.010000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.001, 6U, "1.001000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0001, 6U, "1.000100", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.00001, 6U, "1.000010", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.000001, 6U, "1.000001", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0000001, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.00000001, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.000000001, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0000000001, 6U, "1.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.1000000000000006, 6U, "0.100000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.000000000000011, 6U, "0.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.02, 6U, "0.020000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.002, 6U, "0.002000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0002, 6U, "0.000200", __LINE__);

    RealToStreamEqualFixed(test, stream, 123456789.0, 6U, "123456789.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 2147483647.0, 6U, "2147483647.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 4294967295.0, 6U, "4294967295.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 4294967295.625, 6U, "4294967295.625000", __LINE__);
    RealToStreamEqualFixed(test, stream, 2251799813685247.0, 6U, "2251799813685247.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 4503599627370495.0, 6U, "4503599627370495.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 4611686018427387904.0, 6U, "4611686018427387904.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 9223372036854775808.0, 6U, "9223372036854775808.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.9000000000000006, 6U, "0.900000", __LINE__);

    RealToStreamEqualFixed(test, stream, 9.999999999999999, 6U, "10.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1999999.0, 6U, "1999999.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 9999999999999999.0, 6U, "10000000000000000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 999999999999999999.0, 6U, "1000000000000000000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 9999999999999999999.0, 6U, "10000000000000000000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 99999999999999999999.0, 6U, "100000000000000000000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 9999999999999999999999999999999999999999.0, 6U,
                           "10000000000000000303786028427003666890752.000000", __LINE__);
    RealToStreamEqualFixed(
        test, stream, 99e100, 6U,
        "989999999999999971062477677470550235220096190889648004812994130017827049653182301025734968880029237248.000000",
        __LINE__);

    RealToStreamEqual(test, stream, 599.9999, 6U, "600", __LINE__);
    RealToStreamEqual(test, stream, 6999.999, 6U, "7000", __LINE__);
    RealToStreamEqual(test, stream, 79999.999, 6U, "80000", __LINE__);
    RealToStreamEqual(test, stream, 899999.999, 6U, "900000", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.999, 6U, "0.999000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.999999, 6U, "0.999999", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.999999999, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.999999999, 6U, "10.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 99.999999999, 6U, "100.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 999.999999999, 6U, "1000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.999999999999999, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9999999999999999, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9999999999999999, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9999999999999995, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9999999999999959, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9999999999999996, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.99999999999999959, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.99999999999999999, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.99999999999999999999999999999999, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.99999999999999999999999999999999999999999, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9999999999999999e-99, 6U, "0.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 10000005.0, 6U, "10000005.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 10000005.0, 7U, "10000005.0000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 10000006.0, 7U, "10000006.0000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 10000000.0, 6U, "10000000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 69999999.0, 6U, "69999999.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 999999999.0, 6U, "999999999.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.10000000, 6U, "0.100000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9999999, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.19999999, 6U, "0.200000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.29999999, 6U, "0.300000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.39999999, 6U, "0.400000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.49999999, 6U, "0.500000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.59999999, 6U, "0.600000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.69999999, 6U, "0.700000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.79999999, 6U, "0.800000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.89999999, 6U, "0.900000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.99999999, 6U, "1.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.99999999, 6U, "2.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.99999999, 6U, "3.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 3.99999999, 6U, "4.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 4.99999999, 6U, "5.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.99999999, 6U, "6.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 6.99999999, 6U, "7.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 7.99999999, 6U, "8.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 8.99999999, 6U, "9.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.99999999, 6U, "10.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 99.99999999, 6U, "100.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 999.99999999, 6U, "1000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 99999.89999999, 6U, "99999.900000", __LINE__);
    RealToStreamEqualFixed(test, stream, 99999.99999999, 6U, "100000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 999999.99999999, 6U, "1000000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 9e-50, 6U, "0.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 1e1, 6U, "10.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e2, 6U, "100.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e3, 6U, "1000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e4, 6U, "10000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e5, 6U, "100000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e10, 6U, "10000000000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e13, 6U, "10000000000000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e15, 6U, "1000000000000000.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 1e-1, 6U, "0.100000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e-2, 6U, "0.010000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e-3, 6U, "0.001000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e-4, 6U, "0.000100", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e-5, 6U, "0.000010", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e-10, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e-13, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e-14, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1e-15, 6U, "0.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 1.25e-50, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.125e-50, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.125e+10, 6U, "11250000000.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, -854534.5, 6U, "-854534.500000", __LINE__);
    RealToStreamEqualFixed(test, stream, -854534.05, 6U, "-854534.050000", __LINE__);
    RealToStreamEqualFixed(test, stream, 854534.5, 6U, "854534.500000", __LINE__);
    RealToStreamEqualFixed(test, stream, 854534.05, 6U, "854534.050000", __LINE__);
    RealToStreamEqualFixed(test, stream, -9765625.9, 6U, "-9765625.900000", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.2765625, 6U, "-0.276562", __LINE__);

    RealToStreamEqualFixed(test, stream, 97765625.0, 6U, "97765625.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 97765645.0, 6U, "97765645.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 97765665.0, 6U, "97765665.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 97765685.0, 6U, "97765685.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 97765605.0, 6U, "97765605.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 9.9999925, 6U, "9.999992", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.9999945, 6U, "9.999994", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.9999965, 6U, "9.999996", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.9999985, 6U, "9.999999", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.9999905, 6U, "9.999990", __LINE__);

    RealToStreamEqualFixed(test, stream, 1.7765625, 6U, "1.776563", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.1765625, 6U, "0.176563", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.2765625, 6U, "0.276562", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.3765625, 6U, "0.376563", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.4765625, 6U, "0.476562", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.5765625, 6U, "0.576562", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.6765625, 6U, "0.676562", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.7765625, 6U, "0.776563", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.8765625, 6U, "0.876563", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9765625, 6U, "0.976562", __LINE__);

    RealToStreamEqualFixed(test, stream, 2.1765625, 6U, "2.176563", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.2765625, 6U, "2.276562", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.3765625, 6U, "2.376562", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.4765625, 6U, "2.476562", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.5765625, 6U, "2.576563", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.6765625, 6U, "2.676563", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.7765625, 6U, "2.776562", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.8765625, 6U, "2.876562", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.9765625, 6U, "2.976562", __LINE__);

    RealToStreamEqualFixed(test, stream, 5.165625, 6U, "5.165625", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.265625, 6U, "5.265625", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.365625, 6U, "5.365625", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.465625, 6U, "5.465625", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.565625, 6U, "5.565625", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.665625, 6U, "5.665625", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.765625, 6U, "5.765625", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.865625, 6U, "5.865625", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.965625, 6U, "5.965625", __LINE__);

    RealToStreamEqualFixed(test, stream, 9.7765615, 6U, "9.776561", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.7765625, 6U, "9.776563", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.7765635, 6U, "9.776563", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.7765645, 6U, "9.776564", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.7765655, 6U, "9.776566", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.7765665, 6U, "9.776566", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.7765675, 6U, "9.776568", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.7765685, 6U, "9.776568", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.7765695, 6U, "9.776570", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9765615, 6U, "0.976561", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9765625, 6U, "0.976562", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9765635, 6U, "0.976564", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9765645, 6U, "0.976564", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9765655, 6U, "0.976565", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9765665, 6U, "0.976567", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9765675, 6U, "0.976568", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9765685, 6U, "0.976568", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.9765695, 6U, "0.976569", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.765625e-1, 6U, "0.976562", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.765625e-2, 6U, "0.097656", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.765625e-3, 6U, "0.009766", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.765625e-4, 6U, "0.000977", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.765625e-5, 6U, "0.000098", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.765625e-10, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.765625e-13, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.765625e-14, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.765625e-15, 6U, "0.000000", __LINE__);

    RealToStreamEqualFixed(test, stream, -9765625.0, 6U, "-9765625.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, -9765695.0, 6U, "-9765695.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, -9999465.0, 6U, "-9999465.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.999945, 6U, "-9.999945", __LINE__);

    RealToStreamEqualFixed(test, stream, -1.8765625, 6U, "-1.876562", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.1765625, 6U, "-0.176563", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.2765625, 6U, "-0.276562", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.3765625, 6U, "-0.376563", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.4765625, 6U, "-0.476562", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.5765625, 6U, "-0.576562", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.6765625, 6U, "-0.676562", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.7765625, 6U, "-0.776563", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.8765625, 6U, "-0.876563", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.9765625, 6U, "-0.976562", __LINE__);

    RealToStreamEqualFixed(test, stream, -2.8165625, 6U, "-2.816562", __LINE__);
    RealToStreamEqualFixed(test, stream, -2.2653625, 6U, "-2.265363", __LINE__);
    RealToStreamEqualFixed(test, stream, -2.3653625, 6U, "-2.365362", __LINE__);
    RealToStreamEqualFixed(test, stream, -2.4653625, 6U, "-2.465362", __LINE__);
    RealToStreamEqualFixed(test, stream, -2.5653625, 6U, "-2.565362", __LINE__);
    RealToStreamEqualFixed(test, stream, -2.6653625, 6U, "-2.665363", __LINE__);
    RealToStreamEqualFixed(test, stream, -2.7653625, 6U, "-2.765363", __LINE__);
    RealToStreamEqualFixed(test, stream, -2.8653625, 6U, "-2.865362", __LINE__);
    RealToStreamEqualFixed(test, stream, -2.9653625, 6U, "-2.965362", __LINE__);

    RealToStreamEqualFixed(test, stream, -5.1656325, 6U, "-5.165633", __LINE__);
    RealToStreamEqualFixed(test, stream, -5.2656325, 6U, "-5.265632", __LINE__);
    RealToStreamEqualFixed(test, stream, -5.3656325, 6U, "-5.365633", __LINE__);
    RealToStreamEqualFixed(test, stream, -5.4656325, 6U, "-5.465632", __LINE__);
    RealToStreamEqualFixed(test, stream, -5.5656325, 6U, "-5.565633", __LINE__);
    RealToStreamEqualFixed(test, stream, -5.6656325, 6U, "-5.665633", __LINE__);
    RealToStreamEqualFixed(test, stream, -5.7656325, 6U, "-5.765632", __LINE__);
    RealToStreamEqualFixed(test, stream, -5.8656325, 6U, "-5.865633", __LINE__);
    RealToStreamEqualFixed(test, stream, -5.9656325, 6U, "-5.965632", __LINE__);

    RealToStreamEqualFixed(test, stream, -9.765615, 6U, "-9.765615", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.765625, 6U, "-9.765625", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.765635, 6U, "-9.765635", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.765645, 6U, "-9.765645", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.765655, 6U, "-9.765655", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.765665, 6U, "-9.765665", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.765675, 6U, "-9.765675", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.765685, 6U, "-9.765685", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.765695, 6U, "-9.765695", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.9765615, 6U, "-0.976561", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.9765625, 6U, "-0.976562", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.9765635, 6U, "-0.976564", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.9765645, 6U, "-0.976564", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.9765655, 6U, "-0.976565", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.9765665, 6U, "-0.976567", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.9765675, 6U, "-0.976568", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.9765685, 6U, "-0.976568", __LINE__);
    RealToStreamEqualFixed(test, stream, -0.9765695, 6U, "-0.976569", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.765625e-1, 6U, "-0.976562", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.765625e-2, 6U, "-0.097656", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.765625e-3, 6U, "-0.009766", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.765625e-4, 6U, "-0.000977", __LINE__);
    RealToStreamEqualFixed(test, stream, -9.765625e-5, 6U, "-0.000098", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.00001844674407370955161500, 6U, "0.000018", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0001844674407370955161500, 6U, "0.000184", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.001844674407370955161500, 6U, "0.001845", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.001844674407370955161500, 6U, "5.001845", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.01844674407370955161500, 6U, "0.018447", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.1844674407370955161500, 6U, "0.184467", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.05, 6U, "0.050000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.005, 6U, "0.005000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0005, 6U, "0.000500", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.00005, 6U, "0.000050", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.000005, 6U, "0.000005", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.55, 6U, "1.550000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0055, 6U, "1.005500", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0059999, 6U, "1.006000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0099999, 6U, "1.010000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.00055, 6U, "0.000550", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.00055, 6U, "1.000550", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.50009, 6U, "0.500090", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.000000000050000000005, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0000000000005, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0000000000000000000000000005, 6U, "0.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.3125, 6U, "5.312500", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.3, 6U, "5.300000", __LINE__);
    RealToStreamEqualFixed(test, stream, 5.5, 6U, "5.500000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.55, 6U, "0.550000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.25, 6U, "0.250000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.75, 6U, "0.750000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.125, 6U, "0.125000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.625, 6U, "0.625000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.3125, 6U, "0.312500", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.95367431640625, 6U, "0.953674", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.298023223876953125, 6U, "0.298023", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.1490116119384765625, 6U, "0.149012", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.7450580596923828125, 6U, "0.745058", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.37252902984619140625, 6U, "0.372529", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.186264514923095703125, 6U, "0.186265", __LINE__);

    RealToStreamEqualFixed(test, stream, 1.12345, 6U, "1.123450", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.953674, 6U, "1.953674", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0953674, 6U, "1.095367", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.00953674, 6U, "1.009537", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.000953674, 6U, "1.000954", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.953674, 6U, "9.953674", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.0953674, 6U, "9.095367", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.00953674, 6U, "9.009537", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.000953674, 6U, "9.000954", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.953674, 6U, "0.953674", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0953674, 6U, "0.095367", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.00953674, 6U, "0.009537", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.000953674, 6U, "0.000954", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.123455, 6U, "0.123455", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.1123405, 6U, "0.112340", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.1123095, 6U, "0.112310", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.78126, 6U, "0.781260", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.300048828125, 6U, "0.300049", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.111, 6U, "0.111000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.11111, 6U, "0.111110", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.1111111111111111, 6U, "0.111111", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.1111111111111115, 6U, "0.111111", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.11111111111111159, 6U, "0.111111", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.1111111111111116, 6U, "0.111111", __LINE__);

    RealToStreamEqualFixed(test, stream, 105.625, 6U, "105.625000", __LINE__);
    RealToStreamEqualFixed(test, stream, 25e20, 6U, "2500000000000000000000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.5e21, 6U, "2500000000000000000000.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 3814697265625.0, 6U, "3814697265625.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 381469725.0, 6U, "381469725.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.3814697265625, 6U, "0.381470", __LINE__);
    RealToStreamEqualFixed(test, stream, 10.5, 6U, "10.500000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.3333, 6U, "0.333300", __LINE__);

    RealToStreamEqualFixed(test, stream, 4554534.0, 6U, "4554534.000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 854534.055, 6U, "854534.055000", __LINE__);
    RealToStreamEqualFixed(test, stream, 854534.0055, 6U, "854534.005500", __LINE__);
    RealToStreamEqualFixed(test, stream, 854534.000055, 6U, "854534.000055", __LINE__);
    RealToStreamEqualFixed(test, stream, 854534.0000005, 14U, "854534.00000050000381", __LINE__);
    RealToStreamEqualFixed(test, stream, 8.0000005, 6U, "8.000001", __LINE__);
    RealToStreamEqualFixed(test, stream, 8.000005, 6U, "8.000005", __LINE__);
    RealToStreamEqualFixed(test, stream, 8.005, 6U, "8.005000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0099999, 6U, "0.010000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0099999, 6U, "1.010000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.0999999, 6U, "0.100000", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.0999999, 6U, "1.100000", __LINE__);

    RealToStreamEqualFixed(test, stream, 5.55, 1U, "5.5", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.996, 5U, "9.99600", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.996, 4U, "9.9960", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.996, 3U, "9.996", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.996, 2U, "10.00", __LINE__);
    RealToStreamEqualFixed(test, stream, 9.996, 1U, "10.0", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.5, 1U, "0.5", __LINE__);
    RealToStreamEqualFixed(test, stream, 1.5, 1U, "1.5", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.5, 1U, "2.5", __LINE__);
    RealToStreamEqualFixed(test, stream, 3.5, 1U, "3.5", __LINE__);
    RealToStreamEqualFixed(test, stream, 4.5, 1U, "4.5", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.25, 1U, "2.2", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.75, 1U, "2.8", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.05, 1U, "0.1", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.15, 1U, "0.1", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.25, 1U, "0.2", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.35, 1U, "0.3", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.45, 1U, "0.5", __LINE__);

    RealToStreamEqualFixed(test, stream, 2.626, 2U, "2.63", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.66, 2U, "2.66", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.26, 2U, "2.26", __LINE__);
    RealToStreamEqualFixed(test, stream, 3.266, 2U, "3.27", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.626, 2U, "2.63", __LINE__);

    RealToStreamEqualFixed(test, stream, 2.635, 2U, "2.63", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.636, 2U, "2.64", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.36, 2U, "2.36", __LINE__);
    RealToStreamEqualFixed(test, stream, 3.35, 2U, "3.35", __LINE__);
    RealToStreamEqualFixed(test, stream, 2.635, 2U, "2.63", __LINE__);

    RealToStreamEqualFixed(test, stream, 95.0, 1U, "95.0", __LINE__);
    RealToStreamEqualFixed(test, stream, 95.11, 1U, "95.1", __LINE__);
    RealToStreamEqualFixed(test, stream, 95.99, 1U, "96.0", __LINE__);

    RealToStreamEqualFixed(test, stream, 95000000.0, 1U, "95000000.0", __LINE__);

    RealToStreamEqualFixed(test, stream, 3.0, 15U, "3.000000000000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 3.14, 15U, "3.140000000000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 140737488355327.953674, 15U, "140737488355327.953125000000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 140737488355327.015625, 15U, "140737488355327.015625000000000", __LINE__);

    RealToStreamEqualFixed(test, stream, 140737458355327.953674, 15U, "140737458355327.953125000000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 140737488355327.953674, 6U, "140737488355327.953125", __LINE__);

    RealToStreamEqualFixed(test, stream, 0.95367431640625, 20U, "0.95367431640625000000", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.298023223876953125, 20U, "0.29802322387695312500", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.1490116119384765625, 20U, "0.14901161193847656250", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.7450580596923828125, 20U, "0.74505805969238281250", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.37252902984619140625, 20U, "0.37252902984619140625", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.186264514923095703125, 20U, "0.18626451492309570312", __LINE__);
    RealToStreamEqualFixed(test, stream, 0.186264514923095703125, 21U, "0.186264514923095703125", __LINE__);
}

static void TestFloatToString1(QTest &test, StringStream<char> &stream) {
    RealToStreamEqual(test, stream, -0.0F, 6U, "-0", __LINE__);
    RealToStreamEqual(test, stream, 0.0F, 6U, "0", __LINE__);

    RealToStreamEqual(test, stream, QNumber32{2139095040U}.Real, 6U, "inf", __LINE__);
    RealToStreamEqual(test, stream, QNumber32{4286578688U}.Real, 6U, "-inf", __LINE__);

#ifndef QENTEM_COMPARE_DIGIT_WITH_STL
    RealToStreamEqual(test, stream, QNumber32{4290772992U}.Real, 6U, "nan", __LINE__);
#endif

    RealToStreamEqual(test, stream, 1.0F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 2.0F, 6U, "2", __LINE__);
    RealToStreamEqual(test, stream, 3.0F, 6U, "3", __LINE__);
    RealToStreamEqual(test, stream, 4.0F, 6U, "4", __LINE__);
    RealToStreamEqual(test, stream, 5.0F, 6U, "5", __LINE__);
    RealToStreamEqual(test, stream, 6.0F, 6U, "6", __LINE__);
    RealToStreamEqual(test, stream, 7.0F, 6U, "7", __LINE__);
    RealToStreamEqual(test, stream, 8.0F, 6U, "8", __LINE__);
    RealToStreamEqual(test, stream, 9.0F, 6U, "9", __LINE__);
    RealToStreamEqual(test, stream, 220.0F, 6U, "220", __LINE__);
    RealToStreamEqual(test, stream, 0.1F, 6U, "0.1", __LINE__);
    RealToStreamEqual(test, stream, 0.2F, 6U, "0.2", __LINE__);
    RealToStreamEqual(test, stream, 0.3F, 6U, "0.3", __LINE__);
    RealToStreamEqual(test, stream, 0.4F, 6U, "0.4", __LINE__);
    RealToStreamEqual(test, stream, 0.5F, 6U, "0.5", __LINE__);
    RealToStreamEqual(test, stream, 0.6F, 6U, "0.6", __LINE__);
    RealToStreamEqual(test, stream, 0.7F, 6U, "0.7", __LINE__);
    RealToStreamEqual(test, stream, 0.8F, 6U, "0.8", __LINE__);
    RealToStreamEqual(test, stream, 0.9F, 6U, "0.9", __LINE__);

    RealToStreamEqual(test, stream, 0.09F, 6U, "0.09", __LINE__);
    RealToStreamEqual(test, stream, 0.009F, 6U, "0.009", __LINE__);
    RealToStreamEqual(test, stream, 0.0009F, 6U, "0.0009", __LINE__);
    RealToStreamEqual(test, stream, 0.00009F, 6U, "9e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.000009F, 6U, "9e-06", __LINE__);
    RealToStreamEqual(test, stream, 0.0000009F, 6U, "9e-07", __LINE__);
    RealToStreamEqual(test, stream, 0.00000009F, 6U, "9e-08", __LINE__);
    RealToStreamEqual(test, stream, 0.000000009F, 6U, "9e-09", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000009F, 6U, "9e-10", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000009F, 6U, "9e-11", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000009F, 6U, "9e-12", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000009F, 6U, "9e-13", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000000009F, 6U, "9e-14", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000000009F, 6U, "9e-15", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000000009F, 6U, "9e-16", __LINE__);

    RealToStreamEqual(test, stream, 0.01F, 6U, "0.01", __LINE__);
    RealToStreamEqual(test, stream, 0.001F, 6U, "0.001", __LINE__);
    RealToStreamEqual(test, stream, 0.0001F, 6U, "0.0001", __LINE__);
    RealToStreamEqual(test, stream, 0.00001F, 6U, "1e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.000001F, 6U, "1e-06", __LINE__);
    RealToStreamEqual(test, stream, 0.0000001F, 6U, "1e-07", __LINE__);
    RealToStreamEqual(test, stream, 0.00000001F, 6U, "1e-08", __LINE__);
    RealToStreamEqual(test, stream, 0.000000001F, 6U, "1e-09", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000001F, 6U, "1e-10", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000001F, 6U, "1e-11", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000001F, 6U, "1e-12", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000001F, 6U, "1e-13", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000000001F, 6U, "1e-14", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000000001F, 6U, "1e-15", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000000001F, 6U, "1e-16", __LINE__);

    RealToStreamEqual(test, stream, 0.02F, 6U, "0.02", __LINE__);
    RealToStreamEqual(test, stream, 0.002F, 6U, "0.002", __LINE__);
    RealToStreamEqual(test, stream, 0.0002F, 6U, "0.0002", __LINE__);
    RealToStreamEqual(test, stream, 0.00002F, 6U, "2e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.000002F, 6U, "2e-06", __LINE__);
    RealToStreamEqual(test, stream, 0.0000002F, 6U, "2e-07", __LINE__);
    RealToStreamEqual(test, stream, 0.00000002F, 6U, "2e-08", __LINE__);
    RealToStreamEqual(test, stream, 0.000000002F, 6U, "2e-09", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000002F, 6U, "2e-10", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000002F, 6U, "2e-11", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000002F, 6U, "2e-12", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000002F, 6U, "2e-13", __LINE__);
    RealToStreamEqual(test, stream, 0.00000000000002F, 6U, "2e-14", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000000002F, 6U, "2e-15", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000000002F, 6U, "2e-16", __LINE__);

    RealToStreamEqual(test, stream, 1.5F, 6U, "1.5", __LINE__);
    RealToStreamEqual(test, stream, 1.05F, 6U, "1.05", __LINE__);
    RealToStreamEqual(test, stream, 1.005F, 6U, "1.005", __LINE__);
    RealToStreamEqual(test, stream, 1.0005F, 6U, "1.0005", __LINE__);
    RealToStreamEqual(test, stream, 1.00005F, 6U, "1.00005", __LINE__);
    RealToStreamEqual(test, stream, 1.000005F, 6U, "1.00001", __LINE__);
    RealToStreamEqual(test, stream, 1.0000005F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000005F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000005F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000005F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000005F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000005F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000005F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000000005F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000000005F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000000005F, 6U, "1", __LINE__);

    RealToStreamEqual(test, stream, 1.9F, 6U, "1.9", __LINE__);
    RealToStreamEqual(test, stream, 1.09F, 6U, "1.09", __LINE__);
    RealToStreamEqual(test, stream, 1.009F, 6U, "1.009", __LINE__);
    RealToStreamEqual(test, stream, 1.0009F, 6U, "1.0009", __LINE__);
    RealToStreamEqual(test, stream, 1.00009F, 6U, "1.00009", __LINE__);
    RealToStreamEqual(test, stream, 1.000009F, 6U, "1.00001", __LINE__);
    RealToStreamEqual(test, stream, 1.0000009F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000009F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000009F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000009F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000009F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000009F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000009F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000000009F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000000009F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000000009F, 6U, "1", __LINE__);

    RealToStreamEqual(test, stream, 1.1F, 6U, "1.1", __LINE__);
    RealToStreamEqual(test, stream, 1.01F, 6U, "1.01", __LINE__);
    RealToStreamEqual(test, stream, 1.001F, 6U, "1.001", __LINE__);
    RealToStreamEqual(test, stream, 1.0001F, 6U, "1.0001", __LINE__);
    RealToStreamEqual(test, stream, 1.00001F, 6U, "1.00001", __LINE__);
    RealToStreamEqual(test, stream, 1.000001F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000001F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000001F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000001F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000001F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000001F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000001F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.0000000000001F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.00000000000001F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.000000000000001F, 6U, "1", __LINE__);
}

static void TestFloatToString2(QTest &test, StringStream<char> &stream) {
    RealToStreamEqual(test, stream, 0.1000000000000006F, 6U, "0.1", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000000011F, 6U, "1.1e-14", __LINE__);

    RealToStreamEqual(test, stream, 0.02F, 6U, "0.02", __LINE__);
    RealToStreamEqual(test, stream, 0.002F, 6U, "0.002", __LINE__);
    RealToStreamEqual(test, stream, 0.0002F, 6U, "0.0002", __LINE__);

    RealToStreamEqual(test, stream, 123456789.0F, 6U, "1.23457e+08", __LINE__);
    RealToStreamEqual(test, stream, 2147483647.0F, 6U, "2.14748e+09", __LINE__);
    RealToStreamEqual(test, stream, 4294967295.0F, 6U, "4.29497e+09", __LINE__);
    RealToStreamEqual(test, stream, 4294967295.625F, 6U, "4.29497e+09", __LINE__);
    RealToStreamEqual(test, stream, 2251799813685247.0F, 6U, "2.2518e+15", __LINE__);
    RealToStreamEqual(test, stream, 4503599627370495.0F, 6U, "4.5036e+15", __LINE__);
    RealToStreamEqual(test, stream, 4611686018427387903.0F, 6U, "4.61169e+18", __LINE__);
    RealToStreamEqual(test, stream, 9223372036854775806.0F, 6U, "9.22337e+18", __LINE__);
    RealToStreamEqual(test, stream, 9223372036854775807.0F, 6U, "9.22337e+18", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551615.0F, 6U, "1.84467e+19", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551616.0F, 6U, "1.84467e+19", __LINE__);
    RealToStreamEqual(test, stream, 34028236692093846341.0F, 6U, "3.40282e+19", __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463412.0F, 6U, "3.40282e+20", __LINE__);
    RealToStreamEqual(test, stream, 3402823669209384634633746074317.0F, 6U, "3.40282e+30", __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463426481119284341.0F, 6U, "3.40282e+32", __LINE__);
    RealToStreamEqual(test, stream, 3402823669209384634264811192843414.0F, 6U, "3.40282e+33", __LINE__);
    RealToStreamEqual(test, stream, 3689348814741910333.0F, 6U, "3.68935e+18", __LINE__);
    RealToStreamEqual(test, stream, 36893488147419103230.0F, 6U, "3.68935e+19", __LINE__);
    RealToStreamEqual(test, stream, 36893488147419103333.0F, 6U, "3.68935e+19", __LINE__);
    RealToStreamEqual(test, stream, 368934881474191033333.0F, 6U, "3.68935e+20", __LINE__);
    RealToStreamEqual(test, stream, 9007199254740992.0F, 6U, "9.0072e+15", __LINE__);
    RealToStreamEqual(test, stream, 18014398509481984.0F, 6U, "1.80144e+16", __LINE__);
    RealToStreamEqual(test, stream, 36028797018963968.0F, 6U, "3.60288e+16", __LINE__);

    RealToStreamEqual(test, stream, 0.9000000000000006F, 6U, "0.9", __LINE__);

    RealToStreamEqual(test, stream, 9.999999999999999F, 6U, "10", __LINE__);
    RealToStreamEqual(test, stream, 1999999.0F, 6U, "2e+06", __LINE__);
    RealToStreamEqual(test, stream, 9999999999999999.0F, 6U, "1e+16", __LINE__);
    RealToStreamEqual(test, stream, 999999999999999999.0F, 6U, "1e+18", __LINE__);
    RealToStreamEqual(test, stream, 9999999999999999999.0F, 6U, "1e+19", __LINE__);
    RealToStreamEqual(test, stream, 99999999999999999999.0F, 6U, "1e+20", __LINE__);
    RealToStreamEqual(test, stream, 99e10F, 6U, "9.9e+11", __LINE__);

    RealToStreamEqual(test, stream, 0.999999999F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999999F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.999F, 6U, "0.999", __LINE__);
    RealToStreamEqual(test, stream, 0.999999F, 6U, "0.999999", __LINE__);
    RealToStreamEqual(test, stream, 0.999999999999999F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999999F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999995F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999959F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999999999996F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999999999959F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999999999999F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999999999999999999999999999F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999999999999999999999999999999999999F, 6U, "1", __LINE__);

    RealToStreamEqual(test, stream, 10000005.0F, 6U, "1e+07", __LINE__);
    RealToStreamEqual(test, stream, 10000005.0F, 7U, "1e+07", __LINE__);
    RealToStreamEqual(test, stream, 10000006.0F, 7U, "1.000001e+07", __LINE__);

    RealToStreamEqual(test, stream, 10000000.0F, 6U, "1e+07", __LINE__);
    RealToStreamEqual(test, stream, 9999999.0F, 6U, "1e+07", __LINE__);
    RealToStreamEqual(test, stream, 19999999.0F, 6U, "2e+07", __LINE__);
    RealToStreamEqual(test, stream, 29999999.0F, 6U, "3e+07", __LINE__);
    RealToStreamEqual(test, stream, 39999999.0F, 6U, "4e+07", __LINE__);
    RealToStreamEqual(test, stream, 49999999.0F, 6U, "5e+07", __LINE__);
    RealToStreamEqual(test, stream, 59999999.0F, 6U, "6e+07", __LINE__);
    RealToStreamEqual(test, stream, 69999999.0F, 6U, "7e+07", __LINE__);
    RealToStreamEqual(test, stream, 79999999.0F, 6U, "8e+07", __LINE__);
    RealToStreamEqual(test, stream, 89999999.0F, 6U, "9e+07", __LINE__);
    RealToStreamEqual(test, stream, 99999999.0F, 6U, "1e+08", __LINE__);
    RealToStreamEqual(test, stream, 999999999.0F, 6U, "1e+09", __LINE__);

    RealToStreamEqual(test, stream, 0.10000000F, 6U, "0.1", __LINE__);
    RealToStreamEqual(test, stream, 0.9999999F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 0.19999999F, 6U, "0.2", __LINE__);
    RealToStreamEqual(test, stream, 0.29999999F, 6U, "0.3", __LINE__);
    RealToStreamEqual(test, stream, 0.39999999F, 6U, "0.4", __LINE__);
    RealToStreamEqual(test, stream, 0.49999999F, 6U, "0.5", __LINE__);
    RealToStreamEqual(test, stream, 0.59999999F, 6U, "0.6", __LINE__);
    RealToStreamEqual(test, stream, 0.69999999F, 6U, "0.7", __LINE__);
    RealToStreamEqual(test, stream, 0.79999999F, 6U, "0.8", __LINE__);
    RealToStreamEqual(test, stream, 0.89999999F, 6U, "0.9", __LINE__);
    RealToStreamEqual(test, stream, 0.99999999F, 6U, "1", __LINE__);
    RealToStreamEqual(test, stream, 1.99999999F, 6U, "2", __LINE__);
    RealToStreamEqual(test, stream, 2.99999999F, 6U, "3", __LINE__);
    RealToStreamEqual(test, stream, 3.99999999F, 6U, "4", __LINE__);
    RealToStreamEqual(test, stream, 4.99999999F, 6U, "5", __LINE__);
    RealToStreamEqual(test, stream, 5.99999999F, 6U, "6", __LINE__);
    RealToStreamEqual(test, stream, 6.99999999F, 6U, "7", __LINE__);
    RealToStreamEqual(test, stream, 7.99999999F, 6U, "8", __LINE__);
    RealToStreamEqual(test, stream, 8.99999999F, 6U, "9", __LINE__);
    RealToStreamEqual(test, stream, 9.99999999F, 6U, "10", __LINE__);
    RealToStreamEqual(test, stream, 99.99999999F, 6U, "100", __LINE__);
    RealToStreamEqual(test, stream, 999.99999999F, 6U, "1000", __LINE__);
    RealToStreamEqual(test, stream, 99999.89999999F, 6U, "99999.9", __LINE__);
    RealToStreamEqual(test, stream, 99999.99999999F, 6U, "100000", __LINE__);
    RealToStreamEqual(test, stream, 999999.99999999F, 6U, "1e+06", __LINE__);

    RealToStreamEqual(test, stream, 1e1F, 6U, "10", __LINE__);
    RealToStreamEqual(test, stream, 1e2F, 6U, "100", __LINE__);
    RealToStreamEqual(test, stream, 1e3F, 6U, "1000", __LINE__);
    RealToStreamEqual(test, stream, 1e4F, 6U, "10000", __LINE__);
    RealToStreamEqual(test, stream, 1e5F, 6U, "100000", __LINE__);
    RealToStreamEqual(test, stream, 1e10F, 6U, "1e+10", __LINE__);
    RealToStreamEqual(test, stream, 1e13F, 6U, "1e+13", __LINE__);
    RealToStreamEqual(test, stream, 1e14F, 6U, "1e+14", __LINE__);
    RealToStreamEqual(test, stream, 1e15F, 6U, "1e+15", __LINE__);

    RealToStreamEqual(test, stream, 1e-1F, 6U, "0.1", __LINE__);
    RealToStreamEqual(test, stream, 1e-2F, 6U, "0.01", __LINE__);
    RealToStreamEqual(test, stream, 1e-3F, 6U, "0.001", __LINE__);
    RealToStreamEqual(test, stream, 1e-4F, 6U, "0.0001", __LINE__);
    RealToStreamEqual(test, stream, 1e-5F, 6U, "1e-05", __LINE__);
    RealToStreamEqual(test, stream, 1e-10F, 6U, "1e-10", __LINE__);
    RealToStreamEqual(test, stream, 1e-13F, 6U, "1e-13", __LINE__);
    RealToStreamEqual(test, stream, 1e-14F, 6U, "1e-14", __LINE__);
    RealToStreamEqual(test, stream, 1e-15F, 6U, "1e-15", __LINE__);

    RealToStreamEqual(test, stream, 1.40129846e-45F, 6U, "1.4013e-45", __LINE__);
    RealToStreamEqual(test, stream, 3.40282347e+38F, 6U, "3.40282e+38", __LINE__);
    RealToStreamEqual(test, stream, -1.40129846e-45F, 6U, "-1.4013e-45", __LINE__);
    RealToStreamEqual(test, stream, -3.40282347e+38F, 6U, "-3.40282e+38", __LINE__);

    RealToStreamEqual(test, stream, 1.40129846e-45F, 45U, "1.40129846432481707092372958328991613128026194e-45",
                      __LINE__);
    RealToStreamEqual(test, stream, -3.40282347e+38F, 39U, "-340282346638528859811704183484516925440", __LINE__);
    RealToStreamEqual(test, stream, -3.40282347e+38F, 38U, "-3.4028234663852885981170418348451692544e+38", __LINE__);
}

static void TestFloatToString3(QTest &test, StringStream<char> &stream) {
    RealToStreamEqual(test, stream, -854534.5F, 6U, "-854534", __LINE__);
    RealToStreamEqual(test, stream, -854534.05F, 6U, "-854534", __LINE__);
    RealToStreamEqual(test, stream, 854534.5F, 6U, "854534", __LINE__);
    RealToStreamEqual(test, stream, 854534.05F, 6U, "854534", __LINE__);
    RealToStreamEqual(test, stream, -9765625.0F, 6U, "-9.76562e+06", __LINE__);
    RealToStreamEqual(test, stream, -0.2765625F, 6U, "-0.276563", __LINE__);

    RealToStreamEqual(test, stream, 9765625.0F, 6U, "9.76562e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765645.0F, 6U, "9.76564e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765665.0F, 6U, "9.76566e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765685.0F, 6U, "9.76568e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765605.0F, 6U, "9.7656e+06", __LINE__);

    RealToStreamEqual(test, stream, 9765615.0F, 6U, "9.76562e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765635.0F, 6U, "9.76564e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765655.0F, 6U, "9.76566e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765675.0F, 6U, "9.76568e+06", __LINE__);
    RealToStreamEqual(test, stream, 9765695.0F, 6U, "9.7657e+06", __LINE__);

    RealToStreamEqual(test, stream, 9999425.0F, 6U, "9.99942e+06", __LINE__);
    RealToStreamEqual(test, stream, 9999445.0F, 6U, "9.99944e+06", __LINE__);
    RealToStreamEqual(test, stream, 9999465.0F, 6U, "9.99946e+06", __LINE__);
    RealToStreamEqual(test, stream, 9999485.0F, 6U, "9.99948e+06", __LINE__);
    RealToStreamEqual(test, stream, 9999405.0F, 6U, "9.9994e+06", __LINE__);

    RealToStreamEqual(test, stream, 9.999925F, 6U, "9.99992", __LINE__);
    RealToStreamEqual(test, stream, 9.999945F, 6U, "9.99994", __LINE__);
    RealToStreamEqual(test, stream, 9.999965F, 6U, "9.99996", __LINE__);
    RealToStreamEqual(test, stream, 9.999985F, 6U, "9.99998", __LINE__);
    RealToStreamEqual(test, stream, 9.999905F, 6U, "9.9999", __LINE__);

    RealToStreamEqual(test, stream, 1.765625F, 6U, "1.76562", __LINE__);
    RealToStreamEqual(test, stream, 0.1765625F, 6U, "0.176563", __LINE__);
    RealToStreamEqual(test, stream, 0.2765625F, 6U, "0.276563", __LINE__);
    RealToStreamEqual(test, stream, 0.3765625F, 6U, "0.376563", __LINE__);
    RealToStreamEqual(test, stream, 0.4765625F, 6U, "0.476562", __LINE__);
    RealToStreamEqual(test, stream, 0.5765625F, 6U, "0.576563", __LINE__);
    RealToStreamEqual(test, stream, 0.6765625F, 6U, "0.676562", __LINE__);
    RealToStreamEqual(test, stream, 0.7765625F, 6U, "0.776563", __LINE__);
    RealToStreamEqual(test, stream, 0.8765625F, 6U, "0.876562", __LINE__);
    RealToStreamEqual(test, stream, 0.9765625F, 6U, "0.976562", __LINE__);

    RealToStreamEqual(test, stream, 2.165625F, 6U, "2.16563", __LINE__);
    RealToStreamEqual(test, stream, 2.265625F, 6U, "2.26562", __LINE__);
    RealToStreamEqual(test, stream, 2.365625F, 6U, "2.36562", __LINE__);
    RealToStreamEqual(test, stream, 2.465625F, 6U, "2.46563", __LINE__);
    RealToStreamEqual(test, stream, 2.565625F, 6U, "2.56562", __LINE__);
    RealToStreamEqual(test, stream, 2.665625F, 6U, "2.66563", __LINE__);
    RealToStreamEqual(test, stream, 2.765625F, 6U, "2.76562", __LINE__);
    RealToStreamEqual(test, stream, 2.865625F, 6U, "2.86562", __LINE__);
    RealToStreamEqual(test, stream, 2.965625F, 6U, "2.96563", __LINE__);

    RealToStreamEqual(test, stream, 5.165625F, 6U, "5.16563", __LINE__);
    RealToStreamEqual(test, stream, 5.265625F, 6U, "5.26562", __LINE__);
    RealToStreamEqual(test, stream, 5.365625F, 6U, "5.36562", __LINE__);
    RealToStreamEqual(test, stream, 5.465625F, 6U, "5.46562", __LINE__);
    RealToStreamEqual(test, stream, 5.565625F, 6U, "5.56563", __LINE__);
    RealToStreamEqual(test, stream, 5.665625F, 6U, "5.66563", __LINE__);
    RealToStreamEqual(test, stream, 5.765625F, 6U, "5.76562", __LINE__);
    RealToStreamEqual(test, stream, 5.865625F, 6U, "5.86562", __LINE__);
    RealToStreamEqual(test, stream, 5.965625F, 6U, "5.96562", __LINE__);

    RealToStreamEqual(test, stream, 9.765615F, 6U, "9.76562", __LINE__);
    RealToStreamEqual(test, stream, 9.765625F, 6U, "9.76562", __LINE__);
    RealToStreamEqual(test, stream, 9.765635F, 6U, "9.76563", __LINE__);
    RealToStreamEqual(test, stream, 9.765645F, 6U, "9.76565", __LINE__);
    RealToStreamEqual(test, stream, 9.765655F, 6U, "9.76565", __LINE__);
    RealToStreamEqual(test, stream, 9.765665F, 6U, "9.76567", __LINE__);
    RealToStreamEqual(test, stream, 9.765675F, 6U, "9.76567", __LINE__);
    RealToStreamEqual(test, stream, 9.765685F, 6U, "9.76569", __LINE__);
    RealToStreamEqual(test, stream, 9.765695F, 6U, "9.76569", __LINE__);
    RealToStreamEqual(test, stream, 0.9765615F, 6U, "0.976561", __LINE__);
    RealToStreamEqual(test, stream, 0.9765625F, 6U, "0.976562", __LINE__);
    RealToStreamEqual(test, stream, 0.9765635F, 6U, "0.976564", __LINE__);
    RealToStreamEqual(test, stream, 0.9765645F, 6U, "0.976565", __LINE__);
    RealToStreamEqual(test, stream, 0.9765655F, 6U, "0.976565", __LINE__);
    RealToStreamEqual(test, stream, 0.9765665F, 6U, "0.976566", __LINE__);
    RealToStreamEqual(test, stream, 0.9765675F, 6U, "0.976568", __LINE__);
    RealToStreamEqual(test, stream, 0.9765685F, 6U, "0.976569", __LINE__);
    RealToStreamEqual(test, stream, 0.9765695F, 6U, "0.976569", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-1F, 6U, "0.976562", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-2F, 6U, "0.0976562", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-3F, 6U, "0.00976562", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-4F, 6U, "0.000976562", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-5F, 6U, "9.76563e-05", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-10F, 6U, "9.76562e-10", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-13F, 6U, "9.76562e-13", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-14F, 6U, "9.76563e-14", __LINE__);
    RealToStreamEqual(test, stream, 9.765625e-15F, 6U, "9.76562e-15", __LINE__);

    RealToStreamEqual(test, stream, 0.00001844674407370955161500F, 6U, "1.84467e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.0001844674407370955161500F, 6U, "0.000184467", __LINE__);
    RealToStreamEqual(test, stream, 0.001844674407370955161500F, 6U, "0.00184467", __LINE__);
    RealToStreamEqual(test, stream, 5.001844674407370955161500F, 6U, "5.00184", __LINE__);
    RealToStreamEqual(test, stream, 0.01844674407370955161500F, 6U, "0.0184467", __LINE__);
    RealToStreamEqual(test, stream, 0.1844674407370955161500F, 6U, "0.184467", __LINE__);

    RealToStreamEqual(test, stream, 18446744073709551614.0F, 6U, "1.84467e+19", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551620.0F, 6U, "1.84467e+19", __LINE__);
    RealToStreamEqual(test, stream, 184467440737095516150.0F, 6U, "1.84467e+20", __LINE__);
    RealToStreamEqual(test, stream, 1844674407370955161500.0F, 6U, "1.84467e+21", __LINE__);
    RealToStreamEqual(test, stream, 17446744073709551614.0F, 6U, "1.74467e+19", __LINE__);
    RealToStreamEqual(test, stream, 9923372036854775809.0F, 6U, "9.92337e+18", __LINE__);

    RealToStreamEqual(test, stream, 0.05F, 6U, "0.05", __LINE__);
    RealToStreamEqual(test, stream, 0.005F, 6U, "0.005", __LINE__);
    RealToStreamEqual(test, stream, 0.0005F, 6U, "0.0005", __LINE__);
    RealToStreamEqual(test, stream, 0.00005F, 6U, "5e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.000005F, 6U, "5e-06", __LINE__);
    RealToStreamEqual(test, stream, 1.55F, 6U, "1.55", __LINE__);
    RealToStreamEqual(test, stream, 1.0055F, 6U, "1.0055", __LINE__);
    RealToStreamEqual(test, stream, 1.005999F, 6U, "1.006", __LINE__);
    RealToStreamEqual(test, stream, 1.009999F, 6U, "1.01", __LINE__);
    RealToStreamEqual(test, stream, 0.00055F, 6U, "0.00055", __LINE__);
    RealToStreamEqual(test, stream, 1.00055F, 6U, "1.00055", __LINE__);
    RealToStreamEqual(test, stream, 0.50009F, 6U, "0.50009", __LINE__);
    RealToStreamEqual(test, stream, 0.000000000050000000005F, 6U, "5e-11", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000005F, 6U, "5e-13", __LINE__);
    RealToStreamEqual(test, stream, 0.0000000000000000000000000005F, 6U, "5e-28", __LINE__);
    RealToStreamEqual(test, stream, 5.3125F, 6U, "5.3125", __LINE__);
    RealToStreamEqual(test, stream, 5.3F, 6U, "5.3", __LINE__);
    RealToStreamEqual(test, stream, 5.5F, 6U, "5.5", __LINE__);
    RealToStreamEqual(test, stream, 0.55F, 6U, "0.55", __LINE__);
    RealToStreamEqual(test, stream, 0.25F, 6U, "0.25", __LINE__);
    RealToStreamEqual(test, stream, 0.75F, 6U, "0.75", __LINE__);
    RealToStreamEqual(test, stream, 0.125F, 6U, "0.125", __LINE__);
    RealToStreamEqual(test, stream, 0.625F, 6U, "0.625", __LINE__);
    RealToStreamEqual(test, stream, 0.3125F, 6U, "0.3125", __LINE__);
    RealToStreamEqual(test, stream, 0.95367431640625F, 6U, "0.953674", __LINE__);
    RealToStreamEqual(test, stream, 0.298023223876953125F, 6U, "0.298023", __LINE__);
    RealToStreamEqual(test, stream, 0.1490116119384765625F, 6U, "0.149012", __LINE__);
    RealToStreamEqual(test, stream, 0.7450580596923828125F, 6U, "0.745058", __LINE__);
    RealToStreamEqual(test, stream, 0.37252902984619140625F, 6U, "0.372529", __LINE__);
    RealToStreamEqual(test, stream, 0.186264514923095703125F, 6U, "0.186265", __LINE__);
}

static void TestFloatToString4(QTest &test, StringStream<char> &stream) {
    RealToStreamEqual(test, stream, 1.12345F, 6U, "1.12345", __LINE__);
    RealToStreamEqual(test, stream, 1.953674F, 6U, "1.95367", __LINE__);
    RealToStreamEqual(test, stream, 1.0953674F, 6U, "1.09537", __LINE__);
    RealToStreamEqual(test, stream, 1.00953674F, 6U, "1.00954", __LINE__);
    RealToStreamEqual(test, stream, 1.000953674F, 6U, "1.00095", __LINE__);
    RealToStreamEqual(test, stream, 9.953674F, 6U, "9.95367", __LINE__);
    RealToStreamEqual(test, stream, 9.0953674F, 6U, "9.09537", __LINE__);
    RealToStreamEqual(test, stream, 9.00953674F, 6U, "9.00954", __LINE__);
    RealToStreamEqual(test, stream, 9.000953674F, 6U, "9.00095", __LINE__);

    RealToStreamEqual(test, stream, 0.953674F, 6U, "0.953674", __LINE__);
    RealToStreamEqual(test, stream, 0.0953674F, 6U, "0.0953674", __LINE__);
    RealToStreamEqual(test, stream, 0.00953674F, 6U, "0.00953674", __LINE__);
    RealToStreamEqual(test, stream, 0.000953674F, 6U, "0.000953674", __LINE__);
    RealToStreamEqual(test, stream, 0.123455F, 6U, "0.123455", __LINE__);
    RealToStreamEqual(test, stream, 0.1123405F, 6U, "0.112341", __LINE__);
    RealToStreamEqual(test, stream, 0.1123095F, 6U, "0.11231", __LINE__);
    RealToStreamEqual(test, stream, 0.78126F, 6U, "0.78126", __LINE__);
    RealToStreamEqual(test, stream, 0.300048828125F, 6U, "0.300049", __LINE__);

    RealToStreamEqual(test, stream, 0.111F, 6U, "0.111", __LINE__);
    RealToStreamEqual(test, stream, 0.11111F, 6U, "0.11111", __LINE__);
    RealToStreamEqual(test, stream, 0.1111111111111111F, 6U, "0.111111", __LINE__);
    RealToStreamEqual(test, stream, 0.1111111111111115F, 6U, "0.111111", __LINE__);
    RealToStreamEqual(test, stream, 0.11111111111111159F, 6U, "0.111111", __LINE__);
    RealToStreamEqual(test, stream, 0.1111111111111116F, 6U, "0.111111", __LINE__);

    RealToStreamEqual(test, stream, 105.625F, 6U, "105.625", __LINE__);
    RealToStreamEqual(test, stream, 25e20F, 6U, "2.5e+21", __LINE__);
    RealToStreamEqual(test, stream, 2.5e21F, 6U, "2.5e+21", __LINE__);
    RealToStreamEqual(test, stream, 3814697265625.0F, 6U, "3.8147e+12", __LINE__);
    RealToStreamEqual(test, stream, 381469725.0F, 6U, "3.8147e+08", __LINE__);
    RealToStreamEqual(test, stream, 0.3814697265625F, 6U, "0.38147", __LINE__);
    RealToStreamEqual(test, stream, 10.5F, 6U, "10.5", __LINE__);
    RealToStreamEqual(test, stream, 10.0F, 6U, "10", __LINE__);

    RealToStreamEqual(test, stream, 2097151.125F, 6U, "2.09715e+06", __LINE__);
    RealToStreamEqual(test, stream, 2097151.0F, 6U, "2.09715e+06", __LINE__);
    RealToStreamEqual(test, stream, 16217975.0F, 6U, "1.6218e+07", __LINE__);
    RealToStreamEqual(test, stream, 32995191.0F, 6U, "3.29952e+07", __LINE__);
    RealToStreamEqual(test, stream, 444444444444444444444.0F, 6U, "4.44444e+20", __LINE__);
    RealToStreamEqual(test, stream, 555555555555555555555.0F, 6U, "5.55556e+20", __LINE__);
    RealToStreamEqual(test, stream, 3668262915145728.0F, 6U, "3.66826e+15", __LINE__);
    RealToStreamEqual(test, stream, 7336525430291456.0F, 6U, "7.33653e+15", __LINE__);
    RealToStreamEqual(test, stream, 7336525830291456.0F, 6U, "7.33653e+15", __LINE__);
    RealToStreamEqual(test, stream, 7336535830291456.0F, 6U, "7.33654e+15", __LINE__);
    RealToStreamEqual(test, stream, 14673051660582912.0F, 6U, "1.46731e+16", __LINE__);

    RealToStreamEqual(test, stream, 1000000.390625F, 6U, "1e+06", __LINE__);
    RealToStreamEqual(test, stream, 1000009.590625F, 6U, "1.00001e+06", __LINE__);
    RealToStreamEqual(test, stream, 1000008.590625F, 6U, "1.00001e+06", __LINE__);
    RealToStreamEqual(test, stream, 7.3365258e-10F, 6U, "7.33653e-10", __LINE__);
    RealToStreamEqual(test, stream, 7.3365258e-10F, 8U, "7.3365258e-10", __LINE__);
    RealToStreamEqual(test, stream, 0.3333F, 6U, "0.3333", __LINE__);

    RealToStreamEqual(test, stream, 4554534.0F, 6U, "4.55453e+06", __LINE__);
    RealToStreamEqual(test, stream, 854534.055F, 6U, "854534", __LINE__);
    RealToStreamEqual(test, stream, 854534.0055F, 6U, "854534", __LINE__);
    RealToStreamEqual(test, stream, 854534.000055F, 6U, "854534", __LINE__);
    RealToStreamEqual(test, stream, 854534.0000005F, 14U, "854534", __LINE__);
    RealToStreamEqual(test, stream, 8.0000005F, 6U, "8", __LINE__);
    RealToStreamEqual(test, stream, 8.000005F, 6U, "8", __LINE__);
    RealToStreamEqual(test, stream, 8.005F, 6U, "8.005", __LINE__);

    RealToStreamEqual(test, stream, 5.55F, 1U, "6", __LINE__);
    RealToStreamEqual(test, stream, 9.996F, 5U, "9.996", __LINE__);
    RealToStreamEqual(test, stream, 9.996F, 4U, "9.996", __LINE__);
    RealToStreamEqual(test, stream, 9.996F, 3U, "10", __LINE__);
    RealToStreamEqual(test, stream, 9.996F, 2U, "10", __LINE__);
    RealToStreamEqual(test, stream, 9.996F, 1U, "1e+01", __LINE__);

    RealToStreamEqual(test, stream, 0.5F, 1U, "0.5", __LINE__);
    RealToStreamEqual(test, stream, 1.5F, 1U, "2", __LINE__);
    RealToStreamEqual(test, stream, 2.5F, 1U, "2", __LINE__);
    RealToStreamEqual(test, stream, 3.5F, 1U, "4", __LINE__);
    RealToStreamEqual(test, stream, 4.5F, 1U, "4", __LINE__);
    RealToStreamEqual(test, stream, 2.25F, 1U, "2", __LINE__);
    RealToStreamEqual(test, stream, 2.75F, 1U, "3", __LINE__);

    RealToStreamEqual(test, stream, 2.625F, 2U, "2.6", __LINE__);
    RealToStreamEqual(test, stream, 2.65F, 2U, "2.7", __LINE__);
    RealToStreamEqual(test, stream, 2.25F, 2U, "2.2", __LINE__);
    RealToStreamEqual(test, stream, 3.25F, 2U, "3.2", __LINE__);
    RealToStreamEqual(test, stream, 2.625F, 2U, "2.6", __LINE__);

    RealToStreamEqual(test, stream, 2.635F, 2U, "2.6", __LINE__);
    RealToStreamEqual(test, stream, 2.35F, 2U, "2.3", __LINE__);
    RealToStreamEqual(test, stream, 3.35F, 2U, "3.3", __LINE__);
    RealToStreamEqual(test, stream, 2.635F, 2U, "2.6", __LINE__);

    RealToStreamEqual(test, stream, 91.0F, 1U, "9e+01", __LINE__);
    RealToStreamEqual(test, stream, 95.0F, 1U, "1e+02", __LINE__);

    RealToStreamEqual(test, stream, 95000000.0F, 1U, "1e+08", __LINE__);

    RealToStreamEqual(test, stream, 3.0F, 15U, "3", __LINE__);
    RealToStreamEqual(test, stream, 3.14F, 15U, "3.14000010490417", __LINE__);

    RealToStreamEqual(test, stream, 140737488355327.953674F, 15U, "140737488355328", __LINE__);
    RealToStreamEqual(test, stream, 140737488355327.015625F, 15U, "140737488355328", __LINE__);

    RealToStreamEqual(test, stream, 140737458355327.953674F, 15U, "140737454800896", __LINE__);
    RealToStreamEqual(test, stream, 140737488355327.953674F, 6U, "1.40737e+14", __LINE__);

    RealToStreamEqual(test, stream, 0.95367431640625F, 20U, "0.95367431640625", __LINE__);
    RealToStreamEqual(test, stream, 0.298023223876953125F, 20U, "0.298023223876953125", __LINE__);
    RealToStreamEqual(test, stream, 0.1490116119384765625F, 20U, "0.1490116119384765625", __LINE__);
    RealToStreamEqual(test, stream, 0.7450580596923828125F, 20U, "0.7450580596923828125", __LINE__);
    RealToStreamEqual(test, stream, 0.37252902984619140625F, 20U, "0.37252902984619140625", __LINE__);
    RealToStreamEqual(test, stream, 0.186264514923095703125F, 20U, "0.18626451492309570312", __LINE__);
    RealToStreamEqual(test, stream, 0.186264514923095703125F, 21U, "0.186264514923095703125", __LINE__);

    RealToStreamEqual(test, stream, 18446744073709551614.0F, 20U, "18446744073709551616", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551620.0F, 20U, "18446744073709551616", __LINE__);
    RealToStreamEqual(test, stream, 184467440737095516150.0F, 20U, "1.8446744073709551616e+20", __LINE__);
    RealToStreamEqual(test, stream, 1844674407370955161500.0F, 20U, "1.8446744073709551616e+21", __LINE__);
    RealToStreamEqual(test, stream, 17446744073709551614.0F, 21U, "17446743745805418496", __LINE__);
    RealToStreamEqual(test, stream, 17446744073709551614.0F, 20U, "17446743745805418496", __LINE__);
    RealToStreamEqual(test, stream, 9923372036854775809.0F, 21U, "9923371716631855104", __LINE__);
    RealToStreamEqual(test, stream, 9923372036854775809.0F, 20U, "9923371716631855104", __LINE__);

    RealToStreamEqual(test, stream, 2251799813685247.0F, 21U, "2251799813685248", __LINE__);
    RealToStreamEqual(test, stream, 4503599627370495.0F, 21U, "4503599627370496", __LINE__);
    RealToStreamEqual(test, stream, 4611686018427387903.0F, 21U, "4611686018427387904", __LINE__);
    RealToStreamEqual(test, stream, 9223372036854775806.0F, 21U, "9223372036854775808", __LINE__);
    RealToStreamEqual(test, stream, 9223372036854775807.0F, 21U, "9223372036854775808", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551615.0F, 21U, "18446744073709551616", __LINE__);
    RealToStreamEqual(test, stream, 18446744073709551616.0F, 21U, "18446744073709551616", __LINE__);
    RealToStreamEqual(test, stream, 34028236692093846341.0F, 21U, "34028235612225536000", __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463412.0F, 21U, "340282382510534426624", __LINE__);
    RealToStreamEqual(test, stream, 3402823669209384634633746074317.0F, 34U, "3402823648528822565846915219456",
                      __LINE__);
    RealToStreamEqual(test, stream, 340282366920938463426481119284341.0F, 34U, "340282366061808076199320696651776",
                      __LINE__);
    RealToStreamEqual(test, stream, 3402823669209384634264811192843414.0F, 34U, "3402823583246828306656939785322496",
                      __LINE__);
}

static void TestFloatToString5(QTest &test, StringStream<char> &stream) {
    RealToStreamEqual(test, stream, -1.0F, 6U, "-1", __LINE__);

    RealToStreamEqual(test, stream, 70.0F, 6U, "70", __LINE__);
    RealToStreamEqual(test, stream, 7e+10F, 6U, "7e+10", __LINE__);
    RealToStreamEqual(test, stream, 75e+10F, 6U, "7.5e+11", __LINE__);
    RealToStreamEqual(test, stream, 7e+30F, 6U, "7e+30", __LINE__);
    RealToStreamEqual(test, stream, 5000000000000000.0F, 6U, "5e+15", __LINE__);

    RealToStreamEqual(test, stream, 9999998.0F, 6U, "1e+07", __LINE__);
    RealToStreamEqual(test, stream, 9999998.9F, 6U, "1e+07", __LINE__);
    RealToStreamEqual(test, stream, 999999999.999999999F, 6U, "1e+09", __LINE__);

    RealToStreamEqual(test, stream, 111.5F, 6U, "111.5", __LINE__);
    RealToStreamEqual(test, stream, 999.625F, 6U, "999.625", __LINE__);
    RealToStreamEqual(test, stream, 1e+33F, 6U, "1e+33", __LINE__);

    RealToStreamEqual(test, stream, 18446744073709551615.0F, 2U, "1.8e+19", __LINE__);
    RealToStreamEqual(test, stream, 0.18446744073709551615F, 2U, "0.18", __LINE__);

    RealToStreamEqual(test, stream, 10.7F, 6U, "10.7", __LINE__);
    RealToStreamEqual(test, stream, 20.7F, 6U, "20.7", __LINE__);
    RealToStreamEqual(test, stream, 30.7F, 6U, "30.7", __LINE__);
    RealToStreamEqual(test, stream, 40.7F, 6U, "40.7", __LINE__);
    RealToStreamEqual(test, stream, 50.7F, 6U, "50.7", __LINE__);
    RealToStreamEqual(test, stream, 60.7F, 6U, "60.7", __LINE__);
    RealToStreamEqual(test, stream, 70.7F, 6U, "70.7", __LINE__);
    RealToStreamEqual(test, stream, 80.7F, 6U, "80.7", __LINE__);
    RealToStreamEqual(test, stream, 90.7F, 6U, "90.7", __LINE__);
    RealToStreamEqual(test, stream, 50.25F, 6U, "50.25", __LINE__);
    RealToStreamEqual(test, stream, 500000000000000.25F, 6U, "5e+14", __LINE__);
    RealToStreamEqual(test, stream, 7.7F, 6U, "7.7", __LINE__);
    RealToStreamEqual(test, stream, 7.00007F, 7U, "7.00007", __LINE__);
    RealToStreamEqual(test, stream, 1.25F, 6U, "1.25", __LINE__);

    RealToStreamEqual(test, stream, 0.07F, 6U, "0.07", __LINE__);
    RealToStreamEqual(test, stream, 0.3125F, 4U, "0.3125", __LINE__);
    RealToStreamEqual(test, stream, 5e-4F, 6U, "0.0005", __LINE__);
    RealToStreamEqual(test, stream, 5e-10F, 6U, "5e-10", __LINE__);
    RealToStreamEqual(test, stream, 7e-24F, 6U, "7e-24", __LINE__);

    RealToStreamEqual(test, stream, 0.390625F, 6U, "0.390625", __LINE__);
    RealToStreamEqual(test, stream, 0.000390625F, 6U, "0.000390625", __LINE__);
    RealToStreamEqual(test, stream, 0.0000390625F, 6U, "3.90625e-05", __LINE__);
    RealToStreamEqual(test, stream, 0.00000390625F, 6U, "3.90625e-06", __LINE__);
    RealToStreamEqual(test, stream, 0.000009999999F, 6U, "1e-05", __LINE__);
    RealToStreamEqual(test, stream, 505.3125F, 6U, "505.312", __LINE__);
    RealToStreamEqual(test, stream, 505.003125F, 6U, "505.003", __LINE__);

#if defined(QENTEM_ENABLE_FLOAT_16) && (QENTEM_ENABLE_FLOAT_16 == 1)
    RealToStreamEqual(test, stream, 15.5F16, 6U, "15.5", __LINE__);
    RealToStreamEqual(test, stream, 65504.0F16, 6U, "65504", __LINE__);
    RealToStreamEqual(test, stream, 5.96046e-8F16, 6U, "5.96046e-08", __LINE__);
#endif
}

static int RunDigitTests() {
    QTest test{"Digit.hpp", __FILE__};

    // test.ContinueOnError(true);
    test.PrintGroupName();

    test.Test("StringToNumber Test 1", TestStringToNumber1);
    test.Test("StringToNumber Test 2", TestStringToNumber2);
    test.Test("StringToNumber Test 3", TestStringToNumber3);
    test.Test("StringToNumber Test 4", TestStringToNumber4);
    test.Test("StringToNumber Test 5", TestStringToNumber5);
    test.Test("StringToNumber Test 6", TestStringToNumber6);
    test.Test("StringToNumber Test 7", TestStringToNumber7);

    test.Test("HexStringToNumber Test 1", TestHexStringToNumber1);
    test.Test("HexStringToNumber Test 2", TestHexStringToNumber2);

    StringStream<char> stream{512};

    test.Test("IntToString Test 1", TestIntToString1, false, stream);

    test.Test("DoubleToString Test 1", TestDoubleToString1, false, stream);
    test.Test("DoubleToString Test 2", TestDoubleToString2, false, stream);
    test.Test("DoubleToString Test 3", TestDoubleToString3, false, stream);
    test.Test("DoubleToString Test 4", TestDoubleToString4, false, stream);
    test.Test("DoubleToString Test 5", TestDoubleToString5, false, stream);
    test.Test("DoubleToString Test 6", TestDoubleToString6, false, stream);
    test.Test("DoubleToString Test 7", TestDoubleToString7, false, stream);
    test.Test("DoubleToString Test 8", TestDoubleToString8, false, stream);

    test.Test("DoubleToStringSemiFixed Test 3", TestDoubleToStringSemiFixed, false, stream);
    test.Test("DoubleToStringFixed Test 3", TestDoubleToStringFixed, false, stream);

    test.Test("FloatToString Test 1", TestFloatToString1, false, stream);
    test.Test("FloatToString Test 2", TestFloatToString2, false, stream);
    test.Test("FloatToString Test 3", TestFloatToString3, false, stream);
    test.Test("FloatToString Test 4", TestFloatToString4, false, stream);
    test.Test("FloatToString Test 5", TestFloatToString5, false, stream);

    stream.Reset();

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
