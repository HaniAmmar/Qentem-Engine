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
#include "Unicode.hpp"

#ifndef QENTEM_UNICODE_TESTS_H_
#define QENTEM_UNICODE_TESTS_H_

namespace Qentem {
namespace Test {

static int TestToUTF8() {
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

    ULong  length;
    QChar *str = HAllocator::Allocate<QChar>(5);

    length = Unicode::ToUTF8(0x0000, str);
    SHOULD_EQUAL_VALUE(length, 1, "length");
    SHOULD_EQUAL_VALUE(str[0], 0, "str[0]");

    length = Unicode::ToUTF8(0x003D, str);
    SHOULD_EQUAL_VALUE(length, 1, "length");
    SHOULD_EQUAL_VALUE(str[0], '=', "str[0]");

    length = Unicode::ToUTF8(0x007F, str);
    SHOULD_EQUAL_VALUE(length, 1, "length");
    SHOULD_EQUAL_VALUE(str[0], QChar(127), "str[0]");

    length = Unicode::ToUTF8(0x0080, str);
    SHOULD_EQUAL_VALUE(length, 2, "length");
    SHOULD_EQUAL_VALUE(str[0], QChar(194), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], QChar(128), "str[1]");

    length = Unicode::ToUTF8(0x00A1, str);
    SHOULD_EQUAL_VALUE(length, 2, "length");
    SHOULD_EQUAL_VALUE(str[0], QChar(194), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], QChar(161), "str[1]");

    length = Unicode::ToUTF8(0x07FF, str);
    SHOULD_EQUAL_VALUE(length, 2, "length");
    SHOULD_EQUAL_VALUE(str[0], QChar(223), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], QChar(191), "str[1]");

    length = Unicode::ToUTF8(0x0800, str);
    SHOULD_EQUAL_VALUE(length, 3, "length");
    SHOULD_EQUAL_VALUE(str[0], QChar(224), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], QChar(160), "str[1]");
    SHOULD_EQUAL_VALUE(str[2], QChar(128), "str[2]");

    length = Unicode::ToUTF8(0x08A7, str);
    SHOULD_EQUAL_VALUE(length, 3, "length");
    SHOULD_EQUAL_VALUE(str[0], QChar(224), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], QChar(162), "str[1]");
    SHOULD_EQUAL_VALUE(str[2], QChar(167), "str[2]");

    length = Unicode::ToUTF8(0xFFFF, str);
    SHOULD_EQUAL_VALUE(length, 3, "length");
    SHOULD_EQUAL_VALUE(str[0], QChar(239), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], QChar(191), "str[1]");
    SHOULD_EQUAL_VALUE(str[2], QChar(191), "str[2]");

    length = Unicode::ToUTF8(0x10000, str);
    SHOULD_EQUAL_VALUE(length, 4, "length");
    SHOULD_EQUAL_VALUE(str[0], QChar(240), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], QChar(144), "str[1]");
    SHOULD_EQUAL_VALUE(str[2], QChar(128), "str[2]");
    SHOULD_EQUAL_VALUE(str[3], QChar(128), "str[3]");

    length = Unicode::ToUTF8(0x10A7B, str);
    SHOULD_EQUAL_VALUE(length, 4, "length");
    SHOULD_EQUAL_VALUE(str[0], QChar(240), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], QChar(144), "str[1]");
    SHOULD_EQUAL_VALUE(str[2], QChar(169), "str[2]");
    SHOULD_EQUAL_VALUE(str[3], QChar(187), "str[3]");

    length = Unicode::ToUTF8(0xE01EF, str);
    SHOULD_EQUAL_VALUE(length, 4, "length");
    SHOULD_EQUAL_VALUE(str[0], QChar(243), "str[0]");
    SHOULD_EQUAL_VALUE(str[1], QChar(160), "str[1]");
    SHOULD_EQUAL_VALUE(str[2], QChar(135), "str[2]");
    SHOULD_EQUAL_VALUE(str[3], QChar(175), "str[3]");

    HAllocator::Deallocate(str);

    END_SUB_TEST;
}

// static String<char> decToHex(ULong code) {
//     StringStream<char> ss;
//     ULong              hex = 0;

//     while (code != 0) {
//         hex  = code % 16U;
//         code = code / 16;
//         ss += char(hex + ((hex < 10) ? 48U : 55U));
//     }

//     String<char> val = ss.GetString();

//     if (val.Length() > 1) {
//         ULong x = 0;
//         ULong y = (val.Length() - 1);

//         // Reverse
//         do {
//             char tmp = val[x];
//             val[x]   = val[y];
//             val[y]   = tmp;
//             ++x;
//             --y;
//         } while (x < y);
//     }

//     return val;
// }

// static void convertTo4Hex(ULong code) {
//     if (code < 0x10000U) {
//         std::cout << "\\u" << decToHex(code).Storage() << "\n";
//     } else {
//         code -= 0x10000U;
//         std::cout << "\n\\u" << decToHex((code >> 10U) | 0xD800U).Storage();
//         std::cout << "\\u" << decToHex((code ^ 0x800U) | 0xDC00U).Storage()
//                   << "\n";
//     }
// }

static int RunUnicodeTests() {
    STARTING_TEST("Unicode.hpp");

    START_TEST("Unicode::ToUTF8", TestToUTF8);

    END_TEST("Unicode.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
