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

#include <Common.hpp>

#ifndef QENTEM_UNICODE_H_
#define QENTEM_UNICODE_H_

namespace Qentem {
namespace Unicode {

// TODO: Add 16 and 32 BE and LE

template <typename Char_T_>
static UInt ToUTF8(UInt unicode, Char_T_ *str) noexcept {
    /*
     * Char_T_ *str = new Char_T_[5];
     * ULong len;
     * len = ToUTF8(0xC3D, str);
     * len = ToUTF8(0x00A1, str);
     * len = ToUTF8(0x08A7, str);
     * len = ToUTF8(0x10A7B, str);
     * str[len] = 0;
     */

    if (unicode < 0x80U) {
        str[0] = Char_T_(unicode);
        return 1;
    }

    UInt length;

    if (unicode < 0x800U) {
        str[0] = Char_T_(0xC0U | (unicode >> 6U));
        length = 2;
    } else if (unicode < 0x10000U) {
        str[0] = Char_T_(0xE0U | (unicode >> 12U));
        str[1] = Char_T_(0x80U | ((unicode >> 6U) & 0x3FU));
        ++str;
        length = 3;
    } else {
        str[0] = Char_T_(0xF0U | (unicode >> 18U));
        str[1] = Char_T_(0x80U | ((unicode >> 12U) & 0x3FU));
        str[2] = Char_T_(0x80U | ((unicode >> 6U) & 0x3FU));
        str += 2;
        length = 4;
    }

    *(++str) = Char_T_(0x80U | (unicode & 0x3FU));

    return length;
}

} // namespace Unicode
} // namespace Qentem

#endif
