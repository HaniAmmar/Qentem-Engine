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

#ifndef QENTEM_TEMPLATE_H
#define QENTEM_TEMPLATE_H

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
 *  - {if case="var|s" true="rvar|var|s" false="rvar|var|s"}
 *
 *      - Inline if,  rvar: Raw variable, var: Variable, s: String.
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
struct TemplatePatterns_T;

template <typename>
struct QOperationSymbol_T;

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
        using TemplateSubCV = TemplateSub<Char_T, Value_T, StringStream_T>;
        const TemplateSubCV temp{content, length, &stream, &value};

        if (tags_cache.IsEmpty()) {
            temp.Parse(tags_cache);
        }

        const Tags::TagBit *tag = tags_cache.First();
        const Tags::TagBit *end = (tag + tags_cache.Size());
        temp.Render(tag, end);
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
        Array<Tags::TagBit> tags_cache;
        Render(content, StringUtils::Count(content), value, stream, tags_cache);
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
struct TemplateSub {
    TemplateSub() = delete;

    TemplateSub(const Char_T *content, SizeT length, StringStream_T *stream, const Value_T *root_value,
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
    using IfTag           = Tags::IfTag;
    using TagBit          = Tags::TagBit;
    using QExpressions    = Array<QExpression>;
    using QOperation      = QExpression::QOperation;
    using ExpressionType  = QExpression::ExpressionType;
    using ExpressionValue = QExpression::ExpressionValue;
    using TagPatterns     = Tags::TagPatterns_T<Char_T>;

  public:
    void Render(const TagBit *tag, const TagBit *end) const {
        render(tag, end, SizeT{0}, _length);
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

        _stream->Write((_content + offset), (end_offset - offset));
    }

    void parse(Array<TagBit> &tags_cache, SizeT offset, const SizeT end_offset) const {
        while (offset < end_offset) {
            if (_content[offset] == TagPatterns::InLinePrefix) {
                SizeT current_offset = offset;
                ++current_offset;

                switch (_content[current_offset]) {
                    case TagPatterns::Var_2ND_Char: {
                        static constexpr const Char_T *var_prefix_p_2 = (TagPatterns::VariablePrefix + SizeT{2});
                        constexpr SizeT var_prefix_length_m_2         = (TagPatterns::VariablePrefixLength - SizeT{2});
                        constexpr SizeT var_prefix_length_m_1         = (TagPatterns::VariablePrefixLength - SizeT{1});
                        ++current_offset;

                        if (StringUtils::IsEqual(var_prefix_p_2, (_content + current_offset), var_prefix_length_m_2)) {
                            current_offset += var_prefix_length_m_1;
                            current_offset = Engine::FindOne<Char_T>(TagPatterns::InLineSuffix, _content,
                                                                     current_offset, end_offset, _length);

                            if (current_offset != SizeT{0}) {
                                const TagBit &tag = tags_cache.Insert(TagBit{TagType::Variable});

                                offset += TagPatterns::VariablePrefixLength;
                                parseVariableTag(offset, (current_offset - TagPatterns::InLineSuffixLength),
                                                 tag.GeVariableTag());
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

                        if (StringUtils::IsEqual(r_var_prefix_p_2, (_content + current_offset),
                                                 r_var_prefix_length_m_2)) {
                            current_offset += r_var_prefix_length_m_1;
                            current_offset = Engine::FindOne<Char_T>(TagPatterns::InLineSuffix, _content,
                                                                     current_offset, end_offset, _length);

                            if (current_offset != SizeT{0}) {
                                const TagBit &tag = tags_cache.Insert(TagBit{TagType::RawVariable});

                                offset += TagPatterns::RawVariablePrefixLength;
                                parseVariableTag(offset, (current_offset - TagPatterns::InLineSuffixLength),
                                                 tag.GeRawVariableTag());
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

                        if (StringUtils::IsEqual(math_prefix_p_2, (_content + current_offset),
                                                 math_prefix_length_m_2)) {
                            current_offset += math_prefix_length_m_2;
                            current_offset =
                                Engine::SkipInnerPatterns<Char_T>(TagPatterns::InLinePrefix, TagPatterns::InLineSuffix,
                                                                  _content, current_offset, end_offset, _length);

                            if (current_offset != SizeT{0}) {
                                const TagBit &tag = tags_cache.Insert(TagBit{TagType::Math});

                                parseMathTag(offset, current_offset, tag.GetMathTag());
                                offset = current_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TagPatterns::InlineIf_2ND_Char: {
                        static constexpr const Char_T *ii_if_prefix_p_2 = (TagPatterns::InLineIfPrefix + SizeT{2});
                        constexpr SizeT ii_if_prefix_length_m_2 = (TagPatterns::InLineIfPrefixLength - SizeT{2});
                        ++current_offset;

                        if (StringUtils::IsEqual(ii_if_prefix_p_2, (_content + current_offset),
                                                 ii_if_prefix_length_m_2)) {
                            current_offset += ii_if_prefix_length_m_2;
                            current_offset =
                                Engine::SkipInnerPatterns<Char_T>(TagPatterns::InLinePrefix, TagPatterns::InLineSuffix,
                                                                  _content, current_offset, end_offset, _length);

                            if (current_offset != SizeT{0}) {
                                const TagBit &tag = tags_cache.Insert(TagBit{TagType::InLineIf});

                                parseInLineIfTag(offset, current_offset, tag.GetInLineIfTag());
                                offset = current_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    default: {
                    }
                }
            } else if (_content[offset] == TagPatterns::MultiLinePrefix) { // <loop or <if
                SizeT current_offset = offset;
                ++current_offset;

                if (_content[current_offset] == TagPatterns::Loop_2ND_Char) { // <loop
                    static constexpr const Char_T *loop_prefix_p_2        = (TagPatterns::LoopPrefix + SizeT{2});
                    constexpr SizeT                loop_prefix_length_m_2 = (TagPatterns::LoopPrefixLength - SizeT{2});
                    ++current_offset;

                    if (StringUtils::IsEqual(loop_prefix_p_2, (_content + current_offset), loop_prefix_length_m_2)) {
                        current_offset += loop_prefix_length_m_2;
                        current_offset = Engine::SkipInnerPatterns<Char_T>(
                            TagPatterns::LoopPrefix, TagPatterns::LoopPrefixLength, TagPatterns::LoopSuffix,
                            TagPatterns::LoopSuffixLength, _content, current_offset, end_offset, _length);

                        if (current_offset != SizeT{0}) {
                            const TagBit &tag = tags_cache.Insert(TagBit{TagType::Loop});

                            parseLoopTag(offset, current_offset, tag.GetLoopTag());
                            offset = current_offset;
                            continue;
                        }
                    }
                } else if (_content[current_offset] == TagPatterns::If_2ND_Char) { // <if
                    static constexpr const Char_T *if_prefix_p_2        = (TagPatterns::IfPrefix + SizeT{2});
                    constexpr SizeT                if_prefix_length_m_2 = (TagPatterns::IfPrefixLength - SizeT{2});
                    ++current_offset;

                    if (StringUtils::IsEqual(if_prefix_p_2, (_content + current_offset), if_prefix_length_m_2)) {
                        current_offset += if_prefix_length_m_2;
                        current_offset = Engine::SkipInnerPatterns<Char_T>(
                            TagPatterns::IfPrefix, TagPatterns::IfPrefixLength, TagPatterns::IfSuffix,
                            TagPatterns::IfSuffixLength, _content, current_offset, end_offset, _length);

                        if (current_offset != SizeT{0}) {
                            const TagBit &tag = tags_cache.Insert(TagBit{TagType::If});

                            parseIfTag(offset, current_offset, tag.GetIfTag());
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
    void lightParse(Array<TagBit> &tags_cache, SizeT offset, const SizeT end_offset) const {
        while (true) {
            offset = Engine::FindOne<Char_T>(TagPatterns::InLinePrefix, _content, offset, end_offset, _length);

            if (offset != 0) {
                switch (_content[offset]) {
                    case TagPatterns::Var_2ND_Char: {
                        static constexpr const Char_T *var_prefix_p_2 = (TagPatterns::VariablePrefix + SizeT{2});
                        constexpr SizeT var_prefix_length_m_2         = (TagPatterns::VariablePrefixLength - SizeT{2});
                        constexpr SizeT var_prefix_length_m_1         = (TagPatterns::VariablePrefixLength - SizeT{1});
                        SizeT           current_offset                = offset;
                        ++current_offset;

                        if (StringUtils::IsEqual(var_prefix_p_2, (_content + current_offset), var_prefix_length_m_2)) {
                            current_offset += var_prefix_length_m_1;
                            current_offset = Engine::FindOne<Char_T>(TagPatterns::InLineSuffix, _content,
                                                                     current_offset, end_offset, _length);

                            const TagBit &tag = tags_cache.Insert(TagBit{TagType::Variable});

                            offset -= TagPatterns::InLinePrefixLength;
                            offset += TagPatterns::VariablePrefixLength;
                            parseVariableTag(offset, (current_offset - TagPatterns::InLineSuffixLength),
                                             tag.GeVariableTag());
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

                        if (StringUtils::IsEqual(r_var_prefix_p_2, (_content + current_offset),
                                                 r_var_prefix_length_m_2)) {
                            current_offset += r_var_prefix_length_m_1;
                            current_offset = Engine::FindOne<Char_T>(TagPatterns::InLineSuffix, _content,
                                                                     current_offset, end_offset, _length);

                            const TagBit &tag = tags_cache.Insert(TagBit{TagType::RawVariable});

                            offset -= TagPatterns::InLinePrefixLength;
                            offset += TagPatterns::RawVariablePrefixLength;
                            parseVariableTag(offset, (current_offset - TagPatterns::InLineSuffixLength),
                                             tag.GeRawVariableTag());
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

                        if (StringUtils::IsEqual(math_prefix_p_2, (_content + current_offset),
                                                 math_prefix_length_m_2)) {
                            current_offset += math_prefix_length_m_2;
                            current_offset =
                                Engine::SkipInnerPatterns<Char_T>(TagPatterns::InLinePrefix, TagPatterns::InLineSuffix,
                                                                  _content, current_offset, end_offset, _length);

                            const TagBit &tag = tags_cache.Insert(TagBit{TagType::Math});

                            offset -= TagPatterns::InLinePrefixLength;
                            parseMathTag(offset, current_offset, tag.GetMathTag());
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

        _stream->Write((_content + offset), (t_offset - offset));
        offset = t_offset;
        offset += length;

        const Value_T *value = getValue(tag);

        if ((value == nullptr) || !(value->CopyValueTo(*_stream, Config::TemplatePrecision, &escapeHTMLSpecialChars))) {
            if ((_loop_key != nullptr) && (tag.IsLoopValue == SizeT8{1})) {
                escapeHTMLSpecialChars(*_stream, _loop_key, _loop_key_length);
            } else {
                _stream->Write((_content + t_offset), length);
            }
        }
    }

    void renderRawVariable(const VariableTag &tag, SizeT &offset) const {
        const SizeT t_offset = (tag.Offset - TagPatterns::RawVariablePrefixLength);
        const SizeT length   = (tag.Length + TagPatterns::RawVariableFullLength);

        _stream->Write((_content + offset), (t_offset - offset));
        offset = t_offset;
        offset += length;

        const Value_T *value = getValue(tag);

        if ((value == nullptr) || !(value->CopyValueTo(*_stream, Config::TemplatePrecision))) {
            _stream->Write((_content + t_offset), length);
        }
    }

    void renderMath(const MathTag &tag, SizeT &offset) const {
        const QExpression *expr = tag.Expressions.First();
        QExpression        result;

        _stream->Write((_content + offset), (tag.Offset - offset));
        offset = tag.EndOffset;

        if (tag.Expressions.IsNotEmpty() && evaluate(result, expr, QOperation::NoOp)) {
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
            _stream->Write((_content + tag.Offset), (tag.EndOffset - tag.Offset));
        }
    }

    void renderLoop(const LoopTag &tag, SizeT &offset) const {
        Value_T        grouped_set;
        const Value_T *loop_set;

        _stream->Write((_content + offset), (tag.Offset - offset));
        offset = tag.EndOffset;
        offset += TagPatterns::LoopSuffixLength;

        // Set (Array|Object)
        if (tag.SetLength != 0) {
            loop_set = getValue((tag.Offset + tag.SetOffset), tag.SetLength, tag.SetLevel,
                                (tag.Options & LoopTagOptions::SetIsLoopValue));
        } else {
            loop_set = _value;
        }

        if (loop_set != nullptr) {
            // Group
            if (tag.GroupLength != 0) {
                if (!(loop_set->GroupBy(grouped_set, (_content + tag.Offset + tag.GroupOffset), tag.GroupLength))) {
                    return;
                }

                loop_set = &grouped_set;
            }

            // Sort
            if (tag.Options > 1) {
                if (tag.GroupLength == 0) {
                    grouped_set = *loop_set;
                    loop_set    = &grouped_set;
                }

                grouped_set.Sort((tag.Options & LoopTagOptions::SortAscend) == LoopTagOptions::SortAscend);
            }

            const SizeT16 level(_level + 1U);

            // Stage 4: Render
            TemplateSub loop_template{_content, _length, _stream, _value, this, level};
            loop_template._loop_value_offset = (tag.Offset + tag.ValueOffset);
            loop_template._loop_value_length = tag.ValueLength;

            const TagBit *s_tag = tag.SubTags.First();
            const TagBit *s_end = (s_tag + tag.SubTags.Size());

            const SizeT loop_size  = loop_set->Size();
            SizeT       loop_index = 0;

            if (loop_set->IsObject()) {
                while (loop_index < loop_size) {
                    loop_set->SetValueKeyLength(loop_index, loop_template._loop_value, loop_template._loop_key,
                                                loop_template._loop_key_length);

                    if (loop_template._loop_value != nullptr) {
                        loop_template.render(s_tag, s_end, tag.ContentOffset, tag.EndOffset);
                    }

                    ++loop_index;
                }
            } else {
                while (loop_index < loop_size) {
                    loop_template._loop_value = loop_set->GetValue(loop_index);

                    if (loop_template._loop_value != nullptr) {
                        loop_template.render(s_tag, s_end, tag.ContentOffset, tag.EndOffset);
                    }

                    ++loop_index;
                }
            }
        }
    }

    void renderInLineIf(const InLineIfTag &tag, SizeT &offset) const {
        const QExpression *expr = tag.Case.First();
        QExpression        result;

        _stream->Write((_content + offset), (tag.Offset - offset));
        offset = tag.Offset;
        offset += tag.Length;

        if (tag.Case.IsNotEmpty() && evaluate(result, expr, QOperation::NoOp)) {
            const TagBit *s_tag     = tag.SubTags.First();
            const TagBit *s_end     = (s_tag + tag.SubTags.Size());
            SizeT         true_size = 0;

            if (++expr != tag.Case.End()) {
                true_size = SizeT(expr->Number.Natural);
            }

            if (result > 0U) {
                if (tag.SubTags.Size() != true_size) {
                    s_end -= true_size;
                }

                const SizeT true_offset     = (tag.Offset + tag.TrueOffset);
                const SizeT true_end_offset = (true_offset + tag.TrueLength);

                render(s_tag, s_end, true_offset, true_end_offset);
            } else {
                s_tag += true_size;

                const SizeT false_offset     = (tag.Offset + tag.FalseOffset);
                const SizeT false_end_offset = (false_offset + tag.FalseLength);

                render(s_tag, s_end, false_offset, false_end_offset);
            }
        }
    }

    void renderIf(const IfTag &tag, SizeT &offset) const {
        const IfTagCase *item = tag.Cases.First();
        const IfTagCase *end  = (item + tag.Cases.Size());
        QExpression      result;

        _stream->Write((_content + offset), (tag.Offset - offset));
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

    void parseVariableTag(SizeT offset, SizeT end_offset, VariableTag &tag) const noexcept {
        tag.Offset = offset;
        tag.Length = SizeT16((end_offset - offset) & SizeT16{0xFF}); // Limit var length to 255 meter per second.

        if (_loop_value_length != 0) {
            const TemplateSub *temp = this;

            SizeT l_offset;
            SizeT length;
            SizeT level = _level;

            while (level > 0) {
                l_offset = temp->_loop_value_offset;
                length   = temp->_loop_value_length;

                if (StringUtils::IsEqual((_content + offset), (_content + l_offset), length)) {
                    tag.IsLoopValue = 1;
                    tag.Level       = SizeT16(level & SizeT16{0xFF});
                    break;
                }

                temp = temp->_parent;
                --level;
            }
        }
    }

    void parseMathTag(SizeT offset, SizeT end_offset, MathTag &tag) const {
        tag.Offset    = offset;
        tag.EndOffset = end_offset;

        offset += TagPatterns::MathPrefixLength;
        end_offset -= TagPatterns::InLineSuffixLength;

        tag.Expressions = parseExpressions(offset, end_offset);
    }

    /*
     * Gets everything between "..."
     */
    SizeT getQuotedValue(SizeT &offset, SizeT end_offset) const noexcept {
        offset = Engine::FindOne<Char_T>(TagPatterns::QuoteChar, _content, offset, end_offset, _length);

        if (offset != SizeT{0}) {
            end_offset = Engine::FindOne<Char_T>(TagPatterns::QuoteChar, _content, offset, end_offset, _length);
        }

        return end_offset;
    }

    void parseLoopTag(SizeT offset, SizeT end_offset, LoopTag &tag) const {
        const SizeT16 level = (_level + 1);
        const SizeT   loop_content_offset =
            Engine::FindOne<Char_T>(TagPatterns::MultiLineSuffix, _content, offset, end_offset, _length);

        TemplateSub loop_template{_content, _length, nullptr, nullptr, this, level};

        tag.Offset = offset;
        offset += TagPatterns::LoopPrefixLength;
        end_offset -= TagPatterns::LoopSuffixLength;
        tag.EndOffset     = end_offset;
        tag.ContentOffset = loop_content_offset;

        SizeT offset2     = getQuotedValue(offset, loop_content_offset);
        SizeT last_offset = offset;
        offset = SizeT(offset - SizeT{6}); // (=) plus (") plus (two chars) = 4 plus (the char before them) = 5

        while ((offset2 != SizeT{0}) && (offset < loop_content_offset)) {
            switch (_content[offset]) {
                case TagPatterns::SetChar: {
                    const SizeT set_offset = last_offset;
                    const SizeT set_length = SizeT((offset2 - SizeT{1}) - last_offset);

                    VariableTag set_var{};
                    parseVariableTag(set_offset, (set_offset + set_length), set_var);
                    tag.SetOffset = SizeT8(set_var.Offset - tag.Offset);
                    tag.SetLength = SizeT8(set_var.Length);
                    tag.SetLevel  = set_var.Level;
                    tag.Options |= set_var.IsLoopValue;

                    offset      = offset2;
                    offset2     = getQuotedValue(offset, loop_content_offset);
                    last_offset = offset;
                    offset -= SizeT{6};
                    break;
                }

                case TagPatterns::ValueChar: {
                    tag.ValueOffset                  = SizeT8(last_offset - tag.Offset);
                    tag.ValueLength                  = SizeT8((offset2 - SizeT{1}) - last_offset);
                    loop_template._loop_value_offset = last_offset;
                    loop_template._loop_value_length = tag.ValueLength;
                    offset                           = offset2;
                    offset2                          = getQuotedValue(offset, loop_content_offset);
                    last_offset                      = offset;
                    offset -= SizeT{6};
                    break;
                }

                case TagPatterns::SortChar: {
                    tag.Options |=
                        ((_content[last_offset] == 'a') ? LoopTagOptions::SortAscend : LoopTagOptions::SortDescend);
                    offset      = offset2;
                    offset2     = getQuotedValue(offset, loop_content_offset);
                    last_offset = offset;
                    offset -= SizeT{6};
                    break;
                }

                case TagPatterns::GroupChar: {
                    tag.GroupOffset = SizeT8(last_offset - tag.Offset);
                    tag.GroupLength = SizeT8((offset2 - SizeT{1}) - last_offset);
                    offset          = offset2;
                    offset2         = getQuotedValue(offset, loop_content_offset);
                    last_offset     = offset;
                    offset -= SizeT{6};
                    break;
                }

                default: {
                    ++offset;
                }
            }
        }

        loop_template.parse(tag.SubTags, loop_content_offset, end_offset);
    }

    void parseInLineIfTag(SizeT offset, SizeT end_offset, InLineIfTag &tag) const {
        tag.Offset = offset;
        tag.Length = SizeT16(end_offset - offset);

        offset += TagPatterns::InLineIfPrefixLength;
        end_offset -= TagPatterns::InLineSuffixLength;

        SizeT offset2          = getQuotedValue(offset, end_offset);
        SizeT last_offset      = offset;
        SizeT true_offset      = 0;
        SizeT false_offset     = 0;
        SizeT true_end_offset  = 0;
        SizeT false_end_offset = 0;

        offset = SizeT(offset - SizeT{5}); // (=) plus (") plus (two chars) = 4 plus (the char before them) = 5

        while ((offset2 != SizeT{0}) && (offset < end_offset)) {
            switch (_content[offset]) {
                case TagPatterns::CaseChar: {
                    tag.Case    = parseExpressions(last_offset, (offset2 - SizeT{1})); // 1 = (") at the end
                    offset      = offset2;
                    offset2     = getQuotedValue(offset, end_offset);
                    last_offset = offset;
                    offset      = SizeT(offset - SizeT{5});
                    break;
                }

                case TagPatterns::TrueChar: {
                    true_offset     = last_offset;
                    true_end_offset = (offset2 - SizeT{1});
                    offset          = offset2;
                    offset2         = getQuotedValue(offset, end_offset);
                    last_offset     = offset;
                    offset          = SizeT(offset - SizeT{5});
                    break;
                }

                case TagPatterns::FalseChar: {
                    false_offset     = last_offset;
                    false_end_offset = (offset2 - SizeT{1});
                    offset           = offset2;
                    offset2          = getQuotedValue(offset, end_offset);
                    last_offset      = offset;
                    offset           = SizeT(offset - SizeT{5});
                    break;
                }

                default: {
                    ++offset;
                }
            }
        }

        if (tag.Case.IsNotEmpty()) {
            if (true_offset != true_end_offset) {
                lightParse(tag.SubTags, true_offset, true_end_offset);
                QExpression &expr   = tag.Case.Insert(QExpression{});
                expr.Number.Natural = tag.SubTags.Size(); // To use one heap for true and false.

                tag.TrueOffset = SizeT16(true_offset - tag.Offset);
                tag.TrueLength = SizeT16(true_end_offset - true_offset);
            }

            if (false_offset != false_end_offset) {
                lightParse(tag.SubTags, false_offset, false_end_offset);

                tag.FalseOffset = SizeT16(false_offset - tag.Offset);
                tag.FalseLength = SizeT16(false_end_offset - false_offset);
            }
        }
    }

    void parseIfTag(SizeT offset, SizeT end_offset, IfTag &tag) const {
        tag.Offset    = offset;
        tag.EndOffset = end_offset;

        offset += TagPatterns::IfPrefixLength;
        end_offset -= TagPatterns::IfSuffixLength;

        Array<TagBit> sub_tags;
        SizeT         offset2 = getQuotedValue(offset, end_offset);
        SizeT         case_end_offset;
        SizeT         else_offset;
        SizeT         content_offset;

        while ((offset2 != SizeT{0}) && (offset < end_offset)) {
            case_end_offset = (offset2 - SizeT{1});
            content_offset =
                Engine::FindOne<Char_T>(TagPatterns::MultiLineSuffix, _content, offset2, end_offset, _length);

            if (content_offset != SizeT{0}) {
                else_offset = nextElse(content_offset, end_offset);

                if (else_offset == SizeT{0}) {
                    parse(sub_tags, content_offset, end_offset);
                    tag.Cases += IfTagCase{parseExpressions(offset, case_end_offset), Memory::Move(sub_tags),
                                           content_offset, end_offset};

                    break;
                }

                parse(sub_tags, content_offset, (else_offset - TagPatterns::ElsePrefixLength));
                tag.Cases += IfTagCase{parseExpressions(offset, case_end_offset), Memory::Move(sub_tags),
                                       content_offset, SizeT(else_offset - TagPatterns::ElsePrefixLength)};

                if ((_content[else_offset] != TagPatterns::ElseIfChar)) {
                    else_offset = Engine::FindOne<Char_T>(TagPatterns::MultiLineSuffix, _content, else_offset,
                                                          end_offset, _length);

                    if (else_offset != SizeT{0}) {
                        parse(sub_tags, else_offset, end_offset);
                        tag.Cases += IfTagCase{QExpressions{}, Memory::Move(sub_tags), else_offset,
                                               end_offset}; // else without if
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
            else_offset = Engine::Find<Char_T>(TagPatterns::ElsePrefix, TagPatterns::ElsePrefixLength, _content, offset,
                                               end_offset, _length);

            if (else_offset == SizeT{0}) {
                // No <else.
                break;
            }

            const SizeT next_if = Engine::Find<Char_T>(TagPatterns::IfPrefix, TagPatterns::IfPrefixLength, _content,
                                                       offset, end_offset, _length);

            if ((next_if == SizeT{0}) || (else_offset < next_if)) {
                // No nesting <ifs or <else before a sub-if.
                break;
            }

            offset = Engine::Find<Char_T>(TagPatterns::IfSuffix, TagPatterns::IfSuffixLength, _content, next_if,
                                          end_offset, _length);

            if (else_offset > offset) {
                // <else is after a sub-if.
                break;
            }
        }

        return else_offset;
    }

    inline const Value_T *getValue(const VariableTag &variable) const noexcept {
        return getValue(variable.Offset, variable.Length, variable.Level, variable.IsLoopValue);
    }

    const Value_T *getValue(SizeT v_offset, SizeT16 v_length, SizeT8 v_level, SizeT8 v_is_loop_value) const noexcept {
        const Value_T *value     = nullptr;
        const Char_T  *id        = (_content + v_offset);
        const SizeT    length    = v_length;
        SizeT          offset    = 0;
        const bool     has_index = (id[(length - SizeT{1})] == TagPatterns::VariableIndexSuffix);

        if (v_is_loop_value != SizeT8{1}) {
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
                    const Value_T *val = getValue(expr->Variable);

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
        QNumber64      val_number;
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
        using QOperationSymbol = QOperationSymbol_T<Char_T>;

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
                        parseVariableTag(offset, end_offset, expr.Variable);
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
        using QOperationSymbol = QOperationSymbol_T<Char_T>;

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
                    offset = Engine::SkipInnerPatterns<Char_T>(QOperationSymbol::ParenthesesStart,
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
                        Engine::FindOne<Char_T>(QOperationSymbol::BracketEnd, _content, offset, end_offset, _length);

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
                    const Char_T ch = _content[offset];
                    return ((ch < QOperationSymbol::ColonChar) && (ch > QOperationSymbol::SlashChar));
                }
            }
        }

        return false;
    }

    const Char_T      *_content;
    StringStream_T    *_stream;
    const Value_T     *_value;
    const TemplateSub *_parent;
    const Value_T     *_loop_value{nullptr};
    const Char_T      *_loop_key{nullptr};
    SizeT              _loop_key_length{0};
    SizeT              _loop_value_offset{0};
    const SizeT        _length;
    SizeT16            _loop_value_length{0};
    const SizeT16      _level;
};

} // namespace Qentem

#endif
