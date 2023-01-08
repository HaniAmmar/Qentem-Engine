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

#include "Array.hpp"
#include "Digit.hpp"
#include "Engine.hpp"
#include "HArray.hpp"
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
 *
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
 * <loop set="..." value="..." group="..." sort="...">...</loop>
 * <loop value="...">...</loop>
 * <loop set="..." value="...">...</loop>
 * <loop set="..." value="..." group="...">...</loop>
 * <loop set="..." value="..." sort="...">...</loop>
 * <loop set="..." value="..." group="..." sort="...">...</loop>
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
 * <loop value="loop1-value" sort="ascend|descend">
 *     loop1-value
 * </loop>
 *
 * <loop value="loop1-value" group="xyz">
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
struct ALEExpressions_T_;

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

    union ALENumber;

    template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    inline static void CachedRender(const Char_T_ *content, const SizeT length, const Value_T_ &value,
                                    StringStream_T_ &stream, const Char_T_ *name, const SizeT name_length) {
        // This is not a thread safe function, and its here to show how to cache geterated tags.
        // cab be used in single threaded prosesses, but it is better to change it.

        // Usage:
        // CachedRender("<html>...</html>", 16, value, stringstream, "page1", 5);

        using TemplateSubCV = TemplateSub<Char_T_, Value_T_, StringStream_T_>;

        const TemplateSubCV temp{content, &stream, &value};

        static HArray<Array<TagBit<Char_T_>>, Char_T_> cache;

        Array<TagBit<Char_T_>> &tags = cache.GetOrAdd(name, name_length);

        if (tags.IsEmpty()) {
            TemplateSubCV::Parse(tags, content, 0, length);
        }

        const TagBit<Char_T_> *tag = tags.First();
        const TagBit<Char_T_> *end = (tag + tags.Size());
        temp.Render(tag, end);
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    inline static void Render(const Char_T_ *content, SizeT length, const Value_T_ &value, StringStream_T_ &stream,
                              Array<TagBit<Char_T_>> &tags_cache) {
        using TemplateSubCV = TemplateSub<Char_T_, Value_T_, StringStream_T_>;
        const TemplateSubCV temp{content, &stream, &value};

        if (tags_cache.IsEmpty()) {
            TemplateSubCV::Parse(tags_cache, content, 0, length);
        }

        const TagBit<Char_T_> *tag = tags_cache.First();
        const TagBit<Char_T_> *end = (tag + tags_cache.Size());
        temp.Render(tag, end);
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_>
    inline static void Render(const Char_T_ *content, const SizeT length, const Value_T_ &value,
                              StringStream_T_ &stream) {
        Array<TagBit<Char_T_>> tags_cache;
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

    template <typename Char_T_, typename Value_T_, typename StringStream_T_ = StringStream<Char_T_>>
    inline static bool Evaluate(double &number, const Char_T_ *content, const Value_T_ &value) {
        // For testing only.

        using TemplateSubCV = TemplateSub<Char_T_, Value_T_, StringStream_T_>;
        StringStream_T_     stream;
        const TemplateSubCV temp{content, &stream, &value};
        ALENumber           num;

        bool ret = temp.evaluate(num, 0, StringUtils::Count(content));

        if (ret) {
            number = num.Number;
            return true;
        }

        number = 0;
        return false;
    }

    template <typename Char_T_, typename Value_T_, typename StringStream_T_ = StringStream<Char_T_>>
    inline static double Evaluate(const Char_T_ *content, const Value_T_ &value) {
        // For testing only.
        double number;
        Evaluate(number, content, value);
        return number;
    }

    union ALENumber {
        double Number;

        struct {
            // int is half the size of double. DONT change the type, or it will break.
            unsigned int Offset;
            unsigned int Length;
        } Content{0, 0};
    };

    enum TagType : unsigned char {
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
        unsigned short         SetOffset;
        unsigned short         SetLength;
        unsigned short         GroupOffset;
        unsigned short         GroupLength;
        unsigned short         Sort;
    };

    template <typename Char_T_>
    struct InlineIfTag_T_ {
        Array<TagBit<Char_T_>> SubTags;
        SizeT                  CaseOffset;
        SizeT                  CaseEndOffset;
        SizeT                  TrueTagsSize;
    };

    template <typename Char_T_>
    struct IfTagCase_T_ {
        Array<TagBit<Char_T_>> SubTags;
        const SizeT            CaseOffset;
        const SizeT            CaseEndOffset;
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

        TagBit(TagType type) : type_{type} {
            switch (type) {
                case TagType::Loop: {
                    LoopTag_T_<Char_T_> *loop_info = Memory::AllocateInit<LoopTag_T_<Char_T_>>();

                    info_ = loop_info;
                    break;
                }

                case TagType::InLineIf: {
                    InlineIfTag_T_<Char_T_> *i_if_info = Memory::AllocateInit<InlineIfTag_T_<Char_T_>>();

                    info_ = i_if_info;
                    break;
                }

                case TagType::If: {
                    Array<IfTagCase_T_<Char_T_>> *if_info = Memory::AllocateInit<Array<IfTagCase_T_<Char_T_>>>();

                    info_ = if_info;
                    break;
                }

                default: {
                }
            }
        }

        TagBit(TagType type, SizeT offset, SizeT length_) : type_{type} {
            content_.offset  = offset;
            content_.length_ = length_;
        }

        ~TagBit() {
            switch (type_) {
                case TagType::Loop: {
                    Memory::Dispose(static_cast<LoopTag_T_<Char_T_> *>(info_));
                    Memory::Deallocate(info_);
                    break;
                }

                case TagType::InLineIf: {
                    Memory::Dispose(static_cast<InlineIfTag_T_<Char_T_> *>(info_));
                    Memory::Deallocate(info_);
                    break;
                }

                case TagType::If: {
                    Memory::Dispose(static_cast<Array<IfTagCase_T_<Char_T_>> *>(info_));
                    Memory::Deallocate(info_);
                    break;
                }

                default: {
                }
            }
        }

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
#endif
        inline void *GetInfo() const noexcept { return info_; }
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
        inline TagType GetType() const noexcept { return type_; }
        inline SizeT   GetOffset() const noexcept { return content_.offset; }
        inline SizeT   GetLength() const noexcept { return content_.length_; }

      private:
        struct Content_ {
            SizeT offset;
            SizeT length_;
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

    TemplateSub(const Char_T_ *content, StringStream_T_ *stream, const Value_T_ *root_value,
                const TemplateSub *parent = nullptr, const SizeT level = 0) noexcept
        : content_{content}, stream_{stream}, root_value_{root_value}, parent_{parent}, level_{level} {}

  private:
    friend struct Qentem::Template;

    using ALENumber        = Template::ALENumber;
    using TagBit           = Template::TagBit<Char_T_>;
    using TagType          = Template::TagType;
    using LoopTag          = Template::LoopTag_T_<Char_T_>;
    using InlineIfTag      = Template::InlineIfTag_T_<Char_T_>;
    using IfTagCase        = Template::IfTagCase_T_<Char_T_>;
    using IfTag            = Array<Template::IfTagCase_T_<Char_T_>>;
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
                    renderLoop(static_cast<LoopTag *>(tag->GetInfo()));
                    break;
                }

                case TagType::InLineIf: {
                    renderInLineIf(static_cast<InlineIfTag *>(tag->GetInfo()));
                    break;
                }

                case TagType::If: {
                    renderIf(static_cast<IfTag *>(tag->GetInfo()));
                    break;
                }

                case TagType::RawText: {
                    stream_->Insert((content_ + tag->GetOffset()), tag->GetLength());
                    break;
                }

                default: {
                }
            }

            ++tag;
        }
    }

    static void Parse(Array<TagBit> &tags_cache, const Char_T_ *content, SizeT offset, const SizeT end_offset,
                      SizeT const level = 0) {
        SizeT last_offset = offset;

        while (offset < end_offset) {
            if (content[offset] == TemplatePatterns::InLinePrefix) {
                const SizeT current_offset = (offset + 1);

                switch (content[current_offset]) {
                    case TemplatePatterns::Var_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::VariablePrefix + 2), (content + current_offset + 1),
                                                 (TemplatePatterns::VariablePrefixLength - 2))) {
                            const SizeT v_end_offset = Engine::FindOne<Char_T_>(
                                TemplatePatterns::InLineSuffix, content,
                                (current_offset + TemplatePatterns::VariablePrefixLength), end_offset);

                            if (v_end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                                }

                                offset += TemplatePatterns::VariablePrefixLength;
                                tags_cache += TagBit{TagType::Variable, offset,
                                                     ((v_end_offset - TemplatePatterns::InLineSuffixLength) - offset)};
                                last_offset = v_end_offset;
                                offset      = v_end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns::Raw_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::RawVariablePrefix + 2),
                                                 (content + current_offset + 1),
                                                 (TemplatePatterns::RawVariablePrefixLength - 2))) {
                            const SizeT r_end_offset = Engine::FindOne<Char_T_>(
                                TemplatePatterns::InLineSuffix, content,
                                (current_offset + TemplatePatterns::RawVariablePrefixLength), end_offset);

                            if (r_end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                                }

                                offset += TemplatePatterns::RawVariablePrefixLength;
                                tags_cache += TagBit{TagType::RawVariable, offset,
                                                     ((r_end_offset - TemplatePatterns::InLineSuffixLength) - offset)};
                                last_offset = r_end_offset;
                                offset      = r_end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns::Math_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::MathPrefix + 2), (content + current_offset + 1),
                                                 (TemplatePatterns::MathPrefixLength - 2))) {
                            const SizeT m_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TemplatePatterns::InLinePrefix, TemplatePatterns::InLineSuffix, content,
                                (current_offset + TemplatePatterns::MathPrefixLength - 1), end_offset);

                            if (m_end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                                }

                                offset += TemplatePatterns::MathPrefixLength;
                                tags_cache += TagBit{TagType::Math, offset,
                                                     ((m_end_offset - TemplatePatterns::InLineSuffixLength) - offset)};
                                last_offset = m_end_offset;
                                offset      = m_end_offset;
                                continue;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns::InlineIf_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::InLineIfPrefix + 2), (content + current_offset + 1),
                                                 (TemplatePatterns::InLineIfPrefixLength - 2))) {
                            const SizeT ii_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TemplatePatterns::InLinePrefix, TemplatePatterns::InLineSuffix, content,
                                (current_offset + TemplatePatterns::InLineIfPrefixLength - 1), end_offset);

                            if (ii_end_offset != 0) {
                                if (last_offset != offset) {
                                    tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                                }

                                offset += TemplatePatterns::InLineIfPrefixLength;
                                tags_cache += TagBit{TagType::InLineIf};
                                generateInLineIfInfo(content, offset,
                                                     (ii_end_offset - TemplatePatterns::InLineSuffixLength),
                                                     static_cast<InlineIfTag *>(tags_cache.Last()->GetInfo()));
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
            } else if (content[offset] == TemplatePatterns::MultiLinePrefix) { // <loop
                const SizeT current_offset = (offset + 1);

                if (content[current_offset] == TemplatePatterns::Loop_2ND_Char) {
                    if (StringUtils::IsEqual((TemplatePatterns::LoopPrefix + 2), (content + current_offset + 1),
                                             (TemplatePatterns::LoopPrefixLength - 2))) {
                        const SizeT l_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                            TemplatePatterns::LoopPrefix, TemplatePatterns::LoopPrefixLength,
                            TemplatePatterns::LoopSuffix, TemplatePatterns::LoopSuffixLength, content,
                            (current_offset + TemplatePatterns::InLineIfPrefixLength - 1), end_offset);

                        if (l_end_offset != 0) {
                            if (last_offset != offset) {
                                tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::InLineIfPrefixLength;
                            tags_cache += TagBit{TagType::Loop};
                            generateLoopContent(content, offset, (l_end_offset - TemplatePatterns::LoopSuffixLength),
                                                static_cast<LoopTag *>(tags_cache.Last()->GetInfo()), level);
                            last_offset = l_end_offset;
                            offset      = l_end_offset;
                            continue;
                        }
                    }
                } else if (content[current_offset] == TemplatePatterns::If_2ND_Char) { // <if
                    if (StringUtils::IsEqual((TemplatePatterns::IfPrefix + 2), (content + current_offset + 1),
                                             (TemplatePatterns::IfPrefixLength - 2))) {
                        const SizeT i_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                            TemplatePatterns::IfPrefix, TemplatePatterns::IfPrefixLength, TemplatePatterns::IfSuffix,
                            TemplatePatterns::IfSuffixLength, content, current_offset, end_offset);

                        if (i_end_offset != 0) {
                            if (last_offset != offset) {
                                tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::IfPrefixLength;
                            tags_cache += TagBit{TagType::If};
                            generateIfCases(content, offset, (i_end_offset - TemplatePatterns::IfSuffixLength),
                                            static_cast<IfTag *>(tags_cache.Last()->GetInfo()), level);
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
            tags_cache += TagBit{TagType::RawText, last_offset, (end_offset - last_offset)};
        }
    }

  private:
    // Part of parse()
    static void lightParse(Array<TagBit> &tags_cache, const Char_T_ *content, SizeT offset, const SizeT end_offset) {
        SizeT last_offset = offset;

        while (true) {
            offset = Engine::FindOne<Char_T_>(TemplatePatterns::InLinePrefix, content, offset, end_offset);

            if (offset != 0) {
                switch (content[offset]) {
                    case TemplatePatterns::Var_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::VariablePrefix + 2), (content + offset + 1),
                                                 (TemplatePatterns::VariablePrefixLength - 2))) {
                            const SizeT v_end_offset =
                                Engine::FindOne<Char_T_>(TemplatePatterns::InLineSuffix, content,
                                                         (offset + TemplatePatterns::VariablePrefixLength), end_offset);
                            offset -= TemplatePatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::VariablePrefixLength;
                            tags_cache += TagBit{TagType::Variable, offset,
                                                 ((v_end_offset - TemplatePatterns::InLineSuffixLength) - offset)};
                            last_offset = v_end_offset;
                            offset      = v_end_offset;
                        }

                        continue;
                    }

                    case TemplatePatterns::Raw_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::RawVariablePrefix + 2), (content + offset + 1),
                                                 (TemplatePatterns::RawVariablePrefixLength - 2))) {
                            const SizeT r_end_offset = Engine::FindOne<Char_T_>(
                                TemplatePatterns::InLineSuffix, content,
                                (offset + TemplatePatterns::RawVariablePrefixLength), end_offset);
                            offset -= TemplatePatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::RawVariablePrefixLength;
                            tags_cache += TagBit{TagType::RawVariable, offset,
                                                 ((r_end_offset - TemplatePatterns::InLineSuffixLength) - offset)};
                            last_offset = r_end_offset;
                            offset      = r_end_offset;
                        }

                        continue;
                    }

                    case TemplatePatterns::Math_2ND_Char: {
                        if (StringUtils::IsEqual((TemplatePatterns::MathPrefix + 2), (content + offset + 1),
                                                 (TemplatePatterns::MathPrefixLength - 2))) {
                            const SizeT m_end_offset = Engine::SkipInnerPatterns<Char_T_>(
                                TemplatePatterns::InLinePrefix, TemplatePatterns::InLineSuffix, content,
                                (offset + TemplatePatterns::MathPrefixLength - 1), end_offset);
                            offset -= TemplatePatterns::InLinePrefixLength;

                            if (last_offset != offset) {
                                tags_cache += TagBit{TagType::RawText, last_offset, (offset - last_offset)};
                            }

                            offset += TemplatePatterns::MathPrefixLength;
                            tags_cache += TagBit{TagType::Math, offset,
                                                 ((m_end_offset - TemplatePatterns::InLineSuffixLength) - offset)};
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
            tags_cache += TagBit{TagType::RawText, last_offset, (end_offset - last_offset)};
        }
    }

    static void escapeHTMLSpecialChars(StringStream_T_ *stream, const Char_T_ *str, SizeT length) {
        SizeT offset = 0;
        SizeT index  = 0;

        while (index < length) {
            switch (str[index]) {
                case '&': {
                    stream->Insert((str + offset), (index - offset));
                    stream->Insert(TemplatePatterns::HTMLAnd, TemplatePatterns::HTMLAndLength);
                    offset = (++index);
                    break;
                }

                case '<': {
                    stream->Insert((str + offset), (index - offset));
                    stream->Insert(TemplatePatterns::HTMLLess, TemplatePatterns::HTMLLessLength);
                    offset = (++index);
                    break;
                }

                case '>': {
                    stream->Insert((str + offset), (index - offset));
                    stream->Insert(TemplatePatterns::HTMLBigger, TemplatePatterns::HTMLBiggerLength);
                    offset = (++index);
                    break;
                }

                case '"': {
                    stream->Insert((str + offset), (index - offset));
                    stream->Insert(TemplatePatterns::HTMLQuote, TemplatePatterns::HTMLQuoteLength);
                    offset = (++index);
                    break;
                }

                case '\'': {
                    stream->Insert((str + offset), (index - offset));
                    stream->Insert(TemplatePatterns::HTMLSingleQuote, TemplatePatterns::HTMLSingleQuoteLength);
                    offset = (++index);
                    break;
                }

                default: {
                    ++index;
                }
            }
        }

        stream->Insert((str + offset), (length - offset));
    }

    void renderVariable(const TagBit *tag) const {
        const Char_T_  *content = (content_ + tag->GetOffset());
        const Value_T_ *value   = findValue(content, tag->GetLength());

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

            if ((*content == TemplatePatterns::TildeChar) && (loop_key_ != nullptr)) {
                if (Config::AutoEscapeHTML) {
                    escapeHTMLSpecialChars(stream_, loop_key_, loop_key_length_);
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

    void renderRawVariable(const TagBit *tag) const {
        const Char_T_  *content = (content_ + tag->GetOffset());
        const Value_T_ *value   = findValue(content, tag->GetLength());

        if ((value != nullptr) && value->CopyStringValueTo(*stream_)) {
            return;
        }

        if (*content != TemplatePatterns::TildeChar) {
            stream_->Insert((content - TemplatePatterns::RawVariablePrefixLength),
                            (tag->GetLength() + TemplatePatterns::RawVariableFulllength));
        }
    }

    void renderMath(const TagBit *tag) const {
        ALENumber number;

        if (evaluate(number, tag->GetOffset(), (tag->GetLength() + tag->GetOffset()))) {
            Digit<Char_T_>::NumberToString(*stream_, number.Number, 1, 0, 3);
        } else {
            stream_->Insert(((content_ + tag->GetOffset()) - TemplatePatterns::MathPrefixLength),
                            (tag->GetLength() + TemplatePatterns::MathFulllength));
        }
    }

    void renderLoop(const LoopTag *tag) const {
        // Stage 3: Data
        Value_T_        grouped_set;
        const Value_T_ *loop_set;

        // Set (Array|Object)
        if (tag->SetLength != 0) {
            loop_set = findValue((content_ + tag->SetOffset), tag->SetLength);
        } else {
            loop_set = root_value_;
        }

        if (loop_set == nullptr) {
            return;
        }

        // Group
        if (tag->GroupLength != 0) {
            if (!(loop_set->GroupBy(grouped_set, (content_ + tag->GroupOffset), tag->GroupLength))) {
                return;
            }

            loop_set = &grouped_set;
        }

        // Sort
        if (tag->Sort != 0) {
            if (tag->GroupLength == 0) {
                grouped_set = *loop_set;
                loop_set    = &grouped_set;
            }

            grouped_set.Sort(tag->Sort == 1);
        }

        // Stage 4: Render
        const Char_T_ *loop_content = tag->InnerTemplate.First();
        TemplateSub    loop_template{loop_content, stream_, root_value_, this, (level_ + 1)};

        const SizeT loop_size  = loop_set->Size();
        SizeT       loop_index = 0;

        if (loop_set->IsObject()) {
            while (loop_index < loop_size) {
                loop_template.loop_value_ = loop_set->GetValue(loop_index);

                if (loop_template.loop_value_ != nullptr) {
                    loop_set->SetKeyCharAndLength(loop_index, loop_template.loop_key_, loop_template.loop_key_length_);
                    const TagBit *s_tag = tag->SubTags.First();
                    const TagBit *s_end = (s_tag + tag->SubTags.Size());
                    loop_template.Render(s_tag, s_end);
                }

                ++loop_index;
            }
        } else {
            while (loop_index < loop_size) {
                loop_template.loop_value_ = loop_set->GetValue(loop_index);

                if (loop_template.loop_value_ != nullptr) {
                    const TagBit *s_tag = tag->SubTags.First();
                    const TagBit *s_end = (s_tag + tag->SubTags.Size());
                    loop_template.Render(s_tag, s_end);
                }

                ++loop_index;
            }
        }
    }

    void renderInLineIf(const InlineIfTag *tag) const {
        ALENumber result;

        if (evaluate(result, tag->CaseOffset, tag->CaseEndOffset)) {
            if (result.Number > 0.0) {
                if (tag->TrueTagsSize != 0) {
                    const TagBit *s_tag = tag->SubTags.First();
                    const TagBit *s_end = (s_tag + tag->SubTags.Size());

                    if (tag->SubTags.Size() != tag->TrueTagsSize) {
                        s_end -= tag->TrueTagsSize;
                    }

                    Render(s_tag, s_end);
                }
            } else if (tag->SubTags.IsNotEmpty()) {
                const TagBit *s_tag = tag->SubTags.First();
                const TagBit *s_end = (s_tag + tag->SubTags.Size());
                s_tag += tag->TrueTagsSize;

                Render(s_tag, s_end);
            }
        }
    }

    void renderIf(const IfTag *tag) const {
        const IfTagCase *item = tag->First();
        const IfTagCase *end  = (item + tag->Size());
        ALENumber        result;

        while (item < end) {
            // <else> without if = (item->CaseLength == 0)
            if ((item->CaseOffset == item->CaseEndOffset) ||
                (evaluate(result, item->CaseOffset, item->CaseEndOffset) && (result.Number > 0))) {
                const TagBit *s_tag = item->SubTags.First();
                const TagBit *s_end = (s_tag + item->SubTags.Size());
                Render(s_tag, s_end);
                break;
            }

            ++item;
        }
    }

    /*
     * Gets everything between "..."
     */

    static SizeT getQuotedRemove(const Char_T_ *content, SizeT &offset, SizeT end_offset) noexcept {
        offset = Engine::FindOne<Char_T_>(TemplatePatterns::QuoteChar, content, offset, end_offset);

        if (offset != 0) {
            end_offset = Engine::FindOne<Char_T_>(TemplatePatterns::QuoteChar, content, offset, end_offset);

            if (end_offset != 0) {
                return ((end_offset - 1) - offset);
            }
        }

        return 0;
    }

    static SizeT getQuotedValue(const Char_T_ *content, SizeT &offset, SizeT end_offset) noexcept {
        offset = Engine::FindOne<Char_T_>(TemplatePatterns::QuoteChar, content, offset, end_offset);

        if (offset != 0) {
            end_offset = Engine::FindOne<Char_T_>(TemplatePatterns::QuoteChar, content, offset, end_offset);
        }

        return end_offset;
    }

    static void generateLoopContent(const Char_T_ *content, SizeT offset, SizeT end_offset, LoopTag *tag,
                                    const SizeT level) {
        const Char_T_ *loop_value        = nullptr;
        SizeT          tmp_length        = 0;
        SizeT          loop_value_length = 0;
        SizeT          previous_offset   = 0;
        SizeT          tmp_offset;
        bool           break_loop;

        const SizeT start_offset =
            Engine::FindOne<Char_T_>(TemplatePatterns::MultiLineSuffix, content, offset, end_offset);

        // Stage 1: Info extraction
        while (true) {
            ++tmp_length;
            offset += tmp_length; // Move to the next Char_T_.

            tmp_length = getQuotedRemove(content, offset, start_offset);

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

                    case TemplatePatterns::GroupChar: {
                        tag->GroupOffset = static_cast<unsigned short>(offset);
                        tag->GroupLength = static_cast<unsigned short>(tmp_length);
                        break_loop       = true;
                        break;
                    }

                    case TemplatePatterns::SortChar: {
                        tag->Sort  = ((content[offset] == 'a') ? 1 : 2);
                        break_loop = true;
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
                    offset = Engine::Find<Char_T_>(loop_value, loop_value_length, content, previous_offset, end_offset);
                } else {
                    offset = Engine::FindOne<Char_T_>(*loop_value, content, previous_offset, end_offset);
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
                           (sub_offset < end_offset)) {
                        ++sub_offset;
                    }

                    ++sub_offset;
                }

                if (sub_offset > end_offset) {
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

        tag->InnerTemplate.Insert((content + previous_offset), (end_offset - previous_offset));

        Parse(tag->SubTags, tag->InnerTemplate.First(), 0, tag->InnerTemplate.Length(), (level + 1));
    }

    void static generateInLineIfInfo(const Char_T_ *content, SizeT offset, SizeT end_offset, InlineIfTag *tag) {
        SizeT offset2 = getQuotedValue(content, offset, end_offset);
        // (=) plus (") plus (two chars) = 4 plus (the char before them) = 5

        SizeT true_offset      = 0;
        SizeT true_end_offset  = 0;
        SizeT false_offset     = 0;
        SizeT false_end_offset = 0;

        SizeT last_offset = offset;
        offset -= 5U;

        while ((offset2 != 0) && (offset < end_offset)) {
            switch (content[offset]) {
                case TemplatePatterns::CaseChar: {
                    tag->CaseOffset    = last_offset;
                    tag->CaseEndOffset = (offset2 - 1); // 1 = (") at the end
                    offset             = offset2;
                    offset2            = getQuotedValue(content, offset, end_offset);
                    last_offset        = offset;
                    offset -= 5U;
                    break;
                }

                case TemplatePatterns::TrueChar: {
                    true_offset     = last_offset;
                    true_end_offset = (offset2 - 1);
                    offset          = offset2;
                    offset2         = getQuotedValue(content, offset, end_offset);
                    last_offset     = offset;
                    offset -= 5U;
                    break;
                }

                case TemplatePatterns::FalseChar: {
                    false_offset     = last_offset;
                    false_end_offset = (offset2 - 1);
                    offset           = offset2;
                    offset2          = getQuotedValue(content, offset, end_offset);
                    last_offset      = offset;
                    offset -= 5U;
                    break;
                }

                default: {
                    ++offset;
                }
            }
        }

        if (tag->CaseOffset != tag->CaseEndOffset) {
            if (true_offset != true_end_offset) {
                lightParse(tag->SubTags, content, true_offset, true_end_offset);
                tag->TrueTagsSize = tag->SubTags.Size();
            }

            if (false_offset != false_end_offset) {
                lightParse(tag->SubTags, content, false_offset, false_end_offset);
            }
        }
    }

    static void generateIfCases(const Char_T_ *content, SizeT offset, const SizeT end_offset, IfTag *tag,
                                const SizeT level) {
        Array<TagBit> sub_tags;
        SizeT         offset2 = getQuotedValue(content, offset, end_offset);
        SizeT         case_end_offset; // 1 = (") at the end
        SizeT         else_offset;
        SizeT         content_offset;

        while ((offset2 != 0) && (offset < end_offset)) {
            case_end_offset = (offset2 - 1);
            content_offset  = Engine::FindOne<Char_T_>(TemplatePatterns::MultiLineSuffix, content, offset2, end_offset);

            if (content_offset != 0) {
                else_offset = nextElse(content, content_offset, end_offset);

                if (else_offset == 0) {
                    Parse(sub_tags, content, content_offset, end_offset, level);
                    *tag += IfTagCase{static_cast<Array<TagBit> &&>(sub_tags), offset, case_end_offset};

                    break;
                }

                Parse(sub_tags, content, content_offset, (else_offset - TemplatePatterns::ElsePrefixLength), level);
                *tag += IfTagCase{static_cast<Array<TagBit> &&>(sub_tags), offset, case_end_offset};

                if ((content[else_offset] != TemplatePatterns::ElseIfChar)) {
                    else_offset =
                        Engine::FindOne<Char_T_>(TemplatePatterns::MultiLineSuffix, content, else_offset, end_offset);

                    if (else_offset != 0) {
                        Parse(sub_tags, content, else_offset, end_offset, level);
                        *tag += IfTagCase{static_cast<Array<TagBit> &&>(sub_tags), 0, 0}; // else without if
                    }

                    break;
                }

                offset  = else_offset;
                offset2 = getQuotedValue(content, offset, end_offset);
                continue;
            }

            break;
        }
    }

    static SizeT nextElse(const Char_T_ *content, SizeT offset, const SizeT end_offset) noexcept {
        SizeT else_offset = 0;

        while (true) {
            else_offset = Engine::Find<Char_T_>(TemplatePatterns::ElsePrefix, TemplatePatterns::ElsePrefixLength,
                                                content, offset, end_offset);

            if (else_offset == 0) {
                // No <else.
                break;
            }

            const SizeT next_if = Engine::Find<Char_T_>(TemplatePatterns::IfPrefix, TemplatePatterns::IfPrefixLength,
                                                        content, offset, end_offset);

            if ((next_if == 0) || (else_offset < next_if)) {
                // No nesting <ifs or <else before a sub-if.
                break;
            }

            offset = Engine::Find<Char_T_>(TemplatePatterns::IfSuffix, TemplatePatterns::IfSuffixLength, content,
                                           next_if, end_offset);

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

    enum ALEExpression {
        None,
        Or,             // ||
        And,            // &&
        Equal,          // ==
        NotEqual,       // !=
        BiggerOrEqual,  // >=
        LessOrEqual,    // <=
        Bigger,         // >
        Less,           // <
        Addition,       // +
        Subtraction,    // -
        Multiplication, // *
        Division,       // /
        Remainder,      // %
        Exponent,       // ^
        Error           // X
    };

    bool evaluate(ALENumber &number, SizeT offset, const SizeT end_offset) const noexcept {
        const SizeT   num_offset = offset;
        ALEExpression expr       = getExpression(offset, end_offset);
        return subEvaluate(number, offset, num_offset, end_offset, expr, ALEExpression::None);
    }

    bool subEvaluate(ALENumber &left, SizeT &offset, SizeT num_offset, const SizeT end_offset, ALEExpression &expr,
                     const ALEExpression previous_expr) const noexcept {
        ALENumber     right;
        ALEExpression next_expr;

        if (expr != ALEExpression::Error) {
            const bool no_equal = ((expr != ALEExpression::Equal) && (expr != ALEExpression::NotEqual));

            if (!getExpressionValue(left, num_offset, offset, no_equal)) {
                return false;
            }

            while (offset < end_offset) {
                ++offset;

                if (expr < ALEExpression::Bigger) {
                    ++offset;
                }

                num_offset = offset;
                next_expr  = getExpression(offset, end_offset);

                if (expr >= next_expr) {
                    if (!getExpressionValue(right, num_offset, offset,
                                            ((expr != ALEExpression::Equal) && (expr != ALEExpression::NotEqual))) ||
                        !processExpression(left, right, no_equal, false, expr)) {
                        return false;
                    }

                    expr = next_expr;

                    if (previous_expr >= next_expr) {
                        return true;
                    }
                } else {
                    if (!subEvaluate(right, offset, num_offset, end_offset, next_expr, expr) ||
                        !processExpression(left, right, no_equal, true, expr)) {
                        return false;
                    }

                    expr = next_expr;
                }
            }

            return true;
        }

        return false;
    }

    bool getExpressionValue(ALENumber &val, SizeT offset, SizeT end_offset, bool no_equal) const noexcept {
        SizeT length = (end_offset - offset);
        StringUtils::Trim(content_, offset, length);

        if (no_equal) {
            return getExpressionNumber(val, offset, (length + offset));
        }

        val.Content.Offset = static_cast<unsigned int>(offset);
        val.Content.Length = static_cast<unsigned int>(length);

        return true;
    }

    bool getExpressionNumber(ALENumber &val, SizeT offset, SizeT end_offset) const noexcept {
        using ALEExpressions = ALEExpressions_T_<Char_T_>;

        switch (content_[offset]) {
            case ALEExpressions::ParenthesStart: {
                // getExpression check for closed parenthes, so "length" will never go over the actual length.
                ++offset;     // after (
                --end_offset; // before )

                return evaluate(val, offset, end_offset);
            }

            case ALEExpressions::BracketStart: {
                return (setNumber(val, offset, end_offset));
            }

            default: {
                return (Digit<Char_T_>::StringToNumber(val.Number, (content_ + offset), (end_offset - offset)));
            }
        }
    }

    bool processExpression(ALENumber &left, ALENumber right, bool left_evaluated, bool right_evaluated,
                           ALEExpression expr) const noexcept {
        switch (expr) {
            case ALEExpression::Exponent: { // ^
                if (right.Number != 0.0) {
                    // TODO: Needs more work to evaluate fractions
                    if (left.Number != 0.0) {
                        const bool neg = (right.Number < 0);

                        if (neg) {
                            right.Number = -right.Number; // neg ASM
                        }

                        if (right.Number < 1) {
                            return false;
                        }

                        unsigned int times = static_cast<unsigned int>(right.Number);
                        const double num   = left.Number;

                        while (--times != 0) {
                            left.Number *= num;
                        }

                        if (neg) {
                            left.Number = (1 / left.Number);
                        }
                    }

                    break;
                }

                left.Number = 1;
                break;
            }

            case ALEExpression::Remainder: { // %
                left.Number = static_cast<double>(static_cast<unsigned long long>(left.Number) %
                                                  static_cast<unsigned long long>(right.Number));
                break;
            }

            case ALEExpression::Multiplication: { // *
                left.Number *= right.Number;
                break;
            }

            case ALEExpression::Division: { // /
                if (right.Number != 0.0) {
                    left.Number /= right.Number;
                    break;
                }

                return false;
            }

            case ALEExpression::Addition: { // +
                left.Number += right.Number;
                break;
            }

            case ALEExpression::Subtraction: { // -
                left.Number -= right.Number;
                break;
            }

            case ALEExpression::Less: { // <
                left.Number = (left.Number < right.Number) ? 1 : 0;
                break;
            }

            case ALEExpression::LessOrEqual: { // <=
                left.Number = (left.Number <= right.Number) ? 1 : 0;
                break;
            }

            case ALEExpression::Bigger: { // >
                left.Number = (left.Number > right.Number) ? 1 : 0;
                break;
            }

            case ALEExpression::BiggerOrEqual: { // >=
                left.Number = (left.Number >= right.Number) ? 1 : 0;
                break;
            }

            case ALEExpression::And: { // &&
                left.Number = ((left.Number > 0) && (right.Number > 0)) ? 1 : 0;
                break;
            }

            case ALEExpression::Or: { // ||
                left.Number = ((left.Number > 0) || (right.Number > 0)) ? 1 : 0;
                break;
            }

            case ALEExpression::Equal:      // ==
            case ALEExpression::NotEqual: { // !=
                bool is_equal;

                if (isEqual(is_equal, left, right, left_evaluated, right_evaluated)) {
                    if (expr == ALEExpression::Equal) {
                        left.Number = (is_equal ? 1 : 0);
                    } else {
                        left.Number = (is_equal ? 0 : 1);
                    }

                    break;
                }

                return false;
            }

            default: {
            }
        }

        return true;
    }

    ALEExpression getExpression(SizeT &offset, const SizeT end_offset) const noexcept {
        using ALEExpressions = ALEExpressions_T_<Char_T_>;

        while (offset < end_offset) {
            switch (content_[offset]) {
                case ALEExpressions::OrExp: { // ||
                    if (content_[(offset + 1)] == ALEExpressions::OrExp) {
                        return ALEExpression::Or;
                    }

                    return ALEExpression::Error;
                }

                case ALEExpressions::AndExp: { // &&
                    if (content_[(offset + 1)] == ALEExpressions::AndExp) {
                        return ALEExpression::And;
                    }

                    return ALEExpression::Error;
                }

                case ALEExpressions::BiggerExp: { // > or >=
                    if (content_[(offset + 1)] == ALEExpressions::EqualExp) {
                        return ALEExpression::BiggerOrEqual;
                    }

                    return ALEExpression::Bigger;
                }

                case ALEExpressions::LessExp: { // < or <=
                    if (content_[(offset + 1)] == ALEExpressions::EqualExp) {
                        return ALEExpression::LessOrEqual;
                    }

                    return ALEExpression::Less;
                }

                case ALEExpressions::NotExp: { // !=
                    if (content_[(offset + 1)] == ALEExpressions::EqualExp) {
                        return ALEExpression::NotEqual;
                    }

                    return ALEExpression::Error;
                }

                case ALEExpressions::EqualExp: { // ==
                    if (content_[(offset + 1)] == ALEExpressions::EqualExp) {
                        return ALEExpression::Equal;
                    }

                    return ALEExpression::Error;
                }

                case ALEExpressions::SubtractExp: {
                    if (isExpression(offset)) {
                        return ALEExpression::Subtraction;
                    }

                    break;
                }

                case ALEExpressions::AddExp: {
                    if (isExpression(offset)) {
                        return ALEExpression::Addition;
                    }

                    break;
                }

                case ALEExpressions::DivideExp: {
                    return ALEExpression::Division;
                }

                case ALEExpressions::MultipleExp: {
                    return ALEExpression::Multiplication;
                }

                case ALEExpressions::RemainderExp: {
                    return ALEExpression::Remainder;
                }

                case ALEExpressions::ExponentExp: {
                    return ALEExpression::Exponent;
                }

                case ALEExpressions::ParenthesStart: {
                    // (...) are evaluated to numbers.

                    ++offset;
                    offset = Engine::SkipInnerPatterns<Char_T_>(
                        ALEExpressions::ParenthesStart, ALEExpressions::ParenthesEnd, content_, offset, end_offset);

                    if (offset != 0) {
                        continue;
                    }

                    return ALEExpression::Error;
                }

                case ALEExpressions::BracketStart: {
                    // {...} are evaluated by callback to a number or
                    // string.

                    ++offset;
                    offset = Engine::FindOne<Char_T_>(ALEExpressions::BracketEnd, content_, offset, end_offset);

                    if (offset != 0) {
                        continue;
                    }

                    offset = end_offset;
                    return ALEExpression::Error;
                }

                default: {
                }
            }

            ++offset;
        }

        return ALEExpression::None;
    }

    bool isExpression(SizeT offset) const noexcept {
        using ALEExpressions = ALEExpressions_T_<Char_T_>;

        while (offset != 0) {
            --offset;

            switch (content_[offset]) {
                case ALEExpressions::SpaceChar: {
                    break;
                }

                case ALEExpressions::ParenthesEnd:
                case ALEExpressions::BracketEnd: {
                    // (...) and {} are numbers.
                    return true;
                }

                default: {
                    // A number
                    const Char_T_ ch = content_[offset];
                    return ((ch < ALEExpressions::ColonChar) && (ch > ALEExpressions::SlashChar));
                }
            }
        }

        return false;
    }

    bool setNumber(ALENumber &number, SizeT offset, SizeT end_offset) const noexcept {
        const Value_T_ *value  = nullptr;
        SizeT           length = (end_offset - offset);

        if (length > TemplatePatterns::VariableFulllength) {
            value = findValue((content_ + TemplatePatterns::VariablePrefixLength + offset),
                              (length - TemplatePatterns::VariableFulllength));
        }

        return ((value != nullptr) && value->SetNumber(number.Number));
    }

    bool isEqual(bool &result, ALENumber left, ALENumber right, bool left_evaluated,
                 bool right_evaluated) const noexcept {
        using ALEExpressions = ALEExpressions_T_<Char_T_>;

        const Value_T_ *left_value  = nullptr;
        const Value_T_ *right_value = nullptr;

        const Char_T_ *left_content  = (content_ + left.Content.Offset);
        const Char_T_ *right_content = (content_ + right.Content.Offset);
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
                    left_evaluated =
                        evaluate(left, (left.Content.Offset + 1), ((left_length - 1) + left.Content.Offset));
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
            } else {
                if (*right_content != ALEExpressions::ParenthesStart) {
                    right_evaluated = Digit<Char_T_>::StringToNumber(right.Number, right_content, right_length);
                } else {
                    right_evaluated =
                        evaluate(right, (right.Content.Offset + 1), ((right_length - 1) + right.Content.Offset));
                }
            }
        }

        if (left_evaluated || right_evaluated) {
            if (left_evaluated || ((left_value != nullptr) && left_value->SetNumber(left.Number))) {
                if (right_evaluated || ((right_value != nullptr) && right_value->SetNumber(right.Number))) {
                    result = (left.Number == right.Number);
                    return true;
                }
            }
        } else if ((left_value == nullptr) || left_value->SetCharAndLength(left_content, left_length)) {
            if ((right_value == nullptr) || right_value->SetCharAndLength(right_content, right_length)) {
                if ((left_length != 0) && (right_length != 0)) {
                    result = ((left_length == right_length) &&
                              StringUtils::IsEqual(left_content, right_content, right_length));
                    return true;
                }
            }
        }

        return false;
    }

    const Char_T_     *content_;
    StringStream_T_   *stream_;
    const Value_T_    *root_value_;
    const TemplateSub *parent_;
    const Value_T_    *loop_value_{nullptr};
    const Char_T_     *loop_key_{nullptr};
    SizeT              loop_key_length_{0};
    const SizeT        level_;
};

template <typename Char_T_>
struct ALEExpressions_T_ {
  public:
    static constexpr Char_T_ RemainderExp = '%';
    static constexpr Char_T_ MultipleExp  = '*';
    static constexpr Char_T_ DivideExp    = '/';
    static constexpr Char_T_ AddExp       = '+';
    static constexpr Char_T_ SubtractExp  = '-';
    static constexpr Char_T_ EqualExp     = '=';
    static constexpr Char_T_ NotExp       = '!';
    static constexpr Char_T_ LessExp      = '<';
    static constexpr Char_T_ BiggerExp    = '>';
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
