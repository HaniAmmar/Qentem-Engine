
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

#ifndef QENTEM_TAGS_H
#define QENTEM_TAGS_H

#include "QExpression.hpp"
#include "VariableTag.hpp"

namespace Qentem {
namespace Tags {

struct MathTag;
struct SuperVariableTag;
struct InLineIfTag;
struct LoopTag;
struct IfTag;
//////////////////////
enum struct TagType : SizeT8 {
    None = 0,
    Variable,      // {var:x}
    RawVariable,   // {raw:x}
    Math,          // {math:x}
    SuperVariable, // {svar:x1,x2,x3}
    InLineIf,      // {if x}
    Loop,          // <loop ...></loop>
    If             // <if case="..."></if>
};

template <bool>
struct TagBitData;

template <>
struct TagBitData<true> {
    // With pointer tagging
    TagBitData() noexcept  = default;
    ~TagBitData() noexcept = default;

    TagBitData(const TagBitData &)            = delete;
    TagBitData &operator=(TagBitData &&)      = delete;
    TagBitData &operator=(const TagBitData &) = delete;

    TagBitData(TagBitData &&src) noexcept : storage_{Memory::Move(src.storage_)} {
    }

    inline void SetType(TagType type) noexcept {
        storage_.SetHighByte(SizeT8(type));
    }

    inline TagType GetType() const noexcept {
        return TagType(storage_.GetHighByte());
    }

    inline void SetStorage(void *storage) noexcept {
        storage_.SetPointer(storage);
    }

    inline void *GetStorage() const noexcept {
        return storage_.GetPointer();
    }

  private:
    QPointer<void> storage_{};
};

template <>
struct TagBitData<false> {
    // Without tag
    TagBitData() noexcept  = default;
    ~TagBitData() noexcept = default;

    TagBitData(const TagBitData &)            = delete;
    TagBitData &operator=(TagBitData &&)      = delete;
    TagBitData &operator=(const TagBitData &) = delete;

    TagBitData(TagBitData &&src) noexcept : storage_{Memory::Move(src.storage_)}, type_{src.type_} {
        src.type_ = TagType::None;
    }

    inline void SetType(TagType type) noexcept {
        type_ = type;
    }

    inline TagType GetType() const noexcept {
        return type_;
    }

    inline void SetStorage(void *storage) noexcept {
        storage_.SetPointer(storage);
    }

    inline void *GetStorage() const noexcept {
        return storage_.GetPointer();
    }

  private:
    QPointer<void> storage_{};
    TagType        type_{TagType::None};
};

struct TagBit {
  public:
    TagBit()                          = default;
    TagBit(const TagBit &)            = delete;
    TagBit &operator=(const TagBit &) = delete;
    TagBit &operator=(TagBit &&)      = delete;
    TagBit(TagBit &&tag) noexcept     = default;

    VariableTag *MakeVariableTag() {
        VariableTag *tag = Memory::AllocateInit<VariableTag>();

        data_.SetType(TagType::Variable);
        data_.SetStorage(tag);

        return tag;
    }

    VariableTag *MakeRawVariableTag() {
        VariableTag *tag = Memory::AllocateInit<VariableTag>();

        data_.SetType(TagType::RawVariable);
        data_.SetStorage(tag);

        return tag;
    }

    MathTag *MakeMathTag() {
        MathTag *tag = Memory::AllocateInit<MathTag>();

        data_.SetType(TagType::Math);
        data_.SetStorage(tag);

        return tag;
    }

    SuperVariableTag *MakeSuperVariableTag() {
        SuperVariableTag *tag = Memory::AllocateInit<SuperVariableTag>();

        data_.SetType(TagType::SuperVariable);
        data_.SetStorage(tag);

        return tag;
    }

    InLineIfTag *MakeInLineIfTag() {
        InLineIfTag *tag = Memory::AllocateInit<InLineIfTag>();

        data_.SetType(TagType::InLineIf);
        data_.SetStorage(tag);

        return tag;
    }

    LoopTag *MakeLoopTag() {
        LoopTag *tag = Memory::AllocateInit<LoopTag>();

        data_.SetType(TagType::Loop);
        data_.SetStorage(tag);

        return tag;
    }

    IfTag *MakeIfTag() {
        IfTag *tag = Memory::AllocateInit<IfTag>();

        data_.SetType(TagType::If);
        data_.SetStorage(tag);

        return tag;
    }

    ~TagBit() {
        switch (data_.GetType()) {
            case TagType::Variable:
            case TagType::RawVariable: {
                Memory::Deallocate(data_.GetStorage());
                break;
            }

            case TagType::Math: {
                MathTag *ptr = (MathTag *)(data_.GetStorage());
                Memory::Dispose(ptr);
                Memory::Deallocate(ptr);
                break;
            }

            case TagType::SuperVariable: {
                SuperVariableTag *ptr = (SuperVariableTag *)(data_.GetStorage());
                Memory::Dispose(ptr);
                Memory::Deallocate(ptr);
                break;
            }

            case TagType::InLineIf: {
                InLineIfTag *ptr = (InLineIfTag *)(data_.GetStorage());
                Memory::Dispose(ptr);
                Memory::Deallocate(ptr);
                break;
            }

            case TagType::Loop: {
                LoopTag *ptr = (LoopTag *)(data_.GetStorage());
                Memory::Dispose(ptr);
                Memory::Deallocate(ptr);
                break;
            }

            case TagType::If: {
                IfTag *ptr = (IfTag *)(data_.GetStorage());
                Memory::Dispose(ptr);
                Memory::Deallocate(ptr);
                break;
            }

            default: {
            }
        }
    }

    inline VariableTag &GeVariableTag() const noexcept {
        return *(Memory::ChangePointer<VariableTag>(data_.GetStorage()));
    }

    inline MathTag &GetMathTag() const noexcept {
        return *(Memory::ChangePointer<MathTag>(data_.GetStorage()));
    }

    inline SuperVariableTag &GetSuperVariableTag() const noexcept {
        return *(Memory::ChangePointer<SuperVariableTag>(data_.GetStorage()));
    }

    inline InLineIfTag &GetInLineIfTag() const noexcept {
        return *(Memory::ChangePointer<InLineIfTag>(data_.GetStorage()));
    }

    inline LoopTag &GetLoopTag() const noexcept {
        return *(Memory::ChangePointer<LoopTag>(data_.GetStorage()));
    }

    inline IfTag &GetIfTag() const noexcept {
        return *(Memory::ChangePointer<IfTag>(data_.GetStorage()));
    }

    inline TagType GetType() const noexcept {
        return data_.GetType();
    }

  private:
    TagBitData<Config::PointerTagging> data_{};
};

// MathTag -------------------------------------------
struct MathTag {
    Array<QExpression> Expressions{};
    SizeT              Offset{0};
    SizeT              EndOffset{0};
};

// SuperVariableTag -------------------------------------------
struct SuperVariableTag {
    Array<TagBit> SubTags{};
    VariableTag   Variable{};
    SizeT         Offset{0};
    SizeT         EndOffset{0};
};

// InLineIfTag -------------------------------------------
struct InLineIfTag {
    Array<QExpression> Case{};
    Array<TagBit>      SubTags{};
    SizeT              Offset{0};
    SizeT16            Length{0};
    SizeT16            TrueOffset{0};
    SizeT16            TrueLength{0};
    SizeT16            TrueTagsSize{0};
    SizeT16            FalseOffset{0};
    SizeT16            FalseLength{0};
};

// LoopTagOptions -------------------------------------------
struct LoopTagOptions {
    static constexpr SizeT8 None{0};
    static constexpr SizeT8 SortAscend{2};
    static constexpr SizeT8 SortDescend{4};
};

// LoopTagOptions -------------------------------------------
struct LoopTag {
    Array<TagBit> SubTags{};
    VariableTag   Set{};

    const LoopTag *Parent{nullptr};

    SizeT Offset{0};
    SizeT EndOffset{0};

    SizeT16 ContentOffset{0};

    SizeT8 ValueOffset{0};
    SizeT8 ValueLength{0};

    SizeT8 GroupOffset{0};
    SizeT8 GroupLength{0};

    SizeT8 Options{0};
    SizeT8 Level{0};
};

// IfTag -------------------------------------------
struct IfTagCase {
    Array<QExpression> Case{};
    Array<TagBit>      SubTags{};
    SizeT              Offset{0};
    SizeT              EndOffset{0};
};

struct IfTag {
    Array<IfTagCase> Cases{};
    SizeT            Offset{0};
    SizeT            EndOffset{0};
};

template <typename, SizeT32>
struct TPStrings_T {};

template <typename Char_T>
struct TagPatterns_T {
  private:
    using TPStrings = TPStrings_T<Char_T, sizeof(Char_T)>;

  public:
    /*
     *InLineSuffixLength and InLinePrefixLength should not be more than 1
     */

    static constexpr SizeT InLinePrefixLength{1};
    static constexpr SizeT InLineSuffixLength{1};

    static constexpr Char_T InLinePrefix = '{';
    static constexpr Char_T InLineSuffix = '}';

    static constexpr Char_T MultiLinePrefix = '<';
    static constexpr Char_T MultiLineSuffix = '>';

    static constexpr SizeT MultiLineSuffixLength{1};

    static constexpr Char_T VariableIndexPrefix = '[';
    static constexpr Char_T VariableIndexSuffix = ']';

    // {var:
    static constexpr const Char_T *VariablePrefix = TPStrings::VariablePrefix;
    static constexpr Char_T        Var_2ND_Char   = VariablePrefix[1U]; // Second character
    // static constexpr SizeT          VariablePrefixLength = StringUtils::ConstCount(VariablePrefix);
    static constexpr SizeT VariablePrefixLength{5};
    static constexpr SizeT VariableFullLength = (VariablePrefixLength + InLineSuffixLength);

    // {raw:
    static constexpr const Char_T *RawVariablePrefix = TPStrings::RawVariablePrefix;
    static constexpr Char_T        Raw_2ND_Char      = RawVariablePrefix[1U]; // Second character
    // static constexpr SizeT          RawVariablePrefixLength = StringUtils::ConstCount(RawVariablePrefix);
    static constexpr SizeT RawVariablePrefixLength{5};
    static constexpr SizeT RawVariableFullLength = (RawVariablePrefixLength + InLineSuffixLength);

    // {math:
    static constexpr const Char_T *MathPrefix    = TPStrings::MathPrefix;
    static constexpr Char_T        Math_2ND_Char = MathPrefix[1U]; // Second character
    // static constexpr SizeT          MathPrefixLength = StringUtils::ConstCount(MathPrefix);
    static constexpr SizeT MathPrefixLength{6};

    // {svar:
    static constexpr const Char_T *SuperVariablePrefix    = TPStrings::SuperVariablePrefix;
    static constexpr Char_T        SuperVariable_2ND_Char = SuperVariablePrefix[1U]; // Second character
    // static constexpr SizeT          MathPrefixLength = StringUtils::ConstCount(MathPrefix);
    static constexpr SizeT SuperVariablePrefixLength{6};
    static constexpr SizeT SuperVariableFullLength = (SuperVariablePrefixLength + InLineSuffixLength);

    // {if
    static constexpr const Char_T *InLineIfPrefix    = TPStrings::InLineIfPrefix;
    static constexpr Char_T        InlineIf_2ND_Char = InLineIfPrefix[1U]; // Second character
    // static constexpr SizeT          InLineIfPrefixLength = StringUtils::ConstCount(InLineIfPrefix);
    static constexpr SizeT InLineIfPrefixLength{3};

    // <loop
    static constexpr const Char_T *LoopPrefix    = TPStrings::LoopPrefix;
    static constexpr Char_T        Loop_2ND_Char = LoopPrefix[1U]; // Second character
    // static constexpr SizeT          LoopPrefixLength = StringUtils::ConstCount(LoopPrefix);
    static constexpr SizeT LoopPrefixLength{5};

    // </loop>
    static constexpr const Char_T *LoopSuffix = TPStrings::LoopSuffix;
    // static constexpr SizeT          LoopSuffixLength = StringUtils::ConstCount(LoopSuffix);
    static constexpr SizeT LoopSuffixLength{7};

    // <if
    static constexpr const Char_T *IfPrefix    = TPStrings::IfPrefix;
    static constexpr Char_T        If_2ND_Char = IfPrefix[1U]; // Second character
    // static constexpr SizeT          IfPrefixLength = StringUtils::ConstCount(IfPrefix);
    static constexpr SizeT IfPrefixLength{3};

    static constexpr SizeT IfLength{2}; // else[if]

    // </if>
    static constexpr const Char_T *IfSuffix = TPStrings::IfSuffix;
    // static constexpr SizeT          IfSuffixLength = StringUtils::ConstCount(IfSuffix);
    static constexpr SizeT IfSuffixLength{5};

    // <else
    static constexpr Char_T        ElseIfChar = 'i'; // else[i]f
    static constexpr const Char_T *ElsePrefix = TPStrings::ElsePrefix;
    // static constexpr SizeT          ElsePrefixLength = StringUtils::ConstCount(ElsePrefix);
    static constexpr SizeT ElsePrefixLength{5};

    // &amp; &
    static constexpr const Char_T *HTMLAnd = TPStrings::HTMLAnd;
    // static constexpr SizeT          HTMLAndLength = StringUtils::ConstCount(HTMLAnd);
    static constexpr SizeT HTMLAndLength{5};

    // &lt; <
    static constexpr const Char_T *HTMLLess = TPStrings::HTMLLess;
    // static constexpr SizeT          HTMLLessLength = StringUtils::ConstCount(HTMLLess);
    static constexpr SizeT HTMLLessLength{4};

    // &gt; >
    static constexpr const Char_T *HTMLGreater = TPStrings::HTMLGreater;
    // static constexpr SizeT          HTMLGreaterLength = StringUtils::ConstCount(HTMLGreater);
    static constexpr SizeT HTMLGreaterLength{4};

    // &quot; "
    static constexpr const Char_T *HTMLQuote = TPStrings::HTMLQuote;
    // static constexpr SizeT          HTMLQuoteLength = StringUtils::ConstCount(HTMLQuote);
    static constexpr SizeT HTMLQuoteLength{6};

    // &apos; ' (HTML5)
    static constexpr const Char_T *HTMLSingleQuote = TPStrings::HTMLSingleQuote;
    // static constexpr SizeT          HTMLSingleQuoteLength = StringUtils::ConstCount(HTMLSingleQuote);
    static constexpr SizeT HTMLSingleQuoteLength{6};

    static constexpr Char_T DoubleQuoteChar        = '"';
    static constexpr Char_T SingleQuoteChar        = '\'';
    static constexpr Char_T EqualChar              = '=';
    static constexpr Char_T SpaceChar              = ' ';
    static constexpr Char_T VariablesSeparatorChar = ',';

    // Inline If attributes
    static constexpr Char_T CaseChar  = 'c';
    static constexpr Char_T TrueChar  = 't';
    static constexpr Char_T FalseChar = 'f';

    static constexpr const Char_T *Case = TPStrings::Case;
    static const SizeT             CaseLength{4};

    static constexpr const Char_T *True = TPStrings::True;
    static const SizeT             TrueLength{4};

    static constexpr const Char_T *False = TPStrings::False;
    static const SizeT             FalseLength{5};

    // Loop attributes
    static constexpr Char_T SetSortChar = 's';
    static constexpr Char_T ValueChar   = 'v';
    static constexpr Char_T GroupChar   = 'g';

    static constexpr const Char_T *Set = TPStrings::Set;
    static const SizeT             SetLength{3};

    static constexpr const Char_T *Value = TPStrings::Value;
    static const SizeT             ValueLength{5};

    static constexpr const Char_T *Group = TPStrings::Group;
    static const SizeT             GroupLength{5};

    static constexpr const Char_T *Sort = TPStrings::Sort;
    static const SizeT             SortLength{4};
};

// char
template <typename Char_T>
struct TPStrings_T<Char_T, 1U> {
    static constexpr const Char_T *VariablePrefix      = "{var:";
    static constexpr const Char_T *RawVariablePrefix   = "{raw:";
    static constexpr const Char_T *MathPrefix          = "{math:";
    static constexpr const Char_T *SuperVariablePrefix = "{svar:";
    static constexpr const Char_T *InLineIfPrefix      = "{if";
    static constexpr const Char_T *LoopPrefix          = "<loop";
    static constexpr const Char_T *LoopSuffix          = "</loop>";
    static constexpr const Char_T *IfPrefix            = "<if";
    static constexpr const Char_T *IfSuffix            = "</if>";
    static constexpr const Char_T *ElsePrefix          = "<else";

    // Inline If attributes
    static constexpr const Char_T *Case  = "case";
    static constexpr const Char_T *True  = "true";
    static constexpr const Char_T *False = "false";

    // Loop attributes
    static constexpr const Char_T *Set   = "set";
    static constexpr const Char_T *Value = "value";
    static constexpr const Char_T *Sort  = "sort";
    static constexpr const Char_T *Group = "group";

    static constexpr const Char_T *HTMLAnd         = "&amp;";
    static constexpr const Char_T *HTMLLess        = "&lt;";
    static constexpr const Char_T *HTMLGreater     = "&gt;";
    static constexpr const Char_T *HTMLQuote       = "&quot;";
    static constexpr const Char_T *HTMLSingleQuote = "&apos;";
};

// char16_t
template <typename Char_T>
struct TPStrings_T<Char_T, 2U> {
    static constexpr const Char_T *VariablePrefix      = u"{var:";
    static constexpr const Char_T *RawVariablePrefix   = u"{raw:";
    static constexpr const Char_T *MathPrefix          = u"{math:";
    static constexpr const Char_T *SuperVariablePrefix = u"{svar:";
    static constexpr const Char_T *InLineIfPrefix      = u"{if";
    static constexpr const Char_T *LoopPrefix          = u"<loop";
    static constexpr const Char_T *LoopSuffix          = u"</loop>";
    static constexpr const Char_T *IfPrefix            = u"<if";
    static constexpr const Char_T *IfSuffix            = u"</if>";
    static constexpr const Char_T *ElsePrefix          = u"<else";

    // Inline If attributes
    static constexpr const Char_T *Case  = u"case";
    static constexpr const Char_T *True  = u"true";
    static constexpr const Char_T *False = u"false";

    // Loop attributes
    static constexpr const Char_T *Set   = u"set";
    static constexpr const Char_T *Value = u"value";
    static constexpr const Char_T *Sort  = u"sort";
    static constexpr const Char_T *Group = u"group";

    static constexpr const Char_T *HTMLAnd         = u"&amp;";
    static constexpr const Char_T *HTMLLess        = u"&lt;";
    static constexpr const Char_T *HTMLGreater     = u"&gt;";
    static constexpr const Char_T *HTMLQuote       = u"&quot;";
    static constexpr const Char_T *HTMLSingleQuote = u"&apos;";
};

// char32_t
template <typename Char_T>
struct TPStrings_T<Char_T, 4U> {
    static constexpr const Char_T *VariablePrefix      = U"{var:";
    static constexpr const Char_T *RawVariablePrefix   = U"{raw:";
    static constexpr const Char_T *MathPrefix          = U"{math:";
    static constexpr const Char_T *SuperVariablePrefix = U"{svar:";
    static constexpr const Char_T *InLineIfPrefix      = U"{if";
    static constexpr const Char_T *LoopPrefix          = U"<loop";
    static constexpr const Char_T *LoopSuffix          = U"</loop>";
    static constexpr const Char_T *IfPrefix            = U"<if";
    static constexpr const Char_T *IfSuffix            = U"</if>";
    static constexpr const Char_T *ElsePrefix          = U"<else";

    // Inline If attributes
    static constexpr const Char_T *Case  = U"case";
    static constexpr const Char_T *True  = U"true";
    static constexpr const Char_T *False = U"false";

    // Loop attributes
    static constexpr const Char_T *Set   = U"set";
    static constexpr const Char_T *Value = U"value";
    static constexpr const Char_T *Sort  = U"sort";
    static constexpr const Char_T *Group = U"group";

    static constexpr const Char_T *HTMLAnd         = U"&amp;";
    static constexpr const Char_T *HTMLLess        = U"&lt;";
    static constexpr const Char_T *HTMLGreater     = U"&gt;";
    static constexpr const Char_T *HTMLQuote       = U"&quot;";
    static constexpr const Char_T *HTMLSingleQuote = U"&apos;";
};

// wchar_t size = 4
template <>
struct TPStrings_T<wchar_t, 4U> {
    static constexpr const wchar_t *VariablePrefix      = L"{var:";
    static constexpr const wchar_t *RawVariablePrefix   = L"{raw:";
    static constexpr const wchar_t *MathPrefix          = L"{math:";
    static constexpr const wchar_t *SuperVariablePrefix = L"{svar:";
    static constexpr const wchar_t *InLineIfPrefix      = L"{if";
    static constexpr const wchar_t *LoopPrefix          = L"<loop";
    static constexpr const wchar_t *LoopSuffix          = L"</loop>";
    static constexpr const wchar_t *IfPrefix            = L"<if";
    static constexpr const wchar_t *IfSuffix            = L"</if>";
    static constexpr const wchar_t *ElsePrefix          = L"<else";

    // Inline If attributes
    static constexpr const wchar_t *Case  = L"case";
    static constexpr const wchar_t *True  = L"true";
    static constexpr const wchar_t *False = L"false";

    // Loop attributes
    static constexpr const wchar_t *Set   = L"set";
    static constexpr const wchar_t *Value = L"value";
    static constexpr const wchar_t *Sort  = L"sort";
    static constexpr const wchar_t *Group = L"group";

    static constexpr const wchar_t *HTMLAnd         = L"&amp;";
    static constexpr const wchar_t *HTMLLess        = L"&lt;";
    static constexpr const wchar_t *HTMLGreater     = L"&gt;";
    static constexpr const wchar_t *HTMLQuote       = L"&quot;";
    static constexpr const wchar_t *HTMLSingleQuote = L"&apos;";
};

// wchar_t size = 2
template <>
struct TPStrings_T<wchar_t, 2U> {
    static constexpr const wchar_t *VariablePrefix      = L"{var:";
    static constexpr const wchar_t *RawVariablePrefix   = L"{raw:";
    static constexpr const wchar_t *MathPrefix          = L"{math:";
    static constexpr const wchar_t *SuperVariablePrefix = L"{svar:";
    static constexpr const wchar_t *InLineIfPrefix      = L"{if";
    static constexpr const wchar_t *LoopPrefix          = L"<loop";
    static constexpr const wchar_t *LoopSuffix          = L"</loop>";
    static constexpr const wchar_t *IfPrefix            = L"<if";
    static constexpr const wchar_t *IfSuffix            = L"</if>";
    static constexpr const wchar_t *ElsePrefix          = L"<else";

    // Inline If attributes
    static constexpr const wchar_t *Case  = L"case";
    static constexpr const wchar_t *True  = L"true";
    static constexpr const wchar_t *False = L"false";

    // Loop attributes
    static constexpr const wchar_t *Set   = L"set";
    static constexpr const wchar_t *Value = L"value";
    static constexpr const wchar_t *Sort  = L"sort";
    static constexpr const wchar_t *Group = L"group";

    static constexpr const wchar_t *HTMLAnd         = L"&amp;";
    static constexpr const wchar_t *HTMLLess        = L"&lt;";
    static constexpr const wchar_t *HTMLGreater     = L"&gt;";
    static constexpr const wchar_t *HTMLQuote       = L"&quot;";
    static constexpr const wchar_t *HTMLSingleQuote = L"&apos;";
};

} // namespace Tags
} // namespace Qentem

#endif
