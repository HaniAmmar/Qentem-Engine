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
 *  - <loop set="s"? value="s"? repeat="var|n"? index="var|n"? group="s"? sort="ascend|descend"?>...</loop>
 *
 *      - s: String, n: Number, var: Raw ariable,
 *      - set: child name in the passed colloction: Optional.
 *      - value: the current value in the colloction: Optional.
 *          Note: Choose a unique name.
 *      - repeat: if set, it will be used instead of the "set" size.
 *      - index: starting index.
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
 *
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
 *
 * <loop value="loop1-value" sort="ascend|descend">
 *     loop1-value
 * </loop>
 *
 * <loop value="loop1-value" group="s">
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

template <typename, typename, typename>
struct TemplateSub;

template <typename>
struct TemplatePatterns_T_;

struct Template {
    Template()                            = delete;
    Template(Template &&)                 = delete;
    Template(const Template &)            = delete;
    Template &operator=(Template &&)      = delete;
    Template &operator=(const Template &) = delete;
    ~Template()                           = delete;

    template <typename>
    struct TagBit;

    template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    inline static void CachedRender(const Char_T_ *content, const SizeT length, const Value_T_ &root_value,
                                    StringStream_T_ &stream, const Char_T_ *name, const SizeT name_length) {
        // This is not a thread safe function, and its here to show how to cache geterated tags.
        // Use this in a single threaded prosess, or change it.

        // Usage:
        // CachedRender("<html>...</html>", 16, value, stringstream, "page1", 5);

        static HArray<Array<TagBit<Char_T_>>, Char_T_> cache;

        Array<TagBit<Char_T_>> &tags = cache.GetOrAdd(name, name_length);

        if (tags.IsEmpty()) {
            GenerateTags<Char_T_, Value_T_, StringStream_T_>(content, length, tags);
        }

        RenderOnly(content, root_value, stream, tags);
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    inline static void Render(const Char_T_ *content, SizeT length, const Value_T_ &root_value, StringStream_T_ &stream,
                              Array<TagBit<Char_T_>> &tags_cache) {
        if (tags_cache.IsEmpty()) {
            GenerateTags<Char_T_, Value_T_, StringStream_T_>(content, length, tags_cache);
        }

        RenderOnly(content, root_value, stream, tags_cache);
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    inline static void RenderOnly(const Char_T_ *content, const Value_T_ &root_value, StringStream_T_ &stream,
                                  const Array<TagBit<Char_T_>> &tags_cache) {
        using TemplateSubCV = TemplateSub<Char_T_, Value_T_, StringStream_T_>;
        // tags_cache should not be empty. Use GenerateTags() once befor calling this function in a loop.
        const TemplateSubCV temp{&stream, &root_value};
        temp.render(content, tags_cache);
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    inline static void GenerateTags(const Char_T_ *content, const SizeT length, Array<TagBit<Char_T_>> &tags_cache) {
        using TemplateSubCV = TemplateSub<Char_T_, Value_T_, StringStream_T_>;
        // tags_cache should be empty.
        TemplateSubCV::parse(tags_cache, content, length, 0);
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    inline static void Render(const Char_T_ *content, const SizeT length, const Value_T_ &root_value,
                              StringStream_T_ &stream) {
        Array<TagBit<Char_T_>> tags_cache;
        Render(content, length, root_value, stream, tags_cache);
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_ = StringStream<Char_T_>>
    inline static StringStream_T_ Render(const Char_T_ *content, const SizeT length, const Value_T_ &root_value) {
        StringStream_T_ stream;
        Render(content, length, root_value, stream);
        return stream;
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_ = StringStream<Char_T_>>
    inline static StringStream_T_ Render(const Char_T_ *content, const Value_T_ &root_value) {
        return Render(content, StringUtils::Count(content), root_value);
    }

    enum struct TagType : unsigned char {
        None = 0,
        Variable,    // {var:x}
        RawVariable, // {raw:x}
        Math,        // {math:x}
        InLineIf,    // {if:x}
        Loop,        // <loop ...></loop>
        If,          // <if case="..."></if>
        RawText
    };

    template <typename Char_T_>
    struct LoopTag_T_ {
        // TODO: Remove StringStream;
        StringStream<Char_T_>  InnerTemplate;
        Array<TagBit<Char_T_>> SubTags;
        SizeT                  Offset;
        SizeT                  Length;
        unsigned short         SetOffset;
        unsigned short         SetLength;
        unsigned short         FixedIndex;
        unsigned short         IndexOffset;
        unsigned short         IndexLength;
        unsigned short         FixedSize;
        unsigned short         RepeatOffset;
        unsigned short         RepeatLength;
        unsigned short         GroupOffset;
        unsigned short         GroupLength;
        unsigned short         SortOffset;
        unsigned short         SortLength;
    };

    template <typename Char_T_>
    struct InlineIfTag_T_ {
        Array<TagBit<Char_T_>> TrueSubTags;
        Array<TagBit<Char_T_>> FalseSubTags;
        SizeT                  Offset;
        SizeT                  Length;
        SizeT                  CaseOffset;
        SizeT                  CaseLength;
        SizeT                  TrueOffset;
        SizeT                  TrueLength;
        SizeT                  FalseOffset;
        SizeT                  FalseLength;
    };

    template <typename Char_T_>
    struct IfTagCase_T_ {
        Array<TagBit<Char_T_>> SubTags;
        const SizeT            CaseOffset;
        const SizeT            CaseLength;
        const SizeT            ContentOffset;
        const SizeT            ContentLength;
    };

    template <typename Char_T_>
    struct IfTag_T_ {
        Array<IfTagCase_T_<Char_T_>> Cases;
        SizeT                        Offset;
        SizeT                        Length;
    };

    template <typename Char_T_>
    struct TagBit {
      public:
        TagBit()                             = delete;
        TagBit(const TagBit &tag)            = delete;
        TagBit &operator=(const TagBit &tag) = delete;
        TagBit &operator=(TagBit &&)         = delete;

        TagBit(TagBit &&tag) noexcept : content_(tag.content_), type_{tag.type_} {
            // tag.content_ = {0, 0};
            tag.type_ = TagType::None;
        }

        TagBit(TagType type, SizeT offset, SizeT length_) : type_{type} {
            switch (type) {
                case TagType::Loop: {
                    loop_info_         = Memory::AllocateInit<LoopTag_T_<Char_T_>>();
                    loop_info_->Offset = offset;
                    loop_info_->Length = length_;
                    break;
                }

                case TagType::InLineIf: {
                    i_if_info_         = Memory::AllocateInit<InlineIfTag_T_<Char_T_>>();
                    i_if_info_->Offset = offset;
                    i_if_info_->Length = length_;
                    break;
                }

                case TagType::If: {
                    if_info_         = Memory::AllocateInit<IfTag_T_<Char_T_>>();
                    if_info_->Offset = offset;
                    if_info_->Length = length_;
                    break;
                }

                default: {
                    content_.offset  = offset;
                    content_.length_ = length_;
                }
            }
        }

        ~TagBit() {
            if (loop_info_ != nullptr) {
                switch (type_) {
                    case TagType::Loop: {
                        Memory::Dispose(loop_info_);
                        Memory::Deallocate(loop_info_);
                        break;
                    }

                    case TagType::InLineIf: {
                        Memory::Dispose(i_if_info_);
                        Memory::Deallocate(i_if_info_);
                        break;
                    }

                    case TagType::If: {
                        Memory::Dispose(if_info_);
                        Memory::Deallocate(if_info_);
                        break;
                    }

                    default: {
                    }
                }
            }
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
        inline LoopTag_T_<Char_T_>     *GetLoopInfo() const noexcept { return loop_info_; }
        inline InlineIfTag_T_<Char_T_> *GetInlineIfInfo() const noexcept { return i_if_info_; }
        inline IfTag_T_<Char_T_>       *GetIfInfo() const noexcept { return if_info_; }
#pragma GCC diagnostic pop

        inline TagType GetType() const noexcept { return type_; }
        inline SizeT   GetOffset() const noexcept { return content_.offset; }
        inline SizeT   GetLength() const noexcept { return content_.length_; }

      private:
        struct Content_ {
            SizeT offset;
            SizeT length_;
        };

        union {
            LoopTag_T_<Char_T_>     *loop_info_;
            InlineIfTag_T_<Char_T_> *i_if_info_;
            IfTag_T_<Char_T_>       *if_info_;
            Content_                 content_{0, 0};
        };

        TagType type_{TagType::None};
    };
};

template <typename Char_T_, typename Value_T_, typename StringStream_T_>
struct TemplateSub {
    TemplateSub() = delete;

  private:
    friend struct Qentem::ALE;
    friend struct Qentem::Template;

    using TagBit           = Template::TagBit<Char_T_>;
    using TagType          = Template::TagType;
    using LoopTag          = Template::LoopTag_T_<Char_T_>;
    using InlineIfTag      = Template::InlineIfTag_T_<Char_T_>;
    using IfTagCase        = Template::IfTagCase_T_<Char_T_>;
    using IfTag            = Template::IfTag_T_<Char_T_>;
    using TemplatePatterns = TemplatePatterns_T_<Char_T_>;

    void render(const Char_T_ *content, const Array<TagBit> &tags_cache) const {
        const TagBit *tag = tags_cache.First();
        const TagBit *end = (tag + tags_cache.Size());

        while (tag < end) {
            switch (tag->GetType()) {
                case TagType::Variable: {
                    renderVariable((content + tag->GetOffset()), tag);
                    break;
                }

                case TagType::RawVariable: {
                    renderRawVariable((content + tag->GetOffset()), tag);
                    break;
                }

                case TagType::Math: {
                    renderMath((content + tag->GetOffset()), tag);
                    break;
                }

                case TagType::Loop: {
                    const LoopTag *loop_tag = tag->GetLoopInfo();
                    renderLoop((content + loop_tag->Offset), loop_tag);
                    break;
                }

                case TagType::InLineIf: {
                    const InlineIfTag *i_if_tag = tag->GetInlineIfInfo();
                    renderInLineIf((content + i_if_tag->Offset), i_if_tag);
                    break;
                }

                case TagType::If: {
                    const IfTag *if_tag = tag->GetIfInfo();
                    renderIf((content + if_tag->Offset), if_tag);
                    break;
                }

                case TagType::RawText: {
                    stream_->Insert((content + tag->GetOffset()), tag->GetLength());
                    break;
                }

                default: {
                }
            }

            ++tag;
        }
    }

    static void parse(Array<TagBit> &tags_cache, const Char_T_ *content, const SizeT length, SizeT const level) {
        SizeT offset      = 0;
        SizeT last_offset = 0;

        while (offset < length) {
            if (content[offset] == TemplatePatterns::InLinePrefix) {
                const SizeT current_offset = (offset + 1);

                switch (content[current_offset]) {
                    case TemplatePatterns::Var_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::VariablePrefix + 2), (content + current_offset + 1),
                                                 (TemplatePatterns::VariablePrefixLength - 2))) {
                            const SizeT end_offset = Engine::FindOne<Char_T_>(
                                TemplatePatterns::InLineSuffix, content,
                                (current_offset + TemplatePatterns::VariablePrefixLength), length);

                            if (end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                                }

                                offset += TemplatePatterns::VariablePrefixLength;
                                tags_cache += TagBit{TagType::Variable, offset,
                                                     ((end_offset - TemplatePatterns::InLineSuffixLength) - offset)};
                                last_offset = end_offset;
                                offset      = end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns::Raw_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::RawVariablePrefix + 2),
                                                 (content + current_offset + 1),
                                                 (TemplatePatterns::RawVariablePrefixLength - 2))) {
                            const SizeT end_offset = Engine::FindOne<Char_T_>(
                                TemplatePatterns::InLineSuffix, content,
                                (current_offset + TemplatePatterns::RawVariablePrefixLength), length);

                            if (end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                                }

                                offset += TemplatePatterns::RawVariablePrefixLength;
                                tags_cache += TagBit{TagType::RawVariable, offset,
                                                     ((end_offset - TemplatePatterns::InLineSuffixLength) - offset)};
                                last_offset = end_offset;
                                offset      = end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns::Math_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::MathPrefix + 2), (content + current_offset + 1),
                                                 (TemplatePatterns::MathPrefixLength - 2))) {
                            const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TemplatePatterns::InLinePrefix, TemplatePatterns::InLineSuffix, content,
                                (current_offset + TemplatePatterns::MathPrefixLength - 1), length);

                            if (end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                                }

                                offset += TemplatePatterns::MathPrefixLength;
                                tags_cache += TagBit{TagType::Math, offset,
                                                     ((end_offset - TemplatePatterns::InLineSuffixLength) - offset)};
                                last_offset = end_offset;
                                offset      = end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns::InlineIf_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::InLineIfPrefix + 2), (content + current_offset + 1),
                                                 (TemplatePatterns::InLineIfPrefixLength - 2))) {
                            const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TemplatePatterns::InLinePrefix, TemplatePatterns::InLineSuffix, content,
                                (current_offset + TemplatePatterns::InLineIfPrefixLength - 1), length);

                            if (end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                                }

                                offset += TemplatePatterns::InLineIfPrefixLength;
                                tags_cache += TagBit{TagType::InLineIf, offset,
                                                     ((end_offset - TemplatePatterns::InLineSuffixLength) - offset)};
                                generateInLineIfInfo((content + offset), tags_cache.Last()->GetInlineIfInfo());
                                last_offset = end_offset;
                                offset      = end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    default: {
                    }
                }
            } else if (content[offset] == TemplatePatterns::MultiLinePrefix) { // <loop
                const SizeT current_offset = (offset + 1);

                if (content[current_offset] == TemplatePatterns::Loop_2ND_Char) {
                    if (StringUtils::IsEqual((TemplatePatterns::LoopPrefix + 2), (content + current_offset + 1),
                                             (TemplatePatterns::LoopPrefixLength - 2))) {
                        const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                            TemplatePatterns::LoopPrefix, TemplatePatterns::LoopPrefixLength,
                            TemplatePatterns::LoopSuffix, TemplatePatterns::LoopSuffixLength, content,
                            (current_offset + TemplatePatterns::InLineIfPrefixLength - 1), length);

                        if (end_offset != 0) {
                            if (last_offset != offset) {
                                tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::InLineIfPrefixLength;
                            tags_cache += TagBit{TagType::Loop, offset,
                                                 ((end_offset - TemplatePatterns::LoopSuffixLength) - offset)};
                            generateLoopContent((content + offset), tags_cache.Last()->GetLoopInfo(), level);
                            last_offset = end_offset;
                            offset      = end_offset;
                            continue;
                        }
                    }
                } else if (content[current_offset] == TemplatePatterns::If_2ND_Char) { // <if
                    if (StringUtils::IsEqual((TemplatePatterns::IfPrefix + 2), (content + current_offset + 1),
                                             (TemplatePatterns::IfPrefixLength - 2))) {
                        const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                            TemplatePatterns::IfPrefix, TemplatePatterns::IfPrefixLength, TemplatePatterns::IfSuffix,
                            TemplatePatterns::IfSuffixLength, content, current_offset, length);

                        if (end_offset != 0) {
                            if (last_offset != offset) {
                                tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::IfPrefixLength;
                            tags_cache += TagBit{TagType::If, offset, (end_offset - offset)};
                            generateIfCases((content + offset), tags_cache.Last()->GetIfInfo(), level);
                            last_offset = end_offset;
                            offset      = end_offset;
                            continue;
                        }
                    }
                }
            }

            ++offset;
        }

        if (last_offset != length) {
            tags_cache += TagBit{TagType::RawText, last_offset, (length - last_offset)};
        }
    }

    // Part of parse()
    static void lightParse(Array<TagBit> &tags_cache, const Char_T_ *content, const SizeT length) {
        SizeT offset      = 0;
        SizeT last_offset = 0;

        while (true) {
            offset = Engine::FindOne<Char_T_>(TemplatePatterns::InLinePrefix, content, offset, length);

            if (offset != 0) {
                switch (content[offset]) {
                    case TemplatePatterns::Var_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::VariablePrefix + 2), (content + offset + 1),
                                                 (TemplatePatterns::VariablePrefixLength - 2))) {
                            const SizeT end_offset =
                                Engine::FindOne<Char_T_>(TemplatePatterns::InLineSuffix, content,
                                                         (offset + TemplatePatterns::VariablePrefixLength), length);
                            offset -= TemplatePatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::VariablePrefixLength;
                            tags_cache += TagBit{TagType::Variable, offset,
                                                 ((end_offset - TemplatePatterns::InLineSuffixLength) - offset)};
                            last_offset = end_offset;
                            offset      = end_offset;
                        }

                        continue;
                    }

                    case TemplatePatterns::Raw_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::RawVariablePrefix + 2), (content + offset + 1),
                                                 (TemplatePatterns::RawVariablePrefixLength - 2))) {
                            const SizeT end_offset =
                                Engine::FindOne<Char_T_>(TemplatePatterns::InLineSuffix, content,
                                                         (offset + TemplatePatterns::RawVariablePrefixLength), length);
                            offset -= TemplatePatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::RawVariablePrefixLength;
                            tags_cache += TagBit{TagType::RawVariable, offset,
                                                 ((end_offset - TemplatePatterns::InLineSuffixLength) - offset)};
                            last_offset = end_offset;
                            offset      = end_offset;
                        }

                        continue;
                    }

                    case TemplatePatterns::Math_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::MathPrefix + 2), (content + offset + 1),
                                                 (TemplatePatterns::MathPrefixLength - 2))) {
                            const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TemplatePatterns::InLinePrefix, TemplatePatterns::InLineSuffix, content,
                                (offset + TemplatePatterns::MathPrefixLength - 1), length);
                            offset -= TemplatePatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::MathPrefixLength;
                            tags_cache += TagBit{TagType::Math, offset,
                                                 ((end_offset - TemplatePatterns::InLineSuffixLength) - offset)};
                            last_offset = end_offset;
                            offset      = end_offset;
                        }

                        continue;
                    }

                    default: {
                    }
                }
            }

            break;
        }

        if (last_offset != length) {
            tags_cache += TagBit{TagType::RawText, last_offset, (length - last_offset)};
        }
    }

    static void escapeHTMLSpecialChars(StringStream_T_ &stream, const Char_T_ *str, SizeT length) {
        SizeT offset = 0;
        SizeT index  = 0;

        while (index < length) {
            switch (str[index]) {
                case '&': {
                    stream.Insert((str + offset), (index - offset));
                    stream.Insert(TemplatePatterns::HTMLAnd, TemplatePatterns::HTMLAndLength);
                    offset = (++index);
                    break;
                }

                case '<': {
                    stream.Insert((str + offset), (index - offset));
                    stream.Insert(TemplatePatterns::HTMLLess, TemplatePatterns::HTMLLessLength);
                    offset = (++index);
                    break;
                }

                case '>': {
                    stream.Insert((str + offset), (index - offset));
                    stream.Insert(TemplatePatterns::HTMLBigger, TemplatePatterns::HTMLBiggerLength);
                    offset = (++index);
                    break;
                }

                case '"': {
                    stream.Insert((str + offset), (index - offset));
                    stream.Insert(TemplatePatterns::HTMLQuote, TemplatePatterns::HTMLQuoteLength);
                    offset = (++index);
                    break;
                }

                case '\'': {
                    stream.Insert((str + offset), (index - offset));
                    stream.Insert(TemplatePatterns::HTMLSingleQuote, TemplatePatterns::HTMLSingleQuoteLength);
                    offset = (++index);
                    break;
                }

                default: {
                    ++index;
                }
            }
        }

        stream.Insert((str + offset), (length - offset));
    }

    void renderVariable(const Char_T_ *content, const TagBit *tag) const {
        const Value_T_ *value = findValue(content, tag->GetLength());

        if (value != nullptr) {
            if (Config::AutoEscapeHTML) {
                if (value->IsString()) {
                    const Char_T_ *str;
                    SizeT          len;
                    value->SetCharAndLength(str, len);
                    escapeHTMLSpecialChars(*stream_, str, len);
                    return;
                }
            }

            if (value->CopyStringValueTo(*stream_)) {
                return;
            }

            if ((*content == TemplatePatterns::TildeChar) && (loop_key_ != nullptr)) {
                if (Config::AutoEscapeHTML) {
                    escapeHTMLSpecialChars(*stream_, loop_key_, loop_key_length_);
                } else {
                    stream_->Insert(loop_key_, loop_key_length_);
                }
            }
        }

        if (*content != TemplatePatterns::TildeChar) {
            stream_->Insert((content - TemplatePatterns::VariablePrefixLength),
                            (tag->GetLength() + TemplatePatterns::VariableFulllength));
        }
    }

    void renderRawVariable(const Char_T_ *content, const TagBit *tag) const {
        const Value_T_ *value = findValue(content, tag->GetLength());

        if ((value != nullptr) && value->CopyStringValueTo(*stream_)) {
            return;
        }

        if (*content != TemplatePatterns::TildeChar) {
            stream_->Insert((content - TemplatePatterns::RawVariablePrefixLength),
                            (tag->GetLength() + TemplatePatterns::RawVariableFulllength));
        }
    }

    void renderMath(const Char_T_ *content, const TagBit *tag) const {
        double number;

        if (ALE::Evaluate(number, content, tag->GetLength(), this)) {
            Digit<Char_T_>::NumberToString(*stream_, number, 1, 0, 3);
        } else {
            stream_->Insert((content - TemplatePatterns::MathPrefixLength),
                            (tag->GetLength() + TemplatePatterns::MathFulllength));
        }
    }

    void renderLoop(const Char_T_ *content, const LoopTag *tag) const {
        // Stage 3: Data
        Value_T_        grouped_set;
        const Value_T_ *loop_set   = root_value_;
        SizeT           loop_index = tag->FixedIndex;
        SizeT           loop_size  = tag->FixedSize;

        // Set (Array|Object)
        if (tag->SetLength != 0) {
            loop_set = findValue((content + tag->SetOffset), tag->SetLength);

            if (loop_set == nullptr) {
                return; // set="..." has invalid value.
            }
        }

        // Index
        if (tag->IndexLength != 0) {
            const Value_T_ *tmp_value = findValue((content + tag->IndexOffset), tag->IndexLength);

            if ((tmp_value == nullptr) || !(tmp_value->SetNumber(loop_index))) {
                return; // Not a number
            }
        }

        // Size
        if (tag->RepeatLength != 0) {
            const Value_T_ *tmp_value = findValue((content + tag->RepeatOffset), tag->RepeatLength);

            if ((tmp_value == nullptr) || !(tmp_value->SetNumber(loop_size))) {
                return; // Not a number
            }
        }

        // Group
        if (tag->GroupLength != 0) {
            if (!(loop_set->GroupBy(grouped_set, (content + tag->GroupOffset), tag->GroupLength))) {
                return;
            }

            loop_set = &grouped_set;
        }

        // Sort
        if (tag->SortLength != 0) {
            if (tag->GroupLength == 0) {
                grouped_set = *loop_set;
                loop_set    = &grouped_set;
            }

            grouped_set.Sort(content[tag->SortOffset] == TemplatePatterns::SortAscend);
        }

        if (loop_size == 0) {
            loop_size = loop_set->Size();

            if (loop_size <= loop_index) {
                return;
            }

            loop_size -= loop_index;
        }

        // Stage 4: Render
        TemplateSub    loop_template{stream_, root_value_, this, (level_ + 1)};
        const Char_T_ *loop_content = tag->InnerTemplate.First();

        if (loop_set->IsObject()) {
            do {
                loop_template.loop_value_ = loop_set->GetValue(loop_index);
                loop_set->SetKeyCharAndLength(loop_index, loop_template.loop_key_, loop_template.loop_key_length_);
                loop_template.render(loop_content, tag->SubTags);
                ++loop_index;
            } while ((--loop_size) != 0);
        } else {
            do {
                loop_template.loop_value_ = loop_set->GetValue(loop_index);
                loop_template.render(loop_content, tag->SubTags);
                ++loop_index;
            } while ((--loop_size) != 0);
        }
    }

    void renderInLineIf(const Char_T_ *content, const InlineIfTag *tag) const {
        if (tag->CaseLength != 0) {
            double result;

            if (ALE::Evaluate(result, (content + tag->CaseOffset), tag->CaseLength, this)) {
                if (result > 0.0) {
                    if (tag->TrueLength != 0) {
                        render((content + tag->TrueOffset), tag->TrueSubTags);
                    }
                } else if (tag->FalseLength != 0) {
                    render((content + tag->FalseOffset), tag->FalseSubTags);
                }
            }
        }
    }

    void renderIf(const Char_T_ *content, const IfTag *tag) const {
        const IfTagCase *item = tag->Cases.First();
        const IfTagCase *end  = (item + tag->Cases.Size());
        double           result;

        while (item < end) {
            // <else> without if = (item->CaseLength == 0)
            if ((item->CaseLength == 0) ||
                (ALE::Evaluate(result, (content + item->CaseOffset), item->CaseLength, this) && (result > 0))) {
                render((content + item->ContentOffset), item->SubTags);
                break;
            }

            ++item;
        }
    }

    /*
     * Gets anything between "..."
     */

    static SizeT getQuoted(const Char_T_ *content, SizeT &offset, const SizeT length) noexcept {
        offset = Engine::FindOne<Char_T_>(TemplatePatterns::QuoteChar, content, offset, length);

        if (offset != 0) {
            const SizeT start_offset = Engine::FindOne<Char_T_>(TemplatePatterns::QuoteChar, content, offset, length);

            if (start_offset != 0) {
                return ((start_offset - 1) - offset);
            }
        }

        return 0;
    }

    bool static IsStaticNumber(const Char_T_ *content, const SizeT length) noexcept {
        if (length > TemplatePatterns::VariableFulllength) {
            if (Engine::Find<Char_T_>(TemplatePatterns::VariablePrefix, TemplatePatterns::VariablePrefixLength, content,
                                      SizeT{0}, length) != 0) {
                return false;
            }
        }

        return true;
    }

    void static findKey(const Char_T_ *content, SizeT offset, SizeT end_offset, unsigned short &i_offset,
                        unsigned short &i_length) noexcept {
        offset = Engine::Find<Char_T_>(TemplatePatterns::VariablePrefix, TemplatePatterns::VariablePrefixLength,
                                       content, offset, end_offset);

        i_length = 1;

        if (offset != 0) {
            end_offset = Engine::FindOne<Char_T_>(TemplatePatterns::InLineSuffix, content, offset, end_offset);

            if (end_offset != 0) {
                i_offset = static_cast<unsigned short>(offset);
                i_length = static_cast<unsigned short>(((end_offset - TemplatePatterns::InLineSuffixLength) - offset));
            }
        }
    }

    static void generateLoopContent(const Char_T_ *content, LoopTag *tag, const SizeT level) {
        const Char_T_ *loop_value        = nullptr;
        SizeT          offset            = 0;
        SizeT          tmp_length        = 0;
        SizeT          loop_value_length = 0;
        SizeT          previous_offset   = 0;
        SizeT          tmp_offset;
        bool           break_loop;

        const SizeT start_offset =
            Engine::FindOne<Char_T_>(TemplatePatterns::MultiLineSuffix, content, SizeT{0}, tag->Length);

        // Stage 1: Info extraction
        while (true) {
            ++tmp_length;
            offset += tmp_length; // Move to the next Char_T_.

            tmp_length = getQuoted(content, offset, start_offset);

            if (tmp_length == 0) {
                // The syntax is wrong.
                break;
            }

            // XY="..."
            // 4: Goes back to X
            tmp_offset = (offset - 4);
            break_loop = false;

            do {
                switch (content[tmp_offset]) {
                    case TemplatePatterns::ValueChar: {
                        loop_value        = (content + offset);
                        loop_value_length = tmp_length;
                        break_loop        = true;
                        break;
                    }

                    case TemplatePatterns::SetChar: {
                        tag->SetOffset = static_cast<unsigned short>(offset);
                        tag->SetLength = static_cast<unsigned short>(tmp_length);

                        if ((content[offset] == TemplatePatterns::InLinePrefix) &&
                            (tmp_length > TemplatePatterns::VariableFulllength)) {
                            tag->SetOffset += TemplatePatterns::VariablePrefixLength;
                            tag->SetLength -= static_cast<unsigned short>(TemplatePatterns::VariableFulllength);
                        }

                        break_loop = true;
                        break;
                    }

                    case TemplatePatterns::IndexChar: {
                        const Char_T_ *tmp_content = (content + offset);

                        if (!IsStaticNumber(tmp_content, tmp_length) ||
                            !(Digit<Char_T_>::StringToNumber(tag->FixedIndex, tmp_content, tmp_length))) {
                            findKey(content, offset, (tmp_length + offset), tag->IndexOffset, tag->IndexLength);
                        }

                        break_loop = true;
                        break;
                    }

                    case TemplatePatterns::RepeatChar: {
                        const Char_T_ *tmp_content = (content + offset);

                        if (!IsStaticNumber(tmp_content, tmp_length) ||
                            !(Digit<Char_T_>::StringToNumber(tag->FixedSize, tmp_content, tmp_length))) {
                            findKey(content, offset, (tmp_length + offset), tag->RepeatOffset, tag->RepeatLength);
                        }

                        break_loop = true;
                        break;
                    }

                    case TemplatePatterns::GroupChar: {
                        tag->GroupOffset = static_cast<unsigned short>(offset);
                        tag->GroupLength = static_cast<unsigned short>(tmp_length);
                        break_loop       = true;
                        break;
                    }

                    case TemplatePatterns::SortChar: {
                        tag->SortOffset = static_cast<unsigned short>(offset);
                        tag->SortLength = static_cast<unsigned short>(tmp_length);
                        break_loop      = true;
                        break;
                    }

                    default: {
                    }
                }

                if (break_loop) {
                    break;
                }

            } while (--tmp_offset > previous_offset);

            previous_offset = offset;
        }

        // Stage 2
        offset          = start_offset;
        previous_offset = start_offset;

        if (loop_value != nullptr) {
            while (true) {
                if (loop_value_length > 1U) {
                    offset =
                        Engine::Find<Char_T_>(loop_value, loop_value_length, content, previous_offset, tag->Length);
                } else {
                    offset = Engine::FindOne<Char_T_>(*loop_value, content, previous_offset, tag->Length);
                }

                if (offset == 0) {
                    break;
                }

                tag->InnerTemplate.Insert((content + previous_offset),
                                          ((offset - loop_value_length) - previous_offset));

                tag->InnerTemplate.Insert(TemplatePatterns::VariablePrefix, TemplatePatterns::VariablePrefixLength);

                Char_T_ *buffer = tag->InnerTemplate.Buffer(level + 1);
                SizeT    lvl    = level;

                while (true) {
                    buffer[lvl] = TemplatePatterns::TildeChar;

                    if (lvl == 0) {
                        break;
                    }

                    --lvl;
                }

                SizeT sub_offset = offset;

                while (content[sub_offset] == TemplatePatterns::VariableIndexPrefix) {
                    while ((content[sub_offset] != TemplatePatterns::VariableIndexSuffix) &&
                           (sub_offset < tag->Length)) {
                        ++sub_offset;
                    }

                    ++sub_offset;
                }

                if (sub_offset > tag->Length) {
                    // Unclosed bracket
                    break;
                }

                if ((content[(sub_offset - 1)] == TemplatePatterns::VariableIndexSuffix)) {
                    tag->InnerTemplate.Insert((content + offset), (sub_offset - offset));
                }

                previous_offset = sub_offset;
                tag->InnerTemplate += TemplatePatterns::InLineSuffix;
            }
        }

        tag->InnerTemplate.Insert((content + previous_offset), (tag->Length - previous_offset));

        parse(tag->SubTags, tag->InnerTemplate.First(), tag->InnerTemplate.Length(), (level + 1));
    }

    void static generateInLineIfInfo(const Char_T_ *content, InlineIfTag *tag) {
        SizeT previous_offset = 0;
        SizeT tmp_length      = 0;
        SizeT offset          = 0;
        bool  break_loop;

        while (true) {
            break_loop = false;
            ++tmp_length;
            offset += tmp_length; // Move to the next Char_T_.

            tmp_length = getQuoted(content, offset, tag->Length);

            if (tmp_length == 0) {
                break;
            }

            // = + " + a two chars == 4 + the char before it == 5
            SizeT tmp_offset = (offset - 5);

            do {
                switch (content[tmp_offset]) {
                    case TemplatePatterns::CaseChar: {
                        tag->CaseOffset = offset;
                        tag->CaseLength = tmp_length;
                        break_loop      = true;
                        break;
                    }

                    case TemplatePatterns::TrueChar: {
                        tag->TrueOffset = offset;
                        tag->TrueLength = tmp_length;
                        break_loop      = true;
                        break;
                    }

                    case TemplatePatterns::FalseChar: {
                        tag->FalseOffset = offset;
                        tag->FalseLength = tmp_length;
                        break_loop       = true;
                        break;
                    }

                    default: {
                    }
                }
            } while (!break_loop && (--tmp_offset > previous_offset));

            previous_offset = (offset + tmp_length);
        }

        if (tag->TrueLength != 0) {
            lightParse(tag->TrueSubTags, (content + tag->TrueOffset), tag->TrueLength);
        }

        if (tag->FalseLength != 0) {
            lightParse(tag->FalseSubTags, (content + tag->FalseOffset), tag->FalseLength);
        }
    }

    static void generateIfCases(const Char_T_ *content, IfTag *tag, const SizeT level) {
        Array<TagBit> sub_tags;
        SizeT         case_offset = 0;
        SizeT         case_length;
        SizeT         content_offset;
        SizeT         content_length;

        // The content without </if>
        const SizeT length2 = (tag->Length - TemplatePatterns::IfSuffixLength);

        do {
            case_length = getQuoted(content, case_offset, tag->Length);

            if (case_length != 0) {
                content_offset = Engine::FindOne<Char_T_>(TemplatePatterns::MultiLineSuffix, content,
                                                          (case_offset + case_length + 1), length2);

                if (content_offset != 0) {
                    SizeT else_offset = nextElse(content, content_offset, tag->Length);

                    if (else_offset == 0) {
                        content_length = (length2 - content_offset);
                        parse(sub_tags, (content + content_offset), content_length, level);

                        tag->Cases += IfTagCase{static_cast<Array<TagBit> &&>(sub_tags), case_offset, case_length,
                                                content_offset, content_length};

                        break;
                    }

                    content_length = ((else_offset - TemplatePatterns::ElsePrefixLength) - content_offset);
                    parse(sub_tags, (content + content_offset), content_length, level);

                    tag->Cases += IfTagCase{static_cast<Array<TagBit> &&>(sub_tags), case_offset, case_length,
                                            content_offset, content_length};

                    if ((content[else_offset] != TemplatePatterns::ElseIfChar)) {
                        else_offset =
                            Engine::FindOne<Char_T_>(TemplatePatterns::MultiLineSuffix, content, else_offset, length2);

                        if (else_offset != 0) {
                            const SizeT else_length = (length2 - else_offset);

                            parse(sub_tags, (content + else_offset), else_length, level);

                            tag->Cases += IfTagCase{static_cast<Array<TagBit> &&>(sub_tags), case_offset, 0,
                                                    else_offset, else_length};
                        }

                        break;
                    }

                    case_offset = else_offset;
                    continue;
                }
            }

            break;
        } while (true);
    }

    static SizeT nextElse(const Char_T_ *content, SizeT offset, const SizeT length) noexcept {
        SizeT else_offset = 0;

        while (true) {
            else_offset = Engine::Find<Char_T_>(TemplatePatterns::ElsePrefix, TemplatePatterns::ElsePrefixLength,
                                                content, offset, length);

            if (else_offset == 0) {
                // No <else.
                break;
            }

            const SizeT next_if = Engine::Find<Char_T_>(TemplatePatterns::IfPrefix, TemplatePatterns::IfPrefixLength,
                                                        content, offset, length);

            if ((next_if == 0) || (else_offset < next_if)) {
                // No nesting <ifs or <else before a sub-if.
                break;
            }

            offset = Engine::Find<Char_T_>(TemplatePatterns::IfSuffix, TemplatePatterns::IfSuffixLength, content,
                                           next_if, length);

            if (else_offset > offset) {
                // <else is after a sub-if.
                break;
            }
        }

        return else_offset;
    }

    const Value_T_ *findValue(const Char_T_ *key, const SizeT length) const noexcept {
        const Value_T_ *value = nullptr;

        if (length != 0) {
            SizeT offset = 0;
            SizeT tmp    = 1;

            if (*key != TemplatePatterns::TildeChar) {
                if ((key[(length - 1)] != TemplatePatterns::VariableIndexSuffix)) {
                    return root_value_->GetValue(key, length);
                }

                // [...]
                while ((tmp < length) && (key[tmp] != TemplatePatterns::VariableIndexPrefix)) {
                    ++tmp;
                }

                value = root_value_;
            } else {
                const TemplateSub *obj = this;

                do {
                    ++offset;
                } while (key[offset] == TemplatePatterns::TildeChar);

                if (offset > level_) {
                    return nullptr;
                }

                SizeT lvl = offset;

                while (lvl < level_) {
                    ++lvl;
                    obj = obj->parent_;
                }

                if ((key[(length - 1)] != TemplatePatterns::VariableIndexSuffix)) {
                    return obj->loop_value_;
                }

                // [...]
                while (key[tmp] != TemplatePatterns::VariableIndexSuffix) {
                    ++tmp;
                }

                value = obj->loop_value_;
                ++offset;
            }

            do {
                value = value->GetValue((key + offset), (tmp - offset));

                ++tmp;

                if (tmp >= length) {
                    break;
                }

                if (key[tmp] == TemplatePatterns::VariableIndexPrefix) {
                    ++tmp;
                }

                offset = tmp;

                while (key[tmp] != TemplatePatterns::VariableIndexSuffix) {
                    ++tmp;
                }
            } while (value != nullptr);
        }

        return value;
    }

    bool ALESetNumber(double &number, const Value_T_ *value) const noexcept {
        if (!(value->SetNumber(number))) {
            const Char_T_ *str;
            SizeT          len;

            return (value->SetCharAndLength(str, len) && ALE::Evaluate(number, str, len, this));
        }

        return true;
    }

    bool ALESetNumber(double &number, const Char_T_ *content, SizeT length) const noexcept {
        const Value_T_ *value = nullptr;

        if (length > TemplatePatterns::VariableFulllength) {
            content += TemplatePatterns::VariablePrefixLength;
            length -= TemplatePatterns::VariableFulllength;
            value = findValue(content, length);
        }

        return ((value != nullptr) && ALESetNumber(number, value));
    }

    bool ALEIsEqual(bool &result, const Char_T_ *content, ALE::Number left, ALE::Number right, bool left_evaluated,
                    bool right_evaluated) const noexcept {
        using ALEExpressions = ALEExpressions_T_<Char_T_>;

        const Value_T_ *left_value  = nullptr;
        const Value_T_ *right_value = nullptr;

        const Char_T_ *left_content  = (content + left.Content.Offset);
        const Char_T_ *right_content = (content + right.Content.Offset);
        SizeT          left_length   = left.Content.Length;
        SizeT          right_length  = right.Content.Length;

        if (!left_evaluated) {
            if ((*left_content == TemplatePatterns::InLinePrefix) &&
                (left_length > TemplatePatterns::VariableFulllength)) {
                left_value = findValue((left_content + TemplatePatterns::VariablePrefixLength),
                                       (left_length - TemplatePatterns::VariableFulllength)); // {var:x}

                if (left_value != nullptr) {
                    if (left_value->IsNumber()) {
                        left_value->SetNumber(left.Number);
                        left_evaluated = true;
                    }
                } else {
                    return false;
                }
            } else {
                if (*left_content != ALEExpressions::ParenthesStart) {
                    left_evaluated = Digit<Char_T_>::StringToNumber(left.Number, left_content, left_length);
                } else {
                    left_evaluated = ALE::Evaluate(left.Number, (left_content + 1), (left_length -= 2), this);
                }
            }
        }

        if (!right_evaluated) {
            if ((*right_content == TemplatePatterns::InLinePrefix) &&
                (right_length > TemplatePatterns::VariableFulllength)) {
                right_value = findValue((right_content + TemplatePatterns::VariablePrefixLength),
                                        (right_length - TemplatePatterns::VariableFulllength)); // {var:x}

                if (right_value != nullptr) {
                    if (right_value->IsNumber()) {
                        right_value->SetNumber(right.Number);
                        right_evaluated = true;
                    }
                } else {
                    return false;
                }
            } else if (left_evaluated) {
                if (*right_content != ALEExpressions::ParenthesStart) {
                    right_evaluated = Digit<Char_T_>::StringToNumber(right.Number, right_content, right_length);
                } else {
                    right_evaluated = ALE::Evaluate(right.Number, (right_content + 1), (right_length -= 2), this);
                }
            }
        }

        if (left_evaluated || right_evaluated) {
            if (left_evaluated || ((left_value != nullptr) && ALESetNumber(left.Number, left_value))) {
                if (right_evaluated || ((right_value != nullptr) && ALESetNumber(right.Number, right_value))) {
                    result = (left.Number == right.Number);
                    return true;
                }
            }
        } else if ((left_value == nullptr) || left_value->SetCharAndLength(left_content, left_length)) {
            if ((right_value == nullptr) || right_value->SetCharAndLength(right_content, right_length)) {
                result =
                    ((left_length == right_length) && StringUtils::IsEqual(left_content, right_content, right_length));
                return true;
            }
        }

        return false;
    }

    TemplateSub(StringStream_T_ *stream, const Value_T_ *root_value, const TemplateSub *parent = nullptr,
                const SizeT level = 0) noexcept
        : stream_{stream}, root_value_{root_value}, parent_{parent}, level_{level} {}

    StringStream_T_   *stream_;
    const Value_T_    *root_value_;
    const TemplateSub *parent_;
    const Value_T_    *loop_value_{nullptr};
    const Char_T_     *loop_key_{nullptr};
    SizeT              loop_key_length_{0};
    const SizeT        level_;
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
    static constexpr SizeT          HTMLBiggerLength = 4U;
    static constexpr const Char_T_ *HTMLBigger       = TPStrings<Char_T_, size_>::HTMLBigger;

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
    static constexpr Char_T_ SetChar    = 's'; // [s]et
    static constexpr Char_T_ ValueChar  = 'l'; // va[l]ue
    static constexpr Char_T_ RepeatChar = 'p'; // re[p]eat
    static constexpr Char_T_ IndexChar  = 'd'; // in[d]ex
    static constexpr Char_T_ GroupChar  = 'o'; // gr[o]up
    static constexpr Char_T_ SortChar   = 'r'; // so[r]t
    static constexpr Char_T_ SortAscend = 'a'; // [a]scend

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
    static constexpr const Char_T_ *HTMLBigger        = "&gt;";
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
    static constexpr const Char_T_ *HTMLBigger        = u"&gt;";
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
    static constexpr const Char_T_ *HTMLBigger        = U"&gt;";
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
    static constexpr const wchar_t *HTMLBigger        = L"&gt;";
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
    static constexpr const wchar_t *HTMLBigger        = L"&gt;";
    static constexpr const wchar_t *HTMLQuote         = L"&quot;";
    static constexpr const wchar_t *HTMLSingleQuote   = L"&apos;";
};

} // namespace Qentem

#endif
