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

#include "StringStream.hpp"
#include "TestHelper.hpp"

#ifndef QENTEM_STRINGSTREAM_TESTS_H_
#define QENTEM_STRINGSTREAM_TESTS_H_

namespace Qentem {
namespace Test {

static int TestStringStream() {
    StringStream<char> ss1;
    StringStream<char> ss2(10); // Preset size
    String<char>       str;

    SHOULD_EQUAL_VALUE(ss1.Length(), 0, "Length");
    SHOULD_EQUAL_VALUE(ss1.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(ss1.First(), nullptr, "First()", "null");

    SHOULD_EQUAL_VALUE((ss2.Capacity() >= 10), true, "Capacity() >= 10");
    SHOULD_EQUAL_VALUE(ss2.Length(), 0, "Length");
    SHOULD_NOT_EQUAL(ss2.First(), nullptr, "First()", "null");

    ss2.Reset();
    SHOULD_EQUAL_VALUE(ss2.Length(), 0, "Length");
    SHOULD_EQUAL_VALUE(ss2.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(ss2.First(), nullptr, "First()", "null");

    ss1 = StringStream<char>(1);
    ss1 += 'a';
    SHOULD_EQUAL_VALUE(ss1.Length(), 1, "Length");
    SHOULD_EQUAL_TRUE((ss1.Capacity() >= 1), "(ss1.Capacity() >= 1)");
    SHOULD_NOT_EQUAL(ss1.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(ss1.First()[0], 'a', "First()[0]");
    SHOULD_EQUAL_VALUE(ss1.GetString(), "a", "GetString()");
    SHOULD_EQUAL_VALUE(ss1.Length(), 0, "Length");
    SHOULD_EQUAL_VALUE(ss1.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(ss1.First(), nullptr, "First()", "null");

    ss2 += String<char>("a");
    SHOULD_EQUAL_VALUE(ss2.Length(), 1, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)");
    SHOULD_EQUAL_VALUE(ss2.GetString(), "a", "GetString()==a");
    SHOULD_NOT_EQUAL_VALUE(ss2.GetString(), "aa", "GetString()!=aa");

    str = String<char>("a");
    ss2 += str;
    SHOULD_EQUAL_VALUE(ss2.Length(), 1, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)");
    SHOULD_EQUAL_VALUE(ss2, "a", "StringStream==a");
    SHOULD_NOT_EQUAL_VALUE(ss2, "aa", "StringStream!=aa");

    ss2.Reset();

    ss2 += "abc";
    SHOULD_EQUAL_VALUE(ss2.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss2.GetString(), "abc", "GetString()");

    ss2 += String<char>("abc");
    SHOULD_EQUAL_VALUE(ss2.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss2, "abc", "StringStream");
    ss2.Reset();

    str = String<char>("abc");
    ss2 += str;
    SHOULD_EQUAL_VALUE(ss2.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss2, "abc", "StringStream");
    ss2.Reset();

    ss2 += String<char>("abc");
    ss1 = ss2; // Copy
    SHOULD_EQUAL_VALUE(ss1.Length(), ss2.Length(), "Length");
    SHOULD_EQUAL_TRUE((ss1.Capacity() >= ss2.Length()),
                      "(ss1.Capacity() >= ss2.Length())");
    SHOULD_EQUAL_VALUE(ss1, "abc", "StringStream");
    ss1.Reset();

    SizeT lss = ss1.Capacity();

    ss1.Clear();
    SHOULD_EQUAL_VALUE(ss1.Length(), 0, "Length");
    SHOULD_EQUAL_VALUE(ss1.Capacity(), lss, "Capacity");

    ss1.Reset();
    ss1 = static_cast<StringStream<char> &&>(ss2); // Move
    SHOULD_EQUAL_VALUE(ss1.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss1.Capacity() >= 3), "(ss1.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss1, "abc", "StringStream");
    SHOULD_EQUAL_VALUE(ss2.Length(), 0, "Length");
    SHOULD_EQUAL_VALUE(ss2.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(ss2.First(), nullptr, "First()", "null");

    ss2 += 'a';
    ss1.Reset();
    ss1 = static_cast<StringStream<char> &&>(ss2); // Move
    SHOULD_EQUAL_VALUE(ss1.Length(), 1, "Length");
    SHOULD_EQUAL_TRUE((ss1.Capacity() >= 1), "(ss1.Capacity() >= 1)");
    SHOULD_EQUAL_VALUE(ss1, "a", "StringStream");
    ss1.Reset();

    ss1 += "abc";
    ss2 = StringStream<char>(ss1); // Copy
    SHOULD_EQUAL_VALUE(ss1.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss1.Capacity() >= 3), "(ss1.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss1, "abc", "StringStream");
    SHOULD_EQUAL_VALUE(ss2.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss2, "abc", "StringStream");
    ss1.Reset();
    ss2.Reset();

    ss1 += "efg";
    ss2 = StringStream<char>(static_cast<StringStream<char> &&>(ss2)); // Move
    SHOULD_EQUAL_VALUE(ss1.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss1.Capacity() >= 3), "(ss1.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss1, "efg", "StringStream");
    SHOULD_EQUAL_VALUE(ss2.Length(), 0, "Length");
    SHOULD_EQUAL_VALUE(ss2.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(ss2.First(), nullptr, "First()", "null");

    ss2.Insert("a", 1);
    SHOULD_EQUAL_VALUE(ss2.Length(), 1, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)");
    SHOULD_NOT_EQUAL(ss2.First(), nullptr, "First()", "null");
    SHOULD_EQUAL_VALUE(ss2.First()[0], 'a', "First()[0]");

    ss2.Insert("bc", 2);
    SHOULD_EQUAL_VALUE(ss2.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss2, "abc", "StringStream");
    ss2.Reset();

    ss2.Insert("abcdef", 1);
    ss2.Insert("bcdef", 1);
    ss2.Insert("cdef", 1);

    char *e_str    = ss2.Eject();
    bool  is_equal = StringUtils::IsEqual(e_str, "abc", 3);
    HAllocator::Deallocate(e_str);

    SHOULD_EQUAL_TRUE(is_equal, "is_equal");

    ss2 += "abcdef";
    ss2.StepBack(10);
    SHOULD_EQUAL_VALUE(ss2.Length(), 6, "Length");

    ss2.StepBack(1);
    SHOULD_EQUAL_VALUE(ss2.Length(), 5, "Length");

    ss2.StepBack(2);
    SHOULD_EQUAL_VALUE(ss2.Length(), 3, "Length");

    ss2.StepBack(3);
    SHOULD_EQUAL_VALUE(ss2.Length(), 0, "Length");

    ss2 += "123456789";
    ss2 += "123456789";
    ss2 += "123456789";
    ss2 += "123456789";
    ss2 += "123456789";
    ss2 += "123456789";
    ss2 += "123456789";
    ss2 += "123456789";
    ss2 += "123456789";
    ss2 += "123456789";
    ss2 += "123456789";
    ss2 += "123456789";
    ss2 += "123456789";

    SHOULD_EQUAL_VALUE(ss2.Length(), 117, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 117), "(ss2.Capacity() >= 117)");
    SHOULD_EQUAL(
        ss2,
        "123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789",
        "StringStream", "123456789*13");

    ss1.Reset();
    ss2.Reset();

    char *buffer = ss1.Buffer(2);

    SHOULD_EQUAL_VALUE(ss1.Length(), 2, "Length");

    buffer[0] = 'a';
    buffer[1] = 'b';

    const char *ab = "ab";

    SHOULD_EQUAL_VALUE(ss1.Length(), 2, "Length");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual(ss1.First(), ab, 2), "IsEqual()");

    buffer = ss1.Buffer(1);
    SHOULD_EQUAL_VALUE(ss1.Length(), 3, "Length");

    buffer[0]       = 'c';
    const char *ab2 = "abc";
    SHOULD_EQUAL_VALUE(ss1.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual(ss1.First(), ab2, 3), "IsEqual()");

    ss2.Clear();
    ss2 += "ab";
    SHOULD_EQUAL_VALUE((ss1 == ss2), false, "IsEqual()");

    ss1 += "de";
    SHOULD_EQUAL_VALUE(ss1.Length(), 5, "Length");
    SHOULD_EQUAL_VALUE(ss1, "abcde", "StringStream");

    END_SUB_TEST;
}

static int RunStringStreamTests() {
    STARTING_TEST("StringStream.hpp");

    START_TEST("StringStream Test", TestStringStream);

    END_TEST("StringStream.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
