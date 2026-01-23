/**
 * @file JSONUtils.hpp
 * @brief Declares utilities and helpers for JSON processing in Qentem Engine.
 *
 * This header provides core functions, types, and algorithms for working with JSON data,
 * including parsing, serialization, and manipulation of JSON structures. JSONUtils.hpp
 * supports the implementation of high-performance, flexible JSON handling throughout
 * the Qentem Engine library.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_JSON_UTILS_H
#define QENTEM_JSON_UTILS_H

#include "Qentem/Digit.hpp"
#include "Qentem/Unicode.hpp"

namespace Qentem {

template <typename, SizeT32>
struct JSONLiterals_T;

// char
template <typename Char_T>
struct JSONLiterals_T<Char_T, 1U> {
    static constexpr const Char_T *FalseString = "false";
    static constexpr const Char_T *TrueString  = "true";
    static constexpr const Char_T *NullString  = "null";
};

// char16_t
template <typename Char_T>
struct JSONLiterals_T<Char_T, 2U> {
    static constexpr const Char_T *FalseString = u"false";
    static constexpr const Char_T *TrueString  = u"true";
    static constexpr const Char_T *NullString  = u"null";
};

// char32_t
template <typename Char_T>
struct JSONLiterals_T<Char_T, 4U> {
    static constexpr const Char_T *FalseString = U"false";
    static constexpr const Char_T *TrueString  = U"true";
    static constexpr const Char_T *NullString  = U"null";
};

// wchar_t size = 4
template <>
struct JSONLiterals_T<wchar_t, 4U> {
    static constexpr const wchar_t *FalseString = L"false";
    static constexpr const wchar_t *TrueString  = L"true";
    static constexpr const wchar_t *NullString  = L"null";
};

// wchar_t size = 2
template <>
struct JSONLiterals_T<wchar_t, 2U> {
    static constexpr const wchar_t *FalseString = L"false";
    static constexpr const wchar_t *TrueString  = L"true";
    static constexpr const wchar_t *NullString  = L"null";
};

struct JSONUtils {
    template <typename>
    struct NotationConstants;

    template <typename Char_T, typename Stream_T>
    static SizeT UnEscape(const Char_T *content, SizeT length, Stream_T &stream) {
        using NotationConstants = NotationConstants_T<Char_T>;

        SizeT offset  = 0;
        SizeT offset2 = 0;

        while (offset < length) {
            switch (content[offset]) {
                case NotationConstants::QuoteChar: {
                    if (stream.IsNotEmpty()) {
                        stream.Write((content + offset2), (offset - offset2));
                    }

                    ++offset;
                    return offset;
                }

                case NotationConstants::BSlashChar: {
                    stream.Write((content + offset2), (offset - offset2));

                    ++offset;
                    offset2 = offset;
                    ++offset2;
                    const Char_T ch = content[offset];

                    switch (ch) {
                        case NotationConstants::QuoteChar:
                        case NotationConstants::BSlashChar:
                        case NotationConstants::SlashChar: {
                            stream.Write(ch);
                            break;
                        }

                        case NotationConstants::B_Char: {
                            stream.Write(NotationConstants::BackSpaceControlChar);
                            break;
                        }

                        case NotationConstants::T_Char: {
                            stream.Write(NotationConstants::TabControlChar);
                            break;
                        }

                        case NotationConstants::N_Char: {
                            stream.Write(NotationConstants::LineControlChar);
                            break;
                        }

                        case NotationConstants::F_Char: {
                            stream.Write(NotationConstants::FormfeedControlChar);
                            break;
                        }

                        case NotationConstants::R_Char: {
                            stream.Write(NotationConstants::CarriageControlChar);
                            break;
                        }

                        case NotationConstants::CU_Char:
                        case NotationConstants::U_Char: {
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

                            return 0;
                        }

                        default: {
                            return 0;
                        }
                    }

                    break;
                }

                case NotationConstants::LineControlChar:
                case NotationConstants::TabControlChar:
                case NotationConstants::CarriageControlChar: {
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

    template <typename Char_T, typename Stream_T>
    static void Escape(const Char_T *content, SizeT length, Stream_T &stream) {
        using NotationConstants = NotationConstants_T<Char_T>;

        static constexpr Char_T ReplaceList[] = {0, 0, 0, 0, 0, 0, 0, 0, 'b', 't', 'n', 0, 'f', 'r'};

        SizeT offset  = 0;
        SizeT offset2 = 0;

        while (offset < length) {
            const Char_T ch = content[offset];

            switch (ch) {
                case NotationConstants::QuoteChar:
                case NotationConstants::BSlashChar:
                case NotationConstants::SlashChar: {
                    stream.Write((content + offset2), (offset - offset2));

                    stream.Write(NotationConstants::BSlashChar);
                    offset2 = offset;
                    ++offset2;

                    stream.Write(ch);
                    break;
                }

                case NotationConstants::BackSpaceControlChar:
                case NotationConstants::TabControlChar:
                case NotationConstants::LineControlChar:
                case NotationConstants::FormfeedControlChar:
                case NotationConstants::CarriageControlChar: {
                    stream.Write((content + offset2), (offset - offset2));

                    stream.Write(NotationConstants::BSlashChar);
                    offset2 = offset;
                    ++offset2;

                    stream.Write(ReplaceList[static_cast<SizeT8>(ch)]);
                    break;
                }

                default: {
                }
            }

            ++offset;
        }

        stream.Write((content + offset2), (offset - offset2));
    }

    /**
     * @brief Holds all constant characters and canonical string literals used in JSON notation.
     *
     * Provides compile-time access to structural and escape characters for any character type.
     * Delegates canonical "true", "false", and "null" strings to JSONLiterals_T.
     */
    template <typename Char_T>
    struct NotationConstants_T {
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

        static constexpr Char_T B_Char  = 'b';
        static constexpr Char_T T_Char  = 't';
        static constexpr Char_T N_Char  = 'n';
        static constexpr Char_T F_Char  = 'f';
        static constexpr Char_T R_Char  = 'r';
        static constexpr Char_T U_Char  = 'u';
        static constexpr Char_T CU_Char = 'U';

        static constexpr SizeT         TrueStringLength = SizeT{4};
        static constexpr const Char_T *TrueString       = &(JSONLiterals_T<Char_T, sizeof(Char_T)>::TrueString[0]);

        static constexpr SizeT         FalseStringLength = SizeT{5};
        static constexpr const Char_T *FalseString       = &(JSONLiterals_T<Char_T, sizeof(Char_T)>::FalseString[0]);

        static constexpr SizeT         NullStringLength = SizeT{4};
        static constexpr const Char_T *NullString       = &(JSONLiterals_T<Char_T, sizeof(Char_T)>::NullString[0]);
    };
};

} // namespace Qentem

#endif
