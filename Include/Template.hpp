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

// TODO: Add autoescape

namespace Qentem {

/*
 * Tags:
 *
 *  - {var:s|n}
 *      - var: Variable, s: String, n: Number.
 *
 *  - {math:var|e|n}
 *      - var|e|n: Raw variable, Equation or Number.
 *
 *  - {if case="var|s" true="var|var|s" false="var|var|s"}
 *      - Inline if,  var: Raw variable, var: Variable, s: String.
 *
 *  <...>
 *  - <loop set="s"? value="s"? repeat="var|n"? index="var|n"?>
 *                                                          ...</loop>
 *      - s: String, n: Number, var: Raw ariable,
 *      - set: child name in the passed colloction: Optional.
 *      - value: the current value in the colloction: Optional.
 *          Note: Choose a unique name.
 *      - repeat: if set, it will be used instead of the "set" size.
 *      - index: starting index.
 *
 *  - <if case="var|e|n">...<else(if)? ... >?...</if>
 *      - var|e|n: Raw variable, Equation or Number.
 *      - <else if ....> same as if: Optional.
 *      - <else> no case: Optional.
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
 */

/*
 * If tag:
 *
 * <if case="{case}">...</if>
 * <if case="{case}">...<else />...</if>
 * <if case="{case1}">...<elseif case="{case2}" />...</if>
 * <if case="{case}">...<if case="{case2}" />...</if></if>
 */

template <typename, typename>
class Template_CV;

template <typename>
struct TemplatePatterns;

struct Template {
  public:
    template <typename>
    struct TagBit;

    template <typename Char_T_, typename Value_T_, typename Number_T_>
    inline static void
    Render(const Char_T_ *content, Number_T_ length, const Value_T_ *root_value,
           StringStream<Char_T_> *ss, Array<TagBit<Char_T_>> *tags_cache) {
        Template_CV<Char_T_, Value_T_>::Process(
            content, static_cast<SizeT>(length), root_value, ss, *tags_cache);
    }

    template <typename Char_T_, typename Value_T_, typename Number_T_>
    inline static void Render(const Char_T_ *content, Number_T_ length,
                              const Value_T_ *       root_value,
                              StringStream<Char_T_> *ss) {
        Array<TagBit<Char_T_>> tags_cache;
        Render(content, length, root_value, ss, &tags_cache);
    }

    template <typename Char_T_, typename Value_T_, typename Number_T_>
    inline static StringStream<Char_T_> Render(const Char_T_ * content,
                                               Number_T_       length,
                                               const Value_T_ *root_value) {
        StringStream<Char_T_> ss;
        Render(content, length, root_value, &ss);
        return ss;
    }

    template <typename Char_T_, typename Value_T_>
    inline static StringStream<Char_T_> Render(const Char_T_ * content,
                                               const Value_T_ *root_value) {
        return Render(content, StringUtils::Count(content), root_value);
    }

    enum class TagType : unsigned char {
        None = 0,
        Variable, // {var:x}
        Math,     // {math:x}
        InLineIf, // {if:x}
        Loop,     // <loop set="..." key="..." value="...">
        If,       // <if case="...">
    };

    template <typename Char_T_>
    struct LoopData_T {
        StringStream<Char_T_>  Content{};
        Array<TagBit<Char_T_>> SubTags{};
        unsigned char          SetOffset{0};
        unsigned char          SetLength{0};
        unsigned char          IndexOffset{0};
        unsigned char          IndexLength{0};
        unsigned char          RepeatOffset{0};
        unsigned char          RepeatLength{0};
    };

    template <typename Char_T_>
    struct IfCase_T {
        Array<TagBit<Char_T_>> SubTags{};
        SizeT                  CaseOffset{};
        SizeT                  CaseLength{};
        SizeT                  ContentOffset{};
        SizeT                  ContentLength{};
    };

    template <typename Char_T_>
    struct IfData_T {
        Array<IfCase_T<Char_T_>> Cases{1};
    };

    template <typename Char_T_>
    struct TagBit {
        using LoopData_ = LoopData_T<Char_T_>;
        using IfData_   = IfData_T<Char_T_>;

      public:
        TagBit() = default;

        TagBit(const TagBit &tag) = delete;
        TagBit &operator=(const TagBit &tag) = delete;

        TagBit(TagType type, SizeT offset, SizeT end_offset) noexcept
            : offset_(offset), end_offset_(end_offset) {
            if (type == TagType::Loop) {
                setData(Memory::AllocateInit<LoopData_>());
            } else if (type == TagType::If) {
                setData(Memory::AllocateInit<IfData_>());
            }

            setType(type);
        }

        TagBit(TagBit &&tag) noexcept
            : offset_(tag.offset_), end_offset_(tag.end_offset_),
              data_(static_cast<QPointer<void> &&>(tag.data_)) {
#if !defined(QENTEM_POINTER_TAGGING) || QENTEM_POINTER_TAGGING != 1
            type_ = tag.type_;
#endif

            tag.setType(TagType::None);
        }

        ~TagBit() {
            if (GetType() != TagType::None) {
                Reset();
            }
        }

        TagBit &operator=(TagBit &&tag) noexcept {
            if (this != &tag) {
                if (GetType() != TagType::None) {
                    Reset();
                }

                offset_     = tag.offset_;
                end_offset_ = tag.end_offset_;
                data_.Set(static_cast<QPointer<void> &&>(tag.data_));

#if !defined(QENTEM_POINTER_TAGGING) || QENTEM_POINTER_TAGGING != 1
                type_ = tag.type_;
#endif

                tag.setType(TagType::None);
            }

            return *this;
        }

        void Reset() {
            const TagType type = GetType();

            if (type == TagType::Loop) {
                LoopData_ *loop_data = GetLoopData();
                Memory::Destruct(loop_data);
                Memory::Deallocate(loop_data);
            } else if (type == TagType::If) {
                IfData_ *if_data = GetIfData();
                Memory::Destruct(if_data);
                Memory::Deallocate(if_data);
            }

            clearData();
        }

        inline LoopData_ *GetLoopData() const noexcept {
            return static_cast<LoopData_ *>(getData());
        }

        inline IfData_ *GetIfData() const noexcept {
            return static_cast<IfData_ *>(getData());
        }

        inline TagType GetType() const noexcept {
#if !defined(QENTEM_POINTER_TAGGING) || QENTEM_POINTER_TAGGING != 1
            return type_;
#else
            return static_cast<TagType>(data_.GetHighTag());
#endif
        }

        inline SizeT Offset() const noexcept { return offset_; }
        inline SizeT EndOffset() const noexcept { return end_offset_; }

      private:
        void setType(TagType type) noexcept {
#if !defined(QENTEM_POINTER_TAGGING) || QENTEM_POINTER_TAGGING != 1
            type_ = type;
#else
            data_.SetHighTag(static_cast<unsigned char>(type));
#endif
        }

        void         clearData() noexcept { data_.Reset(); }
        void         setData(void *ptr) noexcept { data_.SetPointer(ptr); }
        inline void *getData() const noexcept { return data_.GetPointer(); }

        SizeT          offset_{0};
        SizeT          end_offset_{0};
        QPointer<void> data_{};
#if !defined(QENTEM_POINTER_TAGGING) || QENTEM_POINTER_TAGGING != 1
        TagType type_{};
#endif
    };
};

template <typename Char_T_, typename Value_T_>
class Template_CV {
  private:
    friend class Qentem::ALE;
    friend class Qentem::Template;

    using TagBit    = Template::TagBit<Char_T_>;
    using TagType   = Template::TagType;
    using LoopData_ = Template::LoopData_T<Char_T_>;
    using IfCase_   = Template::IfCase_T<Char_T_>;
    using IfData_   = Template::IfData_T<Char_T_>;

    using TemplatePatterns_C_ = TemplatePatterns<Char_T_>;

  public:
    Template_CV() = delete;

    static void Process(const Char_T_ *content, SizeT length,
                        const Value_T_ *root_value, StringStream<Char_T_> *ss,
                        Array<TagBit> &tags_cache) {
        Template_CV temp{ss, root_value};
        temp.process(content, length, tags_cache);
    }

  private:
    void process(const Char_T_ *content, SizeT length,
                 Array<TagBit> &tags_cache) const {
        if (tags_cache.IsEmpty()) {
            parse(tags_cache, content, length);
        }

        if (tags_cache.IsNotEmpty()) {
            TagBit *      start = tags_cache.Storage();
            const TagBit *end   = (start + tags_cache.Size());
            render(start, end, content);
            const SizeT offset = (end - 1)->EndOffset();
            // Add the remaining string.
            ss_->Insert((content + offset), (length - offset));
        } else {
            ss_->Insert(content, length);
        }
    }

    QENTEM_NOINLINE static void parse(Array<TagBit> &tags_cache,
                                      const Char_T_ *content, SizeT length) {
        SizeT offset = 0;
        SizeT current_offset;
        SizeT tmp_offset;

        static const Char_T_ *variable_prefix =
            TemplatePatterns_C_::GetVariablePrefix();
        static const Char_T_ *math_prefix =
            TemplatePatterns_C_::GetMathPrefix();
        static const Char_T_ *inLine_if_prefix =
            TemplatePatterns_C_::GetInLineIfPrefix();
        static const Char_T_ *loop_prefix =
            TemplatePatterns_C_::GetLoopPrefix();
        static const Char_T_ *loop_suffix =
            TemplatePatterns_C_::GetLoopSuffix();
        static const Char_T_ *if_prefix = TemplatePatterns_C_::GetIfPrefix();
        static const Char_T_ *if_suffix = TemplatePatterns_C_::GetIfSuffix();

        static const Char_T_ *inline_suffix =
            TemplatePatterns_C_::GetInLineSuffix();

        while (offset < length) {
            if (content[offset] == TemplatePatterns_C_::InLinePrefix) {
                current_offset = offset;
                ++current_offset;

                switch (content[current_offset]) {
                    case TemplatePatterns_C_::Var_2ND_Char: {
                        if ((TemplatePatterns_C_::VariablePrefixLength +
                             current_offset) < length) {
                            tmp_offset = 1;

                            do {
                                ++current_offset;
                                ++tmp_offset;
                            } while (
                                (tmp_offset !=
                                 TemplatePatterns_C_::VariablePrefixLength) &&
                                (content[current_offset] ==
                                 variable_prefix[tmp_offset]));

                            if (tmp_offset ==
                                TemplatePatterns_C_::VariablePrefixLength) {
                                const SizeT end_offset =
                                    Engine::FindOne(*inline_suffix, content,
                                                    current_offset, length);

                                if (end_offset != 0) {
                                    tags_cache += TagBit{TagType::Variable,
                                                         offset, end_offset};
                                    offset = end_offset;
                                    continue;
                                }

                                offset = current_offset;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns_C_::Math_2ND_Char: {
                        if ((TemplatePatterns_C_::MathPrefixLength +
                             current_offset) < length) {
                            tmp_offset = 1;

                            do {
                                ++current_offset;
                                ++tmp_offset;
                            } while ((tmp_offset !=
                                      TemplatePatterns_C_::MathPrefixLength) &&
                                     (content[current_offset] ==
                                      math_prefix[tmp_offset]));

                            if (tmp_offset ==
                                TemplatePatterns_C_::MathPrefixLength) {
                                SizeT end_offset = Engine::SkipInnerPatterns(
                                    variable_prefix,
                                    TemplatePatterns_C_::VariablePrefixLength,
                                    inline_suffix, 1, content, current_offset,
                                    length);

                                if (end_offset != 0) {
                                    tags_cache += TagBit{TagType::Math, offset,
                                                         end_offset};
                                    offset = end_offset;
                                    continue;
                                }

                                offset = current_offset;
                            }
                        }

                        break;
                    }

                    case TemplatePatterns_C_::InlineIf_2ND_Char: {
                        if ((TemplatePatterns_C_::InLineIfPrefixLength +
                             current_offset) < length) {
                            tmp_offset = 1;

                            do {
                                ++current_offset;
                                ++tmp_offset;
                            } while (
                                (tmp_offset !=
                                 TemplatePatterns_C_::InLineIfPrefixLength) &&
                                (content[current_offset] ==
                                 inLine_if_prefix[tmp_offset]));

                            if (tmp_offset ==
                                TemplatePatterns_C_::InLineIfPrefixLength) {
                                SizeT end_offset = Engine::SkipInnerPatterns(
                                    variable_prefix,
                                    TemplatePatterns_C_::VariablePrefixLength,
                                    inline_suffix, 1, content, current_offset,
                                    length);

                                if (end_offset != 0) {
                                    tags_cache += TagBit{TagType::InLineIf,
                                                         offset, end_offset};
                                    offset = end_offset;
                                    continue;
                                }

                                offset = current_offset;
                            }
                        }
                    }
                }
            } else if (content[offset] ==
                       TemplatePatterns_C_::MultiLinePrefix) {
                current_offset = offset;
                ++current_offset;

                if (content[current_offset] ==
                    TemplatePatterns_C_::Loop_2ND_Char) { // <loop
                    if ((TemplatePatterns_C_::LoopPrefixLength +
                         current_offset) < length) {
                        tmp_offset = 1;

                        do {
                            ++current_offset;
                            ++tmp_offset;
                        } while ((tmp_offset <
                                  TemplatePatterns_C_::LoopPrefixLength) &&
                                 (content[current_offset] ==
                                  loop_prefix[tmp_offset]));

                        if (tmp_offset ==
                            TemplatePatterns_C_::LoopPrefixLength) {
                            SizeT end_offset = Engine::SkipInnerPatterns(
                                loop_prefix,
                                TemplatePatterns_C_::LoopPrefixLength,
                                loop_suffix,
                                TemplatePatterns_C_::LoopSuffixLength, content,
                                current_offset, length);

                            if (end_offset != 0) {
                                tags_cache +=
                                    TagBit{TagType::Loop, offset, end_offset};
                                offset = end_offset;
                                continue;
                            }

                            offset = current_offset;
                        }
                    }
                } else if (content[current_offset] ==
                           TemplatePatterns_C_::If_2ND_Char) { // <if
                    if ((TemplatePatterns_C_::IfPrefixLength + current_offset) <
                        length) {
                        tmp_offset = 1;

                        do {
                            ++current_offset;
                            ++tmp_offset;
                        } while (
                            (tmp_offset !=
                             TemplatePatterns_C_::IfPrefixLength) &&
                            (content[current_offset] == if_prefix[tmp_offset]));

                        if (tmp_offset == TemplatePatterns_C_::IfPrefixLength) {
                            SizeT end_offset = Engine::SkipInnerPatterns(
                                if_prefix, TemplatePatterns_C_::IfPrefixLength,
                                if_suffix, TemplatePatterns_C_::IfSuffixLength,
                                content, current_offset, length);

                            if (end_offset != 0) {
                                tags_cache +=
                                    TagBit{TagType::If, offset, end_offset};
                                offset = end_offset;
                                continue;
                            }

                            offset = current_offset;
                        }
                    }
                }
            }

            ++offset;
        }
    }

    QENTEM_NOINLINE void render(TagBit *tag, const TagBit *end,
                                const Char_T_ *content) const {
        SizeT previous_offset = 0;

        while (tag != end) {
            ss_->Insert((content + previous_offset),
                        (tag->Offset() - previous_offset));

            switch (tag->GetType()) {
                case TagType::Variable: {
                    const SizeT content_offset =
                        tag->Offset() +
                        TemplatePatterns_C_::VariablePrefixLength;

                    renderVariable((content + content_offset),
                                   ((tag->EndOffset() - 1) - content_offset));

                    // - 1 is - TemplatePatterns_C_::InLineSuffixLength
                    break;
                }

                case TagType::Math: {
                    const SizeT content_offset =
                        tag->Offset() + TemplatePatterns_C_::MathPrefixLength;

                    renderMath((content + content_offset),
                               ((tag->EndOffset() - 1) - content_offset));

                    // - 1 is - TemplatePatterns_C_::InLineSuffixLength
                    break;
                }

                case TagType::InLineIf: {
                    const SizeT content_offset =
                        tag->Offset() +
                        TemplatePatterns_C_::InLineIfPrefixLength;

                    renderInLineIf((content + content_offset),
                                   ((tag->EndOffset() - 1) - content_offset));

                    // - 1 is - TemplatePatterns_C_::InLineSuffixLength
                    break;
                }

                case TagType::Loop: {
                    const SizeT content_offset =
                        tag->Offset() + TemplatePatterns_C_::LoopPrefixLength;
                    LoopData_ *loop_data = tag->GetLoopData();

                    if (loop_data->Content.IsNotEmpty()) { // Cached
                        renderLoop((content + content_offset), loop_data);
                    } else {
                        generateLoopContent(
                            (content + content_offset),
                            ((tag->EndOffset() -
                              TemplatePatterns_C_::LoopSuffixLength) -
                             content_offset),
                            loop_data);
                    }

                    break;
                }

                case TagType::If: {
                    const SizeT content_offset =
                        tag->Offset() + TemplatePatterns_C_::IfPrefixLength;
                    IfData_ *if_data = tag->GetIfData();

                    if (if_data->Cases.IsNotEmpty()) {
                        renderIf((content + content_offset), if_data);
                    } else {
                        generateIfCases((content + content_offset),
                                        (tag->EndOffset() - content_offset),
                                        if_data);
                    }
                }

                default: {
                }
            }

            previous_offset = tag->EndOffset();
            ++tag;
        }
    }

    void renderVariable(const Char_T_ *content, SizeT length) const {
        const Value_T_ *value = findValue(content, length);

        if (((value == nullptr) || !(value->InsertString(*ss_))) &&
            (*content != TemplatePatterns_C_::TildeChar)) {
            ss_->Insert((content - TemplatePatterns_C_::VariablePrefixLength),
                        (length + TemplatePatterns_C_::VariableFulllength));
        }
    }

    QENTEM_NOINLINE void renderMath(const Char_T_ *content,
                                    SizeT          length) const {
        double number;

        if (ALE::Evaluate(number, content, length, this)) {
            Digit<Char_T_>::NumberToStringStream(*ss_, number, 1, 0, 3);
        } else {
            ss_->Insert((content - TemplatePatterns_C_::MathPrefixLength),
                        (length + TemplatePatterns_C_::MathFulllength));
        }
    }

    void parseVariables(const Char_T_ *content, SizeT length) const {
        static const Char_T_ inline_suffix_c =
            *(TemplatePatterns_C_::GetInLineSuffix());
        static const Char_T_ *variable_prefix =
            TemplatePatterns_C_::GetVariablePrefix();

        SizeT offset = 0;
        SizeT previous_offset;

        do {
            previous_offset = offset;
            offset          = Engine::Find(variable_prefix,
                                  TemplatePatterns_C_::VariablePrefixLength,
                                  content, offset, length);

            if (offset == 0) {
                break;
            }

            // Add any content that comes before any {var:x}
            ss_->Insert((content + previous_offset),
                        ((offset - TemplatePatterns_C_::VariablePrefixLength) -
                         previous_offset));

            const SizeT start_offset = offset;
            offset = Engine::FindOne(inline_suffix_c, content, offset, length);

            renderVariable((content + start_offset),
                           ((offset - 1) - start_offset));

            // -1 is - TemplatePatterns_C_::InLineSuffixLength
        } while (true);

        // Add any content that comes after }
        ss_->Insert((content + previous_offset), (length - previous_offset));
    }

    /*
     * Gets anything between "..."
     */

    SizeT getQuoted(const Char_T_ *content, SizeT &offset,
                    SizeT length) const noexcept {
        offset = Engine::FindOne(TemplatePatterns_C_::QuoteChar, content,
                                 offset, length);

        if (offset != 0) {
            const SizeT start_offset = Engine::FindOne(
                TemplatePatterns_C_::QuoteChar, content, offset, length);

            if (start_offset != 0) {
                return ((start_offset - 1) - offset);
            }
        }

        return 0;
    }

    QENTEM_NOINLINE void renderInLineIf(const Char_T_ *content,
                                        const SizeT    length) const {
        SizeT offset          = 0;
        SizeT previous_offset = 0;
        SizeT len             = 0;
        SizeT times           = 3;
        bool  case_value      = false;

        do {
            ++len;
            offset += len; // Move to the next Char_T_.

            len = getQuoted(content, offset, length);

            if (len == 0) {
                break;
            }

            // = + " + a Char_T_ == 3 + the Char_T_ before it == 4
            SizeT tmp_offset   = offset - 4;
            bool  iif_not_done = true;

            do {
                switch (content[tmp_offset]) {
                    case TemplatePatterns_C_::CaseChar: {
                        iif_not_done = false;
                        double number;

                        if (!(ALE::Evaluate(number, (content + offset), len,
                                            this))) {
                            // A messed-up case.
                            return;
                        }

                        case_value = (number > 0.0);
                        break;
                    }

                    case TemplatePatterns_C_::TrueChar: {
                        iif_not_done = false;

                        if (case_value) {
                            parseVariables((content + offset), len);
                            return;
                        }

                        break;
                    }

                    case TemplatePatterns_C_::FalseChar: {
                        iif_not_done = false;

                        if (!case_value) {
                            parseVariables((content + offset), len);
                            return;
                        }

                        break;
                    }
                }
            } while (iif_not_done && (--tmp_offset > previous_offset));

            previous_offset = offset;
        } while (--times != 0);
    }

    QENTEM_NOINLINE bool parseNumber(SizeT &number, const Char_T_ *content,
                                     const SizeT length) const noexcept {
        static const Char_T_ inline_suffix_c =
            *(TemplatePatterns_C_::GetInLineSuffix());
        static const Char_T_ *variable_prefix =
            TemplatePatterns_C_::GetVariablePrefix();

        if (length > TemplatePatterns_C_::VariableFulllength) {
            SizeT offset = 0;
            offset       = Engine::Find(variable_prefix,
                                  TemplatePatterns_C_::VariablePrefixLength,
                                  content, offset, length);

            if (offset != 0) {
                const SizeT end_offset =
                    Engine::FindOne(inline_suffix_c, content, offset, length);

                if (end_offset == 0) {
                    return false;
                }

                const Value_T_ *value =
                    findValue((content + offset), ((end_offset - 1) - offset));
                // -1 is - TemplatePatterns_C_::InLineSuffixLength

                return ((value != nullptr) && (value->SetNumber(number)));
            }
        }

        return Digit<Char_T_>::StringToNumber(number, content, length);
    }

    QENTEM_NOINLINE void generateLoopContent(const Char_T_ *content,
                                             SizeT          length,
                                             LoopData_ *    loop_data) const {
        static const Char_T_ inline_suffix_c =
            *(TemplatePatterns_C_::GetInLineSuffix());
        static const Char_T_ *variable_prefix =
            TemplatePatterns_C_::GetVariablePrefix();

        const SizeT start_offset = Engine::FindOne(
            TemplatePatterns_C_::MultiLineSuffix, content, SizeT(0), length);

        if (start_offset == 0) {
            // The syntax is wrong.
            return;
        }

        const Char_T_ *loop_value        = nullptr;
        SizeT          loop_value_length = 0;
        SizeT          len               = 0;
        SizeT          offset            = 0;
        SizeT          previous_offset   = 0;

        SizeT options    = 4; // set, value, times, index
        bool  break_loop = false;

        // Stage 1: Info extraction
        do {
            ++len;
            offset += len; // Move to the next Char_T_.

            len = getQuoted(content, offset, start_offset);

            if (len == 0) {
                // The syntax is wrong.
                break;
            }

            // X="|
            // 3: Goes back to X
            // |X="
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
                        loop_data->SetOffset =
                            static_cast<unsigned char>(offset);
                        loop_data->SetLength = static_cast<unsigned char>(len);

                        if ((content[offset] ==
                             TemplatePatterns_C_::InLinePrefix) &&
                            (len > TemplatePatterns_C_::VariableFulllength)) {
                            loop_data->SetOffset +=
                                TemplatePatterns_C_::VariablePrefixLength;
                            loop_data->SetLength -=
                                TemplatePatterns_C_::VariableFulllength;
                        }

                        break_loop = true;
                        break;
                    }

                    case TemplatePatterns_C_::IndexChar: {
                        loop_data->IndexOffset =
                            static_cast<unsigned char>(offset);
                        loop_data->IndexLength =
                            static_cast<unsigned char>(len);
                        break_loop = true;
                        break;
                    }

                    case TemplatePatterns_C_::RepeatChar: {
                        loop_data->RepeatOffset =
                            static_cast<unsigned char>(offset);
                        loop_data->RepeatLength =
                            static_cast<unsigned char>(len);
                        break_loop = true;
                        break;
                    }
                }

                if (break_loop) {
                    break;
                }

                --tmp_offset;
            } while (tmp_offset > previous_offset);

            break_loop      = false;
            previous_offset = offset;
        } while (--options != 0);

        // Stage 2
        offset          = start_offset;
        previous_offset = start_offset;

        if (loop_value != nullptr) {
            do {
                offset = Engine::Find(loop_value, loop_value_length, content,
                                      previous_offset, length);

                if (offset == 0) {
                    break;
                }

                loop_data->Content.Insert(
                    (content + previous_offset),
                    ((offset - loop_value_length) - previous_offset));

                loop_data->Content.Insert(
                    variable_prefix, TemplatePatterns_C_::VariablePrefixLength);

                SizeT lvl = 0;
                while (lvl <= level_) {
                    loop_data->Content += TemplatePatterns_C_::TildeChar;
                    ++lvl;
                }

                SizeT sub_offset = offset;

                while (content[sub_offset] ==
                       TemplatePatterns_C_::VariableIndexPrefix) {
                    while ((content[sub_offset] !=
                            TemplatePatterns_C_::VariableIndexSuffix) &&
                           (sub_offset < length)) {
                        ++sub_offset;
                    }

                    ++sub_offset;
                }

                if (sub_offset > length) {
                    // Unclosed bracket
                    break;
                }

                if ((content[(sub_offset - 1)] ==
                     TemplatePatterns_C_::VariableIndexSuffix)) {
                    loop_data->Content.Insert((content + offset),
                                              (sub_offset - offset));
                }

                previous_offset = sub_offset;
                loop_data->Content += inline_suffix_c;
            } while (true);
        }

        loop_data->Content.Insert((content + previous_offset),
                                  (length - previous_offset));

        renderLoop(content, loop_data);
    }

    QENTEM_NOINLINE void renderLoop(const Char_T_ *content,
                                    LoopData_ *    loop_data) const {
        // Stage 3: Data
        const Value_T_ *loop_set   = root_value_;
        SizeT           loop_index = 0;
        SizeT           loop_size  = 0;

        // Set (Array)
        if (loop_data->SetLength != 0) {
            loop_set = findValue((content + loop_data->SetOffset),
                                 loop_data->SetLength);

            if (loop_set == nullptr) {
                return; // set="..." has invalid value.
            }
        }

        // Size
        if ((loop_data->RepeatLength != 0) &&
            (!parseNumber(loop_size, (content + loop_data->RepeatOffset),
                          loop_data->RepeatLength))) {
            return; // Not a number
        }

        // Index
        if ((loop_data->IndexLength != 0) &&
            (!parseNumber(loop_index, (content + loop_data->IndexOffset),
                          loop_data->IndexLength))) {
            return; // Not a number
        }

        if (loop_size == 0) {
            loop_size = loop_set->Size();

            if (loop_size <= loop_index) {
                return;
            }

            loop_size -= loop_index;
        }

        // Stage 4: Render
        const Char_T_ *loop_content = loop_data->Content.First();
        const SizeT    loop_length  = loop_data->Content.Length();
        Template_CV    loop_template{ss_, loop_set, this, (level_ + 1)};

        do {
            loop_template.loop_value_ = loop_set->GetValue(loop_index);
            loop_template.process(loop_content, loop_length,
                                  loop_data->SubTags);
            --loop_size;

            if (loop_size == 0) {
                break;
            }

            ++loop_index;
        } while (true);
    }

    QENTEM_NOINLINE void generateIfCases(const Char_T_ *content, SizeT length,
                                         IfData_ *if_data) const {
        IfCase_ case_bit;
        case_bit.CaseOffset = 0;

        // The content without </if>
        const SizeT length2 = (length - TemplatePatterns_C_::IfSuffixLength);

        do {
            case_bit.CaseLength =
                getQuoted(content, case_bit.CaseOffset, length);

            if (case_bit.CaseLength == 0) {
                return;
            }

            case_bit.ContentOffset = Engine::FindOne(
                TemplatePatterns_C_::MultiLineSuffix, content,
                (case_bit.CaseOffset + case_bit.CaseLength + 1), length2);

            if (case_bit.ContentOffset == 0) {
                return;
            }

            SizeT else_offset =
                nextElse(content, case_bit.ContentOffset, length);

            if (else_offset == 0) {
                case_bit.ContentLength = (length2 - case_bit.ContentOffset);
                if_data->Cases += static_cast<IfCase_ &&>(case_bit);
                break;
            }

            case_bit.ContentLength =
                ((else_offset - TemplatePatterns_C_::ElsePrefixLength) -
                 case_bit.ContentOffset);
            if_data->Cases += static_cast<IfCase_ &&>(case_bit);

            if ((content[else_offset] != TemplatePatterns_C_::ElseIfChar)) {
                else_offset =
                    Engine::FindOne(TemplatePatterns_C_::MultiLineSuffix,
                                    content, else_offset, length2);

                if (else_offset == 0) {
                    return;
                }

                case_bit.CaseLength    = 0;
                case_bit.ContentOffset = else_offset;
                case_bit.ContentLength = (length2 - else_offset);

                if_data->Cases += static_cast<IfCase_ &&>(case_bit);
                break;
            }

            case_bit.CaseOffset = else_offset;
        } while (true);

        renderIf(content, if_data);
    }

    static SizeT nextElse(const Char_T_ *content, SizeT offset,
                          SizeT length) noexcept {
        static const Char_T_ *else_txt  = TemplatePatterns_C_::GetElsePrefix();
        static const Char_T_ *if_prefix = TemplatePatterns_C_::GetIfPrefix();
        static const Char_T_ *if_suffix = TemplatePatterns_C_::GetIfSuffix();

        SizeT else_offset = 0;

        do {
            else_offset =
                Engine::Find(else_txt, TemplatePatterns_C_::ElsePrefixLength,
                             content, offset, length);

            if (else_offset == 0) {
                // No <else.
                break;
            }

            const SizeT next_if =
                Engine::Find(if_prefix, TemplatePatterns_C_::IfPrefixLength,
                             content, offset, length);

            if ((next_if == 0) || (else_offset < next_if)) {
                // No nesting <ifs or <else is before the child <if.
                break;
            }

            offset =
                Engine::Find(if_suffix, TemplatePatterns_C_::IfSuffixLength,
                             content, next_if, length);

            if (else_offset > offset) {
                // <else came after the child if.
                break;
            }
        } while (true);

        return else_offset;
    }

    void renderIf(const Char_T_ *content, IfData_ *if_data) const {
        for (IfCase_ *item = if_data->Cases.Storage(),
                     *end  = (item + if_data->Cases.Size());
             item < end; item++) {
            double result;

            if ((item->CaseLength == 0) ||
                (ALE::Evaluate(result, (content + item->CaseOffset),
                               item->CaseLength, this) &&
                 (result > 0))) {
                process((content + item->ContentOffset), item->ContentLength,
                        item->SubTags);
                break;
            }
        }
    }

    QENTEM_NOINLINE const Value_T_ *findValue(const Char_T_ *key,
                                              SizeT length) const noexcept {
        const Value_T_ *value = nullptr;

        if (length != 0) {
            SizeT offset = 0;
            SizeT tmp    = 1;

            if (*key != TemplatePatterns_C_::TildeChar) {
                value = root_value_;

                if ((key[(length - 1)] !=
                     TemplatePatterns_C_::VariableIndexSuffix)) {
                    return value->GetValue(key, length);
                }

                // [...]
                while ((key[tmp] != TemplatePatterns_C_::VariableIndexPrefix) &&
                       (tmp < length)) {
                    ++tmp;
                }
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

                if ((key[(length - 1)] !=
                     TemplatePatterns_C_::VariableIndexSuffix)) {
                    return obj->loop_value_;
                }

                value  = obj->loop_value_;
                offset = lvl + 1;
                // [...]
                while (key[tmp] != TemplatePatterns_C_::VariableIndexSuffix) {
                    ++tmp;
                }
            }

            do {
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
            } while (true);
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

    bool ALESetNumber(double &number, const Char_T_ *content,
                      SizeT length) const noexcept {
        const Value_T_ *value = nullptr;

        if (length > TemplatePatterns_C_::VariableFulllength) {
            content += TemplatePatterns_C_::VariablePrefixLength;
            length -= TemplatePatterns_C_::VariableFulllength;
            value = findValue(content, length);
        }

        return ((value != nullptr) && ALESetNumber(number, value));
    }

    bool ALEIsEqual(bool &result, const Char_T_ *content, ALE::Number left,
                    ALE::Number right, bool left_evaluated,
                    bool right_evaluated) const noexcept {
        using ALEOperations_T_ = ALEOperations<Char_T_>;

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
                value_left = findValue(
                    (left_content + TemplatePatterns_C_::VariablePrefixLength),
                    (left_length -
                     TemplatePatterns_C_::VariableFulllength)); // {var:x}

                if (value_left == nullptr) {
                    return false;
                }

                if (value_left->IsNumber()) {
                    is_number = true;
                }
            } else {
                if (*left_content != ALEOperations_T_::ParenthesStart) {
                    left_evaluated = Digit<Char_T_>::StringToNumber(
                        left.Number, left_content, left_length);
                } else {
                    left_evaluated =
                        ALE::Evaluate(left.Number, (++left_content),
                                      (left_length -= 2), this);
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
                value_right = findValue(
                    (right_content + TemplatePatterns_C_::VariablePrefixLength),
                    (right_length -
                     TemplatePatterns_C_::VariableFulllength)); // {var:x}

                if (value_right == nullptr) {
                    return false;
                }

                if (value_right->IsNumber()) {
                    is_number = true;
                }
            } else if (is_number) {
                if (*right_content != ALEOperations_T_::ParenthesStart) {
                    if (!(Digit<Char_T_>::StringToNumber(
                            right.Number, right_content, right_length))) {
                        return false;
                    }
                } else if (!(ALE::Evaluate(right.Number, (++right_content),
                                           (right_length - 2), this))) {
                    return false;
                }
            }
        }

        if (is_number) {
            if (!left_evaluated && ((value_left == nullptr) ||
                                    !(ALESetNumber(left.Number, value_left)))) {
                return false;
            }

            if ((value_right != nullptr) &&
                !(ALESetNumber(right.Number, value_right))) {
                return false;
            }
        } else {
            if ((value_left != nullptr) &&
                !(value_left->SetCharAndLength(left_content, left_length))) {
                return false;
            }

            if ((value_right != nullptr) &&
                !(value_right->SetCharAndLength(right_content, right_length))) {
                return false;
            }
        }

        if (is_number) {
            result = (left.Number == right.Number);
        } else {
            result = ((left_length == right_length) &&
                      StringUtils::IsEqual(left_content, right_content,
                                           right_length));
        }

        return true;
    }

    Template_CV(StringStream<Char_T_> *ss, const Value_T_ *root_value,
                const Template_CV *parent = nullptr, SizeT level = 0) noexcept
        : ss_(ss), root_value_(root_value), parent_(parent), level_(level) {}

    StringStream<Char_T_> *ss_;
    const Value_T_ *       root_value_;
    const Value_T_ *       loop_value_{nullptr};
    const Template_CV *    parent_;

    const SizeT level_;
};

template <typename Char_T_>
struct TemplatePatterns {
    /*
     *InLineSuffixLength and InLinePrefixLength should not be more than 1
     */

    // static constexpr unsigned char InLineSuffixLength = 1U;
    // static constexpr unsigned char InLinePrefixLength = 1U;

    static constexpr unsigned char VariablePrefixLength = 5U;
    static constexpr unsigned char VariableFulllength =
        (VariablePrefixLength + 1U); // + InLineSuffixLength

    static constexpr unsigned char MathPrefixLength = 6U;
    static constexpr unsigned char MathFulllength =
        (MathPrefixLength + 1U); // + InLineSuffixLength

    static constexpr unsigned char InLineIfPrefixLength = 3U;

    static constexpr unsigned char LoopPrefixLength = 5U;
    static constexpr unsigned char LoopSuffixLength = 7U;

    static constexpr unsigned char IfPrefixLength = 3U;
    static constexpr unsigned char IfSuffixLength = 5U;

    static constexpr unsigned char ElsePrefixLength = 5U;
    static constexpr unsigned char ElseSuffixLength = 2U;

    static constexpr Char_T_ InLinePrefix = '{';
    static const Char_T_ *   GetInLineSuffix() noexcept {
        static constexpr Char_T_ val[] = {'}'};
        return &(val[0]);
    }

    static constexpr Char_T_ MultiLinePrefix = '<';
    static constexpr Char_T_ MultiLineSuffix = '>';

    static constexpr Char_T_ VariableIndexPrefix = '[';
    static constexpr Char_T_ VariableIndexSuffix = ']';

    // {var:
    static constexpr Char_T_ Var_2ND_Char = 'v'; // Second character
    static const Char_T_ *   GetVariablePrefix() noexcept {
        static constexpr Char_T_ val[] = {'{', 'v', 'a', 'r', ':'};
        return &(val[0]);
    }

    // {math:
    static constexpr Char_T_ Math_2ND_Char = 'm'; // Second character
    static const Char_T_ *   GetMathPrefix() noexcept {
        static constexpr Char_T_ val[] = {'{', 'm', 'a', 't', 'h', ':'};
        return &(val[0]);
    }

    // {if:
    static constexpr Char_T_ InlineIf_2ND_Char = 'i'; // Second character
    static const Char_T_ *   GetInLineIfPrefix() noexcept {
        static constexpr Char_T_ val[] = {'{', 'i', 'f', ':'};
        return &(val[0]);
    }

    // <loop
    static constexpr Char_T_ Loop_2ND_Char = 'l'; // Second character
    static const Char_T_ *   GetLoopPrefix() noexcept {
        static constexpr Char_T_ val[] = {'<', 'l', 'o', 'o', 'p'};
        return &(val[0]);
    }

    // </loop>
    static const Char_T_ *GetLoopSuffix() noexcept {
        static constexpr Char_T_ val[] = {'<', '/', 'l', 'o', 'o', 'p', '>'};
        return &(val[0]);
    }

    // <if
    static constexpr Char_T_ If_2ND_Char = 'i'; // Second character
    static const Char_T_ *   GetIfPrefix() noexcept {
        static constexpr Char_T_ val[] = {'<', 'i', 'f'};
        return &(val[0]);
    }

    // </if>
    static const Char_T_ *GetIfSuffix() noexcept {
        static constexpr Char_T_ val[] = {'<', '/', 'i', 'f', '>'};
        return &(val[0]);
    }

    // <else
    static constexpr Char_T_ ElseIfChar = 'i'; // else[i]f
    static const Char_T_ *   GetElsePrefix() noexcept {
        static constexpr Char_T_ val[] = {'<', 'e', 'l', 's', 'e'};
        return &(val[0]);
    }

    // />
    static const Char_T_ *GetElseSuffix() noexcept {
        static constexpr Char_T_ val[] = {'/', '>'};
        return &(val[0]);
    }

    static constexpr Char_T_ QuoteChar  = '"';
    static constexpr Char_T_ CaseChar   = 'a'; // c[a]se
    static constexpr Char_T_ TrueChar   = 'u'; // tr[u]e
    static constexpr Char_T_ FalseChar  = 'l'; // fa[l]se
    static constexpr Char_T_ SetChar    = 's'; // [s]et
    static constexpr Char_T_ ValueChar  = 'u'; // val[u]e
    static constexpr Char_T_ RepeatChar = 'p'; // re[p]eat
    static constexpr Char_T_ IndexChar  = 'd'; // in[d]ex
    static constexpr Char_T_ TildeChar  = '~'; // Tilde
};

} // namespace Qentem

#endif
