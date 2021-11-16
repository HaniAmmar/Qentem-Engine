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

class StringUtils {
  public:
    template <typename Char_T_>
    class WhiteSpaceChars {
      public:
        static constexpr Char_T_ SpaceChar           = ' ';
        static constexpr Char_T_ LineControlChar     = '\n';
        static constexpr Char_T_ TabControlChar      = '\t';
        static constexpr Char_T_ CarriageControlChar = '\r';
    };

    template <typename Char_T_, typename Number_T_ = SizeT>
    static Number_T_ Count(const Char_T_ *str) noexcept {
        Number_T_ len = 0;

        if (str != nullptr) {
            while (str[len] != 0) {
                ++len;
            };
        }

        return len;
    }

    template <typename Char_T_, typename Number_T_>
    static void TrimLeft(const Char_T_ *str, Number_T_ &offset,
                         Number_T_ end_before) noexcept {
        using WhiteSpaceChars_T_ = WhiteSpaceChars<Char_T_>;

        while (offset < end_before) {
            const Char_T_ c = str[offset];

            if ((c != WhiteSpaceChars_T_::SpaceChar) &&
                (c != WhiteSpaceChars_T_::LineControlChar) &&
                (c != WhiteSpaceChars_T_::TabControlChar) &&
                (c != WhiteSpaceChars_T_::CarriageControlChar)) {
                return;
            }

            ++offset;
        }
    }

    template <typename Char_T_, typename Number_T_>
    static void Trim(const Char_T_ *str, Number_T_ &offset,
                     Number_T_ &length) noexcept {
        using WhiteSpaceChars_T_ = WhiteSpaceChars<Char_T_>;

        if (length != 0) {
            Number_T_ end_before = (length + offset);
            TrimLeft(str, offset, end_before);

            while (--end_before > offset) {
                const Char_T_ c = str[end_before];

                if ((c != WhiteSpaceChars_T_::SpaceChar) &&
                    (c != WhiteSpaceChars_T_::LineControlChar) &&
                    (c != WhiteSpaceChars_T_::TabControlChar) &&
                    (c != WhiteSpaceChars_T_::CarriageControlChar)) {
                    break;
                }
            }

            ++end_before;
            length = (end_before - offset);
        }
    }

    template <typename Char_T_>
    static bool IsLess(const Char_T_ *left, const Char_T_ *right,
                       SizeT left_length, SizeT right_length,
                       bool orEqual) noexcept {
        SizeT offset = 0;

        while ((left_length != offset) && (right_length != offset)) {
            if (left[offset] > right[offset]) {
                return false;
            }

            if (left[offset] < right[offset]) {
                return true;
            }

            ++offset;
        }

        return (orEqual && (left_length == right_length));
    }

    template <typename Char_T_>
    static bool IsBigger(const Char_T_ *left, const Char_T_ *right,
                         SizeT left_length, SizeT right_length,
                         bool orEqual) noexcept {
        SizeT offset = 0;

        while ((left_length != offset) && (right_length != offset)) {
            if (left[offset] < right[offset]) {
                return false;
            }

            if (left[offset] > right[offset]) {
                return true;
            }

            ++offset;
        }

        return (orEqual && (left_length == right_length));
    }

    template <typename Char_T_>
    static bool IsEqual(const Char_T_ *left, const Char_T_ *right,
                        SizeT length) noexcept {
        SizeT offset = 0;

        // if ((left != nullptr) && (right != nullptr)) {
        while ((length != offset) && (left[offset] == right[offset])) {
            ++offset;
        }
        // }

        return (length == offset);
    }

    template <typename Char_T_>
    static SizeT Hash(const Char_T_ *key, SizeT length) noexcept {
        static constexpr SizeT highest_bit =
            (SizeT{1} << ((sizeof(SizeT) * 8) - 1));

        SizeT hash   = 11;
        SizeT base   = 33;
        SizeT offset = 0;

        while (offset != length) {
            const unsigned int num = static_cast<unsigned char>(key[offset]);
            ++offset;
            hash += (base * offset * num);
            base += offset;

            if (offset != length) {
                hash *= (length ^ offset);
                base += offset;
                --length;
                hash += static_cast<SizeT>(key[length]);
            }
        }

        return (hash | highest_bit); // Never return zero.
    }
};

} // namespace Qentem

#endif
