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

#include "QTest.hpp"
#include "Template.hpp"
#include "Value.hpp"
#include "StringStream.hpp"

#ifndef QENTEM_EVALUATE_TESTS_H
#define QENTEM_EVALUATE_TESTS_H

namespace Qentem {
namespace Test {

QENTEM_NOINLINE static bool TestEvaluate(QNumber64 &number, const char *content, const Value<char> &value) {
    using TemplateSubCV             = TemplateSub<char, Value<char>, StringStream<char>>;
    const SizeT              length = StringUtils::Count(content);
    const TemplateSubCV      temp{content, length, nullptr, &value};
    const Array<QExpression> exprs = temp.ParseExpressions(0, length);
    QExpression              result;

    if (temp.Evaluate(result, exprs)) {
        number = result.Value.Number;
        return true;
    }

    number = 0;
    return false;
}

QENTEM_NOINLINE static QNumber64 TestEvaluate(const char *content, const Value<char> &value) {
    QNumber64 number;
    TestEvaluate(number, content, value);
    return number;
}

static void TestEvaluate1(QTest &helper) {
    const Value<char> value;
    QNumber64         number;
    const char       *content;
    bool              is_valid;

    content  = "";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = " ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "  ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "               ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "()";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "( )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(  )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(        )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = " ()";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = " ( )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = " (  )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = " (        )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "() ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "( ) ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(  ) ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(        ) ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "  ()";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "  ( )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "  (  )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "  (        )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "()  ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "( )  ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(  )  ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(        )  ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = " () ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = " ( ) ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = " (  ) ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = " (        ) ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "   ()   ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "   ( )   ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "   (  )   ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "   (        )   ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "a";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "10/0";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "10/(2-2)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "fj";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "fgh";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "5f2s4s5dg";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(d)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(oo)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(www46fs5d4f5ds)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "() ()";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(())";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "() (()) ()";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(()) (()()) ((()())())";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "3()";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "3(1)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "3(1+1)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);
}

static void TestEvaluate2(QTest &helper) {
    const Value<char> value;
    QNumber64         number{-99};
    const char       *content;
    bool              is_valid;

    content  = "1";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    // NOTE: Implement power of fraction.
    content  = "2^0.2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);
    // helper.IsEqual(number.Natural, 1.148698355,  __LINE__);
    // helper.IsTrue(is_valid,  __LINE__);

    content  = " 4";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 4ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "5 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 5ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " 10 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 10ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " 7 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 7ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "100";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 100ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " 40";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 40ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "5000 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 5000ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " 1004 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1004ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " 714 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 714ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(1)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " (4)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 4ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(5) ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 5ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " (10) ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 10ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " (7) ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 7ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(100)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 100ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " (40)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 40ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(5000) ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 5000ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " (1004) ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1004ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " ( 714 ) ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 714ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "((1))";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " (( 4))";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 4ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "((5     ) )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 5ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "( 10 )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 10ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "( (((       7   ))) )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 7ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(100)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 100ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "( 40)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 40ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(5000 )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 5000ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(((((((((( 1004 ))))))))))";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1004ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "( 714 )";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 714ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "0^10";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);
}

static void TestEvaluate3(QTest &helper) {
    const Value<char> value;
    QNumber64         number;
    const char       *content;
    bool              is_valid;

    content = "3^2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 9ULL, __LINE__);

    content = "7%5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 2ULL, __LINE__);

    content = "3*6";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 18ULL, __LINE__);

    content = "10/2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.0, __LINE__);

    content = "3+5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 8ULL, __LINE__);

    content = "5-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 3ULL, __LINE__);

    content = "3>2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "2>2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content  = "5==2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "5==5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "5!=2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "10!=10";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "1>5";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "5>=1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "8>=8";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "5>=7";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "4<8";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "7<7";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "10<9";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "3<=16";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "3<=3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "17<=3";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "1&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&800";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "0&&5";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "0&&0";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "8||6";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "4||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0||9";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "0||0";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "   3   ^2   ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 9ULL, __LINE__);

    content = "   7%   5   ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 2ULL, __LINE__);

    content = "   3   *   6";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 18ULL, __LINE__);

    content = "10   /   2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.0, __LINE__);

    content = "3   +   5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 8ULL, __LINE__);

    content  = " 5 == 2 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = " 5 == 5 ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = " 5 != 2 ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = " 10 != 10 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = " 5 - 2 ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 3ULL, __LINE__);

    content = "   3 > 2   ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "   2   >2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "1>   5   ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = " 5   >=1   ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "   8>=   8 ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "5   >=   7";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "   4   <8   ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "   7<   7   ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "   10<   0";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "   3   <=   16";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "   3   <=   3     ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "   17   <=3";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "   1   &&   1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "   1   &&   800    ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "   0&&5  ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "   0   &&0";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "   8   ||   6  ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "4   ||   0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "   0||9";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "   0   ||0";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);
}

static void TestEvaluate4(QTest &helper) {
    const Value<char> value;
    QNumber64         number;
    const char       *content;
    bool              is_valid;

    content = "2 ^ -2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 0.25, __LINE__);

    content = "4 ^-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 0.0625, __LINE__);

    content = "-3^0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "5564685^0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "-3^2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 9ULL, __LINE__);

    content = "-8^-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -0.015625, __LINE__);

    content = "-4 * 5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -20, __LINE__);

    content = "5 *-6";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -30, __LINE__);

    content = "-3*-4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 12ULL, __LINE__);

    content = "-10 / 2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -5.0, __LINE__);

    content = "7 /-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -3.5, __LINE__);

    content = "7 + -2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 5ULL, __LINE__);

    content = "2 + -7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -5, __LINE__);

    content = "-9 / -3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 3, __LINE__);

    content = "-7 + 5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -2, __LINE__);

    content = "-11 + -2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -13, __LINE__);

    content = "-8+-3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -11, __LINE__);

    content = "5 --2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 7ULL, __LINE__);

    content = "-8-3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -11, __LINE__);

    content  = " 5 == -5 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " 5 ==-5 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " -5 == 5 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = " -5 == 5 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = " -5 != 2 ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = " 10 != -10 ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = " -10 != 10 ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = " 10 !=-10 ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = " 10 != -10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "3 >-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "10 > -10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "-2 > 2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "-2 > -11";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "-1>5";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "5 >=-19";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "-1 >=-100";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "-8 >=-8";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "-5 >= 7";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "3 <-2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "10 < -10";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "-2 < 2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "-2 < -11";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "-1<5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "5 <=-19";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "5 - <=-19";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "-5 <=-19 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "-8 <= -8";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "-5 <= 7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "-1 && 1";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "1 &&   -800";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "0 && -5";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "-8 && -9";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "-8 || 6";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "4 ||-9";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "0 ||           -9";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "-7 || -500";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);
}

static void TestEvaluate5(QTest &helper) {
    const Value<char> value;
    QNumber64         number;
    const char       *content;
    bool              is_valid;

    content = "16 ^ +2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 256ULL, __LINE__);

    content = "8 ^+2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 64ULL, __LINE__);

    content = "+3^0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "5564685^0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "+3^2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 9ULL, __LINE__);

    content = "+3^+3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 27ULL, __LINE__);

    content = "+4 * 5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 20ULL, __LINE__);

    content = "5 *+6";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 30ULL, __LINE__);

    content = "+3*+4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 12ULL, __LINE__);

    content = "+10 / 2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.0, __LINE__);

    content = "7 /+2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 3.5, __LINE__);

    content = "+9 / +3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 3.0, __LINE__);

    content = "+7 - 5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 2ULL, __LINE__);

    content = "+11 - +2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 9ULL, __LINE__);

    content = "+8-+3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 5ULL, __LINE__);

    content = "5 ++2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 7ULL, __LINE__);

    content = "+8+3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 11ULL, __LINE__);

    content = "5 + +2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 7ULL, __LINE__);

    content = "+5 + +2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 7ULL, __LINE__);

    content  = "+5==2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "5==+2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "+5==+2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "+5==+5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "+5!=+2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "+10!=+10";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "+10!=10";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "10!=+10";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "3 >+2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "10 > +10";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "+2 > 2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "+11 > +2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "+1>5";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "+19 >=5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "+1 >=+100 ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "+8 >=  +8 ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "7+ >= 5";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "3 <+2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "10 < +10";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "+1 < 2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "+2 < +11";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "+1<5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "+19 <=5";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "+5 <=+19";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "+8 <= +8";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "+5 <= 7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "+1 && 1";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "1&&+800";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "0 && +5";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "+8 && +9";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "+8 || 6";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "4 ||+9";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content =
        R"(4                                                                      ||1
                                                                                       ||
                                                                                             2)";
    number = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "0 || +9";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "+7 || +500";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);
}

static void TestEvaluate6(QTest &helper) {
    const Value<char> value;
    QNumber64         number{-1};
    const char       *content;
    bool              is_valid;

    content  = "(0)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(+1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(-1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -1, __LINE__);

    content = "( 1 )";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "( +1 )";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "( -1 )";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -1, __LINE__);

    content = "(1 )";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(+1 )";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(-1 )";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -1, __LINE__);

    content = "( 1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "( +1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "( -1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -1, __LINE__);

    content = "(       1            )";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(       +1             )";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(        -1           )";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -1, __LINE__);

    content = "(2*3)^4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1296ULL, __LINE__);

    content = "(8*3) %7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 3ULL, __LINE__);

    content = "(6+2)  *6";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 48ULL, __LINE__);

    content = " (15-5)  /5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 2.0, __LINE__);

    content = "   (4*3)   +  5  ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 17ULL, __LINE__);

    content = "(18/3)- 2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 4.0, __LINE__);

    content = "(13%5)           >2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(3                  ^-2)>2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content  = "(1+2)== 2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(6-1) ==5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = " (3^2)!=2 ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(20/2) != 10";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(  10  -  +9  )>5";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(15%10)>=1             ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "          (16/2)>=8";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(4*1.5)>=7";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(0.999999+0.1)<8";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(20*0.5)<7";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(4^2)<9";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(9/2)<=16";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1.5*2)<=3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(20-8)<=3";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(1*1)&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&1)&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(100-5)&&800";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(0*10)&&5";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(8%2)&&0";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(4/2)||6";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1||0)||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(9%3)||9";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(0||0)||0";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);
}

static void TestEvaluate7(QTest &helper) {
    const Value<char> value;
    QNumber64         number{-1};
    const char       *content;
    bool              is_valid;

    content = "6^(8/2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1296ULL, __LINE__);

    content = "24 %(3.5*2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 3ULL, __LINE__);

    content = "8  *(12/2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 48.0, __LINE__);

    content = " 10  /(7-2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 2.0, __LINE__);

    content = "   12   +  (2.5*2)  ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 17.0, __LINE__);

    content = "6- (10/5)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 4.0, __LINE__);

    content = "(3)           >(2*1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0.9999999999>(4-2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content  = "3== (3-1)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(5) ==(10-5)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = " 9!=(1+1) ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "10 != (20/2)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "1>(3+2)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "5>=(100^0)             ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = " 8>=(2^3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "6>=(14/2)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "1<(4+2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "10<(10-3)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "16<(18/2)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "4.5<=(4*4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "3<=(1.5+1.5)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "12<=(3*1)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "1&&(1^10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "10&&(10^1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7&&(10-5)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "0&&(2+2)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "0&&(0^0)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "9||        (3*2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "        1||(0*10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0          ||(3*3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "        0||(10&&0)          ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(2*3)^(8/2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1296ULL, __LINE__);

    content = "(8*3) %(3.5*2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 3ULL, __LINE__);

    content = "(6+2)  *(12/2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 48.0, __LINE__);

    content = " (15-5)  /(7-2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 2.0, __LINE__);

    content = "   (4*3)   +  (2.5*2)  ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 17.0, __LINE__);

    content = "(18/3)- (10/5)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 4.0, __LINE__);

    content = "(13%5)           >(2*1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(3                  ^-2)>(4-2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content  = "(1+2)== (3-1)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(6-1) ==(10-5)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = " (3^2)!=(1+1) ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(20/2) != (20/2)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(  10-+9  )>(3+2)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(15%10)>=(100^0)             ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "          (16/2)>=(2^3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(4*1.5)>=(14/2)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(0.999999+0.1)<(4+2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(20*0.5)<(10-3)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(4^2)<(18/2)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(9/2)<=(4*4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1.5*2)<=(1.5+1.5)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(20-8)<=(3*1)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(1*1)&&(1^10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&1)&&(10^1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(100-5)&&(10-5)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(0*10)&&(2+2)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(8%2)&&(0^0)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(4/2)||(3*2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1||0)||(0*10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(9%3)||(3*3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(0||0)||(10&&0)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);
}

static void TestEvaluate8(QTest &helper) {
    const Value<char> value;
    QNumber64         number{-1};
    const char       *content;
    bool              is_valid;

    content = "3*2^3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 24ULL, __LINE__);

    content = "3*2^2+1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 13ULL, __LINE__);

    content = "3*10%7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 9ULL, __LINE__);

    content = "135/3^3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.0, __LINE__);

    content = "18/10%7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 6.0, __LINE__);

    content = "4^2*2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 32ULL, __LINE__);

    content = "10%7*2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 6ULL, __LINE__);

    content = "3^3/4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 6.75, __LINE__);

    content = "10%4/2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 1.0, __LINE__);

    content = "4+2*2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 8ULL, __LINE__);

    content = "10-7*2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -4, __LINE__);

    content = "3+16/4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 7.0, __LINE__);

    content = "10-6/3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 8.0, __LINE__);

    content = "2*8-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 14ULL, __LINE__);

    content = "3*4-20";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -8, __LINE__);

    content = "14/7+3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.0, __LINE__);

    content = "9/3-4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -1.0, __LINE__);

    content = "3+4==7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7-8==-1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "8-2!=2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7+3!=3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7==3+4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "-1==7-8";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "8!=8-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7!=7+3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "7!=10-3";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "7-10!=-3";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "3+4>=7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "3-8>-10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "8-2<10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7+3<=10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7<=3+4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "-1<7+8";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "8>8-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7>=10-3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&1==1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&10==10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&5!=1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&7!=1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0==0&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1==1&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1!=0&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0!=1&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1||10==1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0||10==10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1||5!=1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0||7!=1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0==0||1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1==1||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1!=0||1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0!=1||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&1<=1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&3<10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&5>1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&0>=0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0<=0&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1<2&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1>0&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "10>=5&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1||10<=1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0||1<10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1||5>=10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0||7>1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0>=0||1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "4>3||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1<0||1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0<1||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
}

static void TestEvaluate9(QTest &helper) {
    const Value<char> value;
    QNumber64         number{-1};
    const char       *content;
    bool              is_valid;

    content = "(3*2^3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 24ULL, __LINE__);

    content = "(3*10%7)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 9ULL, __LINE__);

    content = "(135/3^3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.0, __LINE__);

    content = "(18/10%7)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 6.0, __LINE__);

    content = "(4^2*2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 32ULL, __LINE__);

    content = "(10%7*2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 6ULL, __LINE__);

    content = "(3^3/4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 6.75, __LINE__);

    content = "(10%4/2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 1.0, __LINE__);

    content = "(4+2*2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 8ULL, __LINE__);

    content = "(10-7*2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -4, __LINE__);

    content = "(3+16/4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 7.0, __LINE__);

    content = "(10-6/3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 8.0, __LINE__);

    content = "(2*8-2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 14ULL, __LINE__);

    content = "(3*4-20)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -8, __LINE__);

    content = "(14/7+3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.0, __LINE__);

    content = "(9/3-4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -1.0, __LINE__);

    content = "(3+4==7)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7-8==-1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(8-2!=2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7+3!=3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7==3+4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(-1==7-8)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(8!=8-2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7!=7+3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(7!=10-3)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(7-10!=-3)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(3+4>=7)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(3-8>-10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(8-2<10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7+3<=10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7<=3+4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(-1<7+8)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(8>8-2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7>=10-3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&1==1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&10==10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&5!=1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&7!=1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0==0&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1==1&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1!=0&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0!=1&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1||10==1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0||10==10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1||5!=1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0||7!=1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0==0||1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1==1||0)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1!=0||1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0!=1||0)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&1<=1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&3<10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&5>1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&0>=0)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0<=0&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1<2&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1>0&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(10>=5&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1||10<=1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0||1<10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1||5>=10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0||7>1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0>=0||1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(4>3||0)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1<0||1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0<1||0)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
}

static void TestEvaluate10(QTest &helper) {
    const Value<char> value;
    QNumber64         number{-1};
    const char       *content;
    bool              is_valid;

    content = "3*(2)^3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 24ULL, __LINE__);

    content = "3*(5+5)%7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 9ULL, __LINE__);

    content = "135/(5-2)^3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.0, __LINE__);

    content = "18/(5*2)%7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 6.0, __LINE__);

    content = "4^(4/2)*2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 32ULL, __LINE__);

    content = "10%(3.5*2)*2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 6ULL, __LINE__);

    content = "3^(1.5*2)/4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 6.75, __LINE__);

    content = "10%(8-4)/2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 1.0, __LINE__);

    content = "4+(8/4)*2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 8.0, __LINE__);

    content = "10-(14/2)*2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -4.0, __LINE__);

    content = "3+(4+12)/4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 7.0, __LINE__);

    content = "10-(3+2+1)/3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 8.0, __LINE__);

    content = "2*(2+1+5)-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 14ULL, __LINE__);

    content = "3*(8-4)-20";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -8, __LINE__);

    content = "14/(10-3)+3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.0, __LINE__);

    content = "9/(1+1+1)-4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -1.0, __LINE__);

    content = "3+(2*2)==7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7-(10-2)==-1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "8-(1+1)!=2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7+(9/3)!=3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7==(12/4)+4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "-1==(2+5)-8";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "8!=(2*2)-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7!=(1+5+1)+3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "7!=(10)-3";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "7-(10)!=-3";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "3+(4)>=7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "3-(8)>-10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "8-(2)<10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7+(3)<=10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7<=(3)+4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "-1<(7)+8";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "8>(8)-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7>=(10)-3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&(10-9)==1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&(5+5)==10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&(10/2)!=1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&(4+4)!=1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0==(0/4)&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1==(4^0)&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1!=(3-3)&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0!=(4-1-2)&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1||(2%2)==1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0||(1==1)==1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1||(15%10)!=1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0||(-4)!=1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0==(0+0)||1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1==(100-99)||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1!=(4%5)||1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0!=(1^1)||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&(10%9)<=1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&(2+1)<10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&(2.5+2.5)>1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&(8%2)>=0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0<=(0)&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1<(8-1)&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1>(4-8)&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "10>=(100/10)&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1||(3+3)<=1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0||(6%6)<10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1||(5^2)>=10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0||(7-2)>1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0>=(0&&1)||1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "4>(2&&1)||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1<(4-4)||1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0<(4^0)||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
}

static void TestEvaluate11(QTest &helper) {
    const Value<char> value;
    QNumber64         number{-1};
    const char       *content;
    bool              is_valid;

    content = "(3*(2)^3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 24ULL, __LINE__);

    content = "(3*(5+5)%7)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 9ULL, __LINE__);

    content = "(135/(5-2)^3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.0, __LINE__);

    content = "(18/(5*2)%7)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 6.0, __LINE__);

    content = "(4^(4/2)*2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 32ULL, __LINE__);

    content = "(10%(3.5*2)*2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 6ULL, __LINE__);

    content = "(3^(1.5*2)/4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 6.75, __LINE__);

    content = "(10%(8-4)/2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 1.0, __LINE__);

    content = "(4+(8/4)*2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 8.0, __LINE__);

    content = "(10-(14/2)*2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -4.0, __LINE__);

    content = "(3+(4+12)/4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 7.0, __LINE__);

    content = "(10-(3+2+1)/3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 8.0, __LINE__);

    content = "(2*(2+1+5)-2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 14ULL, __LINE__);

    content = "(3*(8-4)-20)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -8, __LINE__);

    content = "(14/(10-3)+3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.0, __LINE__);

    content = "(9/(1+1+1)-4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -1.0, __LINE__);

    content = "(3+(2*2)==7)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7-(10-2)==-1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(8-(1+1)!=2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7+(9/3)!=3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7==(12/4)+4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(-1==(2+5)-8)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(8!=(2*2)-2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7!=(1+5+1)+3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "(7!=(10)-3)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(7-(10)!=-3)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "(3+(4)>=7)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(3-(8)>-10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(8-(2)<10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7+(3)<=10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7<=(3)+4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(-1<(7)+8)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(8>(8)-2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7>=(10)-3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&(10-9)==1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&(5+5)==10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&(10/2)!=1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&(4+4)!=1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0==(0/4)&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1==(4^0)&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1!=(3-3)&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0!=(4-1-2)&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1||(2%2)==1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0||(1==1)==1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1||(15%10)!=1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0||(-4)!=1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0==(0+0)||1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1==(100-99)||0)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1!=(4%5)||1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0!=(1^1)||0)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&(10%9)<=1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&(2+1)<10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&(2.5+2.5)>1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&(8%2)>=0)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0<=(0)&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1<(8-1)&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1>(4-8)&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(10>=(100/10)&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1||(3+3)<=1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0||(6%6)<10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1||(5^2)>=10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0||(7-2)>1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0>=(0&&1)||1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(4>(2&&1)||0)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1<(4-4)||1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(0<(4^0)||0)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "((1+2)^(1+2))/2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 13.5, __LINE__);

    content = "((5/5+1)*2+1)+3*3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 14.0, __LINE__);

    content = "5+2*4-8/2==9 && 1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1+2*3 == 7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "((5/5+1)*2+1)+3*3 != 12 && 1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "((2*4) + 1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 9ULL, __LINE__);

    content = "((2* (2 + 3)) + 1 - 4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 7ULL, __LINE__);

    content = "10==2+2*2^2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "2^2*2+2==10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "3^2*2+2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 20ULL, __LINE__);

    content = "2+2*2^3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 18ULL, __LINE__);

    content = "2+3*3+2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 13ULL, __LINE__);

    content = " -1 - -2 + 3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 4ULL, __LINE__);

    content = "-1--2+3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 4ULL, __LINE__);

    content = "2+-3--4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 3ULL, __LINE__);

    content = R"(2  * 1 * 3 + 1 - 4 + (10 - 5 - 6 + 1 + 1 + 1 + 1) * (8 / 4 + 1) -1 - -1 + 2 == 14)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content =
        R"(((2* (1 * 3)) + 1 - 4) + (((10 - 5) -
        6 + ((1 + 1) + (1 + 1))) * (8 / 4 + 1)) -
        (1) - (-1) + 2)";
    number = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 14.0, __LINE__);

    content = R"(2  * 1 * 3 + 1 - 4 + (10 - 5 - 6 + 1 + 1 + 1 + 1) * (8 / 4 + 1) -
        1 - -1 + 2 == ((5/5+1)*2+1)+3*3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = R"(2*3^2+5)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 23ULL, __LINE__);

    content = R"(5+3^2*2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 23ULL, __LINE__);

    content = R"(((((((((2+2*2^3)))))))))";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 18ULL, __LINE__);
}

static void TestEvaluate12(QTest &helper) {
    const Value<char> value;
    QNumber64         number{-1};
    const char       *content;
    bool              is_valid;

    content = "a==a";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "a==c";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "a!=b";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "a!=a";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "a == a";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "a == c";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "a != b";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "a != a";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = " a == a ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = " a == c ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = " a != b ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = " a != a ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "  a    == a";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "  a    == a          ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "      a==a";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "      a             ==a";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "a==         a              ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "   a      == c         ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "   a !=         b";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "   a!=a      ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "AbcDEF==AbcDEF";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "AbcDEF==zwx";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "AbcDEF!=BBBB";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "AbcDEF!=AbcDEF";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "AbcDEF == AbcDEF";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "AbcDEF == zwx";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "AbcDEF != BBBB";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "AbcDEF != AbcDEF";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = " AbcDEF == AbcDEF ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = " AbcDEF == zwx ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = " AbcDEF != BBBB ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = " AbcDEF != AbcDEF ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "  AbcDEF    == AbcDEF";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "  AbcDEF    == AbcDEF          ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "      AbcDEF==AbcDEF";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "      AbcDEF             ==AbcDEF";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "AbcDEF==         AbcDEF              ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "   AbcDEF      == zwx         ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "   AbcDEF !=         BBBB";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "   AbcDEF!=AbcDEF      ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);
}

static void TestEvaluate13(QTest &helper) {
    Value<char> value;
    QNumber64   number{-1};
    const char *content;
    bool        is_valid;

    content  = "{var:1} + 5";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "3 + {var:9}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "10 == A";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "A == 10";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "10 == (A)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(A) == 10";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:A} == A";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:A} != A";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "A=={var:A}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "A==";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "A!={var:A}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(4+3";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "4+3*(3";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(4+(2)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{4+5";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:A}>4";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content = "a";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    value["1"]   = 6;
    value["A"]   = 6U;
    value["AB"]  = 13;
    value["ABC"] = 26;
    value["Q"]   = "Qentem";
    value["QA"]  = "Qentem Evaluate";

    content = "a";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:A}+10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 16ULL, __LINE__);

    content = "{var:AB}-3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 10ULL, __LINE__);

    content = "{var:ABC}*    2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 52ULL, __LINE__);

    content = "{var:A}    /2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 3.0, __LINE__);

    content = "   {var:A}^2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 36ULL, __LINE__);

    content = "{var:AB}  %  5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 3ULL, __LINE__);

    content = "{var:ABC}>7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "   {var:AB} <6";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "{var:A}  >=4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:ABC}<=30";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "{var:AB} && 0  ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "{var:A} && 1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:ABC} || 0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "   {var:AB} || 1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "10+{var:A}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 16ULL, __LINE__);

    content = "3+{var:AB}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 16ULL, __LINE__);

    content = "2*    {var:ABC}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 52ULL, __LINE__);

    content = " 104   /{var:ABC}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 4.0, __LINE__);

    content = " 2  ^{var:A}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 64ULL, __LINE__);

    content = "35 &&  {var:AB} ";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7<{var:ABC}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "   8 >{var:AB}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "3  <={var:A}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "26>={var:ABC}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "0 && {var:AB}  ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "1 && {var:A}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0 || {var:ABC}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = " 1   || {var:AB}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "{var:d} +5  ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:w}*0  ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:}&&0";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "aa > 0";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);
}

static void TestEvaluate14(QTest &helper) {
    Value<char> value;
    QNumber64   number{-1};
    const char *content;
    bool        is_valid;

    value["1"]   = 6;
    value["A"]   = 6U;
    value["AB"]  = 13;
    value["ABC"] = 26;
    value["Q"]   = "Qentem";
    value["QA"]  = "Qentem Evaluate";

    content = "3*{var:A}^3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 648ULL, __LINE__);

    content = "3*{var:AB}%7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 18ULL, __LINE__);

    content = "135/{var:A}^3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 0.625, __LINE__);

    content = "54/{var:ABC}%10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 9.0, __LINE__);

    content = "4^{var:A}*2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 8192ULL, __LINE__);

    content = "10%({var:A})*2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 8ULL, __LINE__);

    content = "3^{var:A}/(4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 182.25, __LINE__);

    content = "(10)%{var:AB}/2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.0, __LINE__);

    content = "4+({var:A})*2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 16ULL, __LINE__);

    content = "10-({var:AB})*2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -16, __LINE__);

    content = "3+{var:ABC}/(4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 9.5, __LINE__);

    content = "(10)-{var:A}/3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 8.0, __LINE__);

    content = "2*{var:ABC}-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 50ULL, __LINE__);

    content = "(3*{var:A}-20)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -2, __LINE__);

    content = "(65/{var:AB}+3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 8.0, __LINE__);

    content = "52/{var:ABC}-4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -2.0, __LINE__);

    content = "(3+{var:ABC}==29)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7-{var:AB}==-6";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(8-{var:AB})!=2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7+{var:ABC}!=3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "10=={var:A}+4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "10==({var:A}+4)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "-1==({var:A})-7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "8!=({var:ABC}-2)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7!=({var:AB}+3)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7!={var:AB}+3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "7!=({var:AB}-6)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "(4-{var:A})!=-2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "4-{var:A}!=-2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "3+{var:ABC}>=7";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "3-{var:A}>-10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "8-{var:A}<10";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7+{var:ABC}<=40";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7<={var:AB}+4";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "-1<{var:ABC}+8";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "8>{var:A}-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "7>={var:A}-3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&({var:ABC})==26";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&({var:AB})==13";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1&&{var:A})!=61";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&({var:ABC})!=126";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0==(0/{var:AB})&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1==({var:A}^0)&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1!=({var:ABC}))&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0!=({var:A}&&1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1||({var:AB})==1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0||{var:Q}==Qentem";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1||{var:Q}!=Qentem";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0||{var:Q}!=Qen";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1!={var:1}||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "6==({var:A})||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "6=={var:A}||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "Qentem!={var:QA}||1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0!=({var:AB})||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&{var:A}<=6";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&{var:AB}<27";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&{var:ABC}>1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1&&{var:A}>=0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0<={var:AB}&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1<{var:A}&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content  = "1>{var:ABC}&&1";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content = "10>={var:A}&&1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1||({var:AB}<=1)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0||{var:AB}<14";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(1||{var:A}>=10)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0||{var:ABC}>1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0>={var:AB}||1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "(7>{var:A}||0)";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "1<{var:AB}||1";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0<{var:ABC}||0";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
}

static void TestEvaluate15(QTest &helper) {
    Value<char> value;
    QNumber64   number{-1};
    const char *content;
    bool        is_valid;

    value["1"]   = 6;
    value["A"]   = 6U;
    value["AB"]  = 13;
    value["ABC"] = 26;
    value["Q"]   = "Qentem";
    value["QA"]  = "Qentem Evaluate";

    content  = "{var:Q}   ==Qentem";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "{var:QA}==     Qentem Evaluate";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "        {var:Q}   ==Qentem";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "             {var:QA}==     Qentem Evaluate";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "{var:Q}==          Qentem";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "{var:QA}==Qentem Evaluate        ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "{var:Q}==    Qentem          ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "  {var:QA}    ==     Qentem Evaluate     ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "{var:10}==Qentem";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:Q}!=Qentem";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "{var:QA}!=Qentem Evaluate";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "{var:2}!=Qentem Evaluate";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:Q}==Qente";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "{var:QA}==Evaluate";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "{var:Q}!=Qente";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "{var:QA}!=Evaluate";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "Qentem   =={var:Q}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "Qentem Evaluate==     {var:QA}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "        Qentem   =={var:Q}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "             Qentem Evaluate==     {var:QA}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "Qentem==          {var:Q}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "Qentem Evaluate=={var:QA}        ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "Qentem==    {var:Q}          ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "  Qentem Evaluate    ==     {var:QA}     ";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "Qentem=={var:10}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "Qentem!={var:Q}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "Qentem Evaluate!={var:QA}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "Qentem Evaluate!={var:2}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "Qente=={var:Q}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "Evaluate=={var:QA}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "Qente!={var:Q}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "Evaluate!={var:QA}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);

    content  = "{var:A}==6";
    is_valid = TestEvaluate(number, content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
    helper.IsTrue(is_valid, __LINE__);
}

static void TestEvaluate16(QTest &helper) {
    Value<char> value;
    QNumber64   number{-1};
    const char *content;
    bool        is_valid;

    value["1"]   = 6;
    value["A"]   = 6U;
    value["AB"]  = 13;
    value["ABC"] = 26;
    value["Q"]   = "Qentem";
    value["QA"]  = "Qentem Evaluate";

    content  = "2      {var:A}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "7{var:ABC}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:AB}3";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:AB} 9";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:AB}  &  5";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "(3)3";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "3(3)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "4+3(7)";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "1!#1";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "1|#1";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:Q}=={A}+1";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "1U0=={var:A}+4";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:A}=={Q}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:A}==1U0";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "5+{var:QA}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "3=={var:Q}";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    content  = "{var:Q}==2";
    is_valid = TestEvaluate(number, content, value);
    helper.IsFalse(is_valid, __LINE__);

    value.Reset();
    value += -2;
    value += 3U;
    value += 3.5;
    value += 0.5;

    value += "-2";
    value += "3";
    value += "3.5";
    value += "0.5";

    content = "{var:0}==-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:1}==3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:2}==3.5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:3}==0.5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "-2=={var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "3=={var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "3.5=={var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0.5=={var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    //////
    content = "{var:4}==-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:5}==3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:6}==3.5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:7}==0.5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "-2=={var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "3=={var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "3.5=={var:6}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "0.5=={var:7}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    ///////////
    content = "{var:0}+-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -4, __LINE__);

    content = "{var:1}+3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 6ULL, __LINE__);

    content = "{var:2}+3.5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 7.0, __LINE__);

    content = "{var:3}+0.5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 1.0, __LINE__);

    content = "-2+{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -4, __LINE__);

    content = "3+{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 6ULL, __LINE__);

    content = "3.5+{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 7.0, __LINE__);

    content = "0.5+{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 1.0, __LINE__);

    //////
    content = "{var:4}+-2";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -4, __LINE__);

    content = "{var:5}+3";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 6ULL, __LINE__);

    content = "{var:6}+3.5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 7.0, __LINE__);

    content = "{var:7}+0.5";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 1.0, __LINE__);

    content = "-2+{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -4, __LINE__);

    content = "3+{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 6ULL, __LINE__);

    content = "3.5+{var:6}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 7.0, __LINE__);

    content = "0.5+{var:7}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 1.0, __LINE__);
}

static void TestEvaluate17(QTest &helper) {
    Value<char> value;
    QNumber64   number{-1};
    const char *content;

    value += -3;
    value += 5U;
    value += 8.5;
    value += int{3};

    content = "{var:0}+{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -6, __LINE__);

    content = "{var:1}+{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 10ULL, __LINE__);

    content = "{var:2}+{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 17.0, __LINE__);

    content = "{var:0}+{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 2ULL, __LINE__);

    content = "{var:1}+{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 2ULL, __LINE__);

    content = "{var:0}+{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.5, __LINE__);

    content = "{var:2}+{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 5.5, __LINE__);

    content = "{var:2}+{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 13.5, __LINE__);

    content = "{var:1}+{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 13.5, __LINE__);

    ///////////////////////////////////

    content = "{var:0}-{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:1}-{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:2}-{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:0}-{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -8, __LINE__);

    content = "{var:1}-{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 8ULL, __LINE__);

    content = "{var:0}-{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -11.5, __LINE__);

    content = "{var:2}-{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 11.5, __LINE__);

    content = "{var:2}-{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 3.5, __LINE__);

    content = "{var:1}-{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -3.5, __LINE__);

    ///////////////////////////////////

    content = "{var:0}*{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 9ULL, __LINE__);

    content = "{var:1}*{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 25ULL, __LINE__);

    content = "{var:2}*{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 72.25, __LINE__);

    content = "{var:0}*{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -15, __LINE__);

    content = "{var:1}*{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -15, __LINE__);

    content = "{var:0}*{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -25.5, __LINE__);

    content = "{var:2}*{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -25.5, __LINE__);

    content = "{var:2}*{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 42.5, __LINE__);

    content = "{var:1}*{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 42.5, __LINE__);

    content = "{var:3}*{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 15ULL, __LINE__);

    content = "{var:1}*{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 15ULL, __LINE__);
    ///////////////////////////////////
    value.Reset();
    value += -8;
    value += 16U;
    value += 4;

    content = "{var:0}/{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 1.0, __LINE__);

    content = "{var:1}/{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 1.0, __LINE__);

    content = "{var:2}/{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 1.0, __LINE__);

    content = "{var:0}/{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -0.5, __LINE__);

    content = "{var:1}/{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -2.0, __LINE__);

    content = "{var:0}/{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -2.0, __LINE__);

    content = "{var:2}/{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -0.5, __LINE__);

    content = "{var:2}/{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 0.25, __LINE__);

    content = "{var:1}/{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 4.0, __LINE__);

    ///////////////////////////////////

    content = "{var:0}=={var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:1}=={var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:2}=={var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:0}=={var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:1}=={var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:0}=={var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:2}=={var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:2}=={var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:1}=={var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    ///////////////////////////////////

    content = "{var:0}>={var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:1}>={var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:2}>={var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:0}>={var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:1}>={var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:0}>={var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:2}>={var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:2}>={var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:1}>={var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    ///////////////////////////////////

    content = "{var:0}<={var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:1}<={var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:2}<={var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:0}<={var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:1}<={var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:0}<={var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:2}<={var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:2}<={var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:1}<={var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    ///////////////////////////////////

    content = "{var:0}>{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:1}>{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:2}>{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:0}>{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:1}>{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:0}>{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:2}>{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:2}>{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:1}>{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    ///////////////////////////////////

    content = "{var:0}<{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:1}<{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:2}<{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:0}<{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:1}<{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:0}<{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:2}<{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:2}<{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:1}<{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
}

static void TestEvaluate18(QTest &helper) {
    Value<char> value;
    QNumber64   number{-1};
    const char *content;

    value += 2U;
    value += 3U;
    value += -2;
    value += -3;
    value += 4.0F;
    value += -4.0F;

    content = "{var:0}%{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:0}%{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 2ULL, __LINE__);

    content = "{var:0}%{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:0}%{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 2ULL, __LINE__);

    content = "{var:0}%{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 2ULL, __LINE__);

    content = "{var:0}%{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 2ULL, __LINE__);

    //////////////////

    content = "{var:1}%{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:1}%{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:1}%{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:1}%{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:1}%{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 3ULL, __LINE__);

    content = "{var:1}%{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 3ULL, __LINE__);

    //////////////////

    content = "{var:2}%{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:2}%{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -2, __LINE__);

    content = "{var:2}%{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:2}%{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -2, __LINE__);

    content = "{var:2}%{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -2, __LINE__);

    content = "{var:2}%{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -2, __LINE__);

    //////////////////

    content = "{var:3}%{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -1, __LINE__);

    content = "{var:3}%{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:3}%{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -1, __LINE__);

    content = "{var:3}%{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:3}%{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -3, __LINE__);

    content = "{var:3}%{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -3, __LINE__);

    //////////

    content = "{var:4}%{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:4}%{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:4}%{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:4}%{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:4}%{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:4}%{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    //////////

    content = "{var:5}%{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:5}%{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -1, __LINE__);

    content = "{var:5}%{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:5}%{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -1, __LINE__);

    content = "{var:5}%{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:5}%{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    ////////////////////////////////////////

    content = "{var:1}%{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:2}%{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:3}%{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -1, __LINE__);

    content = "{var:4}%{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:5}%{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    //////////////////

    content = "{var:0}%{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 2ULL, __LINE__);

    content = "{var:2}%{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -2, __LINE__);

    content = "{var:3}%{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:4}%{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:5}%{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -1, __LINE__);

    //////////////////

    content = "{var:0}%{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:1}%{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:3}%{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -1, __LINE__);

    content = "{var:4}%{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:5}%{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    //////////////////

    content = "{var:0}%{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 2ULL, __LINE__);

    content = "{var:1}%{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:2}%{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -2, __LINE__);

    content = "{var:4}%{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:5}%{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -1, __LINE__);

    //////////

    content = "{var:0}%{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 2ULL, __LINE__);

    content = "{var:1}%{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 3ULL, __LINE__);

    content = "{var:2}%{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -2, __LINE__);

    content = "{var:3}%{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -3, __LINE__);

    content = "{var:5}%{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    //////////

    content = "{var:0}%{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 2ULL, __LINE__);

    content = "{var:1}%{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 3ULL, __LINE__);

    content = "{var:2}%{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -2, __LINE__);

    content = "{var:3}%{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -3, __LINE__);

    content = "{var:4}%{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);
}

static void TestEvaluate19(QTest &helper) {
    Value<char> value;
    QNumber64   number{-1};
    const char *content;

    value += 2U;
    value += 3U;
    value += -2;
    value += int{2};
    value += 4.0F;
    value += -4.0F;
    value += 0.25F;
    value += -3.0F;
    value += 3.0F;
    value += 12.0F;

    content = "{var:6}^{var:1}";
    helper.IsFalse(TestEvaluate(number, content, value), __LINE__);

    content = "{var:0}^{var:0}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 4ULL, __LINE__);

    content = "{var:2}^{var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -8, __LINE__);

    content = "{var:0}^{var:2}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 0.25, __LINE__);

    content = "{var:0}^{var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 4ULL, __LINE__);

    content = "{var:5}^{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 256ULL, __LINE__);

    content = "{var:7}^{var:8}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Integer, -27, __LINE__);

    content = "{var:4}^{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 0.00390625, __LINE__);

    content = "{var:7}^{var:5}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, -0.012345679012345678, __LINE__);

    content = "{var:9}/{var:8}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Real, 4.0, __LINE__);

    content = "{var:3}>={var:8}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:3}<={var:8}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:3}<{var:8}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:3}>{var:8}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:5}<{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:2}=={var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:1}>{var:8}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:8}>{var:4}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 0ULL, __LINE__);

    content = "{var:4}!={var:1}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);

    content = "{var:4}!={var:3}";
    number  = TestEvaluate(content, value);
    helper.IsEqual(number.Natural, 1ULL, __LINE__);
}

static int RunEvaluateTests() {
    QTest helper{"Evaluate.hpp", __FILE__};

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
    helper.Test("Evaluate Test 17", TestEvaluate17);
    helper.Test("Evaluate Test 18", TestEvaluate18);
    helper.Test("Evaluate Test 19", TestEvaluate19);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
