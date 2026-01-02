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

#ifndef QENTEM_STRING_UTILS_TESTS_H
#define QENTEM_STRING_UTILS_TESTS_H

#include "Qentem/QTest.hpp"
#include "Qentem/StringUtils.hpp"

namespace Qentem {
namespace Test {

static void TestHash(QTest &test) {
    SizeT hash;
    SizeT hash2;
    SizeT hash3;
    SizeT hash4;

    hash = StringUtils::Hash("", 0U);
    test.IsNotEqual(hash, 0U, __LINE__);

    hash  = StringUtils::Hash("1", 1U);
    hash2 = StringUtils::Hash("0", 1U);
    test.IsNotEqual(hash, 0U, __LINE__);
    test.IsNotEqual(hash2, 0U, __LINE__);
    test.IsNotEqual(hash, hash2, __LINE__);

    hash3 = StringUtils::Hash("10", 2U);
    hash4 = StringUtils::Hash("01", 2U);
    test.IsNotEqual(hash3, 0U, __LINE__);
    test.IsNotEqual(hash4, 0U, __LINE__);
    test.IsNotEqual(hash, hash3, __LINE__);
    test.IsNotEqual(hash2, hash3, __LINE__);
    test.IsNotEqual(hash, hash4, __LINE__);
    test.IsNotEqual(hash2, hash4, __LINE__);
    test.IsNotEqual(hash3, hash4, __LINE__);

    hash  = StringUtils::Hash("100", 3U);
    hash2 = StringUtils::Hash("001", 3U);
    test.IsNotEqual(hash, 0U, __LINE__);
    test.IsNotEqual(hash2, 0U, __LINE__);
    test.IsNotEqual(hash, hash3, __LINE__);
    test.IsNotEqual(hash2, hash3, __LINE__);
    test.IsNotEqual(hash, hash4, __LINE__);
    test.IsNotEqual(hash2, hash4, __LINE__);

    hash  = StringUtils::Hash("abc", 3U);
    hash2 = StringUtils::Hash("cba", 3U);
    test.IsNotEqual(hash, hash2, __LINE__);
    test.IsNotEqual(hash, 0U, __LINE__);
    test.IsNotEqual(hash2, 0U, __LINE__);
}

static void TestCount(QTest &test) {
    SizeT length = StringUtils::Count("");
    test.IsEqual(length, 0U, __LINE__);

    length = StringUtils::Count("a");
    test.IsEqual(length, 1U, __LINE__);

    length = StringUtils::Count("abcde");
    test.IsEqual(length, 5U, __LINE__);

    length = StringUtils::Count("\0");
    test.IsEqual(length, 0U, __LINE__);

    length = StringUtils::Count("1234567");
    test.IsEqual(length, 7U, __LINE__);

    length = StringUtils::Count("123456781234567");
    test.IsEqual(length, 15U, __LINE__);

    length = StringUtils::Count("1234567812345678123456781234567");
    test.IsEqual(length, 31U, __LINE__);

    length = StringUtils::Count("123456781234567812345678123456781234567812345678123456781234567");
    test.IsEqual(length, 63U, __LINE__);
}

static void TestTrimLeft(QTest &test) {
    SizeT32 offset = 0;

    StringUtils::TrimLeft("", offset, 0U);
    test.IsEqual(offset, 0U, __LINE__);

    StringUtils::TrimLeft("", offset, 1U);
    test.IsEqual(offset, 0U, __LINE__);

    StringUtils::TrimLeft("", offset, 10U);
    test.IsEqual(offset, 0U, __LINE__);

    StringUtils::TrimLeft("a", offset, 1U);
    test.IsEqual(offset, 0U, __LINE__);

    StringUtils::TrimLeft("abc", offset, 3U);
    test.IsEqual(offset, 0U, __LINE__);

    StringUtils::TrimLeft("a ", offset, 2U);
    test.IsEqual(offset, 0U, __LINE__);

    StringUtils::TrimLeft("abc ", offset, 4U);
    test.IsEqual(offset, 0U, __LINE__);

    StringUtils::TrimLeft("a  ", offset, 3U);
    test.IsEqual(offset, 0U, __LINE__);

    StringUtils::TrimLeft("abc  ", offset, 5U);
    test.IsEqual(offset, 0U, __LINE__);

    StringUtils::TrimLeft("a   ", offset, 4U);
    test.IsEqual(offset, 0U, __LINE__);

    StringUtils::TrimLeft("abc   ", offset, 6U);
    test.IsEqual(offset, 0U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft(" a", offset, 2U);
    test.IsEqual(offset, 1U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft(" abc", offset, 4U);
    test.IsEqual(offset, 1U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft("  a", offset, 3U);
    test.IsEqual(offset, 2U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft("  abc", offset, 5U);
    test.IsEqual(offset, 2U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft("   a", offset, 4U);
    test.IsEqual(offset, 3U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft("   abc", offset, 6U);
    test.IsEqual(offset, 3U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft(" a ", offset, 3U);
    test.IsEqual(offset, 1U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft(" abc ", offset, 5U);
    test.IsEqual(offset, 1U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft("  a ", offset, 4U);
    test.IsEqual(offset, 2U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft("  abc ", offset, 6U);
    test.IsEqual(offset, 2U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft("   a  ", offset, 6U);
    test.IsEqual(offset, 3U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft("   abc  ", offset, 8U);
    test.IsEqual(offset, 3U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft(" ", offset, 1U);
    test.IsEqual(offset, 1U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft("  ", offset, 2U);
    test.IsEqual(offset, 2U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft("   ", offset, 3U);
    test.IsEqual(offset, 3U, __LINE__);

    offset = 1;
    StringUtils::TrimLeft(" a ", offset, 3U);
    test.IsEqual(offset, 1U, __LINE__);

    offset = 1;
    StringUtils::TrimLeft(" abc ", offset, 5U);
    test.IsEqual(offset, 1U, __LINE__);

    offset = 2;
    StringUtils::TrimLeft("  a ", offset, 4U);
    test.IsEqual(offset, 2U, __LINE__);

    offset = 2;
    StringUtils::TrimLeft("  abc ", offset, 6U);
    test.IsEqual(offset, 2U, __LINE__);

    offset = 2;
    StringUtils::TrimLeft("   a  ", offset, 6U);
    test.IsEqual(offset, 3U, __LINE__);

    offset = 2;
    StringUtils::TrimLeft("   a\n\r\t", offset, 7U);
    test.IsEqual(offset, 3U, __LINE__);

    offset = 2;
    StringUtils::TrimLeft("   abc  ", offset, 8U);
    test.IsEqual(offset, 3U, __LINE__);

    offset = 1;
    StringUtils::TrimLeft("  ", offset, 2U);
    test.IsEqual(offset, 2U, __LINE__);

    offset = 2;
    StringUtils::TrimLeft("   ", offset, 3U);
    test.IsEqual(offset, 3U, __LINE__);

    offset = 0;
    StringUtils::TrimLeft("  ", offset, 1U);
    test.IsEqual(offset, 1U, __LINE__);

    offset = 1;
    StringUtils::TrimLeft("   ", offset, 2U);
    test.IsEqual(offset, 2U, __LINE__);
}

static void TestTrimRight(QTest &test) {
    SizeT32 end_offset = 0;

    StringUtils::TrimRight("", 0U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    StringUtils::TrimRight("", 1U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    StringUtils::TrimRight("", 10U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    StringUtils::TrimRight("a", 0U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    StringUtils::TrimRight("abc", 0U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    StringUtils::TrimRight("a ", 1U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    StringUtils::TrimRight("abc ", 4U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    StringUtils::TrimRight("a  ", 3U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    StringUtils::TrimRight("abc  ", 5U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    StringUtils::TrimRight("a   ", 4U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    StringUtils::TrimRight("abc   ", 6U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    end_offset = 2;
    StringUtils::TrimRight("a ", 0U, end_offset);
    test.IsEqual(end_offset, 1U, __LINE__);

    end_offset = 4;
    StringUtils::TrimRight("abc ", 0U, end_offset);
    test.IsEqual(end_offset, 3U, __LINE__);

    end_offset = 3;
    StringUtils::TrimRight("a  ", 0U, end_offset);
    test.IsEqual(end_offset, 1U, __LINE__);

    end_offset = 5;
    StringUtils::TrimRight("abc  ", 0U, end_offset);
    test.IsEqual(end_offset, 3U, __LINE__);

    end_offset = 4;
    StringUtils::TrimRight("a   ", 4U, end_offset);
    test.IsEqual(end_offset, 4U, __LINE__);

    end_offset = 4;
    StringUtils::TrimRight("a   ", 0U, end_offset);
    test.IsEqual(end_offset, 1U, __LINE__);

    end_offset = 6;
    StringUtils::TrimRight("abc   ", 6U, end_offset);
    test.IsEqual(end_offset, 6U, __LINE__);

    end_offset = 6;
    StringUtils::TrimRight("abc   ", 3U, end_offset);
    test.IsEqual(end_offset, 3U, __LINE__);

    end_offset = 6;
    StringUtils::TrimRight("abc   ", 2U, end_offset);
    test.IsEqual(end_offset, 3U, __LINE__);

    end_offset = 6;
    StringUtils::TrimRight("abc   ", 0U, end_offset);
    test.IsEqual(end_offset, 3U, __LINE__);

    end_offset = 2;
    StringUtils::TrimRight(" a", 0U, end_offset);
    test.IsEqual(end_offset, 2U, __LINE__);

    end_offset = 2;
    StringUtils::TrimRight(" a", 1U, end_offset);
    test.IsEqual(end_offset, 2U, __LINE__);

    end_offset = 2;
    StringUtils::TrimRight(" a", 2U, end_offset);
    test.IsEqual(end_offset, 2U, __LINE__);

    end_offset = 1;
    StringUtils::TrimRight(" a", 0U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    end_offset = 1;
    StringUtils::TrimRight(" a", 1U, end_offset);
    test.IsEqual(end_offset, 1U, __LINE__);

    end_offset = 4;
    StringUtils::TrimRight(" abc", 0U, end_offset);
    test.IsEqual(end_offset, 4U, __LINE__);

    end_offset = 3;
    StringUtils::TrimRight("  a", 0U, end_offset);
    test.IsEqual(end_offset, 3U, __LINE__);

    end_offset = 5;
    StringUtils::TrimRight("  abc", 0U, end_offset);
    test.IsEqual(end_offset, 5U, __LINE__);

    end_offset = 4;
    StringUtils::TrimRight("   a", 0U, end_offset);
    test.IsEqual(end_offset, 4U, __LINE__);

    end_offset = 3;
    StringUtils::TrimRight("   a", 0U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    end_offset = 3;
    StringUtils::TrimRight("   a", 2U, end_offset);
    test.IsEqual(end_offset, 2U, __LINE__);

    end_offset = 3;
    StringUtils::TrimRight("   a", 1U, end_offset);
    test.IsEqual(end_offset, 1U, __LINE__);

    end_offset = 6;
    StringUtils::TrimRight("   abc", 0U, end_offset);
    test.IsEqual(end_offset, 6U, __LINE__);

    end_offset = 5;
    StringUtils::TrimRight("   abc", 0U, end_offset);
    test.IsEqual(end_offset, 5U, __LINE__);

    end_offset = 4;
    StringUtils::TrimRight("   abc", 0U, end_offset);
    test.IsEqual(end_offset, 4U, __LINE__);

    end_offset = 3;
    StringUtils::TrimRight("   abc", 0U, end_offset);
    test.IsEqual(end_offset, 0U, __LINE__);

    end_offset = 3;
    StringUtils::TrimRight("   abc", 1U, end_offset);
    test.IsEqual(end_offset, 1U, __LINE__);

    end_offset = 3;
    StringUtils::TrimRight("   abc", 2U, end_offset);
    test.IsEqual(end_offset, 2U, __LINE__);

    end_offset = 3;
    StringUtils::TrimRight(" a ", 0U, end_offset);
    test.IsEqual(end_offset, 2U, __LINE__);

    end_offset = 5;
    StringUtils::TrimRight(" abc ", 0U, end_offset);
    test.IsEqual(end_offset, 4U, __LINE__);

    end_offset = 4;
    StringUtils::TrimRight(" a  ", 4U, end_offset);
    test.IsEqual(end_offset, 4U, __LINE__);

    end_offset = 5;
    StringUtils::TrimRight(" a\n\r\t", 0U, end_offset);
    test.IsEqual(end_offset, 2U, __LINE__);

    end_offset = 6;
    StringUtils::TrimRight(" abc  ", 0U, end_offset);
    test.IsEqual(end_offset, 4U, __LINE__);

    end_offset = 4;
    StringUtils::TrimRight("  a   ", 0U, end_offset);
    test.IsEqual(end_offset, 3U, __LINE__);

    end_offset = 8;
    StringUtils::TrimRight("  abc   ", 0U, end_offset);
    test.IsEqual(end_offset, 5U, __LINE__);

    end_offset = 1;
    StringUtils::TrimRight(" ", 1U, end_offset);
    test.IsEqual(end_offset, 1U, __LINE__);

    end_offset = 2;
    StringUtils::TrimRight("  ", 2U, end_offset);
    test.IsEqual(end_offset, 2U, __LINE__);

    end_offset = 3;
    StringUtils::TrimRight("   ", 3U, end_offset);
    test.IsEqual(end_offset, 3U, __LINE__);

    end_offset = 3;
    StringUtils::TrimRight(" a ", 3U, end_offset);
    test.IsEqual(end_offset, 3U, __LINE__);

    end_offset = 5;
    StringUtils::TrimRight(" abc ", 5U, end_offset);
    test.IsEqual(end_offset, 5U, __LINE__);

    end_offset = 4;
    StringUtils::TrimRight(" a  ", 4U, end_offset);
    test.IsEqual(end_offset, 4U, __LINE__);

    end_offset = 6;
    StringUtils::TrimRight(" abc  ", 6U, end_offset);
    test.IsEqual(end_offset, 6U, __LINE__);

    end_offset = 7;
    StringUtils::TrimRight("  a    ", 6U, end_offset);
    test.IsEqual(end_offset, 6U, __LINE__);

    end_offset = 8;
    StringUtils::TrimRight("  abc   ", 4U, end_offset);
    test.IsEqual(end_offset, 5U, __LINE__);
}

static void TestTrim(QTest &test) {
    SizeT offset = 0;
    SizeT length = 0;

    StringUtils::Trim("", offset, length);
    test.IsEqual(offset, 0U, __LINE__);
    test.IsEqual(length, 0U, __LINE__);

    offset = 0;
    length = 1;
    StringUtils::Trim(" ", offset, length);
    test.IsEqual(offset, 1U, __LINE__);
    test.IsEqual(length, 0U, __LINE__);

    offset = 0;
    length = 1;
    StringUtils::Trim("  ", offset, length);
    test.IsEqual(offset, 1U, __LINE__);
    test.IsEqual(length, 0U, __LINE__);

    offset = 0;
    length = 2;
    StringUtils::Trim("  ", offset, length);
    test.IsEqual(offset, 2U, __LINE__);
    test.IsEqual(length, 0U, __LINE__);

    offset = 0;
    length = 2;
    StringUtils::Trim(" a", offset, length);
    test.IsEqual(offset, 1U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 0;
    length = 4;
    StringUtils::Trim(" abc", offset, length);
    test.IsEqual(offset, 1U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 1;
    length = 1;
    StringUtils::Trim(" a", offset, length);
    test.IsEqual(offset, 1U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 1;
    length = 3;
    StringUtils::Trim(" abc", offset, length);
    test.IsEqual(offset, 1U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 2;
    length = 1;
    StringUtils::Trim("  a", offset, length);
    test.IsEqual(offset, 2U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 2;
    length = 3;
    StringUtils::Trim("  abc", offset, length);
    test.IsEqual(offset, 2U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 0;
    length = 3;
    StringUtils::Trim("  a", offset, length);
    test.IsEqual(offset, 2U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 0;
    length = 5;
    StringUtils::Trim("  abc", offset, length);
    test.IsEqual(offset, 2U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 0;
    length = 4;
    StringUtils::Trim("   a", offset, length);
    test.IsEqual(offset, 3U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 0;
    length = 6;
    StringUtils::Trim("   abc", offset, length);
    test.IsEqual(offset, 3U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 0;
    length = 2;
    StringUtils::Trim("a ", offset, length);
    test.IsEqual(offset, 0U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 0;
    length = 4;
    StringUtils::Trim("abc ", offset, length);
    test.IsEqual(offset, 0U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 0;
    length = 1;
    StringUtils::Trim("a ", offset, length);
    test.IsEqual(offset, 0U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 0;
    length = 3;
    StringUtils::Trim("abc ", offset, length);
    test.IsEqual(offset, 0U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 0;
    length = 1;
    StringUtils::Trim("a  ", offset, length);
    test.IsEqual(offset, 0U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 0;
    length = 3;
    StringUtils::Trim("abc  ", offset, length);
    test.IsEqual(offset, 0U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 0;
    length = 3;
    StringUtils::Trim("a  ", offset, length);
    test.IsEqual(offset, 0U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 0;
    length = 5;
    StringUtils::Trim("abc  ", offset, length);
    test.IsEqual(offset, 0U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 0;
    length = 4;
    StringUtils::Trim("a   ", offset, length);
    test.IsEqual(offset, 0U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 0;
    length = 6;
    StringUtils::Trim("abc   ", offset, length);
    test.IsEqual(offset, 0U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 0;
    length = 3;
    StringUtils::Trim(" a ", offset, length);
    test.IsEqual(offset, 1U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 0;
    length = 5;
    StringUtils::Trim(" abc ", offset, length);
    test.IsEqual(offset, 1U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 0;
    length = 5;
    StringUtils::Trim("  a  ", offset, length);
    test.IsEqual(offset, 2U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 0;
    length = 7;
    StringUtils::Trim("  abc  ", offset, length);
    test.IsEqual(offset, 2U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 0;
    length = 7;
    StringUtils::Trim("   a   ", offset, length);
    test.IsEqual(offset, 3U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 0;
    length = 9;
    StringUtils::Trim("   abc   ", offset, length);
    test.IsEqual(offset, 3U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 1;
    length = 1;
    StringUtils::Trim(" a ", offset, length);
    test.IsEqual(offset, 1U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 1;
    length = 3;
    StringUtils::Trim(" abc ", offset, length);
    test.IsEqual(offset, 1U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 2;
    length = 1;
    StringUtils::Trim("  a  ", offset, length);
    test.IsEqual(offset, 2U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 2;
    length = 3;
    StringUtils::Trim("  abc  ", offset, length);
    test.IsEqual(offset, 2U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);

    offset = 3;
    length = 1;
    StringUtils::Trim("   a   ", offset, length);
    test.IsEqual(offset, 3U, __LINE__);
    test.IsEqual(length, 1U, __LINE__);

    offset = 3;
    length = 3;
    StringUtils::Trim("   abc   ", offset, length);
    test.IsEqual(offset, 3U, __LINE__);
    test.IsEqual(length, 3U, __LINE__);
}

static void TestIsEqual(QTest &test) {
    test.IsTrue(StringUtils::IsEqual("", "", 0U), __LINE__);
    test.IsTrue(StringUtils::IsEqual("a", "a", 1U), __LINE__);
    test.IsTrue(StringUtils::IsEqual("ab", "ab", 2U), __LINE__);
    test.IsTrue(StringUtils::IsEqual("abcdefgh", "abcdefgh", 8U), __LINE__);
    test.IsTrue(StringUtils::IsEqual("a", "abcdefgh", 1U), __LINE__);
    test.IsTrue(StringUtils::IsEqual("abc", "abcdefgh", 3U), __LINE__);
    test.IsTrue(StringUtils::IsEqual("abcdefgh", "a", 1U), __LINE__);
    test.IsTrue(StringUtils::IsEqual("abcdefgh", "abc", 3U), __LINE__);
    test.IsTrue(StringUtils::IsEqual("0123456789123456", "0123456789123456", 16U), __LINE__);
    test.IsTrue(StringUtils::IsEqual("01234567891234560123456789123456", "01234567891234560123456789123456", 32U),
                __LINE__);
    test.IsTrue(StringUtils::IsEqual("0123456789123456012345678912345601234567891234560123456789123456",
                                     "0123456789123456012345678912345601234567891234560123456789123456", 64U),
                __LINE__);

    test.IsFalse(StringUtils::IsEqual("a", "b", 1U), __LINE__);
    test.IsFalse(StringUtils::IsEqual("ab", "ba", 2U), __LINE__);
    test.IsFalse(StringUtils::IsEqual("abcdefgh", "--------", 8U), __LINE__);
    test.IsFalse(StringUtils::IsEqual("h", "abcdefgh", 1U), __LINE__);
    test.IsFalse(StringUtils::IsEqual("abc", "def", 3U), __LINE__);
    test.IsFalse(StringUtils::IsEqual("abcdefgh", "b", 1U), __LINE__);
    test.IsFalse(StringUtils::IsEqual("abcdefgh", "cde", 3U), __LINE__);

    test.IsFalse(StringUtils::IsEqual("01234567891234568", "0123456789123456", 17U), __LINE__);
    test.IsFalse(StringUtils::IsEqual("012345678912345601234567891234567", "012345678912345601234567891234568", 33U),
                 __LINE__);
    test.IsFalse(StringUtils::IsEqual("01234567829123456012345678912345601234567891234560123456789123456",
                                      "00234567829123456012345678912345601234567891234560123456789123456", 65U),
                 __LINE__);

    test.IsFalse(StringUtils::IsEqual("01234567829123456012345678912345601234567891234560123456789123456",
                                      "01234567829123456012345678912345601234567891234560123456789123457", 65U),
                 __LINE__);

    test.IsFalse(StringUtils::IsEqual("01234567829123456012345678912345601234567891234560123456789123456",
                                      "01234567829123456012345678912345601234567891235560123456789123456", 65U),
                 __LINE__);

    test.IsFalse(StringUtils::IsEqual("01234567829123456012345678912345601234567891234560123456789123456",
                                      "01234567829123456012335678912345601234567891234560123456789123456", 65U),
                 __LINE__);

    test.IsFalse(StringUtils::IsEqual("a2345678912345678912345678912345612340678912345678912345678912345w",
                                      "a2345678912345678912345678912345612345678912345678912345678912345w", 65U),
                 __LINE__);
}

static void TestIsGreater(QTest &test) {
    test.IsTrue(StringUtils::IsGreater("", "", 0U, 0U, true), __LINE__);
    test.IsTrue(StringUtils::IsGreater("b", "a", 1U, 1U, false), __LINE__);
    test.IsTrue(StringUtils::IsGreater("a", "a", 1U, 1U, true), __LINE__);
    test.IsTrue(StringUtils::IsGreater("a", "A", 1U, 1U, false), __LINE__);
    test.IsTrue(StringUtils::IsGreater("a", "A", 1U, 1U, true), __LINE__);
    test.IsTrue(StringUtils::IsGreater("aa", "aA", 2U, 2U, false), __LINE__);
    test.IsTrue(StringUtils::IsGreater("aa", "aA", 2U, 2U, true), __LINE__);
    test.IsTrue(StringUtils::IsGreater("abcdefgh", "aBcdefgh", 8U, 8U, true), __LINE__);
    test.IsTrue(StringUtils::IsGreater("abcdefgh", "aBcdefgh", 8U, 8U, false), __LINE__);
    test.IsTrue(StringUtils::IsGreater("abcdefgh", "abcdefgh", 8U, 8U, true), __LINE__);
    test.IsTrue(StringUtils::IsGreater("201811111", "20189999", 4U, 4U, true), __LINE__);
    test.IsTrue(StringUtils::IsGreater("2018", "2018", 4U, 4U, true), __LINE__);
    test.IsTrue(StringUtils::IsGreater("201911111", "20189999", 4U, 4U, true), __LINE__);
    test.IsTrue(StringUtils::IsGreater("2019", "2018", 4U, 4U, false), __LINE__);
    test.IsTrue(StringUtils::IsGreater("201911111", "20189999", 4U, 4U, false), __LINE__);
    test.IsTrue(StringUtils::IsGreater("2019", "2018", 4U, 4U, true), __LINE__);
    test.IsTrue(StringUtils::IsGreater("2019", "2018", 3U, 3U, true), __LINE__);
    test.IsTrue(StringUtils::IsGreater("2021", "2018", 4U, 4U, true), __LINE__);
    test.IsTrue(StringUtils::IsGreater("2021", "2018", 4U, 4U, false), __LINE__);

    test.IsFalse(StringUtils::IsGreater("", "", 0U, 0U, false), __LINE__);
    test.IsFalse(StringUtils::IsGreater("a", "a", 1U, 1U, false), __LINE__);
    test.IsFalse(StringUtils::IsGreater("a", "b", 1U, 1U, false), __LINE__);
    test.IsFalse(StringUtils::IsGreater("A", "a", 1U, 1U, false), __LINE__);
    test.IsFalse(StringUtils::IsGreater("A", "a", 1U, 1U, true), __LINE__);
    test.IsFalse(StringUtils::IsGreater("aA", "aa", 2U, 2U, false), __LINE__);
    test.IsFalse(StringUtils::IsGreater("aA", "aa", 2U, 2U, true), __LINE__);
    test.IsFalse(StringUtils::IsGreater("aBcdefgh", "abcdefgh", 8U, 8U, true), __LINE__);
    test.IsFalse(StringUtils::IsGreater("aBcdefgh", "abcdefgh", 8U, 8U, false), __LINE__);
    test.IsFalse(StringUtils::IsGreater("abcdefgh", "abcdefgh", 8U, 8U, false), __LINE__);
    test.IsFalse(StringUtils::IsGreater("201811111", "20189999", 4U, 4U, false), __LINE__);
    test.IsFalse(StringUtils::IsGreater("2018", "2018", 4U, 4U, false), __LINE__);
    test.IsFalse(StringUtils::IsGreater("20189999", "201911111", 4U, 4U, true), __LINE__);
    test.IsFalse(StringUtils::IsGreater("2018", "2019", 4U, 4U, false), __LINE__);
    test.IsFalse(StringUtils::IsGreater("20189999", "201911111", 4U, 4U, false), __LINE__);
    test.IsFalse(StringUtils::IsGreater("2018", "2019", 4U, 4U, true), __LINE__);
    test.IsFalse(StringUtils::IsGreater("2018", "2019", 3U, 3U, false), __LINE__);
    test.IsFalse(StringUtils::IsGreater("2019", "2018", 3U, 3U, false), __LINE__);
    test.IsFalse(StringUtils::IsGreater("2018", "2021", 4U, 4U, true), __LINE__);
    test.IsFalse(StringUtils::IsGreater("2018", "2021", 4U, 4U, false), __LINE__);
}

static void TestIsLess(QTest &test) {
    test.IsTrue(StringUtils::IsLess("", "", 0U, 0U, true), __LINE__);
    test.IsTrue(StringUtils::IsLess("a", "b", 1U, 1U, false), __LINE__);
    test.IsTrue(StringUtils::IsLess("a", "a", 1U, 1U, true), __LINE__);
    test.IsTrue(StringUtils::IsLess("A", "a", 1U, 1U, false), __LINE__);
    test.IsTrue(StringUtils::IsLess("A", "a", 1U, 1U, true), __LINE__);
    test.IsTrue(StringUtils::IsLess("aA", "aa", 2U, 2U, false), __LINE__);
    test.IsTrue(StringUtils::IsLess("aA", "aa", 2U, 2U, true), __LINE__);
    test.IsTrue(StringUtils::IsLess("aBcdefgh", "abcdefgh", 8U, 8U, true), __LINE__);
    test.IsTrue(StringUtils::IsLess("aBcdefgh", "abcdefgh", 8U, 8U, false), __LINE__);
    test.IsTrue(StringUtils::IsLess("abcdefgh", "abcdefgh", 8U, 8U, true), __LINE__);
    test.IsTrue(StringUtils::IsLess("20189999", "201811111", 4U, 4U, true), __LINE__);
    test.IsTrue(StringUtils::IsLess("2018", "2018", 4U, 4U, true), __LINE__);
    test.IsTrue(StringUtils::IsLess("20189999", "201911111", 4U, 4U, true), __LINE__);
    test.IsTrue(StringUtils::IsLess("2018", "2019", 4U, 4U, false), __LINE__);
    test.IsTrue(StringUtils::IsLess("20189999", "201911111", 4U, 4U, false), __LINE__);
    test.IsTrue(StringUtils::IsLess("2018", "2019", 4U, 4U, true), __LINE__);
    test.IsTrue(StringUtils::IsLess("2018", "2019", 3U, 3U, true), __LINE__);
    test.IsTrue(StringUtils::IsLess("2018", "2021", 4U, 4U, true), __LINE__);
    test.IsTrue(StringUtils::IsLess("2018", "2021", 4U, 4U, false), __LINE__);

    test.IsFalse(StringUtils::IsLess("", "", 0U, 0U, false), __LINE__);
    test.IsFalse(StringUtils::IsLess("a", "a", 1U, 1U, false), __LINE__);
    test.IsFalse(StringUtils::IsLess("b", "a", 1U, 1U, false), __LINE__);
    test.IsFalse(StringUtils::IsLess("a", "A", 1U, 1U, false), __LINE__);
    test.IsFalse(StringUtils::IsLess("a", "A", 1U, 1U, true), __LINE__);
    test.IsFalse(StringUtils::IsLess("aa", "aA", 2U, 2U, false), __LINE__);
    test.IsFalse(StringUtils::IsLess("aa", "aA", 2U, 2U, true), __LINE__);
    test.IsFalse(StringUtils::IsLess("abcdefgh", "aBcdefgh", 8U, 8U, true), __LINE__);
    test.IsFalse(StringUtils::IsLess("abcdefgh", "aBcdefgh", 8U, 8U, false), __LINE__);
    test.IsFalse(StringUtils::IsLess("abcdefgh", "abcdefgh", 8U, 8U, false), __LINE__);
    test.IsFalse(StringUtils::IsLess("201811111", "20189999", 4U, 4U, false), __LINE__);
    test.IsFalse(StringUtils::IsLess("2018", "2018", 4U, 4U, false), __LINE__);
    test.IsFalse(StringUtils::IsLess("20199999", "201811111", 4U, 4U, true), __LINE__);
    test.IsFalse(StringUtils::IsLess("2019", "2018", 4U, 4U, false), __LINE__);
    test.IsFalse(StringUtils::IsLess("201911111", "20189999", 4U, 4U, false), __LINE__);
    test.IsFalse(StringUtils::IsLess("2019", "2018", 4U, 4U, true), __LINE__);
    test.IsFalse(StringUtils::IsLess("2019", "2018", 3U, 3U, false), __LINE__);
    test.IsFalse(StringUtils::IsLess("2018", "2019", 3U, 3U, false), __LINE__);
    test.IsFalse(StringUtils::IsLess("2021", "2018", 4U, 4U, true), __LINE__);
    test.IsFalse(StringUtils::IsLess("2021", "2018", 4U, 4U, false), __LINE__);
}

static int RunStringUtilsTests() {
    QTest test{"StringUtils.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("Hash Test", TestHash);
    test.Test("Count Test", TestCount);
    test.Test("TrimLeft Test", TestTrimLeft);
    test.Test("TrimLeft Test", TestTrimRight);
    test.Test("Trim Test", TestTrim);
    test.Test("IsEqual Test", TestIsEqual);
    test.Test("IsGreater Test", TestIsGreater);
    test.Test("IsLess Test", TestIsLess);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
