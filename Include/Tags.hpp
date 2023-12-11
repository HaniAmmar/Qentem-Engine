
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

#ifndef QENTEM_TAGS_H_
#define QENTEM_TAGS_H_

namespace Qentem::Tags {
struct MathTag;
struct LoopTag;
struct InLineIfTag;
struct IfTagCase;
//////////////////////
enum class TagType : unsigned char {
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

    TagBit(TagBit &&tag) noexcept : content_{tag.content_}, type_{tag.type_} {
        // content_ should equal info_ or bigger,
        // using 'short' for SizeT will need to uncomment the next lines;

        // if (type_ != TagType::RawText) {
        //     info_ = tag.info_;
        // } else {
        //     content_ = tag.content_;
        // }

        tag.type_ = TagType::None;
    }

    explicit TagBit(TagType type) : type_{type} {
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

    TagBit(SizeT offset, SizeT length) noexcept : content_{offset, length}, type_{TagType::RawText} {
    }

    ~TagBit() {
        switch (type_) {
            case TagType::Variable:
            case TagType::RawVariable: {
                Memory::Deallocate(info_);
                break;
            }

            case TagType::Math: {
                Memory::Dispose((MathTag *)(info_));
                Memory::Deallocate(info_);
                break;
            }

            case TagType::Loop: {
                Memory::Dispose((LoopTag *)(info_));
                Memory::Deallocate(info_);
                break;
            }

            case TagType::InLineIf: {
                Memory::Dispose((InLineIfTag *)(info_));
                Memory::Deallocate(info_);
                break;
            }

            case TagType::If: {
                Memory::Dispose((Array<IfTagCase> *)(info_));
                Memory::Deallocate(info_);
                break;
            }

            default: {
            }
        }
    }

    inline void *GetInfo() const noexcept {
        return info_;
    }

    inline TagType GetType() const noexcept {
        return type_;
    }

    inline SizeT GetOffset() const noexcept {
        return content_.offset;
    }

    inline SizeT GetLength() const noexcept {
        return content_.length;
    }

  private:
    struct Content_ {
        SizeT offset;
        SizeT length;
    };

    union {
        void    *info_;
        Content_ content_{0, 0};
    };

    TagType type_{TagType::None};
};

// MathTag -------------------------------------------
struct MathTag {
    Array<QExpression> Expressions;
    SizeT              Offset;
    SizeT              EndOffset;
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
    Array<TagBit>      SubTags;
    Array<QExpression> Case;
};

// IfTag -------------------------------------------
struct IfTagCase {
    Array<TagBit>      SubTags;
    Array<QExpression> Case;
};

template <typename, SizeT32>
struct TPStrings {};

template <typename Char_T_>
struct TagPatterns_T_ {
  private:
    static constexpr SizeT32 size_ = sizeof(Char_T_);

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
    static constexpr const Char_T_ *VariablePrefix = TPStrings<Char_T_, size_>::VariablePrefix;
    static constexpr Char_T_        Var_2ND_Char   = VariablePrefix[1U]; // Second character
    // static constexpr SizeT          VariablePrefixLength = StringUtils::ConstCount(VariablePrefix);
    static constexpr SizeT VariablePrefixLength = 5U;
    static constexpr SizeT VariableFullLength   = (VariablePrefixLength + InLineSuffixLength);

    // {raw:
    static constexpr const Char_T_ *RawVariablePrefix = TPStrings<Char_T_, size_>::RawVariablePrefix;
    static constexpr Char_T_        Raw_2ND_Char      = RawVariablePrefix[1U]; // Second character
    // static constexpr SizeT          RawVariablePrefixLength = StringUtils::ConstCount(RawVariablePrefix);
    static constexpr SizeT RawVariablePrefixLength = 5U;
    static constexpr SizeT RawVariableFullLength   = (RawVariablePrefixLength + InLineSuffixLength);

    // {math:
    static constexpr const Char_T_ *MathPrefix    = TPStrings<Char_T_, size_>::MathPrefix;
    static constexpr Char_T_        Math_2ND_Char = MathPrefix[1U]; // Second character
    // static constexpr SizeT          MathPrefixLength = StringUtils::ConstCount(MathPrefix);
    static constexpr SizeT MathPrefixLength = 6U;
    static constexpr SizeT MathFullLength   = (MathPrefixLength + InLineSuffixLength);

    // {if
    static constexpr const Char_T_ *InLineIfPrefix    = TPStrings<Char_T_, size_>::InLineIfPrefix;
    static constexpr Char_T_        InlineIf_2ND_Char = InLineIfPrefix[1U]; // Second character
    // static constexpr SizeT          InLineIfPrefixLength = StringUtils::ConstCount(InLineIfPrefix);
    static constexpr SizeT InLineIfPrefixLength = 3U;

    // <loop
    static constexpr const Char_T_ *LoopPrefix    = TPStrings<Char_T_, size_>::LoopPrefix;
    static constexpr Char_T_        Loop_2ND_Char = LoopPrefix[1U]; // Second character
    // static constexpr SizeT          LoopPrefixLength = StringUtils::ConstCount(LoopPrefix);
    static constexpr SizeT LoopPrefixLength = 5U;

    // </loop>
    static constexpr const Char_T_ *LoopSuffix = TPStrings<Char_T_, size_>::LoopSuffix;
    // static constexpr SizeT          LoopSuffixLength = StringUtils::ConstCount(LoopSuffix);
    static constexpr SizeT LoopSuffixLength = 7U;

    // <if
    static constexpr const Char_T_ *IfPrefix    = TPStrings<Char_T_, size_>::IfPrefix;
    static constexpr Char_T_        If_2ND_Char = IfPrefix[1U]; // Second character
    // static constexpr SizeT          IfPrefixLength = StringUtils::ConstCount(IfPrefix);
    static constexpr SizeT IfPrefixLength = 3U;

    // </if>
    static constexpr const Char_T_ *IfSuffix = TPStrings<Char_T_, size_>::IfSuffix;
    // static constexpr SizeT          IfSuffixLength = StringUtils::ConstCount(IfSuffix);
    static constexpr SizeT IfSuffixLength = 5U;

    // <else
    static constexpr Char_T_        ElseIfChar = 'i'; // else[i]f
    static constexpr const Char_T_ *ElsePrefix = TPStrings<Char_T_, size_>::ElsePrefix;
    // static constexpr SizeT          ElsePrefixLength = StringUtils::ConstCount(ElsePrefix);
    static constexpr SizeT ElsePrefixLength = 5U;

    // />
    static constexpr const Char_T_ *ElseSuffix = TPStrings<Char_T_, size_>::ElseSuffix;
    // static constexpr SizeT          ElseSuffixLength = StringUtils::ConstCount(ElseSuffix);
    static constexpr SizeT ElseSuffixLength = 2U;

    // &amp; &
    static constexpr const Char_T_ *HTMLAnd = TPStrings<Char_T_, size_>::HTMLAnd;
    // static constexpr SizeT          HTMLAndLength = StringUtils::ConstCount(HTMLAnd);
    static constexpr SizeT HTMLAndLength = 5U;

    // &lt; <
    static constexpr const Char_T_ *HTMLLess = TPStrings<Char_T_, size_>::HTMLLess;
    // static constexpr SizeT          HTMLLessLength = StringUtils::ConstCount(HTMLLess);
    static constexpr SizeT HTMLLessLength = 4U;

    // &gt; >
    static constexpr const Char_T_ *HTMLGreater = TPStrings<Char_T_, size_>::HTMLGreater;
    // static constexpr SizeT          HTMLGreaterLength = StringUtils::ConstCount(HTMLGreater);
    static constexpr SizeT HTMLGreaterLength = 4U;

    // &quot; "
    static constexpr const Char_T_ *HTMLQuote = TPStrings<Char_T_, size_>::HTMLQuote;
    // static constexpr SizeT          HTMLQuoteLength = StringUtils::ConstCount(HTMLQuote);
    static constexpr SizeT HTMLQuoteLength = 6U;

    // &apos; ' (HTML5)
    static constexpr const Char_T_ *HTMLSingleQuote = TPStrings<Char_T_, size_>::HTMLSingleQuote;
    // static constexpr SizeT          HTMLSingleQuoteLength = StringUtils::ConstCount(HTMLSingleQuote);
    static constexpr SizeT HTMLSingleQuoteLength = 6U;

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
};

// char
template <typename Char_T_>
struct TPStrings<Char_T_, 1U> {
    static constexpr const Char_T_ *VariablePrefix    = "{var:";
    static constexpr const Char_T_ *RawVariablePrefix = "{raw:";
    static constexpr const Char_T_ *MathPrefix        = "{math:";
    static constexpr const Char_T_ *InLineIfPrefix    = "{if";
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
struct TPStrings<Char_T_, 2U> {
    static constexpr const Char_T_ *VariablePrefix    = u"{var:";
    static constexpr const Char_T_ *RawVariablePrefix = u"{raw:";
    static constexpr const Char_T_ *MathPrefix        = u"{math:";
    static constexpr const Char_T_ *InLineIfPrefix    = u"{if";
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
struct TPStrings<Char_T_, 4U> {
    static constexpr const Char_T_ *VariablePrefix    = U"{var:";
    static constexpr const Char_T_ *RawVariablePrefix = U"{raw:";
    static constexpr const Char_T_ *MathPrefix        = U"{math:";
    static constexpr const Char_T_ *InLineIfPrefix    = U"{if";
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
struct TPStrings<wchar_t, 4U> {
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
struct TPStrings<wchar_t, 2U> {
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

} // namespace Qentem::Tags
#endif
