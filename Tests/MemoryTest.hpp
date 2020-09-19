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

#include "Memory.hpp"
#include "TestHelper.hpp"

#ifndef QENTEM_MEMORY_TESTS_H_
#define QENTEM_MEMORY_TESTS_H_

namespace Qentem {
namespace Test {

static int TestIsEqual() {
    SHOULD_EQUAL_TRUE(Memory::IsEqual("", "", 0), "IsEqual");
    SHOULD_EQUAL_TRUE(Memory::IsEqual("a", "a", 1), "IsEqual");
    SHOULD_EQUAL_TRUE(Memory::IsEqual("ab", "ab", 2), "IsEqual");
    SHOULD_EQUAL_TRUE(Memory::IsEqual("abcdefgh", "abcdefgh", 8), "IsEqual");
    SHOULD_EQUAL_TRUE(Memory::IsEqual("a", "abcdefgh", 1), "IsEqual");
    SHOULD_EQUAL_TRUE(Memory::IsEqual("abc", "abcdefgh", 3), "IsEqual");
    SHOULD_EQUAL_TRUE(Memory::IsEqual("abcdefgh", "a", 1), "IsEqual");
    SHOULD_EQUAL_TRUE(Memory::IsEqual("abcdefgh", "abc", 3), "IsEqual");
    SHOULD_EQUAL_TRUE(
        Memory::IsEqual("0123456789123456", "0123456789123456", 16), "IsEqual");
    SHOULD_EQUAL_TRUE(Memory::IsEqual("01234567891234560123456789123456",
                                      "01234567891234560123456789123456", 32),
                      "IsEqual");
    SHOULD_EQUAL_TRUE(
        Memory::IsEqual(
            "0123456789123456012345678912345601234567891234560123456789123456",
            "0123456789123456012345678912345601234567891234560123456789123456",
            64),
        "IsEqual");

    SHOULD_NOT_EQUAL_TRUE(Memory::IsEqual("a", "b", 1), "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(Memory::IsEqual("ab", "ba", 2), "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(Memory::IsEqual("abcdefgh", "--------", 8),
                          "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(Memory::IsEqual("h", "abcdefgh", 1), "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(Memory::IsEqual("abc", "def", 3), "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(Memory::IsEqual("abcdefgh", "b", 1), "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(Memory::IsEqual("abcdefgh", "cde", 3), "IsEqual");

    SHOULD_NOT_EQUAL_TRUE(
        Memory::IsEqual("01234567891234568", "0123456789123456", 17),
        "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(Memory::IsEqual("012345678912345601234567891234567",
                                          "012345678912345601234567891234568",
                                          33),
                          "IsEqual");
    SHOULD_NOT_EQUAL_TRUE(
        Memory::IsEqual(
            "01234567829123456012345678912345601234567891234560123456789123456",
            "00234567829123456012345678912345601234567891234560123456789123456",
            65),
        "IsEqual");

    SHOULD_NOT_EQUAL_TRUE(
        Memory::IsEqual(
            "01234567829123456012345678912345601234567891234560123456789123456",
            "01234567829123456012345678912345601234567891234560123456789123457",
            65),
        "IsEqual");

    SHOULD_NOT_EQUAL_TRUE(
        Memory::IsEqual(
            "01234567829123456012345678912345601234567891234560123456789123456",
            "01234567829123456012345678912345601234567891235560123456789123456",
            65),
        "IsEqual");

    SHOULD_NOT_EQUAL_TRUE(
        Memory::IsEqual(
            "01234567829123456012345678912345601234567891234560123456789123456",
            "01234567829123456012335678912345601234567891234560123456789123456",
            65),
        "IsEqual");

    SHOULD_NOT_EQUAL_TRUE(
        Memory::IsEqual(
            "a2345678912345678912345678912345612340678912345678912345678912345w",
            "a2345678912345678912345678912345612345678912345678912345678912345w",
            65),
        "IsEqual");

    END_SUB_TEST;
}

static int RunMemoryTests() {
    STARTING_TEST("Memory.hpp");

    START_TEST("IsEqual Test", TestIsEqual);

    END_TEST("Memory.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
