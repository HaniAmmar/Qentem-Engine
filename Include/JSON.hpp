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

#include "Platform.hpp"
#include "Value.hpp"

#ifndef QENTEM_JSON_H_
#define QENTEM_JSON_H_

namespace Qentem {
namespace JSON {

template <typename>
class JSONParser;

template <typename Char_T_>
inline static Value<Char_T_> Parse(const Char_T_ *content, SizeT length) {
    return JSONParser<Char_T_>::Parse(content, length);
}

template <typename Char_T_, typename Number_T>
inline static Value<Char_T_> Parse(const Char_T_ *content, Number_T length) {
    return Parse(content, static_cast<SizeT>(length));
}

template <typename Char_T_>
inline static Value<Char_T_> Parse(const Char_T_ *content) {
    return Parse(content, StringUtils::Count(content));
}

template <typename Char_T_>
class JSONParser {
    struct FindCache_T_;

    using VValue = Value<Char_T_>;

  public:
    JSONParser() = default;

    inline static VValue Parse(const Char_T_ *content, SizeT length) {
        JSONParser jp;
        SizeT      offset = 0;
        VValue     value;

        while (offset < length) {
            if ((content[offset] == JSONotation_T_::SCurlyChar) ||
                (content[offset] == JSONotation_T_::SSquareChar)) {
                while (offset != length) {
                    --length;

                    switch (content[length]) {
                        case JSONotation_T_::CommaChar:
                        case JSONotation_T_::QuoteChar: {
                            return value;
                        }

                        case JSONotation_T_::ECurlyChar:
                        case JSONotation_T_::ESquareChar: {
                            const bool obj_child =
                                (content[offset] == JSONotation_T_::SCurlyChar);
                            ++length;

                            if (obj_child) {
                                value = jp.parseObject(content, offset, length);
                            } else {
                                value = jp.parseArray(content, offset, length);
                            }

                            if (offset == length) {
                                break;
                            }

                            return VValue{};
                        }
                    }
                }
            }

            ++offset;
        }

        return value;
    }

  private:
    using JSONotation_T_ = JSONotation<Char_T_>;
    using VObject        = HArray<VValue, Char_T_>;
    using VArray         = Array<VValue>;
    using VString        = String<Char_T_>;

    VValue parseObject(const Char_T_ *content, SizeT &offset, SizeT length) {
        VObject obj;

        ++offset;
        StringUtils::StartTrim(content, offset, length);

        if (content[offset] == JSONotation_T_::ECurlyChar) {
            ++offset;
            return VValue{ValueType::Object};
        }

        while (offset < length) {
            if (content[offset] != JSONotation_T_::QuoteChar) {
                offset = length;
                break;
            }

            ++offset;
            const Char_T_ *str = (content + offset);
            SizeT          len = UnEscapeJSON(str, length, buffer_);

            if (len == 0) {
                break;
            }

            offset += len;
            --len;

            if (buffer_.IsNotEmpty()) {
                str = buffer_.First();
                len = buffer_.Length();
                buffer_.Clear();
            }

            VValue &obj_value = obj[VString{str, len}];
            StringUtils::StartTrim(content, offset, length);

            if (content[offset] != JSONotation_T_::ColonChar) {
                break;
            }

            ++offset;
            StringUtils::StartTrim(content, offset, length);
            obj_value = parseValue(content, offset, length);
            StringUtils::StartTrim(content, offset, length);

            if ((content[offset] != JSONotation_T_::CommaChar) ||
                (content[offset] == JSONotation_T_::ECurlyChar)) {
                break;
            }

            ++offset;
            StringUtils::StartTrim(content, offset, length);
        }

        ++offset;
        return VValue{static_cast<VObject &&>(obj)};
    }

    VValue parseArray(const Char_T_ *content, SizeT &offset, SizeT length) {
        VArray arr;

        ++offset;
        StringUtils::StartTrim(content, offset, length);

        if (content[offset] != JSONotation_T_::ESquareChar) {
            while (offset < length) {
                arr += parseValue(content, offset, length);
                StringUtils::StartTrim(content, offset, length);

                if ((content[offset] != JSONotation_T_::CommaChar) ||
                    (content[offset] == JSONotation_T_::ESquareChar)) {
                    break;
                }

                ++offset;
                StringUtils::StartTrim(content, offset, length);
            }
        }

        ++offset;
        arr.Compress();
        return VValue{static_cast<VArray &&>(arr)};
    }

    VValue parseValue(const Char_T_ *content, SizeT &offset, SizeT length) {
        static const Char_T_ *true_string  = JSONotation_T_::GetTrueString();
        static const Char_T_ *false_string = JSONotation_T_::GetFalseString();
        static const Char_T_ *null_string  = JSONotation_T_::GetNullString();
        SizeT                 tmp_offset   = 0;

        switch (content[offset]) {
            case JSONotation_T_::SCurlyChar: {
                return parseObject(content, offset, length);
            }

            case JSONotation_T_::SSquareChar: {
                return parseArray(content, offset, length);
            }

            case JSONotation_T_::QuoteChar: {
                ++offset;

                const Char_T_ *str = (content + offset);
                SizeT          len = UnEscapeJSON(str, length, buffer_);

                if (len == 0) {
                    break;
                }

                offset += len;
                --len;

                if (buffer_.IsNotEmpty()) {
                    str = buffer_.First();
                    len = buffer_.Length();
                    buffer_.Clear();
                }

                return VValue{str, len};
            }

            case JSONotation_T_::T_Char: {
                do {
                    ++offset;
                    ++tmp_offset;
                } while ((content[offset] == true_string[tmp_offset]));

                if (tmp_offset == JSONotation_T_::TrueStringLength) {
                    return VValue{true};
                }

                break;
            }

            case JSONotation_T_::F_Char: {
                do {
                    ++offset;
                    ++tmp_offset;
                } while ((content[offset] == false_string[tmp_offset]));

                if (tmp_offset == JSONotation_T_::FalseStringLength) {
                    return VValue{false};
                }

                break;
            }

            case JSONotation_T_::N_Char: {
                do {
                    ++offset;
                    ++tmp_offset;
                } while ((content[offset] == null_string[tmp_offset]));

                if (tmp_offset == JSONotation_T_::NullStringLength) {
                    return VValue{nullptr};
                }

                break;
            }

            default: {
                // bool is_float = false;
                // bool is_negative   = false;

                const SizeT num_offset = offset;

                while (offset < length) {
                    switch (content[offset]) {
                        case JSONotation_T_::SpaceChar:
                        case JSONotation_T_::LineControlChar:
                        case JSONotation_T_::TabControlChar:
                        case JSONotation_T_::CarriageControlChar:
                        case JSONotation_T_::CommaChar:
                        case JSONotation_T_::ECurlyChar:
                        case JSONotation_T_::ESquareChar: {
                            double num;

                            if (Digit<Char_T_>::StringToNumber(
                                    num, (content + num_offset),
                                    (offset - num_offset))) {
                                return VValue{num};
                            }

                            offset = length;
                        }
                    }

                    ++offset;
                }
            }
        }

        offset = length;
        return VValue();
    }

    StringStream<Char_T_> buffer_{};
#ifdef QENTEM_SIMD_ENABLED_
    struct FindCache_T_ {
        SizeT        Offset{0};
        SizeT        NextOffset{0};
        QMM_Number_T Bits{0};
    } find_cache_{};
#endif
};

} // namespace JSON
} // namespace Qentem

#endif
