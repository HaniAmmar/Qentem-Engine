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

#include "Digit.hpp"
#include "Unicode.hpp"

#ifndef QENTEM_JSON_UTILS_H_
#define QENTEM_JSON_UTILS_H_

namespace Qentem {

template <typename>
struct JSONotation_T_;

template <typename Char_T_, typename Stream_T_>
static SizeT UnEscapeJSON(const Char_T_ *content, SizeT length, Stream_T_ &stream) {
    using JSONotation = JSONotation_T_<Char_T_>;

    SizeT offset  = 0;
    SizeT offset2 = 0;

    while (offset < length) {
        switch (content[offset]) {
            case JSONotation::BSlashChar: {
                if (offset > offset2) {
                    stream.Write((content + offset2), (offset - offset2));
                }

                ++offset;
                offset2 = (offset + 1);

                switch (content[offset]) {
                    case JSONotation::QuoteChar:
                    case JSONotation::BSlashChar:
                    case JSONotation::SlashChar: {
                        stream += content[offset];
                        break;
                    }

                    case JSONotation::B_Char: {
                        stream += JSONotation::BackSpaceControlChar;
                        break;
                    }

                    case JSONotation::F_Char: {
                        stream += JSONotation::FormfeedControlChar;
                        break;
                    }

                    case JSONotation::N_Char: {
                        stream += JSONotation::LineControlChar;
                        break;
                    }
                    case JSONotation::R_Char: {
                        stream += JSONotation::CarriageControlChar;
                        break;
                    }

                    case JSONotation::T_Char: {
                        stream += JSONotation::TabControlChar;
                        break;
                    }

                    case JSONotation::CU_Char:
                    case JSONotation::U_Char: {
                        ++offset;

                        if ((length - offset) > 3) {
                            SizeT32 code = Digit::HexStringToNumber((content + offset), 4);
                            offset += 4;
                            offset2 = offset;

                            if ((code >> 8U) != 0xD8U) {
                                Unicode::ToUTF<Char_T_>(code, stream);
                                continue;
                            }

                            // Surrogate
                            if ((length - offset) > 5) {
                                code = (code ^ 0xD800U) << 10U;
                                offset += SizeT{2};

                                code += Digit::HexStringToNumber((content + offset), 4) & 0x3FFU;
                                code += 0x10000U;

                                Unicode::ToUTF<Char_T_>(code, stream);

                                offset += SizeT{4};
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

            case JSONotation::QuoteChar: {
                if (stream.IsNotEmpty()) {
                    stream.Write((content + offset2), (offset - offset2));
                }

                ++offset;
                return offset;
            }

            case JSONotation::LineControlChar:
            case JSONotation::TabControlChar:
            case JSONotation::CarriageControlChar: {
                return 0;
            }

            default: {
            }
        }

        ++offset;
    }

    if (stream.IsNotEmpty()) {
        stream.Write((content + offset2), (offset - offset2));
    }

    return offset;
}

template <typename Char_T_, typename Stream_T_>
static void EscapeJSON(const Char_T_ *content, SizeT length, Stream_T_ &stream) {
    using JSONotation = JSONotation_T_<Char_T_>;

    SizeT offset  = 0;
    SizeT offset2 = 0;

    while (offset < length) {
        switch (content[offset]) {
            case JSONotation::QuoteChar:
            case JSONotation::BSlashChar:
            case JSONotation::SlashChar:
            case JSONotation::BackSpaceControlChar:
            case JSONotation::FormfeedControlChar:
            case JSONotation::LineControlChar:
            case JSONotation::CarriageControlChar:
            case JSONotation::TabControlChar: {
                if (offset > offset2) {
                    stream.Write((content + offset2), (offset - offset2));
                }

                stream += JSONotation::BSlashChar;
                offset2 = offset + 1;

                switch (content[offset]) {
                    case JSONotation::QuoteChar: {
                        stream += JSONotation::QuoteChar;
                        break;
                    }

                    case JSONotation::BSlashChar: {
                        stream += JSONotation::BSlashChar;
                        break;
                    }

                    case JSONotation::SlashChar: {
                        stream += JSONotation::SlashChar;
                        break;
                    }

                    case JSONotation::BackSpaceControlChar: {
                        stream += JSONotation::B_Char;
                        break;
                    }

                    case JSONotation::FormfeedControlChar: {
                        stream += JSONotation::F_Char;
                        break;
                    }

                    case JSONotation::LineControlChar: {
                        stream += JSONotation::N_Char;
                        break;
                    }

                    case JSONotation::CarriageControlChar: {
                        stream += JSONotation::R_Char;
                        break;
                    }

                    case JSONotation::TabControlChar: {
                        stream += JSONotation::T_Char;
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

    stream.Write((content + offset2), (offset - offset2));
}

template <typename, SizeT32>
struct JSONotationStrings;

template <typename Char_T_>
struct JSONotation_T_ {
  private:
    static constexpr SizeT32 size_ = sizeof(Char_T_);

  public:
    static constexpr Char_T_ QuoteChar    = '"';
    static constexpr Char_T_ CommaChar    = ',';
    static constexpr Char_T_ ColonChar    = ':';
    static constexpr Char_T_ SCurlyChar   = '{'; // Start
    static constexpr Char_T_ ECurlyChar   = '}'; // End
    static constexpr Char_T_ SSquareChar  = '['; // Start
    static constexpr Char_T_ ESquareChar  = ']'; // End
    static constexpr Char_T_ SlashChar    = '/';
    static constexpr Char_T_ BSlashChar   = '\\';
    static constexpr Char_T_ SpaceChar    = ' ';
    static constexpr Char_T_ DotChar      = '.';
    static constexpr Char_T_ NegativeChar = '-';

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
    static constexpr Char_T_ CE_Char = 'E';

    static constexpr SizeT          TrueStringLength = 4U;
    static constexpr const Char_T_ *TrueString       = &(JSONotationStrings<Char_T_, size_>::TrueString[0]);

    static constexpr SizeT          FalseStringLength = 5U;
    static constexpr const Char_T_ *FalseString       = &(JSONotationStrings<Char_T_, size_>::FalseString[0]);

    static constexpr SizeT          NullStringLength = 4U;
    static constexpr const Char_T_ *NullString       = &(JSONotationStrings<Char_T_, size_>::NullString[0]);
};

// char
template <typename Char_T_>
struct JSONotationStrings<Char_T_, 1U> {
    static constexpr const Char_T_ *FalseString = "false";
    static constexpr const Char_T_ *TrueString  = "true";
    static constexpr const Char_T_ *NullString  = "null";
};

// char16_t
template <typename Char_T_>
struct JSONotationStrings<Char_T_, 2U> {
    static constexpr const Char_T_ *FalseString = u"false";
    static constexpr const Char_T_ *TrueString  = u"true";
    static constexpr const Char_T_ *NullString  = u"null";
};

// char32_t
template <typename Char_T_>
struct JSONotationStrings<Char_T_, 4U> {
    static constexpr const Char_T_ *FalseString = U"false";
    static constexpr const Char_T_ *TrueString  = U"true";
    static constexpr const Char_T_ *NullString  = U"null";
};

// wchar_t size = 4
template <>
struct JSONotationStrings<wchar_t, 4U> {
    static constexpr const wchar_t *FalseString = L"false";
    static constexpr const wchar_t *TrueString  = L"true";
    static constexpr const wchar_t *NullString  = L"null";
};

// wchar_t size = 2
template <>
struct JSONotationStrings<wchar_t, 2U> {
    static constexpr const wchar_t *FalseString = L"false";
    static constexpr const wchar_t *TrueString  = L"true";
    static constexpr const wchar_t *NullString  = L"null";
};

} // namespace Qentem

#endif
