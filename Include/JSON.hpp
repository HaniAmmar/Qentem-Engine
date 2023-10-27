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

#include "Value.hpp"

#ifndef QENTEM_JSON_H_
#define QENTEM_JSON_H_

namespace Qentem {
struct JSON {
    template <typename, typename>
    struct JSONParser;

    template <typename Char_T_, typename Number_T_, typename Stream_T_>
    inline static Value<Char_T_> Parse(Stream_T_ &stream, const Char_T_ *content, Number_T_ length) {
        return JSONParser<Char_T_, Stream_T_>::Parse(stream, content, static_cast<SizeT>(length));
    }

    template <typename Char_T_, typename Number_T_>
    inline static Value<Char_T_> Parse(const Char_T_ *content, Number_T_ length) {
        StringStream<Char_T_> stream{};
        return Parse(stream, content, static_cast<SizeT>(length));
    }

    template <typename Char_T_>
    inline static Value<Char_T_> Parse(const Char_T_ *content) {
        return Parse(content, StringUtils::Count(content));
    }

    template <typename Char_T_, typename Stream_T_>
    struct JSONParser {
        using VValue = Value<Char_T_>;

      public:
        JSONParser()                              = delete;
        JSONParser(JSONParser &&)                 = delete;
        JSONParser(const JSONParser &)            = delete;
        JSONParser &operator=(JSONParser &&)      = delete;
        JSONParser &operator=(const JSONParser &) = delete;
        ~JSONParser()                             = delete;

        static VValue Parse(Stream_T_ &stream, const Char_T_ *content, SizeT length) {
            VValue value;

            if (length != 0) {
                SizeT offset = 0;
                StringUtils::TrimLeft(content, offset, length);
                parseValue(value, stream, content, offset, length);
                StringUtils::TrimLeft(content, offset, length);

                if (offset != length) {
                    value.Reset();
                }
            }

            return value;
        }

      private:
        using JSONotation = JSONotation_T_<Char_T_>;
        using VObject     = HArray<VValue, Char_T_>;
        using VArray      = Array<VValue>;
        using VString     = String<Char_T_>;

        static void parseObject(VValue &value, Stream_T_ &stream, const Char_T_ *content, SizeT &offset,
                                const SizeT length) {
            StringUtils::TrimLeft(content, offset, length);

            if (content[offset] != JSONotation::ECurlyChar) {
                VObject obj;

                while (offset < length) {
                    if (content[offset] != JSONotation::QuoteChar) {
                        offset = length;
                        break;
                    }

                    ++offset;
                    const Char_T_ *str = (content + offset);
                    SizeT          len = UnEscapeJSON(str, length, stream);

                    if (len == 0) {
                        break;
                    }

                    offset += len;
                    --len;

                    if (stream.IsNotEmpty()) {
                        str = stream.First();
                        len = stream.Length();
                        stream.Clear();
                    }

                    VValue &obj_value = obj[VString{str, len}];
                    StringUtils::TrimLeft(content, offset, length);

                    if (content[offset] != JSONotation::ColonChar) {
                        break;
                    }

                    ++offset;
                    StringUtils::TrimLeft(content, offset, length);
                    parseValue(obj_value, stream, content, offset, length);
                    StringUtils::TrimLeft(content, offset, length);

                    const Char_T_ c = content[offset];

                    if (c == JSONotation::ECurlyChar) {
                        ++offset;
                        value = static_cast<VObject &&>(obj);
                        break;
                    }

                    if (c != JSONotation::CommaChar) {
                        break;
                    }

                    ++offset;
                    StringUtils::TrimLeft(content, offset, length);
                }
            } else {
                ++offset;
                value = VValue{ValueType::Object};
            }
        }

        static void parseArray(VValue &value, Stream_T_ &stream, const Char_T_ *content, SizeT &offset,
                               const SizeT length) {
            StringUtils::TrimLeft(content, offset, length);

            if (content[offset] != JSONotation::ESquareChar) {
                VArray arr;

                while (offset < length) {
                    parseValue(value, stream, content, offset, length);
                    arr += static_cast<VValue &&>(value);
                    StringUtils::TrimLeft(content, offset, length);

                    const Char_T_ c = content[offset];

                    if (c == JSONotation::ESquareChar) {
                        ++offset;
                        // arr.Compress();
                        value = static_cast<VArray &&>(arr);
                        break;
                    }

                    if (c != JSONotation::CommaChar) {
                        break;
                    }

                    ++offset;
                    StringUtils::TrimLeft(content, offset, length);
                }
            } else {
                ++offset;
                value = VValue{ValueType::Array};
            }
        }

        static void parseValue(VValue &value, Stream_T_ &stream, const Char_T_ *content, SizeT &offset,
                               const SizeT length) {
            switch (content[offset]) {
                case JSONotation::SCurlyChar: {
                    ++offset;
                    parseObject(value, stream, content, offset, length);
                    return;
                }

                case JSONotation::SSquareChar: {
                    ++offset;
                    parseArray(value, stream, content, offset, length);
                    return;
                }

                case JSONotation::QuoteChar: {
                    ++offset;

                    const Char_T_ *str = (content + offset);
                    SizeT          len = UnEscapeJSON(str, (length - offset), stream);

                    if (len == 0) {
                        break;
                    }

                    offset += len;
                    --len;

                    if (stream.IsNotEmpty()) {
                        str = stream.First();
                        len = stream.Length();
                        stream.Clear();
                    }

                    value = VString{str, len};
                    return;
                }

                case JSONotation::T_Char: {
                    const Char_T_ *true_string = JSONotation::TrueString;

                    do {
                    } while ((content[++offset] == *(++true_string)));

                    if (*(true_string) == 0) {
                        value = true;
                        return;
                    }

                    break;
                }

                case JSONotation::F_Char: {
                    const Char_T_ *false_string = JSONotation::FalseString;

                    do {
                    } while ((content[++offset] == *(++false_string)));

                    if (*(false_string) == 0) {
                        value = false;
                        return;
                    }

                    break;
                }

                case JSONotation::N_Char: {
                    const Char_T_ *null_string = JSONotation::NullString;

                    do {
                    } while ((content[++offset] == *(++null_string)));

                    if (*(null_string) == 0) {
                        value = nullptr;
                        return;
                    }

                    break;
                }

                default: {
                    QNumber number;

                    switch (Digit<Char_T_>::StringToNumber(number, content, offset, length)) {
                        case 1: {
                            value = number.Natural;
                            return;
                        }

                        case 2: {
                            value = number.Integer;
                            return;
                        }

                        case 3: {
                            value = number.Real;
                            return;
                        }

                        default: {
                        }
                    };
                }
            }

            offset = length;
        }
    };
};
} // namespace Qentem

#endif
