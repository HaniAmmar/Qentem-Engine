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
    StringStream<char> ss2{10}; // Preset size
    String<char>       str;

    EQ_VALUE(ss1.Length(), 0, "Length");
    EQ_VALUE(ss1.Capacity(), 0, "Capacity");
    EQ_TO(ss1.First(), nullptr, "First()", "null");
    EQ_TO(ss1.Last(), nullptr, "Last()", "null");

    EQ_TRUE((ss2.Capacity() >= 10), "Capacity() >= 10");
    EQ_VALUE(ss2.Length(), 0, "Length");
    NOT_EQ_TO(ss2.First(), nullptr, "First()", "null");

    ss2.Reset();
    EQ_VALUE(ss2.Length(), 0, "Length");
    EQ_VALUE(ss2.Capacity(), 0, "Capacity");
    EQ_TO(ss2.First(), nullptr, "First()", "null");

    ss1 = StringStream<char>(1);
    ss1 += 'a';
    EQ_VALUE(ss1.Length(), 1, "Length");
    EQ_TRUE((ss1.Capacity() >= 1), "(ss1.Capacity() >= 1)");
    NOT_EQ_TO(ss1.First(), nullptr, "First()", "null");
    NOT_EQ_TO(ss1.Last(), nullptr, "Last()", "null");
    EQ_VALUE(ss1.First()[0], 'a', "First()[0]");
    EQ_VALUE(ss1.GetString(), "a", "GetString()");
    EQ_VALUE(ss1.Length(), 0, "Length");
    EQ_VALUE(ss1.Capacity(), 0, "Capacity");
    EQ_TO(ss1.First(), nullptr, "First()", "null");

    ss2 += String<char>("a");
    EQ_VALUE(ss2.Length(), 1, "Length");
    EQ_TRUE((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)");
    EQ_VALUE(ss2.GetString(), "a", "GetString()==a");
    NOT_EQ_VALUE(ss2.GetString(), "aa", "GetString()!=aa");

    str = String<char>("a");
    ss2 += str;
    EQ_VALUE(ss2.Length(), 1, "Length");
    EQ_TRUE((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)");
    EQ_VALUE(ss2, "a", "StringStream==a");
    NOT_EQ_VALUE(ss2, "aa", "StringStream!=aa");

    ss2.Reset();

    ss2 += "abc";
    EQ_VALUE(ss2.Length(), 3, "Length");
    EQ_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    EQ_VALUE(ss2.GetString(), "abc", "GetString()");
    String<char> sss;

    ss2 += String<char>("abc");
    EQ_VALUE(ss2.Length(), 3, "Length");
    EQ_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    EQ_VALUE(ss2, "abc", "StringStream");
    ss2.Reset();

    str = String<char>("abc");
    ss2 += str;
    EQ_VALUE(ss2.Length(), 3, "Length");
    EQ_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    EQ_VALUE(ss2, "abc", "StringStream");
    ss2.Reset();

    ss2 += String<char>("abc");
    ss1 = ss2; // Copy
    EQ_VALUE(ss1.Length(), ss2.Length(), "Length");
    EQ_TRUE((ss1.Capacity() >= ss2.Length()), "(ss1.Capacity() >= ss2.Length())");
    EQ_VALUE(ss1, "abc", "StringStream");
    ss1.Reset();

    const SizeT lss = ss1.Capacity();

    ss1.Clear();
    EQ_VALUE(ss1.Length(), 0, "Length");
    EQ_VALUE(ss1.Capacity(), lss, "Capacity");

    ss1.Reset();
    ss1 = static_cast<StringStream<char> &&>(ss2); // Move
    EQ_VALUE(ss1.Length(), 3, "Length");
    EQ_TRUE((ss1.Capacity() >= 3), "(ss1.Capacity() >= 3)");
    EQ_VALUE(ss1, "abc", "StringStream");
    EQ_VALUE(ss2.Length(), 0, "Length");
    EQ_VALUE(ss2.Capacity(), 0, "Capacity");
    EQ_TO(ss2.First(), nullptr, "First()", "null");

    ss2 += 'a';
    ss1.Reset();
    ss1 = static_cast<StringStream<char> &&>(ss2); // Move
    EQ_VALUE(ss1.Length(), 1, "Length");
    EQ_TRUE((ss1.Capacity() >= 1), "(ss1.Capacity() >= 1)");
    EQ_VALUE(ss1, "a", "StringStream");
    ss1.Reset();

    ss1 += "abc";
    ss2 = StringStream<char>(ss1); // Copy
    EQ_VALUE(ss1.Length(), 3, "Length");
    EQ_TRUE((ss1.Capacity() >= 3), "(ss1.Capacity() >= 3)");
    EQ_VALUE(ss1, "abc", "StringStream");
    EQ_VALUE(ss2.Length(), 3, "Length");
    EQ_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    EQ_VALUE(ss2, "abc", "StringStream");
    ss1.Reset();
    ss2.Reset();

    ss1 += "efg";
    ss2 = StringStream<char>(static_cast<StringStream<char> &&>(ss1)); // Move
    EQ_VALUE(ss1.Length(), 0, "Length");
    EQ_TRUE((ss1.Capacity() == 0), "(ss1.Capacity() = 0)");
    EQ_TO(ss1.First(), nullptr, "First()", "null");
    EQ_VALUE(ss2, "efg", "StringStream");
    EQ_VALUE(ss2.Length(), 3, "Length");
    EQ_TRUE((ss2.Capacity() >= 3), "(ss1.Capacity() >= 3)");
    EQ_VALUE(ss2.First()[2], 'g', "First()[0]");

    ss2.Reset();
    ss2.Insert("a", 1);
    EQ_VALUE(ss2.Length(), 1, "Length");
    EQ_TRUE((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)");
    EQ_TRUE((ss2.Capacity() >= 1), "(ss2.Capacity() >= 1)");
    NOT_EQ_TO(ss2.First(), nullptr, "First()", "null");
    EQ_VALUE(ss2.First()[0], 'a', "First()[0]");

    ss2.Insert("bc", 2);
    EQ_VALUE(ss2.Length(), 3, "Length");
    EQ_TRUE((ss2.Capacity() >= 3), "(ss2.Capacity() >= 3)");
    EQ_VALUE(ss2, "abc", "StringStream");
    ss2.Reset();

    ss2.Insert("abcdef", 1);
    ss2.Insert("bcdef", 1);
    ss2.Insert("cdef", 1);

    char      *e_str    = ss2.Eject();
    const bool is_equal = StringUtils::IsEqual(e_str, "abc", 3);
    Memory::Deallocate(e_str);

    EQ_TRUE(is_equal, "is_equal");

    ss2 += "abcdef";
    ss2.StepBack(10);
    EQ_VALUE(ss2.Length(), 6, "Length");

    ss2.StepBack(1);
    EQ_VALUE(ss2.Length(), 5, "Length");

    ss2.StepBack(2);
    EQ_VALUE(ss2.Length(), 3, "Length");

    ss2.StepBack(3);
    EQ_VALUE(ss2.Length(), 0, "Length");

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

    EQ_VALUE(ss2.Length(), 117, "Length");
    EQ_TRUE((ss2.Capacity() >= 117), "(ss2.Capacity() >= 117)");
    EQ_TO(
        ss2,
        "123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789",
        "StringStream", "123456789*13");

    ss1.Reset();
    ss2.Reset();

    char *buffer = ss1.Buffer(2);

    EQ_VALUE(ss1.Length(), 2, "Length");

    buffer[0] = 'a';
    buffer[1] = 'b';

    const char *ab = "ab";

    EQ_VALUE(ss1.Length(), 2, "Length");
    EQ_TRUE(StringUtils::IsEqual(ss1.First(), ab, 2), "IsEqual()");

    buffer = ss1.Buffer(1);
    EQ_VALUE(ss1.Length(), 3, "Length");

    buffer[0]       = 'c';
    const char *ab2 = "abc";
    EQ_VALUE(ss1.Length(), 3, "Length");
    EQ_TRUE(StringUtils::IsEqual(ss1.First(), ab2, 3), "IsEqual()");

    ss2.Clear();
    ss2 += "ab";
    EQ_VALUE((ss1 == ss2), false, "IsEqual()");

    ss1 += "de";
    EQ_VALUE(ss1.Length(), 5, "Length");
    EQ_VALUE(ss1, "abcde", "StringStream");

    ss2 = ss1;
    EQ_VALUE(ss2.Length(), 5, "Length");
    EQ_VALUE(ss2, "abcde", "StringStream");

    ss2.Reset();

    /////////
    ss2 += "123456789123456789";
    const char *ss2_p = ss2.First();
    str               = ss2.GetString();
    EQ_VALUE(ss2_p, str.First(), "StringStream");
    EQ_VALUE(str, "123456789123456789", "StringStream");

    /////////

    ss2.Reset();

    ss1 = "abcdefgh";
    EQ_VALUE(ss1, "abcdefgh", "StringStream");

    ss2 << ss1;
    EQ_VALUE(ss1, "abcdefgh", "StringStream");
    EQ_VALUE(ss2, "abcdefgh", "StringStream");

    String<char> n_str = String<char>("12345678");
    ss1                = n_str;
    EQ_VALUE(ss1, "12345678", "StringStream");

    ss2.Reset();

    ss2 << n_str;
    EQ_VALUE(ss2, "12345678", "StringStream");

    struct SimpleStream {
        char               str[16]{0};
        const unsigned int max = 16;
        unsigned int       index{0};

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
    EQ_TRUE(StringUtils::IsEqual(&(sis.str[0]), "12345678", 8), "SimpleStream");

    ss2 << n_str;
    EQ_VALUE(ss2, "1234567812345678", "StringStream");
    EQ_VALUE(ss2.Capacity(), 16, "Capacity");

    sis.index = 0;
    sis << ss2;

    EQ_TRUE(StringUtils::IsEqual(&(sis.str[0]), "1234567812345678", 16), "SimpleStream");

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
