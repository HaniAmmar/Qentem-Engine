/*
 * Copyright (c) 2024 Hani Ammar
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

#ifndef QENTEM_TEMPLATE_H
#define QENTEM_TEMPLATE_H

#include "Engine.hpp"
#include "Digit.hpp"
#include "Tags.hpp"
#include "StringView.hpp"

namespace Qentem {

/*
 * Tags:
 *
 *  - {var:s|n}
 *
 *      - var: Variable, s: String, n: Number.
 *
 *
 *  - {raw:s|n}
 *
 *      - raw: Same as {var:...} but without escaping, s: String, n: Number.
 *
 *
 *  - {math:var|e|n}
 *
 *      - var|e|n: Raw variable, Equation or Number.
 *
 *
 *  - {svar:var, var|e|n, var|e|n, ... }
 *
 *      - var: variable only.
 *      - sub variables, var|e|n: Raw variable, Equation or Number.
 *      - max sub variables 10; (0-9).
 *
 *
 *  - {if case="var|e|n" true="rvar|var|s" false="rvar|var|s"}
 *
 *      - case, var|e|n: Raw variable, Equation or Number.
 *      - true and false, rvar: Raw variable, var: Variable, s: String.
 *
 *
 *  <...>
 *  - <loop set="s"? value="s"? group="s"? sort="ascend|descend"?>...</loop>
 *
 *      - s: String, n: Number.
 *      - set: child name in the passed collection: Optional.
 *      - value: the current value in the collection: Optional.
 *          Note: Choose a unique name.
 *      - group: group an array using sub value of an object.
 *      - sort: sort an array or object (ascend or descend).
 *      - ?: means optional.
 *      - ...: means inner content.
 *
 *
 *  - <if case="var|e|n">...<else(if)? ... >?...</if>
 *
 *      - var|e|n: Raw variable, Equation or Number.
 *      - <else if ....> same as if: Optional.
 *      - <else> no case: Optional.
 *      - ?: means optional.
 *      - ...: means inner content.
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
 * Raw Variable Tag:
 *
 * {raw:name}, {raw:name[id]}
 * {raw:id}, {raw:id[id2]}, {raw:id[id2][id3]}
 * {raw:id[name]}, {raw:id[id2][name]}, {raw:id[id2][name1][id3][name2]}
 *
 * {raw:name}, {raw:name[name2]}, {raw:name[name2][name3][...]}
 * {raw:name}, {raw:name[id]}, {raw:name[id][id2][...]}
 * {raw:id}, {raw:id[id2]}, {raw:id[id2][...]}
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
 */

/*
 * Super Variable Tag:
 *
 * {svar:welcome_x_to_y, {var:name}, {var:site}}
 * {svar:welcome_x_to_y_its_z_at_your_city, {var:name}, {var:site}, {math: 10+3}, {raw:city}}
 *
 * welcome_x_to_y: "Welcome {0} to {1}."
 * welcome_x_to_y_its_z_at_your_city: "Welcome {0} to {1}. its {2}:am in your {3}."
 *
 * {svar:hello_x_your_name_was_y_now_x_y_is_better_x, {var:name2}, {var:name1}}
 *
 * hello_x_your_name_was_y_now_x_y_is_better_x: "Hello {1}. Your name was {0}, now it's {1}, {1} is better than {0}."
 *
 * Note: sub variables can be used in any order any time.
 */

/*
 * Inline If Tag:
 *
 * NOTE: You can use single or double quotes.
 *
 * {if case="3 == 3" true="Yes" false="No"}
 * {if case='{var:var_five} == 5' true='5' false='no'}
 * {if case="{var:var1}" true="{var:var_five} is equal to 5" false="no"}
 * {if case='{var:bool1}' true='bool1 is true' false='bool1 is false'}
 * {if case="{var:string1}" true="string1 is not empty" false="null, empty or not a string"}
 * {if case="3 == 3" true="Yes" false="No"}
 * {if case='3 == 3' true='{var:1}' false='{var:2}'}
 * {if case="3 == 3" true="{var:v1}" false="{var:v2}"}
 */

/*
 * Loop Tag:
 *
 * NOTE: You can use single or double quotes.
 *
 * <loop set="..." value="..." group="..." sort="...">...</loop>
 * <loop value="...">...</loop>
 * <loop set='...' value='...'>...</loop>
 * <loop set="..." value="..." group="...">...</loop>
 * <loop set='...' value='...' sort='...'>...</loop>
 * <loop set="..." value="..." group="..." sort="...">...</loop>
 *
 * <loop set="tree_name" value="loop1-value">
 *     {var:loop1-value}
 * </loop>
 *
 * <loop set="items" value="item">
 *     {var:item[subitem1]}, {var:item[subitem2]}, {var:item[subitem3]}
 * </loop>
 *
 * <loop set="tree_name" value="loop1-value">
 *      {var:loop1-value}
 *
 *      <loop set="tree_name2" value="loop2-value">
 *          {var:loop2-value}
 *      </loop>
 * </loop>
 *
 * <loop value="loop1-value" sort="ascend|descend">
 *     {var:loop1-value}
 * </loop>
 *
 * <loop value="loop1-value" group="xyz">
 *     {var:loop1-value}
 * </loop>
 */

/*
 * If tag:
 *
 * NOTE: You can use single or double quotes.
 *
 * <if case="...">...</if>
 * <if case='...'>...</if>
 * <if case="...">...<else />...</if>
 * <if case="...">...<elseif case="..." />...</if>
 * <if case="...">...<else if case="..." >...</if>
 * <if case='...'>...<elseif case='...' >...<else>...</if>
 * <if case='...'>...<else if case='...' >...<else>...</if>
 * <if case='...'>...<elseif case='...' /> <elseif case='...' />...<else />...</if>
 * <if case='...'>...<else if case='...' /> <else if case='...' />...<else>...</if>
 */

template <typename, typename, typename>
struct TemplateCore;

template <typename>
struct TemplatePatterns_T;

template <typename>
struct QOperationSymbol_T;

template <typename Value_T, typename Char_T>
struct LoopItem_T {
    const Value_T     *Value{nullptr};
    StringView<Char_T> Key{};
};

struct Template {
    Template()                            = delete;
    Template(Template &&)                 = delete;
    Template(const Template &)            = delete;
    Template &operator=(Template &&)      = delete;
    Template &operator=(const Template &) = delete;
    ~Template()                           = delete;

    // template <typename Char_T, typename Value_T, typename StringStream_T>
    // inline static void CachedRender(const Char_T *content, const SizeT length, const Value_T &value,
    //                                 StringStream_T &stream, const Char_T *name, const SizeT name_length) {
    //     // See Examples/Template/Template17.cpp
    // }

    template <typename Char_T, typename Value_T, typename StringStream_T>
    inline static StringStream_T &Render(const Char_T *content, SizeT length, const Value_T &value,
                                         StringStream_T &stream, Array<Tags::TagBit> &tags_cache) {
        TemplateCore<Char_T, Value_T, StringStream_T> temp{content, length};

        if (tags_cache.IsEmpty()) {
            temp.Parse(tags_cache);
        }

        temp.Render(tags_cache, value, stream);

        return stream;
    }

    template <typename Char_T, typename Value_T, typename StringStream_T>
    inline static StringStream_T &Render(const Char_T *content, const SizeT length, const Value_T &value,
                                         StringStream_T &stream) {
        Array<Tags::TagBit> tags_cache;
        Render(content, length, value, stream, tags_cache);
        return stream;
    }

    template <typename Char_T, typename Value_T, typename StringStream_T>
    inline static StringStream_T &Render(const Char_T *content, const Value_T &value, StringStream_T &stream) {
        Render(content, StringUtils::Count(content), value, stream);
        return stream;
    }

    template <typename StringStream_T, typename Char_T, typename Value_T>
    inline static StringStream_T Render(const Char_T *content, const SizeT length, const Value_T &value) {
        StringStream_T stream;
        Render(content, length, value, stream);
        return stream;
    }

    template <typename StringStream_T, typename Char_T, typename Value_T>
    inline static StringStream_T Render(const Char_T *content, const Value_T &value) {
        return Render<StringStream_T>(content, StringUtils::Count(content), value);
    }
};

template <typename Char_T, typename Value_T, typename StringStream_T>
struct TemplateCore {
    using LoopItem = LoopItem_T<Value_T, Char_T>;

    TemplateCore() = delete;

    TemplateCore(const Char_T *content, SizeT length) noexcept : content_{content}, length_{length} {
    }

  private:
    using TagType          = Tags::TagType;
    using VariableTag      = Tags::VariableTag;
    using SuperVariableTag = Tags::SuperVariableTag;
    using MathTag          = Tags::MathTag;
    using LoopTag          = Tags::LoopTag;
    using LoopTagOptions   = Tags::LoopTagOptions;
    using InLineIfTag      = Tags::InLineIfTag;
    using IfTagCase        = Tags::IfTagCase;
    using IfTag            = Tags::IfTag;
    using TagBit           = Tags::TagBit;
    using QExpressions     = Array<QExpression>;
    using QOperation       = QExpression::QOperation;
    using ExpressionType   = QExpression::ExpressionType;
    using TagPatterns      = Tags::TagPatterns_T<Char_T>;

  public:
    inline void Parse(Array<TagBit> &tags_cache) {
        parse(tags_cache, SizeT{0}, length_, nullptr);
    }

    void Render(const Array<Tags::TagBit> &tags_cache, const Value_T &value, StringStream_T &stream) {
        Array<LoopItem>     loops_items{};
        const Tags::TagBit *tag = tags_cache.First();
        const Tags::TagBit *end = (tag + tags_cache.Size());

        value_       = &value;
        stream_      = &stream;
        loops_items_ = &loops_items;

        render(tag, end, SizeT{0}, length_);
    }

    inline bool Evaluate(QExpression &number, const QExpressions &exprs, const Value_T &value) noexcept {
        const QExpression *expr = exprs.First();
        value_                  = &value;

        if (expr != nullptr) {
            return evaluate(number, expr, QOperation::NoOp);
        }

        return false;
    }

    inline QExpressions ParseExpressions(SizeT offset, const SizeT end_offset) const {
        return parseExpressions(offset, end_offset, nullptr);
    }

  private:
    void render(const TagBit *tag, const TagBit *end, SizeT offset, SizeT end_offset) const {
        while (tag < end) {
            switch (tag->GetType()) {
                case TagType::Variable: {
                    renderVariable(tag->GeVariableTag(), offset);
                    break;
                }

                case TagType::RawVariable: {
                    renderRawVariable(tag->GeRawVariableTag(), offset);
                    break;
                }

                case TagType::Math: {
                    renderMath(tag->GetMathTag(), offset);
                    break;
                }

                case TagType::SuperVariable: {
                    renderSuperVariable(tag->GetSuperVariableTag(), offset);
                    break;
                }

                case TagType::Loop: {
                    renderLoop(tag->GetLoopTag(), offset);
                    break;
                }

                case TagType::InLineIf: {
                    renderInLineIf(tag->GetInLineIfTag(), offset);
                    break;
                }

                case TagType::If: {
                    renderIf(tag->GetIfTag(), offset);
                    break;
                }

                default: {
                }
            }

            ++tag;
        }

        stream_->Write((content_ + offset), (end_offset - offset));
    }

    void parse(Array<TagBit> &tags_cache, SizeT offset, const SizeT end_offset, const LoopTag *loop_tag) const {
        while (offset < end_offset) {
            if (content_[offset] == TagPatterns::InLinePrefix) {
                SizeT current_offset = offset;
                ++current_offset;

                switch (content_[current_offset]) {
                    case TagPatterns::Var_2ND_Char: {
                        static constexpr const Char_T *var_prefix_p_2 = (TagPatterns::VariablePrefix + SizeT{2});
                        constexpr SizeT var_prefix_length_m_2         = (TagPatterns::VariablePrefixLength - SizeT{2});
                        constexpr SizeT var_prefix_length_m_1         = (TagPatterns::VariablePrefixLength - SizeT{1});
                        ++current_offset;

                        if (StringUtils::IsEqual(var_prefix_p_2, (content_ + current_offset), var_prefix_length_m_2)) {
                            current_offset += var_prefix_length_m_1;
                            current_offset = Engine::FindOne<Char_T>(TagPatterns::InLineSuffix, content_,
                                                                     current_offset, end_offset, length_);

                            if (current_offset != SizeT{0}) {
                                offset += TagPatterns::VariablePrefixLength;
                                parseVariableTag(offset, (current_offset - TagPatterns::InLineSuffixLength),
                                                 (tags_cache.Insert(TagBit{TagType::Variable})).GeVariableTag(),
                                                 loop_tag);
                                offset = current_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TagPatterns::Raw_2ND_Char: {
                        static constexpr const Char_T *r_var_prefix_p_2 = (TagPatterns::RawVariablePrefix + SizeT{2});
                        constexpr SizeT r_var_prefix_length_m_2 = (TagPatterns::RawVariablePrefixLength - SizeT{2});
                        constexpr SizeT r_var_prefix_length_m_1 = (TagPatterns::RawVariablePrefixLength - SizeT{1});
                        ++current_offset;

                        if (StringUtils::IsEqual(r_var_prefix_p_2, (content_ + current_offset),
                                                 r_var_prefix_length_m_2)) {
                            current_offset += r_var_prefix_length_m_1;
                            current_offset = Engine::FindOne<Char_T>(TagPatterns::InLineSuffix, content_,
                                                                     current_offset, end_offset, length_);

                            if (current_offset != SizeT{0}) {
                                offset += TagPatterns::RawVariablePrefixLength;
                                parseVariableTag(offset, (current_offset - TagPatterns::InLineSuffixLength),
                                                 (tags_cache.Insert(TagBit{TagType::RawVariable})).GeRawVariableTag(),
                                                 loop_tag);
                                offset = current_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TagPatterns::Math_2ND_Char: {
                        static constexpr const Char_T *math_prefix_p_2 = (TagPatterns::MathPrefix + SizeT{2});
                        constexpr SizeT math_prefix_length_m_2         = (TagPatterns::MathPrefixLength - SizeT{2});
                        ++current_offset;

                        if (StringUtils::IsEqual(math_prefix_p_2, (content_ + current_offset),
                                                 math_prefix_length_m_2)) {
                            current_offset += math_prefix_length_m_2;
                            current_offset =
                                Engine::SkipInnerPatterns<Char_T>(TagPatterns::InLinePrefix, TagPatterns::InLineSuffix,
                                                                  content_, current_offset, end_offset, length_);

                            if (current_offset != SizeT{0}) {
                                parseMathTag(offset, current_offset,
                                             (tags_cache.Insert(TagBit{TagType::Math})).GetMathTag(), loop_tag);
                                offset = current_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TagPatterns::SuperVariable_2ND_Char: {
                        static constexpr const Char_T *s_var_prefix_p_2 = (TagPatterns::SuperVariablePrefix + SizeT{2});
                        constexpr SizeT s_var_prefix_length_m_2 = (TagPatterns::SuperVariablePrefixLength - SizeT{2});
                        ++current_offset;

                        if (StringUtils::IsEqual(s_var_prefix_p_2, (content_ + current_offset),
                                                 s_var_prefix_length_m_2)) {
                            current_offset += s_var_prefix_length_m_2;
                            current_offset =
                                Engine::SkipInnerPatterns<Char_T>(TagPatterns::InLinePrefix, TagPatterns::InLineSuffix,
                                                                  content_, current_offset, end_offset, length_);

                            if (current_offset != SizeT{0}) {
                                parseSuperVariableTag(
                                    offset, current_offset,
                                    (tags_cache.Insert(TagBit{TagType::SuperVariable})).GetSuperVariableTag(),
                                    loop_tag);
                                offset = current_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TagPatterns::InlineIf_2ND_Char: {
                        static constexpr const Char_T *i_if_prefix_p_2 = (TagPatterns::InLineIfPrefix + SizeT{2});
                        constexpr SizeT i_if_prefix_length_m_2         = (TagPatterns::InLineIfPrefixLength - SizeT{2});
                        ++current_offset;

                        if (StringUtils::IsEqual(i_if_prefix_p_2, (content_ + current_offset),
                                                 i_if_prefix_length_m_2)) {
                            current_offset += i_if_prefix_length_m_2;
                            current_offset =
                                Engine::SkipInnerPatterns<Char_T>(TagPatterns::InLinePrefix, TagPatterns::InLineSuffix,
                                                                  content_, current_offset, end_offset, length_);

                            if (current_offset != SizeT{0}) {
                                parseInLineIfTag(offset, current_offset,
                                                 (tags_cache.Insert(TagBit{TagType::InLineIf})).GetInLineIfTag(),
                                                 loop_tag);
                                offset = current_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    default: {
                    }
                }
            } else if (content_[offset] == TagPatterns::MultiLinePrefix) { // <loop or <if
                SizeT current_offset = offset;
                ++current_offset;

                if (content_[current_offset] == TagPatterns::Loop_2ND_Char) { // <loop
                    static constexpr const Char_T *loop_prefix_p_2        = (TagPatterns::LoopPrefix + SizeT{2});
                    constexpr SizeT                loop_prefix_length_m_2 = (TagPatterns::LoopPrefixLength - SizeT{2});
                    ++current_offset;

                    if (StringUtils::IsEqual(loop_prefix_p_2, (content_ + current_offset), loop_prefix_length_m_2)) {
                        current_offset += loop_prefix_length_m_2;
                        current_offset = Engine::SkipInnerPatterns<Char_T>(
                            TagPatterns::LoopPrefix, TagPatterns::LoopPrefixLength, TagPatterns::LoopSuffix,
                            TagPatterns::LoopSuffixLength, content_, current_offset, end_offset, length_);

                        if (current_offset != SizeT{0}) {
                            parseLoopTag(offset, current_offset,
                                         (tags_cache.Insert(TagBit{TagType::Loop})).GetLoopTag(), loop_tag);
                            offset = current_offset;
                            continue;
                        }
                    }
                } else if (content_[current_offset] == TagPatterns::If_2ND_Char) { // <if
                    static constexpr const Char_T *if_prefix_p_2        = (TagPatterns::IfPrefix + SizeT{2});
                    constexpr SizeT                if_prefix_length_m_2 = (TagPatterns::IfPrefixLength - SizeT{2});
                    ++current_offset;

                    if (StringUtils::IsEqual(if_prefix_p_2, (content_ + current_offset), if_prefix_length_m_2)) {
                        current_offset += if_prefix_length_m_2;
                        current_offset = Engine::SkipInnerPatterns<Char_T>(
                            TagPatterns::IfPrefix, TagPatterns::IfPrefixLength, TagPatterns::IfSuffix,
                            TagPatterns::IfSuffixLength, content_, current_offset, end_offset, length_);

                        if (current_offset != SizeT{0}) {
                            parseIfTag(offset, current_offset, (tags_cache.Insert(TagBit{TagType::If})).GetIfTag(),
                                       loop_tag);
                            offset = current_offset;
                            continue;
                        }
                    }
                }
            }

            ++offset;
        }
    }

    // Part of parse()
    void lightParse(Array<TagBit> &tags_cache, SizeT offset, const SizeT end_offset, const LoopTag *loop_tag) const {
        while (true) {
            offset = Engine::FindOne<Char_T>(TagPatterns::InLinePrefix, content_, offset, end_offset, length_);

            if (offset != 0) {
                switch (content_[offset]) {
                    case TagPatterns::Var_2ND_Char: {
                        static constexpr const Char_T *var_prefix_p_2 = (TagPatterns::VariablePrefix + SizeT{2});
                        constexpr SizeT var_prefix_length_m_2         = (TagPatterns::VariablePrefixLength - SizeT{2});
                        constexpr SizeT var_prefix_length_m_1         = (TagPatterns::VariablePrefixLength - SizeT{1});
                        SizeT           current_offset                = offset;
                        ++current_offset;

                        if (StringUtils::IsEqual(var_prefix_p_2, (content_ + current_offset), var_prefix_length_m_2)) {
                            current_offset += var_prefix_length_m_1;
                            current_offset = Engine::FindOne<Char_T>(TagPatterns::InLineSuffix, content_,
                                                                     current_offset, end_offset, length_);

                            offset -= TagPatterns::InLinePrefixLength;
                            offset += TagPatterns::VariablePrefixLength;
                            parseVariableTag(offset, (current_offset - TagPatterns::InLineSuffixLength),
                                             (tags_cache.Insert(TagBit{TagType::Variable})).GeVariableTag(), loop_tag);
                            offset = current_offset;
                            continue;
                        }

                        break;
                    }

                    case TagPatterns::Raw_2ND_Char: {
                        static constexpr const Char_T *r_var_prefix_p_2 = (TagPatterns::RawVariablePrefix + SizeT{2});
                        constexpr SizeT r_var_prefix_length_m_2 = (TagPatterns::RawVariablePrefixLength - SizeT{2});
                        constexpr SizeT r_var_prefix_length_m_1 = (TagPatterns::RawVariablePrefixLength - SizeT{1});
                        SizeT           current_offset          = offset;
                        ++current_offset;

                        if (StringUtils::IsEqual(r_var_prefix_p_2, (content_ + current_offset),
                                                 r_var_prefix_length_m_2)) {
                            current_offset += r_var_prefix_length_m_1;
                            current_offset = Engine::FindOne<Char_T>(TagPatterns::InLineSuffix, content_,
                                                                     current_offset, end_offset, length_);

                            offset -= TagPatterns::InLinePrefixLength;
                            offset += TagPatterns::RawVariablePrefixLength;
                            parseVariableTag(offset, (current_offset - TagPatterns::InLineSuffixLength),
                                             (tags_cache.Insert(TagBit{TagType::RawVariable})).GeRawVariableTag(),
                                             loop_tag);
                            offset = current_offset;
                            continue;
                        }

                        break;
                    }

                    case TagPatterns::Math_2ND_Char: {
                        static constexpr const Char_T *math_prefix_p_2 = (TagPatterns::MathPrefix + SizeT{2});
                        constexpr SizeT math_prefix_length_m_2         = (TagPatterns::MathPrefixLength - SizeT{2});
                        SizeT           current_offset                 = offset;
                        ++current_offset;

                        if (StringUtils::IsEqual(math_prefix_p_2, (content_ + current_offset),
                                                 math_prefix_length_m_2)) {
                            current_offset += math_prefix_length_m_2;
                            current_offset =
                                Engine::SkipInnerPatterns<Char_T>(TagPatterns::InLinePrefix, TagPatterns::InLineSuffix,
                                                                  content_, current_offset, end_offset, length_);

                            offset -= TagPatterns::InLinePrefixLength;
                            parseMathTag(offset, current_offset,
                                         (tags_cache.Insert(TagBit{TagType::Math})).GetMathTag(), loop_tag);
                            offset = current_offset;
                            continue;
                        }

                        break;
                    }

                    default: {
                    }
                }
            }

            break;
        }
    }

    static void escapeHTMLSpecialChars(StringStream_T &stream, const Char_T *str, SizeT length) {
        if (Config::AutoEscapeHTML) {
            SizeT offset = 0;
            SizeT index  = 0;

            while (index < length) {
                switch (str[index]) {
                    case '&': {
                        stream.Write((str + offset), (index - offset));
                        stream.Write(TagPatterns::HTMLAnd, TagPatterns::HTMLAndLength);
                        ++index;
                        offset = index;
                        break;
                    }

                    case '<': {
                        stream.Write((str + offset), (index - offset));
                        stream.Write(TagPatterns::HTMLLess, TagPatterns::HTMLLessLength);
                        ++index;
                        offset = index;
                        break;
                    }

                    case '>': {
                        stream.Write((str + offset), (index - offset));
                        stream.Write(TagPatterns::HTMLGreater, TagPatterns::HTMLGreaterLength);
                        ++index;
                        offset = index;
                        break;
                    }

                    case '"': {
                        stream.Write((str + offset), (index - offset));
                        stream.Write(TagPatterns::HTMLQuote, TagPatterns::HTMLQuoteLength);
                        ++index;
                        offset = index;
                        break;
                    }

                    case '\'': {
                        stream.Write((str + offset), (index - offset));
                        stream.Write(TagPatterns::HTMLSingleQuote, TagPatterns::HTMLSingleQuoteLength);
                        ++index;
                        offset = index;
                        break;
                    }

                    default: {
                        ++index;
                    }
                }
            }

            stream.Write((str + offset), (length - offset));
        } else {
            stream.Write(str, length);
        }
    }

    void renderVariable(const VariableTag &tag, SizeT &offset) const {
        const SizeT t_offset = (tag.Offset - TagPatterns::VariablePrefixLength);
        const SizeT length   = (tag.Length + TagPatterns::VariableFullLength);

        stream_->Write((content_ + offset), (t_offset - offset));
        offset = t_offset;
        offset += length;

        const Value_T *value = getValue(tag);

        if ((value == nullptr) ||
            !(value->CopyValueTo(*stream_, {Config::TemplatePrecision, QENTEM_TEMPLATE_DOUBLE_FORMAT},
                                 &escapeHTMLSpecialChars))) {
            if (tag.IDLength != SizeT8{0}) {
                const StringView<Char_T> &key = loops_items_->Storage()[tag.Level].Key;

                if (key.Length() != SizeT{0}) {
                    escapeHTMLSpecialChars(*stream_, key.First(), key.Length());
                    return;
                }
            }

            escapeHTMLSpecialChars(*stream_, (content_ + t_offset), length);
        }
    }

    void renderRawVariable(const VariableTag &tag, SizeT &offset) const {
        const SizeT t_offset = (tag.Offset - TagPatterns::RawVariablePrefixLength);
        const SizeT length   = (tag.Length + TagPatterns::RawVariableFullLength);

        stream_->Write((content_ + offset), (t_offset - offset));
        offset = t_offset;
        offset += length;

        const Value_T *value = getValue(tag);

        if ((value == nullptr) ||
            !(value->CopyValueTo(*stream_, {Config::TemplatePrecision, QENTEM_TEMPLATE_DOUBLE_FORMAT}))) {
            stream_->Write((content_ + t_offset), length);
        }
    }

    void renderMath(const MathTag &tag, SizeT &offset) const {
        const QExpression *expr = tag.Expressions.First();
        QExpression        result;

        stream_->Write((content_ + offset), (tag.Offset - offset));
        offset = tag.EndOffset;

        if (tag.Expressions.IsNotEmpty() && evaluate(result, expr, QOperation::NoOp)) {
            switch (result.Type) {
                case ExpressionType::NaturalNumber: {
                    Digit::NumberToString(*stream_, result.Value.Number.Natural);
                    break;
                }

                case ExpressionType::IntegerNumber: {
                    Digit::NumberToString(*stream_, result.Value.Number.Integer);
                    break;
                }

                case ExpressionType::RealNumber: {
                    Digit::NumberToString(*stream_, result.Value.Number.Real,
                                          {Config::TemplatePrecision, QENTEM_TEMPLATE_DOUBLE_FORMAT});
                    break;
                }

                default: {
                }
            }
        } else {
            stream_->Write((content_ + tag.Offset), (tag.EndOffset - tag.Offset));
        }
    }

    void renderSuperVariable(const SuperVariableTag &tag, SizeT &offset) const {
        const Value_T *s_var   = getValue(tag.Variable);
        const Char_T  *content = nullptr;
        SizeT          length  = 0;

        stream_->Write((content_ + offset), (tag.Offset - offset));
        offset = tag.EndOffset;

        if ((s_var != nullptr) && s_var->SetCharAndLength(content, length)) {
            SizeT index      = 0;
            SizeT last_index = 0;

            while (index < length) {
                if (content[index] == TagPatterns::InLinePrefix) {
                    const SizeT start = index;

                    escapeHTMLSpecialChars(*stream_, (content + last_index), (start - last_index));
                    last_index = start;
                    ++index;

                    if (index < length) {
                        const SizeT id = SizeT(content[index] - DigitUtils::DigitChar::Zero);
                        ++index;

                        if ((index < length) && (content[index] == TagPatterns::InLineSuffix)) {
                            ++index;

                            if (id < tag.SubTags.Size()) {
                                const TagBit *sub_tag = (tag.SubTags.First() + id);
                                last_index            = index;

                                switch (sub_tag->GetType()) {
                                    case TagType::Variable: {
                                        const VariableTag &var = sub_tag->GeVariableTag();
                                        SizeT var_offset       = (var.Offset - TagPatterns::VariablePrefixLength);
                                        renderVariable(var, var_offset);
                                        break;
                                    }

                                    case TagType::RawVariable: {
                                        const VariableTag &r_var = sub_tag->GeRawVariableTag();
                                        SizeT r_var_offset = (r_var.Offset - TagPatterns::RawVariablePrefixLength);
                                        renderRawVariable(r_var, r_var_offset);
                                        break;
                                    }

                                    case TagType::Math: {
                                        const MathTag &math        = sub_tag->GetMathTag();
                                        SizeT          math_offset = math.Offset;
                                        renderMath(math, math_offset);
                                        break;
                                    }

                                    default: {
                                    }
                                }

                                continue;
                            }
                        }
                    }
                }

                ++index;
            }

            escapeHTMLSpecialChars(*stream_, (content + last_index), (index - last_index));
        } else {
            stream_->Write((content_ + tag.Offset), (tag.EndOffset - tag.Offset));
        }
    }

    void renderLoop(const LoopTag &tag, SizeT &offset) const {
        Value_T        grouped_set;
        const Value_T *loop_set;

        stream_->Write((content_ + offset), (tag.Offset - offset));
        offset = tag.EndOffset;
        offset += TagPatterns::LoopSuffixLength;

        // Set (Array|Object)
        if (tag.Set.Length != 0) {
            loop_set = getValue(tag.Set);
        } else {
            loop_set = value_;
        }

        if (loop_set != nullptr) {
            // Group
            if (tag.GroupLength != 0) {
                if (!(loop_set->GroupBy(grouped_set, (content_ + tag.Offset + tag.GroupOffset), tag.GroupLength))) {
                    return;
                }

                loop_set = &grouped_set;
            }

            // Sort
            if (tag.Options > SizeT8{1}) {
                if (tag.GroupLength == 0) {
                    grouped_set = *loop_set;
                    loop_set    = &grouped_set;
                }

                grouped_set.Sort((tag.Options & LoopTagOptions::SortAscend) == LoopTagOptions::SortAscend);
            }

            // Stage 4: Render
            const TagBit *s_tag          = tag.SubTags.First();
            const TagBit *s_end          = (s_tag + tag.SubTags.Size());
            const SizeT   content_offset = (tag.Offset + tag.ContentOffset);
            const SizeT   loop_size      = loop_set->Size();
            SizeT         loop_index     = 0;

            if (loops_items_->Size() <= tag.Level) {
                *loops_items_ += LoopItem{};
            }

            if (loop_set->IsObject()) {
                while (loop_index < loop_size) {
                    LoopItem &item = loops_items_->Storage()[tag.Level];
                    loop_set->SetValueAndKey(loop_index, item.Value, item.Key);

                    if (item.Value != nullptr) {
                        render(s_tag, s_end, content_offset, tag.EndOffset);
                    }

                    ++loop_index;
                }
            } else {
                while (loop_index < loop_size) {
                    LoopItem &item = loops_items_->Storage()[tag.Level];
                    item.Value     = loop_set->GetValue(loop_index);

                    if (item.Value != nullptr) {
                        render(s_tag, s_end, content_offset, tag.EndOffset);
                    }

                    ++loop_index;
                }
            }
        }
    }

    void renderInLineIf(const InLineIfTag &tag, SizeT &offset) const {
        QExpression        result;
        const QExpression *expr = tag.Case.First();

        stream_->Write((content_ + offset), (tag.Offset - offset));
        offset = tag.Offset;
        offset += tag.Length;

        if (tag.Case.IsNotEmpty() && evaluate(result, expr, QOperation::NoOp)) {
            const TagBit *s_tag = tag.SubTags.First();
            const TagBit *s_end;
            SizeT         value_offset;
            SizeT         value_end_offset;

            if (result > 0U) {
                s_end            = (s_tag + tag.TrueTagsSize);
                value_offset     = (tag.Offset + tag.TrueOffset);
                value_end_offset = (value_offset + tag.TrueLength);
            } else {
                s_tag += tag.TrueTagsSize;
                s_end            = tag.SubTags.End();
                value_offset     = (tag.Offset + tag.FalseOffset);
                value_end_offset = (value_offset + tag.FalseLength);
            }

            render(s_tag, s_end, value_offset, value_end_offset);
        }
    }

    void renderIf(const IfTag &tag, SizeT &offset) const {
        const IfTagCase *item = tag.Cases.First();
        const IfTagCase *end  = (item + tag.Cases.Size());
        QExpression      result;

        stream_->Write((content_ + offset), (tag.Offset - offset));
        offset = tag.EndOffset;

        if ((item != nullptr) && item->Case.IsNotEmpty()) { // First case should not be empty
            do {
                // <else> without if = (item->Case == nothing)
                const QExpression *expr = item->Case.First();

                if (item->Case.IsEmpty() || (evaluate(result, expr, QOperation::NoOp) && (result > 0U))) {
                    const TagBit *s_tag = item->SubTags.First();
                    const TagBit *s_end = (s_tag + item->SubTags.Size());
                    render(s_tag, s_end, item->Offset, item->EndOffset);
                    break;
                }

                ++item;
            } while (item < end);
        }
    }

    void parseVariableTag(SizeT offset, SizeT end_offset, VariableTag &tag, const LoopTag *loop_tag) const noexcept {
        const Char_T *var = (content_ + offset);
        tag.Offset        = offset;
        tag.Length        = SizeT8((end_offset - offset) & SizeT{0xFF}); // Limit var length to 255 meter per second.

        while (loop_tag != nullptr) {
            if (StringUtils::IsEqual(var, (content_ + (loop_tag->Offset + loop_tag->ValueOffset)),
                                     loop_tag->ValueLength)) {
                tag.IDLength = loop_tag->ValueLength;
                tag.Level    = loop_tag->Level;

                break;
            }

            loop_tag = loop_tag->Parent;
        }
    }

    void parseMathTag(SizeT offset, SizeT end_offset, MathTag &tag, const LoopTag *loop_tag) const {
        tag.Offset    = offset;
        tag.EndOffset = end_offset;

        offset += TagPatterns::MathPrefixLength;
        end_offset -= TagPatterns::InLineSuffixLength;

        tag.Expressions = parseExpressions(offset, end_offset, loop_tag);
    }

    void parseSuperVariableTag(SizeT offset, SizeT end_offset, SuperVariableTag &tag, const LoopTag *loop_tag) const {
        tag.Offset    = offset;
        tag.EndOffset = end_offset;

        offset += TagPatterns::MathPrefixLength;
        end_offset -= TagPatterns::InLineSuffixLength;

        SizeT var_end_offset = offset;

        while ((var_end_offset < end_offset) && (content_[var_end_offset] != TagPatterns::VariablesSeparatorChar)) {
            ++var_end_offset;
        }

        if (var_end_offset < end_offset) {
            parseVariableTag(offset, var_end_offset, tag.Variable, loop_tag);
            ++var_end_offset;

            lightParse(tag.SubTags, var_end_offset, end_offset, loop_tag);
        }
    }

    void parseLoopTag(SizeT offset, const SizeT end_offset, LoopTag &tag, const LoopTag *loop_tag) const {
        enum struct LoopAttributes { None = 0U, Set, Value, Sort, Group };
        LoopAttributes att_type = LoopAttributes::None;

        tag.Offset = offset;
        const SizeT content_offset =
            Engine::FindOne<Char_T>(TagPatterns::MultiLineSuffix, content_, offset, end_offset, length_);

        tag.EndOffset     = (end_offset - TagPatterns::LoopSuffixLength);
        tag.ContentOffset = SizeT16(content_offset - tag.Offset);

        offset += TagPatterns::LoopPrefixLength;

        while (offset < content_offset) {
            while ((offset < content_offset) && (content_[offset] == TagPatterns::SpaceChar)) {
                ++offset;
            }

            if (offset < content_offset) {
                switch (content_[offset]) {
                    case TagPatterns::SetSortChar: {
                        const SizeT tmp_length = (content_offset - offset);

                        if ((tmp_length > TagPatterns::SetLength) &&
                            StringUtils::IsEqual((content_ + offset), TagPatterns::Set, TagPatterns::SetLength)) {
                            offset += TagPatterns::SetLength;
                            att_type = LoopAttributes::Set;
                            break;
                        }

                        if ((tmp_length > TagPatterns::SortLength) &&
                            StringUtils::IsEqual((content_ + offset), TagPatterns::Sort, TagPatterns::SortLength)) {
                            offset += TagPatterns::SortLength;
                            att_type = LoopAttributes::Sort;
                            break;
                        }

                        ++offset;
                        break;
                    }

                    case TagPatterns::ValueChar: {
                        if (((content_offset - offset) > TagPatterns::ValueLength) &&
                            StringUtils::IsEqual((content_ + offset), TagPatterns::Value, TagPatterns::ValueLength)) {
                            offset += TagPatterns::ValueLength;
                            att_type = LoopAttributes::Value;
                            break;
                        }

                        ++offset;
                        break;
                    }

                    case TagPatterns::GroupChar: {
                        if (((content_offset - offset) > TagPatterns::GroupLength) &&
                            StringUtils::IsEqual((content_ + offset), TagPatterns::Group, TagPatterns::GroupLength)) {
                            offset += TagPatterns::GroupLength;
                            att_type = LoopAttributes::Group;
                            break;
                        }

                        ++offset;
                        break;
                    }

                    default: {
                        ++offset;
                        continue;
                    }
                }
            }

            if (offset < content_offset) {
                while ((offset < content_offset) && (content_[offset] != TagPatterns::EqualChar)) {
                    ++offset;
                }

                while ((++offset < content_offset) && (content_[offset] == TagPatterns::SpaceChar)) {
                }

                if (offset < content_offset) {
                    const SizeT att_offset = (offset + SizeT{1});

                    if (content_[offset] == TagPatterns::DoubleQuoteChar) {
                        while ((++offset < content_offset) && (content_[offset] != TagPatterns::DoubleQuoteChar)) {
                        }
                    } else {
                        while ((++offset < content_offset) && (content_[offset] != TagPatterns::SingleQuoteChar)) {
                        }
                    }

                    switch (att_type) {
                        case LoopAttributes::Set: {
                            parseVariableTag(att_offset, offset, tag.Set, loop_tag);
                            break;
                        }

                        case LoopAttributes::Value: {
                            tag.ValueOffset = SizeT8(att_offset - tag.Offset);
                            tag.ValueLength = SizeT8(offset - att_offset);
                            break;
                        }

                        case LoopAttributes::Sort: {
                            tag.Options |= ((content_[att_offset] == 'a') ? LoopTagOptions::SortAscend
                                                                          : LoopTagOptions::SortDescend);
                            break;
                        }

                        case LoopAttributes::Group: {
                            tag.GroupOffset = SizeT8(att_offset - tag.Offset);
                            tag.GroupLength = SizeT8(offset - att_offset);
                            break;
                        }

                        default: {
                        }
                    }

                    ++offset;
                }
            }
        }

        if (loop_tag != nullptr) {
            tag.Parent = loop_tag;
            tag.Level  = SizeT8(loop_tag->Level + SizeT8{1});
        }

        parse(tag.SubTags, content_offset, tag.EndOffset, &tag);
    }

    void parseInLineIfTag(SizeT offset, SizeT end_offset, InLineIfTag &tag, const LoopTag *loop_tag) const {
        tag.Offset = offset;
        tag.Length = SizeT16(end_offset - offset);

        offset += TagPatterns::InLineIfPrefixLength;
        end_offset -= TagPatterns::InLineSuffixLength;

        enum struct InLineIfAttributes { None = 0U, Case, True, False };

        SizeT              true_offset{0};
        SizeT              true_end_offset{0};
        SizeT              false_offset{0};
        SizeT              false_end_offset{0};
        InLineIfAttributes att_type = InLineIfAttributes::None;

        while (offset < end_offset) {
            while ((offset < end_offset) && (content_[offset] == TagPatterns::SpaceChar)) {
                ++offset;
            }

            if (offset < end_offset) {
                switch (content_[offset]) {
                    case TagPatterns::CaseChar: {
                        if (((end_offset - offset) > TagPatterns::CaseLength) &&
                            StringUtils::IsEqual((content_ + offset), TagPatterns::Case, TagPatterns::CaseLength)) {
                            att_type = InLineIfAttributes::Case;
                            offset += TagPatterns::CaseLength;
                            break;
                        }

                        ++offset;
                        break;
                    }

                    case TagPatterns::TrueChar: {
                        if (((end_offset - offset) > TagPatterns::TrueLength) &&
                            StringUtils::IsEqual((content_ + offset), TagPatterns::True, TagPatterns::TrueLength)) {
                            att_type = InLineIfAttributes::True;
                            offset += TagPatterns::TrueLength;
                            break;
                        }

                        ++offset;
                        break;
                    }

                    case TagPatterns::FalseChar: {
                        if (((end_offset - offset) > TagPatterns::FalseLength) &&
                            StringUtils::IsEqual((content_ + offset), TagPatterns::False, TagPatterns::FalseLength)) {
                            att_type = InLineIfAttributes::False;
                            offset += TagPatterns::FalseLength;
                            break;
                        }

                        ++offset;
                        break;
                    }

                    default: {
                        ++offset;
                        continue;
                    }
                }
            }

            if (offset < end_offset) {
                while ((offset < end_offset) && (content_[offset] != TagPatterns::EqualChar)) {
                    ++offset;
                }

                while ((++offset < end_offset) && (content_[offset] == TagPatterns::SpaceChar)) {
                }

                if (offset < end_offset) {
                    const SizeT att_offset = (offset + SizeT{1});

                    if (content_[offset] == TagPatterns::DoubleQuoteChar) {
                        while ((++offset < end_offset) && (content_[offset] != TagPatterns::DoubleQuoteChar)) {
                        }
                    } else {
                        while ((++offset < end_offset) && (content_[offset] != TagPatterns::SingleQuoteChar)) {
                        }
                    }

                    switch (att_type) {
                        case InLineIfAttributes::Case: {
                            tag.Case = parseExpressions(att_offset, offset, loop_tag);
                            break;
                        }

                        case InLineIfAttributes::True: {
                            tag.TrueLength  = SizeT16(offset - att_offset);
                            true_offset     = att_offset;
                            true_end_offset = offset;
                            break;
                        }

                        case InLineIfAttributes::False: {
                            tag.FalseLength  = SizeT16(offset - att_offset);
                            false_offset     = att_offset;
                            false_end_offset = offset;
                            break;
                        }

                        default: {
                        }
                    }

                    ++offset;
                }
            }
        }

        if (tag.Case.IsNotEmpty()) {
            if (tag.TrueLength != 0) {
                tag.TrueOffset = SizeT16(true_offset - tag.Offset);
                lightParse(tag.SubTags, true_offset, true_end_offset, loop_tag);
                tag.TrueTagsSize = SizeT16(tag.SubTags.Size()); // To use one heap for true and false.
            }

            if (tag.FalseLength != 0) {
                tag.FalseOffset = SizeT16(false_offset - tag.Offset);
                lightParse(tag.SubTags, false_offset, false_end_offset, loop_tag);
            }
        }
    }

    void parseIfTag(SizeT offset, SizeT end_offset, IfTag &tag, const LoopTag *loop_tag) const {
        Array<TagBit> sub_tags;

        tag.Offset    = offset;
        tag.EndOffset = end_offset;

        offset += TagPatterns::IfPrefixLength;
        end_offset -= TagPatterns::IfSuffixLength;

        while (offset < end_offset) {
            while ((offset < end_offset) && (content_[offset] == TagPatterns::SpaceChar)) {
                ++offset;
            }

            if (((end_offset - offset) > TagPatterns::CaseLength) &&
                StringUtils::IsEqual((content_ + offset), TagPatterns::Case, TagPatterns::CaseLength)) {
                offset += TagPatterns::CaseLength;

                while ((offset < end_offset) && (content_[offset] != TagPatterns::EqualChar)) {
                    ++offset;
                }

                while ((++offset < end_offset) && (content_[offset] == TagPatterns::SpaceChar)) {
                }

                if (offset < end_offset) {
                    const SizeT att_offset = (offset + SizeT{1});

                    if (content_[offset] == TagPatterns::DoubleQuoteChar) {
                        while ((++offset < end_offset) && (content_[offset] != TagPatterns::DoubleQuoteChar)) {
                        }
                    } else {
                        while ((++offset < end_offset) && (content_[offset] != TagPatterns::SingleQuoteChar)) {
                        }
                    }

                    const SizeT content_offset =
                        Engine::FindOne<Char_T>(TagPatterns::MultiLineSuffix, content_, offset, end_offset, length_);

                    if (content_offset != SizeT{0}) {
                        SizeT else_offset = nextElse(content_offset, end_offset);

                        if (else_offset == SizeT{0}) {
                            parse(sub_tags, content_offset, end_offset, loop_tag);
                            tag.Cases += IfTagCase{parseExpressions(att_offset, offset, loop_tag),
                                                   Memory::Move(sub_tags), content_offset, end_offset};

                            break;
                        }

                        const SizeT else_end_offset = (else_offset - TagPatterns::ElsePrefixLength);
                        parse(sub_tags, content_offset, else_end_offset, loop_tag);
                        tag.Cases += IfTagCase{parseExpressions(att_offset, offset, loop_tag), Memory::Move(sub_tags),
                                               content_offset, else_end_offset};

                        while ((else_offset < end_offset) && (content_[else_offset] == TagPatterns::SpaceChar)) {
                            ++else_offset;
                        }

                        if ((content_[else_offset] != TagPatterns::ElseIfChar)) {
                            else_offset = Engine::FindOne<Char_T>(TagPatterns::MultiLineSuffix, content_, else_offset,
                                                                  end_offset, length_);

                            if (else_offset != SizeT{0}) {
                                parse(sub_tags, else_offset, end_offset, loop_tag);
                                tag.Cases += IfTagCase{QExpressions{}, Memory::Move(sub_tags), else_offset,
                                                       end_offset}; // else without if
                            }
                        }

                        offset = else_offset;
                        offset += TagPatterns::IfLength;
                        continue;
                    }
                }
            }

            break;
        }
    }

    SizeT nextElse(SizeT offset, const SizeT end_offset) const noexcept {
        SizeT else_offset = 0;

        while (true) {
            else_offset = Engine::Find<Char_T>(TagPatterns::ElsePrefix, TagPatterns::ElsePrefixLength, content_, offset,
                                               end_offset, length_);

            if (else_offset == SizeT{0}) {
                // No <else.
                break;
            }

            const SizeT next_if = Engine::Find<Char_T>(TagPatterns::IfPrefix, TagPatterns::IfPrefixLength, content_,
                                                       offset, end_offset, length_);

            if ((next_if == SizeT{0}) || (else_offset < next_if)) {
                // No nesting <ifs or <else before a sub-if.
                break;
            }

            offset = Engine::Find<Char_T>(TagPatterns::IfSuffix, TagPatterns::IfSuffixLength, content_, next_if,
                                          end_offset, length_);

            if (else_offset > offset) {
                // <else is after a sub-if.
                break;
            }
        }

        return else_offset;
    }

    inline const Value_T *getValue(const VariableTag &variable) const noexcept {
        const Value_T *value  = nullptr;
        const Char_T  *id     = (content_ + variable.Offset);
        const SizeT    length = variable.Length;
        SizeT          offset = 0;
        const bool has_index  = ((length != SizeT{0}) && (id[(length - SizeT{1})] == TagPatterns::VariableIndexSuffix));

        if (variable.IDLength == SizeT8{0}) {
            if (!has_index) {
                return value_->GetValue(id, length);
            }

            while ((offset < length) && (id[offset] != TagPatterns::VariableIndexPrefix)) {
                ++offset;
            }

            if (offset != SizeT{0}) {
                // {var:abc[...]}
                // if offset == 0 then its {var:[...]}
                value = value_->GetValue(id, offset);
            }
        } else {
            value = loops_items_->Storage()[variable.Level].Value;

            if (!has_index) {
                return value;
            }

            offset += variable.IDLength;
        }

        ++offset; // The char after [
        SizeT offset2 = offset;

        while (value != nullptr) {
            while ((offset2 < length) && (id[offset2] != TagPatterns::VariableIndexSuffix)) {
                ++offset2;
            };

            value = value->GetValue((id + offset), (offset2 - offset));

            ++offset2; // The char after ]

            if (id[offset2] != TagPatterns::VariableIndexPrefix) {
                break;
            }

            ++offset2; // The char after [
            offset = offset2;
        }

        return value;
    }

    // Evaluate /////////////////////////////////////
    bool evaluate(QExpression &left, const QExpression *&expr, const QOperation previous_oper) const noexcept {
        const QExpression *next_expr;
        QExpression        right;

        if (GetExpressionValue(left, expr, expr->Operation)) {
            while (expr->Operation != QOperation::NoOp) {
                next_expr = (expr + 1U);

                if (expr->Operation >= next_expr->Operation) {
                    if (GetExpressionValue(right, next_expr, expr->Operation) &&
                        evaluateExpression(left, right, expr->Operation)) {
                        expr = next_expr;

                        if (previous_oper < expr->Operation) {
                            continue;
                        }

                        return true;
                    }
                } else if (evaluate(right, next_expr, expr->Operation) &&
                           evaluateExpression(left, right, expr->Operation)) {
                    expr = next_expr;
                    continue;
                }

                return false;
            }

            return (left.Type != ExpressionType::NotANumber);
        }

        return false;
    }

    bool GetExpressionValue(QExpression &result, const QExpression *expr, const QOperation operation) const noexcept {
        switch (expr->Type) {
            case ExpressionType::SubOperation: {
                const QExpression *sub_expr = expr->SubExpressions.First();
                return evaluate(result, sub_expr, QOperation::NoOp);
            }

            case ExpressionType::Variable: {
                if ((operation != QOperation::Equal) && (operation != QOperation::NotEqual)) {
                    const Value_T *val = getValue(expr->Variable);

                    if (val != nullptr) {
                        switch (val->SetNumber(result.Value.Number)) {
                            case QNumberType::Natural: {
                                result.Type = ExpressionType::NaturalNumber;
                                return true;
                            }

                            case QNumberType::Integer: {
                                result.Type = ExpressionType::IntegerNumber;
                                return true;
                            }

                            case QNumberType::Real: {
                                result.Type = ExpressionType::RealNumber;
                                return true;
                            }

                            default: {
                                break;
                            }
                        }
                    }

                    // If (case="...") contains only a variable, (true="...") content will be printed
                    // only if the value is a string and the length of that string is not zero.
                    // This will make it easy to use something like:
                    //  {if case="value[some_string]" true=", value[some_string]"} or
                    // <if case="value[some_string]"><span>value[some_string]-value[another_string]</span></if>
                    if ((operation == QOperation::NoOp) && (expr->Operation == QOperation::NoOp)) {
                        result.Value.Number =
                            SizeT64((val != nullptr) && val->IsString() && (val->Length() != SizeT{0}));
                        result.Type = ExpressionType::NaturalNumber;
                        return true;
                    }

                    return false;
                }

                break;
            }

            default: {
            }
        }

        result.Value = expr->Value;
        result.Type  = expr->Type;
        return true;
    }

    bool evaluateExpression(QExpression &left, QExpression &right, const QOperation oper) const noexcept {
        switch (oper) {
            case QOperation::Exponent: { // ^
                return (left ^= right);
            }

            case QOperation::Remainder: { // %
                left.Value.Number.Integer = (left % right);
                left.Type                 = ExpressionType::IntegerNumber;
                break;
            }

            case QOperation::Multiplication: { // *
                left *= right;
                break;
            }

            case QOperation::Division: { // /
                if (right != 0ULL) {
                    left /= right;
                    break;
                }

                return false;
            }

            case QOperation::Addition: { // +
                left += right;
                break;
            }

            case QOperation::Subtraction: { // -
                left -= right;
                break;
            }

            case QOperation::BitwiseAnd: { // &
                left &= right;
                break;
            }

            case QOperation::BitwiseOr: { // |
                left |= right;
                break;
            }

            case QOperation::Less: { // <
                left.Value.Number.Natural = SizeT64(left < right);
                left.Type                 = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::LessOrEqual: { // <=
                left.Value.Number.Natural = SizeT64(left <= right);
                left.Type                 = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::Greater: { // >
                left.Value.Number.Natural = SizeT64(left > right);
                left.Type                 = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::GreaterOrEqual: { // >=
                left.Value.Number.Natural = SizeT64(left >= right);
                left.Type                 = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::And: { // &&
                left.Value.Number.Natural = SizeT64((left > 0U) && (right > 0U));
                left.Type                 = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::Or: { // ||
                left.Value.Number.Natural = SizeT64((left > 0U) || (right > 0U));
                left.Type                 = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::Equal: { // ==
                if (isEqual(left, right)) {
                    break;
                }

                return false;
            }

            case QOperation::NotEqual: { // !=
                if (isEqual(left, right)) {
                    left.Value.Number.Natural = (left.Value.Number.Natural ^ 1ULL);
                    break;
                }

                return false;
            }

            default: {
                // It will not reach this.
            }
        }

        return true;
    }

    bool isEqual(QExpression &left, QExpression &right) const noexcept {
        const Value_T *left_value        = nullptr;
        const Value_T *right_value       = nullptr;
        const Char_T  *left_content      = nullptr;
        const Char_T  *right_content     = nullptr;
        SizeT          left_length       = 0;
        SizeT          right_length      = 0;
        bool           left_is_a_number  = false;
        bool           right_is_a_number = false;

        switch (left.Type) {
            case ExpressionType::RealNumber:
            case ExpressionType::NaturalNumber:
            case ExpressionType::IntegerNumber: {
                left_is_a_number = true;
                break;
            }

            case ExpressionType::Variable: {
                left_value = getValue(left.Variable);

                if (left_value != nullptr) {
                    if (left_value->GetNumberType() != QNumberType::NotANumber) {
                        left_is_a_number = true;

                        switch (left_value->SetNumber(left.Value.Number)) {
                            case QNumberType::Natural: {
                                left.Type = ExpressionType::NaturalNumber;
                                break;
                            }

                            case QNumberType::Integer: {
                                left.Type = ExpressionType::IntegerNumber;
                                break;
                            }

                            case QNumberType::Real: {
                                left.Type = ExpressionType::RealNumber;
                                break;
                            }

                            default: {
                            }
                        }
                    } else if (!(left_value->SetCharAndLength(left_content, left_length))) {
                        return false;
                    }

                    break;
                }

                return false;
            }

            default: {
                left_content = (content_ + left.Value.Offset);
                left_length  = left.Value.Length;
            }
        }

        switch (right.Type) {
            case ExpressionType::RealNumber:
            case ExpressionType::NaturalNumber:
            case ExpressionType::IntegerNumber: {
                right_is_a_number = true;
                break;
            }

            case ExpressionType::Variable: {
                right_value = getValue(right.Variable);

                if (right_value != nullptr) {
                    if (right_value->GetNumberType() != QNumberType::NotANumber) {
                        right_is_a_number = true;

                        switch (right_value->SetNumber(right.Value.Number)) {
                            case QNumberType::Natural: {
                                right.Type = ExpressionType::NaturalNumber;
                                break;
                            }

                            case QNumberType::Integer: {
                                right.Type = ExpressionType::IntegerNumber;
                                break;
                            }

                            case QNumberType::Real: {
                                right.Type = ExpressionType::RealNumber;
                                break;
                            }

                            default: {
                            }
                        }
                    } else if (!(right_value->SetCharAndLength(right_content, right_length))) {
                        return false;
                    }

                    break;
                }

                return false;
            }

            default: {
                right_content     = (content_ + right.Value.Offset);
                right_length      = right.Value.Length;
                right_is_a_number = false;
            }
        }

        if (left_is_a_number || right_is_a_number) {
            if (!left_is_a_number) {
                if (left_value != nullptr) {
                    switch (left_value->SetNumber(left.Value.Number)) {
                        case QNumberType::Natural: {
                            left.Type = ExpressionType::NaturalNumber;
                            break;
                        }

                        case QNumberType::Integer: {
                            left.Type = ExpressionType::IntegerNumber;
                            break;
                        }

                        case QNumberType::Real: {
                            left.Type = ExpressionType::RealNumber;
                            break;
                        }

                        default: {
                            return false;
                        }
                    }
                } else {
                    return false;
                }
            }

            if (!right_is_a_number) {
                if (right_value != nullptr) {
                    switch (right_value->SetNumber(right.Value.Number)) {
                        case QNumberType::Natural: {
                            right.Type = ExpressionType::NaturalNumber;
                            break;
                        }

                        case QNumberType::Integer: {
                            right.Type = ExpressionType::IntegerNumber;
                            break;
                        }

                        case QNumberType::Real: {
                            right.Type = ExpressionType::RealNumber;
                            break;
                        }

                        default: {
                            return false;
                        }
                    }
                } else {
                    return false;
                }
            }

            left.Value.Number.Natural = SizeT64(left == right);
            left.Type                 = ExpressionType::NaturalNumber;
            return true;
        }

        left.Value.Number.Natural =
            ((left_length == right_length) && StringUtils::IsEqual(left_content, right_content, right_length));
        left.Type = ExpressionType::NaturalNumber;

        return true;
    }

    QExpressions parseExpressions(SizeT offset, const SizeT end_offset, const LoopTag *loop_tag) const {
        QExpressions exprs;
        QOperation   last_oper = QOperation::NoOp;

        while (offset < end_offset) {
            const SizeT      num_offset = offset;
            const QOperation oper       = getOperation(offset, end_offset);

            if ((oper != QOperation::Error) && parseValue(exprs, oper, last_oper, num_offset, offset, loop_tag)) {
                ++offset;

                if (oper < QOperation::Greater) {
                    // ==, &&, ||, !=m <=, >= 2 chars
                    ++offset;
                }

                last_oper = oper;
                continue;
            }

            break;
        }

        if (offset > end_offset) {
            return exprs;
        }

        return QExpressions{};
    }

    bool parseValue(QExpressions &exprs, const QOperation oper, const QOperation last_oper, SizeT offset,
                    SizeT end_offset, const LoopTag *loop_tag) const {
        using QOperationSymbol = QOperationSymbol_T<Char_T>;

        StringUtils::TrimLeft(content_, offset, end_offset);
        StringUtils::TrimRight(content_, offset, end_offset);

        if (offset != end_offset) {
            switch (content_[offset]) {
                case QOperationSymbol::ParenthesesStart: {
                    ++offset;     // Drop (
                    --end_offset; // Drop )

                    if ((last_oper != oper) || (oper != QOperation::NoOp)) {
                        const QExpression &expr =
                            exprs.Insert(QExpression{parseExpressions(offset, end_offset, loop_tag), oper});
                        return (expr.SubExpressions.Size() != SizeT{0});
                    }

                    // The entire expression is inside (...)
                    exprs = parseExpressions(offset, end_offset, loop_tag);
                    return (exprs.Size() != SizeT{0});
                }

                case QOperationSymbol::BracketStart: {
                    if ((end_offset - offset) > TagPatterns::VariableFullLength) {
                        end_offset -= TagPatterns::InLineSuffixLength;

                        if (content_[end_offset] == TagPatterns::InLineSuffix) {
                            offset += TagPatterns::VariablePrefixLength;
                            QExpression &expr = exprs.Insert(QExpression{ExpressionType::Variable, oper});
                            parseVariableTag(offset, end_offset, expr.Variable, loop_tag);
                            return true;
                        }
                    }

                    break;
                }

                default: {
                    const SizeT original_offset = offset;
                    QExpression expr;
                    expr.Operation = oper;

                    const QNumberType n_type = Digit::StringToNumber(expr.Value.Number, content_, offset, end_offset);

                    if ((n_type != QNumberType::NotANumber) && (offset == end_offset)) {
                        switch (n_type) {
                            case QNumberType::Natural: {
                                expr.Type = ExpressionType::NaturalNumber;
                                break;
                            }

                            case QNumberType::Integer: {
                                expr.Type = ExpressionType::IntegerNumber;
                                break;
                            }

                            case QNumberType::Real: {
                                expr.Type = ExpressionType::RealNumber;
                                break;
                            }

                            default: {
                            }
                        }
                    } else {
                        if ((last_oper != QOperation::Equal) && (last_oper != QOperation::NotEqual) &&
                            (oper != QOperation::Equal) && (oper != QOperation::NotEqual)) {
                            return false;
                        }

                        expr.Value.Offset = original_offset;
                        expr.Value.Length = (end_offset - original_offset);
                        expr.Type         = ExpressionType::NotANumber;
                    }

                    exprs += Memory::Move(expr);

                    return true;
                }
            }
        }

        return false;
    }

    QOperation getOperation(SizeT &offset, const SizeT end_offset) const noexcept {
        using QOperationSymbol = QOperationSymbol_T<Char_T>;

        while (offset < end_offset) {
            switch (content_[offset]) {
                case QOperationSymbol::OrExp: { // ||
                    if (content_[(offset + 1)] == QOperationSymbol::OrExp) {
                        return QOperation::Or;
                    }

                    return QOperation::BitwiseOr;
                }

                case QOperationSymbol::AndExp: { // &&
                    if (content_[(offset + 1)] == QOperationSymbol::AndExp) {
                        return QOperation::And;
                    }

                    return QOperation::BitwiseAnd;
                }

                case QOperationSymbol::GreaterExp: { // > or >=
                    if (content_[(offset + 1)] == QOperationSymbol::EqualExp) {
                        return QOperation::GreaterOrEqual;
                    }

                    return QOperation::Greater;
                }

                case QOperationSymbol::LessExp: { // < or <=
                    if (content_[(offset + 1)] == QOperationSymbol::EqualExp) {
                        return QOperation::LessOrEqual;
                    }

                    return QOperation::Less;
                }

                case QOperationSymbol::NotExp: { // !=
                    if (content_[(offset + 1)] == QOperationSymbol::EqualExp) {
                        return QOperation::NotEqual;
                    }

                    return QOperation::Error;
                }

                case QOperationSymbol::EqualExp: { // ==
                    if (content_[(offset + 1)] == QOperationSymbol::EqualExp) {
                        return QOperation::Equal;
                    }

                    return QOperation::Error;
                }

                case QOperationSymbol::SubtractExp: {
                    if (isExpression(offset)) {
                        return QOperation::Subtraction;
                    }

                    break;
                }

                case QOperationSymbol::AddExp: {
                    if (isExpression(offset)) {
                        return QOperation::Addition;
                    }

                    break;
                }

                case QOperationSymbol::DivideExp: {
                    return QOperation::Division;
                }

                case QOperationSymbol::MultipleExp: {
                    return QOperation::Multiplication;
                }

                case QOperationSymbol::RemainderExp: {
                    return QOperation::Remainder;
                }

                case QOperationSymbol::ExponentExp: {
                    return QOperation::Exponent;
                }

                case QOperationSymbol::ParenthesesStart: {
                    ++offset;
                    offset = Engine::SkipInnerPatterns<Char_T>(QOperationSymbol::ParenthesesStart,
                                                               QOperationSymbol::ParenthesesEnd, content_, offset,
                                                               end_offset, length_);

                    if (offset != SizeT{0}) {
                        continue;
                    }

                    return QOperation::Error;
                }

                case QOperationSymbol::BracketStart: {
                    ++offset;
                    offset =
                        Engine::FindOne<Char_T>(QOperationSymbol::BracketEnd, content_, offset, end_offset, length_);

                    if (offset != SizeT{0}) {
                        continue;
                    }

                    offset = end_offset;
                    return QOperation::Error;
                }

                default: {
                }
            }

            ++offset;
        }

        return QOperation::NoOp;
    }

    bool isExpression(SizeT offset) const noexcept {
        using QOperationSymbol = QOperationSymbol_T<Char_T>;

        while (offset != SizeT{0}) {
            --offset;

            switch (content_[offset]) {
                case QOperationSymbol::SpaceChar: {
                    break;
                }

                case QOperationSymbol::ParenthesesEnd:
                case QOperationSymbol::BracketEnd: {
                    // (...) and {} are numbers.
                    return true;
                }

                default: {
                    // A number
                    const Char_T ch = content_[offset];
                    return ((ch < QOperationSymbol::ColonChar) && (ch > QOperationSymbol::SlashChar));
                }
            }
        }

        return false;
    }

    const Value_T   *value_{nullptr};
    StringStream_T  *stream_{nullptr};
    Array<LoopItem> *loops_items_{nullptr};
    const Char_T    *content_;
    const SizeT      length_;
};

} // namespace Qentem

#endif
