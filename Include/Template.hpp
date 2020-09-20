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

#include "ALE.hpp"
#include "StringStream.hpp"
#include "TemplatePatterns.hpp"

#ifndef QENTEM_TEMPLATE_H_
#define QENTEM_TEMPLATE_H_

// TODO: Add autoescape and {raw:...} tag

namespace Qentem {

template <typename, typename>
class Template_T_;

class Template {
  public:
    Template()                 = delete;
    Template(Template &&)      = delete;
    Template(const Template &) = delete;
    Template &operator=(Template &&) = delete;
    Template &operator=(const Template &) = delete;
    ~Template()                           = delete;

    template <typename Char_T_, typename Value_T_>
    inline static void Render(StringStream<Char_T_> &ss, const Char_T_ *content,
                              ULong length, const Value_T_ *root_value) {
        Template_T_<Char_T_, Value_T_> temp(&ss, root_value);
        temp.find(content, 0, length);
        // Add the remaining string.
        ss.Insert((content + temp.last_offset_), (length - temp.last_offset_));
    }

    template <typename Char_T_, typename Value_T_>
    inline static StringStream<Char_T_> Render(const Char_T_ * content,
                                               const ULong     length,
                                               const Value_T_ *root_value) {
        StringStream<Char_T_> ss;
        Render(ss, content, length, root_value);

        return ss;
    }

    template <typename Char_T_, typename Value_T_>
    inline static StringStream<Char_T_> Render(const Char_T_ * content,
                                               const Value_T_ *root_value) {
        return Render(content, StringUtils::Count(content), root_value);
    }
};

template <typename Char_T_, typename Value_T_>
class Template_T_ {
    using TemplatePatterns_T_ = TemplatePatterns<Char_T_>;

  public:
    Template_T_() = delete;

  private:
    template <typename, typename>
    friend class Qentem::ALE_T_;
    friend class Qentem::Template;

    enum class Tag {
        Raw = 0,  // {raw:x}
        Variable, // {var:x}
        Math,     // {math:x}
        InLineIf, // {if:x}
        Loop,     // <loop set="..." key="..." value="...">
        If,       // <if case="...">
    };

#ifdef QENTEM_SIMD_ENABLED_
#if QENTEM_AVX512BW_ == 1 || QENTEM_AVX2_ == 1
    // TODO: Add 16 and 32-bit character
    void qmmFind(const char *content, ULong offset, ULong end_before) noexcept {
        const __m256i rv64 =
            _mm256_set1_epi64x(TemplatePatterns_T_::RawVariable64bit);
        const __m256i v64 =
            _mm256_set1_epi64x(TemplatePatterns_T_::Variable64bit);
        const __m256i m64 = _mm256_set1_epi64x(TemplatePatterns_T_::Math64bit);
        const __m256i iif64 =
            _mm256_set1_epi64x(TemplatePatterns_T_::InLineIf64bit);
        const __m256i l64  = _mm256_set1_epi64x(TemplatePatterns_T_::Loop64bit);
        const __m256i if64 = _mm256_set1_epi64x(TemplatePatterns_T_::If64bit);

        do {
            find_cache_.Offset     = offset;
            find_cache_.NextOffset = (find_cache_.Offset + 32U);
            offset                 = find_cache_.NextOffset;

            __m256i m_content =
                _mm256_loadu_si256(reinterpret_cast<const __m256i *>(
                    content + find_cache_.Offset));

            find_cache_.Bits = static_cast<QMM_Number_T>(_mm256_movemask_epi8(
                                   _mm256_cmpeq_epi16(rv64, m_content))) &
                               0x55555555U;
            find_cache_.Bits |= static_cast<QMM_Number_T>(_mm256_movemask_epi8(
                                    _mm256_cmpeq_epi16(v64, m_content))) &
                                0x55555555U;
            find_cache_.Bits |= static_cast<QMM_Number_T>(_mm256_movemask_epi8(
                                    _mm256_cmpeq_epi16(m64, m_content))) &
                                0x55555555U;
            find_cache_.Bits |= static_cast<QMM_Number_T>(_mm256_movemask_epi8(
                                    _mm256_cmpeq_epi16(iif64, m_content))) &
                                0x55555555U;
            find_cache_.Bits |= static_cast<QMM_Number_T>(_mm256_movemask_epi8(
                                    _mm256_cmpeq_epi16(l64, m_content))) &
                                0x55555555U;
            find_cache_.Bits |= static_cast<QMM_Number_T>(_mm256_movemask_epi8(
                                    _mm256_cmpeq_epi16(if64, m_content))) &
                                0x55555555U;

            m_content = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(
                content + find_cache_.Offset + 1));

            find_cache_.Bits |= static_cast<QMM_Number_T>(_mm256_movemask_epi8(
                                    _mm256_cmpeq_epi16(rv64, m_content))) &
                                0xAAAAAAAAU;
            find_cache_.Bits |= static_cast<QMM_Number_T>(_mm256_movemask_epi8(
                                    _mm256_cmpeq_epi16(v64, m_content))) &
                                0xAAAAAAAAU;
            find_cache_.Bits |= static_cast<QMM_Number_T>(_mm256_movemask_epi8(
                                    _mm256_cmpeq_epi16(m64, m_content))) &
                                0xAAAAAAAAU;
            find_cache_.Bits |= static_cast<QMM_Number_T>(_mm256_movemask_epi8(
                                    _mm256_cmpeq_epi16(iif64, m_content))) &
                                0xAAAAAAAAU;
            find_cache_.Bits |= static_cast<QMM_Number_T>(_mm256_movemask_epi8(
                                    _mm256_cmpeq_epi16(l64, m_content))) &
                                0xAAAAAAAAU;
            find_cache_.Bits |= static_cast<QMM_Number_T>(_mm256_movemask_epi8(
                                    _mm256_cmpeq_epi16(if64, m_content))) &
                                0xAAAAAAAAU;
        } while ((find_cache_.Bits == 0) &&
                 (find_cache_.NextOffset < end_before));
    }
#else
    void qmmFind(const char *content, ULong offset, ULong end_before) noexcept {
        do {
            find_cache_.Offset     = offset;
            find_cache_.NextOffset = (find_cache_.Offset + QMM_SIZE_);
            offset                 = find_cache_.NextOffset;

            QMM_VAR_ m_content = QMM_LOAD_(reinterpret_cast<const QMM_VAR_ *>(
                content + find_cache_.Offset));

            find_cache_.Bits =
                QMM_COMPARE_16_MASK_8_(
                    QMM_SETONE_64_(TemplatePatterns_T_::RawVariable64bit),
                    m_content) &
                QMM_BIT_ONE_;
            find_cache_.Bits |=
                QMM_COMPARE_16_MASK_8_(
                    QMM_SETONE_64_(TemplatePatterns_T_::Variable64bit),
                    m_content) &
                QMM_BIT_ONE_;
            find_cache_.Bits |=
                QMM_COMPARE_16_MASK_8_(
                    QMM_SETONE_64_(TemplatePatterns_T_::Math64bit), m_content) &
                QMM_BIT_ONE_;
            find_cache_.Bits |=
                QMM_COMPARE_16_MASK_8_(
                    QMM_SETONE_64_(TemplatePatterns_T_::InLineIf64bit),
                    m_content) &
                QMM_BIT_ONE_;
            find_cache_.Bits |=
                QMM_COMPARE_16_MASK_8_(
                    QMM_SETONE_64_(TemplatePatterns_T_::Loop64bit), m_content) &
                QMM_BIT_ONE_;
            find_cache_.Bits |=
                QMM_COMPARE_16_MASK_8_(
                    QMM_SETONE_64_(TemplatePatterns_T_::If64bit), m_content) &
                QMM_BIT_ONE_;

            m_content = QMM_LOAD_(reinterpret_cast<const QMM_VAR_ *>(
                content + find_cache_.Offset + 1));

            find_cache_.Bits |=
                QMM_COMPARE_16_MASK_8_(
                    QMM_SETONE_64_(TemplatePatterns_T_::RawVariable64bit),
                    m_content) &
                QMM_BIT_TWO_;
            find_cache_.Bits |=
                QMM_COMPARE_16_MASK_8_(
                    QMM_SETONE_64_(TemplatePatterns_T_::Variable64bit),
                    m_content) &
                QMM_BIT_TWO_;
            find_cache_.Bits |=
                QMM_COMPARE_16_MASK_8_(
                    QMM_SETONE_64_(TemplatePatterns_T_::Math64bit), m_content) &
                QMM_BIT_TWO_;
            find_cache_.Bits |=
                QMM_COMPARE_16_MASK_8_(
                    QMM_SETONE_64_(TemplatePatterns_T_::InLineIf64bit),
                    m_content) &
                QMM_BIT_TWO_;
            find_cache_.Bits |=
                QMM_COMPARE_16_MASK_8_(
                    QMM_SETONE_64_(TemplatePatterns_T_::Loop64bit), m_content) &
                QMM_BIT_TWO_;
            find_cache_.Bits |=
                QMM_COMPARE_16_MASK_8_(
                    QMM_SETONE_64_(TemplatePatterns_T_::If64bit), m_content) &
                QMM_BIT_TWO_;
        } while ((find_cache_.Bits == 0) &&
                 (find_cache_.NextOffset < end_before));
    }
#endif
#endif
    void find(const Char_T_ *content, ULong offset, ULong end_before) {
        static const Char_T_ *raw_variable_prefix =
            TemplatePatterns_T_::GetRawVariablePrefix();
        static const Char_T_ *variable_prefix =
            TemplatePatterns_T_::GetVariablePrefix();
        static const Char_T_ *math_prefix =
            TemplatePatterns_T_::GetMathPrefix();
        static const Char_T_ *inLine_if_prefix =
            TemplatePatterns_T_::GetInLineIfPrefix();
        static const Char_T_ *loop_prefix =
            TemplatePatterns_T_::GetLoopPrefix();
        static const Char_T_ *if_prefix = TemplatePatterns_T_::GetIfPrefix();

        UInt tmp_offset;

#ifdef QENTEM_SIMD_ENABLED_
        do {
            if ((find_cache_.Bits == 0) &&
                (find_cache_.NextOffset < end_before)) {
                qmmFind(content, find_cache_.NextOffset, end_before);
            }

            if (find_cache_.Bits == 0) {
                break;
            }

            ULong index =
                (Platform::CTZL(find_cache_.Bits) + find_cache_.Offset);
            find_cache_.Bits &= (find_cache_.Bits - 1);

            if (index >= end_before) {
                return;
            }

            if (index >= offset) {
                ++offset;
                switch (content[index]) {
#else
        ULong index = 0;
        while (offset < end_before) {
            switch (content[offset]) {
#endif
                    case TemplatePatterns_T_::InLinePrefix: {
#ifndef QENTEM_SIMD_ENABLED_
                        index = offset;
#endif
                        ++index;

                        switch (content[index]) {
                            case TemplatePatterns_T_::Raw_Var_2ND_Char: {
                                if ((TemplatePatterns_T_::
                                         RawVariablePrefixLength +
                                     index) < end_before) {
                                    ++index;
                                    tmp_offset = 2;

                                    while ((tmp_offset !=
                                            TemplatePatterns_T_::
                                                RawVariablePrefixLength) &&
                                           (content[index] ==
                                            raw_variable_prefix[tmp_offset])) {
                                        ++index;
                                        ++tmp_offset;
                                    }

                                    if (tmp_offset ==
                                        TemplatePatterns_T_::
                                            RawVariablePrefixLength) {
                                        ULong current_offset = Engine::FindOne(
                                            TemplatePatterns_T_::
                                                GetInLineSuffix()[0],
                                            content, index, end_before);

                                        if (current_offset != 0) {
                                            tag_ = Tag::Raw;
                                            found(content, end_before, index,
                                                  current_offset);
                                            offset = current_offset;
                                        } else {
                                            offset = index;
                                        }

                                        continue;
                                    }
                                }

                                break;
                            }

                            case TemplatePatterns_T_::Var_2ND_Char: {
                                if ((TemplatePatterns_T_::VariablePrefixLength +
                                     index) < end_before) {
                                    ++index;
                                    tmp_offset = 2;

                                    while ((tmp_offset !=
                                            TemplatePatterns_T_::
                                                VariablePrefixLength) &&
                                           (content[index] ==
                                            variable_prefix[tmp_offset])) {
                                        ++index;
                                        ++tmp_offset;
                                    }

                                    if (tmp_offset ==
                                        TemplatePatterns_T_::
                                            VariablePrefixLength) {
                                        ULong current_offset = Engine::FindOne(
                                            TemplatePatterns_T_::
                                                GetInLineSuffix()[0],
                                            content, index, end_before);

                                        if (current_offset != 0) {
                                            tag_ = Tag::Variable;
                                            found(content, end_before, index,
                                                  current_offset);
                                            offset = current_offset;
                                        } else {
                                            offset = index;
                                        }

                                        continue;
                                    }
                                }

                                break;
                            }

                            case TemplatePatterns_T_::MAth_2ND_Char: {
                                if ((TemplatePatterns_T_::MathPrefixLength +
                                     index) < end_before) {
                                    ++index;
                                    tmp_offset = 2;

                                    while ((tmp_offset !=
                                            TemplatePatterns_T_::
                                                MathPrefixLength) &&
                                           (content[index] ==
                                            math_prefix[tmp_offset])) {
                                        ++index;
                                        ++tmp_offset;
                                    }

                                    if (tmp_offset ==
                                        TemplatePatterns_T_::MathPrefixLength) {
                                        ULong current_offset = Engine::FindOne(
                                            TemplatePatterns_T_::
                                                GetInLineSuffix()[0],
                                            content, index, end_before);

                                        if (current_offset != 0) {
                                            tag_ = Tag::Math;
                                            found(content, end_before, index,
                                                  current_offset);
                                            offset = current_offset;
                                        } else {
                                            offset = index;
                                        }

                                        continue;
                                    }
                                }

                                break;
                            }

                            case TemplatePatterns_T_::InlineIf_2ND_Char: {
                                if ((TemplatePatterns_T_::InLineIfPrefixLength +
                                     index) < end_before) {
                                    ++index;
                                    tmp_offset = 2;

                                    while ((tmp_offset !=
                                            TemplatePatterns_T_::
                                                InLineIfPrefixLength) &&
                                           (content[index] ==
                                            inLine_if_prefix[tmp_offset])) {
                                        ++index;
                                        ++tmp_offset;
                                    }

                                    if (tmp_offset ==
                                        TemplatePatterns_T_::
                                            InLineIfPrefixLength) {
                                        ULong current_offset = Engine::FindOne(
                                            TemplatePatterns_T_::
                                                GetInLineSuffix()[0],
                                            content, index, end_before);

                                        if (current_offset != 0) {
                                            tag_ = Tag::InLineIf;
                                            found(content, end_before, index,
                                                  current_offset);
                                            offset = current_offset;
                                        } else {
                                            offset = index;
                                        }

                                        continue;
                                    }
                                }
                            }

                            default: {
                            }
                        }

                        break;
                    }

                    case TemplatePatterns_T_::MultiLinePrefix: {
#ifndef QENTEM_SIMD_ENABLED_
                        index = offset;
#endif
                        ++index;

                        switch (content[index]) {
                            case TemplatePatterns_T_::Loop_2ND_Char: { // <loop
                                if ((TemplatePatterns_T_::LoopPrefixLength +
                                     index) < end_before) {
                                    ++index;
                                    tmp_offset = 2;

                                    while ((tmp_offset <
                                            TemplatePatterns_T_::
                                                LoopPrefixLength) &&
                                           (content[index] ==
                                            loop_prefix[tmp_offset])) {
                                        ++index;
                                        ++tmp_offset;
                                    }

                                    if (tmp_offset ==
                                        TemplatePatterns_T_::LoopPrefixLength) {
                                        ULong current_offset = Engine::Find(
                                            TemplatePatterns_T_::
                                                GetLoopSuffix(),
                                            TemplatePatterns_T_::
                                                LoopSuffixLength,
                                            content, index, end_before);

                                        if (current_offset != 0) {
                                            tag_ = Tag::Loop;
                                            found(content, end_before, index,
                                                  current_offset);
                                            offset = current_offset;
                                        } else {
                                            offset = index;
                                        }

                                        continue;
                                    }
                                }

                                break;
                            }

                            case TemplatePatterns_T_::If_2ND_Char: { // <if
                                if ((TemplatePatterns_T_::IfPrefixLength +
                                     index) < end_before) {
                                    ++index;
                                    tmp_offset = 2;

                                    while (
                                        (tmp_offset !=
                                         TemplatePatterns_T_::IfPrefixLength) &&
                                        (content[index] ==
                                         if_prefix[tmp_offset])) {
                                        ++index;
                                        ++tmp_offset;
                                    }

                                    if (tmp_offset ==
                                        TemplatePatterns_T_::IfPrefixLength) {
                                        ULong current_offset = Engine::Find(
                                            TemplatePatterns_T_::GetIfSuffix(),
                                            TemplatePatterns_T_::IfSuffixLength,
                                            content, index, end_before);

                                        if (current_offset != 0) {
                                            tag_ = Tag::If;
                                            found(content, end_before, index,
                                                  current_offset);
                                            offset = current_offset;
                                        } else {
                                            offset = index;
                                        }

                                        continue;
                                    }
                                }

                                break;
                            }

                            default: {
                            }
                        }
                    }

                    default: {
                    }
                }

#ifdef QENTEM_SIMD_ENABLED_
            }
        } while (true);
#else
            ++offset;
        }
#endif
    }

    inline void addPreviousContent(const Char_T_ *content, ULong offset) const {
        // Noneed to check if the  length is zero; because Stringstream will
        // ignore it If it is.
        ss_->Insert((content + last_offset_), (offset - last_offset_));
    }

    void found(const Char_T_ *content, ULong end_before, ULong start_offset,
               ULong &current_offset) {
        switch (tag_) {
            case Tag::Raw: {
                addPreviousContent(
                    content,
                    (start_offset -
                     TemplatePatterns_T_::RawVariablePrefixLength)); // {var:

                renderRawVariable((content + start_offset),
                                  ((current_offset - 1) - start_offset));

                // - 1 is - TemplatePatterns_T_::InLineSuffixLength
                break;
            }

            case Tag::Variable: {
                addPreviousContent(
                    content,
                    (start_offset -
                     TemplatePatterns_T_::VariablePrefixLength)); // {var:

                renderVariable((content + start_offset),
                               ((current_offset - 1) - start_offset));

                // - 1 is - TemplatePatterns_T_::InLineSuffixLength
                break;
            }

            case Tag::Math: {
                addPreviousContent(
                    content,
                    (start_offset -
                     TemplatePatterns_T_::MathPrefixLength)); // {math:

                current_offset = Engine::SkipInnerPatterns(
                    TemplatePatterns_T_::GetVariablePrefix(),
                    TemplatePatterns_T_::VariablePrefixLength,
                    TemplatePatterns_T_::GetInLineSuffix(), 1, content,
                    start_offset, current_offset, end_before);

                renderMath(
                    (content + start_offset),
                    static_cast<UInt>((current_offset - 1) - start_offset));

                // - 1 is - TemplatePatterns_T_::InLineSuffixLength
                break;
            }

            case Tag::InLineIf: {
                addPreviousContent(
                    content,
                    (start_offset -
                     TemplatePatterns_T_::InLineIfPrefixLength)); // {if

                current_offset = Engine::SkipInnerPatterns(
                    TemplatePatterns_T_::GetVariablePrefix(),
                    TemplatePatterns_T_::VariablePrefixLength,
                    TemplatePatterns_T_::GetInLineSuffix(), 1, content,
                    start_offset, current_offset, end_before);

                renderInLineIf((content + start_offset),
                               ((current_offset - 1) - start_offset));

                // - 1 is - TemplatePatterns_T_::InLineSuffixLength
                break;
            }

            case Tag::Loop: {
                addPreviousContent(
                    content,
                    (start_offset -
                     TemplatePatterns_T_::LoopPrefixLength)); // <loop

                current_offset = Engine::SkipInnerPatterns(
                    TemplatePatterns_T_::GetLoopPrefix(),
                    TemplatePatterns_T_::LoopPrefixLength,
                    TemplatePatterns_T_::GetLoopSuffix(),
                    TemplatePatterns_T_::LoopSuffixLength, content,
                    start_offset, current_offset, end_before);

                renderLoop(
                    (content + start_offset),
                    ((current_offset - TemplatePatterns_T_::LoopSuffixLength) -
                     start_offset));
                break;
            }

            case Tag::If: {
                addPreviousContent(
                    content,
                    (start_offset -
                     TemplatePatterns_T_::IfPrefixLength)); // <if

                current_offset = Engine::SkipInnerPatterns(
                    TemplatePatterns_T_::GetIfPrefix(),
                    TemplatePatterns_T_::IfPrefixLength,
                    TemplatePatterns_T_::GetIfSuffix(),
                    TemplatePatterns_T_::IfSuffixLength, content, start_offset,
                    current_offset, end_before);

                renderIf((content + start_offset),
                         (current_offset - start_offset));
            }

            default: {
            }
        }

        last_offset_ = current_offset;
    }

    /*
     * Gets anything between "..."
     */
    bool getQuoted(UInt &offset, UInt &length, const Char_T_ *content,
                   const ULong end_before) const noexcept {
        offset = static_cast<UInt>(Engine::FindOne(
            TemplatePatterns_T_::QuoteChar, content, offset, end_before));

        if (offset != 0) {
            const ULong start_offset = Engine::FindOne(
                TemplatePatterns_T_::QuoteChar, content, offset, end_before);

            if (start_offset != 0) {
                length = static_cast<UInt>((start_offset - 1) - offset);
                return true;
            }
        }

        return false;
    }

    bool parseNumber(ULong &number, const Char_T_ *content,
                     const UInt length) const noexcept {
        if (length > TemplatePatterns_T_::VariableFulllength) {
            ULong offset = 0;
            offset = Engine::Find(TemplatePatterns_T_::GetVariablePrefix(),
                                  TemplatePatterns_T_::VariablePrefixLength,
                                  content, offset, length);

            if (offset != 0) {
                const ULong end_offset =
                    Engine::FindOne(TemplatePatterns_T_::GetInLineSuffix()[0],
                                    content, offset, length);

                if (end_offset == 0) {
                    return false;
                }

                const Value_T_ *value =
                    findValue((content + offset), ((end_offset - 1) - offset));
                // -1 is - TemplatePatterns_T_::InLineSuffixLength

                if (value != nullptr) {
                    double num;

                    if (value->GetNumber(num)) {
                        number = static_cast<ULong>(num);
                        return true;
                    }
                }

                return false;
            }
        }

        return Digit<Char_T_>::StringToNumber(number, content, length);
    }

    void parseVariables(const Char_T_ *content, UInt length) const {
        ULong offset = 0;
        ULong last_offset;

        do {
            last_offset = offset;

            offset = Engine::Find(TemplatePatterns_T_::GetVariablePrefix(),
                                  TemplatePatterns_T_::VariablePrefixLength,
                                  content, offset, length);

            if (offset == 0) {
                break;
            }

            if (last_offset < offset) {
                // Add any content that comes before any {var:x}
                ss_->Insert(
                    (content + last_offset),
                    ((offset - TemplatePatterns_T_::VariablePrefixLength) -
                     last_offset));
            }

            const ULong start_offset = offset;
            offset = Engine::FindOne(TemplatePatterns_T_::GetInLineSuffix()[0],
                                     content, offset, length);

            renderVariable((content + start_offset),
                           ((offset - 1) - start_offset));

            // -1 is - TemplatePatterns_T_::InLineSuffixLength
        } while (true);

        if (last_offset < length) {
            // Add any content that comes after }
            ss_->Insert((content + last_offset), (length - last_offset));
        }
    }

    void renderRawVariable(const Char_T_ *content, ULong length) const {
        const Value_T_ *value = findValue(content, length);

        if ((value == nullptr) || !(value->InsertString(*ss_))) {
            length += TemplatePatterns_T_::RawVariableFulllength;
            ss_->Insert(
                (content - TemplatePatterns_T_::RawVariablePrefixLength),
                length);
        }
    }

    void renderVariable(const Char_T_ *content, ULong length) const {
        const Value_T_ *value = findValue(content, length);

        if ((value == nullptr) || !(value->InsertString(*ss_))) {
            length += TemplatePatterns_T_::VariableFulllength;
            ss_->Insert((content - TemplatePatterns_T_::VariablePrefixLength),
                        length);
        }
    }

    void renderMath(const Char_T_ *content, UInt length) const {
        double number;

        if (ALE::Evaluate(number, content, length, this)) {
            Digit<Char_T_>::NumberToStringStream(*ss_, number, 1, 0, 3);
        } else {
            length += TemplatePatterns_T_::MathFulllength;
            ss_->Insert((content - TemplatePatterns_T_::MathPrefixLength),
                        length);
        }
    }

    void renderInLineIf(const Char_T_ *content, const ULong length) const {
        UInt offset      = 0;
        UInt last_offset = 0;
        UInt len         = 0;
        UInt times       = 3;
        bool case_value  = false;

        do {
            ++len;
            offset += len; // Move to the next Char_T_.

            if (!getQuoted(offset, len, content, length)) {
                break;
            }

            if (len == 0) {
                return;
            }

            // = + " + a Char_T_ == 3 + the Char_T_ before it == 4
            UInt tmp_offset   = offset - 4;
            bool iif_not_done = true;

            do {
                switch (content[tmp_offset]) {
                    case TemplatePatterns_T_::CaseChar: {
                        iif_not_done = false;
                        double number;

                        if (!(ALE::Evaluate(number, (content + offset), len,
                                            this))) {
                            // A messed-up case.
                            return;
                        }

                        case_value = (number > 0.0);
                        break;
                    }

                    case TemplatePatterns_T_::TrueChar: {
                        iif_not_done = false;

                        if (case_value) {
                            parseVariables((content + offset), len);
                            return;
                        }

                        break;
                    }

                    case TemplatePatterns_T_::FalseChar: {
                        iif_not_done = false;

                        if (!case_value) {
                            parseVariables((content + offset), len);
                            return;
                        }

                        break;
                    }

                    default: {
                    }
                }
            } while (iif_not_done && (--tmp_offset > last_offset));

            last_offset = offset;
        } while (--times != 0);
    }

    struct loopItem_ {
        UInt Offset;
        UInt Type;
        UInt SubOffset;
        UInt SubLength;
    };

    void renderLoop(const Char_T_ *content, ULong length) const {
        const Value_T_ *root_value   = root_value_;
        const Char_T_ * key_str      = nullptr;
        const Char_T_ * value_str    = nullptr;
        ULong           loop_times   = 0;
        ULong           index        = 0;
        UInt            key_length   = 0;
        UInt            value_length = 0;
        UInt            len          = 0;
        UInt            offset       = 0;
        UInt            last_offset  = 0;

        UInt times = 5; // set, key, value, times, index

        const ULong start_offset = Engine::FindOne(
            TemplatePatterns_T_::MultiLineSuffix, content, 0, length);
        if (start_offset == 0) {
            // The syntax is wrong.
            return;
        }

        length -= start_offset;

        do {
            ++len;
            offset += len; // Move to the next Char_T_.

            if (!getQuoted(offset, len, content, start_offset)) {
                break;
            }

            if (len == 0) {
                // The syntax is wrong.
                return;
            }

            // X="|
            // 3: Goes back to X
            // |X="
            UInt tmp_offset = (offset - 3);

            do {
                switch (content[tmp_offset]) {
                    case TemplatePatterns_T_::SetChar: {
                        const Value_T_ *value =
                            findValue((content + offset), len);

                        if ((value != nullptr) &&
                            (value->IsArray() || value->IsObject())) {
                            root_value  = value;
                            last_offset = tmp_offset; // Break loop.
                            break;
                        }

                        return; // set="..." has invalid value.
                    }

                    case TemplatePatterns_T_::KeyChar: {
                        key_str     = (content + offset);
                        key_length  = len;
                        last_offset = tmp_offset; // Break loop.
                        break;
                    }

                    case TemplatePatterns_T_::ValueChar: {
                        value_str    = (content + offset);
                        value_length = len;
                        last_offset  = tmp_offset; // Break loop.
                        break;
                    }

                    case TemplatePatterns_T_::TimesChar: {
                        if (!parseNumber(loop_times, (content + offset), len)) {
                            return;
                        }

                        last_offset = tmp_offset; // Break loop.
                        break;
                    }

                    case TemplatePatterns_T_::IndexChar: {
                        if (!parseNumber(index, (content + offset), len)) {
                            return;
                        }

                        last_offset = tmp_offset; // Break loop.
                        break;
                    }

                    default: {
                    }
                }
            } while (--tmp_offset > last_offset);

            last_offset = offset;
        } while (--times != 0);

        content = (content + start_offset);

        const Array<loopItem_> loop_items = loopMatch(
            content, length, key_str, value_str, key_length, value_length);

        loopReplace(loop_items, content, length, index, loop_times, root_value,
                    key_length, value_length);
    }

    Array<loopItem_> loopMatch(const Char_T_ *content, ULong length,
                               const Char_T_ *key, const Char_T_ *value,
                               UInt key_length, UInt value_length) const {
        Array<loopItem_> items;
        loopItem_        key_item{0, 0, 0, 0};
        loopItem_        value_item{0, 0, 0, 0};

        UInt value_offset = 0;
        UInt key_offset   = 0;

        do {
            UInt sub_offset = 0;
            UInt sub_length = 0;

            if ((value != nullptr) && (value_item.Type == 0)) {
                value_offset = static_cast<UInt>(Engine::Find(
                    value, value_length, content, value_offset, length));

                if (value_offset == 0) {
                    value = nullptr; // To stop looking for it.
                } else {
                    if (content[value_offset] ==
                        TemplatePatterns_T_::VariableIndexPrefix) {
                        sub_offset = (value_offset + 1);

                        for (UInt i = sub_offset; i < length; i++) {
                            if (content[i] ==
                                TemplatePatterns_T_::VariableIndexSuffix) {
                                sub_length = (i - sub_offset);
                                break;
                            }
                        }
                    }

                    value_item = {(value_offset - value_length), 2, sub_offset,
                                  sub_length}; // value is 2
                }
            }

            if ((key != nullptr) && (key_item.Type == 0)) {
                key_offset = static_cast<UInt>(
                    Engine::Find(key, key_length, content, key_offset, length));

                if (key_offset == 0) {
                    key = nullptr; // To stop looking for it.
                } else {
                    key_item = {(key_offset - key_length), 1, 0,
                                0}; // key is one
                }
            }

            // Sorting by the offset
            if (key_item.Type == 0) {
                if (value_item.Type != 0) {
                    items += value_item;
                    value_item.Type = 0; // Reset
                }
            } else if (value_item.Type == 0) {
                items += key_item;
                key_item.Type = 0; // Reset
            } else {
                // Both are set
                if (value_item.Offset > key_item.Offset) {
                    items += key_item;
                    key_item.Type = 0; // Reset
                } else {
                    items += value_item;
                    value_item.Type = 0; // Reset
                }
            }

        } while ((key != nullptr) || (value != nullptr));

        // items.Compress();

        return items;
    }

    void loopReplace(const Array<loopItem_> &items, const Char_T_ *content,
                     ULong length, ULong index, ULong times,
                     const Value_T_ *root_value, UInt key_length,
                     UInt value_length) const {
        StringStream<Char_T_>  loop_ss;
        ULong                  last_offset;
        ULong                  sub_id;
        StringStream<Char_T_> *current_ss = &loop_ss;
        const Value_T_ *       value      = nullptr;
        const loopItem_ *      end        = (items.Storage() + items.Size());

        if (times == 0) {
            times = root_value->Size();

            if (times <= index) {
                return;
            }

            times -= index;
        }

        while (times != 0) {
            last_offset           = 0;
            const loopItem_ *item = items.Storage();

            while (item < end) {
                if (last_offset < item->Offset) {
                    current_ss->Insert((content + last_offset),
                                       (item->Offset - last_offset));
                }

                if (item->Type == 2) {
                    last_offset = item->Offset;
                    last_offset += value_length;

                    value = root_value->GetValue(index);

                    if (value != nullptr) {
                        if (item->SubLength == 0) {
                            value->InsertString(*current_ss);
                        } else {
                            last_offset += item->SubLength;
                            last_offset += 2U;

                            if (value->IsObject()) {
                                value =
                                    value->GetValue((content + item->SubOffset),
                                                    item->SubLength);
                            } else {
                                if ((Digit<Char_T_>::StringToNumber(
                                        sub_id, (content + item->SubOffset),
                                        item->SubLength))) {
                                    value = value->GetValue(sub_id);
                                } else {
                                    value = nullptr;
                                }
                            }

                            if (value != nullptr) {
                                value->InsertString(*current_ss);
                            }
                        }
                    }
                } else {
                    last_offset = item->Offset;
                    last_offset += key_length;

                    if (root_value->IsObject()) {
                        root_value->InsertKey(*current_ss, index);
                    } else {
                        Digit<Char_T_>::NumberToStringStream(*current_ss,
                                                             index);
                    }
                }

                ++item;
            }

            // Add the remaining string.
            if (last_offset != length) {
                current_ss->Insert((content + last_offset),
                                   (length - last_offset));
            }

            if (current_ss == &loop_ss) {
                Template_T_<Char_T_, Value_T_> loop_temp{ss_, root_value_};

                loop_temp.find(loop_ss.Storage(), 0, loop_ss.Length());

                ss_->Insert((loop_ss.Storage() + loop_temp.last_offset_),
                            (loop_ss.Length() - loop_temp.last_offset_));

                if (loop_temp.last_offset_ != 0) {
                    loop_ss.SoftReset();
                } else {
                    loop_ss.Reset();
                    // It hash no inner tags, therefore, No need for rendering.
                    current_ss = ss_;
                }
            }

            ++index;
            --times;
        }
    }

    void renderIf(const Char_T_ *content, ULong length) {
        double result      = 0;
        ULong  offset      = 0;
        UInt   case_offset = 0;
        UInt   case_length = 0;

        if (getQuoted(case_offset, case_length, content, length)) {
            offset = case_offset;
            offset += case_length;

            offset = Engine::FindOne(TemplatePatterns_T_::MultiLineSuffix,
                                     content, offset, length);
        }

        do {
            if (((case_length != 0) &&
                 !(ALE::Evaluate(result, (content + case_offset), case_length,
                                 this))) ||
                (offset == 0)) {
                // Error: <if> syntax is wrong; no case=""
                return;
            }

            // Find <else..
            case_offset = findNextElse(content, offset, length);

            if ((result > 0) || (case_offset == 0)) {
                break;
            }

            // <else has no "i" after it.
            if (content[case_offset] != TemplatePatterns_T_::ElseIfChar) {
                result      = 1; // Mark <else as true.
                case_length = 0;
            } else if (!getQuoted(case_offset, case_length, content, length)) {
                // Error: <elseif has no case=""
                return;
            }

            // Find the end of else/elseif
            offset = Engine::Find(TemplatePatterns_T_::GetElseSuffix(),
                                  TemplatePatterns_T_::ElseSuffixLength,
                                  content, (case_offset + case_length), length);
        } while (true);

        if (result > 0) {
            if (case_offset != 0) {
                length = case_offset;
            }

            Template::Render(
                *ss_, (content + offset),
                ((length - TemplatePatterns_T_::IfSuffixLength) - offset),
                root_value_);
        }
    }

    static UInt findNextElse(const Char_T_ *content, ULong offset,
                             ULong length) noexcept {
        ULong else_offset = 0;

        do {
            else_offset = Engine::Find(TemplatePatterns_T_::GetElsePrefix(),
                                       TemplatePatterns_T_::ElsePrefixLength,
                                       content, offset, length);

            if (else_offset == 0) {
                // No <else was found.
                return 0;
            }

            const ULong next_if = Engine::Find(
                TemplatePatterns_T_::GetIfPrefix(),
                TemplatePatterns_T_::IfPrefixLength, content, offset, length);

            if ((next_if == 0) || (else_offset < next_if)) {
                // No nesting <ifs were found, or <else
                // comes before the child <if
                break;
            }

            offset = Engine::Find(TemplatePatterns_T_::GetIfSuffix(),
                                  TemplatePatterns_T_::IfSuffixLength, content,
                                  next_if, length);

            if (else_offset > offset) {
                // <else came after the child if.
                break;
            }
        } while (true);

        return static_cast<UInt>(else_offset);
    }

    /*
     * Key can be: name/id, name/id[name/id], name/id[name/id][sub-name/id],
     * name/id[name/id][sub-name/id][sub-sub-name/id]... "name": a string that
     * is stored in "keys_". "id" is the index that starts with 0: values_[id]
     */
    const Value_T_ *findValue(const Char_T_ *key, ULong length) const noexcept {
        // TODO: Implement a caching method for values.

        if (length != 0) {
            const Value_T_ *root_value = root_value_;
            ULong           offset     = 0;
            ULong           next_offset;

            if (key[(length - 1)] == TemplatePatterns_T_::VariableIndexSuffix) {
                next_offset = 0;
                // Starting with a string followed by [...]
                while ((next_offset < length) &&
                       (key[++next_offset] !=
                        TemplatePatterns_T_::VariableIndexPrefix)) {
                }

                --length; // No use for ending bracket.
            } else {
                next_offset = length;
            }

            do {
                const Value_T_ *value = nullptr;

                if (root_value->IsArray()) {
                    ULong value_id;

                    if (!(Digit<Char_T_>::StringToNumber(
                            value_id, (key + offset),
                            static_cast<UInt>(next_offset - offset)))) {
                        break;
                    }

                    value = root_value->GetValue(value_id);
                } else {
                    value = root_value->GetValue((key + offset),
                                                 (next_offset - offset));
                }

                if (value == nullptr) {
                    break;
                }

                if (next_offset == length) {
                    return value;
                }

                root_value = value;

                // Next part: starts at next_offset
                offset = next_offset;
                while (
                    (offset < length) &&
                    (key[offset] != TemplatePatterns_T_::VariableIndexPrefix)) {
                    ++offset;
                }

                // Ends at offset + 1
                next_offset = ++offset; // ++ to the next Char_T_ after [
                while ((next_offset < length) &&
                       (key[++next_offset] !=
                        TemplatePatterns_T_::VariableIndexSuffix)) {
                }
            } while (true);
        }

        return nullptr;
    }

    bool ALESetNumber(double &number, const Char_T_ *content,
                      UInt length) const {
        if (length > TemplatePatterns_T_::VariableFulllength) {
            const Value_T_ *value = findValue(
                (content + TemplatePatterns_T_::VariablePrefixLength),
                (length - TemplatePatterns_T_::VariableFulllength)); // {var:x}

            if (value != nullptr) {
                if (value->GetNumber(number)) {
                    return true;
                }

                const Char_T_ *str;
                ULong          len;

                if (value->SetCharAndLength(str, len)) {
                    return ALE::Evaluate(number, str, static_cast<UInt>(len),
                                         this);
                }

                return value->GetNumber(number);
            }
        }

        return false;
    }

    bool ALEIsEqual(bool &result, const Char_T_ *left, UInt left_length,
                    const Char_T_ *right, UInt right_length) const {
        const Value_T_ *value_left{nullptr};
        const Value_T_ *value_right{nullptr};
        const Char_T_ * str_left;
        const Char_T_ * str_right;
        ULong           str_length_left;
        ULong           str_length_right;

        bool is_number = false;

        // If the right side is a variable
        if ((left[0] == TemplatePatterns_T_::GetVariablePrefix()[0]) &&
            (left_length > TemplatePatterns_T_::VariableFulllength)) {
            value_left =
                findValue((left + TemplatePatterns_T_::VariablePrefixLength),
                          (left_length -
                           TemplatePatterns_T_::VariableFulllength)); // {var:x}

            if (value_left == nullptr) {
                return false;
            }

            is_number = value_left->IsNumber();
        }

        // If the left side is a variable
        if ((right[0] == TemplatePatterns_T_::GetVariablePrefix()[0]) &&
            (right_length > TemplatePatterns_T_::VariableFulllength)) {
            value_right =
                findValue((right + TemplatePatterns_T_::VariablePrefixLength),
                          (right_length -
                           TemplatePatterns_T_::VariableFulllength)); // {var:x}

            if (value_right == nullptr) {
                return false;
            }

            if (!is_number) {
                is_number = value_right->IsNumber();
            }
        }

        if (is_number) {
            double num_left;

            if (value_left != nullptr) {
                if (!(value_left->GetNumber(num_left))) {
                    return false;
                }
            } else if (!(Digit<Char_T_>::StringToNumber(num_left, left,
                                                        left_length))) {
                return false;
            }

            double num_right;

            if (value_right != nullptr) {
                if (!(value_right->GetNumber(num_right))) {
                    return false;
                }
            } else if (!(Digit<Char_T_>::StringToNumber(num_right, right,
                                                        right_length))) {
                return false;
            }

            result = (num_left == num_right);
            return true;
        }

        if (value_left != nullptr) {
            if (!(value_left->SetCharAndLength(str_left, str_length_left))) {
                return false;
            }
        } else {
            str_left        = left;
            str_length_left = left_length;
        }

        if (value_right != nullptr) {
            if (!(value_right->SetCharAndLength(str_right, str_length_right))) {
                return false;
            }
        } else {
            str_right        = right;
            str_length_right = right_length;
        }

        result = ((str_length_left == str_length_right) &&
                  StringUtils::IsEqual(str_left, str_right, str_length_right));
        return true;
    }

    Template_T_(StringStream<Char_T_> *ss, const Value_T_ *root_value) noexcept
        : ss_(ss), root_value_(root_value) {
    }

    struct FindCache_ {
        ULong        Offset{0};
        ULong        NextOffset{0};
        QMM_Number_T Bits{0};
    };

    StringStream<Char_T_> *ss_;
    const Value_T_ *       root_value_;
    FindCache_             find_cache_{};

    ULong last_offset_{0};
    Tag   tag_{Tag::Variable};
};

} // namespace Qentem

#endif
