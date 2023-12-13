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
#include "StringStream.hpp"

#ifndef QENTEM_STRINGSTREAM_TESTS_H_
#define QENTEM_STRINGSTREAM_TESTS_H_

namespace Qentem {
namespace Test {

static void TestStringStream(QTest &helper) {
    StringStream<char> ss1;
    StringStream<char> ss2{10}; // Preset size
    String<char>       str;

    helper.Equal(ss1.Length(), 0U, "Length", __LINE__);
    helper.Equal(ss1.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(ss1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(ss1.Last(), nullptr, "Last()", "null", __LINE__);

    helper.EqualsTrue((ss2.Capacity() >= 10), "Capacity() >= 10", __LINE__);
    helper.Equal(ss2.Length(), 0U, "Length", __LINE__);
    helper.NotEqual(ss2.First(), nullptr, "First()", "null", __LINE__);

    ss2.Reset();
    helper.Equal(ss2.Length(), 0U, "Length", __LINE__);
    helper.Equal(ss2.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(ss2.First(), nullptr, "First()", "null", __LINE__);

    ss1 = StringStream<char>(1);
    ss1 += 'a';
    helper.Equal(ss1.Length(), 1U, "Length", __LINE__);
    helper.EqualsTrue((ss1.Capacity() >= 1), "(ss1.Capacity() >= 1)", __LINE__);
    helper.NotEqual(ss1.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(ss1.Last(), nullptr, "Last()", "null", __LINE__);
    helper.Equal(ss1.First()[0], 'a', "First()[0]", __LINE__);
    helper.Equal(ss1.GetString(), "a", "GetString()", __LINE__);
    helper.Equal(ss1.Length(), 0U, "Length", __LINE__);
    helper.Equal(ss1.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(ss1.First(), nullptr, "First()", "null", __LINE__);

    ss2 += String<char>("a");
    helper.Equal(ss2.Length(), 1U, "Length", __LINE__);
    helper.EqualsTrue((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)", __LINE__);
    helper.Equal(ss2.GetString(), "a", "GetString()==a", __LINE__);
    helper.NotEqual(ss2.GetString(), "aa", "GetString()!=aa", __LINE__);

    str = String<char>("a");
    ss2 += str;
    helper.Equal(ss2.Length(), 1U, "Length", __LINE__);
    helper.EqualsTrue((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)", __LINE__);
    helper.Equal(ss2, "a", "StringStream==a", __LINE__);
    helper.NotEqual(ss2, "aa", "StringStream!=aa", __LINE__);

    ss2.Reset();

    ss2 += "abc";
    helper.Equal(ss2.Length(), 3U, "Length", __LINE__);
    helper.EqualsTrue((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)", __LINE__);
    helper.Equal(ss2.GetString(), "abc", "GetString()", __LINE__);
    const String<char> sss;

    ss2 += String<char>("abc");
    helper.Equal(ss2.Length(), 3U, "Length", __LINE__);
    helper.EqualsTrue((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)", __LINE__);
    helper.Equal(ss2, "abc", "StringStream", __LINE__);
    ss2.Reset();

    str = String<char>("abc");
    ss2 += str;
    helper.Equal(ss2.Length(), 3U, "Length", __LINE__);
    helper.EqualsTrue((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)", __LINE__);
    helper.Equal(ss2, "abc", "StringStream", __LINE__);
    ss2.Reset();

    ss2 += String<char>("abc");
    ss1 = ss2; // Copy
    helper.Equal(ss1.Length(), ss2.Length(), "Length", __LINE__);
    helper.EqualsTrue((ss1.Capacity() >= ss2.Length()), "(ss1.Capacity() >= ss2.Length())", __LINE__);
    helper.Equal(ss1, "abc", "StringStream", __LINE__);
    ss1.Reset();

    const SizeT lss = ss1.Capacity();

    ss1.Clear();
    helper.Equal(ss1.Length(), 0U, "Length", __LINE__);
    helper.Equal(ss1.Capacity(), lss, "Capacity", __LINE__);

    ss1.Reset();
    ss1 = Memory::Move(ss2); // Move
    helper.Equal(ss1.Length(), 3U, "Length", __LINE__);
    helper.EqualsTrue((ss1.Capacity() >= 3), "(ss1.Capacity() >= 3)", __LINE__);
    helper.Equal(ss1, "abc", "StringStream", __LINE__);
    helper.Equal(ss2.Storage(), nullptr, "Storage()", "null", __LINE__);
    helper.Equal(ss2.Length(), 0U, "Length", __LINE__);
    helper.Equal(ss2.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(ss2.First(), nullptr, "First()", "null", __LINE__);

    ss2 += 'a';
    ss1.Reset();
    ss1 = Memory::Move(ss2); // Move
    helper.Equal(ss1.Length(), 1U, "Length", __LINE__);
    helper.EqualsTrue((ss1.Capacity() >= 8), "(ss1.Capacity() >= 1)", __LINE__);
    helper.Equal(ss1, "a", "StringStream", __LINE__);
    ss1.Reset();

    ss1 += "abc";
    ss2 = StringStream<char>(ss1); // Copy
    helper.Equal(ss1.Length(), 3U, "Length", __LINE__);
    helper.EqualsTrue((ss1.Capacity() >= 3), "(ss1.Capacity() >= 3)", __LINE__);
    helper.Equal(ss1, "abc", "StringStream", __LINE__);
    helper.Equal(ss2.Length(), 3U, "Length", __LINE__);
    helper.EqualsTrue((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)", __LINE__);
    helper.Equal(ss2, "abc", "StringStream", __LINE__);
    ss1.Reset();
    ss2.Reset();

    ss1 += "efg";
    ss2 = StringStream<char>(Memory::Move(ss1)); // Move
    helper.Equal(ss1.Storage(), nullptr, "Storage()", "null", __LINE__);
    helper.Equal(ss1.Length(), 0U, "Length", __LINE__);
    helper.EqualsTrue((ss1.Capacity() == 0), "(ss1.Capacity() = 0)", __LINE__);
    helper.Equal(ss1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(ss2, "efg", "StringStream", __LINE__);
    helper.Equal(ss2.Length(), 3U, "Length", __LINE__);
    helper.EqualsTrue((ss2.Capacity() >= 3), "(ss1.Capacity() >= 3)", __LINE__);
    helper.Equal(ss2.First()[2], 'g', "First()[0]", __LINE__);

    ss2.Reset();
    ss2.Write("a", 1);
    helper.Equal(ss2.Length(), 1U, "Length", __LINE__);
    helper.EqualsTrue((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)", __LINE__);
    helper.EqualsTrue((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)", __LINE__);
    helper.NotEqual(ss2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(ss2.First()[0], 'a', "First()[0]", __LINE__);

    ss2.Write("bc", 2);
    helper.Equal(ss2.Length(), 3U, "Length", __LINE__);
    helper.EqualsTrue((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)", __LINE__);
    helper.Equal(ss2, "abc", "StringStream", __LINE__);
    ss2.Reset();

    ss2.Write("abcdef", 1);
    ss2.Write("bcdef", 1);
    ss2.Write("cdef", 1);

    char      *e_str    = ss2.Detach();
    const bool is_equal = StringUtils::IsEqual(e_str, "abc", 3);
    Memory::Deallocate(e_str);

    helper.EqualsTrue(is_equal, "is_equal", __LINE__);

    ss2 += "abcdef";
    ss2.StepBack(10);
    helper.Equal(ss2.Length(), 6U, "Length", __LINE__);

    ss2.StepBack(1);
    helper.Equal(ss2.Length(), 5U, "Length", __LINE__);

    ss2.StepBack(2);
    helper.Equal(ss2.Length(), 3U, "Length", __LINE__);

    ss2.StepBack(3);
    helper.Equal(ss2.Length(), 0U, "Length", __LINE__);

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

    helper.Equal(ss2.Length(), 117U, "Length", __LINE__);
    helper.EqualsTrue((ss2.Capacity() >= 117), "(ss2.Capacity() >= 117)", __LINE__);
    helper.Equal(
        ss2,
        "123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789",
        "StringStream", "123456789*13", __LINE__);

    ss1.Reset();
    ss2.Reset();

    char *buffer = ss1.Buffer(2);

    helper.Equal(ss1.Length(), 2U, "Length", __LINE__);

    buffer[0] = 'a';
    buffer[1] = 'b';

    const char *ab = "ab";

    helper.Equal(ss1.Length(), 2U, "Length", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(ss1.First(), ab, 2), "IsEqual()", __LINE__);

    buffer = ss1.Buffer(1);
    helper.Equal(ss1.Length(), 3U, "Length", __LINE__);

    buffer[0]       = 'c';
    const char *ab2 = "abc";
    helper.Equal(ss1.Length(), 3U, "Length", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(ss1.First(), ab2, 3), "IsEqual()", __LINE__);

    ss2.Clear();
    ss2 += "ab";
    helper.Equal((ss1 == ss2), false, "IsEqual()", __LINE__);

    ss1 += "de";
    helper.Equal(ss1.Length(), 5U, "Length", __LINE__);
    helper.Equal(ss1, "abcde", "StringStream", __LINE__);

    ss2 = ss1;
    helper.Equal(ss2.Length(), 5U, "Length", __LINE__);
    helper.Equal(ss2, "abcde", "StringStream", __LINE__);

    ss2.Reset();

    /////////
    ss2 += "123456789123456789123456789123456789";
    const char *ss2_p = ss2.First();
    str               = ss2.GetString();
    helper.Equal(ss2_p, str.First(), "StringStream", __LINE__);
    helper.Equal(str, "123456789123456789123456789123456789", "StringStream", __LINE__);

    /////////

    ss2.Reset();

    ss1 = "abcdefgh";
    helper.Equal(ss1, "abcdefgh", "StringStream", __LINE__);

    ss2 << ss1;
    helper.Equal(ss1, "abcdefgh", "StringStream", __LINE__);
    helper.Equal(ss2, "abcdefgh", "StringStream", __LINE__);

    const String<char> n_str = String<char>("12345678");
    ss1                      = n_str;
    helper.Equal(ss1, "12345678", "StringStream", __LINE__);

    ss2.Reset();

    ss2 << n_str;
    helper.Equal(ss2, "12345678", "StringStream", __LINE__);

    struct SimpleStream {
        char          str[16]{0};
        const SizeT32 max = 16;
        SizeT32       index{0};

        void operator<<(const char *string) noexcept {
            for (SizeT i = 0; i < max; i++) {
                const char c = string[index];

                if (c == 0) {
                    break;
                }

                str[index] = c;
                ++index;
            }
        }
    };

    SimpleStream sis;
    sis << ss2;
    helper.EqualsTrue(StringUtils::IsEqual(&(sis.str[0]), "12345678", 8), "SimpleStream", __LINE__);

    ss2 << n_str;
    helper.Equal(ss2, "1234567812345678", "StringStream", __LINE__);
    helper.Equal(ss2.Capacity(), 16U, "Capacity", __LINE__);

    sis.index = 0;
    sis << ss2;

    helper.EqualsTrue(StringUtils::IsEqual(&(sis.str[0]), "1234567812345678", 16), "SimpleStream", __LINE__);

    ss2.StepBack(1);
    sis.index = 0;
    sis << ss2;
    helper.EqualsTrue(StringUtils::IsEqual(&(sis.str[0]), "1234567", 7), "SimpleStream", __LINE__);
}

static int RunStringStreamTests() {
    QTest helper{"StringStream.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("StringStream Test", TestStringStream);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
