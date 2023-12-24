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
    helper.IsEqual(str1.Length(), SizeT{0U}, __LINE__);
    helper.IsNull(str1.First(), __LINE__);
    helper.IsNull(str1.Last(), __LINE__);

    QString str2{"0123456789"};
    helper.IsEqual(str2.Length(), SizeT{10U}, __LINE__);
    helper.IsNotNull(str2.First(), __LINE__);
    helper.IsNotNull(str2.Last(), __LINE__);
    helper.IsEqual(str2.First()[str2.Length()], char{0}, __LINE__);

    str2.Reset();
    helper.IsEqual(str2.Length(), SizeT{0U}, __LINE__);
    helper.IsNull(str2.First(), __LINE__);
    helper.IsNull(str2.Last(), __LINE__);

    str1 = QString("abcd");
    helper.IsEqual(str1.Length(), SizeT{4U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "abcd", __LINE__);
    helper.IsNotEqual(str1, "abcdef", __LINE__);

    str1 = QString("abcd-bcd");
    helper.IsEqual(str1.Length(), SizeT{8U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "abcd-bcd", __LINE__);

    str1 = QString("abcd", 2U);
    helper.IsEqual(str1.Length(), SizeT{2U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "ab", __LINE__);
    helper.IsNotEqual(str1, "abcd", __LINE__);

    str1 = "ABCDEF0123456789ABCDEF0123456789";
    helper.IsEqual(str1.Length(), SizeT{32U}, __LINE__);

    length  = str1.Length();
    str_ptr = str1.Detach();
    helper.IsNotNull(str_ptr, __LINE__);

    str2 = QString(str_ptr, length); // Manage
    helper.IsEqual(str2.First(), str_ptr, __LINE__);
    helper.IsEqual(str2.Length(), length, __LINE__);

    str1 = Memory::Move(str2); // Move
    helper.IsEqual(str1.First(), str_ptr, __LINE__);
    helper.IsEqual(str1.Length(), length, __LINE__);
    helper.IsNull(str2.Storage(), __LINE__);
    helper.IsEqual(str2.Length(), SizeT{0U}, __LINE__);
    helper.IsNull(str2.First(), __LINE__);

    str2 = "abcdef"; // Copy
    helper.IsEqual(str2.Length(), SizeT{6U}, __LINE__);
    helper.IsNotNull(str2.First(), __LINE__);
    helper.IsEqual(str2.First()[str2.Length()], char{0}, __LINE__);
    helper.IsTrue(StringUtils::IsEqual(str2.First(), "abcdef", 5U), __LINE__);

    str2 = str1; // Copy
    helper.IsEqual(str2.Length(), str1.Length(), __LINE__);
    helper.IsNotNull(str2.First(), __LINE__);
    helper.IsNotEqual(str2.First(), str1.First(), __LINE__);
    helper.IsTrue(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), __LINE__);

    str2.Reset();
    str1 = "efg";         // Copy
    str2 = QString(str1); // Copy
    helper.IsEqual(str2.Length(), str1.Length(), __LINE__);
    helper.IsNotNull(str2.First(), __LINE__);
    helper.IsNotEqual(str2.First(), str1.First(), __LINE__);
    helper.IsTrue(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), __LINE__);

    str1 = "hig";         // Copy
    str2 = QString(str1); // Copy over a value
    helper.IsEqual(str2.Length(), str1.Length(), __LINE__);
    helper.IsNotEqual(str2.First(), str1.First(), __LINE__);
    helper.IsTrue(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), __LINE__);

    length = str1.Length();
    str2   = QString(Memory::Move(str1));
    helper.IsNull(str1.Storage(), __LINE__);
    helper.IsNull(str1.First(), __LINE__);
    helper.IsEqual(str1.Length(), SizeT{0U}, __LINE__);
    helper.IsEqual(str2.Length(), length, __LINE__);

    str1 = "A";
    helper.IsEqual(str1.Length(), SizeT{1U}, __LINE__);
    str_ptr = str1.Detach();
    helper.IsEqual(str_ptr[0], 'A', __LINE__);
    Memory::Deallocate(str_ptr);

    char         *tmp_size_2 = Memory::Allocate<char>(2);
    const QString str_size_2 = QString(tmp_size_2, 2U);

    if (Config::ShortStringOptimization) {
        helper.IsNotEqual(str_size_2.First(), tmp_size_2, __LINE__);
    } else {
        helper.IsEqual(str_size_2.First(), tmp_size_2, __LINE__);
    }
}

static void TestStringCompare(QTest &helper) {
    QString     str1("a");
    QString     str2("a");
    const char *str3 = "a";

    helper.IsEqual(str1, str2, __LINE__);
    helper.IsEqual(str2, str1, __LINE__);
    helper.IsEqual(str1, str3, __LINE__);
    helper.IsEqual(str2, str3, __LINE__);

    str1 = "abc";
    str2 = "abc";
    str3 = "abc";
    helper.IsEqual(str1, str2, __LINE__);
    helper.IsEqual(str2, str1, __LINE__);
    helper.IsEqual(str1, str3, __LINE__);
    helper.IsEqual(str2, str3, __LINE__);

    str1 = "a";
    str2 = "b";
    str3 = "c";
    helper.IsNotEqual(str1, str2, __LINE__);
    helper.IsNotEqual(str2, str1, __LINE__);
    helper.IsNotEqual(str1, str3, __LINE__);
    helper.IsNotEqual(str2, str3, __LINE__);

    str1 = "abc";
    str2 = "efg";
    str3 = "hij";
    helper.IsNotEqual(str1, str2, __LINE__);
    helper.IsNotEqual(str2, str1, __LINE__);
    helper.IsNotEqual(str1, str3, __LINE__);
    helper.IsNotEqual(str2, str3, __LINE__);

    str1 = "a";
    str2 = "ef";
    str3 = "abc";
    helper.IsNotEqual(str1, str2, __LINE__);
    helper.IsNotEqual(str2, str1, __LINE__);
    helper.IsNotEqual(str1, str3, __LINE__);
    helper.IsNotEqual(str2, str3, __LINE__);

    str1 = "";
    helper.IsFalse(str1.IsEqual(" ", 1U), __LINE__);

    str1 = "";
    str2 = "";
    helper.IsTrue((str1 >= str2), __LINE__);
    helper.IsTrue((str1 <= str2), __LINE__);
    helper.IsFalse((str1 > str2), __LINE__);
    helper.IsFalse((str1 < str2), __LINE__);

    str1 = "a";
    str2 = "a";
    helper.IsTrue((str1 >= str2), __LINE__);
    helper.IsTrue((str1 <= str2), __LINE__);
    helper.IsFalse((str1 > str2), __LINE__);
    helper.IsFalse((str1 < str2), __LINE__);

    str1 = "a";
    str2 = "A";
    helper.IsTrue((str1 >= str2), __LINE__);
    helper.IsTrue((str1 > str2), __LINE__);
    helper.IsFalse((str1 <= str2), __LINE__);
    helper.IsFalse((str1 < str2), __LINE__);
    //
    helper.IsFalse((str2 >= str1), __LINE__);
    helper.IsFalse((str2 > str1), __LINE__);
    helper.IsTrue((str2 <= str1), __LINE__);
    helper.IsTrue((str2 < str1), __LINE__);

    str1 = "a";
    str2 = "B";
    helper.IsTrue((str1 >= str2), __LINE__);
    helper.IsTrue((str1 > str2), __LINE__);
    helper.IsFalse((str1 <= str2), __LINE__);
    helper.IsFalse((str1 < str2), __LINE__);
    //
    helper.IsFalse((str2 >= str1), __LINE__);
    helper.IsFalse((str2 > str1), __LINE__);
    helper.IsTrue((str2 <= str1), __LINE__);
    helper.IsTrue((str2 < str1), __LINE__);

    str1 = "aa";
    str2 = "aA";
    helper.IsTrue((str1 >= str2), __LINE__);
    helper.IsTrue((str1 > str2), __LINE__);
    helper.IsFalse((str1 <= str2), __LINE__);
    helper.IsFalse((str1 < str2), __LINE__);
    //
    helper.IsFalse((str2 >= str1), __LINE__);
    helper.IsFalse((str2 > str1), __LINE__);
    helper.IsTrue((str2 <= str1), __LINE__);
    helper.IsTrue((str2 < str1), __LINE__);

    str1 = "2021";
    str2 = "2018";
    helper.IsTrue((str1 >= str2), __LINE__);
    helper.IsTrue((str1 > str2), __LINE__);
    helper.IsFalse((str1 <= str2), __LINE__);
    helper.IsFalse((str1 < str2), __LINE__);
    //
    helper.IsFalse((str2 >= str1), __LINE__);
    helper.IsFalse((str2 > str1), __LINE__);
    helper.IsTrue((str2 <= str1), __LINE__);
    helper.IsTrue((str2 < str1), __LINE__);
}

static void TestString2(QTest &helper) {
    QString str1;
    QString str2;

    str1.Write("a", 1U);
    helper.IsEqual(str1.Length(), SizeT{1U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "a", __LINE__);

    str1.Write("bc", 2U);
    helper.IsEqual(str1.Length(), SizeT{3U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "abc", __LINE__);

    str1.Write("", 0U);
    helper.IsEqual(str1.Length(), SizeT{3U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "abc", __LINE__);

    str2 = QString::Merge(str1, QString("def"));
    helper.IsEqual(str2.Length(), SizeT{6U}, __LINE__);
    helper.IsNotNull(str2.First(), __LINE__);
    helper.IsEqual(str2.First()[str2.Length()], char{0}, __LINE__);
    helper.IsEqual(str2, "abcdef", __LINE__);

    str2 = QString::Merge(str2, QString(""));
    helper.IsEqual(str2.Length(), SizeT{6U}, __LINE__);
    helper.IsNotNull(str2.First(), __LINE__);
    helper.IsEqual(str2.First()[str2.Length()], char{0}, __LINE__);
    helper.IsEqual(str2, "abcdef", __LINE__);

    str2 = QString::Merge(QString(""), str2);
    helper.IsEqual(str2.Length(), SizeT{6U}, __LINE__);
    helper.IsNotNull(str2.First(), __LINE__);
    helper.IsEqual(str2.First()[str2.Length()], char{0}, __LINE__);
    helper.IsEqual(str2, "abcdef", __LINE__);

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

        helper.IsEqual(str1.Length(), (QString::ShortStringMax + 1U), __LINE__);
        helper.IsNotNull(str1.First(), __LINE__);
        helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
        helper.IsEqual(str1, &(sstr[0]), __LINE__);

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
        helper.IsEqual(str1.Length(), (QString::ShortStringMax - 2U), __LINE__);
        helper.IsNotNull(str1.First(), __LINE__);
        helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
        helper.IsEqual(str1, &(sstr[0]), __LINE__);

        str1 += "B";
        sstr[sso_x]   = 'B';
        sstr[++sso_x] = '\0';
        helper.IsEqual(str1.Length(), (QString::ShortStringMax - 1), __LINE__);
        helper.IsNotNull(str1.First(), __LINE__);
        helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
        helper.IsEqual(str1, &(sstr[0]), __LINE__);

        str1 += "_C";
        sstr[sso_x]   = '_';
        sstr[++sso_x] = 'C';
        sstr[++sso_x] = '\0';
        helper.IsEqual(str1.Length(), (QString::ShortStringMax + 1), __LINE__);
        helper.IsNotNull(str1.First(), __LINE__);
        helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
        helper.IsEqual(str1, &(sstr[0]), __LINE__);
    }

    str1.Reset();
    str1 += "a";
    helper.IsEqual(str1.Length(), SizeT{1U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "a", __LINE__);

    str1 += "bc";
    helper.IsEqual(str1.Length(), SizeT{3U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "abc", __LINE__);

    str2 = "def";
    str1 += str2;
    helper.IsEqual(str1.Length(), SizeT{6U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "abcdef", __LINE__);

    str2 = "ghi";
    str1 += Memory::Move(str2);
    helper.IsEqual(str1.Length(), SizeT{9U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "abcdefghi", __LINE__);
    helper.IsEqual(str2.Length(), SizeT{0U}, __LINE__);
    helper.IsNull(str2.First(), __LINE__);

    str1 = str1 + "";
    helper.IsEqual(str1.Length(), SizeT{9U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "abcdefghi", __LINE__);

    str1 = str1 + "gkl";
    helper.IsEqual(str1.Length(), SizeT{12U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "abcdefghigkl", __LINE__);

    str2 = "123";
    str1 = str1 + str2;
    helper.IsEqual(str1.Length(), SizeT{15U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "abcdefghigkl123", __LINE__);

    str1 = "123";
    str1 += '4';
    helper.IsEqual(str1.Length(), SizeT{4U}, __LINE__);
    helper.IsEqual(str1, "1234", __LINE__);

    str1 = "aaaaaaaaaaaaaaaaaaaaaaaa";
    str1 += '4';
    helper.IsEqual(str1.Length(), SizeT{25U}, __LINE__);
    helper.IsEqual(str1, "aaaaaaaaaaaaaaaaaaaaaaaa4", __LINE__);

    str1 = "aaaaaaaaaaaaaaaaaaaaaaaa";
    str1 += "aaaaaaaaaaaaaaaaaaaaaaaa";
    helper.IsEqual(str1.Length(), SizeT{48U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", __LINE__);

    str1.Reset();
    str1 = "1234";
    str2 = "5678";
    str1 = str1 + Memory::Move(str2);
    helper.IsEqual(str1.Length(), SizeT{8U}, __LINE__);
    helper.IsNotNull(str1.First(), __LINE__);
    helper.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    helper.IsEqual(str1, "12345678", __LINE__);
    helper.IsEqual(str2.Length(), SizeT{0U}, __LINE__);
    helper.IsNull(str2.First(), __LINE__);

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

    helper.IsTrue(StringUtils::IsEqual(&(sis.str[0]), "12345678", 8U), __LINE__);

    str1.Reset();
    str1 = "1234";

    SizeT total = 0;

    for (const auto &item : str1) {
        total = SizeT(total + (SizeT(item) - SizeT{'0'}));
    }

    helper.IsEqual(total, SizeT{10}, __LINE__);

    for (auto &item : str1) {
        item += '0';
    }

    helper.IsEqual(str1, "abcd", __LINE__);

    str1.Reset();
    str1 = "0123456789012345678901234567890123456789";

    str1.StepBack(SizeT{1});
    helper.IsEqual(str1, "012345678901234567890123456789012345678", __LINE__);
    helper.IsEqual(str1.Length(), SizeT{39}, __LINE__);

    str1.StepBack(SizeT{10});
    helper.IsEqual(str1, "01234567890123456789012345678", __LINE__);
    helper.IsEqual(str1.Length(), SizeT{29}, __LINE__);

    str1.StepBack(SizeT{25});
    helper.IsEqual(str1, "0123", __LINE__);
    helper.IsEqual(str1.Length(), SizeT{4}, __LINE__);

    str1.StepBack(SizeT{3});
    helper.IsEqual(str1, "0", __LINE__);
    helper.IsEqual(str1.Length(), SizeT{1}, __LINE__);

    str1.StepBack(SizeT{1});
    helper.IsEqual(str1.Length(), SizeT{0}, __LINE__);

    str1.Reset();
    str1 = "1234";

    str1.StepBack(SizeT{1});
    helper.IsEqual(str1, "123", __LINE__);
    helper.IsEqual(str1.Length(), SizeT{3U}, __LINE__);

    str1.StepBack(SizeT{2});
    helper.IsEqual(str1, "1", __LINE__);
    helper.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1.StepBack(SizeT{1});
    helper.IsEqual(str1.Length(), SizeT{0U}, __LINE__);

    str1.Reset();
    str1 = "01234567890123";
    str1.StepBack(SizeT{10});
    helper.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1.Reset();
    str1 = "0123456789012";
    str1.StepBack(SizeT{10});
    helper.IsEqual(str1.Length(), SizeT{3U}, __LINE__);
}

static void TestString3(QTest &helper) {
    QString str1;

    str1 = "123";

    str1.Reverse();
    helper.IsEqual(str1.Length(), SizeT{3U}, __LINE__);
    helper.IsEqual(str1, "321", __LINE__);

    str1.Reverse(0);
    helper.IsEqual(str1.Length(), SizeT{3U}, __LINE__);
    helper.IsEqual(str1, "123", __LINE__);

    str1 = "0123456789";

    str1.Reverse();
    helper.IsEqual(str1.Length(), SizeT{10U}, __LINE__);
    helper.IsEqual(str1, "9876543210", __LINE__);

    str1.Reverse();
    helper.IsEqual(str1.Length(), SizeT{10U}, __LINE__);
    helper.IsEqual(str1, "0123456789", __LINE__);

    str1.Reverse(3);
    helper.IsEqual(str1.Length(), SizeT{10U}, __LINE__);
    helper.IsEqual(str1, "0129876543", __LINE__);

    str1.Reverse(7);
    helper.IsEqual(str1.Length(), SizeT{10U}, __LINE__);
    helper.IsEqual(str1, "0129876345", __LINE__);

    str1.Reverse(100); // Nothing will happened
    helper.IsEqual(str1.Length(), SizeT{10U}, __LINE__);
    helper.IsEqual(str1, "0129876345", __LINE__);
    ////////////////////////////////////////////
    str1 = "9";

    str1.InsertAt('0', 0);
    helper.IsEqual(str1.Length(), SizeT{2}, __LINE__);
    helper.IsEqual(str1, "09", __LINE__);

    str1.InsertAt('4', 1);
    helper.IsEqual(str1.Length(), SizeT{3}, __LINE__);
    helper.IsEqual(str1, "049", __LINE__);

    str1.InsertAt('4', 10); // Nothing will happened
    helper.IsEqual(str1.Length(), SizeT{3}, __LINE__);
    helper.IsEqual(str1, "049", __LINE__);

    str1.InsertAt('2', 1);
    str1.InsertAt('1', 1);

    helper.IsEqual(str1.Length(), SizeT{5}, __LINE__);
    helper.IsEqual(str1, "01249", __LINE__);

    str1.InsertAt('3', 3);
    str1.InsertAt('5', 5);

    helper.IsEqual(str1.Length(), SizeT{7}, __LINE__);
    helper.IsEqual(str1, "0123459", __LINE__);

    str1.InsertAt('8', 6);
    str1.InsertAt('7', 6);
    str1.InsertAt('6', 6);

    helper.IsEqual(str1.Length(), SizeT{10}, __LINE__);
    helper.IsEqual(str1, "0123456789", __LINE__);
}

static void TestStringTrim(QTest &helper) {
    QString str1("");

    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{0U}, __LINE__);

    str1 = " ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{0U}, __LINE__);

    str1 = "  ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{0U}, __LINE__);

    str1 = "                    ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{0U}, __LINE__);

    str1 = " a";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "a ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = " a ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "  a";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "a  ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "  a  ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "                      a";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "a                      ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "                      a                      ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = " abcd";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "abcd ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = " abcd ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "  abcd";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "abcd  ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "  abcd  ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "                      abcd";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "abcd                      ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "                      abcd                      ";
    str1 = QString::Trim(str1);
    helper.IsEqual(str1.Length(), SizeT{4U}, __LINE__);
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
