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
#include "Array.hpp"
#include "FixedArray.hpp"
#include "StringStream.hpp"

#ifndef QENTEM_TEMPLATE_H_
#define QENTEM_TEMPLATE_H_

#ifndef QENTEM_TEMPLATE_PARSE_ARRAY_SIZE_H_
#define QENTEM_TEMPLATE_PARSE_ARRAY_SIZE_H_ 16
#endif

// TODO: Add autoescape and use * for raw variables

namespace Qentem {

/*
 * Tags:
 *
 *  - {var:s|n}
 *      - var: Variable, s: String, n: Number.
 *
 *  - {math:var|e|n}
 *      - var|e|n: Raw variable, Equation or Number.
 *
 *  - {if case="var|s" true="var|var|s" false="var|var|s"}
 *      - Inline if,  var: Raw variable, var: Variable, s: String.
 *
 *  <...>
 *  - <loop set="s"? value="s"? repeat="var|n"? index="var|n"?>
 *                                                          ...</loop>
 *      - s: String, n: Number, var: Raw ariable,
 *      - set: child name in the passed colloction: Optional.
 *      - value: the current value in the colloction: Optional.
 *          Note: Choose a unique name.
 *      - repeat: if set, it will be used instead of the "set" size.
 *      - index: starting index.
 *
 *  - <if case="var|e|n">...<else(if)? ... >?...</if>
 *      - var|e|n: Raw variable, Equation or Number.
 *      - <else if ....> same as if: Optional.
 *      - <else> no case: Optional.
 */

/*
 * Variable Tag:
 *
 * {var:name}, {var:name[id]}
 * {var:id}, {var:id[id2]}, {var:id[id2][id3]}
 * {var:id[name]}, {var:id[id2][name]}, {var:id[id2][name1][id3][name2]}
 *
 * {var:name}, {var:name[name2]}, {var:name[name2][name3][...]}
 * {var:name}, {var:name[id]}, {var:name[id][id2][...]}
 * {var:id}, {var:id[id2]}, {var:id[id2][...]}
 */

/*
 * Math Tag:
 *
 * {math:1+5} {math:2^3} {math:8^-2} {math:2*4} {math:2+2*4} {math:2+2*4-1}
 * {math:8/2} {math:8%2}
 * {math: 1 && 1 } {math:0 || 1} {math: (1 && 1) || (3&&3) }
 * {math: 3==3} {math: 3!=4} {math: 3>4}  {math: 3>=4}
 * {math: 2<3} {math: 2 <= 3}
 * {math: {var:n1} * {var:n2}}
 * {math: (5+3*(1+2)/2^2 == 7.25) || (3==((8-2)/2))}
 * {math: 0.2 + 0.3}
 * See ALE::Evaluate
 */

/*
 * Inline If Tag:
 *
 * {if case="3 == 3" true="Yes" false="No"}
 * {if case="{var:var_five} == 5" true="5" false="no"}
 * {if case="{var:var1}" true="{var:var_five} is equal to 5" false="no"}
 * {if case="3 == 3" true="Yes" false="No"}
 * {if case="3 == 3" true="{var:1}" false="{var:2}"}
 * {if case="3 == 3" true="{var:v1}" false="{var:v2}"}
 */

/*
 * Loop Tag:
 *
 * <loop set="tree_name" value="loop1-value">
 *     loop1-value
 * </loop>
 *
 * <loop set="items" value="item">
 *     item[subitem1], item[subitem2], item[subitem3]
 * </loop>
 *
 * <loop set="tree_name" value="loop1-value">
 *      loop1-value
 *
 *      <loop set="tree_name2" value="loop2-value">
 *          loop2-value
 *      </loop>
 * </loop>
 *
 * <loop value="loop1-value" repeat="number|var">
 *     loop1-value
 * </loop>
 *
 * <loop value="loop1-value" index="number|var">
 *     loop1-value
 * </loop>
 */

/*
 * If tag:
 *
 * <if case="{case}">...</if>
 * <if case="{case}">...<else />...</if>
 * <if case="{case1}">...<elseif case="{case2}" />...</if>
 * <if case="{case}">...<if case="{case2}" />...</if></if>
 */

template <typename>
struct TemplatePatterns;

template <typename Char_T_, typename Value_T_>
class Template_T_ {
  public:
    Template_T_() = delete;

    static void Process(const Char_T_ *content, SizeT length,
                        const Value_T_ *root_value, StringStream<Char_T_> *ss) {
        Template_T_{ss, root_value}.process(content, length);
    }

  private:
    enum class TagType_ : SizeT {
        Variable = 0, // {var:x}
        Math,         // {math:x}
        InLineIf,     // {if:x}
        Loop,         // <loop set="..." key="..." value="...">
        If,           // <if case="...">
    };

    struct Tag_T_;
    struct LoopData_T_;
    friend class Qentem::ALE;

    using TemplatePatterns_T_ = TemplatePatterns<Char_T_>;
    using Tags_T_ = FixedArray<Tag_T_, QENTEM_TEMPLATE_PARSE_ARRAY_SIZE_H_>;

    QENTEM_NOINLINE void process(const Char_T_ *content, SizeT length) const {
        Tags_T_ tags;
        SizeT   offset = 0;

        while (offset != length) {
            parse(tags, content, offset, length);

            if (tags.IsEmpty()) {
                break;
            }

            render(tags.Storage(), tags.End(), content, offset);
            offset = (tags.First() + tags.Size() - 1)->EndOffset();

            if (!(tags.IsFull())) {
                break;
            }

            tags.Clear();
        }

        // Add the remaining string.
        ss_->Insert((content + offset), (length - offset));
    }

    QENTEM_NOINLINE static void parse(Tags_T_ &tags, const Char_T_ *content,
                                      SizeT offset, SizeT length) {
        SizeT current_offset;
        SizeT tmp_offset;

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
            if (content[offset] == TemplatePatterns_T_::InLinePrefix) {
                current_offset = offset;
                ++current_offset;

                switch (content[current_offset]) {
                    case TemplatePatterns_T_::Var_2ND_Char: {
                        if ((TemplatePatterns_T_::VariablePrefixLength +
                             current_offset) < length) {
                            tmp_offset = 1;

                            do {
                                ++current_offset;
                                ++tmp_offset;
                            } while (
                                (tmp_offset !=
                                 TemplatePatterns_T_::VariablePrefixLength) &&
                                (content[current_offset] ==
                                 variable_prefix[tmp_offset]));

                            if (tmp_offset ==
                                TemplatePatterns_T_::VariablePrefixLength) {
                                const SizeT end_offset = Engine::FindOne(
                                    TemplatePatterns_T_::GetInLineSuffix()[0],
                                    content, current_offset, length);

                                if (end_offset != 0) {
                                    tags += Tag_T_{TagType_::Variable, offset,
                                                   current_offset, end_offset,
                                                   nullptr};

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
                            tmp_offset = 1;

                            do {
                                ++current_offset;
                                ++tmp_offset;
                            } while ((tmp_offset !=
                                      TemplatePatterns_T_::MathPrefixLength) &&
                                     (content[current_offset] ==
                                      math_prefix[tmp_offset]));

                            if (tmp_offset ==
                                TemplatePatterns_T_::MathPrefixLength) {
                                SizeT end_offset = Engine::SkipInnerPatterns(
                                    TemplatePatterns_T_::GetVariablePrefix(),
                                    TemplatePatterns_T_::VariablePrefixLength,
                                    TemplatePatterns_T_::GetInLineSuffix(), 1,
                                    content, current_offset, length);

                                if (end_offset != 0) {
                                    tags += Tag_T_{TagType_::Math, offset,
                                                   current_offset, end_offset,
                                                   nullptr};

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
                            tmp_offset = 1;

                            do {
                                ++current_offset;
                                ++tmp_offset;
                            } while (
                                (tmp_offset !=
                                 TemplatePatterns_T_::InLineIfPrefixLength) &&
                                (content[current_offset] ==
                                 inLine_if_prefix[tmp_offset]));

                            if (tmp_offset ==
                                TemplatePatterns_T_::InLineIfPrefixLength) {
                                SizeT end_offset = Engine::SkipInnerPatterns(
                                    TemplatePatterns_T_::GetVariablePrefix(),
                                    TemplatePatterns_T_::VariablePrefixLength,
                                    TemplatePatterns_T_::GetInLineSuffix(), 1,
                                    content, current_offset, length);

                                if (end_offset != 0) {
                                    tags += Tag_T_{TagType_::InLineIf, offset,
                                                   current_offset, end_offset,
                                                   nullptr};

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
                }
            } else if (content[offset] ==
                       TemplatePatterns_T_::MultiLinePrefix) {
                current_offset = offset;
                ++current_offset;

                if (content[current_offset] ==
                    TemplatePatterns_T_::Loop_2ND_Char) { // <loop
                    if ((TemplatePatterns_T_::LoopPrefixLength +
                         current_offset) < length) {
                        tmp_offset = 1;

                        do {
                            ++current_offset;
                            ++tmp_offset;
                        } while ((tmp_offset <
                                  TemplatePatterns_T_::LoopPrefixLength) &&
                                 (content[current_offset] ==
                                  loop_prefix[tmp_offset]));

                        if (tmp_offset ==
                            TemplatePatterns_T_::LoopPrefixLength) {
                            SizeT end_offset = Engine::SkipInnerPatterns(
                                TemplatePatterns_T_::GetLoopPrefix(),
                                TemplatePatterns_T_::LoopPrefixLength,
                                TemplatePatterns_T_::GetLoopSuffix(),
                                TemplatePatterns_T_::LoopSuffixLength, content,
                                current_offset, length);

                            if (end_offset != 0) {
                                tags +=
                                    Tag_T_{TagType_::Loop, offset,
                                           current_offset, end_offset,
                                           Memory::AllocateInit<LoopData_T_>()};

                                if (tags.IsFull()) {
                                    return;
                                }

                                offset = end_offset;
                                continue;
                            }

                            offset = current_offset;
                        }
                    }
                } else if (content[current_offset] ==
                           TemplatePatterns_T_::If_2ND_Char) { // <if
                    if ((TemplatePatterns_T_::IfPrefixLength + current_offset) <
                        length) {
                        tmp_offset = 1;

                        do {
                            ++current_offset;
                            ++tmp_offset;
                        } while (
                            (tmp_offset !=
                             TemplatePatterns_T_::IfPrefixLength) &&
                            (content[current_offset] == if_prefix[tmp_offset]));

                        if (tmp_offset == TemplatePatterns_T_::IfPrefixLength) {
                            SizeT end_offset = Engine::SkipInnerPatterns(
                                TemplatePatterns_T_::GetIfPrefix(),
                                TemplatePatterns_T_::IfPrefixLength,
                                TemplatePatterns_T_::GetIfSuffix(),
                                TemplatePatterns_T_::IfSuffixLength, content,
                                current_offset, length);

                            if (end_offset != 0) {
                                tags +=
                                    Tag_T_{TagType_::If, offset, current_offset,
                                           end_offset, nullptr};

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
            }

            ++offset;
        }
    }

    QENTEM_NOINLINE void render(Tag_T_ *tag, const Tag_T_ *end,
                                const Char_T_ *content, SizeT offset) const {
        SizeT previous_offset = offset;

        while (tag != end) {
            ss_->Insert((content + previous_offset),
                        (tag->Offset() - previous_offset));

            switch (tag->Type()) {
                case TagType_::Variable: {
                    renderVariable(
                        (content + tag->ContentOffset()),
                        ((tag->EndOffset() - 1) - tag->ContentOffset()));

                    // - 1 is - TemplatePatterns_T_::InLineSuffixLength
                    break;
                }

                case TagType_::Math: {
                    renderMath((content + tag->ContentOffset()),
                               ((tag->EndOffset() - 1) - tag->ContentOffset()));

                    // - 1 is - TemplatePatterns_T_::InLineSuffixLength
                    break;
                }

                case TagType_::InLineIf: {
                    renderInLineIf(
                        (content + tag->ContentOffset()),
                        ((tag->EndOffset() - 1) - tag->ContentOffset()));

                    // - 1 is - TemplatePatterns_T_::InLineSuffixLength
                    break;
                }

                case TagType_::Loop: {
                    if (tag->LoopData()->Content.IsNotEmpty()) { // Cached
                        generateLoopContent((content + tag->ContentOffset()),
                                            tag->LoopData());
                    } else {
                        renderLoop(tag->LoopData(),
                                   (content + tag->ContentOffset()),
                                   ((tag->EndOffset() -
                                     TemplatePatterns_T_::LoopSuffixLength) -
                                    tag->ContentOffset()));
                    }

                    break;
                }

                case TagType_::If: {
                    renderIf((content + tag->ContentOffset()),
                             (tag->EndOffset() - tag->ContentOffset()));
                }
            }

            previous_offset = tag->EndOffset();
            ++tag;
        }
    }

    void renderVariable(const Char_T_ *content, SizeT length) const {
        bool            loop_var = (*content == TemplatePatterns_T_::TildeChar);
        const Value_T_ *value    = findValue(content, length);

        if (((value == nullptr) || !(value->InsertString(*ss_))) && !loop_var) {
            length += TemplatePatterns_T_::VariableFulllength;
            ss_->Insert((content - TemplatePatterns_T_::VariablePrefixLength),
                        length);
        }
    }

    QENTEM_NOINLINE void renderMath(const Char_T_ *content,
                                    SizeT          length) const {
        double number;

        if (ALE::Evaluate(number, content, length, this)) {
            Digit<Char_T_>::NumberToStringStream(*ss_, number, 1, 0, 3);
        } else {
            length += TemplatePatterns_T_::MathFulllength;
            ss_->Insert((content - TemplatePatterns_T_::MathPrefixLength),
                        length);
        }
    }

    void parseVariables(const Char_T_ *content, SizeT length) const {
        SizeT offset = 0;
        SizeT previous_offset;

        do {
            previous_offset = offset;
            offset = Engine::Find(TemplatePatterns_T_::GetVariablePrefix(),
                                  TemplatePatterns_T_::VariablePrefixLength,
                                  content, offset, length);

            if (offset == 0) {
                break;
            }

            if (previous_offset < offset) {
                // Add any content that comes before any {var:x}
                ss_->Insert(
                    (content + previous_offset),
                    ((offset - TemplatePatterns_T_::VariablePrefixLength) -
                     previous_offset));
            }

            const SizeT start_offset = offset;
            offset = Engine::FindOne(TemplatePatterns_T_::GetInLineSuffix()[0],
                                     content, offset, length);

            renderVariable((content + start_offset),
                           ((offset - 1) - start_offset));

            // -1 is - TemplatePatterns_T_::InLineSuffixLength
        } while (true);

        if (previous_offset < length) {
            // Add any content that comes after }
            ss_->Insert((content + previous_offset),
                        (length - previous_offset));
        }
    }

    /*
     * Gets anything between "..."
     */
    bool getQuoted(SizeT &offset, SizeT &end_offset, const Char_T_ *content,
                   const SizeT length) const noexcept {
        offset = Engine::FindOne(TemplatePatterns_T_::QuoteChar, content,
                                 offset, length);

        if (offset != 0) {
            const SizeT start_offset = Engine::FindOne(
                TemplatePatterns_T_::QuoteChar, content, offset, length);

            if (start_offset != 0) {
                end_offset = ((start_offset - 1) - offset);
                return true;
            }
        }

        return false;
    }

    QENTEM_NOINLINE void renderInLineIf(const Char_T_ *content,
                                        const SizeT    length) const {
        SizeT offset          = 0;
        SizeT previous_offset = 0;
        SizeT len             = 0;
        SizeT times           = 3;
        bool  case_value      = false;

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
            SizeT tmp_offset   = offset - 4;
            bool  iif_not_done = true;

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
                }
            } while (iif_not_done && (--tmp_offset > previous_offset));

            previous_offset = offset;
        } while (--times != 0);
    }

    bool parseNumber(SizeT &number, const Char_T_ *content,
                     const SizeT length) const noexcept {
        if (length > TemplatePatterns_T_::VariableFulllength) {
            SizeT offset = 0;
            offset = Engine::Find(TemplatePatterns_T_::GetVariablePrefix(),
                                  TemplatePatterns_T_::VariablePrefixLength,
                                  content, offset, length);

            if (offset != 0) {
                const SizeT end_offset =
                    Engine::FindOne(TemplatePatterns_T_::GetInLineSuffix()[0],
                                    content, offset, length);

                if (end_offset == 0) {
                    return false;
                }

                const Value_T_ *value =
                    findValue((content + offset), ((end_offset - 1) - offset));
                // -1 is - TemplatePatterns_T_::InLineSuffixLength

                return ((value != nullptr) && (value->SetNumber(number)));
            }
        }

        return Digit<Char_T_>::StringToNumber(number, content, length);
    }

    QENTEM_NOINLINE void renderLoop(LoopData_T_ *  loop_data,
                                    const Char_T_ *content,
                                    SizeT          length) const {
        const SizeT start_offset = Engine::FindOne(
            TemplatePatterns_T_::MultiLineSuffix, content, SizeT(0), length);

        if (start_offset == 0) {
            // The syntax is wrong.
            return;
        }

        if (loop_data->Content.IsEmpty()) {
            const Char_T_ *loop_value        = nullptr;
            SizeT          loop_value_length = 0;
            SizeT          len               = 0;
            SizeT          offset            = 0;
            SizeT          previous_offset   = 0;

            SizeT options    = 4; // set, value, times, index
            bool  break_loop = false;

            // Stage 1
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
                SizeT tmp_offset = (offset - 4);

                do {
                    switch (content[tmp_offset]) {
                        case TemplatePatterns_T_::ValueChar: {
                            loop_value        = (content + offset);
                            loop_value_length = len;
                            break_loop        = true;
                            break;
                        }

                        case TemplatePatterns_T_::SetChar: {
                            loop_data->SetOffset = offset;
                            loop_data->SetLength = len;
                            break_loop           = true;
                            break;
                        }

                        case TemplatePatterns_T_::IndexChar: {
                            loop_data->IndexOffset = offset;
                            loop_data->IndexLength = len;
                            break_loop             = true;
                            break;
                        }

                        case TemplatePatterns_T_::RepeatChar: {
                            loop_data->RepeatOffset = offset;
                            loop_data->RepeatLength = len;
                            break_loop              = true;
                            break;
                        }
                    }

                    if (break_loop) {
                        break;
                    }

                    --tmp_offset;
                } while (tmp_offset > previous_offset);

                break_loop      = false;
                previous_offset = offset;
            } while (--options != 0);

            // Stage 2
            offset          = start_offset;
            previous_offset = start_offset;

            if (loop_value != nullptr) {
                do {
                    offset = Engine::Find(loop_value, loop_value_length,
                                          content, previous_offset, length);

                    if (offset == 0) {
                        break;
                    }

                    loop_data->Content.Insert(
                        (content + previous_offset),
                        ((offset - loop_value_length) - previous_offset));
                    loop_data->Content.Insert(
                        TemplatePatterns_T_::GetVariablePrefix(),
                        TemplatePatterns_T_::VariablePrefixLength);

                    SizeT lvl = 0;
                    while (lvl <= level_) {
                        loop_data->Content += TemplatePatterns_T_::TildeChar;
                        ++lvl;
                    }

                    SizeT sub_offset = offset;

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

                    if ((content[(sub_offset - 1)] ==
                         TemplatePatterns_T_::VariableIndexSuffix)) {
                        loop_data->Content.Insert((content + offset),
                                                  (sub_offset - offset));
                    }

                    previous_offset = sub_offset;
                    loop_data->Content +=
                        TemplatePatterns_T_::GetInLineSuffix()[0];
                } while (true);
            }

            loop_data->Content.Insert((content + previous_offset),
                                      (length - previous_offset));
        }

        // Stage 3
        generateLoopContent(content, loop_data);
    }

    QENTEM_NOINLINE void generateLoopContent(const Char_T_ *content,
                                             LoopData_T_ *  loop_data) const {
        // Stage 4: Data
        const Value_T_ *loop_set   = root_value_;
        SizeT           loop_index = 0;
        SizeT           loop_size  = 0;

        // Set
        if (loop_data->SetLength != 0) {
            const Char_T_ *content_set = (content + loop_data->SetOffset);

            if ((*content_set == TemplatePatterns_T_::InLinePrefix) &&
                (loop_data->SetLength >
                 TemplatePatterns_T_::VariableFulllength)) {
                loop_set = findValue(
                    (content_set + TemplatePatterns_T_::VariablePrefixLength),
                    (loop_data->SetLength -
                     TemplatePatterns_T_::VariableFulllength)); // {var:x}
            } else {
                loop_set = findValue(content_set, loop_data->SetLength);
            }

            if ((loop_set == nullptr) ||
                (!(loop_set->IsArray()) && !(loop_set->IsObject()))) {
                return; // set="..." has invalid value.
            }
        }

        // Size
        if ((loop_data->RepeatLength != 0) &&
            (!parseNumber(loop_size, (content + loop_data->RepeatOffset),
                          loop_data->RepeatLength))) {
            return; // Not a number
        }

        // Index
        if ((loop_data->IndexLength != 0) &&
            (!parseNumber(loop_index, (content + loop_data->IndexOffset),
                          loop_data->IndexLength))) {
            return; // Not a number
        }

        if (loop_size == 0) {
            loop_size = loop_set->Size();

            if (loop_size <= loop_index) {
                return;
            }

            loop_size -= loop_index;
        }

        // Stage 5: Parse
        Template_T_ loop_template{ss_, loop_set, this, (level_ + 1)};
        Tags_T_     tags_buffer;
        SizeT       loop_offset = 0;

        ss_->Expect(loop_data->Content.Length() * loop_size);

        if (loop_data->SubTags.IsEmpty()) {
            do {
                parse(tags_buffer, loop_data->Content.First(), loop_offset,
                      loop_data->Content.Length());

                if (tags_buffer.IsEmpty()) {
                    break;
                }

                loop_data->SubTags.Expect(tags_buffer.Size());

                Tag_T_ *      tag = tags_buffer.Storage();
                const Tag_T_ *end = tags_buffer.End();

                while (tag != end) {
                    loop_data->SubTags += static_cast<Tag_T_ &&>(*tag);
                    ++tag;
                }

                if (!(tags_buffer.IsFull())) {
                    break;
                }

                loop_offset =
                    (tags_buffer.First() + tags_buffer.Size() - 1)->EndOffset();
                tags_buffer.Clear();
            } while (true);
        }

        if (loop_data->SubTags.Last() != nullptr) {
            const SizeT    tag_offset = loop_data->SubTags.Last()->EndOffset();
            const Char_T_ *remain_str =
                (loop_data->Content.First() + tag_offset);
            const SizeT remain_len = (loop_data->Content.Length() - tag_offset);

            // TODO: Split the loop on 4 threads.
            do {
                loop_template.loop_value_ = loop_set->GetValue(loop_index);

                loop_template.render(loop_data->SubTags.Storage(),
                                     loop_data->SubTags.End(),
                                     loop_data->Content.First(), 0);

                // Add the remaining string.
                ss_->Insert(remain_str, remain_len);

                ++loop_index;
                --loop_size;
            } while (loop_size != 0);
        } else {
            do {
                ss_->Insert(loop_data->Content.First(),
                            loop_data->Content.Length());
                --loop_size;
            } while (loop_size != 0);
        }
    }

    QENTEM_NOINLINE void renderIf(const Char_T_ *content, SizeT length) const {
        double result      = 0;
        SizeT  offset      = 0;
        SizeT  case_offset = 0;
        SizeT  case_length = 0;

        // TODO: Optimize

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

            process((content + offset),
                    ((length - TemplatePatterns_T_::IfSuffixLength)) - offset);
        }
    }

    static SizeT findNextElse(const Char_T_ *content, SizeT offset,
                              SizeT length) noexcept {
        SizeT else_offset = 0;

        do {
            else_offset = Engine::Find(TemplatePatterns_T_::GetElsePrefix(),
                                       TemplatePatterns_T_::ElsePrefixLength,
                                       content, offset, length);

            if (else_offset == 0) {
                // No <else.
                return 0;
            }

            const SizeT next_if = Engine::Find(
                TemplatePatterns_T_::GetIfPrefix(),
                TemplatePatterns_T_::IfPrefixLength, content, offset, length);

            if ((next_if == 0) || (else_offset < next_if)) {
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
                                  SizeT          length) const noexcept {
        SizeT lvl = 0;
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

        if (length != 0) { // ~[]
            SizeT offset = 1;
            SizeT tmp    = 1;
            SizeT id;

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
                                              SizeT length) const noexcept {
        const Value_T_ *value = root_value_;

        if (length != 0) {
            if (*key == TemplatePatterns_T_::TildeChar) {
                return findLoopValue(key, length);
            }

            SizeT id;

            if ((key[(length - 1)] ==
                 TemplatePatterns_T_::VariableIndexSuffix)) {
                // String followed by [...]

                SizeT offset = 0;
                SizeT tmp    = 1;

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

    bool ALESetNumber(double &number, const Value_T_ *value) const noexcept {
        if (value->SetNumber(number)) {
            return true;
        }

        const Char_T_ *str;
        SizeT          len;

        if (value->SetCharAndLength(str, len)) {
            return ALE::Evaluate(number, str, len, this);
        }

        return false;
    }

    bool ALESetNumber(double &number, const Char_T_ *content,
                      SizeT length) const noexcept {
        const Value_T_ *value = nullptr;

        if (length > TemplatePatterns_T_::VariableFulllength) {
            content += TemplatePatterns_T_::VariablePrefixLength;
            length -= TemplatePatterns_T_::VariableFulllength;
            value = findValue(content, length);
        }

        return ((value != nullptr) && ALESetNumber(number, value));
    }

    bool ALEIsEqual(bool &result, const Char_T_ *content, ALE::Number left,
                    ALE::Number right, bool left_evaluated,
                    bool right_evaluated) const noexcept {
        using ALEOperations_T_ = ALEOperations<Char_T_>;

        const Value_T_ *value_left  = nullptr;
        const Value_T_ *value_right = nullptr;

        const Char_T_ *left_content  = (content + left.Content.Offset);
        const Char_T_ *right_content = (content + right.Content.Offset);
        SizeT          left_length   = left.Content.Length;
        SizeT          right_length  = right.Content.Length;

        bool is_number = (left_evaluated || right_evaluated);

        // If the left side is a variable
        if (!left_evaluated) {
            if ((*left_content == TemplatePatterns_T_::InLinePrefix) &&
                (left_length > TemplatePatterns_T_::VariableFulllength)) {
                value_left = findValue(
                    (left_content + TemplatePatterns_T_::VariablePrefixLength),
                    (left_length -
                     TemplatePatterns_T_::VariableFulllength)); // {var:x}

                if (value_left == nullptr) {
                    return false;
                }

                if (value_left->IsNumber()) {
                    is_number = true;
                }
            } else {
                if (*left_content != ALEOperations_T_::ParenthesStart) {
                    left_evaluated = Digit<Char_T_>::StringToNumber(
                        left.Number, left_content, left_length);
                } else {
                    left_evaluated =
                        ALE::Evaluate(left.Number, (++left_content),
                                      (left_length -= 2), this);
                }

                if (!left_evaluated && is_number) {
                    return false;
                }

                is_number = left_evaluated;
            }
        }

        if (!right_evaluated) {
            // If the right side is a variable
            if ((*right_content == TemplatePatterns_T_::InLinePrefix) &&
                (right_length > TemplatePatterns_T_::VariableFulllength)) {
                value_right = findValue(
                    (right_content + TemplatePatterns_T_::VariablePrefixLength),
                    (right_length -
                     TemplatePatterns_T_::VariableFulllength)); // {var:x}

                if (value_right == nullptr) {
                    return false;
                }

                if (value_right->IsNumber()) {
                    is_number = true;
                }
            } else if (is_number) {
                if (*right_content != ALEOperations_T_::ParenthesStart) {
                    if (!(Digit<Char_T_>::StringToNumber(
                            right.Number, right_content, right_length))) {
                        return false;
                    }
                } else if (!(ALE::Evaluate(right.Number, (++right_content),
                                           (right_length - 2), this))) {
                    return false;
                }
            }
        }

        if (is_number) {
            if (!left_evaluated && ((value_left == nullptr) ||
                                    !(ALESetNumber(left.Number, value_left)))) {
                return false;
            }

            if ((value_right != nullptr) &&
                !(ALESetNumber(right.Number, value_right))) {
                return false;
            }
        } else {
            if ((value_left != nullptr) &&
                !(value_left->SetCharAndLength(left_content, left_length))) {
                return false;
            }

            if ((value_right != nullptr) &&
                !(value_right->SetCharAndLength(right_content, right_length))) {
                return false;
            }
        }

        if (is_number) {
            result = (left.Number == right.Number);
        } else {
            result = ((left_length == right_length) &&
                      StringUtils::IsEqual(left_content, right_content,
                                           right_length));
        }

        return true;
    }

    Template_T_(StringStream<Char_T_> *ss, const Value_T_ *root_value,
                const Template_T_ *parent = nullptr, SizeT level = 0) noexcept
        : ss_(ss), root_value_(root_value), parent_(parent), level_(level) {}

    struct LoopData_T_ {
        StringStream<Char_T_> Content{};
        Array<Tag_T_>         SubTags{};
        SizeT                 SetOffset{0};
        SizeT                 SetLength{0};
        SizeT                 IndexOffset{0};
        SizeT                 IndexLength{0};
        SizeT                 RepeatOffset{0};
        SizeT                 RepeatLength{0};
    };

    struct Tag_T_ {
      public:
        Tag_T_() = default;

        Tag_T_(const Tag_T_ &tag) = delete;
        Tag_T_ &operator=(const Tag_T_ &tag) = delete;

        Tag_T_(TagType_ type, SizeT offset, SizeT content_offset,
               SizeT end_offset, LoopData_T_ *loop_data) noexcept
            : type_(type), offset_(offset), content_offset_(content_offset),
              end_offset_(end_offset), loop_data_(loop_data) {}

        Tag_T_(Tag_T_ &&tag) noexcept
            : type_(tag.type_), offset_(tag.offset_),
              content_offset_(tag.content_offset_),
              end_offset_(tag.end_offset_), loop_data_(tag.loop_data_) {
            tag.loop_data_ = nullptr;
        }

        ~Tag_T_() { Reset(); }

        Tag_T_ &operator=(Tag_T_ &&tag) noexcept {
            if (this != &tag) {
                Reset();

                type_           = tag.type_;
                offset_         = tag.offset_;
                content_offset_ = tag.content_offset_;
                end_offset_     = tag.end_offset_;
                loop_data_      = tag.loop_data_;

                tag.loop_data_ = nullptr;
            }

            return *this;
        }

        void Reset() {
            if (loop_data_ != nullptr) {
                Memory::Destruct(loop_data_);
                Memory::Deallocate(loop_data_);
                loop_data_ = nullptr;
            }
        }

        inline LoopData_T_ *LoopData() const noexcept { return loop_data_; }
        inline TagType_     Type() const noexcept { return type_; }
        inline SizeT        Offset() const noexcept { return offset_; }
        inline SizeT ContentOffset() const noexcept { return content_offset_; }
        inline SizeT EndOffset() const noexcept { return end_offset_; }

      private:
        TagType_     type_{TagType_::Variable};
        SizeT        offset_{0};
        SizeT        content_offset_{0};
        SizeT        end_offset_{0};
        LoopData_T_ *loop_data_{nullptr};
    };

    StringStream<Char_T_> *ss_;
    const Value_T_ *       root_value_;
    const Value_T_ *       loop_value_{nullptr};
    const Template_T_ *    parent_;

    const SizeT level_;
};

template <typename Char_T_>
struct TemplatePatterns {
    /*
     *InLineSuffixLength and InLinePrefixLength should not be more than 1
     */

    // static constexpr SizeT InLineSuffixLength = 1U;
    // static constexpr SizeT InLinePrefixLength = 1U;

    static constexpr SizeT VariablePrefixLength = 5U;
    static constexpr SizeT VariableFulllength =
        (VariablePrefixLength + 1U); // + InLineSuffixLength

    static constexpr SizeT MathPrefixLength = 6U;
    static constexpr SizeT MathFulllength =
        (MathPrefixLength + 1U); // + InLineSuffixLength

    static constexpr SizeT InLineIfPrefixLength = 3U;

    static constexpr SizeT LoopPrefixLength = 5U;
    static constexpr SizeT LoopSuffixLength = 7U;

    static constexpr SizeT IfPrefixLength = 3U;
    static constexpr SizeT IfSuffixLength = 5U;

    static constexpr SizeT ElsePrefixLength = 5U;
    static constexpr SizeT ElseSuffixLength = 2U;

    static constexpr Char_T_ InLinePrefix = '{';
    static const Char_T_ *   GetInLineSuffix() noexcept {
        static constexpr Char_T_ val[] = {'}'};
        return &(val[0]);
    }

    static constexpr Char_T_ MultiLinePrefix = '<';
    static constexpr Char_T_ MultiLineSuffix = '>';

    static constexpr Char_T_ VariableIndexPrefix = '[';
    static constexpr Char_T_ VariableIndexSuffix = ']';

    // {var:
    static constexpr Char_T_ Var_2ND_Char = 'v'; // Second character
    static const Char_T_ *   GetVariablePrefix() noexcept {
        static constexpr Char_T_ val[] = {'{', 'v', 'a', 'r', ':'};
        return &(val[0]);
    }

    // {math:
    static constexpr Char_T_ Math_2ND_Char = 'm'; // Second character
    static const Char_T_ *   GetMathPrefix() noexcept {
        static constexpr Char_T_ val[] = {'{', 'm', 'a', 't', 'h', ':'};
        return &(val[0]);
    }

    // {if:
    static constexpr Char_T_ InlineIf_2ND_Char = 'i'; // Second character
    static const Char_T_ *   GetInLineIfPrefix() noexcept {
        static constexpr Char_T_ val[] = {'{', 'i', 'f', ':'};
        return &(val[0]);
    }

    // <loop
    static constexpr Char_T_ Loop_2ND_Char = 'l'; // Second character
    static const Char_T_ *   GetLoopPrefix() noexcept {
        static constexpr Char_T_ val[] = {'<', 'l', 'o', 'o', 'p'};
        return &(val[0]);
    }

    // </loop>
    static const Char_T_ *GetLoopSuffix() noexcept {
        static constexpr Char_T_ val[] = {'<', '/', 'l', 'o', 'o', 'p', '>'};
        return &(val[0]);
    }

    // <if
    static constexpr Char_T_ If_2ND_Char = 'i'; // Second character
    static const Char_T_ *   GetIfPrefix() noexcept {
        static constexpr Char_T_ val[] = {'<', 'i', 'f'};
        return &(val[0]);
    }

    // </if>
    static const Char_T_ *GetIfSuffix() noexcept {
        static constexpr Char_T_ val[] = {'<', '/', 'i', 'f', '>'};
        return &(val[0]);
    }

    // <else
    static constexpr Char_T_ ElseIfChar = 'i'; // else[i]f
    static const Char_T_ *   GetElsePrefix() noexcept {
        static constexpr Char_T_ val[] = {'<', 'e', 'l', 's', 'e'};
        return &(val[0]);
    }

    // />
    static const Char_T_ *GetElseSuffix() noexcept {
        static constexpr Char_T_ val[] = {'/', '>'};
        return &(val[0]);
    }

    static constexpr Char_T_ QuoteChar  = '"';
    static constexpr Char_T_ CaseChar   = 'a'; // c[a]se
    static constexpr Char_T_ TrueChar   = 'u'; // tr[u]e
    static constexpr Char_T_ FalseChar  = 'l'; // fa[l]se
    static constexpr Char_T_ SetChar    = 's'; // [s]et
    static constexpr Char_T_ ValueChar  = 'u'; // val[u]e
    static constexpr Char_T_ RepeatChar = 'p'; // re[p]eat
    static constexpr Char_T_ IndexChar  = 'd'; // in[d]ex
    static constexpr Char_T_ TildeChar  = '~'; // Tilde
};

struct Template {
    template <typename Char_T_, typename Value_T_, typename Number_T_>
    inline static void Render(const Char_T_ *content, Number_T_ length,
                              const Value_T_ *       root_value,
                              StringStream<Char_T_> *ss) {
        Template_T_<Char_T_, Value_T_>::Process(
            content, static_cast<SizeT>(length), root_value, ss);
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

} // namespace Qentem

#endif
