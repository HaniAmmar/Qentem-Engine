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

#include "ALE.hpp"
#include "Array.hpp"

#ifndef QENTEM_ALE_TESTS_H_
#define QENTEM_ALE_TESTS_H_

namespace Qentem {
namespace Test {

class ALEHelper {
  public:
    // See class Template_T_ for another example.
    struct Value_T_ {
        const char *Name;
        SizeT       NameLength;

        // Number Value
        SizeT Number;

        // String Value
        const char *Str;
        SizeT       StrLength;
    };

    void SetItems() {
        if (items_.IsEmpty()) {
            items_ += Value_T_{"{1}", 3, 6, nullptr, 0};
            items_ += Value_T_{"{A}", 3, 6, nullptr, 0};
            items_ += Value_T_{"{AB}", 4, 13, nullptr, 0};
            items_ += Value_T_{"{ABC}", 5, 26, nullptr, 0};
            items_ += Value_T_{"{Q}", 3, 0, "Qentem", 6};
            items_ += Value_T_{"{QA}", 4, 0, "Qentem ALE", 10};
        }
    }

    bool FindItem(const Value_T_ *&item, const char *content, SizeT length) const noexcept {
        for (SizeT i = 0; i < items_.Size(); i++) {
            item = (items_.First() + i);

            if ((item->NameLength == length) && StringUtils::IsEqual(item->Name, content, length)) {
                return true;
            }
        }

        return false;
    }

    bool ALESetNumber(double &number, const char *content, SizeT length) const noexcept {
        const Value_T_ *item;

        if (FindItem(item, content, length) && (item->Str == nullptr)) {
            number = static_cast<double>(item->Number);
            return true;
        }

        return false;
    }

    bool ALEIsEqual(bool &result, const char *content, ALE::Number left, ALE::Number right, bool left_evaluated,
                    bool right_evaluated) const noexcept {
        using ALEExpressions_T_ = ALEExpressions<char>;

        const char     *left_content  = (content + left.Content.Offset);
        const char     *right_content = (content + right.Content.Offset);
        const Value_T_ *item;
        SizeT           left_length  = left.Content.Length;
        SizeT           right_length = right.Content.Length;

        bool is_number = (left_evaluated || right_evaluated);

        if (!left_evaluated) {
            if (*left_content == ALEExpressions_T_::BracketStart) {
                if (!FindItem(item, left_content, left_length)) {
                    return false;
                }

                if (item->Str == nullptr) {
                    left.Number = static_cast<double>(item->Number);
                    is_number   = true;
                } else {
                    if (is_number) {
                        return false;
                    }

                    left_content = item->Str;
                    left_length  = item->StrLength;
                }
            } else {
                if (*left_content == ALEExpressions_T_::ParenthesStart) {
                    ++left_content;
                    left_length -= 2;
                }

                left_evaluated = ALE::Evaluate(left.Number, left_content, left_length, this);
                if (!left_evaluated && is_number) {
                    return false;
                }

                is_number = left_evaluated;
            }
        }

        if (!right_evaluated) {
            if (*right_content == ALEExpressions_T_::BracketStart) {
                if (!FindItem(item, right_content, right_length)) {
                    return false;
                }

                if (is_number && (item->Str != nullptr)) {
                    return false;
                }

                if (item->Str == nullptr) {
                    right.Number = static_cast<double>(item->Number);
                } else {
                    right_content = item->Str;
                    right_length  = item->StrLength;
                }

            } else if (is_number) {
                if (*right_content == ALEExpressions_T_::ParenthesStart) {
                    ++right_content;
                    right_length -= 2;
                }

                if (!(ALE::Evaluate(right.Number, right_content, right_length, this))) {
                    return false;
                }
            }
        }

        if (is_number) {
            result = (left.Number == right.Number);
        } else {
            result = ((left_length == right_length) && StringUtils::IsEqual(left_content, right_content, right_length));
        }

        return true;
    }

  private:
    Array<Value_T_> items_;
};

static void TestALE1(TestHelper &helper) {
    double      number;
    const char *content;
    bool        is_valid;

    content  = "";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "  ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "               ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "()";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "( )";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(  )";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(        )";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " ()";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " ( )";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " (  )";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " (        )";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "() ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "( ) ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(  ) ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(        ) ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "  ()";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "  ( )";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "  (  )";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "  (        )";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "()  ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "( )  ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(  )  ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(        )  ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " () ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " ( ) ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " (  ) ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = " (        ) ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "   ()   ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "   ( )   ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "   (  )   ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "   (        )   ";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "a";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "10/0";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "10/(2-2)";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "fj";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "fgh";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "5f2s4s5dg";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(d)";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(oo)";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(www46fs5d4f5ds)";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "() ()";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(())";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "() (()) ()";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(()) (()()) ((()())())";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);
}

static void TestALE2(TestHelper &helper) {
    double      number = -99;
    const char *content;
    bool        is_valid;

    content  = "1";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    // NOTE: Implement power of fraction.
    content  = "2^0.2";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);
    // helper.Equal(number, 1.148698355, "number", __LINE__);
    // helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 4";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 4, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "5 ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 5, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 10 ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 10, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 7 ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 7, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "100";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 100, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 40";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 40, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "5000 ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 5000, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 1004 ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 1004, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 714 ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 714, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(1)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " (4)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 4, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(5) ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 5, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " (10) ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 10, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " (7) ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 7, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(100)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 100, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " (40)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 40, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(5000) ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 5000, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " (1004) ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 1004, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " ( 714 ) ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 714, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "((1))";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " (( 4))";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 4, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "((5     ) )";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 5, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "( 10 )";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 10, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "( (((       7   ))) )";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 7, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(100)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 100, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "( 40)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 40, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(5000 )";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 5000, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(((((((((( 1004 ))))))))))";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 1004, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "( 714 )";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 714, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "0^10";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestALE3(TestHelper &helper) {
    double      number;
    const char *content;
    bool        is_valid;

    content = "3^2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 9, "number", __LINE__);

    content = "7%5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 2, "number", __LINE__);

    content = "3*6";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 18, "number", __LINE__);

    content = "10/2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 5, "number", __LINE__);

    content = "3+5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 8, "number", __LINE__);

    content = "5-2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 3, "number", __LINE__);

    content = "3>2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "2>2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 0, "number", __LINE__);

    content  = "5==2";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "5==5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "5!=2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "10!=10";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "1>5";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "5>=1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8>=8";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "5>=7";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "4<8";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "7<7";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "10<9";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "3<=16";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3<=3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "17<=3";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "1&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&800";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "0&&5";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "0&&0";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "8||6";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4||0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||9";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "0||0";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   3   ^2   ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 9, "number", __LINE__);

    content = "   7%   5   ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 2, "number", __LINE__);

    content = "   3   *   6";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 18, "number", __LINE__);

    content = "10   /   2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 5, "number", __LINE__);

    content = "3   +   5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 8, "number", __LINE__);

    content  = " 5 == 2 ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " 5 == 5 ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 5 != 2 ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = " 10 != 10 ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " 5 - 2 ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 3, "number", __LINE__);

    content = "   3 > 2   ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   2   >2";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "1>   5   ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " 5   >=1   ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "   8>=   8 ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "5   >=   7";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   4   <8   ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   7<   7   ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "   10<   0";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   3   <=   16";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "   3   <=   3     ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   17   <=3";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   1   &&   1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "   1   &&   800    ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   0&&5  ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "   0   &&0";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   8   ||   6  ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4   ||   0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "   0||9";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   0   ||0";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestALE4(TestHelper &helper) {
    double      number;
    const char *content;
    bool        is_valid;

    content = "16 ^ -2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 0.00390625, "number", __LINE__);

    content = "8 ^-2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 0.015625, "number", __LINE__);

    content = "-3^0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "5564685^0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-3^2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 9, "number", __LINE__);

    content = "-8^-2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 0.015625, "number", __LINE__);

    content = "-4 * 5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -20, "number", __LINE__);

    content = "5 *-6";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -30, "number", __LINE__);

    content = "-3*-4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 12, "number", __LINE__);

    content = "-10 / 2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -5, "number", __LINE__);

    content = "7 /-2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -3.5, "number", __LINE__);

    content = "-9 / -3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 3, "number", __LINE__);

    content = "-7 + 5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -2, "number", __LINE__);

    content = "-11 + -2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -13, "number", __LINE__);

    content = "-8+-3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -11, "number", __LINE__);

    content = "5 --2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 7, "number", __LINE__);

    content = "-8-3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -11, "number", __LINE__);

    content = "5 ---2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 3, "number", __LINE__);

    content = "---5 ---2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -7, "number", __LINE__);

    content  = " 5 == - 5 ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 5 == -5 ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " 5 ==-5 ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " -5 == 5 ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = " - 5 == 5 ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " -5 != 2 ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 10 != -10 ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = " -10 != 10 ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = " -  10 != 10 ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 10 !=-10 ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 10 !=- 10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3 >-2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10 > -10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "-2 > 2";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "-2 > -11";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "-1>5";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "5 >=-19";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1 >=- 100";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-8 >= -   8";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "-5 >= 7";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "3 <-2";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "10 < -10";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "-2 < 2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "-2 < -11";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "-1<5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "5 <=-19";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "5 - <=-19";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "-5 <=- 19";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "-8 <= -   8";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-5 <= 7";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "-1 && 1";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "1 &&- 800";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "0 && -5";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "-8 && -9";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "-8 || 6";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4 ||-9";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "0 || - 9";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "-7 || -500";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestALE5(TestHelper &helper) {
    double      number;
    const char *content;
    bool        is_valid;

    content = "16 ^ +2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 256, "number", __LINE__);

    content = "8 ^+2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 64, "number", __LINE__);

    content = "+3^0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "5564685^0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "+3^2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 9, "number", __LINE__);

    content = "+3^+3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 27, "number", __LINE__);

    content = "+4 * 5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 20, "number", __LINE__);

    content = "5 *+6";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 30, "number", __LINE__);

    content = "+3*+4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 12, "number", __LINE__);

    content = "+10 / 2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 5, "number", __LINE__);

    content = "7 /+2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 3.5, "number", __LINE__);

    content = "+9 / +3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 3, "number", __LINE__);

    content = "+7 - 5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 2, "number", __LINE__);

    content = "+11 - +2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 9, "number", __LINE__);

    content = "+8-+3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 5, "number", __LINE__);

    content = "5 ++2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 7, "number", __LINE__);

    content = "+8+3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, +11, "number", __LINE__);

    content = "5 +++2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 7, "number", __LINE__);

    content = "+++5 +++2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 7, "number", __LINE__);

    content  = "+5==2";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "5==+2";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "+5==+2";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+5==+5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "+5!=+2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "+10!=+10";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "+10!=10";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "10!=+10";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "3 >+2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "10 > +10";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "+2 > 2";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+11 > +2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "+1>5";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+19 >=5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "+1 >=+ 100";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+8 >= +   8";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "7+ >= 5";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "3 <+2";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "10 < +10";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+1 < 2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "+2 < +11";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "+1<5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "+19 <=5";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "+5 <=+ 19";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+8 <= +   8";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "+5 <= 7";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "+1 && 1";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "1 &&+ 800";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "0 && +5";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "+8 && +9";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "+8 || 6";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4 ||+9";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content =
        R"(4                                                                      ||1
                                                                                       ||
                                                                                             2)";
    number = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "0 || + 9";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "+7 || +500";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestALE6(TestHelper &helper) {
    double      number = -1;
    const char *content;
    bool        is_valid;

    content  = "(0)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(+1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(-1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -1, "number", __LINE__);

    content = "( 1 )";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "( +1 )";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "( -1 )";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -1, "number", __LINE__);

    content = "(1 )";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(+1 )";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(-1 )";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -1, "number", __LINE__);

    content = "( 1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "( +1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "( -1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -1, "number", __LINE__);

    content = "(       1            )";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(       +      1             )";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(        -       1           )";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -1, "number", __LINE__);

    content = "(2*3)^4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1296, "number", __LINE__);

    content = "(8*3) %7";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 3, "number", __LINE__);

    content = "(6+2)  *6";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 48, "number", __LINE__);

    content = " (15-5)  /5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 2, "number", __LINE__);

    content = "   (4*3)   +  5  ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 17, "number", __LINE__);

    content = "(18/3)- 2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 4, "number", __LINE__);

    content = "(13%5)           >2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(3                  ^-2)>2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 0, "number", __LINE__);

    content  = "(1+2)== 2";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(6-1) ==5";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = " (3^2)!=2 ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(20/2) != 10";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(  10  -  +  9  )>5";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(15%10)>=1             ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "          (16/2)>=8";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(4*1.5)>=7";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(0.999999+0.1)<8";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(20*0.5)<7";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(4^2)<9";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(9/2)<=16";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1.5*2)<=3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(20-8)<=3";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(1*1)&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&1)&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(100-5)&&800";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(0*10)&&5";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(8%2)&&0";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(4/2)||6";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||0)||0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(9%3)||9";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(0||0)||0";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestALE7(TestHelper &helper) {
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "6^(8/2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1296, "number", __LINE__);

    content = "24 %(3.5*2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 3, "number", __LINE__);

    content = "8  *(12/2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 48, "number", __LINE__);

    content = " 10  /(7-2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 2, "number", __LINE__);

    content = "   12   +  (2.5*2)  ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 17, "number", __LINE__);

    content = "6- (10/5)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 4, "number", __LINE__);

    content = "(3)           >(2*1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0.9999999999>(4-2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 0, "number", __LINE__);

    content  = "3== (3-1)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(5) ==(10-5)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 9!=(1+1) ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "10 != (20/2)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "1>(3+2)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "5>=(100^0)             ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 8>=(2^3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "6>=(14/2)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "1<(4+2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "10<(10-3)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "16<(18/2)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "4.5<=(4*4)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3<=(1.5+1.5)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "12<=(3*1)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "1&&(1^10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10&&(10^1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7&&(10-5)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "0&&(2+2)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "0&&(0^0)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "9||        (3*2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "        1||(0*10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0          ||(3*3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "        0||(10&&0)          ";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(2*3)^(8/2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1296, "number", __LINE__);

    content = "(8*3) %(3.5*2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 3, "number", __LINE__);

    content = "(6+2)  *(12/2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 48, "number", __LINE__);

    content = " (15-5)  /(7-2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 2, "number", __LINE__);

    content = "   (4*3)   +  (2.5*2)  ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 17, "number", __LINE__);

    content = "(18/3)- (10/5)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 4, "number", __LINE__);

    content = "(13%5)           >(2*1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(3                  ^-2)>(4-2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 0, "number", __LINE__);

    content  = "(1+2)== (3-1)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(6-1) ==(10-5)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = " (3^2)!=(1+1) ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(20/2) != (20/2)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(  10  -  +  9  )>(3+2)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(15%10)>=(100^0)             ";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "          (16/2)>=(2^3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(4*1.5)>=(14/2)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(0.999999+0.1)<(4+2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(20*0.5)<(10-3)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(4^2)<(18/2)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(9/2)<=(4*4)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1.5*2)<=(1.5+1.5)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(20-8)<=(3*1)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(1*1)&&(1^10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&1)&&(10^1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(100-5)&&(10-5)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(0*10)&&(2+2)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(8%2)&&(0^0)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(4/2)||(3*2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||0)||(0*10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(9%3)||(3*3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(0||0)||(10&&0)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestALE8(TestHelper &helper) {
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "3*2^3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 24, "number", __LINE__);

    content = "3*2^2+1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 13, "number", __LINE__);

    content = "3*10%7";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 9, "number", __LINE__);

    content = "135/3^3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 5, "number", __LINE__);

    content = "18/10%7";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 6, "number", __LINE__);

    content = "4^2*2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 32, "number", __LINE__);

    content = "10%7*2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 6, "number", __LINE__);

    content = "3^3/4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 6.75, "number", __LINE__);

    content = "10%4/2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4+2*2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 8, "number", __LINE__);

    content = "10-7*2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -4, "number", __LINE__);

    content = "3+16/4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 7, "number", __LINE__);

    content = "10-6/3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 8, "number", __LINE__);

    content = "2*8-2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 14, "number", __LINE__);

    content = "3*4-20";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -8, "number", __LINE__);

    content = "14/7+3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 5, "number", __LINE__);

    content = "9/3-4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -1, "number", __LINE__);

    content = "3+4==7";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7-8==-1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8-2!=2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7+3!=3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7==3+4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1==7-8";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8!=8-2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7!=7+3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "7!=10-3";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "7-10!=-3";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "3+4>=7";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3-8>-10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8-2<10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7+3<=10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7<=3+4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1<7+8";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8>8-2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7>=10-3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&1==1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&10==10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&5!=1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&7!=1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0==0&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1==1&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1!=0&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0!=1&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||10==1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||10==10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||5!=1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||7!=1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0==0||1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1==1||0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1!=0||1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0!=1||0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&1<=1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&3<10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&5>1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&0>=0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0<=0&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1<2&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1>0&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10>=5&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||10<=1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||1<10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||5>=10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||7>1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0>=0||1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4>3||0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1<0||1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0<1||0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);
}

static void TestALE9(TestHelper &helper) {
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "(3*2^3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 24, "number", __LINE__);

    content = "(3*10%7)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 9, "number", __LINE__);

    content = "(135/3^3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 5, "number", __LINE__);

    content = "(18/10%7)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 6, "number", __LINE__);

    content = "(4^2*2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 32, "number", __LINE__);

    content = "(10%7*2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 6, "number", __LINE__);

    content = "(3^3/4)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 6.75, "number", __LINE__);

    content = "(10%4/2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(4+2*2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 8, "number", __LINE__);

    content = "(10-7*2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -4, "number", __LINE__);

    content = "(3+16/4)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 7, "number", __LINE__);

    content = "(10-6/3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 8, "number", __LINE__);

    content = "(2*8-2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 14, "number", __LINE__);

    content = "(3*4-20)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -8, "number", __LINE__);

    content = "(14/7+3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 5, "number", __LINE__);

    content = "(9/3-4)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -1, "number", __LINE__);

    content = "(3+4==7)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7-8==-1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8-2!=2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7+3!=3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7==3+4)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(-1==7-8)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8!=8-2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7!=7+3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(7!=10-3)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(7-10!=-3)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(3+4>=7)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(3-8>-10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8-2<10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7+3<=10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7<=3+4)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(-1<7+8)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8>8-2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7>=10-3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&1==1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&10==10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&5!=1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&7!=1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0==0&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1==1&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1!=0&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0!=1&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||10==1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||10==10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||5!=1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||7!=1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0==0||1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1==1||0)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1!=0||1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0!=1||0)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&1<=1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&3<10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&5>1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&0>=0)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0<=0&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1<2&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1>0&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(10>=5&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||10<=1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||1<10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||5>=10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||7>1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0>=0||1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(4>3||0)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1<0||1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0<1||0)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);
}

static void TestALE10(TestHelper &helper) {
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "3*(2)^3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 24, "number", __LINE__);

    content = "3*(5+5)%7";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 9, "number", __LINE__);

    content = "135/(5-2)^3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 5, "number", __LINE__);

    content = "18/(5*2)%7";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 6, "number", __LINE__);

    content = "4^(4/2)*2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 32, "number", __LINE__);

    content = "10%(3.5*2)*2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 6, "number", __LINE__);

    content = "3^(1.5*2)/4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 6.75, "number", __LINE__);

    content = "10%(8-4)/2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4+(8/4)*2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 8, "number", __LINE__);

    content = "10-(14/2)*2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -4, "number", __LINE__);

    content = "3+(4+12)/4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 7, "number", __LINE__);

    content = "10-(3+2+1)/3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 8, "number", __LINE__);

    content = "2*(2+1+5)-2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 14, "number", __LINE__);

    content = "3*(8-4)-20";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -8, "number", __LINE__);

    content = "14/(10-3)+3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 5, "number", __LINE__);

    content = "9/(1+1+1)-4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -1, "number", __LINE__);

    content = "3+(2*2)==7";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7-(10-2)==-1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8-(1+1)!=2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7+(9/3)!=3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7==(12/4)+4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1==(2+5)-8";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8!=(2*2)-2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7!=(1+5+1)+3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "7!=(10)-3";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "7-(10)!=-3";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "3+(4)>=7";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3-(8)>-10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8-(2)<10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7+(3)<=10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7<=(3)+4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1<(7)+8";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8>(8)-2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7>=(10)-3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(10-9)==1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(5+5)==10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(10/2)!=1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(4+4)!=1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0==(0/4)&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1==(4^0)&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1!=(3-3)&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0!=(4-1-2)&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||(2%2)==1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||(1==1)==1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||(15%10)!=1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||(-4)!=1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0==(0+0)||1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1==(100-99)||0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1!=(4%5)||1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0!=(1^1)||0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(10%9)<=1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(2+1)<10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(2.5+2.5)>1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&(8%2)>=0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0<=(0)&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1<(8-1)&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1>(4-8)&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10>=(100/10)&&1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||(3+3)<=1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||(6%6)<10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||(5^2)>=10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||(7-2)>1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0>=(0&&1)||1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "4>(2&&1)||0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1<(4-4)||1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0<(4^0)||0";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);
}

static void TestALE11(TestHelper &helper) {
    double      number = -1;
    const char *content;
    bool        is_valid;

    content = "(3*(2)^3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 24, "number", __LINE__);

    content = "(3*(5+5)%7)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 9, "number", __LINE__);

    content = "(135/(5-2)^3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 5, "number", __LINE__);

    content = "(18/(5*2)%7)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 6, "number", __LINE__);

    content = "(4^(4/2)*2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 32, "number", __LINE__);

    content = "(10%(3.5*2)*2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 6, "number", __LINE__);

    content = "(3^(1.5*2)/4)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 6.75, "number", __LINE__);

    content = "(10%(8-4)/2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(4+(8/4)*2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 8, "number", __LINE__);

    content = "(10-(14/2)*2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -4, "number", __LINE__);

    content = "(3+(4+12)/4)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 7, "number", __LINE__);

    content = "(10-(3+2+1)/3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 8, "number", __LINE__);

    content = "(2*(2+1+5)-2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 14, "number", __LINE__);

    content = "(3*(8-4)-20)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -8, "number", __LINE__);

    content = "(14/(10-3)+3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 5, "number", __LINE__);

    content = "(9/(1+1+1)-4)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, -1, "number", __LINE__);

    content = "(3+(2*2)==7)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7-(10-2)==-1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8-(1+1)!=2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7+(9/3)!=3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7==(12/4)+4)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(-1==(2+5)-8)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8!=(2*2)-2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7!=(1+5+1)+3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "(7!=(10)-3)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(7-(10)!=-3)";
    is_valid = ALE::Evaluate(number, content);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "(3+(4)>=7)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(3-(8)>-10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8-(2)<10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7+(3)<=10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7<=(3)+4)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(-1<(7)+8)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8>(8)-2)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7>=(10)-3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(10-9)==1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(5+5)==10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(10/2)!=1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(4+4)!=1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0==(0/4)&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1==(4^0)&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1!=(3-3)&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0!=(4-1-2)&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||(2%2)==1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||(1==1)==1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||(15%10)!=1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||(-4)!=1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0==(0+0)||1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1==(100-99)||0)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1!=(4%5)||1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0!=(1^1)||0)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(10%9)<=1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(2+1)<10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(2.5+2.5)>1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&(8%2)>=0)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0<=(0)&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1<(8-1)&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1>(4-8)&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(10>=(100/10)&&1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||(3+3)<=1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||(6%6)<10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||(5^2)>=10)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0||(7-2)>1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0>=(0&&1)||1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(4>(2&&1)||0)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1<(4-4)||1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(0<(4^0)||0)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "((1+2)^(1+2))/2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 13.5, "number", __LINE__);

    content = "((5/5+1)*2+1)+3*3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 14, "number", __LINE__);

    content = "5+2*4-8/2==9 && 1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1+2*3 == 7";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "((5/5+1)*2+1)+3*3 != 12 && 1";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "((2*4) + 1)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 9, "number", __LINE__);

    content = "((2* (2 + 3)) + 1 - 4)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 7, "number", __LINE__);

    content = "10==2+2*2^2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "2^2*2+2==10";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3^2*2+2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 20, "number", __LINE__);

    content = "2+2*2^3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 18, "number", __LINE__);

    content = "2+3*3+2";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 13, "number", __LINE__);

    content = " -1 - -2 + 3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 4, "number", __LINE__);

    content = "-1--2+3";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 4, "number", __LINE__);

    content = "2+-3--4";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 3, "number", __LINE__);

    content = R"(2  * 1 * 3 + 1 - 4 + (10 - 5 - 6 + 1 + 1 + 1 + 1) * (8 / 4 + 1) -1 - -1 + 2 == 14)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);

    content =
        R"(((2* (1 * 3)) + 1 - 4) + (((10 - 5) -
        6 + ((1 + 1) + (1 + 1))) * (8 / 4 + 1)) -
        (1) - (-1) + 2)";
    number = ALE::Evaluate(content);
    helper.Equal(number, 14, "number", __LINE__);

    content = R"(2  * 1 * 3 + 1 - 4 + (10 - 5 - 6 + 1 + 1 + 1 + 1) * (8 / 4 + 1) -
        1 - -1 + 2 == ((5/5+1)*2+1)+3*3)";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 1, "number", __LINE__);
}

static void TestALE12(TestHelper &helper) {
    double      number = -1;
    const char *content;
    bool        is_valid;
    ALEHelper   ale;

    content = "a==a";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "a==c";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "a!=b";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "a!=a";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "a == a";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "a == c";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "a != b";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "a != a";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " a == a ";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = " a == c ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " a != b ";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = " a != a ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "  a    == a";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "  a    == a          ";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "      a==a";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "      a             ==a";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "a==         a              ";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   a      == c         ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   a !=         b";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   a!=a      ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "AbcDEF==AbcDEF";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "AbcDEF==zwx";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "AbcDEF!=BBBB";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "AbcDEF!=AbcDEF";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "AbcDEF == AbcDEF";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "AbcDEF == zwx";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "AbcDEF != BBBB";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "AbcDEF != AbcDEF";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " AbcDEF == AbcDEF ";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = " AbcDEF == zwx ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = " AbcDEF != BBBB ";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = " AbcDEF != AbcDEF ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "  AbcDEF    == AbcDEF";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "  AbcDEF    == AbcDEF          ";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "      AbcDEF==AbcDEF";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "      AbcDEF             ==AbcDEF";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "AbcDEF==         AbcDEF              ";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   AbcDEF      == zwx         ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "   AbcDEF !=         BBBB";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   AbcDEF!=AbcDEF      ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestALE13(TestHelper &helper) {
    double      number = -1;
    const char *content;
    bool        is_valid;
    ALEHelper   ale;

    ale.SetItems();

    content  = "{1} + 5";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "3 + {9}";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "10 == A";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "A == 10";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "10 == (A)";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(A) == 10";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{A} == A";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{A} != A";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "A=={A}";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "A==";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "A!={A}";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(4+3";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "4+3*(3";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(4+(2)";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{4+5";
    is_valid = ALE::Evaluate(number, content);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content = "a";
    number  = ALE::Evaluate(content);
    helper.Equal(number, 0, "number", __LINE__);

    content = "a";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 0, "number", __LINE__);

    content = "{A}+10";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 16, "number", __LINE__);

    content = "{AB}-3";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 10, "number", __LINE__);

    content = "{ABC}*    2";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 52, "number", __LINE__);

    content = "{A}    /2";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 3, "number", __LINE__);

    content = "   {A}^2";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 36, "number", __LINE__);

    content = "{AB}  %  5";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 3, "number", __LINE__);

    content = "{ABC}>7";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   {AB} <6";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "{A}  >=4";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "{ABC}<=30";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "{AB} && 0  ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "{A} && 1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "{ABC} || 0";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "   {AB} || 1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10+{A}";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 16, "number", __LINE__);

    content = "3+{AB}";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 16, "number", __LINE__);

    content = "2*    {ABC}";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 52, "number", __LINE__);

    content = " 104   /{ABC}";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 4, "number", __LINE__);

    content = " 2  ^{A}";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 64, "number", __LINE__);

    content = "35 &&  {AB} ";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7<{ABC}";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "   8 >{AB}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "3  <={A}";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "26>={ABC}";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "0 && {AB}  ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "1 && {A}";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0 || {ABC}";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = " 1   || {AB}";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "{d} +5  ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{w}*0  ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{}&&0";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);
}

static void TestALE14(TestHelper &helper) {
    double      number = -1;
    const char *content;
    bool        is_valid;
    ALEHelper   ale;

    ale.SetItems();

    content = "3*{A}^3";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 648, "number", __LINE__);

    content = "3*{AB}%7";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 18, "number", __LINE__);

    content = "135/{A}^3";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 0.625, "number", __LINE__);

    content = "54/{ABC}%10";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 9, "number", __LINE__);

    content = "4^{A}*2";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 8192, "number", __LINE__);

    content = "10%({A})*2";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 8, "number", __LINE__);

    content = "3^{A}/(4)";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 182.25, "number", __LINE__);

    content = "(10)%{AB}/2";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 5, "number", __LINE__);

    content = "4+({A})*2";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 16, "number", __LINE__);

    content = "10-({AB})*2";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, -16, "number", __LINE__);

    content = "3+{ABC}/(4)";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 9.5, "number", __LINE__);

    content = "(10)-{A}/3";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 8, "number", __LINE__);

    content = "2*{ABC}-2";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 50, "number", __LINE__);

    content = "(3*{A}-20)";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, -2, "number", __LINE__);

    content = "(65/{AB}+3)";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 8, "number", __LINE__);

    content = "52/{ABC}-4";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, -2, "number", __LINE__);

    content = "(3+{ABC}==29)";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7-{AB}==-6";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(8-{AB})!=2";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7+{ABC}!=3";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10=={A}+4";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "10==({A}+4)";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1==({A})-7";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8!=({ABC}-2)";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7!=({AB}+3)";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7!={AB}+3";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "7!=({AB}-6)";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "(4-{A})!=-2";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "4-{A}!=-2";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "3+{ABC}>=7";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "3-{A}>-10";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8-{A}<10";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7+{ABC}<=40";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7<={AB}+4";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "-1<{ABC}+8";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "8>{A}-2";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "7>={A}-3";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&({ABC})==26";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&({AB})==13";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1&&{A})!=61";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&({ABC})!=126";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0==(0/{AB})&&1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1==({A}^0)&&1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1!=({ABC}))&&1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0!=({A}&&1)";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||({AB})==1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||{Q}==Qentem";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||{Q}!=Qentem";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||{Q}!=Qen";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1!={1}||0";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "6==({A})||0";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "6=={A}||0";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "Qentem!={QA}||1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0!=({AB})||0";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&{A}<=6";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&{AB}<27";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&{ABC}>1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1&&{A}>=0";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0<={AB}&&1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1<{A}&&1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content  = "1>{ABC}&&1";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content = "10>={A}&&1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1||({AB}<=1)";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||{AB}<14";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(1||{A}>=10)";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0||{ABC}>1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0>={AB}||1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "(7>{A}||0)";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "1<{AB}||1";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);

    content = "0<{ABC}||0";
    number  = ALE::Evaluate(content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
}

static void TestALE15(TestHelper &helper) {
    double      number = -1;
    const char *content;
    bool        is_valid;
    ALEHelper   ale;

    ale.SetItems();

    content  = "{Q}   ==Qentem";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{QA}==     Qentem ALE";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "        {Q}   ==Qentem";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "             {QA}==     Qentem ALE";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{Q}==          Qentem";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{QA}==Qentem ALE        ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{Q}==    Qentem          ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "  {QA}    ==     Qentem ALE     ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{10}==Qentem";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{Q}!=Qentem";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{QA}!=Qentem ALE";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{2}!=Qentem ALE";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{Q}==Qente";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{QA}==ALE";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{Q}!=Qente";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{QA}!=ALE";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem   =={Q}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem ALE==     {QA}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "        Qentem   =={Q}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "             Qentem ALE==     {QA}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem==          {Q}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem ALE=={QA}        ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem==    {Q}          ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "  Qentem ALE    ==     {QA}     ";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem=={10}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "Qentem!={Q}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem ALE!={QA}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qentem ALE!={2}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "Qente=={Q}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "ALE=={QA}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 0, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "Qente!={Q}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "ALE!={QA}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);

    content  = "{A}==6";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.Equal(number, 1, "number", __LINE__);
    helper.EqualsTrue(is_valid, "is_valid", __LINE__);
}

static void TestALE16(TestHelper &helper) {
    double      number = -1;
    const char *content;
    bool        is_valid;
    ALEHelper   ale;

    ale.SetItems();

    content  = "2      {A}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "7{ABC}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{AB}3";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{AB} 9";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{AB}  &  5";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "(3)3";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "3(3)";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "4+3(7)";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "1!#1";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "1|#1";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{Q}=={A}+1";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "1U0=={A}+4";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{A}=={Q}";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);

    content  = "{A}==1U0";
    is_valid = ALE::Evaluate(number, content, &ale);
    helper.EqualsFalse(is_valid, "is_valid", __LINE__);
}

static int RunALETests() {
    TestHelper helper{"ALE.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("ALE Test 1", TestALE1);
    helper.Test("ALE Test 2", TestALE2);
    helper.Test("ALE Test 3", TestALE3);
    helper.Test("ALE Test 4", TestALE4);
    helper.Test("ALE Test 5", TestALE5);
    helper.Test("ALE Test 6", TestALE6);
    helper.Test("ALE Test 7", TestALE7);
    helper.Test("ALE Test 8", TestALE8);
    helper.Test("ALE Test 9", TestALE9);
    helper.Test("ALE Test 10", TestALE10);
    helper.Test("ALE Test 11", TestALE11);
    helper.Test("ALE Test 12", TestALE12);
    helper.Test("ALE Test 13", TestALE13);
    helper.Test("ALE Test 14", TestALE14);
    helper.Test("ALE Test 15", TestALE15);
    helper.Test("ALE Test 16", TestALE16);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
