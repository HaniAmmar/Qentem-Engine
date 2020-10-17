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
#include "Array.hpp"
#include "TestHelper.hpp"

#ifndef QENTEM_ALE_TESTS_H_
#define QENTEM_ALE_TESTS_H_

namespace Qentem {
namespace Test {

struct aleHelper {
    // See class Template_T_ for another example.

    struct Value_T_ {
        const char *Name;
        const SizeT NameLength;

        // Number Value
        const SizeT Number;

        // String Value
        const char *Str;
        const SizeT StrLength;
    };

    static Array<Value_T_> &GetItems() {
        static Array<Value_T_> items;

        if (items.IsEmpty()) {
            items += {"{1}", 3, 6, nullptr, 0};
            items += {"{A}", 3, 6, nullptr, 0};
            items += {"{AB}", 4, 13, nullptr, 0};
            items += {"{ABC}", 5, 26, nullptr, 0};
            items += {"{Q}", 3, 0, "Qentem", 6};
            items += {"{QA}", 4, 0, "Qentem ALE", 10};
        }

        return items;
    }
    static bool FindItem(Value_T_ *&item, const char *content, SizeT length) {
        static const Array<Value_T_> &items = GetItems();

        for (SizeT i = 0; i < items.Size(); i++) {
            item = &(items[i]);

            if ((item->NameLength == length) &&
                StringUtils::IsEqual(item->Name, content, length)) {
                return true;
            }
        }

        return false;
    }

    bool ALESetNumber(double &number, const char *content,
                      SizeT length) const noexcept {
        Value_T_ *item;

        if (FindItem(item, content, length) && (item->Str == nullptr)) {
            number = item->Number;
            return true;
        }

        return false;
    }

    bool ALEIsEqual(bool &result, const char *content, ALE::Number left,
                    ALE::Number right, bool left_evaluated,
                    bool right_evaluated) const noexcept {
        const char *left_content  = (content + left.Content.Offset);
        const char *right_content = (content + right.Content.Offset);
        Value_T_ *  item;
        SizeT       left_length  = left.Content.Length;
        SizeT       right_length = right.Content.Length;

        bool is_number = (left_evaluated || right_evaluated);

        if (!left_evaluated) {
            if (*left_content == ALEOperations<char>::BracketStart) {
                if (!FindItem(item, left_content, left_length)) {
                    return false;
                }

                if (item->Str == nullptr) {
                    left.Number = item->Number;
                    is_number   = true;
                } else {
                    left_content = item->Str;
                    left_length  = item->StrLength;
                }
            } else {
                if (*left_content == ALEOperations<char>::ParenthesStart) {
                    ++left_content;
                    left_length -= 2;
                }

                left_evaluated =
                    ALE::Evaluate(left.Number, left_content, left_length, this);
                if (!left_evaluated && is_number) {
                    return false;
                }

                is_number = left_evaluated;
            }
        }

        if (!right_evaluated) {
            if (*right_content == ALEOperations<char>::BracketStart) {
                if (!FindItem(item, right_content, right_length)) {
                    return false;
                }

                if (is_number && (item->Str != nullptr)) {
                    return false;
                }

                if (item->Str == nullptr) {
                    right.Number = item->Number;
                } else {
                    right_content = item->Str;
                    right_length  = item->StrLength;
                }

            } else if (is_number) {
                if (*right_content == ALEOperations<char>::ParenthesStart) {
                    ++right_content;
                    right_length -= 2;
                }

                if (!(ALE::Evaluate(right.Number, right_content, right_length,
                                    this))) {
                    return false;
                }
            }
        }

        if (is_number) {
            result = (left.Number == right.Number);
        } else {
            result = ((left_length == right_length) &&
                      StringUtils::IsEqual(left_content, right_content,
                                           right_length));
        }

        return true;
    }
};

static int TestALE1() {
    double      number;
    const char *content;
    bool        is_valid;

    content  = "";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = " ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "  ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "               ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "()";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "( )";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(  )";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(        )";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = " ()";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = " ( )";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = " (  )";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = " (        )";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "() ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "( ) ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(  ) ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(        ) ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "  ()";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "  ( )";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "  (  )";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "  (        )";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "()  ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "( )  ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(  )  ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(        )  ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = " () ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = " ( ) ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = " (  ) ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = " (        ) ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "   ()   ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "   ( )   ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "   (  )   ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "   (        )   ";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "a";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "10/0";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "10/(2-2)";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "fj";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "fgh";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "5f2s4s5dg";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(d)";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(oo)";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(www46fs5d4f5ds)";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "() ()";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(())";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "() (()) ()";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(()) (()()) ((()())())";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE2() {
    double      number;
    const char *content;
    bool        is_valid;

    content  = "1";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    // NOTE: Implement power of fraction.
    content  = "2^0.2";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");
    // EQ_VALUE(number, 1.148698355, "number");
    // EQ_TRUE(is_valid, "is_valid");

    content  = " 4";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 4, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "5 ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 5, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " 10 ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 10, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " 7 ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 7, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "100";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 100, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " 40";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 40, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "5000 ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 5000, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " 1004 ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 1004, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " 714 ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 714, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(1)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " (4)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 4, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(5) ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 5, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " (10) ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 10, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " (7) ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 7, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(100)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 100, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " (40)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 40, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(5000) ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 5000, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " (1004) ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 1004, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " ( 714 ) ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 714, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "((1))";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " (( 4))";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 4, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "((5     ) )";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 5, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "( 10 )";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 10, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "( (((       7   ))) )";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 7, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(100)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 100, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "( 40)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 40, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(5000 )";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 5000, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(((((((((( 1004 ))))))))))";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 1004, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "( 714 )";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 714, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "0^10";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE3() {
    double      number;
    const char *content;
    bool        is_valid;

    content = "3^2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 9, "number");

    content = "7%5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 2, "number");

    content = "3*6";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 18, "number");

    content = "10/2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 5, "number");

    content = "3+5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 8, "number");

    content = "5-2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 3, "number");

    content = "3>2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "2>2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 0, "number");

    content  = "5==2";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "5==5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "5!=2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "10!=10";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "1>5";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "5>=1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "8>=8";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "5>=7";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "4<8";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "7<7";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "10<9";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "3<=16";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "3<=3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "17<=3";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "1&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&800";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "0&&5";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "0&&0";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "8||6";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "4||0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0||9";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "0||0";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "   3   ^2   ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 9, "number");

    content = "   7%   5   ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 2, "number");

    content = "   3   *   6";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 18, "number");

    content = "10   /   2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 5, "number");

    content = "3   +   5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 8, "number");

    content  = " 5 == 2 ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = " 5 == 5 ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = " 5 != 2 ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = " 10 != 10 ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = " 5 - 2 ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 3, "number");

    content = "   3 > 2   ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "   2   >2";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "1>   5   ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = " 5   >=1   ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "   8>=   8 ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "5   >=   7";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "   4   <8   ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "   7<   7   ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "   10<   0";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "   3   <=   16";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "   3   <=   3     ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "   17   <=3";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "   1   &&   1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "   1   &&   800    ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "   0&&5  ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "   0   &&0";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "   8   ||   6  ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "4   ||   0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "   0||9";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "   0   ||0";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE4() {
    double      number;
    const char *content;
    bool        is_valid;

    content = "16 ^ -2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 0.00390625, "number");

    content = "8 ^-2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 0.015625, "number");

    content = "-3^0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "5564685^0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "-3^2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 9, "number");

    content = "-8^-2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 0.015625, "number");

    content = "-4 * 5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -20, "number");

    content = "5 *-6";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -30, "number");

    content = "-3*-4";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 12, "number");

    content = "-10 / 2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -5, "number");

    content = "7 /-2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -3.5, "number");

    content = "-9 / -3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 3, "number");

    content = "-7 + 5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -2, "number");

    content = "-11 + -2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -13, "number");

    content = "-8+-3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -11, "number");

    content = "5 --2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 7, "number");

    content = "-8-3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -11, "number");

    content = "5 ---2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 3, "number");

    content = "---5 ---2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -7, "number");

    content  = " 5 == - 5 ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " 5 == -5 ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " 5 ==-5 ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " -5 == 5 ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = " - 5 == 5 ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = " -5 != 2 ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = " 10 != -10 ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = " -10 != 10 ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = " -  10 != 10 ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = " 10 !=-10 ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = " 10 !=- 10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "3 >-2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "10 > -10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "-2 > 2";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "-2 > -11";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "-1>5";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "5 >=-19";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "-1 >=- 100";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "-8 >= -   8";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "-5 >= 7";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "3 <-2";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "10 < -10";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "-2 < 2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "-2 < -11";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "-1<5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "5 <=-19";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "5 - <=-19";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "-5 <=- 19";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "-8 <= -   8";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "-5 <= 7";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "-1 && 1";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "1 &&- 800";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "0 && -5";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "-8 && -9";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "-8 || 6";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "4 ||-9";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "0 || - 9";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "-7 || -500";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE5() {
    double      number;
    const char *content;
    bool        is_valid;

    content = "16 ^ +2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 256, "number");

    content = "8 ^+2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 64, "number");

    content = "+3^0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "5564685^0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "+3^2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 9, "number");

    content = "+3^+3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 27, "number");

    content = "+4 * 5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 20, "number");

    content = "5 *+6";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 30, "number");

    content = "+3*+4";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 12, "number");

    content = "+10 / 2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 5, "number");

    content = "7 /+2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 3.5, "number");

    content = "+9 / +3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 3, "number");

    content = "+7 - 5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 2, "number");

    content = "+11 - +2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 9, "number");

    content = "+8-+3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 5, "number");

    content = "5 ++2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 7, "number");

    content = "+8+3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, +11, "number");

    content = "5 +++2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 7, "number");

    content = "+++5 +++2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 7, "number");

    content  = "+5==2";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "5==+2";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "+5==+2";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "+5==+5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "+5!=+2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "+10!=+10";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "+10!=10";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "10!=+10";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "3 >+2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "10 > +10";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "+2 > 2";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "+11 > +2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "+1>5";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "+19 >=5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "+1 >=+ 100";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "+8 >= +   8";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "7+ >= 5";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "3 <+2";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "10 < +10";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "+1 < 2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "+2 < +11";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "+1<5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "+19 <=5";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "+5 <=+ 19";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "+8 <= +   8";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "+5 <= 7";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "+1 && 1";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "1 &&+ 800";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "0 && +5";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "+8 && +9";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "+8 || 6";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "4 ||+9";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content =
        R"(4                                                                      ||1
                                                                                       ||
                                                                                             2)";
    number = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "0 || + 9";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "+7 || +500";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE6() {
    double      number = -1;
    const char *content;
    bool        is_valid;

    content  = "(0)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(+1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(-1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -1, "number");

    content = "( 1 )";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "( +1 )";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "( -1 )";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -1, "number");

    content = "(1 )";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(+1 )";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(-1 )";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -1, "number");

    content = "( 1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "( +1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "( -1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -1, "number");

    content = "(       1            )";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(       +      1             )";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(        -       1           )";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -1, "number");

    content = "(2*3)^4";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1296, "number");

    content = "(8*3) %7";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 3, "number");

    content = "(6+2)  *6";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 48, "number");

    content = " (15-5)  /5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 2, "number");

    content = "   (4*3)   +  5  ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 17, "number");

    content = "(18/3)- 2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 4, "number");

    content = "(13%5)           >2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(3                  ^-2)>2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 0, "number");

    content  = "(1+2)== 2";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(6-1) ==5";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = " (3^2)!=2 ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(20/2) != 10";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(  10  -  +  9  )>5";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(15%10)>=1             ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "          (16/2)>=8";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(4*1.5)>=7";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(0.999999+0.1)<8";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(20*0.5)<7";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(4^2)<9";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(9/2)<=16";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1.5*2)<=3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(20-8)<=3";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(1*1)&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&1)&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(100-5)&&800";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(0*10)&&5";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(8%2)&&0";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(4/2)||6";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1||0)||0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(9%3)||9";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(0||0)||0";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE7() {
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "6^(8/2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1296, "number");

    content = "24 %(3.5*2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 3, "number");

    content = "8  *(12/2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 48, "number");

    content = " 10  /(7-2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 2, "number");

    content = "   12   +  (2.5*2)  ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 17, "number");

    content = "6- (10/5)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 4, "number");

    content = "(3)           >(2*1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0.9999999999>(4-2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 0, "number");

    content  = "3== (3-1)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(5) ==(10-5)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = " 9!=(1+1) ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "10 != (20/2)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "1>(3+2)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "5>=(100^0)             ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = " 8>=(2^3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "6>=(14/2)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "1<(4+2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "10<(10-3)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "16<(18/2)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "4.5<=(4*4)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "3<=(1.5+1.5)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "12<=(3*1)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "1&&(1^10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "10&&(10^1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7&&(10-5)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "0&&(2+2)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "0&&(0^0)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "9||        (3*2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "        1||(0*10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0          ||(3*3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "        0||(10&&0)          ";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(2*3)^(8/2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1296, "number");

    content = "(8*3) %(3.5*2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 3, "number");

    content = "(6+2)  *(12/2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 48, "number");

    content = " (15-5)  /(7-2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 2, "number");

    content = "   (4*3)   +  (2.5*2)  ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 17, "number");

    content = "(18/3)- (10/5)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 4, "number");

    content = "(13%5)           >(2*1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(3                  ^-2)>(4-2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 0, "number");

    content  = "(1+2)== (3-1)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(6-1) ==(10-5)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = " (3^2)!=(1+1) ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(20/2) != (20/2)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(  10  -  +  9  )>(3+2)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(15%10)>=(100^0)             ";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "          (16/2)>=(2^3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(4*1.5)>=(14/2)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(0.999999+0.1)<(4+2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(20*0.5)<(10-3)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(4^2)<(18/2)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(9/2)<=(4*4)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1.5*2)<=(1.5+1.5)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(20-8)<=(3*1)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(1*1)&&(1^10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&1)&&(10^1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(100-5)&&(10-5)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(0*10)&&(2+2)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(8%2)&&(0^0)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(4/2)||(3*2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1||0)||(0*10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(9%3)||(3*3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(0||0)||(10&&0)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE8() {
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "3*2^3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 24, "number");

    content = "3*2^2+1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 13, "number");

    content = "3*10%7";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 9, "number");

    content = "135/3^3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 5, "number");

    content = "18/10%7";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 6, "number");

    content = "4^2*2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 32, "number");

    content = "10%7*2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 6, "number");

    content = "3^3/4";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 6.75, "number");

    content = "10%4/2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "4+2*2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 8, "number");

    content = "10-7*2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -4, "number");

    content = "3+16/4";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 7, "number");

    content = "10-6/3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 8, "number");

    content = "2*8-2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 14, "number");

    content = "3*4-20";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -8, "number");

    content = "14/7+3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 5, "number");

    content = "9/3-4";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -1, "number");

    content = "3+4==7";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7-8==-1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "8-2!=2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7+3!=3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7==3+4";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "-1==7-8";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "8!=8-2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7!=7+3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "7!=10-3";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "7-10!=-3";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "3+4>=7";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "3-8>-10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "8-2<10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7+3<=10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7<=3+4";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "-1<7+8";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "8>8-2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7>=10-3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&1==1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&10==10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&5!=1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&7!=1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0==0&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1==1&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1!=0&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0!=1&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1||10==1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0||10==10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1||5!=1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0||7!=1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0==0||1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1==1||0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1!=0||1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0!=1||0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&1<=1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&3<10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&5>1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&0>=0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0<=0&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1<2&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1>0&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "10>=5&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1||10<=1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0||1<10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1||5>=10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0||7>1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0>=0||1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "4>3||0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1<0||1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0<1||0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    END_SUB_TEST;
}

static int TestALE9() {
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "(3*2^3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 24, "number");

    content = "(3*10%7)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 9, "number");

    content = "(135/3^3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 5, "number");

    content = "(18/10%7)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 6, "number");

    content = "(4^2*2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 32, "number");

    content = "(10%7*2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 6, "number");

    content = "(3^3/4)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 6.75, "number");

    content = "(10%4/2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(4+2*2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 8, "number");

    content = "(10-7*2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -4, "number");

    content = "(3+16/4)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 7, "number");

    content = "(10-6/3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 8, "number");

    content = "(2*8-2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 14, "number");

    content = "(3*4-20)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -8, "number");

    content = "(14/7+3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 5, "number");

    content = "(9/3-4)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -1, "number");

    content = "(3+4==7)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7-8==-1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(8-2!=2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7+3!=3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7==3+4)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(-1==7-8)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(8!=8-2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7!=7+3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(7!=10-3)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(7-10!=-3)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(3+4>=7)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(3-8>-10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(8-2<10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7+3<=10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7<=3+4)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(-1<7+8)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(8>8-2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7>=10-3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&1==1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&10==10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&5!=1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&7!=1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0==0&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1==1&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1!=0&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0!=1&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1||10==1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0||10==10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1||5!=1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0||7!=1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0==0||1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1==1||0)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1!=0||1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0!=1||0)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&1<=1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&3<10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&5>1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&0>=0)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0<=0&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1<2&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1>0&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(10>=5&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1||10<=1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0||1<10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1||5>=10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0||7>1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0>=0||1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(4>3||0)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1<0||1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0<1||0)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    END_SUB_TEST;
}

static int TestALE10() {
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "3*(2)^3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 24, "number");

    content = "3*(5+5)%7";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 9, "number");

    content = "135/(5-2)^3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 5, "number");

    content = "18/(5*2)%7";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 6, "number");

    content = "4^(4/2)*2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 32, "number");

    content = "10%(3.5*2)*2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 6, "number");

    content = "3^(1.5*2)/4";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 6.75, "number");

    content = "10%(8-4)/2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "4+(8/4)*2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 8, "number");

    content = "10-(14/2)*2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -4, "number");

    content = "3+(4+12)/4";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 7, "number");

    content = "10-(3+2+1)/3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 8, "number");

    content = "2*(2+1+5)-2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 14, "number");

    content = "3*(8-4)-20";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -8, "number");

    content = "14/(10-3)+3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 5, "number");

    content = "9/(1+1+1)-4";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -1, "number");

    content = "3+(2*2)==7";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7-(10-2)==-1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "8-(1+1)!=2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7+(9/3)!=3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7==(12/4)+4";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "-1==(2+5)-8";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "8!=(2*2)-2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7!=(1+5+1)+3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "7!=(10)-3";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "7-(10)!=-3";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "3+(4)>=7";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "3-(8)>-10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "8-(2)<10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7+(3)<=10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7<=(3)+4";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "-1<(7)+8";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "8>(8)-2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "7>=(10)-3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&(10-9)==1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&(5+5)==10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&(10/2)!=1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&(4+4)!=1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0==(0/4)&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1==(4^0)&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1!=(3-3)&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0!=(4-1-2)&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1||(2%2)==1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0||(1==1)==1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1||(15%10)!=1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0||(-4)!=1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0==(0+0)||1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1==(100-99)||0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1!=(4%5)||1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0!=(1^1)||0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&(10%9)<=1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&(2+1)<10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&(2.5+2.5)>1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1&&(8%2)>=0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0<=(0)&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1<(8-1)&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1>(4-8)&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "10>=(100/10)&&1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1||(3+3)<=1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0||(6%6)<10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1||(5^2)>=10";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0||(7-2)>1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0>=(0&&1)||1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "4>(2&&1)||0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1<(4-4)||1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "0<(4^0)||0";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    END_SUB_TEST;
}

static int TestALE11() {
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "(3*(2)^3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 24, "number");

    content = "(3*(5+5)%7)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 9, "number");

    content = "(135/(5-2)^3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 5, "number");

    content = "(18/(5*2)%7)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 6, "number");

    content = "(4^(4/2)*2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 32, "number");

    content = "(10%(3.5*2)*2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 6, "number");

    content = "(3^(1.5*2)/4)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 6.75, "number");

    content = "(10%(8-4)/2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(4+(8/4)*2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 8, "number");

    content = "(10-(14/2)*2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -4, "number");

    content = "(3+(4+12)/4)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 7, "number");

    content = "(10-(3+2+1)/3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 8, "number");

    content = "(2*(2+1+5)-2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 14, "number");

    content = "(3*(8-4)-20)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -8, "number");

    content = "(14/(10-3)+3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 5, "number");

    content = "(9/(1+1+1)-4)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, -1, "number");

    content = "(3+(2*2)==7)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7-(10-2)==-1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(8-(1+1)!=2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7+(9/3)!=3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7==(12/4)+4)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(-1==(2+5)-8)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(8!=(2*2)-2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7!=(1+5+1)+3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content  = "(7!=(10)-3)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(7-(10)!=-3)";
    is_valid = ALE::Evaluate(number, content);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "(3+(4)>=7)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(3-(8)>-10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(8-(2)<10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7+(3)<=10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7<=(3)+4)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(-1<(7)+8)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(8>(8)-2)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(7>=(10)-3)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&(10-9)==1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&(5+5)==10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&(10/2)!=1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&(4+4)!=1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0==(0/4)&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1==(4^0)&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1!=(3-3)&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0!=(4-1-2)&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1||(2%2)==1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0||(1==1)==1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1||(15%10)!=1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0||(-4)!=1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0==(0+0)||1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1==(100-99)||0)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1!=(4%5)||1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0!=(1^1)||0)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&(10%9)<=1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&(2+1)<10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&(2.5+2.5)>1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1&&(8%2)>=0)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0<=(0)&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1<(8-1)&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1>(4-8)&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(10>=(100/10)&&1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1||(3+3)<=1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0||(6%6)<10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1||(5^2)>=10)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0||(7-2)>1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0>=(0&&1)||1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(4>(2&&1)||0)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(1<(4-4)||1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "(0<(4^0)||0)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "((1+2)^(1+2))/2";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 13.5, "number");

    content = "((5/5+1)*2+1)+3*3";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 14, "number");

    content = "5+2*4-8/2==9 && 1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "1+2*3 == 7";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "((5/5+1)*2+1)+3*3 != 12 && 1";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content =
        R"(2  * 1 * 3 + 1 - 4 + (10 - 5 - 6 + 1 + 1 + 1 + 1) * (8 / 4 + 1) -1 - -1 + 2 == 14)";
    number = ALE::Evaluate(content);
    EQ_VALUE(number, 1, "number");

    content = "((2*4) + 1)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 9, "number");

    content = "((2* (2 + 3)) + 1 - 4)";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 7, "number");

    content =
        R"(((2* (1 * 3)) + 1 - 4) + (((10 - 5) -
        6 + ((1 + 1) + (1 + 1))) * (8 / 4 + 1)) -
        (1) - (-1) + 2)";
    number = ALE::Evaluate(content);
    EQ_VALUE(number, 14, "number");

    END_SUB_TEST;
}

static int TestALE12() {
    double      number = -1;
    const char *content;
    bool        is_valid;
    aleHelper   ale;

    content = "a==a";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "a==c";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "a!=b";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "a!=a";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "a == a";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "a == c";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "a != b";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "a != a";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = " a == a ";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = " a == c ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = " a != b ";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = " a != a ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "  a    == a";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "  a    == a          ";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "      a==a";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "      a             ==a";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "a==         a              ";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "   a      == c         ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "   a !=         b";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "   a!=a      ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "AbcDEF==AbcDEF";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "AbcDEF==zwx";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "AbcDEF!=BBBB";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "AbcDEF!=AbcDEF";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "AbcDEF == AbcDEF";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "AbcDEF == zwx";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "AbcDEF != BBBB";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "AbcDEF != AbcDEF";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = " AbcDEF == AbcDEF ";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = " AbcDEF == zwx ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = " AbcDEF != BBBB ";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = " AbcDEF != AbcDEF ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "  AbcDEF    == AbcDEF";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "  AbcDEF    == AbcDEF          ";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "      AbcDEF==AbcDEF";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "      AbcDEF             ==AbcDEF";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "AbcDEF==         AbcDEF              ";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "   AbcDEF      == zwx         ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "   AbcDEF !=         BBBB";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "   AbcDEF!=AbcDEF      ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE13() {
    double      number = -1;
    const char *content;
    bool        is_valid;

    content  = "{1} + 5";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "3 + {9}";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "{A} == A";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "{A} != A";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "A=={A}";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "A==";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "A!={A}";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(4+3";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "4+3*(3";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(4+(2)";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "{4+5";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content = "a";
    number  = ALE::Evaluate(content);
    EQ_VALUE(number, 0, "number");

    aleHelper ale;

    content = "a";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 0, "number");

    content = "{A}+10";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 16, "number");

    content = "{AB}-3";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 10, "number");

    content = "{ABC}*    2";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 52, "number");

    content = "{A}    /2";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 3, "number");

    content = "   {A}^2";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 36, "number");

    content = "{AB}  %  5";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 3, "number");

    content = "{ABC}>7";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "   {AB} <6";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "{A}  >=4";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "{ABC}<=30";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "{AB} && 0  ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "{A} && 1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "{ABC} || 0";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "   {AB} || 1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "10+{A}";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 16, "number");

    content = "3+{AB}";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 16, "number");

    content = "2*    {ABC}";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 52, "number");

    content = " 104   /{ABC}";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 4, "number");

    content = " 2  ^{A}";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 64, "number");

    content = "35 &&  {AB} ";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "7<{ABC}";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "   8 >{AB}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "3  <={A}";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "26>={ABC}";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "0 && {AB}  ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "1 && {A}";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "0 || {ABC}";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = " 1   || {AB}";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "{d} +5  ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "{w}*0  ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "{}&&0";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE14() {
    double      number = -1;
    const char *content;
    bool        is_valid;
    aleHelper   ale;

    content = "3*{A}^3";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 648, "number");

    content = "3*{AB}%7";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 18, "number");

    content = "135/{A}^3";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 0.625, "number");

    content = "54/{ABC}%10";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 9, "number");

    content = "4^{A}*2";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 8192, "number");

    content = "10%({A})*2";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 8, "number");

    content = "3^{A}/(4)";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 182.25, "number");

    content = "(10)%{AB}/2";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 5, "number");

    content = "4+({A})*2";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 16, "number");

    content = "10-({AB})*2";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, -16, "number");

    content = "3+{ABC}/(4)";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 9.5, "number");

    content = "(10)-{A}/3";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 8, "number");

    content = "2*{ABC}-2";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 50, "number");

    content = "(3*{A}-20)";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, -2, "number");

    content = "(65/{AB}+3)";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 8, "number");

    content = "52/{ABC}-4";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, -2, "number");

    content = "(3+{ABC}==29)";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "7-{AB}==-6";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "(8-{AB})!=2";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "7+{ABC}!=3";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "10=={A}+4";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "10==({A}+4)";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "-1==({A})-7";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "8!=({ABC}-2)";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "7!=({AB}+3)";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "7!={AB}+3";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "7!=({AB}-6)";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "(4-{A})!=-2";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "4-{A}!=-2";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "3+{ABC}>=7";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "3-{A}>-10";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "8-{A}<10";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "7+{ABC}<=40";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "7<={AB}+4";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "-1<{ABC}+8";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "8>{A}-2";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "7>={A}-3";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1&&({ABC})==26";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1&&({AB})==13";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "(1&&{A})!=61";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1&&({ABC})!=126";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "0==(0/{AB})&&1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1==({A}^0)&&1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "(1!=({ABC}))&&1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "0!=({A}&&1)";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1||({AB})==1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "0||{Q}==Qentem";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1||{Q}!=Qentem";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "0||{Q}!=Qen";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1!={1}||0";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "6==({A})||0";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "6=={A}||0";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "Qentem!={QA}||1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "0!=({AB})||0";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1&&{A}<=6";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1&&{AB}<27";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1&&{ABC}>1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1&&{A}>=0";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "0<={AB}&&1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1<{A}&&1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content  = "1>{ABC}&&1";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content = "10>={A}&&1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1||({AB}<=1)";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "0||{AB}<14";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "(1||{A}>=10)";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "0||{ABC}>1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "0>={AB}||1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "(7>{A}||0)";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "1<{AB}||1";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    content = "0<{ABC}||0";
    number  = ALE::Evaluate(content, &ale);
    EQ_VALUE(number, 1, "number");

    END_SUB_TEST;
}

static int TestALE15() {
    double      number = -1;
    const char *content;
    bool        is_valid;
    aleHelper   ale;

    content  = "{Q}   ==Qentem";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "{QA}==     Qentem ALE";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "        {Q}   ==Qentem";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "             {QA}==     Qentem ALE";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "{Q}==          Qentem";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "{QA}==Qentem ALE        ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "{Q}==    Qentem          ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "  {QA}    ==     Qentem ALE     ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "{10}==Qentem";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "{Q}!=Qentem";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "{QA}!=Qentem ALE";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "{2}!=Qentem ALE";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "{Q}==Qente";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "{QA}==ALE";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "{Q}!=Qente";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "{QA}!=ALE";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "Qentem   =={Q}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "Qentem ALE==     {QA}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "        Qentem   =={Q}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "             Qentem ALE==     {QA}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "Qentem==          {Q}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "Qentem ALE=={QA}        ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "Qentem==    {Q}          ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "  Qentem ALE    ==     {QA}     ";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "Qentem=={10}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "Qentem!={Q}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "Qentem ALE!={QA}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "Qentem ALE!={2}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "Qente=={Q}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "ALE=={QA}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 0, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "Qente!={Q}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    content  = "ALE!={QA}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_VALUE(number, 1, "number");
    EQ_TRUE(is_valid, "is_valid");

    END_SUB_TEST;
}

static int TestALE16() {
    double      number = -1;
    const char *content;
    bool        is_valid;
    aleHelper   ale;

    content  = "2      {A}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "7{ABC}";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "{AB}3";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "{AB} 9";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "{AB}  &  5";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "(3)3";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "3(3)";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "4+3(7)";
    is_valid = ALE::Evaluate(number, content, &ale);
    EQ_FALSE(is_valid, "is_valid");

    content  = "1!#1";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

    content  = "1|#1";
    is_valid = ALE::Evaluate(number, content);
    EQ_FALSE(is_valid, "is_valid");

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
    START_TEST("ALE Test 16", TestALE16);

    END_TEST("ALE.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
