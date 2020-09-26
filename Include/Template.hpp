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
#include "FixedArray.hpp"
#include "StringStream.hpp"
#include "TemplatePatterns.hpp"

#ifndef QENTEM_TEMPLATE_H_
#define QENTEM_TEMPLATE_H_

#ifndef QENTEM_TEMPLATE_PARSE_ARRAY_SIZE_H_
#define QENTEM_TEMPLATE_PARSE_ARRAY_SIZE_H_ 16U
#endif

// TODO: Add autoescape and use * for raw variables

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

    template <typename Char_T_, typename Value_T_, typename Number_T_>
    inline static void Render(const Char_T_ *content, Number_T_ length,
                              const Value_T_ *       root_value,
                              StringStream<Char_T_> *ss) {
        Template_T_<Char_T_, Value_T_>{ss, root_value}.process(
            content, static_cast<UInt>(length));
    }

    template <typename Char_T_, typename Value_T_, typename Number_T_>
    inline static StringStream<Char_T_> Render(const Char_T_ * content,
                                               Number_T_       length,
                                               const Value_T_ *root_value) {
        StringStream<Char_T_> ss;
        Render(content, length, root_value, &ss);
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
    enum class TagType_ : UInt {
        Variable = 0U, // {var:x}
        Math,          // {math:x}
        InLineIf,      // {if:x}
        Loop,          // <loop set="..." key="..." value="...">
        If,            // <if case="...">
    };

    struct Tag_T_ {
        TagType_ Type;
        UInt     Offset;
        UInt     ContentOffset;
        UInt     EndOffset;
    };

    using Tags_T_ = FixedArray<Tag_T_, QENTEM_TEMPLATE_PARSE_ARRAY_SIZE_H_>;

    template <typename, typename>
    friend class Qentem::ALE_T_;
    friend class Qentem::Template;

    QENTEM_NOINLINE void process(const Char_T_ *content, UInt length) const {
        Tags_T_ tags;
        UInt    offset = 0U;

        while (offset != length) {
            parse(tags, content, offset, length);

            if (tags.IsEmpty()) {
                break;
            }

            render(tags.First(), tags.End(), content, offset);
            offset = (tags.First() + tags.Size() - 1)->EndOffset;

            if (!(tags.IsFull())) {
                break;
            }

            tags.Clear();
        }

        // Add the remaining string.
        ss_->Insert((content + offset), (length - offset));
    }

    QENTEM_NOINLINE static void parse(Tags_T_ &tags, const Char_T_ *content,
                                      UInt offset, UInt length) {
        UInt current_offset;
        UInt tmp_offset;

        static const Char_T_ *variable_prefix =
            TemplatePatterns_T_::GetVariablePrefix();
        static const Char_T_ *math_prefix =
            TemplatePatterns_T_::GetMathPrefix();
        static const Char_T_ *inLine_if_prefix =
            TemplatePatterns_T_::GetInLineIfPrefix();
        static const Char_T_ *loop_prefix =
            TemplatePatterns_T_::GetLoopPrefix();
        static const Char_T_ *if_prefix = TemplatePatterns_T_::GetIfPrefix();

        while (offset < length) {
            switch (content[offset]) {
                case TemplatePatterns_T_::InLinePrefix: {
                    current_offset = offset;
                    ++current_offset;

                    switch (content[current_offset]) {
                        case TemplatePatterns_T_::Var_2ND_Char: {
                            if ((TemplatePatterns_T_::VariablePrefixLength +
                                 current_offset) < length) {
                                ++current_offset;
                                tmp_offset = 2U;

                                while ((tmp_offset !=
                                        TemplatePatterns_T_::
                                            VariablePrefixLength) &&
                                       (content[current_offset] ==
                                        variable_prefix[tmp_offset])) {
                                    ++current_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset ==
                                    TemplatePatterns_T_::VariablePrefixLength) {
                                    UInt end_offset = Engine::FindOne(
                                        TemplatePatterns_T_::GetInLineSuffix()
                                            [0U],
                                        content, current_offset, length);

                                    if (end_offset != 0U) {
                                        tags += {TagType_::Variable, offset,
                                                 current_offset, end_offset};

                                        if (tags.IsFull()) {
                                            return;
                                        }

                                        offset = end_offset;
                                        continue;
                                    }

                                    offset = current_offset;
                                }
                            }

                            break;
                        }

                        case TemplatePatterns_T_::Math_2ND_Char: {
                            if ((TemplatePatterns_T_::MathPrefixLength +
                                 current_offset) < length) {
                                ++current_offset;
                                tmp_offset = 2U;

                                while (
                                    (tmp_offset !=
                                     TemplatePatterns_T_::MathPrefixLength) &&
                                    (content[current_offset] ==
                                     math_prefix[tmp_offset])) {
                                    ++current_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset ==
                                    TemplatePatterns_T_::MathPrefixLength) {
                                    UInt end_offset = Engine::FindOne(
                                        TemplatePatterns_T_::GetInLineSuffix()
                                            [0U],
                                        content, current_offset, length);

                                    if (end_offset != 0U) {
                                        end_offset = Engine::SkipInnerPatterns(
                                            TemplatePatterns_T_::
                                                GetVariablePrefix(),
                                            TemplatePatterns_T_::
                                                VariablePrefixLength,
                                            TemplatePatterns_T_::
                                                GetInLineSuffix(),
                                            1U, content, current_offset,
                                            end_offset, length);

                                        tags += {TagType_::Math, offset,
                                                 current_offset, end_offset};

                                        if (tags.IsFull()) {
                                            return;
                                        }

                                        offset = end_offset;
                                        continue;
                                    }

                                    offset = current_offset;
                                }
                            }

                            break;
                        }

                        case TemplatePatterns_T_::InlineIf_2ND_Char: {
                            if ((TemplatePatterns_T_::InLineIfPrefixLength +
                                 current_offset) < length) {
                                ++current_offset;
                                tmp_offset = 2U;

                                while ((tmp_offset !=
                                        TemplatePatterns_T_::
                                            InLineIfPrefixLength) &&
                                       (content[current_offset] ==
                                        inLine_if_prefix[tmp_offset])) {
                                    ++current_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset ==
                                    TemplatePatterns_T_::InLineIfPrefixLength) {
                                    UInt end_offset = Engine::FindOne(
                                        TemplatePatterns_T_::GetInLineSuffix()
                                            [0U],
                                        content, current_offset, length);

                                    if (end_offset != 0U) {
                                        end_offset = Engine::SkipInnerPatterns(
                                            TemplatePatterns_T_::
                                                GetVariablePrefix(),
                                            TemplatePatterns_T_::
                                                VariablePrefixLength,
                                            TemplatePatterns_T_::
                                                GetInLineSuffix(),
                                            1U, content, current_offset,
                                            end_offset, length);

                                        tags += {TagType_::InLineIf, offset,
                                                 current_offset, end_offset};

                                        if (tags.IsFull()) {
                                            return;
                                        }

                                        offset = end_offset;
                                        continue;
                                    }

                                    offset = current_offset;
                                }
                            }
                        }

                        default: {
                        }
                    }

                    break;
                }

                case TemplatePatterns_T_::MultiLinePrefix: {
                    current_offset = offset;
                    ++current_offset;

                    switch (content[current_offset]) {
                        case TemplatePatterns_T_::Loop_2ND_Char: { // <loop
                            if ((TemplatePatterns_T_::LoopPrefixLength +
                                 current_offset) < length) {
                                ++current_offset;
                                tmp_offset = 2U;

                                while (
                                    (tmp_offset <
                                     TemplatePatterns_T_::LoopPrefixLength) &&
                                    (content[current_offset] ==
                                     loop_prefix[tmp_offset])) {
                                    ++current_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset ==
                                    TemplatePatterns_T_::LoopPrefixLength) {
                                    UInt end_offset = Engine::Find(
                                        TemplatePatterns_T_::GetLoopSuffix(),
                                        TemplatePatterns_T_::LoopSuffixLength,
                                        content, current_offset, length);

                                    if (end_offset != 0U) {
                                        end_offset = Engine::SkipInnerPatterns(
                                            TemplatePatterns_T_::
                                                GetLoopPrefix(),
                                            TemplatePatterns_T_::
                                                LoopPrefixLength,
                                            TemplatePatterns_T_::
                                                GetLoopSuffix(),
                                            TemplatePatterns_T_::
                                                LoopSuffixLength,
                                            content, current_offset, end_offset,
                                            length);

                                        tags += {TagType_::Loop, offset,
                                                 current_offset, end_offset};

                                        if (tags.IsFull()) {
                                            return;
                                        }

                                        offset = end_offset;
                                        continue;
                                    }

                                    offset = current_offset;
                                }
                            }

                            break;
                        }

                        case TemplatePatterns_T_::If_2ND_Char: { // <if
                            if ((TemplatePatterns_T_::IfPrefixLength +
                                 current_offset) < length) {
                                ++current_offset;
                                tmp_offset = 2U;

                                while ((tmp_offset !=
                                        TemplatePatterns_T_::IfPrefixLength) &&
                                       (content[current_offset] ==
                                        if_prefix[tmp_offset])) {
                                    ++current_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset ==
                                    TemplatePatterns_T_::IfPrefixLength) {
                                    UInt end_offset = Engine::Find(
                                        TemplatePatterns_T_::GetIfSuffix(),
                                        TemplatePatterns_T_::IfSuffixLength,
                                        content, current_offset, length);

                                    if (end_offset != 0U) {
                                        end_offset = Engine::SkipInnerPatterns(
                                            TemplatePatterns_T_::GetIfPrefix(),
                                            TemplatePatterns_T_::IfPrefixLength,
                                            TemplatePatterns_T_::GetIfSuffix(),
                                            TemplatePatterns_T_::IfSuffixLength,
                                            content, current_offset, end_offset,
                                            length);

                                        tags += {TagType_::If, offset,
                                                 current_offset, end_offset};

                                        if (tags.IsFull()) {
                                            return;
                                        }

                                        offset = end_offset;
                                        continue;
                                    }

                                    offset = current_offset;
                                }
                            }
                        }

                        default: {
                        }
                    }
                }

                default: {
                }
            }

            ++offset;
        }
    }

    QENTEM_NOINLINE void render(const Tag_T_ *tag, const Tag_T_ *end,
                                const Char_T_ *content, UInt offset) const {
        UInt last_offset = offset;

        while (tag != end) {
            ss_->Insert((content + last_offset), (tag->Offset - last_offset));

            switch (tag->Type) {
                case TagType_::Variable: {
                    renderVariable(
                        (content + tag->ContentOffset),
                        ((tag->EndOffset - 1U) - tag->ContentOffset));

                    // - 1 is - TemplatePatterns_T_::InLineSuffixLength
                    break;
                }

                case TagType_::Math: {
                    renderMath((content + tag->ContentOffset),
                               ((tag->EndOffset - 1U) - tag->ContentOffset));

                    // - 1 is - TemplatePatterns_T_::InLineSuffixLength
                    break;
                }

                case TagType_::InLineIf: {
                    renderInLineIf(
                        (content + tag->ContentOffset),
                        ((tag->EndOffset - 1U) - tag->ContentOffset));

                    // - 1 is - TemplatePatterns_T_::InLineSuffixLength
                    break;
                }

                case TagType_::Loop: {
                    renderLoop((content + tag->ContentOffset),
                               ((tag->EndOffset -
                                 TemplatePatterns_T_::LoopSuffixLength) -
                                tag->ContentOffset));
                    break;
                }

                case TagType_::If: {
                    renderIf((content + tag->ContentOffset),
                             (tag->EndOffset - tag->ContentOffset));
                }
            }

            last_offset = tag->EndOffset;
            ++tag;
        }
    }

    void renderVariable(const Char_T_ *content, UInt length) const {
        bool            loop_var = (*content == TemplatePatterns_T_::TildeChar);
        const Value_T_ *value    = findValue(content, length);

        if (((value == nullptr) || !(value->InsertString(*ss_))) && !loop_var) {
            length += TemplatePatterns_T_::VariableFulllength;
            ss_->Insert((content - TemplatePatterns_T_::VariablePrefixLength),
                        length);
        }
    }

    QENTEM_NOINLINE void renderMath(const Char_T_ *content, UInt length) const {
        double number;

        if (ALE::Evaluate(number, content, length, this)) {
            Digit<Char_T_>::NumberToStringStream(*ss_, number, 1U, 0U, 3U);
        } else {
            length += TemplatePatterns_T_::MathFulllength;
            ss_->Insert((content - TemplatePatterns_T_::MathPrefixLength),
                        length);
        }
    }

    void parseVariables(const Char_T_ *content, UInt length) const {
        UInt offset = 0U;
        UInt last_offset;

        do {
            last_offset = offset;
            offset      = Engine::Find(TemplatePatterns_T_::GetVariablePrefix(),
                                  TemplatePatterns_T_::VariablePrefixLength,
                                  content, offset, length);

            if (offset == 0U) {
                break;
            }

            if (last_offset < offset) {
                // Add any content that comes before any {var:x}
                ss_->Insert(
                    (content + last_offset),
                    ((offset - TemplatePatterns_T_::VariablePrefixLength) -
                     last_offset));
            }

            const UInt start_offset = offset;
            offset = Engine::FindOne(TemplatePatterns_T_::GetInLineSuffix()[0U],
                                     content, offset, length);

            renderVariable((content + start_offset),
                           ((offset - 1U) - start_offset));

            // -1 is - TemplatePatterns_T_::InLineSuffixLength
        } while (true);

        if (last_offset < length) {
            // Add any content that comes after }
            ss_->Insert((content + last_offset), (length - last_offset));
        }
    }

    /*
     * Gets anything between "..."
     */
    bool getQuoted(UInt &offset, UInt &end_offset, const Char_T_ *content,
                   const UInt length) const noexcept {
        offset = Engine::FindOne(TemplatePatterns_T_::QuoteChar, content,
                                 offset, length);

        if (offset != 0U) {
            const UInt start_offset = Engine::FindOne(
                TemplatePatterns_T_::QuoteChar, content, offset, length);

            if (start_offset != 0U) {
                end_offset = ((start_offset - 1U) - offset);
                return true;
            }
        }

        return false;
    }

    QENTEM_NOINLINE void renderInLineIf(const Char_T_ *content,
                                        const UInt     length) const {
        UInt offset      = 0U;
        UInt last_offset = 0U;
        UInt len         = 0U;
        UInt times       = 3U;
        bool case_value  = false;

        do {
            ++len;
            offset += len; // Move to the next Char_T_.

            if (!getQuoted(offset, len, content, length)) {
                break;
            }

            if (len == 0U) {
                return;
            }

            // = + " + a Char_T_ == 3 + the Char_T_ before it == 4
            UInt tmp_offset   = offset - 4U;
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
        } while (--times != 0U);
    }

    bool parseNumber(UInt &number, const Char_T_ *content,
                     const UInt length) const noexcept {
        if (length > TemplatePatterns_T_::VariableFulllength) {
            UInt offset = 0U;
            offset      = Engine::Find(TemplatePatterns_T_::GetVariablePrefix(),
                                  TemplatePatterns_T_::VariablePrefixLength,
                                  content, offset, length);

            if (offset != 0U) {
                const UInt end_offset =
                    Engine::FindOne(TemplatePatterns_T_::GetInLineSuffix()[0U],
                                    content, offset, length);

                if (end_offset == 0U) {
                    return false;
                }

                const Value_T_ *value =
                    findValue((content + offset), ((end_offset - 1U) - offset));
                // -1 is - TemplatePatterns_T_::InLineSuffixLength

                if (value != nullptr) {
                    double num;

                    if (value->SetNumber(num)) {
                        number = static_cast<UInt>(num);
                        return true;
                    }
                }

                return false;
            }
        }

        return Digit<Char_T_>::StringToNumber(number, content, length);
    }

    QENTEM_NOINLINE void renderLoop(const Char_T_ *content, UInt length) const {
        const UInt start_offset = Engine::FindOne(
            TemplatePatterns_T_::MultiLineSuffix, content, 0U, length);

        if (start_offset == 0U) {
            // The syntax is wrong.
            return;
        }

        const Value_T_ *loop_set          = root_value_;
        const Char_T_ * loop_value        = nullptr;
        UInt            loop_value_length = 0U;
        UInt            loop_index        = 0U;
        UInt            loop_size         = 0U;
        UInt            len               = 0U;
        UInt            offset            = 0U;
        UInt            last_offset       = 0U;

        UInt options    = 4; // set, value, times, index
        bool break_loop = false;

        ///// Stage 1

        do {
            ++len;
            offset += len; // Move to the next Char_T_.

            if (!getQuoted(offset, len, content, start_offset)) {
                break;
            }

            if (len == 0U) {
                // The syntax is wrong.
                return;
            }

            // X="|
            // 3: Goes back to X
            // |X="
            UInt tmp_offset = (offset - 4U);

            do {
                switch (content[tmp_offset]) {
                    case TemplatePatterns_T_::SetChar: {
                        const Char_T_ * content_set = (content + offset);
                        const Value_T_ *value;

                        if ((*content_set ==
                             TemplatePatterns_T_::InLinePrefix) &&
                            (len > TemplatePatterns_T_::VariableFulllength)) {
                            value = findValue(
                                (content_set +
                                 TemplatePatterns_T_::VariablePrefixLength),
                                (len - TemplatePatterns_T_::
                                           VariableFulllength)); // {var:x}
                        } else {
                            value = findValue(content_set, len);
                        }

                        if ((value != nullptr) &&
                            (value->IsArray() || value->IsObject())) {
                            loop_set   = value;
                            break_loop = true;
                            break;
                        }

                        return; // set="..." has invalid value.
                    }

                    case TemplatePatterns_T_::ValueChar: {
                        loop_value        = (content + offset);
                        loop_value_length = len;
                        break_loop        = true;
                        break;
                    }

                    case TemplatePatterns_T_::IndexChar: {
                        if (!parseNumber(loop_index, (content + offset), len)) {
                            return;
                        }

                        break_loop = true;
                        break;
                    }

                    case TemplatePatterns_T_::RepeatChar: {
                        if (!parseNumber(loop_size, (content + offset), len)) {
                            return;
                        }

                        break_loop = true;
                        break;
                    }

                    default: {
                    }
                }

                if (break_loop) {
                    break;
                }

                --tmp_offset;
            } while (tmp_offset > last_offset);

            break_loop  = false;
            last_offset = offset;
        } while (--options != 0U);

        if (loop_size == 0U) {
            loop_size = static_cast<UInt>(loop_set->Size());

            if (loop_size <= loop_index) {
                return;
            }

            loop_size -= loop_index;
        }

        ///// Stage 2

        length -= start_offset;
        content += start_offset;

        StringStream<Char_T_> loop_ss;
        offset      = 0U;
        last_offset = 0U;

        if (loop_value != nullptr) {
            do {
                offset = Engine::Find(loop_value, loop_value_length, content,
                                      last_offset, length);

                if (offset == 0U) {
                    break;
                }

                loop_ss.Insert((content + last_offset),
                               ((offset - loop_value_length) - last_offset));
                loop_ss.Insert(TemplatePatterns_T_::GetVariablePrefix(),
                               TemplatePatterns_T_::VariablePrefixLength);

                UInt lvl = 0U;
                while (lvl <= level_) {
                    loop_ss += TemplatePatterns_T_::TildeChar;
                    ++lvl;
                }

                UInt sub_offset = offset;

                while (content[sub_offset] ==
                       TemplatePatterns_T_::VariableIndexPrefix) {
                    while ((content[sub_offset] !=
                            TemplatePatterns_T_::VariableIndexSuffix) &&
                           (sub_offset < length)) {
                        ++sub_offset;
                    }

                    ++sub_offset;
                }

                if (sub_offset > length) {
                    // Unclosed bracket
                    break;
                }

                if ((content[(sub_offset - 1U)] ==
                     TemplatePatterns_T_::VariableIndexSuffix)) {
                    loop_ss.Insert((content + offset), (sub_offset - offset));
                }

                last_offset = sub_offset;
                loop_ss += TemplatePatterns_T_::GetInLineSuffix()[0U];
            } while (true);
        }

        loop_ss.Insert((content + last_offset), (length - last_offset));

        ///// Stage 3
        generateLoopContent(loop_set, content, length,
                            static_cast<StringStream<Char_T_> &&>(loop_ss),
                            loop_size, loop_index);
    }

    QENTEM_NOINLINE void
    generateLoopContent(const Value_T_ *loop_set, const Char_T_ *content,
                        UInt length, StringStream<Char_T_> &&loop_ss,
                        UInt loop_size, UInt loop_index) const {

        Template_T_   loop_template{ss_, loop_set, this, (level_ + 1U)};
        Tags_T_       loop_tags;
        Array<Tag_T_> all_tags;

        UInt loop_ss_len = static_cast<UInt>(loop_ss.Length());
        UInt loop_offset = 0U;

        ss_->Expect(loop_ss.Length() * loop_size);

        do {
            parse(loop_tags, loop_ss.First(), loop_offset, loop_ss_len);

            if (loop_tags.IsEmpty()) {
                break;
            }

            all_tags.Expect(loop_tags.Size());

            const Tag_T_ *tag = loop_tags.First();
            const Tag_T_ *end = loop_tags.End();

            while (tag != end) {
                all_tags += *tag;
                ++tag;
            }

            if (!(loop_tags.IsFull())) {
                break;
            }

            loop_offset = (loop_tags.First() + loop_tags.Size() - 1)->EndOffset;
            loop_tags.Clear();
        } while (true);

        if (all_tags.Last() != nullptr) {
            // TODO: Split the loop on 4 threads.
            do {
                loop_template.loop_value_ = loop_set->GetValue(loop_index);

                loop_template.render(all_tags.First(), all_tags.End(),
                                     loop_ss.First(), 0U);

                const UInt t_offset = all_tags.Last()->EndOffset;
                // // Add the remaining string.
                ss_->Insert((loop_ss.First() + t_offset),
                            (loop_ss_len - t_offset));

                ++loop_index;
                --loop_size;
            } while (loop_size != 0U);
        } else {
            do {
                ss_->Insert(content, length);
                --loop_size;
            } while (loop_size != 0U);
        }
    }

    QENTEM_NOINLINE void renderIf(const Char_T_ *content, UInt length) const {
        double result      = 0U;
        UInt   offset      = 0U;
        UInt   case_offset = 0U;
        UInt   case_length = 0U;

        // TODO: Optimize

        if (getQuoted(case_offset, case_length, content, length)) {
            offset = case_offset;
            offset += case_length;

            offset = Engine::FindOne(TemplatePatterns_T_::MultiLineSuffix,
                                     content, offset, length);
        }

        do {
            if (((case_length != 0U) &&
                 !(ALE::Evaluate(result, (content + case_offset), case_length,
                                 this))) ||
                (offset == 0U)) {
                // Error: <if> syntax is wrong; no case=""
                return;
            }

            // Find <else..
            case_offset = findNextElse(content, offset, length);

            if ((result > 0U) || (case_offset == 0U)) {
                break;
            }

            // <else has no "i" after it.
            if (content[case_offset] != TemplatePatterns_T_::ElseIfChar) {
                result      = 1U; // Mark <else as true.
                case_length = 0U;
            } else if (!getQuoted(case_offset, case_length, content, length)) {
                // Error: <elseif has no case=""
                return;
            }

            // Find the end of else/elseif
            offset = Engine::Find(TemplatePatterns_T_::GetElseSuffix(),
                                  TemplatePatterns_T_::ElseSuffixLength,
                                  content, (case_offset + case_length), length);
        } while (true);

        if (result > 0U) {
            if (case_offset != 0U) {
                length = case_offset;
            }

            process((content + offset),
                    ((length - TemplatePatterns_T_::IfSuffixLength)) - offset);
        }
    }

    static UInt findNextElse(const Char_T_ *content, UInt offset,
                             UInt length) noexcept {
        UInt else_offset = 0U;

        do {
            else_offset = Engine::Find(TemplatePatterns_T_::GetElsePrefix(),
                                       TemplatePatterns_T_::ElsePrefixLength,
                                       content, offset, length);

            if (else_offset == 0U) {
                // No <else.
                return 0U;
            }

            const UInt next_if = Engine::Find(
                TemplatePatterns_T_::GetIfPrefix(),
                TemplatePatterns_T_::IfPrefixLength, content, offset, length);

            if ((next_if == 0U) || (else_offset < next_if)) {
                // No nesting <ifs or <else is before the child <if.
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

        return else_offset;
    }

    /*
     * Key can be: name/id, name/id[name/id], name/id[name/id][sub-name/id],
     * name/id[name/id][sub-name/id][sub-sub-name/id]... "name": a string that
     * is stored in "keys_". "id" is the index that starts with 0: values_[id]
     */
    const Value_T_ *findLoopValue(const Char_T_ *key,
                                  UInt           length) const noexcept {
        UInt lvl = 0U;
        do {
            ++lvl;
            ++key;
        } while (*key == TemplatePatterns_T_::TildeChar);
        length -= lvl;

        const Value_T_ *   value;
        const Template_T_ *obj = this;

        while (lvl < level_) {
            ++lvl;
            obj = obj->parent_;
        }

        value = obj->loop_value_;

        if (length != 0U) { // ~[]
            UInt offset = 1U;
            UInt tmp    = 1U;
            UInt id;

            do {
                while (key[tmp] != TemplatePatterns_T_::VariableIndexSuffix) {
                    ++tmp;
                }

                if (value->IsObject()) {
                    value = value->GetValue((key + offset), (tmp - offset));
                } else if ((Digit<Char_T_>::StringToNumber(id, (key + offset),
                                                           (tmp - offset)))) {
                    value = value->GetValue(id);
                } else {
                    return nullptr;
                }

                ++tmp;

                if (tmp >= length) {
                    break;
                }

                if (key[tmp] == TemplatePatterns_T_::VariableIndexPrefix) {
                    ++tmp;
                }

                offset = tmp;

                while (key[tmp] != TemplatePatterns_T_::VariableIndexSuffix) {
                    ++tmp;
                }
            } while (true);
        }

        return value;
    }

    QENTEM_NOINLINE const Value_T_ *findValue(const Char_T_ *key,
                                              UInt length) const noexcept {
        const Value_T_ *value = root_value_;

        if (length != 0U) {
            if (*key == TemplatePatterns_T_::TildeChar) {
                return findLoopValue(key, length);
            }

            UInt id;

            if ((key[(length - 1U)] ==
                 TemplatePatterns_T_::VariableIndexSuffix)) {
                // String followed by [...]

                UInt offset = 0U;
                UInt tmp    = 1U;

                while ((key[tmp] != TemplatePatterns_T_::VariableIndexPrefix) &&
                       (tmp < length)) {
                    ++tmp;
                }

                do {
                    if (value == nullptr) {
                        break;
                    }

                    if (value->IsObject()) {
                        value = value->GetValue((key + offset), (tmp - offset));
                    } else if ((Digit<Char_T_>::StringToNumber(
                                   id, (key + offset), (tmp - offset)))) {
                        value = value->GetValue(id);
                    }

                    ++tmp;

                    if (tmp >= length) {
                        break;
                    }

                    if (key[tmp] == TemplatePatterns_T_::VariableIndexPrefix) {
                        ++tmp;
                    }

                    offset = tmp;

                    while (key[tmp] !=
                           TemplatePatterns_T_::VariableIndexSuffix) {
                        ++tmp;
                    }
                } while (true);
            } else if (value->IsObject()) {
                value = value->GetValue(key, length);
            } else if ((Digit<Char_T_>::StringToNumber(id, key, length))) {
                value = value->GetValue(id);
            }
        }

        return value;
    }

    bool ALESetNumber(double &number, const Char_T_ *content,
                      UInt length) const noexcept {
        if (length > TemplatePatterns_T_::VariableFulllength) {
            content += TemplatePatterns_T_::VariablePrefixLength;
            length -= TemplatePatterns_T_::VariableFulllength;
            const Value_T_ *value = findValue(content, length);

            if (value != nullptr) {
                if (value->SetNumber(number)) {
                    return true;
                }

                const Char_T_ *str;
                UInt           len;

                if (value->SetCharAndLength(str, len)) {
                    return ALE::Evaluate(number, str, len, this);
                }

                return value->SetNumber(number);
            }
        }

        return false;
    }

    bool ALEIsEqual(bool &result, const Char_T_ *left, UInt left_length,
                    const Char_T_ *right, UInt right_length) const noexcept {
        const Value_T_ *value_left{nullptr};
        const Value_T_ *value_right{nullptr};
        const Char_T_ * str_left;
        const Char_T_ * str_right;
        UInt            str_length_left;
        UInt            str_length_right;

        bool is_number = false;

        // If the left side is a variable
        if ((*left == TemplatePatterns_T_::InLinePrefix) &&
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

        // If the right side is a variable
        if ((*right == TemplatePatterns_T_::InLinePrefix) &&
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
                if (!(value_left->SetNumber(num_left))) {
                    return false;
                }
            } else if (!(Digit<Char_T_>::StringToNumber(num_left, left,
                                                        left_length))) {
                return false;
            }

            double num_right;

            if (value_right != nullptr) {
                if (!(value_right->SetNumber(num_right))) {
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

    Template_T_(StringStream<Char_T_> *ss, const Value_T_ *root_value,
                const Template_T_ *parent = nullptr, UInt level = 0U) noexcept
        : ss_(ss), root_value_(root_value), parent_(parent), level_(level) {
    }

    StringStream<Char_T_> *ss_;
    const Value_T_ *       root_value_;
    const Value_T_ *       loop_value_{nullptr};
    const Template_T_ *    parent_;

    const UInt level_;
};

} // namespace Qentem

#endif
