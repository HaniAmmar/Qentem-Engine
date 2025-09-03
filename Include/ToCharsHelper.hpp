/**
 * @file ToCharsHelper.hpp
 * @brief Type-safe and Unicode-aware streaming utility for converting values to character streams.
 *
 * This header defines the `ToCharsHelper` utility, a compile-time dispatch system for writing values
 * of arbitrary types into Qentem-compatible character streams. It handles numeric types, strings,
 * pointers, and Unicode characters (including `char16_t`, `char32_t`, and `wchar_t`) with proper UTF conversion.
 *
 * Used internally by `QTest`, `Console`, and various Qentem modules to render formatted output
 * in a lightweight, header-only, STL-free environment.
 *
 * The system uses type decay and static dispatch to eliminate ambiguity and avoid overload pitfalls
 * across pointer and value categories. Supports safe fallback for unknown types via iteration-based rendering.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_TO_CHARS_HELPER_H
#define QENTEM_TO_CHARS_HELPER_H

#include "Digit.hpp"
#include "QTraits.hpp"
#include "Unicode.hpp"
#include "StringUtils.hpp"

namespace Qentem {

struct ToCharsHelper {
    template <typename Stream_T, typename Value_T>
    struct Writer;

    template <typename Stream_T, typename Value_T>
    struct CharConverter_T;

    template <typename Stream_T, typename... Values_T>
    inline static void Write(Stream_T &stream, const Values_T &...values) {
#if __cplusplus > 201402L
        (Writer<Stream_T, typename QTraits::Decay<Values_T>::Type>::Write(stream, values), ...);
#else
        const int dummy[sizeof...(Values_T)] = {
            (Writer<Stream_T, typename QTraits::Decay<Values_T>::Type>::Write(stream, values), 0)...};
        (void)dummy;
#endif
    }

    template <typename Stream_T, typename Value_T>
    struct Writer {
        static void Write(Stream_T &stream, const Value_T &value) {
            using CharConverter = CharConverter_T<Stream_T, typename QTraits::Decay<decltype(value.begin())>::Type>;

            for (const auto &v : value) {
                CharConverter::Convert(stream, v);
            }
        }

        static void Write(Stream_T &stream, const Value_T *value) {
            Writer<Stream_T, void>::Write(stream, value);
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, NullType> {
        static void Write(Stream_T &stream, NullType) {
            stream.Write("null", SizeT{4});
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, bool> {
        static void Write(Stream_T &stream, bool b) {
            b ? stream.Write("true", SizeT{4}) : stream.Write("false", SizeT{5});
        }

        static void Write(Stream_T &stream, const bool *b) {
            Writer<Stream_T, void>::Write(stream, b);
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, char> {
        static void Write(Stream_T &stream, char ch) {
            stream.Write(ch);
        }

        static void Write(Stream_T &stream, const char *str) {
            stream.Write(str, StringUtils::Count(str));
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, unsigned char> {
        static void Write(Stream_T &stream, unsigned char ch) {
            stream.Write(static_cast<char>(ch));
        }

        static void Write(Stream_T &stream, const unsigned char *str) {
            stream.Write(reinterpret_cast<const char *>(str), StringUtils::Count(str));
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, char16_t> {
        static void Write(Stream_T &stream, char16_t ch) {
            Unicode::ToUTF<char>(ch, stream);
        }

        static void Write(Stream_T &stream, const char16_t *str) {
            SizeT       index{0};
            const SizeT length = StringUtils::Count(str);

            while (index < length) {
                Unicode::ToUTF<char>(static_cast<SizeT32>(str[index]), stream);
                ++index;
            }
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, char32_t> {
        static void Write(Stream_T &stream, char32_t ch) {
            Unicode::ToUTF<char>(ch, stream);
        }

        static void Write(Stream_T &stream, const char32_t *str) {
            SizeT       index{0};
            const SizeT length = StringUtils::Count(str);

            while (index < length) {
                Unicode::ToUTF<char>(str[index], stream);
                ++index;
            }
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, wchar_t> {
        static void Write(Stream_T &stream, wchar_t ch) {
            Unicode::ToUTF<char>(ch, stream);
        }

        static void Write(Stream_T &stream, const wchar_t *str) {
            SizeT       index{0};
            const SizeT length = StringUtils::Count(str);

            while (index < length) {
                Unicode::ToUTF<char>(static_cast<SizeT32>(str[index]), stream);
                ++index;
            }
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, void> {
        static void Write(Stream_T &stream, const void *ptr) {
            constexpr SizeT       TOTAL_LENGTH{sizeof(void *) * 2U};
            char                  buffer[TOTAL_LENGTH];
            PtrCast_T<const void> ptr_value{ptr};
            const SizeT           length = Digit::NumberToHex(&(buffer[0]), ptr_value.Number);
            const SizeT           offset = (TOTAL_LENGTH - length);

            stream.Write("0x", SizeT{2});
            Digit::InsertZeros(stream, offset);
            stream.Write(&(buffer[offset]), length);
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, short> {
        static void Write(Stream_T &stream, short number) {
            Digit::NumberToString(stream, number);
        }

        static void Write(Stream_T &stream, const short int *number) {
            Writer<Stream_T, void>::Write(stream, number);
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, unsigned short> {
        static void Write(Stream_T &stream, unsigned short number) {
            Digit::NumberToString(stream, number);
        }

        static void Write(Stream_T &stream, const unsigned short *number) {
            Writer<Stream_T, void>::Write(stream, number);
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, int> {
        static void Write(Stream_T &stream, int number) {
            Digit::NumberToString(stream, number);
        }

        static void Write(Stream_T &stream, const int *number) {
            Writer<Stream_T, void>::Write(stream, number);
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, unsigned int> {
        static void Write(Stream_T &stream, unsigned int number) {
            Digit::NumberToString(stream, number);
        }

        static void Write(Stream_T &stream, const unsigned int *number) {
            Writer<Stream_T, void>::Write(stream, number);
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, long> {
        static void Write(Stream_T &stream, long number) {
            Digit::NumberToString(stream, number);
        }

        static void Write(Stream_T &stream, const long *number) {
            Writer<Stream_T, void>::Write(stream, number);
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, unsigned long> {
        static void Write(Stream_T &stream, unsigned long number) {
            Digit::NumberToString(stream, number);
        }

        static void Write(Stream_T &stream, const unsigned long *number) {
            Writer<Stream_T, void>::Write(stream, number);
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, long long> {
        static void Write(Stream_T &stream, long long number) {
            Digit::NumberToString(stream, number);
        }

        static void Write(Stream_T &stream, const long long *number) {
            Writer<Stream_T, void>::Write(stream, number);
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, unsigned long long> {
        static void Write(Stream_T &stream, unsigned long long number) {
            Digit::NumberToString(stream, number);
        }

        static void Write(Stream_T &stream, const unsigned long long *number) {
            Writer<Stream_T, void>::Write(stream, number);
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, float> {
        static void Write(Stream_T &stream, float number) {
            Digit::NumberToString(stream, number, {2, Digit::RealFormatType::SemiFixed});
        }

        static void Write(Stream_T &stream, const float *number) {
            Writer<Stream_T, void>::Write(stream, number);
        }
    };

    template <typename Stream_T>
    struct Writer<Stream_T, double> {
        static void Write(Stream_T &stream, double number) {
            Digit::NumberToString(stream, number, {2, Digit::RealFormatType::SemiFixed});
        }

        static void Write(Stream_T &stream, const double *number) {
            Writer<Stream_T, void>::Write(stream, number);
        }
    };

    template <typename Stream_T, typename Value_T>
    struct CharConverter_T {
        static void Convert(Stream_T &stream, Value_T ch) {
            stream << ch;
        }
    };

    template <typename Stream_T>
    struct CharConverter_T<Stream_T, char> {
        static void Convert(Stream_T &stream, char ch) {
            stream.Write(ch);
        }
    };

    template <typename Stream_T>
    struct CharConverter_T<Stream_T, unsigned char> {
        static void Convert(Stream_T &stream, unsigned char ch) {
            stream.Write(static_cast<char>(ch));
        }
    };

    template <typename Stream_T>
    struct CharConverter_T<Stream_T, char16_t> {
        static void Convert(Stream_T &stream, char16_t ch) {
            Unicode::ToUTF<char>(static_cast<SizeT32>(ch), stream);
        }
    };

    template <typename Stream_T>
    struct CharConverter_T<Stream_T, char32_t> {
        static void Convert(Stream_T &stream, char32_t ch) {
            Unicode::ToUTF<char>(ch, stream);
        }
    };

    template <typename Stream_T>
    struct CharConverter_T<Stream_T, wchar_t> {
        static void Convert(Stream_T &stream, wchar_t ch) {
            Unicode::ToUTF<char>(static_cast<SizeT32>(ch), stream);
        }
    };
};

} // namespace Qentem

#endif
