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

#include "Engine.hpp"
#include "Value.hpp"

#ifndef QENTEM_JSON_H_
#define QENTEM_JSON_H_

namespace Qentem {
namespace JSON {

template <typename>
class JSONParser;

template <typename Char_T_>
inline static Value<Char_T_> Parse(const Char_T_ *content, ULong length) {
    Value<Char_T_> value;
    JSONParser<Char_T_>().Parse(value, content, length);
    return value;
}

template <typename Char_T_>
inline static Value<Char_T_> Parse(const Char_T_ *content) {
    return Parse<Char_T_>(content, StringUtils::Count(content));
}

template <typename Char_T_>
class JSONParser {
    using JSONotation_T_ = JSONotation<Char_T_>;
    enum class Type_;
    struct FindCache_T_;

    friend class Qentem::Engine;

  public:
    JSONParser() = default;

    void Parse(Value<Char_T_> &value, const Char_T_ *content, ULong length) {
        ULong offset = 0;

        while (offset < length) {
            switch (content[offset]) {
                case JSONotation_T_::OCurlyChar:
                case JSONotation_T_::OSquareChar: {
                    while (offset != length) {
                        switch (content[--length]) {
                            case JSONotation_T_::CommaChar:
                            case JSONotation_T_::QuoteChar: {
                                return;
                            }

                            case JSONotation_T_::CCurlyChar:
                            case JSONotation_T_::CSquareChar: {
                                if (((content[offset] ==
                                      JSONotation_T_::OCurlyChar) &&
                                     (content[length] ==
                                      JSONotation_T_::CCurlyChar)) ||
                                    ((content[offset] ==
                                      JSONotation_T_::OSquareChar) &&
                                     (content[length] ==
                                      JSONotation_T_::CSquareChar))) {

                                    if (content[offset] ==
                                        JSONotation_T_::OCurlyChar) {
                                        obj_ = HAllocator::Allocate(
                                            HArray<Value<Char_T_>, Char_T_>());
                                        value = Value<Char_T_>{obj_};
                                    } else {
                                        arr_ = HAllocator::Allocate(
                                            Array<Value<Char_T_>>());
                                        value = Value<Char_T_>{arr_};
                                    }

#ifdef QENTEM_SIMD_ENABLED_
                                    FindCache_T_ fc;
                                    find_cache_ = &fc;
#endif

                                    Engine::FindNest(content, ++offset, length,
                                                     length, this);

                                    if (!has_error_ && !pass_comma_) {
                                        length -= (offset - 1);
                                        searchAgain(content, offset, length);
                                    }

                                    if (next_offset_ == 0) {
                                        // Prevent checkPoint2 form failing on
                                        // an empty objects or arrays
                                        next_offset_ = offset;
                                    }

                                    if (has_error_ ||
                                        (!checkPoint2(content, length))) {
                                        value.Reset();
                                        return;
                                    }

                                    // value.Compress();
                                }

                                return;
                            }

                            default: {
                            }
                        }
                    }
                }

                default: {
                }
            }

            ++offset;
        }
    }

    //////////// Private ////////////

  private:
    inline bool HasTail() const noexcept {
        switch (type_) {
            case Type_::Quote:
            case Type_::Curly:
            case Type_::Square: {
                return true;
            }

            default: {
            }
        }

        return false;
    }

#ifdef QENTEM_SIMD_ENABLED_
    // TODO: Add 16 and 32-bit character
    void qmmFindH(const char *content, ULong offset,
                  ULong end_before) const noexcept {
        do {
            find_cache_->Offset     = offset;
            find_cache_->NextOffset = (find_cache_->Offset + QMM_SIZE_);
            offset                  = find_cache_->NextOffset;

            const QMM_VAR_ m_content =
                QMM_LOAD_(reinterpret_cast<const QMM_VAR_ *>(
                    content + find_cache_->Offset));

            // The value of 8 characters:
            constexpr unsigned long long colon  = 4195730024608447034ULL;
            constexpr unsigned long long curly  = 8897841259083430779ULL;
            constexpr unsigned long long square = 6582955728264977243ULL;
            constexpr unsigned long long quote  = 2459565876494606882ULL;
            constexpr unsigned long long comma  = 3182967604875373612ULL;

            find_cache_->Bits =
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(colon));
            find_cache_->Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(curly));
            find_cache_->Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(square));
            find_cache_->Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(quote));
            find_cache_->Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(comma));
        } while ((find_cache_->Bits == 0) &&
                 (find_cache_->NextOffset < end_before));
    }

    inline ULong FindH(const char *content, ULong offset,
                       ULong end_before) noexcept {
        do {
            if (find_cache_->Bits != 0) {
                ULong index =
                    (Platform::CTZ(find_cache_->Bits) + find_cache_->Offset);

                if (index >= offset) {
                    if (index >= end_before) {
                        return 0;
                    }

                    switch (content[index]) {
                        case JSONotation_T_::QuoteChar: {
                            type_ = Type_::Quote;
                            return (index + 1);
                        }

                        case JSONotation_T_::CommaChar: {
                            type_ = Type_::Comma;
                            return (index + 1);
                        }

                        case JSONotation_T_::ColonChar: {
                            if (has_colon_ || (obj_ == nullptr) ||
                                (obj_value_ == nullptr)) {
                                Failed();
                                return 0;
                            }

                            has_colon_   = true;
                            pass_comma_  = false;
                            next_offset_ = (index + 1);
                            offset       = next_offset_;
                            break;
                        }

                        case JSONotation_T_::OCurlyChar:
                            type_      = Type_::Curly;
                            child_obj_ = HAllocator::Allocate(
                                HArray<Value<char>, char>());
                            return (index + 1);

                        case JSONotation_T_::OSquareChar: {
                            type_ = Type_::Square;
                            child_arr_ =
                                HAllocator::Allocate(Array<Value<char>>());
                            return (index + 1);
                        }

                        default: {
                        }
                    }
                }

                // Remove the current offset.
                find_cache_->Bits &= (find_cache_->Bits - 1);
            }

            if ((find_cache_->Bits == 0) && (offset < end_before)) {
                qmmFindH(content, offset, end_before);
            }
        } while (find_cache_->Bits != 0);

        return 0;
    }
#else
    inline ULong FindH(const Char_T_ *content, ULong offset,
                       ULong end_before) noexcept {
        while (offset < end_before) {
            switch (content[offset]) {
                case JSONotation_T_::QuoteChar: {
                    type_ = Type_::Quote;
                    return (offset + 1);
                }

                case JSONotation_T_::CommaChar: {
                    type_ = Type_::Comma;
                    return (offset + 1);
                }

                case JSONotation_T_::ColonChar: {
                    if (has_colon_ || (obj_ == nullptr) ||
                        (obj_value_ == nullptr)) {
                        Failed();
                        return 0;
                    }

                    has_colon_  = true;
                    pass_comma_ = false;
                    ++offset;
                    next_offset_ = offset;
                    continue;
                }

                case JSONotation_T_::OCurlyChar: {
                    type_ = Type_::Curly;
                    child_obj_ =
                        HAllocator::Allocate(HArray<Value<Char_T_>, Char_T_>());
                    return (offset + 1);
                }

                case JSONotation_T_::OSquareChar: {
                    type_      = Type_::Square;
                    child_arr_ = HAllocator::Allocate(Array<Value<Char_T_>>());
                    return (offset + 1);
                }

                default: {
                }
            }

            ++offset;
        }

        // No match.
        return 0;
    }
#endif

    ULong FindT(const Char_T_ *content, ULong offset,
                ULong end_before) const noexcept {
        switch (type_) {
            case Type_::Square: {
                return Engine::FindOne(JSONotation_T_::CSquareChar, content,
                                       offset, end_before);
            }

            case Type_::Curly: {
                return Engine::FindOne(JSONotation_T_::CCurlyChar, content,
                                       offset, end_before);
            }

            default: {
                return Engine::FindOne(JSONotation_T_::QuoteChar, content,
                                       offset, end_before);
            }
        }
    }

    ULong Nest(const Char_T_ *content, ULong offset, ULong end_before,
               ULong max_end_before) {
        // Note: Rewrite it like Template.hpp

        switch (type_) {
            case Type_::Curly:
            case Type_::Square: {
                JSONParser jp{child_obj_, child_arr_, child_obj_value_,
                              pass_comma_};
#ifdef QENTEM_SIMD_ENABLED_
                jp.find_cache_ = find_cache_;
#endif

                if (next_offset_ >= offset) {
                    // Prevent false error.
                    jp.next_offset_ = next_offset_;
                }

                const ULong ret = Engine::FindNest(content, offset, end_before,
                                                   max_end_before, &jp);

                pass_comma_ = (ret != 0); // If it has found a value, then the
                                          // next comma needs to be passed.

                if (!(jp.has_error_) && (!(pass_comma_) || !(jp.pass_comma_)) &&
                    ((child_obj_ == nullptr) || jp.has_colon_)) {
                    // if the scan is not done.
                    jp.searchAgain(content, offset, (end_before - offset));
                }

                if (jp.has_error_) {
                    Failed();
                    return max_end_before;
                }

                next_offset_ = jp.next_offset_; // Set the current offset.
                child_obj_value_ =
                    jp.obj_value_; // Preserve the child value pointer.

                return ret;
            }

            default: {
                UInt  times      = 0;
                ULong tmp_offset = (end_before - 1);

                while ((offset < tmp_offset) &&
                       (content[--tmp_offset] == JSONotation_T_::BSlashChar)) {
                    ++times;
                }

                if ((times % 2U) != 0) {
                    return end_before;
                }

                type_ = Type_::QuoteEnd;

                return 0;
            }
        }
    }

    void Found(const Char_T_ *content, ULong offset, ULong end_before,
               ULong start_offset, ULong &current_offset) {
        static const Char_T_ *true_string  = JSONotation_T_::GetTrueString();
        static const Char_T_ *false_string = JSONotation_T_::GetFalseString();
        static const Char_T_ *null_string  = JSONotation_T_::GetNullString();

        switch (type_) {
            case Type_::Comma: { // ,
                if (!pass_comma_) {
                    if (!checkPoint()) {
                        current_offset = end_before;
                        Failed();
                        return;
                    }

                    has_colon_ = false;

                    ULong item_offset =
                        (next_offset_ != 0) ? next_offset_ : offset;

                    StringUtils::StartTrim(content, item_offset, start_offset);

                    if (item_offset == start_offset) {
                        // Empty
                        current_offset = end_before;
                        Failed();
                        return;
                    }

                    --start_offset;

                    // End trim.
                    while (
                        (item_offset < --start_offset) &&
                        ((content[start_offset] == JSONotation_T_::SpaceChar) ||
                         (content[start_offset] ==
                          JSONotation_T_::LineControlChar) ||
                         (content[start_offset] ==
                          JSONotation_T_::TabControlChar) ||
                         (content[start_offset] ==
                          JSONotation_T_::CarriageControlChar))) {
                    }

                    const UInt len =
                        static_cast<UInt>((start_offset + 1) - item_offset);

                    if (len == 0) {
                        // Empty
                        current_offset = end_before;
                        Failed();
                        return;
                    }

                    UInt tmp_offset = 1;

                    switch (content[item_offset]) {
                        case JSONotation_T_::T_Char: {
                            if (len == JSONotation_T_::TrueStringLength) {
                                ++item_offset;

                                while ((tmp_offset !=
                                        JSONotation_T_::TrueStringLength) &&
                                       (content[item_offset] ==
                                        true_string[tmp_offset])) {
                                    ++item_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset ==
                                    JSONotation_T_::TrueStringLength) {
                                    insert(Value<Char_T_>{ValueType::True});
                                    break;
                                }
                            }

                            current_offset = end_before;
                            Failed();
                            return;
                        }

                        case JSONotation_T_::F_Char: {
                            if (len == JSONotation_T_::FalseStringLength) {
                                ++item_offset;

                                while ((tmp_offset !=
                                        JSONotation_T_::FalseStringLength) &&
                                       (content[item_offset] ==
                                        false_string[tmp_offset])) {
                                    ++item_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset ==
                                    JSONotation_T_::FalseStringLength) {
                                    insert(Value<Char_T_>{ValueType::False});
                                    break;
                                }
                            }

                            current_offset = end_before;
                            Failed();
                            return;
                        }

                        case JSONotation_T_::N_Char: {
                            if (len == JSONotation_T_::NullStringLength) {
                                ++item_offset;

                                while ((tmp_offset !=
                                        JSONotation_T_::NullStringLength) &&
                                       (content[item_offset] ==
                                        null_string[tmp_offset])) {
                                    ++item_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset ==
                                    JSONotation_T_::NullStringLength) {
                                    insert(Value<Char_T_>{ValueType::Null});
                                    break;
                                }
                            }

                            current_offset = end_before;
                            Failed();
                            return;
                        }

                        default: {
                            double num;
                            if (!Digit<Char_T_>::StringToNumber(
                                    num, (content + item_offset), len)) {
                                current_offset = end_before;
                                Failed();
                                return;
                            }

                            insert(Value<Char_T_>{num});
                            break;
                        }
                    }
                } else {
                    // Checking for anything extra.
                    if (!checkPoint2(content, --start_offset)) {
                        current_offset = end_before;
                        Failed();
                        return;
                    }

                    pass_comma_ = false;
                }

                next_offset_ = current_offset;
                break;
            }

            case Type_::QuoteEnd: { // ""
                if (!checkPoint3(content, offset, start_offset)) {
                    current_offset = end_before;
                    Failed();
                    return;
                }

                next_offset_ = current_offset;

                ULong    tmp_length = (current_offset - (start_offset + 1));
                Char_T_ *str =
                    UnEscapeJSON((content + start_offset), tmp_length);

                if (str == nullptr) {
                    current_offset = end_before;
                    Failed();
                    return;
                }

                if ((obj_ == nullptr) || has_colon_) { // String
                    insert(Value<Char_T_>{HAllocator::Allocate(
                        String<Char_T_>(str, tmp_length))});

                    has_colon_  = false;
                    pass_comma_ = true;
                } else { // Key
                    obj_value_ = &((*obj_)[String<Char_T_>(str, tmp_length)]);
                }

                break;
            }

            case Type_::Curly:    // {}
            case Type_::Square: { // []
                if (!checkPoint() ||
                    !checkPoint3(content, offset, start_offset)) {
                    current_offset = end_before;
                    Failed();
                    return;
                }

                has_colon_   = false;
                pass_comma_  = true;
                next_offset_ = current_offset;

                if (type_ == Type_::Curly) {
                    // child_obj_->Compress();
                    insert(Value<Char_T_>{child_obj_});
                    child_obj_ = nullptr;
                } else {
                    // child_arr_->Compress();
                    insert(Value<Char_T_>{child_arr_});
                    child_arr_ = nullptr;
                }

                break;
            }

            default: {
            }
        }

        type_ = Type_::None;
    }

    void Failed() noexcept {
        if (child_obj_ != nullptr) {
            HAllocator::Destruct(child_obj_);
            HAllocator::Deallocate(child_obj_);
            child_obj_ = nullptr;
        }

        if (child_arr_ != nullptr) {
            HAllocator::Destruct(child_arr_);
            HAllocator::Deallocate(child_arr_);
            child_arr_ = nullptr;
        }

        has_error_ = true;
    }

    /*
     * Look for anything before ] or } and after comma, as not possable to
     * match if it was a number/false/true/null
     */
    inline void searchAgain(const Char_T_ *content, ULong offset,
                            ULong length) {
        type_ = Type_::Comma;
        --length;

        StringUtils::SoftTrim(content, offset, length);

        if (length != 0) {
            ULong fake_comma = ((offset + length) + 1);
            Found(content, offset, fake_comma, fake_comma, fake_comma);
        }
    }

    inline void insert(Value<Char_T_> &&val) {
        if (obj_ != nullptr) {
            *obj_value_ = static_cast<Value<Char_T_> &&>(val);
            obj_value_  = nullptr;
        } else {
            *arr_ += static_cast<Value<Char_T_> &&>(val);
        }
    }

    inline bool checkPoint() const noexcept {
        if (obj_ != nullptr) {
            return (has_colon_ && !has_error_);
        }

        return (!has_colon_ && !has_error_);
    }

    inline bool checkPoint2(const Char_T_ *content, ULong offset) noexcept {
        if (next_offset_ < offset) {
            StringUtils::StartTrim(content, next_offset_, offset);

            if (next_offset_ != offset) {
                return false;
            }
        }

        return true;
    }

#ifdef QENTEM_SIMD_ENABLED_
    // TODO: Add 16 and 32-bit character
    bool checkPoint3(const char *content, ULong offset,
                     ULong start_offset) const noexcept {
        if (!has_colon_) {
            --start_offset;

            while (offset < start_offset) {
                ULong offset2;
                ULong length = (start_offset - offset);

                if (length <= QMM_SIZE_) {
                    offset2 = offset;
                } else {
                    offset2 = (start_offset - QMM_SIZE_);
                    length  = QMM_SIZE_;
                }

                const QMM_VAR_ m_content = QMM_LOAD_(
                    reinterpret_cast<const QMM_VAR_ *>(content + offset2));

                constexpr unsigned long long line     = 723401728380766730ULL;
                constexpr unsigned long long tab      = 651061555542690057ULL;
                constexpr unsigned long long carriage = 940422246894996749ULL;
                constexpr unsigned long long space    = 2314885530818453536ULL;

                ULong bits;
                bits = QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(line));
                bits |= QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(tab));
                bits |=
                    QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(carriage));
                bits |= QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(space));
                bits ^= QMM_MAX_NUMBER_; // Remove any whitespace

                if (length != QMM_SIZE_) {
                    bits &= ((ULong{1} << length) - 1U); // Droping whats beyond
                }

                while (bits != 0) {
                    switch (content[(Platform::CLZL(bits) + offset2)]) {
                        case JSONotation_T_::OCurlyChar:
                        case JSONotation_T_::OSquareChar:
                        case JSONotation_T_::CommaChar:
                            return true;

                        default: {
                            return false;
                        }
                    }
                }

                start_offset = offset2;
            }
        }

        return true;
    }
#else
    bool checkPoint3(const Char_T_ *content, ULong offset,
                     ULong start_offset) const noexcept {
        if (!has_colon_) {
            --start_offset;

            while (start_offset > offset) {
                --start_offset;

                switch (content[start_offset]) {
                    case JSONotation_T_::TabControlChar:
                    case JSONotation_T_::LineControlChar:
                    case JSONotation_T_::CarriageControlChar:
                    case JSONotation_T_::SpaceChar: {
                        break;
                    }

                    case JSONotation_T_::OCurlyChar:
                    case JSONotation_T_::OSquareChar:
                    case JSONotation_T_::CommaChar: {
                        return true;
                    }

                    default: {
                        return false;
                    }
                }
            }
        }

        return true;
    }
#endif

    JSONParser(HArray<Value<Char_T_>, Char_T_> *obj, Array<Value<Char_T_>> *arr,
               Value<Char_T_> *obj_value, bool pass_comma) noexcept
        : obj_(obj), arr_(arr), obj_value_(obj_value), pass_comma_(pass_comma) {
    }

    enum class Type_ { None = 0, Curly, Square, Quote, QuoteEnd, Comma };

    ULong                            next_offset_{0};
    HArray<Value<Char_T_>, Char_T_> *obj_{nullptr};
    Array<Value<Char_T_>> *          arr_{nullptr};

    HArray<Value<Char_T_>, Char_T_> *child_obj_{nullptr};
    Array<Value<Char_T_>> *          child_arr_{nullptr};

    Value<Char_T_> *child_obj_value_{nullptr};
    Value<Char_T_> *obj_value_{nullptr};

#ifdef QENTEM_SIMD_ENABLED_
    struct FindCache_T_ {
        ULong        Offset{0};
        ULong        NextOffset{0};
        QMM_Number_T Bits{0};
    };

    FindCache_T_ *find_cache_{nullptr};
#endif

    Type_ type_{Type_::None};

    bool has_colon_{false};
    bool pass_comma_{false};
    bool has_error_{false};
};

} // namespace JSON
} // namespace Qentem

#endif
