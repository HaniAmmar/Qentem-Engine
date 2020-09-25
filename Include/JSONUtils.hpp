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

#include "Digit.hpp"
#include "StringStream.hpp"
#include "Unicode.hpp"

#ifndef QENTEM_JSONUTILS_H_
#define QENTEM_JSONUTILS_H_

namespace Qentem {
namespace JSON {

template <typename Char_T_, ULong S = sizeof(Char_T_)>
struct JSONotation {};

// One byte character.
template <typename Char_T_>
struct JSONotation<Char_T_, 1> {
    static constexpr Char_T_ QuoteChar   = '"';
    static constexpr Char_T_ CommaChar   = ',';
    static constexpr Char_T_ ColonChar   = ':';
    static constexpr Char_T_ OCurlyChar  = '{';
    static constexpr Char_T_ CCurlyChar  = '}';
    static constexpr Char_T_ OSquareChar = '[';
    static constexpr Char_T_ CSquareChar = ']';
    static constexpr Char_T_ SlashChar   = '/';
    static constexpr Char_T_ BSlashChar  = '\\';
    static constexpr Char_T_ SpaceChar   = ' ';

    static constexpr Char_T_ LineControlChar      = '\n';
    static constexpr Char_T_ TabControlChar       = '\t';
    static constexpr Char_T_ CarriageControlChar  = '\r';
    static constexpr Char_T_ BackSpaceControlChar = '\b';
    static constexpr Char_T_ FormfeedControlChar  = '\f';

    static constexpr Char_T_ N_Char  = 'n';
    static constexpr Char_T_ T_Char  = 't';
    static constexpr Char_T_ R_Char  = 'r';
    static constexpr Char_T_ B_Char  = 'b';
    static constexpr Char_T_ F_Char  = 'f';
    static constexpr Char_T_ U_Char  = 'u';
    static constexpr Char_T_ CU_Char = 'U';

    static constexpr ULong TrueStringLength = 4;
    static const Char_T_ * GetTrueString() noexcept {
        static constexpr const Char_T_ val[] = {'t', 'r', 'u', 'e'};
        return &(val[0]);
    }

    static constexpr ULong FalseStringLength = 5;
    static const Char_T_ * GetFalseString() noexcept {
        static constexpr const Char_T_ val[] = {'f', 'a', 'l', 's', 'e'};
        return &(val[0]);
    }

    static constexpr ULong NullStringLength = 4;
    static const Char_T_ * GetNullString() noexcept {
        static constexpr const Char_T_ val[] = {'n', 'u', 'l', 'l'};
        return &(val[0]);
    }

    ////////// Escaped //////////

    static const Char_T_ *GetEscapedQuote() noexcept {
        static constexpr Char_T_ val[] = {'\\', '"'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedBSlash() noexcept {
        static constexpr Char_T_ val[] = {'\\', '\\'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedSlash() noexcept {
        static constexpr Char_T_ val[] = {'\\', '/'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedLine() noexcept {
        static constexpr Char_T_ val[] = {'\\', 'n'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedTab() noexcept {
        static constexpr Char_T_ val[] = {'\\', 't'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedCarriage() noexcept {
        static constexpr Char_T_ val[] = {'\\', 'r'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedBackSpace() noexcept {
        static constexpr Char_T_ val[] = {'\\', 'b'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedFormfeed() noexcept {
        static constexpr Char_T_ val[] = {'\\', 'f'};
        return &(val[0]);
    }
};

// Two bytes character.
template <typename Char_T_>
struct JSONotation<Char_T_, 2> {
    static constexpr Char_T_ QuoteChar   = u'"';
    static constexpr Char_T_ CommaChar   = u',';
    static constexpr Char_T_ ColonChar   = u':';
    static constexpr Char_T_ OCurlyChar  = u'{';
    static constexpr Char_T_ CCurlyChar  = u'}';
    static constexpr Char_T_ OSquareChar = u'[';
    static constexpr Char_T_ CSquareChar = u']';
    static constexpr Char_T_ SlashChar   = u'/';
    static constexpr Char_T_ BSlashChar  = u'\\';
    static constexpr Char_T_ SpaceChar   = u' ';

    static constexpr Char_T_ LineControlChar      = u'\n';
    static constexpr Char_T_ TabControlChar       = u'\t';
    static constexpr Char_T_ CarriageControlChar  = u'\r';
    static constexpr Char_T_ BackSpaceControlChar = u'\b';
    static constexpr Char_T_ FormfeedControlChar  = u'\f';

    static constexpr Char_T_ N_Char  = u'n';
    static constexpr Char_T_ T_Char  = u't';
    static constexpr Char_T_ R_Char  = u'r';
    static constexpr Char_T_ B_Char  = u'b';
    static constexpr Char_T_ F_Char  = u'f';
    static constexpr Char_T_ U_Char  = u'u';
    static constexpr Char_T_ CU_Char = u'U';

    static constexpr ULong TrueStringLength = 4;
    static const Char_T_ * GetTrueString() noexcept {
        static constexpr Char_T_ val[] = {u't', u'r', u'u', u'e'};
        return &(val[0]);
    }

    static constexpr ULong FalseStringLength = 5;
    static const Char_T_ * GetFalseString() noexcept {
        static constexpr Char_T_ val[] = {u'f', u'a', u'l', u's', u'e'};
        return &(val[0]);
    }

    static constexpr ULong NullStringLength = 4;
    static const Char_T_ * GetNullString() noexcept {
        static constexpr Char_T_ val[] = {u'n', u'u', u'l', u'l'};
        return &(val[0]);
    }

    ////////// Escaped //////////

    static const Char_T_ *GetEscapedQuote() noexcept {
        static constexpr Char_T_ val[] = {u'\\', u'"'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedBSlash() noexcept {
        static constexpr Char_T_ val[] = {u'\\', u'\\'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedSlash() noexcept {
        static constexpr Char_T_ val[] = {u'\\', u'/'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedLine() noexcept {
        static constexpr Char_T_ val[] = {u'\\', u'n'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedTab() noexcept {
        static constexpr Char_T_ val[] = {u'\\', u't'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedCarriage() noexcept {
        static constexpr Char_T_ val[] = {u'\\', u'r'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedBackSpace() noexcept {
        static constexpr Char_T_ val[] = {u'\\', u'b'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedFormfeed() noexcept {
        static constexpr Char_T_ val[] = {u'\\', u'f'};
        return &(val[0]);
    }
};

// Four bytes character.
template <typename Char_T_>
struct JSONotation<Char_T_, 4> {
    static constexpr Char_T_ QuoteChar   = U'"';
    static constexpr Char_T_ CommaChar   = U',';
    static constexpr Char_T_ ColonChar   = U':';
    static constexpr Char_T_ OCurlyChar  = U'{';
    static constexpr Char_T_ CCurlyChar  = U'}';
    static constexpr Char_T_ OSquareChar = U'[';
    static constexpr Char_T_ CSquareChar = U']';
    static constexpr Char_T_ SlashChar   = U'/';
    static constexpr Char_T_ BSlashChar  = U'\\';
    static constexpr Char_T_ SpaceChar   = U' ';

    static constexpr Char_T_ LineControlChar      = U'\n';
    static constexpr Char_T_ TabControlChar       = U'\t';
    static constexpr Char_T_ CarriageControlChar  = U'\r';
    static constexpr Char_T_ BackSpaceControlChar = U'\b';
    static constexpr Char_T_ FormfeedControlChar  = U'\f';

    static constexpr Char_T_ N_Char  = U'n';
    static constexpr Char_T_ T_Char  = U't';
    static constexpr Char_T_ R_Char  = U'r';
    static constexpr Char_T_ B_Char  = U'b';
    static constexpr Char_T_ F_Char  = U'f';
    static constexpr Char_T_ U_Char  = U'u';
    static constexpr Char_T_ CU_Char = U'U';

    static constexpr ULong TrueStringLength = 4;
    static const Char_T_ * GetTrueString() noexcept {
        static constexpr Char_T_ val[] = {U't', U'r', U'u', U'e'};
        return &(val[0]);
    }

    static constexpr ULong FalseStringLength = 5;
    static const Char_T_ * GetFalseString() noexcept {
        static constexpr Char_T_ val[] = {U'f', U'a', U'l', U's', U'e'};
        return &(val[0]);
    }

    static constexpr ULong NullStringLength = 4;
    static const Char_T_ * GetNullString() noexcept {
        static constexpr Char_T_ val[] = {U'n', U'u', U'l', U'l'};
        return &(val[0]);
    }

    ////////// Escaped //////////

    static const Char_T_ *GetEscapedQuote() noexcept {
        static constexpr Char_T_ val[] = {U'\\', U'"'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedBSlash() noexcept {
        static constexpr Char_T_ val[] = {U'\\', U'\\'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedSlash() noexcept {
        static constexpr Char_T_ val[] = {U'\\', U'/'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedLine() noexcept {
        static constexpr Char_T_ val[] = {U'\\', U'n'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedTab() noexcept {
        static constexpr Char_T_ val[] = {U'\\', U't'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedCarriage() noexcept {
        static constexpr Char_T_ val[] = {U'\\', U'r'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedBackSpace() noexcept {
        static constexpr Char_T_ val[] = {U'\\', U'b'};
        return &(val[0]);
    }

    static const Char_T_ *GetEscapedFormfeed() noexcept {
        static constexpr Char_T_ val[] = {U'\\', U'f'};
        return &(val[0]);
    }
};

// wchar_t
template <>
struct JSONotation<wchar_t> {
    static constexpr wchar_t QuoteChar   = L'"';
    static constexpr wchar_t CommaChar   = L',';
    static constexpr wchar_t ColonChar   = L':';
    static constexpr wchar_t OCurlyChar  = L'{';
    static constexpr wchar_t CCurlyChar  = L'}';
    static constexpr wchar_t OSquareChar = L'[';
    static constexpr wchar_t CSquareChar = L']';
    static constexpr wchar_t SlashChar   = L'/';
    static constexpr wchar_t BSlashChar  = L'\\';
    static constexpr wchar_t SpaceChar   = L' ';

    static constexpr wchar_t LineControlChar      = L'\n';
    static constexpr wchar_t TabControlChar       = L'\t';
    static constexpr wchar_t CarriageControlChar  = L'\r';
    static constexpr wchar_t BackSpaceControlChar = L'\b';
    static constexpr wchar_t FormfeedControlChar  = L'\f';

    static constexpr wchar_t N_Char  = L'n';
    static constexpr wchar_t T_Char  = L't';
    static constexpr wchar_t R_Char  = L'r';
    static constexpr wchar_t B_Char  = L'b';
    static constexpr wchar_t F_Char  = L'f';
    static constexpr wchar_t U_Char  = L'u';
    static constexpr wchar_t CU_Char = L'U';

    static constexpr ULong TrueStringLength = 4;
    static const wchar_t * GetTrueString() noexcept {
        static const wchar_t val[] = {L't', L'r', L'u', L'e'};
        return &(val[0]);
    }

    static constexpr ULong FalseStringLength = 5;
    static const wchar_t * GetFalseString() noexcept {
        static const wchar_t val[] = {L'f', L'a', L'l', L's', L'e'};
        return &(val[0]);
    }

    static constexpr ULong NullStringLength = 4;
    static const wchar_t * GetNullString() noexcept {
        static const wchar_t val[] = {L'n', L'u', L'l', L'l'};
        return &(val[0]);
    }

    ////////// Escaped //////////

    static const wchar_t *GetEscapedQuote() noexcept {
        static const wchar_t val[] = {L'\\', L'"'};
        return &(val[0]);
    }

    static const wchar_t *GetEscapedBSlash() noexcept {
        static const wchar_t val[] = {L'\\', L'\\'};
        return &(val[0]);
    }

    static const wchar_t *GetEscapedSlash() noexcept {
        static const wchar_t val[] = {L'\\', L'/'};
        return &(val[0]);
    }

    static const wchar_t *GetEscapedLine() noexcept {
        static const wchar_t val[] = {L'\\', L'n'};
        return &(val[0]);
    }

    static const wchar_t *GetEscapedTab() noexcept {
        static const wchar_t val[] = {L'\\', L't'};
        return &(val[0]);
    }

    static const wchar_t *GetEscapedCarriage() noexcept {
        static const wchar_t val[] = {L'\\', L'r'};
        return &(val[0]);
    }

    static const wchar_t *GetEscapedBackSpace() noexcept {
        static const wchar_t val[] = {L'\\', L'b'};
        return &(val[0]);
    }

    static const wchar_t *GetEscapedFormfeed() noexcept {
        static const wchar_t val[] = {L'\\', L'f'};
        return &(val[0]);
    }
};

#ifdef QENTEM_SIMD_ENABLED_
// TODO: Add 16 and 32-bit character
QENTEM_MAYBE_UNUSED_
static char *UnEscapeJSON(const char *content, ULong &length) {
    using JSONotation_T_ = JSONotation<char>;

    constexpr unsigned long long line       = 723401728380766730ULL;
    constexpr unsigned long long tab        = 651061555542690057ULL;
    constexpr unsigned long long carriage   = 940422246894996749ULL;
    constexpr unsigned long long back_slash = 6655295901103053916ULL;

    char *str      = HAllocator::Allocate<char>(length + 1);
    ULong offset   = 0;
    ULong offset2  = 0;
    ULong m_offset = 0;
    ULong length2;

    if (length != 0) {
        QMM_Number_T bits;

        do {
            const QMM_VAR_ m_content = QMM_LOAD_(
                reinterpret_cast<const QMM_VAR_ *>(content + m_offset));
            bits = QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(line));
            bits |= QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(tab));
            bits |= QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(carriage));

            if ((bits != 0) && ((Platform::CTZ(bits) + m_offset) < length)) {
                HAllocator::Deallocate(str);
                return nullptr;
            }

            bits = QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(back_slash));

            while (bits != 0) {
                const ULong index = (Platform::CTZ(bits) + m_offset);

                if (index >= length) {
                    break;
                }

                length2 = (index - offset);

                if (length2 != 0) {
                    Memory::Copy<char>((str + offset2), (content + offset),
                                       length2);
                    offset2 += length2;
                }

                offset = index;
                ++offset;

                switch (content[offset]) {
                    case JSONotation_T_::BSlashChar: {
                        str[offset2] = content[offset];
                        bits &= (bits - 1);
                        break;
                    }

                    case JSONotation_T_::QuoteChar:
                    case JSONotation_T_::SlashChar: {
                        str[offset2] = content[offset];
                        break;
                    }

                    case JSONotation_T_::B_Char: {
                        str[offset2] = JSONotation_T_::BackSpaceControlChar;
                        break;
                    }

                    case JSONotation_T_::F_Char: {
                        str[offset2] = JSONotation_T_::FormfeedControlChar;
                        break;
                    }

                    case JSONotation_T_::N_Char: {
                        str[offset2] = JSONotation_T_::LineControlChar;
                        break;
                    }
                    case JSONotation_T_::R_Char: {
                        str[offset2] = JSONotation_T_::CarriageControlChar;
                        break;
                    }

                    case JSONotation_T_::T_Char: {
                        str[offset2] = JSONotation_T_::TabControlChar;
                        break;
                    }

                    case JSONotation_T_::CU_Char:
                    case JSONotation_T_::U_Char: {
                        ++offset;

                        if ((length - offset) < 4U) {
                            HAllocator::Deallocate(str);
                            return nullptr;
                        }

                        UInt code = Digit<char>::HexStringToNumber(
                            (content + offset), 4U);
                        offset += 4U;

                        if ((code >> 8U) == 0xD8U) {
                            // Surrogate
                            if ((length - offset) < 6U) {
                                HAllocator::Deallocate(str);
                                return nullptr;
                            }

                            code = (code ^ 0xD800U) << 10U;
                            offset += 2U;
                            code += Digit<char>::HexStringToNumber(
                                        (content + offset), 4U) &
                                    0x3FFU;
                            code += 0x10000U;

                            offset2 += Unicode::ToUTF(code, (str + offset2));
                            offset += 4U;
                            bits &= (bits - 1U);
                        } else {
                            offset2 += Unicode::ToUTF(code, (str + offset2));
                        }

                        bits &= (bits - 1U);
                        continue;
                    }

                    default: {
                        HAllocator::Deallocate(str);
                        return nullptr;
                    }
                }

                ++offset;
                ++offset2;

                bits &= (bits - 1);
            }

            m_offset += QMM_SIZE_;

            if (m_offset < offset) {
                m_offset = offset;
            }
        } while (m_offset < length);
    }

    length2 = (length - offset);

    if (length2 != 0) {
        Memory::Copy<char>((str + offset2), (content + offset), length2);
        offset2 += length2;
    }

    str[offset2] = 0;
    length       = offset2;

    return str;
}
#else
template <typename Char_T_>
QENTEM_MAYBE_UNUSED_ static Char_T_ *UnEscapeJSON(const Char_T_ *content,
                                                  ULong &        length) {
    using JSONotation_T_ = JSONotation<Char_T_>;

    UInt     offset  = 0;
    UInt     offset2 = 0;
    Char_T_ *str     = HAllocator::Allocate<Char_T_>(length + 1);

    while (offset < length) {
        switch (content[offset]) {
            case JSONotation_T_::BSlashChar: {
                ++offset;

                switch (content[offset]) {
                    case JSONotation_T_::QuoteChar:
                    case JSONotation_T_::BSlashChar:
                    case JSONotation_T_::SlashChar: {
                        str[offset2] = content[offset];
                        break;
                    }

                    case JSONotation_T_::B_Char: {
                        str[offset2] = JSONotation_T_::BackSpaceControlChar;
                        break;
                    }

                    case JSONotation_T_::F_Char: {
                        str[offset2] = JSONotation_T_::FormfeedControlChar;
                        break;
                    }

                    case JSONotation_T_::N_Char: {
                        str[offset2] = JSONotation_T_::LineControlChar;
                        break;
                    }
                    case JSONotation_T_::R_Char: {
                        str[offset2] = JSONotation_T_::CarriageControlChar;
                        break;
                    }

                    case JSONotation_T_::T_Char: {
                        str[offset2] = JSONotation_T_::TabControlChar;
                        break;
                    }

                    case JSONotation_T_::CU_Char:
                    case JSONotation_T_::U_Char: {
                        ++offset;

                        if ((length - offset) < 4) {
                            HAllocator::Deallocate(str);
                            return nullptr;
                        }

                        UInt code = Digit<Char_T_>::HexStringToNumber(
                            (content + offset), 4);
                        offset += 4;

                        if ((code >> 8U) == 0xD8U) {
                            // Surrogate
                            if ((length - offset) < 6U) {
                                HAllocator::Deallocate(str);
                                return nullptr;
                            }

                            code = (code ^ 0xD800U) << 10U;
                            offset += 2;
                            code += Digit<Char_T_>::HexStringToNumber(
                                        (content + offset), 4) &
                                    0x3FFU;
                            code += 0x10000U;

                            offset2 += Unicode::ToUTF(code, (str + offset2));
                            offset += 4;
                        } else {
                            offset2 += Unicode::ToUTF(code, (str + offset2));
                        }

                        continue;
                    }

                    default: {
                        HAllocator::Deallocate(str);
                        return nullptr;
                    }
                }

                break;
            }

            case JSONotation_T_::LineControlChar:
            case JSONotation_T_::TabControlChar:
            case JSONotation_T_::CarriageControlChar: {
                HAllocator::Deallocate(str);
                return nullptr;
            }

            default: {
                str[offset2] = content[offset];
            }
        }

        ++offset;
        ++offset2;
    }

    str[offset2] = 0;
    length       = offset2;

    return str;
}
#endif

template <typename Char_T_>
static void EscapeJSON(const Char_T_ *content, ULong length,
                       StringStream<Char_T_> &ss) {
    using JSONotation_T_ = JSONotation<Char_T_>;

    ULong offset  = 0;
    ULong offset2 = 0;

    while (offset < length) {
        switch (content[offset]) {
            case JSONotation_T_::QuoteChar:
            case JSONotation_T_::BSlashChar:
            case JSONotation_T_::SlashChar:
            case JSONotation_T_::BackSpaceControlChar:
            case JSONotation_T_::FormfeedControlChar:
            case JSONotation_T_::LineControlChar:
            case JSONotation_T_::CarriageControlChar:
            case JSONotation_T_::TabControlChar: {
                if (offset > offset2) {
                    ss.Insert((content + offset2), (offset - offset2));
                }

                offset2 = offset + 1;

                switch (content[offset]) {
                    case JSONotation_T_::QuoteChar: {
                        ss.Insert(JSONotation_T_::GetEscapedQuote(), 2);
                        break;
                    }

                    case JSONotation_T_::BSlashChar: {
                        ss.Insert(JSONotation_T_::GetEscapedBSlash(), 2);
                        break;
                    }

                    case JSONotation_T_::SlashChar: {
                        ss.Insert(JSONotation_T_::GetEscapedSlash(), 2);
                        break;
                    }

                    case JSONotation_T_::BackSpaceControlChar: {
                        ss.Insert(JSONotation_T_::GetEscapedBackSpace(), 2);
                        break;
                    }

                    case JSONotation_T_::FormfeedControlChar: {
                        ss.Insert(JSONotation_T_::GetEscapedFormfeed(), 2);
                        break;
                    }

                    case JSONotation_T_::LineControlChar: {
                        ss.Insert(JSONotation_T_::GetEscapedLine(), 2);
                        break;
                    }

                    case JSONotation_T_::CarriageControlChar: {
                        ss.Insert(JSONotation_T_::GetEscapedCarriage(), 2);
                        break;
                    }

                    case JSONotation_T_::TabControlChar: {
                        ss.Insert(JSONotation_T_::GetEscapedTab(), 2);
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            default: {
            }
        }

        ++offset;
    }

    if (offset > offset2) {
        ss.Insert((content + offset2), (offset - offset2));
    }
}

} // namespace JSON
} // namespace Qentem

#endif
