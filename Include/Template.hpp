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
#include "Digit.hpp"
#include "QExpresion.hpp"
#include "StringStream.hpp"

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
 *      - s: String, n: Number, var: Raw ariable,
 *      - set: child name in the passed colloction: Optional.
 *      - value: the current value in the colloction: Optional.
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

    struct TagBit;

    // template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    // inline static void CachedRender(const Char_T_ *content, const SizeT length, const Value_T_ &value,
    //                                 StringStream_T_ &stream, const Char_T_ *name, const SizeT name_length) {
    //     // This is not a thread safe function, and its here to show how to cache geterated tags.
    //     // Can be used in a single threaded prosess, but it is better to change it.

    //     // Usage:
    //     // #include "HArray.hpp"
    //     // #include "Value.hpp"
    //     // #include "Template.hpp"
    //     //
    //     // CachedRender("<html>...</html>", 16, value, stringstream, "page1", 5);

    //     using TemplateSubCV = TemplateSub<Char_T_, Value_T_, StringStream_T_>;

    //     const TemplateSubCV temp{content, length, &stream, &value};

    //     static HArray<Array<TagBit>, Char_T_> cache;

    //     Array<TagBit> &tags = cache.GetOrAdd(name, name_length);

    //     if (tags.IsEmpty()) {
    //         temp.Parse(tags);
    //     }

    //     const TagBit *tag = tags.First();
    //     const TagBit *end = (tag + tags.Size());
    //     temp.Render(tag, end);
    // }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    inline static void Render(const Char_T_ *content, SizeT length, const Value_T_ &value, StringStream_T_ &stream,
                              Array<TagBit> &tags_cache) {
        using TemplateSubCV = TemplateSub<Char_T_, Value_T_, StringStream_T_>;
        const TemplateSubCV temp{content, length, &stream, &value};

        if (tags_cache.IsEmpty()) {
            temp.Parse(tags_cache);
        }

        const TagBit *tag = tags_cache.First();
        const TagBit *end = (tag + tags_cache.Size());
        temp.Render(tag, end);
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    inline static void Render(const Char_T_ *content, const SizeT length, const Value_T_ &value,
                              StringStream_T_ &stream) {
        Array<TagBit> tags_cache;
        Render(content, length, value, stream, tags_cache);
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_ = StringStream<Char_T_>>
    inline static StringStream_T_ Render(const Char_T_ *content, const SizeT length, const Value_T_ &value) {
        StringStream_T_ stream;
        Render(content, length, value, stream);
        return stream;
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_ = StringStream<Char_T_>>
    inline static StringStream_T_ Render(const Char_T_ *content, const Value_T_ &value) {
        return Render(content, StringUtils::Count(content), value);
    }

    // VariableTag -------------------------------------------
    struct VariableTag {
        SizeT          Offset;
        unsigned short Length;
        unsigned char  Level;
        unsigned char  IsLoopValue;
    };

    using Expresion = QExpresion<VariableTag>;

    // TagType -------------------------------------------
    enum class TagType : unsigned char {
        None = 0,
        Variable,    // {var:x}
        RawVariable, // {raw:x}
        Math,        // {math:x}
        Loop,        // <loop ...></loop>
        InLineIf,    // {if:x}
        If,          // <if case="..."></if>
        RawText
    };

    // MathTag -------------------------------------------
    struct MathTag {
        Array<Expresion> Expresions;
        SizeT            Offset;
        SizeT            EndOffset;
    };

    // LoopTagOptions -------------------------------------------
    struct LoopTagOptions {
        static constexpr unsigned char None           = 0;
        static constexpr unsigned char SetIsLoopValue = 1;
        static constexpr unsigned char SortAscend     = 2;
        static constexpr unsigned char SortDescend    = 4;
    };

    // LoopTagOptions -------------------------------------------
    struct LoopTag {
        Array<TagBit> SubTags;
        SizeT         Offset;
        unsigned char SetOffset;
        unsigned char SetLength;
        unsigned char SetLevel;
        unsigned char ValueOffset;
        unsigned char GroupOffset;
        unsigned char ValueLength;
        unsigned char GroupLength;
        unsigned char Options;
    };

    // InLineIfTag -------------------------------------------
    struct InLineIfTag {
        Array<TagBit>    SubTags;
        Array<Expresion> Case;
    };

    // IfTag -------------------------------------------
    struct IfTagCase {
        Array<TagBit>    SubTags;
        Array<Expresion> Case;
    };

    // TagBit -------------------------------------------
    struct TagBit {
      public:
        TagBit()                          = delete;
        TagBit(const TagBit &)            = delete;
        TagBit &operator=(const TagBit &) = delete;
        TagBit &operator=(TagBit &&)      = delete;

        TagBit(TagBit &&tag) noexcept : content_(tag.content_), type_{tag.type_} {
            // tag.content_ = {0, 0};
            tag.type_ = TagType::None;
        }

        TagBit(TagType type) : type_{type} {
            switch (type) {
                case TagType::Variable:
                case TagType::RawVariable: {
                    info_ = Memory::AllocateInit<VariableTag>();
                    break;
                }

                case TagType::Math: {
                    info_ = Memory::AllocateInit<MathTag>();
                    break;
                }

                case TagType::Loop: {
                    info_ = Memory::AllocateInit<LoopTag>();
                    break;
                }

                case TagType::InLineIf: {
                    info_ = Memory::AllocateInit<InLineIfTag>();
                    break;
                }

                case TagType::If: {
                    info_ = Memory::AllocateInit<Array<IfTagCase>>();
                    break;
                }

                default: {
                }
            }
        }

        TagBit(SizeT offset, SizeT length) : type_{TagType::RawText} {
            content_.offset = static_cast<unsigned int>(offset);
            content_.length = static_cast<unsigned int>(length);
        }

        ~TagBit() {
            switch (type_) {
                case TagType::Variable:
                case TagType::RawVariable: {
                    Memory::Deallocate(info_);
                    break;
                }

                case TagType::Math: {
                    Memory::Dispose(static_cast<MathTag *>(info_));
                    Memory::Deallocate(info_);
                    break;
                }

                case TagType::Loop: {
                    Memory::Dispose(static_cast<LoopTag *>(info_));
                    Memory::Deallocate(info_);
                    break;
                }

                case TagType::InLineIf: {
                    Memory::Dispose(static_cast<InLineIfTag *>(info_));
                    Memory::Deallocate(info_);
                    break;
                }

                case TagType::If: {
                    Memory::Dispose(static_cast<Array<IfTagCase> *>(info_));
                    Memory::Deallocate(info_);
                    break;
                }

                default: {
                }
            }
        }

        inline void        *GetInfo() const noexcept { return info_; }
        inline TagType      GetType() const noexcept { return type_; }
        inline unsigned int GetOffset() const noexcept { return content_.offset; }
        inline unsigned int GetLength() const noexcept { return content_.length; }

      private:
        struct Content_ {
            unsigned int offset;
            unsigned int length;
        };

        union {
            void    *info_;
            Content_ content_{0, 0};
        };

        TagType type_{TagType::None};
    };
};

template <typename Char_T_, typename Value_T_, typename StringStream_T_>
struct TemplateSub {
    TemplateSub() = delete;

    TemplateSub(const Char_T_ *content, SizeT length, StringStream_T_ *stream, const Value_T_ *root_value,
                const TemplateSub *parent = nullptr, const unsigned short level = 0) noexcept
        : content_{content}, stream_{stream}, value_{root_value}, parent_{parent}, length_{length}, level_{level} {}

  private:
    using TagType          = Template::TagType;
    using VariableTag      = Template::VariableTag;
    using MathTag          = Template::MathTag;
    using LoopTag          = Template::LoopTag;
    using LoopTagOptions   = Template::LoopTagOptions;
    using InLineIfTag      = Template::InLineIfTag;
    using IfTagCase        = Template::IfTagCase;
    using IfTag            = Array<IfTagCase>;
    using TagBit           = Template::TagBit;
    using Expresion        = QExpresion<VariableTag>;
    using Expresions       = Array<Expresion>;
    using QOperation       = Expresion::QOperation;
    using ExpresionType    = Expresion::ExpresionType;
    using QNumber          = Expresion::QNumber;
    using TemplatePatterns = TemplatePatterns_T_<Char_T_>;

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

    inline void Parse(Array<TagBit> &tags_cache) const { parse(tags_cache, 0, length_); }

    inline bool Evaluate(Expresion &number, const Expresions &exprs) const noexcept {
        const Expresion *expr = exprs.First();

        if (expr != nullptr) {
            return evaluate(number, expr, QOperation::NoOp);
        }

        return false;
    }

    inline Expresions ParseExpressions(SizeT offset, const SizeT end_offset) const noexcept {
        return parseExpressions(offset, end_offset);
    }

  private:
    void parse(Array<TagBit> &tags_cache, SizeT offset, const SizeT end_offset) const {
        SizeT last_offset = offset;

        while (offset < end_offset) {
            if (content_[offset] == TemplatePatterns::InLinePrefix) {
                const SizeT current_offset = (offset + 1);

                switch (content_[current_offset]) {
                    case TemplatePatterns::Var_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::VariablePrefix + 2),
                                                 (content_ + current_offset + 1),
                                                 (TemplatePatterns::VariablePrefixLength - 2))) {
                            const SizeT v_end_offset = Engine::FindOne<Char_T_>(
                                TemplatePatterns::InLineSuffix, content_,
                                (current_offset + TemplatePatterns::VariablePrefixLength), end_offset, length_);

                            if (v_end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{last_offset, (offset - last_offset)};
                                }

                                offset += TemplatePatterns::VariablePrefixLength;
                                TagBit &tag = tags_cache.InsertGet(TagBit{TagType::Variable});
                                parseVariableTag(offset, (v_end_offset - TemplatePatterns::InLineSuffixLength),
                                                 tag.GetInfo());
                                last_offset = v_end_offset;
                                offset      = v_end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns::Raw_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::RawVariablePrefix + 2),
                                                 (content_ + current_offset + 1),
                                                 (TemplatePatterns::RawVariablePrefixLength - 2))) {
                            const SizeT r_end_offset = Engine::FindOne<Char_T_>(
                                TemplatePatterns::InLineSuffix, content_,
                                (current_offset + TemplatePatterns::RawVariablePrefixLength), end_offset, length_);

                            if (r_end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{last_offset, (offset - last_offset)};
                                }

                                offset += TemplatePatterns::RawVariablePrefixLength;
                                TagBit &tag = tags_cache.InsertGet(TagBit{TagType::RawVariable});
                                parseVariableTag(offset, (r_end_offset - TemplatePatterns::InLineSuffixLength),
                                                 tag.GetInfo());
                                last_offset = r_end_offset;
                                offset      = r_end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns::Math_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::MathPrefix + 2), (content_ + current_offset + 1),
                                                 (TemplatePatterns::MathPrefixLength - 2))) {
                            const SizeT m_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TemplatePatterns::InLinePrefix, TemplatePatterns::InLineSuffix, content_,
                                (current_offset + TemplatePatterns::MathPrefixLength - 1), end_offset, length_);

                            if (m_end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{last_offset, (offset - last_offset)};
                                }

                                offset += TemplatePatterns::MathPrefixLength;
                                TagBit &tag = tags_cache.InsertGet(TagBit{TagType::Math});
                                parseMathTag(offset, (m_end_offset - TemplatePatterns::InLineSuffixLength),
                                             tag.GetInfo());
                                last_offset = m_end_offset;
                                offset      = m_end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns::InlineIf_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::InLineIfPrefix + 2),
                                                 (content_ + current_offset + 1),
                                                 (TemplatePatterns::InLineIfPrefixLength - 2))) {
                            const SizeT ii_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TemplatePatterns::InLinePrefix, TemplatePatterns::InLineSuffix, content_,
                                (current_offset + TemplatePatterns::InLineIfPrefixLength - 1), end_offset, length_);

                            if (ii_end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{last_offset, (offset - last_offset)};
                                }

                                offset += TemplatePatterns::InLineIfPrefixLength;
                                TagBit &tag = tags_cache.InsertGet(TagBit{TagType::InLineIf});
                                parseInLineIfTag(offset, (ii_end_offset - TemplatePatterns::InLineSuffixLength),
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
            } else if (content_[offset] == TemplatePatterns::MultiLinePrefix) { // <loop
                const SizeT current_offset = (offset + 1);

                if (content_[current_offset] == TemplatePatterns::Loop_2ND_Char) {
                    if (StringUtils::IsEqual((TemplatePatterns::LoopPrefix + 2), (content_ + current_offset + 1),
                                             (TemplatePatterns::LoopPrefixLength - 2))) {
                        const SizeT l_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                            TemplatePatterns::LoopPrefix, TemplatePatterns::LoopPrefixLength,
                            TemplatePatterns::LoopSuffix, TemplatePatterns::LoopSuffixLength, content_,
                            (current_offset + TemplatePatterns::InLineIfPrefixLength - 1), end_offset, length_);

                        if (l_end_offset != 0) {
                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::InLineIfPrefixLength;
                            TagBit &tag = tags_cache.InsertGet(TagBit{TagType::Loop});
                            parseLoopTag(offset, (l_end_offset - TemplatePatterns::LoopSuffixLength), tag.GetInfo());
                            last_offset = l_end_offset;
                            offset      = l_end_offset;
                            continue;
                        }
                    }
                } else if (content_[current_offset] == TemplatePatterns::If_2ND_Char) { // <if
                    if (StringUtils::IsEqual((TemplatePatterns::IfPrefix + 2), (content_ + current_offset + 1),
                                             (TemplatePatterns::IfPrefixLength - 2))) {
                        const SizeT i_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                            TemplatePatterns::IfPrefix, TemplatePatterns::IfPrefixLength, TemplatePatterns::IfSuffix,
                            TemplatePatterns::IfSuffixLength, content_, current_offset, end_offset, length_);

                        if (i_end_offset != 0) {
                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::IfPrefixLength;
                            TagBit &tag = tags_cache.InsertGet(TagBit{TagType::If});
                            parseIfTag(offset, (i_end_offset - TemplatePatterns::IfSuffixLength), tag.GetInfo());
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
            offset = Engine::FindOne<Char_T_>(TemplatePatterns::InLinePrefix, content_, offset, end_offset, length_);

            if (offset != 0) {
                switch (content_[offset]) {
                    case TemplatePatterns::Var_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::VariablePrefix + 2), (content_ + offset + 1),
                                                 (TemplatePatterns::VariablePrefixLength - 2))) {
                            const SizeT v_end_offset = Engine::FindOne<Char_T_>(
                                TemplatePatterns::InLineSuffix, content_,
                                (offset + TemplatePatterns::VariablePrefixLength), end_offset, length_);
                            offset -= TemplatePatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::VariablePrefixLength;
                            TagBit &tag = tags_cache.InsertGet(TagBit{TagType::Variable});
                            parseVariableTag(offset, (v_end_offset - TemplatePatterns::InLineSuffixLength),
                                             tag.GetInfo());
                            last_offset = v_end_offset;
                            offset      = v_end_offset;
                        }

                        continue;
                    }

                    case TemplatePatterns::Raw_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::RawVariablePrefix + 2), (content_ + offset + 1),
                                                 (TemplatePatterns::RawVariablePrefixLength - 2))) {
                            const SizeT r_end_offset = Engine::FindOne<Char_T_>(
                                TemplatePatterns::InLineSuffix, content_,
                                (offset + TemplatePatterns::RawVariablePrefixLength), end_offset, length_);
                            offset -= TemplatePatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::RawVariablePrefixLength;
                            TagBit &tag = tags_cache.InsertGet(TagBit{TagType::RawVariable});
                            parseVariableTag(offset, (r_end_offset - TemplatePatterns::InLineSuffixLength),
                                             tag.GetInfo());
                            last_offset = r_end_offset;
                            offset      = r_end_offset;
                        }

                        continue;
                    }

                    case TemplatePatterns::Math_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::MathPrefix + 2), (content_ + offset + 1),
                                                 (TemplatePatterns::MathPrefixLength - 2))) {
                            const SizeT m_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TemplatePatterns::InLinePrefix, TemplatePatterns::InLineSuffix, content_,
                                (offset + TemplatePatterns::MathPrefixLength - 1), end_offset, length_);
                            offset -= TemplatePatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::MathPrefixLength;
                            TagBit &tag = tags_cache.InsertGet(TagBit{TagType::Math});
                            parseMathTag(offset, (m_end_offset - TemplatePatterns::InLineSuffixLength), tag.GetInfo());
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
                    stream->Write(TemplatePatterns::HTMLAnd, TemplatePatterns::HTMLAndLength);
                    offset = (++index);
                    break;
                }

                case '<': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TemplatePatterns::HTMLLess, TemplatePatterns::HTMLLessLength);
                    offset = (++index);
                    break;
                }

                case '>': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TemplatePatterns::HTMLGreater, TemplatePatterns::HTMLGreaterLength);
                    offset = (++index);
                    break;
                }

                case '"': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TemplatePatterns::HTMLQuote, TemplatePatterns::HTMLQuoteLength);
                    offset = (++index);
                    break;
                }

                case '\'': {
                    stream->Write((str + offset), (index - offset));
                    stream->Write(TemplatePatterns::HTMLSingleQuote, TemplatePatterns::HTMLSingleQuoteLength);
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
        const VariableTag &i_tag = *(static_cast<const VariableTag *>(tag->GetInfo()));
        const Value_T_    *value = getValue(i_tag);

        if (value != nullptr) {
            if (Config::AutoEscapeHTML) {
                if (value->IsString()) {
                    const Char_T_ *str;
                    SizeT          len;
                    value->SetCharAndLength(str, len);
                    escapeHTMLSpecialChars(stream_, str, len);
                    return;
                }
            }

            if (value->CopyStringValueTo(*stream_)) {
                return;
            }

            if ((i_tag.IsLoopValue == 1) && (loop_key_ != nullptr)) {
                if (Config::AutoEscapeHTML) {
                    escapeHTMLSpecialChars(stream_, loop_key_, loop_key_length_);
                } else {
                    stream_->Write(loop_key_, loop_key_length_);
                }

                return;
            }
        }

        stream_->Write(((content_ + i_tag.Offset) - TemplatePatterns::VariablePrefixLength),
                       (i_tag.Length + TemplatePatterns::VariablePrefixLength + TemplatePatterns::InLineSuffixLength));
    }

    void renderRawVariable(const TagBit *tag) const {
        const VariableTag &i_tag = *(static_cast<const VariableTag *>(tag->GetInfo()));
        const Value_T_    *value = getValue(i_tag);

        if ((value != nullptr) && value->CopyStringValueTo(*stream_)) {
            return;
        }

        stream_->Write(((content_ + i_tag.Offset) - TemplatePatterns::RawVariablePrefixLength),
                       (i_tag.Length + TemplatePatterns::RawVariableFulllength));
    }

    void renderMath(const TagBit *tag) const {
        const MathTag   &i_tag = *(static_cast<const MathTag *>(tag->GetInfo()));
        const Expresion *expr  = i_tag.Expresions.First();
        Expresion        result;

        if (i_tag.Expresions.IsNotEmpty() && evaluate(result, expr, QOperation::NoOp)) {
            switch (result.Type) {
                case ExpresionType::NaturalNumber: {
                    Digit<Char_T_>::NumberToString(*stream_, result.Number.Natural);
                    break;
                }

                case ExpresionType::IntegerNumber: {
                    Digit<Char_T_>::NumberToString(*stream_, result.Number.Integer);
                    break;
                }

                case ExpresionType::RealNumber: {
                    Digit<Char_T_>::NumberToString(*stream_, result.Number.Real, 1, 0, 3);
                    break;
                }

                default: {
                }
            }

        } else {
            stream_->Write(((content_ + i_tag.Offset) - TemplatePatterns::MathPrefixLength),
                           ((i_tag.EndOffset - i_tag.Offset) + TemplatePatterns::MathPrefixLength +
                            TemplatePatterns::InLineSuffixLength));
        }
    }

    void renderLoop(const TagBit *tag) const {
        const LoopTag  &i_tag = *(static_cast<const LoopTag *>(tag->GetInfo()));
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
        const InLineIfTag &i_tag = *(static_cast<const InLineIfTag *>(tag->GetInfo()));
        const Expresion   *expr  = i_tag.Case.First();
        Expresion          result;

        if (i_tag.Case.IsNotEmpty() && evaluate(result, expr, QOperation::NoOp)) {
            const TagBit *s_tag     = i_tag.SubTags.First();
            const TagBit *s_end     = (s_tag + i_tag.SubTags.Size());
            SizeT         true_size = 0;

            if (++expr != i_tag.Case.End()) {
                true_size = static_cast<SizeT>(expr->Number.Natural);
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
        const IfTag     &i_tag = *(static_cast<const IfTag *>(tag->GetInfo()));
        const IfTagCase *item  = i_tag.First();
        const IfTagCase *end   = (item + i_tag.Size());
        Expresion        result;

        if ((item != nullptr) && item->Case.IsNotEmpty()) { // First case should not be empty
            do {
                // <else> without if = (item->Case == nothing)
                const Expresion *expr = item->Case.First();

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

    void renderRawText(const TagBit *tag) const { stream_->Write((content_ + tag->GetOffset()), tag->GetLength()); }

    void parseVariableTag(SizeT offset, SizeT end_offset, void *tag) const {
        VariableTag &i_tag = *(static_cast<VariableTag *>(tag));
        i_tag.Offset       = offset;
        i_tag.Length       = static_cast<unsigned char>(end_offset - offset);

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
                    i_tag.Level       = static_cast<unsigned char>(level);
                    break;
                }

                temp = temp->parent_;
                --level;
            }
        }
    }

    void parseMathTag(SizeT offset, SizeT end_offset, void *tag) const {
        MathTag &i_tag   = *(static_cast<MathTag *>(tag));
        i_tag.Expresions = parseExpressions(offset, end_offset);
        i_tag.Offset     = offset;
        i_tag.EndOffset  = end_offset;
    }

    /*
     * Gets everything between "..."
     */
    SizeT getQuotedValue(SizeT &offset, SizeT end_offset) const noexcept {
        offset = Engine::FindOne<Char_T_>(TemplatePatterns::QuoteChar, content_, offset, end_offset, length_);

        if (offset != 0) {
            end_offset = Engine::FindOne<Char_T_>(TemplatePatterns::QuoteChar, content_, offset, end_offset, length_);
        }

        return end_offset;
    }

    void parseLoopTag(SizeT offset, SizeT end_offset, void *tag) const {
        LoopTag    &i_tag = *(static_cast<LoopTag *>(tag));
        const SizeT loop_content_offset =
            Engine::FindOne<Char_T_>(TemplatePatterns::MultiLineSuffix, content_, offset, end_offset, length_);

        SizeT offset2     = getQuotedValue(offset, loop_content_offset);
        i_tag.Offset      = offset;
        SizeT last_offset = offset;
        offset -= 6U; // (=) plus (") plus (two chars) = 4 plus (the char before them) = 5

        while ((offset2 != 0) && (offset < loop_content_offset)) {
            switch (content_[offset]) {
                case TemplatePatterns::SetChar: {
                    SizeT set_offset = last_offset;
                    SizeT set_length = ((offset2 - 1) - last_offset);

                    if ((content_[last_offset] == TemplatePatterns::InLinePrefix) &&
                        (set_length > TemplatePatterns::VariableFulllength)) {
                        set_offset += TemplatePatterns::VariablePrefixLength;
                        set_length -= TemplatePatterns::VariableFulllength;
                    }

                    VariableTag set_var{};
                    parseVariableTag(set_offset, (set_offset + set_length), &set_var);
                    i_tag.SetOffset = static_cast<unsigned char>(set_var.Offset - i_tag.Offset);
                    i_tag.SetLength = static_cast<unsigned char>(set_var.Length);
                    i_tag.SetLevel  = set_var.Level;
                    i_tag.Options |= set_var.IsLoopValue;

                    offset      = offset2;
                    offset2     = getQuotedValue(offset, loop_content_offset);
                    last_offset = offset;
                    offset -= 6U;
                    break;
                }

                case TemplatePatterns::ValueChar: {
                    i_tag.ValueOffset = static_cast<unsigned char>(last_offset - i_tag.Offset);
                    i_tag.ValueLength = static_cast<unsigned char>((offset2 - 1U) - last_offset);
                    offset            = offset2;
                    offset2           = getQuotedValue(offset, loop_content_offset);
                    last_offset       = offset;
                    offset -= 6U;
                    break;
                }

                case TemplatePatterns::SortChar: {
                    i_tag.Options |=
                        ((content_[last_offset] == 'a') ? LoopTagOptions::SortAscend : LoopTagOptions::SortDescend);
                    offset      = offset2;
                    offset2     = getQuotedValue(offset, loop_content_offset);
                    last_offset = offset;
                    offset -= 6U;
                    break;
                }

                case TemplatePatterns::GroupChar: {
                    i_tag.GroupOffset = static_cast<unsigned char>(last_offset - i_tag.Offset);
                    i_tag.GroupLength = static_cast<unsigned char>((offset2 - 1) - last_offset);
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
        InLineIfTag &i_tag = *(static_cast<InLineIfTag *>(tag));

        SizeT true_offset      = 0;
        SizeT true_end_offset  = 0;
        SizeT false_offset     = 0;
        SizeT false_end_offset = 0;

        SizeT offset2     = getQuotedValue(offset, end_offset);
        SizeT last_offset = offset;

        offset -= 5U; // (=) plus (") plus (two chars) = 4 plus (the char before them) = 5

        while ((offset2 != 0) && (offset < end_offset)) {
            switch (content_[offset]) {
                case TemplatePatterns::CaseChar: {
                    i_tag.Case  = parseExpressions(last_offset, (offset2 - 1)); // 1 = (") at the end
                    offset      = offset2;
                    offset2     = getQuotedValue(offset, end_offset);
                    last_offset = offset;
                    offset -= 5U;
                    break;
                }

                case TemplatePatterns::TrueChar: {
                    true_offset     = last_offset;
                    true_end_offset = (offset2 - 1);
                    offset          = offset2;
                    offset2         = getQuotedValue(offset, end_offset);
                    last_offset     = offset;
                    offset -= 5U;
                    break;
                }

                case TemplatePatterns::FalseChar: {
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
                Expresion &expr     = i_tag.Case.InsertGet(Expresion{});
                expr.Number.Natural = i_tag.SubTags.Size(); // To use only one heap for true and false.
            }

            if (false_offset != false_end_offset) {
                lightParse(i_tag.SubTags, false_offset, false_end_offset);
            }
        }
    }

    void parseIfTag(SizeT offset, const SizeT end_offset, void *tag) const {
        IfTag        &i_tag = *(static_cast<IfTag *>(tag));
        Array<TagBit> sub_tags;
        SizeT         offset2 = getQuotedValue(offset, end_offset);
        SizeT         case_end_offset;
        SizeT         else_offset;
        SizeT         content_offset;

        while ((offset2 != 0) && (offset < end_offset)) {
            case_end_offset = (offset2 - 1);
            content_offset =
                Engine::FindOne<Char_T_>(TemplatePatterns::MultiLineSuffix, content_, offset2, end_offset, length_);

            if (content_offset != 0) {
                else_offset = nextElse(content_offset, end_offset);

                if (else_offset == 0) {
                    parse(sub_tags, content_offset, end_offset);
                    i_tag +=
                        IfTagCase{static_cast<Array<TagBit> &&>(sub_tags), parseExpressions(offset, case_end_offset)};

                    break;
                }

                parse(sub_tags, content_offset, (else_offset - TemplatePatterns::ElsePrefixLength));
                i_tag += IfTagCase{static_cast<Array<TagBit> &&>(sub_tags), parseExpressions(offset, case_end_offset)};

                if ((content_[else_offset] != TemplatePatterns::ElseIfChar)) {
                    else_offset = Engine::FindOne<Char_T_>(TemplatePatterns::MultiLineSuffix, content_, else_offset,
                                                           end_offset, length_);

                    if (else_offset != 0) {
                        parse(sub_tags, else_offset, end_offset);
                        i_tag += IfTagCase{static_cast<Array<TagBit> &&>(sub_tags), Expresions{}}; // else without if
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
            else_offset = Engine::Find<Char_T_>(TemplatePatterns::ElsePrefix, TemplatePatterns::ElsePrefixLength,
                                                content_, offset, end_offset, length_);

            if (else_offset == 0) {
                // No <else.
                break;
            }

            const SizeT next_if = Engine::Find<Char_T_>(TemplatePatterns::IfPrefix, TemplatePatterns::IfPrefixLength,
                                                        content_, offset, end_offset, length_);

            if ((next_if == 0) || (else_offset < next_if)) {
                // No nesting <ifs or <else before a sub-if.
                break;
            }

            offset = Engine::Find<Char_T_>(TemplatePatterns::IfSuffix, TemplatePatterns::IfSuffixLength, content_,
                                           next_if, end_offset, length_);

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
        const bool      has_index = (id[(length - 1)] == TemplatePatterns::VariableIndexSuffix);

        if (v_is_loop_value != 1) {
            if (!has_index) {
                return value_->GetValue(id, length);
            }

            while ((offset < length) && (id[offset] != TemplatePatterns::VariableIndexPrefix)) {
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
                const unsigned int level    = static_cast<unsigned int>(level_);
                unsigned int       iv_level = static_cast<unsigned int>(v_level);
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

            while ((offset2 < length) && (id[offset2] != TemplatePatterns::VariableIndexSuffix)) {
                ++offset2;
            };

            value = value->GetValue((id + offset), (offset2 - offset));

            ++offset2; // The char after ]

            if (id[offset2] != TemplatePatterns::VariableIndexPrefix) {
                break;
            }

            offset = offset2;
        }

        return value;
    }

    // Evaluate /////////////////////////////////////
    bool evaluate(Expresion &left, const Expresion *&expr, const QOperation previous_oper) const noexcept {
        const Expresion *next_expr;
        Expresion        right;
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

            return (left.Type != ExpresionType::NotANumber);
        }

        return false;
    }

    bool GetExpressionValue(Expresion &number, const Expresion *expr, bool not_equal) const noexcept {
        switch (expr->Type) {
            case ExpresionType::SubOperation: {
                const Expresion *sub_expr = expr->SubExpresions.First();
                return evaluate(number, sub_expr, QOperation::NoOp);
            }

            case ExpresionType::Variable: {
                if (not_equal) {
                    const Value_T_ *val = getValue(expr->Variable);

                    if (val != nullptr) {
                        switch (val->GetNumberType()) {
                            case 1U: {
                                val->SetNumber(number.Number.Natural);
                                number.Type = ExpresionType::NaturalNumber;

                                return true;
                            }

                            case 2U: {
                                val->SetNumber(number.Number.Integer);
                                number.Type = ExpresionType::IntegerNumber;

                                return true;
                            }

                            case 3U: {
                                val->SetNumber(number.Number.Real);
                                number.Type = ExpresionType::RealNumber;

                                return true;
                            }

                            default: {
                                if (val->SetNumber(number.Number.Real)) {
                                    number.Type = ExpresionType::RealNumber;
                                    return true;
                                }

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

    bool evaluateExpression(Expresion &left, Expresion &right, const QOperation oper) const noexcept {
        switch (oper) {
            case QOperation::Exponent: { // ^
                return (left ^= right);
            }

            case QOperation::Remainder: { // %
                left.Number.Natural = (left % right);
                left.Type           = ExpresionType::NaturalNumber;
                break;
            }

            case QOperation::Multiplication: { // *
                left *= right;
                break;
            }

            case QOperation::Division: { // /
                if (right != 0) {
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
                left.Number.Natural = (left < right);
                left.Type           = ExpresionType::NaturalNumber;
                break;
            }

            case QOperation::LessOrEqual: { // <=
                left.Number.Natural = (left <= right);
                left.Type           = ExpresionType::NaturalNumber;
                break;
            }

            case QOperation::Greater: { // >
                left.Number.Natural = (left > right);
                left.Type           = ExpresionType::NaturalNumber;
                break;
            }

            case QOperation::GreaterOrEqual: { // >=
                left.Number.Natural = (left >= right);
                left.Type           = ExpresionType::NaturalNumber;
                break;
            }

            case QOperation::And: { // &&
                left.Number.Natural = ((left > 0U) && (right > 0U));
                left.Type           = ExpresionType::NaturalNumber;
                break;
            }

            case QOperation::Or: { // ||
                left.Number.Natural = ((left > 0U) || (right > 0U));
                left.Type           = ExpresionType::NaturalNumber;
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
                    left.Number.Natural = (left.Number.Natural ^ 1U);
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

    bool isEqual(Expresion &left, Expresion &right) const noexcept {
        const Value_T_ *left_value    = nullptr;
        const Value_T_ *right_value   = nullptr;
        const Char_T_  *left_content  = nullptr;
        const Char_T_  *right_content = nullptr;
        SizeT           left_length   = 0;
        SizeT           right_length  = 0;
        bool            left_is_a_number;
        bool            right_is_a_number;

        switch (left.Type) {
            case ExpresionType::RealNumber:
            case ExpresionType::NaturalNumber:
            case ExpresionType::IntegerNumber: {
                left_is_a_number = true;
                break;
            }

            case ExpresionType::Variable: {
                left_value = getValue(left.Variable);

                if (left_value != nullptr) {
                    switch (left_value->GetNumberType()) {
                        case 1U: {
                            left_value->SetNumber(left.Number.Natural);
                            left.Type        = ExpresionType::NaturalNumber;
                            left_is_a_number = true;
                            break;
                        }

                        case 2U: {
                            left_value->SetNumber(left.Number.Integer);
                            left.Type        = ExpresionType::IntegerNumber;
                            left_is_a_number = true;
                            break;
                        }

                        case 3U: {
                            left_value->SetNumber(left.Number.Real);
                            left.Type        = ExpresionType::RealNumber;
                            left_is_a_number = true;
                            break;
                        }

                        default: {
                            if (!(left_value->SetCharAndLength(left_content, left_length))) {
                                return false;
                            }

                            left_is_a_number = false;
                        }
                    }

                    break;
                }

                return false;
            }

            case ExpresionType::NotANumber: {
                left_content     = (content_ + left.Number.Offset);
                left_length      = left.Number.Length;
                left_is_a_number = false;
                break;
            }

            default: {
                return false; // It should not get to this.
            }
        }

        switch (right.Type) {
            case ExpresionType::RealNumber:
            case ExpresionType::NaturalNumber:
            case ExpresionType::IntegerNumber: {
                right_is_a_number = true;
                break;
            }

            case ExpresionType::Variable: {
                right_value = getValue(right.Variable);

                if (right_value != nullptr) {
                    switch (right_value->GetNumberType()) {
                        case 1U: {
                            right_value->SetNumber(right.Number.Natural);
                            right.Type        = ExpresionType::NaturalNumber;
                            right_is_a_number = true;
                            break;
                        }

                        case 2U: {
                            right_value->SetNumber(right.Number.Integer);
                            right.Type        = ExpresionType::IntegerNumber;
                            right_is_a_number = true;
                            break;
                        }

                        case 3U: {
                            right_value->SetNumber(right.Number.Real);
                            right.Type        = ExpresionType::RealNumber;
                            right_is_a_number = true;
                            break;
                        }

                        default: {
                            if (!(right_value->SetCharAndLength(right_content, right_length))) {
                                return false;
                            }

                            right_is_a_number = false;
                        }
                    }

                    break;
                }

                return false;
            }

            case ExpresionType::NotANumber: {
                right_content     = (content_ + right.Number.Offset);
                right_length      = right.Number.Length;
                right_is_a_number = false;
                break;
            }

            default: {
                return false; // It should not get to this.
            }
        }

        if (left_is_a_number || right_is_a_number) {
            if (!left_is_a_number) {
                if ((left_value != nullptr) && left_value->SetNumber(left.Number.Real)) {
                    left.Type = ExpresionType::RealNumber;
                } else {
                    return false;
                }
            }

            if (!right_is_a_number) {
                if ((right_value != nullptr) && right_value->SetNumber(right.Number.Real)) {
                    right.Type = ExpresionType::RealNumber;
                } else {
                    return false;
                }
            }

            left.Number.Natural = (left == right);
            left.Type           = ExpresionType::NaturalNumber;
            return true;
        }

        left.Number.Natural =
            ((left_length == right_length) && StringUtils::IsEqual(left_content, right_content, right_length));
        left.Type = ExpresionType::NaturalNumber;

        return true;
    }

    Expresions parseExpressions(SizeT offset, const SizeT end_offset) const noexcept {
        Expresions exprs;
        QOperation last_oper = QOperation::NoOp;

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

        return Expresions{};
    }

    bool parseValue(Expresions &exprs, const QOperation oper, const QOperation last_oper, SizeT offset,
                    SizeT end_offset) const noexcept {
        using QOperationSymbol = QOperationSymbol_T_<Char_T_>;

        StringUtils::TrimLeft(content_, offset, end_offset);
        StringUtils::TrimRight(content_, offset, end_offset);

        if (offset != end_offset) {
            switch (content_[offset]) {
                case QOperationSymbol::ParenthesStart: {
                    ++offset;     // Drop (
                    --end_offset; // Drop )

                    if ((last_oper != oper) || (oper != QOperation::NoOp)) {
                        Expresion &expr = exprs.InsertGet(Expresion{parseExpressions(offset, end_offset), oper});
                        return (expr.SubExpresions.Size() != 0);
                    }

                    // The entire expression is inside (...)
                    exprs = parseExpressions(offset, end_offset);
                    return (exprs.Size() != 0);
                }

                case QOperationSymbol::BracketStart: {
                    if (end_offset - offset > TemplatePatterns::VariableFulllength) {
                        offset += TemplatePatterns::VariablePrefixLength;
                        end_offset -= TemplatePatterns::InLineSuffixLength;

                        Expresion &expr = exprs.InsertGet(Expresion{ExpresionType::Variable, oper});
                        parseVariableTag(offset, end_offset, &(expr.Variable));
                        return true;
                    }

                    break;
                }

                default: {
                    const SizeT length = (end_offset - offset);
                    Expresion   expr;
                    expr.Operation = oper;

                    if (Digit<Char_T_>::StringToNumber(expr.Number.Real, (content_ + offset), length)) {
                        expr.Type = ExpresionType::RealNumber;
                    } else {
                        if ((last_oper != QOperation::Equal) && (last_oper != QOperation::NotEqual) &&
                            (oper != QOperation::Equal) && (oper != QOperation::NotEqual)) {
                            return false;
                        }

                        expr.Number.Offset = offset;
                        expr.Number.Length = length;
                        expr.Type          = ExpresionType::NotANumber;
                    }

                    exprs += static_cast<Expresion &&>(expr);

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

                case QOperationSymbol::ParenthesStart: {
                    ++offset;
                    offset = Engine::SkipInnerPatterns<Char_T_>(QOperationSymbol::ParenthesStart,
                                                                QOperationSymbol::ParenthesEnd, content_, offset,
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

                case QOperationSymbol::ParenthesEnd:
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

template <typename Char_T_>
struct QOperationSymbol_T_ {
  public:
    static constexpr Char_T_ RemainderExp = '%';
    static constexpr Char_T_ MultipleExp  = '*';
    static constexpr Char_T_ DivideExp    = '/';
    static constexpr Char_T_ AddExp       = '+';
    static constexpr Char_T_ SubtractExp  = '-';
    static constexpr Char_T_ EqualExp     = '=';
    static constexpr Char_T_ NotExp       = '!';
    static constexpr Char_T_ LessExp      = '<';
    static constexpr Char_T_ GreaterExp   = '>';
    static constexpr Char_T_ AndExp       = '&';
    static constexpr Char_T_ OrExp        = '|';

    static constexpr Char_T_ ParenthesStart = '(';
    static constexpr Char_T_ ParenthesEnd   = ')';
    static constexpr Char_T_ BracketStart   = '{';
    static constexpr Char_T_ BracketEnd     = '}';
    static constexpr Char_T_ ExponentExp    = '^';
    static constexpr Char_T_ SpaceChar      = ' ';
    static constexpr Char_T_ ColonChar      = ':';
    static constexpr Char_T_ SlashChar      = '/';
};

template <typename Char_T_, int S>
struct TPStrings {};

template <typename Char_T_>
struct TemplatePatterns_T_ {
  private:
    static constexpr int size_ = static_cast<int>(sizeof(Char_T_));

  public:
    /*
     *InLineSuffixLength and InLinePrefixLength should not be more than 1
     */

    static constexpr SizeT InLineSuffixLength = 1U;
    static constexpr SizeT InLinePrefixLength = 1U;

    static constexpr Char_T_ InLinePrefix = '{';
    static constexpr Char_T_ InLineSuffix = '}';

    static constexpr Char_T_ MultiLinePrefix = '<';
    static constexpr Char_T_ MultiLineSuffix = '>';

    static constexpr Char_T_ VariableIndexPrefix = '[';
    static constexpr Char_T_ VariableIndexSuffix = ']';

    // {var:
    static constexpr Char_T_        Var_2ND_Char         = 'v'; // Second character
    static constexpr SizeT          VariablePrefixLength = 5U;
    static constexpr SizeT          VariableFulllength   = (VariablePrefixLength + InLineSuffixLength);
    static constexpr const Char_T_ *VariablePrefix       = TPStrings<Char_T_, size_>::VariablePrefix;

    // {raw:
    static constexpr Char_T_        Raw_2ND_Char            = 'r'; // Second character
    static constexpr SizeT          RawVariablePrefixLength = 5U;
    static constexpr SizeT          RawVariableFulllength   = (VariablePrefixLength + InLineSuffixLength);
    static constexpr const Char_T_ *RawVariablePrefix       = TPStrings<Char_T_, size_>::RawVariablePrefix;

    // {math:
    static constexpr Char_T_        Math_2ND_Char    = 'm'; // Second character
    static constexpr SizeT          MathPrefixLength = 6U;
    static constexpr SizeT          MathFulllength   = (MathPrefixLength + InLineSuffixLength);
    static constexpr const Char_T_ *MathPrefix       = TPStrings<Char_T_, size_>::MathPrefix;

    // {if:
    static constexpr Char_T_        InlineIf_2ND_Char    = 'i'; // Second character
    static constexpr SizeT          InLineIfPrefixLength = 3U;
    static constexpr const Char_T_ *InLineIfPrefix       = TPStrings<Char_T_, size_>::InLineIfPrefix;

    // <loop
    static constexpr Char_T_        Loop_2ND_Char    = 'l'; // Second character
    static constexpr SizeT          LoopPrefixLength = 5U;
    static constexpr const Char_T_ *LoopPrefix       = TPStrings<Char_T_, size_>::LoopPrefix;

    // </loop>
    static constexpr SizeT          LoopSuffixLength = 7U;
    static constexpr const Char_T_ *LoopSuffix       = TPStrings<Char_T_, size_>::LoopSuffix;

    // <if
    static constexpr Char_T_        If_2ND_Char    = 'i'; // Second character
    static constexpr SizeT          IfPrefixLength = 3U;
    static constexpr const Char_T_ *IfPrefix       = TPStrings<Char_T_, size_>::IfPrefix;

    // </if>
    static constexpr SizeT          IfSuffixLength = 5U;
    static constexpr const Char_T_ *IfSuffix       = TPStrings<Char_T_, size_>::IfSuffix;

    // <else
    static constexpr Char_T_        ElseIfChar       = 'i'; // else[i]f
    static constexpr SizeT          ElsePrefixLength = 5U;
    static constexpr const Char_T_ *ElsePrefix       = TPStrings<Char_T_, size_>::ElsePrefix;

    // />
    static constexpr SizeT          ElseSuffixLength = 2U;
    static constexpr const Char_T_ *ElseSuffix       = TPStrings<Char_T_, size_>::ElseSuffix;

    // &amp; &
    static constexpr SizeT          HTMLAndLength = 5U;
    static constexpr const Char_T_ *HTMLAnd       = TPStrings<Char_T_, size_>::HTMLAnd;

    // &lt; <
    static constexpr SizeT          HTMLLessLength = 4U;
    static constexpr const Char_T_ *HTMLLess       = TPStrings<Char_T_, size_>::HTMLLess;

    // &gt; >
    static constexpr SizeT          HTMLGreaterLength = 4U;
    static constexpr const Char_T_ *HTMLGreater       = TPStrings<Char_T_, size_>::HTMLGreater;

    // &quot; "
    static constexpr SizeT          HTMLQuoteLength = 6U;
    static constexpr const Char_T_ *HTMLQuote       = TPStrings<Char_T_, size_>::HTMLQuote;

    // &apos; ' (HTML5)
    static constexpr SizeT          HTMLSingleQuoteLength = 6U;
    static constexpr const Char_T_ *HTMLSingleQuote       = TPStrings<Char_T_, size_>::HTMLSingleQuote;

    static constexpr Char_T_ QuoteChar = '"';

    // Inline If
    static constexpr Char_T_ CaseChar  = 'a'; // c[a]se
    static constexpr Char_T_ TrueChar  = 'r'; // t[r]ue
    static constexpr Char_T_ FalseChar = 'l'; // fa[l]se

    // Loop
    static constexpr Char_T_ SetChar   = 'e'; //  s[e]t
    static constexpr Char_T_ ValueChar = 'a'; // v[a]lue
    static constexpr Char_T_ SortChar  = 'o'; // s[o]rt
    static constexpr Char_T_ GroupChar = 'r'; // g[r]oup

    // Var
    static constexpr Char_T_ TildeChar = '~'; // Tilde
};

// char
template <typename Char_T_>
struct TPStrings<Char_T_, 1> {
    static constexpr const Char_T_ *VariablePrefix    = "{var:";
    static constexpr const Char_T_ *RawVariablePrefix = "{raw:";
    static constexpr const Char_T_ *MathPrefix        = "{math:";
    static constexpr const Char_T_ *InLineIfPrefix    = "{if:";
    static constexpr const Char_T_ *LoopPrefix        = "<loop";
    static constexpr const Char_T_ *LoopSuffix        = "</loop>";
    static constexpr const Char_T_ *IfPrefix          = "<if";
    static constexpr const Char_T_ *IfSuffix          = "</if>";
    static constexpr const Char_T_ *ElsePrefix        = "<else";
    static constexpr const Char_T_ *ElseSuffix        = "/>";
    static constexpr const Char_T_ *HTMLAnd           = "&amp;";
    static constexpr const Char_T_ *HTMLLess          = "&lt;";
    static constexpr const Char_T_ *HTMLGreater       = "&gt;";
    static constexpr const Char_T_ *HTMLQuote         = "&quot;";
    static constexpr const Char_T_ *HTMLSingleQuote   = "&apos;";
};

// char16_t
template <typename Char_T_>
struct TPStrings<Char_T_, 2> {
    static constexpr const Char_T_ *VariablePrefix    = u"{var:";
    static constexpr const Char_T_ *RawVariablePrefix = u"{raw:";
    static constexpr const Char_T_ *MathPrefix        = u"{math:";
    static constexpr const Char_T_ *InLineIfPrefix    = u"{if:";
    static constexpr const Char_T_ *LoopPrefix        = u"<loop";
    static constexpr const Char_T_ *LoopSuffix        = u"</loop>";
    static constexpr const Char_T_ *IfPrefix          = u"<if";
    static constexpr const Char_T_ *IfSuffix          = u"</if>";
    static constexpr const Char_T_ *ElsePrefix        = u"<else";
    static constexpr const Char_T_ *ElseSuffix        = u"/>";
    static constexpr const Char_T_ *HTMLAnd           = u"&amp;";
    static constexpr const Char_T_ *HTMLLess          = u"&lt;";
    static constexpr const Char_T_ *HTMLGreater       = u"&gt;";
    static constexpr const Char_T_ *HTMLQuote         = u"&quot;";
    static constexpr const Char_T_ *HTMLSingleQuote   = u"&apos;";
};

// char32_t
template <typename Char_T_>
struct TPStrings<Char_T_, 4> {
    static constexpr const Char_T_ *VariablePrefix    = U"{var:";
    static constexpr const Char_T_ *RawVariablePrefix = U"{raw:";
    static constexpr const Char_T_ *MathPrefix        = U"{math:";
    static constexpr const Char_T_ *InLineIfPrefix    = U"{if:";
    static constexpr const Char_T_ *LoopPrefix        = U"<loop";
    static constexpr const Char_T_ *LoopSuffix        = U"</loop>";
    static constexpr const Char_T_ *IfPrefix          = U"<if";
    static constexpr const Char_T_ *IfSuffix          = U"</if>";
    static constexpr const Char_T_ *ElsePrefix        = U"<else";
    static constexpr const Char_T_ *ElseSuffix        = U"/>";
    static constexpr const Char_T_ *HTMLAnd           = U"&amp;";
    static constexpr const Char_T_ *HTMLLess          = U"&lt;";
    static constexpr const Char_T_ *HTMLGreater       = U"&gt;";
    static constexpr const Char_T_ *HTMLQuote         = U"&quot;";
    static constexpr const Char_T_ *HTMLSingleQuote   = U"&apos;";
};

// wchar_t size = 4
template <>
struct TPStrings<wchar_t, 4> {
    static constexpr const wchar_t *VariablePrefix    = L"{var:";
    static constexpr const wchar_t *RawVariablePrefix = L"{raw:";
    static constexpr const wchar_t *MathPrefix        = L"{math:";
    static constexpr const wchar_t *InLineIfPrefix    = L"{if:";
    static constexpr const wchar_t *LoopPrefix        = L"<loop";
    static constexpr const wchar_t *LoopSuffix        = L"</loop>";
    static constexpr const wchar_t *IfPrefix          = L"<if";
    static constexpr const wchar_t *IfSuffix          = L"</if>";
    static constexpr const wchar_t *ElsePrefix        = L"<else";
    static constexpr const wchar_t *ElseSuffix        = L"/>";
    static constexpr const wchar_t *HTMLAnd           = L"&amp;";
    static constexpr const wchar_t *HTMLLess          = L"&lt;";
    static constexpr const wchar_t *HTMLGreater       = L"&gt;";
    static constexpr const wchar_t *HTMLQuote         = L"&quot;";
    static constexpr const wchar_t *HTMLSingleQuote   = L"&apos;";
};

// wchar_t size = 2
template <>
struct TPStrings<wchar_t, 2> {
    static constexpr const wchar_t *VariablePrefix    = L"{var:";
    static constexpr const wchar_t *RawVariablePrefix = L"{raw:";
    static constexpr const wchar_t *MathPrefix        = L"{math:";
    static constexpr const wchar_t *InLineIfPrefix    = L"{if:";
    static constexpr const wchar_t *LoopPrefix        = L"<loop";
    static constexpr const wchar_t *LoopSuffix        = L"</loop>";
    static constexpr const wchar_t *IfPrefix          = L"<if";
    static constexpr const wchar_t *IfSuffix          = L"</if>";
    static constexpr const wchar_t *ElsePrefix        = L"<else";
    static constexpr const wchar_t *ElseSuffix        = L"/>";
    static constexpr const wchar_t *HTMLAnd           = L"&amp;";
    static constexpr const wchar_t *HTMLLess          = L"&lt;";
    static constexpr const wchar_t *HTMLGreater       = L"&gt;";
    static constexpr const wchar_t *HTMLQuote         = L"&quot;";
    static constexpr const wchar_t *HTMLSingleQuote   = L"&apos;";
};
} // namespace Qentem

#endif
