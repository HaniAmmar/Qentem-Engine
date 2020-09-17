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
#include "TestHelper.hpp"

#ifndef QENTEM_ALE_TESTS_H_
#define QENTEM_ALE_TESTS_H_

namespace Qentem {
namespace Test {

struct aleHelper {
    static bool ALESetNumber(double &number, const char *content, UInt length) {
        static const char *   a_val     = "{A}";
        static constexpr UInt a_val_len = 3;

        static const char *   ab_val     = "{AB}";
        static constexpr UInt ab_val_len = 4;

        static const char *   abc_val     = "{ABC}";
        static constexpr UInt abc_val_len = 5;

        if ((a_val_len == length) && Memory::IsEqual(a_val, content, length)) {
            number = 6;
            return true;
        }

        if ((ab_val_len == length) &&
            Memory::IsEqual(ab_val, content, length)) {
            number = 13;
            return true;
        }

        if ((abc_val_len == length) &&
            Memory::IsEqual(abc_val, content, length)) {
            number = 26;
            return true;
        }

        return false;
    }

    static bool ALEIsEqual(bool &result, const char *left, UInt left_length,
                           const char *right, UInt right_length) {
        static const char *a_str = "{1}";
        static const ULong a_len = 3;

        static const char *ab_str = "{20}";
        static const ULong ab_len = 4;

        static const char *a_val     = "Qentem";
        static const ULong a_val_len = 6;

        static const char *ab_val     = "Qentem ALE";
        static const ULong ab_val_len = 10;

        const char *str_left;
        const char *str_right;
        ULong       str_left_length;
        ULong       str_right_length;

        if (left[0] == '{') {
            if ((left_length == a_len) && Memory::IsEqual(left, a_str, a_len)) {
                str_left        = a_val;
                str_left_length = a_val_len;
            } else if ((left_length == ab_len) &&
                       Memory::IsEqual(left, ab_str, ab_len)) {
                str_left        = ab_val;
                str_left_length = ab_val_len;
            } else {
                return false;
            }
        } else {
            str_left        = left;
            str_left_length = left_length;
        }

        if (right[0] == '{') {
            if ((right_length == a_len) &&
                Memory::IsEqual(right, a_str, a_len)) {
                str_right        = a_val;
                str_right_length = a_val_len;
            } else if ((right_length == ab_len) &&
                       Memory::IsEqual(right, ab_str, ab_len)) {
                str_right        = ab_val;
                str_right_length = ab_val_len;
            } else {
                return false;
            }
        } else {
            str_right        = right;
            str_right_length = right_length;
        }

        result = ((str_left_length == str_right_length) &&
                  Memory::IsEqual(str_right, str_left, str_right_length));

        return true;
    }
};

static int TestALE1() {
    double      number;
    const char *equation;
    bool        is_valid;

    equation = "";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = " ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "  ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "               ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "()";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "( )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(  )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(        )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = " ()";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = " ( )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = " (  )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = " (        )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "() ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "( ) ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(  ) ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(        ) ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "  ()";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "  ( )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "  (  )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "  (        )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "()  ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "( )  ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(  )  ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(        )  ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = " () ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = " ( ) ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = " (  ) ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = " (        ) ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "   ()   ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "   ( )   ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "   (  )   ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "   (        )   ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "a";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "10/0";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "10/(2-2)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "fj";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "fgh";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "5f2s4s5dg";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(d)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(oo)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(www46fs5d4f5ds)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "() ()";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(())";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "() (()) ()";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(()) (()()) ((()())())";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    // NOTE: Implement power of fraction.
    equation = "2^0.2";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE2() {
    double      number;
    const char *equation;
    bool        is_valid;

    equation = "1";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " 4";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 4, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "5 ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " 10 ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 10, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " 7 ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 7, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "100";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 100, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " 40";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 40, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "5000 ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 5000, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " 1004 ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 1004, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " 714 ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 714, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(1)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " (4)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 4, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(5) ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " (10) ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 10, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " (7) ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 7, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(100)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 100, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " (40)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 40, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(5000) ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 5000, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " (1004) ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 1004, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " ( 714 ) ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 714, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "((1))";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " (( 4))";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 4, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "((5     ) )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "( 10 )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 10, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "( (((       7   ))) )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 7, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(100)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 100, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "( 40)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 40, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(5000 )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 5000, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(((((((((( 1004 ))))))))))";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 1004, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "( 714 )";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 714, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "0^10";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE3() {
    double      number;
    const char *equation;
    bool        is_valid;

    equation = "3^2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 9, "number");

    equation = "7%5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 2, "number");

    equation = "3*6";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 18, "number");

    equation = "10/2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");

    equation = "3+5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 8, "number");

    equation = "5-2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 3, "number");

    equation = "3>2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "2>2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");

    equation = "5==2";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "5==5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "5!=2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "10!=10";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "1>5";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "5>=1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "8>=8";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "5>=7";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "4<8";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7<7";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "10<9";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "3<=16";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "3<=3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "17<=3";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "1&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&800";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0&&5";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "0&&0";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "8||6";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "4||0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||9";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||0";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "   3   ^2   ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 9, "number");

    equation = "   7%   5   ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 2, "number");

    equation = "   3   *   6";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 18, "number");

    equation = "10   /   2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");

    equation = "3   +   5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 8, "number");

    equation = " 5 == 2 ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " 5 == 5 ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " 5 != 2 ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " 10 != 10 ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " 5 - 2 ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 3, "number");

    equation = "   3 > 2   ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   2   >2";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "1>   5   ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " 5   >=1   ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   8>=   8 ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "5   >=   7";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "   4   <8   ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   7<   7   ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "   10<   0";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "   3   <=   16";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   3   <=   3     ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   17   <=3";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "   1   &&   1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   1   &&   800    ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   0&&5  ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "   0   &&0";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "   8   ||   6  ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "4   ||   0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   0||9";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   0   ||0";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE4() {
    double      number;
    const char *equation;
    bool        is_valid;

    equation = "16 ^ -2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 0.00390625, "number");

    equation = "8 ^-2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 0.015625, "number");

    equation = "-3^0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "5564685^0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-3^2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 9, "number");

    equation = "-8^-2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 0.015625, "number");

    equation = "-4 * 5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -20, "number");

    equation = "5 *-6";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -30, "number");

    equation = "-3*-4";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 12, "number");

    equation = "-10 / 2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -5, "number");

    equation = "7 /-2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -3.5, "number");

    equation = "-9 / -3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 3, "number");

    equation = "-7 + 5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -2, "number");

    equation = "-11 + -2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -13, "number");

    equation = "-8+-3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -11, "number");

    equation = "5 --2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 7, "number");

    equation = "-8-3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -11, "number");

    equation = "5 ---2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 3, "number");

    equation = "---5 ---2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -7, "number");

    equation = " 5 == - 5 ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " 5 == -5 ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " 5 ==-5 ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " -5 == 5 ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " - 5 == 5 ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " -5 != 2 ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " 10 != -10 ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " -10 != 10 ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " -  10 != 10 ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " 10 !=-10 ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " 10 !=- 10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "3 >-2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "10 > -10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-2 > 2";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "-2 > -11";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-1>5";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "5 >=-19";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-1 >=- 100";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-8 >= -   8";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-5 >= 7";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "3 <-2";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "10 < -10";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "-2 < 2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-2 < -11";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "-1<5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "5 <=-19";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "5 - <=-19";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "-5 <=- 19";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "-8 <= -   8";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-5 <= 7";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-1 && 1";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "1 &&- 800";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "0 && -5";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "-8 && -9";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "-8 || 6";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "4 ||-9";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0 || - 9";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "-7 || -500";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE5() {
    double      number;
    const char *equation;
    bool        is_valid;

    equation = "16 ^ +2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 256, "number");

    equation = "8 ^+2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 64, "number");

    equation = "+3^0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "5564685^0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "+3^2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 9, "number");

    equation = "+3^+3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 27, "number");

    equation = "+4 * 5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 20, "number");

    equation = "5 *+6";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 30, "number");

    equation = "+3*+4";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 12, "number");

    equation = "+10 / 2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");

    equation = "7 /+2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 3.5, "number");

    equation = "+9 / +3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 3, "number");

    equation = "+7 - 5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 2, "number");

    equation = "+11 - +2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 9, "number");

    equation = "+8-+3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");

    equation = "5 ++2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 7, "number");

    equation = "+8+3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, +11, "number");

    equation = "5 +++2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 7, "number");

    equation = "+++5 +++2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 7, "number");

    equation = "+5==2";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "5==+2";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "+5==+2";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "+5==+5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "+5!=+2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "+10!=+10";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "+10!=10";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "10!=+10";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "3 >+2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "10 > +10";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "+2 > 2";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "+11 > +2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "+1>5";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "+19 >=5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "+1 >=+ 100";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "+8 >= +   8";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7+ >= 5";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "3 <+2";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "10 < +10";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "+1 < 2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "+2 < +11";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "+1<5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "+19 <=5";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "+5 <=+ 19";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "+8 <= +   8";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "+5 <= 7";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "+1 && 1";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "1 &&+ 800";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "0 && +5";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "+8 && +9";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "+8 || 6";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "4 ||+9";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation =
        R"(4                                                                      ||1
                                                                                       ||
                                                                                             2)";
    number = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0 || + 9";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "+7 || +500";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE6() {
    double      number = -1;
    const char *equation;
    bool        is_valid;

    equation = "(0)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(+1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(-1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -1, "number");

    equation = "( 1 )";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "( +1 )";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "( -1 )";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -1, "number");

    equation = "(1 )";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(+1 )";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(-1 )";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -1, "number");

    equation = "( 1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "( +1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "( -1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -1, "number");

    equation = "(       1            )";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(       +      1             )";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(        -       1           )";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -1, "number");

    equation = "(2*3)^4";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1296, "number");

    equation = "(8*3) %7";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 3, "number");

    equation = "(6+2)  *6";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 48, "number");

    equation = " (15-5)  /5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 2, "number");

    equation = "   (4*3)   +  5  ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 17, "number");

    equation = "(18/3)- 2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 4, "number");

    equation = "(13%5)           >2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(3                  ^-2)>2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");

    equation = "(1+2)== 2";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(6-1) ==5";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " (3^2)!=2 ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(20/2) != 10";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(  10  -  +  9  )>5";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(15%10)>=1             ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "          (16/2)>=8";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(4*1.5)>=7";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(0.999999+0.1)<8";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(20*0.5)<7";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(4^2)<9";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(9/2)<=16";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1.5*2)<=3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(20-8)<=3";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(1*1)&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&1)&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(100-5)&&800";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0*10)&&5";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(8%2)&&0";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(4/2)||6";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1||0)||0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(9%3)||9";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0||0)||0";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE7() {
    double      number = -1;
    const char *equation;
    bool        is_valid;

    equation = "6^(8/2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1296, "number");

    equation = "24 %(3.5*2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 3, "number");

    equation = "8  *(12/2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 48, "number");

    equation = " 10  /(7-2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 2, "number");

    equation = "   12   +  (2.5*2)  ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 17, "number");

    equation = "6- (10/5)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 4, "number");

    equation = "(3)           >(2*1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0.9999999999>(4-2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");

    equation = "3== (3-1)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(5) ==(10-5)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " 9!=(1+1) ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "10 != (20/2)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "1>(3+2)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "5>=(100^0)             ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " 8>=(2^3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "6>=(14/2)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "1<(4+2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "10<(10-3)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "16<(18/2)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "4.5<=(4*4)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "3<=(1.5+1.5)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "12<=(3*1)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "1&&(1^10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "10&&(10^1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7&&(10-5)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0&&(2+2)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "0&&(0^0)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "9||        (3*2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "        1||(0*10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0          ||(3*3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "        0||(10&&0)          ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(2*3)^(8/2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1296, "number");

    equation = "(8*3) %(3.5*2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 3, "number");

    equation = "(6+2)  *(12/2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 48, "number");

    equation = " (15-5)  /(7-2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 2, "number");

    equation = "   (4*3)   +  (2.5*2)  ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 17, "number");

    equation = "(18/3)- (10/5)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 4, "number");

    equation = "(13%5)           >(2*1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(3                  ^-2)>(4-2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");

    equation = "(1+2)== (3-1)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(6-1) ==(10-5)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " (3^2)!=(1+1) ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(20/2) != (20/2)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(  10  -  +  9  )>(3+2)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(15%10)>=(100^0)             ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "          (16/2)>=(2^3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(4*1.5)>=(14/2)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(0.999999+0.1)<(4+2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(20*0.5)<(10-3)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(4^2)<(18/2)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(9/2)<=(4*4)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1.5*2)<=(1.5+1.5)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(20-8)<=(3*1)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(1*1)&&(1^10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&1)&&(10^1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(100-5)&&(10-5)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0*10)&&(2+2)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(8%2)&&(0^0)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(4/2)||(3*2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1||0)||(0*10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(9%3)||(3*3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0||0)||(10&&0)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE8() {
    double      number = -1;
    const char *equation;
    bool        is_valid;

    equation = "3*2^3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 24, "number");

    equation = "3*10%7";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 9, "number");

    equation = "135/3^3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");

    equation = "18/10%7";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 6, "number");

    equation = "4^2*2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 32, "number");

    equation = "10%7*2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 6, "number");

    equation = "3^3/4";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 6.75, "number");

    equation = "10%4/2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "4+2*2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 8, "number");

    equation = "10-7*2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -4, "number");

    equation = "3+16/4";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 7, "number");

    equation = "10-6/3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 8, "number");

    equation = "2*8-2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 14, "number");

    equation = "3*4-20";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -8, "number");

    equation = "14/7+3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");

    equation = "9/3-4";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -1, "number");

    equation = "3+4==7";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7-8==-1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "8-2!=2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7+3!=3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7==3+4";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-1==7-8";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "8!=8-2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7!=7+3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7!=10-3";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "7-10!=-3";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "3+4>=7";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "3-8>-10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "8-2<10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7+3<=10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7<=3+4";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-1<7+8";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "8>8-2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7>=10-3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&1==1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&10==10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&5!=1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&7!=1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0==0&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1==1&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1!=0&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0!=1&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1||10==1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||10==10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1||5!=1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||7!=1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0==0||1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1==1||0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1!=0||1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0!=1||0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&1<=1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&3<10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&5>1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&0>=0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0<=0&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1<2&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1>0&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "10>=5&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1||10<=1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||1<10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1||5>=10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||7>1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0>=0||1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "4>3||0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1<0||1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0<1||0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    END_SUB_TEST;
}

static int TestALE9() {
    double      number = -1;
    const char *equation;
    bool        is_valid;

    equation = "(3*2^3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 24, "number");

    equation = "(3*10%7)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 9, "number");

    equation = "(135/3^3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");

    equation = "(18/10%7)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 6, "number");

    equation = "(4^2*2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 32, "number");

    equation = "(10%7*2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 6, "number");

    equation = "(3^3/4)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 6.75, "number");

    equation = "(10%4/2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(4+2*2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 8, "number");

    equation = "(10-7*2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -4, "number");

    equation = "(3+16/4)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 7, "number");

    equation = "(10-6/3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 8, "number");

    equation = "(2*8-2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 14, "number");

    equation = "(3*4-20)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -8, "number");

    equation = "(14/7+3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");

    equation = "(9/3-4)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -1, "number");

    equation = "(3+4==7)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7-8==-1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(8-2!=2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7+3!=3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7==3+4)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(-1==7-8)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(8!=8-2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7!=7+3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7!=10-3)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(7-10!=-3)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(3+4>=7)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(3-8>-10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(8-2<10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7+3<=10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7<=3+4)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(-1<7+8)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(8>8-2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7>=10-3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&1==1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&10==10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&5!=1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&7!=1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0==0&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1==1&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1!=0&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0!=1&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1||10==1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0||10==10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1||5!=1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0||7!=1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0==0||1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1==1||0)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1!=0||1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0!=1||0)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&1<=1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&3<10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&5>1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&0>=0)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0<=0&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1<2&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1>0&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(10>=5&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1||10<=1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0||1<10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1||5>=10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0||7>1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0>=0||1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(4>3||0)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1<0||1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0<1||0)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    END_SUB_TEST;
}

static int TestALE10() {
    double      number = -1;
    const char *equation;
    bool        is_valid;

    equation = "3*(2)^3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 24, "number");

    equation = "3*(5+5)%7";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 9, "number");

    equation = "135/(5-2)^3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");

    equation = "18/(5*2)%7";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 6, "number");

    equation = "4^(4/2)*2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 32, "number");

    equation = "10%(3.5*2)*2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 6, "number");

    equation = "3^(1.5*2)/4";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 6.75, "number");

    equation = "10%(8-4)/2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "4+(8/4)*2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 8, "number");

    equation = "10-(14/2)*2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -4, "number");

    equation = "3+(4+12)/4";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 7, "number");

    equation = "10-(3+2+1)/3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 8, "number");

    equation = "2*(2+1+5)-2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 14, "number");

    equation = "3*(8-4)-20";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -8, "number");

    equation = "14/(10-3)+3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");

    equation = "9/(1+1+1)-4";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -1, "number");

    equation = "3+(2*2)==7";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7-(10-2)==-1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "8-(1+1)!=2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7+(9/3)!=3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7==(12/4)+4";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-1==(2+5)-8";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "8!=(2*2)-2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7!=(1+5+1)+3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7!=(10)-3";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "7-(10)!=-3";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "3+(4)>=7";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "3-(8)>-10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "8-(2)<10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7+(3)<=10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7<=(3)+4";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-1<(7)+8";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "8>(8)-2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7>=(10)-3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&(10-9)==1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&(5+5)==10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&(10/2)!=1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&(4+4)!=1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0==(0/4)&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1==(4^0)&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1!=(3-3)&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0!=(4-1-2)&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1||(2%2)==1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||(1==1)==1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1||(15%10)!=1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||(-4)!=1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0==(0+0)||1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1==(100-99)||0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1!=(4%5)||1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0!=(1^1)||0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&(10%9)<=1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&(2+1)<10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&(2.5+2.5)>1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&(8%2)>=0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0<=(0)&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1<(8-1)&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1>(4-8)&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "10>=(100/10)&&1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1||(3+3)<=1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||(6%6)<10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1||(5^2)>=10";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||(7-2)>1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0>=(0&&1)||1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "4>(2&&1)||0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1<(4-4)||1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0<(4^0)||0";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    END_SUB_TEST;
}

static int TestALE11() {
    double      number = -1;
    const char *equation;
    bool        is_valid;

    equation = "(3*(2)^3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 24, "number");

    equation = "(3*(5+5)%7)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 9, "number");

    equation = "(135/(5-2)^3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");

    equation = "(18/(5*2)%7)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 6, "number");

    equation = "(4^(4/2)*2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 32, "number");

    equation = "(10%(3.5*2)*2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 6, "number");

    equation = "(3^(1.5*2)/4)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 6.75, "number");

    equation = "(10%(8-4)/2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(4+(8/4)*2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 8, "number");

    equation = "(10-(14/2)*2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -4, "number");

    equation = "(3+(4+12)/4)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 7, "number");

    equation = "(10-(3+2+1)/3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 8, "number");

    equation = "(2*(2+1+5)-2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 14, "number");

    equation = "(3*(8-4)-20)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -8, "number");

    equation = "(14/(10-3)+3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 5, "number");

    equation = "(9/(1+1+1)-4)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, -1, "number");

    equation = "(3+(2*2)==7)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7-(10-2)==-1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(8-(1+1)!=2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7+(9/3)!=3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7==(12/4)+4)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(-1==(2+5)-8)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(8!=(2*2)-2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7!=(1+5+1)+3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7!=(10)-3)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(7-(10)!=-3)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(3+(4)>=7)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(3-(8)>-10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(8-(2)<10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7+(3)<=10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7<=(3)+4)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(-1<(7)+8)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(8>(8)-2)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7>=(10)-3)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&(10-9)==1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&(5+5)==10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&(10/2)!=1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&(4+4)!=1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0==(0/4)&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1==(4^0)&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1!=(3-3)&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0!=(4-1-2)&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1||(2%2)==1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0||(1==1)==1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1||(15%10)!=1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0||(-4)!=1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0==(0+0)||1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1==(100-99)||0)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1!=(4%5)||1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0!=(1^1)||0)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&(10%9)<=1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&(2+1)<10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&(2.5+2.5)>1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&(8%2)>=0)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0<=(0)&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1<(8-1)&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1>(4-8)&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(10>=(100/10)&&1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1||(3+3)<=1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0||(6%6)<10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1||(5^2)>=10)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0||(7-2)>1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0>=(0&&1)||1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(4>(2&&1)||0)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1<(4-4)||1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(0<(4^0)||0)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "((1+2)^(1+2))/2";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 13.5, "number");

    equation = "((5/5+1)*2+1)+3*3";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 14, "number");

    equation = "5+2*4-8/2==9 && 1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "((5/5+1)*2+1)+3*3 != 12 && 1";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation =
        R"(2  * 1 * 3 + 1 - 4 + (10 - 5 - 6 + 1 + 1 + 1 + 1) * (8 / 4 + 1) -
        1 - -1 + 2 == 14)";
    number = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "((2*4) + 1)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 9, "number");

    equation = "((2* (2 + 3)) + 1 - 4)";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 7, "number");

    equation =
        R"(((2* (1 * 3)) + 1 - 4) + (((10 - 5) -
        6 + ((1 + 1) + (1 + 1))) * (8 / 4 + 1)) -
        (1) - (-1) + 2)";
    number = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 14, "number");

    END_SUB_TEST;
}

static int TestALE12() {
    double      number = -1;
    const char *equation;
    bool        is_valid;

    equation = "a==a";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "a==c";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "a!=b";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "a!=a";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "a == a";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "a == c";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "a != b";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "a != a";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " a == a ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " a == c ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " a != b ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " a != a ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "  a    == a";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "  a    == a          ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "      a==a";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "      a             ==a";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "a==         a              ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   a      == c         ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "   a !=         b";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   a!=a      ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "AbcDEF==AbcDEF";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "AbcDEF==zwx";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "AbcDEF!=BBBB";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "AbcDEF!=AbcDEF";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "AbcDEF == AbcDEF";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "AbcDEF == zwx";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "AbcDEF != BBBB";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "AbcDEF != AbcDEF";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " AbcDEF == AbcDEF ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " AbcDEF == zwx ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = " AbcDEF != BBBB ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " AbcDEF != AbcDEF ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "  AbcDEF    == AbcDEF";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "  AbcDEF    == AbcDEF          ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "      AbcDEF==AbcDEF";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "      AbcDEF             ==AbcDEF";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "AbcDEF==         AbcDEF              ";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   AbcDEF      == zwx         ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "   AbcDEF !=         BBBB";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   AbcDEF!=AbcDEF      ";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE13() {
    double      number = -1;
    const char *equation;
    bool        is_valid;

    equation = "a==a";
    number   = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "{1} + 5";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "3 + {9}";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{A} == A";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{A} != A";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "A=={A}";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "A==";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "A!={A}";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(4+3";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "4+3*(3";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(4+(2)";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{4+5";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    number = ALE::Evaluate(equation);
    SHOULD_EQUAL_VALUE(number, 0, "number");

    aleHelper ale;

    equation = "{A}+10";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 16, "number");

    equation = "{AB}-3";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 10, "number");

    equation = "{ABC}*    2";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 52, "number");

    equation = "{A}    /2";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 3, "number");

    equation = "   {A}^2";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 36, "number");

    equation = "{AB}  %  5";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 3, "number");

    equation = "{ABC}>7";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   {AB} <6";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{A}  >=4";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "{ABC}<=30";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "{AB} && 0  ";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{A} && 1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "{ABC} || 0";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   {AB} || 1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "10+{A}";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 16, "number");

    equation = "3+{AB}";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 16, "number");

    equation = "2*    {ABC}";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 52, "number");

    equation = " 104   /{ABC}";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 4, "number");

    equation = " 2  ^{A}";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 64, "number");

    equation = "35 &&  {AB} ";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7<{ABC}";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "   8 >{AB}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "3  <={A}";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "26>={ABC}";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0 && {AB}  ";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "1 && {A}";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0 || {ABC}";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = " 1   || {AB}";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "{d} +5  ";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{w}*0  ";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{}&&0";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE14() {
    double      number = -1;
    const char *equation;
    bool        is_valid;
    aleHelper   ale;

    equation = "3*{A}^3";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 648, "number");

    equation = "3*{AB}%7";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 18, "number");

    equation = "135/{A}^3";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0.625, "number");

    equation = "54/{ABC}%10";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 9, "number");

    equation = "4^{A}*2";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 8192, "number");

    equation = "10%({A})*2";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 8, "number");

    equation = "3^{A}/(4)";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 182.25, "number");

    equation = "(10)%{AB}/2";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 5, "number");

    equation = "4+({A})*2";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 16, "number");

    equation = "10-({AB})*2";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, -16, "number");

    equation = "3+{ABC}/(4)";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 9.5, "number");

    equation = "(10)-{A}/3";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 8, "number");

    equation = "2*{ABC}-2";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 50, "number");

    equation = "(3*{A}-20)";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, -2, "number");

    equation = "(65/{AB}+3)";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 8, "number");

    equation = "52/{ABC}-4";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, -2, "number");

    equation = "(3+{ABC}==29)";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7-{AB}==-6";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(8-{AB})!=2";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7+{ABC}!=3";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "10==({A}+4)";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-1==({A})-7";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "8!=({ABC}-2)";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7!=({AB}+3)";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7!=({AB}-6)";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(4-{A})!=-2";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "3+{ABC}>=7";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "3-{A}>-10";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "8-{A}<10";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7+{ABC}<=40";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7<={AB}+4";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "-1<{ABC}+8";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "8>{A}-2";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "7>={A}-3";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&({ABC})==26";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&({AB})==13";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1&&{A})!=61";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&({ABC})!=126";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0==(0/{AB})&&1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1==({A}^0)&&1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1!=({ABC}))&&1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0!=({A}&&1)";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1||({AB})==1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||{1}==Qentem";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1||{1}!=Qentem";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||{1}!=Qen";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1!={1}||0";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "6==({A})||0";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "Qentem!={20}||1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0!=({AB})||0";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&{A}<=6";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&{AB}<27";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&{ABC}>1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1&&{A}>=0";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0<={AB}&&1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1<{A}&&1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1>{ABC}&&1";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "10>={A}&&1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1||({AB}<=1)";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||{AB}<14";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(1||{A}>=10)";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0||{ABC}>1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0>={AB}||1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "(7>{A}||0)";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "1<{AB}||1";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    equation = "0<{ABC}||0";
    number   = ALE::Evaluate(equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");

    END_SUB_TEST;
}

static int TestALE15() {
    double      number = -1;
    const char *equation;
    bool        is_valid;
    aleHelper   ale;

    equation = "{1}   ==Qentem";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{20}==     Qentem ALE";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "        {1}   ==Qentem";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "             {20}==     Qentem ALE";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{1}==          Qentem";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{20}==Qentem ALE        ";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{1}==    Qentem          ";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "  {20}    ==     Qentem ALE     ";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{10}==Qentem";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{1}!=Qentem";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{20}!=Qentem ALE";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{2}!=Qentem ALE";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{1}==Qente";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{20}==ALE";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{1}!=Qente";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{20}!=ALE";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "Qentem   =={1}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "Qentem ALE==     {20}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "        Qentem   =={1}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "             Qentem ALE==     {20}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "Qentem==          {1}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "Qentem ALE=={20}        ";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "Qentem==    {1}          ";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "  Qentem ALE    ==     {20}     ";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "Qentem=={10}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "Qentem!={1}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "Qentem ALE!={20}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "Qentem ALE!={2}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "Qente=={1}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "ALE=={20}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 0, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "Qente!={1}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    equation = "ALE!={20}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_EQUAL_VALUE(number, 1, "number");
    SHOULD_EQUAL_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE16() {
    double      number = -1;
    const char *equation;
    bool        is_valid;
    aleHelper   ale;

    equation = "2      {A}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "7{ABC}";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{AB}3";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{AB} 9";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "{AB}  &  5";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "(3)3";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "3(3)";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "4+3(7)";
    is_valid = ALE::Evaluate(number, equation, &ale);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "1!#1";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    equation = "1|#1";
    is_valid = ALE::Evaluate(number, equation);
    SHOULD_NOT_EQUAL_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int RunALETests() {
    STARTING_TEST("ALE.hpp");

    START_TEST("ALE Test 1", TestALE1);
    START_TEST("ALE Test 2", TestALE2);
    START_TEST("ALE Test 3", TestALE3);
    START_TEST("ALE Test 4", TestALE4);
    START_TEST("ALE Test 5", TestALE5);
    START_TEST("ALE Test 6", TestALE6);
    START_TEST("ALE Test 7", TestALE7);
    START_TEST("ALE Test 8", TestALE8);
    START_TEST("ALE Test 9", TestALE9);
    START_TEST("ALE Test 10", TestALE10);
    START_TEST("ALE Test 11", TestALE11);
    START_TEST("ALE Test 12", TestALE12);
    START_TEST("ALE Test 13", TestALE13);
    START_TEST("ALE Test 14", TestALE14);
    START_TEST("ALE Test 15", TestALE15);
    START_TEST("ALE Test 15", TestALE16);

    END_TEST("ALE.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
