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

#include "JSON.hpp"
#include "Template.hpp"
#include "TestHelper.hpp"

#ifndef QENTEM_TEMPLATEX_TESTS_H_
#define QENTEM_TEMPLATEX_TESTS_H_

namespace Qentem {
namespace Test {

static int TestVariableXTag1() {
    const wchar_t *content;

    Value<wchar_t> value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    Value<wchar_t> sub_value = JSON::Parse(
        LR"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = LR"({var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(A)", L"Render()");

    content = LR"({var:1})";
    EQ_VALUE(Template::Render(content, &value), LR"(abc)", L"Render()");

    content = LR"({var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(true)", L"Render()");

    content = LR"({var:3})";
    EQ_VALUE(Template::Render(content, &value), LR"(456)", L"Render()");

    content = LR"({var:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5)", L"Render()");

    content = LR"({var:5[0]})";
    EQ_VALUE(Template::Render(content, &value), LR"(null)", L"Render()");

    content = LR"({var:5[1]})";
    EQ_VALUE(Template::Render(content, &value), LR"(false)", L"Render()");

    content = LR"({var:5[2][0]})";
    EQ_VALUE(Template::Render(content, &value), LR"(Qentem)", L"Render()");

    //////

    content = LR"({var:key1})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(a)", L"Render()");

    content = LR"({var:key2})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(ABC)", L"Render()");

    content = LR"({var:key3})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(false)", L"Render()");

    content = LR"({var:key4})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(100)", L"Render()");

    content = LR"({var:key5})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(1.5)", L"Render()");

    content = LR"({var:key6[one]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(1)", L"Render()");

    content = LR"({var:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(null)", L"Render()");

    content = LR"({var:key7[1]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(false)", L"Render()");

    content = LR"({var:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(Qentem)", L"Render()");

    //
    content = LR"({var:6[key1]})";
    EQ_VALUE(Template::Render(content, &value), LR"(a)", L"Render()");

    content = LR"({var:6[key2]})";
    EQ_VALUE(Template::Render(content, &value), LR"(ABC)", L"Render()");

    content = LR"({var:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), LR"(false)", L"Render()");

    content = LR"({var:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), LR"(100)", L"Render()");

    content = LR"({var:6[key5]})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5)", L"Render()");

    content = LR"({var:6[key6][one]})";
    EQ_VALUE(Template::Render(content, &value), LR"(1)", L"Render()");

    ////////////////

    content = LR"(-{var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(-true)", L"Render()");

    content = LR"(-{var:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(-null)", L"Render()");

    content = LR"(-{var:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(-Qentem)", L"Render()");

    content = LR"(-{var:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), LR"(-false)", L"Render()");

    content = LR"(-{var:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), LR"(-100)", L"Render()");
    ////////////

    content = LR"({var:2}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(true-)", L"Render()");

    content = LR"({var:key7[0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(null-)", L"Render()");

    content = LR"({var:key7[2][0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(Qentem-)", L"Render()");

    content = LR"({var:6[key3]}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(false-)", L"Render()");

    content = LR"({var:6[key4]}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(100-)", L"Render()");

    ////////////

    content = LR"(-{var:2}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(-true-)", L"Render()");

    content = LR"(-{var:key7[0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(-null-)", L"Render()");

    content = LR"(-{var:key7[2][0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(-Qentem-)",
             L"Render()");

    content = LR"(-{var:6[key3]}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(-false-)", L"Render()");

    content = LR"(-{var:6[key4]}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(-100-)", L"Render()");

    ////////////

    content = LR"(------{var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(------true)", L"Render()");

    content = LR"(------{var:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(------null)",
             L"Render()");

    content = LR"(------{var:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(------Qentem)",
             L"Render()");

    content = LR"(------{var:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), LR"(------false)", L"Render()");

    content = LR"(------{var:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), LR"(------100)", L"Render()");

    ////////////

    content = LR"({var:2}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(true------)", L"Render()");

    content = LR"({var:key7[0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(null------)",
             L"Render()");

    content = LR"({var:key7[2][0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(Qentem------)",
             L"Render()");

    content = LR"({var:6[key3]}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(false------)", L"Render()");

    content = LR"({var:6[key4]}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(100------)", L"Render()");

    ////////////

    content = LR"(------{var:2}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(------true------)",
             L"Render()");

    content = LR"(------{var:key7[0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(------null------)",
             L"Render()");

    content = LR"(------{var:key7[2][0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(------Qentem------)",
             L"Render()");

    content = LR"(------{var:6[key3]}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(------false------)",
             L"Render()");

    content = LR"(------{var:6[key4]}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(------100------)",
             L"Render()");

    content = LR"({var:key7[2[0]})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:key7[2[0]})",
             L"Render()");

    content = LR"({var:6key3]})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:6key3]})",
             L"Render()");

    END_SUB_TEST;
}

static int TestVariableXTag2() {
    Value<wchar_t> value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5])");
    const wchar_t *content;

    content = LR"({var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(AA)", L"Render()");

    content = LR"({var:1}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(abcA)", L"Render()");

    content = LR"({var:1}{var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(abctrue)", L"Render()");

    content = LR"({var:2}{var:3}{var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(true456true)", L"Render()");

    content = LR"({var:4}{var:4}{var:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.51.51.5)", L"Render()");
    ///

    content = LR"({var:0}-{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(A-A)", L"Render()");

    content = LR"({var:1}--{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(abc--A)", L"Render()");

    content = LR"({var:1}---{var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(abc---true)", L"Render()");

    content = LR"({var:2}{var:3}--{var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(true456--true)",
             L"Render()");

    content = LR"({var:4}--{var:4}{var:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5--1.51.5)", L"Render()");

    content = LR"({var:4}--{var:4}--{var:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5--1.5--1.5)",
             L"Render()");

    content = LR"({var:4}---{var:4}---{var:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5---1.5---1.5)",
             L"Render()");

    END_SUB_TEST;
}

static int TestVariableXTag3() {
    Value<wchar_t> value;
    const wchar_t *content;

    content = LR"({var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0})", L"Render()");

    EQ_VALUE(Template::Render(LR"({var:a})", &value), LR"({var:a})",
             L"Render()");

    content = LR"({var:0[0]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({var:0[0]})", L"Render()");

    content = LR"({var:a[0]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({var:a[0]})", L"Render()");

    content = LR"({var:0[a]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({var:0[a]})", L"Render()");

    content = LR"({var:a[abc]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({var:a[abc]})", L"Render()");

    ////////////////

    value = JSON::Parse(LR"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = LR"({var:0})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({var:0})", L"Render()");

    content = LR"({var:0[0]})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0[0]})", L"Render()");

    content = LR"({var:0[0][0]})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0[0][0]})",
             L"Render()");

    /////

    content = LR"({var:1})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:1})", L"Render()");

    content = LR"({var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:2})", L"Render()");

    content = LR"({var:1[a]})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:1[a]})", L"Render()");

    content = LR"({var:1[b]})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:1[b]})", L"Render()");

    ////

    value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5])");

    content = LR"({var:0)";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0)", L"Render()");

    content = LR"(var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(var:0})", L"Render()");

    content = LR"({v})";
    EQ_VALUE(Template::Render(content, &value), LR"({v})", L"Render()");

    content = LR"({va})";
    EQ_VALUE(Template::Render(content, &value), LR"({va})", L"Render()");

    content = LR"({var})";
    EQ_VALUE(Template::Render(content, &value), LR"({var})", L"Render()");

    content = LR"({var:})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:})", L"Render()");

    content = LR"({v:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({v:0})", L"Render()");

    content = LR"({va:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({va:0})", L"Render()");
    ////

    content = LR"({var:0{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0{var:0})",
             L"Render()");

    content = LR"(var:0{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(var:0A)", L"Render()");

    content = LR"(var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(var:0}A)", L"Render()");

    content = LR"({var:0{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0{var:0}A)",
             L"Render()");

    ////

    content = LR"({var:0{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0{var:0})",
             L"Render()");

    content = LR"(var:0{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(var:0A)", L"Render()");

    content = LR"(var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(var:0}A)", L"Render()");

    content = LR"({var:0{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0{var:0}A)",
             L"Render()");

    ////

    content = LR"({{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({A)", L"Render()");

    content = LR"({{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({AA)", L"Render()");

    content = LR"({v{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({vA)", L"Render()");

    content = LR"({v{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({vAA)", L"Render()");

    content = LR"({va{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({vaA)", L"Render()");

    content = LR"({va{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({vaAA)", L"Render()");

    content = LR"({var{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({varA)", L"Render()");

    content = LR"({var{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({varAA)", L"Render()");

    ///

    content = LR"({-{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({-A)", L"Render()");

    content = LR"({-{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({-AA)", L"Render()");

    content = LR"({v-{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({v-A)", L"Render()");

    content = LR"({v-{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({v-AA)", L"Render()");

    content = LR"({va-{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({va-A)", L"Render()");

    content = LR"({va-{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({va-AA)", L"Render()");

    content = LR"({var-{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var-A)", L"Render()");

    content = LR"({var-{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var-AA)", L"Render()");

    //

    content = LR"({var-0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var-0})", L"Render()");

    content = LR"({var 0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var 0})", L"Render()");

    content = LR"({var:0 })";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0 })", L"Render()");

    content = LR"({var:0 )";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0 )", L"Render()");

    content = LR"({var:0)";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0)", L"Render()");

    END_SUB_TEST;
}

static int TestMathXTag1() {
    Value<wchar_t> value;

    value[L"a1"] = 5;
    value[L"a2"] = true;
    value[L"a3"] = nullptr;
    value[L"a4"] = false;
    value[L"a5"] = L"10";
    value[L"a6"] = L"20";
    value[L"a7"] = 6;
    value[L"a8"] = 1;
    value[L"a9"] = L"1";
    value[L"eq"] = L"(8+1+{var:a8})";

    EQ_VALUE(Template::Render(L"{math:1+1}", &value), L"2", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a1}+8}", &value), LR"(13)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a7}+8}", &value), LR"(14)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a7}+{var:a1}}", &value), LR"(11)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a5}+{var:a1}}", &value), LR"(15)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a1}+{var:a5}}", &value), LR"(15)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a6}+{var:a5}}", &value), LR"(30)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a6}*{var:a2}}", &value), LR"(20)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a6}*{var:a4}}", &value), LR"(0)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a6}*{var:a7}}", &value), LR"(120)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a7}+{var:a6}}", &value), LR"(26)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a1}+{var:a2}}", &value), LR"(6)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a8}=={var:a2}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a2}=={var:a8}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a5}!={var:a1}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a2}!={var:a4}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a2}==true}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a9}=={var:a8}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a8}=={var:a9}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:1=={var:a8}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:6-5==({var:a9})}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:6-5==({var:a8})}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:null!={var:a3}}", &value), LR"(0)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:({var:a3})==(0)}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a1}+{var:a7}}", &value), L"11",
             L"Render()");

    EQ_VALUE(Template::Render(
                 L"{math:{var:a1}+{var:a7}}{math:{var:a1}+{var:a7}}", &value),
             L"1111", L"Render()");

    EQ_VALUE(Template::Render(
                 L"{math:{var:a1}+{var:a7}}*{math:{var:a1}+{var:a7}}", &value),
             L"11*11", L"Render()");

    EQ_VALUE(Template::Render(
                 L"{math:{var:a1}+{var:a7}}##{math:{var:a1}+{var:a7}}", &value),
             L"11##11", L"Render()");

    EQ_VALUE(Template::Render(
                 L"{math:{var:a1}+{var:a7}}&&&%%^^&&*{math:{var:a1}+{var:a7}}",
                 &value),
             L"11&&&%%^^&&*11", L"Render()");

    ///////////////////

    EQ_VALUE(Template::Render(L"{math: {var:a1}+8}", &value), LR"(13)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:  {var:a7}+8}", &value), LR"(14)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:     {var:a7}+{var:a1}}", &value),
             LR"(11)", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a5} +{var:a1}}", &value), LR"(15)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a1}  +{var:a5}}", &value), LR"(15)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a6}    +{var:a5}}", &value),
             LR"(30)", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a6}* {var:a2}}", &value), LR"(20)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a6}*  {var:a4}}", &value), LR"(0)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a6}*      {var:a7}}", &value),
             LR"(120)", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a7}+{var:a6} }", &value), LR"(26)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a1}+{var:a2}  }", &value), LR"(6)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a8}=={var:a2}      }", &value),
             LR"(1)", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a2}=={var:a8}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math: {var:a5}!={var:a1} }", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:  {var:a2}!={var:a4}  }", &value),
             LR"(1)", L"Render()");

    EQ_VALUE(Template::Render(L"{math:    1=={var:a9}     }", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a9} == {var:a8}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:a8}  ==  {var:a9}}", &value),
             LR"(1)", L"Render()");

    EQ_VALUE(Template::Render(L"{math:1==          {var:a8}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:6-5         ==1}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(
        Template::Render(L"{math:1          ==            {var:a8}}", &value),
        LR"(1)", L"Render()");

    EQ_VALUE(
        Template::Render(L"{math:     0     !=    ({var:a3})        }", &value),
        LR"(0)", L"Render()");

    EQ_VALUE(
        Template::Render(L"{math:{var:a3}       ==       null     }", &value),
        LR"(1)", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:eq}}", &value), LR"(10)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:10==(8+1+{var:a8})}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:eq}==9+1}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:(5*2)=={var:eq}}", &value), LR"(1)",
             L"Render()");

    //////////////
    value.Reset();

    value += 5;
    value += true;
    value += nullptr;
    value += false;
    value += L"10";
    value += L"20";
    value += 6;
    value += 1;
    value += L"1";
    value += L"Qentem";

    EQ_VALUE(Template::Render(L"{math:{var:0}+8}", &value), LR"(13)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:6}+8}", &value), LR"(14)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:6}+{var:0}}", &value), LR"(11)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:4}+{var:0}}", &value), LR"(15)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:0}+{var:4}}", &value), LR"(15)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:5}+{var:4}}", &value), LR"(30)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:5}*{var:1}}", &value), LR"(20)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:5}*{var:3}}", &value), LR"(0)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:5}*{var:6}}", &value), LR"(120)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:6}+{var:5}}", &value), LR"(26)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:0}+{var:1}}", &value), LR"(6)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:7}=={var:1}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:1}=={var:7}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:4}!={var:0}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:1}!={var:3}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:({var:1})==({var:8})}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"-{math:{var:8}=={var:7}}", &value), LR"(-1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"--{math:{var:7}=={var:8}}", &value), LR"(--1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"---{math:1=={var:7}}", &value), LR"(---1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:1==({var:8})}-", &value), LR"(1-)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:6-5==({var:7})}--", &value), LR"(1--)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:0==({var:2})}---", &value), LR"(1---)",
             L"Render()");

    EQ_VALUE(Template::Render(L"-{math:{var:2}!=null}-", &value), LR"(-0-)",
             L"Render()");

    EQ_VALUE(Template::Render(L"--{math:Qente=={var:9}}--", &value),
             LR"(--0--)", L"Render()");

    EQ_VALUE(Template::Render(L"---{math:Qente !={var:9}}---", &value),
             LR"(---1---)", L"Render()");

    EQ_VALUE(Template::Render(L"{math:  Qentem   =={var:9}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:  Qentem!={var:9}}", &value), LR"(0)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:9}   ==    Qente}", &value), LR"(0)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:9} !=    Qente    }", &value),
             LR"(1)", L"Render()");

    EQ_VALUE(Template::Render(L"{math:  {var:9}   ==Qentem}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math: {var:9} !=Qentem}", &value), LR"(0)",
             L"Render()");

    /////////

    EQ_VALUE(Template::Render(L"{math: true == {var:1}}", &value), LR"(1)",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math: {var:1} == true}", &value), LR"(1)",
             L"Render()");

    END_SUB_TEST;
}

static int TestMathXTag2() {
    Value<wchar_t> value;

    value[0] = Array<Value<wchar_t>>();
    value[1] = HArray<Value<wchar_t>, wchar_t>();
    value[2] = 5;

    EQ_VALUE(Template::Render(L"{math:{var:0}+8}", &value), L"{math:{var:0}+8}",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:1}+8}", &value),
             LR"({math:{var:1}+8})", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:2}+{var:0}}", &value),
             LR"({math:{var:2}+{var:0}})", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:2}+{var:0}}", &value),
             LR"({math:{var:2}+{var:0}})", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:0}+{var:1}}", &value),
             L"{math:{var:0}+{var:1}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:1}+{var:2}}", &value),
             L"{math:{var:1}+{var:2}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:2}*{var:1}}", &value),
             L"{math:{var:2}*{var:1}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:0}*{var:1}}", &value),
             L"{math:{var:0}*{var:1}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:0}*{var:2}}", &value),
             L"{math:{var:0}*{var:2}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:2}+{var:0}}", &value),
             L"{math:{var:2}+{var:0}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:1}+{var:2}}", &value),
             L"{math:{var:1}+{var:2}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:0}=={var:1}}", &value),
             L"{math:{var:0}=={var:1}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:1}=={var:0}}", &value),
             L"{math:{var:1}=={var:0}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:0}!={var:2}}", &value),
             L"{math:{var:0}!={var:2}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:2}!={var:0}}", &value),
             L"{math:{var:2}!={var:0}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:1}=={var:2}}", &value),
             L"{math:{var:1}=={var:2}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:8}=={var:7}}", &value),
             L"{math:{var:8}=={var:7}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:7}=={var:2}}", &value),
             L"{math:{var:7}=={var:2}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:1=={var:7}}", &value),
             L"{math:1=={var:7}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:(6-5)=={var:8}}", &value),
             L"{math:(6-5)=={var:8}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:(6-5)=={var:0}}", &value),
             L"{math:(6-5)=={var:0}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:0}=={var:8}}", &value),
             L"{math:{var:0}=={var:8}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:0}=={var:0}}", &value),
             L"{math:{var:0}=={var:0}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:0=={var:1}}", &value),
             L"{math:0=={var:1}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:1}!=0}", &value),
             L"{math:{var:1}!=0}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:W={var:0}}", &value), L"{math:W={var:0}}",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:W=={var:0}}", &value),
             L"{math:W=={var:0}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:2}==c}", &value),
             L"{math:{var:2}==c}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:abc=={var:2}}", &value),
             L"{math:abc=={var:2}}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:sds}", &value), L"{math:sds}",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:1", &value), L"{math:1", L"Render()");

    EQ_VALUE(Template::Render(L"math:1}", &value), L"math:1}", L"Render()");

    EQ_VALUE(Template::Render(L"{math:{var:2}", &value), L"{math:{var:2}",
             L"Render()");

    EQ_VALUE(Template::Render(L"{{math:{var:2}+5}", &value), L"{10",
             L"Render()");

    EQ_VALUE(Template::Render(L"{m{var:2}}", &value), L"{m5}", L"Render()");

    EQ_VALUE(Template::Render(L"{ma{var:2}}", &value), L"{ma5}", L"Render()");

    EQ_VALUE(Template::Render(L"{mat{var:2}}", &value), L"{mat5}", L"Render()");

    EQ_VALUE(Template::Render(L"{math{var:2}}", &value), L"{math5}",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math {var:2}}", &value), L"{math 5}",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:}", &value), L"{math:}", L"Render()");

    EQ_VALUE(Template::Render(L"{math-{var:2}}{math:{var:2}+5}", &value),
             L"{math-5}10", L"Render()");

    EQ_VALUE(Template::Render(L"{math-4}{math:{var:2}+5}", &value),
             L"{math-4}10", L"Render()");

    EQ_VALUE(Template::Render(L"{math-4} {math:{var:2}+5}", &value),
             L"{math-4} 10", L"Render()");

    EQ_VALUE(Template::Render(L"{math:4", &value), L"{math:4", L"Render()");

    EQ_VALUE(Template::Render(L"{math:4    ", &value), L"{math:4    ",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:A==1+1}", &value), L"{math:A==1+1}",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:(A)!=1+1}", &value), L"{math:(A)!=1+1}",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:1+1==A}", &value), L"{math:1+1==A}",
             L"Render()");

    EQ_VALUE(Template::Render(L"{math:1+1!=(A)}", &value), L"{math:1+1!=(A)}",
             L"Render()");

    END_SUB_TEST;
}

static int TestInlineIfXTag() {
    Value<wchar_t> value;
    const wchar_t *content;

    value += 0;
    value += 1;
    value += -1;
    value += true;
    value += false;
    value += nullptr;
    value += L"ABC";
    value += Array<Value<wchar_t>>();

    content = LR"({if case="0" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"F", L"Render()");

    content = LR"({if case="-1" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"F", L"Render()");

    content = LR"({if case="0.1" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content = LR"({if case="1" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    ////

    content = LR"({if case="0" true="T"})";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"({if case="-1" true="T"})";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"({if case="0.1" true="T"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content = LR"({if case="1" true="T"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    ///

    content = LR"({if case="0" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"F", L"Render()");

    content = LR"({if case="-1" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"F", L"Render()");

    content = LR"({if case="0.1" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"({if case="1" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    /////

    content = LR"({if case="{var:0}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"F", L"Render()");

    content = LR"({if case="{var:2}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"F", L"Render()");

    content = LR"({if case="{var:4}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"F", L"Render()");

    content = LR"({if case="{var:5}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"F", L"Render()");

    content = LR"({if case="{var:6}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"({if case="fas" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"({if case="{var:7}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"({if case="{var:20}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    ////
    content = LR"({if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content = LR"({if case="{var:3}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content = LR"({if case="{var:6}==ABC" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    /////////////////

    content = LR"({if case=" {var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content = LR"({if case="{var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content = LR"({if case=" {var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content = LR"({if case="  {var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content = LR"({if case="{var:1}  " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content = LR"({if case="  {var:1}  " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content = LR"({if case="      {var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content = LR"({if case="{var:1}          " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content = LR"({if case="    {var:1}        " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");
    ////

    content = LR"(-{if case=" {var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"-T", L"Render()");

    content = LR"({if case=" {var:1} " true="T" false="F"}-)";
    EQ_VALUE(Template::Render(content, &value), L"T-", L"Render()");

    content = LR"(-{if case=" {var:1} " true="T" false="F"}-)";
    EQ_VALUE(Template::Render(content, &value), L"-T-", L"Render()");

    content = LR"(--{if case=" {var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"--T", L"Render()");

    content = LR"({if case=" {var:1} " true="T" false="F"}--)";
    EQ_VALUE(Template::Render(content, &value), L"T--", L"Render()");

    content = LR"(--{if case=" {var:1} " true="T" false="F"}--)";
    EQ_VALUE(Template::Render(content, &value), L"--T--", L"Render()");

    content = LR"(---{if case=" {var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"---T", L"Render()");

    content = LR"({if case=" {var:1} " true="T" false="F"}---)";
    EQ_VALUE(Template::Render(content, &value), L"T---", L"Render()");

    content = LR"(---{if case=" {var:1} " true="T" false="F"}---)";
    EQ_VALUE(Template::Render(content, &value), L"---T---", L"Render()");

    content =
        LR"({if case="1" true="T"}{if case="0" false="F"}{if case="1" true="T"})";
    EQ_VALUE(Template::Render(content, &value), L"TFT", L"Render()");

    ///////

    content =
        LR"({if case="{var:7}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content = LR"({if case="01" true="{var:3}" false="{var:4}"}--)";
    EQ_VALUE(Template::Render(content, &value), L"--", L"Render()");

    content = LR"({if case="" true="c" false="d"})";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"({i)";
    EQ_VALUE(Template::Render(content, &value), L"{i", L"Render()");

    content = LR"({if)";
    EQ_VALUE(Template::Render(content, &value), L"{if", L"Render()");

    content = LR"({if})";
    EQ_VALUE(Template::Render(content, &value), L"{if}", L"Render()");

    content = LR"({{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"{T", L"Render()");

    content = LR"({i{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"{iT", L"Render()");

    content = LR"({if{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), L"T", L"Render()");

    content =
        LR"({if{if case="1" true="T" false="F"}}{if case="1" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T}T)", L"Render()");

    content =
        LR"({if{if case="{var:1}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(TT)", L"Render()");

    /////
    content = LR"({if case="0" true="{var:3}" false="{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), L"false", L"Render()");

    content = LR"({if case="1" true="{var:3}" false="{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), L"true", L"Render()");

    content = LR"({if case="0" true="{var:3}{var:3}" false="{var:4}{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), L"falsefalse", L"Render()");

    content =
        LR"({if case="1000" true="{var:3}{var:3}" false="{var:4}{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), L"truetrue", L"Render()");

    content =
        LR"({if case="0" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), L"false---false", L"Render()");

    content =
        LR"({if case="1" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), L"true---true", L"Render()");

    content = LR"({if case="0" true="{var:10}" false="{var:20}"})";
    EQ_VALUE(Template::Render(content, &value), L"{var:20}", L"Render()");

    content = LR"({if case="1" true="{var:10}" false="{var:20}"})";
    EQ_VALUE(Template::Render(content, &value), L"{var:10}", L"Render()");

    content = LR"({if case="1" true="1" false="0")";
    EQ_VALUE(Template::Render(content, &value),
             LR"({if case="1" true="1" false="0")", L"Render()");

    content = LR"({if case="1" true="1" false="0")";
    EQ_VALUE(Template::Render(content, &value),
             LR"({if case="1" true="1" false="0")", L"Render()");

    END_SUB_TEST;
}

static int TestLoopXTag1() {
    Value<wchar_t> value;
    const wchar_t *content;

    value += 100;
    value += -50;
    value += L"Qentem";
    value += true;
    value += false;
    value += nullptr;
    value += 3;

    content = LR"(<loop repeat="10">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"AAAAAAAAAA", L"Render()");

    content = LR"(<loop             repeat="1">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"A", L"Render()");

    content = LR"(<loop repeat="3"         >ABC</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"ABCABCABC", L"Render()");

    content = LR"(-<loop repeat="3">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"-AAA", L"Render()");

    content = LR"(<loop repeat="3">A</loop>-)";
    EQ_VALUE(Template::Render(content, &value), L"AAA-", L"Render()");

    content = LR"(-<loop repeat="3">A</loop>-)";
    EQ_VALUE(Template::Render(content, &value), L"-AAA-", L"Render()");

    content = LR"(--<loop repeat="3">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"--AAA", L"Render()");

    content = LR"(<loop repeat="3">A</loop>--)";
    EQ_VALUE(Template::Render(content, &value), L"AAA--", L"Render()");

    content = LR"(--<loop repeat="3">A</loop>--)";
    EQ_VALUE(Template::Render(content, &value), L"--AAA--", L"Render()");

    content = LR"(---<loop repeat="3">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"---AAA", L"Render()");

    content = LR"(<loop repeat="3">A</loop>---)";
    EQ_VALUE(Template::Render(content, &value), L"AAA---", L"Render()");

    content = LR"(---<loop repeat="3">A</loop>---)";
    EQ_VALUE(Template::Render(content, &value), L"---AAA---", L"Render()");

    content = LR"(<loop repeat="2">A</loop><loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"AABBB", L"Render()");

    content = LR"(<loop repeat="2">A</loop>-<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"AA-BBB", L"Render()");

    content = LR"(<loop repeat="2">A</loop>--<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"AA--BBB", L"Render()");

    content = LR"(<loop repeat="2">A</loop>---<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"AA---BBB", L"Render()");

    content =
        LR"(<loop repeat="2">A</loop>            <loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"AA            BBB",
             L"Render()");

    ////

    content =
        LR"(<loop repeat="4">CC</loop><loop repeat="2">A</loop><loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"CCCCCCCCAABBB", L"Render()");

    content =
        LR"(<loop repeat="4">CC</loop>-<loop repeat="2">A</loop>-<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"CCCCCCCC-AA-BBB",
             L"Render()");

    content =
        LR"(<loop repeat="4">CC</loop>--<loop repeat="2">A</loop>--<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"CCCCCCCC--AA--BBB",
             L"Render()");

    content =
        LR"(<loop repeat="4">CC</loop>---<loop repeat="2">A</loop>---<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"CCCCCCCC---AA---BBB",
             L"Render()");

    content =
        LR"(<loop repeat="4">CC</loop>     <loop repeat="2">A</loop>            <loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value),
             L"CCCCCCCC     AA            BBB", L"Render()");

    ////////////////

    content = LR"(<loop repeat="6"value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value),
             L"100, -50, Qentem, true, false, null, ", L"Render()");

    content =
        LR"(<loop repeat="6" value="loop1-value">loop1-value, loop1-value </loop>)";
    EQ_VALUE(
        Template::Render(content, &value),
        L"100, 100 -50, -50 Qentem, Qentem true, true false, false null, null ",
        L"Render()");

    content = LR"(<loop index="2" repeat="4" value="loop1-value">loop1-value{if
        case="loop1-value != null" true=", "}</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"Qentem, true, false, null",
             L"Render()");

    ////////////////

    value.Reset();
    value += 0;
    value += 1;
    value += 2;

    content = LR"(<loop repeat="3" value="loop1-value"><loop
        repeat="2" value="loop2-value">(loop1-value: loop2-value) </loop></loop>)";
    EQ_VALUE(Template::Render(content, &value),
             L"(0: 0) (0: 1) (1: 0) (1: 1) (2: 0) (2: 1) ", L"Render()");

    content =
        LR"(<loop repeat="2" value="loop1-value"><loop repeat="2" value="loop2-value"><loop
        repeat="2" value="loop3-value">(loop1-value: loop2-value: loop3-value) </loop></loop></loop>)";
    EQ_VALUE(
        Template::Render(content, &value),
        L"(0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1) ",
        L"Render()");

    END_SUB_TEST;
}

static int TestLoopXTag2() {
    Value<wchar_t> value1;
    Value<wchar_t> value2;
    Value<wchar_t> value3;
    const wchar_t *content;

    value1 += 100;
    value1 += -50;
    value1 += L"A";
    value1 += true;
    value1 += false;
    value1 += nullptr;

    value2[L"k-1"] = 4;
    value2[L"k-2"] = 1.5;
    value2[L"k-3"] = L"ABC";
    value2[L"k-4"] = true;
    value2[L"k-5"] = false;
    value2[L"k-6"] = nullptr;

    //////////////////////
    value3[L"arr1"] = value1;

    content = LR"(<loop value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value1),
             L"100, -50, A, true, false, null, ", L"Render()");

    content = LR"(<loop value="loop1-value" index="3">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value2), L"true, false, null, ",
             L"Render()");

    content = LR"(<loop value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value2),
             L"4, 1.5, ABC, true, false, null, ", L"Render()");

    content = LR"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             L"100, -50, A, true, false, null, ", L"Render()");

    value3[L"arr1"] = value2;

    content = LR"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             L"4, 1.5, ABC, true, false, null, ", L"Render()");

    //////////////////////
    value3[L"arr1"] = value1;

    content = LR"(<loop value="loop1-value" >loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value1),
             L"100, -50, A, true, false, null, ", L"Render()");

    content = LR"(<loop value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value2),
             L"4, 1.5, ABC, true, false, null, ", L"Render()");

    content = LR"(<loop value="loop1-value"set="arr1">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             L"100, -50, A, true, false, null, ", L"Render()");

    value3[L"arr1"] = value2;

    content = LR"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             L"4, 1.5, ABC, true, false, null, ", L"Render()");

    //////////////////////
    value3.Reset();
    value3[L"arr1"][L"arr2"][L"arr3"] = value1;

    content =
        LR"(<loop set="arr1[arr2][arr3]" value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             L"100, -50, A, true, false, null, ", L"Render()");

    value3.Reset();
    value3[0][0][0] = value2;

    content = LR"(<loop set="0[0][0]"value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             L"4, 1.5, ABC, true, false, null, ", L"Render()");

    value3.Reset();
    value3[L"k1"][0][L"k3"] = value1;

    content =
        LR"(<loop value="loop1-value" set="k1[0][k3]">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             L"100, -50, A, true, false, null, ", L"Render()");

    value3.Reset();
    value3[0][L"k2"][0] = value2;

    content =
        LR"(<loop set="0[k2][0]"key="loop1-key"value="loop1-value">loop1-value, loop1-value, </loop>)";
    EQ_VALUE(
        Template::Render(content, &value3),
        LR"(4, 4, 1.5, 1.5, ABC, ABC, true, true, false, false, null, null, )",
        L"Render()");

    value3 = JSON::Parse(LR"({"group":[[10],[20],[30]]})");

    content =
        LR"(<loop set="group" value="_Val1"><loop set="_Val1" value="_Val2">_Val2</loop></loop>)";
    EQ_VALUE(Template::Render(content, &value3), L"102030", L"Render()");

    value3 = JSON::Parse(LR"({"group":[1,2,3,4]})");

    content = LR"(<loop set="group" value="_Val" repeat="1">_Val</loop>)";
    EQ_VALUE(Template::Render(content, &value3), L"1", L"Render()");

    content = LR"(<loop set="group" value="_Val" index="3">_Val</loop>)";
    EQ_VALUE(Template::Render(content, &value3), L"4", L"Render()");

    content =
        LR"(<loop index="2" repeat="1" set="group" value="_Val">_Val</loop>)";
    EQ_VALUE(Template::Render(content, &value3), L"3", L"Render()");

    value3  = JSON::Parse(LR"({"numbers":[1,2,3,4,5,6,7,8]})");
    content = LR"(A<loop set="numbers" value="t">t</loop>B)";
    EQ_VALUE(Template::Render(content, &value3), L"A12345678B", L"Render()");

    content = LR"(<loop set="numbers" value="t">t[0]</loop>)";
    EQ_VALUE(Template::Render(content, &value3), L"", L"Render()");

    END_SUB_TEST;
}

static int TestLoopXTag3() {
    Value<wchar_t> value;
    const wchar_t *content;

    content = LR"(<loop></loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"(<loop>abcd</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"(<l</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"<l</loop>", L"Render()");

    content = LR"(<lo</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"<lo</loop>", L"Render()");

    content = LR"(<loo</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"<loo</loop>", L"Render()");

    content = LR"(<loop></loop><loop repeat="2">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"AA", L"Render()");

    content = LR"(<loop key="a">a</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"(<loop value="a">a</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"(<loop set="ss" value="a">a</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"(<loop set="" value="a">a</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"(<loop repeat="2" A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"(<loop repeat="OOO">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"(<loop repeat="{var:10}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    value[L"in"] = Array<Value<wchar_t>>();

    content = LR"(<loop repeat="{var:in}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"(<loop repeat="10" index="{var:in}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"(<loop repeat="10" index="{var:in">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"(<loop repeat="10" index="{var:100}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"(<loop repeat="10" index="O">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content = LR"(<loop value="v">v</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    value.Reset();
    value[L"in"] = 2;

    content = LR"(<loop repeat="{var:in}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"AA", L"Render()");

    /////
    value.Reset();
    value[L"k1"] = 10;
    value[L"k2"] = 20;
    value[L"k3"] = 30;

    value[L"k2"].Reset();

    content = LR"(<loop value="v">v</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"1030", L"Render()");

    value.RemoveIndex(1);

    content = LR"(<loop value="v">v)";
    EQ_VALUE(Template::Render(content, &value), LR"(<loop value="v">v)",
             L"Render()");

    content = LR"(<loop value="v">v     )";
    EQ_VALUE(Template::Render(content, &value), LR"(<loop value="v">v     )",
             L"Render()");

    value.Reset();
    value += 10;
    value += 20;
    value += 30;

    value.RemoveIndex(1);

    content = LR"(<loop value="v">v</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"1030", L"Render()");

    value = JSON::Parse(LR"(
{
    "object": [
        {
            "var1": "value1",
            "var2": "value2",
            "var3": "value3",
            "var4": "value4"
        }
    ],
    "array": [
        [
            "value10",
            "value20",
            "value30",
            "value40"
        ]
    ]
}
    )");

    content =
        LR"(<loop set="object" value="item">item[var1]item[var2]item[var3] item[var4]</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"value1value2value3 value4",
             L"Render()");

    content =
        LR"(<loop set="array" value="item"> item[0] item[1] item[2] item[3] </loop>)";
    EQ_VALUE(Template::Render(content, &value),
             L" value10 value20 value30 value40 ", L"Render()");

    content =
        LR"(<loop set="object" value="item">item[var11]item[var22]item[var33] item[var44]</loop>)";
    EQ_VALUE(Template::Render(content, &value), L" ", L"Render()");

    content =
        LR"(<loop set="array" value="item">item[var11]item[var22]item[var33] item[var44]</loop>)";
    EQ_VALUE(Template::Render(content, &value), L" ", L"Render()");

    content = LR"(<loop repeat="1"><l</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"<l", L"Render()");

    END_SUB_TEST;
}

static int TestLoopXTag4() {
    constexpr unsigned int size_4 = (8 * 4);

    StringStream<wchar_t> content;
    StringStream<wchar_t> output;
    String<wchar_t>       str;
    Value<wchar_t>        value;

    for (unsigned int i = 0; i < size_4; i++) {
        value += i;
    }

    content += LR"(<loop repeat="1">)";
    for (unsigned int i = 0; i < size_4; i++) {
        content += L"{var:";
        str = Digit<wchar_t>::NumberToString(i);
        content += str;
        content += L"}";

        output += str;
    }
    content += LR"(</loop>)";

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        L"Render()");

    //////////////////////

    content.Clear();
    output.Clear();

    content += LR"(<loop value="loop1-value">A loop1-value B</loop>)";
    for (unsigned int i = 0; i < size_4; i++) {
        output += L"A ";
        Digit<wchar_t>::NumberToStringStream(output, i);
        output += L" B";
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        L"Render()");

    END_SUB_TEST;
}

static int TestIfXTag1() {
    Value<wchar_t> value;
    const wchar_t *content;

    value[L"name"] = L"Qen";
    value[L"t"]    = true;
    value[L"f"]    = false;
    value[L"n"]    = nullptr;
    value[L"1"]    = 1;
    value[L"one"]  = L"1";
    value[L"zero"] = 0;

    content = LR"(<if case="1>0">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), L"Qen", L"Render()");

    content = LR"(#<if case="{var:one}">{var:name}</if>#)";
    EQ_VALUE(Template::Render(content, &value), L"#Qen#", L"Render()");

    content = LR"(##<if case="{var:zero}">{var:name}</if>##)";
    EQ_VALUE(Template::Render(content, &value), L"####", L"Render()");

    content = LR"(<if case="{var:1}">{var:name}1<else />{var:name}2</if>)";
    EQ_VALUE(Template::Render(content, &value), L"Qen1", L"Render()");

    content = LR"(<if case="{var:f}">{var:name}1<else />{var:name}2</if>)";
    EQ_VALUE(Template::Render(content, &value), L"Qen2", L"Render()");

    content = LR"(#<if case="{var:t}">G<if case="1">oo</if>d</if>!#)";
    EQ_VALUE(Template::Render(content, &value), L"#Good!#", L"Render()");

    content =
        LR"(###<if case="1">G<if case="1">o</if>o<if case="1">d!</if></if>###)";
    EQ_VALUE(Template::Render(content, &value), L"###Good!###", L"Render()");

    content =
        LR"(Be <if case="1">G<if case="1">oo<if case="1">d</if></if></if>!)";
    EQ_VALUE(Template::Render(content, &value), L"Be Good!", L"Render()");

    content = LR"(<if case="1">Good!<elseif case="0" />Bad!</if>)";
    EQ_VALUE(Template::Render(content, &value), L"Good!", L"Render()");

    content = LR"(<if case="{var:n}">Bad!<elseif case="1" />Good!</if>#)";
    EQ_VALUE(Template::Render(content, &value), L"Good!#", L"Render()");

    content = LR"(<if case="{var:f}">Bad!<elseif case="0" />Very Bad!</if>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content =
        LR"(#<if case="0">Bad!<elseif case="0" />Very Bad!<else />Very Good!</if>)";
    EQ_VALUE(Template::Render(content, &value), L"#Very Good!", L"Render()");

    content = LR"(<if case="1">a<else />b</if>)";
    EQ_VALUE(Template::Render(content, &value), L"a", L"Render()");

    content = LR"(<if case="0">a<else />b</if>)";
    EQ_VALUE(Template::Render(content, &value), L"b", L"Render()");

    content = LR"(<if case="0">a<else /><if case="1">b</if></if>)";
    EQ_VALUE(Template::Render(content, &value), L"b", L"Render()");

    content = LR"(<if case="1">a<else /><if case="1">b</if></if>)";
    EQ_VALUE(Template::Render(content, &value), L"a", L"Render()");

    content = LR"(<if case="0">a<if case="1">b</if>c</if>Empty)";
    EQ_VALUE(Template::Render(content, &value), L"Empty", L"Render()");

    content = LR"(<if case="1">a<else /><if case="1">b</if>c</if>===========)";
    EQ_VALUE(Template::Render(content, &value), L"a===========", L"Render()");

    content = LR"(===========<if case="1">a<if case="1">b</if><else />c</if>)";
    EQ_VALUE(Template::Render(content, &value), L"===========ab", L"Render()");

    content =
        LR"(<if case="1">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    EQ_VALUE(Template::Render(content, &value), L"a", L"Render()");

    content =
        LR"(<if case="0">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    EQ_VALUE(Template::Render(content, &value), L"b", L"Render()");

    content =
        LR"(===========<if case="0">a<elseif case="0" />b<elseif case="1" />c<else />d</if>===========)";
    EQ_VALUE(Template::Render(content, &value), L"===========c===========",
             L"Render()");

    content =
        LR"(<if case="0">a<elseif case="0" />b<elseif case="0" />c<else />d</if>)";
    EQ_VALUE(Template::Render(content, &value), L"d", L"Render()");

    content = LR"(<if case="1">a<if case="0">b<elseif case="0"/>c</if></if>)";
    EQ_VALUE(Template::Render(content, &value), L"a", L"Render()");

    content = LR"(<if case="1">a<if case="1">b<elseif case="0"/>c</if></if>)";
    EQ_VALUE(Template::Render(content, &value), L"ab", L"Render()");

    content =
        LR"(<if case="1"><if case="1">b<elseif case="c"/>c</if>a<else />c</if>)";
    EQ_VALUE(Template::Render(content, &value), L"ba", L"Render()");

    content =
        LR"(<if case="0">a<else />c<if case="1">b<elseif case="0"/>c</if></if>)";
    EQ_VALUE(Template::Render(content, &value), L"cb", L"Render()");

    content =
        LR"(<if_case="1"><if case="0">Bad1!<elseif case="0" />Bad2!</if>a</if>)";
    EQ_VALUE(Template::Render(content, &value), L"a", L"Render()");

    content = LR"(<if case="1">
                <if case="0">Bad1!
                <elseif case="0" />Bad2!
                <elseif case="0" />Bad3!
                </if>a<elseif case="1" />b
                <if case="1">Bad1!
                <elseif case="1"/>Bad2!
                <elseif case="1" />Bad3!
                </if>
            <elseif case="1" />
                <if case="1">Bad1!
                <elseif case="1" />Bad2!
                <elseif case="1" />Bad3!
                </if>c
            <else />d
                <if case="1">Bad1!
                <elseif case="1" />Bad2!
                <elseif case="1" />Bad3!
                </if>
            </if>)";
    EQ_VALUE(
        String<wchar_t>::Trim(Template::Render(content, &value).GetString()),
        L"a", L"Render()");

    content = LR"(<if case="0">a
                <if case="1">Bad1!
                <elseif case="1" />Bad2!
                <else/>Bad3!
                </if>
            <elseif case="1" />
                <if case="0">Bad1!
                <elseif case="0"/>Bad2!
                <elseif case="0" />Bad3!
                </if>b
            <elseif case="1" />c
                <if case="1">Bad1!
                <elseif case="1" />Bad2!
                <elseif case="1" />Bad3!
                </if>
            <else />
                <if case="1">Bad1!
                <elseif case="1" />Bad2!
                <elseif case="1" />Bad3!
                </if>d
            </if>)";
    EQ_VALUE(
        String<wchar_t>::Trim(Template::Render(content, &value).GetString()),
        L"b", L"Render()");

    content = LR"(<if case="0">
                <if case="1">Bad1!
                <elseif case="1" />Bad2!
                <else/>Bad3!
                </if>a
            <elseif case="0" />b
                <if case="1">Bad1!
                <elseif case="1"/>Bad2!
                <else/>Bad3!
                </if>
            <elseif case="1" />
                <if case="0">Bad1!
                <elseif case="0" />Bad2!
                <elseif case="0" />Bad3!
                </if>c
            <else />d
                <if case="1">Bad1!
                <elseif case="1" />Bad2!
                <else/>Bad3!
                </if>
            </if>)";
    EQ_VALUE(
        String<wchar_t>::Trim(Template::Render(content, &value).GetString()),
        L"c", L"Render()");

    content = LR"(<if case="0">a
                <if case="1">Bad1!
                <elseif case="1" />Bad2!
                <else/>Bad3!
                </if>
            <elseif case="0" />
                <if case="1">Bad1!
                <elseif case="1"/>Bad2!
                <else/>Bad3!
                </if>b
            <elseif case="0" />c
                <if case="1">Bad1!
                <elseif case="1" />Bad2!
                <else/>Bad3!
                </if>
            <else />
                <if case="0">Bad1!
                <elseif case="0" />Bad2!
                <elseif case="0" />Bad3!
                </if>d
            </if>)";
    EQ_VALUE(
        String<wchar_t>::Trim(Template::Render(content, &value).GetString()),
        L"d", L"Render()");

    END_SUB_TEST;
}

static int TestIfXTag2() {
    Value<wchar_t> value;
    const wchar_t *content;

    value[L"name"] = L"Qentem";

    content = LR"(<if case="1">{var:name})";
    EQ_VALUE(Template::Render(content, &value), LR"(<if case="1">Qentem)",
             L"Render()");

    content = LR"(<if case="1"><if case="1">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(<if case="1">Qentem)",
             L"Render()");

    content = LR"(<if case="1"><if case="1"><if case="1">{var:name}</if></if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(<if case="1">Qentem)",
             L"Render()");

    content = LR"(<if case="ABC">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", L"Render()");

    content = LR"(<if>{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", L"Render()");

    content = LR"(<if case="0"><elseif />{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", L"Render()");

    content = LR"(<iw case="0">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(<iw case="0">Qentem</if>)",
             L"Render()");

    END_SUB_TEST;
}

static int TestRenderX1() {
    constexpr unsigned int size_4 = (8 * 4);

    StringStream<wchar_t> content;
    StringStream<wchar_t> output;
    String<wchar_t>       str;
    Value<wchar_t>        value;

    for (unsigned int i = 0; i < size_4; i++) {
        value += i;
    }

    for (unsigned int i = 0; i < size_4; i++) {
        content += L"{var:";
        str = Digit<wchar_t>::NumberToString(i);
        content += str;
        output += str;
        content += L"}";
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        L"Render()");

    content.Clear();
    output.Clear();

    unsigned int size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += L"{var:";
            str = Digit<wchar_t>::NumberToString(i);
            content += str;
            content += L"}";

            output += str;
        } else {
            size += 8;

            content += L"{math: 1 + ";
            str = Digit<wchar_t>::NumberToString(i);
            content += str;
            content += L"}";

            Digit<wchar_t>::NumberToStringStream(output, 1U + i);
        }
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        L"Render()");

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += L"{var:";
            str = Digit<wchar_t>::NumberToString(i);
            content += str;
            content += L"}";

            output += str;
        } else {
            size += 8;

            content += LR"({if case="{var:)";
            Digit<wchar_t>::NumberToStringStream(content, i);
            content += LR"(}<)";
            Digit<wchar_t>::NumberToStringStream(content, 1U + i);
            content += LR"(" true="yes"})";

            output += L"yes";
        }
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        L"Render()");

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += L"{var:";
            str = Digit<wchar_t>::NumberToString(i);
            content += str;
            content += L"}";

            output += str;
        } else {
            size += 8;
            content += LR"(<loop repeat="1">A</loop>)";
            output += L"A";
        }
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        L"Render()");

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += L"{var:";
            str = Digit<wchar_t>::NumberToString(i);
            content += str;
            content += L"}";

            output += str;
        } else {
            size += 8;
            content += LR"(<if case="1">A</if>)";
            output += L"A";
        }
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        L"Render()");

    content.Clear();
    output.Clear();

    constexpr unsigned int size_2_1 = (8 * 2) - 1;

    for (unsigned int i = 0; i < size_2_1; i++) {
        value += i;
    }

    content += LR"(<loop repeat="1">)";
    for (unsigned int i = 0; i < size_2_1; i++) {
        content += L"{var:";
        str = Digit<wchar_t>::NumberToString(i);
        content += str;
        output += str;
        content += L"}";
    }
    content += LR"(</loop>)";

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        L"Render()");

    END_SUB_TEST;
}

static int TestRenderX2() {
    Value<wchar_t> value;
    const wchar_t *content;

    value[0] = 0;
    value[1] = 1;
    value[2] = 2;
    value[3] = 5;
    value[4] = 10;

    content = LR"(<loop set="numbers" value="val_">val_</loop>)";
    EQ_VALUE(Template::Render(content, &value), L"", L"Render()");

    content =
        LR"(<loop value="this_number"><if case="(this_number % 2) == 1">this_number</if></loop>)";
    EQ_VALUE(Template::Render(content, &value), L"15", L"Render()");

    content =
        LR"(<loop value="loop1_val">{if case="loop1_val < 5", true="loop1_val"}</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(012)", L"Render()");

    content =
        LR"(<loop value="loop1_val">{if case="loop1_val < 5", true="{var:4}"}</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(101010)", L"Render()");

    content = LR"(<loop value="loop1_val">loop1_val[]</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", L"Render()");

    content = LR"(<loop value="loop1_val">loop1_val[0 </loop>)";
    EQ_VALUE(
        Template::Render(content, &value),
        LR"({var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 )",
        L"Render()");

    value.Reset();
    value[0][0] += 1;
    value[0][0] += 2;
    value[0][0] += 3;

    content = LR"(<loop value="loop1_val">loop1_val[0][2]</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(3)", L"Render()");

    END_SUB_TEST;
}

// std::wcout << '\n'
//            << Template::Render(content, &value).GetString().First() << '\n';

static int RunTemplateLTests() {
    STARTING_TEST("Template.hpp (Wide character)");

    START_TEST("Variable Tag Test 1", TestVariableXTag1);
    START_TEST("Variable Tag Test 2", TestVariableXTag2);
    START_TEST("Variable Tag Test 3", TestVariableXTag3);

    START_TEST("Math Tag Test 1", TestMathXTag1);
    START_TEST("Math Tag Test 2", TestMathXTag2);

    START_TEST("Inline if Tag Test", TestInlineIfXTag);

    START_TEST("Loop Tag Test 1", TestLoopXTag1);
    START_TEST("Loop Tag Test 2", TestLoopXTag2);
    START_TEST("Loop Tag Test 3", TestLoopXTag3);
    START_TEST("Loop Tag Test 4", TestLoopXTag4);

    START_TEST("If Tag Test 1", TestIfXTag1);
    START_TEST("If Tag Test 2", TestIfXTag2);

    START_TEST("Render Test 1", TestRenderX1);
    START_TEST("Render Test 2", TestRenderX2);

    END_TEST("Template.hpp (Wide character)");
}

} // namespace Test
} // namespace Qentem

#endif
