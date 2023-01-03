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

#include "StringUtils.hpp"

#ifndef QENTEM_STRINGUTILS_TESTS_H_
#define QENTEM_STRINGUTILS_TESTS_H_

namespace Qentem {
namespace Test {

static void TestHash(TestHelper &helper) {
    SizeT hash;
    SizeT hash2;
    SizeT hash3;
    SizeT hash4;

    hash = StringUtils::Hash("", 0U);
    helper.NotEqual(hash, 0U, "hash", "0", __LINE__);

    hash  = StringUtils::Hash("1", 1U);
    hash2 = StringUtils::Hash("0", 1U);
    helper.NotEqual(hash, 0U, "hash", "0", __LINE__);
    helper.NotEqual(hash2, 0U, "hash2", "0", __LINE__);
    helper.NotEqual(hash, hash2, "hash", "hash2", __LINE__);

    hash3 = StringUtils::Hash("10", 2U);
    hash4 = StringUtils::Hash("01", 2U);
    helper.NotEqual(hash3, 0U, "hash3", "0", __LINE__);
    helper.NotEqual(hash4, 0U, "hash4", "0", __LINE__);
    helper.NotEqual(hash, hash3, "hash", "hash3", __LINE__);
    helper.NotEqual(hash2, hash3, "hash2", "hash3", __LINE__);
    helper.NotEqual(hash, hash4, "hash", "hash4", __LINE__);
    helper.NotEqual(hash2, hash4, "hash2", "hash4", __LINE__);
    helper.NotEqual(hash3, hash4, "hash3", "hash4", __LINE__);

    hash  = StringUtils::Hash("100", 3U);
    hash2 = StringUtils::Hash("001", 3U);
    helper.NotEqual(hash, 0U, "hash", "0", __LINE__);
    helper.NotEqual(hash2, 0U, "hash2", "0", __LINE__);
    helper.NotEqual(hash, hash3, "hash", "hash3", __LINE__);
    helper.NotEqual(hash2, hash3, "hash2", "hash3", __LINE__);
    helper.NotEqual(hash, hash4, "hash", "hash4", __LINE__);
    helper.NotEqual(hash2, hash4, "hash2", "hash4", __LINE__);

    hash  = StringUtils::Hash("abc", 3U);
    hash2 = StringUtils::Hash("cba", 3U);
    helper.NotEqual(hash, hash2, "hash", "hash2", __LINE__);
    helper.NotEqual(hash, 0U, "hash", "0", __LINE__);
    helper.NotEqual(hash2, 0U, "hash2", "0", __LINE__);
}

static void TestCount(TestHelper &helper) {
    SizeT length = StringUtils::Count("");
    helper.Equal(length, 0U, "length", __LINE__);

    length = StringUtils::Count("a");
    helper.Equal(length, 1U, "length", __LINE__);

    length = StringUtils::Count("abcde");
    helper.Equal(length, 5U, "length", __LINE__);

    length = StringUtils::Count("\0");
    helper.Equal(length, 0U, "length", __LINE__);

    length = StringUtils::Count("1234567");
    helper.Equal(length, 7U, "length", __LINE__);

    length = StringUtils::Count("123456781234567");
    helper.Equal(length, 15U, "length", __LINE__);

    length = StringUtils::Count("1234567812345678123456781234567");
    helper.Equal(length, 31U, "length", __LINE__);

    length = StringUtils::Count("123456781234567812345678123456781234567812345678123456781234567");
    helper.Equal(length, 63U, "length", __LINE__);
}

static void TestLeftTrim(TestHelper &helper) {
    unsigned int offset = 0;

    StringUtils::TrimLeft("", offset, 0U);
    helper.Equal(offset, 0U, "offset", __LINE__);

    StringUtils::TrimLeft("", offset, 1U);
    helper.Equal(offset, 0U, "offset", __LINE__);

    StringUtils::TrimLeft("", offset, 10U);
    helper.Equal(offset, 0U, "offset", __LINE__);

    StringUtils::TrimLeft("a", offset, 1U);
    helper.Equal(offset, 0U, "offset", __LINE__);

    StringUtils::TrimLeft("abc", offset, 3U);
    helper.Equal(offset, 0U, "offset", __LINE__);

    StringUtils::TrimLeft("a ", offset, 2U);
    helper.Equal(offset, 0U, "offset", __LINE__);

    StringUtils::TrimLeft("abc ", offset, 4U);
    helper.Equal(offset, 0U, "offset", __LINE__);

    StringUtils::TrimLeft("a  ", offset, 3U);
    helper.Equal(offset, 0U, "offset", __LINE__);

    StringUtils::TrimLeft("abc  ", offset, 5U);
    helper.Equal(offset, 0U, "offset", __LINE__);

    StringUtils::TrimLeft("a   ", offset, 4U);
    helper.Equal(offset, 0U, "offset", __LINE__);

    StringUtils::TrimLeft("abc   ", offset, 6U);
    helper.Equal(offset, 0U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft(" a", offset, 2U);
    helper.Equal(offset, 1U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft(" abc", offset, 4U);
    helper.Equal(offset, 1U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft("  a", offset, 3U);
    helper.Equal(offset, 2U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft("  abc", offset, 5U);
    helper.Equal(offset, 2U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft("   a", offset, 4U);
    helper.Equal(offset, 3U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft("   abc", offset, 6U);
    helper.Equal(offset, 3U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft(" a ", offset, 3U);
    helper.Equal(offset, 1U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft(" abc ", offset, 5U);
    helper.Equal(offset, 1U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft("  a ", offset, 4U);
    helper.Equal(offset, 2U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft("  abc ", offset, 6U);
    helper.Equal(offset, 2U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft("   a  ", offset, 6U);
    helper.Equal(offset, 3U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft("   abc  ", offset, 8U);
    helper.Equal(offset, 3U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft(" ", offset, 1U);
    helper.Equal(offset, 1U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft("  ", offset, 2U);
    helper.Equal(offset, 2U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft("   ", offset, 3U);
    helper.Equal(offset, 3U, "offset", __LINE__);

    offset = 1;
    StringUtils::TrimLeft(" a ", offset, 3U);
    helper.Equal(offset, 1U, "offset", __LINE__);

    offset = 1;
    StringUtils::TrimLeft(" abc ", offset, 5U);
    helper.Equal(offset, 1U, "offset", __LINE__);

    offset = 2;
    StringUtils::TrimLeft("  a ", offset, 4U);
    helper.Equal(offset, 2U, "offset", __LINE__);

    offset = 2;
    StringUtils::TrimLeft("  abc ", offset, 6U);
    helper.Equal(offset, 2U, "offset", __LINE__);

    offset = 2;
    StringUtils::TrimLeft("   a  ", offset, 6U);
    helper.Equal(offset, 3U, "offset", __LINE__);

    offset = 2;
    StringUtils::TrimLeft("   abc  ", offset, 8U);
    helper.Equal(offset, 3U, "offset", __LINE__);

    offset = 1;
    StringUtils::TrimLeft("  ", offset, 2U);
    helper.Equal(offset, 2U, "offset", __LINE__);

    offset = 2;
    StringUtils::TrimLeft("   ", offset, 3U);
    helper.Equal(offset, 3U, "offset", __LINE__);

    offset = 0;
    StringUtils::TrimLeft("  ", offset, 1U);
    helper.Equal(offset, 1U, "offset", __LINE__);

    offset = 1;
    StringUtils::TrimLeft("   ", offset, 2U);
    helper.Equal(offset, 2U, "offset", __LINE__);
}

static void TestSoftTrim(TestHelper &helper) {
    SizeT offset = 0;
    SizeT length = 0;

    StringUtils::Trim("", offset, length);
    helper.Equal(offset, 0U, "offset", __LINE__);
    helper.Equal(length, 0U, "length", __LINE__);

    offset = 0;
    length = 1;
    StringUtils::Trim(" ", offset, length);
    helper.Equal(offset, 1U, "offset", __LINE__);
    helper.Equal(length, 0U, "length", __LINE__);

    offset = 0;
    length = 1;
    StringUtils::Trim("  ", offset, length);
    helper.Equal(offset, 1U, "offset", __LINE__);
    helper.Equal(length, 0U, "length", __LINE__);

    offset = 0;
    length = 2;
    StringUtils::Trim("  ", offset, length);
    helper.Equal(offset, 2U, "offset", __LINE__);
    helper.Equal(length, 0U, "length", __LINE__);

    offset = 0;
    length = 2;
    StringUtils::Trim(" a", offset, length);
    helper.Equal(offset, 1U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 0;
    length = 4;
    StringUtils::Trim(" abc", offset, length);
    helper.Equal(offset, 1U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 1;
    length = 1;
    StringUtils::Trim(" a", offset, length);
    helper.Equal(offset, 1U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 1;
    length = 3;
    StringUtils::Trim(" abc", offset, length);
    helper.Equal(offset, 1U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 2;
    length = 1;
    StringUtils::Trim("  a", offset, length);
    helper.Equal(offset, 2U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 2;
    length = 3;
    StringUtils::Trim("  abc", offset, length);
    helper.Equal(offset, 2U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 0;
    length = 3;
    StringUtils::Trim("  a", offset, length);
    helper.Equal(offset, 2U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 0;
    length = 5;
    StringUtils::Trim("  abc", offset, length);
    helper.Equal(offset, 2U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 0;
    length = 4;
    StringUtils::Trim("   a", offset, length);
    helper.Equal(offset, 3U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 0;
    length = 6;
    StringUtils::Trim("   abc", offset, length);
    helper.Equal(offset, 3U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 0;
    length = 2;
    StringUtils::Trim("a ", offset, length);
    helper.Equal(offset, 0U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 0;
    length = 4;
    StringUtils::Trim("abc ", offset, length);
    helper.Equal(offset, 0U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 0;
    length = 1;
    StringUtils::Trim("a ", offset, length);
    helper.Equal(offset, 0U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 0;
    length = 3;
    StringUtils::Trim("abc ", offset, length);
    helper.Equal(offset, 0U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 0;
    length = 1;
    StringUtils::Trim("a  ", offset, length);
    helper.Equal(offset, 0U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 0;
    length = 3;
    StringUtils::Trim("abc  ", offset, length);
    helper.Equal(offset, 0U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 0;
    length = 3;
    StringUtils::Trim("a  ", offset, length);
    helper.Equal(offset, 0U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 0;
    length = 5;
    StringUtils::Trim("abc  ", offset, length);
    helper.Equal(offset, 0U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 0;
    length = 4;
    StringUtils::Trim("a   ", offset, length);
    helper.Equal(offset, 0U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 0;
    length = 6;
    StringUtils::Trim("abc   ", offset, length);
    helper.Equal(offset, 0U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 0;
    length = 3;
    StringUtils::Trim(" a ", offset, length);
    helper.Equal(offset, 1U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 0;
    length = 5;
    StringUtils::Trim(" abc ", offset, length);
    helper.Equal(offset, 1U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 0;
    length = 5;
    StringUtils::Trim("  a  ", offset, length);
    helper.Equal(offset, 2U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 0;
    length = 7;
    StringUtils::Trim("  abc  ", offset, length);
    helper.Equal(offset, 2U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 0;
    length = 7;
    StringUtils::Trim("   a   ", offset, length);
    helper.Equal(offset, 3U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 0;
    length = 9;
    StringUtils::Trim("   abc   ", offset, length);
    helper.Equal(offset, 3U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 1;
    length = 1;
    StringUtils::Trim(" a ", offset, length);
    helper.Equal(offset, 1U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 1;
    length = 3;
    StringUtils::Trim(" abc ", offset, length);
    helper.Equal(offset, 1U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 2;
    length = 1;
    StringUtils::Trim("  a  ", offset, length);
    helper.Equal(offset, 2U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 2;
    length = 3;
    StringUtils::Trim("  abc  ", offset, length);
    helper.Equal(offset, 2U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);

    offset = 3;
    length = 1;
    StringUtils::Trim("   a   ", offset, length);
    helper.Equal(offset, 3U, "offset", __LINE__);
    helper.Equal(length, 1U, "length", __LINE__);

    offset = 3;
    length = 3;
    StringUtils::Trim("   abc   ", offset, length);
    helper.Equal(offset, 3U, "offset", __LINE__);
    helper.Equal(length, 3U, "length", __LINE__);
}

static void TestIsEqual(TestHelper &helper) {
    helper.EqualsTrue(StringUtils::IsEqual("", "", 0U), "IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("a", "a", 1U), "IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("ab", "ab", 2U), "IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("abcdefgh", "abcdefgh", 8U), "IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("a", "abcdefgh", 1U), "IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("abc", "abcdefgh", 3U), "IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("abcdefgh", "a", 1U), "IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("abcdefgh", "abc", 3U), "IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("0123456789123456", "0123456789123456", 16U), "IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("01234567891234560123456789123456", "01234567891234560123456789123456", 32U),
                      "IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("0123456789123456012345678912345601234567891234560123456789123456",
                                           "0123456789123456012345678912345601234567891234560123456789123456", 64U),
                      "IsEqual", __LINE__);

    helper.EqualsFalse(StringUtils::IsEqual("a", "b", 1U), "IsEqual", __LINE__);
    helper.EqualsFalse(StringUtils::IsEqual("ab", "ba", 2U), "IsEqual", __LINE__);
    helper.EqualsFalse(StringUtils::IsEqual("abcdefgh", "--------", 8U), "IsEqual", __LINE__);
    helper.EqualsFalse(StringUtils::IsEqual("h", "abcdefgh", 1U), "IsEqual", __LINE__);
    helper.EqualsFalse(StringUtils::IsEqual("abc", "def", 3U), "IsEqual", __LINE__);
    helper.EqualsFalse(StringUtils::IsEqual("abcdefgh", "b", 1U), "IsEqual", __LINE__);
    helper.EqualsFalse(StringUtils::IsEqual("abcdefgh", "cde", 3U), "IsEqual", __LINE__);

    helper.EqualsFalse(StringUtils::IsEqual("01234567891234568", "0123456789123456", 17U), "IsEqual", __LINE__);
    helper.EqualsFalse(
        StringUtils::IsEqual("012345678912345601234567891234567", "012345678912345601234567891234568", 33U), "IsEqual",
        __LINE__);
    helper.EqualsFalse(StringUtils::IsEqual("01234567829123456012345678912345601234567891234560123456789123456",
                                            "00234567829123456012345678912345601234567891234560123456789123456", 65U),
                       "IsEqual", __LINE__);

    helper.EqualsFalse(StringUtils::IsEqual("01234567829123456012345678912345601234567891234560123456789123456",
                                            "01234567829123456012345678912345601234567891234560123456789123457", 65U),
                       "IsEqual", __LINE__);

    helper.EqualsFalse(StringUtils::IsEqual("01234567829123456012345678912345601234567891234560123456789123456",
                                            "01234567829123456012345678912345601234567891235560123456789123456", 65U),
                       "IsEqual", __LINE__);

    helper.EqualsFalse(StringUtils::IsEqual("01234567829123456012345678912345601234567891234560123456789123456",
                                            "01234567829123456012335678912345601234567891234560123456789123456", 65U),
                       "IsEqual", __LINE__);

    helper.EqualsFalse(StringUtils::IsEqual("a2345678912345678912345678912345612340678912345678912345678912345w",
                                            "a2345678912345678912345678912345612345678912345678912345678912345w", 65U),
                       "IsEqual", __LINE__);
}

static void TestIsBigger(TestHelper &helper) {
    helper.EqualsTrue(StringUtils::IsBigger("", "", 0U, 0U, true), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("b", "a", 1U, 1U, false), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("a", "a", 1U, 1U, true), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("a", "A", 1U, 1U, false), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("a", "A", 1U, 1U, true), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("aa", "aA", 2U, 2U, false), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("aa", "aA", 2U, 2U, true), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("abcdefgh", "aBcdefgh", 8U, 8U, true), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("abcdefgh", "aBcdefgh", 8U, 8U, false), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("abcdefgh", "abcdefgh", 8U, 8U, true), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("201811111", "20189999", 4U, 4U, true), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("2018", "2018", 4U, 4U, true), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("201911111", "20189999", 4U, 4U, true), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("2019", "2018", 4U, 4U, false), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("201911111", "20189999", 4U, 4U, false), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("2019", "2018", 4U, 4U, true), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("2019", "2018", 3U, 3U, true), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("2021", "2018", 4U, 4U, true), "IsBigger", __LINE__);
    helper.EqualsTrue(StringUtils::IsBigger("2021", "2018", 4U, 4U, false), "IsBigger", __LINE__);

    helper.EqualsFalse(StringUtils::IsBigger("", "", 0U, 0U, false), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("a", "a", 1U, 1U, false), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("a", "b", 1U, 1U, false), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("A", "a", 1U, 1U, false), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("A", "a", 1U, 1U, true), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("aA", "aa", 2U, 2U, false), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("aA", "aa", 2U, 2U, true), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("aBcdefgh", "abcdefgh", 8U, 8U, true), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("aBcdefgh", "abcdefgh", 8U, 8U, false), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("abcdefgh", "abcdefgh", 8U, 8U, false), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("201811111", "20189999", 4U, 4U, false), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("2018", "2018", 4U, 4U, false), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("20189999", "201911111", 4U, 4U, true), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("2018", "2019", 4U, 4U, false), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("20189999", "201911111", 4U, 4U, false), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("2018", "2019", 4U, 4U, true), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("2018", "2019", 3U, 3U, false), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("2019", "2018", 3U, 3U, false), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("2018", "2021", 4U, 4U, true), "IsBigger", __LINE__);
    helper.EqualsFalse(StringUtils::IsBigger("2018", "2021", 4U, 4U, false), "IsBigger", __LINE__);
}

static void TestIsLess(TestHelper &helper) {
    helper.EqualsTrue(StringUtils::IsLess("", "", 0U, 0U, true), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("a", "b", 1U, 1U, false), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("a", "a", 1U, 1U, true), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("A", "a", 1U, 1U, false), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("A", "a", 1U, 1U, true), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("aA", "aa", 2U, 2U, false), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("aA", "aa", 2U, 2U, true), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("aBcdefgh", "abcdefgh", 8U, 8U, true), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("aBcdefgh", "abcdefgh", 8U, 8U, false), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("abcdefgh", "abcdefgh", 8U, 8U, true), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("20189999", "201811111", 4U, 4U, true), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("2018", "2018", 4U, 4U, true), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("20189999", "201911111", 4U, 4U, true), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("2018", "2019", 4U, 4U, false), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("20189999", "201911111", 4U, 4U, false), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("2018", "2019", 4U, 4U, true), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("2018", "2019", 3U, 3U, true), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("2018", "2021", 4U, 4U, true), "IsLess", __LINE__);
    helper.EqualsTrue(StringUtils::IsLess("2018", "2021", 4U, 4U, false), "IsLess", __LINE__);

    helper.EqualsFalse(StringUtils::IsLess("", "", 0U, 0U, false), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("a", "a", 1U, 1U, false), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("b", "a", 1U, 1U, false), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("a", "A", 1U, 1U, false), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("a", "A", 1U, 1U, true), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("aa", "aA", 2U, 2U, false), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("aa", "aA", 2U, 2U, true), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("abcdefgh", "aBcdefgh", 8U, 8U, true), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("abcdefgh", "aBcdefgh", 8U, 8U, false), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("abcdefgh", "abcdefgh", 8U, 8U, false), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("201811111", "20189999", 4U, 4U, false), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("2018", "2018", 4U, 4U, false), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("20199999", "201811111", 4U, 4U, true), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("2019", "2018", 4U, 4U, false), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("201911111", "20189999", 4U, 4U, false), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("2019", "2018", 4U, 4U, true), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("2019", "2018", 3U, 3U, false), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("2018", "2019", 3U, 3U, false), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("2021", "2018", 4U, 4U, true), "IsLess", __LINE__);
    helper.EqualsFalse(StringUtils::IsLess("2021", "2018", 4U, 4U, false), "IsLess", __LINE__);
}

static int RunStringUtilsTests() {
    TestHelper helper{"StringUtils.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("Hash Test", TestHash);
    helper.Test("Count Test", TestCount);
    helper.Test("TrimLeft Test", TestLeftTrim);
    helper.Test("Trim Test", TestSoftTrim);
    helper.Test("IsEqual Test", TestIsEqual);
    helper.Test("IsBigger Test", TestIsBigger);
    helper.Test("IsLess Test", TestIsLess);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
