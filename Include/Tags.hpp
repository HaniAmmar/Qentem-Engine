
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

#include "QExpression.hpp"
#include "VariableTag.hpp"

#ifndef _QENTEM_TAGS_H
#define _QENTEM_TAGS_H

namespace Qentem {
namespace Tags {
struct MathTag;
struct LoopTag;
struct InLineIfTag;
struct IfTagCase;
//////////////////////
enum class TagType : SizeT8 {
    None = 0,
    Variable,    // {var:x}
    RawVariable, // {raw:x}
    Math,        // {math:x}
    Loop,        // <loop ...></loop>
    InLineIf,    // {if x}
    If,          // <if case="..."></if>
    RawText
};

struct TagBit {
  public:
    TagBit()                          = delete;
    TagBit(const TagBit &)            = delete;
    TagBit &operator=(const TagBit &) = delete;
    TagBit &operator=(TagBit &&)      = delete;

    TagBit(TagBit &&tag) noexcept : _content{tag._content}, _type{tag._type} {
        constexpr bool is_smaller = sizeof(_Content_T) < sizeof(void *);

        if (is_smaller) {
            if ((_type != TagType::RawText)) {
                _info = tag._info;
            }
        }

        tag._type = TagType::None;
    }

    explicit TagBit(TagType type) : _type{type} {
        switch (type) {
            case TagType::Variable:
            case TagType::RawVariable: {
                _info = Memory::AllocateInit<VariableTag>();
                break;
            }

            case TagType::Math: {
                _info = Memory::AllocateInit<MathTag>();
                break;
            }

            case TagType::Loop: {
                _info = Memory::AllocateInit<LoopTag>();
                break;
            }

            case TagType::InLineIf: {
                _info = Memory::AllocateInit<InLineIfTag>();
                break;
            }

            case TagType::If: {
                _info = Memory::AllocateInit<Array<IfTagCase>>();
                break;
            }

            default: {
            }
        }
    }

    TagBit(SizeT offset, SizeT length) noexcept : _content{offset, length}, _type{TagType::RawText} {
    }

    ~TagBit() {
        switch (GetType()) {
            case TagType::Variable:
            case TagType::RawVariable: {
                Memory::Deallocate(getInfo());
                break;
            }

            case TagType::Math: {
                Memory::Dispose((MathTag *)(getInfo()));
                Memory::Deallocate(getInfo());
                break;
            }

            case TagType::Loop: {
                Memory::Dispose((LoopTag *)(getInfo()));
                Memory::Deallocate(getInfo());
                break;
            }

            case TagType::InLineIf: {
                Memory::Dispose((InLineIfTag *)(getInfo()));
                Memory::Deallocate(getInfo());
                break;
            }

            case TagType::If: {
                Memory::Dispose((Array<IfTagCase> *)(getInfo()));
                Memory::Deallocate(getInfo());
                break;
            }

            default: {
            }
        }
    }

    inline VariableTag &GeVariableTag() const noexcept {
        return *(Memory::ChangePointer<VariableTag>(getInfo()));
    }

    inline VariableTag &GeRawVariableTag() const noexcept {
        return *(Memory::ChangePointer<VariableTag>(getInfo()));
    }

    inline MathTag &GetMathTag() const noexcept {
        return *(Memory::ChangePointer<MathTag>(getInfo()));
    }

    inline LoopTag &GetLoopTag() const noexcept {
        return *(Memory::ChangePointer<LoopTag>(getInfo()));
    }

    inline InLineIfTag &GetInLineIfTag() const noexcept {
        return *(Memory::ChangePointer<InLineIfTag>(getInfo()));
    }

    inline Array<IfTagCase> &GetIfTag() const noexcept {
        return *(Memory::ChangePointer<Array<IfTagCase>>(getInfo()));
    }

    inline TagType GetType() const noexcept {
        return _type;
    }

    inline SizeT GetOffset() const noexcept {
        return _content.offset;
    }

    inline SizeT GetLength() const noexcept {
        return _content.length;
    }

  private:
    inline void *getInfo() const noexcept {
        return _info;
    }

    struct _Content_T {
        SizeT offset;
        SizeT length;
    };

    union {
        void      *_info;
        _Content_T _content{0, 0};
    };

    TagType _type{TagType::None};
};

// MathTag -------------------------------------------
struct MathTag {
    Array<QExpression> Expressions;
    SizeT              Offset;
    SizeT              EndOffset;
};

// LoopTagOptions -------------------------------------------
struct LoopTagOptions {
    static constexpr SizeT8 None           = 0;
    static constexpr SizeT8 SetIsLoopValue = 1;
    static constexpr SizeT8 SortAscend     = 2;
    static constexpr SizeT8 SortDescend    = 4;
};

// LoopTagOptions -------------------------------------------
struct LoopTag {
    Array<TagBit> SubTags;
    SizeT         Offset;
    SizeT8        SetOffset;
    SizeT8        SetLength;
    SizeT8        SetLevel;
    SizeT8        ValueOffset;
    SizeT8        GroupOffset;
    SizeT8        ValueLength;
    SizeT8        GroupLength;
    SizeT8        Options;
};

// InLineIfTag -------------------------------------------
struct InLineIfTag {
    Array<TagBit>      SubTags;
    Array<QExpression> Case;
};

// IfTag -------------------------------------------
struct IfTagCase {
    Array<TagBit>      SubTags;
    Array<QExpression> Case;
};

template <typename, SizeT32>
struct _TPStrings_T {};

template <typename _Char_T>
struct _TagPatterns_T {
  private:
    using TPStrings_T = _TPStrings_T<_Char_T, sizeof(_Char_T)>;

  public:
    /*
     *InLineSuffixLength and InLinePrefixLength should not be more than 1
     */

    static constexpr SizeT InLineSuffixLength = 1U;
    static constexpr SizeT InLinePrefixLength = 1U;

    static constexpr _Char_T InLinePrefix = '{';
    static constexpr _Char_T InLineSuffix = '}';

    static constexpr _Char_T MultiLinePrefix = '<';
    static constexpr _Char_T MultiLineSuffix = '>';

    static constexpr _Char_T VariableIndexPrefix = '[';
    static constexpr _Char_T VariableIndexSuffix = ']';

    // {var:
    static constexpr const _Char_T *VariablePrefix = TPStrings_T::VariablePrefix;
    static constexpr _Char_T        Var_2ND_Char   = VariablePrefix[1U]; // Second character
    // static constexpr SizeT          VariablePrefixLength = StringUtils::ConstCount(VariablePrefix);
    static constexpr SizeT VariablePrefixLength = 5U;
    static constexpr SizeT VariableFullLength   = (VariablePrefixLength + InLineSuffixLength);

    // {raw:
    static constexpr const _Char_T *RawVariablePrefix = TPStrings_T::RawVariablePrefix;
    static constexpr _Char_T        Raw_2ND_Char      = RawVariablePrefix[1U]; // Second character
    // static constexpr SizeT          RawVariablePrefixLength = StringUtils::ConstCount(RawVariablePrefix);
    static constexpr SizeT RawVariablePrefixLength = 5U;
    static constexpr SizeT RawVariableFullLength   = (RawVariablePrefixLength + InLineSuffixLength);

    // {math:
    static constexpr const _Char_T *MathPrefix    = TPStrings_T::MathPrefix;
    static constexpr _Char_T        Math_2ND_Char = MathPrefix[1U]; // Second character
    // static constexpr SizeT          MathPrefixLength = StringUtils::ConstCount(MathPrefix);
    static constexpr SizeT MathPrefixLength = 6U;
    static constexpr SizeT MathFullLength   = (MathPrefixLength + InLineSuffixLength);

    // {if
    static constexpr const _Char_T *InLineIfPrefix    = TPStrings_T::InLineIfPrefix;
    static constexpr _Char_T        InlineIf_2ND_Char = InLineIfPrefix[1U]; // Second character
    // static constexpr SizeT          InLineIfPrefixLength = StringUtils::ConstCount(InLineIfPrefix);
    static constexpr SizeT InLineIfPrefixLength = 3U;

    // <loop
    static constexpr const _Char_T *LoopPrefix    = TPStrings_T::LoopPrefix;
    static constexpr _Char_T        Loop_2ND_Char = LoopPrefix[1U]; // Second character
    // static constexpr SizeT          LoopPrefixLength = StringUtils::ConstCount(LoopPrefix);
    static constexpr SizeT LoopPrefixLength = 5U;

    // </loop>
    static constexpr const _Char_T *LoopSuffix = TPStrings_T::LoopSuffix;
    // static constexpr SizeT          LoopSuffixLength = StringUtils::ConstCount(LoopSuffix);
    static constexpr SizeT LoopSuffixLength = 7U;

    // <if
    static constexpr const _Char_T *IfPrefix    = TPStrings_T::IfPrefix;
    static constexpr _Char_T        If_2ND_Char = IfPrefix[1U]; // Second character
    // static constexpr SizeT          IfPrefixLength = StringUtils::ConstCount(IfPrefix);
    static constexpr SizeT IfPrefixLength = 3U;

    // </if>
    static constexpr const _Char_T *IfSuffix = TPStrings_T::IfSuffix;
    // static constexpr SizeT          IfSuffixLength = StringUtils::ConstCount(IfSuffix);
    static constexpr SizeT IfSuffixLength = 5U;

    // <else
    static constexpr _Char_T        ElseIfChar = 'i'; // else[i]f
    static constexpr const _Char_T *ElsePrefix = TPStrings_T::ElsePrefix;
    // static constexpr SizeT          ElsePrefixLength = StringUtils::ConstCount(ElsePrefix);
    static constexpr SizeT ElsePrefixLength = 5U;

    // />
    static constexpr const _Char_T *ElseSuffix = TPStrings_T::ElseSuffix;
    // static constexpr SizeT          ElseSuffixLength = StringUtils::ConstCount(ElseSuffix);
    static constexpr SizeT ElseSuffixLength = 2U;

    // &amp; &
    static constexpr const _Char_T *HTMLAnd = TPStrings_T::HTMLAnd;
    // static constexpr SizeT          HTMLAndLength = StringUtils::ConstCount(HTMLAnd);
    static constexpr SizeT HTMLAndLength = 5U;

    // &lt; <
    static constexpr const _Char_T *HTMLLess = TPStrings_T::HTMLLess;
    // static constexpr SizeT          HTMLLessLength = StringUtils::ConstCount(HTMLLess);
    static constexpr SizeT HTMLLessLength = 4U;

    // &gt; >
    static constexpr const _Char_T *HTMLGreater = TPStrings_T::HTMLGreater;
    // static constexpr SizeT          HTMLGreaterLength = StringUtils::ConstCount(HTMLGreater);
    static constexpr SizeT HTMLGreaterLength = 4U;

    // &quot; "
    static constexpr const _Char_T *HTMLQuote = TPStrings_T::HTMLQuote;
    // static constexpr SizeT          HTMLQuoteLength = StringUtils::ConstCount(HTMLQuote);
    static constexpr SizeT HTMLQuoteLength = 6U;

    // &apos; ' (HTML5)
    static constexpr const _Char_T *HTMLSingleQuote = TPStrings_T::HTMLSingleQuote;
    // static constexpr SizeT          HTMLSingleQuoteLength = StringUtils::ConstCount(HTMLSingleQuote);
    static constexpr SizeT HTMLSingleQuoteLength = 6U;

    static constexpr _Char_T QuoteChar = '"';

    // Inline If
    static constexpr _Char_T CaseChar  = 'a'; // c[a]se
    static constexpr _Char_T TrueChar  = 'r'; // t[r]ue
    static constexpr _Char_T FalseChar = 'l'; // fa[l]se

    // Loop
    static constexpr _Char_T SetChar   = 'e'; //  s[e]t
    static constexpr _Char_T ValueChar = 'a'; // v[a]lue
    static constexpr _Char_T SortChar  = 'o'; // s[o]rt
    static constexpr _Char_T GroupChar = 'r'; // g[r]oup
};

// char
template <typename _Char_T>
struct _TPStrings_T<_Char_T, 1U> {
    static constexpr const _Char_T *VariablePrefix    = "{var:";
    static constexpr const _Char_T *RawVariablePrefix = "{raw:";
    static constexpr const _Char_T *MathPrefix        = "{math:";
    static constexpr const _Char_T *InLineIfPrefix    = "{if";
    static constexpr const _Char_T *LoopPrefix        = "<loop";
    static constexpr const _Char_T *LoopSuffix        = "</loop>";
    static constexpr const _Char_T *IfPrefix          = "<if";
    static constexpr const _Char_T *IfSuffix          = "</if>";
    static constexpr const _Char_T *ElsePrefix        = "<else";
    static constexpr const _Char_T *ElseSuffix        = "/>";
    static constexpr const _Char_T *HTMLAnd           = "&amp;";
    static constexpr const _Char_T *HTMLLess          = "&lt;";
    static constexpr const _Char_T *HTMLGreater       = "&gt;";
    static constexpr const _Char_T *HTMLQuote         = "&quot;";
    static constexpr const _Char_T *HTMLSingleQuote   = "&apos;";
};

// char16_t
template <typename _Char_T>
struct _TPStrings_T<_Char_T, 2U> {
    static constexpr const _Char_T *VariablePrefix    = u"{var:";
    static constexpr const _Char_T *RawVariablePrefix = u"{raw:";
    static constexpr const _Char_T *MathPrefix        = u"{math:";
    static constexpr const _Char_T *InLineIfPrefix    = u"{if";
    static constexpr const _Char_T *LoopPrefix        = u"<loop";
    static constexpr const _Char_T *LoopSuffix        = u"</loop>";
    static constexpr const _Char_T *IfPrefix          = u"<if";
    static constexpr const _Char_T *IfSuffix          = u"</if>";
    static constexpr const _Char_T *ElsePrefix        = u"<else";
    static constexpr const _Char_T *ElseSuffix        = u"/>";
    static constexpr const _Char_T *HTMLAnd           = u"&amp;";
    static constexpr const _Char_T *HTMLLess          = u"&lt;";
    static constexpr const _Char_T *HTMLGreater       = u"&gt;";
    static constexpr const _Char_T *HTMLQuote         = u"&quot;";
    static constexpr const _Char_T *HTMLSingleQuote   = u"&apos;";
};

// char32_t
template <typename _Char_T>
struct _TPStrings_T<_Char_T, 4U> {
    static constexpr const _Char_T *VariablePrefix    = U"{var:";
    static constexpr const _Char_T *RawVariablePrefix = U"{raw:";
    static constexpr const _Char_T *MathPrefix        = U"{math:";
    static constexpr const _Char_T *InLineIfPrefix    = U"{if";
    static constexpr const _Char_T *LoopPrefix        = U"<loop";
    static constexpr const _Char_T *LoopSuffix        = U"</loop>";
    static constexpr const _Char_T *IfPrefix          = U"<if";
    static constexpr const _Char_T *IfSuffix          = U"</if>";
    static constexpr const _Char_T *ElsePrefix        = U"<else";
    static constexpr const _Char_T *ElseSuffix        = U"/>";
    static constexpr const _Char_T *HTMLAnd           = U"&amp;";
    static constexpr const _Char_T *HTMLLess          = U"&lt;";
    static constexpr const _Char_T *HTMLGreater       = U"&gt;";
    static constexpr const _Char_T *HTMLQuote         = U"&quot;";
    static constexpr const _Char_T *HTMLSingleQuote   = U"&apos;";
};

// wchar_t size = 4
template <>
struct _TPStrings_T<wchar_t, 4U> {
    static constexpr const wchar_t *VariablePrefix    = L"{var:";
    static constexpr const wchar_t *RawVariablePrefix = L"{raw:";
    static constexpr const wchar_t *MathPrefix        = L"{math:";
    static constexpr const wchar_t *InLineIfPrefix    = L"{if";
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
struct _TPStrings_T<wchar_t, 2U> {
    static constexpr const wchar_t *VariablePrefix    = L"{var:";
    static constexpr const wchar_t *RawVariablePrefix = L"{raw:";
    static constexpr const wchar_t *MathPrefix        = L"{math:";
    static constexpr const wchar_t *InLineIfPrefix    = L"{if";
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

} // namespace Tags
} // namespace Qentem

#endif
