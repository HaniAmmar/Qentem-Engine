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

#ifndef QENTEM_UNICODE_TESTS_H
#define QENTEM_UNICODE_TESTS_H

#include "QTest.hpp"
#include "StringStream.hpp"
#include "Unicode.hpp"

namespace Qentem {
namespace Test {

static void TestToUTF8(QTest &test) {
    /*
     * HEX         UNICODE      BINARY
     * 0x0000      0            00000000
     * 0x003D      61           00111101
     * 0x007F      127          01111111
     *
     * 0x0080      128          11000010-10000000
     * 0x00A1      161          11000010-10100001
     * 0x07FF      2047         11011111-10111111
     *
     * 0x0800      2048         11100000-10100000-10000000
     * 0x08A7      2215         11100000-10100010-10100111
     * 0xFFFF      65535        11101111-10111111-10111111
     *
     * 0x10000     65536        11110000-10010000-10000000-10000000
     * 0x10A7B     68219        11110000-10010000-10101001-10111011
     * 0xE01EF     917999       11110011-10100000-10000111-10101111
     */

    using QChar = unsigned char;

    StringStream<QChar> stream;

    Unicode::ToUTF<QChar>(0x0000, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{'\0'}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x003D, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{'='}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x007F, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{127}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x0080, stream);
    test.IsEqual(stream.Length(), SizeT{2}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{194}, __LINE__);
    test.IsTrue(*(stream.First() + 1U) == QChar{128}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x00A1, stream);
    test.IsEqual(stream.Length(), SizeT{2}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{194}, __LINE__);
    test.IsTrue(*(stream.First() + 1U) == QChar{161}, __LINE__);
    const QChar v00A1[] = "¡";
    test.IsEqual(stream, &(v00A1[0]), __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x07FF, stream);
    test.IsEqual(stream.Length(), SizeT{2}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{223}, __LINE__);
    test.IsTrue(*(stream.First() + 1U) == QChar{191}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x0800, stream);
    test.IsEqual(stream.Length(), SizeT{3}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{224}, __LINE__);
    test.IsTrue(*(stream.First() + 1U) == QChar{160}, __LINE__);
    test.IsTrue(*(stream.First() + 2U) == QChar{128}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x08A7, stream);
    test.IsEqual(stream.Length(), SizeT{3}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{224}, __LINE__);
    test.IsTrue(*(stream.First() + 1U) == QChar{162}, __LINE__);
    test.IsTrue(*(stream.First() + 2U) == QChar{167}, __LINE__);
    const QChar v08A7[] = "ࢧ";
    test.IsEqual(stream, &(v08A7[0]), __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0xFFFF, stream);
    test.IsEqual(stream.Length(), SizeT{3}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{239}, __LINE__);
    test.IsTrue(*(stream.First() + 1U) == QChar{191}, __LINE__);
    test.IsTrue(*(stream.First() + 2U) == QChar{191}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x10000, stream);
    test.IsEqual(stream.Length(), SizeT{4}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{240}, __LINE__);
    test.IsTrue(*(stream.First() + 1U) == QChar{144}, __LINE__);
    test.IsTrue(*(stream.First() + 2U) == QChar{128}, __LINE__);
    test.IsTrue(*(stream.First() + 3U) == QChar{128}, __LINE__);
    const QChar v10000[] = "𐀀";
    test.IsEqual(stream, &(v10000[0]), __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x10A7B, stream);
    test.IsEqual(stream.Length(), SizeT{4}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{240}, __LINE__);
    test.IsTrue(*(stream.First() + 1U) == QChar{144}, __LINE__);
    test.IsTrue(*(stream.First() + 2U) == QChar{169}, __LINE__);
    test.IsTrue(*(stream.First() + 3U) == QChar{187}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0xE01EF, stream);
    test.IsEqual(stream.Length(), SizeT{4}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{243}, __LINE__);
    test.IsTrue(*(stream.First() + 1U) == QChar{160}, __LINE__);
    test.IsTrue(*(stream.First() + 2U) == QChar{135}, __LINE__);
    test.IsTrue(*(stream.First() + 3U) == QChar{175}, __LINE__);
    stream.Clear();
}

static void TestToUTF16(QTest &test) {
    using QChar = char16_t;

    StringStream<QChar> stream;

    Unicode::ToUTF<QChar>(0x0000, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{u'\0'}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x003D, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{u'='}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x007F, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{127}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x0080, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{128}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x00A1, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{161}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x07FF, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{2047}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x0800, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{2048}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x08A7, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{2215}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0xFFFF, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{65535}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x10000, stream);
    test.IsEqual(stream.Length(), SizeT{2}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{55296}, __LINE__);
    test.IsTrue(*(stream.First() + 1U) == QChar{56320}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x10A7B, stream);
    test.IsEqual(stream.Length(), SizeT{2}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{55298}, __LINE__);
    test.IsTrue(*(stream.First() + 1U) == QChar{56955}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0xE01EF, stream);
    test.IsEqual(stream.Length(), SizeT{2}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{56128}, __LINE__);
    test.IsTrue(*(stream.First() + 1U) == QChar{56815}, __LINE__);
    stream.Clear();
}

static void TestToUTF32(QTest &test) {
    using QChar = char32_t;

    StringStream<char32_t> stream;

    Unicode::ToUTF<QChar>(0x0000, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{U'\0'}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x003D, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{U'='}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x007F, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{127}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x0080, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{128}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x00A1, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{161}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x07FF, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{2047}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x0800, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{2048}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x08A7, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{2215}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0xFFFF, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{65535}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x10000, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{65536}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0x10A7B, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{68219}, __LINE__);
    stream.Clear();

    Unicode::ToUTF<QChar>(0xE01EF, stream);
    test.IsEqual(stream.Length(), SizeT{1}, __LINE__);
    test.IsTrue(*(stream.First()) == QChar{917999}, __LINE__);
    stream.Clear();
}

// static String<char> decToHex(SizeT code) {
//     StringStream<char> stream;
//     SizeT              hex = 0;

//     while (code != 0) {
//         hex  = code % 16U;
//         code = code / 16;
//         stream += char(hex + ((hex < 10) ? 48U : 55U));
//     }

//     String<char> str = stream.GetString();
//     char *       val = str.Storage();

//     if (str.Length() > 1) {
//         SizeT x = 0;
//         SizeT y = (str.Length() - 1);

//         // Reverse
//         do {
//             char tmp = val[x];
//             val[x]   = val[y];
//             val[y]   = tmp;
//             ++x;
//             --y;
//         } while (x < y);
//     }

//     return str;
// }

// static void convertTo4Hex(SizeT code) {
//     if (code < 0x10000U) {
//         std::cout << "\\u" << decToHex(code) << '\n';
//     } else {
//         code -= 0x10000U;
//         std::cout << "\n\\u" << decToHex(0xD800U | (code >> 10U)).First();
//         std::cout << "\\u" << decToHex(0xDC00U | (code & 0x3FFU)).First()
//                   << '\n';
//     }
// }

static int RunUnicodeTests() {
    // convertTo4Hex(0x10A7B);
    // convertTo4Hex(0x1F859);
    // convertTo4Hex(0x10283);

    QTest test{"Unicode.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("ToUTF 8 Test", TestToUTF8);
    test.Test("ToUTF 16 Test", TestToUTF16);
    test.Test("ToUTF 32 Test", TestToUTF32);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
