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

#include "Value.hpp"

#ifndef _QENTEM_JSON_H
#define _QENTEM_JSON_H

namespace Qentem {
struct JSON {
    template <typename, typename>
    struct JSONParser;

    template <typename _Char_T, typename _Number_T, typename _Stream_T>
    inline static Value<_Char_T> Parse(_Stream_T &stream, const _Char_T *content, _Number_T length) {
        return JSONParser<_Char_T, _Stream_T>::Parse(stream, content, SizeT(length));
    }

    template <typename _Char_T, typename _Number_T>
    inline static Value<_Char_T> Parse(const _Char_T *content, _Number_T length) {
        StringStream<_Char_T> stream{};
        return Parse(stream, content, SizeT(length));
    }

    template <typename _Char_T>
    inline static Value<_Char_T> Parse(const _Char_T *content) {
        return Parse(content, StringUtils::Count(content));
    }

    template <typename _Char_T, typename _Stream_T>
    struct JSONParser {
        using VValue = Value<_Char_T>;

        JSONParser()                              = delete;
        JSONParser(JSONParser &&)                 = delete;
        JSONParser(const JSONParser &)            = delete;
        JSONParser &operator=(JSONParser &&)      = delete;
        JSONParser &operator=(const JSONParser &) = delete;
        ~JSONParser()                             = delete;

        static VValue Parse(_Stream_T &stream, const _Char_T *content, SizeT length) {
            VValue value{};

            if (length != SizeT{0}) {
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
        using JSONotation = _JSONotation_T<_Char_T>;
        using VObject     = HArray<VValue, _Char_T>;
        using VArray      = Array<VValue>;
        using VString     = String<_Char_T>;

        static void parseObject(VValue &value, _Stream_T &stream, const _Char_T *content, SizeT &offset,
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
                    const _Char_T *str = (content + offset);
                    SizeT          len = UnEscapeJSON(str, length, stream);

                    if (len == SizeT{0}) {
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

                    const _Char_T c = content[offset];

                    if (c == JSONotation::ECurlyChar) {
                        ++offset;
                        value = Memory::Move(obj);
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

        static void parseArray(VValue &value, _Stream_T &stream, const _Char_T *content, SizeT &offset,
                               const SizeT length) {
            StringUtils::TrimLeft(content, offset, length);

            if (content[offset] != JSONotation::ESquareChar) {
                VArray arr;

                while (offset < length) {
                    parseValue(value, stream, content, offset, length);
                    arr += Memory::Move(value);
                    StringUtils::TrimLeft(content, offset, length);

                    const _Char_T c = content[offset];

                    if (c == JSONotation::ESquareChar) {
                        ++offset;
                        // arr.Compress();
                        value = Memory::Move(arr);
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

        static void parseValue(VValue &value, _Stream_T &stream, const _Char_T *content, SizeT &offset,
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

                    const _Char_T *str = (content + offset);
                    SizeT          len = UnEscapeJSON(str, (length - offset), stream);

                    if (len == SizeT{0}) {
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
                    const _Char_T *true_string = JSONotation::TrueString;

                    do {
                    } while ((content[++offset] == *(++true_string)));

                    if (*(true_string) == _Char_T{0}) {
                        value = true;
                        return;
                    }

                    break;
                }

                case JSONotation::F_Char: {
                    const _Char_T *false_string = JSONotation::FalseString;

                    do {
                    } while ((content[++offset] == *(++false_string)));

                    if (*(false_string) == _Char_T{0}) {
                        value = false;
                        return;
                    }

                    break;
                }

                case JSONotation::N_Char: {
                    const _Char_T *null_string = JSONotation::NullString;

                    do {
                    } while ((content[++offset] == *(++null_string)));

                    if (*(null_string) == _Char_T{0}) {
                        value = nullptr;
                        return;
                    }

                    break;
                }

                default: {
                    QNumber64 number;

                    switch (Digit::StringToNumber(number, content, offset, length)) {
                        case QNumberType::Natural: {
                            value = number.Natural;
                            return;
                        }

                        case QNumberType::Integer: {
                            value = number.Integer;
                            return;
                        }

                        case QNumberType::Real: {
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
