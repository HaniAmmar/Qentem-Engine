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

// Use OVERRIDE_TEMPLATE_PATTERNS_ to use custom tags

#include <Common.hpp>

#ifndef QENTEM_TEMPLATE_PATTERNS_H_
#define QENTEM_TEMPLATE_PATTERNS_H_

namespace Qentem {

#ifndef OVERRIDE_TEMPLATE_PATTERNS_

template <typename Char_T_, ULong S = sizeof(Char_T_)>
struct TemplatePatterns {};

/*
 * Tags:
 *
 *  {...}
 *  - {raw:s|n}
 *      - raw: Raw variable, s: String, n: Number.
 *
 *  - {var:s|n}
 *      - var: Variable, s: String, n: Number.
 *
 *  - {math:raw|e|n}
 *      - raw|e|n: Raw variable, Equation or Number.
 *
 *  - {if case="raw|s" true="raw|var|s" false="raw|var|s"}
 *      - Inline if,  raw: Raw variable, var: Variable, s: String.
 *
 *  <...>
 *  - <loop set="s"? key="s"? value="s"? times="raw|n"? index="raw|n"?>
 *                                                          ...</loop>
 *      - s: String, n: Number, raw: Raw ariable,
 *      - set: child name in the passed colloction: Optional.
 *      - key: the string the will be replaced with keys: Optional.
 *      - value: the current value in the colloction: Optional.
 *          Note: Choose a unique names for key and value.
 *      - times: if set, it will be used instead of the "set" size.
 *      - index: starting index.
 *
 *  - <if case="raw|e|n">...<else(if)? ... >?...</if>
 *      - raw|e|n: Raw variable, Equation or Number.
 *      - <else if ....> same as if: Optional.
 *      - <else> no case: Optional.
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
 * {math: {raw:n1} * {raw:n2}}
 * {math: (5+3*(1+2)/2^2 == 7.25) || (3==((8-2)/2))}
 * {math: 0.2 + 0.3}
 * See ALE::Evaluate
 */

/*
 * Inline If Tag:
 *
 * {if case="3 == 3" true="Yes" false="No"}
 * {if case="{raw:rvar_five} == 5" true="5" false="no"}
 * {if case="{raw:rvar1}" true="{raw:rvar_five} is equal to 5" false="no"}
 * {if case="3 == 3" true="Yes" false="No"}
 * {if case="3 == 3" true="{raw:r1}" false="{raw:r2}"}
 * {if case="3 == 3" true="{var:v1}" false="{var:v2}"}
 */

/*
 * Loop Tag:
 *
 * <loop set="tree_name" key="loop1-key" value="loop1-value">
 *     loop1-key: loop1-value
 * </loop>
 *
 * <loop set="items" value="item">
 *     item[subitem1], item[subitem2], item[subitem3]
 * </loop>
 *
 * <loop set="tree_name" key="loop1-key" value="loop1-value">
 *      loop1-key: loop1-value
 *
 *      <loop set="tree_name2" key="loop2-key" value="loop2-value">
 *          loop2-key: loop2-value
 *      </loop>
 * </loop>
 *
 * <loop key="loop1-key" times="number|raw">
 *     loop1-key
 * </loop>
 *
 * <loop key="loop1-key" index="number|raw">
 *     loop1-key
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

// One byte character.
template <typename Char_T_>
struct TemplatePatterns<Char_T_, 1> {
    /*
     *InLineSuffixLength and InLinePrefixLength should not be more than 1
     */

    // static constexpr UInt InLineSuffixLength = 1U;
    // static constexpr UInt InLinePrefixLength = 1U;

    static constexpr UInt RawVariablePrefixLength = 5U;
    static constexpr UInt RawVariableFulllength =
        (RawVariablePrefixLength + 1U); // + InLineSuffixLength

    static constexpr UInt VariablePrefixLength = 5U;
    static constexpr UInt VariableFulllength =
        (VariablePrefixLength + 1U); // + InLineSuffixLength

    static constexpr UInt MathPrefixLength = 6U;
    static constexpr UInt MathFulllength =
        (MathPrefixLength + 1U); // + InLineSuffixLength

    static constexpr UInt InLineIfPrefixLength = 3U;

    static constexpr UInt LoopPrefixLength = 5U;
    static constexpr UInt LoopSuffixLength = 7U;

    static constexpr UInt IfPrefixLength = 3U;
    static constexpr UInt IfSuffixLength = 5U;

    static constexpr UInt ElsePrefixLength = 5U;
    static constexpr UInt ElseSuffixLength = 2U;

    static constexpr Char_T_ InLinePrefix = '{';
    static const Char_T_ *   GetInLineSuffix() noexcept {
        static constexpr Char_T_ val[] = {'}'};
        return &(val[0]);
    }

    static constexpr Char_T_ MultiLinePrefix = '<';
    static constexpr Char_T_ MultiLineSuffix = '>';

    static constexpr Char_T_ VariableIndexPrefix = '[';
    static constexpr Char_T_ VariableIndexSuffix = ']';

    // {raw:
    static const unsigned long long RawVariable64bit = 8249313016986432123ULL;
    static constexpr Char_T_        Raw_Var_2ND_Char = 'r'; // Second char
    static const Char_T_ *          GetRawVariablePrefix() noexcept {
        static constexpr Char_T_ val[] = {'{', 'r', 'a', 'w', ':'};
        return &(val[0]);
    }

    // {var:
    static const unsigned long long Variable64bit = 8537547791251764859ULL;
    static constexpr Char_T_        Var_2ND_Char  = 'v'; // Second char
    static const Char_T_ *          GetVariablePrefix() noexcept {
        static constexpr Char_T_ val[] = {'{', 'v', 'a', 'r', ':'};
        return &(val[0]);
    }

    // {math:
    static const unsigned long long Math64bit     = 7889019549154766203ULL;
    static constexpr Char_T_        MAth_2ND_Char = 'm'; // Second char
    static const Char_T_ *          GetMathPrefix() noexcept {
        static constexpr Char_T_ val[] = {'{', 'm', 'a', 't', 'h', ':'};
        return &(val[0]);
    }

    // {if:
    static const unsigned long long InLineIf64bit     = 7600784774889433467ULL;
    static constexpr Char_T_        InlineIf_2ND_Char = 'i'; // Second char
    static const Char_T_ *          GetInLineIfPrefix() noexcept {
        static constexpr Char_T_ val[] = {'{', 'i', 'f', ':'};
        return &(val[0]);
    }

    // <loop
    static const unsigned long long Loop64bit     = 7799227661468593212ULL;
    static constexpr Char_T_        Loop_2ND_Char = 'l'; // Second char
    static const Char_T_ *          GetLoopPrefix() noexcept {
        static constexpr Char_T_ val[] = {'<', 'l', 'o', 'o', 'p'};
        return &(val[0]);
    }

    // </loop>
    static const Char_T_ *GetLoopSuffix() noexcept {
        static constexpr Char_T_ val[] = {'<', '/', 'l', 'o', 'o', 'p', '>'};
        return &(val[0]);
    }

    // <if
    static const unsigned long long If64bit     = 7583051580769593660ULL;
    static constexpr Char_T_        If_2ND_Char = 'i'; // Second char
    static const Char_T_ *          GetIfPrefix() noexcept {
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

    static constexpr Char_T_ QuoteChar = '"';
    static constexpr Char_T_ CaseChar  = 'a'; // c[a]se
    static constexpr Char_T_ TrueChar  = 'u'; // tr[u]e
    static constexpr Char_T_ FalseChar = 'l'; // fa[l]se
    static constexpr Char_T_ SetChar   = 't'; // se[t]
    static constexpr Char_T_ KeyChar   = 'y'; // ke[y]
    static constexpr Char_T_ ValueChar = 'u'; // val[u]e
    static constexpr Char_T_ TimesChar = 'm'; // ti[m]es
    static constexpr Char_T_ IndexChar = 'x'; // inde[x]

    /*
        // To get a 64bit value:

        unsigned short var_16 = (static_cast<unsigned short>(
                                    TemplatePatterns<char>::GetVariablePrefix()[1])
                                << 8U);
        var_16 |= static_cast<unsigned short>(
            TemplatePatterns<char>::GetVariablePrefix()[0]);

        ULong var_64 = var_16;
        var_64 <<= 16U;
        var_64 |= var_16;
        var_64 <<= 16U;
        var_64 |= var_16;
        var_64 <<= 16U;
        var_64 |= var_16;
    */
};

// Two bytes character.
template <typename Char_T_>
struct TemplatePatterns<Char_T_, 2> {
    /*
     *InLineSuffixLength and InLinePrefixLength should not be more than 1
     */

    // static constexpr UInt InLineSuffixLength = 1U;
    // static constexpr UInt InLinePrefixLength = 1U;

    static constexpr UInt RawVariablePrefixLength = 5U;
    static constexpr UInt RawVariableFulllength =
        (RawVariablePrefixLength + 1U); // + InLineSuffixLength

    static constexpr UInt VariablePrefixLength = 5U;
    static constexpr UInt VariableFulllength =
        (VariablePrefixLength + 1U); // + InLineSuffixLength

    static constexpr UInt MathPrefixLength = 6U;
    static constexpr UInt MathFulllength =
        (MathPrefixLength + 1U); // + InLineSuffixLength

    static constexpr UInt InLineIfPrefixLength = 3U;

    static constexpr UInt LoopPrefixLength = 5U;
    static constexpr UInt LoopSuffixLength = 7U;

    static constexpr UInt IfPrefixLength = 3U;
    static constexpr UInt IfSuffixLength = 5U;

    static constexpr UInt ElsePrefixLength = 5U;
    static constexpr UInt ElseSuffixLength = 2U;

    static constexpr Char_T_ InLinePrefix = u'{';
    static const Char_T_ *   GetInLineSuffix() noexcept {
        static constexpr Char_T_ val[] = {u'}'};
        return &(val[0]);
    }

    static constexpr Char_T_ MultiLinePrefix = u'<';
    static constexpr Char_T_ MultiLineSuffix = u'>';

    static constexpr Char_T_ VariableIndexPrefix = u'[';
    static constexpr Char_T_ VariableIndexSuffix = u']';

    // {raw:
    static constexpr Char_T_ Raw_Var_2ND_Char = u'r'; // Second char
    static const Char_T_ *   GetRawVariablePrefix() noexcept {
        static constexpr Char_T_ val[] = {u'{', u'r', u'a', u'w', u':'};
        return &(val[0]);
    }

    // {var:
    static constexpr Char_T_ Var_2ND_Char = u'v'; // Second char
    static const Char_T_ *   GetVariablePrefix() noexcept {
        static constexpr Char_T_ val[] = {u'{', u'v', u'a', u'r', u':'};
        return &(val[0]);
    }

    // {math:
    static constexpr Char_T_ MAth_2ND_Char = u'm'; // Second char
    static const Char_T_ *   GetMathPrefix() noexcept {
        static constexpr Char_T_ val[] = {u'{', u'm', u'a', u't', u'h', u':'};
        return &(val[0]);
    }

    // {if:
    static constexpr Char_T_ InlineIf_2ND_Char = u'i'; // Second char
    static const Char_T_ *   GetInLineIfPrefix() noexcept {
        static constexpr Char_T_ val[] = {u'{', u'i', u'f', u':'};
        return &(val[0]);
    }

    // <loop
    static constexpr Char_T_ Loop_2ND_Char = u'l'; // Second char
    static const Char_T_ *   GetLoopPrefix() noexcept {
        static constexpr Char_T_ val[] = {u'<', u'l', u'o', u'o', u'p'};
        return &(val[0]);
    }

    // </loop>
    static const Char_T_ *GetLoopSuffix() noexcept {
        static constexpr Char_T_ val[] = {u'<', u'/', u'l', u'o',
                                          u'o', u'p', u'>'};
        return &(val[0]);
    }

    // <if
    static constexpr Char_T_ If_2ND_Char = u'i'; // Second char
    static const Char_T_ *   GetIfPrefix() noexcept {
        static constexpr Char_T_ val[] = {u'<', u'i', u'f'};
        return &(val[0]);
    }

    // </if>
    static const Char_T_ *GetIfSuffix() noexcept {
        static constexpr Char_T_ val[] = {u'<', u'/', u'i', u'f', u'>'};
        return &(val[0]);
    }

    // <else
    static constexpr Char_T_ ElseIfChar = u'i'; // else[i]f
    static const Char_T_ *   GetElsePrefix() noexcept {
        static constexpr Char_T_ val[] = {u'<', u'e', u'l', u's', u'e'};
        return &(val[0]);
    }

    // />
    static const Char_T_ *GetElseSuffix() noexcept {
        static constexpr Char_T_ val[] = {u'/', u'>'};
        return &(val[0]);
    }

    static constexpr Char_T_ QuoteChar = u'"';
    static constexpr Char_T_ CaseChar  = u'a'; // c[a]se
    static constexpr Char_T_ TrueChar  = u'u'; // tr[u]e
    static constexpr Char_T_ FalseChar = u'l'; // fa[l]se
    static constexpr Char_T_ SetChar   = u't'; // se[t]
    static constexpr Char_T_ KeyChar   = u'y'; // ke[y]
    static constexpr Char_T_ ValueChar = u'u'; // val[u]e
    static constexpr Char_T_ TimesChar = u'm'; // ti[m]es
    static constexpr Char_T_ IndexChar = u'x'; // inde[x]
};

// Four bytes character.
template <typename Char_T_>
struct TemplatePatterns<Char_T_, 4> {
    /*
     *InLineSuffixLength and InLinePrefixLength should not be more than 1
     */

    // static constexpr UInt InLineSuffixLength = 1U;
    // static constexpr UInt InLinePrefixLength = 1U;

    static constexpr UInt RawVariablePrefixLength = 5U;
    static constexpr UInt RawVariableFulllength =
        (RawVariablePrefixLength + 1U); // + InLineSuffixLength

    static constexpr UInt VariablePrefixLength = 5U;
    static constexpr UInt VariableFulllength =
        (VariablePrefixLength + 1U); // + InLineSuffixLength

    static constexpr UInt MathPrefixLength = 6U;
    static constexpr UInt MathFulllength =
        (MathPrefixLength + 1U); // + InLineSuffixLength

    static constexpr UInt InLineIfPrefixLength = 3U;

    static constexpr UInt LoopPrefixLength = 5U;
    static constexpr UInt LoopSuffixLength = 7U;

    static constexpr UInt IfPrefixLength = 3U;
    static constexpr UInt IfSuffixLength = 5U;

    static constexpr UInt ElsePrefixLength = 5U;
    static constexpr UInt ElseSuffixLength = 2U;

    static constexpr Char_T_ InLinePrefix = U'{';
    static const Char_T_ *   GetInLineSuffix() noexcept {
        static constexpr Char_T_ val[] = {U'}'};
        return &(val[0]);
    }

    static constexpr Char_T_ MultiLinePrefix = U'<';
    static constexpr Char_T_ MultiLineSuffix = U'>';

    static constexpr Char_T_ VariableIndexPrefix = U'[';
    static constexpr Char_T_ VariableIndexSuffix = U']';

    // {raw:
    static constexpr Char_T_ Raw_Var_2ND_Char = U'r'; // Second char
    static const Char_T_ *   GetRawVariablePrefix() noexcept {
        static constexpr Char_T_ val[] = {U'{', U'r', U'a', U'w', U':'};
        return &(val[0]);
    }

    // {var:
    static constexpr Char_T_ Var_2ND_Char = U'v'; // Second char
    static const Char_T_ *   GetVariablePrefix() noexcept {
        static constexpr Char_T_ val[] = {U'{', U'v', U'a', U'r', U':'};
        return &(val[0]);
    }

    // {math:
    static constexpr Char_T_ MAth_2ND_Char = U'm'; // Second char
    static const Char_T_ *   GetMathPrefix() noexcept {
        static constexpr Char_T_ val[] = {U'{', U'm', U'a', U't', U'h', U':'};
        return &(val[0]);
    }

    // {if:
    static constexpr Char_T_ InlineIf_2ND_Char = U'i'; // Second char
    static const Char_T_ *   GetInLineIfPrefix() noexcept {
        static constexpr Char_T_ val[] = {U'{', U'i', U'f', U':'};
        return &(val[0]);
    }

    // <loop
    static constexpr Char_T_ Loop_2ND_Char = U'l'; // Second char
    static const Char_T_ *   GetLoopPrefix() noexcept {
        static constexpr Char_T_ val[] = {U'<', U'l', U'o', U'o', U'p'};
        return &(val[0]);
    }

    // </loop>
    static const Char_T_ *GetLoopSuffix() noexcept {
        static constexpr Char_T_ val[] = {U'<', U'/', U'l', U'o',
                                          U'o', U'p', U'>'};
        return &(val[0]);
    }

    // <if
    static constexpr Char_T_ If_2ND_Char = U'i'; // Second char
    static const Char_T_ *   GetIfPrefix() noexcept {
        static constexpr Char_T_ val[] = {U'<', U'i', U'f'};
        return &(val[0]);
    }

    // </if>
    static const Char_T_ *GetIfSuffix() noexcept {
        static constexpr Char_T_ val[] = {U'<', U'/', U'i', U'f', U'>'};
        return &(val[0]);
    }

    // <else
    static constexpr Char_T_ ElseIfChar = U'i'; // else[i]f
    static const Char_T_ *   GetElsePrefix() noexcept {
        static constexpr Char_T_ val[] = {U'<', U'e', U'l', U's', U'e'};
        return &(val[0]);
    }

    // />
    static const Char_T_ *GetElseSuffix() noexcept {
        static constexpr Char_T_ val[] = {U'/', U'>'};
        return &(val[0]);
    }

    static constexpr Char_T_ QuoteChar = U'"';
    static constexpr Char_T_ CaseChar  = U'a'; // c[a]se
    static constexpr Char_T_ TrueChar  = U'u'; // tr[u]e
    static constexpr Char_T_ FalseChar = U'l'; // fa[l]se
    static constexpr Char_T_ SetChar   = U't'; // se[t]
    static constexpr Char_T_ KeyChar   = U'y'; // ke[y]
    static constexpr Char_T_ ValueChar = U'u'; // val[u]e
    static constexpr Char_T_ TimesChar = U'm'; // ti[m]es
    static constexpr Char_T_ IndexChar = U'x'; // inde[x]
};

// wchar_t
template <>
struct TemplatePatterns<wchar_t> {
    /*
     *InLineSuffixLength and InLinePrefixLength should not be more than 1
     */

    // static constexpr UInt InLineSuffixLength = 1U;
    // static constexpr UInt InLinePrefixLength = 1U;

    static constexpr UInt RawVariablePrefixLength = 5U;
    static constexpr UInt RawVariableFulllength =
        (RawVariablePrefixLength + 1U); // + InLineSuffixLength

    static constexpr UInt VariablePrefixLength = 5U;
    static constexpr UInt VariableFulllength =
        (VariablePrefixLength + 1U); // + InLineSuffixLength

    static constexpr UInt MathPrefixLength = 6U;
    static constexpr UInt MathFulllength =
        (MathPrefixLength + 1U); // + InLineSuffixLength

    static constexpr UInt InLineIfPrefixLength = 3U;

    static constexpr UInt LoopPrefixLength = 5U;
    static constexpr UInt LoopSuffixLength = 7U;

    static constexpr UInt IfPrefixLength = 3U;
    static constexpr UInt IfSuffixLength = 5U;

    static constexpr UInt ElsePrefixLength = 5U;
    static constexpr UInt ElseSuffixLength = 2U;

    static constexpr wchar_t InLinePrefix = L'{';
    static const wchar_t *   GetInLineSuffix() noexcept {
        static constexpr wchar_t val[] = {L'}'};
        return &(val[0]);
    }

    static constexpr wchar_t MultiLinePrefix = L'<';
    static constexpr wchar_t MultiLineSuffix = L'>';

    static constexpr wchar_t VariableIndexPrefix = L'[';
    static constexpr wchar_t VariableIndexSuffix = L']';

    // {raw:
    static constexpr wchar_t Raw_Var_2ND_Char = L'r'; // Second char
    static const wchar_t *   GetRawVariablePrefix() noexcept {
        static constexpr wchar_t val[] = {L'{', L'r', L'a', L'w', L':'};
        return &(val[0]);
    }

    // {var:
    static constexpr wchar_t Var_2ND_Char = L'v'; // Second char
    static const wchar_t *   GetVariablePrefix() noexcept {
        static constexpr wchar_t val[] = {L'{', L'v', L'a', L'r', L':'};
        return &(val[0]);
    }

    // {math:
    static constexpr wchar_t MAth_2ND_Char = L'm'; // Second char
    static const wchar_t *   GetMathPrefix() noexcept {
        static constexpr wchar_t val[] = {L'{', L'm', L'a', L't', L'h', L':'};
        return &(val[0]);
    }

    // {if:
    static constexpr wchar_t InlineIf_2ND_Char = L'i'; // Second char
    static const wchar_t *   GetInLineIfPrefix() noexcept {
        static constexpr wchar_t val[] = {L'{', L'i', L'f', L':'};
        return &(val[0]);
    }

    // <loop
    static constexpr wchar_t Loop_2ND_Char = L'l'; // Second char
    static const wchar_t *   GetLoopPrefix() noexcept {
        static constexpr wchar_t val[] = {L'<', L'l', L'o', L'o', L'p'};
        return &(val[0]);
    }

    // </loop>
    static const wchar_t *GetLoopSuffix() noexcept {
        static constexpr wchar_t val[] = {L'<', L'/', L'l', L'o',
                                          L'o', L'p', L'>'};
        return &(val[0]);
    }

    // <if
    static constexpr wchar_t If_2ND_Char = L'i'; // Second char
    static const wchar_t *   GetIfPrefix() noexcept {
        static constexpr wchar_t val[] = {L'<', L'i', L'f'};
        return &(val[0]);
    }

    // </if>
    static const wchar_t *GetIfSuffix() noexcept {
        static constexpr wchar_t val[] = {L'<', L'/', L'i', L'f', L'>'};
        return &(val[0]);
    }

    // <else
    static constexpr wchar_t ElseIfChar = L'i'; // else[i]f
    static const wchar_t *   GetElsePrefix() noexcept {
        static constexpr wchar_t val[] = {L'<', L'e', L'l', L's', L'e'};
        return &(val[0]);
    }

    // />
    static const wchar_t *GetElseSuffix() noexcept {
        static constexpr wchar_t val[] = {L'/', L'>'};
        return &(val[0]);
    }

    static constexpr wchar_t QuoteChar = L'"';
    static constexpr wchar_t CaseChar  = L'a'; // c[a]se
    static constexpr wchar_t TrueChar  = L'u'; // tr[u]e
    static constexpr wchar_t FalseChar = L'l'; // fa[l]se
    static constexpr wchar_t SetChar   = L't'; // se[t]
    static constexpr wchar_t KeyChar   = L'y'; // ke[y]
    static constexpr wchar_t ValueChar = L'u'; // val[u]e
    static constexpr wchar_t TimesChar = L'm'; // ti[m]es
    static constexpr wchar_t IndexChar = L'x'; // inde[x]
};

#endif

} // namespace Qentem

#endif
