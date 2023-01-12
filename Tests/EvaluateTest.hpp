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

#include "TestHelper.hpp"

#include "Template.hpp"
#include "Value.hpp"

#ifndef QENTEM_Evaluate_TESTS_H_
#define QENTEM_Evaluate_TESTS_H_

namespace Qentem {
namespace Test {

static void TestEvaluate1(TestHelper &helper) {
    Value<char> value;
    double      number;
    const char *content;
    bool        is_valid;

    content  = "";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "  ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "               ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "()";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "( )";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(  )";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(        )";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " ()";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " ( )";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " (  )";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " (        )";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "() ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "( ) ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(  ) ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(        ) ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "  ()";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "  ( )";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "  (  )";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "  (        )";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "()  ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "( )  ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(  )  ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(        )  ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " () ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " ( ) ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " (  ) ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " (        ) ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "   ()   ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "   ( )   ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "   (  )   ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "   (        )   ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "a";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "10/0";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "10/(2-2)";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "fj";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "fgh";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "5f2s4s5dg";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(d)";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(oo)";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(www46fs5d4f5ds)";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "() ()";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(())";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "() (()) ()";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(()) (()()) ((()())())";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "3()";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "3(1)";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "3(1+1)";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);
}

static void TestEvaluate2(TestHelper &helper) {
    Value<char> value;
    double      number = -99;
    const char *content;
    bool        is_valid;

    content  = "1";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    // NOTE: Implement power of fraction.
    content  = "2^0.2";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);
    // helper.Equal(number, 1.148698355, "number", __LINE__);
    // helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 4";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 4, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "5 ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 5, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 10 ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 10, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 7 ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 7, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "100";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 100, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 40";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 40, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "5000 ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 5000, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 1004 ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1004, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 714 ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 714, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(1)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " (4)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 4, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(5) ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 5, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " (10) ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 10, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " (7) ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 7, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(100)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 100, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " (40)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 40, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(5000) ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 5000, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " (1004) ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1004, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " ( 714 ) ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 714, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "((1))";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " (( 4))";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 4, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "((5     ) )";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 5, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "( 10 )";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 10, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "( (((       7   ))) )";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 7, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(100)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 100, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "( 40)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 40, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(5000 )";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 5000, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(((((((((( 1004 ))))))))))";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1004, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "( 714 )";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 714, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "0^10";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestEvaluate3(TestHelper &helper) {
    Value<char> value;
    double      number;
    const char *content;
    bool        is_valid;

    content = "3^2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 9, "number", __LINE__);

    content = "7%5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 2, "number", __LINE__);

    content = "3*6";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 18, "number", __LINE__);

    content = "10/2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 5, "number", __LINE__);

    content = "3+5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8, "number", __LINE__);

    content = "5-2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 3, "number", __LINE__);

    content = "3>2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "2>2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 0, "number", __LINE__);

    content  = "5==2";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "5==5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "5!=2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "10!=10";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "1>5";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "5>=1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8>=8";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "5>=7";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "4<8";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "7<7";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "10<9";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "3<=16";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3<=3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "17<=3";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "1&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&800";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "0&&5";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "0&&0";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "8||6";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||9";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "0||0";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   3   ^2   ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 9, "number", __LINE__);

    content = "   7%   5   ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 2, "number", __LINE__);

    content = "   3   *   6";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 18, "number", __LINE__);

    content = "10   /   2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 5, "number", __LINE__);

    content = "3   +   5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8, "number", __LINE__);

    content  = " 5 == 2 ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " 5 == 5 ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 5 != 2 ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = " 10 != 10 ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " 5 - 2 ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 3, "number", __LINE__);

    content = "   3 > 2   ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   2   >2";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "1>   5   ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " 5   >=1   ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "   8>=   8 ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "5   >=   7";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   4   <8   ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   7<   7   ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "   10<   0";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   3   <=   16";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "   3   <=   3     ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   17   <=3";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   1   &&   1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "   1   &&   800    ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   0&&5  ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "   0   &&0";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   8   ||   6  ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4   ||   0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "   0||9";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   0   ||0";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestEvaluate4(TestHelper &helper) {
    Value<char> value;
    double      number;
    const char *content;
    bool        is_valid;

    content = "16 ^ -2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 0.00390625, "number", __LINE__);

    content = "8 ^-2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 0.015625, "number", __LINE__);

    content = "-3^0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "5564685^0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-3^2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 9, "number", __LINE__);

    content = "-8^-2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 0.015625, "number", __LINE__);

    content = "-4 * 5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -20, "number", __LINE__);

    content = "5 *-6";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -30, "number", __LINE__);

    content = "-3*-4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 12, "number", __LINE__);

    content = "-10 / 2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -5, "number", __LINE__);

    content = "7 /-2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -3.5, "number", __LINE__);

    content = "-9 / -3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 3, "number", __LINE__);

    content = "-7 + 5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -2, "number", __LINE__);

    content = "-11 + -2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -13, "number", __LINE__);

    content = "-8+-3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -11, "number", __LINE__);

    content = "5 --2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 7, "number", __LINE__);

    content = "-8-3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -11, "number", __LINE__);

    content = "5 ---2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 3, "number", __LINE__);

    content = "---5 ---2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -7, "number", __LINE__);

    content  = " 5 == - 5 ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 5 == -5 ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 5 ==-5 ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " -5 == 5 ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " - 5 == 5 ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " -5 != 2 ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 10 != -10 ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = " -10 != 10 ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = " -  10 != 10 ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 10 !=-10 ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 10 !=- 10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3 >-2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10 > -10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "-2 > 2";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "-2 > -11";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "-1>5";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "5 >=-19";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1 >=- 100";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-8 >= -   8";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "-5 >= 7";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "3 <-2";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "10 < -10";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "-2 < 2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "-2 < -11";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "-1<5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "5 <=-19";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "5 - <=-19";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "-5 <=- 19";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "-8 <= -   8";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-5 <= 7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "-1 && 1";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "1 &&- 800";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "0 && -5";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "-8 && -9";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "-8 || 6";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4 ||-9";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "0 || - 9";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "-7 || -500";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestEvaluate5(TestHelper &helper) {
    Value<char> value;
    double      number;
    const char *content;
    bool        is_valid;

    content = "16 ^ +2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 256, "number", __LINE__);

    content = "8 ^+2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 64, "number", __LINE__);

    content = "+3^0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "5564685^0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "+3^2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 9, "number", __LINE__);

    content = "+3^+3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 27, "number", __LINE__);

    content = "+4 * 5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 20, "number", __LINE__);

    content = "5 *+6";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 30, "number", __LINE__);

    content = "+3*+4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 12, "number", __LINE__);

    content = "+10 / 2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 5, "number", __LINE__);

    content = "7 /+2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 3.5, "number", __LINE__);

    content = "+9 / +3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 3, "number", __LINE__);

    content = "+7 - 5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 2, "number", __LINE__);

    content = "+11 - +2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 9, "number", __LINE__);

    content = "+8-+3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 5, "number", __LINE__);

    content = "5 ++2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 7, "number", __LINE__);

    content = "+8+3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, +11, "number", __LINE__);

    content = "5 +++2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 7, "number", __LINE__);

    content = "+++5 +++2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 7, "number", __LINE__);

    content  = "+5==2";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "5==+2";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "+5==+2";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+5==+5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "+5!=+2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "+10!=+10";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "+10!=10";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "10!=+10";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "3 >+2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "10 > +10";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "+2 > 2";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+11 > +2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "+1>5";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+19 >=5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "+1 >=+ 100";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+8 >= +   8";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "7+ >= 5";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "3 <+2";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "10 < +10";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+1 < 2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "+2 < +11";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "+1<5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "+19 <=5";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "+5 <=+ 19";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+8 <= +   8";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "+5 <= 7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "+1 && 1";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "1 &&+ 800";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "0 && +5";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "+8 && +9";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+8 || 6";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4 ||+9";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content =
        R"(4                                                                      ||1
                                                                                       ||
                                                                                             2)";
    number = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "0 || + 9";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "+7 || +500";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestEvaluate6(TestHelper &helper) {
    Value<char> value;
    double      number = -1;
    const char *content;
    bool        is_valid;

    content  = "(0)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(+1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(-1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -1, "number", __LINE__);

    content = "( 1 )";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "( +1 )";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "( -1 )";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -1, "number", __LINE__);

    content = "(1 )";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(+1 )";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(-1 )";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -1, "number", __LINE__);

    content = "( 1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "( +1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "( -1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -1, "number", __LINE__);

    content = "(       1            )";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(       +      1             )";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(        -       1           )";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -1, "number", __LINE__);

    content = "(2*3)^4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1296, "number", __LINE__);

    content = "(8*3) %7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 3, "number", __LINE__);

    content = "(6+2)  *6";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 48, "number", __LINE__);

    content = " (15-5)  /5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 2, "number", __LINE__);

    content = "   (4*3)   +  5  ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 17, "number", __LINE__);

    content = "(18/3)- 2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 4, "number", __LINE__);

    content = "(13%5)           >2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(3                  ^-2)>2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 0, "number", __LINE__);

    content  = "(1+2)== 2";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(6-1) ==5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = " (3^2)!=2 ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(20/2) != 10";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(  10  -  +  9  )>5";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(15%10)>=1             ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "          (16/2)>=8";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(4*1.5)>=7";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(0.999999+0.1)<8";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(20*0.5)<7";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(4^2)<9";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(9/2)<=16";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1.5*2)<=3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(20-8)<=3";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(1*1)&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&1)&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(100-5)&&800";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(0*10)&&5";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(8%2)&&0";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(4/2)||6";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||0)||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(9%3)||9";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(0||0)||0";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestEvaluate7(TestHelper &helper) {
    Value<char> value;
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "6^(8/2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1296, "number", __LINE__);

    content = "24 %(3.5*2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 3, "number", __LINE__);

    content = "8  *(12/2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 48, "number", __LINE__);

    content = " 10  /(7-2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 2, "number", __LINE__);

    content = "   12   +  (2.5*2)  ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 17, "number", __LINE__);

    content = "6- (10/5)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 4, "number", __LINE__);

    content = "(3)           >(2*1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0.9999999999>(4-2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 0, "number", __LINE__);

    content  = "3== (3-1)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(5) ==(10-5)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 9!=(1+1) ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "10 != (20/2)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "1>(3+2)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "5>=(100^0)             ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 8>=(2^3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "6>=(14/2)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "1<(4+2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "10<(10-3)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "16<(18/2)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "4.5<=(4*4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3<=(1.5+1.5)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "12<=(3*1)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "1&&(1^10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10&&(10^1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7&&(10-5)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "0&&(2+2)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "0&&(0^0)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "9||        (3*2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "        1||(0*10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0          ||(3*3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "        0||(10&&0)          ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(2*3)^(8/2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1296, "number", __LINE__);

    content = "(8*3) %(3.5*2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 3, "number", __LINE__);

    content = "(6+2)  *(12/2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 48, "number", __LINE__);

    content = " (15-5)  /(7-2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 2, "number", __LINE__);

    content = "   (4*3)   +  (2.5*2)  ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 17, "number", __LINE__);

    content = "(18/3)- (10/5)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 4, "number", __LINE__);

    content = "(13%5)           >(2*1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(3                  ^-2)>(4-2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 0, "number", __LINE__);

    content  = "(1+2)== (3-1)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(6-1) ==(10-5)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = " (3^2)!=(1+1) ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(20/2) != (20/2)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(  10  -  +  9  )>(3+2)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(15%10)>=(100^0)             ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "          (16/2)>=(2^3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(4*1.5)>=(14/2)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(0.999999+0.1)<(4+2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(20*0.5)<(10-3)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(4^2)<(18/2)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(9/2)<=(4*4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1.5*2)<=(1.5+1.5)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(20-8)<=(3*1)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(1*1)&&(1^10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&1)&&(10^1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(100-5)&&(10-5)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(0*10)&&(2+2)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(8%2)&&(0^0)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(4/2)||(3*2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||0)||(0*10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(9%3)||(3*3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(0||0)||(10&&0)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestEvaluate8(TestHelper &helper) {
    Value<char> value;
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "3*2^3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 24, "number", __LINE__);

    content = "3*2^2+1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 13, "number", __LINE__);

    content = "3*10%7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 9, "number", __LINE__);

    content = "135/3^3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 5, "number", __LINE__);

    content = "18/10%7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 6, "number", __LINE__);

    content = "4^2*2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 32, "number", __LINE__);

    content = "10%7*2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 6, "number", __LINE__);

    content = "3^3/4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 6.75, "number", __LINE__);

    content = "10%4/2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4+2*2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8, "number", __LINE__);

    content = "10-7*2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -4, "number", __LINE__);

    content = "3+16/4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 7, "number", __LINE__);

    content = "10-6/3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8, "number", __LINE__);

    content = "2*8-2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 14, "number", __LINE__);

    content = "3*4-20";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -8, "number", __LINE__);

    content = "14/7+3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 5, "number", __LINE__);

    content = "9/3-4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -1, "number", __LINE__);

    content = "3+4==7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7-8==-1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8-2!=2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7+3!=3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7==3+4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1==7-8";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8!=8-2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7!=7+3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "7!=10-3";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "7-10!=-3";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "3+4>=7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3-8>-10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8-2<10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7+3<=10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7<=3+4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1<7+8";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8>8-2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7>=10-3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&1==1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&10==10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&5!=1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&7!=1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0==0&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1==1&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1!=0&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0!=1&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||10==1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||10==10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||5!=1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||7!=1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0==0||1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1==1||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1!=0||1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0!=1||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&1<=1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&3<10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&5>1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&0>=0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0<=0&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1<2&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1>0&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10>=5&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||10<=1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||1<10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||5>=10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||7>1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0>=0||1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4>3||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1<0||1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0<1||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);
}

static void TestEvaluate9(TestHelper &helper) {
    Value<char> value;
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "(3*2^3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 24, "number", __LINE__);

    content = "(3*10%7)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 9, "number", __LINE__);

    content = "(135/3^3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 5, "number", __LINE__);

    content = "(18/10%7)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 6, "number", __LINE__);

    content = "(4^2*2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 32, "number", __LINE__);

    content = "(10%7*2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 6, "number", __LINE__);

    content = "(3^3/4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 6.75, "number", __LINE__);

    content = "(10%4/2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(4+2*2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8, "number", __LINE__);

    content = "(10-7*2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -4, "number", __LINE__);

    content = "(3+16/4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 7, "number", __LINE__);

    content = "(10-6/3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8, "number", __LINE__);

    content = "(2*8-2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 14, "number", __LINE__);

    content = "(3*4-20)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -8, "number", __LINE__);

    content = "(14/7+3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 5, "number", __LINE__);

    content = "(9/3-4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -1, "number", __LINE__);

    content = "(3+4==7)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7-8==-1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8-2!=2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7+3!=3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7==3+4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(-1==7-8)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8!=8-2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7!=7+3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(7!=10-3)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(7-10!=-3)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(3+4>=7)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(3-8>-10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8-2<10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7+3<=10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7<=3+4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(-1<7+8)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8>8-2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7>=10-3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&1==1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&10==10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&5!=1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&7!=1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0==0&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1==1&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1!=0&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0!=1&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||10==1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||10==10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||5!=1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||7!=1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0==0||1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1==1||0)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1!=0||1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0!=1||0)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&1<=1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&3<10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&5>1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&0>=0)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0<=0&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1<2&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1>0&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(10>=5&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||10<=1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||1<10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||5>=10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||7>1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0>=0||1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(4>3||0)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1<0||1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0<1||0)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);
}

static void TestEvaluate10(TestHelper &helper) {
    Value<char> value;
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "3*(2)^3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 24, "number", __LINE__);

    content = "3*(5+5)%7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 9, "number", __LINE__);

    content = "135/(5-2)^3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 5, "number", __LINE__);

    content = "18/(5*2)%7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 6, "number", __LINE__);

    content = "4^(4/2)*2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 32, "number", __LINE__);

    content = "10%(3.5*2)*2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 6, "number", __LINE__);

    content = "3^(1.5*2)/4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 6.75, "number", __LINE__);

    content = "10%(8-4)/2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4+(8/4)*2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8, "number", __LINE__);

    content = "10-(14/2)*2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -4, "number", __LINE__);

    content = "3+(4+12)/4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 7, "number", __LINE__);

    content = "10-(3+2+1)/3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8, "number", __LINE__);

    content = "2*(2+1+5)-2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 14, "number", __LINE__);

    content = "3*(8-4)-20";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -8, "number", __LINE__);

    content = "14/(10-3)+3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 5, "number", __LINE__);

    content = "9/(1+1+1)-4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -1, "number", __LINE__);

    content = "3+(2*2)==7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7-(10-2)==-1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8-(1+1)!=2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7+(9/3)!=3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7==(12/4)+4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1==(2+5)-8";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8!=(2*2)-2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7!=(1+5+1)+3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "7!=(10)-3";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "7-(10)!=-3";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "3+(4)>=7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3-(8)>-10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8-(2)<10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7+(3)<=10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7<=(3)+4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1<(7)+8";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8>(8)-2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7>=(10)-3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(10-9)==1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(5+5)==10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(10/2)!=1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(4+4)!=1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0==(0/4)&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1==(4^0)&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1!=(3-3)&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0!=(4-1-2)&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||(2%2)==1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||(1==1)==1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||(15%10)!=1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||(-4)!=1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0==(0+0)||1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1==(100-99)||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1!=(4%5)||1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0!=(1^1)||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(10%9)<=1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(2+1)<10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(2.5+2.5)>1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(8%2)>=0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0<=(0)&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1<(8-1)&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1>(4-8)&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10>=(100/10)&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||(3+3)<=1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||(6%6)<10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||(5^2)>=10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||(7-2)>1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0>=(0&&1)||1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4>(2&&1)||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1<(4-4)||1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0<(4^0)||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);
}

static void TestEvaluate11(TestHelper &helper) {
    Value<char> value;
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "(3*(2)^3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 24, "number", __LINE__);

    content = "(3*(5+5)%7)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 9, "number", __LINE__);

    content = "(135/(5-2)^3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 5, "number", __LINE__);

    content = "(18/(5*2)%7)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 6, "number", __LINE__);

    content = "(4^(4/2)*2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 32, "number", __LINE__);

    content = "(10%(3.5*2)*2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 6, "number", __LINE__);

    content = "(3^(1.5*2)/4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 6.75, "number", __LINE__);

    content = "(10%(8-4)/2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(4+(8/4)*2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8, "number", __LINE__);

    content = "(10-(14/2)*2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -4, "number", __LINE__);

    content = "(3+(4+12)/4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 7, "number", __LINE__);

    content = "(10-(3+2+1)/3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8, "number", __LINE__);

    content = "(2*(2+1+5)-2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 14, "number", __LINE__);

    content = "(3*(8-4)-20)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -8, "number", __LINE__);

    content = "(14/(10-3)+3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 5, "number", __LINE__);

    content = "(9/(1+1+1)-4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -1, "number", __LINE__);

    content = "(3+(2*2)==7)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7-(10-2)==-1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8-(1+1)!=2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7+(9/3)!=3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7==(12/4)+4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(-1==(2+5)-8)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8!=(2*2)-2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7!=(1+5+1)+3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(7!=(10)-3)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(7-(10)!=-3)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(3+(4)>=7)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(3-(8)>-10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8-(2)<10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7+(3)<=10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7<=(3)+4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(-1<(7)+8)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8>(8)-2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7>=(10)-3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(10-9)==1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(5+5)==10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(10/2)!=1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(4+4)!=1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0==(0/4)&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1==(4^0)&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1!=(3-3)&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0!=(4-1-2)&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||(2%2)==1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||(1==1)==1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||(15%10)!=1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||(-4)!=1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0==(0+0)||1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1==(100-99)||0)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1!=(4%5)||1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0!=(1^1)||0)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(10%9)<=1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(2+1)<10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(2.5+2.5)>1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(8%2)>=0)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0<=(0)&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1<(8-1)&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1>(4-8)&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(10>=(100/10)&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||(3+3)<=1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||(6%6)<10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||(5^2)>=10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||(7-2)>1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0>=(0&&1)||1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(4>(2&&1)||0)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1<(4-4)||1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0<(4^0)||0)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "((1+2)^(1+2))/2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 13.5, "number", __LINE__);

    content = "((5/5+1)*2+1)+3*3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 14, "number", __LINE__);

    content = "5+2*4-8/2==9 && 1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1+2*3 == 7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "((5/5+1)*2+1)+3*3 != 12 && 1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "((2*4) + 1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 9, "number", __LINE__);

    content = "((2* (2 + 3)) + 1 - 4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 7, "number", __LINE__);

    content = "10==2+2*2^2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "2^2*2+2==10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3^2*2+2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 20, "number", __LINE__);

    content = "2+2*2^3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 18, "number", __LINE__);

    content = "2+3*3+2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 13, "number", __LINE__);

    content = " -1 - -2 + 3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 4, "number", __LINE__);

    content = "-1--2+3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 4, "number", __LINE__);

    content = "2+-3--4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 3, "number", __LINE__);

    content = R"(2  * 1 * 3 + 1 - 4 + (10 - 5 - 6 + 1 + 1 + 1 + 1) * (8 / 4 + 1) -1 - -1 + 2 == 14)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content =
        R"(((2* (1 * 3)) + 1 - 4) + (((10 - 5) -
        6 + ((1 + 1) + (1 + 1))) * (8 / 4 + 1)) -
        (1) - (-1) + 2)";
    number = Template::Evaluate(content, value);
    helper.Equal(number, 14, "number", __LINE__);

    content = R"(2  * 1 * 3 + 1 - 4 + (10 - 5 - 6 + 1 + 1 + 1 + 1) * (8 / 4 + 1) -
        1 - -1 + 2 == ((5/5+1)*2+1)+3*3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = R"(2*3^2+5)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 23, "number", __LINE__);

    content = R"(5+3^2*2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 23, "number", __LINE__);

    content = R"(((((((((2+2*2^3)))))))))";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 18, "number", __LINE__);
}

static void TestEvaluate12(TestHelper &helper) {
    Value<char> value;
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "a==a";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "a==c";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "a!=b";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "a!=a";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "a == a";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "a == c";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "a != b";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "a != a";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " a == a ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = " a == c ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " a != b ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = " a != a ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "  a    == a";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "  a    == a          ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "      a==a";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "      a             ==a";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "a==         a              ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   a      == c         ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   a !=         b";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   a!=a      ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "AbcDEF==AbcDEF";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "AbcDEF==zwx";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "AbcDEF!=BBBB";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "AbcDEF!=AbcDEF";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "AbcDEF == AbcDEF";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "AbcDEF == zwx";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "AbcDEF != BBBB";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "AbcDEF != AbcDEF";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " AbcDEF == AbcDEF ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = " AbcDEF == zwx ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " AbcDEF != BBBB ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = " AbcDEF != AbcDEF ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "  AbcDEF    == AbcDEF";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "  AbcDEF    == AbcDEF          ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "      AbcDEF==AbcDEF";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "      AbcDEF             ==AbcDEF";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "AbcDEF==         AbcDEF              ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   AbcDEF      == zwx         ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   AbcDEF !=         BBBB";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   AbcDEF!=AbcDEF      ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestEvaluate13(TestHelper &helper) {
    Value<char> value;
    double      number = -1;
    const char *content;
    bool        is_valid;

    content  = "{var:1} + 5";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "3 + {var:9}";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "10 == A";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "A == 10";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "10 == (A)";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(A) == 10";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{var:A} == A";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{var:A} != A";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "A=={var:A}";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "A==";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "A!={var:A}";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(4+3";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "4+3*(3";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(4+(2)";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{4+5";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content = "a";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 0, "number", __LINE__);

    value["1"]   = 6;
    value["A"]   = 6;
    value["AB"]  = 13;
    value["ABC"] = 26;
    value["Q"]   = "Qentem";
    value["QA"]  = "Qentem Evaluate";

    content = "a";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 0, "number", __LINE__);

    content = "{var:A}+10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 16, "number", __LINE__);

    content = "{var:AB}-3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 10, "number", __LINE__);

    content = "{var:ABC}*    2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 52, "number", __LINE__);

    content = "{var:A}    /2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 3, "number", __LINE__);

    content = "   {var:A}^2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 36, "number", __LINE__);

    content = "{var:AB}  %  5";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 3, "number", __LINE__);

    content = "{var:ABC}>7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   {var:AB} <6";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "{var:A}  >=4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "{var:ABC}<=30";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "{var:AB} && 0  ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "{var:A} && 1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "{var:ABC} || 0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "   {var:AB} || 1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10+{var:A}";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 16, "number", __LINE__);

    content = "3+{var:AB}";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 16, "number", __LINE__);

    content = "2*    {var:ABC}";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 52, "number", __LINE__);

    content = " 104   /{var:ABC}";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 4, "number", __LINE__);

    content = " 2  ^{var:A}";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 64, "number", __LINE__);

    content = "35 &&  {var:AB} ";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7<{var:ABC}";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   8 >{var:AB}";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "3  <={var:A}";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "26>={var:ABC}";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "0 && {var:AB}  ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "1 && {var:A}";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0 || {var:ABC}";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 1   || {var:AB}";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "{var:d} +5  ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{var:w}*0  ";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{var:}&&0";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);
}

static void TestEvaluate14(TestHelper &helper) {
    Value<char> value;
    double      number = -1;
    const char *content;
    bool        is_valid;

    value["1"]   = 6;
    value["A"]   = 6;
    value["AB"]  = 13;
    value["ABC"] = 26;
    value["Q"]   = "Qentem";
    value["QA"]  = "Qentem Evaluate";

    content = "3*{var:A}^3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 648, "number", __LINE__);

    content = "3*{var:AB}%7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 18, "number", __LINE__);

    content = "135/{var:A}^3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 0.625, "number", __LINE__);

    content = "54/{var:ABC}%10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 9, "number", __LINE__);

    content = "4^{var:A}*2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8192, "number", __LINE__);

    content = "10%({var:A})*2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8, "number", __LINE__);

    content = "3^{var:A}/(4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 182.25, "number", __LINE__);

    content = "(10)%{var:AB}/2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 5, "number", __LINE__);

    content = "4+({var:A})*2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 16, "number", __LINE__);

    content = "10-({var:AB})*2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -16, "number", __LINE__);

    content = "3+{var:ABC}/(4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 9.5, "number", __LINE__);

    content = "(10)-{var:A}/3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8, "number", __LINE__);

    content = "2*{var:ABC}-2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 50, "number", __LINE__);

    content = "(3*{var:A}-20)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -2, "number", __LINE__);

    content = "(65/{var:AB}+3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 8, "number", __LINE__);

    content = "52/{var:ABC}-4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, -2, "number", __LINE__);

    content = "(3+{var:ABC}==29)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7-{var:AB}==-6";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8-{var:AB})!=2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7+{var:ABC}!=3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10=={var:A}+4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10==({var:A}+4)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1==({var:A})-7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8!=({var:ABC}-2)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7!=({var:AB}+3)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7!={var:AB}+3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "7!=({var:AB}-6)";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(4-{var:A})!=-2";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "4-{var:A}!=-2";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "3+{var:ABC}>=7";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3-{var:A}>-10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8-{var:A}<10";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7+{var:ABC}<=40";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7<={var:AB}+4";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1<{var:ABC}+8";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8>{var:A}-2";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7>={var:A}-3";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&({var:ABC})==26";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&({var:AB})==13";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&{var:A})!=61";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&({var:ABC})!=126";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0==(0/{var:AB})&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1==({var:A}^0)&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1!=({var:ABC}))&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0!=({var:A}&&1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||({var:AB})==1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||{var:Q}==Qentem";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||{var:Q}!=Qentem";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||{var:Q}!=Qen";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1!={var:1}||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "6==({var:A})||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "6=={var:A}||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "Qentem!={var:QA}||1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0!=({var:AB})||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&{var:A}<=6";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&{var:AB}<27";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&{var:ABC}>1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&{var:A}>=0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0<={var:AB}&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1<{var:A}&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "1>{var:ABC}&&1";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "10>={var:A}&&1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||({var:AB}<=1)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||{var:AB}<14";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||{var:A}>=10)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||{var:ABC}>1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0>={var:AB}||1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7>{var:A}||0)";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1<{var:AB}||1";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0<{var:ABC}||0";
    number  = Template::Evaluate(content, value);
    helper.Equal(number, 1, "number", __LINE__);
}

static void TestEvaluate15(TestHelper &helper) {
    Value<char> value;
    double      number = -1;
    const char *content;
    bool        is_valid;

    value["1"]   = 6;
    value["A"]   = 6;
    value["AB"]  = 13;
    value["ABC"] = 26;
    value["Q"]   = "Qentem";
    value["QA"]  = "Qentem Evaluate";

    content  = "{var:Q}   ==Qentem";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{var:QA}==     Qentem Evaluate";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "        {var:Q}   ==Qentem";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "             {var:QA}==     Qentem Evaluate";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{var:Q}==          Qentem";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{var:QA}==Qentem Evaluate        ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{var:Q}==    Qentem          ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "  {var:QA}    ==     Qentem Evaluate     ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{var:10}==Qentem";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{var:Q}!=Qentem";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{var:QA}!=Qentem Evaluate";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{var:2}!=Qentem Evaluate";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{var:Q}==Qente";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{var:QA}==Evaluate";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{var:Q}!=Qente";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{var:QA}!=Evaluate";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem   =={var:Q}";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem Evaluate==     {var:QA}";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "        Qentem   =={var:Q}";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "             Qentem Evaluate==     {var:QA}";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem==          {var:Q}";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem Evaluate=={var:QA}        ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem==    {var:Q}          ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "  Qentem Evaluate    ==     {var:QA}     ";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem=={var:10}";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "Qentem!={var:Q}";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem Evaluate!={var:QA}";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem Evaluate!={var:2}";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "Qente=={var:Q}";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Evaluate=={var:QA}";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qente!={var:Q}";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Evaluate!={var:QA}";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{var:A}==6";
    is_valid = Template::Evaluate(number, content, value);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestEvaluate16(TestHelper &helper) {
    Value<char> value;
    double      number = -1;
    const char *content;
    bool        is_valid;

    value["1"]   = 6;
    value["A"]   = 6;
    value["AB"]  = 13;
    value["ABC"] = 26;
    value["Q"]   = "Qentem";
    value["QA"]  = "Qentem Evaluate";

    content  = "2      {var:A}";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "7{var:ABC}";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{var:AB}3";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{var:AB} 9";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{var:AB}  &  5";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(3)3";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "3(3)";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "4+3(7)";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "1!#1";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "1|#1";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{var:Q}=={A}+1";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "1U0=={var:A}+4";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{var:A}=={Q}";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{var:A}==1U0";
    is_valid = Template::Evaluate(number, content, value);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);
}

static int RunEvaluateTests() {
    TestHelper helper{"Evaluate.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("Evaluate Test 1", TestEvaluate1);
    helper.Test("Evaluate Test 2", TestEvaluate2);
    helper.Test("Evaluate Test 3", TestEvaluate3);
    helper.Test("Evaluate Test 4", TestEvaluate4);
    helper.Test("Evaluate Test 5", TestEvaluate5);
    helper.Test("Evaluate Test 6", TestEvaluate6);
    helper.Test("Evaluate Test 7", TestEvaluate7);
    helper.Test("Evaluate Test 8", TestEvaluate8);
    helper.Test("Evaluate Test 9", TestEvaluate9);
    helper.Test("Evaluate Test 10", TestEvaluate10);
    helper.Test("Evaluate Test 11", TestEvaluate11);
    helper.Test("Evaluate Test 12", TestEvaluate12);
    helper.Test("Evaluate Test 13", TestEvaluate13);
    helper.Test("Evaluate Test 14", TestEvaluate14);
    helper.Test("Evaluate Test 15", TestEvaluate15);
    helper.Test("Evaluate Test 16", TestEvaluate16);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
