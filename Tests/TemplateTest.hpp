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

#include "Template.hpp"
#include "TestHelper.hpp"

#ifndef QENTEM_TEMPLATETEST_H_
#define QENTEM_TEMPLATETEST_H_

namespace Qentem {
namespace Test {

using Qentem::Value;

static int TestVariableTag1() {
    const char *content;

    Value value = JSON::Parse(
        R"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    Value sub_value = JSON::Parse(
        R"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = R"({var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(A)", "Render()");

    content = R"({var:1})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(abc)",
                       "Render()");

    content = R"({var:2})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(true)",
                       "Render()");

    content = R"({var:3})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(456)",
                       "Render()");

    content = R"({var:4})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(1.5)",
                       "Render()");

    content = R"({var:5[0]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(null)",
                       "Render()");

    content = R"({var:5[1]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(false)",
                       "Render()");

    content = R"({var:5[2][0]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(Qentem)",
                       "Render()");

    //////

    content = R"({var:key1})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(a)",
                       "Render()");

    content = R"({var:key2})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(ABC)",
                       "Render()");

    content = R"({var:key3})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(false)",
                       "Render()");

    content = R"({var:key4})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(100)",
                       "Render()");

    content = R"({var:key5})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(1.5)",
                       "Render()");

    content = R"({var:key6[one]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(1)",
                       "Render()");

    content = R"({var:key7[0]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(null)",
                       "Render()");

    content = R"({var:key7[1]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(false)",
                       "Render()");

    content = R"({var:key7[2][0]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(Qentem)",
                       "Render()");

    //
    content = R"({var:6[key1]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(a)", "Render()");

    content = R"({var:6[key2]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(ABC)",
                       "Render()");

    content = R"({var:6[key3]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(false)",
                       "Render()");

    content = R"({var:6[key4]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(100)",
                       "Render()");

    content = R"({var:6[key5]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(1.5)",
                       "Render()");

    content = R"({var:6[key6][one]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(1)", "Render()");

    ////////////////

    content = R"(-{var:2})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(-true)",
                       "Render()");

    content = R"(-{var:key7[0]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(-null)",
                       "Render()");

    content = R"(-{var:key7[2][0]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(-Qentem)",
                       "Render()");

    content = R"(-{var:6[key3]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(-false)",
                       "Render()");

    content = R"(-{var:6[key4]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(-100)",
                       "Render()");
    ////////////

    content = R"({var:2}-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(true-)",
                       "Render()");

    content = R"({var:key7[0]}-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(null-)",
                       "Render()");

    content = R"({var:key7[2][0]}-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(Qentem-)",
                       "Render()");

    content = R"({var:6[key3]}-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(false-)",
                       "Render()");

    content = R"({var:6[key4]}-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(100-)",
                       "Render()");

    ////////////

    content = R"(-{var:2}-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(-true-)",
                       "Render()");

    content = R"(-{var:key7[0]}-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(-null-)",
                       "Render()");

    content = R"(-{var:key7[2][0]}-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(-Qentem-)",
                       "Render()");

    content = R"(-{var:6[key3]}-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(-false-)",
                       "Render()");

    content = R"(-{var:6[key4]}-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(-100-)",
                       "Render()");

    ////////////

    content = R"(------{var:2})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(------true)",
                       "Render()");

    content = R"(------{var:key7[0]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(------null)",
                       "Render()");

    content = R"(------{var:key7[2][0]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value),
                       R"(------Qentem)", "Render()");

    content = R"(------{var:6[key3]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(------false)",
                       "Render()");

    content = R"(------{var:6[key4]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(------100)",
                       "Render()");

    ////////////

    content = R"({var:2}------)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(true------)",
                       "Render()");

    content = R"({var:key7[0]}------)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value), R"(null------)",
                       "Render()");

    content = R"({var:key7[2][0]}------)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value),
                       R"(Qentem------)", "Render()");

    content = R"({var:6[key3]}------)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(false------)",
                       "Render()");

    content = R"({var:6[key4]}------)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(100------)",
                       "Render()");

    ////////////

    content = R"(------{var:2}------)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       R"(------true------)", "Render()");

    content = R"(------{var:key7[0]}------)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value),
                       R"(------null------)", "Render()");

    content = R"(------{var:key7[2][0]}------)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &sub_value),
                       R"(------Qentem------)", "Render()");

    content = R"(------{var:6[key3]}------)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       R"(------false------)", "Render()");

    content = R"(------{var:6[key4]}------)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       R"(------100------)", "Render()");

    END_SUB_TEST;
}

static int TestVariableTag2() {
    Value       value = JSON::Parse(R"(["A", "abc", true, 456, 1.5])");
    const char *content;

    content = R"({var:0}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(AA)",
                       "Render()");

    content = R"({var:1}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(abcA)",
                       "Render()");

    content = R"({var:1}{var:2})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(abctrue)",
                       "Render()");

    content = R"({var:2}{var:3}{var:2})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(true456true)",
                       "Render()");

    content = R"({var:4}{var:4}{var:4})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(1.51.51.5)",
                       "Render()");
    ///

    content = R"({var:0}-{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(A-A)",
                       "Render()");

    content = R"({var:1}--{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(abc--A)",
                       "Render()");

    content = R"({var:1}---{var:2})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(abc---true)",
                       "Render()");

    content = R"({var:2}{var:3}--{var:2})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(true456--true)",
                       "Render()");

    content = R"({var:4}--{var:4}{var:4})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(1.5--1.51.5)",
                       "Render()");

    content = R"({var:4}--{var:4}--{var:4})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(1.5--1.5--1.5)",
                       "Render()");

    content = R"({var:4}---{var:4}---{var:4})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       R"(1.5---1.5---1.5)", "Render()");

    END_SUB_TEST;
}

static int TestVariableTag3() {
    Value       value;
    const char *content;

    content = R"({var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:0})",
                       "Render()");

    String content2(R"({var:a})");
    SHOULD_EQUAL_VALUE(Template<>::Render(content2, &value), R"({var:a})",
                       "Render()");

    content = R"({var:0[0]})";
    SHOULD_EQUAL_VALUE(
        Template<>::Render(content, String::Count(content), &value),
        R"({var:0[0]})", "Render()");

    content = R"({var:a[0]})";
    SHOULD_EQUAL_VALUE(
        Template<>::Render(content, String::Count(content), &value),
        R"({var:a[0]})", "Render()");

    content = R"({var:0[a]})";
    SHOULD_EQUAL_VALUE(
        Template<>::Render(content, String::Count(content), &value),
        R"({var:0[a]})", "Render()");

    content = R"({var:a[abc]})";
    SHOULD_EQUAL_VALUE(
        Template<>::Render(content, String::Count(content), &value),
        R"({var:a[abc]})", "Render()");

    ////////////////

    value = JSON::Parse(R"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = R"({var:0})";
    SHOULD_EQUAL_VALUE(
        Template<>::Render(content, String::Count(content), &value),
        R"({var:0})", "Render()");

    content = R"({var:0[0]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:0[0]})",
                       "Render()");

    content = R"({var:0[0][0]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:0[0][0]})",
                       "Render()");

    /////

    content = R"({var:1})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:1})",
                       "Render()");

    content = R"({var:2})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:2})",
                       "Render()");

    content = R"({var:1[a]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:1[a]})",
                       "Render()");

    content = R"({var:1[b]})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:1[b]})",
                       "Render()");

    ////

    value = JSON::Parse(R"(["A", "abc", true, 456, 1.5])");

    content = R"({var:0)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:0)",
                       "Render()");

    content = R"(var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(var:0})",
                       "Render()");

    content = R"({v})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({v})",
                       "Render()");

    content = R"({va})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({va})",
                       "Render()");

    content = R"({var})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var})",
                       "Render()");

    content = R"({var:})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:})",
                       "Render()");

    content = R"({v:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({v:0})",
                       "Render()");

    content = R"({va:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({va:0})",
                       "Render()");
    ////

    content = R"({var:0{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:0{var:0})",
                       "Render()");

    content = R"(var:0{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(var:0A)",
                       "Render()");

    content = R"(var:0}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(var:0}A)",
                       "Render()");

    content = R"({var:0{var:0}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:0{var:0}A)",
                       "Render()");

    ////

    content = R"({var:0{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:0{var:0})",
                       "Render()");

    content = R"(var:0{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(var:0A)",
                       "Render()");

    content = R"(var:0}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(var:0}A)",
                       "Render()");

    content = R"({var:0{var:0}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:0{var:0}A)",
                       "Render()");

    ////

    content = R"({{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({A)",
                       "Render()");

    content = R"({{var:0}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({AA)",
                       "Render()");

    content = R"({v{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({vA)",
                       "Render()");

    content = R"({v{var:0}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({vAA)",
                       "Render()");

    content = R"({va{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({vaA)",
                       "Render()");

    content = R"({va{var:0}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({vaAA)",
                       "Render()");

    content = R"({var{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({varA)",
                       "Render()");

    content = R"({var{var:0}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({varAA)",
                       "Render()");

    ///

    content = R"({-{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({-A)",
                       "Render()");

    content = R"({-{var:0}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({-AA)",
                       "Render()");

    content = R"({v-{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({v-A)",
                       "Render()");

    content = R"({v-{var:0}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({v-AA)",
                       "Render()");

    content = R"({va-{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({va-A)",
                       "Render()");

    content = R"({va-{var:0}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({va-AA)",
                       "Render()");

    content = R"({var-{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var-A)",
                       "Render()");

    content = R"({var-{var:0}{var:0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var-AA)",
                       "Render()");

    //

    content = R"({var-0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var-0})",
                       "Render()");

    content = R"({var 0})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var 0})",
                       "Render()");

    content = R"({var:0 })";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"({var:0 })",
                       "Render()");

    END_SUB_TEST;
}

static int TestMathTag1() {
    Value value;

    value["a1"] = 5;
    value["a2"] = true;
    value["a3"] = nullptr;
    value["a4"] = false;
    value["a5"] = "10";
    value["a6"] = "20";
    value["a7"] = 6;
    value["a8"] = 1;
    value["a9"] = "1";

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:1+1}", &value), "2",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a1}+8}", &value), R"(13)",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a7}+8}", &value), R"(14)",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a7}+{var:a1}}", &value),
                       R"(11)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a5}+{var:a1}}", &value),
                       R"(15)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a1}+{var:a5}}", &value),
                       R"(15)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a6}+{var:a5}}", &value),
                       R"(30)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a6}*{var:a2}}", &value),
                       R"(20)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a6}*{var:a4}}", &value),
                       R"(0)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a6}*{var:a7}}", &value),
                       R"(120)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a7}+{var:a6}}", &value),
                       R"(26)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a1}+{var:a2}}", &value),
                       R"(6)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a8}=={var:a2}}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a2}=={var:a8}}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a5}!={var:a1}}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a2}!={var:a4}}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a2}==true}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a9}=={var:a8}}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a8}=={var:a9}}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:1=={var:a8}}", &value), R"(1)",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:6-5==({var:a9})}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:6-5==({var:a8})}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:null!={var:a3}}", &value),
                       R"(0)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:({var:a3})==(0)}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a1}+{var:a7}}", &value),
                       "11", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:{var:a1}+{var:a7}}{math:{var:a1}+{var:a7}}",
                           &value),
        "1111", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:{var:a1}+{var:a7}}*{math:{var:a1}+{var:a7}}",
                           &value),
        "11*11", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:{var:a1}+{var:a7}}##{math:{var:a1}+{var:a7}}",
                           &value),
        "11##11", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render(
            "{math:{var:a1}+{var:a7}}&&&%%^^&&*{math:{var:a1}+{var:a7}}",
            &value),
        "11&&&%%^^&&*11", "Render()");

    ///////////////////

    SHOULD_EQUAL_VALUE(Template<>::Render("{math: {var:a1}+8}", &value),
                       R"(13)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:  {var:a7}+8}", &value),
                       R"(14)", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:     {var:a7}+{var:a1}}", &value), R"(11)",
        "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a5} +{var:a1}}", &value),
                       R"(15)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a1}  +{var:a5}}", &value),
                       R"(15)", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:{var:a6}    +{var:a5}}", &value), R"(30)",
        "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a6}* {var:a2}}", &value),
                       R"(20)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a6}*  {var:a4}}", &value),
                       R"(0)", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:{var:a6}*      {var:a7}}", &value), R"(120)",
        "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a7}+{var:a6} }", &value),
                       R"(26)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a1}+{var:a2}  }", &value),
                       R"(6)", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:{var:a8}=={var:a2}      }", &value), R"(1)",
        "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:a2}=={var:a8}}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math: {var:a5}!={var:a1} }", &value), R"(1)",
        "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:  {var:a2}!={var:a4}  }", &value), R"(1)",
        "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:    1=={var:a9}     }", &value), R"(1)",
        "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:{var:a9} == {var:a8}}", &value), R"(1)",
        "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:{var:a8}  ==  {var:a9}}", &value), R"(1)",
        "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:1==          {var:a8}}", &value), R"(1)",
        "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:6-5         ==1}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:1          ==            {var:a8}}", &value),
        R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:     0     !=    ({var:a3})        }",
                           &value),
        R"(0)", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:{var:a3}       ==       null     }", &value),
        R"(1)", "Render()");

    //////////////
    value.Clear();

    value += 5;
    value += true;
    value += nullptr;
    value += false;
    value += "10";
    value += "20";
    value += 6;
    value += 1;
    value += "1";
    value += "Qentem";

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:0}+8}", &value), R"(13)",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:6}+8}", &value), R"(14)",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:6}+{var:0}}", &value),
                       R"(11)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:4}+{var:0}}", &value),
                       R"(15)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:0}+{var:4}}", &value),
                       R"(15)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:5}+{var:4}}", &value),
                       R"(30)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:5}*{var:1}}", &value),
                       R"(20)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:5}*{var:3}}", &value),
                       R"(0)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:5}*{var:6}}", &value),
                       R"(120)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:6}+{var:5}}", &value),
                       R"(26)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:0}+{var:1}}", &value),
                       R"(6)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:7}=={var:1}}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:1}=={var:7}}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:4}!={var:0}}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:1}!={var:3}}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:({var:1})==({var:8})}", &value), R"(1)",
        "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("-{math:{var:8}=={var:7}}", &value),
                       R"(-1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("--{math:{var:7}=={var:8}}", &value),
                       R"(--1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("---{math:1=={var:7}}", &value),
                       R"(---1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:1==({var:8})}-", &value),
                       R"(1-)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:6-5==({var:7})}--", &value),
                       R"(1--)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:0==({var:2})}---", &value),
                       R"(1---)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("-{math:{var:2}!=null}-", &value),
                       R"(-0-)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("--{math:Qente=={var:9}}--", &value),
                       R"(--0--)", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("---{math:Qente !={var:9}}---", &value),
        R"(---1---)", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:  Qentem   =={var:9}}", &value), R"(1)",
        "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:  Qentem!={var:9}}", &value),
                       R"(0)", "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:{var:9}   ==    Qente}", &value), R"(0)",
        "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:{var:9} !=    Qente    }", &value), R"(1)",
        "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math:  {var:9}   ==Qentem}", &value), R"(1)",
        "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math: {var:9} !=Qentem}", &value),
                       R"(0)", "Render()");

    /////////

    SHOULD_EQUAL_VALUE(Template<>::Render("{math: true == {var:1}}", &value),
                       R"(1)", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math: {var:1} == true}", &value),
                       R"(1)", "Render()");

    END_SUB_TEST;
}

static int TestMathTag2() {
    Value value;

    value[0] = Array<Value>();
    value[1] = HArray<Value>();
    value[2] = 5;

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:0}+8}", &value),
                       "{math:{var:0}+8}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:1}+8}", &value),
                       R"({math:{var:1}+8})", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:2}+{var:0}}", &value),
                       R"({math:{var:2}+{var:0}})", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:2}+{var:0}}", &value),
                       R"({math:{var:2}+{var:0}})", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:0}+{var:1}}", &value),
                       "{math:{var:0}+{var:1}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:1}+{var:2}}", &value),
                       "{math:{var:1}+{var:2}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:2}*{var:1}}", &value),
                       "{math:{var:2}*{var:1}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:0}*{var:1}}", &value),
                       "{math:{var:0}*{var:1}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:0}*{var:2}}", &value),
                       "{math:{var:0}*{var:2}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:2}+{var:0}}", &value),
                       "{math:{var:2}+{var:0}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:1}+{var:2}}", &value),
                       "{math:{var:1}+{var:2}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:0}=={var:1}}", &value),
                       "{math:{var:0}=={var:1}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:1}=={var:0}}", &value),
                       "{math:{var:1}=={var:0}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:0}!={var:2}}", &value),
                       "{math:{var:0}!={var:2}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:2}!={var:0}}", &value),
                       "{math:{var:2}!={var:0}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:1}=={var:2}}", &value),
                       "{math:{var:1}=={var:2}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:8}=={var:7}}", &value),
                       "{math:{var:8}=={var:7}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:7}=={var:2}}", &value),
                       "{math:{var:7}=={var:2}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:1=={var:7}}", &value),
                       "{math:1=={var:7}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:(6-5)=={var:8}}", &value),
                       "{math:(6-5)=={var:8}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:(6-5)=={var:0}}", &value),
                       "{math:(6-5)=={var:0}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:0}=={var:8}}", &value),
                       "{math:{var:0}=={var:8}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:0}=={var:0}}", &value),
                       "{math:{var:0}=={var:0}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:0=={var:1}}", &value),
                       "{math:0=={var:1}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:1}!=0}", &value),
                       "{math:{var:1}!=0}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:W={var:0}}", &value),
                       "{math:W={var:0}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:W=={var:0}}", &value),
                       "{math:W=={var:0}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:2}==c}", &value),
                       "{math:{var:2}==c}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:abc=={var:2}}", &value),
                       "{math:abc=={var:2}}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:sds}", &value), "{math:sds}",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:1", &value), "{math:1",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("math:1}", &value), "math:1}",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:{var:2}", &value),
                       "{math:{var:2}", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{{math:{var:2}+5}", &value), "{10",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{m{var:2}}", &value), "{m5}",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{ma{var:2}}", &value), "{ma5}",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{mat{var:2}}", &value), "{mat5}",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math{var:2}}", &value), "{math5}",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math {var:2}}", &value), "{math 5}",
                       "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math:}", &value), "{math:}",
                       "Render()");

    SHOULD_EQUAL_VALUE(
        Template<>::Render("{math-{var:2}}{math:{var:2}+5}", &value),
        "{math-5}10", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math-4}{math:{var:2}+5}", &value),
                       "{math-4}10", "Render()");

    SHOULD_EQUAL_VALUE(Template<>::Render("{math-4} {math:{var:2}+5}", &value),
                       "{math-4} 10", "Render()");

    ///////////

    END_SUB_TEST;
}

static int TestInlineIfTag() {
    Value       value;
    const char *content;

    value += 0;
    value += 1;
    value += -1;
    value += true;
    value += false;
    value += nullptr;
    value += "ABC";
    value += Array<Value>();

    content = R"({if case="0" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "F", "Render()");

    content = R"({if case="-1" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "F", "Render()");

    content = R"({if case="0.1" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content = R"({if case="1" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    ////

    content = R"({if case="0" true="T"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"({if case="-1" true="T"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"({if case="0.1" true="T"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content = R"({if case="1" true="T"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    ///

    content = R"({if case="0" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "F", "Render()");

    content = R"({if case="-1" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "F", "Render()");

    content = R"({if case="0.1" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"({if case="1" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    /////

    content = R"({if case="{var:0}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "F", "Render()");

    content = R"({if case="{var:2}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "F", "Render()");

    content = R"({if case="{var:4}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "F", "Render()");

    content = R"({if case="{var:5}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "F", "Render()");

    content = R"({if case="{var:6}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"({if case="fas" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"({if case="{var:7}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"({if case="{var:20}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    ////
    content = R"({if case="{var:1}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content = R"({if case="{var:3}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content = R"({if case="{var:6}==ABC" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    /////////////////

    content = R"({if case=" {var:1}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content = R"({if case="{var:1} " true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content = R"({if case=" {var:1} " true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content = R"({if case="  {var:1}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content = R"({if case="{var:1}  " true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content = R"({if case="  {var:1}  " true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content = R"({if case="      {var:1}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content = R"({if case="{var:1}          " true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content = R"({if case="    {var:1}        " true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");
    ////

    content = R"(-{if case=" {var:1} " true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "-T", "Render()");

    content = R"({if case=" {var:1} " true="T" false="F"}-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T-", "Render()");

    content = R"(-{if case=" {var:1} " true="T" false="F"}-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "-T-", "Render()");

    content = R"(--{if case=" {var:1} " true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "--T", "Render()");

    content = R"({if case=" {var:1} " true="T" false="F"}--)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T--", "Render()");

    content = R"(--{if case=" {var:1} " true="T" false="F"}--)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "--T--",
                       "Render()");

    content = R"(---{if case=" {var:1} " true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "---T", "Render()");

    content = R"({if case=" {var:1} " true="T" false="F"}---)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T---", "Render()");

    content = R"(---{if case=" {var:1} " true="T" false="F"}---)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "---T---",
                       "Render()");

    content =
        R"({if case="1" true="T"}{if case="0" false="F"}{if case="1" true="T"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "TFT", "Render()");

    ///////

    content =
        R"({if case="{var:7}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content = R"({if case="01" true="{var:3}" false="{var:4}"}--)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "--", "Render()");

    content = R"({if case="" true="c" false="d"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"({i)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "{i", "Render()");

    content = R"({if)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "{if", "Render()");

    content = R"({if})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "{if}", "Render()");

    content = R"({{if case="{var:1}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "{T", "Render()");

    content = R"({i{if case="{var:1}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "{iT", "Render()");

    content = R"({if{if case="{var:1}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "T", "Render()");

    content =
        R"({if{if case="1" true="T" false="F"}}{if case="1" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(T}T)",
                       "Render()");

    content =
        R"({if{if case="{var:1}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"(TT)",
                       "Render()");

    /////
    content = R"({if case="0" true="{var:3}" false="{var:4}"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "false",
                       "Render()");

    content = R"({if case="1" true="{var:3}" false="{var:4}"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "true", "Render()");

    content = R"({if case="0" true="{var:3}{var:3}" false="{var:4}{var:4}"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "falsefalse",
                       "Render()");

    content =
        R"({if case="1000" true="{var:3}{var:3}" false="{var:4}{var:4}"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "truetrue",
                       "Render()");

    content =
        R"({if case="0" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "false---false",
                       "Render()");

    content =
        R"({if case="1" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "true---true",
                       "Render()");

    content = R"({if case="0" true="{var:10}" false="{var:20}"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "{var:20}",
                       "Render()");

    content = R"({if case="1" true="{var:10}" false="{var:20}"})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "{var:10}",
                       "Render()");

    END_SUB_TEST;
}

static int TestLoopTag1() {
    Value       value;
    const char *content;

    value += 100;
    value += -50;
    value += "Qentem";
    value += true;
    value += false;
    value += nullptr;
    value += 3;

    content = R"(<loop times="10">A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "AAAAAAAAAA",
                       "Render()");

    content = R"(<loop             times="1">A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "A", "Render()");

    content = R"(<loop times="3"         >ABC</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "ABCABCABC",
                       "Render()");

    content = R"(-<loop times="3">A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "-AAA", "Render()");

    content = R"(<loop times="3">A</loop>-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "AAA-", "Render()");

    content = R"(-<loop times="3">A</loop>-)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "-AAA-",
                       "Render()");

    content = R"(--<loop times="3">A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "--AAA",
                       "Render()");

    content = R"(<loop times="3">A</loop>--)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "AAA--",
                       "Render()");

    content = R"(--<loop times="3">A</loop>--)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "--AAA--",
                       "Render()");

    content = R"(---<loop times="3">A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "---AAA",
                       "Render()");

    content = R"(<loop times="3">A</loop>---)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "AAA---",
                       "Render()");

    content = R"(---<loop times="3">A</loop>---)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "---AAA---",
                       "Render()");

    content = R"(<loop times="2">A</loop><loop times="3">B</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "AABBB",
                       "Render()");

    content = R"(<loop times="2">A</loop>-<loop times="3">B</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "AA-BBB",
                       "Render()");

    content = R"(<loop times="2">A</loop>--<loop times="3">B</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "AA--BBB",
                       "Render()");

    content = R"(<loop times="2">A</loop>---<loop times="3">B</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "AA---BBB",
                       "Render()");

    content = R"(<loop times="2">A</loop>            <loop times="3">B</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "AA            BBB",
                       "Render()");

    ////

    content =
        R"(<loop times="4">CC</loop><loop times="2">A</loop><loop times="3">B</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "CCCCCCCCAABBB",
                       "Render()");

    content =
        R"(<loop times="4">CC</loop>-<loop times="2">A</loop>-<loop times="3">B</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "CCCCCCCC-AA-BBB",
                       "Render()");

    content =
        R"(<loop times="4">CC</loop>--<loop times="2">A</loop>--<loop times="3">B</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "CCCCCCCC--AA--BBB",
                       "Render()");

    content =
        R"(<loop times="4">CC</loop>---<loop times="2">A</loop>---<loop times="3">B</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       "CCCCCCCC---AA---BBB", "Render()");

    content =
        R"(<loop times="4">CC</loop>     <loop times="2">A</loop>            <loop times="3">B</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       "CCCCCCCC     AA            BBB", "Render()");

    //////////

    content = R"(<loop times="4" key="loop1-key">loop1-key</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "0123", "Render()");

    content = R"(<loop key="loop1-key" times="3" index="1">loop1-key</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "123", "Render()");

    content =
        R"(<loop key="loop1-key" index="  {var:6}   " times="1">loop1-key</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "3", "Render()");

    content = R"(<loop index="1" times="3" key="loop1-key">loop1-key:C</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "1:C2:C3:C",
                       "Render()");

    /////
    content = R"(<loop times="{var:6}" key="K">K</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "012", "Render()");

    content = R"(<loop key="K" times="3">-K</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "-0-1-2",
                       "Render()");

    content = R"(<loop key="K"               times="3">K-</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "0-1-2-",
                       "Render()");

    content = R"(<looptimes="3"key="K">KKKK</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "000011112222",
                       "Render()");

    content = R"(<loopkey="K"times="3">K-K-K#</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       "0-0-0#1-1-1#2-2-2#", "Render()");

    ////////////////

    content = R"(<loop times="6" key="loop1-key">{var:loop1-key}, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       "100, -50, Qentem, true, false, null, ", "Render()");

    content =
        R"(<loop times="6" key="loop1-key">{var:loop1-key}, {var:loop1-key} </loop>)";
    SHOULD_EQUAL_VALUE(
        Template<>::Render(content, &value),
        "100, 100 -50, -50 Qentem, Qentem true, true false, false null, null ",
        "Render()");

    content =
        R"(<loop times="6" key="loop1-key">{var:loop1-key}{if case="loop1-key<5" true=", "}</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       "100, -50, Qentem, true, false, null", "Render()");
    /////

    content =
        R"(<loop times="3" key="loop1-id"><loop
        times="2" key="loop2-id">(loop1-id: loop2-id) </loop></loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       "(0: 0) (0: 1) (1: 0) (1: 1) (2: 0) (2: 1) ",
                       "Render()");

    content =
        R"(<loop times="2" key="loop1-id"><loop
        times="2" key="loop2-id"><loop
        times="2" key="loop3-id">(loop1-id: loop2-id: loop3-id) </loop></loop></loop>)";
    SHOULD_EQUAL_VALUE(
        Template<>::Render(content, &value),
        "(0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1) ",
        "Render()");

    END_SUB_TEST;
}

static int TestLoopTag2() {
    Value       value1;
    Value       value2;
    Value       value3;
    const char *content;

    value1 += 100;
    value1 += -50;
    value1 += "A";
    value1 += true;
    value1 += false;
    value1 += nullptr;

    value2["k-1"] = 4;
    value2["k-2"] = 1.5;
    value2["k-3"] = "ABC";
    value2["k-4"] = true;
    value2["k-5"] = false;
    value2["k-6"] = nullptr;

    //////////////////////
    value3["arr1"] = value1;

    content = R"(<loop value="loop1-value">loop1-value, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value1),
                       "100, -50, A, true, false, null, ", "Render()");

    content = R"(<loop value="loop1-value" index="3">loop1-value, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value2),
                       "true, false, null, ", "Render()");

    content = R"(<loop value="loop1-value">loop1-value, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value2),
                       "4, 1.5, ABC, true, false, null, ", "Render()");

    content = R"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value3),
                       "100, -50, A, true, false, null, ", "Render()");

    value3["arr1"] = value2;

    content = R"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value3),
                       "4, 1.5, ABC, true, false, null, ", "Render()");

    //////////////////////
    value3["arr1"] = value1;

    content = R"(<loop key="loop1-key">loop1-key, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value1),
                       "0, 1, 2, 3, 4, 5, ", "Render()");

    content = R"(<loop key="loop1-key">loop1-key, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value2),
                       "k-1, k-2, k-3, k-4, k-5, k-6, ", "Render()");

    content = R"(<loop key="loop1-key" index="3">loop1-key, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value2), "k-4, k-5, k-6, ",
                       "Render()");

    content = R"(<loop set="arr1" key="loop1-key">loop1-key, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value3),
                       "0, 1, 2, 3, 4, 5, ", "Render()");

    value3["arr1"] = value2;

    content = R"(<loop set="arr1" key="loop1-key">loop1-key, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value3),
                       "k-1, k-2, k-3, k-4, k-5, k-6, ", "Render()");

    //////////////////////
    value3["arr1"] = value1;

    content =
        R"(<loop value="loop1-value" key="loop1-key">loop1-key: loop1-value, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value1),
                       "0: 100, 1: -50, 2: A, 3: true, 4: false, 5: null, ",
                       "Render()");

    content =
        R"(<loop key="loop1-key" value="loop1-value">loop1-key: loop1-value, </loop>)";
    SHOULD_EQUAL_VALUE(
        Template<>::Render(content, &value2),
        "k-1: 4, k-2: 1.5, k-3: ABC, k-4: true, k-5: false, k-6: null, ",
        "Render()");

    content =
        R"(<loop value="loop1-value" set="arr1" key="loop1-key">loop1-key: loop1-value, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value3),
                       "0: 100, 1: -50, 2: A, 3: true, 4: false, 5: null, ",
                       "Render()");

    value3["arr1"] = value2;

    content =
        R"(<loop set="arr1" value="loop1-value" key="loop1-key">loop1-key: loop1-value, </loop>)";
    SHOULD_EQUAL_VALUE(
        Template<>::Render(content, &value3),
        "k-1: 4, k-2: 1.5, k-3: ABC, k-4: true, k-5: false, k-6: null, ",
        "Render()");

    //////////////////////
    value3.Clear();
    value3["arr1"]["arr2"]["arr3"] = value1;

    content =
        R"(<loop set="arr1[arr2][arr3]" value="loop1-value" key="loop1-key">loop1-key: loop1-value, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value3),
                       "0: 100, 1: -50, 2: A, 3: true, 4: false, 5: null, ",
                       "Render()");

    value3.Clear();
    value3[0][0][0] = value2;

    content =
        R"(<loopset="0[0][0]"key="loop1-key"value="loop1-value">loop1-key: loop1-value, </loop>)";
    SHOULD_EQUAL_VALUE(
        Template<>::Render(content, &value3),
        "k-1: 4, k-2: 1.5, k-3: ABC, k-4: true, k-5: false, k-6: null, ",
        "Render()");

    value3.Clear();
    value3["k1"][0]["k3"] = value1;

    content =
        R"(<loop value="loop1-value" set="k1[0][k3]" key="loop1-key">loop1-key: loop1-value, </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value3),
                       "0: 100, 1: -50, 2: A, 3: true, 4: false, 5: null, ",
                       "Render()");

    value3.Clear();
    value3[0]["k2"][0] = value2;

    content =
        R"(<loopset="0[k2][0]"key="loop1-key"value="loop1-value">loop1-key: loop1-value, loop1-key: loop1-value, </loop>)";
    SHOULD_EQUAL_VALUE(
        Template<>::Render(content, &value3),
        R"(k-1: 4, k-1: 4, k-2: 1.5, k-2: 1.5, k-3: ABC, k-3: ABC, k-4: true, k-4: true, k-5: false, k-5: false, k-6: null, k-6: null, )",
        "Render()");

    value3 = JSON::Parse(R"({"group":[[10],[20],[30]]})");

    content =
        R"(<loop set="group" key="_Key1" value="_Val1"><loop set="group[_Key1]" value="_Val2">_Val2</loop></loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value3), "102030",
                       "Render()");

    value3 = JSON::Parse(R"({"group":[1,2,3,4]})");

    content =
        R"(<loop set="group" key="_Key" value="_Val" times="1">_Key:_Val</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value3), "0:1", "Render()");

    content =
        R"(<loop set="group" key="_Key" value="_Val" index="3">_Key:_Val</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value3), "3:4", "Render()");

    content =
        R"(<loop set="group" key="_Key" value="_Val" index="2" times="1">_Key:_Val</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value3), "2:3", "Render()");

    END_SUB_TEST;
}

static int TestLoopTag3() {
    Value       value;
    const char *content;

    content = R"(<loop></loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"(<loop>abcd</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"(<l</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "<l</loop>",
                       "Render()");

    content = R"(<lo</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "<lo</loop>",
                       "Render()");

    content = R"(<loo</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "<loo</loop>",
                       "Render()");

    content = R"(<loop></loop><loop times="2">A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "AA", "Render()");

    content = R"(<loop key="a">a</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"(<loop value="a">a</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"(<loop set="ss" value="a">a</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"(<loop set="" value="a">a</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"(<loop times="2" A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"(<loop times="OOO">A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"(<loop times="{var:10}">A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    value["in"] = Array<Value>();

    content = R"(<loop times="{var:in}">A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"(<loop times="10" index="{var:in}">A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"(<loop times="10" index="{var:in">A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"(<loop times="10" index="{var:100}">A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"(<loop times="10" index="O">A</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content = R"(<loop value="v">v</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    /////
    value.Clear();
    value["k1"] = 10;
    value["k2"] = 20;
    value["k3"] = 30;

    value["k2"].Clear();

    content = R"(<loop value="v">v</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "1030", "Render()");

    value.Remove(1);

    content = R"(<loop key="k">k</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "k1k3", "Render()");

    value.Clear();
    value += 10;
    value += 20;
    value += 30;

    value.Remove(1);

    content = R"(<loop value="v">v</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "1030", "Render()");

    value = JSON::Parse(R"(
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
        R"(<loop set="object" value="item">item[var1]item[var2]item[var3] item[var4]</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       "value1value2value3 value4", "Render()");

    content =
        R"(<loop set="array" value="item"> item[0] item[1] item[2] item[3] </loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       " value10 value20 value30 value40 ", "Render()");

    content =
        R"(<loop set="object" value="item">item[var11]item[var22]item[var33] item[var44]</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), " ", "Render()");

    content =
        R"(<loop set="array" value="item">item[var11]item[var22]item[var33] item[var44]</loop>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), " ", "Render()");

    END_SUB_TEST;
}

static int TestIfTag1() {
    Value       value;
    const char *content;

    value["name"] = "Qen";
    value["t"]    = true;
    value["f"]    = false;
    value["n"]    = nullptr;
    value["1"]    = 1;
    value["one"]  = "1";
    value["zero"] = 0;

    content = R"(<if case="1>0">{var:name}</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "Qen", "Render()");

    content = R"(#<if case="{var:one}">{var:name}</if>#)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "#Qen#",
                       "Render()");

    content = R"(##<if case="{var:zero}">{var:name}</if>##)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "####", "Render()");

    content = R"(<if case="{var:1}">{var:name}1<else />{var:name}2</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "Qen1", "Render()");

    content = R"(<if case="{var:f}">{var:name}1<else />{var:name}2</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "Qen2", "Render()");

    content = R"(#<if case="{var:t}">G<if case="1">oo</if>d</if>!#)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "#Good!#",
                       "Render()");

    content =
        R"(###<if case="1">G<if case="1">o</if>o<if case="1">d!</if></if>###)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "###Good!###",
                       "Render()");

    content =
        R"(Be <if case="1">G<if case="1">oo<if case="1">d</if></if></if>!)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "Be Good!",
                       "Render()");

    content = R"(<if case="1">Good!<elseif case="0" />Bad!</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "Good!",
                       "Render()");

    content = R"(<if case="{var:n}">Bad!<elseif case="1" />Good!</if>#)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "Good!#",
                       "Render()");

    content = R"(<if case="{var:f}">Bad!<elseif case="0" />Very Bad!</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "", "Render()");

    content =
        R"(#<if case="0">Bad!<elseif case="0" />Very Bad!<else />Very Good!</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "#Very Good!",
                       "Render()");

    content = R"(<if case="1">a<else />b</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "a", "Render()");

    content = R"(<if case="0">a<else />b</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "b", "Render()");

    content = R"(<if case="0">a<else /><if case="1">b</if></if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "b", "Render()");

    content = R"(<if case="1">a<else /><if case="1">b</if></if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "a", "Render()");

    content = R"(<if case="0">a<if case="1">b</if>c</if>Empty)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "Empty",
                       "Render()");

    content = R"(<if case="1">a<else /><if case="1">b</if>c</if>===========)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       "a===========", "Render()");

    content = R"(===========<if case="1">a<if case="1">b</if><else />c</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "===========ab",
                       "Render()");

    content =
        R"(<if case="1">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "a", "Render()");

    content =
        R"(<if case="0">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "b", "Render()");

    content =
        R"(===========<if case="0">a<elseif case="0" />b<elseif case="1" />c<else />d</if>===========)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       "===========c===========", "Render()");

    content =
        R"(<if case="0">a<elseif case="0" />b<elseif case="0" />c<else />d</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "d", "Render()");

    content = R"(<if case="1">a<if case="0">b<elseif case="0"/>c</if></if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "a", "Render()");

    content = R"(<if case="1">a<if case="1">b<elseif case="0"/>c</if></if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "ab", "Render()");

    content =
        R"(<if case="1"><if case="1">b<elseif case="c"/>c</if>a<else />c</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "ba", "Render()");

    content =
        R"(<if case="0">a<else />c<if case="1">b<elseif case="0"/>c</if></if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "cb", "Render()");

    content =
        R"(<if_case="1"><if case="0">Bad1!<elseif case="0" />Bad2!</if>a</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), "a", "Render()");

    content = R"(<if case="1">
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
    SHOULD_EQUAL_VALUE(String::Trim(Template<>::Render(content, &value)), "a",
                       "Render()");

    content = R"(<if case="0">a
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
    SHOULD_EQUAL_VALUE(String::Trim(Template<>::Render(content, &value)), "b",
                       "Render()");

    content = R"(<if case="0">
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
    SHOULD_EQUAL_VALUE(String::Trim(Template<>::Render(content, &value)), "c",
                       "Render()");

    content = R"(<if case="0">a
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
    SHOULD_EQUAL_VALUE(String::Trim(Template<>::Render(content, &value)), "d",
                       "Render()");

    END_SUB_TEST;
}
static int TestIfTag2() {
    Value       value;
    const char *content;

    value["name"] = "Qentem";

    content = R"(<if case="1">{var:name})";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       R"(<if case="1">Qentem)", "Render()");

    content = R"(<if case="1"><if case="1">{var:name}</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       R"(<if case="1">Qentem)", "Render()");

    content = R"(<if case="1"><if case="1"><if case="1">{var:name}</if></if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       R"(<if case="1">Qentem)", "Render()");

    content = R"(<if case="ABC">{var:name}</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"()", "Render()");

    content = R"(<if>{var:name}</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"()", "Render()");

    content = R"(<if case="0"><elseif />{var:name}</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value), R"()", "Render()");

    content = R"(<iw case="0">{var:name}</if>)";
    SHOULD_EQUAL_VALUE(Template<>::Render(content, &value),
                       R"(<iw case="0">Qentem</if>)", "Render()");

    END_SUB_TEST;
}

static int RunTemplateTests() {
    STARTING_TEST("Template.hpp");

    START_TEST("Variable tag Test 1", TestVariableTag1);
    START_TEST("Variable tag Test 2", TestVariableTag2);
    START_TEST("Variable tag Test 3", TestVariableTag3);

    START_TEST("Math tag Test 1", TestMathTag1);
    START_TEST("Math tag Test 2", TestMathTag2);

    START_TEST("Inline if tag Test", TestInlineIfTag);

    START_TEST("Loop tag Test 1", TestLoopTag1);
    START_TEST("Loop tag Test 2", TestLoopTag2);
    START_TEST("Loop tag Test 3", TestLoopTag3);

    START_TEST("If tag Test 1", TestIfTag1);
    START_TEST("If tag Test 2", TestIfTag2);

    END_TEST("Template.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
