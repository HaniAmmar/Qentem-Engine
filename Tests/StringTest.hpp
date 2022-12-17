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
    EQ_VALUE(str1.Length(), 0, "Length");
    EQ_TO(str1.First(), nullptr, "First()", "null");

    String8 str2{"0123456789"};
    EQ_VALUE(str2.Length(), 10, "Length");
    NOT_EQ_TO(str2.First(), nullptr, "First()", "null");
    EQ_VALUE(str2.First()[str2.Length()], 0, "First()[Length]");

    str2.Reset();
    EQ_VALUE(str2.Length(), 0, "Length");
    EQ_TO(str2.First(), nullptr, "First()", "null");

    str1 = String8("abcd");
    EQ_VALUE(str1.Length(), 4, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]"); // Dose it have /0 at the end?
    EQ_VALUE(str1, "abcd", "str1");
    NOT_EQ_TO(str1, "abcdef", "str1", "abcdef");

    str1 = String8("abcd", 2);
    EQ_VALUE(str1.Length(), 2, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "ab", "str1");
    NOT_EQ_TO(str1, "abcd", "str1", "abcd");

    str1 = "ABCDEF0123456789ABCDEF0123456789";
    EQ_VALUE(str1.Length(), 32, "Length");

    length = str1.Length();
    strptr = str1.Eject();
    NOT_EQ_TO(strptr, nullptr, "Eject", "null");

    str2 = String8(strptr, length); // Manage
    EQ_TO(str2.First(), strptr, "First()", "strptr");
    EQ_VALUE(str2.Length(), length, "Length");

    str1 = static_cast<String8 &&>(str2); // Move
    EQ_VALUE(str1.First(), strptr, "First()");
    EQ_VALUE(str1.Length(), length, "Length");
    EQ_VALUE(str2.Length(), 0, "Length");
    EQ_TO(str2.First(), nullptr, "First()", "null");

    str2 = "abcdef"; // Copy
    EQ_VALUE(str2.Length(), 6, "Length");
    NOT_EQ_TO(str2.First(), nullptr, "First()", "null");
    EQ_VALUE(str2.First()[str2.Length()], 0, "First()[Length]");
    EQ_TRUE(StringUtils::IsEqual(str2.First(), "abcdef", 5), "IsEqual");

    str2 = str1; // Copy
    EQ_VALUE(str2.Length(), str1.Length(), "Length");
    NOT_EQ_TO(str2.First(), nullptr, "First()", "null");
    NOT_EQ_TO(str2.First(), str1.First(), "First()", str1.First());
    EQ_TRUE(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), "IsEqual");

    str2.Reset();
    str1 = "efg";         // Copy
    str2 = String8(str1); // Copy
    EQ_VALUE(str2.Length(), str1.Length(), "Length");
    NOT_EQ_TO(str2.First(), nullptr, "First()", "null");
    NOT_EQ_TO(str2.First(), str1.First(), "First()", str1.First());
    EQ_TRUE(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), "IsEqual");

    str1 = "hig";         // Copy
    str2 = String8(str1); // Copy over a value
    EQ_VALUE(str2.Length(), str1.Length(), "Length");
    NOT_EQ_TO(str2.First(), str1.First(), "First()", str1.First());
    EQ_TRUE(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), "IsEqual");

    length = str1.Length();
    str2   = String8(static_cast<String8 &&>(str1));
    EQ_VALUE(str1.Length(), 0, "Length");
    EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str2.Length(), length, "Length");

    str1 = "A";
    EQ_VALUE(str1.Length(), 1, "Length");
    strptr = str1.Eject();
    EQ_TO(strptr[0], 'A', "strptr[0]", "A");
    Memory::Deallocate(strptr);

    char   *tmp_size_2 = Memory::Allocate<char>(2);
    String8 str_size_2 = String8(tmp_size_2, 2);

#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
    NOT_EQ_TO(str_size_2.First(), tmp_size_2, "First()", "tmp_size_2");
#else
    EQ_TO(str_size_2.First(), tmp_size_2, "First()", "tmp_size_2");
#endif

    END_SUB_TEST;
}

static int TestStringCompare() {
    String8     str1("a");
    String8     str2("a");
    const char *str3 = "a";

    EQ_TO(str1, str2, "str1", "str2");
    EQ_TO(str2, str1, "str2", "str1");
    EQ_TO(str1, str3, "str1", "str3");
    EQ_TO(str2, str3, "str2", "str3");

    str1 = "abc";
    str2 = "abc";
    str3 = "abc";
    EQ_TO(str1, str2, "str1", "str2");
    EQ_TO(str2, str1, "str2", "str1");
    EQ_TO(str1, str3, "str1", "str3");
    EQ_TO(str2, str3, "str2", "str3");

    str1 = "a";
    str2 = "b";
    str3 = "c";
    NOT_EQ_TO(str1, str2, "str1", "str2");
    NOT_EQ_TO(str2, str1, "str2", "str1");
    NOT_EQ_TO(str1, str3, "str1", "str3");
    NOT_EQ_TO(str2, str3, "str2", "str3");

    str1 = "abc";
    str2 = "efg";
    str3 = "hij";
    NOT_EQ_TO(str1, str2, "str1", "str2");
    NOT_EQ_TO(str2, str1, "str2", "str1");
    NOT_EQ_TO(str1, str3, "str1", "str3");
    NOT_EQ_TO(str2, str3, "str2", "str3");

    str1 = "a";
    str2 = "ef";
    str3 = "abc";
    NOT_EQ_TO(str1, str2, "str1", "str2");
    NOT_EQ_TO(str2, str1, "str2", "str1");
    NOT_EQ_TO(str1, str3, "str1", "str3");
    NOT_EQ_TO(str2, str3, "str2", "str3");

    str1 = "";
    EQ_FALSE(str1.IsEqual(" ", 1), "IsEqual");

    str1 = "";
    str2 = "";
    EQ_TRUE((str1 >= str2), "IsBigger");
    EQ_TRUE((str1 <= str2), "IsLess");
    EQ_FALSE((str1 > str2), "IsBigger");
    EQ_FALSE((str1 < str2), "IsLess");

    str1 = "a";
    str2 = "a";
    EQ_TRUE((str1 >= str2), "IsBigger");
    EQ_TRUE((str1 <= str2), "IsLess");
    EQ_FALSE((str1 > str2), "IsBigger");
    EQ_FALSE((str1 < str2), "IsLess");

    str1 = "a";
    str2 = "A";
    EQ_TRUE((str1 >= str2), "IsBigger");
    EQ_TRUE((str1 > str2), "IsBigger");
    EQ_FALSE((str1 <= str2), "IsLess");
    EQ_FALSE((str1 < str2), "IsLess");
    //
    EQ_FALSE((str2 >= str1), "IsBigger");
    EQ_FALSE((str2 > str1), "IsBigger");
    EQ_TRUE((str2 <= str1), "IsLess");
    EQ_TRUE((str2 < str1), "IsLess");

    str1 = "a";
    str2 = "B";
    EQ_TRUE((str1 >= str2), "IsBigger");
    EQ_TRUE((str1 > str2), "IsBigger");
    EQ_FALSE((str1 <= str2), "IsLess");
    EQ_FALSE((str1 < str2), "IsLess");
    //
    EQ_FALSE((str2 >= str1), "IsBigger");
    EQ_FALSE((str2 > str1), "IsBigger");
    EQ_TRUE((str2 <= str1), "IsLess");
    EQ_TRUE((str2 < str1), "IsLess");

    str1 = "aa";
    str2 = "aA";
    EQ_TRUE((str1 >= str2), "IsBigger");
    EQ_TRUE((str1 > str2), "IsBigger");
    EQ_FALSE((str1 <= str2), "IsLess");
    EQ_FALSE((str1 < str2), "IsLess");
    //
    EQ_FALSE((str2 >= str1), "IsBigger");
    EQ_FALSE((str2 > str1), "IsBigger");
    EQ_TRUE((str2 <= str1), "IsLess");
    EQ_TRUE((str2 < str1), "IsLess");

    str1 = "2021";
    str2 = "2018";
    EQ_TRUE((str1 >= str2), "IsBigger");
    EQ_TRUE((str1 > str2), "IsBigger");
    EQ_FALSE((str1 <= str2), "IsLess");
    EQ_FALSE((str1 < str2), "IsLess");
    //
    EQ_FALSE((str2 >= str1), "IsBigger");
    EQ_FALSE((str2 > str1), "IsBigger");
    EQ_TRUE((str2 <= str1), "IsLess");
    EQ_TRUE((str2 < str1), "IsLess");

    END_SUB_TEST;
}

static int TestString2() {
    String8 str1;
    String8 str2;

    str1.Insert("a", 1);
    EQ_VALUE(str1.Length(), 1, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "a", "str1");

    str1.Insert("bc", 2);
    EQ_VALUE(str1.Length(), 3, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "abc", "str1");

    str1.Insert("", 0);
    EQ_VALUE(str1.Length(), 3, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "abc", "str1");

    str2 = String8::Merge(str1, String8("def"));
    EQ_VALUE(str2.Length(), 6, "Length");
    NOT_EQ_TO(str2.First(), nullptr, "First()", "null");
    EQ_VALUE(str2.First()[str2.Length()], 0, "First()[Length]");
    EQ_VALUE(str2, "abcdef", "str2");

    str2 = String8::Merge(str2, String8(""));
    EQ_VALUE(str2.Length(), 6, "Length");
    NOT_EQ_TO(str2.First(), nullptr, "First()", "null");
    EQ_VALUE(str2.First()[str2.Length()], 0, "First()[Length]");
    EQ_VALUE(str2, "abcdef", "str2");

    str2 = String8::Merge(String8(""), str2);
    EQ_VALUE(str2.Length(), 6, "Length");
    NOT_EQ_TO(str2.First(), nullptr, "First()", "null");
    EQ_VALUE(str2.First()[str2.Length()], 0, "First()[Length]");
    EQ_VALUE(str2, "abcdef", "str2");

    str1.Reset();
    for (SizeT i = 0; i < String8::ShortStringMax; i++) {
        str1 += "A";
    }

    str1 += "_";
    EQ_VALUE(str1.Length(), (String8::ShortStringMax + 1), "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "AAAAAAAAAAAAAA_", "str1");

    str1.Reset();
    for (SizeT i = 3; i < String8::ShortStringMax; i++) {
        str1 += "A";
    }

    str1 += "_";
    EQ_VALUE(str1.Length(), (String8::ShortStringMax - 2), "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "AAAAAAAAAAA_", "str1");

    str1 += "B";
    EQ_VALUE(str1.Length(), 13, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "AAAAAAAAAAA_B", "str1");

    str1 += "_C";
    EQ_VALUE(str1.Length(), 15, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "AAAAAAAAAAA_B_C", "str1");

    str1.Reset();
    str1 += "a";
    EQ_VALUE(str1.Length(), 1, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "a", "str1");

    str1 += "bc";
    EQ_VALUE(str1.Length(), 3, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "abc", "str1");

    str2 = "def";
    str1 += str2;
    EQ_VALUE(str1.Length(), 6, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "abcdef", "str1");

    str2 = "ghi";
    str1 += static_cast<String8 &&>(str2);
    EQ_VALUE(str1.Length(), 9, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "abcdefghi", "str1");
    EQ_VALUE(str2.Length(), 0, "Length");
    EQ_TO(str2.First(), nullptr, "First()", "null");

    str1 = str1 + "";
    EQ_VALUE(str1.Length(), 9, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "abcdefghi", "str1");

    str1 = str1 + "gkl";
    EQ_VALUE(str1.Length(), 12, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "abcdefghigkl", "str1");

    str2 = "123";
    str1 = str1 + str2;
    EQ_VALUE(str1.Length(), 15, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "abcdefghigkl123", "str1");

    str1 = "aaaaaaaaaaaaaaaaaaaaaaaa";
    str1 += "aaaaaaaaaaaaaaaaaaaaaaaa";
    EQ_VALUE(str1.Length(), 48, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "str1");

    str1.Reset();
    str1 = "1234";
    str2 = "5678";
    str1 = str1 + static_cast<String8 &&>(str2);
    EQ_VALUE(str1.Length(), 8, "Length");
    NOT_EQ_TO(str1.First(), nullptr, "First()", "null");
    EQ_VALUE(str1.First()[str1.Length()], 0, "First()[Length]");
    EQ_VALUE(str1, "12345678", "str1");
    EQ_VALUE(str2.Length(), 0, "Length");
    EQ_TO(str2.First(), nullptr, "First()", "null");

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

    EQ_TRUE(StringUtils::IsEqual(&(sis.str[0]), "12345678", 8), "SimpleStream");

    END_SUB_TEST;
}

static int TestTrim() {
    String8 str1("");

    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 0, "length");

    str1 = " ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 0, "length");

    str1 = "  ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 0, "length");

    str1 = "                    ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 0, "length");

    str1 = " a";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 1, "length");

    str1 = "a ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 1, "length");

    str1 = " a ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 1, "length");

    str1 = "  a";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 1, "length");

    str1 = "a  ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 1, "length");

    str1 = "  a  ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 1, "length");

    str1 = "                      a";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 1, "length");

    str1 = "a                      ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 1, "length");

    str1 = "                      a                      ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 1, "length");

    str1 = " abcd";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 4, "length");

    str1 = "abcd ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 4, "length");

    str1 = " abcd ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 4, "length");

    str1 = "  abcd";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 4, "length");

    str1 = "abcd  ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 4, "length");

    str1 = "  abcd  ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 4, "length");

    str1 = "                      abcd";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 4, "length");

    str1 = "abcd                      ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 4, "length");

    str1 = "                      abcd                      ";
    str1 = String8::Trim(str1);
    EQ_VALUE(str1.Length(), 4, "length");

    END_SUB_TEST;
}

static int RunStringTests() {
    STARTING_TEST("String.hpp");

    START_TEST("String Test 1", TestString1);
    START_TEST("String::Compare", TestStringCompare);
    START_TEST("String Test 2", TestString2);
    START_TEST("String::Trim", TestTrim);

    END_TEST("String.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
