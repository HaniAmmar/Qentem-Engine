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

// One byte character.
template <typename Char_T_>
struct TemplatePatterns<Char_T_, 1> {
    /*
     *InLineSuffixLength and InLinePrefixLength should not be more than 1
     */

    // static constexpr UInt InLineSuffixLength = 1U;
    // static constexpr UInt InLinePrefixLength = 1U;

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

    // {var:
    static constexpr Char_T_ Var_2ND_Char = 'v'; // Second char
    static const Char_T_ *   GetVariablePrefix() noexcept {
        static constexpr Char_T_ val[] = {'{', 'v', 'a', 'r', ':'};
        return &(val[0]);
    }

    // {math:
    static constexpr Char_T_ Math_2ND_Char = 'm'; // Second char
    static const Char_T_ *   GetMathPrefix() noexcept {
        static constexpr Char_T_ val[] = {'{', 'm', 'a', 't', 'h', ':'};
        return &(val[0]);
    }

    // {if:
    static constexpr Char_T_ InlineIf_2ND_Char = 'i'; // Second char
    static const Char_T_ *   GetInLineIfPrefix() noexcept {
        static constexpr Char_T_ val[] = {'{', 'i', 'f', ':'};
        return &(val[0]);
    }

    // <loop
    static constexpr Char_T_ Loop_2ND_Char = 'l'; // Second char
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
    static constexpr Char_T_ If_2ND_Char = 'i'; // Second char
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

// Two bytes character.
template <typename Char_T_>
struct TemplatePatterns<Char_T_, 2> {
    /*
     *InLineSuffixLength and InLinePrefixLength should not be more than 1
     */

    // static constexpr UInt InLineSuffixLength = 1U;
    // static constexpr UInt InLinePrefixLength = 1U;

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

    // {var:
    static constexpr Char_T_ Var_2ND_Char = u'v'; // Second char
    static const Char_T_ *   GetVariablePrefix() noexcept {
        static constexpr Char_T_ val[] = {u'{', u'v', u'a', u'r', u':'};
        return &(val[0]);
    }

    // {math:
    static constexpr Char_T_ Math_2ND_Char = u'm'; // Second char
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

    static constexpr Char_T_ QuoteChar  = u'"';
    static constexpr Char_T_ CaseChar   = u'a'; // c[a]se
    static constexpr Char_T_ TrueChar   = u'u'; // tr[u]e
    static constexpr Char_T_ FalseChar  = u'l'; // fa[l]se
    static constexpr Char_T_ SetChar    = u's'; // [s]et
    static constexpr Char_T_ ValueChar  = u'u'; // val[u]e
    static constexpr Char_T_ RepeatChar = u'p'; // re[p]eat
    static constexpr Char_T_ IndexChar  = u'd'; // in[d]ex
    static constexpr Char_T_ TildeChar  = u'~'; // Tilde
};

// Four bytes character.
template <typename Char_T_>
struct TemplatePatterns<Char_T_, 4> {
    /*
     *InLineSuffixLength and InLinePrefixLength should not be more than 1
     */

    // static constexpr UInt InLineSuffixLength = 1U;
    // static constexpr UInt InLinePrefixLength = 1U;

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

    // {var:
    static constexpr Char_T_ Var_2ND_Char = U'v'; // Second char
    static const Char_T_ *   GetVariablePrefix() noexcept {
        static constexpr Char_T_ val[] = {U'{', U'v', U'a', U'r', U':'};
        return &(val[0]);
    }

    // {math:
    static constexpr Char_T_ Math_2ND_Char = U'm'; // Second char
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

    static constexpr Char_T_ QuoteChar  = U'"';
    static constexpr Char_T_ CaseChar   = U'a'; // c[a]se
    static constexpr Char_T_ TrueChar   = U'u'; // tr[u]e
    static constexpr Char_T_ FalseChar  = U'l'; // fa[l]se
    static constexpr Char_T_ SetChar    = U's'; // [s]et
    static constexpr Char_T_ ValueChar  = U'u'; // val[u]e
    static constexpr Char_T_ RepeatChar = U'p'; // re[p]eat
    static constexpr Char_T_ IndexChar  = U'd'; // in[d]ex
    static constexpr Char_T_ TildeChar  = U'~'; // Tilde
};

// wchar_t
template <>
struct TemplatePatterns<wchar_t> {
    /*
     *InLineSuffixLength and InLinePrefixLength should not be more than 1
     */

    // static constexpr UInt InLineSuffixLength = 1U;
    // static constexpr UInt InLinePrefixLength = 1U;

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

    // {var:
    static constexpr wchar_t Var_2ND_Char = L'v'; // Second char
    static const wchar_t *   GetVariablePrefix() noexcept {
        static constexpr wchar_t val[] = {L'{', L'v', L'a', L'r', L':'};
        return &(val[0]);
    }

    // {math:
    static constexpr wchar_t Math_2ND_Char = L'm'; // Second char
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

    static constexpr wchar_t QuoteChar  = L'"';
    static constexpr wchar_t CaseChar   = L'a'; // c[a]se
    static constexpr wchar_t TrueChar   = L'u'; // tr[u]e
    static constexpr wchar_t FalseChar  = L'l'; // fa[l]se
    static constexpr wchar_t SetChar    = L's'; // [s]et
    static constexpr wchar_t ValueChar  = L'u'; // val[u]e
    static constexpr wchar_t RepeatChar = L'p'; // re[p]eat
    static constexpr wchar_t IndexChar  = L'd'; // in[d]ex
    static constexpr wchar_t TildeChar  = L'~'; // Tilde
};

#endif

} // namespace Qentem

#endif
