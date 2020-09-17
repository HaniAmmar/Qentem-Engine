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

namespace SubStringUtils {
template <typename Char_T_, ULong S = sizeof(Char_T_)>
struct WhiteSpaceChars {};

// One byte character.
template <typename Char_T_>
struct WhiteSpaceChars<Char_T_, 1> {
    static constexpr Char_T_ SpaceChar           = ' ';
    static constexpr Char_T_ LineControlChar     = '\n';
    static constexpr Char_T_ TabControlChar      = '\t';
    static constexpr Char_T_ CarriageControlChar = '\r';
};

// Two bytes character.
template <typename Char_T_>
struct WhiteSpaceChars<Char_T_, 2> {
    static constexpr Char_T_ SpaceChar           = u' ';
    static constexpr Char_T_ LineControlChar     = u'\n';
    static constexpr Char_T_ TabControlChar      = u'\t';
    static constexpr Char_T_ CarriageControlChar = u'\r';
};

// Four bytes character.
template <typename Char_T_>
struct WhiteSpaceChars<Char_T_, 4> {
    static constexpr Char_T_ SpaceChar           = U' ';
    static constexpr Char_T_ LineControlChar     = U'\n';
    static constexpr Char_T_ TabControlChar      = U'\t';
    static constexpr Char_T_ CarriageControlChar = U'\r';
};

// wchar_t
template <>
struct WhiteSpaceChars<wchar_t> {
    static constexpr wchar_t SpaceChar           = L' ';
    static constexpr wchar_t LineControlChar     = L'\n';
    static constexpr wchar_t TabControlChar      = L'\t';
    static constexpr wchar_t CarriageControlChar = L'\r';
};
} // namespace SubStringUtils

struct StringUtils {

    template <typename Char_T_, typename NumberType = ULong>
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
        using WhiteSpaceChars_T_ = SubStringUtils::WhiteSpaceChars<Char_T_>;

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
        using WhiteSpaceChars_T_ = SubStringUtils::WhiteSpaceChars<Char_T_>;

        if (length != 0) {
            Type_ end = (length + offset);
            StartTrim(str, offset, end);

            while ((--end > offset) &&
                   ((str[end] == WhiteSpaceChars_T_::SpaceChar) ||
                    (str[end] == WhiteSpaceChars_T_::LineControlChar) ||
                    (str[end] == WhiteSpaceChars_T_::TabControlChar) ||
                    (str[end] == WhiteSpaceChars_T_::CarriageControlChar))) {
            }

            ++end;
            length = (end - offset);
        }
    }
};

} // namespace Qentem

#endif
