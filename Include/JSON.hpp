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

#ifndef QENTEM_JSON_H
#define QENTEM_JSON_H

#include "Value.hpp"

namespace Qentem {

struct JSON {
    template <typename, typename>
    struct JSONParser;

    template <typename Char_T, typename Number_T, typename Stream_T>
    inline static Value<Char_T> Parse(Stream_T &stream, const Char_T *content, Number_T length) {
        return JSONParser<Char_T, Stream_T>::Parse(stream, content, SizeT(length));
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
    struct JSONParser {
        using ValueT = Value<Char_T>;

        JSONParser()                              = delete;
        JSONParser(JSONParser &&)                 = delete;
        JSONParser(const JSONParser &)            = delete;
        JSONParser &operator=(JSONParser &&)      = delete;
        JSONParser &operator=(const JSONParser &) = delete;
        ~JSONParser()                             = delete;

        static ValueT Parse(Stream_T &stream, const Char_T *content, SizeT length) {
            if (length != SizeT{0}) {
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
        using JSONotation = JSONUtils::JSONotation_T<Char_T>;

        static ValueT parseObject(Stream_T &stream, const Char_T *content, SizeT &offset, const SizeT length) {
            StringUtils::TrimLeft(content, offset, length);

            ValueT value{ValueType::Object};

            if (content[offset] != JSONotation::ECurlyChar) {
                HArray<ValueT, Char_T> *obj = value.GetObject();

                while (offset < length && (content[offset] == JSONotation::QuoteChar)) {
                    ++offset;
                    const Char_T *str = (content + offset);
                    SizeT         len = JSONUtils::UnEscapeJSON(str, length, stream);

                    if (len != SizeT{0}) {
                        offset += len;
                        --len;

                        if (stream.IsNotEmpty()) {
                            str = stream.First();
                            len = stream.Length();
                            stream.Clear();
                        }

                        StringUtils::TrimLeft(content, offset, length);

                        if (content[offset] == JSONotation::ColonChar) {
                            ++offset;
                            StringUtils::TrimLeft(content, offset, length);
                            String<Char_T> key{str, len};
                            obj->Insert(Memory::Move(key), parseValue(stream, content, offset, length));
                            StringUtils::TrimLeft(content, offset, length);

                            if (offset < length) {
                                const Char_T c = content[offset];

                                if (c == JSONotation::CommaChar) {
                                    ++offset;
                                    StringUtils::TrimLeft(content, offset, length);
                                    continue;
                                }

                                if (c == JSONotation::ECurlyChar) {
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

        static ValueT parseArray(Stream_T &stream, const Char_T *content, SizeT &offset, const SizeT length) {
            StringUtils::TrimLeft(content, offset, length);

            ValueT value{ValueType::Array};

            if (content[offset] != JSONotation::ESquareChar) {
                Array<ValueT> *arr = value.GetArray();

                while (offset < length) {
                    arr->Insert(parseValue(stream, content, offset, length));
                    StringUtils::TrimLeft(content, offset, length);

                    if (offset < length) {
                        const Char_T ch = content[offset];

                        if (ch == JSONotation::CommaChar) {
                            ++offset;
                            StringUtils::TrimLeft(content, offset, length);
                            continue;
                        }

                        if (ch == JSONotation::ESquareChar) {
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

        static ValueT parseValue(Stream_T &stream, const Char_T *content, SizeT &offset, const SizeT length) {
            switch (content[offset]) {
                case JSONotation::SCurlyChar: {
                    ++offset;
                    return parseObject(stream, content, offset, length);
                }

                case JSONotation::SSquareChar: {
                    ++offset;
                    return parseArray(stream, content, offset, length);
                }

                case JSONotation::QuoteChar: {
                    ++offset;

                    const Char_T *str = (content + offset);
                    SizeT         len = JSONUtils::UnEscapeJSON(str, (length - offset), stream);

                    if (len != SizeT{0}) {
                        offset += len;
                        --len;

                        if (stream.IsNotEmpty()) {
                            str = stream.First();
                            len = stream.Length();
                            stream.Clear();
                        }

                        return ValueT{str, len};
                    }

                    break;
                }

                case JSONotation::T_Char: {
                    const Char_T *true_string = (JSONotation::TrueString + 1U);

                    ++offset;

                    while ((offset < length) && (content[offset] == *true_string)) {
                        ++true_string;
                        ++offset;
                    }

                    if (*true_string == Char_T{0}) {
                        return ValueT{ValueType::True};
                    }

                    break;
                }

                case JSONotation::F_Char: {
                    const Char_T *false_string = (JSONotation::FalseString + 1U);

                    ++offset;

                    while ((offset < length) && (content[offset] == *false_string)) {
                        ++false_string;
                        ++offset;
                    }

                    if (*false_string == Char_T{0}) {
                        return ValueT{ValueType::False};
                    }

                    break;
                }

                case JSONotation::N_Char: {
                    const Char_T *null_string = (JSONotation::NullString + 1U);

                    ++offset;

                    while ((offset < length) && (content[offset] == *null_string)) {
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

                    switch (Digit::StringToNumber(number, content, offset, length)) {
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

            offset = length;
            return ValueT{};
        }
    };
};

} // namespace Qentem

#endif
