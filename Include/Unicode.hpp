/*
 * Copyright (c) 2024 Hani Ammar
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

#ifndef QENTEM_UNICODE_H
#define QENTEM_UNICODE_H

#include "Common.hpp"

namespace Qentem {
namespace Unicode {

template <typename, typename, SizeT32>
struct UnicodeToUTF;

/*
 * ToUTF(0xC3D, stream);
 * ToUTF(0x00A1, stream);
 * ToUTF(0x08A7, stream);
 * ToUTF(0x10A7B, stream);
 */

template <typename Char_T, typename Stream_T>
static void ToUTF(SizeT32 unicode, Stream_T &stream) {
    UnicodeToUTF<Char_T, Stream_T, sizeof(Char_T)>::ToUTF(unicode, stream);
}

// UTF8
template <typename Char_T, typename Stream_T>
struct UnicodeToUTF<Char_T, Stream_T, 1U> {
    static void ToUTF(SizeT32 unicode, Stream_T &stream) {
        if (unicode < 0x80U) {
            stream += Char_T(unicode);
        } else {
            if (unicode < 0x800U) {
                stream += Char_T(0xC0U | (unicode >> 6U));
            } else if (unicode < 0x10000U) {
                stream += Char_T(0xE0U | (unicode >> 12U));
                stream += Char_T(0x80U | ((unicode >> 6U) & 0x3FU));
            } else {
                stream += Char_T(0xF0U | (unicode >> 18U));
                stream += Char_T(0x80U | ((unicode >> 12U) & 0x3FU));
                stream += Char_T(0x80U | ((unicode >> 6U) & 0x3FU));
            }

            stream += Char_T(0x80U | (unicode & 0x3FU));
        }
    }
};

// UTF16
template <typename Char_T, typename Stream_T>
struct UnicodeToUTF<Char_T, Stream_T, 2U> {
    static void ToUTF(SizeT32 unicode, Stream_T &stream) {
        if (unicode < 0x10000U) {
            stream += Char_T(unicode);
        } else {
            unicode -= 0x10000U;
            stream += Char_T(0xD800U | (unicode >> 10U));
            stream += Char_T(0xDC00U | (unicode & 0x3FFU));
        }
    }
};

// UTF32
template <typename Char_T, typename Stream_T>
struct UnicodeToUTF<Char_T, Stream_T, 4U> {
    static void ToUTF(SizeT32 unicode, Stream_T &stream) {
        stream += Char_T(unicode);
    }
};

} // namespace Unicode
} // namespace Qentem

#endif
