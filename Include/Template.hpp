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

#ifndef _QENTEM_TEMPLATE_H
#define _QENTEM_TEMPLATE_H

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
struct _TemplatePatterns_T;

template <typename>
struct _QOperationSymbol_T;

struct Template {
    Template()                            = delete;
    Template(Template &&)                 = delete;
    Template(const Template &)            = delete;
    Template &operator=(Template &&)      = delete;
    Template &operator=(const Template &) = delete;
    ~Template()                           = delete;

    // template <typename _Char_T, typename _Value_T, typename _StringStream_T>
    // inline static void CachedRender(const _Char_T *content, const SizeT length, const _Value_T &value,
    //                                 _StringStream_T &stream, const _Char_T *name, const SizeT name_length) {
    //     // This is not a thread safe function, and its here to show how to cache processed tags.
    //     // Can be used in a single threaded process to build on.

    //     // Usage:
    //     // #include "HArray.hpp"
    //     // #include "Value.hpp"
    //     // #include "Template.hpp"
    //     //
    //     // CachedRender("<html>...</html>", 16, value, stringstream, "page1", 5);

    //     using TemplateSubCV = TemplateSub<_Char_T, _Value_T, _StringStream_T>;

    //     const TemplateSubCV temp{content, length, &stream, &value};

    //     static HArray<Array<Tags::TagBit>, _Char_T> cache;

    //     Array<Tags::TagBit> &tags = cache.Get(name, name_length);

    //     if (tags.IsEmpty()) {
    //         temp.Parse(tags);
    //     }

    //     const Tags::TagBit *tag = tags.First();
    //     const Tags::TagBit *end = (tag + tags.Size());
    //     temp.Render(tag, end);
    // }

    template <typename _Char_T, typename _Value_T, typename _StringStream_T>
    inline static _StringStream_T &Render(const _Char_T *content, SizeT length, const _Value_T &value,
                                          _StringStream_T &stream, Array<Tags::TagBit> &tags_cache) {
        using TemplateSubCV = TemplateSub<_Char_T, _Value_T, _StringStream_T>;
        const TemplateSubCV temp{content, length, &stream, &value};

        if (tags_cache.IsEmpty()) {
            temp.Parse(tags_cache);
        }

        const Tags::TagBit *tag = tags_cache.First();
        const Tags::TagBit *end = (tag + tags_cache.Size());
        temp.Render(tag, end);
        return stream;
    }

    template <typename _Char_T, typename _Value_T, typename _StringStream_T>
    inline static _StringStream_T &Render(const _Char_T *content, const SizeT length, const _Value_T &value,
                                          _StringStream_T &stream) {
        Array<Tags::TagBit> tags_cache;
        Render(content, length, value, stream, tags_cache);
        return stream;
    }

    template <typename _Char_T, typename _Value_T, typename _StringStream_T>
    inline static _StringStream_T &Render(const _Char_T *content, const _Value_T &value, _StringStream_T &stream) {
        Array<Tags::TagBit> tags_cache;
        Render(content, StringUtils::Count(content), value, stream, tags_cache);
        return stream;
    }

    template <typename _StringStream_T, typename _Char_T, typename _Value_T>
    inline static _StringStream_T Render(const _Char_T *content, const SizeT length, const _Value_T &value) {
        _StringStream_T stream;
        Render(content, length, value, stream);
        return stream;
    }

    template <typename _StringStream_T, typename _Char_T, typename _Value_T>
    inline static _StringStream_T Render(const _Char_T *content, const _Value_T &value) {
        return Render<_StringStream_T>(content, StringUtils::Count(content), value);
    }
};

template <typename _Char_T, typename _Value_T, typename _StringStream_T>
struct TemplateSub {
    TemplateSub() = delete;

    TemplateSub(const _Char_T *content, SizeT length, _StringStream_T *stream, const _Value_T *root_value,
                const TemplateSub *parent = nullptr, const SizeT16 level = 0) noexcept
        : _content{content}, _stream{stream}, _value{root_value}, _parent{parent}, _length{length}, _level{level} {
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
    using TagPatterns     = Tags::_TagPatterns_T<_Char_T>;

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
        parse(tags_cache, 0, _length);
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
            if (_content[offset] == TagPatterns::InLinePrefix) {
                const SizeT current_offset = (offset + SizeT{1});

                switch (_content[current_offset]) {
                    case TagPatterns::Var_2ND_Char: {
                        if (StringUtils::IsEqual((TagPatterns::VariablePrefix + SizeT{2}),
                                                 (_content + current_offset + SizeT{1}),
                                                 (TagPatterns::VariablePrefixLength - SizeT{2}))) {
                            const SizeT v_end_offset = Engine::FindOne<_Char_T>(
                                TagPatterns::InLineSuffix, _content,
                                SizeT(current_offset + TagPatterns::VariablePrefixLength), end_offset, _length);

                            if (v_end_offset != SizeT{0}) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{last_offset, SizeT(offset - last_offset)};
                                }

                                offset += TagPatterns::VariablePrefixLength;
                                const TagBit &tag = tags_cache.Insert(TagBit{TagType::Variable});
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
                        if (StringUtils::IsEqual((TagPatterns::RawVariablePrefix + SizeT{2}),
                                                 (_content + current_offset + SizeT{1}),
                                                 (TagPatterns::RawVariablePrefixLength - SizeT{2}))) {
                            const SizeT r_end_offset = Engine::FindOne<_Char_T>(
                                TagPatterns::InLineSuffix, _content,
                                SizeT(current_offset + TagPatterns::RawVariablePrefixLength), end_offset, _length);

                            if (r_end_offset != SizeT{0}) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{last_offset, SizeT(offset - last_offset)};
                                }

                                offset += TagPatterns::RawVariablePrefixLength;
                                const TagBit &tag = tags_cache.Insert(TagBit{TagType::RawVariable});
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
                        if (StringUtils::IsEqual((TagPatterns::MathPrefix + SizeT{2}),
                                                 (_content + current_offset + SizeT{1}),
                                                 (TagPatterns::MathPrefixLength - SizeT{2}))) {
                            const SizeT m_end_offset = Engine::SkipInnerPatterns<_Char_T>(
                                TagPatterns::InLinePrefix, TagPatterns::InLineSuffix, _content,
                                SizeT(current_offset + TagPatterns::MathPrefixLength - SizeT{1}), end_offset, _length);

                            if (m_end_offset != SizeT{0}) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{last_offset, SizeT(offset - last_offset)};
                                }

                                offset += TagPatterns::MathPrefixLength;
                                const TagBit &tag = tags_cache.Insert(TagBit{TagType::Math});
                                parseMathTag(offset, (m_end_offset - TagPatterns::InLineSuffixLength), tag.GetInfo());
                                last_offset = m_end_offset;
                                offset      = m_end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TagPatterns::InlineIf_2ND_Char: {
                        if (StringUtils::IsEqual((TagPatterns::InLineIfPrefix + SizeT{2}),
                                                 (_content + current_offset + SizeT{1}),
                                                 (TagPatterns::InLineIfPrefixLength - SizeT{2}))) {
                            const SizeT ii_end_offset = Engine::SkipInnerPatterns<_Char_T>(
                                TagPatterns::InLinePrefix, TagPatterns::InLineSuffix, _content,
                                SizeT(current_offset + TagPatterns::InLineIfPrefixLength - SizeT{1}), end_offset,
                                _length);

                            if (ii_end_offset != SizeT{0}) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{last_offset, SizeT(offset - last_offset)};
                                }

                                offset += TagPatterns::InLineIfPrefixLength;
                                const TagBit &tag = tags_cache.Insert(TagBit{TagType::InLineIf});
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
            } else if (_content[offset] == TagPatterns::MultiLinePrefix) { // <loop or <if
                const SizeT current_offset = (offset + SizeT{1});

                if (_content[current_offset] == TagPatterns::Loop_2ND_Char) { // <loop
                    if (StringUtils::IsEqual((TagPatterns::LoopPrefix + SizeT{2}),
                                             (_content + current_offset + SizeT{1}),
                                             (TagPatterns::LoopPrefixLength - SizeT{2}))) {
                        const SizeT l_end_offset = Engine::SkipInnerPatterns<_Char_T>(
                            TagPatterns::LoopPrefix, TagPatterns::LoopPrefixLength, TagPatterns::LoopSuffix,
                            TagPatterns::LoopSuffixLength, _content,
                            SizeT(current_offset + TagPatterns::InLineIfPrefixLength - SizeT{1}), end_offset, _length);

                        if (l_end_offset != SizeT{0}) {
                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, SizeT(offset - last_offset)};
                            }

                            offset += TagPatterns::InLineIfPrefixLength;
                            const TagBit &tag = tags_cache.Insert(TagBit{TagType::Loop});
                            parseLoopTag(offset, (l_end_offset - TagPatterns::LoopSuffixLength), tag.GetInfo());
                            last_offset = l_end_offset;
                            offset      = l_end_offset;
                            continue;
                        }
                    }
                } else if (_content[current_offset] == TagPatterns::If_2ND_Char) { // <if
                    if (StringUtils::IsEqual((TagPatterns::IfPrefix + SizeT{2}), (_content + current_offset + SizeT{1}),
                                             (TagPatterns::IfPrefixLength - SizeT{2}))) {
                        const SizeT i_end_offset = Engine::SkipInnerPatterns<_Char_T>(
                            TagPatterns::IfPrefix, TagPatterns::IfPrefixLength, TagPatterns::IfSuffix,
                            TagPatterns::IfSuffixLength, _content, current_offset, end_offset, _length);

                        if (i_end_offset != SizeT{0}) {
                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, SizeT(offset - last_offset)};
                            }

                            offset += TagPatterns::IfPrefixLength;
                            const TagBit &tag = tags_cache.Insert(TagBit{TagType::If});
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
            tags_cache += TagBit{last_offset, SizeT(end_offset - last_offset)};
        }
    }

    // Part of parse()
    void lightParse(Array<TagBit> &tags_cache, SizeT offset, const SizeT end_offset) const {
        SizeT last_offset = offset;

        while (true) {
            offset = Engine::FindOne<_Char_T>(TagPatterns::InLinePrefix, _content, offset, end_offset, _length);

            if (offset != 0) {
                switch (_content[offset]) {
                    case TagPatterns::Var_2ND_Char: {
                        if (StringUtils::IsEqual((TagPatterns::VariablePrefix + SizeT{2}),
                                                 (_content + offset + SizeT{1}),
                                                 (TagPatterns::VariablePrefixLength - SizeT{2}))) {
                            const SizeT v_end_offset = Engine::FindOne<_Char_T>(
                                TagPatterns::InLineSuffix, _content, SizeT(offset + TagPatterns::VariablePrefixLength),
                                end_offset, _length);
                            offset -= TagPatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, SizeT(offset - last_offset)};
                            }

                            offset += TagPatterns::VariablePrefixLength;
                            const TagBit &tag = tags_cache.Insert(TagBit{TagType::Variable});
                            parseVariableTag(offset, (v_end_offset - TagPatterns::InLineSuffixLength), tag.GetInfo());
                            last_offset = v_end_offset;
                            offset      = v_end_offset;
                        }

                        continue;
                    }

                    case TagPatterns::Raw_2ND_Char: {
                        if (StringUtils::IsEqual((TagPatterns::RawVariablePrefix + SizeT{2}),
                                                 (_content + offset + SizeT{1}),
                                                 (TagPatterns::RawVariablePrefixLength - SizeT{2}))) {
                            const SizeT r_end_offset = Engine::FindOne<_Char_T>(
                                TagPatterns::InLineSuffix, _content,
                                SizeT(offset + TagPatterns::RawVariablePrefixLength), end_offset, _length);
                            offset -= TagPatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, SizeT(offset - last_offset)};
                            }

                            offset += TagPatterns::RawVariablePrefixLength;
                            const TagBit &tag = tags_cache.Insert(TagBit{TagType::RawVariable});
                            parseVariableTag(offset, (r_end_offset - TagPatterns::InLineSuffixLength), tag.GetInfo());
                            last_offset = r_end_offset;
                            offset      = r_end_offset;
                        }

                        continue;
                    }

                    case TagPatterns::Math_2ND_Char: {
                        if (StringUtils::IsEqual((TagPatterns::MathPrefix + SizeT{2}), (_content + offset + SizeT{1}),
                                                 (TagPatterns::MathPrefixLength - SizeT{2}))) {
                            const SizeT m_end_offset = Engine::SkipInnerPatterns<_Char_T>(
                                TagPatterns::InLinePrefix, TagPatterns::InLineSuffix, _content,
                                SizeT(offset + TagPatterns::MathPrefixLength - SizeT{1}), end_offset, _length);
                            offset -= TagPatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, SizeT(offset - last_offset)};
                            }

                            offset += TagPatterns::MathPrefixLength;
                            const TagBit &tag = tags_cache.Insert(TagBit{TagType::Math});
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
            tags_cache += TagBit{last_offset, SizeT(end_offset - last_offset)};
        }
    }

    static void escapeHTMLSpecialChars(_StringStream_T *stream, const _Char_T *str, SizeT length) {
        SizeT offset = 0;
        SizeT index  = 0;

        while (index < length) {
            switch (str[index]) {
                case '&': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TagPatterns::HTMLAnd, TagPatterns::HTMLAndLength);
                    ++index;
                    offset = index;
                    break;
                }

                case '<': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TagPatterns::HTMLLess, TagPatterns::HTMLLessLength);
                    ++index;
                    offset = index;
                    break;
                }

                case '>': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TagPatterns::HTMLGreater, TagPatterns::HTMLGreaterLength);
                    ++index;
                    offset = index;
                    break;
                }

                case '"': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TagPatterns::HTMLQuote, TagPatterns::HTMLQuoteLength);
                    ++index;
                    offset = index;
                    break;
                }

                case '\'': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TagPatterns::HTMLSingleQuote, TagPatterns::HTMLSingleQuoteLength);
                    ++index;
                    offset = index;
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
        const _Value_T    *value = getValue(i_tag);

        if (value != nullptr) {
            if (Config::AutoEscapeHTML) {
                if (value->IsString()) {
                    const _Char_T *str;
                    SizeT          len;
                    value->SetCharAndLength(str, len);
                    escapeHTMLSpecialChars(_stream, str, len);
                    return;
                }
            }

            if (value->CopyValueTo(*_stream, Config::TemplatePrecision)) {
                return;
            }

            if ((i_tag.IsLoopValue == 1) && (_loop_key != nullptr)) {
                if (Config::AutoEscapeHTML) {
                    escapeHTMLSpecialChars(_stream, _loop_key, _loop_key_length);
                } else {
                    _stream->Write(_loop_key, _loop_key_length);
                }

                return;
            }
        }

        _stream->Write(((_content + i_tag.Offset) - TagPatterns::VariablePrefixLength),
                       (i_tag.Length + TagPatterns::VariablePrefixLength + TagPatterns::InLineSuffixLength));
    }

    void renderRawVariable(const TagBit *tag) const {
        const VariableTag &i_tag = *((const VariableTag *)(tag->GetInfo()));
        const _Value_T    *value = getValue(i_tag);

        if ((value == nullptr) || !(value->CopyValueTo(*_stream, Config::TemplatePrecision))) {
            _stream->Write(((_content + i_tag.Offset) - TagPatterns::RawVariablePrefixLength),
                           (i_tag.Length + TagPatterns::RawVariableFullLength));
        }
    }

    void renderMath(const TagBit *tag) const {
        const MathTag     &i_tag = *((const MathTag *)(tag->GetInfo()));
        const QExpression *expr  = i_tag.Expressions.First();
        QExpression        result;

        if (i_tag.Expressions.IsNotEmpty() && evaluate(result, expr, QOperation::NoOp)) {
            switch (result.Type) {
                case ExpressionType::NaturalNumber: {
                    Digit::NumberToString(*_stream, result.Number.Natural);
                    break;
                }

                case ExpressionType::IntegerNumber: {
                    Digit::NumberToString(*_stream, result.Number.Integer);
                    break;
                }

                case ExpressionType::RealNumber: {
                    Digit::NumberToString(*_stream, result.Number.Real, Config::TemplatePrecision);
                    break;
                }

                default: {
                }
            }

        } else {
            _stream->Write(
                ((_content + i_tag.Offset) - TagPatterns::MathPrefixLength),
                ((i_tag.EndOffset - i_tag.Offset) + TagPatterns::MathPrefixLength + TagPatterns::InLineSuffixLength));
        }
    }

    void renderLoop(const TagBit *tag) const {
        const LoopTag  &i_tag = *((const LoopTag *)(tag->GetInfo()));
        _Value_T        grouped_set;
        const _Value_T *loop_set;

        // Set (Array|Object)
        if (i_tag.SetLength != 0) {
            loop_set = getValue((i_tag.Offset + i_tag.SetOffset), i_tag.SetLength, i_tag.SetLevel,
                                (i_tag.Options & LoopTagOptions::SetIsLoopValue));
        } else {
            loop_set = _value;
        }

        if (loop_set != nullptr) {
            // Group
            if (i_tag.GroupLength != 0) {
                if (!(loop_set->GroupBy(grouped_set, (_content + i_tag.Offset + i_tag.GroupOffset),
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

            const SizeT16 level = (_level + 1U);

            // Stage 4: Render
            TemplateSub loop_template{_content, _length, _stream, _value, this, level};
            loop_template._loop_value_offset = (i_tag.Offset + i_tag.ValueOffset);
            loop_template._loop_value_length = i_tag.ValueLength;

            const TagBit *s_tag = i_tag.SubTags.First();
            const TagBit *s_end = (s_tag + i_tag.SubTags.Size());

            const SizeT loop_size  = loop_set->Size();
            SizeT       loop_index = 0;

            if (loop_set->IsObject()) {
                while (loop_index < loop_size) {
                    loop_set->SetValueKeyLength(loop_index, loop_template._loop_value, loop_template._loop_key,
                                                loop_template._loop_key_length);

                    if (loop_template._loop_value != nullptr) {
                        loop_template.Render(s_tag, s_end);
                    }

                    ++loop_index;
                }
            } else {
                while (loop_index < loop_size) {
                    loop_template._loop_value = loop_set->GetValue(loop_index);

                    if (loop_template._loop_value != nullptr) {
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
                if (true_size != SizeT{0}) {
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
        _stream->Write((_content + tag->GetOffset()), SizeT(tag->GetLength()));
    }

    void parseVariableTag(SizeT offset, SizeT end_offset, void *tag) const noexcept {
        VariableTag &i_tag = *((VariableTag *)(tag));
        i_tag.Offset       = offset;
        i_tag.Length       = ((end_offset - offset) & SizeT{0xFF}); // Limit var length to 255 meter per second.

        if (_loop_value_length != 0) {
            const TemplateSub *temp = this;

            SizeT l_offset;
            SizeT length;
            SizeT level = _level;

            while (level > 0) {
                l_offset = temp->_loop_value_offset;
                length   = temp->_loop_value_length;

                if (StringUtils::IsEqual((_content + offset), (_content + l_offset), length)) {
                    i_tag.IsLoopValue = 1;
                    i_tag.Level       = (level & 0xFFU);
                    break;
                }

                temp = temp->_parent;
                --level;
            }
        }
    }

    void parseMathTag(const SizeT offset, const SizeT end_offset, void *tag) const {
        MathTag &i_tag    = *((MathTag *)(tag));
        i_tag.Offset      = offset;
        i_tag.EndOffset   = end_offset;
        i_tag.Expressions = parseExpressions(i_tag.Offset, i_tag.EndOffset);
    }

    /*
     * Gets everything between "..."
     */
    SizeT getQuotedValue(SizeT &offset, SizeT end_offset) const noexcept {
        offset = Engine::FindOne<_Char_T>(TagPatterns::QuoteChar, _content, offset, end_offset, _length);

        if (offset != SizeT{0}) {
            end_offset = Engine::FindOne<_Char_T>(TagPatterns::QuoteChar, _content, offset, end_offset, _length);
        }

        return end_offset;
    }

    void parseLoopTag(SizeT offset, SizeT end_offset, void *tag) const {
        LoopTag    &i_tag = *((LoopTag *)(tag));
        const SizeT loop_content_offset =
            Engine::FindOne<_Char_T>(TagPatterns::MultiLineSuffix, _content, offset, end_offset, _length);

        SizeT offset2     = getQuotedValue(offset, loop_content_offset);
        i_tag.Offset      = offset;
        SizeT last_offset = offset;
        offset -= 6U; // (=) plus (") plus (two chars) = 4 plus (the char before them) = 5

        while ((offset2 != SizeT{0}) && (offset < loop_content_offset)) {
            switch (_content[offset]) {
                case TagPatterns::SetChar: {
                    const SizeT set_offset = last_offset;
                    const SizeT set_length = ((offset2 - SizeT{1}) - last_offset);

                    VariableTag set_var{};
                    parseVariableTag(set_offset, (set_offset + set_length), &set_var);
                    i_tag.SetOffset = SizeT8(set_var.Offset - i_tag.Offset);
                    i_tag.SetLength = SizeT8(set_var.Length);
                    i_tag.SetLevel  = set_var.Level;
                    i_tag.Options |= set_var.IsLoopValue;

                    offset      = offset2;
                    offset2     = getQuotedValue(offset, loop_content_offset);
                    last_offset = offset;
                    offset -= SizeT{6};
                    break;
                }

                case TagPatterns::ValueChar: {
                    i_tag.ValueOffset = SizeT8(last_offset - i_tag.Offset);
                    i_tag.ValueLength = SizeT8((offset2 - SizeT{1}) - last_offset);
                    offset            = offset2;
                    offset2           = getQuotedValue(offset, loop_content_offset);
                    last_offset       = offset;
                    offset -= SizeT{6};
                    break;
                }

                case TagPatterns::SortChar: {
                    i_tag.Options |=
                        ((_content[last_offset] == 'a') ? LoopTagOptions::SortAscend : LoopTagOptions::SortDescend);
                    offset      = offset2;
                    offset2     = getQuotedValue(offset, loop_content_offset);
                    last_offset = offset;
                    offset -= SizeT{6};
                    break;
                }

                case TagPatterns::GroupChar: {
                    i_tag.GroupOffset = SizeT8(last_offset - i_tag.Offset);
                    i_tag.GroupLength = SizeT8((offset2 - SizeT{1}) - last_offset);
                    offset            = offset2;
                    offset2           = getQuotedValue(offset, loop_content_offset);
                    last_offset       = offset;
                    offset -= SizeT{6};
                    break;
                }

                default: {
                    ++offset;
                }
            }
        }

        const SizeT16 level = (_level + 1);
        TemplateSub   loop_template{_content, _length, nullptr, nullptr, this, level};
        loop_template._loop_value_offset = (i_tag.Offset + i_tag.ValueOffset);
        loop_template._loop_value_length = i_tag.ValueLength;
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

        while ((offset2 != SizeT{0}) && (offset < end_offset)) {
            switch (_content[offset]) {
                case TagPatterns::CaseChar: {
                    i_tag.Case  = parseExpressions(last_offset, (offset2 - SizeT{1})); // 1 = (") at the end
                    offset      = offset2;
                    offset2     = getQuotedValue(offset, end_offset);
                    last_offset = offset;
                    offset -= 5U;
                    break;
                }

                case TagPatterns::TrueChar: {
                    true_offset     = last_offset;
                    true_end_offset = (offset2 - SizeT{1});
                    offset          = offset2;
                    offset2         = getQuotedValue(offset, end_offset);
                    last_offset     = offset;
                    offset -= 5U;
                    break;
                }

                case TagPatterns::FalseChar: {
                    false_offset     = last_offset;
                    false_end_offset = (offset2 - SizeT{1});
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
                QExpression &expr   = i_tag.Case.Insert(QExpression{});
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

        while ((offset2 != SizeT{0}) && (offset < end_offset)) {
            case_end_offset = (offset2 - SizeT{1});
            content_offset =
                Engine::FindOne<_Char_T>(TagPatterns::MultiLineSuffix, _content, offset2, end_offset, _length);

            if (content_offset != SizeT{0}) {
                else_offset = nextElse(content_offset, end_offset);

                if (else_offset == SizeT{0}) {
                    parse(sub_tags, content_offset, end_offset);
                    i_tag += IfTagCase{Memory::Move(sub_tags), parseExpressions(offset, case_end_offset)};

                    break;
                }

                parse(sub_tags, content_offset, (else_offset - TagPatterns::ElsePrefixLength));
                i_tag += IfTagCase{Memory::Move(sub_tags), parseExpressions(offset, case_end_offset)};

                if ((_content[else_offset] != TagPatterns::ElseIfChar)) {
                    else_offset = Engine::FindOne<_Char_T>(TagPatterns::MultiLineSuffix, _content, else_offset,
                                                           end_offset, _length);

                    if (else_offset != SizeT{0}) {
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
            else_offset = Engine::Find<_Char_T>(TagPatterns::ElsePrefix, TagPatterns::ElsePrefixLength, _content,
                                                offset, end_offset, _length);

            if (else_offset == SizeT{0}) {
                // No <else.
                break;
            }

            const SizeT next_if = Engine::Find<_Char_T>(TagPatterns::IfPrefix, TagPatterns::IfPrefixLength, _content,
                                                        offset, end_offset, _length);

            if ((next_if == SizeT{0}) || (else_offset < next_if)) {
                // No nesting <ifs or <else before a sub-if.
                break;
            }

            offset = Engine::Find<_Char_T>(TagPatterns::IfSuffix, TagPatterns::IfSuffixLength, _content, next_if,
                                           end_offset, _length);

            if (else_offset > offset) {
                // <else is after a sub-if.
                break;
            }
        }

        return else_offset;
    }

    const _Value_T *getValue(const VariableTag &variable) const noexcept {
        return getValue(variable.Offset, variable.Length, variable.Level, variable.IsLoopValue);
    }

    const _Value_T *getValue(SizeT v_offset, SizeT16 v_length, SizeT8 v_level, SizeT8 v_is_loop_value) const noexcept {
        const _Value_T *value     = nullptr;
        const _Char_T  *id        = (_content + v_offset);
        const SizeT     length    = v_length;
        SizeT           offset    = 0;
        const bool      has_index = (id[(length - SizeT{1})] == TagPatterns::VariableIndexSuffix);

        if (v_is_loop_value != 1) {
            if (!has_index) {
                return _value->GetValue(id, length);
            }

            while ((offset < length) && (id[offset] != TagPatterns::VariableIndexPrefix)) {
                ++offset;
            }

            if (offset != SizeT{0}) {
                // {var:abc[...]}
                // if offset == 0 then its {var:[...]}
                value = _value->GetValue(id, offset);
            }
        } else {
            const TemplateSub *that = this;
            {
                const SizeT32 level    = SizeT32(_level);
                SizeT32       iv_level = SizeT32(v_level);
                value                  = _loop_value;

                while (iv_level < level) {
                    that = that->_parent;
                    ++iv_level;
                }
            }

            value = that->_loop_value;

            if (!has_index) {
                return value;
            }

            offset += _loop_value_length;
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
                    const _Value_T *val = getValue(expr->Variable);

                    if (val != nullptr) {
                        QNumber64 val_number;

                        switch (val->SetNumber(val_number)) {
                            case QNumberType::Natural: {
                                number.Number.Natural = val_number.Natural;
                                number.Type           = ExpressionType::NaturalNumber;
                                return true;
                            }

                            case QNumberType::Integer: {
                                number.Number.Integer = val_number.Integer;
                                number.Type           = ExpressionType::IntegerNumber;
                                return true;
                            }

                            case QNumberType::Real: {
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
                left.Number.Natural = SizeT64(left < right);
                left.Type           = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::LessOrEqual: { // <=
                left.Number.Natural = SizeT64(left <= right);
                left.Type           = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::Greater: { // >
                left.Number.Natural = SizeT64(left > right);
                left.Type           = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::GreaterOrEqual: { // >=
                left.Number.Natural = SizeT64(left >= right);
                left.Type           = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::And: { // &&
                left.Number.Natural = SizeT64((left > 0U) && (right > 0U));
                left.Type           = ExpressionType::NaturalNumber;
                break;
            }

            case QOperation::Or: { // ||
                left.Number.Natural = SizeT64((left > 0U) || (right > 0U));
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
        QNumber64       val_number;
        const _Value_T *left_value        = nullptr;
        const _Value_T *right_value       = nullptr;
        const _Char_T  *left_content      = nullptr;
        const _Char_T  *right_content     = nullptr;
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
                    if (left_value->GetNumberType() != QNumberType::NotANumber) {
                        left_is_a_number = true;

                        switch (left_value->SetNumber(val_number)) {
                            case QNumberType::Natural: {
                                left.Number.Natural = val_number.Natural;
                                left.Type           = ExpressionType::NaturalNumber;
                                break;
                            }

                            case QNumberType::Integer: {
                                left.Number.Integer = val_number.Integer;
                                left.Type           = ExpressionType::IntegerNumber;
                                break;
                            }

                            case QNumberType::Real: {
                                left.Number.Real = val_number.Real;
                                left.Type        = ExpressionType::RealNumber;
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
                left_content = (_content + left.Number.Offset);
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
                    if (right_value->GetNumberType() != QNumberType::NotANumber) {
                        right_is_a_number = true;

                        switch (right_value->SetNumber(val_number)) {
                            case QNumberType::Natural: {
                                right.Number.Natural = val_number.Natural;
                                right.Type           = ExpressionType::NaturalNumber;
                                break;
                            }

                            case QNumberType::Integer: {
                                right.Number.Integer = val_number.Integer;
                                right.Type           = ExpressionType::IntegerNumber;
                                break;
                            }

                            case QNumberType::Real: {
                                right.Number.Real = val_number.Real;
                                right.Type        = ExpressionType::RealNumber;
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
                right_content     = (_content + right.Number.Offset);
                right_length      = right.Number.Length;
                right_is_a_number = false;
            }
        }

        if (left_is_a_number || right_is_a_number) {
            if (!left_is_a_number) {
                if (left_value != nullptr) {
                    switch (left_value->SetNumber(val_number)) {
                        case QNumberType::Natural: {
                            left.Number.Natural = val_number.Natural;
                            left.Type           = ExpressionType::NaturalNumber;
                            break;
                        }

                        case QNumberType::Integer: {
                            left.Number.Integer = val_number.Integer;
                            left.Type           = ExpressionType::IntegerNumber;
                            break;
                        }

                        case QNumberType::Real: {
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
                        case QNumberType::Natural: {
                            right.Number.Natural = val_number.Natural;
                            right.Type           = ExpressionType::NaturalNumber;
                            break;
                        }

                        case QNumberType::Integer: {
                            right.Number.Integer = val_number.Integer;
                            right.Type           = ExpressionType::IntegerNumber;
                            break;
                        }

                        case QNumberType::Real: {
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

            left.Number.Natural = SizeT64(left == right);
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
        using QOperationSymbol = _QOperationSymbol_T<_Char_T>;

        StringUtils::TrimLeft(_content, offset, end_offset);
        StringUtils::TrimRight(_content, offset, end_offset);

        if (offset != end_offset) {
            switch (_content[offset]) {
                case QOperationSymbol::ParenthesesStart: {
                    ++offset;     // Drop (
                    --end_offset; // Drop )

                    if ((last_oper != oper) || (oper != QOperation::NoOp)) {
                        const QExpression &expr = exprs.Insert(QExpression{parseExpressions(offset, end_offset), oper});
                        return (expr.SubExpressions.Size() != SizeT{0});
                    }

                    // The entire expression is inside (...)
                    exprs = parseExpressions(offset, end_offset);
                    return (exprs.Size() != SizeT{0});
                }

                case QOperationSymbol::BracketStart: {
                    if (end_offset - offset > TagPatterns::VariableFullLength) {
                        offset += TagPatterns::VariablePrefixLength;
                        end_offset -= TagPatterns::InLineSuffixLength;

                        QExpression &expr = exprs.Insert(QExpression{ExpressionType::Variable, oper});
                        parseVariableTag(offset, end_offset, &(expr.Variable));
                        return true;
                    }

                    break;
                }

                default: {
                    const SizeT original_offset = offset;
                    QExpression expr;
                    QNumber64   number;
                    expr.Operation = oper;

                    const QNumberType n_type = Digit::StringToNumber(number, _content, offset, end_offset);

                    if ((n_type != QNumberType::NotANumber) && (offset == end_offset)) {
                        switch (n_type) {
                            case QNumberType::Natural: {
                                expr.Number.Natural = number.Natural;
                                expr.Type           = ExpressionType::NaturalNumber;
                                break;
                            }

                            case QNumberType::Integer: {
                                expr.Number.Integer = number.Integer;
                                expr.Type           = ExpressionType::IntegerNumber;
                                break;
                            }

                            case QNumberType::Real: {
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
        using QOperationSymbol = _QOperationSymbol_T<_Char_T>;

        while (offset < end_offset) {
            switch (_content[offset]) {
                case QOperationSymbol::OrExp: { // ||
                    if (_content[(offset + 1)] == QOperationSymbol::OrExp) {
                        return QOperation::Or;
                    }

                    return QOperation::Error;
                }

                case QOperationSymbol::AndExp: { // &&
                    if (_content[(offset + 1)] == QOperationSymbol::AndExp) {
                        return QOperation::And;
                    }

                    return QOperation::Error;
                }

                case QOperationSymbol::GreaterExp: { // > or >=
                    if (_content[(offset + 1)] == QOperationSymbol::EqualExp) {
                        return QOperation::GreaterOrEqual;
                    }

                    return QOperation::Greater;
                }

                case QOperationSymbol::LessExp: { // < or <=
                    if (_content[(offset + 1)] == QOperationSymbol::EqualExp) {
                        return QOperation::LessOrEqual;
                    }

                    return QOperation::Less;
                }

                case QOperationSymbol::NotExp: { // !=
                    if (_content[(offset + 1)] == QOperationSymbol::EqualExp) {
                        return QOperation::NotEqual;
                    }

                    return QOperation::Error;
                }

                case QOperationSymbol::EqualExp: { // ==
                    if (_content[(offset + 1)] == QOperationSymbol::EqualExp) {
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
                    offset = Engine::SkipInnerPatterns<_Char_T>(QOperationSymbol::ParenthesesStart,
                                                                QOperationSymbol::ParenthesesEnd, _content, offset,
                                                                end_offset, _length);

                    if (offset != SizeT{0}) {
                        continue;
                    }

                    return QOperation::Error;
                }

                case QOperationSymbol::BracketStart: {
                    ++offset;
                    offset =
                        Engine::FindOne<_Char_T>(QOperationSymbol::BracketEnd, _content, offset, end_offset, _length);

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
        using QOperationSymbol = _QOperationSymbol_T<_Char_T>;

        while (offset != SizeT{0}) {
            --offset;

            switch (_content[offset]) {
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
                    const _Char_T ch = _content[offset];
                    return ((ch < QOperationSymbol::ColonChar) && (ch > QOperationSymbol::SlashChar));
                }
            }
        }

        return false;
    }

    const _Char_T     *_content;
    _StringStream_T   *_stream;
    const _Value_T    *_value;
    const TemplateSub *_parent;
    const _Value_T    *_loop_value{nullptr};
    const _Char_T     *_loop_key{nullptr};
    SizeT              _loop_key_length{0};
    SizeT              _loop_value_offset{0};
    const SizeT        _length;
    SizeT16            _loop_value_length{0};
    const SizeT16      _level;
};

} // namespace Qentem

#endif
