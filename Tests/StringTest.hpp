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

#ifndef QENTEM_STRING_TESTS_H_
#define QENTEM_STRING_TESTS_H_

namespace Qentem {
namespace Test {

using String8 = String<char>;

static int TestString1() {
    SizeT length;
    char *strptr;

    String8 str1;
    SHOULD_EQUAL_VALUE(str1.Length(), 0, "Length");
    SHOULD_EQUAL(str1.First(), nullptr, "First()", "null");

    String8 str2{"0123456789"};
    SHOULD_EQUAL_VALUE(str2.Length(), 10, "Length");
    SHOULD_NOT_EQUAL(str2.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str2.First()[str2.Length()], 0, "First()[Length]");

    str2.Reset();
    SHOULD_EQUAL_VALUE(str2.Length(), 0, "Length");
    SHOULD_EQUAL(str2.First(), nullptr, "First()", "null");

    str1 = String8("abcd");
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "Length");
    SHOULD_NOT_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abcd", "str1");
    SHOULD_NOT_EQUAL(str1, "abcdef", "str1", "abcdef");

    str1 = String8("abcd", 2);
    SHOULD_EQUAL_VALUE(str1.Length(), 2, "Length");
    SHOULD_NOT_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str1, "ab", "str1");
    SHOULD_NOT_EQUAL(str1, "abcd", "str1", "abcd");

    length = str1.Length();
    strptr = str1.Eject();
    SHOULD_NOT_EQUAL(strptr, nullptr, "Eject", "null");

    str2 = String8(strptr, length); // Manage
    SHOULD_EQUAL_VALUE(str2.First(), strptr, "First()");
    SHOULD_EQUAL_VALUE(str2.Length(), length, "Length");

    str1 = static_cast<String8 &&>(str2); // Move
    SHOULD_EQUAL_VALUE(str1.First(), strptr, "First()");
    SHOULD_EQUAL_VALUE(str1.Length(), length, "Length");
    SHOULD_EQUAL_VALUE(str2.Length(), 0, "Length");
    SHOULD_EQUAL(str2.First(), nullptr, "First()", "null");

    str2 = "abcdef"; // Copy
    SHOULD_EQUAL_VALUE(str2.Length(), 6, "Length");
    SHOULD_NOT_EQUAL(str2.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str2.First()[str2.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual(str2.First(), "abcdef", 5),
                      "IsEqual");

    str2 = str1; // Copy
    SHOULD_EQUAL_VALUE(str2.Length(), str1.Length(), "Length");
    SHOULD_NOT_EQUAL(str2.First(), nullptr, "First()", "null");
    SHOULD_NOT_EQUAL(str2.First(), str1.First(), "First()", str1.First());
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()),
        "IsEqual");

    str2.Reset();
    str1 = "efg";         // Copy
    str2 = String8(str1); // Copy
    SHOULD_EQUAL_VALUE(str2.Length(), str1.Length(), "Length");
    SHOULD_NOT_EQUAL(str2.First(), nullptr, "First()", "null");
    SHOULD_NOT_EQUAL(str2.First(), str1.First(), "First()", str1.First());
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()),
        "IsEqual");

    str1 = "hig";         // Copy
    str2 = String8(str1); // Copy over a value
    SHOULD_EQUAL_VALUE(str2.Length(), str1.Length(), "Length");
    SHOULD_NOT_EQUAL(str2.First(), str1.First(), "First()", str1.First());
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()),
        "IsEqual");

    length = str1.Length();
    str2   = String8(static_cast<String8 &&>(str1));
    SHOULD_EQUAL_VALUE(str1.Length(), 0, "Length");
    SHOULD_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str2.Length(), length, "Length");

    END_SUB_TEST;
}

static int TestStringCompare() {
    String8     str1("a");
    String8     str2("a");
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
    SHOULD_NOT_EQUAL_TRUE(str1.IsEqual(" ", 1), "IsEqual");

    END_SUB_TEST;
}

static int TestString2() {
    String8 str1;
    String8 str2;

    str1.Insert("a", 1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "Length");
    SHOULD_NOT_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str1, "a", "str1");

    str1.Insert("bc", 2);
    SHOULD_EQUAL_VALUE(str1.Length(), 3, "Length");
    SHOULD_NOT_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abc", "str1");

    str1.Insert("", 0);
    SHOULD_EQUAL_VALUE(str1.Length(), 3, "Length");
    SHOULD_NOT_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abc", "str1");

    str2 = String8::Insert(str1, String8("def"));
    SHOULD_EQUAL_VALUE(str2.Length(), 6, "Length");
    SHOULD_NOT_EQUAL(str2.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str2.First()[str2.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str2, "abcdef", "str2");

    str2 = String8::Insert(str2, String8(""));
    SHOULD_EQUAL_VALUE(str2.Length(), 6, "Length");
    SHOULD_NOT_EQUAL(str2.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str2.First()[str2.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str2, "abcdef", "str2");

    str2 = String8::Insert(String8(""), str2);
    SHOULD_EQUAL_VALUE(str2.Length(), 6, "Length");
    SHOULD_NOT_EQUAL(str2.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str2.First()[str2.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str2, "abcdef", "str2");

    str1.Reset();
    str1 += "a";
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "Length");
    SHOULD_NOT_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str1, "a", "str1");

    str1 += "bc";
    SHOULD_EQUAL_VALUE(str1.Length(), 3, "Length");
    SHOULD_NOT_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abc", "str1");

    str2 = "def";
    str1 += str2;
    SHOULD_EQUAL_VALUE(str1.Length(), 6, "Length");
    SHOULD_NOT_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abcdef", "str1");

    str2 = "ghi";
    str1 += static_cast<String8 &&>(str2);
    SHOULD_EQUAL_VALUE(str1.Length(), 9, "Length");
    SHOULD_NOT_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abcdefghi", "str1");
    SHOULD_EQUAL_VALUE(str2.Length(), 0, "Length");
    SHOULD_EQUAL(str2.First(), nullptr, "First()", "null");

    str1 = str1 + "";
    SHOULD_EQUAL_VALUE(str1.Length(), 9, "Length");
    SHOULD_NOT_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abcdefghi", "str1");

    str1 = str1 + "gkl";
    SHOULD_EQUAL_VALUE(str1.Length(), 12, "Length");
    SHOULD_NOT_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abcdefghigkl", "str1");

    str2 = "123";
    str1 = str1 + str2;
    SHOULD_EQUAL_VALUE(str1.Length(), 15, "Length");
    SHOULD_NOT_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str1, "abcdefghigkl123", "str1");

    str1.Reset();
    str1 = "123";
    str2 = "456";
    str1 = str1 + static_cast<String8 &&>(str2);
    SHOULD_EQUAL_VALUE(str1.Length(), 6, "Length");
    SHOULD_NOT_EQUAL(str1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    SHOULD_EQUAL_VALUE(str1, "123456", "str1");
    SHOULD_EQUAL_VALUE(str2.Length(), 0, "Length");
    SHOULD_EQUAL(str2.First(), nullptr, "First()", "null");

    END_SUB_TEST;
}

static int TestTrim() {
    String8 str1("");

    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 0, "length");

    str1 = " ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 0, "length");

    str1 = "  ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 0, "length");

    str1 = "                    ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 0, "length");

    str1 = " a";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "a ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = " a ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "  a";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "a  ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "  a  ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "                      a";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "a                      ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = "                      a                      ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 1, "length");

    str1 = " abcd";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "abcd ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = " abcd ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "  abcd";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "abcd  ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "  abcd  ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "                      abcd";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "abcd                      ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    str1 = "                      abcd                      ";
    str1 = String8::Trim(str1);
    SHOULD_EQUAL_VALUE(str1.Length(), 4, "length");

    END_SUB_TEST;
}

static int RunStringTests() {
    STARTING_TEST("String.hpp");

    START_TEST("String Test 1", TestString1);
    START_TEST("String::IsEqual", TestStringCompare);
    START_TEST("String Test 2", TestString2);
    START_TEST("String::Trim", TestTrim);

    END_TEST("String.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
