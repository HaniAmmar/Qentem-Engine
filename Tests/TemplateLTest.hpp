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

#ifndef QENTEM_TEMPLATE_L_TESTS_H_
#define QENTEM_TEMPLATE_L_TESTS_H_

namespace Qentem {
namespace Test {

static int TestVariableLTag1() {
    const wchar_t *content;

    Value<wchar_t> value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    Value<wchar_t> sub_value = JSON::Parse(
        LR"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = LR"({var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(A)", LR"(Render())");

    content = LR"({var:1})";
    EQ_VALUE(Template::Render(content, &value), LR"(abc)", LR"(Render())");

    content = LR"({var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(true)", LR"(Render())");

    content = LR"({var:3})";
    EQ_VALUE(Template::Render(content, &value), LR"(456)", LR"(Render())");

    content = LR"({var:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5)", LR"(Render())");

    content = LR"({var:5[0]})";
    EQ_VALUE(Template::Render(content, &value), LR"(null)", LR"(Render())");

    content = LR"({var:5[1]})";
    EQ_VALUE(Template::Render(content, &value), LR"(false)", LR"(Render())");

    content = LR"({var:5[2][0]})";
    EQ_VALUE(Template::Render(content, &value), LR"(Qentem)", LR"(Render())");

    //////

    content = LR"({var:key1})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(a)", LR"(Render())");

    content = LR"({var:key2})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(ABC)", LR"(Render())");

    content = LR"({var:key3})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(false)",
             LR"(Render())");

    content = LR"({var:key4})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(100)", LR"(Render())");

    content = LR"({var:key5})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(1.5)", LR"(Render())");

    content = LR"({var:key6[one]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(1)", LR"(Render())");

    content = LR"({var:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(null)", LR"(Render())");

    content = LR"({var:key7[1]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(false)",
             LR"(Render())");

    content = LR"({var:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(Qentem)",
             LR"(Render())");

    //
    content = LR"({var:6[key1]})";
    EQ_VALUE(Template::Render(content, &value), LR"(a)", LR"(Render())");

    content = LR"({var:6[key2]})";
    EQ_VALUE(Template::Render(content, &value), LR"(ABC)", LR"(Render())");

    content = LR"({var:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), LR"(false)", LR"(Render())");

    content = LR"({var:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), LR"(100)", LR"(Render())");

    content = LR"({var:6[key5]})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5)", LR"(Render())");

    content = LR"({var:6[key6][one]})";
    EQ_VALUE(Template::Render(content, &value), LR"(1)", LR"(Render())");

    ////////////////

    content = LR"(-{var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(-true)", LR"(Render())");

    content = LR"(-{var:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(-null)",
             LR"(Render())");

    content = LR"(-{var:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(-Qentem)",
             LR"(Render())");

    content = LR"(-{var:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), LR"(-false)", LR"(Render())");

    content = LR"(-{var:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), LR"(-100)", LR"(Render())");
    ////////////

    content = LR"({var:2}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(true-)", LR"(Render())");

    content = LR"({var:key7[0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(null-)",
             LR"(Render())");

    content = LR"({var:key7[2][0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(Qentem-)",
             LR"(Render())");

    content = LR"({var:6[key3]}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(false-)", LR"(Render())");

    content = LR"({var:6[key4]}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(100-)", LR"(Render())");

    ////////////

    content = LR"(-{var:2}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(-true-)", LR"(Render())");

    content = LR"(-{var:key7[0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(-null-)",
             LR"(Render())");

    content = LR"(-{var:key7[2][0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(-Qentem-)",
             LR"(Render())");

    content = LR"(-{var:6[key3]}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(-false-)", LR"(Render())");

    content = LR"(-{var:6[key4]}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(-100-)", LR"(Render())");

    ////////////

    content = LR"(------{var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(------true)",
             LR"(Render())");

    content = LR"(------{var:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(------null)",
             LR"(Render())");

    content = LR"(------{var:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(------Qentem)",
             LR"(Render())");

    content = LR"(------{var:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), LR"(------false)",
             LR"(Render())");

    content = LR"(------{var:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), LR"(------100)",
             LR"(Render())");

    ////////////

    content = LR"({var:2}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(true------)",
             LR"(Render())");

    content = LR"({var:key7[0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(null------)",
             LR"(Render())");

    content = LR"({var:key7[2][0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(Qentem------)",
             LR"(Render())");

    content = LR"({var:6[key3]}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(false------)",
             LR"(Render())");

    content = LR"({var:6[key4]}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(100------)",
             LR"(Render())");

    ////////////

    content = LR"(------{var:2}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(------true------)",
             LR"(Render())");

    content = LR"(------{var:key7[0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(------null------)",
             LR"(Render())");

    content = LR"(------{var:key7[2][0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(------Qentem------)",
             LR"(Render())");

    content = LR"(------{var:6[key3]}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(------false------)",
             LR"(Render())");

    content = LR"(------{var:6[key4]}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(------100------)",
             LR"(Render())");

    content = LR"({var:key7[2[0]})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:key7[2[0]})",
             LR"(Render())");

    content = LR"({var:6key3]})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:6key3]})",
             LR"(Render())");

    END_SUB_TEST;
}

static int TestVariableLTag2() {
    Value<wchar_t> value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5])");
    const wchar_t *content;

    content = LR"({var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(AA)", LR"(Render())");

    content = LR"({var:1}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(abcA)", LR"(Render())");

    content = LR"({var:1}{var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(abctrue)", LR"(Render())");

    content = LR"({var:2}{var:3}{var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(true456true)",
             LR"(Render())");

    content = LR"({var:4}{var:4}{var:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.51.51.5)",
             LR"(Render())");
    ///

    content = LR"({var:0}-{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(A-A)", LR"(Render())");

    content = LR"({var:1}--{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(abc--A)", LR"(Render())");

    content = LR"({var:1}---{var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(abc---true)",
             LR"(Render())");

    content = LR"({var:2}{var:3}--{var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(true456--true)",
             LR"(Render())");

    content = LR"({var:4}--{var:4}{var:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5--1.51.5)",
             LR"(Render())");

    content = LR"({var:4}--{var:4}--{var:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5--1.5--1.5)",
             LR"(Render())");

    content = LR"({var:4}---{var:4}---{var:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5---1.5---1.5)",
             LR"(Render())");

    END_SUB_TEST;
}

static int TestVariableLTag3() {
    Value<wchar_t> value;
    const wchar_t *content;

    content = LR"({var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({var:a})", &value), LR"({var:a})",
             LR"(Render())");

    content = LR"({var:0[0]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({var:0[0]})", LR"(Render())");

    content = LR"({var:a[0]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({var:a[0]})", LR"(Render())");

    content = LR"({var:0[a]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({var:0[a]})", LR"(Render())");

    content = LR"({var:a[abc]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({var:a[abc]})", LR"(Render())");

    ////////////////

    value = JSON::Parse(LR"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = LR"({var:0})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({var:0})", LR"(Render())");

    content = LR"({var:0[0]})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0[0]})",
             LR"(Render())");

    content = LR"({var:0[0][0]})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0[0][0]})",
             LR"(Render())");

    /////

    content = LR"({var:1})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:1})", LR"(Render())");

    content = LR"({var:2})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:2})", LR"(Render())");

    content = LR"({var:1[a]})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:1[a]})",
             LR"(Render())");

    content = LR"({var:1[b]})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:1[b]})",
             LR"(Render())");

    ////

    value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5])");

    content = LR"({var:0)";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0)", LR"(Render())");

    content = LR"(var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(var:0})", LR"(Render())");

    content = LR"({v})";
    EQ_VALUE(Template::Render(content, &value), LR"({v})", LR"(Render())");

    content = LR"({va})";
    EQ_VALUE(Template::Render(content, &value), LR"({va})", LR"(Render())");

    content = LR"({var})";
    EQ_VALUE(Template::Render(content, &value), LR"({var})", LR"(Render())");

    content = LR"({var:})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:})", LR"(Render())");

    content = LR"({v:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({v:0})", LR"(Render())");

    content = LR"({va:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({va:0})", LR"(Render())");
    ////

    content = LR"({var:0{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0{var:0})",
             LR"(Render())");

    content = LR"(var:0{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(var:0A)", LR"(Render())");

    content = LR"(var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(var:0}A)", LR"(Render())");

    content = LR"({var:0{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0{var:0}A)",
             LR"(Render())");

    ////

    content = LR"({var:0{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0{var:0})",
             LR"(Render())");

    content = LR"(var:0{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(var:0A)", LR"(Render())");

    content = LR"(var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(var:0}A)", LR"(Render())");

    content = LR"({var:0{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0{var:0}A)",
             LR"(Render())");

    ////

    content = LR"({{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({A)", LR"(Render())");

    content = LR"({{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({AA)", LR"(Render())");

    content = LR"({v{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({vA)", LR"(Render())");

    content = LR"({v{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({vAA)", LR"(Render())");

    content = LR"({va{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({vaA)", LR"(Render())");

    content = LR"({va{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({vaAA)", LR"(Render())");

    content = LR"({var{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({varA)", LR"(Render())");

    content = LR"({var{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({varAA)", LR"(Render())");

    ///

    content = LR"({-{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({-A)", LR"(Render())");

    content = LR"({-{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({-AA)", LR"(Render())");

    content = LR"({v-{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({v-A)", LR"(Render())");

    content = LR"({v-{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({v-AA)", LR"(Render())");

    content = LR"({va-{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({va-A)", LR"(Render())");

    content = LR"({va-{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({va-AA)", LR"(Render())");

    content = LR"({var-{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var-A)", LR"(Render())");

    content = LR"({var-{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var-AA)", LR"(Render())");

    //

    content = LR"({var-0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var-0})", LR"(Render())");

    content = LR"({var 0})";
    EQ_VALUE(Template::Render(content, &value), LR"({var 0})", LR"(Render())");

    content = LR"({var:0 })";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0 })", LR"(Render())");

    content = LR"({var:0 )";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0 )", LR"(Render())");

    content = LR"({var:0)";
    EQ_VALUE(Template::Render(content, &value), LR"({var:0)", LR"(Render())");

    content = LR"( {var-0})";
    EQ_VALUE(Template::Render(content, &value), LR"( {var-0})", LR"(Render())");

    content = LR"( {var 0})";
    EQ_VALUE(Template::Render(content, &value), LR"( {var 0})", LR"(Render())");

    content = LR"( {var:0 })";
    EQ_VALUE(Template::Render(content, &value), LR"( {var:0 })",
             LR"(Render())");

    content = LR"( {var:0 )";
    EQ_VALUE(Template::Render(content, &value), LR"( {var:0 )", LR"(Render())");

    content = LR"( {var:0)";
    EQ_VALUE(Template::Render(content, &value), LR"( {var:0)", LR"(Render())");

    END_SUB_TEST;
}

static int TestVariableLTag4() {
    Value<wchar_t> value;

    value += LR"(<)";
    value += LR"(>)";
    value += LR"(&)";
    value += LR"(")";
    value += LR"(')";
    value += LR"(<>)";
    value += LR"(<&)";
    value += LR"(<&)";
    value += LR"(>")";
    value += LR"("')";
    value += LR"(<">)";
    value += LR"(<'>)";
    value += LR"(<&>)";
    value += LR"(&"&)";
    value += LR"("'")";
    value += LR"('<')";
    value += LR"('&')";
    value += LR"(<>&'")";
    value += LR"('"<>&)";
    value += LR"(<"&'>)";

    value += LR"(<<<<<)";
    value += LR"(>>>>>)";
    value += LR"(&&&&&)";
    value += LR"(""""")";
    value += LR"(''''')";

    value += LR"(A<)";
    value += LR"(A>)";
    value += LR"(A&)";
    value += LR"(A")";
    value += LR"(A')";
    value += LR"(<A)";
    value += LR"(>A)";
    value += LR"(&A)";
    value += LR"("A)";
    value += LR"('A)";
    value += LR"(A<A)";
    value += LR"(A>A)";
    value += LR"(A&A)";
    value += LR"(A"A)";
    value += LR"(A'A)";

    value += LR"(AA<AA)";
    value += LR"(AA>AA)";
    value += LR"(AA&AA)";
    value += LR"(AA"AA)";
    value += LR"(AA'AA)";

    value += LR"(AA<<<<AA)";
    value += LR"(AA>>>>AA)";
    value += LR"(AA&&&&AA)";
    value += LR"(AA""""AA)";
    value += LR"(AA''''AA)";

    value += LR"(<A>B'C"D&E'F"G<H>I&G"K)";
    value += LR"(AB"CD'EF<GH>IGK')";
    value += LR"("ABC'DEF<GHI>GK<)";
    value += LR"(A""BC<<DE>>FG''HI&&GK)";

#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
    EQ_VALUE(Template::Render(LR"({var:0})", &value), LR"(&lt;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:1})", &value), LR"(&gt;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:2})", &value), LR"(&amp;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:3})", &value), LR"(&quot;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:4})", &value), LR"(&apos;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:5})", &value), LR"(&lt;&gt;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:6})", &value), LR"(&lt;&amp;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:7})", &value), LR"(&lt;&amp;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:8})", &value), LR"(&gt;&quot;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:9})", &value), LR"(&quot;&apos;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:10})", &value), LR"(&lt;&quot;&gt;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:11})", &value), LR"(&lt;&apos;&gt;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:12})", &value), LR"(&lt;&amp;&gt;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:13})", &value), LR"(&amp;&quot;&amp;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:14})", &value), LR"(&quot;&apos;&quot;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:15})", &value), LR"(&apos;&lt;&apos;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:16})", &value), LR"(&apos;&amp;&apos;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:17})", &value),
             LR"(&lt;&gt;&amp;&apos;&quot;)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:18})", &value),
             LR"(&apos;&quot;&lt;&gt;&amp;)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:19})", &value),
             LR"(&lt;&quot;&amp;&apos;&gt;)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:20})", &value),
             LR"(&lt;&lt;&lt;&lt;&lt;)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:21})", &value),
             LR"(&gt;&gt;&gt;&gt;&gt;)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:22})", &value),
             LR"(&amp;&amp;&amp;&amp;&amp;)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:23})", &value),
             LR"(&quot;&quot;&quot;&quot;&quot;)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:24})", &value),
             LR"(&apos;&apos;&apos;&apos;&apos;)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:25})", &value), LR"(A&lt;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:26})", &value), LR"(A&gt;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:27})", &value), LR"(A&amp;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:28})", &value), LR"(A&quot;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:29})", &value), LR"(A&apos;)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:30})", &value), LR"(&lt;A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:31})", &value), LR"(&gt;A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:32})", &value), LR"(&amp;A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:33})", &value), LR"(&quot;A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:34})", &value), LR"(&apos;A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:35})", &value), LR"(A&lt;A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:36})", &value), LR"(A&gt;A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:37})", &value), LR"(A&amp;A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:38})", &value), LR"(A&quot;A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:39})", &value), LR"(A&apos;A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:40})", &value), LR"(AA&lt;AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:41})", &value), LR"(AA&gt;AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:42})", &value), LR"(AA&amp;AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:43})", &value), LR"(AA&quot;AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:44})", &value), LR"(AA&apos;AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:45})", &value),
             LR"(AA&lt;&lt;&lt;&lt;AA)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:46})", &value),
             LR"(AA&gt;&gt;&gt;&gt;AA)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:47})", &value),
             LR"(AA&amp;&amp;&amp;&amp;AA)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:48})", &value),
             LR"(AA&quot;&quot;&quot;&quot;AA)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:49})", &value),
             LR"(AA&apos;&apos;&apos;&apos;AA)", LR"(Render())");
    EQ_VALUE(
        Template::Render(LR"({var:50})", &value),
        LR"(&lt;A&gt;B&apos;C&quot;D&amp;E&apos;F&quot;G&lt;H&gt;I&amp;G&quot;K)",
        LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:51})", &value),
             LR"(AB&quot;CD&apos;EF&lt;GH&gt;IGK&apos;)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:52})", &value),
             LR"(&quot;ABC&apos;DEF&lt;GHI&gt;GK&lt;)", LR"(Render())");
    EQ_VALUE(
        Template::Render(LR"({var:53})", &value),
        LR"(A&quot;&quot;BC&lt;&lt;DE&gt;&gt;FG&apos;&apos;HI&amp;&amp;GK)",
        LR"(Render())");
#else
    EQ_VALUE(Template::Render(LR"({var:0})", &value), LR"(<)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:1})", &value), LR"(>)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:2})", &value), LR"(&)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:3})", &value), LR"(")", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:4})", &value), LR"(')", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:5})", &value), LR"(<>)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:6})", &value), LR"(<&)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:7})", &value), LR"(<&)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:8})", &value), LR"(>")", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:9})", &value), LR"("')", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:10})", &value), LR"(<">)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:11})", &value), LR"(<'>)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:12})", &value), LR"(<&>)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:13})", &value), LR"(&"&)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:14})", &value), LR"("'")",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:15})", &value), LR"('<')",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:16})", &value), LR"('&')",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:17})", &value), LR"(<>&'")",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:18})", &value), LR"('"<>&)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:19})", &value), LR"(<"&'>)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:20})", &value), LR"(<<<<<)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:21})", &value), LR"(>>>>>)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:22})", &value), LR"(&&&&&)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:23})", &value), LR"(""""")",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:24})", &value), LR"(''''')",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:25})", &value), LR"(A<)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:26})", &value), LR"(A>)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:27})", &value), LR"(A&)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:28})", &value), LR"(A")",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:29})", &value), LR"(A')",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:30})", &value), LR"(<A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:31})", &value), LR"(>A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:32})", &value), LR"(&A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:33})", &value), LR"("A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:34})", &value), LR"('A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:35})", &value), LR"(A<A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:36})", &value), LR"(A>A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:37})", &value), LR"(A&A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:38})", &value), LR"(A"A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:39})", &value), LR"(A'A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:40})", &value), LR"(AA<AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:41})", &value), LR"(AA>AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:42})", &value), LR"(AA&AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:43})", &value), LR"(AA"AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:44})", &value), LR"(AA'AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:45})", &value), LR"(AA<<<<AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:46})", &value), LR"(AA>>>>AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:47})", &value), LR"(AA&&&&AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:48})", &value), LR"(AA""""AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:49})", &value), LR"(AA''''AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:50})", &value),
             LR"(<A>B'C"D&E'F"G<H>I&G"K)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:51})", &value), LR"(AB"CD'EF<GH>IGK')",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:52})", &value), LR"("ABC'DEF<GHI>GK<)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({var:53})", &value),
             LR"(A""BC<<DE>>FG''HI&&GK)", LR"(Render())");
#endif

    END_SUB_TEST;
}

static int TestRawVariableLTag1() {
    const wchar_t *content;

    Value<wchar_t> value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    Value<wchar_t> sub_value = JSON::Parse(
        LR"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = LR"({raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(A)", LR"(Render())");

    content = LR"({raw:1})";
    EQ_VALUE(Template::Render(content, &value), LR"(abc)", LR"(Render())");

    content = LR"({raw:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(true)", LR"(Render())");

    content = LR"({raw:3})";
    EQ_VALUE(Template::Render(content, &value), LR"(456)", LR"(Render())");

    content = LR"({raw:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5)", LR"(Render())");

    content = LR"({raw:5[0]})";
    EQ_VALUE(Template::Render(content, &value), LR"(null)", LR"(Render())");

    content = LR"({raw:5[1]})";
    EQ_VALUE(Template::Render(content, &value), LR"(false)", LR"(Render())");

    content = LR"({raw:5[2][0]})";
    EQ_VALUE(Template::Render(content, &value), LR"(Qentem)", LR"(Render())");

    //////

    content = LR"({raw:key1})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(a)", LR"(Render())");

    content = LR"({raw:key2})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(ABC)", LR"(Render())");

    content = LR"({raw:key3})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(false)",
             LR"(Render())");

    content = LR"({raw:key4})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(100)", LR"(Render())");

    content = LR"({raw:key5})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(1.5)", LR"(Render())");

    content = LR"({raw:key6[one]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(1)", LR"(Render())");

    content = LR"({raw:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(null)", LR"(Render())");

    content = LR"({raw:key7[1]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(false)",
             LR"(Render())");

    content = LR"({raw:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(Qentem)",
             LR"(Render())");

    //
    content = LR"({raw:6[key1]})";
    EQ_VALUE(Template::Render(content, &value), LR"(a)", LR"(Render())");

    content = LR"({raw:6[key2]})";
    EQ_VALUE(Template::Render(content, &value), LR"(ABC)", LR"(Render())");

    content = LR"({raw:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), LR"(false)", LR"(Render())");

    content = LR"({raw:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), LR"(100)", LR"(Render())");

    content = LR"({raw:6[key5]})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5)", LR"(Render())");

    content = LR"({raw:6[key6][one]})";
    EQ_VALUE(Template::Render(content, &value), LR"(1)", LR"(Render())");

    ////////////////

    content = LR"(-{raw:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(-true)", LR"(Render())");

    content = LR"(-{raw:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(-null)",
             LR"(Render())");

    content = LR"(-{raw:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(-Qentem)",
             LR"(Render())");

    content = LR"(-{raw:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), LR"(-false)", LR"(Render())");

    content = LR"(-{raw:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), LR"(-100)", LR"(Render())");
    ////////////

    content = LR"({raw:2}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(true-)", LR"(Render())");

    content = LR"({raw:key7[0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(null-)",
             LR"(Render())");

    content = LR"({raw:key7[2][0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(Qentem-)",
             LR"(Render())");

    content = LR"({raw:6[key3]}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(false-)", LR"(Render())");

    content = LR"({raw:6[key4]}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(100-)", LR"(Render())");

    ////////////

    content = LR"(-{raw:2}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(-true-)", LR"(Render())");

    content = LR"(-{raw:key7[0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(-null-)",
             LR"(Render())");

    content = LR"(-{raw:key7[2][0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(-Qentem-)",
             LR"(Render())");

    content = LR"(-{raw:6[key3]}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(-false-)", LR"(Render())");

    content = LR"(-{raw:6[key4]}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(-100-)", LR"(Render())");

    ////////////

    content = LR"(------{raw:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(------true)",
             LR"(Render())");

    content = LR"(------{raw:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(------null)",
             LR"(Render())");

    content = LR"(------{raw:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(------Qentem)",
             LR"(Render())");

    content = LR"(------{raw:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), LR"(------false)",
             LR"(Render())");

    content = LR"(------{raw:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), LR"(------100)",
             LR"(Render())");

    ////////////

    content = LR"({raw:2}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(true------)",
             LR"(Render())");

    content = LR"({raw:key7[0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(null------)",
             LR"(Render())");

    content = LR"({raw:key7[2][0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(Qentem------)",
             LR"(Render())");

    content = LR"({raw:6[key3]}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(false------)",
             LR"(Render())");

    content = LR"({raw:6[key4]}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(100------)",
             LR"(Render())");

    ////////////

    content = LR"(------{raw:2}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(------true------)",
             LR"(Render())");

    content = LR"(------{raw:key7[0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(------null------)",
             LR"(Render())");

    content = LR"(------{raw:key7[2][0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), LR"(------Qentem------)",
             LR"(Render())");

    content = LR"(------{raw:6[key3]}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(------false------)",
             LR"(Render())");

    content = LR"(------{raw:6[key4]}------)";
    EQ_VALUE(Template::Render(content, &value), LR"(------100------)",
             LR"(Render())");

    content = LR"({raw:key7[2[0]})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:key7[2[0]})",
             LR"(Render())");

    content = LR"({raw:6key3]})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:6key3]})",
             LR"(Render())");

    END_SUB_TEST;
}

static int TestRawVariableLTag2() {
    Value<wchar_t> value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5])");
    const wchar_t *content;

    content = LR"({raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(AA)", LR"(Render())");

    content = LR"({raw:1}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(abcA)", LR"(Render())");

    content = LR"({raw:1}{raw:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(abctrue)", LR"(Render())");

    content = LR"({raw:2}{raw:3}{raw:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(true456true)",
             LR"(Render())");

    content = LR"({raw:4}{raw:4}{raw:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.51.51.5)",
             LR"(Render())");
    ///

    content = LR"({raw:0}-{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(A-A)", LR"(Render())");

    content = LR"({raw:1}--{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(abc--A)", LR"(Render())");

    content = LR"({raw:1}---{raw:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(abc---true)",
             LR"(Render())");

    content = LR"({raw:2}{raw:3}--{raw:2})";
    EQ_VALUE(Template::Render(content, &value), LR"(true456--true)",
             LR"(Render())");

    content = LR"({raw:4}--{raw:4}{raw:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5--1.51.5)",
             LR"(Render())");

    content = LR"({raw:4}--{raw:4}--{raw:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5--1.5--1.5)",
             LR"(Render())");

    content = LR"({raw:4}---{raw:4}---{raw:4})";
    EQ_VALUE(Template::Render(content, &value), LR"(1.5---1.5---1.5)",
             LR"(Render())");

    END_SUB_TEST;
}

static int TestRawVariableLTag3() {
    Value<wchar_t> value;
    const wchar_t *content;

    content = LR"({raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:0})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({raw:a})", &value), LR"({raw:a})",
             LR"(Render())");

    content = LR"({raw:0[0]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({raw:0[0]})", LR"(Render())");

    content = LR"({raw:a[0]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({raw:a[0]})", LR"(Render())");

    content = LR"({raw:0[a]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({raw:0[a]})", LR"(Render())");

    content = LR"({raw:a[abc]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({raw:a[abc]})", LR"(Render())");

    ////////////////

    value = JSON::Parse(LR"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = LR"({raw:0})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             LR"({raw:0})", LR"(Render())");

    content = LR"({raw:0[0]})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:0[0]})",
             LR"(Render())");

    content = LR"({raw:0[0][0]})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:0[0][0]})",
             LR"(Render())");

    /////

    content = LR"({raw:1})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:1})", LR"(Render())");

    content = LR"({raw:2})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:2})", LR"(Render())");

    content = LR"({raw:1[a]})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:1[a]})",
             LR"(Render())");

    content = LR"({raw:1[b]})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:1[b]})",
             LR"(Render())");

    ////

    value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5])");

    content = LR"({raw:0)";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:0)", LR"(Render())");

    content = LR"(raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(raw:0})", LR"(Render())");

    content = LR"({r})";
    EQ_VALUE(Template::Render(content, &value), LR"({r})", LR"(Render())");

    content = LR"({ra})";
    EQ_VALUE(Template::Render(content, &value), LR"({ra})", LR"(Render())");

    content = LR"({raw})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw})", LR"(Render())");

    content = LR"({raw:})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:})", LR"(Render())");

    content = LR"({r:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({r:0})", LR"(Render())");

    content = LR"({ra:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({ra:0})", LR"(Render())");
    ////

    content = LR"({raw:0{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:0{raw:0})",
             LR"(Render())");

    content = LR"(raw:0{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(raw:0A)", LR"(Render())");

    content = LR"(raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(raw:0}A)", LR"(Render())");

    content = LR"({raw:0{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:0{raw:0}A)",
             LR"(Render())");

    ////

    content = LR"({raw:0{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:0{raw:0})",
             LR"(Render())");

    content = LR"(raw:0{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(raw:0A)", LR"(Render())");

    content = LR"(raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"(raw:0}A)", LR"(Render())");

    content = LR"({raw:0{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:0{raw:0}A)",
             LR"(Render())");

    ////

    content = LR"({{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({A)", LR"(Render())");

    content = LR"({{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({AA)", LR"(Render())");

    content = LR"({r{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({rA)", LR"(Render())");

    content = LR"({r{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({rAA)", LR"(Render())");

    content = LR"({ra{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({raA)", LR"(Render())");

    content = LR"({ra{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({raAA)", LR"(Render())");

    content = LR"({raw{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({rawA)", LR"(Render())");

    content = LR"({raw{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({rawAA)", LR"(Render())");

    ///

    content = LR"({-{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({-A)", LR"(Render())");

    content = LR"({-{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({-AA)", LR"(Render())");

    content = LR"({r-{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({r-A)", LR"(Render())");

    content = LR"({r-{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({r-AA)", LR"(Render())");

    content = LR"({ra-{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({ra-A)", LR"(Render())");

    content = LR"({ra-{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({ra-AA)", LR"(Render())");

    content = LR"({raw-{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw-A)", LR"(Render())");

    content = LR"({raw-{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw-AA)", LR"(Render())");

    //

    content = LR"({raw-0})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw-0})", LR"(Render())");

    content = LR"({raw 0})";
    EQ_VALUE(Template::Render(content, &value), LR"({raw 0})", LR"(Render())");

    content = LR"({raw:0 })";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:0 })", LR"(Render())");

    content = LR"({raw:0 )";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:0 )", LR"(Render())");

    content = LR"({raw:0)";
    EQ_VALUE(Template::Render(content, &value), LR"({raw:0)", LR"(Render())");

    content = LR"({ raw-0})";
    EQ_VALUE(Template::Render(content, &value), LR"({ raw-0})", LR"(Render())");

    content = LR"({ raw 0})";
    EQ_VALUE(Template::Render(content, &value), LR"({ raw 0})", LR"(Render())");

    content = LR"({ raw:0 })";
    EQ_VALUE(Template::Render(content, &value), LR"({ raw:0 })",
             LR"(Render())");

    content = LR"({ raw:0 )";
    EQ_VALUE(Template::Render(content, &value), LR"({ raw:0 )", LR"(Render())");

    content = LR"({ raw:0)";
    EQ_VALUE(Template::Render(content, &value), LR"({ raw:0)", LR"(Render())");
    END_SUB_TEST;
}

static int TestRawVariableLTag4() {
    Value<wchar_t> value;

    value += LR"(<)";
    value += LR"(>)";
    value += LR"(&)";
    value += LR"(")";
    value += LR"(')";
    value += LR"(<>)";
    value += LR"(<&)";
    value += LR"(<&)";
    value += LR"(>")";
    value += LR"("')";
    value += LR"(<">)";
    value += LR"(<'>)";
    value += LR"(<&>)";
    value += LR"(&"&)";
    value += LR"("'")";
    value += LR"('<')";
    value += LR"('&')";
    value += LR"(<>&'")";
    value += LR"('"<>&)";
    value += LR"(<"&'>)";

    value += LR"(<<<<<)";
    value += LR"(>>>>>)";
    value += LR"(&&&&&)";
    value += LR"(""""")";
    value += LR"(''''')";

    value += LR"(A<)";
    value += LR"(A>)";
    value += LR"(A&)";
    value += LR"(A")";
    value += LR"(A')";
    value += LR"(<A)";
    value += LR"(>A)";
    value += LR"(&A)";
    value += LR"("A)";
    value += LR"('A)";
    value += LR"(A<A)";
    value += LR"(A>A)";
    value += LR"(A&A)";
    value += LR"(A"A)";
    value += LR"(A'A)";

    value += LR"(AA<AA)";
    value += LR"(AA>AA)";
    value += LR"(AA&AA)";
    value += LR"(AA"AA)";
    value += LR"(AA'AA)";

    value += LR"(AA<<<<AA)";
    value += LR"(AA>>>>AA)";
    value += LR"(AA&&&&AA)";
    value += LR"(AA""""AA)";
    value += LR"(AA''''AA)";

    value += LR"(<A>B'C"D&E'F"G<H>I&G"K)";
    value += LR"(AB"CD'EF<GH>IGK')";
    value += LR"("ABC'DEF<GHI>GK<)";
    value += LR"(A""BC<<DE>>FG''HI&&GK)";

    EQ_VALUE(Template::Render(LR"({raw:0})", &value), LR"(<)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:1})", &value), LR"(>)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:2})", &value), LR"(&)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:3})", &value), LR"(")", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:4})", &value), LR"(')", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:5})", &value), LR"(<>)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:6})", &value), LR"(<&)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:7})", &value), LR"(<&)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:8})", &value), LR"(>")", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:9})", &value), LR"("')", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:10})", &value), LR"(<">)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:11})", &value), LR"(<'>)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:12})", &value), LR"(<&>)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:13})", &value), LR"(&"&)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:14})", &value), LR"("'")",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:15})", &value), LR"('<')",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:16})", &value), LR"('&')",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:17})", &value), LR"(<>&'")",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:18})", &value), LR"('"<>&)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:19})", &value), LR"(<"&'>)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:20})", &value), LR"(<<<<<)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:21})", &value), LR"(>>>>>)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:22})", &value), LR"(&&&&&)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:23})", &value), LR"(""""")",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:24})", &value), LR"(''''')",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:25})", &value), LR"(A<)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:26})", &value), LR"(A>)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:27})", &value), LR"(A&)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:28})", &value), LR"(A")",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:29})", &value), LR"(A')",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:30})", &value), LR"(<A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:31})", &value), LR"(>A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:32})", &value), LR"(&A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:33})", &value), LR"("A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:34})", &value), LR"('A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:35})", &value), LR"(A<A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:36})", &value), LR"(A>A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:37})", &value), LR"(A&A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:38})", &value), LR"(A"A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:39})", &value), LR"(A'A)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:40})", &value), LR"(AA<AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:41})", &value), LR"(AA>AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:42})", &value), LR"(AA&AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:43})", &value), LR"(AA"AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:44})", &value), LR"(AA'AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:45})", &value), LR"(AA<<<<AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:46})", &value), LR"(AA>>>>AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:47})", &value), LR"(AA&&&&AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:48})", &value), LR"(AA""""AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:49})", &value), LR"(AA''''AA)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:50})", &value),
             LR"(<A>B'C"D&E'F"G<H>I&G"K)", LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:51})", &value), LR"(AB"CD'EF<GH>IGK')",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:52})", &value), LR"("ABC'DEF<GHI>GK<)",
             LR"(Render())");
    EQ_VALUE(Template::Render(LR"({raw:53})", &value),
             LR"(A""BC<<DE>>FG''HI&&GK)", LR"(Render())");

    END_SUB_TEST;
}

static int TestMathLTag1() {
    Value<wchar_t> value;

    value[LR"(a1)"] = 5;
    value[LR"(a2)"] = true;
    value[LR"(a3)"] = nullptr;
    value[LR"(a4)"] = false;
    value[LR"(a5)"] = LR"(10)";
    value[LR"(a6)"] = LR"(20)";
    value[LR"(a7)"] = 6;
    value[LR"(a8)"] = 1;
    value[LR"(a9)"] = LR"(1)";
    value[LR"(eq)"] = LR"((8+1+{var:a8}))";

    EQ_VALUE(Template::Render(LR"({math:1+1})", &value), LR"(2)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a1}+8})", &value), LR"(13)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a7}+8})", &value), LR"(14)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a7}+{var:a1}})", &value), LR"(11)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a5}+{var:a1}})", &value), LR"(15)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a1}+{var:a5}})", &value), LR"(15)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a6}+{var:a5}})", &value), LR"(30)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a6}*{var:a2}})", &value), LR"(20)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a6}*{var:a4}})", &value), LR"(0)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a6}*{var:a7}})", &value),
             LR"(120)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a7}+{var:a6}})", &value), LR"(26)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a1}+{var:a2}})", &value), LR"(6)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a8}=={var:a2}})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a2}=={var:a8}})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a5}!={var:a1}})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a2}!={var:a4}})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a2}==true})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a9}=={var:a8}})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a8}=={var:a9}})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:1=={var:a8}})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:6-5==({var:a9})})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:6-5==({var:a8})})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:null!={var:a3}})", &value), LR"(0)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:({var:a3})==(0)})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a1}+{var:a7}})", &value), LR"(11)",
             LR"(Render())");

    EQ_VALUE(
        Template::Render(LR"({math:{var:a1}+{var:a7}}{math:{var:a1}+{var:a7}})",
                         &value),
        LR"(1111)", LR"(Render())");

    EQ_VALUE(
        Template::Render(
            LR"({math:{var:a1}+{var:a7}}*{math:{var:a1}+{var:a7}})", &value),
        LR"(11*11)", LR"(Render())");

    EQ_VALUE(
        Template::Render(
            LR"({math:{var:a1}+{var:a7}}##{math:{var:a1}+{var:a7}})", &value),
        LR"(11##11)", LR"(Render())");

    EQ_VALUE(
        Template::Render(
            LR"({math:{var:a1}+{var:a7}}&&&%%^^&&*{math:{var:a1}+{var:a7}})",
            &value),
        LR"(11&&&%%^^&&*11)", LR"(Render())");

    ///////////////////

    EQ_VALUE(Template::Render(LR"({math: {var:a1}+8})", &value), LR"(13)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:  {var:a7}+8})", &value), LR"(14)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:     {var:a7}+{var:a1}})", &value),
             LR"(11)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a5} +{var:a1}})", &value),
             LR"(15)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a1}  +{var:a5}})", &value),
             LR"(15)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a6}    +{var:a5}})", &value),
             LR"(30)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a6}* {var:a2}})", &value),
             LR"(20)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a6}*  {var:a4}})", &value),
             LR"(0)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a6}*      {var:a7}})", &value),
             LR"(120)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a7}+{var:a6} })", &value),
             LR"(26)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a1}+{var:a2}  })", &value),
             LR"(6)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a8}=={var:a2}      })", &value),
             LR"(1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a2}=={var:a8}})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math: {var:a5}!={var:a1} })", &value),
             LR"(1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:  {var:a2}!={var:a4}  })", &value),
             LR"(1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:    1=={var:a9}     })", &value),
             LR"(1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a9} == {var:a8}})", &value),
             LR"(1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a8}  ==  {var:a9}})", &value),
             LR"(1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:1==          {var:a8}})", &value),
             LR"(1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:6-5         ==1})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:1          ==            {var:a8}})",
                              &value),
             LR"(1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:     0     !=    ({var:a3})        })",
                              &value),
             LR"(0)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:a3}       ==       null     })",
                              &value),
             LR"(1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:eq}})", &value), LR"(10)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:10==(8+1+{var:a8})})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:eq}==9+1})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:(5*2)=={var:eq}})", &value), LR"(1)",
             LR"(Render())");

    //////////////
    value.Reset();

    value += 5;
    value += true;
    value += nullptr;
    value += false;
    value += LR"(10)";
    value += LR"(20)";
    value += 6;
    value += 1;
    value += LR"(1)";
    value += LR"(Qentem)";

    EQ_VALUE(Template::Render(LR"({math:{var:0}+8})", &value), LR"(13)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:6}+8})", &value), LR"(14)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:6}+{var:0}})", &value), LR"(11)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:4}+{var:0}})", &value), LR"(15)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:0}+{var:4}})", &value), LR"(15)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:5}+{var:4}})", &value), LR"(30)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:5}*{var:1}})", &value), LR"(20)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:5}*{var:3}})", &value), LR"(0)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:5}*{var:6}})", &value), LR"(120)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:6}+{var:5}})", &value), LR"(26)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:0}+{var:1}})", &value), LR"(6)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:7}=={var:1}})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:1}=={var:7}})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:4}!={var:0}})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:1}!={var:3}})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:({var:1})==({var:8})})", &value),
             LR"(1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"(-{math:{var:8}=={var:7}})", &value), LR"(-1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"(--{math:{var:7}=={var:8}})", &value),
             LR"(--1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"(---{math:1=={var:7}})", &value), LR"(---1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:1==({var:8})}-)", &value), LR"(1-)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:6-5==({var:7})}--)", &value), LR"(1--)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:0==({var:2})}---)", &value), LR"(1---)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"(-{math:{var:2}!=null}-)", &value), LR"(-0-)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"(--{math:Qente=={var:9}}--)", &value),
             LR"(--0--)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"(---{math:Qente !={var:9}}---)", &value),
             LR"(---1---)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:  Qentem   =={var:9}})", &value),
             LR"(1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:  Qentem!={var:9}})", &value), LR"(0)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:9}   ==    Qente})", &value),
             LR"(0)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:9} !=    Qente    })", &value),
             LR"(1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:  {var:9}   ==Qentem})", &value),
             LR"(1)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math: {var:9} !=Qentem})", &value), LR"(0)",
             LR"(Render())");

    /////////

    EQ_VALUE(Template::Render(LR"({math: true == {var:1}})", &value), LR"(1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math: {var:1} == true})", &value), LR"(1)",
             LR"(Render())");

    END_SUB_TEST;
}

static int TestMathLTag2() {
    Value<wchar_t> value;

    value += Array<Value<wchar_t>>();
    value += HArray<Value<wchar_t>, wchar_t>();
    value += 5;

    EQ_VALUE(Template::Render(LR"({math:{var:0}+8})", &value),
             LR"({math:{var:0}+8})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:1}+8})", &value),
             LR"({math:{var:1}+8})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:2}+{var:0}})", &value),
             LR"({math:{var:2}+{var:0}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:2}+{var:0}})", &value),
             LR"({math:{var:2}+{var:0}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:0}+{var:1}})", &value),
             LR"({math:{var:0}+{var:1}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:1}+{var:2}})", &value),
             LR"({math:{var:1}+{var:2}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:2}*{var:1}})", &value),
             LR"({math:{var:2}*{var:1}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:0}*{var:1}})", &value),
             LR"({math:{var:0}*{var:1}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:0}*{var:2}})", &value),
             LR"({math:{var:0}*{var:2}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:2}+{var:0}})", &value),
             LR"({math:{var:2}+{var:0}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:1}+{var:2}})", &value),
             LR"({math:{var:1}+{var:2}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:0}=={var:1}})", &value),
             LR"({math:{var:0}=={var:1}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:1}=={var:0}})", &value),
             LR"({math:{var:1}=={var:0}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:0}!={var:2}})", &value),
             LR"({math:{var:0}!={var:2}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:2}!={var:0}})", &value),
             LR"({math:{var:2}!={var:0}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:1}=={var:2}})", &value),
             LR"({math:{var:1}=={var:2}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:8}=={var:7}})", &value),
             LR"({math:{var:8}=={var:7}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:7}=={var:2}})", &value),
             LR"({math:{var:7}=={var:2}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:1=={var:7}})", &value),
             LR"({math:1=={var:7}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:(6-5)=={var:8}})", &value),
             LR"({math:(6-5)=={var:8}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:(6-5)=={var:0}})", &value),
             LR"({math:(6-5)=={var:0}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:0}=={var:8}})", &value),
             LR"({math:{var:0}=={var:8}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:0}=={var:0}})", &value),
             LR"({math:{var:0}=={var:0}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:0=={var:1}})", &value),
             LR"({math:0=={var:1}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:1}!=0})", &value),
             LR"({math:{var:1}!=0})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:W={var:0}})", &value),
             LR"({math:W={var:0}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:W=={var:0}})", &value),
             LR"({math:W=={var:0}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:2}==c})", &value),
             LR"({math:{var:2}==c})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:abc=={var:2}})", &value),
             LR"({math:abc=={var:2}})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:sds})", &value), LR"({math:sds})",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:1)", &value), LR"({math:1)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"(math:1})", &value), LR"(math:1})",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:{var:2})", &value), LR"({math:5)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({{math:{var:2}+5})", &value), LR"({10)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({m{var:2}})", &value), LR"({m5})",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({ma{var:2}})", &value), LR"({ma5})",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({mat{var:2}})", &value), LR"({mat5})",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math{var:2}})", &value), LR"({math5})",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math {var:2}})", &value), LR"({math 5})",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:})", &value), LR"({math:})",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math-{var:2}}{math:{var:2}+5})", &value),
             LR"({math-5}10)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math-4}{math:{var:2}+5})", &value),
             LR"({math-4}10)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math-4} {math:{var:2}+5})", &value),
             LR"({math-4} 10)", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:4)", &value), LR"({math:4)",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:4    )", &value), LR"({math:4    )",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:A==1+1})", &value), LR"({math:A==1+1})",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:(A)!=1+1})", &value),
             LR"({math:(A)!=1+1})", LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:1+1==A})", &value), LR"({math:1+1==A})",
             LR"(Render())");

    EQ_VALUE(Template::Render(LR"({math:1+1!=(A)})", &value),
             LR"({math:1+1!=(A)})", LR"(Render())");

    END_SUB_TEST;
}

static int TestInlineIfLTag() {
    Value<wchar_t> value;
    const wchar_t *content;

    value += 0;
    value += 1;
    value += -1;
    value += true;
    value += false;
    value += nullptr;
    value += LR"(ABC)";
    value += Array<Value<wchar_t>>();

    content = LR"({if case="0" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(F)", LR"(Render())");

    content = LR"({if case="-1" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(F)", LR"(Render())");

    content = LR"({if case="0.1" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    content = LR"({if case="1" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    ////

    content = LR"({if case="0" true="T"})";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"({if case="-1" true="T"})";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"({if case="0.1" true="T"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    content = LR"({if case="1" true="T"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    ///

    content = LR"({if case="0" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(F)", LR"(Render())");

    content = LR"({if case="-1" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(F)", LR"(Render())");

    content = LR"({if case="0.1" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"({if case="1" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    /////

    content = LR"({if case="{var:0}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(F)", LR"(Render())");

    content = LR"({if case="{var:2}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(F)", LR"(Render())");

    content = LR"({if case="{raw:4}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(F)", LR"(Render())");

    content = LR"({if case="{var:5}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(F)", LR"(Render())");

    content = LR"({if case="{var:6}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"({if case="fas" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"({if case="{var:7}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"({if case="{var:20}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    ////
    content = LR"({if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    content = LR"({if case="{var:3}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    content = LR"({if case="{var:6}==ABC" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    /////////////////

    content = LR"({if case=" {var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    content = LR"({if case="{var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    content = LR"({if case=" {var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    content = LR"({if case="  {var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    content = LR"({if case="{var:1}  " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    content = LR"({if case="  {var:1}  " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    content = LR"({if case="      {var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    content = LR"({if case="{var:1}          " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    content = LR"({if case="    {var:1}        " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");
    ////

    content = LR"(-{if case=" {var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(-T)", LR"(Render())");

    content = LR"({if case=" {var:1} " true="T" false="F"}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(T-)", LR"(Render())");

    content = LR"(-{if case=" {var:1} " true="T" false="F"}-)";
    EQ_VALUE(Template::Render(content, &value), LR"(-T-)", LR"(Render())");

    content = LR"(--{if case=" {var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(--T)", LR"(Render())");

    content = LR"({if case=" {var:1} " true="T" false="F"}--)";
    EQ_VALUE(Template::Render(content, &value), LR"(T--)", LR"(Render())");

    content = LR"(--{if case=" {var:1} " true="T" false="F"}--)";
    EQ_VALUE(Template::Render(content, &value), LR"(--T--)", LR"(Render())");

    content = LR"(---{if case=" {var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(---T)", LR"(Render())");

    content = LR"({if case=" {var:1} " true="T" false="F"}---)";
    EQ_VALUE(Template::Render(content, &value), LR"(T---)", LR"(Render())");

    content = LR"(---{if case=" {var:1} " true="T" false="F"}---)";
    EQ_VALUE(Template::Render(content, &value), LR"(---T---)", LR"(Render())");

    content =
        LR"({if case="1" true="T"}{if case="0" false="F"}{if case="1" true="T"})";
    EQ_VALUE(Template::Render(content, &value), LR"(TFT)", LR"(Render())");

    ///////

    content =
        LR"({if case="{var:7}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"(T)", LR"(Render())");

    content = LR"({if case="01" true="{var:3}" false="{var:4}"}--)";
    EQ_VALUE(Template::Render(content, &value), LR"(--)", LR"(Render())");

    content = LR"({if case="" true="c" false="d"})";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"({i)";
    EQ_VALUE(Template::Render(content, &value), LR"({i)", LR"(Render())");

    content = LR"({if)";
    EQ_VALUE(Template::Render(content, &value), LR"({if)", LR"(Render())");

    content = LR"({if})";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"({{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"({T)", LR"(Render())");

    content = LR"({i{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"({iT)", LR"(Render())");

    content = LR"({if{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"({ifT)", LR"(Render())");

    content = LR"({if case="{var:1}"                    put="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content =
        LR"({if{if case="1" true="T" false="F"}}{if case="1" true="T" false="F"})";

    EQ_VALUE(Template::Render(content, &value), LR"(TT)", LR"(Render())");

    content =
        LR"({if{if case="{raw:1}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), LR"({ifTT)", LR"(Render())");

    /////
    content = LR"({if case="0" true="{var:3}" false="{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), LR"(false)", LR"(Render())");

    content = LR"({if case="1" true="{var:3}" false="{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), LR"(true)", LR"(Render())");

    content = LR"({if case="0" true="{raw:3}{raw:3}" false="{var:4}{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), LR"(falsefalse)",
             LR"(Render())");

    content =
        LR"({if case="1000" true="{var:3}{var:3}" false="{var:4}{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), LR"(truetrue)", LR"(Render())");

    content =
        LR"({if case="0" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), LR"(false---false)",
             LR"(Render())");

    content =
        LR"({if case="1" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), LR"(true---true)",
             LR"(Render())");

    content = LR"({if case="0" true="{var:10}" false="{var:20}"})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:20})", LR"(Render())");

    content = LR"({if case="1" true="{var:10}" false="{var:20}"})";
    EQ_VALUE(Template::Render(content, &value), LR"({var:10})", LR"(Render())");

    content = LR"({if case="1" true="1" false="0")";
    EQ_VALUE(Template::Render(content, &value),
             LR"({if case="1" true="1" false="0")", LR"(Render())");

    content = LR"({if case="1" true="1" false="0")";
    EQ_VALUE(Template::Render(content, &value),
             LR"({if case="1" true="1" false="0")", LR"(Render())");

    ///////
    Value<wchar_t> value2;

    value2 += LR"(&)";
    value2 += LR"(")";
    value2 += 10;
    value2 += 5;
    value2 += 15;

    content = LR"({if case="1" true="{var:0}" false="{var:1}"})";
#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
    EQ_VALUE(Template::Render(content, &value2), LR"(&amp;)", LR"(Render())");
#else
    EQ_VALUE(Template::Render(content, &value2), LR"(&)", LR"(Render())");
#endif

    content = LR"({if case="1" true="{raw:0}" false="{raw:1}"})";
    EQ_VALUE(Template::Render(content, &value2), LR"(&)", LR"(Render())");

    content = LR"({if case="0" true="{var:0}" false="{var:1}"})";
#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
    EQ_VALUE(Template::Render(content, &value2), LR"(&quot;)", LR"(Render())");
#else
    EQ_VALUE(Template::Render(content, &value2), LR"(")", LR"(Render())");
#endif

    content = LR"({if case="0" true="{raw:0}" false="{raw:1}"})";
    EQ_VALUE(Template::Render(content, &value2), LR"(")", LR"(Render())");

    content =
        LR"({if case="{var:2}+{var:3} == {var:4}" true="{math:{var:3}+{var:4}}" false="{math: {var:2}+{var:4}}"})";
    EQ_VALUE(Template::Render(content, &value2), LR"(20)", LR"(Render())");

    content =
        LR"({if case=" {var:4}-{var:3} != {var:2} " true="{math: {var:3}+{var:4} } " false=" {math: {var:2}+{var:4} } "})";
    EQ_VALUE(Template::Render(content, &value2), LR"( 25 )", LR"(Render())");

    END_SUB_TEST;
}

static int TestLoopLTag1() {
    Value<wchar_t> value;
    const wchar_t *content;

    value += 100;
    value += -50;
    value += LR"(Qentem)";
    value += true;
    value += false;
    value += nullptr;
    value += 3;

    content = LR"(<loop repeat="10">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(AAAAAAAAAA)",
             LR"(Render())");

    content = LR"(<loop             repeat="1">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(A)", LR"(Render())");

    content = LR"(<loop<loop repeat="1">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(<loopA)", LR"(Render())");

    content = LR"(<loop repeat="3"         >ABC</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(ABCABCABC)",
             LR"(Render())");

    content = LR"(-<loop repeat="3">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(-AAA)", LR"(Render())");

    content = LR"(<loop repeat="3">A</loop>-)";
    EQ_VALUE(Template::Render(content, &value), LR"(AAA-)", LR"(Render())");

    content = LR"(-<loop repeat="3">A</loop>-)";
    EQ_VALUE(Template::Render(content, &value), LR"(-AAA-)", LR"(Render())");

    content = LR"(--<loop repeat="3">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(--AAA)", LR"(Render())");

    content = LR"(<loop repeat="3">A</loop>--)";
    EQ_VALUE(Template::Render(content, &value), LR"(AAA--)", LR"(Render())");

    content = LR"(--<loop repeat="3">A</loop>--)";
    EQ_VALUE(Template::Render(content, &value), LR"(--AAA--)", LR"(Render())");

    content = LR"(---<loop repeat="3">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(---AAA)", LR"(Render())");

    content = LR"(<loop repeat="3">A</loop>---)";
    EQ_VALUE(Template::Render(content, &value), LR"(AAA---)", LR"(Render())");

    content = LR"(---<loop repeat="3">A</loop>---)";
    EQ_VALUE(Template::Render(content, &value), LR"(---AAA---)",
             LR"(Render())");

    content = LR"(<loop repeat="2">A</loop><loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(AABBB)", LR"(Render())");

    content = LR"(<loop repeat="2">A</loop>-<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(AA-BBB)", LR"(Render())");

    content = LR"(<loop repeat="2">A</loop>--<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(AA--BBB)", LR"(Render())");

    content = LR"(<loop repeat="2">A</loop>---<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(AA---BBB)", LR"(Render())");

    content =
        LR"(<loop repeat="2">A</loop>            <loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(AA            BBB)",
             LR"(Render())");

    ////

    content =
        LR"(<loop repeat="4">CC</loop><loop repeat="2">A</loop><loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(CCCCCCCCAABBB)",
             LR"(Render())");

    content =
        LR"(<loop repeat="4">CC</loop>-<loop repeat="2">A</loop>-<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(CCCCCCCC-AA-BBB)",
             LR"(Render())");

    content =
        LR"(<loop repeat="4">CC</loop>--<loop repeat="2">A</loop>--<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(CCCCCCCC--AA--BBB)",
             LR"(Render())");

    content =
        LR"(<loop repeat="4">CC</loop>---<loop repeat="2">A</loop>---<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(CCCCCCCC---AA---BBB)",
             LR"(Render())");

    content =
        LR"(<loop repeat="4">CC</loop>     <loop repeat="2">A</loop>            <loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value),
             LR"(CCCCCCCC     AA            BBB)", LR"(Render())");

    ////////////////

    content = LR"(<loop repeat="6"value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value),
             LR"(100, -50, Qentem, true, false, null, )", LR"(Render())");

    content =
        LR"(<loop repeat="6" value="loop1-value">loop1-value, loop1-value </loop>)";
    EQ_VALUE(
        Template::Render(content, &value),
        LR"(100, 100 -50, -50 Qentem, Qentem true, true false, false null, null )",
        LR"(Render())");

    content = LR"(<loop index="2" repeat="4" value="loop1-value">loop1-value{if
        case="loop1-value != null" true=", "}</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(Qentem, true, false, null)",
             LR"(Render())");

    ////////////////

    value.Reset();
    value += 0;
    value += 1;
    value += 2;

    content = LR"(<loop repeat="3" value="loop1-value"><loop
        repeat="2" value="loop2-value">(loop1-value: loop2-value) </loop></loop>)";
    EQ_VALUE(Template::Render(content, &value),
             LR"((0: 0) (0: 1) (1: 0) (1: 1) (2: 0) (2: 1) )", LR"(Render())");

    content =
        LR"(<loop repeat="2" value="loop1-value"><loop repeat="2" value="loop2-value"><loop
        repeat="2" value="loop3-value">(loop1-value: loop2-value: loop3-value) </loop></loop></loop>)";
    EQ_VALUE(
        Template::Render(content, &value),
        LR"((0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1) )",
        LR"(Render())");

    END_SUB_TEST;
}

static int TestLoopLTag2() {
    Value<wchar_t> value3;
    const wchar_t *content;

    Value<wchar_t> value1 =
        JSON::Parse(LR"([100, -50, "A", true, false, null])");
    Value<wchar_t> value2 = JSON::Parse(
        LR"({"k-1": 4, "k-2":1.5, "k-3":"ABC", "k-4":true, "k-5":false, "k-6":null})");

    //////////////////////
    value3[LR"(arr1)"] = value1;

    content = LR"(<loop value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value1),
             LR"(100, -50, A, true, false, null, )", LR"(Render())");

    content = LR"(<loop value="loop1-value" index="3">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value2), LR"(true, false, null, )",
             LR"(Render())");

    content = LR"(<loop value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value2),
             LR"(4, 1.5, ABC, true, false, null, )", LR"(Render())");

    content = LR"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             LR"(100, -50, A, true, false, null, )", LR"(Render())");

    value3[LR"(arr1)"] = value2;

    content = LR"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             LR"(4, 1.5, ABC, true, false, null, )", LR"(Render())");

    //////////////////////
    value3[LR"(arr1)"] = value1;

    content = LR"(<loop value="loop1-value" >loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value1),
             LR"(100, -50, A, true, false, null, )", LR"(Render())");

    content = LR"(<loop value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value2),
             LR"(4, 1.5, ABC, true, false, null, )", LR"(Render())");

    content = LR"(<loop value="loop1-value"set="arr1">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             LR"(100, -50, A, true, false, null, )", LR"(Render())");

    value3[LR"(arr1)"] = value2;

    content = LR"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             LR"(4, 1.5, ABC, true, false, null, )", LR"(Render())");

    //////////////////////
    value3.Reset();
    value3[LR"(arr1)"][LR"(arr2)"][LR"(arr3)"] = value1;

    content =
        LR"(<loop set="arr1[arr2][arr3]" value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             LR"(100, -50, A, true, false, null, )", LR"(Render())");

    value3.Reset();
    value3[0][0] += value2;

    content = LR"(<loop set="0[0][0]"value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             LR"(4, 1.5, ABC, true, false, null, )", LR"(Render())");

    value3.Reset();
    value3[LR"(k1)"][0][LR"(k3)"] = value1;

    content =
        LR"(<loop value="loop1-value" set="k1[0][k3]">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             LR"(100, -50, A, true, false, null, )", LR"(Render())");

    value3.Reset();
    value3[0][LR"(k2)"] += value2;

    content =
        LR"(<loop set="0[k2][0]"key="loop1-key"value="loop1-value">loop1-value, loop1-value, </loop>)";
    EQ_VALUE(
        Template::Render(content, &value3),
        LR"(4, 4, 1.5, 1.5, ABC, ABC, true, true, false, false, null, null, )",
        LR"(Render())");

    value3 = JSON::Parse(LR"({"group":[[10],[20],[30]]})");

    content =
        LR"(<loop set="group" value="_Val1"><loop set="_Val1" value="_Val2">_Val2</loop></loop>)";
    EQ_VALUE(Template::Render(content, &value3), LR"(102030)", LR"(Render())");

    value3 = JSON::Parse(LR"({"group":[1,2,3,4]})");

    content = LR"(<loop set="group" value="_Val" repeat="1">_Val</loop>)";
    EQ_VALUE(Template::Render(content, &value3), LR"(1)", LR"(Render())");

    content = LR"(<loop set="group" value="_Val" index="3">_Val</loop>)";
    EQ_VALUE(Template::Render(content, &value3), LR"(4)", LR"(Render())");

    content =
        LR"(<loop index="2" repeat="1" set="group" value="_Val">_Val</loop>)";
    EQ_VALUE(Template::Render(content, &value3), LR"(3)", LR"(Render())");

    value3  = JSON::Parse(LR"({"numbers":[1,2,3,4,5,6,7,8]})");
    content = LR"(A<loop set="numbers" value="t">t</loop>B)";
    EQ_VALUE(Template::Render(content, &value3), LR"(A12345678B)",
             LR"(Render())");

    content = LR"(<loop set="numbers" value="t">t[0]</loop>)";
    EQ_VALUE(Template::Render(content, &value3), LR"()", LR"(Render())");

    END_SUB_TEST;
}

static int TestLoopLTag3() {
    Value<wchar_t> value;
    const wchar_t *content;

    content = LR"(<loop></loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<loop>abcd</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<l</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(<l</loop>)",
             LR"(Render())");

    content = LR"(<lo</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(<lo</loop>)",
             LR"(Render())");

    content = LR"(<loo</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(<loo</loop>)",
             LR"(Render())");

    content = LR"(<loop></loop><loop repeat="2">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(AA)", LR"(Render())");

    content = LR"(<loop key="a">a</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<loop value="a">a</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<loop set="ss" value="a">a</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<loop set="" value="a">a</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<loop repeat="2" A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<loop repeat="OOO">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<loop repeat="{var:10}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    value[LR"(in)"] = Array<Value<wchar_t>>();

    content = LR"(<loop repeat="{var:in}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<loop repeat="10" index="{var:in}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<loop repeat="10" index="{var:in">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<loop repeat="10" index="{var:100}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<loop repeat="10" index="O">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<loop value="v">v</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(in)", LR"(Render())");

    value.Reset();
    value[LR"(in)"] = 2;

    content = LR"(<loop repeat="{var:in}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(AA)", LR"(Render())");

    /////
    value.Reset();
    value[LR"(k1)"] = 10;
    value[LR"(k2)"] = 20;
    value[LR"(k3)"] = 30;

    value[LR"(k2)"].Reset();

    content = LR"(<loop value="v">v</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(1030)", LR"(Render())");

    value.RemoveIndex(1);

    content = LR"(<loop value="v">v)";
    EQ_VALUE(Template::Render(content, &value), LR"(<loop value="v">v)",
             LR"(Render())");

    content = LR"(<loop value="v">v     )";
    EQ_VALUE(Template::Render(content, &value), LR"(<loop value="v">v     )",
             LR"(Render())");

    value.Reset();
    value += 10;
    value += 20;
    value += 30;

    value.RemoveIndex(1);

    content = LR"(<loop value="v">v</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(1030)", LR"(Render())");

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
    EQ_VALUE(Template::Render(content, &value), LR"(value1value2value3 value4)",
             LR"(Render())");

    content =
        LR"(<loop set="array" value="item"> item[0] item[1] item[2] item[3] </loop>)";
    EQ_VALUE(Template::Render(content, &value),
             LR"( value10 value20 value30 value40 )", LR"(Render())");

    content =
        LR"(<loop set="object" value="item">item[var11]item[var22]item[var33] item[var44]</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"( )", LR"(Render())");

    content =
        LR"(<loop set="array" value="item">item[var11]item[var22]item[var33] item[var44]</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"( )", LR"(Render())");

    content = LR"(<loop repeat="1"><l</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(<l)", LR"(Render())");

    value = JSON::Parse(LR"(
{
    "name": "some_val",
    "2020": [
        {
            "month": 5
        },
        {
            "month": 6
        },
        {
            "month": 7
        }
    ]
}
    )");

    content = LR"(<loop set="2020">{var:name}</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(some_valsome_valsome_val)",
             LR"(Render())");

    END_SUB_TEST;
}

static int TestLoopLTag4() {
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
        content += LR"({var:)";
        str = Digit<wchar_t>::NumberToString(i);
        content += str;
        content += LR"(})";

        output += str;
    }
    content += LR"(</loop>)";

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        LR"(Render())");

    //////////////////////

    content.Clear();
    output.Clear();

    content += LR"(<loop value="loop1-value">A loop1-value B</loop>)";
    for (unsigned int i = 0; i < size_4; i++) {
        output += LR"(A )";
        Digit<wchar_t>::NumberToStringStream(output, i);
        output += LR"( B)";
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        LR"(Render())");

    END_SUB_TEST;
}

static int TestLoopLTag5() {
    Value<wchar_t> value;
    const wchar_t *content;

    value = JSON::Parse(LR"(
[
    {
        "year": 2019,
        "quarter": "q1",
        "week": 1,
        "total": 100
    },
    {
        "year": 2019,
        "quarter": "q1",
        "week": 1,
        "total": 125
    },
    {
        "year": 2019,
        "quarter": "q2",
        "week": 1,
        "total": 200
    },
    {
        "year": 2017,
        "quarter": "q2",
        "week": 2,
        "total": 300
    },
    {
        "year": 2020,
        "quarter": "q1",
        "week": 1,
        "total": 400
    },
    {
        "year": 2020,
        "quarter": "q1",
        "week": 1,
        "total": 450
    },
    {
        "year": 2020,
        "quarter": "q1",
        "week": 1,
        "total": 450
    },
    {
        "year": 2018,
        "quarter": "q2",
        "week": 1,
        "total": 200
    },
    {
        "year": 2018,
        "quarter": "q2",
        "week": 2,
        "total": 300
    },
    {
        "year": 2019,
        "quarter": "q2",
        "week": 2,
        "total": 300
    }
]
    )");

    content =
        LR"(<loop value="val1_" group="year111" sort="descend"><loop set="val1_" value="val2_"><loop set="val2_" value="val3_">val3_</loop></loop></loop>)";

    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content =
        LR"(<loop value="val1_" group="year" sort="descend"><loop set="val1_" value="val2_"><loop set="val2_" value="val3_">val3_</loop></loop></loop>)";

    EQ_VALUE(Template::Render(content, &value),
             LR"(q11400q11450q11450q11100q11125q21200q22300q21200q22300q22300)",
             LR"(Render())");

    content =
        LR"(<loop value="val1_" group="year" sort="descend"><loop set="val1_" value="val2_" group="quarter" sort="ascend"><loop set="val2_" value="val3_"><loop set="val3_" value="val4_">val4_</loop></loop></loop></loop>)";

    EQ_VALUE(Template::Render(content, &value),
             LR"(1400145014501100112512002300120023002300)", LR"(Render())");

    content =
        LR"(<loop value="val1_" group="year" sort="descend">-- val1_-<loop set="val1_" value="val2_" group="quarter" sort="ascend">val2_-<loop set="val2_" value="val3_" group="week" sort="ascend">val2_:<loop set="val3_" value="val4_"><loop set="val4_" value="val5_"> val5_</loop></loop></loop></loop></loop>)";

    EQ_VALUE(
        Template::Render(content, &value),
        LR"(-- 2020-q1-1: 400 450 450-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2018-q2-1: 2002: 300-- 2017-q2-2: 300)",
        LR"(Render())");

    content =
        LR"(<loop value="val1_" group="year">-- val1_-<loop set="val1_" value="val2_" group="quarter">val2_-<loop set="val2_" value="val3_" group="week">val2_:<loop set="val3_" value="val4_"><loop set="val4_" value="val5_"> val5_</loop></loop></loop></loop></loop>)";

    EQ_VALUE(
        Template::Render(content, &value),
        LR"(-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2017-q2-2: 300-- 2020-q1-1: 400 450 450-- 2018-q2-1: 2002: 300)",
        LR"(Render())");

    ////////////

    value.Reset();

    value += 4;
    value += 1;
    value += 3;
    value += 5;
    value += 2;
    value += 7;
    value += 6;

    content = LR"(<loop value="val1_" sort="a">val1_</loop>)";

    EQ_VALUE(Template::Render(content, &value), LR"(1234567)", LR"(Render())");

    content = LR"(<loop value="val1_" sort="d">val1_</loop>)";

    EQ_VALUE(Template::Render(content, &value), LR"(7654321)", LR"(Render())");

    END_SUB_TEST;
}

static int TestIfLTag1() {
    Value<wchar_t> value;
    const wchar_t *content;

    value[LR"(name)"] = LR"(Qen)";
    value[LR"(t)"]    = true;
    value[LR"(f)"]    = false;
    value[LR"(n)"]    = nullptr;
    value[LR"(1)"]    = 1;
    value[LR"(one)"]  = LR"(1)";
    value[LR"(zero)"] = 0;

    content = LR"(<if case="1>0">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(Qen)", LR"(Render())");

    content = LR"(#<if case="{var:one}">{var:name}</if>#)";
    EQ_VALUE(Template::Render(content, &value), LR"(#Qen#)", LR"(Render())");

    content = LR"(##<if case="{var:zero}">{var:name}</if>##)";
    EQ_VALUE(Template::Render(content, &value), LR"(####)", LR"(Render())");

    content = LR"(<if case="{var:1}">{var:name}1<else />{var:name}2</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(Qen1)", LR"(Render())");

    content = LR"(<if case="{var:f}">{var:name}1<else />{var:name}2</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(Qen2)", LR"(Render())");

    content = LR"(#<if case="{var:t}">G<if case="1">oo</if>d</if>!#)";
    EQ_VALUE(Template::Render(content, &value), LR"(#Good!#)", LR"(Render())");

    content =
        LR"(###<if case="1">G<if case="1">o</if>o<if case="1">d!</if></if>###)";
    EQ_VALUE(Template::Render(content, &value), LR"(###Good!###)",
             LR"(Render())");

    content =
        LR"(Be <if case="1">G<if case="1">oo<if case="1">d</if></if></if>!)";
    EQ_VALUE(Template::Render(content, &value), LR"(Be Good!)", LR"(Render())");

    content = LR"(<if case="1">Good!<elseif case="0" />Bad!</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(Good!)", LR"(Render())");

    content = LR"(<if case="{var:n}">Bad!<elseif case="1" />Good!</if>#)";
    EQ_VALUE(Template::Render(content, &value), LR"(Good!#)", LR"(Render())");

    content = LR"(<if case="{var:f}">Bad!<elseif case="0" />Very Bad!</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content =
        LR"(#<if case="0">Bad!<elseif case="0" />Very Bad!<else />Very Good!</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(#Very Good!)",
             LR"(Render())");

    content = LR"(<if case="1">a<else />b</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(a)", LR"(Render())");

    content = LR"(<if case="0">a<else />b</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(b)", LR"(Render())");

    content = LR"(<if case="0">a<else /><if case="1">b</if></if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(b)", LR"(Render())");

    content = LR"(<if case="1">a<else /><if case="1">b</if></if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(a)", LR"(Render())");

    content = LR"(<if case="0">a<if case="1">b</if>c</if>Empty)";
    EQ_VALUE(Template::Render(content, &value), LR"(Empty)", LR"(Render())");

    content = LR"(<if case="1">a<else /><if case="1">b</if>c</if>===========)";
    EQ_VALUE(Template::Render(content, &value), LR"(a===========)",
             LR"(Render())");

    content = LR"(===========<if case="1">a<if case="1">b</if><else />c</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(===========ab)",
             LR"(Render())");

    content =
        LR"(<if case="1">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(a)", LR"(Render())");

    content =
        LR"(<if case="0">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(b)", LR"(Render())");

    content =
        LR"(===========<if case="0">a<elseif case="0" />b<elseif case="1" />c<else />d</if>===========)";
    EQ_VALUE(Template::Render(content, &value), LR"(===========c===========)",
             LR"(Render())");

    content =
        LR"(<if case="0">a<elseif case="0" />b<elseif case="0" />c<else />d</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(d)", LR"(Render())");

    content = LR"(<if case="1">a<if case="0">b<elseif case="0"/>c</if></if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(a)", LR"(Render())");

    content = LR"(<if case="1">a<if case="1">b<elseif case="0"/>c</if></if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(ab)", LR"(Render())");

    content =
        LR"(<if case="1"><if case="1">b<elseif case="c"/>c</if>a<else />c</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(ba)", LR"(Render())");

    content =
        LR"(<if case="0">a<else />c<if case="1">b<elseif case="0"/>c</if></if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(cb)", LR"(Render())");

    content =
        LR"(<if_case="1"><if case="0">Bad1!<elseif case="0" />Bad2!</if>a</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(a)", LR"(Render())");

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
        LR"(a)", LR"(Render())");

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
        LR"(b)", LR"(Render())");

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
        LR"(c)", LR"(Render())");

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
        LR"(d)", LR"(Render())");

    END_SUB_TEST;
}

static int TestIfLTag2() {
    Value<wchar_t> value;
    const wchar_t *content;

    value[LR"(name)"] = LR"(Qentem)";

    content = LR"(<if case="1">{var:name})";
    EQ_VALUE(Template::Render(content, &value), LR"(<if case="1">Qentem)",
             LR"(Render())");

    content = LR"(<if<if case="1">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(<ifQentem)",
             LR"(Render())");

    content = LR"(<if case="1"><if case="1">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(<if case="1">Qentem)",
             LR"(Render())");

    content = LR"(<if case="1"><if case="1"><if case="1">{var:name}</if></if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(<if case="1">Qentem)",
             LR"(Render())");

    content = LR"(<if case="ABC">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<if>{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<if case="0"><elseif />{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<iw case="0">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"(<iw case="0">Qentem</if>)",
             LR"(Render())");

    content = LR"(<if case="0"{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<if case="0"><else {var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    END_SUB_TEST;
}

static int TestRenderL1() {
    constexpr unsigned int size_4 = (8 * 4);

    StringStream<wchar_t> content;
    StringStream<wchar_t> output;
    String<wchar_t>       str;
    Value<wchar_t>        value;

    for (unsigned int i = 0; i < size_4; i++) {
        value += i;
    }

    for (unsigned int i = 0; i < size_4; i++) {
        content += LR"({var:)";
        str = Digit<wchar_t>::NumberToString(i);
        content += str;
        output += str;
        content += LR"(})";
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        LR"(Render())");

    content.Clear();
    output.Clear();

    unsigned int size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += LR"({var:)";
            str = Digit<wchar_t>::NumberToString(i);
            content += str;
            content += LR"(})";

            output += str;
        } else {
            size += 8;

            content += LR"({math: 1 + )";
            str = Digit<wchar_t>::NumberToString(i);
            content += str;
            content += LR"(})";

            Digit<wchar_t>::NumberToStringStream(output, 1U + i);
        }
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        LR"(Render())");

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += LR"({var:)";
            str = Digit<wchar_t>::NumberToString(i);
            content += str;
            content += LR"(})";

            output += str;
        } else {
            size += 8;

            content += LR"({if case="{var:)";
            Digit<wchar_t>::NumberToStringStream(content, i);
            content += LR"(}<)";
            Digit<wchar_t>::NumberToStringStream(content, 1U + i);
            content += LR"(" true="yes"})";

            output += LR"(yes)";
        }
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        LR"(Render())");

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += LR"({var:)";
            str = Digit<wchar_t>::NumberToString(i);
            content += str;
            content += LR"(})";

            output += str;
        } else {
            size += 8;
            content += LR"(<loop repeat="1">A</loop>)";
            output += LR"(A)";
        }
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        LR"(Render())");

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += LR"({var:)";
            str = Digit<wchar_t>::NumberToString(i);
            content += str;
            content += LR"(})";

            output += str;
        } else {
            size += 8;
            content += LR"(<if case="1">A</if>)";
            output += LR"(A)";
        }
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        LR"(Render())");

    content.Clear();
    output.Clear();

    constexpr unsigned int size_2_1 = (8 * 2) - 1;

    for (unsigned int i = 0; i < size_2_1; i++) {
        value += i;
    }

    content += LR"(<loop repeat="1">)";
    for (unsigned int i = 0; i < size_2_1; i++) {
        content += LR"({var:)";
        str = Digit<wchar_t>::NumberToString(i);
        content += str;
        output += str;
        content += LR"(})";
    }
    content += LR"(</loop>)";

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        LR"(Render())");

    END_SUB_TEST;
}

static int TestRenderL2() {
    Value<wchar_t> value;
    const wchar_t *content;

    value += 0;
    value += 1;
    value += 2;
    value += 5;
    value += 10;

    content = LR"(<loop set="numbers" value="val_">val_</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content =
        LR"(<loop value="this_number"><if case="(this_number % 2) == 1">this_number</if></loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(15)", LR"(Render())");

    content =
        LR"(<loop value="loop1_val">{if case="loop1_val < 5", true="loop1_val"}</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(012)", LR"(Render())");

    content =
        LR"(<loop value="loop1_val">{if case="loop1_val < 5", true="{var:4}"}</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(101010)", LR"(Render())");

    content = LR"(<loop value="loop1_val">loop1_val[]</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"()", LR"(Render())");

    content = LR"(<loop value="loop1_val">loop1_val[0 </loop>)";
    EQ_VALUE(
        Template::Render(content, &value),
        LR"({var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 )",
        LR"(Render())");

    value = JSON::Parse(LR"([[[1,2,3]]])");

    content = LR"(<loop value="loop1_val">loop1_val[0][2]</loop>)";
    EQ_VALUE(Template::Render(content, &value), LR"(3)", LR"(Render())");

    END_SUB_TEST;
}

static int RunTemplateLTests() {
    STARTING_TEST("Template.hpp (32-bit character)");

    START_TEST("Variable Tag Test 1", TestVariableLTag1);
    START_TEST("Variable Tag Test 2", TestVariableLTag2);
    START_TEST("Variable Tag Test 3", TestVariableLTag3);
    START_TEST("Variable Tag Test 4", TestVariableLTag4);

    START_TEST("Raw Variable Tag Test 1", TestRawVariableLTag1);
    START_TEST("Raw Variable Tag Test 2", TestRawVariableLTag2);
    START_TEST("Raw Variable Tag Test 3", TestRawVariableLTag3);
    START_TEST("Raw Variable Tag Test 4", TestRawVariableLTag4);

    START_TEST("Math Tag Test 1", TestMathLTag1);
    START_TEST("Math Tag Test 2", TestMathLTag2);

    START_TEST("Inline if Tag Test", TestInlineIfLTag);

    START_TEST("Loop Tag Test 1", TestLoopLTag1);
    START_TEST("Loop Tag Test 2", TestLoopLTag2);
    START_TEST("Loop Tag Test 3", TestLoopLTag3);
    START_TEST("Loop Tag Test 4", TestLoopLTag4);
    START_TEST("Loop Tag Test 5", TestLoopLTag5);

    START_TEST("If Tag Test 1", TestIfLTag1);
    START_TEST("If Tag Test 2", TestIfLTag2);

    START_TEST("Render Test 1", TestRenderL1);
    START_TEST("Render Test 2", TestRenderL2);

    END_TEST("Template.hpp (32-bit character)");
}

} // namespace Test
} // namespace Qentem

#endif
