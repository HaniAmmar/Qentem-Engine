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

#include "QTest.hpp"
#include "String.hpp"

#ifndef QENTEM_STRING_TESTS_H
#define QENTEM_STRING_TESTS_H

namespace Qentem {
namespace Test {

using QString = String<char>;

static void TestString1(QTest &helper) {
    SizeT length;
    char *str_ptr;

    QString str1;
    helper.Equal(str1.Length(), SizeT{0U}, "Length", __LINE__);
    helper.Equal(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.Last(), nullptr, "Last()", "null", __LINE__);

    QString str2{"0123456789"};
    helper.Equal(str2.Length(), SizeT{10U}, "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(str2.Last(), nullptr, "Last()", "null", __LINE__);
    helper.Equal(str2.First()[str2.Length()], char{0}, "First()[Length]", __LINE__);

    str2.Reset();
    helper.Equal(str2.Length(), SizeT{0U}, "Length", __LINE__);
    helper.Equal(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str2.Last(), nullptr, "Last()", "null", __LINE__);

    str1 = QString("abcd");
    helper.Equal(str1.Length(), SizeT{4U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "abcd", "str1", __LINE__);
    helper.NotEqual(str1, "abcdef", "str1", "abcdef", __LINE__);

    str1 = QString("abcd-bcd");
    helper.Equal(str1.Length(), SizeT{8U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "abcd-bcd", "str1", __LINE__);

    str1 = QString("abcd", 2U);
    helper.Equal(str1.Length(), SizeT{2U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "ab", "str1", __LINE__);
    helper.NotEqual(str1, "abcd", "str1", "abcd", __LINE__);

    str1 = "ABCDEF0123456789ABCDEF0123456789";
    helper.Equal(str1.Length(), SizeT{32U}, "Length", __LINE__);

    length  = str1.Length();
    str_ptr = str1.Detach();
    helper.NotEqual(str_ptr, nullptr, "Detach", "null", __LINE__);

    str2 = QString(str_ptr, length); // Manage
    helper.Equal(str2.First(), str_ptr, "First()", "str_ptr", __LINE__);
    helper.Equal(str2.Length(), length, "Length", __LINE__);

    str1 = Memory::Move(str2); // Move
    helper.Equal(str1.First(), str_ptr, "First()", __LINE__);
    helper.Equal(str1.Length(), length, "Length", __LINE__);
    helper.Equal(str2.Storage(), nullptr, "Storage()", "null", __LINE__);
    helper.Equal(str2.Length(), SizeT{0U}, "Length", __LINE__);
    helper.Equal(str2.First(), nullptr, "First()", "null", __LINE__);

    str2 = "abcdef"; // Copy
    helper.Equal(str2.Length(), SizeT{6U}, "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str2.First()[str2.Length()], char{0}, "First()[Length]", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(str2.First(), "abcdef", 5U), "IsEqual", __LINE__);

    str2 = str1; // Copy
    helper.Equal(str2.Length(), str1.Length(), "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(str2.First(), str1.First(), "First()", str1.First(), __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), "IsEqual", __LINE__);

    str2.Reset();
    str1 = "efg";         // Copy
    str2 = QString(str1); // Copy
    helper.Equal(str2.Length(), str1.Length(), "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(str2.First(), str1.First(), "First()", str1.First(), __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), "IsEqual", __LINE__);

    str1 = "hig";         // Copy
    str2 = QString(str1); // Copy over a value
    helper.Equal(str2.Length(), str1.Length(), "Length", __LINE__);
    helper.NotEqual(str2.First(), str1.First(), "First()", str1.First(), __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), "IsEqual", __LINE__);

    length = str1.Length();
    str2   = QString(Memory::Move(str1));
    helper.Equal(str1.Storage(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.Length(), SizeT{0U}, "Length", __LINE__);
    helper.Equal(str2.Length(), length, "Length", __LINE__);

    str1 = "A";
    helper.Equal(str1.Length(), SizeT{1U}, "Length", __LINE__);
    str_ptr = str1.Detach();
    helper.Equal(str_ptr[0], 'A', "str_ptr[0]", "A", __LINE__);
    Memory::Deallocate(str_ptr);

    char         *tmp_size_2 = Memory::Allocate<char>(2);
    const QString str_size_2 = QString(tmp_size_2, 2U);

    if (Config::ShortStringOptimization) {
        helper.NotEqual(str_size_2.First(), tmp_size_2, "First()", "tmp_size_2", __LINE__);
    } else {
        helper.Equal(str_size_2.First(), tmp_size_2, "First()", "tmp_size_2", __LINE__);
    }
}

static void TestStringCompare(QTest &helper) {
    QString     str1("a");
    QString     str2("a");
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

static void TestString2(QTest &helper) {
    QString str1;
    QString str2;

    str1.Write("a", 1U);
    helper.Equal(str1.Length(), SizeT{1U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "a", "str1", __LINE__);

    str1.Write("bc", 2U);
    helper.Equal(str1.Length(), SizeT{3U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "abc", "str1", __LINE__);

    str1.Write("", 0U);
    helper.Equal(str1.Length(), SizeT{3U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "abc", "str1", __LINE__);

    str2 = QString::Merge(str1, QString("def"));
    helper.Equal(str2.Length(), SizeT{6U}, "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str2.First()[str2.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str2, "abcdef", "str2", __LINE__);

    str2 = QString::Merge(str2, QString(""));
    helper.Equal(str2.Length(), SizeT{6U}, "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str2.First()[str2.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str2, "abcdef", "str2", __LINE__);

    str2 = QString::Merge(QString(""), str2);
    helper.Equal(str2.Length(), SizeT{6U}, "Length", __LINE__);
    helper.NotEqual(str2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str2.First()[str2.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str2, "abcdef", "str2", __LINE__);

    if (Config::ShortStringOptimization) {
        char sstr[32];
        str1.Reset();
        for (SizeT i = 0; i < QString::ShortStringMax; i++) {
            str1 += "A";
            sstr[i] = 'A';
        }

        str1 += "_";
        sstr[QString::ShortStringMax]     = '_';
        sstr[QString::ShortStringMax + 1] = '\0';

        helper.Equal(str1.Length(), (QString::ShortStringMax + 1U), "Length", __LINE__);
        helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
        helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
        helper.Equal(str1, &(sstr[0]), "str1", __LINE__);

        str1.Reset();
        SizeT sso_x = 0;
        for (SizeT i = 3; i < QString::ShortStringMax; i++) {
            str1 += "A";
            sstr[sso_x] = 'A';
            ++sso_x;
        }

        str1 += "_";
        sstr[sso_x]   = '_';
        sstr[++sso_x] = '\0';
        helper.Equal(str1.Length(), (QString::ShortStringMax - 2U), "Length", __LINE__);
        helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
        helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
        helper.Equal(str1, &(sstr[0]), "str1", __LINE__);

        str1 += "B";
        sstr[sso_x]   = 'B';
        sstr[++sso_x] = '\0';
        helper.Equal(str1.Length(), (QString::ShortStringMax - 1), "Length", __LINE__);
        helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
        helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
        helper.Equal(str1, &(sstr[0]), "str1", __LINE__);

        str1 += "_C";
        sstr[sso_x]   = '_';
        sstr[++sso_x] = 'C';
        sstr[++sso_x] = '\0';
        helper.Equal(str1.Length(), (QString::ShortStringMax + 1), "Length", __LINE__);
        helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
        helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
        helper.Equal(str1, &(sstr[0]), "str1", __LINE__);
    }

    str1.Reset();
    str1 += "a";
    helper.Equal(str1.Length(), SizeT{1U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "a", "str1", __LINE__);

    str1 += "bc";
    helper.Equal(str1.Length(), SizeT{3U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "abc", "str1", __LINE__);

    str2 = "def";
    str1 += str2;
    helper.Equal(str1.Length(), SizeT{6U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "abcdef", "str1", __LINE__);

    str2 = "ghi";
    str1 += Memory::Move(str2);
    helper.Equal(str1.Length(), SizeT{9U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "abcdefghi", "str1", __LINE__);
    helper.Equal(str2.Length(), SizeT{0U}, "Length", __LINE__);
    helper.Equal(str2.First(), nullptr, "First()", "null", __LINE__);

    str1 = str1 + "";
    helper.Equal(str1.Length(), SizeT{9U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "abcdefghi", "str1", __LINE__);

    str1 = str1 + "gkl";
    helper.Equal(str1.Length(), SizeT{12U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "abcdefghigkl", "str1", __LINE__);

    str2 = "123";
    str1 = str1 + str2;
    helper.Equal(str1.Length(), SizeT{15U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "abcdefghigkl123", "str1", __LINE__);

    str1 = "123";
    str1 += '4';
    helper.Equal(str1.Length(), SizeT{4U}, "Length", __LINE__);
    helper.Equal(str1, "1234", "str1", __LINE__);

    str1 = "aaaaaaaaaaaaaaaaaaaaaaaa";
    str1 += '4';
    helper.Equal(str1.Length(), SizeT{25U}, "Length", __LINE__);
    helper.Equal(str1, "aaaaaaaaaaaaaaaaaaaaaaaa4", "str1", __LINE__);

    str1 = "aaaaaaaaaaaaaaaaaaaaaaaa";
    str1 += "aaaaaaaaaaaaaaaaaaaaaaaa";
    helper.Equal(str1.Length(), SizeT{48U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "str1", __LINE__);

    str1.Reset();
    str1 = "1234";
    str2 = "5678";
    str1 = str1 + Memory::Move(str2);
    helper.Equal(str1.Length(), SizeT{8U}, "Length", __LINE__);
    helper.NotEqual(str1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(str1.First()[str1.Length()], char{0}, "First()[Length]", __LINE__);
    helper.Equal(str1, "12345678", "str1", __LINE__);
    helper.Equal(str2.Length(), SizeT{0U}, "Length", __LINE__);
    helper.Equal(str2.First(), nullptr, "First()", "null", __LINE__);

    struct SimpleStream {
        char          str[8]{0};
        const SizeT32 max = 8;
        SizeT32       index{0};

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

    str1.Reset();
    str1 = "1234";

    SizeT total = 0;

    for (const auto &item : str1) {
        total = SizeT(total + (SizeT(item) - SizeT{'0'}));
    }

    helper.Equal(total, SizeT{10}, "total", __LINE__);

    for (auto &item : str1) {
        item += '0';
    }

    helper.Equal(str1, "abcd", "str1.First()", __LINE__);

    str1.Reset();
    str1 = "0123456789012345678901234567890123456789";

    str1.StepBack(SizeT{1});
    helper.Equal(str1, "012345678901234567890123456789012345678", "str1.First()", __LINE__);
    helper.Equal(str1.Length(), SizeT{39}, "Length", __LINE__);

    str1.StepBack(SizeT{10});
    helper.Equal(str1, "01234567890123456789012345678", "str1.First()", __LINE__);
    helper.Equal(str1.Length(), SizeT{29}, "Length", __LINE__);

    str1.StepBack(SizeT{25});
    helper.Equal(str1, "0123", "str1.First()", __LINE__);
    helper.Equal(str1.Length(), SizeT{4}, "Length", __LINE__);

    str1.StepBack(SizeT{3});
    helper.Equal(str1, "0", "str1.First()", __LINE__);
    helper.Equal(str1.Length(), SizeT{1}, "Length", __LINE__);

    str1.StepBack(SizeT{1});
    helper.Equal(str1.Length(), SizeT{0}, "Length", __LINE__);

    str1.Reset();
    str1 = "1234";

    str1.StepBack(SizeT{1});
    helper.Equal(str1, "123", "str1.First()", __LINE__);
    helper.Equal(str1.Length(), SizeT{3U}, "Length", __LINE__);

    str1.StepBack(SizeT{2});
    helper.Equal(str1, "1", "str1.First()", __LINE__);
    helper.Equal(str1.Length(), SizeT{1U}, "Length", __LINE__);

    str1.StepBack(SizeT{1});
    helper.Equal(str1.Length(), SizeT{0U}, "Length", __LINE__);

    str1.Reset();
    str1 = "01234567890123";
    str1.StepBack(SizeT{10});
    helper.Equal(str1.Length(), SizeT{4U}, "Length", __LINE__);

    str1.Reset();
    str1 = "0123456789012";
    str1.StepBack(SizeT{10});
    helper.Equal(str1.Length(), SizeT{3U}, "Length", __LINE__);
}

static void TestString3(QTest &helper) {
    QString str1;

    str1 = "123";

    str1.Reverse();
    helper.Equal(str1.Length(), SizeT{3U}, "Length", __LINE__);
    helper.Equal(str1, "321", "str1", __LINE__);

    str1.Reverse(0);
    helper.Equal(str1.Length(), SizeT{3U}, "Length", __LINE__);
    helper.Equal(str1, "123", "str1", __LINE__);

    str1 = "0123456789";

    str1.Reverse();
    helper.Equal(str1.Length(), SizeT{10U}, "Length", __LINE__);
    helper.Equal(str1, "9876543210", "str1", __LINE__);

    str1.Reverse();
    helper.Equal(str1.Length(), SizeT{10U}, "Length", __LINE__);
    helper.Equal(str1, "0123456789", "str1", __LINE__);

    str1.Reverse(3);
    helper.Equal(str1.Length(), SizeT{10U}, "Length", __LINE__);
    helper.Equal(str1, "0129876543", "str1", __LINE__);

    str1.Reverse(7);
    helper.Equal(str1.Length(), SizeT{10U}, "Length", __LINE__);
    helper.Equal(str1, "0129876345", "str1", __LINE__);

    str1.Reverse(100); // Nothing will happened
    helper.Equal(str1.Length(), SizeT{10U}, "Length", __LINE__);
    helper.Equal(str1, "0129876345", "str1", __LINE__);
    ////////////////////////////////////////////
    str1 = "9";

    str1.InsertAt('0', 0);
    helper.Equal(str1.Length(), SizeT{2}, "Length", __LINE__);
    helper.Equal(str1, "09", "str1", __LINE__);

    str1.InsertAt('4', 1);
    helper.Equal(str1.Length(), SizeT{3}, "Length", __LINE__);
    helper.Equal(str1, "049", "str1", __LINE__);

    str1.InsertAt('4', 10); // Nothing will happened
    helper.Equal(str1.Length(), SizeT{3}, "Length", __LINE__);
    helper.Equal(str1, "049", "str1", __LINE__);

    str1.InsertAt('2', 1);
    str1.InsertAt('1', 1);

    helper.Equal(str1.Length(), SizeT{5}, "Length", __LINE__);
    helper.Equal(str1, "01249", "str1", __LINE__);

    str1.InsertAt('3', 3);
    str1.InsertAt('5', 5);

    helper.Equal(str1.Length(), SizeT{7}, "Length", __LINE__);
    helper.Equal(str1, "0123459", "str1", __LINE__);

    str1.InsertAt('8', 6);
    str1.InsertAt('7', 6);
    str1.InsertAt('6', 6);

    helper.Equal(str1.Length(), SizeT{10}, "Length", __LINE__);
    helper.Equal(str1, "0123456789", "str1", __LINE__);
}

static void TestStringTrim(QTest &helper) {
    QString str1("");

    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{0U}, "length", __LINE__);

    str1 = " ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{0U}, "length", __LINE__);

    str1 = "  ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{0U}, "length", __LINE__);

    str1 = "                    ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{0U}, "length", __LINE__);

    str1 = " a";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{1U}, "length", __LINE__);

    str1 = "a ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{1U}, "length", __LINE__);

    str1 = " a ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{1U}, "length", __LINE__);

    str1 = "  a";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{1U}, "length", __LINE__);

    str1 = "a  ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{1U}, "length", __LINE__);

    str1 = "  a  ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{1U}, "length", __LINE__);

    str1 = "                      a";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{1U}, "length", __LINE__);

    str1 = "a                      ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{1U}, "length", __LINE__);

    str1 = "                      a                      ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{1U}, "length", __LINE__);

    str1 = " abcd";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{4U}, "length", __LINE__);

    str1 = "abcd ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{4U}, "length", __LINE__);

    str1 = " abcd ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{4U}, "length", __LINE__);

    str1 = "  abcd";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{4U}, "length", __LINE__);

    str1 = "abcd  ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{4U}, "length", __LINE__);

    str1 = "  abcd  ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{4U}, "length", __LINE__);

    str1 = "                      abcd";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{4U}, "length", __LINE__);

    str1 = "abcd                      ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{4U}, "length", __LINE__);

    str1 = "                      abcd                      ";
    str1 = QString::Trim(str1);
    helper.Equal(str1.Length(), SizeT{4U}, "length", __LINE__);
}

static int RunStringTests() {
    QTest helper{"String.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("String Test 1", TestString1);
    helper.Test("String::Compare", TestStringCompare);
    helper.Test("String Test 2", TestString2);
    helper.Test("String Test 2", TestString3);
    helper.Test("String::Trim", TestStringTrim);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
