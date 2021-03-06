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

static int TestHash() {
    SizeT hash;
    SizeT hash2;
    SizeT hash3;
    SizeT hash4;

    hash = StringUtils::Hash("", 0);
    NOT_EQ_TO(hash, 0, "hash", "0");

    hash  = StringUtils::Hash("1", 1);
    hash2 = StringUtils::Hash("0", 1);
    NOT_EQ_TO(hash, 0, "hash", "0");
    NOT_EQ_TO(hash2, 0, "hash2", "0");
    NOT_EQ_TO(hash, hash2, "hash", "hash2");

    hash3 = StringUtils::Hash("10", 2);
    hash4 = StringUtils::Hash("01", 2);
    NOT_EQ_TO(hash3, 0, "hash3", "0");
    NOT_EQ_TO(hash4, 0, "hash4", "0");
    NOT_EQ_TO(hash, hash3, "hash", "hash3");
    NOT_EQ_TO(hash2, hash3, "hash2", "hash3");
    NOT_EQ_TO(hash, hash4, "hash", "hash4");
    NOT_EQ_TO(hash2, hash4, "hash2", "hash4");
    NOT_EQ_TO(hash3, hash4, "hash3", "hash4");

    hash  = StringUtils::Hash("100", 3);
    hash2 = StringUtils::Hash("001", 3);
    NOT_EQ_TO(hash, 0, "hash", "0");
    NOT_EQ_TO(hash2, 0, "hash2", "0");
    NOT_EQ_TO(hash, hash3, "hash", "hash3");
    NOT_EQ_TO(hash2, hash3, "hash2", "hash3");
    NOT_EQ_TO(hash, hash4, "hash", "hash4");
    NOT_EQ_TO(hash2, hash4, "hash2", "hash4");

    hash  = StringUtils::Hash("abc", 3);
    hash2 = StringUtils::Hash("cba", 3);
    NOT_EQ_TO(hash, hash2, "hash", "hash2");
    NOT_EQ_TO(hash, 0, "hash", "0");
    NOT_EQ_TO(hash2, 0, "hash2", "0");

    END_SUB_TEST;
}

static int TestCount() {
    SizeT length = StringUtils::Count("");
    EQ_VALUE(length, 0, "length");

    length = StringUtils::Count("a");
    EQ_VALUE(length, 1, "length");

    length = StringUtils::Count("abcde");
    EQ_VALUE(length, 5, "length");

    length = StringUtils::Count("\0");
    EQ_VALUE(length, 0, "length");

    length = StringUtils::Count("1234567");
    EQ_VALUE(length, 7, "length");

    length = StringUtils::Count("123456781234567");
    EQ_VALUE(length, 15, "length");

    length = StringUtils::Count("1234567812345678123456781234567");
    EQ_VALUE(length, 31, "length");

    length = StringUtils::Count(
        "123456781234567812345678123456781234567812345678123456781234567");
    EQ_VALUE(length, 63, "length");

    END_SUB_TEST;
}

static int TestLeftTrim() {
    int offset = 0;

    StringUtils::TrimLeft("", offset, 0);
    EQ_VALUE(offset, 0, "offset");

    StringUtils::TrimLeft("", offset, 1);
    EQ_VALUE(offset, 0, "offset");

    StringUtils::TrimLeft("", offset, 10);
    EQ_VALUE(offset, 0, "offset");

    StringUtils::TrimLeft("a", offset, 1);
    EQ_VALUE(offset, 0, "offset");

    StringUtils::TrimLeft("abc", offset, 3);
    EQ_VALUE(offset, 0, "offset");

    StringUtils::TrimLeft("a ", offset, 2);
    EQ_VALUE(offset, 0, "offset");

    StringUtils::TrimLeft("abc ", offset, 4);
    EQ_VALUE(offset, 0, "offset");

    StringUtils::TrimLeft("a  ", offset, 3);
    EQ_VALUE(offset, 0, "offset");

    StringUtils::TrimLeft("abc  ", offset, 5);
    EQ_VALUE(offset, 0, "offset");

    StringUtils::TrimLeft("a   ", offset, 4);
    EQ_VALUE(offset, 0, "offset");

    StringUtils::TrimLeft("abc   ", offset, 6);
    EQ_VALUE(offset, 0, "offset");

    offset = 0;
    StringUtils::TrimLeft(" a", offset, 2);
    EQ_VALUE(offset, 1, "offset");

    offset = 0;
    StringUtils::TrimLeft(" abc", offset, 4);
    EQ_VALUE(offset, 1, "offset");

    offset = 0;
    StringUtils::TrimLeft("  a", offset, 3);
    EQ_VALUE(offset, 2, "offset");

    offset = 0;
    StringUtils::TrimLeft("  abc", offset, 5);
    EQ_VALUE(offset, 2, "offset");

    offset = 0;
    StringUtils::TrimLeft("   a", offset, 4);
    EQ_VALUE(offset, 3, "offset");

    offset = 0;
    StringUtils::TrimLeft("   abc", offset, 6);
    EQ_VALUE(offset, 3, "offset");

    offset = 0;
    StringUtils::TrimLeft(" a ", offset, 3);
    EQ_VALUE(offset, 1, "offset");

    offset = 0;
    StringUtils::TrimLeft(" abc ", offset, 5);
    EQ_VALUE(offset, 1, "offset");

    offset = 0;
    StringUtils::TrimLeft("  a ", offset, 4);
    EQ_VALUE(offset, 2, "offset");

    offset = 0;
    StringUtils::TrimLeft("  abc ", offset, 6);
    EQ_VALUE(offset, 2, "offset");

    offset = 0;
    StringUtils::TrimLeft("   a  ", offset, 6);
    EQ_VALUE(offset, 3, "offset");

    offset = 0;
    StringUtils::TrimLeft("   abc  ", offset, 8);
    EQ_VALUE(offset, 3, "offset");

    offset = 0;
    StringUtils::TrimLeft(" ", offset, 1);
    EQ_VALUE(offset, 1, "offset");

    offset = 0;
    StringUtils::TrimLeft("  ", offset, 2);
    EQ_VALUE(offset, 2, "offset");

    offset = 0;
    StringUtils::TrimLeft("   ", offset, 3);
    EQ_VALUE(offset, 3, "offset");

    offset = 1;
    StringUtils::TrimLeft(" a ", offset, 3);
    EQ_VALUE(offset, 1, "offset");

    offset = 1;
    StringUtils::TrimLeft(" abc ", offset, 5);
    EQ_VALUE(offset, 1, "offset");

    offset = 2;
    StringUtils::TrimLeft("  a ", offset, 4);
    EQ_VALUE(offset, 2, "offset");

    offset = 2;
    StringUtils::TrimLeft("  abc ", offset, 6);
    EQ_VALUE(offset, 2, "offset");

    offset = 2;
    StringUtils::TrimLeft("   a  ", offset, 6);
    EQ_VALUE(offset, 3, "offset");

    offset = 2;
    StringUtils::TrimLeft("   abc  ", offset, 8);
    EQ_VALUE(offset, 3, "offset");

    offset = 1;
    StringUtils::TrimLeft("  ", offset, 2);
    EQ_VALUE(offset, 2, "offset");

    offset = 2;
    StringUtils::TrimLeft("   ", offset, 3);
    EQ_VALUE(offset, 3, "offset");

    offset = 0;
    StringUtils::TrimLeft("  ", offset, 1);
    EQ_VALUE(offset, 1, "offset");

    offset = 1;
    StringUtils::TrimLeft("   ", offset, 2);
    EQ_VALUE(offset, 2, "offset");

    END_SUB_TEST;
}

static int TestSoftTrim() {
    SizeT offset = 0;
    SizeT length = 0;

    StringUtils::Trim("", offset, length);
    EQ_VALUE(offset, 0, "offset");
    EQ_VALUE(length, 0, "length");

    offset = 0;
    length = 1;
    StringUtils::Trim(" ", offset, length);
    EQ_VALUE(offset, 1, "offset");
    EQ_VALUE(length, 0, "length");

    offset = 0;
    length = 1;
    StringUtils::Trim("  ", offset, length);
    EQ_VALUE(offset, 1, "offset");
    EQ_VALUE(length, 0, "length");

    offset = 0;
    length = 2;
    StringUtils::Trim("  ", offset, length);
    EQ_VALUE(offset, 2, "offset");
    EQ_VALUE(length, 0, "length");

    offset = 0;
    length = 2;
    StringUtils::Trim(" a", offset, length);
    EQ_VALUE(offset, 1, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 0;
    length = 4;
    StringUtils::Trim(" abc", offset, length);
    EQ_VALUE(offset, 1, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 1;
    length = 1;
    StringUtils::Trim(" a", offset, length);
    EQ_VALUE(offset, 1, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 1;
    length = 3;
    StringUtils::Trim(" abc", offset, length);
    EQ_VALUE(offset, 1, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 2;
    length = 1;
    StringUtils::Trim("  a", offset, length);
    EQ_VALUE(offset, 2, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 2;
    length = 3;
    StringUtils::Trim("  abc", offset, length);
    EQ_VALUE(offset, 2, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 0;
    length = 3;
    StringUtils::Trim("  a", offset, length);
    EQ_VALUE(offset, 2, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 0;
    length = 5;
    StringUtils::Trim("  abc", offset, length);
    EQ_VALUE(offset, 2, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 0;
    length = 4;
    StringUtils::Trim("   a", offset, length);
    EQ_VALUE(offset, 3, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 0;
    length = 6;
    StringUtils::Trim("   abc", offset, length);
    EQ_VALUE(offset, 3, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 0;
    length = 2;
    StringUtils::Trim("a ", offset, length);
    EQ_VALUE(offset, 0, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 0;
    length = 4;
    StringUtils::Trim("abc ", offset, length);
    EQ_VALUE(offset, 0, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 0;
    length = 1;
    StringUtils::Trim("a ", offset, length);
    EQ_VALUE(offset, 0, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 0;
    length = 3;
    StringUtils::Trim("abc ", offset, length);
    EQ_VALUE(offset, 0, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 0;
    length = 1;
    StringUtils::Trim("a  ", offset, length);
    EQ_VALUE(offset, 0, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 0;
    length = 3;
    StringUtils::Trim("abc  ", offset, length);
    EQ_VALUE(offset, 0, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 0;
    length = 3;
    StringUtils::Trim("a  ", offset, length);
    EQ_VALUE(offset, 0, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 0;
    length = 5;
    StringUtils::Trim("abc  ", offset, length);
    EQ_VALUE(offset, 0, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 0;
    length = 4;
    StringUtils::Trim("a   ", offset, length);
    EQ_VALUE(offset, 0, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 0;
    length = 6;
    StringUtils::Trim("abc   ", offset, length);
    EQ_VALUE(offset, 0, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 0;
    length = 3;
    StringUtils::Trim(" a ", offset, length);
    EQ_VALUE(offset, 1, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 0;
    length = 5;
    StringUtils::Trim(" abc ", offset, length);
    EQ_VALUE(offset, 1, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 0;
    length = 5;
    StringUtils::Trim("  a  ", offset, length);
    EQ_VALUE(offset, 2, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 0;
    length = 7;
    StringUtils::Trim("  abc  ", offset, length);
    EQ_VALUE(offset, 2, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 0;
    length = 7;
    StringUtils::Trim("   a   ", offset, length);
    EQ_VALUE(offset, 3, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 0;
    length = 9;
    StringUtils::Trim("   abc   ", offset, length);
    EQ_VALUE(offset, 3, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 1;
    length = 1;
    StringUtils::Trim(" a ", offset, length);
    EQ_VALUE(offset, 1, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 1;
    length = 3;
    StringUtils::Trim(" abc ", offset, length);
    EQ_VALUE(offset, 1, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 2;
    length = 1;
    StringUtils::Trim("  a  ", offset, length);
    EQ_VALUE(offset, 2, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 2;
    length = 3;
    StringUtils::Trim("  abc  ", offset, length);
    EQ_VALUE(offset, 2, "offset");
    EQ_VALUE(length, 3, "length");

    offset = 3;
    length = 1;
    StringUtils::Trim("   a   ", offset, length);
    EQ_VALUE(offset, 3, "offset");
    EQ_VALUE(length, 1, "length");

    offset = 3;
    length = 3;
    StringUtils::Trim("   abc   ", offset, length);
    EQ_VALUE(offset, 3, "offset");
    EQ_VALUE(length, 3, "length");

    END_SUB_TEST;
}

static int TestIsEqual() {
    EQ_TRUE(StringUtils::IsEqual("", "", 0), "IsEqual");
    EQ_TRUE(StringUtils::IsEqual("a", "a", 1), "IsEqual");
    EQ_TRUE(StringUtils::IsEqual("ab", "ab", 2), "IsEqual");
    EQ_TRUE(StringUtils::IsEqual("abcdefgh", "abcdefgh", 8), "IsEqual");
    EQ_TRUE(StringUtils::IsEqual("a", "abcdefgh", 1), "IsEqual");
    EQ_TRUE(StringUtils::IsEqual("abc", "abcdefgh", 3), "IsEqual");
    EQ_TRUE(StringUtils::IsEqual("abcdefgh", "a", 1), "IsEqual");
    EQ_TRUE(StringUtils::IsEqual("abcdefgh", "abc", 3), "IsEqual");
    EQ_TRUE(StringUtils::IsEqual("0123456789123456", "0123456789123456", 16),
            "IsEqual");
    EQ_TRUE(StringUtils::IsEqual("01234567891234560123456789123456",
                                 "01234567891234560123456789123456", 32),
            "IsEqual");
    EQ_TRUE(
        StringUtils::IsEqual(
            "0123456789123456012345678912345601234567891234560123456789123456",
            "0123456789123456012345678912345601234567891234560123456789123456",
            64),
        "IsEqual");

    EQ_FALSE(StringUtils::IsEqual("a", "b", 1), "IsEqual");
    EQ_FALSE(StringUtils::IsEqual("ab", "ba", 2), "IsEqual");
    EQ_FALSE(StringUtils::IsEqual("abcdefgh", "--------", 8), "IsEqual");
    EQ_FALSE(StringUtils::IsEqual("h", "abcdefgh", 1), "IsEqual");
    EQ_FALSE(StringUtils::IsEqual("abc", "def", 3), "IsEqual");
    EQ_FALSE(StringUtils::IsEqual("abcdefgh", "b", 1), "IsEqual");
    EQ_FALSE(StringUtils::IsEqual("abcdefgh", "cde", 3), "IsEqual");

    EQ_FALSE(StringUtils::IsEqual("01234567891234568", "0123456789123456", 17),
             "IsEqual");
    EQ_FALSE(StringUtils::IsEqual("012345678912345601234567891234567",
                                  "012345678912345601234567891234568", 33),
             "IsEqual");
    EQ_FALSE(
        StringUtils::IsEqual(
            "01234567829123456012345678912345601234567891234560123456789123456",
            "00234567829123456012345678912345601234567891234560123456789123456",
            65),
        "IsEqual");

    EQ_FALSE(
        StringUtils::IsEqual(
            "01234567829123456012345678912345601234567891234560123456789123456",
            "01234567829123456012345678912345601234567891234560123456789123457",
            65),
        "IsEqual");

    EQ_FALSE(
        StringUtils::IsEqual(
            "01234567829123456012345678912345601234567891234560123456789123456",
            "01234567829123456012345678912345601234567891235560123456789123456",
            65),
        "IsEqual");

    EQ_FALSE(
        StringUtils::IsEqual(
            "01234567829123456012345678912345601234567891234560123456789123456",
            "01234567829123456012335678912345601234567891234560123456789123456",
            65),
        "IsEqual");

    EQ_FALSE(
        StringUtils::IsEqual(
            "a2345678912345678912345678912345612340678912345678912345678912345w",
            "a2345678912345678912345678912345612345678912345678912345678912345w",
            65),
        "IsEqual");

    END_SUB_TEST;
}

static int RunStringUtilsTests() {
    STARTING_TEST("StringUtils.hpp");

    START_TEST("Hash Test", TestHash);
    START_TEST("Count Test", TestCount);
    START_TEST("TrimLeft Test", TestLeftTrim);
    START_TEST("Trim Test", TestSoftTrim);
    START_TEST("IsEqual Test", TestIsEqual);

    END_TEST("StringUtils.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
