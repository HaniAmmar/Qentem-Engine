
/**
 * @file Unicode.hpp
 * @brief Provides Unicode code point to UTF-8, UTF-16, and UTF-32 encoding utilities for Qentem Engine.
 *
 * This header defines templates for efficient, dependency-free conversion of Unicode code points
 * into UTF-8, UTF-16, or UTF-32 character sequences, depending on the output character type.
 * Designed to be header-only, cross-platform, and STL-free, these utilities are used internally
 * by the Qentem Engine for Unicode-aware template rendering and parsing.
 *
 * Note: These functions assume the caller supplies a valid Unicode code point in the range [0, 0x10FFFF].
 *       No runtime validation or error handling for invalid or surrogate code points is performed.
 *
 * Example:
 *   Qentem::Unicode::ToUTF<char>(0x10A7B, output);
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_UNICODE_H
#define QENTEM_UNICODE_H

#include "QCommon.hpp"

namespace Qentem {
struct Unicode {
    template <typename, typename, SizeT32>
    struct UnicodeToUTF;

    /*
     * ToUTF(0xC3D, stream);
     * ToUTF(0x00A1, stream);
     * ToUTF(0x08A7, stream);
     * ToUTF(0x10A7B, stream);
     */

    /**
     * Converts a Unicode code point (0x0000 - 0x10FFFF) to UTF-8/16/32 sequence
     * based on the Char_T size. The input value is assumed valid; caller must ensure
     * no invalid or surrogate code points are passed.
     */
    template <typename Char_T, typename Stream_T>
    QENTEM_INLINE static void ToUTF(SizeT32 unicode, Stream_T &stream) {
        UnicodeToUTF<Char_T, Stream_T, sizeof(Char_T)>::ToUTF(unicode, stream);
    }

    // UTF8
    template <typename Char_T, typename Stream_T>
    struct UnicodeToUTF<Char_T, Stream_T, 1U> {
        static void ToUTF(SizeT32 unicode, Stream_T &stream) {
            if (unicode < 0x80U) {
                stream.Write(Char_T(unicode));
            } else {
                if (unicode < 0x800U) {
                    stream.Write(Char_T(0xC0U | (unicode >> 6U)));
                } else if (unicode < 0x10000U) {
                    stream.Write(Char_T(0xE0U | (unicode >> 12U)));
                    stream.Write(Char_T(0x80U | ((unicode >> 6U) & 0x3FU)));
                } else {
                    stream.Write(Char_T(0xF0U | (unicode >> 18U)));
                    stream.Write(Char_T(0x80U | ((unicode >> 12U) & 0x3FU)));
                    stream.Write(Char_T(0x80U | ((unicode >> 6U) & 0x3FU)));
                }

                stream.Write(Char_T(0x80U | (unicode & 0x3FU)));
            }
        }
    };

    // UTF16
    template <typename Char_T, typename Stream_T>
    struct UnicodeToUTF<Char_T, Stream_T, 2U> {
        static void ToUTF(SizeT32 unicode, Stream_T &stream) {
            if (unicode < 0x10000U) {
                stream.Write(Char_T(unicode));
            } else {
                unicode -= 0x10000U;
                stream.Write(Char_T(0xD800U | (unicode >> 10U)));
                stream.Write(Char_T(0xDC00U | (unicode & 0x3FFU)));
            }
        }
    };

    // UTF32
    template <typename Char_T, typename Stream_T>
    struct UnicodeToUTF<Char_T, Stream_T, 4U> {
        QENTEM_INLINE static void ToUTF(SizeT32 unicode, Stream_T &stream) {
            // if (unicode > 0x10FFFFU || (unicode >= 0xD800U && unicode <= 0xDFFFU)) {
            //     // Invalid code point: ignore or handle error
            //     return;
            // }

            stream.Write(Char_T(unicode));
        }
    };
};

} // namespace Qentem

#endif
