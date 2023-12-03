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

#ifndef QENTEM_UNICODE_H_
#define QENTEM_UNICODE_H_

namespace Qentem::Unicode {

template <typename, typename, SizeT32>
struct UnicodeToUTF {};

template <typename Char_T_, typename Stream_T_>
static constexpr void ToUTF(SizeT32 unicode, Stream_T_ &stream) {
    UnicodeToUTF<Char_T_, Stream_T_, sizeof(Char_T_)>::ToUTF(unicode, stream);
}

// UTF8
template <typename Char_T_, typename Stream_T_>
struct UnicodeToUTF<Char_T_, Stream_T_, 1U> {
    static void ToUTF(SizeT32 unicode, Stream_T_ &stream) {
        /*
         * ToUTF(0xC3D, stream);
         * ToUTF(0x00A1, stream);
         * ToUTF(0x08A7, stream);
         * ToUTF(0x10A7B, stream);
         */

        if (unicode < 0x80U) {
            stream += Char_T_(unicode);
        } else {
            if (unicode < 0x800U) {
                stream += Char_T_(0xC0U | (unicode >> 6U));
            } else if (unicode < 0x10000U) {
                stream += Char_T_(0xE0U | (unicode >> 12U));
                stream += Char_T_(0x80U | ((unicode >> 6U) & 0x3FU));
            } else {
                stream += Char_T_(0xF0U | (unicode >> 18U));
                stream += Char_T_(0x80U | ((unicode >> 12U) & 0x3FU));
                stream += Char_T_(0x80U | ((unicode >> 6U) & 0x3FU));
            }

            stream += Char_T_(0x80U | (unicode & 0x3FU));
        }
    }
};

// UTF16
template <typename Char_T_, typename Stream_T_>
struct UnicodeToUTF<Char_T_, Stream_T_, 2U> {
    static void ToUTF(SizeT32 unicode, Stream_T_ &stream) {
        if (unicode < 0x10000U) {
            stream += Char_T_(unicode);
        } else {
            unicode -= 0x10000U;
            stream += Char_T_(0xD800U | (unicode >> 10U));
            stream += Char_T_(0xDC00U | (unicode & 0x3FFU));
        }
    }
};

// UTF32
template <typename Char_T_, typename Stream_T_>
struct UnicodeToUTF<Char_T_, Stream_T_, 4U> {
    static void ToUTF(SizeT32 unicode, Stream_T_ &stream) {
        stream += Char_T_(unicode);
    }
};

} // namespace Qentem::Unicode

#endif
