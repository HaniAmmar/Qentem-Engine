/**
 * @file StringUtils.hpp
 * @brief Provides string manipulation utilities for Qentem Engine.
 *
 * This header contains a collection of functions and utilities for common
 * string operations, such as trimming, searching, splitting, and case conversion.
 * supporting the internal needs of the Qentem Engine library.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_STRING_UTILS_H
#define QENTEM_STRING_UTILS_H

#include "QCommon.hpp"

namespace Qentem {

template <typename, SizeT32>
struct HTMLSpecialChars_T {};

struct StringUtils {
    template <typename Char_T>
    struct WhiteSpaceChars_T {
        static constexpr Char_T SpaceChar           = ' ';
        static constexpr Char_T LineControlChar     = '\n';
        static constexpr Char_T TabControlChar      = '\t';
        static constexpr Char_T CarriageControlChar = '\r';
    };

    template <typename Char_T, typename Number_T = SizeT>
    static QENTEM_CONST_EXPRESSION Number_T Count(const Char_T *str) noexcept {
        Number_T length = 0;

        if (str != nullptr) {
            while (*str != Char_T{0}) {
                ++str;
                ++length;
            }
        }

        return length;
    }

    template <typename Char_T, typename Number_T>
    QENTEM_INLINE static void Write(Char_T *&to, const Char_T *from, Number_T length) noexcept {
        while (length != 0) {
            --length;
            *to = *from;
            ++to;
            ++from;
        }
    }

    template <typename Char_T, typename Number_T>
    QENTEM_INLINE static void Reverse(Char_T *str, Number_T start, Number_T length) noexcept {
        while (start < length) {
            const Char_T tmp = str[start];

            --length;
            str[start]  = str[length];
            str[length] = tmp;
            ++start;
        }
    }

    template <typename Char_T, typename Number_T>
    QENTEM_INLINE static void Reverse(Char_T *str, Number_T offset, Number_T end, Number_T limit) noexcept {
        while ((offset < end) && (limit != 0)) {
            const Char_T tmp = str[offset];

            --end;
            str[offset] = str[end];
            str[end]    = tmp;
            ++offset;
            --limit;
        }
    }

    template <typename Stream_T>
    static void InsertAt(Stream_T &stream, typename Stream_T::CharType ch, SizeT index) {
        if (index < stream.Length()) {
            stream.Write('\0');

            typename Stream_T::CharType *storage = stream.Storage();

            // Shift right: move everything [index, length) -> [index+1, new_length)
            SizeT offset = stream.Length() - 1;

            while (offset > index) {
                storage[offset] = storage[offset - SizeT{1}];
                --offset;
            }

            storage[index] = ch;
        }
    }

    template <typename Char_T, typename Number_T>
    QENTEM_INLINE static void TrimLeft(const Char_T *str, Number_T &offset, const Number_T end_offset) noexcept {
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
    QENTEM_INLINE static void TrimRight(const Char_T *str, const Number_T offset, Number_T &end_offset) noexcept {
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
    QENTEM_INLINE static bool IsEqual(const Char_T *left, const Char_T *right, SizeT length) noexcept {
        SizeT offset = 0;

        // if ((left != nullptr) && (right != nullptr)) {
        while ((length > offset) && (left[offset] == right[offset])) {
            ++offset;
        }
        // }

        return (length == offset);
    }

    template <typename Char_T>
    static SizeT Hash(const Char_T *str, SizeT length) noexcept {
        constexpr SizeT highest_bit = (SizeT{1} << ((sizeof(SizeT) * 8) - SizeT{1}));

        SizeT hash   = SizeT{11};
        SizeT base   = SizeT{33};
        SizeT offset = 0;

        while (offset < length) {
            hash += SizeT(base * offset * SizeT(str[offset]));
            base += offset;

            if (offset != length) {
                hash *= (length ^ offset);
                base += offset;
                --length;
                hash += SizeT(str[length]);
            }

            ++offset;
        }

        return (hash | highest_bit);
    }

    template <typename Char_T>
    QENTEM_INLINE static void ToLowerCase(Char_T *str, SizeT length) noexcept {
        const Char_T *end = (str + length);

        while (str < end) {
            if ((*str >= 'A') && (*str <= 'Z')) {
                *str += ' ';
            }

            ++str;
        }
    }

    template <typename StringStream_T, typename Char_T>
    static void EscapeHTMLSpecialChars(StringStream_T &stream, const Char_T *str, SizeT length) {
        using HTMLSpecialChars = HTMLSpecialChars_T<Char_T, sizeof(Char_T)>;

        if (QentemConfig::AutoEscapeHTML) {
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

    /**
     * @brief Replaces C/C++ style inline (`//`) and block (`/ * ... * /`) comments with whitespace,
     *        preserving string literals and original buffer structure.
     *
     * This function operates directly on a raw character buffer, scanning it in-place and replacing
     * all comment content with space (`' '`) or newline (`'\n'`). The goal is to preserve the text's
     * structure, line count, and character offsets without reserving new memory or altering length.
     *
     * String literals are preserved, including escaped quotes (e.g., `\"`), and comments within
     * them are ignored. Block comments that are unterminated at the end of the buffer are masked
     * until the buffer’s end without throwing errors or escaping bounds.
     *
     * @tparam Char_T Character type (typically `char` or `wchar_t`).
     * @param str     Pointer to the character buffer.
     * @param length  Total length of the buffer to scan.
     */
    template <typename Char_T>
    static void StripComments(Char_T *str, SizeT length) {
        SizeT offset      = 0;     // Current scan position
        bool  inside_text = false; // True if currently inside a string literal

        // Scan through the buffer
        while (offset < length) {
            // Advance to the next possible comment or quote character
            while ((offset < length) && (str[offset] != '/') && (str[offset] != '"')) {
                ++offset;
            }

            SizeT tmp = offset;
            ++offset; // Move past the found '/' or '"' for analysis

            if (offset < length) {
                if (str[tmp] == '"') {
                    // Handle string literals: check for escaped quote
                    SizeT escapes = 0;

                    while ((tmp != 0) && (str[--tmp] == '\\')) {
                        ++escapes;
                    }

                    // Toggle inside_text only if quote is not escaped
                    if (((escapes & SizeT{1}) == 0)) {
                        inside_text = !inside_text;
                    }
                } else if (!inside_text) {
                    // Only consider comments outside of string literals
                    if (offset != length) {
                        if ((str[offset] == '/') || (str[offset] == '*')) {
                            str[offset - SizeT{1}] = ' ';

                            if (str[offset] == '/') {
                                str[offset] = ' ';
                                // Inline comment: skip to end of line
                                while ((++offset < length) && (str[offset] != '\n')) {
                                    str[offset] = ' ';
                                }
                            } else {
                                str[offset] = ' ';

                                // Block comment: skip to closing '*/' or EOF
                                while (true) {
                                    while ((++offset < length) && (str[offset] != '*')) {
                                        if (str[offset] != '\n') { // preserve newlines
                                            str[offset] = ' ';
                                        }
                                    }

                                    if (offset < length) {
                                        str[offset] = ' ';
                                        ++offset;

                                        if ((offset < length) && (str[offset] == '/')) {
                                            str[offset] = ' ';
                                            break;
                                        }
                                    } else {
                                        break;
                                    }
                                }

                                ++offset; // Move past '/'
                            }
                        }
                    }
                }
            }
        }
    }
};

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

} // namespace Qentem

#endif
