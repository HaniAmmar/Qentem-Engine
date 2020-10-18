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

#ifndef QENTEM_STRINGUTILS_H_
#define QENTEM_STRINGUTILS_H_

namespace Qentem {

struct StringUtils {
    template <typename Char_T_>
    struct WhiteSpaceChars {
        static constexpr Char_T_ SpaceChar           = ' ';
        static constexpr Char_T_ LineControlChar     = '\n';
        static constexpr Char_T_ TabControlChar      = '\t';
        static constexpr Char_T_ CarriageControlChar = '\r';
    };

    template <typename Char_T_, typename NumberType = SizeT>
    static NumberType Count(const Char_T_ *str) noexcept {
        NumberType len = 0;

        if (str != nullptr) {
            while (str[len] != 0) {
                ++len;
            };
        }

        return len;
    }

    template <typename Char_T_, typename Type_>
    static void StartTrim(const Char_T_ *str, Type_ &offset,
                          Type_ end_before) noexcept {
        using WhiteSpaceChars_T_ = WhiteSpaceChars<Char_T_>;

        while ((offset < end_before) &&
               ((str[offset] == WhiteSpaceChars_T_::SpaceChar) ||
                (str[offset] == WhiteSpaceChars_T_::LineControlChar) ||
                (str[offset] == WhiteSpaceChars_T_::TabControlChar) ||
                (str[offset] == WhiteSpaceChars_T_::CarriageControlChar))) {
            ++offset;
        }
    }

    template <typename Char_T_, typename Type_>
    static void SoftTrim(const Char_T_ *str, Type_ &offset,
                         Type_ &length) noexcept {
        using WhiteSpaceChars_T_ = WhiteSpaceChars<Char_T_>;

        if (length != 0) {
            Type_ end_before = (length + offset);
            StartTrim(str, offset, end_before);

            while ((--end_before > offset) &&
                   ((str[end_before] == WhiteSpaceChars_T_::SpaceChar) ||
                    (str[end_before] == WhiteSpaceChars_T_::LineControlChar) ||
                    (str[end_before] == WhiteSpaceChars_T_::TabControlChar) ||
                    (str[end_before] ==
                     WhiteSpaceChars_T_::CarriageControlChar))) {
            }

            ++end_before;
            length = (end_before - offset);
        }
    }

    template <typename Char_T_>
    static bool IsEqual(const Char_T_ *left, const Char_T_ *right,
                        SizeT length) noexcept {
        if ((left != nullptr) && (right != nullptr)) {
            while ((length != 0) && (*left == *right)) {
                ++left;
                ++right;
                --length;
            }
        }

        return (length == 0);
    }

    template <typename Char_T_>
    static ULSizeT Hash(const Char_T_ *key, SizeT length) noexcept {
        ULSizeT hash   = 11;
        SizeT   base   = 33;
        SizeT   offset = 0;

        while (offset != length) {
            const unsigned int num = static_cast<unsigned int>(key[offset]);
            ++offset;
            hash += (base * offset * num);
            base += offset;

            if (offset != length) {
                hash *= (length ^ offset);
                base += offset;
                --length;
                hash += static_cast<ULSizeT>(key[length]);
            }
        }

        return hash;
    }
};

} // namespace Qentem

#endif
