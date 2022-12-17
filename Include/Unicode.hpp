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

#include "Common.hpp"
#include "StringStream.hpp"

#ifndef QENTEM_UNICODE_H_
#define QENTEM_UNICODE_H_

namespace Qentem {
namespace Unicode {

template <typename Char_T_, int S>
struct UnicodeToUTF {};

template <typename Char_T_>
static void ToUTF(unsigned int unicode, StringStream<Char_T_> &ss) {
    UnicodeToUTF<Char_T_, static_cast<int>(sizeof(Char_T_))>::ToUTF(unicode, ss);
}

// UTF8
template <typename Char_T_>
class UnicodeToUTF<Char_T_, 1> {
  public:
    static void ToUTF(unsigned int unicode, StringStream<Char_T_> &ss) {
        /*
         * ToUTF(0xC3D, ss);
         * ToUTF(0x00A1, ss);
         * ToUTF(0x08A7, ss);
         * ToUTF(0x10A7B, ss);
         */

        if (unicode < 0x80U) {
            ss += static_cast<Char_T_>(unicode);
        } else {
            if (unicode < 0x800U) {
                ss += static_cast<Char_T_>(0xC0U | (unicode >> 6U));
            } else if (unicode < 0x10000U) {
                ss += static_cast<Char_T_>(0xE0U | (unicode >> 12U));
                ss += static_cast<Char_T_>(0x80U | ((unicode >> 6U) & 0x3FU));
            } else {
                ss += static_cast<Char_T_>(0xF0U | (unicode >> 18U));
                ss += static_cast<Char_T_>(0x80U | ((unicode >> 12U) & 0x3FU));
                ss += static_cast<Char_T_>(0x80U | ((unicode >> 6U) & 0x3FU));
            }

            ss += static_cast<Char_T_>(0x80U | (unicode & 0x3FU));
        }
    }
};

// UTF16
template <typename Char_T_>
class UnicodeToUTF<Char_T_, 2> {
  public:
    static void ToUTF(unsigned int unicode, StringStream<Char_T_> &ss) {
        if (unicode < 0x10000U) {
            ss += static_cast<Char_T_>(unicode);
        } else {
            unicode -= 0x10000;
            ss += static_cast<Char_T_>(0xD800U | (unicode >> 10U));
            ss += static_cast<Char_T_>(0xDC00U | (unicode & 0x3FFU));
        }
    }
};

// UTF32
template <typename Char_T_>
class UnicodeToUTF<Char_T_, 4> {
  public:
    static void ToUTF(unsigned int unicode, StringStream<Char_T_> &ss) { ss += static_cast<Char_T_>(unicode); }
};

} // namespace Unicode
} // namespace Qentem

#endif
