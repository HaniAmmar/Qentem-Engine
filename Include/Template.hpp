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

#include "Engine.hpp"
#include "Digit.hpp"
#include "Tags.hpp"

#ifndef QENTEM_TEMPLATE_H_
#define QENTEM_TEMPLATE_H_

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
 *  - {if case="var|s" true="var|var|s" false="var|var|s"}
 *
 *      - Inline if,  var: Raw variable, var: Variable, s: String.
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
 * Inline If Tag:
 *
 * NOTE: No spaces before '='
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
 * NOTE: No spaces before '='
 *
 * <loop set="..." value="..." group="..." sort="...">...</loop>
 * <loop value="...">...</loop>
 * <loop set="..." value="...">...</loop>
 * <loop set="..." value="..." group="...">...</loop>
 * <loop set="..." value="..." sort="...">...</loop>
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
 * <if case="{case}">...</if>
 * <if case="{case}">...<else />...</if>
 * <if case="{case1}">...<elseif case="{case2}" />...</if>
 * <if case="{case}">...<if case="{case2}" />...</if></if>
 */

template <typename, typename, typename>
struct TemplateSub;

template <typename>
struct TemplatePatterns_T_;

template <typename>
struct QOperationSymbol_T_;

struct Template {
    Template()                            = delete;
    Template(Template &&)                 = delete;
    Template(const Template &)            = delete;
    Template &operator=(Template &&)      = delete;
    Template &operator=(const Template &) = delete;
    ~Template()                           = delete;

    // template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    // inline static void CachedRender(const Char_T_ *content, const SizeT length, const Value_T_ &value,
    //                                 StringStream_T_ &stream, const Char_T_ *name, const SizeT name_length) {
    //     // This is not a thread safe function, and its here to show how to cache processed tags.
    //     // Can be used in a single threaded process to build on.

    //     // Usage:
    //     // #include "HArray.hpp"
    //     // #include "Value.hpp"
    //     // #include "Template.hpp"
    //     //
    //     // CachedRender("<html>...</html>", 16, value, stringstream, "page1", 5);

    //     using TemplateSubCV = TemplateSub<Char_T_, Value_T_, StringStream_T_>;

    //     const TemplateSubCV temp{content, length, &stream, &value};

    //     static HArray<Array<Tags::TagBit>, Char_T_> cache;

    //     Array<Tags::TagBit> &tags = cache.GetOrAdd(name, name_length);

    //     if (tags.IsEmpty()) {
    //         temp.Parse(tags);
    //     }

    //     const Tags::TagBit *tag = tags.First();
    //     const Tags::TagBit *end = (tag + tags.Size());
    //     temp.Render(tag, end);
    // }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    inline static StringStream_T_ &Render(const Char_T_ *content, SizeT length, const Value_T_ &value,
                                          StringStream_T_ &stream, Array<Tags::TagBit> &tags_cache) {
        using TemplateSubCV = TemplateSub<Char_T_, Value_T_, StringStream_T_>;
        const TemplateSubCV temp{content, length, &stream, &value};

        if (tags_cache.IsEmpty()) {
            temp.Parse(tags_cache);
        }

        const Tags::TagBit *tag = tags_cache.First();
        const Tags::TagBit *end = (tag + tags_cache.Size());
        temp.Render(tag, end);
        return stream;
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    inline static StringStream_T_ &Render(const Char_T_ *content, const SizeT length, const Value_T_ &value,
                                          StringStream_T_ &stream) {
        Array<Tags::TagBit> tags_cache;
        Render(content, length, value, stream, tags_cache);
        return stream;
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    inline static StringStream_T_ &Render(const Char_T_ *content, const Value_T_ &value, StringStream_T_ &stream) {
        Array<Tags::TagBit> tags_cache;
        Render(content, StringUtils::Count(content), value, stream, tags_cache);
        return stream;
    }

    // template <typename Char_T_, typename Value_T_>
    // inline static StringStream<Char_T_> &Render(const Char_T_ *content, const Value_T_ &value,
    //                                             StringStream<Char_T_> &stream) {
    //     Array<Tags::TagBit> tags_cache;
    //     Render(content, StringUtils::Count(content), value, stream, tags_cache);
    //     return stream;
    // }

    template <typename StringStream_T_, typename Char_T_, typename Value_T_>
    inline static StringStream_T_ Render(const Char_T_ *content, const SizeT length, const Value_T_ &value) {
        StringStream_T_ stream;
        Render(content, length, value, stream);
        return stream;
    }

    template <typename StringStream_T_, typename Char_T_, typename Value_T_>
    inline static StringStream_T_ Render(const Char_T_ *content, const Value_T_ &value) {
        return Render<StringStream_T_>(content, StringUtils::Count(content), value);
    }
};

template <typename Char_T_, typename Value_T_, typename StringStream_T_>
struct TemplateSub {
    TemplateSub() = delete;

    TemplateSub(const Char_T_ *content, SizeT length, StringStream_T_ *stream, const Value_T_ *root_value,
                const TemplateSub *parent = nullptr, const unsigned short level = 0) noexcept
        : content_{content}, stream_{stream}, value_{root_value}, parent_{parent}, length_{length}, level_{level} {
    }

  private:
    using TagType         = Tags::TagType;
    using VariableTag     = Tags::VariableTag;
    using MathTag         = Tags::MathTag;
    using LoopTag         = Tags::LoopTag;
    using LoopTagOptions  = Tags::LoopTagOptions;
    using InLineIfTag     = Tags::InLineIfTag;
    using IfTagCase       = Tags::IfTagCase;
    using IfTag           = Array<IfTagCase>;
    using TagBit          = Tags::TagBit;
    using QExpressions    = Array<QExpression>;
    using QOperation      = QExpression::QOperation;
    using ExpressionType  = QExpression::ExpressionType;
    using ExpressionValue = QExpression::ExpressionValue;
    using TagPatterns     = Tags::TagPatterns_T_<Char_T_>;

  public:
    void Render(const TagBit *tag, const TagBit *end) const {
        while (tag < end) {
            switch (tag->GetType()) {
                case TagType::Variable: {
                    renderVariable(tag);
                    break;
                }

                case TagType::RawVariable: {
                    renderRawVariable(tag);
                    break;
                }

                case TagType::Math: {
                    renderMath(tag);
                    break;
                }

                case TagType::Loop: {
                    renderLoop(tag);
                    break;
                }

                case TagType::InLineIf: {
                    renderInLineIf(tag);
                    break;
                }

                case TagType::If: {
                    renderIf(tag);
                    break;
                }

                case TagType::RawText: {
                    renderRawText(tag);
                    break;
                }

                default: {
                }
            }

            ++tag;
        }
    }

    inline void Parse(Array<TagBit> &tags_cache) const {
        parse(tags_cache, 0, length_);
    }

    inline bool Evaluate(QExpression &number, const QExpressions &exprs) const noexcept {
        const QExpression *expr = exprs.First();

        if (expr != nullptr) {
            return evaluate(number, expr, QOperation::NoOp);
        }

        return false;
    }

    inline QExpressions ParseExpressions(SizeT offset, const SizeT end_offset) const {
        return parseExpressions(offset, end_offset);
    }

  private:
    void parse(Array<TagBit> &tags_cache, SizeT offset, const SizeT end_offset) const {
        SizeT last_offset = offset;

        while (offset < end_offset) {
            if (content_[offset] == TagPatterns::InLinePrefix) {
                const SizeT current_offset = (offset + 1);

                switch (content_[current_offset]) {
                    case TagPatterns::Var_2ND_Char: {
                        if (StringUtils::IsEqual((TagPatterns::VariablePrefix + 2), (content_ + current_offset + 1),
                                                 (TagPatterns::VariablePrefixLength - 2))) {
                            const SizeT v_end_offset = Engine::FindOne<Char_T_>(
                                TagPatterns::InLineSuffix, content_,
                                (current_offset + TagPatterns::VariablePrefixLength), end_offset, length_);

                            if (v_end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{last_offset, (offset - last_offset)};
                                }

                                offset += TagPatterns::VariablePrefixLength;
                                const TagBit &tag = tags_cache.InsertGet(TagBit{TagType::Variable});
                                parseVariableTag(offset, (v_end_offset - TagPatterns::InLineSuffixLength),
                                                 tag.GetInfo());
                                last_offset = v_end_offset;
                                offset      = v_end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TagPatterns::Raw_2ND_Char: {
                        if (StringUtils::IsEqual((TagPatterns::RawVariablePrefix + 2), (content_ + current_offset + 1),
                                                 (TagPatterns::RawVariablePrefixLength - 2))) {
                            const SizeT r_end_offset = Engine::FindOne<Char_T_>(
                                TagPatterns::InLineSuffix, content_,
                                (current_offset + TagPatterns::RawVariablePrefixLength), end_offset, length_);

                            if (r_end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{last_offset, (offset - last_offset)};
                                }

                                offset += TagPatterns::RawVariablePrefixLength;
                                const TagBit &tag = tags_cache.InsertGet(TagBit{TagType::RawVariable});
                                parseVariableTag(offset, (r_end_offset - TagPatterns::InLineSuffixLength),
                                                 tag.GetInfo());
                                last_offset = r_end_offset;
                                offset      = r_end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TagPatterns::Math_2ND_Char: {
                        if (StringUtils::IsEqual((TagPatterns::MathPrefix + 2), (content_ + current_offset + 1),
                                                 (TagPatterns::MathPrefixLength - 2))) {
                            const SizeT m_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TagPatterns::InLinePrefix, TagPatterns::InLineSuffix, content_,
                                (current_offset + TagPatterns::MathPrefixLength - 1), end_offset, length_);

                            if (m_end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{last_offset, (offset - last_offset)};
                                }

                                offset += TagPatterns::MathPrefixLength;
                                const TagBit &tag = tags_cache.InsertGet(TagBit{TagType::Math});
                                parseMathTag(offset, (m_end_offset - TagPatterns::InLineSuffixLength), tag.GetInfo());
                                last_offset = m_end_offset;
                                offset      = m_end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TagPatterns::InlineIf_2ND_Char: {
                        if (StringUtils::IsEqual((TagPatterns::InLineIfPrefix + 2), (content_ + current_offset + 1),
                                                 (TagPatterns::InLineIfPrefixLength - 2))) {
                            const SizeT ii_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TagPatterns::InLinePrefix, TagPatterns::InLineSuffix, content_,
                                (current_offset + TagPatterns::InLineIfPrefixLength - 1), end_offset, length_);

                            if (ii_end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{last_offset, (offset - last_offset)};
                                }

                                offset += TagPatterns::InLineIfPrefixLength;
                                const TagBit &tag = tags_cache.InsertGet(TagBit{TagType::InLineIf});
                                parseInLineIfTag(offset, (ii_end_offset - TagPatterns::InLineSuffixLength),
                                                 tag.GetInfo());
                                last_offset = ii_end_offset;
                                offset      = ii_end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    default: {
                    }
                }
            } else if (content_[offset] == TagPatterns::MultiLinePrefix) { // <loop
                const SizeT current_offset = (offset + 1);

                if (content_[current_offset] == TagPatterns::Loop_2ND_Char) {
                    if (StringUtils::IsEqual((TagPatterns::LoopPrefix + 2), (content_ + current_offset + 1),
                                             (TagPatterns::LoopPrefixLength - 2))) {
                        const SizeT l_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                            TagPatterns::LoopPrefix, TagPatterns::LoopPrefixLength, TagPatterns::LoopSuffix,
                            TagPatterns::LoopSuffixLength, content_,
                            (current_offset + TagPatterns::InLineIfPrefixLength - 1), end_offset, length_);

                        if (l_end_offset != 0) {
                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, (offset - last_offset)};
                            }

                            offset += TagPatterns::InLineIfPrefixLength;
                            const TagBit &tag = tags_cache.InsertGet(TagBit{TagType::Loop});
                            parseLoopTag(offset, (l_end_offset - TagPatterns::LoopSuffixLength), tag.GetInfo());
                            last_offset = l_end_offset;
                            offset      = l_end_offset;
                            continue;
                        }
                    }
                } else if (content_[current_offset] == TagPatterns::If_2ND_Char) { // <if
                    if (StringUtils::IsEqual((TagPatterns::IfPrefix + 2), (content_ + current_offset + 1),
                                             (TagPatterns::IfPrefixLength - 2))) {
                        const SizeT i_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                            TagPatterns::IfPrefix, TagPatterns::IfPrefixLength, TagPatterns::IfSuffix,
                            TagPatterns::IfSuffixLength, content_, current_offset, end_offset, length_);

                        if (i_end_offset != 0) {
                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, (offset - last_offset)};
                            }

                            offset += TagPatterns::IfPrefixLength;
                            const TagBit &tag = tags_cache.InsertGet(TagBit{TagType::If});
                            parseIfTag(offset, (i_end_offset - TagPatterns::IfSuffixLength), tag.GetInfo());
                            last_offset = i_end_offset;
                            offset      = i_end_offset;
                            continue;
                        }
                    }
                }
            }

            ++offset;
        }

        if (last_offset != end_offset) {
            tags_cache += TagBit{last_offset, (end_offset - last_offset)};
        }
    }

    // Part of parse()
    void lightParse(Array<TagBit> &tags_cache, SizeT offset, const SizeT end_offset) const {
        SizeT last_offset = offset;

        while (true) {
            offset = Engine::FindOne<Char_T_>(TagPatterns::InLinePrefix, content_, offset, end_offset, length_);

            if (offset != 0) {
                switch (content_[offset]) {
                    case TagPatterns::Var_2ND_Char: {
                        if (StringUtils::IsEqual((TagPatterns::VariablePrefix + 2), (content_ + offset + 1),
                                                 (TagPatterns::VariablePrefixLength - 2))) {
                            const SizeT v_end_offset = Engine::FindOne<Char_T_>(
                                TagPatterns::InLineSuffix, content_, (offset + TagPatterns::VariablePrefixLength),
                                end_offset, length_);
                            offset -= TagPatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, (offset - last_offset)};
                            }

                            offset += TagPatterns::VariablePrefixLength;
                            const TagBit &tag = tags_cache.InsertGet(TagBit{TagType::Variable});
                            parseVariableTag(offset, (v_end_offset - TagPatterns::InLineSuffixLength), tag.GetInfo());
                            last_offset = v_end_offset;
                            offset      = v_end_offset;
                        }

                        continue;
                    }

                    case TagPatterns::Raw_2ND_Char: {
                        if (StringUtils::IsEqual((TagPatterns::RawVariablePrefix + 2), (content_ + offset + 1),
                                                 (TagPatterns::RawVariablePrefixLength - 2))) {
                            const SizeT r_end_offset = Engine::FindOne<Char_T_>(
                                TagPatterns::InLineSuffix, content_, (offset + TagPatterns::RawVariablePrefixLength),
                                end_offset, length_);
                            offset -= TagPatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, (offset - last_offset)};
                            }

                            offset += TagPatterns::RawVariablePrefixLength;
                            const TagBit &tag = tags_cache.InsertGet(TagBit{TagType::RawVariable});
                            parseVariableTag(offset, (r_end_offset - TagPatterns::InLineSuffixLength), tag.GetInfo());
                            last_offset = r_end_offset;
                            offset      = r_end_offset;
                        }

                        continue;
                    }

                    case TagPatterns::Math_2ND_Char: {
                        if (StringUtils::IsEqual((TagPatterns::MathPrefix + 2), (content_ + offset + 1),
                                                 (TagPatterns::MathPrefixLength - 2))) {
                            const SizeT m_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TagPatterns::InLinePrefix, TagPatterns::InLineSuffix, content_,
                                (offset + TagPatterns::MathPrefixLength - 1), end_offset, length_);
                            offset -= TagPatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, (offset - last_offset)};
                            }

                            offset += TagPatterns::MathPrefixLength;
                            const TagBit &tag = tags_cache.InsertGet(TagBit{TagType::Math});
                            parseMathTag(offset, (m_end_offset - TagPatterns::InLineSuffixLength), tag.GetInfo());
                            last_offset = m_end_offset;
                            offset      = m_end_offset;
                        }

                        continue;
                    }

                    default: {
                    }
                }
            }

            break;
        }

        if (last_offset != end_offset) {
            tags_cache += TagBit{last_offset, (end_offset - last_offset)};
        }
    }

    static void escapeHTMLSpecialChars(StringStream_T_ *stream, const Char_T_ *str, SizeT length) {
        SizeT offset = 0;
        SizeT index  = 0;

        while (index < length) {
            switch (str[index]) {
                case '&': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TagPatterns::HTMLAnd, TagPatterns::HTMLAndLength);
                    offset = (++index);
                    break;
                }

                case '<': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TagPatterns::HTMLLess, TagPatterns::HTMLLessLength);
                    offset = (++index);
                    break;
                }

                case '>': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TagPatterns::HTMLGreater, TagPatterns::HTMLGreaterLength);
                    offset = (++index);
                    break;
                }

                case '"': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TagPatterns::HTMLQuote, TagPatterns::HTMLQuoteLength);
                    offset = (++index);
                    break;
                }

                case '\'': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TagPatterns::HTMLSingleQuote, TagPatterns::HTMLSingleQuoteLength);
                    offset = (++index);
                    break;
                }

                default: {
                    ++index;
                }
            }
        }

        stream->Write((str + offset), (length - offset));
    }

    void renderVariable(const TagBit *tag) const {
        const VariableTag &i_tag = *((const VariableTag *)(tag->GetInfo()));
        const Value_T_    *value = getValue(i_tag);

        if (value != nullptr) {
            if constexpr (Config::AutoEscapeHTML) {
                if (value->IsString()) {
                    const Char_T_ *str;
                    SizeT          len;
                    value->SetCharAndLength(str, len);
                    escapeHTMLSpecialChars(stream_, str, len);
                    return;
                }
            }

            if (value->CopyStringValueTo(*stream_, Config::TemplatePrecision)) {
                return;
            }

            if ((i_tag.IsLoopValue == 1) && (loop_key_ != nullptr)) {
                if constexpr (Config::AutoEscapeHTML) {
                    escapeHTMLSpecialChars(stream_, loop_key_, loop_key_length_);
                } else {
                    stream_->Write(loop_key_, loop_key_length_);
                }

                return;
            }
        }

        stream_->Write(((content_ + i_tag.Offset) - TagPatterns::VariablePrefixLength),
                       (i_tag.Length + TagPatterns::VariablePrefixLength + TagPatterns::InLineSuffixLength));
    }

    void renderRawVariable(const TagBit *tag) const {
        const VariableTag &i_tag = *((const VariableTag *)(tag->GetInfo()));
        const Value_T_    *value = getValue(i_tag);

        if ((value != nullptr) && value->CopyStringValueTo(*stream_, Config::TemplatePrecision)) {
            return;
        }

        stream_->Write(((content_ + i_tag.Offset) - TagPatterns::RawVariablePrefixLength),
                       (i_tag.Length + TagPatterns::RawVariableFullLength));
    }

    void renderMath(const TagBit *tag) const {
        const MathTag     &i_tag = *((const MathTag *)(tag->GetInfo()));
        const QExpression *expr  = i_tag.Expressions.First();
        QExpression        result;

        if (i_tag.Expressions.IsNotEmpty() && evaluate(result, expr, QOperation::NoOp)) {
            switch (result.Type) {
                case ExpressionType::NaturalNumber: {
                    Digit::NumberToString(*stream_, result.Number.Natural);
                    break;
                }

                case ExpressionType::IntegerNumber: {
                    Digit::NumberToString(*stream_, result.Number.Integer);
                    break;
                }

                case ExpressionType::RealNumber: {
                    Digit::NumberToString(*stream_, result.Number.Real, Config::TemplatePrecision);
                    break;
                }

                default: {
                }
            }

        } else {
            stream_->Write(
                ((content_ + i_tag.Offset) - TagPatterns::MathPrefixLength),
                ((i_tag.EndOffset - i_tag.Offset) + TagPatterns::MathPrefixLength + TagPatterns::InLineSuffixLength));
        }
    }

    void renderLoop(const TagBit *tag) const {
        const LoopTag  &i_tag = *((const LoopTag *)(tag->GetInfo()));
        Value_T_        grouped_set;
        const Value_T_ *loop_set;

        // Set (Array|Object)
        if (i_tag.SetLength != 0) {
            loop_set = getValue((i_tag.Offset + i_tag.SetOffset), i_tag.SetLength, i_tag.SetLevel,
                                (i_tag.Options & LoopTagOptions::SetIsLoopValue));
        } else {
            loop_set = value_;
        }

        if (loop_set != nullptr) {
            // Group
            if (i_tag.GroupLength != 0) {
                if (!(loop_set->GroupBy(grouped_set, (content_ + i_tag.Offset + i_tag.GroupOffset),
                                        i_tag.GroupLength))) {
                    return;
                }

                loop_set = &grouped_set;
            }

            // Sort
            if (i_tag.Options > 1) {
                if (i_tag.GroupLength == 0) {
                    grouped_set = *loop_set;
                    loop_set    = &grouped_set;
                }

                grouped_set.Sort((i_tag.Options & LoopTagOptions::SortAscend) == LoopTagOptions::SortAscend);
            }

            const unsigned short level = (level_ + 1);

            // Stage 4: Render
            TemplateSub loop_template{content_, length_, stream_, value_, this, level};
            loop_template.loop_value_offset_ = (i_tag.Offset + i_tag.ValueOffset);
            loop_template.loop_value_length_ = i_tag.ValueLength;

            const TagBit *s_tag = i_tag.SubTags.First();
            const TagBit *s_end = (s_tag + i_tag.SubTags.Size());

            const SizeT loop_size  = loop_set->Size();
            SizeT       loop_index = 0;

            if (loop_set->IsObject()) {
                while (loop_index < loop_size) {
                    loop_set->SetValueKeyLength(loop_index, loop_template.loop_value_, loop_template.loop_key_,
                                                loop_template.loop_key_length_);

                    if (loop_template.loop_value_ != nullptr) {
                        loop_template.Render(s_tag, s_end);
                    }

                    ++loop_index;
                }
            } else {
                while (loop_index < loop_size) {
                    loop_template.loop_value_ = loop_set->GetValue(loop_index);

                    if (loop_template.loop_value_ != nullptr) {
                        loop_template.Render(s_tag, s_end);
                    }

                    ++loop_index;
                }
            }
        }
    }

    void renderInLineIf(const TagBit *tag) const {
        const InLineIfTag &i_tag = *((const InLineIfTag *)(tag->GetInfo()));
        const QExpression *expr  = i_tag.Case.First();
        QExpression        result;

        if (i_tag.Case.IsNotEmpty() && evaluate(result, expr, QOperation::NoOp)) {
            const TagBit *s_tag     = i_tag.SubTags.First();
            const TagBit *s_end     = (s_tag + i_tag.SubTags.Size());
            SizeT         true_size = 0;

            if (++expr != i_tag.Case.End()) {
                true_size = SizeT(expr->Number.Natural);
            }

            if (result > 0U) {
                if (true_size != 0) {
                    if (i_tag.SubTags.Size() != true_size) {
                        s_end -= true_size;
                    }

                    Render(s_tag, s_end);
                }
            } else {
                s_tag += true_size;

                Render(s_tag, s_end);
            }
        }
    }

    void renderIf(const TagBit *tag) const {
        const IfTag     &i_tag = *((const IfTag *)(tag->GetInfo()));
        const IfTagCase *item  = i_tag.First();
        const IfTagCase *end   = (item + i_tag.Size());
        QExpression      result;

        if ((item != nullptr) && item->Case.IsNotEmpty()) { // First case should not be empty
            do {
                // <else> without if = (item->Case == nothing)
                const QExpression *expr = item->Case.First();

                if (item->Case.IsEmpty() || (evaluate(result, expr, QOperation::NoOp) && (result > 0U))) {
                    const TagBit *s_tag = item->SubTags.First();
                    const TagBit *s_end = (s_tag + item->SubTags.Size());
                    Render(s_tag, s_end);
                    break;
                }

                ++item;
            } while (item < end);
        }
    }

    void renderRawText(const TagBit *tag) const {
        stream_->Write((content_ + tag->GetOffset()), tag->GetLength());
    }

    void parseVariableTag(SizeT offset, SizeT end_offset, void *tag) const noexcept {
        VariableTag &i_tag = *((VariableTag *)(tag));
        i_tag.Offset       = offset;
        i_tag.Length       = ((end_offset - offset) & 0xFFU);

        if (loop_value_length_ != 0) {
            const TemplateSub *temp = this;

            SizeT l_offset;
            SizeT length;
            SizeT level = level_;

            while (level > 0) {
                l_offset = temp->loop_value_offset_;
                length   = temp->loop_value_length_;

                if (StringUtils::IsEqual((content_ + offset), (content_ + l_offset), length)) {
                    i_tag.IsLoopValue = 1;
                    i_tag.Level       = (level & 0xFFU);
                    break;
                }

                temp = temp->parent_;
                --level;
            }
        }
    }

    void parseMathTag(SizeT offset, SizeT end_offset, void *tag) const {
        MathTag &i_tag    = *((MathTag *)(tag));
        i_tag.Expressions = parseExpressions(offset, end_offset);
        i_tag.Offset      = offset;
        i_tag.EndOffset   = end_offset;
    }

    /*
     * Gets everything between "..."
     */
    SizeT getQuotedValue(SizeT &offset, SizeT end_offset) const noexcept {
        offset = Engine::FindOne<Char_T_>(TagPatterns::QuoteChar, content_, offset, end_offset, length_);

        if (offset != 0) {
            end_offset = Engine::FindOne<Char_T_>(TagPatterns::QuoteChar, content_, offset, end_offset, length_);
        }

        return end_offset;
    }

    void parseLoopTag(SizeT offset, SizeT end_offset, void *tag) const {
        LoopTag    &i_tag = *((LoopTag *)(tag));
        const SizeT loop_content_offset =
            Engine::FindOne<Char_T_>(TagPatterns::MultiLineSuffix, content_, offset, end_offset, length_);

        SizeT offset2     = getQuotedValue(offset, loop_content_offset);
        i_tag.Offset      = offset;
        SizeT last_offset = offset;
        offset -= 6U; // (=) plus (") plus (two chars) = 4 plus (the char before them) = 5

        while ((offset2 != 0) && (offset < loop_content_offset)) {
            switch (content_[offset]) {
                case TagPatterns::SetChar: {
                    const SizeT set_offset = last_offset;
                    const SizeT set_length = ((offset2 - 1) - last_offset);

                    VariableTag set_var{};
                    parseVariableTag(set_offset, (set_offset + set_length), &set_var);
                    i_tag.SetOffset = (unsigned char)(set_var.Offset - i_tag.Offset);
                    i_tag.SetLength = (unsigned char)(set_var.Length);
                    i_tag.SetLevel  = set_var.Level;
                    i_tag.Options |= set_var.IsLoopValue;

                    offset      = offset2;
                    offset2     = getQuotedValue(offset, loop_content_offset);
                    last_offset = offset;
                    offset -= 6U;
                    break;
                }

                case TagPatterns::ValueChar: {
                    i_tag.ValueOffset = (unsigned char)(last_offset - i_tag.Offset);
                    i_tag.ValueLength = (unsigned char)((offset2 - 1U) - last_offset);
                    offset            = offset2;
                    offset2           = getQuotedValue(offset, loop_content_offset);
                    last_offset       = offset;
                    offset -= 6U;
                    break;
                }

                case TagPatterns::SortChar: {
                    i_tag.Options |=
                        ((content_[last_offset] == 'a') ? LoopTagOptions::SortAscend : LoopTagOptions::SortDescend);
                    offset      = offset2;
                    offset2     = getQuotedValue(offset, loop_content_offset);
                    last_offset = offset;
                    offset -= 6U;
                    break;
                }

                case TagPatterns::GroupChar: {
                    i_tag.GroupOffset = (unsigned char)(last_offset - i_tag.Offset);
                    i_tag.GroupLength = (unsigned char)((offset2 - 1) - last_offset);
                    offset            = offset2;
                    offset2           = getQuotedValue(offset, loop_content_offset);
                    last_offset       = offset;
                    offset -= 6U;
                    break;
                }

                default: {
                    ++offset;
                }
            }
        }

        const unsigned short level = (level_ + 1);
        TemplateSub          loop_template{content_, length_, nullptr, nullptr, this, level};
        loop_template.loop_value_offset_ = (i_tag.Offset + i_tag.ValueOffset);
        loop_template.loop_value_length_ = i_tag.ValueLength;
        loop_template.parse(i_tag.SubTags, loop_content_offset, end_offset);
    }

    void parseInLineIfTag(SizeT offset, SizeT end_offset, void *tag) const {
        InLineIfTag &i_tag = *((InLineIfTag *)(tag));

        SizeT true_offset      = 0;
        SizeT true_end_offset  = 0;
        SizeT false_offset     = 0;
        SizeT false_end_offset = 0;

        SizeT offset2     = getQuotedValue(offset, end_offset);
        SizeT last_offset = offset;

        offset -= 5U; // (=) plus (") plus (two chars) = 4 plus (the char before them) = 5

        while ((offset2 != 0) && (offset < end_offset)) {
            switch (content_[offset]) {
                case TagPatterns::CaseChar: {
                    i_tag.Case  = parseExpressions(last_offset, (offset2 - 1)); // 1 = (") at the end
                    offset      = offset2;
                    offset2     = getQuotedValue(offset, end_offset);
                    last_offset = offset;
                    offset -= 5U;
                    break;
                }

                case TagPatterns::TrueChar: {
                    true_offset     = last_offset;
                    true_end_offset = (offset2 - 1);
                    offset          = offset2;
                    offset2         = getQuotedValue(offset, end_offset);
                    last_offset     = offset;
                    offset -= 5U;
                    break;
                }

                case TagPatterns::FalseChar: {
                    false_offset     = last_offset;
                    false_end_offset = (offset2 - 1);
                    offset           = offset2;
                    offset2          = getQuotedValue(offset, end_offset);
                    last_offset      = offset;
                    offset -= 5U;
                    break;
                }

                default: {
                    ++offset;
                }
            }
        }

        if (i_tag.Case.IsNotEmpty()) {
            if (true_offset != true_end_offset) {
                lightParse(i_tag.SubTags, true_offset, true_end_offset);
                QExpression &expr   = i_tag.Case.InsertGet(QExpression{});
                expr.Number.Natural = i_tag.SubTags.Size(); // To use only one heap for true and false.
            }

            if (false_offset != false_end_offset) {
                lightParse(i_tag.SubTags, false_offset, false_end_offset);
            }
        }
    }

    void parseIfTag(SizeT offset, const SizeT end_offset, void *tag) const {
        IfTag        &i_tag = *((IfTag *)(tag));
        Array<TagBit> sub_tags;
        SizeT         offset2 = getQuotedValue(offset, end_offset);
        SizeT         case_end_offset;
        SizeT         else_offset;
        SizeT         content_offset;

        while ((offset2 != 0) && (offset < end_offset)) {
            case_end_offset = (offset2 - 1);
            content_offset =
                Engine::FindOne<Char_T_>(TagPatterns::MultiLineSuffix, content_, offset2, end_offset, length_);

            if (content_offset != 0) {
                else_offset = nextElse(content_offset, end_offset);

                if (else_offset == 0) {
                    parse(sub_tags, content_offset, end_offset);
                    i_tag += IfTagCase{Memory::Move(sub_tags), parseExpressions(offset, case_end_offset)};

                    break;
                }

                parse(sub_tags, content_offset, (else_offset - TagPatterns::ElsePrefixLength));
                i_tag += IfTagCase{Memory::Move(sub_tags), parseExpressions(offset, case_end_offset)};

                if ((content_[else_offset] != TagPatterns::ElseIfChar)) {
                    else_offset = Engine::FindOne<Char_T_>(TagPatterns::MultiLineSuffix, content_, else_offset,
                                                           end_offset, length_);

                    if (else_offset != 0) {
                        parse(sub_tags, else_offset, end_offset);
                        i_tag += IfTagCase{Memory::Move(sub_tags), QExpressions{}}; // else without if
                    }

                    break;
                }

                offset  = else_offset;
                offset2 = getQuotedValue(offset, end_offset);
                continue;
            }

            break;
        }
    }

    SizeT nextElse(SizeT offset, const SizeT end_offset) const noexcept {
        SizeT else_offset = 0;

        while (true) {
            else_offset = Engine::Find<Char_T_>(TagPatterns::ElsePrefix, TagPatterns::ElsePrefixLength, content_,
                                                offset, end_offset, length_);

            if (else_offset == 0) {
                // No <else.
                break;
            }

            const SizeT next_if = Engine::Find<Char_T_>(TagPatterns::IfPrefix, TagPatterns::IfPrefixLength, content_,
                                                        offset, end_offset, length_);

            if ((next_if == 0) || (else_offset < next_if)) {
                // No nesting <ifs or <else before a sub-if.
                break;
            }

            offset = Engine::Find<Char_T_>(TagPatterns::IfSuffix, TagPatterns::IfSuffixLength, content_, next_if,
                                           end_offset, length_);

            if (else_offset > offset) {
                // <else is after a sub-if.
                break;
            }
        }

        return else_offset;
    }

    const Value_T_ *getValue(const VariableTag &variable) const noexcept {
        return getValue(variable.Offset, variable.Length, variable.Level, variable.IsLoopValue);
    }

    const Value_T_ *getValue(SizeT v_offset, unsigned short v_length, unsigned char v_level,
                             unsigned char v_is_loop_value) const noexcept {
        const Value_T_ *value     = nullptr;
        const Char_T_  *id        = (content_ + v_offset);
        const SizeT     length    = v_length;
        SizeT           offset    = 0;
        const bool      has_index = (id[(length - 1)] == TagPatterns::VariableIndexSuffix);

        if (v_is_loop_value != 1) {
            if (!has_index) {
                return value_->GetValue(id, length);
            }

            while ((offset < length) && (id[offset] != TagPatterns::VariableIndexPrefix)) {
                ++offset;
            }

            if (offset != 0) {
                // {var:abc[...]}
                // if offset == 0 then its {var:[...]}
                value = value_->GetValue(id, offset);
            }
        } else {
            const TemplateSub *that = this;
            {
                const unsigned int level    = (unsigned int)(level_);
                unsigned int       iv_level = (unsigned int)(v_level);
                value                       = loop_value_;

                while (iv_level < level) {
                    that = that->parent_;
                    ++iv_level;
                }
            }

            value = that->loop_value_;

            if (!has_index) {
                return value;
            }

            offset += loop_value_length_;
        }

        SizeT offset2;

        while (value != nullptr) {
            ++offset; // The char after [

            offset2 = offset;

            while ((offset2 < length) && (id[offset2] != TagPatterns::VariableIndexSuffix)) {
                ++offset2;
            };

            value = value->GetValue((id + offset), (offset2 - offset));

            ++offset2; // The char after ]

            if (id[offset2] != TagPatterns::VariableIndexPrefix) {
                break;
            }

            offset = offset2;
        }

        return value;
    }

    // Evaluate /////////////////////////////////////
    bool evaluate(QExpression &left, const QExpression *&expr, const QOperation previous_oper) const noexcept {
        const QExpression *next_expr;
        QExpression        right;
        const bool not_equal = ((expr->Operation != QOperation::Equal) && (expr->Operation != QOperation::NotEqual));

        if (GetExpressionValue(left, expr, not_equal)) {
            while (expr->Operation != QOperation::NoOp) {
                next_expr = (expr + 1U);

                if (expr->Operation >= next_expr->Operation) {
                    if (GetExpressionValue(right, next_expr, not_equal) &&
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

    bool GetExpressionValue(QExpression &number, const QExpression *expr, bool not_equal) const noexcept {
        switch (expr->Type) {
            case ExpressionType::SubOperation: {
                const QExpression *sub_expr = expr->SubExpressions.First();
                return evaluate(number, sub_expr, QOperation::NoOp);
            }

            case ExpressionType::Variable: {
                if (not_equal) {
                    const Value_T_ *val = getValue(expr->Variable);

                    if (val != nullptr) {
                        QNumber val_number;

                        switch (val->SetNumber(val_number)) {
                            case 1U: {
                                number.Number.Natural = val_number.Natural;
                                number.Type           = ExpressionType::NaturalNumber;
                                return true;
                            }

                            case 2U: {
                                number.Number.Integer = val_number.Integer;
                                number.Type           = ExpressionType::IntegerNumber;
                                return true;
                            }

                            case 3U: {
                                number.Number.Real = val_number.Real;
                                number.Type        = ExpressionType::RealNumber;
                                return true;
                            }

                            default: {
                                return false;
                            }
                        }
                    } else {
                        return false;
                    }
                } else {
                    number.Number = expr->Number;
                    number.Type   = expr->Type;
                }

                return true;
            }

            default: {
                number.Number = expr->Number;
                number.Type   = expr->Type;
                return true;
            }
        }
    }

    bool evaluateExpression(QExpression &left, QExpression &right, const QOperation oper) const noexcept {
        switch (oper) {
            case QOperation::Exponent: { // ^
                return (left ^= right);
            }

            case QOperation::Remainder: { // %
                left.Number.Integer = (left % right);
                left.Type           = ExpressionType::IntegerNumber;
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

            case QOperation::Less: { // <
                left.Number.Natural = (unsigned long long)(left < right);
                left.Type           = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::LessOrEqual: { // <=
                left.Number.Natural = (unsigned long long)(left <= right);
                left.Type           = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::Greater: { // >
                left.Number.Natural = (unsigned long long)(left > right);
                left.Type           = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::GreaterOrEqual: { // >=
                left.Number.Natural = (unsigned long long)(left >= right);
                left.Type           = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::And: { // &&
                left.Number.Natural = (unsigned long long)((left > 0U) && (right > 0U));
                left.Type           = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::Or: { // ||
                left.Number.Natural = (unsigned long long)((left > 0U) || (right > 0U));
                left.Type           = ExpressionType::NaturalNumber;
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
                    left.Number.Natural = (left.Number.Natural ^ 1ULL);
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
        QNumber         val_number;
        const Value_T_ *left_value        = nullptr;
        const Value_T_ *right_value       = nullptr;
        const Char_T_  *left_content      = nullptr;
        const Char_T_  *right_content     = nullptr;
        SizeT           left_length       = 0;
        SizeT           right_length      = 0;
        bool            left_is_a_number  = false;
        bool            right_is_a_number = false;

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
                    switch (left_value->GetNumberType()) {
                        case 1U:
                        case 2U:
                        case 3U: {
                            left_is_a_number = true;

                            switch (left_value->SetNumber(val_number)) {
                                case 1U: {
                                    left.Number.Natural = val_number.Natural;
                                    left.Type           = ExpressionType::NaturalNumber;
                                    break;
                                }

                                case 2U: {
                                    left.Number.Integer = val_number.Integer;
                                    left.Type           = ExpressionType::IntegerNumber;
                                    break;
                                }

                                case 3U: {
                                    left.Number.Real = val_number.Real;
                                    left.Type        = ExpressionType::RealNumber;
                                    break;
                                }

                                default: {
                                }
                            }

                            break;
                        }

                        default: {
                            if (!(left_value->SetCharAndLength(left_content, left_length))) {
                                return false;
                            }
                        }
                    }

                    break;
                }

                return false;
            }

            default: {
                left_content = (content_ + left.Number.Offset);
                left_length  = left.Number.Length;
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
                    switch (right_value->GetNumberType()) {
                        case 1U:
                        case 2U:
                        case 3U: {
                            right_is_a_number = true;

                            switch (right_value->SetNumber(val_number)) {
                                case 1U: {
                                    right.Number.Natural = val_number.Natural;
                                    right.Type           = ExpressionType::NaturalNumber;
                                    break;
                                }

                                case 2U: {
                                    right.Number.Integer = val_number.Integer;
                                    right.Type           = ExpressionType::IntegerNumber;
                                    break;
                                }

                                case 3U: {
                                    right.Number.Real = val_number.Real;
                                    right.Type        = ExpressionType::RealNumber;
                                    break;
                                }

                                default: {
                                }
                            }

                            break;
                        }

                        default: {
                            if (!(right_value->SetCharAndLength(right_content, right_length))) {
                                return false;
                            }
                        }
                    }

                    break;
                }

                return false;
            }

            default: {
                right_content     = (content_ + right.Number.Offset);
                right_length      = right.Number.Length;
                right_is_a_number = false;
            }
        }

        if (left_is_a_number || right_is_a_number) {
            if (!left_is_a_number) {
                if (left_value != nullptr) {
                    switch (left_value->SetNumber(val_number)) {
                        case 1U: {
                            left.Number.Natural = val_number.Natural;
                            left.Type           = ExpressionType::NaturalNumber;
                            break;
                        }

                        case 2U: {
                            left.Number.Integer = val_number.Integer;
                            left.Type           = ExpressionType::IntegerNumber;
                            break;
                        }

                        case 3U: {
                            left.Number.Real = val_number.Real;
                            left.Type        = ExpressionType::RealNumber;
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
                    switch (right_value->SetNumber(val_number)) {
                        case 1U: {
                            right.Number.Natural = val_number.Natural;
                            right.Type           = ExpressionType::NaturalNumber;
                            break;
                        }

                        case 2U: {
                            right.Number.Integer = val_number.Integer;
                            right.Type           = ExpressionType::IntegerNumber;
                            break;
                        }

                        case 3U: {
                            right.Number.Real = val_number.Real;
                            right.Type        = ExpressionType::RealNumber;
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

            left.Number.Natural = (unsigned long long)(left == right);
            left.Type           = ExpressionType::NaturalNumber;
            return true;
        }

        left.Number.Natural =
            ((left_length == right_length) && StringUtils::IsEqual(left_content, right_content, right_length));
        left.Type = ExpressionType::NaturalNumber;

        return true;
    }

    QExpressions parseExpressions(SizeT offset, const SizeT end_offset) const {
        QExpressions exprs;
        QOperation   last_oper = QOperation::NoOp;

        while (offset < end_offset) {
            const SizeT      num_offset = offset;
            const QOperation oper       = getOperation(offset, end_offset);

            if ((oper != QOperation::Error) && parseValue(exprs, oper, last_oper, num_offset, offset)) {
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
                    SizeT end_offset) const {
        using QOperationSymbol = QOperationSymbol_T_<Char_T_>;

        StringUtils::TrimLeft(content_, offset, end_offset);
        StringUtils::TrimRight(content_, offset, end_offset);

        if (offset != end_offset) {
            switch (content_[offset]) {
                case QOperationSymbol::ParenthesesStart: {
                    ++offset;     // Drop (
                    --end_offset; // Drop )

                    if ((last_oper != oper) || (oper != QOperation::NoOp)) {
                        const QExpression &expr =
                            exprs.InsertGet(QExpression{parseExpressions(offset, end_offset), oper});
                        return (expr.SubExpressions.Size() != 0);
                    }

                    // The entire expression is inside (...)
                    exprs = parseExpressions(offset, end_offset);
                    return (exprs.Size() != 0);
                }

                case QOperationSymbol::BracketStart: {
                    if (end_offset - offset > TagPatterns::VariableFullLength) {
                        offset += TagPatterns::VariablePrefixLength;
                        end_offset -= TagPatterns::InLineSuffixLength;

                        QExpression &expr = exprs.InsertGet(QExpression{ExpressionType::Variable, oper});
                        parseVariableTag(offset, end_offset, &(expr.Variable));
                        return true;
                    }

                    break;
                }

                default: {
                    const SizeT original_offset = offset;
                    QExpression expr;
                    QNumber     number;
                    expr.Operation = oper;

                    const unsigned int n_type = Digit::StringToNumber(number, content_, offset, end_offset);

                    if ((n_type != 0) && (offset == end_offset)) {
                        switch (n_type) {
                            case 1U: {
                                expr.Number.Natural = number.Natural;
                                expr.Type           = ExpressionType::NaturalNumber;
                                break;
                            }

                            case 2U: {
                                expr.Number.Integer = number.Integer;
                                expr.Type           = ExpressionType::IntegerNumber;
                                break;
                            }

                            case 3U: {
                                expr.Number.Real = number.Real;
                                expr.Type        = ExpressionType::RealNumber;
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

                        expr.Number.Offset = original_offset;
                        expr.Number.Length = (end_offset - original_offset);
                        expr.Type          = ExpressionType::NotANumber;
                    }

                    exprs += Memory::Move(expr);

                    return true;
                }
            }
        }

        return false;
    }

    QOperation getOperation(SizeT &offset, const SizeT end_offset) const noexcept {
        using QOperationSymbol = QOperationSymbol_T_<Char_T_>;

        while (offset < end_offset) {
            switch (content_[offset]) {
                case QOperationSymbol::OrExp: { // ||
                    if (content_[(offset + 1)] == QOperationSymbol::OrExp) {
                        return QOperation::Or;
                    }

                    return QOperation::Error;
                }

                case QOperationSymbol::AndExp: { // &&
                    if (content_[(offset + 1)] == QOperationSymbol::AndExp) {
                        return QOperation::And;
                    }

                    return QOperation::Error;
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
                    offset = Engine::SkipInnerPatterns<Char_T_>(QOperationSymbol::ParenthesesStart,
                                                                QOperationSymbol::ParenthesesEnd, content_, offset,
                                                                end_offset, length_);

                    if (offset != 0) {
                        continue;
                    }

                    return QOperation::Error;
                }

                case QOperationSymbol::BracketStart: {
                    ++offset;
                    offset =
                        Engine::FindOne<Char_T_>(QOperationSymbol::BracketEnd, content_, offset, end_offset, length_);

                    if (offset != 0) {
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
        using QOperationSymbol = QOperationSymbol_T_<Char_T_>;

        while (offset != 0) {
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
                    const Char_T_ ch = content_[offset];
                    return ((ch < QOperationSymbol::ColonChar) && (ch > QOperationSymbol::SlashChar));
                }
            }
        }

        return false;
    }

    const Char_T_       *content_;
    StringStream_T_     *stream_;
    const Value_T_      *value_;
    const TemplateSub   *parent_;
    const Value_T_      *loop_value_{nullptr};
    const Char_T_       *loop_key_{nullptr};
    SizeT                loop_key_length_{0};
    SizeT                loop_value_offset_{0};
    const SizeT          length_;
    unsigned short       loop_value_length_{0};
    const unsigned short level_;
};

} // namespace Qentem

#endif
