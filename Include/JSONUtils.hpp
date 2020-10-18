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

template <typename>
struct JSONotation;

template <typename Char_T_>
QENTEM_MAYBE_UNUSED static SizeT UnEscapeJSON(const Char_T_ *        content,
                                              SizeT                  length,
                                              StringStream<Char_T_> &buffer) {
    using JSONotation_T_ = JSONotation<Char_T_>;

    SizeT offset  = 0;
    SizeT offset2 = 0;

    while (offset < length) {
        switch (content[offset]) {
            case JSONotation_T_::BSlashChar: {
                if (offset > offset2) {
                    buffer.Insert((content + offset2), (offset - offset2));
                }

                ++offset;
                offset2 = (offset + 1);

                switch (content[offset]) {
                    case JSONotation_T_::QuoteChar:
                    case JSONotation_T_::BSlashChar:
                    case JSONotation_T_::SlashChar: {
                        buffer += content[offset];
                        break;
                    }

                    case JSONotation_T_::B_Char: {
                        buffer += JSONotation_T_::BackSpaceControlChar;
                        break;
                    }

                    case JSONotation_T_::F_Char: {
                        buffer += JSONotation_T_::FormfeedControlChar;
                        break;
                    }

                    case JSONotation_T_::N_Char: {
                        buffer += JSONotation_T_::LineControlChar;
                        break;
                    }
                    case JSONotation_T_::R_Char: {
                        buffer += JSONotation_T_::CarriageControlChar;
                        break;
                    }

                    case JSONotation_T_::T_Char: {
                        buffer += JSONotation_T_::TabControlChar;
                        break;
                    }

                    case JSONotation_T_::CU_Char:
                    case JSONotation_T_::U_Char: {
                        ++offset;

                        if ((length - offset) > 3) {
                            unsigned int code =
                                Digit<Char_T_>::HexStringToNumber(
                                    (content + offset), 4);
                            offset += 4;
                            offset2 = offset;

                            if ((code >> 8U) != 0xD8U) {
                                Unicode::ToUTF(code, buffer);
                                continue;
                            }

                            // Surrogate
                            if ((length - offset) > 5) {
                                code = (code ^ 0xD800U) << 10U;
                                offset += 2;

                                code += Digit<Char_T_>::HexStringToNumber(
                                            (content + offset), 4) &
                                        0x3FFU;
                                code += 0x10000U;

                                Unicode::ToUTF(code, buffer);

                                offset += 4;
                                offset2 = offset;
                                continue;
                            }
                        }

                        return 0;
                    }

                    default: {
                        return 0;
                    }
                }

                break;
            }

            case JSONotation_T_::QuoteChar: {
                if (buffer.IsNotEmpty()) {
                    buffer.Insert((content + offset2), (offset - offset2));
                }

                ++offset;
                return offset;
            }

            case JSONotation_T_::LineControlChar:
            case JSONotation_T_::TabControlChar:
            case JSONotation_T_::CarriageControlChar: {
                return 0;
            }
        }

        ++offset;
    }

    if (buffer.IsNotEmpty()) {
        buffer.Insert((content + offset2), (offset - offset2));
    }

    return offset;
}

template <typename Char_T_>
static void EscapeJSON(const Char_T_ *content, SizeT length,
                       StringStream<Char_T_> &buffer) {
    using JSONotation_T_ = JSONotation<Char_T_>;

    SizeT offset  = 0;
    SizeT offset2 = 0;

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
                    buffer.Insert((content + offset2), (offset - offset2));
                }

                offset2 = offset + 1;

                switch (content[offset]) {
                    case JSONotation_T_::QuoteChar: {
                        buffer.Insert(JSONotation_T_::GetEscapedQuote(), 2);
                        break;
                    }

                    case JSONotation_T_::BSlashChar: {
                        buffer.Insert(JSONotation_T_::GetEscapedBSlash(), 2);
                        break;
                    }

                    case JSONotation_T_::SlashChar: {
                        buffer.Insert(JSONotation_T_::GetEscapedSlash(), 2);
                        break;
                    }

                    case JSONotation_T_::BackSpaceControlChar: {
                        buffer.Insert(JSONotation_T_::GetEscapedBackSpace(), 2);
                        break;
                    }

                    case JSONotation_T_::FormfeedControlChar: {
                        buffer.Insert(JSONotation_T_::GetEscapedFormfeed(), 2);
                        break;
                    }

                    case JSONotation_T_::LineControlChar: {
                        buffer.Insert(JSONotation_T_::GetEscapedLine(), 2);
                        break;
                    }

                    case JSONotation_T_::CarriageControlChar: {
                        buffer.Insert(JSONotation_T_::GetEscapedCarriage(), 2);
                        break;
                    }

                    case JSONotation_T_::TabControlChar: {
                        buffer.Insert(JSONotation_T_::GetEscapedTab(), 2);
                        break;
                    }
                }

                break;
            }
        }

        ++offset;
    }

    buffer.Insert((content + offset2), (offset - offset2));
}

template <typename Char_T_>
struct JSONotation {
    static constexpr Char_T_ QuoteChar   = '"';
    static constexpr Char_T_ CommaChar   = ',';
    static constexpr Char_T_ ColonChar   = ':';
    static constexpr Char_T_ SCurlyChar  = '{'; // Start
    static constexpr Char_T_ ECurlyChar  = '}'; // End
    static constexpr Char_T_ SSquareChar = '['; // Start
    static constexpr Char_T_ ESquareChar = ']'; // End
    static constexpr Char_T_ SlashChar   = '/';
    static constexpr Char_T_ BSlashChar  = '\\';
    static constexpr Char_T_ SpaceChar   = ' ';
    static constexpr Char_T_ DotChar     = '.';

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
    static constexpr Char_T_ E_Char  = 'e';
    static constexpr Char_T_ UE_Char = 'E';

    static constexpr SizeT TrueStringLength = 4;
    static const Char_T_ * GetTrueString() noexcept {
        static constexpr const Char_T_ val[] = {'t', 'r', 'u', 'e'};
        return &(val[0]);
    }

    static constexpr SizeT FalseStringLength = 5;
    static const Char_T_ * GetFalseString() noexcept {
        static constexpr const Char_T_ val[] = {'f', 'a', 'l', 's', 'e'};
        return &(val[0]);
    }

    static constexpr SizeT NullStringLength = 4;
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

} // namespace JSON
} // namespace Qentem

#endif
