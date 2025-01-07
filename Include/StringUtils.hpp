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

#ifndef QENTEM_STRING_UTILS_H
#define QENTEM_STRING_UTILS_H

#include "QCommon.hpp"

namespace Qentem {

namespace StringUtils {
template <typename Char_T>
struct WhiteSpaceChars_T {
    static constexpr Char_T SpaceChar           = ' ';
    static constexpr Char_T LineControlChar     = '\n';
    static constexpr Char_T TabControlChar      = '\t';
    static constexpr Char_T CarriageControlChar = '\r';
};

template <typename Char_T, typename Number_T = SizeT>
static Number_T Count(const Char_T *str) noexcept {
    Number_T len = 0;

    if (str != nullptr) {
        while (*str != Char_T{0}) {
            ++str;
            ++len;
        }
    }

    return len;
}

// template <typename Char_T, typename Number_T = SizeT>
// static constexpr Number_T ConstCount(const Char_T *str) noexcept {
//     const Char_T *str_2 = str;
//     Number_T      len   = 0;

//     if (str_2 != nullptr) {
//         while (*str_2 != Char_T{0}) {
//             ++str_2;
//             ++len;
//         }
//     }

//     return len;
// }

template <typename Char_T, typename Number_T>
static void TrimLeft(const Char_T *str, Number_T &offset, const Number_T end_offset) noexcept {
    using WhiteSpaceChars = WhiteSpaceChars_T<Char_T>;

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

template <typename Char_T, typename Number_T>
static void TrimRight(const Char_T *str, const Number_T offset, Number_T &end_offset) noexcept {
    using WhiteSpaceChars = WhiteSpaceChars_T<Char_T>;

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
template <typename Char_T, typename Number_T>
static void Trim(const Char_T *str, Number_T &offset, Number_T &length) noexcept {
    if (length != 0) {
        Number_T end_offset = (length + offset);
        TrimLeft(str, offset, end_offset);
        TrimRight(str, offset, end_offset);
        length = (end_offset - offset);
    }
}

template <typename Char_T>
static bool IsLess(const Char_T *left, const Char_T *right, SizeT left_length, SizeT right_length,
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

    return (orEqual & (left_length == right_length));
}

template <typename Char_T>
static bool IsGreater(const Char_T *left, const Char_T *right, SizeT left_length, SizeT right_length,
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

    return (orEqual & (left_length == right_length));
}

template <typename Char_T>
static bool IsEqual(const Char_T *left, const Char_T *right, SizeT length) noexcept {
    SizeT offset = 0;

    // if ((left != nullptr) && (right != nullptr)) {
    while ((length > offset) && (left[offset] == right[offset])) {
        ++offset;
    }
    // }

    return (length == offset);
}

template <typename Char_T>
static SizeT Hash(const Char_T *key, SizeT length) noexcept {
    constexpr SizeT highest_bit = (SizeT{1} << ((sizeof(SizeT) * 8) - SizeT{1}));

    SizeT hash   = SizeT{11};
    SizeT base   = SizeT{33};
    SizeT offset = 0;

    while (offset < length) {
        hash = (hash + (base * offset * SizeT(key[offset])));
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

template <typename Char_T>
static void ToLowerCase(Char_T *str, SizeT length) noexcept {
    const Char_T *end = (str + length);

    while (str < end) {
        if ((*str >= 'A') && (*str <= 'Z')) {
            *str += ' ';
        }

        ++str;
    }
}

template <typename, SizeT32>
struct HTMLSpecialChars_T {};

template <typename StringStream_T, typename Char_T>
static void EscapeHTMLSpecialChars(StringStream_T &stream, const Char_T *str, SizeT length) {
    using HTMLSpecialChars = HTMLSpecialChars_T<Char_T, sizeof(Char_T)>;

    if (Config::AutoEscapeHTML) {
        SizeT offset = 0;
        SizeT index  = 0;

        while (index < length) {
            switch (str[index]) {
                case '&': {
                    const SizeT   rem_length = (length - index);
                    const Char_T *n_str      = (str + index);

                    if ((rem_length > SizeT{5}) && (n_str[SizeT{5}] == HTMLSpecialChars::SemicolonChar)) {
                        if (StringUtils::IsEqual(n_str, HTMLSpecialChars::HTMLQuote, SizeT{5}) ||
                            StringUtils::IsEqual(n_str, HTMLSpecialChars::HTMLSingleQuote, SizeT{5})) {
                            index += SizeT{6};
                            break;
                        }
                    }

                    if ((rem_length > SizeT{4}) && (n_str[SizeT{4}] == HTMLSpecialChars::SemicolonChar) &&
                        StringUtils::IsEqual(n_str, HTMLSpecialChars::HTMLAnd, SizeT{4})) {
                        index += SizeT{5};
                        break;
                    }

                    if ((rem_length > SizeT{3}) && (n_str[SizeT{3}] == HTMLSpecialChars::SemicolonChar)) {
                        if (StringUtils::IsEqual(n_str, HTMLSpecialChars::HTMLLess, SizeT{3}) ||
                            StringUtils::IsEqual(n_str, HTMLSpecialChars::HTMLGreater, SizeT{3})) {
                            index += SizeT{4};
                            break;
                        }
                    }

                    stream.Write((str + offset), (index - offset));
                    stream.Write(HTMLSpecialChars::HTMLAnd, HTMLSpecialChars::HTMLAndLength);
                    ++index;
                    offset = index;
                    break;
                }

                case '<': {
                    stream.Write((str + offset), (index - offset));
                    stream.Write(HTMLSpecialChars::HTMLLess, HTMLSpecialChars::HTMLLessLength);
                    ++index;
                    offset = index;
                    break;
                }

                case '>': {
                    stream.Write((str + offset), (index - offset));
                    stream.Write(HTMLSpecialChars::HTMLGreater, HTMLSpecialChars::HTMLGreaterLength);
                    ++index;
                    offset = index;
                    break;
                }

                case '"': {
                    stream.Write((str + offset), (index - offset));
                    stream.Write(HTMLSpecialChars::HTMLQuote, HTMLSpecialChars::HTMLQuoteLength);
                    ++index;
                    offset = index;
                    break;
                }

                case '\'': {
                    stream.Write((str + offset), (index - offset));
                    stream.Write(HTMLSpecialChars::HTMLSingleQuote, HTMLSpecialChars::HTMLSingleQuoteLength);
                    ++index;
                    offset = index;
                    break;
                }

                default: {
                    ++index;
                }
            }
        }

        stream.Write((str + offset), (length - offset));
    } else {
        stream.Write(str, length);
    }
}

// char
template <typename Char_T>
struct HTMLSpecialChars_T<Char_T, 1U> {
    static constexpr const Char_T *HTMLAnd = "&amp;"; // &
    static constexpr SizeT         HTMLAndLength{5};

    static constexpr const Char_T *HTMLLess = "&lt;"; // <
    static constexpr SizeT         HTMLLessLength{4};

    static constexpr const Char_T *HTMLGreater = "&gt;"; //  >
    static constexpr SizeT         HTMLGreaterLength{4};

    static constexpr const Char_T *HTMLQuote = "&quot;"; // "
    static constexpr SizeT         HTMLQuoteLength{6};

    static constexpr const Char_T *HTMLSingleQuote = "&apos;"; // ' (HTML5)
    static constexpr SizeT         HTMLSingleQuoteLength{6};

    static constexpr const Char_T SemicolonChar = ';';
};

// char16_t
template <typename Char_T>
struct HTMLSpecialChars_T<Char_T, 2U> {
    static constexpr const Char_T *HTMLAnd = u"&amp;"; // &
    static constexpr SizeT         HTMLAndLength{5};

    static constexpr const Char_T *HTMLLess = u"&lt;"; // <
    static constexpr SizeT         HTMLLessLength{4};

    static constexpr const Char_T *HTMLGreater = u"&gt;"; //  >
    static constexpr SizeT         HTMLGreaterLength{4};

    static constexpr const Char_T *HTMLQuote = u"&quot;"; // "
    static constexpr SizeT         HTMLQuoteLength{6};

    static constexpr const Char_T *HTMLSingleQuote = u"&apos;"; // ' (HTML5)
    static constexpr SizeT         HTMLSingleQuoteLength{6};

    static constexpr const Char_T SemicolonChar = u';';
};

// char32_t
template <typename Char_T>
struct HTMLSpecialChars_T<Char_T, 4U> {
    static constexpr const Char_T *HTMLAnd = U"&amp;"; // &
    static constexpr SizeT         HTMLAndLength{5};

    static constexpr const Char_T *HTMLLess = U"&lt;"; // <
    static constexpr SizeT         HTMLLessLength{4};

    static constexpr const Char_T *HTMLGreater = U"&gt;"; //  >
    static constexpr SizeT         HTMLGreaterLength{4};

    static constexpr const Char_T *HTMLQuote = U"&quot;"; // "
    static constexpr SizeT         HTMLQuoteLength{6};

    static constexpr const Char_T *HTMLSingleQuote = U"&apos;"; // ' (HTML5)
    static constexpr SizeT         HTMLSingleQuoteLength{6};

    static constexpr const Char_T SemicolonChar = U';';
};

// wchar_t size = 4
template <>
struct HTMLSpecialChars_T<wchar_t, 4U> {
    static constexpr const wchar_t *HTMLAnd = L"&amp;"; // &
    static constexpr SizeT          HTMLAndLength{5};

    static constexpr const wchar_t *HTMLLess = L"&lt;"; // <
    static constexpr SizeT          HTMLLessLength{4};

    static constexpr const wchar_t *HTMLGreater = L"&gt;"; //  >
    static constexpr SizeT          HTMLGreaterLength{4};

    static constexpr const wchar_t *HTMLQuote = L"&quot;"; // "
    static constexpr SizeT          HTMLQuoteLength{6};

    static constexpr const wchar_t *HTMLSingleQuote = L"&apos;"; // ' (HTML5)
    static constexpr SizeT          HTMLSingleQuoteLength{6};

    static constexpr const wchar_t SemicolonChar = L';';
};

// wchar_t size = 2
template <>
struct HTMLSpecialChars_T<wchar_t, 2U> {
    static constexpr const wchar_t *HTMLAnd = L"&amp;"; // &
    static constexpr SizeT          HTMLAndLength{5};

    static constexpr const wchar_t *HTMLLess = L"&lt;"; // <
    static constexpr SizeT          HTMLLessLength{4};

    static constexpr const wchar_t *HTMLGreater = L"&gt;"; //  >
    static constexpr SizeT          HTMLGreaterLength{4};

    static constexpr const wchar_t *HTMLQuote = L"&quot;"; // "
    static constexpr SizeT          HTMLQuoteLength{6};

    static constexpr const wchar_t *HTMLSingleQuote = L"&apos;"; // ' (HTML5)
    static constexpr SizeT          HTMLSingleQuoteLength{6};

    static constexpr const wchar_t SemicolonChar = L';';
};

} // namespace StringUtils
} // namespace Qentem

#endif
