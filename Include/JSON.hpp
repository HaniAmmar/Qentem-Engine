/**
 * @file JSON.hpp
 * @brief Implements the main JSON data structures and logic for Qentem Engine.
 *
 * This header defines the core types and algorithms for parsing, representing,
 * and manipulating JSON documents. JSON.hpp provides the primary interface for
 * working with JSON data within the Qentem Engine library, enabling efficient
 * and flexible handling of structured data.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_JSON_H
#define QENTEM_JSON_H

#include "Value.hpp"

namespace Qentem {

struct JSON {
    template <typename, typename>
    struct Parser;

    template <typename Char_T, typename Number_T, typename Stream_T>
    inline static Value<Char_T> Parse(Stream_T &stream, const Char_T *content, Number_T length) {
        return Parser<Char_T, Stream_T>::Parse(stream, content, SizeT(length));
    }

    template <typename Char_T, typename Number_T>
    inline static Value<Char_T> Parse(const Char_T *content, Number_T length) {
        StringStream<Char_T> stream;
        return Parse(stream, content, SizeT(length));
    }

    template <typename Char_T>
    inline static Value<Char_T> Parse(const Char_T *content) {
        return Parse(content, StringUtils::Count(content));
    }

    template <typename Char_T, typename Stream_T>
    struct Parser {
        using ValueT = Value<Char_T>;

        Parser()                          = delete;
        Parser(Parser &&)                 = delete;
        Parser(const Parser &)            = delete;
        Parser &operator=(Parser &&)      = delete;
        Parser &operator=(const Parser &) = delete;
        ~Parser()                         = delete;

        static ValueT Parse(Stream_T &stream, const Char_T *content, SizeT length) {
            if (length != 0) {
                SizeT offset = 0;
                StringUtils::TrimLeft(content, offset, length);
                ValueT value = parseValue(stream, content, offset, length);
                StringUtils::TrimLeft(content, offset, length);

                if (offset == length) {
                    return value;
                }
            }

            return ValueT{};
        }

      private:
        using notation = JSONUtils::Notation_T<Char_T>;

        static ValueT parseObject(Stream_T &stream, const Char_T *content, SizeT &offset, const SizeT end) {
            using ObjectT = typename ValueT::ObjectT;

            StringUtils::TrimLeft(content, offset, end);

            ValueT value{ValueType::Object};

            if (content[offset] != notation::ECurlyChar) {
                ObjectT *obj = value.GetObject();

                while (offset < end && (content[offset] == notation::QuoteChar)) {
                    ++offset;
                    const Char_T *str = (content + offset);
                    SizeT         len = JSONUtils::UnEscape(str, end, stream);

                    if (len != 0) {
                        offset += len;
                        --len;

                        if (stream.IsNotEmpty()) {
                            str = stream.First();
                            len = stream.Length();
                            stream.Clear();
                        }

                        StringUtils::TrimLeft(content, offset, end);

                        if (content[offset] == notation::ColonChar) {
                            ++offset;
                            StringUtils::TrimLeft(content, offset, end);
                            String<Char_T> key{str, len};
                            obj->Insert(Memory::Move(key), parseValue(stream, content, offset, end));
                            StringUtils::TrimLeft(content, offset, end);

                            if (offset < end) {
                                const Char_T c = content[offset];

                                if (c == notation::CommaChar) {
                                    ++offset;
                                    StringUtils::TrimLeft(content, offset, end);
                                    continue;
                                }

                                if (c == notation::ECurlyChar) {
                                    ++offset;
                                    return value;
                                }
                            }
                        }
                    }

                    break;
                }

                value.Reset();
            }

            ++offset;
            return value;
        }

        static ValueT parseArray(Stream_T &stream, const Char_T *content, SizeT &offset, const SizeT end) {
            StringUtils::TrimLeft(content, offset, end);

            ValueT value{ValueType::Array};

            if (content[offset] != notation::ESquareChar) {
                Array<ValueT> *arr = value.GetArray();

                while (offset < end) {
                    *arr += parseValue(stream, content, offset, end);
                    StringUtils::TrimLeft(content, offset, end);

                    if (offset < end) {
                        const Char_T ch = content[offset];

                        if (ch == notation::CommaChar) {
                            ++offset;
                            StringUtils::TrimLeft(content, offset, end);
                            continue;
                        }

                        if (ch == notation::ESquareChar) {
                            ++offset;
                            return value;
                        }
                    }

                    break;
                }

                value.Reset();
            }

            ++offset;
            return value;
        }

        static ValueT parseValue(Stream_T &stream, const Char_T *content, SizeT &offset, const SizeT end) {
            switch (content[offset]) {
                case notation::SCurlyChar: {
                    ++offset;
                    return parseObject(stream, content, offset, end);
                }

                case notation::SSquareChar: {
                    ++offset;
                    return parseArray(stream, content, offset, end);
                }

                case notation::QuoteChar: {
                    ++offset;

                    const Char_T *str = (content + offset);
                    SizeT         len = JSONUtils::UnEscape(str, (end - offset), stream);

                    if (len != 0) {
                        offset += len;
                        --len;

                        if (stream.IsNotEmpty()) {
                            str = stream.First();
                            len = stream.Length();
                            stream.Clear();
                        }

                        return ValueT{String<Char_T>{str, len}};
                    }

                    break;
                }

                case notation::T_Char: {
                    const Char_T *true_string = (notation::TrueString + 1U);

                    ++offset;

                    while ((offset < end) && (content[offset] == *true_string)) {
                        ++true_string;
                        ++offset;
                    }

                    if (*true_string == Char_T{0}) {
                        return ValueT{ValueType::True};
                    }

                    break;
                }

                case notation::F_Char: {
                    const Char_T *false_string = (notation::FalseString + 1U);

                    ++offset;

                    while ((offset < end) && (content[offset] == *false_string)) {
                        ++false_string;
                        ++offset;
                    }

                    if (*false_string == Char_T{0}) {
                        return ValueT{ValueType::False};
                    }

                    break;
                }

                case notation::N_Char: {
                    const Char_T *null_string = (notation::NullString + 1U);

                    ++offset;

                    while ((offset < end) && (content[offset] == *null_string)) {
                        ++null_string;
                        ++offset;
                    }

                    if (*null_string == Char_T{0}) {
                        return ValueT{ValueType::Null};
                    }

                    break;
                }

                default: {
                    QNumber64 number;

                    switch (Digit::StringToNumber(number, content, offset, end)) {
                        case QNumberType::Natural: {
                            return ValueT{number.Natural};
                        }

                        case QNumberType::Integer: {
                            return ValueT{number.Integer};
                        }

                        case QNumberType::Real: {
                            return ValueT{number.Real};
                        }

                        default: {
                        }
                    };
                }
            }

            offset = end;
            return ValueT{};
        }
    };
};

} // namespace Qentem

#endif
