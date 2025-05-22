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

#include "Finder.hpp"
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
 *      - var|e|n: variable, Equation or Number.
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
 *      - case, var|e|n: variable, Equation or Number.
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
 *      - var|e|n: variable, Equation or Number.
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
 * {svar:welcome_X_to_Y_its_Z_in_C, {var:name}, {var:site}, {math: 10+3}, {raw:city}}
 *
 * welcome_x_to_y: "Welcome {0} to {1}."
 * welcome_X_to_Y_its_Z_in_C: "Welcome {0} to {1}. its {2}:am in your {3}."
 *
 * {svar:hello_x_your_name_was_y_now_x_y_is_better_x, {var:name2}, {var:name1}}
 *
 * hello_x_your_name_was_y_now_x_y_is_better_x: "Hello {1}. Your name was {0}, now it's {1}, {1} is better than {0}."
 *
 * Note: sub variables can be used in any order at any time.
 */

/*
 * Inline If Tag:
 *
 * NOTE: single or double quotes.
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
 * NOTE: single or double quotes.
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
 * NOTE: 'case' can have single or double quotes.
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
struct QOperationSymbol_T;

struct Template {
    Template()                            = delete;
    Template(Template &&)                 = delete;
    Template(const Template &)            = delete;
    Template &operator=(Template &&)      = delete;
    Template &operator=(const Template &) = delete;
    ~Template()                           = delete;

    // template <typename Char_T, typename Value_T, typename StringStream_T>
    // inline static void CachedRender(const StringView<Char_T> &content, const Value_T &value, StringStream_T &stream,
    //                                 const StringView<Char_T> &template_name) {
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
    TemplateCore() = delete;

    TemplateCore(const Char_T *content, SizeT length) noexcept : content_{content}, length_{length} {
    }

  private:
    using TagType          = Tags::TagType;
    using VariableTag      = Tags::VariableTag;
    using MathTag          = Tags::MathTag;
    using SuperVariableTag = Tags::SuperVariableTag;
    using InLineIfTag      = Tags::InLineIfTag;
    using LoopTag          = Tags::LoopTag;
    using LoopTagOptions   = Tags::LoopTagOptions;
    using IfTagCase        = Tags::IfTagCase;
    using IfTag            = Tags::IfTag;
    using TagBit           = Tags::TagBit;
    using QExpressions     = Array<QExpression>;
    using QOperation       = QExpression::QOperation;
    using ExpressionType   = QExpression::ExpressionType;
    using TagPatterns      = Tags::TagPatterns_T<Char_T>;

    struct LoopItem {
        const Value_T     *Value{nullptr};
        StringView<Char_T> Key{};
    };

  public:
    inline void Parse(Array<TagBit> &tags_cache) const {
        parse(content_, length_, tags_cache);
    }

    inline static void Parse(const Char_T *content, const SizeT length, Array<TagBit> &tags_cache) {
        parse(content, length, tags_cache);
    }

    void Render(const Array<Tags::TagBit> &tags_cache, const Value_T &value, StringStream_T &stream) {
        Array<LoopItem> loops_items{};

        value_       = &value;
        stream_      = &stream;
        loops_items_ = &loops_items;

        render(tags_cache.First(), tags_cache.End(), 0, length_);
    }

    inline bool Evaluate(QExpression &number, const QExpressions &exprs, const Value_T &value) noexcept {
        const QExpression *expr = exprs.First();
        value_                  = &value;

        if (expr != nullptr) {
            return evaluate(number, expr, QOperation::NoOp);
        }

        return false;
    }

    inline static QExpressions ParseExpressions(const Char_T *content, SizeT length) {
        return parseExpressions(content, 0, length, nullptr);
    }

  private:
    static void parse(const Char_T *content, SizeT length, Array<TagBit> &tags_cache) {
        Finder<Tags::List<Char_T>, Char_T, SizeT> finder{content, length};

        Array<Array<TagBit> *> parent_storage{SizeT{8}};
        Array<TagBit>         *storage{&tags_cache};
        const LoopTag         *loop_tag{nullptr};

        SizeT32 match;
        bool    is_child{false};

        finder.NextSegment();

        while ((match = finder.CurrentMatch()) != 0U) {
            switch (match) {
                case TagPatterns::LineEndID: {
                    if (is_child && parent_storage.IsNotEmpty()) {
                        is_child = false;
                        storage  = *(parent_storage.Last());
                        parent_storage.Drop(SizeT{1});

                        TagBit *tag_bit = storage->Last();

                        switch (tag_bit->GetType()) {
                            case TagType::SuperVariable: {
                                SuperVariableTag &tag = tag_bit->GetSuperVariableTag();
                                tag.EndOffset         = finder.GetOffset();
                                break;
                            }

                            case TagType::InLineIf: {
                                InLineIfTag &tag        = tag_bit->GetInLineIfTag();
                                const SizeT  end_offset = finder.GetOffset();
                                const SizeT  true_offset =
                                    tag.TrueOffset; // See the end of 'case TagPatterns::InLineIfID:'

                                SizeT offset = tag.Offset;
                                offset += true_offset;
                                tag.TrueOffset = 0;

                                bool is_true{false};
                                tag.Length = SizeT16(end_offset - tag.Offset);

                                do {
                                    while ((offset < end_offset) && (content[offset] == TagPatterns::SpaceChar)) {
                                        ++offset;
                                    }

                                    if (offset < end_offset) {
                                        if (content[offset] == TagPatterns::TrueChar) {
                                            if (((end_offset - offset) > TagPatterns::TrueLength) &&
                                                StringUtils::IsEqual((content + offset), TagPatterns::True,
                                                                     TagPatterns::TrueLength)) {
                                                offset += TagPatterns::TrueLength;
                                                is_true = true;
                                            }
                                        } else if ((content[offset] == TagPatterns::FalseChar) &&
                                                   ((end_offset - offset) > TagPatterns::FalseLength) &&
                                                   StringUtils::IsEqual((content + offset), TagPatterns::False,
                                                                        TagPatterns::FalseLength)) {
                                            offset += TagPatterns::FalseLength;
                                        } else {
                                            break;
                                        }

                                        while ((offset < end_offset) && (content[offset] != TagPatterns::EqualChar)) {
                                            ++offset;
                                        }

                                        do {
                                            ++offset;
                                        } while ((offset < end_offset) && (content[offset] == TagPatterns::SpaceChar));

                                        if (offset < end_offset) {
                                            const Char_T quote_char = content[offset];
                                            ++offset;
                                            const SizeT att_offset = offset;

                                            while ((offset < end_offset) && (content[offset] != quote_char)) {
                                                ++offset;
                                            }

                                            if (offset < end_offset) {
                                                if (is_true) {
                                                    is_true        = false;
                                                    tag.TrueOffset = SizeT16(att_offset - tag.Offset);
                                                    tag.TrueLength = SizeT16(offset - att_offset);
                                                } else {
                                                    tag.FalseOffset = SizeT16(att_offset - tag.Offset);
                                                    tag.FalseLength = SizeT16(offset - att_offset);
                                                }

                                                continue;
                                            }

                                            // Found '}' inside 'True' or 'False'.
                                            is_child = true;
                                            parent_storage += storage;
                                            storage        = &(tag.SubTags);
                                            tag.TrueOffset = SizeT16(true_offset);
                                            break;
                                        }
                                    }
                                } while (++offset < end_offset);

                                // Set StartID
                                if ((tag.TrueOffset != SizeT16{0}) || (tag.FalseOffset != SizeT16{0})) {
                                    const TagBit *s_tag     = tag.SubTags.First();
                                    const TagBit *s_tag_end = tag.SubTags.End();
                                    SizeT32       id{0};
                                    const SizeT   first_offset =
                                        (SizeT((tag.TrueOffset < tag.FalseOffset) ? tag.FalseOffset : tag.TrueOffset) +
                                         tag.Offset);

                                    bool skip = false;

                                    while (s_tag < s_tag_end) {
                                        switch (s_tag->GetType()) {
                                            case TagType::Variable:
                                            case TagType::RawVariable: {
                                                offset = s_tag->GetVariableTag().Offset;
                                                break;
                                            }

                                            case TagType::Math: {
                                                offset = s_tag->GetMathTag().Offset;
                                                break;
                                            }

                                            default: {
                                                storage->Drop(SizeT{1});
                                                s_tag = s_tag_end;
                                                skip  = true;
                                            }
                                        }

                                        if (offset >= first_offset) {
                                            break;
                                        }

                                        ++id;
                                        ++s_tag;
                                    }

                                    if (!skip) {
                                        if (tag.TrueOffset < tag.FalseOffset) {
                                            tag.FalseTagsStartID = SizeT8(id);
                                        } else {
                                            tag.TrueTagsStartID = SizeT8(id);
                                        }
                                    }
                                } else if ((tag.TrueOffset == SizeT16{0}) && (tag.FalseOffset == SizeT16{0})) {
                                    storage->Drop(SizeT{1});
                                }

                                break;
                            }

                            default: {
                            }
                        }
                    }

                    finder.NextSegment();
                    break;
                }

                case TagPatterns::VariableID: {
                    const SizeT offset = finder.GetOffset();

                    finder.NextSegment();

                    if (finder.CurrentMatch() == TagPatterns::LineEndID) {
                        const SizeT var_length = (((finder.GetOffset() - offset) - TagPatterns::InLineSuffixLength)
                                                  // Limit var length to 255 meter per second.;
                                                  & SizeT{0xFF});

                        if (var_length != 0) {
                            VariableTag *tag = (storage->Insert(TagBit{})).MakeVariableTag();
                            tag->Offset      = offset;
                            tag->Length      = SizeT16(var_length);

                            checkLoopVariable(content, *tag, loop_tag);
                        }

                        finder.NextSegment();
                    }

                    break;
                }

                case TagPatterns::RawVariableID: {
                    const SizeT offset = finder.GetOffset();

                    finder.NextSegment();

                    if (finder.CurrentMatch() == TagPatterns::LineEndID) {
                        const SizeT var_length = (((finder.GetOffset() - offset) - TagPatterns::InLineSuffixLength)
                                                  // Limit var length to 255 meter per second.;
                                                  & SizeT{0xFF});

                        if (var_length != 0) {
                            VariableTag *tag = (storage->Insert(TagBit{})).MakeRawVariableTag();
                            tag->Offset      = offset;
                            tag->Length      = SizeT16(var_length);

                            checkLoopVariable(content, *tag, loop_tag);
                        }

                        finder.NextSegment();
                    }

                    break;
                }

                case TagPatterns::MathID: {
                    const SizeT offset = finder.GetOffset();
                    SizeT       end_offset{0};
                    SizeT32     skip_var{0};

                    finder.NextSegment();

                    while (true) {
                        match = finder.CurrentMatch();

                        if ((match < TagPatterns::MathID) && (match != TagPatterns::LineEndID)) {
                            finder.NextSegment();
                            match = finder.CurrentMatch();
                            ++skip_var;
                        }

                        if (match == TagPatterns::LineEndID) {
                            if (skip_var != 0U) {
                                finder.NextSegment();
                                --skip_var;
                                continue;
                            }

                            end_offset = finder.GetOffset();
                            finder.NextSegment();
                        }

                        break;
                    }

                    if (match != 0) {
                        MathTag *tag   = (storage->Insert(TagBit{})).MakeMathTag();
                        tag->Offset    = (offset - TagPatterns::MathPrefixLength);
                        tag->EndOffset = end_offset;
                        tag->Expressions =
                            parseExpressions(content, offset, (end_offset - TagPatterns::InLineSuffixLength), loop_tag);
                    }

                    break;
                }

                case TagPatterns::SuperVariableID: {
                    SizeT       offset         = finder.GetOffset();
                    const SizeT svar_id_offset = offset;
                    const SizeT svar_offset    = (offset - TagPatterns::SuperVariablePrefixLength);

                    finder.NextSegment();
                    const SizeT end_offset = finder.GetOffset();

                    while ((offset < end_offset) && (content[offset] != TagPatterns::VariablesSeparatorChar)) {
                        ++offset;
                    }

                    const SizeT16 var_length = SizeT16((offset - svar_id_offset)
                                                       // Limit var length to 255 meter per second.;
                                                       & SizeT(0xFF));

                    if (var_length != SizeT16{0}) {
                        SuperVariableTag *tag = (storage->Insert(TagBit{})).MakeSuperVariableTag();
                        tag->Offset           = svar_offset;
                        tag->Variable.Offset  = svar_id_offset;
                        tag->Variable.Length  = var_length;

                        is_child = true;
                        parent_storage += storage;
                        storage = &(tag->SubTags);
                    }

                    break;
                }

                case TagPatterns::InLineIfID: {
                    SizeT       offset     = finder.GetOffset();
                    const SizeT iif_offset = (offset - TagPatterns::InLineIfPrefixLength);

                    finder.NextSegment();
                    SizeT end_offset = finder.GetOffset();

                    while ((offset < end_offset) && (content[offset] == TagPatterns::SpaceChar)) {
                        ++offset;
                    }

                    if ((offset < end_offset) && ((end_offset - offset) > TagPatterns::CaseLength) &&
                        StringUtils::IsEqual((content + offset), TagPatterns::Case, TagPatterns::CaseLength)) {
                        offset += TagPatterns::CaseLength;

                        while ((offset < end_offset) && (content[offset] != TagPatterns::EqualChar)) {
                            ++offset;
                        }

                        do {
                            ++offset;
                        } while ((offset < end_offset) && (content[offset] == TagPatterns::SpaceChar));

                        if (offset < end_offset) {
                            const Char_T quote_char = content[offset];

                            ++offset;

                            const SizeT case_offset = offset;

                            while ((match = finder.CurrentMatch()) != 0U) {
                                while ((offset < end_offset) && (content[offset] != quote_char)) {
                                    ++offset;
                                }

                                if (offset < end_offset) {
                                    break;
                                }

                                finder.NextSegment();
                                match = finder.CurrentMatch();

                                if (match == TagPatterns::LineEndID) {
                                    finder.NextSegment();
                                    end_offset = finder.GetOffset();
                                    continue;
                                }

                                break;
                            }

                            if (match != 0) {
                                InLineIfTag *tag = (storage->Insert(TagBit{})).MakeInLineIfTag();
                                tag->Offset      = iif_offset;
                                tag->Case        = parseExpressions(content, case_offset, offset, loop_tag);

                                ++offset;
                                tag->TrueOffset =
                                    SizeT16(offset - iif_offset); // Not the actual offset but close enough.

                                is_child = true;
                                parent_storage += storage;
                                storage = &(tag->SubTags);
                            }
                        }
                    }

                    break;
                }

                case TagPatterns::LoopID: {
                    SizeT       offset      = finder.GetOffset();
                    const SizeT loop_offset = (offset - TagPatterns::LoopPrefixLength);

                    finder.NextSegment();
                    const SizeT end_offset = finder.GetOffset();

                    while ((offset < end_offset) && (content[offset] != TagPatterns::MultiLineLastChar)) {
                        ++offset;
                    }

                    if (offset < end_offset) {
                        LoopTag *tag = (storage->Insert(TagBit{})).MakeLoopTag();
                        tag->Offset  = loop_offset;
                        tag->Parent  = loop_tag;
                        tag->Level   = SizeT8(parent_storage.Size());
                        loop_tag     = tag;

                        parseLoopAttributes(content, offset, *tag);

                        offset += TagPatterns::MultiLineSuffixLength;
                        tag->ContentOffset = SizeT16(offset - loop_offset);

                        parent_storage += storage;
                        storage = &(tag->SubTags);
                    }

                    break;
                }

                case TagPatterns::LoopEndID: {
                    if ((loop_tag != nullptr) && parent_storage.IsNotEmpty()) {
                        storage = *(parent_storage.Last());
                        parent_storage.Drop(SizeT{1});

                        LoopTag &tag  = storage->Last()->GetLoopTag();
                        tag.EndOffset = (finder.GetOffset() - TagPatterns::LoopSuffixLength);
                        loop_tag      = tag.Parent;
                    }

                    finder.NextSegment();
                    break;
                }

                case TagPatterns::IfID: {
                    SizeT       offset    = finder.GetOffset();
                    const SizeT if_offset = (offset - TagPatterns::IfPrefixLength);
                    SizeT       case_offset{0};
                    SizeT       case_end_offset{0};

                    parseIfCase(content, offset, length, case_offset, case_end_offset);

                    finder.SetOffset(offset);

                    if (offset < length) {
                        IfTag *tag  = (storage->Insert(TagBit{})).MakeIfTag();
                        tag->Offset = if_offset;

                        IfTagCase &if_case = tag->Cases.Insert(IfTagCase{});
                        if_case.Offset     = offset;
                        if_case.Case       = parseExpressions(content, case_offset, case_end_offset, loop_tag);

                        parent_storage += storage;
                        storage = &(if_case.SubTags);
                    }

                    finder.NextSegment();
                    break;
                }

                case TagPatterns::IfEndID: {
                    if (parent_storage.IsNotEmpty()) {
                        Array<TagBit> *tmp     = *(parent_storage.Last());
                        TagBit        *tag_bit = tmp->Last();

                        if (tag_bit->GetType() == TagType::If) {
                            const SizeT offset = finder.GetOffset();
                            IfTag      &tag    = tag_bit->GetIfTag();

                            tag.Cases.Last()->EndOffset = (offset - TagPatterns::IfSuffixLength);
                            tag.EndOffset               = offset;

                            storage = tmp;
                            parent_storage.Drop(SizeT{1});
                        }
                    }

                    finder.NextSegment();
                    break;
                }

                case TagPatterns::ElseID: {
                    if (parent_storage.IsNotEmpty()) {
                        Array<TagBit> *tmp     = *(parent_storage.Last());
                        TagBit        *tag_bit = tmp->Last();

                        if (tag_bit->GetType() == TagType::If) {
                            IfTag &tag        = tag_bit->GetIfTag();
                            SizeT  offset     = finder.GetOffset();
                            bool   is_if_else = false;

                            tag.Cases.Last()->EndOffset = (offset - TagPatterns::ElsePrefixLength);

                            IfTagCase &if_case = tag.Cases.Insert(IfTagCase{});

                            while ((offset < length) && (content[offset] != TagPatterns::MultiLineLastChar)) {
                                if (content[offset] == TagPatterns::IfPrefix[0]) {
                                    offset += TagPatterns::IfAfterElseLength;
                                    is_if_else = true;
                                    break;
                                }

                                ++offset;
                            }

                            if (is_if_else) {
                                SizeT case_offset{0};
                                SizeT case_end_offset{0};

                                parseIfCase(content, offset, length, case_offset, case_end_offset);
                                finder.SetOffset(offset);
                                finder.NextSegment();

                                if ((offset < length) && (case_end_offset != 0)) {
                                    if_case.Offset = offset;
                                    if_case.Case   = parseExpressions(content, case_offset, case_end_offset, loop_tag);

                                    storage = &(if_case.SubTags);
                                    break;
                                }
                            } else if (offset < length) {
                                ++offset;
                                if_case.Offset = offset;
                                storage        = &(if_case.SubTags);
                                finder.SetOffset(offset);
                                finder.NextSegment();
                                break;
                            }

                            // Bad else
                            storage = tmp;
                            parent_storage.Drop(SizeT{1});
                            storage->Drop(SizeT{1});
                        }
                    }

                    finder.NextSegment();
                    break;
                }

                default: {
                }
            }
        }

        while (parent_storage.Size() != 0) {
            storage = *(parent_storage.Last());
            storage->Drop(SizeT{1});
            parent_storage.Drop(SizeT{1});
        }
    }

    inline static void checkLoopVariable(const Char_T *content, VariableTag &tag, const LoopTag *loop_tag) noexcept {
        const Char_T *var = (content + tag.Offset);

        while (loop_tag != nullptr) {
            if (StringUtils::IsEqual(var, (content + (loop_tag->Offset + loop_tag->ValueOffset)),
                                     loop_tag->ValueLength)) {
                tag.IDLength = loop_tag->ValueLength;
                tag.Level    = loop_tag->Level;

                break;
            }

            loop_tag = loop_tag->Parent;
        }
    }

    inline static void parseLoopAttributes(const Char_T *content, const SizeT end_offset, LoopTag &tag) noexcept {
        enum struct LoopAttributes : SizeT8 { None = 0, Set, Value, Sort, Group };
        SizeT offset = (tag.Offset + TagPatterns::LoopPrefixLength);

        LoopAttributes att_type = LoopAttributes::None;

        do {
            while ((offset < end_offset) && (content[offset] == TagPatterns::SpaceChar)) {
                ++offset;
            }

            if (offset < end_offset) {
                switch (content[offset]) {
                    case TagPatterns::SetSortChar: {
                        const SizeT tmp_length = (end_offset - offset);

                        if ((tmp_length > TagPatterns::SetLength) &&
                            StringUtils::IsEqual((content + offset), TagPatterns::Set, TagPatterns::SetLength)) {
                            offset += TagPatterns::SetLength;
                            att_type = LoopAttributes::Set;
                            break;
                        }

                        if ((tmp_length > TagPatterns::SortLength) &&
                            StringUtils::IsEqual((content + offset), TagPatterns::Sort, TagPatterns::SortLength)) {
                            offset += TagPatterns::SortLength;
                            att_type = LoopAttributes::Sort;
                            break;
                        }

                        ++offset;
                        break;
                    }

                    case TagPatterns::ValueChar: {
                        if (((end_offset - offset) > TagPatterns::ValueLength) &&
                            StringUtils::IsEqual((content + offset), TagPatterns::Value, TagPatterns::ValueLength)) {
                            offset += TagPatterns::ValueLength;
                            att_type = LoopAttributes::Value;
                            break;
                        }

                        ++offset;
                        break;
                    }

                    case TagPatterns::GroupChar: {
                        if (((end_offset - offset) > TagPatterns::GroupLength) &&
                            StringUtils::IsEqual((content + offset), TagPatterns::Group, TagPatterns::GroupLength)) {
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

            while ((offset < end_offset) && (content[offset] != TagPatterns::EqualChar)) {
                ++offset;
            }

            do {
                ++offset;
            } while ((offset < end_offset) && (content[offset] == TagPatterns::SpaceChar));

            if (offset < end_offset) {
                const SizeT  att_offset = (offset + SizeT{1});
                const Char_T quote_char = content[offset];

                do {
                    ++offset;
                } while ((offset < end_offset) && (content[offset] != quote_char));

                switch (att_type) {
                    case LoopAttributes::Set: {
                        tag.Set.Offset = att_offset;
                        tag.Set.Length = SizeT16(offset - att_offset);
                        checkLoopVariable(content, tag.Set, tag.Parent);
                        break;
                    }

                    case LoopAttributes::Value: {
                        tag.ValueOffset = SizeT8(att_offset - tag.Offset);
                        tag.ValueLength = SizeT8(offset - att_offset);
                        break;
                    }

                    case LoopAttributes::Sort: {
                        tag.Options |=
                            ((content[att_offset] == 'a') ? LoopTagOptions::SortAscend : LoopTagOptions::SortDescend);
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
        } while (offset < end_offset);
    }

    inline static void parseIfCase(const Char_T *content, SizeT &offset, const SizeT end_offset, SizeT &case_offset,
                                   SizeT &case_end_offset) noexcept {
        while ((offset < end_offset) && (content[offset] == TagPatterns::SpaceChar)) {
            ++offset;
        }

        if ((offset < end_offset) && ((end_offset - offset) > TagPatterns::CaseLength) &&
            StringUtils::IsEqual((content + offset), TagPatterns::Case, TagPatterns::CaseLength)) {
            offset += TagPatterns::CaseLength;

            while ((offset < end_offset) && (content[offset] != TagPatterns::EqualChar)) {
                ++offset;
            }

            do {
                ++offset;
            } while ((offset < end_offset) && (content[offset] == TagPatterns::SpaceChar));

            if (offset < end_offset) {
                const Char_T quote_char = content[offset];

                ++offset;

                case_offset = offset;

                while ((offset < end_offset) && (content[offset] != quote_char)) {
                    ++offset;
                }

                case_end_offset = offset;

                while ((offset < end_offset) && (content[offset] != TagPatterns::MultiLineLastChar)) {
                    ++offset;
                }

                ++offset;
            }
        }
    }

    // Render
    void render(const TagBit *tag, const TagBit *end, SizeT offset, SizeT end_offset) const {
        while (tag < end) {
            switch (tag->GetType()) {
                case TagType::Variable: {
                    renderVariable(tag->GetVariableTag(), offset);
                    break;
                }

                case TagType::RawVariable: {
                    renderRawVariable(tag->GetVariableTag(), offset);
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

                case TagType::InLineIf: {
                    renderInLineIf(tag->GetInLineIfTag(), offset);
                    break;
                }

                case TagType::Loop: {
                    renderLoop(tag->GetLoopTag(), offset);
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

    void renderVariable(const VariableTag &tag, SizeT &offset) const {
        const SizeT t_offset = (tag.Offset - TagPatterns::VariablePrefixLength);
        const SizeT length   = (tag.Length + TagPatterns::VariableFullLength);

        stream_->Write((content_ + offset), (t_offset - offset));
        offset = t_offset;
        offset += length;

        const Value_T *value = getValue(tag);

        if ((value == nullptr) ||
            !(value->CopyValueTo(*stream_, {Config::TemplatePrecision, QENTEM_TEMPLATE_DOUBLE_FORMAT},
                                 &(StringUtils::EscapeHTMLSpecialChars<StringStream_T, Char_T>)))) {
            if (tag.IDLength != SizeT8{0}) {
                const StringView<Char_T> &key = loops_items_->Storage()[tag.Level].Key;

                if (key.Length() != 0) {
                    StringUtils::EscapeHTMLSpecialChars(*stream_, key.First(), key.Length());
                    return;
                }
            }

            StringUtils::EscapeHTMLSpecialChars(*stream_, (content_ + t_offset), length);
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
                if (content[index] == TagPatterns::InLineFirstChar) {
                    const SizeT start = index;

                    StringUtils::EscapeHTMLSpecialChars(*stream_, (content + last_index), (start - last_index));
                    last_index = start;
                    ++index;

                    if (index < length) {
                        const SizeT id = SizeT(content[index] - DigitUtils::DigitChar::Zero);
                        ++index;

                        if ((index < length) && (content[index] == TagPatterns::InLineLastChar)) {
                            ++index;

                            if (id < tag.SubTags.Size()) {
                                const TagBit *sub_tag = (tag.SubTags.First() + id);
                                last_index            = index;

                                switch (sub_tag->GetType()) {
                                    case TagType::Variable: {
                                        const VariableTag &var = sub_tag->GetVariableTag();
                                        SizeT var_offset       = (var.Offset - TagPatterns::VariablePrefixLength);
                                        renderVariable(var, var_offset);
                                        break;
                                    }

                                    case TagType::RawVariable: {
                                        const VariableTag &r_var = sub_tag->GetVariableTag();
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

            StringUtils::EscapeHTMLSpecialChars(*stream_, (content + last_index), (index - last_index));
        } else {
            stream_->Write((content_ + tag.Offset), (tag.EndOffset - tag.Offset));
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
            const TagBit *s_end{nullptr};
            SizeT         value_offset;
            SizeT         value_end_offset;

            if (result > 0U) {
                if (tag.TrueOffset < tag.FalseOffset) {
                    s_end = (s_tag + tag.FalseTagsStartID);
                } else {
                    s_end = tag.SubTags.End();
                    s_tag += tag.TrueTagsStartID;
                }

                value_offset     = (tag.Offset + tag.TrueOffset);
                value_end_offset = (value_offset + tag.TrueLength);
            } else {
                if (tag.FalseOffset < tag.TrueOffset) {
                    s_end = (s_tag + tag.TrueTagsStartID);
                } else {
                    s_end = tag.SubTags.End();
                    s_tag += tag.FalseTagsStartID;
                }

                value_offset     = (tag.Offset + tag.FalseOffset);
                value_end_offset = (value_offset + tag.FalseLength);
            }

            render(s_tag, s_end, value_offset, value_end_offset);
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

    void renderIf(const IfTag &tag, SizeT &offset) const {
        const IfTagCase *item = tag.Cases.First();
        const IfTagCase *end  = tag.Cases.End();
        QExpression      result;

        stream_->Write((content_ + offset), (tag.Offset - offset));
        offset = tag.EndOffset;

        if ((item != nullptr) && item->Case.IsNotEmpty()) { // First case should not be empty
            do {
                // <else> without if = (item->Case == nothing)
                const QExpression *expr = item->Case.First();

                if (item->Case.IsEmpty() || (evaluate(result, expr, QOperation::NoOp) && (result > 0U))) {
                    render(item->SubTags.First(), item->SubTags.End(), item->Offset, item->EndOffset);
                    break;
                }

                ++item;
            } while (item < end);
        }
    }

    inline const Value_T *getValue(const VariableTag &variable) const noexcept {
        const Value_T *value     = nullptr;
        const Char_T  *id        = (content_ + variable.Offset);
        const SizeT    length    = variable.Length;
        SizeT          offset    = 0;
        const bool     has_index = ((length != 0) && (id[(length - SizeT{1})] == TagPatterns::VariableIndexSuffix));

        if (variable.IDLength == SizeT8{0}) {
            if (!has_index) {
                return value_->GetValue(id, length);
            }

            while ((offset < length) && (id[offset] != TagPatterns::VariableIndexPrefix)) {
                ++offset;
            }

            if (offset != 0) {
                // {var:abc[...]}
                // if offset == 0 then it's {var:[...]}
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
                            }
                        }
                    }

                    // If (case="...") contains only a variable, (true="...") content will be printed
                    // only if the value is a string and the length of that string is not zero.
                    // This will make it easy to use something like:
                    //  {if case="value[some_string]" true=", value[some_string]"} or
                    // <if case="value[some_string]"><span>value[some_string]-value[another_string]</span></if>
                    if ((operation == QOperation::NoOp) && (expr->Operation == QOperation::NoOp)) {
                        result.Value.Number = SizeT64((val != nullptr) && val->IsString() && (val->Length() != 0));
                        result.Type         = ExpressionType::NaturalNumber;
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

    inline static QExpressions parseExpressions(const Char_T *content, SizeT offset, const SizeT end_offset,
                                                const LoopTag *loop_tag) {
        QExpressions exprs;
        QOperation   last_oper = QOperation::NoOp;

        while (offset < end_offset) {
            const SizeT      num_offset = offset;
            const QOperation oper       = getOperation(content, offset, end_offset);

            if ((oper != QOperation::Error) &&
                parseValue(exprs, oper, last_oper, content, num_offset, offset, loop_tag)) {
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

    static bool parseValue(QExpressions &exprs, const QOperation oper, const QOperation last_oper,
                           const Char_T *content, SizeT offset, SizeT end_offset, const LoopTag *loop_tag) {
        using QOperationSymbol = QOperationSymbol_T<Char_T>;

        StringUtils::TrimLeft(content, offset, end_offset);
        StringUtils::TrimRight(content, offset, end_offset);

        if (offset < end_offset) {
            switch (content[offset]) {
                case QOperationSymbol::ParenthesesStart: {
                    ++offset;     // Drop (
                    --end_offset; // Drop )

                    if ((last_oper != oper) || (oper != QOperation::NoOp)) {
                        const QExpression &expr =
                            exprs.Insert(QExpression{parseExpressions(content, offset, end_offset, loop_tag), oper});
                        return (expr.SubExpressions.Size() != 0);
                    }

                    // The entire expression is inside (...)
                    exprs = parseExpressions(content, offset, end_offset, loop_tag);
                    return (exprs.Size() != 0);
                }

                case QOperationSymbol::BracketStart: {
                    if ((end_offset - offset) > TagPatterns::VariableFullLength) {
                        end_offset -= TagPatterns::InLineSuffixLength;

                        if (content[end_offset] == TagPatterns::InLineLastChar) {
                            offset += TagPatterns::VariablePrefixLength;
                            QExpression &expr    = exprs.Insert(QExpression{ExpressionType::Variable, oper});
                            expr.Variable.Offset = offset;
                            expr.Variable.Length = SizeT16(end_offset - offset);
                            checkLoopVariable(content, expr.Variable, loop_tag);
                            return true;
                        }
                    }

                    break;
                }

                default: {
                    const SizeT original_offset = offset;
                    QExpression expr;
                    expr.Operation = oper;

                    const QNumberType n_type = Digit::StringToNumber(expr.Value.Number, content, offset, end_offset);

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

    static QOperation getOperation(const Char_T *content, SizeT &offset, const SizeT end_offset) noexcept {
        using QOperationSymbol = QOperationSymbol_T<Char_T>;

        while (offset < end_offset) {
            switch (content[offset]) {
                case QOperationSymbol::OrExp: { // ||
                    if (content[(offset + 1)] == QOperationSymbol::OrExp) {
                        return QOperation::Or;
                    }

                    return QOperation::BitwiseOr;
                }

                case QOperationSymbol::AndExp: { // &&
                    if (content[(offset + 1)] == QOperationSymbol::AndExp) {
                        return QOperation::And;
                    }

                    return QOperation::BitwiseAnd;
                }

                case QOperationSymbol::GreaterExp: { // > or >=
                    if (content[(offset + 1)] == QOperationSymbol::EqualExp) {
                        return QOperation::GreaterOrEqual;
                    }

                    return QOperation::Greater;
                }

                case QOperationSymbol::LessExp: { // < or <=
                    if (content[(offset + 1)] == QOperationSymbol::EqualExp) {
                        return QOperation::LessOrEqual;
                    }

                    return QOperation::Less;
                }

                case QOperationSymbol::NotExp: { // !=
                    if (content[(offset + 1)] == QOperationSymbol::EqualExp) {
                        return QOperation::NotEqual;
                    }

                    return QOperation::Error;
                }

                case QOperationSymbol::EqualExp: { // ==
                    if (content[(offset + 1)] == QOperationSymbol::EqualExp) {
                        return QOperation::Equal;
                    }

                    return QOperation::Error;
                }

                case QOperationSymbol::SubtractExp: {
                    if (isExpression(content, offset)) {
                        return QOperation::Subtraction;
                    }

                    break;
                }

                case QOperationSymbol::AddExp: {
                    if (isExpression(content, offset)) {
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
                    SizeT32 skip{0U};

                    while (offset < end_offset) {
                        if (content[offset] == QOperationSymbol::ParenthesesEnd) {
                            if (skip == 0U) {
                                break;
                            }

                            --skip;
                        } else if (content[offset] == QOperationSymbol::ParenthesesStart) {
                            ++skip;
                        }

                        ++offset;
                    }

                    if (offset < end_offset) {
                        continue;
                    }

                    return QOperation::Error;
                }

                case QOperationSymbol::BracketStart: {
                    do {
                        ++offset;
                    } while ((offset < end_offset) && (content[offset] != QOperationSymbol::BracketEnd));

                    if (offset < end_offset) {
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

    static bool isExpression(const Char_T *content, SizeT offset) noexcept {
        using QOperationSymbol = QOperationSymbol_T<Char_T>;

        while (offset != 0) {
            --offset;

            switch (content[offset]) {
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
                    const Char_T ch = content[offset];
                    return ((ch >= DigitUtils::DigitChar::Zero) && (ch <= DigitUtils::DigitChar::Nine));
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
