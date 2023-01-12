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

#include "String.hpp"

#ifndef QENTEM_STRING_TESTS_H_
#define QENTEM_STRING_TESTS_H_

namespace Qentem {
namespace Test {

using String8 = String<char>;

static void TestString1(TestHelper &helper) {
    SizeT length;
    char *strptr;

    String8 str1;
    helper.Equal(str1.Length(), 0U, "Length", __LINE__);
    helper.Equal(str1.First(), nullptr, "First()", "null", __LINE__);

    String8 str2{"0123456789"};
    helper.Equal(str2.Length(), 10U, "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str2.First()[str2.Length()], '\0', "First()[Length]", __LINE__);

    str2.Reset();
    helper.Equal(str2.Length(), 0U, "Length", __LINE__);
    helper.Equal(str2.First(), nullptr, "First()", "null", __LINE__);

    str1 = String8("abcd");
    helper.Equal(str1.Length(), 4U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__); // Dose it have /0 at the end?
    helper.Equal(str1, "abcd", "str1", __LINE__);
    helper.NotEqual(str1, "abcdef", "str1", "abcdef", __LINE__);

    str1 = String8("abcd", 2U);
    helper.Equal(str1.Length(), 2U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "ab", "str1", __LINE__);
    helper.NotEqual(str1, "abcd", "str1", "abcd", __LINE__);

    str1 = "ABCDEF0123456789ABCDEF0123456789";
    helper.Equal(str1.Length(), 32U, "Length", __LINE__);

    length = str1.Length();
    strptr = str1.Detach();
    helper.NotEqual(strptr, nullptr, "Detach", "null", __LINE__);

    str2 = String8(strptr, length); // Manage
    helper.Equal(str2.First(), strptr, "First()", "strptr", __LINE__);
    helper.Equal(str2.Length(), length, "Length", __LINE__);

    str1 = static_cast<String8 &&>(str2); // Move
    helper.Equal(str1.First(), strptr, "First()", __LINE__);
    helper.Equal(str1.Length(), length, "Length", __LINE__);
    helper.Equal(str2.Length(), 0U, "Length", __LINE__);
    helper.Equal(str2.First(), nullptr, "First()", "null", __LINE__);

    str2 = "abcdef"; // Copy
    helper.Equal(str2.Length(), 6U, "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str2.First()[str2.Length()], '\0', "First()[Length]", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(str2.First(), "abcdef", 5U), "IsEqual", __LINE__);

    str2 = str1; // Copy
    helper.Equal(str2.Length(), str1.Length(), "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(str2.First(), str1.First(), "First()", str1.First(), __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), "IsEqual", __LINE__);

    str2.Reset();
    str1 = "efg";         // Copy
    str2 = String8(str1); // Copy
    helper.Equal(str2.Length(), str1.Length(), "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(str2.First(), str1.First(), "First()", str1.First(), __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), "IsEqual", __LINE__);

    str1 = "hig";         // Copy
    str2 = String8(str1); // Copy over a value
    helper.Equal(str2.Length(), str1.Length(), "Length", __LINE__);
    helper.NotEqual(str2.First(), str1.First(), "First()", str1.First(), __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), "IsEqual", __LINE__);

    length = str1.Length();
    str2   = String8(static_cast<String8 &&>(str1));
    helper.Equal(str1.Length(), 0U, "Length", __LINE__);
    helper.Equal(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str2.Length(), length, "Length", __LINE__);

    str1 = "A";
    helper.Equal(str1.Length(), 1U, "Length", __LINE__);
    strptr = str1.Detach();
    helper.Equal(strptr[0], 'A', "strptr[0]", "A", __LINE__);
    Memory::Deallocate(strptr);

    char   *tmp_size_2 = Memory::Allocate<char>(2);
    String8 str_size_2 = String8(tmp_size_2, 2U);

    if (Config::ShortStringOptimization) {
        helper.NotEqual(str_size_2.First(), tmp_size_2, "First()", "tmp_size_2", __LINE__);
    } else {
        helper.Equal(str_size_2.First(), tmp_size_2, "First()", "tmp_size_2", __LINE__);
    }
}

static void TestStringCompare(TestHelper &helper) {
    String8     str1("a");
    String8     str2("a");
    const char *str3 = "a";

    helper.Equal(str1, str2, "str1", "str2", __LINE__);
    helper.Equal(str2, str1, "str2", "str1", __LINE__);
    helper.Equal(str1, str3, "str1", "str3", __LINE__);
    helper.Equal(str2, str3, "str2", "str3", __LINE__);

    str1 = "abc";
    str2 = "abc";
    str3 = "abc";
    helper.Equal(str1, str2, "str1", "str2", __LINE__);
    helper.Equal(str2, str1, "str2", "str1", __LINE__);
    helper.Equal(str1, str3, "str1", "str3", __LINE__);
    helper.Equal(str2, str3, "str2", "str3", __LINE__);

    str1 = "a";
    str2 = "b";
    str3 = "c";
    helper.NotEqual(str1, str2, "str1", "str2", __LINE__);
    helper.NotEqual(str2, str1, "str2", "str1", __LINE__);
    helper.NotEqual(str1, str3, "str1", "str3", __LINE__);
    helper.NotEqual(str2, str3, "str2", "str3", __LINE__);

    str1 = "abc";
    str2 = "efg";
    str3 = "hij";
    helper.NotEqual(str1, str2, "str1", "str2", __LINE__);
    helper.NotEqual(str2, str1, "str2", "str1", __LINE__);
    helper.NotEqual(str1, str3, "str1", "str3", __LINE__);
    helper.NotEqual(str2, str3, "str2", "str3", __LINE__);

    str1 = "a";
    str2 = "ef";
    str3 = "abc";
    helper.NotEqual(str1, str2, "str1", "str2", __LINE__);
    helper.NotEqual(str2, str1, "str2", "str1", __LINE__);
    helper.NotEqual(str1, str3, "str1", "str3", __LINE__);
    helper.NotEqual(str2, str3, "str2", "str3", __LINE__);

    str1 = "";
    helper.EqualsFalse(str1.IsEqual(" ", 1U), "IsEqual", __LINE__);

    str1 = "";
    str2 = "";
    helper.EqualsTrue((str1 >= str2), "IsGreater", __LINE__);
    helper.EqualsTrue((str1 <= str2), "IsLess", __LINE__);
    helper.EqualsFalse((str1 > str2), "IsGreater", __LINE__);
    helper.EqualsFalse((str1 < str2), "IsLess", __LINE__);

    str1 = "a";
    str2 = "a";
    helper.EqualsTrue((str1 >= str2), "IsGreater", __LINE__);
    helper.EqualsTrue((str1 <= str2), "IsLess", __LINE__);
    helper.EqualsFalse((str1 > str2), "IsGreater", __LINE__);
    helper.EqualsFalse((str1 < str2), "IsLess", __LINE__);

    str1 = "a";
    str2 = "A";
    helper.EqualsTrue((str1 >= str2), "IsGreater", __LINE__);
    helper.EqualsTrue((str1 > str2), "IsGreater", __LINE__);
    helper.EqualsFalse((str1 <= str2), "IsLess", __LINE__);
    helper.EqualsFalse((str1 < str2), "IsLess", __LINE__);
    //
    helper.EqualsFalse((str2 >= str1), "IsGreater", __LINE__);
    helper.EqualsFalse((str2 > str1), "IsGreater", __LINE__);
    helper.EqualsTrue((str2 <= str1), "IsLess", __LINE__);
    helper.EqualsTrue((str2 < str1), "IsLess", __LINE__);

    str1 = "a";
    str2 = "B";
    helper.EqualsTrue((str1 >= str2), "IsGreater", __LINE__);
    helper.EqualsTrue((str1 > str2), "IsGreater", __LINE__);
    helper.EqualsFalse((str1 <= str2), "IsLess", __LINE__);
    helper.EqualsFalse((str1 < str2), "IsLess", __LINE__);
    //
    helper.EqualsFalse((str2 >= str1), "IsGreater", __LINE__);
    helper.EqualsFalse((str2 > str1), "IsGreater", __LINE__);
    helper.EqualsTrue((str2 <= str1), "IsLess", __LINE__);
    helper.EqualsTrue((str2 < str1), "IsLess", __LINE__);

    str1 = "aa";
    str2 = "aA";
    helper.EqualsTrue((str1 >= str2), "IsGreater", __LINE__);
    helper.EqualsTrue((str1 > str2), "IsGreater", __LINE__);
    helper.EqualsFalse((str1 <= str2), "IsLess", __LINE__);
    helper.EqualsFalse((str1 < str2), "IsLess", __LINE__);
    //
    helper.EqualsFalse((str2 >= str1), "IsGreater", __LINE__);
    helper.EqualsFalse((str2 > str1), "IsGreater", __LINE__);
    helper.EqualsTrue((str2 <= str1), "IsLess", __LINE__);
    helper.EqualsTrue((str2 < str1), "IsLess", __LINE__);

    str1 = "2021";
    str2 = "2018";
    helper.EqualsTrue((str1 >= str2), "IsGreater", __LINE__);
    helper.EqualsTrue((str1 > str2), "IsGreater", __LINE__);
    helper.EqualsFalse((str1 <= str2), "IsLess", __LINE__);
    helper.EqualsFalse((str1 < str2), "IsLess", __LINE__);
    //
    helper.EqualsFalse((str2 >= str1), "IsGreater", __LINE__);
    helper.EqualsFalse((str2 > str1), "IsGreater", __LINE__);
    helper.EqualsTrue((str2 <= str1), "IsLess", __LINE__);
    helper.EqualsTrue((str2 < str1), "IsLess", __LINE__);
}

static void TestString2(TestHelper &helper) {
    String8 str1;
    String8 str2;

    str1.Write("a", 1U);
    helper.Equal(str1.Length(), 1U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "a", "str1", __LINE__);

    str1.Write("bc", 2U);
    helper.Equal(str1.Length(), 3U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "abc", "str1", __LINE__);

    str1.Write("", 0U);
    helper.Equal(str1.Length(), 3U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "abc", "str1", __LINE__);

    str2 = String8::Merge(str1, String8("def"));
    helper.Equal(str2.Length(), 6U, "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str2.First()[str2.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str2, "abcdef", "str2", __LINE__);

    str2 = String8::Merge(str2, String8(""));
    helper.Equal(str2.Length(), 6U, "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str2.First()[str2.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str2, "abcdef", "str2", __LINE__);

    str2 = String8::Merge(String8(""), str2);
    helper.Equal(str2.Length(), 6U, "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str2.First()[str2.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str2, "abcdef", "str2", __LINE__);

#if defined(QENTEM_SSO) && (QENTEM_SSO == 1U)
    str1.Reset();
    for (SizeT i = 0; i < String8::ShortStringMax; i++) {
        str1 += "A";
    }

    str1 += "_";
    helper.Equal(str1.Length(), (String8::ShortStringMax + 1U), "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "AAAAAAAAAAAAAA_", "str1", __LINE__);

    str1.Reset();
    for (SizeT i = 3; i < String8::ShortStringMax; i++) {
        str1 += "A";
    }

    str1 += "_";
    helper.Equal(str1.Length(), (String8::ShortStringMax - 2U), "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "AAAAAAAAAAA_", "str1", __LINE__);

    str1 += "B";
    helper.Equal(str1.Length(), 13U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "AAAAAAAAAAA_B", "str1", __LINE__);

    str1 += "_C";
    helper.Equal(str1.Length(), 15U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "AAAAAAAAAAA_B_C", "str1", __LINE__);
#endif

    str1.Reset();
    str1 += "a";
    helper.Equal(str1.Length(), 1U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "a", "str1", __LINE__);

    str1 += "bc";
    helper.Equal(str1.Length(), 3U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "abc", "str1", __LINE__);

    str2 = "def";
    str1 += str2;
    helper.Equal(str1.Length(), 6U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "abcdef", "str1", __LINE__);

    str2 = "ghi";
    str1 += static_cast<String8 &&>(str2);
    helper.Equal(str1.Length(), 9U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "abcdefghi", "str1", __LINE__);
    helper.Equal(str2.Length(), 0U, "Length", __LINE__);
    helper.Equal(str2.First(), nullptr, "First()", "null", __LINE__);

    str1 = str1 + "";
    helper.Equal(str1.Length(), 9U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "abcdefghi", "str1", __LINE__);

    str1 = str1 + "gkl";
    helper.Equal(str1.Length(), 12U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "abcdefghigkl", "str1", __LINE__);

    str2 = "123";
    str1 = str1 + str2;
    helper.Equal(str1.Length(), 15U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "abcdefghigkl123", "str1", __LINE__);

    str1 = "aaaaaaaaaaaaaaaaaaaaaaaa";
    str1 += "aaaaaaaaaaaaaaaaaaaaaaaa";
    helper.Equal(str1.Length(), 48U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "str1", __LINE__);

    str1.Reset();
    str1 = "1234";
    str2 = "5678";
    str1 = str1 + static_cast<String8 &&>(str2);
    helper.Equal(str1.Length(), 8U, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], '\0', "First()[Length]", __LINE__);
    helper.Equal(str1, "12345678", "str1", __LINE__);
    helper.Equal(str2.Length(), 0U, "Length", __LINE__);
    helper.Equal(str2.First(), nullptr, "First()", "null", __LINE__);

    struct SimpleStream {
        char               str[8]{0};
        const unsigned int max = 8;
        unsigned int       index{0};

        void operator<<(const char *c) noexcept {
            while (index < max) {
                str[index] = *c;
                ++index;
                ++c;
            }
        }
    };

    SimpleStream sis;
    sis << str1;

    helper.EqualsTrue(StringUtils::IsEqual(&(sis.str[0]), "12345678", 8U), "SimpleStream", __LINE__);
}

static void TestStringTrim(TestHelper &helper) {
    String8 str1("");

    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 0U, "length", __LINE__);

    str1 = " ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 0U, "length", __LINE__);

    str1 = "  ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 0U, "length", __LINE__);

    str1 = "                    ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 0U, "length", __LINE__);

    str1 = " a";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 1U, "length", __LINE__);

    str1 = "a ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 1U, "length", __LINE__);

    str1 = " a ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 1U, "length", __LINE__);

    str1 = "  a";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 1U, "length", __LINE__);

    str1 = "a  ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 1U, "length", __LINE__);

    str1 = "  a  ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 1U, "length", __LINE__);

    str1 = "                      a";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 1U, "length", __LINE__);

    str1 = "a                      ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 1U, "length", __LINE__);

    str1 = "                      a                      ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 1U, "length", __LINE__);

    str1 = " abcd";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 4U, "length", __LINE__);

    str1 = "abcd ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 4U, "length", __LINE__);

    str1 = " abcd ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 4U, "length", __LINE__);

    str1 = "  abcd";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 4U, "length", __LINE__);

    str1 = "abcd  ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 4U, "length", __LINE__);

    str1 = "  abcd  ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 4U, "length", __LINE__);

    str1 = "                      abcd";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 4U, "length", __LINE__);

    str1 = "abcd                      ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 4U, "length", __LINE__);

    str1 = "                      abcd                      ";
    str1 = String8::Trim(str1);
    helper.Equal(str1.Length(), 4U, "length", __LINE__);
}

static int RunStringTests() {
    TestHelper helper{"String.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("String Test 1", TestString1);
    helper.Test("String::Compare", TestStringCompare);
    helper.Test("String Test 2", TestString2);
    helper.Test("String::Trim", TestStringTrim);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
