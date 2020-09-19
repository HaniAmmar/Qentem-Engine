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

#include "StringUtils.hpp"
#include "TestHelper.hpp"

#ifndef QENTEM_STRINGUTILS_TESTS_H_
#define QENTEM_STRINGUTILS_TESTS_H_

namespace Qentem {
namespace Test {

static int TestCount() {
    ULong length = StringUtils::Count("");
    SHOULD_EQUAL_VALUE(length, 0, "length");

    length = StringUtils::Count("a");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    length = StringUtils::Count("abcde");
    SHOULD_EQUAL_VALUE(length, 5, "length");

    length = StringUtils::Count("\0");
    SHOULD_EQUAL_VALUE(length, 0, "length");

    length = StringUtils::Count("1234567");
    SHOULD_EQUAL_VALUE(length, 7, "length");

    length = StringUtils::Count("123456781234567");
    SHOULD_EQUAL_VALUE(length, 15, "length");

    length = StringUtils::Count("1234567812345678123456781234567");
    SHOULD_EQUAL_VALUE(length, 31, "length");

    length = StringUtils::Count(
        "123456781234567812345678123456781234567812345678123456781234567");
    SHOULD_EQUAL_VALUE(length, 63, "length");

    END_SUB_TEST;
}

static int TestLeftTrim() {
    int offset = 0;

    StringUtils::StartTrim("", offset, 0);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    StringUtils::StartTrim("", offset, 1);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    StringUtils::StartTrim("", offset, 10);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    StringUtils::StartTrim("a", offset, 1);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    StringUtils::StartTrim("abc", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    StringUtils::StartTrim("a ", offset, 2);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    StringUtils::StartTrim("abc ", offset, 4);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    StringUtils::StartTrim("a  ", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    StringUtils::StartTrim("abc  ", offset, 5);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    StringUtils::StartTrim("a   ", offset, 4);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    StringUtils::StartTrim("abc   ", offset, 6);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");

    offset = 0;
    StringUtils::StartTrim(" a", offset, 2);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 0;
    StringUtils::StartTrim(" abc", offset, 4);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 0;
    StringUtils::StartTrim("  a", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 0;
    StringUtils::StartTrim("  abc", offset, 5);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 0;
    StringUtils::StartTrim("   a", offset, 4);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 0;
    StringUtils::StartTrim("   abc", offset, 6);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 0;
    StringUtils::StartTrim(" a ", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 0;
    StringUtils::StartTrim(" abc ", offset, 5);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 0;
    StringUtils::StartTrim("  a ", offset, 4);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 0;
    StringUtils::StartTrim("  abc ", offset, 6);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 0;
    StringUtils::StartTrim("   a  ", offset, 6);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 0;
    StringUtils::StartTrim("   abc  ", offset, 8);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 0;
    StringUtils::StartTrim(" ", offset, 1);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 0;
    StringUtils::StartTrim("  ", offset, 2);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 0;
    StringUtils::StartTrim("   ", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 1;
    StringUtils::StartTrim(" a ", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 1;
    StringUtils::StartTrim(" abc ", offset, 5);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 2;
    StringUtils::StartTrim("  a ", offset, 4);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 2;
    StringUtils::StartTrim("  abc ", offset, 6);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 2;
    StringUtils::StartTrim("   a  ", offset, 6);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 2;
    StringUtils::StartTrim("   abc  ", offset, 8);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 1;
    StringUtils::StartTrim("  ", offset, 2);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    offset = 2;
    StringUtils::StartTrim("   ", offset, 3);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");

    offset = 0;
    StringUtils::StartTrim("  ", offset, 1);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");

    offset = 1;
    StringUtils::StartTrim("   ", offset, 2);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");

    END_SUB_TEST;
}

static int TestSoftTrim() {
    ULong offset = 0;
    ULong length = 0;

    StringUtils::SoftTrim("", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 0, "length");

    offset = 0;
    length = 1;
    StringUtils::SoftTrim(" ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 0, "length");

    offset = 0;
    length = 1;
    StringUtils::SoftTrim("  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 0, "length");

    offset = 0;
    length = 2;
    StringUtils::SoftTrim("  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 0, "length");

    offset = 0;
    length = 2;
    StringUtils::SoftTrim(" a", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 4;
    StringUtils::SoftTrim(" abc", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 1;
    length = 1;
    StringUtils::SoftTrim(" a", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 1;
    length = 3;
    StringUtils::SoftTrim(" abc", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 2;
    length = 1;
    StringUtils::SoftTrim("  a", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 2;
    length = 3;
    StringUtils::SoftTrim("  abc", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 3;
    StringUtils::SoftTrim("  a", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 5;
    StringUtils::SoftTrim("  abc", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 4;
    StringUtils::SoftTrim("   a", offset, length);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 6;
    StringUtils::SoftTrim("   abc", offset, length);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 2;
    StringUtils::SoftTrim("a ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 4;
    StringUtils::SoftTrim("abc ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 1;
    StringUtils::SoftTrim("a ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 3;
    StringUtils::SoftTrim("abc ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 1;
    StringUtils::SoftTrim("a  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 3;
    StringUtils::SoftTrim("abc  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 3;
    StringUtils::SoftTrim("a  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 5;
    StringUtils::SoftTrim("abc  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 4;
    StringUtils::SoftTrim("a   ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 6;
    StringUtils::SoftTrim("abc   ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 0, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 3;
    StringUtils::SoftTrim(" a ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 5;
    StringUtils::SoftTrim(" abc ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 5;
    StringUtils::SoftTrim("  a  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 7;
    StringUtils::SoftTrim("  abc  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 0;
    length = 7;
    StringUtils::SoftTrim("   a   ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 0;
    length = 9;
    StringUtils::SoftTrim("   abc   ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 1;
    length = 1;
    StringUtils::SoftTrim(" a ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 1;
    length = 3;
    StringUtils::SoftTrim(" abc ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 1, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 2;
    length = 1;
    StringUtils::SoftTrim("  a  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 2;
    length = 3;
    StringUtils::SoftTrim("  abc  ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 2, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    offset = 3;
    length = 1;
    StringUtils::SoftTrim("   a   ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");
    SHOULD_EQUAL_VALUE(length, 1, "length");

    offset = 3;
    length = 3;
    StringUtils::SoftTrim("   abc   ", offset, length);
    SHOULD_EQUAL_VALUE(offset, 3, "offset");
    SHOULD_EQUAL_VALUE(length, 3, "length");

    END_SUB_TEST;
}

static int TestIsEqual() {
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("", "", 0), "IsEqual");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("a", "a", 1), "IsEqual");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("ab", "ab", 2), "IsEqual");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("abcdefgh", "abcdefgh", 8),
                      "IsEqual");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("a", "abcdefgh", 1), "IsEqual");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("abc", "abcdefgh", 3), "IsEqual");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("abcdefgh", "a", 1), "IsEqual");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("abcdefgh", "abc", 3), "IsEqual");
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual("0123456789123456", "0123456789123456", 16),
        "IsEqual");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("01234567891234560123456789123456",
                                           "01234567891234560123456789123456",
                                           32),
                      "IsEqual");
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual(
            "0123456789123456012345678912345601234567891234560123456789123456",
            "0123456789123456012345678912345601234567891234560123456789123456",
            64),
        "IsEqual");

    SHOULD_NOT_EQUAL_TRUE(StringUtils::IsEqual("a", "b", 1), "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(StringUtils::IsEqual("ab", "ba", 2), "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(StringUtils::IsEqual("abcdefgh", "--------", 8),
                          "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(StringUtils::IsEqual("h", "abcdefgh", 1), "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(StringUtils::IsEqual("abc", "def", 3), "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(StringUtils::IsEqual("abcdefgh", "b", 1), "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(StringUtils::IsEqual("abcdefgh", "cde", 3),
                          "IsEqual");

    SHOULD_NOT_EQUAL_TRUE(
        StringUtils::IsEqual("01234567891234568", "0123456789123456", 17),
        "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(
        StringUtils::IsEqual("012345678912345601234567891234567",
                             "012345678912345601234567891234568", 33),
        "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(
        StringUtils::IsEqual(
            "01234567829123456012345678912345601234567891234560123456789123456",
            "00234567829123456012345678912345601234567891234560123456789123456",
            65),
        "IsEqual");

    SHOULD_NOT_EQUAL_TRUE(
        StringUtils::IsEqual(
            "01234567829123456012345678912345601234567891234560123456789123456",
            "01234567829123456012345678912345601234567891234560123456789123457",
            65),
        "IsEqual");

    SHOULD_NOT_EQUAL_TRUE(
        StringUtils::IsEqual(
            "01234567829123456012345678912345601234567891234560123456789123456",
            "01234567829123456012345678912345601234567891235560123456789123456",
            65),
        "IsEqual");

    SHOULD_NOT_EQUAL_TRUE(
        StringUtils::IsEqual(
            "01234567829123456012345678912345601234567891234560123456789123456",
            "01234567829123456012335678912345601234567891234560123456789123456",
            65),
        "IsEqual");

    SHOULD_NOT_EQUAL_TRUE(
        StringUtils::IsEqual(
            "a2345678912345678912345678912345612340678912345678912345678912345w",
            "a2345678912345678912345678912345612345678912345678912345678912345w",
            65),
        "IsEqual");

    END_SUB_TEST;
}

static int RunStringUtilsTests() {
    STARTING_TEST("StringUtils.hpp");

    START_TEST("Count Test", TestCount);
    START_TEST("StartTrim Test", TestLeftTrim);
    START_TEST("SoftTrim Test", TestSoftTrim);
    START_TEST("IsEqual Test", TestIsEqual);

    END_TEST("StringUtils.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
