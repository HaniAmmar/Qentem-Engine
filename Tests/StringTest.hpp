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
#include "TestHelper.hpp"

#ifndef QENTEM_STRINGTEST_H_
#define QENTEM_STRINGTEST_H_

namespace Qentem {
namespace Test {

static int TestCount() {
    ULong length = String::Count("");
    SHOULD_EQUAL_VALUE(length, 0, "length");

    length = String::Count(nullptr);
    SHOULD_EQUAL_VALUE(length, 0, "length");

    length = String::Count("a");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    length = String::Count("abcde");
    SHOULD_EQUAL_VALUE(length, 5, "length");

    length = String::Count("\0");
    SHOULD_EQUAL_VALUE(length, 0, "length");

    length = String::Count("1234567");
    SHOULD_EQUAL_VALUE(length, 7, "length");

    length = String::Count("123456781234567");
    SHOULD_EQUAL_VALUE(length, 15, "length");

    length = String::Count("1234567812345678123456781234567");
    SHOULD_EQUAL_VALUE(length, 31, "length");

    length = String::Count(
        "123456781234567812345678123456781234567812345678123456781234567");
    SHOULD_EQUAL_VALUE(length, 63, "length");

    END_SUB_TEST;
}

static int TestString1() {
    ULong length;
    char *strptr;

    String str1;
    SHOULD_EQUAL_VALUE(str1.Length(), 0, "Length");
    SHOULD_EQUAL(str1.Char(), nullptr, "Char()", "null");

    String str2(10); // Preset size
    SHOULD_EQUAL_VALUE(str2.Length(), 10, "Length");
    SHOULD_NOT_EQUAL(str2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str2.Char()[str2.Length()], '\0', "Char()[Length]");

    str2.Clear();
    SHOULD_EQUAL_VALUE(str2.Length(), 0, "Length");
    SHOULD_EQUAL(str2.Char(), nullptr, "Char()", "null");

    str1 = String("abcd");
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "Length");
    SHOULD_NOT_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str1.Char()[str1.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abcd", "str1");
    SHOULD_NOT_EQUAL(str1, "abcdef", "str1", "abcdef");

    str1 = String("abcd", 2);
    SHOULD_EQUAL_VALUE(str1.Length(), 2, "Length");
    SHOULD_NOT_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str1.Char()[str1.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str1, "ab", "str1");
    SHOULD_NOT_EQUAL(str1, "abcd", "str1", "abcd");

    length = str1.Length();
    strptr = str1.Eject();
    SHOULD_NOT_EQUAL(strptr, nullptr, "Eject", "null");

    str2 = String(strptr, length); // Manage
    SHOULD_EQUAL_VALUE(str2.Char(), strptr, "Char()");
    SHOULD_EQUAL_VALUE(str2.Length(), length, "Length");

    str1 = static_cast<String &&>(str2); // Move
    SHOULD_EQUAL_VALUE(str1.Char(), strptr, "Char()");
    SHOULD_EQUAL_VALUE(str1.Length(), length, "Length");
    SHOULD_EQUAL_VALUE(str2.Length(), 0, "Length");
    SHOULD_EQUAL(str2.Char(), nullptr, "Char()", "null");

    str2 = "abcdef"; // Copy
    SHOULD_EQUAL_VALUE(str2.Length(), 6, "Length");
    SHOULD_NOT_EQUAL(str2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str2.Char()[str2.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_TRUE(Memory::Compare(str2.Char(), "abcdef", 5), "Compare");

    str2 = str1; // Copy
    SHOULD_EQUAL_VALUE(str2.Length(), str1.Length(), "Length");
    SHOULD_NOT_EQUAL(str2.Char(), nullptr, "Char()", "null");
    SHOULD_NOT_EQUAL(str2.Char(), str1.Char(), "Char()", str1.Char());
    SHOULD_EQUAL_TRUE(Memory::Compare(str2.Char(), str1.Char(), str2.Length()),
                      "Compare");

    str2.Clear();
    str1 = "efg";        // Copy
    str2 = String(str1); // Copy
    SHOULD_EQUAL_VALUE(str2.Length(), str1.Length(), "Length");
    SHOULD_NOT_EQUAL(str2.Char(), nullptr, "Char()", "null");
    SHOULD_NOT_EQUAL(str2.Char(), str1.Char(), "Char()", str1.Char());
    SHOULD_EQUAL_TRUE(Memory::Compare(str2.Char(), str1.Char(), str2.Length()),
                      "Compare");

    str1 = "hig";        // Copy
    str2 = String(str1); // Copy over a value
    SHOULD_EQUAL_VALUE(str2.Length(), str1.Length(), "Length");
    SHOULD_NOT_EQUAL(str2.Char(), str1.Char(), "Char()", str1.Char());
    SHOULD_EQUAL_TRUE(Memory::Compare(str2.Char(), str1.Char(), str2.Length()),
                      "Compare");

    length = str1.Length();
    strptr = &(str1[0]);
    str2   = String(static_cast<String &&>(str1)); // Move over a value
    SHOULD_EQUAL_VALUE(str1.Length(), 0, "Length");
    SHOULD_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str2.Char(), strptr, "Char()");
    SHOULD_EQUAL_VALUE(str2.Length(), length, "Length");

    END_SUB_TEST;
}

static int TestCompare2() {
    String      str1("a");
    String      str2("a");
    const char *str3 = "a";

    SHOULD_EQUAL(str1, str2, "str1", "str2");
    SHOULD_EQUAL(str2, str1, "str2", "str1");

    SHOULD_EQUAL(str1, str3, "str1", "str3");
    SHOULD_EQUAL(str2, str3, "str2", "str3");

    str1 = "abc";
    str2 = "abc";
    str3 = "abc";

    SHOULD_EQUAL(str1, str2, "str1", "str2");
    SHOULD_EQUAL(str2, str1, "str2", "str1");

    SHOULD_EQUAL(str1, str3, "str1", "str3");
    SHOULD_EQUAL(str2, str3, "str2", "str3");

    str1 = "a";
    str2 = "b";
    str3 = "c";

    SHOULD_NOT_EQUAL(str1, str2, "str1", "str2");
    SHOULD_NOT_EQUAL(str2, str1, "str2", "str1");

    SHOULD_NOT_EQUAL(str1, str3, "str1", "str3");
    SHOULD_NOT_EQUAL(str2, str3, "str2", "str3");

    str1 = "abc";
    str2 = "efg";
    str3 = "hij";

    SHOULD_NOT_EQUAL(str1, str2, "str1", "str2");
    SHOULD_NOT_EQUAL(str2, str1, "str2", "str1");

    SHOULD_NOT_EQUAL(str1, str3, "str1", "str3");
    SHOULD_NOT_EQUAL(str2, str3, "str2", "str3");

    str1 = "a";
    str2 = "ef";
    str3 = "abc";

    SHOULD_NOT_EQUAL(str1, str2, "str1", "str2");
    SHOULD_NOT_EQUAL(str2, str1, "str2", "str1");

    SHOULD_NOT_EQUAL(str1, str3, "str1", "str3");
    SHOULD_NOT_EQUAL(str2, str3, "str2", "str3");

    str1 = "";
    SHOULD_NOT_EQUAL_TRUE(str1.Compare(" ", 1), "Compare");

    END_SUB_TEST;
}

static int TestString2() {
    String str1;
    String str2;

    str1.Add("a", 1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "Length");
    SHOULD_NOT_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str1.Char()[str1.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str1, "a", "str1");

    str1.Add("bc", 2);
    SHOULD_EQUAL_VALUE(str1.Length(), 3, "Length");
    SHOULD_NOT_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str1.Char()[str1.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abc", "str1");

    str1.Add("", 0);
    SHOULD_EQUAL_VALUE(str1.Length(), 3, "Length");
    SHOULD_NOT_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str1.Char()[str1.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abc", "str1");

    str2 = String::Add(str1, String("def"));
    SHOULD_EQUAL_VALUE(str2.Length(), 6, "Length");
    SHOULD_NOT_EQUAL(str2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str2.Char()[str2.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str2, "abcdef", "str2");

    str2 = String::Add(str2, String(""));
    SHOULD_EQUAL_VALUE(str2.Length(), 6, "Length");
    SHOULD_NOT_EQUAL(str2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str2.Char()[str2.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str2, "abcdef", "str2");

    str2 = String::Add(String(""), str2);
    SHOULD_EQUAL_VALUE(str2.Length(), 6, "Length");
    SHOULD_NOT_EQUAL(str2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str2.Char()[str2.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str2, "abcdef", "str2");

    str1.Clear();
    str1 += "a";
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "Length");
    SHOULD_NOT_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str1.Char()[str1.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str1, "a", "str1");

    str1 += "bc";
    SHOULD_EQUAL_VALUE(str1.Length(), 3, "Length");
    SHOULD_NOT_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str1.Char()[str1.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abc", "str1");

    str2 = "def";
    str1 += str2;
    SHOULD_EQUAL_VALUE(str1.Length(), 6, "Length");
    SHOULD_NOT_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str1.Char()[str1.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abcdef", "str1");

    str2 = "ghi";
    str1 += static_cast<String &&>(str2);
    SHOULD_EQUAL_VALUE(str1.Length(), 9, "Length");
    SHOULD_NOT_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str1.Char()[str1.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abcdefghi", "str1");
    SHOULD_EQUAL_VALUE(str2.Length(), 0, "Length");
    SHOULD_EQUAL(str2.Char(), nullptr, "Char()", "null");

    str1 = str1 + "";
    SHOULD_EQUAL_VALUE(str1.Length(), 9, "Length");
    SHOULD_NOT_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str1.Char()[str1.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abcdefghi", "str1");

    str1 = str1 + "gkl";
    SHOULD_EQUAL_VALUE(str1.Length(), 12, "Length");
    SHOULD_NOT_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str1.Char()[str1.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abcdefghigkl", "str1");

    str2 = "123";
    str1 = str1 + str2;
    SHOULD_EQUAL_VALUE(str1.Length(), 15, "Length");
    SHOULD_NOT_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str1.Char()[str1.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abcdefghigkl123", "str1");

    bool did_throw;

    try {
        did_throw = false;
        str1[15];
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    str1.Clear();
    str1 = "123";
    str2 = "456";
    str1 = str1 + static_cast<String &&>(str2);
    SHOULD_EQUAL_VALUE(str1.Length(), 6, "Length");
    SHOULD_NOT_EQUAL(str1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(str1.Char()[str1.Length()], '\0', "Char()[Length]");
    SHOULD_EQUAL_VALUE(str1, "123456", "str1");
    SHOULD_EQUAL_VALUE(str2.Length(), 0, "Length");
    SHOULD_EQUAL(str2.Char(), nullptr, "Char()", "null");

    str1.Clear();

    try {
        did_throw = false;
        str1[0];
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    END_SUB_TEST;
}

static int TestLeftTrim() {
    int offset = 0;

    String::LeftTrim("", offset, 0);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    String::LeftTrim("", offset, 1);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    String::LeftTrim("", offset, 10);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    String::LeftTrim("a", offset, 1);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    String::LeftTrim("abc", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    String::LeftTrim("a ", offset, 2);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    String::LeftTrim("abc ", offset, 4);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    String::LeftTrim("a  ", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    String::LeftTrim("abc  ", offset, 5);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    String::LeftTrim("a   ", offset, 4);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    String::LeftTrim("abc   ", offset, 6);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    offset = 0;
    String::LeftTrim(" a", offset, 2);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 0;
    String::LeftTrim(" abc", offset, 4);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 0;
    String::LeftTrim("  a", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 0;
    String::LeftTrim("  abc", offset, 5);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 0;
    String::LeftTrim("   a", offset, 4);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 0;
    String::LeftTrim("   abc", offset, 6);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 0;
    String::LeftTrim(" a ", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 0;
    String::LeftTrim(" abc ", offset, 5);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 0;
    String::LeftTrim("  a ", offset, 4);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 0;
    String::LeftTrim("  abc ", offset, 6);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 0;
    String::LeftTrim("   a  ", offset, 6);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 0;
    String::LeftTrim("   abc  ", offset, 8);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 0;
    String::LeftTrim(" ", offset, 1);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 0;
    String::LeftTrim("  ", offset, 2);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 0;
    String::LeftTrim("   ", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 1;
    String::LeftTrim(" a ", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 1;
    String::LeftTrim(" abc ", offset, 5);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 2;
    String::LeftTrim("  a ", offset, 4);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 2;
    String::LeftTrim("  abc ", offset, 6);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 2;
    String::LeftTrim("   a  ", offset, 6);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 2;
    String::LeftTrim("   abc  ", offset, 8);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 1;
    String::LeftTrim("  ", offset, 2);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 2;
    String::LeftTrim("   ", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 0;
    String::LeftTrim("  ", offset, 1);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 1;
    String::LeftTrim("   ", offset, 2);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    END_SUB_TEST;
}

static int TestSoftTrim() {
    ULong offset = 0;
    ULong length = 0;

    String::SoftTrim("", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 0, "length");

    offset = 0;
    length = 1;
    String::SoftTrim(" ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 0, "length");

    offset = 0;
    length = 1;
    String::SoftTrim("  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 0, "length");

    offset = 0;
    length = 2;
    String::SoftTrim("  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 0, "length");

    offset = 0;
    length = 2;
    String::SoftTrim(" a", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 4;
    String::SoftTrim(" abc", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 1;
    length = 1;
    String::SoftTrim(" a", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 1;
    length = 3;
    String::SoftTrim(" abc", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 2;
    length = 1;
    String::SoftTrim("  a", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 2;
    length = 3;
    String::SoftTrim("  abc", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 3;
    String::SoftTrim("  a", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 5;
    String::SoftTrim("  abc", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 4;
    String::SoftTrim("   a", offset, length);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 6;
    String::SoftTrim("   abc", offset, length);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 2;
    String::SoftTrim("a ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 4;
    String::SoftTrim("abc ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 1;
    String::SoftTrim("a ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 3;
    String::SoftTrim("abc ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 1;
    String::SoftTrim("a  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 3;
    String::SoftTrim("abc  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 3;
    String::SoftTrim("a  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 5;
    String::SoftTrim("abc  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 4;
    String::SoftTrim("a   ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 6;
    String::SoftTrim("abc   ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 3;
    String::SoftTrim(" a ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 5;
    String::SoftTrim(" abc ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 5;
    String::SoftTrim("  a  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 7;
    String::SoftTrim("  abc  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 7;
    String::SoftTrim("   a   ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 9;
    String::SoftTrim("   abc   ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 1;
    length = 1;
    String::SoftTrim(" a ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 1;
    length = 3;
    String::SoftTrim(" abc ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 2;
    length = 1;
    String::SoftTrim("  a  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 2;
    length = 3;
    String::SoftTrim("  abc  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 3;
    length = 1;
    String::SoftTrim("   a   ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 3;
    length = 3;
    String::SoftTrim("   abc   ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    END_SUB_TEST;
}

static int TestTrim() {
    String str1("");

    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 0, "length");

    str1 = " ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 0, "length");

    str1 = "  ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 0, "length");

    str1 = "                    ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 0, "length");

    str1 = " a";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "a ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = " a ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "  a";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "a  ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "  a  ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "                      a";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "a                      ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "                      a                      ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = " abcd";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "abcd ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = " abcd ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "  abcd";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "abcd  ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "  abcd  ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "                      abcd";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "abcd                      ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "                      abcd                      ";
    str1 = String::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    END_SUB_TEST;
}

static int TestToUTF8() {
    /*
     * HEX         UNICODE      BINARY
     * 0x0000      0            00000000
     * 0x003D      61           00111101
     * 0x007F      127          01111111
     *
     * 0x0080      128          11000010-10000000
     * 0x00A1      161          11000010-10100001
     * 0x07FF      2047         11011111-10111111
     *
     * 0x0800      2048         11100000-10100000-10000000
     * 0x08A7      2215         11100000-10100010-10100111
     * 0xFFFF      65535        11101111-10111111-10111111
     *
     * 0x10000     65536        11110000-10010000-10000000-10000000
     * 0x10A7B     68219        11110000-10010000-10101001-10111011
     * 0xE01EF     917999       11110011-10100000-10000111-10101111
     */

    ULong length;
    char *str = HAllocator::Allocate<char>(5);

    length = String::ToUTF8(0x0000, str);
    SHOULD_EQUAL_VALUE(length, 1, "length");
    SHOULD_EQUAL_VALUE(str[0], '\0', "str[0]");

    length = String::ToUTF8(0x003D, str);
    SHOULD_EQUAL_VALUE(length, 1, "length");
    SHOULD_EQUAL_VALUE(str[0], '=', "str[0]");

    length = String::ToUTF8(0x007F, str);
    SHOULD_EQUAL_VALUE(length, 1, "length");
    SHOULD_EQUAL_VALUE(str[0], char(127), "str[0]");

    length = String::ToUTF8(0x0080, str);
    SHOULD_EQUAL_VALUE(length, 2, "length");
    SHOULD_EQUAL_VALUE(str[0], char(194), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], char(128), "str[1]");

    length = String::ToUTF8(0x00A1, str);
    SHOULD_EQUAL_VALUE(length, 2, "length");
    SHOULD_EQUAL_VALUE(str[0], char(194), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], char(161), "str[1]");

    length = String::ToUTF8(0x07FF, str);
    SHOULD_EQUAL_VALUE(length, 2, "length");
    SHOULD_EQUAL_VALUE(str[0], char(223), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], char(191), "str[1]");

    length = String::ToUTF8(0x0800, str);
    SHOULD_EQUAL_VALUE(length, 3, "length");
    SHOULD_EQUAL_VALUE(str[0], char(224), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], char(160), "str[1]");
    SHOULD_EQUAL_VALUE(str[2], char(128), "str[2]");

    length = String::ToUTF8(0x08A7, str);
    SHOULD_EQUAL_VALUE(length, 3, "length");
    SHOULD_EQUAL_VALUE(str[0], char(224), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], char(162), "str[1]");
    SHOULD_EQUAL_VALUE(str[2], char(167), "str[2]");

    length = String::ToUTF8(0xFFFF, str);
    SHOULD_EQUAL_VALUE(length, 3, "length");
    SHOULD_EQUAL_VALUE(str[0], char(239), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], char(191), "str[1]");
    SHOULD_EQUAL_VALUE(str[2], char(191), "str[2]");

    length = String::ToUTF8(0x10000, str);
    SHOULD_EQUAL_VALUE(length, 4, "length");
    SHOULD_EQUAL_VALUE(str[0], char(240), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], char(144), "str[1]");
    SHOULD_EQUAL_VALUE(str[2], char(128), "str[2]");
    SHOULD_EQUAL_VALUE(str[3], char(128), "str[3]");

    length = String::ToUTF8(0x10A7B, str);
    SHOULD_EQUAL_VALUE(length, 4, "length");
    SHOULD_EQUAL_VALUE(str[0], char(240), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], char(144), "str[1]");
    SHOULD_EQUAL_VALUE(str[2], char(169), "str[2]");
    SHOULD_EQUAL_VALUE(str[3], char(187), "str[3]");

    length = String::ToUTF8(0xE01EF, str);
    SHOULD_EQUAL_VALUE(length, 4, "length");
    SHOULD_EQUAL_VALUE(str[0], char(243), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], char(160), "str[1]");
    SHOULD_EQUAL_VALUE(str[2], char(135), "str[2]");
    SHOULD_EQUAL_VALUE(str[3], char(175), "str[3]");

    HAllocator::Deallocate(str);

    END_SUB_TEST;
}

static int RunStringTests() {
    STARTING_TEST("String.hpp");

    START_TEST("String::Count", TestCount);
    START_TEST("String Test 1", TestString1);
    START_TEST("String::Compare 2", TestCompare2);
    START_TEST("String Test 2", TestString2);
    START_TEST("String::LeftTrim", TestLeftTrim);
    START_TEST("String::SoftTrim", TestSoftTrim);
    START_TEST("String::Trim", TestTrim);
    START_TEST("String::ToUTF8", TestToUTF8);

    END_TEST("String.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
