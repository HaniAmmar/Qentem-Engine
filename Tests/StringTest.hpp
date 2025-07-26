/*
 * Copyright (c) 2025 Hani Ammar
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

#ifndef QENTEM_STRING_TESTS_H
#define QENTEM_STRING_TESTS_H

#include "QTest.hpp"
#include "String.hpp"

namespace Qentem {
namespace Test {

using QString = String<char>;

static void TestString1(QTest &test) {
    SizeT length;
    char *str_ptr;

    QString str1;
    test.IsEqual(str1.Length(), SizeT{0U}, __LINE__);
    test.IsNull(str1.First(), __LINE__);
    test.IsNull(str1.Last(), __LINE__);

    QString str2{"0123456789"};
    test.IsEqual(str2.Length(), SizeT{10U}, __LINE__);
    test.IsNotNull(str2.First(), __LINE__);
    test.IsNotNull(str2.Last(), __LINE__);
    test.IsEqual(str2.First()[str2.Length()], char{0}, __LINE__);

    str2.Reset();
    test.IsEqual(str2.Length(), SizeT{0U}, __LINE__);
    test.IsNull(str2.First(), __LINE__);
    test.IsNull(str2.Last(), __LINE__);

    str1 = QString("abcd");
    test.IsEqual(str1.Length(), SizeT{4U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "abcd", __LINE__);
    test.IsNotEqual(str1, "abcdef", __LINE__);

    str1 = QString("abcd-bcd");
    test.IsEqual(str1.Length(), SizeT{8U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "abcd-bcd", __LINE__);

    str1 = QString("abcd", 2U);
    test.IsEqual(str1.Length(), SizeT{2U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "ab", __LINE__);
    test.IsNotEqual(str1, "abcd", __LINE__);

    str1 = "ABCDEF0123456789ABCDEF0123456789";
    test.IsEqual(str1.Length(), SizeT{32U}, __LINE__);

    length               = str1.Length();
    const SizeT capacity = str1.Capacity();
    str_ptr              = str1.Detach();
    test.IsNotNull(str_ptr, __LINE__);

    str2.Adopt(str_ptr, length, capacity);
    test.IsEqual(str2.First(), str_ptr, __LINE__);
    test.IsEqual(str2.Length(), length, __LINE__);

    str1 = QUtility::Move(str2); // Move
    test.IsEqual(str1.First(), str_ptr, __LINE__);
    test.IsEqual(str1.Length(), length, __LINE__);
    test.IsNull(str2.Storage(), __LINE__);
    test.IsEqual(str2.Length(), SizeT{0U}, __LINE__);
    test.IsNull(str2.First(), __LINE__);

    str2 = "abcdef"; // Copy
    test.IsEqual(str2.Length(), SizeT{6U}, __LINE__);
    test.IsNotNull(str2.First(), __LINE__);
    test.IsEqual(str2.First()[str2.Length()], char{0}, __LINE__);
    test.IsTrue(StringUtils::IsEqual(str2.First(), "abcdef", 5U), __LINE__);

    str2 = str1; // Copy
    test.IsEqual(str2.Length(), str1.Length(), __LINE__);
    test.IsNotNull(str2.First(), __LINE__);
    test.IsNotEqual(str2.First(), str1.First(), __LINE__);
    test.IsTrue(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), __LINE__);

    str2.Reset();
    str1 = "efg";         // Copy
    str2 = QString(str1); // Copy
    test.IsEqual(str2.Length(), str1.Length(), __LINE__);
    test.IsNotNull(str2.First(), __LINE__);
    test.IsNotEqual(str2.First(), str1.First(), __LINE__);
    test.IsTrue(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), __LINE__);

    str1 = "hig";         // Copy
    str2 = QString(str1); // Copy over a value
    test.IsEqual(str2.Length(), str1.Length(), __LINE__);
    test.IsNotEqual(str2.First(), str1.First(), __LINE__);
    test.IsTrue(StringUtils::IsEqual(str2.First(), str1.First(), str2.Length()), __LINE__);

    length = str1.Length();
    str2   = QString(QUtility::Move(str1));
    test.IsNull(str1.Storage(), __LINE__);
    test.IsNull(str1.First(), __LINE__);
    test.IsEqual(str1.Length(), SizeT{0U}, __LINE__);
    test.IsEqual(str2.Length(), length, __LINE__);

    str1 = "A";
    test.IsEqual(str1.Length(), SizeT{1U}, __LINE__);
    const SizeT str_cap = str1.Capacity();
    str_ptr             = str1.Detach();
    test.IsEqual(str_ptr[0], 'A', __LINE__);
    Reserver::Release(str_ptr, str_cap + 1);

    char   *tmp_size_2 = Reserver::Reserve<char>(3); // 2 + null
    QString str_size_2;

    str_size_2.Adopt(tmp_size_2, SizeT{0}, SizeT{2});

    test.IsEqual(str_size_2.First(), tmp_size_2, __LINE__);
}

static void TestStringCompare(QTest &test) {
    QString     str1("a");
    QString     str2("a");
    const char *str3 = "a";

    test.IsEqual(str1, str2, __LINE__);
    test.IsEqual(str2, str1, __LINE__);
    test.IsEqual(str1, str3, __LINE__);
    test.IsEqual(str2, str3, __LINE__);

    str1 = "abc";
    str2 = "abc";
    str3 = "abc";
    test.IsEqual(str1, str2, __LINE__);
    test.IsEqual(str2, str1, __LINE__);
    test.IsEqual(str1, str3, __LINE__);
    test.IsEqual(str2, str3, __LINE__);

    str1 = "a";
    str2 = "b";
    str3 = "c";
    test.IsNotEqual(str1, str2, __LINE__);
    test.IsNotEqual(str2, str1, __LINE__);
    test.IsNotEqual(str1, str3, __LINE__);
    test.IsNotEqual(str2, str3, __LINE__);

    str1 = "abc";
    str2 = "efg";
    str3 = "hij";
    test.IsNotEqual(str1, str2, __LINE__);
    test.IsNotEqual(str2, str1, __LINE__);
    test.IsNotEqual(str1, str3, __LINE__);
    test.IsNotEqual(str2, str3, __LINE__);

    str1 = "a";
    str2 = "ef";
    str3 = "abc";
    test.IsNotEqual(str1, str2, __LINE__);
    test.IsNotEqual(str2, str1, __LINE__);
    test.IsNotEqual(str1, str3, __LINE__);
    test.IsNotEqual(str2, str3, __LINE__);

    str1 = "";
    test.IsFalse(str1.IsEqual(" ", 1U), __LINE__);

    str1 = "";
    str2 = "";
    test.IsTrue((str1 >= str2), __LINE__);
    test.IsTrue((str1 <= str2), __LINE__);
    test.IsFalse((str1 > str2), __LINE__);
    test.IsFalse((str1 < str2), __LINE__);

    str1 = "a";
    str2 = "a";
    test.IsTrue((str1 >= str2), __LINE__);
    test.IsTrue((str1 <= str2), __LINE__);
    test.IsFalse((str1 > str2), __LINE__);
    test.IsFalse((str1 < str2), __LINE__);

    str1 = "a";
    str2 = "A";
    test.IsTrue((str1 >= str2), __LINE__);
    test.IsTrue((str1 > str2), __LINE__);
    test.IsFalse((str1 <= str2), __LINE__);
    test.IsFalse((str1 < str2), __LINE__);
    //
    test.IsFalse((str2 >= str1), __LINE__);
    test.IsFalse((str2 > str1), __LINE__);
    test.IsTrue((str2 <= str1), __LINE__);
    test.IsTrue((str2 < str1), __LINE__);

    str1 = "a";
    str2 = "B";
    test.IsTrue((str1 >= str2), __LINE__);
    test.IsTrue((str1 > str2), __LINE__);
    test.IsFalse((str1 <= str2), __LINE__);
    test.IsFalse((str1 < str2), __LINE__);
    //
    test.IsFalse((str2 >= str1), __LINE__);
    test.IsFalse((str2 > str1), __LINE__);
    test.IsTrue((str2 <= str1), __LINE__);
    test.IsTrue((str2 < str1), __LINE__);

    str1 = "aa";
    str2 = "aA";
    test.IsTrue((str1 >= str2), __LINE__);
    test.IsTrue((str1 > str2), __LINE__);
    test.IsFalse((str1 <= str2), __LINE__);
    test.IsFalse((str1 < str2), __LINE__);
    //
    test.IsFalse((str2 >= str1), __LINE__);
    test.IsFalse((str2 > str1), __LINE__);
    test.IsTrue((str2 <= str1), __LINE__);
    test.IsTrue((str2 < str1), __LINE__);

    str1 = "2021";
    str2 = "2018";
    test.IsTrue((str1 >= str2), __LINE__);
    test.IsTrue((str1 > str2), __LINE__);
    test.IsFalse((str1 <= str2), __LINE__);
    test.IsFalse((str1 < str2), __LINE__);
    //
    test.IsFalse((str2 >= str1), __LINE__);
    test.IsFalse((str2 > str1), __LINE__);
    test.IsTrue((str2 <= str1), __LINE__);
    test.IsTrue((str2 < str1), __LINE__);
}

static void TestString2(QTest &test) {
    QString str1;
    QString str2;

    str1.Write("a", 1U);
    test.IsEqual(str1.Length(), SizeT{1U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "a", __LINE__);

    str1.Write("bc", 2U);
    test.IsEqual(str1.Length(), SizeT{3U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "abc", __LINE__);

    str1.Write("", 0U);
    test.IsEqual(str1.Length(), SizeT{3U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "abc", __LINE__);

    str2 = QString::Merge(str1, QString("def"));
    test.IsEqual(str2.Length(), SizeT{6U}, __LINE__);
    test.IsNotNull(str2.First(), __LINE__);
    test.IsEqual(str2.First()[str2.Length()], char{0}, __LINE__);
    test.IsEqual(str2, "abcdef", __LINE__);

    str2 = QString::Merge(str2, QString(""));
    test.IsEqual(str2.Length(), SizeT{6U}, __LINE__);
    test.IsNotNull(str2.First(), __LINE__);
    test.IsEqual(str2.First()[str2.Length()], char{0}, __LINE__);
    test.IsEqual(str2, "abcdef", __LINE__);

    str2 = QString::Merge(QString(""), str2);
    test.IsEqual(str2.Length(), SizeT{6U}, __LINE__);
    test.IsNotNull(str2.First(), __LINE__);
    test.IsEqual(str2.First()[str2.Length()], char{0}, __LINE__);
    test.IsEqual(str2, "abcdef", __LINE__);

    str1.Reset();
    str1 += "a";
    test.IsEqual(str1.Length(), SizeT{1U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "a", __LINE__);

    str1 += "bc";
    test.IsEqual(str1.Length(), SizeT{3U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "abc", __LINE__);

    str2 = "def";
    str1 += str2;
    test.IsEqual(str1.Length(), SizeT{6U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "abcdef", __LINE__);

    str2 = "ghi";
    str1 += QUtility::Move(str2);
    test.IsEqual(str1.Length(), SizeT{9U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "abcdefghi", __LINE__);
    test.IsEqual(str2.Length(), SizeT{0U}, __LINE__);
    test.IsNull(str2.First(), __LINE__);

    str1 = str1 + "";
    test.IsEqual(str1.Length(), SizeT{9U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "abcdefghi", __LINE__);

    str1 = str1 + "gkl";
    test.IsEqual(str1.Length(), SizeT{12U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "abcdefghigkl", __LINE__);

    str2 = "123";
    str1 = str1 + str2;
    test.IsEqual(str1.Length(), SizeT{15U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "abcdefghigkl123", __LINE__);

    str1 = "123";
    str1 += '4';
    test.IsEqual(str1.Length(), SizeT{4U}, __LINE__);
    test.IsEqual(str1, "1234", __LINE__);

    str1 = "aaaaaaaaaaaaaaaaaaaaaaaa";
    str1 += '4';
    test.IsEqual(str1.Length(), SizeT{25U}, __LINE__);
    test.IsEqual(str1, "aaaaaaaaaaaaaaaaaaaaaaaa4", __LINE__);

    str1 = "aaaaaaaaaaaaaaaaaaaaaaaa";
    str1 += "aaaaaaaaaaaaaaaaaaaaaaaa";
    test.IsEqual(str1.Length(), SizeT{48U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", __LINE__);

    str1.Reset();
    str1 = "1234";
    str2 = "5678";
    str1 = str1 + QUtility::Move(str2);
    test.IsEqual(str1.Length(), SizeT{8U}, __LINE__);
    test.IsNotNull(str1.First(), __LINE__);
    test.IsEqual(str1.First()[str1.Length()], char{0}, __LINE__);
    test.IsEqual(str1, "12345678", __LINE__);
    test.IsEqual(str2.Length(), SizeT{0U}, __LINE__);
    test.IsNull(str2.First(), __LINE__);

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

    test.IsTrue(StringUtils::IsEqual(&(sis.str[0]), "12345678", 8U), __LINE__);

    str1.Reset();
    str1 = "1234";

    SizeT total = 0;

    for (const auto &item : str1) {
        total = SizeT(total + SizeT(item) - SizeT{'0'});
    }

    test.IsEqual(total, SizeT{10}, __LINE__);

    for (auto &item : str1) {
        item += '0';
    }

    test.IsEqual(str1, "abcd", __LINE__);

    str1.Reset();
    str1 = "0123456789012345678901234567890123456789";

    str1.StepBack(SizeT{1});
    test.IsEqual(str1, "012345678901234567890123456789012345678", __LINE__);
    test.IsEqual(str1.Length(), SizeT{39}, __LINE__);

    str1.StepBack(SizeT{10});
    test.IsEqual(str1, "01234567890123456789012345678", __LINE__);
    test.IsEqual(str1.Length(), SizeT{29}, __LINE__);

    str1.StepBack(SizeT{25});
    test.IsEqual(str1, "0123", __LINE__);
    test.IsEqual(str1.Length(), SizeT{4}, __LINE__);

    str1.StepBack(SizeT{3});
    test.IsEqual(str1, "0", __LINE__);
    test.IsEqual(str1.Length(), SizeT{1}, __LINE__);

    str1.StepBack(SizeT{1});
    test.IsEqual(str1.Length(), SizeT{0}, __LINE__);

    str1.Reset();
    str1 = "1234";

    str1.StepBack(SizeT{1});
    test.IsEqual(str1, "123", __LINE__);
    test.IsEqual(str1.Length(), SizeT{3U}, __LINE__);

    str1.StepBack(SizeT{2});
    test.IsEqual(str1, "1", __LINE__);
    test.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1.StepBack(SizeT{1});
    test.IsEqual(str1.Length(), SizeT{0U}, __LINE__);

    str1.Reset();
    str1 = "01234567890123";
    str1.StepBack(SizeT{10});
    test.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1.Reset();
    str1 = "0123456789012";
    str1.StepBack(SizeT{10});
    test.IsEqual(str1.Length(), SizeT{3U}, __LINE__);
}

static void TestString3(QTest &test) {
    QString str1;

    str1 = "123";

    str1.Reverse();
    test.IsEqual(str1.Length(), SizeT{3U}, __LINE__);
    test.IsEqual(str1, "321", __LINE__);

    str1.Reverse(0);
    test.IsEqual(str1.Length(), SizeT{3U}, __LINE__);
    test.IsEqual(str1, "123", __LINE__);

    str1 = "0123456789";

    str1.Reverse();
    test.IsEqual(str1.Length(), SizeT{10U}, __LINE__);
    test.IsEqual(str1, "9876543210", __LINE__);

    str1.Reverse();
    test.IsEqual(str1.Length(), SizeT{10U}, __LINE__);
    test.IsEqual(str1, "0123456789", __LINE__);

    str1.Reverse(3);
    test.IsEqual(str1.Length(), SizeT{10U}, __LINE__);
    test.IsEqual(str1, "0129876543", __LINE__);

    str1.Reverse(7);
    test.IsEqual(str1.Length(), SizeT{10U}, __LINE__);
    test.IsEqual(str1, "0129876345", __LINE__);

    str1.Reverse(100); // Nothing will happened
    test.IsEqual(str1.Length(), SizeT{10U}, __LINE__);
    test.IsEqual(str1, "0129876345", __LINE__);
    ////////////////////////////////////////////
    str1 = "9";

    str1.InsertAt('0', 0);
    test.IsEqual(str1.Length(), SizeT{2}, __LINE__);
    test.IsEqual(str1, "09", __LINE__);

    str1.InsertAt('4', 1);
    test.IsEqual(str1.Length(), SizeT{3}, __LINE__);
    test.IsEqual(str1, "049", __LINE__);

    str1.InsertAt('4', 10); // Nothing will happened
    test.IsEqual(str1.Length(), SizeT{3}, __LINE__);
    test.IsEqual(str1, "049", __LINE__);

    str1.InsertAt('2', 1);
    str1.InsertAt('1', 1);

    test.IsEqual(str1.Length(), SizeT{5}, __LINE__);
    test.IsEqual(str1, "01249", __LINE__);

    str1.InsertAt('3', 3);
    str1.InsertAt('5', 5);

    test.IsEqual(str1.Length(), SizeT{7}, __LINE__);
    test.IsEqual(str1, "0123459", __LINE__);

    str1.InsertAt('8', 6);
    str1.InsertAt('7', 6);
    str1.InsertAt('6', 6);

    test.IsEqual(str1.Length(), SizeT{10}, __LINE__);
    test.IsEqual(str1, "0123456789", __LINE__);
}

static void TestStringTrim(QTest &test) {
    QString str1("");

    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{0U}, __LINE__);

    str1 = " ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{0U}, __LINE__);

    str1 = "  ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{0U}, __LINE__);

    str1 = "                    ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{0U}, __LINE__);

    str1 = " a";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "a ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = " a ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "  a";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "a  ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "  a  ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "                      a";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "a                      ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = "                      a                      ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{1U}, __LINE__);

    str1 = " abcd";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "abcd ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = " abcd ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "  abcd";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "abcd  ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "  abcd  ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "                      abcd";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "abcd                      ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{4U}, __LINE__);

    str1 = "                      abcd                      ";
    str1 = QString::Trim(str1);
    test.IsEqual(str1.Length(), SizeT{4U}, __LINE__);
}

static int RunStringTests() {
    QTest test{"String.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("String Test 1", TestString1);
    test.Test("String::Compare", TestStringCompare);
    test.Test("String Test 2", TestString2);
    test.Test("String Test 2", TestString3);
    test.Test("String::Trim", TestStringTrim);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
