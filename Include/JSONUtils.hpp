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

#ifndef QENTEM_JSON_UTILS_H
#define QENTEM_JSON_UTILS_H

#include "Digit.hpp"
#include "Unicode.hpp"

namespace Qentem {

template <typename>
struct JSONotation_T;

template <typename Char_T, typename Stream_T>
static SizeT UnEscapeJSON(const Char_T *content, SizeT length, Stream_T &stream) {
    using JSONotation = JSONotation_T<Char_T>;

    SizeT offset  = 0;
    SizeT offset2 = 0;

    while (offset < length) {
        switch (content[offset]) {
            case JSONotation::QuoteChar: {
                if (stream.IsNotEmpty()) {
                    stream.Write((content + offset2), (offset - offset2));
                }

                ++offset;
                return offset;
            }

            case JSONotation::BSlashChar: {
                stream.Write((content + offset2), (offset - offset2));

                ++offset;
                offset2 = offset;
                ++offset2;
                const Char_T ch = content[offset];

                switch (ch) {
                    case JSONotation::QuoteChar:
                    case JSONotation::BSlashChar:
                    case JSONotation::SlashChar: {
                        stream += ch;
                        break;
                    }

                    case JSONotation::B_Char: {
                        stream += JSONotation::BackSpaceControlChar;
                        break;
                    }

                    case JSONotation::T_Char: {
                        stream += JSONotation::TabControlChar;
                        break;
                    }

                    case JSONotation::N_Char: {
                        stream += JSONotation::LineControlChar;
                        break;
                    }

                    case JSONotation::F_Char: {
                        stream += JSONotation::FormfeedControlChar;
                        break;
                    }

                    case JSONotation::R_Char: {
                        stream += JSONotation::CarriageControlChar;
                        break;
                    }

                    case JSONotation::CU_Char:
                    case JSONotation::U_Char: {
                        ++offset;

                        if ((length - offset) > SizeT{3}) {
                            SizeT32 code = Digit::HexStringToNumber<SizeT32>((content + offset), SizeT{4});
                            offset += SizeT{4};
                            offset2 = offset;

                            if ((code >> 8U) != 0xD8U) {
                                Unicode::ToUTF<Char_T>(code, stream);
                                continue;
                            }

                            // Surrogate
                            if ((length - offset) > SizeT{5}) {
                                code = (code ^ 0xD800U) << 10U;
                                offset += SizeT{2};

                                code += Digit::HexStringToNumber<SizeT32>((content + offset), SizeT{4}) & 0x3FFU;
                                code += 0x10000U;

                                Unicode::ToUTF<Char_T>(code, stream);

                                offset += SizeT{4};
                                offset2 = offset;
                                continue;
                            }
                        }

                        return SizeT{0};
                    }

                    default: {
                        return SizeT{0};
                    }
                }

                break;
            }

            case JSONotation::LineControlChar:
            case JSONotation::TabControlChar:
            case JSONotation::CarriageControlChar: {
                return SizeT{0};
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

template <typename Char_T, typename Stream_T>
static void EscapeJSON(const Char_T *content, SizeT length, Stream_T &stream) {
    using JSONotation = JSONotation_T<Char_T>;

    SizeT offset  = 0;
    SizeT offset2 = 0;

    while (offset < length) {
        const Char_T ch = content[offset];

        switch (ch) {
            case JSONotation::QuoteChar:
            case JSONotation::BSlashChar:
            case JSONotation::SlashChar: {
                stream.Write((content + offset2), (offset - offset2));

                stream += JSONotation::BSlashChar;
                offset2 = offset;
                ++offset2;

                stream += ch;
                break;
            }

            case JSONotation::BackSpaceControlChar:
            case JSONotation::TabControlChar:
            case JSONotation::LineControlChar:
            case JSONotation::FormfeedControlChar:
            case JSONotation::CarriageControlChar: {
                stream.Write((content + offset2), (offset - offset2));

                stream += JSONotation::BSlashChar;
                offset2 = offset;
                ++offset2;

                stream += JSONotation::GetReplacementChar(SizeT32(ch));
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

template <typename Char_T>
struct JSONotation_T {
  private:
    static constexpr SizeT32 _size = sizeof(Char_T);

  public:
    static constexpr Char_T QuoteChar    = '"';
    static constexpr Char_T CommaChar    = ',';
    static constexpr Char_T ColonChar    = ':';
    static constexpr Char_T SCurlyChar   = '{'; // Start
    static constexpr Char_T ECurlyChar   = '}'; // End
    static constexpr Char_T SSquareChar  = '['; // Start
    static constexpr Char_T ESquareChar  = ']'; // End
    static constexpr Char_T SlashChar    = '/';
    static constexpr Char_T BSlashChar   = '\\';
    static constexpr Char_T SpaceChar    = ' ';
    static constexpr Char_T DotChar      = '.';
    static constexpr Char_T NegativeChar = '-';

    static constexpr Char_T BackSpaceControlChar = '\b';
    static constexpr Char_T TabControlChar       = '\t';
    static constexpr Char_T LineControlChar      = '\n';
    static constexpr Char_T FormfeedControlChar  = '\f';
    static constexpr Char_T CarriageControlChar  = '\r';

    static Char_T GetReplacementChar(SizeT32 index) noexcept {
        static constexpr Char_T ReplaceList[] = {0, 0, 0, 0, 0, 0, 0, 0, 'b', 't', 'n', 0, 'f', 'r'};

        return ReplaceList[index];
    }

    static constexpr Char_T B_Char  = 'b';
    static constexpr Char_T T_Char  = 't';
    static constexpr Char_T N_Char  = 'n';
    static constexpr Char_T F_Char  = 'f';
    static constexpr Char_T R_Char  = 'r';
    static constexpr Char_T U_Char  = 'u';
    static constexpr Char_T CU_Char = 'U';

    static constexpr SizeT         TrueStringLength = SizeT{4};
    static constexpr const Char_T *TrueString       = &(JSONotationStrings<Char_T, _size>::TrueString[0]);

    static constexpr SizeT         FalseStringLength = SizeT{5};
    static constexpr const Char_T *FalseString       = &(JSONotationStrings<Char_T, _size>::FalseString[0]);

    static constexpr SizeT         NullStringLength = SizeT{4};
    static constexpr const Char_T *NullString       = &(JSONotationStrings<Char_T, _size>::NullString[0]);
};

// char
template <typename Char_T>
struct JSONotationStrings<Char_T, 1U> {
    static constexpr const Char_T *FalseString = "false";
    static constexpr const Char_T *TrueString  = "true";
    static constexpr const Char_T *NullString  = "null";
};

// char16_t
template <typename Char_T>
struct JSONotationStrings<Char_T, 2U> {
    static constexpr const Char_T *FalseString = u"false";
    static constexpr const Char_T *TrueString  = u"true";
    static constexpr const Char_T *NullString  = u"null";
};

// char32_t
template <typename Char_T>
struct JSONotationStrings<Char_T, 4U> {
    static constexpr const Char_T *FalseString = U"false";
    static constexpr const Char_T *TrueString  = U"true";
    static constexpr const Char_T *NullString  = U"null";
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
