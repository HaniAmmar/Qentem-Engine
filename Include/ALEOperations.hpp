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

#include <Common.hpp>

#ifndef QENTEM_ALE_OPERATIONS_H_
#define QENTEM_ALE_OPERATIONS_H_

namespace Qentem {

template <typename Char_T_, ULong S = sizeof(Char_T_)>
struct ALEOperations {};

// One byte character.
template <typename Char_T_>
struct ALEOperations<Char_T_, 1> {
    static constexpr Char_T_ BracketOp    = '{';
    static constexpr Char_T_ Bracket2Op   = '}';
    static constexpr Char_T_ ParenthesOp  = '(';
    static constexpr Char_T_ Parenthes2Op = ')';
    static constexpr Char_T_ ExponentOp   = '^';
    static constexpr Char_T_ RemainderOp  = '%';
    static constexpr Char_T_ MultipleOp   = '*';
    static constexpr Char_T_ DivideOp     = '/';
    static constexpr Char_T_ AddOp        = '+';
    static constexpr Char_T_ SubtractOp   = '-';
    static constexpr Char_T_ EqualOp      = '=';
    static constexpr Char_T_ NotOp        = '!';
    static constexpr Char_T_ LessOp       = '<';
    static constexpr Char_T_ BiggerOp     = '>';
    static constexpr Char_T_ AndOp        = '&';
    static constexpr Char_T_ OrOp         = '|';
    static constexpr Char_T_ SpaceChar    = ' ';
    static constexpr Char_T_ ColonChar    = ':';
    static constexpr Char_T_ SlashChar    = '/';
};

// Two bytes character.
template <typename Char_T_>
struct ALEOperations<Char_T_, 2> {
    static constexpr Char_T_ BracketOp    = u'{';
    static constexpr Char_T_ Bracket2Op   = u'}';
    static constexpr Char_T_ ParenthesOp  = u'(';
    static constexpr Char_T_ Parenthes2Op = u')';
    static constexpr Char_T_ ExponentOp   = u'^';
    static constexpr Char_T_ RemainderOp  = u'%';
    static constexpr Char_T_ MultipleOp   = u'*';
    static constexpr Char_T_ DivideOp     = u'/';
    static constexpr Char_T_ AddOp        = u'+';
    static constexpr Char_T_ SubtractOp   = u'-';
    static constexpr Char_T_ EqualOp      = u'=';
    static constexpr Char_T_ NotOp        = u'!';
    static constexpr Char_T_ LessOp       = u'<';
    static constexpr Char_T_ BiggerOp     = u'>';
    static constexpr Char_T_ AndOp        = u'&';
    static constexpr Char_T_ OrOp         = u'|';
    static constexpr Char_T_ SpaceChar    = u' ';
    static constexpr Char_T_ ColonChar    = u':';
    static constexpr Char_T_ SlashChar    = u'/';
};

// Four bytes character.
template <typename Char_T_>
struct ALEOperations<Char_T_, 4> {
    static constexpr Char_T_ BracketOp    = U'{';
    static constexpr Char_T_ Bracket2Op   = U'}';
    static constexpr Char_T_ ParenthesOp  = U'(';
    static constexpr Char_T_ Parenthes2Op = U')';
    static constexpr Char_T_ ExponentOp   = U'^';
    static constexpr Char_T_ RemainderOp  = U'%';
    static constexpr Char_T_ MultipleOp   = U'*';
    static constexpr Char_T_ DivideOp     = U'/';
    static constexpr Char_T_ AddOp        = U'+';
    static constexpr Char_T_ SubtractOp   = U'-';
    static constexpr Char_T_ EqualOp      = U'=';
    static constexpr Char_T_ NotOp        = U'!';
    static constexpr Char_T_ LessOp       = U'<';
    static constexpr Char_T_ BiggerOp     = U'>';
    static constexpr Char_T_ AndOp        = U'&';
    static constexpr Char_T_ OrOp         = U'|';
    static constexpr Char_T_ SpaceChar    = U' ';
    static constexpr Char_T_ ColonChar    = U':';
    static constexpr Char_T_ SlashChar    = U'/';
};

// wchar_t
template <>
struct ALEOperations<wchar_t> {
    static constexpr wchar_t BracketOp    = L'{';
    static constexpr wchar_t Bracket2Op   = L'}';
    static constexpr wchar_t ParenthesOp  = L'(';
    static constexpr wchar_t Parenthes2Op = L')';
    static constexpr wchar_t ExponentOp   = L'^';
    static constexpr wchar_t RemainderOp  = L'%';
    static constexpr wchar_t MultipleOp   = L'*';
    static constexpr wchar_t DivideOp     = L'/';
    static constexpr wchar_t AddOp        = L'+';
    static constexpr wchar_t SubtractOp   = L'-';
    static constexpr wchar_t EqualOp      = L'=';
    static constexpr wchar_t NotOp        = L'!';
    static constexpr wchar_t LessOp       = L'<';
    static constexpr wchar_t BiggerOp     = L'>';
    static constexpr wchar_t AndOp        = L'&';
    static constexpr wchar_t OrOp         = L'|';
    static constexpr wchar_t SpaceChar    = L' ';
    static constexpr wchar_t ColonChar    = L':';
    static constexpr wchar_t SlashChar    = L'/';
};

} // namespace Qentem

#endif
