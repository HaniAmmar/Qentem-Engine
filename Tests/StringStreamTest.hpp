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

#ifndef QENTEM_STRINGSTREAMTEST_H_
#define QENTEM_STRINGSTREAMTEST_H_

namespace Qentem {
namespace Test {

static int TestStringStream() {
    StringStream ss1;
    String       str;

    SHOULD_EQUAL_VALUE(ss1.Length(), 0, "Length");
    SHOULD_EQUAL_VALUE(ss1.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(ss1.Storage(), nullptr, "Storage()", "null");

    StringStream ss2(10); // Preset size
    SHOULD_EQUAL_VALUE(ss2.Capacity(), 10, "Capacity");
    SHOULD_EQUAL_VALUE(ss2.Length(), 0, "Length");
    SHOULD_EQUAL(ss2.Storage(), nullptr, "Storage()", "null");

    ss2.Reset();
    SHOULD_EQUAL_VALUE(ss2.Length(), 0, "Length");
    SHOULD_EQUAL_VALUE(ss2.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(ss2.Storage(), nullptr, "Storage()", "null");

    ss1 += 'a';
    SHOULD_EQUAL_VALUE(ss1.Length(), 1, "Length");
    SHOULD_EQUAL_TRUE((ss1.Capacity() >= 1), "(ss1.Capacity() >= 1)");
    SHOULD_NOT_EQUAL(ss1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(ss1.Storage()[0], 'a', "Storage()[0]");
    SHOULD_EQUAL_VALUE(ss1.Storage()[ss1.Length()], '\0', "Storage()[Length]");
    SHOULD_EQUAL_VALUE(ss1.GetString(), "a", "GetString()");
    SHOULD_EQUAL_VALUE(ss1.Length(), 0, "Length");
    SHOULD_EQUAL_VALUE(ss1.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(ss1.Storage(), nullptr, "Storage()", "null");

    ss2 += String("a");
    SHOULD_EQUAL_VALUE(ss2.Length(), 1, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)");
    SHOULD_EQUAL_VALUE(ss2.GetString(), "a", "GetString()");

    str = String("a");
    ss2 += str;
    SHOULD_EQUAL_VALUE(ss2.Length(), 1, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)");
    SHOULD_EQUAL_VALUE(ss2.GetString(), "a", "GetString()");

    ss2 += "abc";
    SHOULD_EQUAL_VALUE(ss2.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss2.GetString(), "abc", "GetString()");

    ss2 += String("abc");
    SHOULD_EQUAL_VALUE(ss2.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss2.GetString(), "abc", "GetString()");

    str = String("abc");
    ss2 += str;
    SHOULD_EQUAL_VALUE(ss2.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss2.GetString(), "abc", "GetString()");

    ss2 += String("abc");
    ss1 = ss2; // Copy
    SHOULD_EQUAL_VALUE(ss1.Length(), ss2.Length(), "Length");
    SHOULD_EQUAL_TRUE((ss1.Capacity() >= ss2.Length()),
                      "(ss1.Capacity() >= ss2.Length())");
    SHOULD_EQUAL_VALUE(ss1.GetString(), "abc", "GetString()");

    ULong lss = ss1.Capacity();

    ss1.SoftReset();
    SHOULD_EQUAL_VALUE(ss1.Length(), 0, "Length");
    SHOULD_EQUAL_VALUE(ss1.Capacity(), lss, "Capacity");

    ss1.Reset();
    ss1 = static_cast<StringStream &&>(ss2); // Move
    SHOULD_EQUAL_VALUE(ss1.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss1.Capacity() >= 3), "(ss1.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss1.GetString(), "abc", "GetString()");
    SHOULD_EQUAL_VALUE(ss2.Length(), 0, "Length");
    SHOULD_EQUAL_VALUE(ss2.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(ss2.Storage(), nullptr, "Storage()", "null");

    ss2 += 'a';
    ss1.Reset();
    ss1 = static_cast<StringStream &&>(ss2); // Move
    SHOULD_EQUAL_VALUE(ss1.Length(), 1, "Length");
    SHOULD_EQUAL_TRUE((ss1.Capacity() >= 1), "(ss1.Capacity() >= 1)");
    SHOULD_EQUAL_VALUE(ss1.GetString(), "a", "GetString()");

    ss1 += "abc";
    ss2 = StringStream(ss1); // Copy
    SHOULD_EQUAL_VALUE(ss1.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss1.Capacity() >= 3), "(ss1.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss1.GetString(), "abc", "GetString()");
    SHOULD_EQUAL_VALUE(ss2.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss2.GetString(), "abc", "GetString()");

    ss1 += "efg";
    ss2 = StringStream(static_cast<StringStream &&>(ss2)); // Move
    SHOULD_EQUAL_VALUE(ss1.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss1.Capacity() >= 3), "(ss1.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss1.GetString(), "efg", "GetString()");
    SHOULD_EQUAL_VALUE(ss2.Length(), 0, "Length");
    SHOULD_EQUAL_VALUE(ss2.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(ss2.Storage(), nullptr, "Storage()", "null");

    ss2.Insert("a", 1);
    SHOULD_EQUAL_VALUE(ss2.Length(), 1, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)");
    SHOULD_NOT_EQUAL(ss2.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(ss2.Storage()[0], 'a', "Storage()[0]");

    ss2.Insert("bc", 2);
    SHOULD_EQUAL_VALUE(ss2.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    SHOULD_EQUAL_VALUE(ss2.GetString(), "abc", "GetString()");

    ss2.Insert("abcdef", 1);
    ss2.Insert("bcdef", 1);
    ss2.Insert("cdef", 1);

    char *e_str    = ss2.Eject();
    ULong length   = String::Count(e_str);
    bool  is_equal = Memory::Compare(e_str, "abc", 3);
    HAllocator::Deallocate(e_str);

    SHOULD_EQUAL_VALUE(length, 3, "length");
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

    SHOULD_EQUAL_VALUE(ss2.GetString().Storage()[0], '\0', "GetString()[0]");
    ss2.Reset();
    SHOULD_EQUAL_VALUE(ss2.GetString().Storage()[0], '\0', "GetString()[0]");

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
        ss2.GetString(),
        "123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789",
        "GetString()", "123456789*13");

    ss1.Reset();

    char *buffer = ss1.Buffer(2);

    SHOULD_EQUAL_VALUE(ss1.Length(), 2, "Length");

    buffer[0] = 'a';
    buffer[1] = 'b';

    const char *ab = "ab";

    SHOULD_EQUAL_VALUE(ss1.Length(), 2, "Length");
    SHOULD_EQUAL_TRUE(Memory::Compare(ss1.Storage(), ab, 2), "Compare()");

    buffer = ss1.Buffer(1);

    SHOULD_EQUAL_VALUE(ss1.Length(), 3, "Length");

    buffer[0] = 'c';

    const char *ab2 = "abc";
    SHOULD_EQUAL_VALUE(ss1.Length(), 3, "Length");
    SHOULD_EQUAL_TRUE(Memory::Compare(ss1.Storage(), ab2, 3), "Compare()");

    ss1 += "de";

    SHOULD_EQUAL_VALUE(ss1.Length(), 5, "Length");
    SHOULD_EQUAL_VALUE(ss1.GetString(), "abcde", "GetString()");

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
