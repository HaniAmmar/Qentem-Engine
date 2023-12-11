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

#include "Common.hpp"

#ifndef QENTEM_STRING_UTILS_H_
#define QENTEM_STRING_UTILS_H_

namespace Qentem {

struct StringUtils {
    template <typename Char_T_>
    struct WhiteSpaceChars_T_ {
        static constexpr Char_T_ SpaceChar           = ' ';
        static constexpr Char_T_ LineControlChar     = '\n';
        static constexpr Char_T_ TabControlChar      = '\t';
        static constexpr Char_T_ CarriageControlChar = '\r';
    };

    template <typename Char_T_, typename Number_T_ = SizeT>
    static Number_T_ Count(const Char_T_ *str) noexcept {
        Number_T_ len = 0;

        if (str != nullptr) {
            while (*str != Char_T_{'\0'}) {
                ++str;
                ++len;
            }
        }

        return len;
    }

    // template <typename Char_T_, typename Number_T_ = SizeT>
    // static constexpr Number_T_ ConstCount(const Char_T_ *str) noexcept {
    //     const Char_T_ *str_2 = str;
    //     Number_T_      len   = 0;

    //     if (str_2 != nullptr) {
    //         while (*str_2 != Char_T_{'\0'}) {
    //             ++str_2;
    //             ++len;
    //         }
    //     }

    //     return len;
    // }

    template <typename Char_T_, typename Number_T_>
    static void TrimLeft(const Char_T_ *str, Number_T_ &offset, const Number_T_ end_offset) noexcept {
        using WhiteSpaceChars = WhiteSpaceChars_T_<Char_T_>;

        while (offset < end_offset) {
            switch (str[offset]) {
                case WhiteSpaceChars::SpaceChar:
                case WhiteSpaceChars::LineControlChar:
                case WhiteSpaceChars::TabControlChar:
                case WhiteSpaceChars::CarriageControlChar:
                    break;

                default: {
                    return;
                }
            }

            ++offset;
        }
    }

    template <typename Char_T_, typename Number_T_>
    static void TrimRight(const Char_T_ *str, const Number_T_ offset, Number_T_ &end_offset) noexcept {
        using WhiteSpaceChars = WhiteSpaceChars_T_<Char_T_>;

        if (end_offset > offset) {
            do {
                --end_offset;

                switch (str[end_offset]) {
                    case WhiteSpaceChars::SpaceChar:
                    case WhiteSpaceChars::LineControlChar:
                    case WhiteSpaceChars::TabControlChar:
                    case WhiteSpaceChars::CarriageControlChar:
                        break;

                    default: {
                        ++end_offset;
                        return;
                    }
                }

            } while (end_offset > offset);
        }
    }

    // offset: the starting offset
    // length: the number of characters
    template <typename Char_T_, typename Number_T_>
    static void Trim(const Char_T_ *str, Number_T_ &offset, Number_T_ &length) noexcept {
        if (length != Number_T_{0}) {
            Number_T_ end_offset = (length + offset);
            TrimLeft(str, offset, end_offset);
            TrimRight(str, offset, end_offset);
            length = (end_offset - offset);
        }
    }

    template <typename Char_T_>
    static bool IsLess(const Char_T_ *left, const Char_T_ *right, SizeT left_length, SizeT right_length,
                       bool orEqual) noexcept {
        SizeT offset = 0;

        while ((left_length > offset) && (right_length > offset)) {
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
    static bool IsGreater(const Char_T_ *left, const Char_T_ *right, SizeT left_length, SizeT right_length,
                          bool orEqual) noexcept {
        SizeT offset = 0;

        while ((left_length > offset) && (right_length > offset)) {
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
    static bool IsEqual(const Char_T_ *left, const Char_T_ *right, SizeT length) noexcept {
        SizeT offset = 0;

        // if ((left != nullptr) && (right != nullptr)) {
        while ((length > offset) && (left[offset] == right[offset])) {
            ++offset;
        }
        // }

        return (length == offset);
    }

    template <typename Char_T_>
    static SizeT Hash(const Char_T_ *key, SizeT length) noexcept {
        constexpr SizeT highest_bit = (SizeT{1} << ((sizeof(SizeT) * 8) - SizeT{1}));

        SizeT hash   = SizeT{11};
        SizeT base   = SizeT{33};
        SizeT offset = 0;

        while (offset < length) {
            hash += (base * offset * SizeT(key[offset]));
            base += offset;

            if (offset != length) {
                hash *= (length ^ offset);
                base += offset;
                --length;
                hash += SizeT(key[length]);
            }

            ++offset;
        }

        return (hash | highest_bit);
    }
};

} // namespace Qentem

#endif
