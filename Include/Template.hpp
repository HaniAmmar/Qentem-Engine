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
class Template_CV;

template <typename>
class TemplatePatterns;

class Template {
  public:
    template <typename>
    struct TagBit;

    template <typename Char_T_, typename Value_T_, typename Number_T_, typename StringStream_T_>
    inline static void Render(const Char_T_ *content, Number_T_ length, const Value_T_ *root_value, StringStream_T_ *ss,
                              Array<TagBit<Char_T_>> *tags_cache) {
        Template_CV<Char_T_, Value_T_, StringStream_T_>::Process(content, static_cast<SizeT>(length), root_value, ss,
                                                                 *tags_cache);
    }

    template <typename Char_T_, typename Value_T_, typename Number_T_, typename StringStream_T_>
    inline static void Render(const Char_T_ *content, Number_T_ length, const Value_T_ *root_value,
                              StringStream_T_ *ss) {
        Array<TagBit<Char_T_>> tags_cache;
        Render(content, length, root_value, ss, &tags_cache);
    }

    template <typename Char_T_, typename Value_T_, typename Number_T_, typename StringStream_T_ = StringStream<Char_T_>>
    inline static StringStream_T_ Render(const Char_T_ *content, Number_T_ length, const Value_T_ *root_value) {
        StringStream_T_ ss;
        Render(content, length, root_value, &ss);
        return ss;
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_ = StringStream<Char_T_>>
    inline static StringStream_T_ Render(const Char_T_ *content, const Value_T_ *root_value) {
        return Render(content, StringUtils::Count(content), root_value);
    }

    enum class TagType : unsigned char {
        None = 0,
        Variable,    // {var:x}
        RawVariable, // {raw:x}
        Math,        // {math:x}
        InLineIf,    // {if:x}
        Loop,        // <loop ...></loop>
        If,          // <if case="..."></if>
    };

    template <typename Char_T_>
    struct LoopInfo_T {
        const StringStream<Char_T_>  InnerTemplate;
        const Array<TagBit<Char_T_>> SubTags;
        const unsigned short         SetOffset;
        const unsigned short         SetLength;
        const unsigned short         IndexOffset;
        const unsigned short         IndexLength;
        const unsigned short         RepeatOffset;
        const unsigned short         RepeatLength;
        const unsigned short         GroupOffset;
        const unsigned short         GroupLength;
        const unsigned short         SortOffset;
        const unsigned short         SortLength;
    };

    template <typename Char_T_>
    struct InlineIfInfo_T {
        const Array<TagBit<Char_T_>> TrueSubTags;
        const Array<TagBit<Char_T_>> FalseSubTags;
        const unsigned short         CaseOffset;
        const unsigned short         CaseLength;
        const unsigned short         TrueOffset;
        const unsigned short         TrueLength;
        const unsigned short         FalseOffset;
        const unsigned short         FalseLength;
    };

    template <typename Char_T_>
    struct IfCase_T {
        Array<TagBit<Char_T_>> SubTags;
        const SizeT            CaseOffset;
        const SizeT            CaseLength;
        const SizeT            ContentOffset;
        const SizeT            ContentLength;
    };

    template <typename Char_T_>
    struct TagBit {
      public:
        TagBit()                             = delete;
        TagBit(const TagBit &tag)            = delete;
        TagBit &operator=(const TagBit &tag) = delete;
        TagBit &operator=(TagBit &&)         = delete;

        TagBit(TagType type, SizeT offset, SizeT end_offset) noexcept : offset_(offset), end_offset_(end_offset) {
            setType(type);
        }

        TagBit(TagBit &&tag) noexcept
            : info_(static_cast<QPointer<void> &&>(tag.info_)), offset_(tag.offset_), end_offset_(tag.end_offset_) {
#if !defined(QENTEM_POINTER_TAGGING) || (QENTEM_POINTER_TAGGING != 1)
            type_ = tag.type_;
#endif
            tag.setType(TagType::None);
        }

        ~TagBit() {
            switch (GetType()) {
                case TagType::Loop: {
                    LoopInfo_T<Char_T_> *loop_info = static_cast<LoopInfo_T<Char_T_> *>(info_.GetPointer());
                    Memory::Dispose(loop_info);
                    Memory::Deallocate(loop_info);
                    break;
                }

                case TagType::If: {
                    Array<IfCase_T<Char_T_>> *if_info = static_cast<Array<IfCase_T<Char_T_>> *>(info_.GetPointer());
                    Memory::Dispose(if_info);
                    Memory::Deallocate(if_info);
                    break;
                }

                case TagType::InLineIf: {
                    InlineIfInfo_T<Char_T_> *inline_if_info =
                        static_cast<InlineIfInfo_T<Char_T_> *>(info_.GetPointer());
                    Memory::Dispose(inline_if_info);
                    Memory::Deallocate(inline_if_info);
                    break;
                }

                default: {
                }
            }
        }

        void         SetInfo(void *ptr) noexcept { info_.SetPointer(ptr); }
        inline void *GetPointer() const noexcept { return info_.GetPointer(); }

        inline TagType GetType() const noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
            return static_cast<TagType>(info_.GetHighTag());
#else
            return type_;
#endif
        }

        inline SizeT Offset() const noexcept { return offset_; }
        inline SizeT EndOffset() const noexcept { return end_offset_; }

      private:
        void setType(TagType type) noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
            info_.SetHighTag(static_cast<unsigned char>(type));
#else
            type_ = type;
#endif
        }

        QPointer<void> info_{};
        SizeT          offset_{0};
        SizeT          end_offset_{0};
#if !defined(QENTEM_POINTER_TAGGING) || (QENTEM_POINTER_TAGGING != 1)
        TagType type_{};
#endif
    };
};

template <typename Char_T_, typename Value_T_, typename StringStream_T_>
class Template_CV {
  private:
    friend class Qentem::ALE;
    friend class Qentem::Template;

    using TagBit        = Template::TagBit<Char_T_>;
    using TagType       = Template::TagType;
    using LoopInfo_     = Template::LoopInfo_T<Char_T_>;
    using IfCase_       = Template::IfCase_T<Char_T_>;
    using IfInfo_       = Array<IfCase_>;
    using InlineIfInfo_ = Template::InlineIfInfo_T<Char_T_>;

    using TemplatePatterns_C_ = TemplatePatterns<Char_T_>;

  public:
    Template_CV() = delete;

    static void Process(const Char_T_ *content, SizeT length, const Value_T_ *root_value, StringStream_T_ *ss,
                        Array<TagBit> &tags_cache) {
        const Template_CV temp{ss, root_value};

        if (tags_cache.IsEmpty()) {
            parse(tags_cache, content, length);
        }

        temp.process(content, length, tags_cache);
    }

  private:
    void process(const Char_T_ *content, SizeT length, const Array<TagBit> &tags_cache) const {
        SizeT offset = 0;

        if (tags_cache.IsNotEmpty()) {
            TagBit       *start = tags_cache.Storage();
            const TagBit *end   = (start + tags_cache.Size());
            render(start, end, content);
            offset = (end - 1)->EndOffset();
        }

        // Add the remaining string.
        ss_->Insert((content + offset), (length - offset));
    }

    QENTEM_NOINLINE static void parse(Array<TagBit> &tags_cache, const Char_T_ *content, SizeT length) {
        SizeT offset = 0;

        while (offset < length) {
            if (content[offset] == TemplatePatterns_C_::InLinePrefix) {
                const SizeT current_offset = (offset + 1);

                switch (content[current_offset]) {
                    case TemplatePatterns_C_::Var_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns_C_::GetVariablePrefix() + 2),
                                                 (content + current_offset + 1),
                                                 (TemplatePatterns_C_::VariablePrefixLength - 2))) {
                            const SizeT end_offset = Engine::FindOne<Char_T_>(
                                TemplatePatterns_C_::InLineSuffix, content,
                                (current_offset + TemplatePatterns_C_::VariablePrefixLength), length);

                            if (end_offset != 0) {
                                tags_cache += TagBit{TagType::Variable, offset, end_offset};
                                offset = end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns_C_::Raw_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns_C_::GetRawVariablePrefix() + 2),
                                                 (content + current_offset + 1),
                                                 (TemplatePatterns_C_::RawVariablePrefixLength - 2))) {
                            const SizeT end_offset = Engine::FindOne<Char_T_>(
                                TemplatePatterns_C_::InLineSuffix, content,
                                (current_offset + TemplatePatterns_C_::RawVariablePrefixLength), length);

                            if (end_offset != 0) {
                                tags_cache += TagBit{TagType::RawVariable, offset, end_offset};
                                offset = end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns_C_::Math_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns_C_::GetMathPrefix() + 2),
                                                 (content + current_offset + 1),
                                                 (TemplatePatterns_C_::MathPrefixLength - 2))) {
                            const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TemplatePatterns_C_::InLinePrefix, TemplatePatterns_C_::InLineSuffix, content,
                                (current_offset + TemplatePatterns_C_::MathPrefixLength - 1), length);

                            if (end_offset != 0) {
                                tags_cache += TagBit{TagType::Math, offset, end_offset};
                                offset = end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns_C_::InlineIf_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns_C_::GetInLineIfPrefix() + 2),
                                                 (content + current_offset + 1),
                                                 (TemplatePatterns_C_::InLineIfPrefixLength - 2))) {
                            const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TemplatePatterns_C_::InLinePrefix, TemplatePatterns_C_::InLineSuffix, content,
                                (current_offset + TemplatePatterns_C_::InLineIfPrefixLength - 1), length);

                            if (end_offset != 0) {
                                tags_cache += TagBit{TagType::InLineIf, offset, end_offset};
                                offset = end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    default: {
                    }
                }
            } else if (content[offset] == TemplatePatterns_C_::MultiLinePrefix) {
                const SizeT current_offset = (offset + 1);

                if (content[current_offset] == TemplatePatterns_C_::Loop_2ND_Char) { // <loop
                    if (StringUtils::IsEqual((TemplatePatterns_C_::GetLoopPrefix() + 2), (content + current_offset + 1),
                                             (TemplatePatterns_C_::LoopPrefixLength - 2))) {
                        const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                            TemplatePatterns_C_::GetLoopPrefix(), TemplatePatterns_C_::LoopPrefixLength,
                            TemplatePatterns_C_::GetLoopSuffix(), TemplatePatterns_C_::LoopSuffixLength, content,
                            (current_offset + TemplatePatterns_C_::InLineIfPrefixLength - 1), length);

                        if (end_offset != 0) {
                            tags_cache += TagBit{TagType::Loop, offset, end_offset};
                            offset = end_offset;
                            continue;
                        }
                    }
                } else if (content[current_offset] == TemplatePatterns_C_::If_2ND_Char) { // <if
                    if (StringUtils::IsEqual((TemplatePatterns_C_::GetIfPrefix() + 2), (content + current_offset + 1),
                                             (TemplatePatterns_C_::IfPrefixLength - 2))) {
                        const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                            TemplatePatterns_C_::GetIfPrefix(), TemplatePatterns_C_::IfPrefixLength,
                            TemplatePatterns_C_::GetIfSuffix(), TemplatePatterns_C_::IfSuffixLength, content,
                            current_offset, length);

                        if (end_offset != 0) {
                            tags_cache += TagBit{TagType::If, offset, end_offset};
                            offset = end_offset;
                            continue;
                        }
                    }
                }
            }

            ++offset;
        }
    }

    // Part of parse()
    QENTEM_NOINLINE static void light_parse(Array<TagBit> &tags_cache, const Char_T_ *content, SizeT length) {
        SizeT offset = 0;

        while (true) {
            offset = Engine::FindOne<Char_T_>(TemplatePatterns_C_::InLinePrefix, content, offset, length);

            if (offset == 0) {
                break;
            }

            switch (content[offset]) {
                case TemplatePatterns_C_::Var_2ND_Char: {
                    if (StringUtils::IsEqual((TemplatePatterns_C_::GetVariablePrefix() + 2), (content + offset + 1),
                                             (TemplatePatterns_C_::VariablePrefixLength - 2))) {
                        const SizeT end_offset =
                            Engine::FindOne<Char_T_>(TemplatePatterns_C_::InLineSuffix, content,
                                                     (offset + TemplatePatterns_C_::VariablePrefixLength), length);
                        // end_offset cannot be zero

                        tags_cache +=
                            TagBit{TagType::Variable, (offset - TemplatePatterns_C_::InLinePrefixLength), end_offset};
                        offset = end_offset;
                    }

                    break;
                }

                case TemplatePatterns_C_::Raw_2ND_Char: {
                    if (StringUtils::IsEqual((TemplatePatterns_C_::GetRawVariablePrefix() + 2), (content + offset + 1),
                                             (TemplatePatterns_C_::RawVariablePrefixLength - 2))) {
                        const SizeT end_offset =
                            Engine::FindOne<Char_T_>(TemplatePatterns_C_::InLineSuffix, content,
                                                     (offset + TemplatePatterns_C_::RawVariablePrefixLength), length);
                        tags_cache += TagBit{TagType::RawVariable, (offset - TemplatePatterns_C_::InLinePrefixLength),
                                             end_offset};
                        offset = end_offset;
                    }

                    break;
                }

                case TemplatePatterns_C_::Math_2ND_Char: {
                    if (StringUtils::IsEqual((TemplatePatterns_C_::GetMathPrefix() + 2), (content + offset + 1),
                                             (TemplatePatterns_C_::MathPrefixLength - 2))) {
                        const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                            TemplatePatterns_C_::InLinePrefix, TemplatePatterns_C_::InLineSuffix, content,
                            (offset + TemplatePatterns_C_::MathPrefixLength - 1), length);
                        tags_cache +=
                            TagBit{TagType::Math, (offset - TemplatePatterns_C_::InLinePrefixLength), end_offset};
                        offset = end_offset;
                    }

                    break;
                }

                default: {
                }
            }
        }
    }

    QENTEM_NOINLINE void render(TagBit *tag, const TagBit *end, const Char_T_ *content) const {
        SizeT previous_offset = 0;

        while (tag != end) {
            ss_->Insert((content + previous_offset), (tag->Offset() - previous_offset));

            switch (tag->GetType()) {
                case TagType::Variable: {
                    const SizeT content_offset = (tag->Offset() + TemplatePatterns_C_::VariablePrefixLength);

                    renderVariable((content + content_offset),
                                   ((tag->EndOffset() - TemplatePatterns_C_::InLineSuffixLength) - content_offset));
                    break;
                }

                case TagType::RawVariable: {
                    const SizeT content_offset = (tag->Offset() + TemplatePatterns_C_::VariablePrefixLength);

                    renderRawVariable((content + content_offset),
                                      ((tag->EndOffset() - TemplatePatterns_C_::InLineSuffixLength) - content_offset));
                    break;
                }

                case TagType::Math: {
                    const SizeT content_offset = (tag->Offset() + TemplatePatterns_C_::MathPrefixLength);

                    renderMath((content + content_offset),
                               ((tag->EndOffset() - TemplatePatterns_C_::InLineSuffixLength) - content_offset));
                    break;
                }

                case TagType::Loop: {
                    const SizeT    content_offset = (tag->Offset() + TemplatePatterns_C_::LoopPrefixLength);
                    const Char_T_ *loop_content   = (content + content_offset);
                    LoopInfo_     *loop_info      = static_cast<LoopInfo_ *>(tag->GetPointer());

                    if (loop_info == nullptr) {
                        generateLoopContent(
                            loop_content, ((tag->EndOffset() - TemplatePatterns_C_::LoopSuffixLength) - content_offset),
                            loop_info);
                        tag->SetInfo(loop_info);
                    }

                    renderLoop(loop_content, loop_info);

                    break;
                }

                case TagType::InLineIf: {
                    const SizeT    content_offset    = (tag->Offset() + TemplatePatterns_C_::InLineIfPrefixLength);
                    const Char_T_ *inline_if_content = (content + content_offset);
                    InlineIfInfo_ *inline_if_info    = static_cast<InlineIfInfo_ *>(tag->GetPointer());

                    if (inline_if_info == nullptr) {
                        generateInLineIfInfo(
                            inline_if_content,
                            ((tag->EndOffset() - TemplatePatterns_C_::InLineSuffixLength) - content_offset),
                            inline_if_info);
                        tag->SetInfo(inline_if_info);
                    }

                    renderInLineIf(inline_if_content, inline_if_info);

                    break;
                }

                case TagType::If: {
                    const SizeT    content_offset  = tag->Offset() + TemplatePatterns_C_::IfPrefixLength;
                    const Char_T_ *if_info_content = (content + content_offset);
                    IfInfo_       *if_info         = static_cast<IfInfo_ *>(tag->GetPointer());

                    if (if_info == nullptr) {
                        generateIfCases(if_info_content, (tag->EndOffset() - content_offset), if_info);
                        tag->SetInfo(if_info);
                    }

                    renderIf(if_info_content, if_info);

                    break;
                }

                default: {
                }
            }

            previous_offset = tag->EndOffset();
            ++tag;
        }
    }

#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
    void escapeHTMLSpecialChars(const Char_T_ *str, SizeT len) const {
        SizeT offset = 0;

        for (SizeT i = 0; i < len; i++) {
            switch (str[i]) {
                case '&': {
                    ss_->Insert((str + offset), (i - offset));
                    offset = (i + 1);
                    ss_->Insert(TemplatePatterns_C_::GetHTMLAnd(), TemplatePatterns_C_::HTMLAndLength);
                    break;
                }

                case '<': {
                    ss_->Insert((str + offset), (i - offset));
                    offset = (i + 1);
                    ss_->Insert(TemplatePatterns_C_::GetHTMLLess(), TemplatePatterns_C_::HTMLLessLength);
                    break;
                }

                case '>': {
                    ss_->Insert((str + offset), (i - offset));
                    offset = (i + 1);
                    ss_->Insert(TemplatePatterns_C_::GetHTMLBigger(), TemplatePatterns_C_::HTMLBiggerLength);
                    break;
                }

                case '"': {
                    ss_->Insert((str + offset), (i - offset));
                    offset = (i + 1);
                    ss_->Insert(TemplatePatterns_C_::GetHTMLQuote(), TemplatePatterns_C_::HTMLQuoteLength);
                    break;
                }

                case '\'': {
                    ss_->Insert((str + offset), (i - offset));
                    offset = (i + 1);
                    ss_->Insert(TemplatePatterns_C_::GetHTMLSingleQuote(), TemplatePatterns_C_::HTMLSingleQuoteLength);
                    break;
                }

                default: {
                }
            }
        }

        ss_->Insert((str + offset), (len - offset));
    }
#endif

    void renderVariable(const Char_T_ *content, SizeT length) const {
        const Value_T_ *value = findValue(content, length);

        if (value != nullptr) {
#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
            if (value->IsString()) {
                const Char_T_ *str;
                SizeT          len;
                value->SetCharAndLength(str, len);
                escapeHTMLSpecialChars(str, len);
                return;
            }
#endif

            if (value->CopyStringValueTo(*ss_)) {
                return;
            }

            if ((loop_key_ != nullptr) && (*content == TemplatePatterns_C_::TildeChar)) {
#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
                escapeHTMLSpecialChars(loop_key_, loop_key_length_);
#else
                ss_->Insert(loop_key_, loop_key_length_);
#endif
            }
        }

        if (*content != TemplatePatterns_C_::TildeChar) {
            ss_->Insert((content - TemplatePatterns_C_::VariablePrefixLength),
                        (length + TemplatePatterns_C_::VariableFulllength));
        }
    }

    void renderRawVariable(const Char_T_ *content, SizeT length) const {
        const Value_T_ *value = findValue(content, length);

        if ((value != nullptr) && value->CopyStringValueTo(*ss_)) {
            return;
        }

        if (*content != TemplatePatterns_C_::TildeChar) {
            ss_->Insert((content - TemplatePatterns_C_::RawVariablePrefixLength),
                        (length + TemplatePatterns_C_::RawVariableFulllength));
        }
    }

    void renderMath(const Char_T_ *content, SizeT length) const {
        double number;

        if (ALE::Evaluate(number, content, length, this)) {
            Digit<Char_T_>::NumberToStringStream(*ss_, number, 1, 0, 3);
        } else {
            ss_->Insert((content - TemplatePatterns_C_::MathPrefixLength),
                        (length + TemplatePatterns_C_::MathFulllength));
        }
    }

    /*
     * Gets anything between "..."
     */

    SizeT getQuoted(const Char_T_ *content, SizeT &offset, SizeT length) const noexcept {
        offset = Engine::FindOne<Char_T_>(TemplatePatterns_C_::QuoteChar, content, offset, length);

        if (offset != 0) {
            const SizeT start_offset =
                Engine::FindOne<Char_T_>(TemplatePatterns_C_::QuoteChar, content, offset, length);

            if (start_offset != 0) {
                return ((start_offset - 1) - offset);
            }
        }

        return 0;
    }

    QENTEM_NOINLINE bool parseNumber(SizeT &number, const Char_T_ *content, const SizeT length) const noexcept {
        if (length > TemplatePatterns_C_::VariableFulllength) {
            SizeT offset = 0;
            offset       = Engine::Find<Char_T_>(TemplatePatterns_C_::GetVariablePrefix(),
                                           TemplatePatterns_C_::VariablePrefixLength, content, offset, length);

            if (offset != 0) {
                const SizeT end_offset =
                    Engine::FindOne<Char_T_>(TemplatePatterns_C_::InLineSuffix, content, offset, length);

                if (end_offset == 0) {
                    return false;
                }

                const Value_T_ *value =
                    findValue((content + offset), ((end_offset - TemplatePatterns_C_::InLineSuffixLength) - offset));

                return ((value != nullptr) && (value->SetNumber(number)));
            }
        }

        return Digit<Char_T_>::StringToNumber(number, content, length);
    }

    QENTEM_NOINLINE void generateLoopContent(const Char_T_ *content, SizeT length, LoopInfo_ *&loop_info) const {
        const SizeT start_offset =
            Engine::FindOne<Char_T_>(TemplatePatterns_C_::MultiLineSuffix, content, SizeT{0}, length);

        const Char_T_ *loop_value        = nullptr;
        SizeT          loop_value_length = 0;
        SizeT          len               = 0;
        SizeT          offset            = 0;
        SizeT          previous_offset   = 0;
        bool           break_loop        = false;

        Array<TagBit>   sub_tags;
        StringStream_T_ inner_template;
        unsigned short  set_offset    = 0;
        unsigned short  set_length    = 0;
        unsigned short  index_offset  = 0;
        unsigned short  index_length  = 0;
        unsigned short  repeat_offset = 0;
        unsigned short  repeat_length = 0;
        unsigned short  group_offset  = 0;
        unsigned short  group_length  = 0;
        unsigned short  sort_offset   = 0;
        unsigned short  sort_length   = 0;

        // Stage 1: Info extraction
        while (true) {
            ++len;
            offset += len; // Move to the next Char_T_.

            len = getQuoted(content, offset, start_offset);

            if (len == 0) {
                // The syntax is wrong.
                break;
            }

            // XY="..."
            // 4: Goes back to X
            SizeT tmp_offset = (offset - 4);

            do {
                switch (content[tmp_offset]) {
                    case TemplatePatterns_C_::ValueChar: {
                        loop_value        = (content + offset);
                        loop_value_length = len;
                        break_loop        = true;
                        break;
                    }

                    case TemplatePatterns_C_::SetChar: {
                        set_offset = static_cast<unsigned short>(offset);
                        set_length = static_cast<unsigned short>(len);

                        if ((content[offset] == TemplatePatterns_C_::InLinePrefix) &&
                            (len > TemplatePatterns_C_::VariableFulllength)) {
                            set_offset += TemplatePatterns_C_::VariablePrefixLength;
                            set_length -= static_cast<unsigned short>(TemplatePatterns_C_::VariableFulllength);
                        }

                        break_loop = true;
                        break;
                    }

                    case TemplatePatterns_C_::IndexChar: {
                        index_offset = static_cast<unsigned short>(offset);
                        index_length = static_cast<unsigned short>(len);
                        break_loop   = true;
                        break;
                    }

                    case TemplatePatterns_C_::RepeatChar: {
                        repeat_offset = static_cast<unsigned short>(offset);
                        repeat_length = static_cast<unsigned short>(len);
                        break_loop    = true;
                        break;
                    }

                    case TemplatePatterns_C_::GroupChar: {
                        group_offset = static_cast<unsigned short>(offset);
                        group_length = static_cast<unsigned short>(len);
                        break_loop   = true;
                        break;
                    }

                    case TemplatePatterns_C_::SortChar: {
                        sort_offset = static_cast<unsigned short>(offset);
                        sort_length = static_cast<unsigned short>(len);
                        break_loop  = true;
                        break;
                    }

                    default: {
                    }
                }

                if (break_loop) {
                    break;
                }

            } while (--tmp_offset > previous_offset);

            break_loop      = false;
            previous_offset = offset;
        }

        // Stage 2
        offset          = start_offset;
        previous_offset = start_offset;

        if (loop_value != nullptr) {
            while (true) {
                if (loop_value_length > 1U) {
                    offset = Engine::Find<Char_T_>(loop_value, loop_value_length, content, previous_offset, length);
                } else {
                    offset = Engine::FindOne<Char_T_>(*loop_value, content, previous_offset, length);
                }

                if (offset == 0) {
                    break;
                }

                inner_template.Insert((content + previous_offset), ((offset - loop_value_length) - previous_offset));

                inner_template.Insert(TemplatePatterns_C_::GetVariablePrefix(),
                                      TemplatePatterns_C_::VariablePrefixLength);

                SizeT    lvl    = level_;
                Char_T_ *buffer = inner_template.Buffer(level_ + 1);

                while (true) {
                    buffer[lvl] = TemplatePatterns_C_::TildeChar;

                    if (lvl == 0) {
                        break;
                    }

                    --lvl;
                }

                SizeT sub_offset = offset;

                while (content[sub_offset] == TemplatePatterns_C_::VariableIndexPrefix) {
                    while ((content[sub_offset] != TemplatePatterns_C_::VariableIndexSuffix) && (sub_offset < length)) {
                        ++sub_offset;
                    }

                    ++sub_offset;
                }

                if (sub_offset > length) {
                    // Unclosed bracket
                    break;
                }

                if ((content[(sub_offset - 1)] == TemplatePatterns_C_::VariableIndexSuffix)) {
                    inner_template.Insert((content + offset), (sub_offset - offset));
                }

                previous_offset = sub_offset;
                inner_template += TemplatePatterns_C_::InLineSuffix;
            }
        }

        inner_template.Insert((content + previous_offset), (length - previous_offset));

        parse(sub_tags, inner_template.First(), inner_template.Length());

        loop_info = Memory::AllocateInit<LoopInfo_>(static_cast<StringStream_T_ &&>(inner_template),
                                                    static_cast<Array<TagBit> &&>(sub_tags), set_offset, set_length,
                                                    index_offset, index_length, repeat_offset, repeat_length,
                                                    group_offset, group_length, sort_offset, sort_length);
    }

    QENTEM_NOINLINE void renderLoop(const Char_T_ *content, const LoopInfo_ *loop_info) const {
        // Stage 3: Data
        Value_T_        grouped_set;
        const Value_T_ *loop_set   = root_value_;
        SizeT           loop_index = 0;
        SizeT           loop_size  = 0;

        // Set (Array)
        if (loop_info->SetLength != 0) {
            loop_set = findValue((content + loop_info->SetOffset), loop_info->SetLength);

            if (loop_set == nullptr) {
                return; // set="..." has invalid value.
            }
        }

        // Size
        if ((loop_info->RepeatLength != 0) &&
            (!parseNumber(loop_size, (content + loop_info->RepeatOffset), loop_info->RepeatLength))) {
            return; // Not a number
        }

        // Index
        if ((loop_info->IndexLength != 0) &&
            (!parseNumber(loop_index, (content + loop_info->IndexOffset), loop_info->IndexLength))) {
            return; // Not a number
        }

        // Group
        if (loop_info->GroupLength != 0) {
            if (!(loop_set->GroupBy(grouped_set, (content + loop_info->GroupOffset), loop_info->GroupLength))) {
                return;
            }

            loop_set = &grouped_set;
        }

        // Sort
        if (loop_info->SortLength != 0) {
            if (loop_info->GroupLength == 0) {
                grouped_set = *loop_set;
                loop_set    = &grouped_set;
            }

            grouped_set.Sort(content[loop_info->SortOffset] == TemplatePatterns_C_::SortAscend);
        }

        if (loop_size == 0) {
            loop_size = loop_set->Size();

            if (loop_size <= loop_index) {
                return;
            }

            loop_size -= loop_index;
        }

        // Stage 4: Render
        Template_CV    loop_template{ss_, root_value_, this, (level_ + 1)};
        const Char_T_ *loop_content = loop_info->InnerTemplate.First();
        const SizeT    loop_length  = loop_info->InnerTemplate.Length();

        if (loop_set->IsObject()) {
            while (true) {
                loop_template.loop_value_ = loop_set->GetValue(loop_index);
                loop_set->SetKeyCharAndLength(loop_index, loop_template.loop_key_, loop_template.loop_key_length_);
                loop_template.process(loop_content, loop_length, loop_info->SubTags);

                if ((--loop_size) == 0) {
                    break;
                }

                ++loop_index;
            }
        } else {
            while (true) {
                loop_template.loop_value_ = loop_set->GetValue(loop_index);
                loop_template.process(loop_content, loop_length, loop_info->SubTags);

                if ((--loop_size) == 0) {
                    break;
                }

                ++loop_index;
            }
        }
    }

    QENTEM_NOINLINE void generateInLineIfInfo(const Char_T_ *content, SizeT length,
                                              InlineIfInfo_ *&inline_if_info) const {
        Array<TagBit>  true_subtags;
        Array<TagBit>  false_subtags;
        unsigned short case_offset  = 0;
        unsigned short case_length  = 0;
        unsigned short true_offset  = 0;
        unsigned short true_length  = 0;
        unsigned short false_offset = 0;
        unsigned short false_length = 0;

        SizeT offset          = 0;
        SizeT previous_offset = 0;
        SizeT len             = 0;
        bool  break_loop;

        while (true) {
            break_loop = false;
            ++len;
            offset += len; // Move to the next Char_T_.

            len = getQuoted(content, offset, length);

            if (len == 0) {
                break;
            }

            // = + " + a two chars == 4 + the char before it == 5
            SizeT tmp_offset = (offset - 5);

            do {
                switch (content[tmp_offset]) {
                    case TemplatePatterns_C_::CaseChar: {
                        case_offset = static_cast<unsigned short>(offset);
                        case_length = static_cast<unsigned short>(len);
                        break_loop  = true;
                        break;
                    }

                    case TemplatePatterns_C_::TrueChar: {
                        true_offset = static_cast<unsigned short>(offset);
                        true_length = static_cast<unsigned short>(len);
                        break_loop  = true;
                        break;
                    }

                    case TemplatePatterns_C_::FalseChar: {
                        false_offset = static_cast<unsigned short>(offset);
                        false_length = static_cast<unsigned short>(len);
                        break_loop   = true;
                        break;
                    }

                    default: {
                    }
                }
            } while (!break_loop && (--tmp_offset > previous_offset));

            previous_offset = (offset + len);
        }

        if (true_length != 0) {
            light_parse(true_subtags, (content + true_offset), true_length);
        }

        if (false_length != 0) {
            light_parse(false_subtags, (content + false_offset), false_length);
        }

        inline_if_info = Memory::AllocateInit<InlineIfInfo_>(
            static_cast<Array<TagBit> &&>(true_subtags), static_cast<Array<TagBit> &&>(false_subtags), case_offset,
            case_length, true_offset, true_length, false_offset, false_length);
    }

    QENTEM_NOINLINE void renderInLineIf(const Char_T_ *content, const InlineIfInfo_ *inline_if_info) const {
        if (inline_if_info->CaseLength != 0) {
            double result;

            if (!(ALE::Evaluate(result, (content + inline_if_info->CaseOffset), inline_if_info->CaseLength, this))) {
                // A messed-up case.
                return;
            }

            if (result > 0.0) {
                if (inline_if_info->TrueLength != 0) {
                    process((content + inline_if_info->TrueOffset), inline_if_info->TrueLength,
                            inline_if_info->TrueSubTags);
                }
            } else if (inline_if_info->FalseLength != 0) {
                process((content + inline_if_info->FalseOffset), inline_if_info->FalseLength,
                        inline_if_info->FalseSubTags);
            }
        }
    }

    QENTEM_NOINLINE void generateIfCases(const Char_T_ *content, SizeT length, IfInfo_ *&if_info) const {
        SizeT case_offset = 0;
        SizeT case_length;
        SizeT content_offset;
        SizeT content_length;

        if_info = Memory::AllocateInit<IfInfo_>();

        // The content without </if>
        const SizeT length2 = (length - TemplatePatterns_C_::IfSuffixLength);

        while (true) {
            case_length = getQuoted(content, case_offset, length);

            if (case_length == 0) {
                return;
            }

            content_offset = Engine::FindOne<Char_T_>(TemplatePatterns_C_::MultiLineSuffix, content,
                                                      (case_offset + case_length + 1), length2);

            if (content_offset == 0) {
                return;
            }

            SizeT else_offset = nextElse(content, content_offset, length);

            if (else_offset == 0) {
                content_length = (length2 - content_offset);
                if_info->Insert(IfCase_{Array<TagBit>(), case_offset, case_length, content_offset, content_length});
                break;
            }

            content_length = ((else_offset - TemplatePatterns_C_::ElsePrefixLength) - content_offset);
            if_info->Insert(IfCase_{Array<TagBit>(), case_offset, case_length, content_offset, content_length});

            if ((content[else_offset] != TemplatePatterns_C_::ElseIfChar)) {
                else_offset =
                    Engine::FindOne<Char_T_>(TemplatePatterns_C_::MultiLineSuffix, content, else_offset, length2);

                if (else_offset == 0) {
                    return;
                }

                case_length    = 0;
                content_offset = else_offset;
                content_length = (length2 - else_offset);

                if_info->Insert(IfCase_{Array<TagBit>(), case_offset, case_length, content_offset, content_length});
                break;
            }

            case_offset = else_offset;
        }
    }

    static SizeT nextElse(const Char_T_ *content, SizeT offset, SizeT length) noexcept {
        SizeT else_offset = 0;

        while (true) {
            else_offset = Engine::Find<Char_T_>(TemplatePatterns_C_::GetElsePrefix(),
                                                TemplatePatterns_C_::ElsePrefixLength, content, offset, length);

            if (else_offset == 0) {
                // No <else.
                break;
            }

            const SizeT next_if = Engine::Find<Char_T_>(TemplatePatterns_C_::GetIfPrefix(),
                                                        TemplatePatterns_C_::IfPrefixLength, content, offset, length);

            if ((next_if == 0) || (else_offset < next_if)) {
                // No nesting <ifs or <else is before the child <if.
                break;
            }

            offset = Engine::Find<Char_T_>(TemplatePatterns_C_::GetIfSuffix(), TemplatePatterns_C_::IfSuffixLength,
                                           content, next_if, length);

            if (else_offset > offset) {
                // <else came after the child if.
                break;
            }
        }

        return else_offset;
    }

    void renderIf(const Char_T_ *content, IfInfo_ *if_info) const {
        IfCase_       *item = if_info->Storage();
        const IfCase_ *end  = (item + if_info->Size());

        while (item < end) {
            double result;

            if ((item->CaseLength == 0) ||
                (ALE::Evaluate(result, (content + item->CaseOffset), item->CaseLength, this) && (result > 0))) {
                if (item->SubTags.IsEmpty()) {
                    parse(item->SubTags, (content + item->ContentOffset), item->ContentLength);
                }

                process((content + item->ContentOffset), item->ContentLength, item->SubTags);
                break;
            }

            ++item;
        }
    }

    QENTEM_NOINLINE const Value_T_ *findValue(const Char_T_ *key, SizeT length) const noexcept {
        const Value_T_ *value = nullptr;

        if (length != 0) {
            SizeT offset = 0;
            SizeT tmp    = 1;

            if (*key != TemplatePatterns_C_::TildeChar) {
                if ((key[(length - 1)] != TemplatePatterns_C_::VariableIndexSuffix)) {
                    return root_value_->GetValue(key, length);
                }

                // [...]
                while ((key[tmp] != TemplatePatterns_C_::VariableIndexPrefix) && (tmp < length)) {
                    ++tmp;
                }

                value = root_value_;
            } else {
                const Template_CV *obj = this;
                SizeT              lvl = 0;

                do {
                    ++lvl;
                } while (key[lvl] == TemplatePatterns_C_::TildeChar);

                while (lvl < level_) {
                    ++lvl;
                    obj = obj->parent_;
                }

                if ((key[(length - 1)] != TemplatePatterns_C_::VariableIndexSuffix)) {
                    return obj->loop_value_;
                }

                // [...]
                while (key[tmp] != TemplatePatterns_C_::VariableIndexSuffix) {
                    ++tmp;
                }

                value  = obj->loop_value_;
                offset = (lvl + 1);
            }

            while (true) {
                if (value == nullptr) {
                    break;
                }

                value = value->GetValue((key + offset), (tmp - offset));

                ++tmp;

                if (tmp >= length) {
                    break;
                }

                if (key[tmp] == TemplatePatterns_C_::VariableIndexPrefix) {
                    ++tmp;
                }

                offset = tmp;

                while (key[tmp] != TemplatePatterns_C_::VariableIndexSuffix) {
                    ++tmp;
                }
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

    bool ALESetNumber(double &number, const Char_T_ *content, SizeT length) const noexcept {
        const Value_T_ *value = nullptr;

        if (length > TemplatePatterns_C_::VariableFulllength) {
            content += TemplatePatterns_C_::VariablePrefixLength;
            length -= TemplatePatterns_C_::VariableFulllength;
            value = findValue(content, length);
        }

        return ((value != nullptr) && ALESetNumber(number, value));
    }

    bool ALEIsEqual(bool &result, const Char_T_ *content, ALE::Number left, ALE::Number right, bool left_evaluated,
                    bool right_evaluated) const noexcept {
        using ALEExpressions_T_ = ALEExpressions<Char_T_>;

        const Value_T_ *value_left  = nullptr;
        const Value_T_ *value_right = nullptr;

        const Char_T_ *left_content  = (content + left.Content.Offset);
        const Char_T_ *right_content = (content + right.Content.Offset);
        SizeT          left_length   = left.Content.Length;
        SizeT          right_length  = right.Content.Length;

        bool is_number = (left_evaluated || right_evaluated);

        // If the left side is a variable
        if (!left_evaluated) {
            if ((*left_content == TemplatePatterns_C_::InLinePrefix) &&
                (left_length > TemplatePatterns_C_::VariableFulllength)) {
                value_left = findValue((left_content + TemplatePatterns_C_::VariablePrefixLength),
                                       (left_length - TemplatePatterns_C_::VariableFulllength)); // {var:x}

                if (value_left == nullptr) {
                    return false;
                }

                if (value_left->IsNumber()) {
                    is_number = true;
                }
            } else {
                if (*left_content != ALEExpressions_T_::ParenthesStart) {
                    left_evaluated = Digit<Char_T_>::StringToNumber(left.Number, left_content, left_length);
                } else {
                    left_evaluated = ALE::Evaluate(left.Number, (++left_content), (left_length -= 2), this);
                }

                if (!left_evaluated && is_number) {
                    return false;
                }

                is_number = left_evaluated;
            }
        }

        if (!right_evaluated) {
            // If the right side is a variable
            if ((*right_content == TemplatePatterns_C_::InLinePrefix) &&
                (right_length > TemplatePatterns_C_::VariableFulllength)) {
                value_right = findValue((right_content + TemplatePatterns_C_::VariablePrefixLength),
                                        (right_length - TemplatePatterns_C_::VariableFulllength)); // {var:x}

                if (value_right == nullptr) {
                    return false;
                }

                if (value_right->IsNumber()) {
                    is_number = true;
                }
            } else if (is_number) {
                if (*right_content != ALEExpressions_T_::ParenthesStart) {
                    if (!(Digit<Char_T_>::StringToNumber(right.Number, right_content, right_length))) {
                        return false;
                    }
                } else if (!(ALE::Evaluate(right.Number, (++right_content), (right_length - 2), this))) {
                    return false;
                }
            }
        }

        if (is_number) {
            if (!left_evaluated && ((value_left == nullptr) || !(ALESetNumber(left.Number, value_left)))) {
                return false;
            }

            if ((value_right != nullptr) && !(ALESetNumber(right.Number, value_right))) {
                return false;
            }
        } else {
            if ((value_left != nullptr) && !(value_left->SetCharAndLength(left_content, left_length))) {
                return false;
            }

            if ((value_right != nullptr) && !(value_right->SetCharAndLength(right_content, right_length))) {
                return false;
            }
        }

        if (is_number) {
            result = (left.Number == right.Number);
        } else {
            result = ((left_length == right_length) && StringUtils::IsEqual(left_content, right_content, right_length));
        }

        return true;
    }

    Template_CV(StringStream_T_ *ss, const Value_T_ *root_value, const Template_CV *parent = nullptr,
                SizeT level = 0) noexcept
        : ss_{ss}, root_value_{root_value}, parent_{parent}, level_{level} {}

    StringStream_T_   *ss_;
    const Value_T_    *root_value_;
    const Template_CV *parent_;
    const Value_T_    *loop_value_{nullptr};
    const Char_T_     *loop_key_{nullptr};
    SizeT              loop_key_length_{0};
    const SizeT        level_;
};

template <typename Char_T_>
class TemplatePatterns {
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
    static constexpr Char_T_ Var_2ND_Char         = 'v'; // Second character
    static constexpr SizeT   VariablePrefixLength = 5U;
    static constexpr SizeT   VariableFulllength   = (VariablePrefixLength + InLineSuffixLength);
    static const Char_T_    *GetVariablePrefix() noexcept {
           static constexpr Char_T_ val[] = {'{', 'v', 'a', 'r', ':'};
           return &(val[0]);
    }

    // {raw:
    static constexpr Char_T_ Raw_2ND_Char            = 'r'; // Second character
    static constexpr SizeT   RawVariablePrefixLength = 5U;
    static constexpr SizeT   RawVariableFulllength   = (VariablePrefixLength + InLineSuffixLength);
    static const Char_T_    *GetRawVariablePrefix() noexcept {
           static constexpr Char_T_ val[] = {'{', 'r', 'a', 'w', ':'};
           return &(val[0]);
    }

    // {math:
    static constexpr Char_T_ Math_2ND_Char    = 'm'; // Second character
    static constexpr SizeT   MathPrefixLength = 6U;
    static constexpr SizeT   MathFulllength   = (MathPrefixLength + InLineSuffixLength);
    static const Char_T_    *GetMathPrefix() noexcept {
           static constexpr Char_T_ val[] = {'{', 'm', 'a', 't', 'h', ':'};
           return &(val[0]);
    }

    // {if:
    static constexpr Char_T_ InlineIf_2ND_Char    = 'i'; // Second character
    static constexpr SizeT   InLineIfPrefixLength = 3U;
    static const Char_T_    *GetInLineIfPrefix() noexcept {
           static constexpr Char_T_ val[] = {'{', 'i', 'f', ':'};
           return &(val[0]);
    }

    // <loop
    static constexpr Char_T_ Loop_2ND_Char    = 'l'; // Second character
    static constexpr SizeT   LoopPrefixLength = 5U;
    static const Char_T_    *GetLoopPrefix() noexcept {
           static constexpr Char_T_ val[] = {'<', 'l', 'o', 'o', 'p'};
           return &(val[0]);
    }

    // </loop>
    static constexpr SizeT LoopSuffixLength = 7U;
    static const Char_T_  *GetLoopSuffix() noexcept {
         static constexpr Char_T_ val[] = {'<', '/', 'l', 'o', 'o', 'p', '>'};
         return &(val[0]);
    }

    // <if
    static constexpr Char_T_ If_2ND_Char    = 'i'; // Second character
    static constexpr SizeT   IfPrefixLength = 3U;
    static const Char_T_    *GetIfPrefix() noexcept {
           static constexpr Char_T_ val[] = {'<', 'i', 'f'};
           return &(val[0]);
    }

    // </if>
    static constexpr SizeT IfSuffixLength = 5U;
    static const Char_T_  *GetIfSuffix() noexcept {
         static constexpr Char_T_ val[] = {'<', '/', 'i', 'f', '>'};
         return &(val[0]);
    }

    // <else
    static constexpr Char_T_ ElseIfChar       = 'i'; // else[i]f
    static constexpr SizeT   ElsePrefixLength = 5U;
    static const Char_T_    *GetElsePrefix() noexcept {
           static constexpr Char_T_ val[] = {'<', 'e', 'l', 's', 'e'};
           return &(val[0]);
    }

    // />
    static constexpr SizeT ElseSuffixLength = 2U;
    static const Char_T_  *GetElseSuffix() noexcept {
         static constexpr Char_T_ val[] = {'/', '>'};
         return &(val[0]);
    }

    // &amp; &
    static constexpr SizeT HTMLAndLength = 5U;
    static const Char_T_  *GetHTMLAnd() noexcept {
         static constexpr Char_T_ val[] = {'&', 'a', 'm', 'p', ';'};
         return &(val[0]);
    }

    // &lt; <
    static constexpr SizeT HTMLLessLength = 4U;
    static const Char_T_  *GetHTMLLess() noexcept {
         static constexpr Char_T_ val[] = {'&', 'l', 't', ';'};
         return &(val[0]);
    }

    // &gt; >
    static constexpr SizeT HTMLBiggerLength = 4U;
    static const Char_T_  *GetHTMLBigger() noexcept {
         static constexpr Char_T_ val[] = {'&', 'g', 't', ';'};
         return &(val[0]);
    }

    // &quot; "
    static constexpr SizeT HTMLQuoteLength = 6U;
    static const Char_T_  *GetHTMLQuote() noexcept {
         static constexpr Char_T_ val[] = {'&', 'q', 'u', 'o', 't', ';'};
         return &(val[0]);
    }

    // &apos; ' (HTML5)
    static constexpr SizeT HTMLSingleQuoteLength = 6U;
    static const Char_T_  *GetHTMLSingleQuote() noexcept {
         static constexpr Char_T_ val[] = {'&', 'a', 'p', 'o', 's', ';'};
         return &(val[0]);
    }

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

} // namespace Qentem

#endif
