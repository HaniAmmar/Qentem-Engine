/*
 * Copyright (c) 2025 Hani Ammar
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

#ifndef QENTEM_STRINGSTREAM_TESTS_H
#define QENTEM_STRINGSTREAM_TESTS_H

#include "QTest.hpp"
#include "StringStream.hpp"

namespace Qentem {
namespace Test {

static void TestStringStream1(QTest &test) {
    StringStream<char> ss1;
    StringStream<char> ss2{10}; // Preset size
    String<char>       str;

    test.IsEqual(ss1.Length(), SizeT{0}, __LINE__);
    test.IsEqual(ss1.Capacity(), SizeT{0}, __LINE__);
    test.IsNull(ss1.First(), __LINE__);
    test.IsNull(ss1.Last(), __LINE__);

    test.IsEqual(ss2.Capacity(), SizeT{16}, __LINE__);
    test.IsEqual(ss2.Length(), SizeT{0}, __LINE__);
    test.IsNotNull(ss2.First(), __LINE__);

    ss2.Reset();
    test.IsEqual(ss2.Length(), SizeT{0}, __LINE__);
    test.IsEqual(ss2.Capacity(), SizeT{0}, __LINE__);
    test.IsNull(ss2.First(), __LINE__);

    ss1 = StringStream<char>(1);
    ss1 += 'a';
    test.IsEqual(ss1.Length(), SizeT{1}, __LINE__);
    test.IsTrue((ss1.Capacity() >= SizeT{1}), __LINE__);
    test.IsNotNull(ss1.First(), __LINE__);
    test.IsNotNull(ss1.Last(), __LINE__);
    test.IsEqual(ss1.First()[0], 'a', __LINE__);
    test.IsEqual(ss1.GetString(), "a", __LINE__);
    test.IsEqual(ss1.Length(), SizeT{0}, __LINE__);
    test.IsEqual(ss1.Capacity(), SizeT{0}, __LINE__);
    test.IsNull(ss1.First(), __LINE__);

    ss2 += String<char>("a");
    test.IsEqual(ss2.Length(), SizeT{1}, __LINE__);
    test.IsTrue((ss2.Capacity() >= SizeT{1}), __LINE__);
    test.IsEqual(ss2.GetString(), "a", __LINE__);
    test.IsNotEqual(ss2.GetString(), "aa", __LINE__);

    str = String<char>("a");
    ss2 += str;
    test.IsEqual(ss2.Length(), SizeT{1}, __LINE__);
    test.IsTrue((ss2.Capacity() >= SizeT{1}), __LINE__);
    test.IsEqual(ss2, "a", __LINE__);
    test.IsNotEqual(ss2, "aa", __LINE__);

    ss2.Reset();

    ss2 += "abc";
    test.IsEqual(ss2.Length(), SizeT{3}, __LINE__);
    test.IsTrue((ss2.Capacity() >= SizeT{3}), __LINE__);
    test.IsEqual(ss2.GetString(), "abc", __LINE__);
    const String<char> sss;

    ss2 += String<char>("abc");
    test.IsEqual(ss2.Length(), SizeT{3}, __LINE__);
    test.IsTrue((ss2.Capacity() >= SizeT{3}), __LINE__);
    test.IsEqual(ss2, "abc", __LINE__);
    ss2.Reset();

    str = String<char>("abc");
    ss2 += str;
    test.IsEqual(ss2.Length(), SizeT{3}, __LINE__);
    test.IsTrue((ss2.Capacity() >= SizeT{3}), __LINE__);
    test.IsEqual(ss2, "abc", __LINE__);
    ss2.Reset();

    ss2 += String<char>("abc");
    ss1 = ss2; // Copy
    test.IsEqual(ss1.Length(), ss2.Length(), __LINE__);
    test.IsTrue((ss1.Capacity() >= ss2.Length()), __LINE__);
    test.IsEqual(ss1, "abc", __LINE__);
    ss1.Reset();

    const SizeT lss = ss1.Capacity();

    ss1.Clear();
    test.IsEqual(ss1.Length(), SizeT{0}, __LINE__);
    test.IsEqual(ss1.Capacity(), lss, __LINE__);

    ss1.Reset();
    ss1 = Memory::Move(ss2); // Move
    test.IsEqual(ss1.Length(), SizeT{3}, __LINE__);
    test.IsTrue((ss1.Capacity() >= SizeT{3}), __LINE__);
    test.IsEqual(ss1, "abc", __LINE__);
    test.IsNull(ss2.Storage(), __LINE__);
    test.IsEqual(ss2.Length(), SizeT{0}, __LINE__);
    test.IsEqual(ss2.Capacity(), SizeT{0}, __LINE__);
    test.IsNull(ss2.First(), __LINE__);

    ss2 += 'a';
    ss1.Reset();
    ss1 = Memory::Move(ss2); // Move
    test.IsEqual(ss1.Length(), SizeT{1}, __LINE__);
    test.IsTrue((ss1.Capacity() >= SizeT{4}), __LINE__);
    test.IsEqual(ss1, "a", __LINE__);
    ss1.Reset();

    ss1 += "abc";
    ss2 = StringStream<char>(ss1); // Copy
    test.IsEqual(ss1.Length(), SizeT{3}, __LINE__);
    test.IsTrue((ss1.Capacity() >= SizeT{3}), __LINE__);
    test.IsEqual(ss1, "abc", __LINE__);
    test.IsEqual(ss2.Length(), SizeT{3}, __LINE__);
    test.IsTrue((ss2.Capacity() >= SizeT{3}), __LINE__);
    test.IsEqual(ss2, "abc", __LINE__);
    ss1.Reset();
    ss2.Reset();

    ss1 += "efg";
    ss2 = StringStream<char>(Memory::Move(ss1)); // Move
    test.IsNull(ss1.Storage(), __LINE__);
    test.IsEqual(ss1.Length(), SizeT{0}, __LINE__);
    test.IsTrue((ss1.Capacity() == SizeT{0}), __LINE__);
    test.IsNull(ss1.First(), __LINE__);
    test.IsEqual(ss2, "efg", __LINE__);
    test.IsEqual(ss2.Length(), SizeT{3}, __LINE__);
    test.IsTrue((ss2.Capacity() >= SizeT{3}), __LINE__);
    test.IsEqual(ss2.First()[2], 'g', __LINE__);

    ss2.Reset();
    ss2.Write("a", SizeT{1});
    test.IsEqual(ss2.Length(), SizeT{1}, __LINE__);
    test.IsTrue((ss2.Capacity() >= SizeT{1}), __LINE__);
    test.IsTrue((ss2.Capacity() >= SizeT{1}), __LINE__);
    test.IsNotNull(ss2.First(), __LINE__);
    test.IsEqual(ss2.First()[0], 'a', __LINE__);

    ss2.Write("bc", SizeT{2});
    test.IsEqual(ss2.Length(), SizeT{3}, __LINE__);
    test.IsTrue((ss2.Capacity() >= SizeT{3}), __LINE__);
    test.IsEqual(ss2, "abc", __LINE__);
    ss2.Reset();

    ss2.Write("abcdef", SizeT{1});
    ss2.Write("bcdef", SizeT{1});
    ss2.Write("cdef", SizeT{1});

    char      *e_str    = ss2.Detach();
    const bool is_equal = StringUtils::IsEqual(e_str, "abc", SizeT{3});
    Memory::Deallocate(e_str);

    test.IsTrue(is_equal, __LINE__);

    ss2 += "abcdef";
    ss2.StepBack(10);
    test.IsEqual(ss2.Length(), SizeT{6}, __LINE__);

    ss2.StepBack(1);
    test.IsEqual(ss2.Length(), SizeT{5}, __LINE__);

    ss2.StepBack(2);
    test.IsEqual(ss2.Length(), SizeT{3}, __LINE__);

    ss2.StepBack(3);
    test.IsEqual(ss2.Length(), SizeT{0}, __LINE__);

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

    test.IsEqual(ss2.Length(), SizeT{117}, __LINE__);
    test.IsTrue((ss2.Capacity() >= SizeT{117}), __LINE__);
    test.IsEqual(
        ss2,
        "123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789",
        __LINE__);

    ss1.Reset();
    ss2.Reset();

    ss1.SetLength(2);
    test.IsEqual(ss1.Length(), SizeT{2}, __LINE__);

    char *buffer = ss1.Storage();
    buffer[0]    = 'a';
    buffer[1]    = 'b';

    const char *ab = "ab";

    test.IsEqual(ss1.Length(), SizeT{2}, __LINE__);
    test.IsTrue(StringUtils::IsEqual(ss1.First(), ab, SizeT{2}), __LINE__);

    buffer = ss1.Buffer(1);
    test.IsEqual(ss1.Length(), SizeT{3}, __LINE__);

    buffer[0]       = 'c';
    const char *ab2 = "abc";
    test.IsEqual(ss1.Length(), SizeT{3}, __LINE__);
    test.IsTrue(StringUtils::IsEqual(ss1.First(), ab2, SizeT{3}), __LINE__);

    ss2.Clear();
    ss2 += "ab";
    test.IsEqual((ss1 == ss2), false, __LINE__);

    ss1 += "de";
    test.IsEqual(ss1.Length(), SizeT{5}, __LINE__);
    test.IsEqual(ss1, "abcde", __LINE__);

    ss2 = ss1;
    test.IsEqual(ss2.Length(), SizeT{5}, __LINE__);
    test.IsEqual(ss2, "abcde", __LINE__);

    ss2.Reset();

    /////////
    ss2 += "123456789123456789123456789123456789";
    const char *ss2_p = ss2.First();
    str               = ss2.GetString();
    test.IsEqual(ss2_p, str.First(), __LINE__);
    test.IsEqual(str, "123456789123456789123456789123456789", __LINE__);

    /////////

    ss2.Reset();

    ss1 = "abcdefgh";
    test.IsEqual(ss1, "abcdefgh", __LINE__);

    ss2 << ss1;
    test.IsEqual(ss1, "abcdefgh", __LINE__);
    test.IsEqual(ss2, "abcdefgh", __LINE__);

    const String<char> n_str = String<char>("12345678");
    ss1                      = n_str;
    test.IsEqual(ss1, "12345678", __LINE__);

    ss2.Reset();

    ss2 << n_str;
    test.IsEqual(ss2, "12345678", __LINE__);

    struct SimpleStream {
        char          str[32]{0};
        const SizeT32 max = 32U;
        SizeT32       index{0};

        void operator<<(const char ch) noexcept {
            str[index] = ch;
            ++index;
        }
    };

    SimpleStream sis;
    sis << ss2;
    test.IsTrue(StringUtils::IsEqual(&(sis.str[0]), "12345678", SizeT{8}), __LINE__);

    ss2 << n_str;
    test.IsEqual(ss2, "1234567812345678", __LINE__);
    test.IsEqual(ss2.Capacity(), SizeT{32}, __LINE__);

    ss2 << "1234567812345678";
    sis.index = SizeT{0};
    sis << ss2;

    test.IsTrue(StringUtils::IsEqual(&(sis.str[0]), "12345678123456781234567812345678", SizeT{32}), __LINE__);

    ss2.StepBack(1);
    sis.index = SizeT{0};
    sis << ss2;
    test.IsTrue(StringUtils::IsEqual(&(sis.str[0]), "1234567", SizeT{7}), __LINE__);
}

static void TestStringStream2(QTest &test) {
    StringStream<char> stream;

    stream = "123";

    stream.Reverse();
    test.IsEqual(stream.Length(), SizeT{3U}, __LINE__);
    test.IsEqual(stream, "321", __LINE__);

    stream.Reverse(0);
    test.IsEqual(stream.Length(), SizeT{3U}, __LINE__);
    test.IsEqual(stream, "123", __LINE__);

    stream = "0123456789";

    stream.Reverse();
    test.IsEqual(stream.Length(), SizeT{10U}, __LINE__);
    test.IsEqual(stream, "9876543210", __LINE__);

    stream.Reverse();
    test.IsEqual(stream.Length(), SizeT{10U}, __LINE__);
    test.IsEqual(stream, "0123456789", __LINE__);

    stream.Reverse(3);
    test.IsEqual(stream.Length(), SizeT{10U}, __LINE__);
    test.IsEqual(stream, "0129876543", __LINE__);

    stream.Reverse(7);
    test.IsEqual(stream.Length(), SizeT{10U}, __LINE__);
    test.IsEqual(stream, "0129876345", __LINE__);

    stream.Reverse(15); // Nothing will happened
    test.IsEqual(stream.Length(), SizeT{10U}, __LINE__);
    test.IsEqual(stream, "0129876345", __LINE__);
    ///////////////////////////////
    stream = "9";

    stream.InsertAt('0', 0);
    test.IsEqual(stream.Length(), SizeT{2}, __LINE__);
    test.IsEqual(stream, "09", __LINE__);

    stream.InsertAt('4', 1);
    test.IsEqual(stream.Length(), SizeT{3}, __LINE__);
    test.IsEqual(stream, "049", __LINE__);

    stream.InsertAt('4', 10); // Nothing will happened
    test.IsEqual(stream.Length(), SizeT{3}, __LINE__);
    test.IsEqual(stream, "049", __LINE__);

    stream.InsertAt('2', 1);
    stream.InsertAt('1', 1);

    test.IsEqual(stream.Length(), SizeT{5}, __LINE__);
    test.IsEqual(stream, "01249", __LINE__);

    stream.InsertAt('3', 3);
    stream.InsertAt('5', 5);

    test.IsEqual(stream.Length(), SizeT{7}, __LINE__);
    test.IsEqual(stream, "0123459", __LINE__);

    stream.InsertAt('8', 6);
    stream.InsertAt('7', 6);
    stream.InsertAt('6', 6);

    test.IsEqual(stream.Length(), SizeT{10}, __LINE__);
    test.IsEqual(stream, "0123456789", __LINE__);

    stream.Reset();
    stream = "01";
    stream += "23459";
    stream.InsertAt('8', 6);
    stream.InsertAt('7', 6);
    stream.InsertAt('6', 6);

    test.IsEqual(stream.Length(), SizeT{10}, __LINE__);
    test.IsEqual(stream, "0123456789", __LINE__);
    ///////////////////
    stream.Reserve(SizeT{8});

    test.IsEqual(stream.Length(), SizeT{0}, __LINE__);
    test.IsEqual(stream.Capacity(), SizeT{8}, __LINE__);

    stream << "12345678";
    test.IsEqual(stream.Length(), SizeT{8}, __LINE__);
    test.IsEqual(stream.Capacity(), SizeT{8}, __LINE__);
    stream.StepBack(SizeT{1});
    test.IsEqual(*(stream.End()), '8', __LINE__);

    stream.InsertNull();
    test.IsEqual(stream.Length(), SizeT{7}, __LINE__);
    test.IsEqual(stream.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(*(stream.End()), char{0}, __LINE__);

    stream.InsertNull();
    test.IsEqual(stream.Length(), SizeT{7}, __LINE__);
    test.IsEqual(stream.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(*(stream.End()), char{0}, __LINE__);

    stream = "12345678";
    stream.InsertNull();
    test.IsEqual(*(stream.End()), char{0}, __LINE__);
    stream.StepBack(SizeT{1});
    test.IsEqual(*(stream.End()), '8', __LINE__);

    stream = "56789";
    stream.ShiftRight(5);

    test.IsEqual(stream.Length(), SizeT{10}, __LINE__);
    test.IsEqual(stream, "5678956789", __LINE__);

    char *str = stream.Storage();
    str[0]    = '0';
    str[1]    = '1';
    str[2]    = '2';
    str[3]    = '3';
    str[4]    = '4';

    test.IsEqual(stream, "0123456789", __LINE__);

    stream.WriteAt(0, "56789", 5);
    test.IsEqual(stream, "5678956789", __LINE__);

    stream.WriteAt(5, "01234", 5);
    test.IsEqual(stream, "5678901234", __LINE__);

    stream.WriteAt(6, "01234", 5);
    stream.WriteAt(9, "01234", 5);
    stream.WriteAt(40, "01234", 5);
    test.IsEqual(stream, "5678901234", __LINE__);

    stream.WriteAt(0, "A-B-C-D-E-", 10);
    test.IsEqual(stream, "A-B-C-D-E-", __LINE__);

    stream.Reset();
    stream = "56";
    stream += "789";
    stream.ShiftRight(5);

    stream.Storage()[0] = '0';
    stream.Storage()[1] = '1';
    stream.Storage()[2] = '2';
    stream.Storage()[3] = '3';
    stream.Storage()[4] = '4';

    test.IsEqual(stream.Length(), SizeT{10}, __LINE__);
    test.IsEqual(stream, "0123456789", __LINE__);
}

static int RunStringStreamTests() {
    QTest test{"StringStream.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("StringStream Test 1", TestStringStream1);
    test.Test("StringStream Test 2", TestStringStream2);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
