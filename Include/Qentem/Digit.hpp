/**
 * @file Digit.hpp
 * @brief Declares fundamental digit and character classification utilities for Qentem Engine.
 *
 * This header provides essential functions and constants for identifying,
 * classifying, and working with digit characters and numeric values.
 * Digit.hpp serves as a foundational component for parsing and validating
 * numeric input throughout the Qentem Engine library.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_DIGIT_H
#define QENTEM_DIGIT_H

#include "Qentem/QNumber.hpp"
#include "Qentem/BigInt.hpp"
#include "Qentem/DigitUtils.hpp"
#include "Qentem/StringUtils.hpp"

namespace Qentem {

struct Digit {
    // Default: same as std::defaultfloat.
    // Fixed: same as std::fixed.
    // SemiFixed: same as std::fixed, but do not insert zeros.
    // Scientific: not implemented.
    enum struct RealFormatType : SizeT8 { Default = 0, Fixed = 1, SemiFixed = 2, Scientific = 3 };

    struct RealFormatInfo {
        constexpr RealFormatInfo() noexcept {
        }

        constexpr RealFormatInfo(SizeT32 precision) noexcept : Precision{precision} {
        }

        constexpr RealFormatInfo(RealFormatType type) noexcept : Type{type} {
        }

        constexpr RealFormatInfo(SizeT32 precision, RealFormatType type) noexcept : Precision{precision}, Type{type} {
        }

        RealFormatInfo &operator=(SizeT32 precision) noexcept {
            Precision = precision;
            return *this;
        }

        RealFormatInfo &operator=(RealFormatType type) noexcept {
            Type = type;
            return *this;
        }

        SizeT32        Precision{6U};
        RealFormatType Type{RealFormatType::Default};
    };
    /////////////////////////////////////////////////////////////////
    template <bool Reverse_V_T = false, typename Stream_T, typename Number_T>
    QENTEM_INLINE static void NumberToString(Stream_T &stream, Number_T number,
                                             const RealFormatInfo format = RealFormatInfo{}) {
        constexpr SizeT32 n_size = sizeof(Number_T);
        using Char_T             = typename Stream_T::CharType;
        using QNumberType_T      = typename QNumberAutoTypeT<Number_T, n_size>::QNumberType_T;

        QNumberType_T qn{number};

        if constexpr (IsFloat<Number_T>()) {
            realToString<Number_T>(stream, QNumberType_T{number}.Natural, format);
        } else {
            constexpr SizeT32 max_number_of_digits = (((n_size * 8U * 30103U) / 100000) + 1U);
            Char_T            storage[max_number_of_digits];

            if constexpr (!IsUnsigned<Number_T>()) {
                if (number < 0) {
                    qn.Integer = static_cast<Number_T>(-qn.Integer);
                    stream.Write(DigitUtils::DigitChar::Negative);
                }
            }

            if constexpr (Reverse_V_T) {
                stream.Write(&(storage[0]), IntToString<true>(&(storage[0]), qn.Natural));
            } else {
                const SizeT length = IntToString(&(storage[max_number_of_digits]), qn.Natural);
                stream.Write(&(storage[max_number_of_digits - length]), length);
            }
        }
    }

    /////////////////////////////////////////
    template <bool Reverse_V_T = false, typename Char_T, typename Number_T>
    static SizeT IntToString(Char_T *storage, Number_T number) noexcept {
        const Char_T *str = storage;

        if constexpr (!Reverse_V_T) {
            while (number >= Number_T{10}) {
                SizeT index = SizeT(number % Number_T{100});
                number /= Number_T{100};
                index *= SizeT{2};

                --storage;
                *storage = Char_T(DigitUtils::DigitTable1[index + SizeT{1}]);
                --storage;
                *storage = Char_T(DigitUtils::DigitTable1[index]);
            }

            if ((number != 0) || (str == storage)) {
                --storage;
                *storage = Char_T(DigitUtils::DigitTable2[number]);
            }

            return SizeT(str - storage);
        } else {
            while (number >= Number_T{10}) {
                SizeT index = SizeT(number % Number_T{100});
                number /= Number_T{100};
                index *= SizeT{2};

                *storage = Char_T(DigitUtils::DigitTable1[index + SizeT{1}]);
                ++storage;
                *storage = Char_T(DigitUtils::DigitTable1[index]);
                ++storage;
            }

            if ((number != 0) || (str == storage)) {
                *storage = Char_T(DigitUtils::DigitTable2[number]);
                ++storage;
            }

            return SizeT(storage - str);
        }
    }

    template <typename Number_T>
    QENTEM_INLINE static void IntToString(const char *&out, SizeT32 &out_length, Number_T number) noexcept {
        constexpr SizeT max{20}; // Max length for 64-bit number as string
        char            buffer[max];
        out_length = IntToString(&(buffer[max]), number);
        out        = &(buffer[max - out_length]);
    }
    /////////////////////////////////////////////////////////////////
    template <typename Number_T, typename Char_T, typename SizeT_Type>
    static Number_T HexStringToNumber(const Char_T *value, SizeT_Type &offset, const SizeT_Type end_offset) noexcept {
        Number_T number{0};

        while (offset < end_offset) {
            const Char_T digit = value[offset];

            if ((digit >= DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                // 0-9
                number <<= 4U;
                ++offset;
                number |= Number_T(digit - DigitUtils::DigitChar::Zero);
                continue;
            }

            if ((digit >= DigitUtils::DigitChar::UA) && (digit <= DigitUtils::DigitChar::UF)) {
                // A-F
                number <<= 4U;
                ++offset;
                number |= Number_T(digit - DigitUtils::DigitChar::Seven);
                continue;
            }

            if ((digit >= DigitUtils::DigitChar::A) && (digit <= DigitUtils::DigitChar::F)) {
                // a-f
                number <<= 4U;
                ++offset;
                number |= Number_T(digit - DigitUtils::DigitChar::UW);
                continue;
            }

            break;
        }

        return number;
    }

    template <bool Lowercase = false, typename Number_T>
    static SizeT NumberToHex(char *buffer, Number_T number) {
        static constexpr char HexTable[]  = "0123456789ABCDEF";
        static constexpr char HexTableL[] = "0123456789abcdef";
        constexpr SizeT       length      = (sizeof(Number_T) * 2U);

        SizeT index{length};

        if constexpr (Lowercase) {
            while (index != 0) {
                --index;
                buffer[index] = HexTableL[static_cast<SizeT>(number & Number_T{0xF})];
                number >>= 4U;

                if (number != 0) {
                    --index;
                    buffer[index] = HexTableL[static_cast<SizeT>((number & Number_T{0xF}))];
                    number >>= 4U;

                    if (number != 0) {
                        continue;
                    }
                }

                break;
            }

            return (length - index);
        } else {
            while (index != 0) {
                --index;
                buffer[index] = HexTable[static_cast<SizeT>(number & Number_T{0xF})];
                number >>= 4U;

                if (number != 0) {
                    --index;
                    buffer[index] = HexTable[static_cast<SizeT>((number & Number_T{0xF}))];
                    number >>= 4U;

                    if (number != 0) {
                        continue;
                    }
                }

                break;
            }

            return (length - index);
        }
    }

    template <typename Number_T, typename Char_T>
    QENTEM_INLINE static Number_T HexStringToNumber(const Char_T *value, const SizeT length) noexcept {
        SizeT offset{0};
        return HexStringToNumber<Number_T>(value, offset, length);
    }

    /////////////////////////////////////////////////////////////////

    /**
     * @brief Writes the binary representation of a number to a stream.
     *
     * This function prints the bits of @p value from most-significant bit (MSB)
     * to least-significant bit (LSB), independent of machine endianness.
     *
     * The output is formatted into groups of bits (columns) separated by spaces,
     * and wrapped into rows separated by newlines.
     *
     * Internally, the value is converted to its unsigned QNumber representation
     * to ensure well-defined shifting and masking behavior for all integer types,
     * including signed types.
     *
     * @tparam Stream_T  Output stream type. Must provide:
     *                   - CharType
     *                   - Write(CharType)
     * @tparam Number_T  Integer type to be printed.
     *
     * @param stream               Destination stream.
     * @param value                Number whose binary form will be written.
     * @param max_bits_per_column  Number of bits per column (group). Default is 8.
     * @param max_bits_per_row     Number of bits per row before inserting a newline.
     *                             Default is 32.
     */
    template <typename Stream_T, typename Number_T>
    static void NumberToBinary(Stream_T &stream, Number_T value, const SizeT32 max_bits_per_column = 8U,
                               const SizeT32 max_bits_per_row = 32U) {
        using Char_T        = typename Stream_T::CharType;
        using QNumberType_T = typename QNumberAutoTypeT<Number_T, sizeof(Number_T)>::QNumberType_T;

        static constexpr SizeT32  TOTAL_BITS{sizeof(Number_T) * 8U};
        static constexpr SizeT32  TOTAL_BITS_M1{TOTAL_BITS - 1U};
        static constexpr Number_T High{Number_T{1} << TOTAL_BITS_M1};

        QNumberType_T qn{value};
        SizeT32       column{0};
        SizeT32       row{0};

        SizeT32 index{0};

        do {
            if (row == max_bits_per_row) {
                stream.Write(Char_T{'\n'});
                row    = 0;
                column = 0;
            } else if (column == max_bits_per_column) {
                stream.Write(Char_T{' '});
                column = 0;
            }

            stream.Write(Char_T(((qn.Natural & High) >> TOTAL_BITS_M1) + DigitUtils::DigitChar::Zero));
            qn.Natural <<= 1U;

            ++column;
            ++row;

            ++index;
        } while (index < TOTAL_BITS);
    }

    /**
     * @brief Writes the binary representation of a character string to a stream.
     *
     * This function converts each character in the input string into its binary
     * form and writes the result to @p stream, starting from the most-significant
     * bit (MSB) to the least-significant bit (LSB) of each character.
     *
     * The binary output is formatted into columns and rows:
     * - Columns group bits (e.g. 8 bits per character).
     * - Rows wrap after a specified number of bits.
     *
     * Internally, each character is converted to its unsigned QNumber
     * representation to ensure well-defined shifting and masking behavior,
     * even when CharType is a signed type.
     *
     * @tparam Stream_T  Output stream type. Must provide:
     *                   - CharType
     *                   - Write(CharType)
     *
     * @param stream               Destination stream.
     * @param content              Pointer to the character sequence to convert.
     * @param length               Number of characters to process.
     * @param max_bits_per_column  Number of bits per column (group). Default is 8.
     * @param max_bits_per_row     Number of bits per row before inserting a newline.
     *                             Default is 32.
     */
    template <typename Stream_T>
    static void StringToBinary(Stream_T &stream, const typename Stream_T::CharType *content, SizeT length,
                               const SizeT32 max_bits_per_column = 8U, const SizeT32 max_bits_per_row = 32U) {
        using Char_T        = typename Stream_T::CharType;
        using QNumberType_T = typename QNumberAutoTypeT<Char_T, sizeof(Char_T)>::QNumberType_T;
        QNumberType_T qn;

        using Natural_T = decltype(qn.Natural);

        static constexpr SizeT32   TOTAL_BITS{sizeof(Char_T) * 8U};
        static constexpr SizeT32   TOTAL_BITS_M1{TOTAL_BITS - 1U};
        static constexpr Natural_T High{Natural_T{1} << TOTAL_BITS_M1};

        SizeT32 column{0};
        SizeT32 row{0};
        SizeT32 offset{0};

        while (offset < length) {
            qn = content[offset];
            SizeT32 index{0};

            do {
                if (row == max_bits_per_row) {
                    stream.Write(Char_T{'\n'});
                    row    = 0;
                    column = 0;
                } else if (column == max_bits_per_column) {
                    stream.Write(Char_T{' '});
                    column = 0;
                }

                stream.Write(Char_T(((qn.Natural & High) >> TOTAL_BITS_M1) + DigitUtils::DigitChar::Zero));
                qn.Natural <<= 1U;

                ++column;
                ++row;

                ++index;
            } while (index < TOTAL_BITS);

            ++offset;
        }
    }

    /////////////////////////////////////////////////////////////////

    template <typename Number_T, typename Char_T>
    static void FastStringToNumber(Number_T &number, const Char_T *content, SizeT length) noexcept {
        SizeT offset = 0;
        number       = 0;

        if (offset < length) {
            number += Number_T(content[offset]);
            number -= Number_T{DigitUtils::DigitChar::Zero};
            ++offset;

            while (offset < length) {
                number *= Number_T{10};
                number += Number_T(content[offset]);
                number -= Number_T{DigitUtils::DigitChar::Zero};
                ++offset;
            }
        }
    }

    template <typename Char_T>
    QENTEM_INLINE static QNumberType StringToNumber(QNumber64 &number, const Char_T *content, SizeT &offset,
                                                    SizeT end_offset) noexcept {
        return stringToNumber(number, content, offset, end_offset);
    }

    template <typename Char_T>
    QENTEM_INLINE static QNumberType StringToNumber(QNumber64 &number, const Char_T *content, SizeT length) noexcept {
        SizeT offset{0};
        return stringToNumber(number, content, offset, length);
    }

    /**
     * @brief Formats a timestamp into a decimal string with sub-second precision.
     *
     * Converts a `(seconds, nanoseconds)` pair into human-readable form:
     *     "<seconds>.<fraction>"
     *
     * The fractional part is generated from the nanoseconds value, respecting
     * the requested precision (up to 9 digits, since nanoseconds are capped
     * at 1e9).
     *
     * Example (precision = 6):
     *     seconds = 12, nanoseconds = 345678900 → "12.345679"
     *
     * Rounding is applied at the cutoff digit, so "…6789" with precision 6
     * becomes "…678900" → rounded to "…679".
     *
     * @tparam Stream_T   Output stream type; must provide `CharType`, `Write()`.
     * @param stream      Target stream to write into.
     * @param seconds     Whole seconds component.
     * @param nanosecond  Nanoseconds component (0 ≤ ns < 1e9).
     * @param precision   Desired fractional digits (default = 6, max = 9).
     */
    template <typename Stream_T>
    static void FormatBenchmarkTime(Stream_T &stream, SizeT64I seconds, SizeT64I nanosecond,
                                    SizeT32 precision = 4) noexcept {
        using Char_T = typename Stream_T::CharType;
        constexpr SizeT32 max_nanosecond{9};       // maximum nanosecond digits
        Char_T            storage[max_nanosecond]; // temporary digit buffer

        // Ensure nanoseconds is within valid range
        if (nanosecond < SizeT64I{1000000000}) {
            // Convert nanoseconds to string (stored in reverse buffer layout).
            SizeT length = IntToString(&(storage[max_nanosecond]), nanosecond);
            SizeT start  = (max_nanosecond - length);

            // Handle precision trimming and rounding
            if (precision < max_nanosecond) {
                if (start > precision) {
                    // Entire fractional part is below requested precision
                    start  = precision;
                    length = 0;
                } else {
                    const SizeT og_length = length;
                    length                = (precision - start);

                    if (og_length > SizeT{1}) {
                        // Look at digit immediately after cutoff to decide rounding
                        SizeT index = precision;

                        Char_T       *number = (storage + index);
                        const Char_T *first  = (storage + start);

                        // Standard rounding: if digit > 5, increment the previous
                        if (*number > DigitUtils::DigitChar::Five) {
                            --number;

                            // Carry propagation: 999... rolls over
                            while ((number >= first) && (*number == DigitUtils::DigitChar::Nine)) {
                                --number;
                                --length;
                            }

                            if (number < storage) {
                                // All digits rolled over → increment seconds
                                ++seconds;
                                start  = precision;
                                length = 0;
                            } else if (number < first) {
                                // Rounded into a new leading digit (e.g. .999 → .1000)
                                *number = DigitUtils::DigitChar::One;
                                length  = SizeT{1};
                                --start;
                            } else {
                                // Normal carry increment
                                ++(*number);
                            }
                        }
                    }
                }
            }

            // Emit seconds part
            NumberToString(stream, seconds);
            stream.Write(DigitUtils::DigitChar::Dot);

            // Emit fractional part, padded with leading zeros
            InsertZeros(stream, start);
            stream.Write(&(storage[start]), length);

            // Pad with trailing zeros if requested precision not yet reached
            const SizeT total = (start + length);
            if (total < precision) {
                InsertZeros(stream, (precision - total));
            }
        }
    }

    template <typename Stream_T>
    QENTEM_INLINE static void InsertZeros(Stream_T &stream, const SizeT length) {
        using Char_T           = typename Stream_T::CharType;
        constexpr SizeT32 size = sizeof(Char_T);
        stream.Write(DigitUtils::DigitString<Char_T, size>::Zeros, length);
    }

  private:
    template <typename Char_T>
    static QNumberType stringToNumber(QNumber64 &number, const Char_T *content, SizeT &offset,
                                      SizeT end_offset) noexcept {
        using Number_T = decltype(number.Natural);

        constexpr SizeT max_length = SizeT{19};
        number.Natural             = 0;

        if (offset < end_offset) {
            SizeT  dot_offset   = 0;
            SizeT  exp_offset   = 0;
            SizeT  start_offset = 0;
            SizeT  tmp_offset;
            Char_T digit         = content[offset];
            bool   is_negative   = false;
            bool   is_real       = false;
            bool   has_dot       = false;
            bool   fraction_only = false;
            ///////////////////////////////////////////////////////////
            if (digit == DigitUtils::DigitChar::Negative) {
                ++offset;
                is_negative = true;
            } else if (digit == DigitUtils::DigitChar::Positive) {
                ++offset;
            }

            tmp_offset = offset;
            ///////////////////////////////////////////////////////////
            if (offset < end_offset) {
                digit = content[offset];

                if ((digit > DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                    // 123456789
                    number.Natural += Number_T(digit);
                    number.Natural -= Number_T{DigitUtils::DigitChar::Zero};
                    tmp_offset   = (((end_offset - offset) < max_length) ? end_offset : (offset + max_length));
                    start_offset = offset;
                    ++offset;
                } else if ((digit == DigitUtils::DigitChar::Zero) || (digit == DigitUtils::DigitChar::Dot)) {
                    if ((digit == DigitUtils::DigitChar::Zero) && (offset + SizeT{1}) < end_offset) {
                        ++offset;
                        digit = content[offset];

                        if ((digit == DigitUtils::DigitChar::X) || (digit == DigitUtils::DigitChar::UX)) {
                            // Hex
                            ++offset;

                            number.Natural = HexStringToNumber<decltype(number.Natural)>(content, offset, end_offset);
                            return QNumberType::Natural;
                        }

                        if ((digit >= DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                            // 0000xxxx
                            return QNumberType::NotANumber; // Leading zero.
                        }
                    }

                    if (digit == DigitUtils::DigitChar::Dot) {
                        // 0.xxxxxxxx
                        dot_offset = offset;
                        ++offset;
                        start_offset  = offset;
                        is_real       = true;
                        has_dot       = true;
                        fraction_only = true;

                        while (offset < end_offset) {
                            // 0.000000000x
                            digit = content[offset];

                            if (digit == DigitUtils::DigitChar::Zero) {
                                ++offset;
                                continue;
                            }

                            break;
                        }
                        ///////////////////////////////////////////////////////////
                        if ((start_offset == offset) && (dot_offset == tmp_offset) &&
                            ((digit < DigitUtils::DigitChar::Zero) || (digit > DigitUtils::DigitChar::Nine))) {
                            return QNumberType::NotANumber; // Just a dot.
                        }
                        ///////////////////////////////////////////////////////////
                        start_offset = offset;
                    }

                    tmp_offset = (((end_offset - offset) < max_length) ? end_offset : (offset + max_length));
                } else {
                    return QNumberType::NotANumber;
                }
                ///////////////////////////////////////////////////////////
                const SizeT max_end_offset = tmp_offset;
                ///////////////////////////////////////////////////////////
                while (offset < end_offset) {
                    while (offset < max_end_offset) {
                        digit = content[offset];

                        if ((digit >= DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                            number.Natural *= Number_T{10};
                            number.Natural += Number_T(digit);
                            number.Natural -= Number_T{DigitUtils::DigitChar::Zero};
                            ++offset;
                            continue;
                        }

                        break;
                    }

                    if (digit == DigitUtils::DigitChar::Dot) {
                        if (!has_dot) {
                            dot_offset = offset;
                            ++offset;
                            is_real = true;
                            has_dot = true;

                            if (offset < max_end_offset) {
                                digit = content[offset];

                                if ((digit > DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                                    continue;
                                }

                                if ((digit == DigitUtils::DigitChar::Zero) && (offset + 1U) < max_end_offset) {
                                    digit = content[offset + SizeT{1}];

                                    if ((digit >= DigitUtils::DigitChar::Zero) &&
                                        (digit <= DigitUtils::DigitChar::Nine)) {
                                        continue;
                                    }
                                }
                            }

                            // Just zero at the end.
                            break;
                        }

                        return QNumberType::NotANumber; // x.x.x..x
                    }

                    break;
                }
                ///////////////////////////////////////////////////////////
                tmp_offset = offset;
                ///////////////////////////////////////////////////////////
                if (!is_real && (offset < end_offset)) {
                    digit = content[offset];

                    switch (digit) {
                        case DigitUtils::DigitChar::Dot:
                        case DigitUtils::DigitChar::E:
                        case DigitUtils::DigitChar::UE: {
                            is_real = true;
                            break;
                        }

                        default: {
                            if ((digit >= DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                                if ((number.Natural > Number_T{0x1999999999999999}) ||
                                    ((number.Natural == Number_T{0x1999999999999999}) &&
                                     (digit > DigitUtils::DigitChar::Five))) {
                                    is_real = true;
                                    break;
                                }

                                number.Natural *= Number_T{10};
                                number.Natural += (Number_T(digit) - Number_T{DigitUtils::DigitChar::Zero});

                                ++offset;
                                ++tmp_offset;

                                if (offset < end_offset) {
                                    digit = content[offset];

                                    switch (digit) {
                                        case DigitUtils::DigitChar::Dot:
                                        case DigitUtils::DigitChar::E:
                                        case DigitUtils::DigitChar::UE: {
                                            is_real = true;
                                            break;
                                        }

                                        default: {
                                            if ((digit >= DigitUtils::DigitChar::Zero) &&
                                                (digit <= DigitUtils::DigitChar::Nine)) {
                                                is_real = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                ///////////////////////////////////////////////////////////
                if (!is_real) {
                    if (!is_negative) {
                        return QNumberType::Natural;
                    }

                    if (number.Natural == 0) {
                        number.Natural |= Number_T{0x8000000000000000};
                        return QNumberType::Real;
                    }

                    if (number.Natural <= Number_T{0x7FFFFFFFFFFFFFFF}) {
                        number.Integer = -number.Integer;
                        return QNumberType::Integer;
                    }
                }
                ///////////////////////////////////////////////////////////
                if (number.Natural != 0) {
                    const SizeT32 e_p10_power =
                        (SizeT32(tmp_offset - start_offset) - SizeT32(!fraction_only && has_dot));

                    const SizeT32 e_n10_power =
                        (fraction_only ? (e_p10_power + (SizeT32(start_offset - dot_offset) - 1U))
                                       : (has_dot ? (SizeT32(offset - dot_offset) - 1U) : 0));

                    SizeT32 exponent        = 0;
                    bool    is_negative_exp = false;
                    bool    keep_going      = (offset < end_offset);
                    ///////////////////////////////////////////////////////////
                    tmp_offset   = dot_offset;
                    start_offset = offset;
                    ///////////////////////////////////////////////////////////
                    while (keep_going) {
                        digit = content[offset];

                        if ((digit >= DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                            ++offset;
                            keep_going = (offset < end_offset);
                            continue;
                        }

                        switch (content[offset]) {
                            case DigitUtils::DigitChar::Dot: {
                                if (!has_dot) {
                                    dot_offset = offset;
                                    ++offset;
                                    has_dot    = true;
                                    keep_going = (offset < end_offset);
                                    continue;
                                }

                                return QNumberType::NotANumber;
                            }

                            case DigitUtils::DigitChar::E:
                            case DigitUtils::DigitChar::UE: {
                                exp_offset = offset;
                                ++offset;

                                if (parseExponent(content, exponent, is_negative_exp, offset, end_offset)) {
                                    keep_going = false;
                                    continue;
                                }

                                return QNumberType::NotANumber;
                            }

                            default: {
                                keep_going = false;
                            }
                        }
                    }
                    ///////////////////////////////////////
                    SizeT32 e_extra_p10_power = 0;

                    if (!fraction_only && (start_offset != offset)) {
                        if (!has_dot) {
                            e_extra_p10_power =
                                SizeT32((exp_offset == 0) ? (offset - start_offset) : (exp_offset - start_offset));
                        } else if (dot_offset != tmp_offset) {
                            e_extra_p10_power = SizeT32(dot_offset - start_offset);
                        }

                        if (!is_negative_exp) {
                            exponent += e_extra_p10_power;
                        } else if (exponent <= e_extra_p10_power) {
                            exponent        = (e_extra_p10_power - exponent);
                            is_negative_exp = false;
                        } else {
                            exponent -= e_extra_p10_power;
                        }
                    }
                    ///////////////////////////////////////
                    if (is_negative_exp) {
                        exponent += e_n10_power;

                    } else if (exponent >= e_n10_power) {
                        exponent -= e_n10_power;
                    } else {
                        exponent        = (e_n10_power - exponent);
                        is_negative_exp = true;
                    }

                    if ((is_negative_exp && (exponent > e_p10_power) && ((exponent - e_p10_power) > SizeT32{324})) ||
                        (!is_negative_exp && ((exponent + e_p10_power) > SizeT32{309}))) {
                        return QNumberType::NotANumber;
                    }

                    if (is_negative_exp) {
                        powerOfNegativeTen(number.Natural, exponent);
                    } else {
                        powerOfPositiveTen(number.Natural, exponent);
                    }
                }
                ///////////////////////////////////////
                if (is_negative) {
                    number.Natural |= 0x8000000000000000LL;
                }

                return QNumberType::Real;
            }
        }

        return QNumberType::NotANumber;
    }
    /////////////////////////////////////////
    template <typename Number_T>
    static void powerOfNegativeTen(Number_T &number, SizeT32 exponent) noexcept {
        using UNumber_T  = SizeT64;
        using DigitConst = DigitUtils::DigitConst<sizeof(UNumber_T)>;
        //////////////////////////////////////////////////////////////
        BigInt<UNumber_T, 256U> b_int{number};
        //////////////////////////////////////////////////////////////
        SizeT32 shifted{exponent};
        //////////////////////////////////////////////////////////////
        shifted += 64U;
        b_int <<= 64U;
        //////////////////////////////////////////////////////////////
        constexpr bool is_size_8 = (sizeof(UNumber_T) == 8U);
        if constexpr (is_size_8) {
            while (exponent >= DigitConst::MaxPowerOfFive) {
                // 2**126 = 85070591730234615865843651857942052864
                // 5**27 = 7450580596923828125 (MaxPowerOfFive)
                // 2**126 / 5**27 = 11417981541647679048.4
                // 126-64=62; See 2**126 and 64 shift

                b_int *= DigitConst::PowerOfOneOverFive[DigitConst::MaxPowerOfFive];
                b_int >>= DigitConst::MaxShift;
                shifted += DigitConst::PowerOfOneOverFiveShift[DigitConst::MaxPowerOfFive];
                exponent -= DigitConst::MaxPowerOfFive;
            }

            if (exponent != 0) {
                b_int *= DigitConst::PowerOfOneOverFive[exponent];
                b_int >>= DigitConst::MaxShift;
                shifted += DigitConst::PowerOfOneOverFiveShift[exponent];
            }
        } else {
            while (exponent >= DigitConst::MaxPowerOfFive) {
                b_int <<= DigitConst::MaxShift;
                b_int /= DigitConst::PowerOfFive[DigitConst::MaxPowerOfFive];
                shifted += DigitConst::MaxShift;
                exponent -= DigitConst::MaxPowerOfFive;
            }

            if (exponent != 0) {
                b_int <<= DigitConst::MaxShift;
                b_int /= DigitConst::PowerOfFive[exponent];
                shifted += DigitConst::MaxShift;
            }
        }
        //////////////////////////////////////////////////////////////
        SizeT32       exp = DigitUtils::RealNumberInfo<double, 8U>::Bias; // double only
        const SizeT32 bit = b_int.FindLastBit();

        // if (bit <= 52U) {
        //     number = SizeT64(b_int);
        //     number <<= (53U - bit);
        // } else {
        b_int >>= (bit - SizeT32{53});
        number = SizeT64(b_int);
        // }
        //////////////////////////////////////////////////////////////
        if (shifted <= bit) {
            shifted = (bit - shifted);
            exp += shifted;

            number += (number & Number_T{1});
            number >>= 1U;
            exp += (number > Number_T{0x1FFFFFFFFFFFFF});
        } else {
            shifted -= bit;

            if (exp > shifted) {
                exp -= shifted;
                number += (number & Number_T{1});
                number >>= 1U;
                exp += (number > Number_T{0x1FFFFFFFFFFFFF});
            } else {
                shifted -= exp;
                ++shifted;
                number >>= shifted;
                number += (number & Number_T{1});
                number >>= 1U;
                exp = (number > Number_T{0xFFFFFFFFFFFFF});
            }
        }

        number &= 0xFFFFFFFFFFFFFULL;
        number |= (SizeT64(exp) << 52U);
    }
    /////////////////////////////////////////
    static void powerOfPositiveTen(SizeT64 &number, SizeT32 exponent) noexcept {
        using UNumber_T  = SystemLong;
        using DigitConst = DigitUtils::DigitConst<sizeof(UNumber_T)>;
        //////////////////////////////////////////////////////////////
        BigInt<UNumber_T, 256U> b_int{number};
        //////////////////////////////////////////////////////////////
        constexpr SizeT32 b_int_limit{2U};
        SizeT32           shifted{exponent};
        //////////////////////////////////////////////////////////////
        while (exponent >= DigitConst::MaxPowerOfFive) {
            b_int *= DigitConst::PowerOfFive[DigitConst::MaxPowerOfFive];

            if (b_int.Index() > b_int_limit) {
                b_int >>= DigitConst::MaxShift;
                shifted += DigitConst::MaxShift;
            }

            exponent -= DigitConst::MaxPowerOfFive;
        }

        if (exponent != 0) {
            b_int *= DigitConst::PowerOfFive[exponent];
        }
        //////////////////////////////////////////////////////////////
        const SizeT32 bit = b_int.FindLastBit();

        if (bit <= 52U) {
            number = SizeT64(b_int);
            number <<= (52U - bit);
        } else {
            b_int >>= (bit - 53U);
            number = SizeT64(b_int);
            number += (number & SizeT64{1});
            number >>= 1U;
            shifted += SizeT32(number > SizeT64{0x1FFFFFFFFFFFFF});
        }
        //////////////////////////////////////////////////////////////
        SizeT64 exp = DigitUtils::RealNumberInfo<double, 8U>::Bias; // double only
        exp += bit;
        exp += shifted;
        exp <<= 52U;
        number &= SizeT64{0xFFFFFFFFFFFFF};
        number |= exp;
    }
    /////////////////////////////////////////
    template <typename Char_T>
    static bool parseExponent(const Char_T *content, SizeT32 &exponent, bool &is_negative_exp, SizeT &offset,
                              const SizeT end_offset) noexcept {
        bool sign_set{false};

        while (offset < end_offset) {
            switch (content[offset]) {
                case DigitUtils::DigitChar::Positive: {
                    if (!sign_set) {
                        ++offset;
                        sign_set = true;
                        break;
                    }

                    return false;
                }
                case DigitUtils::DigitChar::Negative: {
                    if (!sign_set) {
                        ++offset;
                        is_negative_exp = true;
                        sign_set        = true;
                        break;
                    }

                    return false;
                }

                default: {
                    const SizeT o_offset = offset;

                    while (offset < end_offset) {
                        const Char_T digit = content[offset];

                        if ((digit >= DigitUtils::DigitChar::Zero) && (digit <= DigitUtils::DigitChar::Nine)) {
                            exponent *= SizeT32{10};
                            exponent += SizeT32(digit - DigitUtils::DigitChar::Zero);
                            ++offset;
                            continue;
                        }

                        break;
                    }

                    return (o_offset != offset);
                }
            }
        }

        return false;
    }
    /////////////////////////////////////////
    template <typename Float_T, typename Stream_T, typename Number_T>
    static void realToString(Stream_T &stream, const Number_T number, const RealFormatInfo format) {
        constexpr SizeT32 number_size = sizeof(Number_T);

        using Info_T = DigitUtils::RealNumberInfo<Float_T, number_size>;
        // 4.9406564584124654e-324 needs about 1216 bits to store all its digits.
        using BigIntSys  = BigInt<SystemLong, ((Info_T::Bias + 1U) + (number_size * 8U * 3U))>;
        using DigitConst = DigitUtils::DigitConst<BigIntSys::ByteWidth()>;

        const Number_T bias = (number & Info_T::ExponentMask);

        if (bias != Info_T::ExponentMask) {
            if ((number & Info_T::SignMask) != 0) {
                stream.Write(DigitUtils::DigitChar::Negative);
            }

            Number_T mantissa = (number & Info_T::MantissaMask);

            if ((mantissa != 0) || (bias != 0)) {
                (bias != 0) ? (mantissa |= Info_T::LeadingBit) : (mantissa <<= 1U);

                BigIntSys b_int{mantissa};
                /////////////////////////////////////
                const SizeT32 first_shift      = Platform::FindFirstBit(mantissa);
                const int     exponent         = (int)((bias >> Info_T::MantissaSize) - Info_T::Bias);
                const bool    is_positive_exp  = (exponent >= 0);
                const SizeT32 positive_exp     = SizeT32(is_positive_exp ? exponent : -exponent);
                const SizeT32 first_bit        = (Info_T::MantissaSize - first_shift);
                const SizeT32 exp_actual_value = (positive_exp + ((bias == 0) * first_bit));
                const SizeT32 digits           = (((exp_actual_value * 30103U) / 100000) + 1U);
                SizeT32       fraction_length  = 0;
                const bool    fixed =
                    ((format.Type == RealFormatType::SemiFixed) | (format.Type == RealFormatType::Fixed));
                const bool extra_digits = ((digits > format.Precision) & !fixed);
                const bool big_offset   = (positive_exp >= first_bit);
                const bool no_fraction  = (is_positive_exp & (big_offset | extra_digits));
                bool       round_up     = false;
                /////////////////////////////////////
                if (no_fraction) {
                    const SizeT32 drop    = (SizeT32(extra_digits) * (digits - (format.Precision + 1U)));
                    const SizeT32 m_shift = (Info_T::MantissaSize + drop);

                    int diff = (int(positive_exp) - int(m_shift));

                    if (diff > 0) {
                        b_int <<= SizeT32(diff);
                    } else if (diff < 0) {
                        b_int >>= SizeT32(-diff);
                    }

                    if (drop != 0) {
                        round_up = true;
                        bigIntDropDigits(b_int, drop);
                    }
                } else {
                    SizeT32 shift   = 0;
                    SizeT32 needed  = 0;
                    fraction_length = first_bit;

                    if (is_positive_exp) {
                        fraction_length -= positive_exp;
                        needed = format.Precision;

                        if (!fixed) {
                            needed -= digits;
                        }
                    } else {
                        fraction_length += positive_exp;
                        needed = (digits + format.Precision);
                    }

                    ++needed; // For rounding.

                    if (fraction_length > needed) {
                        shift           = (fraction_length - needed);
                        fraction_length = needed;
                        round_up        = true;
                    }

                    b_int >>= first_shift;

                    SizeT32 times = fraction_length;

                    if (times >= DigitConst::MaxPowerOfFive) {
                        /*  When formatting a floating-point value with a limited precision (e.g.,
                        three significant digits), it is unnecessary—and inefficient—to carry the full precision of the
                        underlying big integer representation.

                        To optimize performance, the algorithm computes a threshold max_index representing the maximal
                        necessary limb count for the requested output precision. If the BigInt's internal index exceeds
                        this threshold, and further shifting is available, blocks of least-significant bits may be
                        discarded via right-shift operations without affecting the accuracy of the final output.

                        This ensures that the computational cost scales with the requested output precision rather than
                        the internal floating-point width, dramatically improving speed when formatting numbers to short
                        or moderate precision.
                        */
                        const SizeT32 max_index = (format.Precision < Info_T::MaxCut)
                                                      ? ((format.Precision / DigitConst::MaxPowerOfTen) + 2U)
                                                      : b_int.MaxIndex();

                        do {
                            b_int *= DigitConst::PowerOfFive[DigitConst::MaxPowerOfFive];

                            if ((b_int.Index() >= max_index) && (shift >= DigitConst::MaxShift)) {
                                b_int >>= DigitConst::MaxShift;
                                shift -= DigitConst::MaxShift;
                            }

                            times -= DigitConst::MaxPowerOfFive;
                        } while (times >= DigitConst::MaxPowerOfFive);
                    }

                    if (times != 0) {
                        b_int *= DigitConst::PowerOfFive[times];
                    }

                    b_int >>= shift;
                }

                const SizeT start_at = stream.Length();
                bigIntToString(stream, b_int);

                switch (format.Type) {
                    case RealFormatType::Default: {
                        formatStringNumberDefault(stream, start_at, format.Precision, digits, fraction_length,
                                                  is_positive_exp, round_up);
                        break;
                    }

                    case RealFormatType::Fixed: {
                        formatStringNumberFixed<true>(stream, start_at, format.Precision, digits, fraction_length,
                                                      round_up);
                        break;
                    }

                    case RealFormatType::SemiFixed: {
                        formatStringNumberFixed<false>(stream, start_at, format.Precision, digits, fraction_length,
                                                       round_up);
                        break;
                    }

                    case RealFormatType::Scientific: {
                        break;
                    }
                }

            } else {
                stream.Write(DigitUtils::DigitChar::Zero);

                if (format.Type == RealFormatType::Fixed) {
                    stream.Write(DigitUtils::DigitChar::Dot);
                    insertZerosLarge(stream, format.Precision);
                }
            }
        } else {
            using Char_T           = typename Stream_T::CharType;
            constexpr SizeT32 size = sizeof(Char_T);
            using DigitString      = DigitUtils::DigitString<Char_T, size>;

            if ((number & Info_T::MantissaMask) == 0) {
                if ((number & Info_T::SignMask) != 0) {
                    stream.Write(DigitUtils::DigitChar::Negative);
                }

                stream.Write(DigitString::Infinity, DigitString::InfinityLength);
            } else {
                stream.Write(DigitString::NotANumber, DigitString::NotANumberLength);
            }
        }
    }

    template <typename Stream_T>
    static void insertPowerOfTen(Stream_T &stream, SizeT power_of_ten, bool positive) {
        stream.Write(DigitUtils::DigitChar::E);
        stream.Write(positive ? DigitUtils::DigitChar::Positive : DigitUtils::DigitChar::Negative);

        if (power_of_ten < SizeT{10}) {
            // e+01,e+09
            stream.Write(DigitUtils::DigitChar::Zero);
        }

        NumberToString(stream, power_of_ten);
    }

    template <typename Stream_T>
    static void insertZerosLarge(Stream_T &stream, SizeT32 length) {
        using Char_T           = typename Stream_T::CharType;
        constexpr SizeT32 size = sizeof(Char_T);
        using DigitString      = DigitUtils::DigitString<Char_T, size>;

        while (length > DigitString::ZerosLength) {
            stream.Write(DigitString::Zeros, SizeT(DigitString::ZerosLength));
            length -= DigitString::ZerosLength;
        }

        stream.Write(DigitString::Zeros, SizeT(length));
    }

    template <typename Stream_T, typename BigInt_T>
    static void bigIntToString(Stream_T &stream, BigInt_T &b_int) {
        using DigitConst = DigitUtils::DigitConst<BigInt_T::ByteWidth()>;

        while (b_int.IsBig()) {
            const SizeT length = stream.Length();
            NumberToString<true>(stream, b_int.Divide(DigitConst::MaxPowerOfTenValue));

            // dividing '1000000000000000000' by '1000000000' yield zeros remainder
            InsertZeros(stream, (DigitConst::MaxPowerOfTen - SizeT(stream.Length() - length)));
        }

        if (b_int.NotZero()) {
            NumberToString<true>(stream, b_int.Number());
        }
    }

    // template <typename Stream_T, typename BigInt_T>
    // static void bigIntToString(Stream_T &stream, BigInt_T &b_int) {
    //     using DigitConst = DigitUtils::DigitConst<BigInt_T::ByteWidth()>;
    //     using NumberType = typename BigInt_T::NumberType;

    //     constexpr auto mask = ((NumberType{1} << DigitConst::MaxPowerOfTen) - 1);

    //     while (b_int.IsBig()) {
    //         const SizeT length = stream.Length();

    //         const NumberType rem1 = b_int.Divide(DigitConst::PowerOfFive[DigitConst::MaxPowerOfTen]);
    //         NumberType       rem2 = (b_int.Number() & mask);
    //         rem2 *= DigitConst::PowerOfFive[DigitConst::MaxPowerOfTen];
    //         b_int >>= DigitConst::MaxPowerOfTen;

    //         NumberToString<true>(stream, (rem2 + rem1));

    //         // dividing '1000000000000000000' by '1000000000' yield zeros remainder
    //         InsertZeros(stream, (DigitConst::MaxPowerOfTen - SizeT(stream.Length() - length)));
    //     }

    //     NumberToString<true>(stream, b_int.Number());
    // }

    template <typename BigInt_T>
    QENTEM_INLINE static void bigIntDropDigits(BigInt_T &b_int, SizeT32 drop) noexcept {
        using DigitConst = DigitUtils::DigitConst<BigInt_T::ByteWidth()>;

        while (drop >= DigitConst::MaxPowerOfFive) {
            b_int /= DigitConst::PowerOfFive[DigitConst::MaxPowerOfFive];
            drop -= DigitConst::MaxPowerOfFive;
        }

        if (drop != 0) {
            b_int /= DigitConst::PowerOfFive[drop];
        }
    }

    // TODO: needs more work for extremely large values (e100)
    // template <typename BigInt_T>
    // QENTEM_INLINE static void bigIntDropDigits(BigInt_T &b_int, SizeT32 drop) noexcept {
    //     using DigitConst = DigitUtils::DigitConst<BigInt_T::ByteWidth()>;
    //     if constexpr (BigInt_T::BitWidth()) {
    //         // Magic division using reciprocal multiply + shift (for 64-bit)

    //         while (drop > DigitConst::MaxPowerOfFive) {
    //             b_int *= DigitConst::PowerOfOneOverFive[DigitConst::MaxPowerOfFive];
    //             b_int >>= (DigitConst::PowerOfOneOverFiveShift[DigitConst::MaxPowerOfFive] + 64U);
    //             drop -= DigitConst::MaxPowerOfFive;
    //         }
    //     }

    //     while (drop >= DigitConst::MaxPowerOfFive) {
    //         b_int /= DigitConst::PowerOfFive[DigitConst::MaxPowerOfFive];
    //         drop -= DigitConst::MaxPowerOfFive;
    //     }

    //     if (drop != 0) {
    //         b_int /= DigitConst::PowerOfFive[drop];
    //     }
    // }

    // TODO: Rewrite formatStringNumberDefault with same rigor as BigInt, post-QenWeb
    // Note: Use (32 * Char_T) fixed buffer size
    template <typename Stream_T>
    static void formatStringNumberDefault(Stream_T &stream, const SizeT started_at, const SizeT32 precision,
                                          const SizeT32 calculated_digits, SizeT32 fraction_length,
                                          const bool is_positive_exp, const bool round_up) {
        using Char_T = typename Stream_T::CharType;

        Char_T     *storage         = stream.Storage();
        const SizeT number_length   = (stream.Length() - started_at);
        SizeT       index           = started_at;
        SizeT       power           = 0;
        bool        power_increased = false;
        /////////////////////////////////////////////////////
        if (number_length > precision) {
            --index;
            index += SizeT(number_length - precision);

            roundStringNumber(stream, index, power_increased, round_up);

            if (is_positive_exp) {
                const SizeT diff =
                    SizeT(((number_length - fraction_length) +
                           ((calculated_digits <= precision) ? 0 : (calculated_digits - (precision + SizeT{1})))) -
                          SizeT(!power_increased));

                if (diff >= precision) {
                    const Char_T *number = (storage + index);
                    const Char_T *last   = stream.Last();

                    while ((number < last) && (*number == DigitUtils::DigitChar::Zero)) {
                        ++number;
                        ++index;
                    }

                    power           = diff;
                    fraction_length = 0;
                }
            }
        }

        if (fraction_length != 0) {
            const Char_T *number        = (storage + index);
            const Char_T *last          = stream.Last();
            const SizeT   dot_index     = SizeT(started_at + fraction_length);
            const bool    fraction_only = (number_length <= fraction_length);

            while ((number < last) && (*number == DigitUtils::DigitChar::Zero)) {
                ++number;
                ++index;
            }

            if (fraction_only) {
                const SizeT diff = SizeT((fraction_length > number_length) ? (fraction_length - number_length) : 0);

                if (!power_increased) {
                    if (diff < SizeT{4}) {
                        InsertZeros(stream, diff);
                        stream.Write(DigitUtils::DigitChar::Dot);
                        stream.Write(DigitUtils::DigitChar::Zero);
                    } else {
                        power = diff;
                        ++power;
                    }
                } else if ((diff != 0) && (diff < SizeT{5})) {
                    InsertZeros(stream, (diff - SizeT{1}));
                    stream.Write(DigitUtils::DigitChar::Dot);
                    stream.Write(DigitUtils::DigitChar::Zero);
                } else {
                    power = diff;
                }
            } else if (index < dot_index) {
                StringUtils::InsertAt(stream, DigitUtils::DigitChar::Dot, dot_index);
            } else {
                SizeT zeros = 0;

                if (power_increased) {
                    zeros = SizeT(number_length - fraction_length);
                } else {
                    const SizeT rem    = (index - started_at);
                    const SizeT needed = SizeT(number_length - calculated_digits);

                    if (rem > needed) {
                        zeros = (rem - needed);
                    }
                }

                while (zeros != 0) {
                    --index;
                    storage[index] = DigitUtils::DigitChar::Zero;
                    --zeros;
                }
            }
        }

        SizeT new_length = (stream.Length() - (index - started_at));

        if ((power != 0) && ((new_length - started_at) > SizeT{1})) {
            StringUtils::InsertAt(stream, DigitUtils::DigitChar::Dot, stream.Length() - SizeT{1});
            ++new_length;
        }

        StringUtils::Reverse(stream.Storage(), started_at, stream.Length(), (new_length - started_at));

        stream.SetLength(new_length);

        if (power != 0) {
            insertPowerOfTen(stream, power, is_positive_exp);
        }
    }

    template <bool Fixed_T, typename Stream_T>
    static void formatStringNumberFixed(Stream_T &stream, const SizeT started_at, const SizeT32 precision,
                                        const SizeT32 calculated_digits, const SizeT32 fraction_length,
                                        const bool round_up) {
        using Char_T = typename Stream_T::CharType;

        Char_T     *storage       = stream.Storage();
        const SizeT number_length = (stream.Length() - started_at);
        SizeT       index         = started_at;
        const SizeT dot_index     = SizeT(started_at + fraction_length);
        SizeT32     diff          = ((fraction_length > number_length) ? SizeT32(fraction_length - number_length) : 0);
        bool        power_increased = false;
        const bool  fraction_only   = (number_length <= fraction_length);
        /////////////////////////////////////////////////////
        if (fraction_length != 0) {
            if (diff <= precision) {
                if (fraction_length > precision) {
                    index += SizeT(fraction_length - (precision + SizeT{1}));
                    roundStringNumber(stream, index, power_increased, (round_up | (diff != 0)));

                    const Char_T *number = (storage + index);
                    const Char_T *last   = stream.Last();

                    while ((number < last) && (*number == DigitUtils::DigitChar::Zero)) {
                        ++number;
                        ++index;
                    }
                }

                if (fraction_only) {
                    if ((index < stream.Length()) || power_increased) {
                        if (diff != 0) {
                            if (power_increased) {
                                index -= SizeT(index == stream.Length());
                                storage[index] = DigitUtils::DigitChar::One;
                            }

                            diff -= SizeT32(power_increased);
                            insertZerosLarge(stream, diff);
                            stream.Write(DigitUtils::DigitChar::Dot);
                            stream.Write(DigitUtils::DigitChar::Zero);
                        } else if (!power_increased) {
                            stream.Write(DigitUtils::DigitChar::Dot);
                            stream.Write(DigitUtils::DigitChar::Zero);
                        }
                    } else {
                        --index;
                        storage[index] = DigitUtils::DigitChar::Zero;
                    }
                } else if (index < dot_index) {
                    StringUtils::InsertAt(stream, DigitUtils::DigitChar::Dot, dot_index);
                } else {
                    SizeT zeros = 0;

                    if (power_increased) {
                        zeros = SizeT(number_length - fraction_length);
                    } else {
                        const SizeT rem    = (index - started_at);
                        const SizeT needed = SizeT(number_length - calculated_digits);

                        if (rem > needed) {
                            zeros = (rem - needed);
                        }
                    }

                    while (zeros != 0) {
                        --index;
                        storage[index] = DigitUtils::DigitChar::Zero;
                        --zeros;
                    }
                }
            } else {
                index += SizeT(number_length - SizeT{1});
                storage[index] = DigitUtils::DigitChar::Zero;
            }
        }

        const SizeT new_length = (stream.Length() - (index - started_at));

        StringUtils::Reverse(stream.Storage(), started_at, stream.Length(), (new_length - started_at));
        stream.SetLength(new_length);

        if constexpr (Fixed_T) {
            if ((dot_index == index) || ((stream.Length() - started_at) == SizeT{1}) ||
                (!fraction_only && power_increased)) {
                stream.Write(DigitUtils::DigitChar::Dot);
                insertZerosLarge(stream, precision);
            } else if (fraction_only) {
                insertZerosLarge(
                    stream, SizeT32(precision -
                                    SizeT32(stream.Length() - (started_at + SizeT{2})))); // 2 is the length of '0.'.
            } else {
                insertZerosLarge(stream, SizeT32(precision - (dot_index - index)));
            }
        }
    }

    template <typename Stream_T>
    static void roundStringNumber(Stream_T &stream, SizeT &index, bool &power_increased, bool round_up) noexcept {
        using Char_T = typename Stream_T::CharType;

        const Char_T *last   = stream.Last();
        Char_T       *number = (stream.Storage() + index);

        ++index;

        const bool round =
            (((*number > DigitUtils::DigitChar::Five) ||
              ((*number == DigitUtils::DigitChar::Five) &&
               (round_up || ((SizeT32(stream.First()[index] - DigitUtils::DigitChar::Zero) & 1U) == 1U)))));

        if (round) {
            ++number;

            while ((number < last) && (*number == DigitUtils::DigitChar::Nine)) {
                ++index;
                ++number;
            }

            if ((number > last) || (*number == DigitUtils::DigitChar::Nine)) {
                power_increased = true;
                *number         = DigitUtils::DigitChar::One;
            } else {
                ++(*number);
            }
        }
    }
};

} // namespace Qentem

#endif
