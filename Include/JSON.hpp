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

#include "Array.hpp"
#include "Digit.hpp"
#include "Engine.hpp"
#include "HArray.hpp"
#include "Value.hpp"

#ifndef QENTEM_JSON_H_
#define QENTEM_JSON_H_

namespace Qentem {
namespace JSON {

#ifdef QENTEM_SIMD_ENABLED_
static char *UnEscapeString(const char *content, ULong &length) {
    static constexpr unsigned long long line       = 723401728380766730ULL;
    static constexpr unsigned long long tab        = 651061555542690057ULL;
    static constexpr unsigned long long carriage   = 940422246894996749ULL;
    static constexpr unsigned long long back_slash = 6655295901103053916ULL;

    char *str      = HAllocator::Allocate<char>(length + 1);
    ULong offset   = 0;
    ULong offset2  = 0;
    ULong m_offset = 0;
    ULong length2;

    if (length != 0) {
        QMM_NUMBER_TYPE_ bits;

        do {
            const QMM_VAR_ m_content = QMM_LOAD_(
                reinterpret_cast<const QMM_VAR_ *>(content + m_offset));
            bits = QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(line));
            bits |= QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(tab));
            bits |= QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(carriage));

            if (bits != 0) {
                ULong index = (Q_CTZL(bits) + m_offset);

                if (index < length) {
                    HAllocator::Deallocate(str);
                    return nullptr;
                }
            }

            bits = QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(back_slash));

            while (bits != 0) {
                ULong index = (Q_CTZL(bits) + m_offset);

                if (index >= length) {
                    break;
                }

                length2 = (index - offset);

                if (length2 != 0) {
                    Memory::Copy((str + offset2), (content + offset), length2);
                    offset2 += length2;
                }

                offset = index;
                ++offset;

                switch (content[offset]) {
                    case '\\': {
                        str[offset2] = content[offset];
                        bits &= (bits - 1);
                        break;
                    }

                    case '"':
                    case '/': {
                        str[offset2] = content[offset];
                        break;
                    }

                    case 'b': {
                        str[offset2] = '\b';
                        break;
                    }

                    case 'f': {
                        str[offset2] = '\f';
                        break;
                    }

                    case 'n': {
                        str[offset2] = '\n';
                        break;
                    }
                    case 'r': {
                        str[offset2] = '\r';
                        break;
                    }

                    case 't': {
                        str[offset2] = '\t';
                        break;
                    }

                    case 'U':
                    case 'u': {
                        ++offset;

                        if ((length - offset) < 4) {
                            HAllocator::Deallocate(str);
                            return nullptr;
                        }

                        UInt code =
                            Digit::HexStringToNumber((content + offset), 4);
                        offset += 4;

                        if ((code >> 8U) == 0xD8U) {
                            // Surrogate
                            if ((length - offset) < 6U) {
                                HAllocator::Deallocate(str);
                                return nullptr;
                            }

                            code = (code ^ 0xD800U) << 10U;
                            offset += 2;
                            code += Digit::HexStringToNumber((content + offset),
                                                             4) &
                                    0x3FFU;
                            code += 0x10000U;

                            offset2 += String::ToUTF8(code, (str + offset2));
                            offset += 4;
                            bits &= (bits - 1);
                        } else {
                            offset2 += String::ToUTF8(code, (str + offset2));
                        }

                        bits &= (bits - 1);
                        continue;
                    }

                    default: {
                        HAllocator::Deallocate(str);
                        return nullptr;
                    }
                }

                ++offset;
                ++offset2;

                bits &= (bits - 1);
            }

            m_offset += QMM_SIZE_;

            if (m_offset < offset) {
                m_offset = offset;
            }
        } while (m_offset < length);
    }

    length2 = (length - offset);

    if (length2 != 0) {
        Memory::Copy((str + offset2), (content + offset), length2);
        offset2 += length2;
    }

    str[offset2] = '\0';
    length       = offset2;

    return str;
}
#else
static char *UnEscapeString(const char *content, ULong &length) {
    UInt  offset  = 0;
    UInt  offset2 = 0;
    char *str     = HAllocator::Allocate<char>(length + 1);

    while (offset < length) {
        switch (content[offset]) {
            case '\\': {
                ++offset;

                switch (content[offset]) {
                    case '"':
                    case '\\':
                    case '/': {
                        str[offset2] = content[offset];
                        break;
                    }

                    case 'b': {
                        str[offset2] = '\b';
                        break;
                    }

                    case 'f': {
                        str[offset2] = '\f';
                        break;
                    }

                    case 'n': {
                        str[offset2] = '\n';
                        break;
                    }
                    case 'r': {
                        str[offset2] = '\r';
                        break;
                    }

                    case 't': {
                        str[offset2] = '\t';
                        break;
                    }

                    case 'U':
                    case 'u': {
                        ++offset;

                        if ((length - offset) < 4) {
                            HAllocator::Deallocate(str);
                            return nullptr;
                        }

                        UInt code =
                            Digit::HexStringToNumber((content + offset), 4);
                        offset += 4;

                        if ((code >> 8U) == 0xD8U) {
                            // Surrogate
                            if ((length - offset) < 6U) {
                                HAllocator::Deallocate(str);
                                return nullptr;
                            }

                            code = (code ^ 0xD800U) << 10U;
                            offset += 2;
                            code += Digit::HexStringToNumber((content + offset),
                                                             4) &
                                    0x3FFU;
                            code += 0x10000U;

                            offset2 += String::ToUTF8(code, (str + offset2));
                            offset += 4;
                        } else {
                            offset2 += String::ToUTF8(code, (str + offset2));
                        }

                        continue;
                    }

                    default: {
                        HAllocator::Deallocate(str);
                        return nullptr;
                    }
                }

                break;
            }

            case '\n':
            case '\t':
            case '\r': {
                HAllocator::Deallocate(str);
                return nullptr;
            }

            default: {
                str[offset2] = content[offset];
            }
        }

        ++offset;
        ++offset2;
    }

    str[offset2] = '\0';
    length       = offset2;

    return str;
}
#endif

////////////////////////////////////////////////////////////

class JSONParser : Engine {
    enum class Type_;
    struct FindCache_;

  public:
    JSONParser() = default;

    void Parse(Value &value, const char *content, ULong length) {
        ULong offset = 0;

        while (offset != length) {
            switch (content[offset]) {
                case '{':
                case '[': {
                    while (offset != length) {
                        switch (content[--length]) {
                            case ',':
                            case '"': {
                                return;
                            }

                            case '}':
                            case ']': {
                                if (((content[offset] == '{') &&
                                     (content[length] == '}')) ||
                                    ((content[offset] == '[') &&
                                     (content[length] == ']'))) {

                                    if (content[offset] == '{') {
                                        obj_ = HAllocator::AllocateClear<
                                            HArray<Value>>(1);

                                        value = Value(obj_);
                                    } else {
                                        arr_ = HAllocator::AllocateClear<
                                            Array<Value>>(1);
                                        value = Value(arr_);
                                    }

#ifdef QENTEM_SIMD_ENABLED_
                                    FindCache_ fc = FindCache_();
                                    find_cache_   = &fc;
#endif

                                    FindNest(content, ++offset, length, length);

                                    if (!has_error_ && !pass_comma_) {
                                        length -= (offset - 1);
                                        digDeeper_(content, offset, length);
                                    }

                                    if (next_offset_ == 0) {
                                        // Prevent checkPoint2_ form failing on
                                        // an empty objects or arrays
                                        next_offset_ = offset;
                                    }

                                    if (has_error_ ||
                                        (!checkPoint2_(content, length))) {
                                        value.Clear();
                                        return;
                                    }

                                    // value.Compress();
                                }

                                return;
                            }
                        }
                    }
                }
            }

            ++offset;
        }
    }

    //////////// Private ////////////

  private:
    inline bool hasTail() const noexcept final {
        switch (type_) {
            case Type_::Quote:
            case Type_::Curly:
            case Type_::Square: {
                return true;
            }

            default: {
                return false;
            }
        }
    }

    ULong find2(const char *content, ULong offset,
                ULong end_before) noexcept final {
        switch (type_) {
            case Type_::Square: {
                return Find("]", 1, content, offset, end_before);
            }

            case Type_::Curly: {
                return Find("}", 1, content, offset, end_before);
            }

            default: {
                return Find("\"", 1, content, offset, end_before);
            }
        }
    }

#ifdef QENTEM_SIMD_ENABLED_
    void qmm_find_(const char *content, ULong offset,
                   ULong end_before) noexcept {
        do {
            find_cache_->Offset     = offset;
            find_cache_->NextOffset = (find_cache_->Offset + QMM_SIZE_);
            offset                  = find_cache_->NextOffset;

            const QMM_VAR_ m_content =
                QMM_LOAD_(reinterpret_cast<const QMM_VAR_ *>(
                    content + find_cache_->Offset));

            // the value of 8 characters:
            static constexpr unsigned long long colon  = 4195730024608447034ULL;
            static constexpr unsigned long long curly  = 8897841259083430779ULL;
            static constexpr unsigned long long square = 6582955728264977243ULL;
            static constexpr unsigned long long quote  = 2459565876494606882ULL;
            static constexpr unsigned long long comma  = 3182967604875373612ULL;

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

    inline ULong find(const char *content, ULong offset,
                      ULong end_before) noexcept final {
        if ((offset > find_cache_->NextOffset) ||
            ((find_cache_->Bits == 0) &&
             (find_cache_->NextOffset < end_before))) {
            qmm_find_(content, offset, end_before);
        }

        ULong index;

        do {
            index = (Q_CTZL(find_cache_->Bits) + find_cache_->Offset);

            if (index >= offset) {
                break;
            }

            // Remove the current offset.
            find_cache_->Bits &= (find_cache_->Bits - 1);

            if ((find_cache_->Bits == 0) &&
                (find_cache_->NextOffset < end_before)) {
                qmm_find_(content, find_cache_->NextOffset, end_before);
            }
        } while (find_cache_->Bits != 0);

        if ((index < end_before) && (index >= offset)) {
            switch (content[index]) {
                case '"': {
                    type_ = Type_::Quote;
                    break;
                }

                case ',': {
                    type_ = Type_::Comma;
                    break;
                }

                case ':': {
                    type_ = Type_::Colon;
                    break;
                }

                case '{':
                    type_      = Type_::Curly;
                    child_obj_ = HAllocator::AllocateClear<HArray<Value>>(1);
                    break;

                case '[': {
                    type_      = Type_::Square;
                    child_arr_ = HAllocator::AllocateClear<Array<Value>>(1);
                }
            }

            return (index + 1);
        }

        // No match.
        return 0;
    }
#else
    inline ULong find(const char *content, ULong offset,
                      ULong end_before) noexcept final {
        bool found = false;

        while (offset < end_before) {
            switch (content[offset]) {
                case '"': {
                    found = true;
                    type_ = Type_::Quote;
                    break;
                }

                case ',': {
                    found = true;
                    type_ = Type_::Comma;
                    break;
                }

                case ':': {
                    found = true;
                    type_ = Type_::Colon;
                    break;
                }

                case '{':
                    found      = true;
                    type_      = Type_::Curly;
                    child_obj_ = HAllocator::AllocateClear<HArray<Value>>(1);
                    break;

                case '[': {
                    found      = true;
                    type_      = Type_::Square;
                    child_arr_ = HAllocator::AllocateClear<Array<Value>>(1);
                }
            }

            ++offset;

            if (found) {
                return offset;
            }
        }

        // No match.
        return 0;
    }
#endif

    ULong nest(const char *content, ULong offset, ULong end_before,
               ULong max_end_before) final {
        switch (type_) {
            case Type_::Curly:
            case Type_::Square: {
                JSONParser jp(child_obj_, child_arr_, child_obj_value_,
                              find_cache_, pass_comma_);

                if (next_offset_ >= offset) {
                    // Prevent false error.
                    jp.next_offset_ = next_offset_;
                }

                ULong ret =
                    jp.FindNest(content, offset, end_before, max_end_before);

                pass_comma_ = (ret != 0); // if it has found a value, then the
                                          // next comma needs to be passed.

                if (!(jp.has_error_) && !(pass_comma_) &&
                    ((child_obj_ == nullptr) || jp.has_colon_)) {
                    // if the scan is not done.
                    jp.digDeeper_(content, offset, (end_before - offset));
                }

                if (jp.has_error_) {
                    failed();
                    return max_end_before;
                }

                next_offset_ = jp.next_offset_; // set the current offset.
                child_obj_value_ =
                    jp.obj_value_; // Preserve the child value pointer.

                return ret;
            }

            case Type_::Quote: {
                // TODO: Use Engine::Find

                UInt  times      = 0;
                ULong tmp_offset = (end_before - 1);

                while ((offset < tmp_offset) &&
                       (content[--tmp_offset] == '\\')) {
                    ++times;
                }

                if ((times % 2) != 0) {
                    return end_before;
                }

                break;
            }

            default: {
            }
        }

        return 0;
    }

    void Found(const char *content, ULong offset, ULong end_before,
               ULong start_offset, ULong &current_offset) final {
        static constexpr UInt true_len  = 4;
        static constexpr UInt false_len = 5;
        static constexpr UInt null_len  = 4;

        switch (type_) {
            case Type_::Comma: { // ,
                if (!pass_comma_) {
                    if (!checkPoint_()) {
                        current_offset = end_before;
                        failed();
                        return;
                    }

                    has_colon_ = false;

                    ULong item_offset =
                        (next_offset_ != 0) ? next_offset_ : offset;

                    String::LeftTrim(content, item_offset, start_offset);

                    if (item_offset == start_offset) {
                        // Empty
                        current_offset = end_before;
                        failed();
                        return;
                    }

                    --start_offset;

                    // Right trim.
                    while ((item_offset < --start_offset) &&
                           ((content[start_offset] == ' ') ||
                            (content[start_offset] == '\n') ||
                            (content[start_offset] == '\t') ||
                            (content[start_offset] == '\r'))) {
                    }

                    const UInt len =
                        static_cast<UInt>((start_offset + 1) - item_offset);

                    if (len == 0) {
                        // Empty
                        current_offset = end_before;
                        failed();
                        return;
                    }

                    UInt tmp_offset = 0;

                    switch (content[item_offset]) {
                        case 't': {
                            if (len == true_len) {
                                ++item_offset;

                                while (tmp_offset < 3) {
                                    if (content[item_offset] !=
                                        "rue"[tmp_offset]) {
                                        tmp_offset = 0;
                                        break;
                                    }

                                    ++item_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset == 3) {
                                    add_(Value(ValueType::True));
                                    break;
                                }
                            }

                            current_offset = end_before;
                            failed();
                            return;
                        }

                        case 'f': {
                            if (len == false_len) {
                                ++item_offset;

                                while (tmp_offset < 4) {
                                    if (content[item_offset] !=
                                        "alse"[tmp_offset]) {
                                        tmp_offset = 0;
                                        break;
                                    }

                                    ++item_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset == 4) {
                                    add_(Value(ValueType::False));
                                    break;
                                }
                            }

                            current_offset = end_before;
                            failed();
                            return;
                        }

                        case 'n': {
                            if (len == null_len) {
                                ++item_offset;

                                while (tmp_offset < 3) {
                                    if (content[item_offset] !=
                                        "ull"[tmp_offset]) {
                                        tmp_offset = 0;
                                        break;
                                    }

                                    ++item_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset == 3) {
                                    add_(Value(ValueType::Null));
                                    break;
                                }
                            }

                            current_offset = end_before;
                            failed();
                            return;
                        }

                        default: {
                            double num;
                            if (!Digit::StringToNumber(
                                    num, (content + item_offset), len)) {
                                current_offset = end_before;
                                failed();
                                return;
                            }

                            add_(Value(num));
                            break;
                        }
                    }
                } else {
                    // Checking for anything extra.
                    if (!checkPoint2_(content, --start_offset)) {
                        current_offset = end_before;
                        failed();
                        return;
                    }

                    pass_comma_ = false;
                }

                next_offset_ = current_offset;
                break;
            }

            case Type_::Quote: { // ""
                if (!checkPoint3_(content, offset, start_offset)) {
                    current_offset = end_before;
                    failed();
                    return;
                }

                next_offset_ = current_offset;

                ULong tmp_length = (current_offset - (start_offset + 1));
                char *str =
                    UnEscapeString((content + start_offset), tmp_length);

                if (str == nullptr) {
                    current_offset = end_before;
                    failed();
                    return;
                }

                if ((obj_ == nullptr) || has_colon_) { // String
                    add_(Value(HAllocator::Allocate(String(str, tmp_length))));

                    has_colon_  = false;
                    pass_comma_ = true;
                } else { // Key
                    obj_value_ = &((*obj_)[String(str, tmp_length)]);
                }

                break;
            }

            case Type_::Curly:    // {}
            case Type_::Square: { // []
                if (!checkPoint_() ||
                    !checkPoint3_(content, offset, start_offset)) {
                    current_offset = end_before;
                    failed();
                    return;
                }

                has_colon_   = false;
                pass_comma_  = true;
                next_offset_ = current_offset;

                if (type_ == Type_::Curly) {
                    // child_obj_->Compress();
                    add_(Value(child_obj_));
                    child_obj_ = nullptr;
                } else {
                    // child_arr_->Compress();
                    add_(Value(child_arr_));
                    child_arr_ = nullptr;
                }

                break;
            }

            case Type_::Colon: { // :
                if (has_colon_ || (obj_ == nullptr) ||
                    (obj_value_ == nullptr)) {
                    current_offset = end_before;
                    failed();
                    return;
                }

                has_colon_   = true;
                pass_comma_  = false;
                next_offset_ = start_offset;
                break;
            }

            default: {
            }
        }

        type_ = Type_::None;
    }

    /*
     * Look for anything before ] or } and after comma, as not possable to
     * match if it was a number/false/true/null
     */
    void digDeeper_(const char *content, ULong offset, ULong length) {
        type_ = Type_::Comma;
        --length;

        String::SoftTrim(content, offset, length);

        if (length != 0) {
            ULong fake_comma = ((offset + length) + 1);
            Found(content, offset, fake_comma, fake_comma, fake_comma);
        }
    }

    void add_(Value &&val) noexcept {
        if (obj_ != nullptr) {
            *obj_value_ = static_cast<Value &&>(val);
            obj_value_  = nullptr;
        } else {
            (*arr_) += static_cast<Value &&>(val);
        }
    }

    bool checkPoint_() const noexcept {
        if (obj_ != nullptr) {
            return (has_colon_ && !has_error_);
        }

        return (!has_colon_ && !has_error_);
    }

    bool checkPoint2_(const char *content, ULong offset) noexcept {
        if (next_offset_ < offset) {
            String::LeftTrim(content, next_offset_, offset);

            if (next_offset_ != offset) {
                return false;
            }
        }

        return true;
    }

#ifdef QENTEM_SIMD_ENABLED_
    bool checkPoint3_(const char *content, ULong offset,
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

                static constexpr unsigned long long line =
                    723401728380766730ULL;
                static constexpr unsigned long long tab = 651061555542690057ULL;
                static constexpr unsigned long long carriage =
                    940422246894996749ULL;
                static constexpr unsigned long long space =
                    2314885530818453536ULL;

                ULong bits;
                bits = QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(line));
                bits |= QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(tab));
                bits |=
                    QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(carriage));
                bits |= QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_64_(space));
                bits ^= QMM_MAX_NUMBER; // Remove any whitespace

                if (length != QMM_SIZE_) {
                    bits &= ((ULong(1) << length) - 1U); // Droping whats beyond
                }

                while (bits != 0) {
                    QMM_NUMBER_TYPE_ bit   = Q_CLZL(bits);
                    ULong            index = (bit + offset2);

                    switch (content[index]) {
                        case '{':
                        case '[':
                        case ',':
                            return true;

                        default: {
                            return false;
                        }
                    }

                    bits ^= (ULong(1) << bit); // Remove the left bit
                }

                start_offset = offset2;
            }
        }

        return true;
    }
#else
    bool checkPoint3_(const char *content, ULong offset,
                      ULong start_offset) const noexcept {
        if (!has_colon_) {
            --start_offset;

            while (start_offset > offset) {
                --start_offset;

                switch (content[start_offset]) {
                    case '\t':
                    case '\n':
                    case '\r':
                    case ' ': {
                        break;
                    }

                    case '{':
                    case '[':
                    case ',': {
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

    void failed() final {
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

    JSONParser(HArray<Value> *obj, Array<Value> *arr, Value *obj_value,
               FindCache_ *find_cache, bool pass_comma) noexcept
        : obj_(obj), arr_(arr), obj_value_(obj_value), find_cache_(find_cache),
          pass_comma_(pass_comma) {
    }

    enum class Type_ { None = 0, Curly, Square, Quote, Comma, Colon };

    struct FindCache_ {
        ULong            Offset{0};
        ULong            NextOffset{0};
        QMM_NUMBER_TYPE_ Bits{0};
    };

    HArray<Value> *obj_{nullptr};
    Array<Value> * arr_{nullptr};

    HArray<Value> *child_obj_{nullptr};
    Array<Value> * child_arr_{nullptr};
    ULong          next_offset_{0};

    Value *     child_obj_value_{nullptr};
    Value *     obj_value_{nullptr};
    FindCache_ *find_cache_{nullptr};

    Type_ type_{Type_::None};

    bool has_colon_{false};
    bool pass_comma_{false};
    bool has_error_{false};
};

////////////////////////////////////////////////////////////

inline static Value Parse(const char *content, ULong length) {
    Value value;
    JSONParser().Parse(value, content, length);
    return value;
}

inline static Value Parse(const String &content) {
    return Parse(content.Char(), content.Length());
}

inline static Value Parse(const char *content) {
    return Parse(content, String::Count(content));
}

} // namespace JSON
} // namespace Qentem

#endif
