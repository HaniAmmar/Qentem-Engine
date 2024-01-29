
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

struct SuperVariableTag;
struct MathTag;
struct LoopTag;
struct InLineIfTag;
struct IfTag;
//////////////////////
enum struct TagType : SizeT8 {
    None = 0,
    Variable,      // {var:x}
    RawVariable,   // {raw:x}
    SuperVariable, // {svar:x1,x2,x3}
    Math,          // {math:x}
    Loop,          // <loop ...></loop>
    InLineIf,      // {if x}
    If             // <if case="..."></if>
};

template <bool>
struct TagBitData;

template <>
struct TagBitData<true> {
    // With tag
    TagBitData() noexcept  = default;
    ~TagBitData() noexcept = default;

    TagBitData(const TagBitData &)            = delete;
    TagBitData &operator=(TagBitData &&)      = delete;
    TagBitData &operator=(const TagBitData &) = delete;

    TagBitData(TagBitData &&src) noexcept : Storage{Memory::Move(src.Storage)} {
    }

    inline void SetType(TagType type) noexcept {
        Storage.SetHighByte(SizeT8(type));
    }

    inline TagType GetType() const noexcept {
        return TagType(Storage.GetHighByte());
    }

    inline void *GetStorage() const noexcept {
        return Storage.GetPointer();
    }

    QPointer<void> Storage{};
};

template <>
struct TagBitData<false> {
    // Without tag
    TagBitData() noexcept  = default;
    ~TagBitData() noexcept = default;

    TagBitData(const TagBitData &)            = delete;
    TagBitData &operator=(TagBitData &&)      = delete;
    TagBitData &operator=(const TagBitData &) = delete;

    TagBitData(TagBitData &&src) noexcept : Storage{Memory::Move(src.Storage)}, Type{src.Type} {
        src.Type = TagType::None;
    }

    inline void SetType(TagType type) noexcept {
        Type = type;
    }

    inline TagType GetType() const noexcept {
        return Type;
    }

    inline void *GetStorage() const noexcept {
        return Storage.GetPointer();
    }

    QPointer<void> Storage{};
    TagType        Type{TagType::None};
};

struct TagBit {
  public:
    TagBit()                          = delete;
    TagBit(const TagBit &)            = delete;
    TagBit &operator=(const TagBit &) = delete;
    TagBit &operator=(TagBit &&)      = delete;
    TagBit(TagBit &&tag) noexcept     = default;

    explicit TagBit(TagType type) {
        data_.SetType(type);

        switch (type) {
            case TagType::Variable:
            case TagType::RawVariable: {
                data_.Storage.SetPointer(Memory::AllocateInit<VariableTag>());
                break;
            }

            case TagType::SuperVariable: {
                data_.Storage.SetPointer(Memory::AllocateInit<SuperVariableTag>());
                break;
            }

            case TagType::Math: {
                data_.Storage.SetPointer(Memory::AllocateInit<MathTag>());
                break;
            }

            case TagType::Loop: {
                data_.Storage.SetPointer(Memory::AllocateInit<LoopTag>());
                break;
            }

            case TagType::InLineIf: {
                data_.Storage.SetPointer(Memory::AllocateInit<InLineIfTag>());
                break;
            }

            case TagType::If: {
                data_.Storage.SetPointer(Memory::AllocateInit<IfTag>());
                break;
            }

            default: {
            }
        }
    }

    ~TagBit() {
        switch (data_.GetType()) {
            case TagType::Variable:
            case TagType::RawVariable: {
                Memory::Deallocate(data_.GetStorage());
                break;
            }

            case TagType::SuperVariable: {
                SuperVariableTag *ptr = (SuperVariableTag *)(data_.GetStorage());
                Memory::Dispose(ptr);
                Memory::Deallocate(ptr);
                break;
            }

            case TagType::Math: {
                MathTag *ptr = (MathTag *)(data_.GetStorage());
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

            case TagType::InLineIf: {
                InLineIfTag *ptr = (InLineIfTag *)(data_.GetStorage());
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

    inline VariableTag &GeRawVariableTag() const noexcept {
        return *(Memory::ChangePointer<VariableTag>(data_.GetStorage()));
    }

    inline SuperVariableTag &GetSuperVariableTag() const noexcept {
        return *(Memory::ChangePointer<SuperVariableTag>(data_.GetStorage()));
    }

    inline MathTag &GetMathTag() const noexcept {
        return *(Memory::ChangePointer<MathTag>(data_.GetStorage()));
    }

    inline LoopTag &GetLoopTag() const noexcept {
        return *(Memory::ChangePointer<LoopTag>(data_.GetStorage()));
    }

    inline InLineIfTag &GetInLineIfTag() const noexcept {
        return *(Memory::ChangePointer<InLineIfTag>(data_.GetStorage()));
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

// LoopTagOptions -------------------------------------------
struct LoopTagOptions {
    static constexpr SizeT8 None{0};
    static constexpr SizeT8 SetIsLoopValue{1};
    static constexpr SizeT8 SortAscend{2};
    static constexpr SizeT8 SortDescend{4};
};

// LoopTagOptions -------------------------------------------
struct LoopTag {
    Array<TagBit> SubTags{};
    SizeT         Offset{0};
    SizeT         EndOffset{0};
    SizeT         ContentOffset{0};
    SizeT8        SetOffset{0};
    SizeT8        SetLength{0};
    SizeT8        SetLevel{0};
    SizeT8        ValueOffset{0};
    SizeT8        GroupOffset{0};
    SizeT8        ValueLength{0};
    SizeT8        GroupLength{0};
    SizeT8        Options{0};
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

// IfTag -------------------------------------------
struct IfTagCase {
    Array<QExpression> Case;
    Array<TagBit>      SubTags;
    SizeT              Offset;
    SizeT              EndOffset;
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

    static constexpr SizeT InLineSuffixLength = 1U;
    static constexpr SizeT InLinePrefixLength = 1U;

    static constexpr Char_T InLinePrefix = '{';
    static constexpr Char_T InLineSuffix = '}';

    static constexpr Char_T MultiLinePrefix = '<';
    static constexpr Char_T MultiLineSuffix = '>';

    static constexpr Char_T VariableIndexPrefix = '[';
    static constexpr Char_T VariableIndexSuffix = ']';

    // {var:
    static constexpr const Char_T *VariablePrefix = TPStrings::VariablePrefix;
    static constexpr Char_T        Var_2ND_Char   = VariablePrefix[1U]; // Second character
    // static constexpr SizeT          VariablePrefixLength = StringUtils::ConstCount(VariablePrefix);
    static constexpr SizeT VariablePrefixLength = 5U;
    static constexpr SizeT VariableFullLength   = (VariablePrefixLength + InLineSuffixLength);

    // {raw:
    static constexpr const Char_T *RawVariablePrefix = TPStrings::RawVariablePrefix;
    static constexpr Char_T        Raw_2ND_Char      = RawVariablePrefix[1U]; // Second character
    // static constexpr SizeT          RawVariablePrefixLength = StringUtils::ConstCount(RawVariablePrefix);
    static constexpr SizeT RawVariablePrefixLength = 5U;
    static constexpr SizeT RawVariableFullLength   = (RawVariablePrefixLength + InLineSuffixLength);

    // {math:
    static constexpr const Char_T *MathPrefix    = TPStrings::MathPrefix;
    static constexpr Char_T        Math_2ND_Char = MathPrefix[1U]; // Second character
    // static constexpr SizeT          MathPrefixLength = StringUtils::ConstCount(MathPrefix);
    static constexpr SizeT MathPrefixLength = 6U;

    // {svar:
    static constexpr const Char_T *SuperVariablePrefix    = TPStrings::SuperVariablePrefix;
    static constexpr Char_T        SuperVariable_2ND_Char = SuperVariablePrefix[1U]; // Second character
    // static constexpr SizeT          MathPrefixLength = StringUtils::ConstCount(MathPrefix);
    static constexpr SizeT SuperVariablePrefixLength = 6U;
    static constexpr SizeT SuperVariableFullLength   = (SuperVariablePrefixLength + InLineSuffixLength);

    // {if
    static constexpr const Char_T *InLineIfPrefix    = TPStrings::InLineIfPrefix;
    static constexpr Char_T        InlineIf_2ND_Char = InLineIfPrefix[1U]; // Second character
    // static constexpr SizeT          InLineIfPrefixLength = StringUtils::ConstCount(InLineIfPrefix);
    static constexpr SizeT InLineIfPrefixLength = 3U;

    // <loop
    static constexpr const Char_T *LoopPrefix    = TPStrings::LoopPrefix;
    static constexpr Char_T        Loop_2ND_Char = LoopPrefix[1U]; // Second character
    // static constexpr SizeT          LoopPrefixLength = StringUtils::ConstCount(LoopPrefix);
    static constexpr SizeT LoopPrefixLength = 5U;

    // </loop>
    static constexpr const Char_T *LoopSuffix = TPStrings::LoopSuffix;
    // static constexpr SizeT          LoopSuffixLength = StringUtils::ConstCount(LoopSuffix);
    static constexpr SizeT LoopSuffixLength = 7U;

    // <if
    static constexpr const Char_T *IfPrefix    = TPStrings::IfPrefix;
    static constexpr Char_T        If_2ND_Char = IfPrefix[1U]; // Second character
    // static constexpr SizeT          IfPrefixLength = StringUtils::ConstCount(IfPrefix);
    static constexpr SizeT IfPrefixLength = 3U;

    // </if>
    static constexpr const Char_T *IfSuffix = TPStrings::IfSuffix;
    // static constexpr SizeT          IfSuffixLength = StringUtils::ConstCount(IfSuffix);
    static constexpr SizeT IfSuffixLength = 5U;

    // <else
    static constexpr Char_T        ElseIfChar = 'i'; // else[i]f
    static constexpr const Char_T *ElsePrefix = TPStrings::ElsePrefix;
    // static constexpr SizeT          ElsePrefixLength = StringUtils::ConstCount(ElsePrefix);
    static constexpr SizeT ElsePrefixLength = 5U;

    // />
    static constexpr const Char_T *ElseSuffix = TPStrings::ElseSuffix;
    // static constexpr SizeT          ElseSuffixLength = StringUtils::ConstCount(ElseSuffix);
    static constexpr SizeT ElseSuffixLength = 2U;

    // &amp; &
    static constexpr const Char_T *HTMLAnd = TPStrings::HTMLAnd;
    // static constexpr SizeT          HTMLAndLength = StringUtils::ConstCount(HTMLAnd);
    static constexpr SizeT HTMLAndLength = 5U;

    // &lt; <
    static constexpr const Char_T *HTMLLess = TPStrings::HTMLLess;
    // static constexpr SizeT          HTMLLessLength = StringUtils::ConstCount(HTMLLess);
    static constexpr SizeT HTMLLessLength = 4U;

    // &gt; >
    static constexpr const Char_T *HTMLGreater = TPStrings::HTMLGreater;
    // static constexpr SizeT          HTMLGreaterLength = StringUtils::ConstCount(HTMLGreater);
    static constexpr SizeT HTMLGreaterLength = 4U;

    // &quot; "
    static constexpr const Char_T *HTMLQuote = TPStrings::HTMLQuote;
    // static constexpr SizeT          HTMLQuoteLength = StringUtils::ConstCount(HTMLQuote);
    static constexpr SizeT HTMLQuoteLength = 6U;

    // &apos; ' (HTML5)
    static constexpr const Char_T *HTMLSingleQuote = TPStrings::HTMLSingleQuote;
    // static constexpr SizeT          HTMLSingleQuoteLength = StringUtils::ConstCount(HTMLSingleQuote);
    static constexpr SizeT HTMLSingleQuoteLength = 6U;

    static constexpr Char_T QuoteChar = '"';

    static constexpr Char_T VariablesSeparatorChar = ',';

    // Inline If
    static constexpr Char_T CaseChar  = 'a'; // c[a]se
    static constexpr Char_T TrueChar  = 'r'; // t[r]ue
    static constexpr Char_T FalseChar = 'l'; // fa[l]se

    // Loop
    static constexpr Char_T SetChar   = 'e'; //  s[e]t
    static constexpr Char_T ValueChar = 'a'; // v[a]lue
    static constexpr Char_T SortChar  = 'o'; // s[o]rt
    static constexpr Char_T GroupChar = 'r'; // g[r]oup
};

// char
template <typename Char_T>
struct TPStrings_T<Char_T, 1U> {
    static constexpr const Char_T *VariablePrefix      = "{var:";
    static constexpr const Char_T *RawVariablePrefix   = "{raw:";
    static constexpr const Char_T *SuperVariablePrefix = "{svar:";
    static constexpr const Char_T *MathPrefix          = "{math:";
    static constexpr const Char_T *InLineIfPrefix      = "{if";
    static constexpr const Char_T *LoopPrefix          = "<loop";
    static constexpr const Char_T *LoopSuffix          = "</loop>";
    static constexpr const Char_T *IfPrefix            = "<if";
    static constexpr const Char_T *IfSuffix            = "</if>";
    static constexpr const Char_T *ElsePrefix          = "<else";
    static constexpr const Char_T *ElseSuffix          = "/>";
    static constexpr const Char_T *HTMLAnd             = "&amp;";
    static constexpr const Char_T *HTMLLess            = "&lt;";
    static constexpr const Char_T *HTMLGreater         = "&gt;";
    static constexpr const Char_T *HTMLQuote           = "&quot;";
    static constexpr const Char_T *HTMLSingleQuote     = "&apos;";
};

// char16_t
template <typename Char_T>
struct TPStrings_T<Char_T, 2U> {
    static constexpr const Char_T *VariablePrefix      = u"{var:";
    static constexpr const Char_T *RawVariablePrefix   = u"{raw:";
    static constexpr const Char_T *SuperVariablePrefix = u"{svar:";
    static constexpr const Char_T *MathPrefix          = u"{math:";
    static constexpr const Char_T *InLineIfPrefix      = u"{if";
    static constexpr const Char_T *LoopPrefix          = u"<loop";
    static constexpr const Char_T *LoopSuffix          = u"</loop>";
    static constexpr const Char_T *IfPrefix            = u"<if";
    static constexpr const Char_T *IfSuffix            = u"</if>";
    static constexpr const Char_T *ElsePrefix          = u"<else";
    static constexpr const Char_T *ElseSuffix          = u"/>";
    static constexpr const Char_T *HTMLAnd             = u"&amp;";
    static constexpr const Char_T *HTMLLess            = u"&lt;";
    static constexpr const Char_T *HTMLGreater         = u"&gt;";
    static constexpr const Char_T *HTMLQuote           = u"&quot;";
    static constexpr const Char_T *HTMLSingleQuote     = u"&apos;";
};

// char32_t
template <typename Char_T>
struct TPStrings_T<Char_T, 4U> {
    static constexpr const Char_T *VariablePrefix      = U"{var:";
    static constexpr const Char_T *RawVariablePrefix   = U"{raw:";
    static constexpr const Char_T *SuperVariablePrefix = U"{svar:";
    static constexpr const Char_T *MathPrefix          = U"{math:";
    static constexpr const Char_T *InLineIfPrefix      = U"{if";
    static constexpr const Char_T *LoopPrefix          = U"<loop";
    static constexpr const Char_T *LoopSuffix          = U"</loop>";
    static constexpr const Char_T *IfPrefix            = U"<if";
    static constexpr const Char_T *IfSuffix            = U"</if>";
    static constexpr const Char_T *ElsePrefix          = U"<else";
    static constexpr const Char_T *ElseSuffix          = U"/>";
    static constexpr const Char_T *HTMLAnd             = U"&amp;";
    static constexpr const Char_T *HTMLLess            = U"&lt;";
    static constexpr const Char_T *HTMLGreater         = U"&gt;";
    static constexpr const Char_T *HTMLQuote           = U"&quot;";
    static constexpr const Char_T *HTMLSingleQuote     = U"&apos;";
};

// wchar_t size = 4
template <>
struct TPStrings_T<wchar_t, 4U> {
    static constexpr const wchar_t *VariablePrefix      = L"{var:";
    static constexpr const wchar_t *RawVariablePrefix   = L"{raw:";
    static constexpr const wchar_t *SuperVariablePrefix = L"{svar:";
    static constexpr const wchar_t *MathPrefix          = L"{math:";
    static constexpr const wchar_t *InLineIfPrefix      = L"{if";
    static constexpr const wchar_t *LoopPrefix          = L"<loop";
    static constexpr const wchar_t *LoopSuffix          = L"</loop>";
    static constexpr const wchar_t *IfPrefix            = L"<if";
    static constexpr const wchar_t *IfSuffix            = L"</if>";
    static constexpr const wchar_t *ElsePrefix          = L"<else";
    static constexpr const wchar_t *ElseSuffix          = L"/>";
    static constexpr const wchar_t *HTMLAnd             = L"&amp;";
    static constexpr const wchar_t *HTMLLess            = L"&lt;";
    static constexpr const wchar_t *HTMLGreater         = L"&gt;";
    static constexpr const wchar_t *HTMLQuote           = L"&quot;";
    static constexpr const wchar_t *HTMLSingleQuote     = L"&apos;";
};

// wchar_t size = 2
template <>
struct TPStrings_T<wchar_t, 2U> {
    static constexpr const wchar_t *VariablePrefix      = L"{var:";
    static constexpr const wchar_t *RawVariablePrefix   = L"{raw:";
    static constexpr const wchar_t *SuperVariablePrefix = L"{svar:";
    static constexpr const wchar_t *MathPrefix          = L"{math:";
    static constexpr const wchar_t *InLineIfPrefix      = L"{if";
    static constexpr const wchar_t *LoopPrefix          = L"<loop";
    static constexpr const wchar_t *LoopSuffix          = L"</loop>";
    static constexpr const wchar_t *IfPrefix            = L"<if";
    static constexpr const wchar_t *IfSuffix            = L"</if>";
    static constexpr const wchar_t *ElsePrefix          = L"<else";
    static constexpr const wchar_t *ElseSuffix          = L"/>";
    static constexpr const wchar_t *HTMLAnd             = L"&amp;";
    static constexpr const wchar_t *HTMLLess            = L"&lt;";
    static constexpr const wchar_t *HTMLGreater         = L"&gt;";
    static constexpr const wchar_t *HTMLQuote           = L"&quot;";
    static constexpr const wchar_t *HTMLSingleQuote     = L"&apos;";
};

} // namespace Tags
} // namespace Qentem

#endif
