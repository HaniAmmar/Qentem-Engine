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
    Value<Char_T_> value;
    JSONParser<Char_T_>::Parse(value, content, length);
    return value;
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

    inline static void Parse(VValue &value, const Char_T_ *content,
                             UInt length) {
        UInt offset = 0;

        while (offset < length) {
            switch (content[offset]) {
                case JSONotation_T_::SCurlyChar:
                case JSONotation_T_::SSquareChar: {
                    while (offset != length) {
                        --length;

                        switch (content[length]) {
                            case JSONotation_T_::CommaChar:
                            case JSONotation_T_::QuoteChar: {
                                return;
                            }

                            case JSONotation_T_::ECurlyChar:
                            case JSONotation_T_::ESquareChar: {
                                const bool obj_child =
                                    (content[offset] ==
                                     JSONotation_T_::SCurlyChar);

                                if ((obj_child &&
                                     (content[length] ==
                                      JSONotation_T_::ECurlyChar)) ||
                                    ((content[offset] ==
                                      JSONotation_T_::SSquareChar) &&
                                     (content[length] ==
                                      JSONotation_T_::ESquareChar))) {

                                    ++offset;
                                    ++length;

                                    JSONParser jp;

                                    if (obj_child) {
                                        value = jp.parseObject(content, offset,
                                                               length);
                                    } else {
                                        value = jp.parseArray(content, offset,
                                                              length);
                                    }

                                    if (offset == length) {
                                        return;
                                    }

                                    value.Reset();
                                }

                                return;
                            }
                        }
                    }

                    return;
                }
            }

            ++offset;
        }
    }

  private:
    using JSONotation_T_ = JSONotation<Char_T_>;
    using VObject        = HArray<VValue, Char_T_>;
    using VArray         = Array<VValue>;
    using VString        = String<Char_T_>;

#ifdef QENTEM_SIMD_ENABLED_
    // TODO: Add 16 and 32-bit character
    QENTEM_NOINLINE void SIMDFind(const Char_T_ *content, SizeT offset,
                                  SizeT length) noexcept {
        while ((offset < length) && (find_cache_.Bits == 0)) {
            find_cache_.Offset     = offset;
            find_cache_.NextOffset = (find_cache_.Offset + QMM_SIZE_);
            offset                 = find_cache_.NextOffset;

            const QMM_VAR_ m_content =
                QMM_LOAD_(reinterpret_cast<const QMM_VAR_ *>(
                    content + find_cache_.Offset));

            // The value of 8 characters:
            constexpr unsigned long long curly    = 8897841259083430779ULL;
            constexpr unsigned long long square   = 6582955728264977243ULL;
            constexpr unsigned long long colon    = 4195730024608447034ULL;
            constexpr unsigned long long quote    = 2459565876494606882ULL;
            constexpr unsigned long long comma    = 3182967604875373612ULL;
            constexpr unsigned long long e_curly  = 9042521604759584125ULL;
            constexpr unsigned long long e_square = 6727636073941130589ULL;

            find_cache_.Bits =
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(curly));
            find_cache_.Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(square));

            find_cache_.Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(colon));
            find_cache_.Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(quote));
            find_cache_.Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(comma));

            find_cache_.Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(e_curly));
            find_cache_.Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(e_square));
        }
    }
#endif

    VValue parseObject(const Char_T_ *content, SizeT &offset, SizeT length) {
        VValue  value;
        VObject obj;
        VValue *obj_value = nullptr;

        SizeT previous_offset = offset;
        bool  pass_comma      = false;
        bool  has_colon       = false;
        bool  is_valid        = false;

#ifndef QENTEM_SIMD_ENABLED_
        while (offset < length) {
#else
        constexpr QMM_Number_T simd_one = 1;

        do {
            if (find_cache_.Bits == 0) {
                SIMDFind(content, offset, length);

                if (find_cache_.Bits == 0) {
                    break;
                }
            }

            SizeT index = Platform::CTZ(find_cache_.Bits);
            find_cache_.Bits ^= (simd_one << index);
            index += find_cache_.Offset;

            if (index >= length) {
                break;
            }

            if (index >= offset) {
                offset = index;
#endif
            switch (content[offset]) {
                case JSONotation_T_::SCurlyChar:
                case JSONotation_T_::SSquareChar: {
                    const bool obj_child =
                        (content[offset] == JSONotation_T_::SCurlyChar);
                    StringUtils::StartTrim(content, previous_offset, offset);

                    if (!pass_comma && has_colon &&
                        (previous_offset == offset)) {
                        ++offset;

                        if (obj_child) {
                            *obj_value = parseObject(content, offset, length);
                        } else {
                            *obj_value = parseArray(content, offset, length);
                        }

                        previous_offset = offset;
                        obj_value       = nullptr;
                        pass_comma      = true;
                        has_colon       = false;

                        if (offset != 0) {
                            continue;
                        }
                    }

                    offset = 0;
                    return value;
                }

                case JSONotation_T_::QuoteChar: {
                    StringUtils::StartTrim(content, previous_offset, offset);

                    if (!pass_comma && (previous_offset == offset)) {
                        ++offset;

                        const Char_T_ *str = (content + offset);
                        const SizeT    len = UnEscapeJSON(str, length, buffer_);
                        offset += len;
                        previous_offset = offset;

                        if (obj_value == nullptr) {
                            obj_value = &(obj[VString{buffer_.First(),
                                                      buffer_.Length()}]);
                        } else if (has_colon) {
                            *obj_value =
                                VValue{buffer_.First(), buffer_.Length()};
                            obj_value  = nullptr;
                            pass_comma = true;
                            has_colon  = false;
                        }

                        buffer_.Clear();

                        if (len != 0) {
                            continue;
                        }
                    }

                    offset = 0;
                    return value;
                }

                case JSONotation_T_::CommaChar: {
                    SizeT len = (offset - previous_offset);
                    StringUtils::SoftTrim(content, previous_offset, len);

                    if (pass_comma && (len == 0)) {
                        pass_comma = false;
                        ++offset;
                        previous_offset = offset;
                        continue;
                    }

                    if ((obj_value == nullptr) || !has_colon) {
                        return value;
                    }

                    *obj_value = trueFalseNullNumber(is_valid, content,
                                                     previous_offset, len);
                    obj_value  = nullptr;
                    has_colon  = false;
                    ++offset;
                    previous_offset = offset;

                    if (is_valid) {
                        continue;
                    }

                    offset = 0;
                    return value;
                }

                case JSONotation_T_::ColonChar: {
                    StringUtils::StartTrim(content, previous_offset, offset);

                    if (!has_colon && (previous_offset == offset)) {
                        has_colon = true;
                        ++offset;
                        previous_offset = offset;

                        if (obj_value != nullptr) {
                            continue;
                        }
                    }

                    offset = 0;
                    return value;
                }

                case JSONotation_T_::ECurlyChar: {
                    const bool has_comma = (content[previous_offset - 1] ==
                                            JSONotation_T_::CommaChar);

                    SizeT len = (offset - previous_offset);
                    StringUtils::SoftTrim(content, previous_offset, len);

                    if ((obj_value != nullptr) || has_comma || (len != 0)) {
                        if (obj_value != nullptr) {
                            *obj_value = trueFalseNullNumber(
                                is_valid, content, previous_offset, len);
                        }

                        if (!is_valid || !has_colon) {
                            offset = 0;
                            return value;
                        }
                    }

                    ++offset;
                    value = static_cast<VObject &&>(obj);
                    return value;
                }
            }
#ifndef QENTEM_SIMD_ENABLED_
            ++offset;
        }
#else
            }
        } while (true);
#endif

        offset = 0;
        return value;
    }

    VValue parseArray(const Char_T_ *content, SizeT &offset, SizeT length) {
        VValue value;
        VArray arr;

        SizeT previous_offset = offset;
        bool  pass_comma      = false;
        bool  is_valid        = false;

#ifndef QENTEM_SIMD_ENABLED_
        while (offset < length) {
#else
        constexpr QMM_Number_T simd_one = 1;

        do {
            if (find_cache_.Bits == 0) {
                SIMDFind(content, offset, length);

                if (find_cache_.Bits == 0) {
                    break;
                }
            }

            SizeT index = Platform::CTZ(find_cache_.Bits);
            find_cache_.Bits ^= (simd_one << index);
            index += find_cache_.Offset;

            if (index >= length) {
                break;
            }

            if (index >= offset) {
                offset = index;
#endif
            switch (content[offset]) {
                case JSONotation_T_::SCurlyChar:
                case JSONotation_T_::SSquareChar: {
                    const bool obj_child =
                        (content[offset] == JSONotation_T_::SCurlyChar);
                    StringUtils::StartTrim(content, previous_offset, offset);

                    if (!pass_comma && (previous_offset == offset)) {
                        ++offset;

                        if (obj_child) {
                            arr += parseObject(content, offset, length);
                        } else {
                            arr += parseArray(content, offset, length);
                        }

                        pass_comma      = true;
                        previous_offset = offset;

                        if (offset != 0) {
                            continue;
                        }
                    }

                    offset = 0;
                    return value;
                }

                case JSONotation_T_::QuoteChar: {
                    if (!pass_comma) {
                        ++offset;
                        buffer_.Clear();
                        const Char_T_ *str = (content + offset);
                        const SizeT    len = UnEscapeJSON(str, length, buffer_);

                        arr += VValue{buffer_.First(), buffer_.Length()};

                        offset += len;
                        previous_offset = offset;
                        pass_comma      = true;

                        if (len != 0) {
                            continue;
                        }
                    }

                    offset = 0;
                    return value;
                }

                case JSONotation_T_::CommaChar: {
                    SizeT len = (offset - previous_offset);
                    StringUtils::SoftTrim(content, previous_offset, len);

                    if (pass_comma && (len == 0)) {
                        pass_comma = false;
                        ++offset;
                        previous_offset = offset;
                        continue;
                    }

                    arr += trueFalseNullNumber(is_valid, content,
                                               previous_offset, len);
                    ++offset;
                    previous_offset = offset;

                    if (is_valid) {
                        continue;
                    }

                    offset = 0;
                    return value;
                }

                case JSONotation_T_::ESquareChar: {
                    const bool has_comma = (content[previous_offset - 1] ==
                                            JSONotation_T_::CommaChar);

                    SizeT len = (offset - previous_offset);
                    StringUtils::SoftTrim(content, previous_offset, len);

                    if (has_comma || (len != 0)) {
                        arr += trueFalseNullNumber(is_valid, content,
                                                   previous_offset, len);

                        if (!is_valid || pass_comma) {
                            offset = 0;
                            return value;
                        }
                    }

                    ++offset;
                    arr.Compress();
                    value = static_cast<VArray &&>(arr);
                    return value;
                }
            }
#ifndef QENTEM_SIMD_ENABLED_
            ++offset;
        }
#else
            }
        } while (true);
#endif

        offset = 0;
        return value;
    }

    static VValue trueFalseNullNumber(bool &is_valid, const Char_T_ *content,
                                      SizeT offset, SizeT len) noexcept {
        static const Char_T_ *true_string  = JSONotation_T_::GetTrueString();
        static const Char_T_ *false_string = JSONotation_T_::GetFalseString();
        static const Char_T_ *null_string  = JSONotation_T_::GetNullString();
        SizeT                 tmp_offset   = 0;

        switch (content[offset]) {
            case JSONotation_T_::T_Char: {
                if (len == JSONotation_T_::TrueStringLength) {
                    do {
                        ++offset;
                        ++tmp_offset;
                    } while ((content[offset] == true_string[tmp_offset]));

                    if (tmp_offset == JSONotation_T_::TrueStringLength) {
                        is_valid = true;
                        return VValue{true};
                    }
                }

                break;
            }

            case JSONotation_T_::F_Char: {
                if (len == JSONotation_T_::FalseStringLength) {
                    do {
                        ++offset;
                        ++tmp_offset;
                    } while ((content[offset] == false_string[tmp_offset]));

                    if (tmp_offset == JSONotation_T_::FalseStringLength) {
                        is_valid = true;
                        return VValue{false};
                    }
                }

                break;
            }

            case JSONotation_T_::N_Char: {
                if (len == JSONotation_T_::NullStringLength) {
                    do {
                        ++offset;
                        ++tmp_offset;
                    } while ((content[offset] == null_string[tmp_offset]));

                    if (tmp_offset == JSONotation_T_::NullStringLength) {
                        is_valid = true;
                        return VValue{nullptr};
                    }
                }

                break;
            }

            default: {
                double num;

                if (Digit<Char_T_>::StringToNumber(num, (content + offset),
                                                   len)) {
                    is_valid = true;
                    return VValue{num};
                }
            }
        }

        is_valid = false;
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
