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

#ifndef QENTEM_STRINGSTREAM_TESTS_H
#define QENTEM_STRINGSTREAM_TESTS_H

namespace Qentem {
namespace Test {

static void TestStringStream1(QTest &helper) {
    StringStream<char> ss1;
    StringStream<char> ss2{10}; // Preset size
    String<char>       str;

    helper.IsEqual(ss1.Length(), SizeT{0}, __LINE__);
    helper.IsEqual(ss1.Capacity(), SizeT{0}, __LINE__);
    helper.IsNull(ss1.First(), __LINE__);
    helper.IsNull(ss1.Last(), __LINE__);

    helper.IsTrue((ss2.Capacity() >= SizeT{10}), __LINE__);
    helper.IsEqual(ss2.Length(), SizeT{0}, __LINE__);
    helper.IsNotNull(ss2.First(), __LINE__);

    ss2.Reset();
    helper.IsEqual(ss2.Length(), SizeT{0}, __LINE__);
    helper.IsEqual(ss2.Capacity(), SizeT{0}, __LINE__);
    helper.IsNull(ss2.First(), __LINE__);

    ss1 = StringStream<char>(1);
    ss1 += 'a';
    helper.IsEqual(ss1.Length(), SizeT{1}, __LINE__);
    helper.IsTrue((ss1.Capacity() >= SizeT{1}), __LINE__);
    helper.IsNotNull(ss1.First(), __LINE__);
    helper.IsNotNull(ss1.Last(), __LINE__);
    helper.IsEqual(ss1.First()[0], 'a', __LINE__);
    helper.IsEqual(ss1.GetString(), "a", __LINE__);
    helper.IsEqual(ss1.Length(), SizeT{0}, __LINE__);
    helper.IsEqual(ss1.Capacity(), SizeT{0}, __LINE__);
    helper.IsNull(ss1.First(), __LINE__);

    ss2 += String<char>("a");
    helper.IsEqual(ss2.Length(), SizeT{1}, __LINE__);
    helper.IsTrue((ss2.Capacity() >= SizeT{1}), __LINE__);
    helper.IsEqual(ss2.GetString(), "a", __LINE__);
    helper.IsNotEqual(ss2.GetString(), "aa", __LINE__);

    str = String<char>("a");
    ss2 += str;
    helper.IsEqual(ss2.Length(), SizeT{1}, __LINE__);
    helper.IsTrue((ss2.Capacity() >= SizeT{1}), __LINE__);
    helper.IsEqual(ss2, "a", __LINE__);
    helper.IsNotEqual(ss2, "aa", __LINE__);

    ss2.Reset();

    ss2 += "abc";
    helper.IsEqual(ss2.Length(), SizeT{3}, __LINE__);
    helper.IsTrue((ss2.Capacity() >= SizeT{3}), __LINE__);
    helper.IsEqual(ss2.GetString(), "abc", __LINE__);
    const String<char> sss;

    ss2 += String<char>("abc");
    helper.IsEqual(ss2.Length(), SizeT{3}, __LINE__);
    helper.IsTrue((ss2.Capacity() >= SizeT{3}), __LINE__);
    helper.IsEqual(ss2, "abc", __LINE__);
    ss2.Reset();

    str = String<char>("abc");
    ss2 += str;
    helper.IsEqual(ss2.Length(), SizeT{3}, __LINE__);
    helper.IsTrue((ss2.Capacity() >= SizeT{3}), __LINE__);
    helper.IsEqual(ss2, "abc", __LINE__);
    ss2.Reset();

    ss2 += String<char>("abc");
    ss1 = ss2; // Copy
    helper.IsEqual(ss1.Length(), ss2.Length(), __LINE__);
    helper.IsTrue((ss1.Capacity() >= ss2.Length()), __LINE__);
    helper.IsEqual(ss1, "abc", __LINE__);
    ss1.Reset();

    const SizeT lss = ss1.Capacity();

    ss1.Clear();
    helper.IsEqual(ss1.Length(), SizeT{0}, __LINE__);
    helper.IsEqual(ss1.Capacity(), lss, __LINE__);

    ss1.Reset();
    ss1 = Memory::Move(ss2); // Move
    helper.IsEqual(ss1.Length(), SizeT{3}, __LINE__);
    helper.IsTrue((ss1.Capacity() >= SizeT{3}), __LINE__);
    helper.IsEqual(ss1, "abc", __LINE__);
    helper.IsNull(ss2.Storage(), __LINE__);
    helper.IsEqual(ss2.Length(), SizeT{0}, __LINE__);
    helper.IsEqual(ss2.Capacity(), SizeT{0}, __LINE__);
    helper.IsNull(ss2.First(), __LINE__);

    ss2 += 'a';
    ss1.Reset();
    ss1 = Memory::Move(ss2); // Move
    helper.IsEqual(ss1.Length(), SizeT{1}, __LINE__);
    helper.IsTrue((ss1.Capacity() >= SizeT{4}), __LINE__);
    helper.IsEqual(ss1, "a", __LINE__);
    ss1.Reset();

    ss1 += "abc";
    ss2 = StringStream<char>(ss1); // Copy
    helper.IsEqual(ss1.Length(), SizeT{3}, __LINE__);
    helper.IsTrue((ss1.Capacity() >= SizeT{3}), __LINE__);
    helper.IsEqual(ss1, "abc", __LINE__);
    helper.IsEqual(ss2.Length(), SizeT{3}, __LINE__);
    helper.IsTrue((ss2.Capacity() >= SizeT{3}), __LINE__);
    helper.IsEqual(ss2, "abc", __LINE__);
    ss1.Reset();
    ss2.Reset();

    ss1 += "efg";
    ss2 = StringStream<char>(Memory::Move(ss1)); // Move
    helper.IsNull(ss1.Storage(), __LINE__);
    helper.IsEqual(ss1.Length(), SizeT{0}, __LINE__);
    helper.IsTrue((ss1.Capacity() == SizeT{0}), __LINE__);
    helper.IsNull(ss1.First(), __LINE__);
    helper.IsEqual(ss2, "efg", __LINE__);
    helper.IsEqual(ss2.Length(), SizeT{3}, __LINE__);
    helper.IsTrue((ss2.Capacity() >= SizeT{3}), __LINE__);
    helper.IsEqual(ss2.First()[2], 'g', __LINE__);

    ss2.Reset();
    ss2.Write("a", SizeT{1});
    helper.IsEqual(ss2.Length(), SizeT{1}, __LINE__);
    helper.IsTrue((ss2.Capacity() >= SizeT{1}), __LINE__);
    helper.IsTrue((ss2.Capacity() >= SizeT{1}), __LINE__);
    helper.IsNotNull(ss2.First(), __LINE__);
    helper.IsEqual(ss2.First()[0], 'a', __LINE__);

    ss2.Write("bc", SizeT{2});
    helper.IsEqual(ss2.Length(), SizeT{3}, __LINE__);
    helper.IsTrue((ss2.Capacity() >= SizeT{3}), __LINE__);
    helper.IsEqual(ss2, "abc", __LINE__);
    ss2.Reset();

    ss2.Write("abcdef", SizeT{1});
    ss2.Write("bcdef", SizeT{1});
    ss2.Write("cdef", SizeT{1});

    char      *e_str    = ss2.Detach();
    const bool is_equal = StringUtils::IsEqual(e_str, "abc", SizeT{3});
    Memory::Deallocate(e_str);

    helper.IsTrue(is_equal, __LINE__);

    ss2 += "abcdef";
    ss2.StepBack(10);
    helper.IsEqual(ss2.Length(), SizeT{6}, __LINE__);

    ss2.StepBack(1);
    helper.IsEqual(ss2.Length(), SizeT{5}, __LINE__);

    ss2.StepBack(2);
    helper.IsEqual(ss2.Length(), SizeT{3}, __LINE__);

    ss2.StepBack(3);
    helper.IsEqual(ss2.Length(), SizeT{0}, __LINE__);

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

    helper.IsEqual(ss2.Length(), SizeT{117}, __LINE__);
    helper.IsTrue((ss2.Capacity() >= SizeT{117}), __LINE__);
    helper.IsEqual(
        ss2,
        "123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789",
        __LINE__);

    ss1.Reset();
    ss2.Reset();

    char *buffer = ss1.Buffer(2);

    helper.IsEqual(ss1.Length(), SizeT{2}, __LINE__);

    buffer[0] = 'a';
    buffer[1] = 'b';

    const char *ab = "ab";

    helper.IsEqual(ss1.Length(), SizeT{2}, __LINE__);
    helper.IsTrue(StringUtils::IsEqual(ss1.First(), ab, SizeT{2}), __LINE__);

    buffer = ss1.Buffer(1);
    helper.IsEqual(ss1.Length(), SizeT{3}, __LINE__);

    buffer[0]       = 'c';
    const char *ab2 = "abc";
    helper.IsEqual(ss1.Length(), SizeT{3}, __LINE__);
    helper.IsTrue(StringUtils::IsEqual(ss1.First(), ab2, SizeT{3}), __LINE__);

    ss2.Clear();
    ss2 += "ab";
    helper.IsEqual((ss1 == ss2), false, __LINE__);

    ss1 += "de";
    helper.IsEqual(ss1.Length(), SizeT{5}, __LINE__);
    helper.IsEqual(ss1, "abcde", __LINE__);

    ss2 = ss1;
    helper.IsEqual(ss2.Length(), SizeT{5}, __LINE__);
    helper.IsEqual(ss2, "abcde", __LINE__);

    ss2.Reset();

    /////////
    ss2 += "123456789123456789123456789123456789";
    const char *ss2_p = ss2.First();
    str               = ss2.GetString();
    helper.IsEqual(ss2_p, str.First(), __LINE__);
    helper.IsEqual(str, "123456789123456789123456789123456789", __LINE__);

    /////////

    ss2.Reset();

    ss1 = "abcdefgh";
    helper.IsEqual(ss1, "abcdefgh", __LINE__);

    ss2 << ss1;
    helper.IsEqual(ss1, "abcdefgh", __LINE__);
    helper.IsEqual(ss2, "abcdefgh", __LINE__);

    const String<char> n_str = String<char>("12345678");
    ss1                      = n_str;
    helper.IsEqual(ss1, "12345678", __LINE__);

    ss2.Reset();

    ss2 << n_str;
    helper.IsEqual(ss2, "12345678", __LINE__);

    struct SimpleStream {
        char          str[32]{0};
        const SizeT32 max = 32U;
        SizeT32       index{0};

        void operator<<(const char *string) noexcept {
            for (SizeT32 i = SizeT{0}; i < max; i++) {
                const char c = string[index];

                if (c == char{0}) {
                    break;
                }

                str[index] = c;
                ++index;
            }
        }
    };

    SimpleStream sis;
    sis << ss2;
    helper.IsTrue(StringUtils::IsEqual(&(sis.str[0]), "12345678", SizeT{8}), __LINE__);

    ss2 << n_str;
    helper.IsEqual(ss2, "1234567812345678", __LINE__);
    helper.IsEqual(ss2.Capacity(), SizeT{32}, __LINE__);

    ss2 << "1234567812345678";
    sis.index = SizeT{0};
    sis << ss2;

    helper.IsTrue(StringUtils::IsEqual(&(sis.str[0]), "12345678123456781234567812345678", SizeT{32}), __LINE__);

    ss2.StepBack(1);
    sis.index = SizeT{0};
    sis << ss2;
    helper.IsTrue(StringUtils::IsEqual(&(sis.str[0]), "1234567", SizeT{7}), __LINE__);
}

static void TestStringStream2(QTest &helper) {
    StringStream<char> stream;

    stream = "123";

    stream.Reverse();
    helper.IsEqual(stream.Length(), SizeT{3U}, __LINE__);
    helper.IsEqual(stream, "321", __LINE__);

    stream.Reverse(0);
    helper.IsEqual(stream.Length(), SizeT{3U}, __LINE__);
    helper.IsEqual(stream, "123", __LINE__);

    stream = "0123456789";

    stream.Reverse();
    helper.IsEqual(stream.Length(), SizeT{10U}, __LINE__);
    helper.IsEqual(stream, "9876543210", __LINE__);

    stream.Reverse();
    helper.IsEqual(stream.Length(), SizeT{10U}, __LINE__);
    helper.IsEqual(stream, "0123456789", __LINE__);

    stream.Reverse(3);
    helper.IsEqual(stream.Length(), SizeT{10U}, __LINE__);
    helper.IsEqual(stream, "0129876543", __LINE__);

    stream.Reverse(7);
    helper.IsEqual(stream.Length(), SizeT{10U}, __LINE__);
    helper.IsEqual(stream, "0129876345", __LINE__);

    stream.Reverse(15); // Nothing will happened
    helper.IsEqual(stream.Length(), SizeT{10U}, __LINE__);
    helper.IsEqual(stream, "0129876345", __LINE__);
    ///////////////////////////////
    stream = "9";

    stream.InsertAt('0', 0);
    helper.IsEqual(stream.Length(), SizeT{2}, __LINE__);
    helper.IsEqual(stream, "09", __LINE__);

    stream.InsertAt('4', 1);
    helper.IsEqual(stream.Length(), SizeT{3}, __LINE__);
    helper.IsEqual(stream, "049", __LINE__);

    stream.InsertAt('4', 10); // Nothing will happened
    helper.IsEqual(stream.Length(), SizeT{3}, __LINE__);
    helper.IsEqual(stream, "049", __LINE__);

    stream.InsertAt('2', 1);
    stream.InsertAt('1', 1);

    helper.IsEqual(stream.Length(), SizeT{5}, __LINE__);
    helper.IsEqual(stream, "01249", __LINE__);

    stream.InsertAt('3', 3);
    stream.InsertAt('5', 5);

    helper.IsEqual(stream.Length(), SizeT{7}, __LINE__);
    helper.IsEqual(stream, "0123459", __LINE__);

    stream.InsertAt('8', 6);
    stream.InsertAt('7', 6);
    stream.InsertAt('6', 6);

    helper.IsEqual(stream.Length(), SizeT{10}, __LINE__);
    helper.IsEqual(stream, "0123456789", __LINE__);
}

static int RunStringStreamTests() {
    QTest helper{"StringStream.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("StringStream Test 1", TestStringStream1);
    helper.Test("StringStream Test 2", TestStringStream2);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
