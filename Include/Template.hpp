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
struct TemplatePatterns;

class Template {
  public:
    Template()                            = delete;
    Template(Template &&)                 = delete;
    Template(const Template &)            = delete;
    Template &operator=(Template &&)      = delete;
    Template &operator=(const Template &) = delete;
    ~Template()                           = delete;

    template <typename>
    struct TagBit;

    template <typename Char_T_, typename Value_T_, typename Number_T_, typename StringStream_T_>
    inline static void Render(const Char_T_ *content, Number_T_ length, const Value_T_ *root_value,
                              StringStream_T_ *stream, Array<TagBit<Char_T_>> *tags_cache) {
        Template_CV<Char_T_, Value_T_, StringStream_T_>::Process(content, static_cast<SizeT>(length), root_value,
                                                                 stream, *tags_cache);
    }

    template <typename Char_T_, typename Value_T_, typename Number_T_, typename StringStream_T_>
    inline static void Render(const Char_T_ *content, Number_T_ length, const Value_T_ *root_value,
                              StringStream_T_ *stream) {
        Array<TagBit<Char_T_>> tags_cache;
        Render(content, length, root_value, stream, &tags_cache);
    }

    template <typename Char_T_, typename Value_T_, typename Number_T_, typename StringStream_T_ = StringStream<Char_T_>>
    inline static StringStream_T_ Render(const Char_T_ *content, Number_T_ length, const Value_T_ *root_value) {
        StringStream_T_ stream;
        Render(content, length, root_value, &stream);
        return stream;
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
    struct LoopTag_T_ {
        // TODO: Remove StringStream;
        StringStream<Char_T_>  InnerTemplate;
        Array<TagBit<Char_T_>> SubTags;
        void                  *padding_1;
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
        void                  *padding_1;
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
            : pointer_(static_cast<QPointer<void> &&>(tag.pointer_)), offset_(tag.offset_),
              end_offset_(tag.end_offset_) {
#if !defined(QENTEM_POINTER_TAGGING) || (QENTEM_POINTER_TAGGING != 1)
            type_ = tag.type_;
#endif
            tag.setType(TagType::None);
        }

        ~TagBit() {
            switch (GetType()) {
                case TagType::Loop: {
                    LoopTag_T_<Char_T_> *loop_info = static_cast<LoopTag_T_<Char_T_> *>(pointer_.GetPointer());
                    Memory::Dispose(loop_info);
                    Memory::Deallocate(loop_info);
                    break;
                }

                case TagType::If: {
                    Array<IfTagCase_T_<Char_T_>> *if_info =
                        static_cast<Array<IfTagCase_T_<Char_T_>> *>(pointer_.GetPointer());
                    Memory::Dispose(if_info);
                    Memory::Deallocate(if_info);
                    break;
                }

                case TagType::InLineIf: {
                    InlineIfTag_T_<Char_T_> *inline_if_info =
                        static_cast<InlineIfTag_T_<Char_T_> *>(pointer_.GetPointer());
                    Memory::Dispose(inline_if_info);
                    Memory::Deallocate(inline_if_info);
                    break;
                }

                default: {
                }
            }
        }
        LoopTag_T_<Char_T_> *MakeLoopInfo() {
            LoopTag_T_<Char_T_> *pointer = Memory::AllocateInit<LoopTag_T_<Char_T_>>();
            pointer_.SetPointer(pointer);
            return pointer;
        }

        Array<IfTagCase_T_<Char_T_>> *MakeIfInfo() {
            Array<IfTagCase_T_<Char_T_>> *pointer = Memory::AllocateInit<Array<IfTagCase_T_<Char_T_>>>();
            pointer_.SetPointer(pointer);
            return pointer;
        }

        InlineIfTag_T_<Char_T_> *MakeInlineIf() {
            InlineIfTag_T_<Char_T_> *pointer = Memory::AllocateInit<InlineIfTag_T_<Char_T_>>();
            pointer_.SetPointer(pointer);
            return pointer;
        }

        inline void *GetInfo() const noexcept { return pointer_.GetPointer(); }

        inline TagType GetType() const noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
            return static_cast<TagType>(pointer_.GetHighByte());
#else
            return type_;
#endif
        }

        inline SizeT GetOffset() const noexcept { return offset_; }
        inline SizeT GetEndOffset() const noexcept { return end_offset_; }

      private:
        void setType(TagType type) noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
            pointer_.SetHighByte(static_cast<unsigned char>(type));
#else
            type_ = type;
#endif
        }

        QPointer<void> pointer_{};
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

    using TagBit      = Template::TagBit<Char_T_>;
    using TagType     = Template::TagType;
    using LoopTag     = Template::LoopTag_T_<Char_T_>;
    using IfTagCase   = Template::IfTagCase_T_<Char_T_>;
    using IfTag       = Array<IfTagCase>;
    using InlineIfTag = Template::InlineIfTag_T_<Char_T_>;

    using TemplatePatterns_C_ = TemplatePatterns<Char_T_>;

  public:
    Template_CV() = delete;

    static void Process(const Char_T_ *content, SizeT length, const Value_T_ *root_value, StringStream_T_ *stream,
                        Array<TagBit> &tags_cache) {
        const Template_CV temp{stream, root_value};

        if (tags_cache.IsEmpty()) {
            parse(tags_cache, content, length, 0);
        }

        temp.render(content, length, tags_cache);
    }

  private:
    void render(const Char_T_ *content, SizeT length, const Array<TagBit> &tags_cache) const {
        TagBit       *tag    = tags_cache.Storage();
        const TagBit *end    = (tag + tags_cache.Size());
        SizeT         offset = 0;

        while (tag != end) {
            ss_->Insert((content + offset), (tag->GetOffset() - offset));

            switch (tag->GetType()) {
                case TagType::Variable: {
                    renderVariable(content, tag);
                    break;
                }

                case TagType::RawVariable: {
                    renderRawVariable(content, tag);
                    break;
                }

                case TagType::Math: {
                    renderMath(content, tag);
                    break;
                }

                case TagType::Loop: {
                    renderLoop(content, tag);
                    break;
                }

                case TagType::InLineIf: {
                    renderInLineIf(content, tag);
                    break;
                }

                case TagType::If: {
                    renderIf(content, tag);
                    break;
                }

                default: {
                }
            }

            offset = tag->GetEndOffset();
            ++tag;
        }

        // Add any remaining string.
        ss_->Insert((content + offset), (length - offset));
    }

    QENTEM_NOINLINE static void parse(Array<TagBit> &tags_cache, const Char_T_ *content, SizeT length, SizeT level) {
        SizeT offset = 0;

        while (offset < length) {
            if (content[offset] == TemplatePatterns_C_::InLinePrefix) {
                const SizeT current_offset = (offset + 1);

                switch (content[current_offset]) {
                    case TemplatePatterns_C_::Var_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns_C_::VariablePrefix + 2),
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
                        if (StringUtils::IsEqual((TemplatePatterns_C_::RawVariablePrefix + 2),
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
                        if (StringUtils::IsEqual((TemplatePatterns_C_::MathPrefix + 2), (content + current_offset + 1),
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
                        if (StringUtils::IsEqual((TemplatePatterns_C_::InLineIfPrefix + 2),
                                                 (content + current_offset + 1),
                                                 (TemplatePatterns_C_::InLineIfPrefixLength - 2))) {
                            const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TemplatePatterns_C_::InLinePrefix, TemplatePatterns_C_::InLineSuffix, content,
                                (current_offset + TemplatePatterns_C_::InLineIfPrefixLength - 1), length);

                            if (end_offset != 0) {
                                tags_cache += TagBit{TagType::InLineIf, offset, end_offset};
                                offset = end_offset;
                                generateInLineIfInfo(content, tags_cache.Last());
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
                    if (StringUtils::IsEqual((TemplatePatterns_C_::LoopPrefix + 2), (content + current_offset + 1),
                                             (TemplatePatterns_C_::LoopPrefixLength - 2))) {
                        const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                            TemplatePatterns_C_::LoopPrefix, TemplatePatterns_C_::LoopPrefixLength,
                            TemplatePatterns_C_::LoopSuffix, TemplatePatterns_C_::LoopSuffixLength, content,
                            (current_offset + TemplatePatterns_C_::InLineIfPrefixLength - 1), length);

                        if (end_offset != 0) {
                            tags_cache += TagBit{TagType::Loop, offset, end_offset};
                            offset = end_offset;
                            generateLoopContent(content, tags_cache.Last(), level);
                            continue;
                        }
                    }
                } else if (content[current_offset] == TemplatePatterns_C_::If_2ND_Char) { // <if
                    if (StringUtils::IsEqual((TemplatePatterns_C_::IfPrefix + 2), (content + current_offset + 1),
                                             (TemplatePatterns_C_::IfPrefixLength - 2))) {
                        const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                            TemplatePatterns_C_::IfPrefix, TemplatePatterns_C_::IfPrefixLength,
                            TemplatePatterns_C_::IfSuffix, TemplatePatterns_C_::IfSuffixLength, content, current_offset,
                            length);

                        if (end_offset != 0) {
                            tags_cache += TagBit{TagType::If, offset, end_offset};
                            offset = end_offset;
                            generateIfCases(content, tags_cache.Last());
                            continue;
                        }
                    }
                }
            }

            ++offset;
        }
    }

    // Part of parse()
    QENTEM_NOINLINE static void lightParse(Array<TagBit> &tags_cache, const Char_T_ *content, SizeT length) {
        SizeT offset = 0;

        while (true) {
            offset = Engine::FindOne<Char_T_>(TemplatePatterns_C_::InLinePrefix, content, offset, length);

            if (offset != 0) {
                switch (content[offset]) {
                    case TemplatePatterns_C_::Var_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns_C_::VariablePrefix + 2), (content + offset + 1),
                                                 (TemplatePatterns_C_::VariablePrefixLength - 2))) {
                            const SizeT end_offset =
                                Engine::FindOne<Char_T_>(TemplatePatterns_C_::InLineSuffix, content,
                                                         (offset + TemplatePatterns_C_::VariablePrefixLength), length);
                            // end_offset cannot be zero

                            tags_cache += TagBit{TagType::Variable, (offset - TemplatePatterns_C_::InLinePrefixLength),
                                                 end_offset};
                            offset = end_offset;
                        }

                        continue;
                    }

                    case TemplatePatterns_C_::Raw_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns_C_::RawVariablePrefix + 2), (content + offset + 1),
                                                 (TemplatePatterns_C_::RawVariablePrefixLength - 2))) {
                            const SizeT end_offset = Engine::FindOne<Char_T_>(
                                TemplatePatterns_C_::InLineSuffix, content,
                                (offset + TemplatePatterns_C_::RawVariablePrefixLength), length);
                            tags_cache += TagBit{TagType::RawVariable,
                                                 (offset - TemplatePatterns_C_::InLinePrefixLength), end_offset};
                            offset = end_offset;
                        }

                        continue;
                    }

                    case TemplatePatterns_C_::Math_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns_C_::MathPrefix + 2), (content + offset + 1),
                                                 (TemplatePatterns_C_::MathPrefixLength - 2))) {
                            const SizeT end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TemplatePatterns_C_::InLinePrefix, TemplatePatterns_C_::InLineSuffix, content,
                                (offset + TemplatePatterns_C_::MathPrefixLength - 1), length);
                            tags_cache +=
                                TagBit{TagType::Math, (offset - TemplatePatterns_C_::InLinePrefixLength), end_offset};
                            offset = end_offset;
                        }

                        continue;
                    }

                    default: {
                    }
                }
            }

            break;
        }
    }

    static void escapeHTMLSpecialChars(StringStream_T_ &stream, const Char_T_ *str, SizeT length) {
        SizeT offset = 0;
        SizeT index  = 0;

        while (index < length) {
            switch (str[index]) {
                case '&': {
                    stream.Insert((str + offset), (index - offset));
                    stream.Insert(TemplatePatterns_C_::HTMLAnd, TemplatePatterns_C_::HTMLAndLength);
                    offset = (++index);
                    break;
                }

                case '<': {
                    stream.Insert((str + offset), (index - offset));
                    stream.Insert(TemplatePatterns_C_::HTMLLess, TemplatePatterns_C_::HTMLLessLength);
                    offset = (++index);
                    break;
                }

                case '>': {
                    stream.Insert((str + offset), (index - offset));
                    stream.Insert(TemplatePatterns_C_::HTMLBigger, TemplatePatterns_C_::HTMLBiggerLength);
                    offset = (++index);
                    break;
                }

                case '"': {
                    stream.Insert((str + offset), (index - offset));
                    stream.Insert(TemplatePatterns_C_::HTMLQuote, TemplatePatterns_C_::HTMLQuoteLength);
                    offset = (++index);
                    break;
                }

                case '\'': {
                    stream.Insert((str + offset), (index - offset));
                    stream.Insert(TemplatePatterns_C_::HTMLSingleQuote, TemplatePatterns_C_::HTMLSingleQuoteLength);
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
        const SizeT content_offset = (tag->GetOffset() + TemplatePatterns_C_::VariablePrefixLength);
        content += content_offset;
        SizeT length = ((tag->GetEndOffset() - TemplatePatterns_C_::InLineSuffixLength) - content_offset);

        const Value_T_ *value = findValue(content, length);

        if (value != nullptr) {
#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
            if (value->IsString()) {
                const Char_T_ *str;
                SizeT          len;
                value->SetCharAndLength(str, len);
                escapeHTMLSpecialChars(*ss_, str, len);
                return;
            }
#endif

            if (value->CopyStringValueTo(*ss_)) {
                return;
            }

            if ((*content == TemplatePatterns_C_::TildeChar) && (loop_key_ != nullptr)) {
#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
                escapeHTMLSpecialChars(*ss_, loop_key_, loop_key_length_);
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

    void renderRawVariable(const Char_T_ *content, const TagBit *tag) const {
        const SizeT content_offset = (tag->GetOffset() + TemplatePatterns_C_::RawVariablePrefixLength);
        content += content_offset;
        SizeT length = ((tag->GetEndOffset() - TemplatePatterns_C_::InLineSuffixLength) - content_offset);

        const Value_T_ *value = findValue(content, length);

        if ((value != nullptr) && value->CopyStringValueTo(*ss_)) {
            return;
        }

        if (*content != TemplatePatterns_C_::TildeChar) {
            ss_->Insert((content - TemplatePatterns_C_::RawVariablePrefixLength),
                        (length + TemplatePatterns_C_::RawVariableFulllength));
        }
    }

    void renderMath(const Char_T_ *content, const TagBit *tag) const {
        double      number;
        const SizeT content_offset = (tag->GetOffset() + TemplatePatterns_C_::MathPrefixLength);
        content += content_offset;
        SizeT length = ((tag->GetEndOffset() - TemplatePatterns_C_::InLineSuffixLength) - content_offset);

        if (ALE::Evaluate(number, content, length, this)) {
            Digit<Char_T_>::NumberToString(*ss_, number, 1, 0, 3);
        } else {
            ss_->Insert((content - TemplatePatterns_C_::MathPrefixLength),
                        (length + TemplatePatterns_C_::MathFulllength));
        }
    }

    /*
     * Gets anything between "..."
     */

    static SizeT getQuoted(const Char_T_ *content, SizeT &offset, SizeT length) noexcept {
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

    QENTEM_NOINLINE bool static IsStaticNumber(const Char_T_ *content, const SizeT length) noexcept {
        if (length > TemplatePatterns_C_::VariableFulllength) {
            if (Engine::Find<Char_T_>(TemplatePatterns_C_::VariablePrefix, TemplatePatterns_C_::VariablePrefixLength,
                                      content, SizeT{0}, length) != 0) {
                return false;
            }
        }

        return true;
    }

    QENTEM_NOINLINE void static findKey(const Char_T_ *content, SizeT offset, SizeT end_offset,
                                        unsigned short &i_offset, unsigned short &i_length) noexcept {
        offset = Engine::Find<Char_T_>(TemplatePatterns_C_::VariablePrefix, TemplatePatterns_C_::VariablePrefixLength,
                                       content, offset, end_offset);

        i_length = 1;

        if (offset != 0) {
            end_offset = Engine::FindOne<Char_T_>(TemplatePatterns_C_::InLineSuffix, content, offset, end_offset);

            if (end_offset != 0) {
                i_offset = static_cast<unsigned short>(offset);
                i_length =
                    static_cast<unsigned short>(((end_offset - TemplatePatterns_C_::InLineSuffixLength) - offset));
            }
        }
    }

    QENTEM_NOINLINE static void generateLoopContent(const Char_T_ *content, TagBit *tag, SizeT level) {
        const Char_T_ *loop_value        = nullptr;
        SizeT          offset            = (tag->GetOffset() + TemplatePatterns_C_::LoopPrefixLength);
        const SizeT    length            = ((tag->GetEndOffset() - TemplatePatterns_C_::LoopSuffixLength) - offset);
        SizeT          tmp_length        = 0;
        SizeT          loop_value_length = 0;
        SizeT          previous_offset   = 0;
        SizeT          tmp_offset;
        bool           break_loop = false;

        content += offset;

        const SizeT start_offset =
            Engine::FindOne<Char_T_>(TemplatePatterns_C_::MultiLineSuffix, content, SizeT{0}, length);

        LoopTag *info = tag->MakeLoopInfo();

        offset = 0;
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

            do {
                switch (content[tmp_offset]) {
                    case TemplatePatterns_C_::ValueChar: {
                        loop_value        = (content + offset);
                        loop_value_length = tmp_length;
                        break_loop        = true;
                        break;
                    }

                    case TemplatePatterns_C_::SetChar: {
                        info->SetOffset = static_cast<unsigned short>(offset);
                        info->SetLength = static_cast<unsigned short>(tmp_length);

                        if ((content[offset] == TemplatePatterns_C_::InLinePrefix) &&
                            (tmp_length > TemplatePatterns_C_::VariableFulllength)) {
                            info->SetOffset += TemplatePatterns_C_::VariablePrefixLength;
                            info->SetLength -= static_cast<unsigned short>(TemplatePatterns_C_::VariableFulllength);
                        }

                        break_loop = true;
                        break;
                    }

                    case TemplatePatterns_C_::IndexChar: {
                        const Char_T_ *tmp_content = (content + offset);

                        if (!IsStaticNumber(tmp_content, tmp_length) ||
                            !(Digit<Char_T_>::StringToNumber(info->FixedIndex, tmp_content, tmp_length))) {
                            findKey(content, offset, (tmp_length + offset), info->IndexOffset, info->IndexLength);
                        }

                        break_loop = true;
                        break;
                    }

                    case TemplatePatterns_C_::RepeatChar: {
                        const Char_T_ *tmp_content = (content + offset);

                        if (!IsStaticNumber(tmp_content, tmp_length) ||
                            !(Digit<Char_T_>::StringToNumber(info->FixedSize, tmp_content, tmp_length))) {
                            findKey(content, offset, (tmp_length + offset), info->RepeatOffset, info->RepeatLength);
                        }

                        break_loop = true;
                        break;
                    }

                    case TemplatePatterns_C_::GroupChar: {
                        info->GroupOffset = static_cast<unsigned short>(offset);
                        info->GroupLength = static_cast<unsigned short>(tmp_length);
                        break_loop        = true;
                        break;
                    }

                    case TemplatePatterns_C_::SortChar: {
                        info->SortOffset = static_cast<unsigned short>(offset);
                        info->SortLength = static_cast<unsigned short>(tmp_length);
                        break_loop       = true;
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

                info->InnerTemplate.Insert((content + previous_offset),
                                           ((offset - loop_value_length) - previous_offset));

                info->InnerTemplate.Insert(TemplatePatterns_C_::VariablePrefix,
                                           TemplatePatterns_C_::VariablePrefixLength);

                Char_T_ *buffer = info->InnerTemplate.Buffer(level + 1);
                SizeT    lvl    = level;

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
                    info->InnerTemplate.Insert((content + offset), (sub_offset - offset));
                }

                previous_offset = sub_offset;
                info->InnerTemplate += TemplatePatterns_C_::InLineSuffix;
            }
        }

        info->InnerTemplate.Insert((content + previous_offset), (length - previous_offset));

        parse(info->SubTags, info->InnerTemplate.First(), info->InnerTemplate.Length(), ++level);
    }

    QENTEM_NOINLINE void renderLoop(const Char_T_ *content, const TagBit *tag) const {
        // Stage 3: Data
        Value_T_        grouped_set;
        const Value_T_ *loop_set   = root_value_;
        LoopTag        *loop_info  = static_cast<LoopTag *>(tag->GetInfo());
        SizeT           loop_index = loop_info->FixedIndex;
        SizeT           loop_size  = loop_info->FixedSize;

        content += (tag->GetOffset() + TemplatePatterns_C_::LoopPrefixLength);

        // Set (Array|Object)
        if (loop_info->SetLength != 0) {
            loop_set = findValue((content + loop_info->SetOffset), loop_info->SetLength);

            if (loop_set == nullptr) {
                return; // set="..." has invalid value.
            }
        }

        // Index
        if (loop_info->IndexLength != 0) {
            const Value_T_ *tmp_value = findValue((content + loop_info->IndexOffset), loop_info->IndexLength);

            if ((tmp_value == nullptr) || !(tmp_value->SetNumber(loop_index))) {
                return; // Not a number
            }
        }

        // Size
        if (loop_info->RepeatLength != 0) {
            const Value_T_ *tmp_value = findValue((content + loop_info->RepeatOffset), loop_info->RepeatLength);

            if ((tmp_value == nullptr) || !(tmp_value->SetNumber(loop_size))) {
                return; // Not a number
            }
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
            do {
                loop_template.loop_value_ = loop_set->GetValue(loop_index);
                loop_set->SetKeyCharAndLength(loop_index, loop_template.loop_key_, loop_template.loop_key_length_);
                loop_template.render(loop_content, loop_length, loop_info->SubTags);
                ++loop_index;
            } while ((--loop_size) != 0);
        } else {
            do {
                loop_template.loop_value_ = loop_set->GetValue(loop_index);
                loop_template.render(loop_content, loop_length, loop_info->SubTags);
                ++loop_index;
            } while ((--loop_size) != 0);
        }
    }

    QENTEM_NOINLINE void static generateInLineIfInfo(const Char_T_ *content, TagBit *tag) {
        SizeT       offset          = (tag->GetOffset() + TemplatePatterns_C_::InLineIfPrefixLength);
        SizeT       previous_offset = 0;
        SizeT       tmp_length      = 0;
        const SizeT length          = ((tag->GetEndOffset() - TemplatePatterns_C_::InLineSuffixLength) - offset);
        bool        break_loop;

        InlineIfTag *info = tag->MakeInlineIf();

        content += offset;
        offset = 0;

        while (true) {
            break_loop = false;
            ++tmp_length;
            offset += tmp_length; // Move to the next Char_T_.

            tmp_length = getQuoted(content, offset, length);

            if (tmp_length == 0) {
                break;
            }

            // = + " + a two chars == 4 + the char before it == 5
            SizeT tmp_offset = (offset - 5);

            do {
                switch (content[tmp_offset]) {
                    case TemplatePatterns_C_::CaseChar: {
                        info->CaseOffset = offset;
                        info->CaseLength = tmp_length;
                        break_loop       = true;
                        break;
                    }

                    case TemplatePatterns_C_::TrueChar: {
                        info->TrueOffset = offset;
                        info->TrueLength = tmp_length;
                        break_loop       = true;
                        break;
                    }

                    case TemplatePatterns_C_::FalseChar: {
                        info->FalseOffset = offset;
                        info->FalseLength = tmp_length;
                        break_loop        = true;
                        break;
                    }

                    default: {
                    }
                }
            } while (!break_loop && (--tmp_offset > previous_offset));

            previous_offset = (offset + tmp_length);
        }

        if (info->TrueLength != 0) {
            lightParse(info->TrueSubTags, (content + info->TrueOffset), info->TrueLength);
        }

        if (info->FalseLength != 0) {
            lightParse(info->FalseSubTags, (content + info->FalseOffset), info->FalseLength);
        }
    }

    QENTEM_NOINLINE void renderInLineIf(const Char_T_ *content, const TagBit *tag) const {
        InlineIfTag *inline_if_info = static_cast<InlineIfTag *>(tag->GetInfo());
        content += (tag->GetOffset() + TemplatePatterns_C_::InLineIfPrefixLength);

        if (inline_if_info->CaseLength != 0) {
            double result;

            if (ALE::Evaluate(result, (content + inline_if_info->CaseOffset), inline_if_info->CaseLength, this)) {
                if (result > 0.0) {
                    if (inline_if_info->TrueLength != 0) {
                        render((content + inline_if_info->TrueOffset), inline_if_info->TrueLength,
                               inline_if_info->TrueSubTags);
                    }
                } else if (inline_if_info->FalseLength != 0) {
                    render((content + inline_if_info->FalseOffset), inline_if_info->FalseLength,
                           inline_if_info->FalseSubTags);
                }
            }
        }
    }

    QENTEM_NOINLINE static void generateIfCases(const Char_T_ *content, TagBit *tag) {
        SizeT       case_offset = (tag->GetOffset() + TemplatePatterns_C_::IfPrefixLength);
        const SizeT length      = (tag->GetEndOffset() - case_offset);
        SizeT       case_length;
        SizeT       content_offset;
        SizeT       content_length;

        content += case_offset;
        case_offset = 0;

        IfTag *if_info = tag->MakeIfInfo();

        // The content without </if>
        const SizeT length2 = (length - TemplatePatterns_C_::IfSuffixLength);

        do {
            case_length = getQuoted(content, case_offset, length);

            if (case_length != 0) {
                content_offset = Engine::FindOne<Char_T_>(TemplatePatterns_C_::MultiLineSuffix, content,
                                                          (case_offset + case_length + 1), length2);

                if (content_offset != 0) {
                    SizeT else_offset = nextElse(content, content_offset, length);

                    if (else_offset == 0) {
                        content_length = (length2 - content_offset);
                        if_info->Insert(
                            IfTagCase{Array<TagBit>{}, case_offset, case_length, content_offset, content_length});
                        break;
                    }

                    content_length = ((else_offset - TemplatePatterns_C_::ElsePrefixLength) - content_offset);
                    if_info->Insert(
                        IfTagCase{Array<TagBit>{}, case_offset, case_length, content_offset, content_length});

                    if ((content[else_offset] != TemplatePatterns_C_::ElseIfChar)) {
                        else_offset = Engine::FindOne<Char_T_>(TemplatePatterns_C_::MultiLineSuffix, content,
                                                               else_offset, length2);

                        if (else_offset != 0) {
                            if_info->Insert(
                                IfTagCase{Array<TagBit>{}, case_offset, 0, else_offset, (length2 - else_offset)});
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

    static SizeT nextElse(const Char_T_ *content, SizeT offset, SizeT length) noexcept {
        SizeT else_offset = 0;

        while (true) {
            else_offset = Engine::Find<Char_T_>(TemplatePatterns_C_::ElsePrefix, TemplatePatterns_C_::ElsePrefixLength,
                                                content, offset, length);

            if (else_offset == 0) {
                // No <else.
                break;
            }

            const SizeT next_if = Engine::Find<Char_T_>(TemplatePatterns_C_::IfPrefix,
                                                        TemplatePatterns_C_::IfPrefixLength, content, offset, length);

            if ((next_if == 0) || (else_offset < next_if)) {
                // No nesting <ifs or <else before a sub-if.
                break;
            }

            offset = Engine::Find<Char_T_>(TemplatePatterns_C_::IfSuffix, TemplatePatterns_C_::IfSuffixLength, content,
                                           next_if, length);

            if (else_offset > offset) {
                // <else is after a sub-if.
                break;
            }
        }

        return else_offset;
    }

    void renderIf(const Char_T_ *content, const TagBit *tag) const {
        IfTag           *if_info = static_cast<IfTag *>(tag->GetInfo());
        IfTagCase       *item    = if_info->Storage();
        const IfTagCase *end     = (item + if_info->Size());

        content += (tag->GetOffset() + TemplatePatterns_C_::IfPrefixLength);

        while (item < end) {
            double result;

            if ((item->CaseLength == 0) ||
                (ALE::Evaluate(result, (content + item->CaseOffset), item->CaseLength, this) && (result > 0))) {
                if (item->SubTags.IsEmpty()) {
                    parse(item->SubTags, (content + item->ContentOffset), item->ContentLength, level_);
                }

                render((content + item->ContentOffset), item->ContentLength, item->SubTags);
                break;
            }

            ++item;
        }
    }

    const Value_T_ *findValue(const Char_T_ *key, SizeT length) const noexcept {
        const Value_T_ *value = nullptr;

        if (length != 0) {
            SizeT offset = 0;
            SizeT tmp    = 1;

            if (*key != TemplatePatterns_C_::TildeChar) {
                if ((key[(length - 1)] != TemplatePatterns_C_::VariableIndexSuffix)) {
                    return root_value_->GetValue(key, length);
                }

                // [...]
                while ((tmp < length) && (key[tmp] != TemplatePatterns_C_::VariableIndexPrefix)) {
                    ++tmp;
                }

                value = root_value_;
            } else {
                const Template_CV *obj = this;

                do {
                    ++offset;
                } while (key[offset] == TemplatePatterns_C_::TildeChar);

                if (offset > level_) {
                    return nullptr;
                }

                SizeT lvl = offset;

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

                value = obj->loop_value_;
                ++offset;
            }

            do {
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

        const Value_T_ *left_value  = nullptr;
        const Value_T_ *right_value = nullptr;

        const Char_T_ *left_content  = (content + left.Content.Offset);
        const Char_T_ *right_content = (content + right.Content.Offset);
        SizeT          left_length   = left.Content.Length;
        SizeT          right_length  = right.Content.Length;

        if (!left_evaluated) {
            if ((*left_content == TemplatePatterns_C_::InLinePrefix) &&
                (left_length > TemplatePatterns_C_::VariableFulllength)) {
                left_value = findValue((left_content + TemplatePatterns_C_::VariablePrefixLength),
                                       (left_length - TemplatePatterns_C_::VariableFulllength)); // {var:x}

                if (left_value != nullptr) {
                    if (left_value->IsNumber()) {
                        left_value->SetNumber(left.Number);
                        left_evaluated = true;
                    }
                } else {
                    return false;
                }
            } else {
                if (*left_content != ALEExpressions_T_::ParenthesStart) {
                    left_evaluated = Digit<Char_T_>::StringToNumber(left.Number, left_content, left_length);
                } else {
                    left_evaluated = ALE::Evaluate(left.Number, (left_content + 1), (left_length -= 2), this);
                }
            }
        }

        if (!right_evaluated) {
            if ((*right_content == TemplatePatterns_C_::InLinePrefix) &&
                (right_length > TemplatePatterns_C_::VariableFulllength)) {
                right_value = findValue((right_content + TemplatePatterns_C_::VariablePrefixLength),
                                        (right_length - TemplatePatterns_C_::VariableFulllength)); // {var:x}

                if (right_value != nullptr) {
                    if (right_value->IsNumber()) {
                        right_value->SetNumber(right.Number);
                        right_evaluated = true;
                    }
                } else {
                    return false;
                }
            } else if (left_evaluated) {
                if (*right_content != ALEExpressions_T_::ParenthesStart) {
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

    Template_CV(StringStream_T_ *stream, const Value_T_ *root_value, const Template_CV *parent = nullptr,
                SizeT level = 0) noexcept
        : ss_{stream}, root_value_{root_value}, parent_{parent}, level_{level} {}

    StringStream_T_   *ss_;
    const Value_T_    *root_value_;
    const Template_CV *parent_;
    const Value_T_    *loop_value_{nullptr};
    const Char_T_     *loop_key_{nullptr};
    SizeT              loop_key_length_{0};
    const SizeT        level_;
};

template <typename Char_T_, int S>
struct TPStrings {};

template <typename Char_T_>
struct TemplatePatterns {
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
