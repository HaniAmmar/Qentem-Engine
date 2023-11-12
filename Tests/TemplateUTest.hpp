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

#include "TestHelper.hpp"
#include "JSON.hpp"
#include "Template.hpp"

#ifndef QENTEM_TEMPLATE_U_TESTS_H_
#define QENTEM_TEMPLATE_U_TESTS_H_

namespace Qentem::Test {

static void TestVariableUTag1(TestHelper &helper) {
    const char16_t *content;

    Value<char16_t> value = JSON::Parse(uR"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    const Value<char16_t> sub_value =
        JSON::Parse(uR"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = uR"({var:0})";
    helper.Equal(Template::Render(content, value), uR"(A)", uR"(Render())", __LINE__);

    content = uR"({var:1})";
    helper.Equal(Template::Render(content, value), uR"(abc)", uR"(Render())", __LINE__);

    content = uR"({var:2})";
    helper.Equal(Template::Render(content, value), uR"(true)", uR"(Render())", __LINE__);

    content = uR"({var:3})";
    helper.Equal(Template::Render(content, value), uR"(456)", uR"(Render())", __LINE__);

    content = uR"({var:4})";
    helper.Equal(Template::Render(content, value), uR"(1.5)", uR"(Render())", __LINE__);

    content = uR"({var:5[0]})";
    helper.Equal(Template::Render(content, value), uR"(null)", uR"(Render())", __LINE__);

    content = uR"({var:5[1]})";
    helper.Equal(Template::Render(content, value), uR"(false)", uR"(Render())", __LINE__);

    content = uR"({var:5[2][0]})";
    helper.Equal(Template::Render(content, value), uR"(Qentem)", uR"(Render())", __LINE__);

    //////

    content = uR"({var:key1})";
    helper.Equal(Template::Render(content, sub_value), uR"(a)", uR"(Render())", __LINE__);

    content = uR"({var:key2})";
    helper.Equal(Template::Render(content, sub_value), uR"(ABC)", uR"(Render())", __LINE__);

    content = uR"({var:key3})";
    helper.Equal(Template::Render(content, sub_value), uR"(false)", uR"(Render())", __LINE__);

    content = uR"({var:key4})";
    helper.Equal(Template::Render(content, sub_value), uR"(100)", uR"(Render())", __LINE__);

    content = uR"({var:key5})";
    helper.Equal(Template::Render(content, sub_value), uR"(1.5)", uR"(Render())", __LINE__);

    content = uR"({var:key6[one]})";
    helper.Equal(Template::Render(content, sub_value), uR"(1)", uR"(Render())", __LINE__);

    content = uR"({var:key7[0]})";
    helper.Equal(Template::Render(content, sub_value), uR"(null)", uR"(Render())", __LINE__);

    content = uR"({var:key7[1]})";
    helper.Equal(Template::Render(content, sub_value), uR"(false)", uR"(Render())", __LINE__);

    content = uR"({var:key7[2][0]})";
    helper.Equal(Template::Render(content, sub_value), uR"(Qentem)", uR"(Render())", __LINE__);

    //
    content = uR"({var:6[key1]})";
    helper.Equal(Template::Render(content, value), uR"(a)", uR"(Render())", __LINE__);

    content = uR"({var:6[key2]})";
    helper.Equal(Template::Render(content, value), uR"(ABC)", uR"(Render())", __LINE__);

    content = uR"({var:6[key3]})";
    helper.Equal(Template::Render(content, value), uR"(false)", uR"(Render())", __LINE__);

    content = uR"({var:6[key4]})";
    helper.Equal(Template::Render(content, value), uR"(100)", uR"(Render())", __LINE__);

    content = uR"({var:6[key5]})";
    helper.Equal(Template::Render(content, value), uR"(1.5)", uR"(Render())", __LINE__);

    content = uR"({var:6[key6][one]})";
    helper.Equal(Template::Render(content, value), uR"(1)", uR"(Render())", __LINE__);

    ////////////////

    content = uR"(-{var:2})";
    helper.Equal(Template::Render(content, value), uR"(-true)", uR"(Render())", __LINE__);

    content = uR"(-{var:key7[0]})";
    helper.Equal(Template::Render(content, sub_value), uR"(-null)", uR"(Render())", __LINE__);

    content = uR"(-{var:key7[2][0]})";
    helper.Equal(Template::Render(content, sub_value), uR"(-Qentem)", uR"(Render())", __LINE__);

    content = uR"(-{var:6[key3]})";
    helper.Equal(Template::Render(content, value), uR"(-false)", uR"(Render())", __LINE__);

    content = uR"(-{var:6[key4]})";
    helper.Equal(Template::Render(content, value), uR"(-100)", uR"(Render())", __LINE__);
    ////////////

    content = uR"({var:2}-)";
    helper.Equal(Template::Render(content, value), uR"(true-)", uR"(Render())", __LINE__);

    content = uR"({var:key7[0]}-)";
    helper.Equal(Template::Render(content, sub_value), uR"(null-)", uR"(Render())", __LINE__);

    content = uR"({var:key7[2][0]}-)";
    helper.Equal(Template::Render(content, sub_value), uR"(Qentem-)", uR"(Render())", __LINE__);

    content = uR"({var:6[key3]}-)";
    helper.Equal(Template::Render(content, value), uR"(false-)", uR"(Render())", __LINE__);

    content = uR"({var:6[key4]}-)";
    helper.Equal(Template::Render(content, value), uR"(100-)", uR"(Render())", __LINE__);

    ////////////

    content = uR"(-{var:2}-)";
    helper.Equal(Template::Render(content, value), uR"(-true-)", uR"(Render())", __LINE__);

    content = uR"(-{var:key7[0]}-)";
    helper.Equal(Template::Render(content, sub_value), uR"(-null-)", uR"(Render())", __LINE__);

    content = uR"(-{var:key7[2][0]}-)";
    helper.Equal(Template::Render(content, sub_value), uR"(-Qentem-)", uR"(Render())", __LINE__);

    content = uR"(-{var:6[key3]}-)";
    helper.Equal(Template::Render(content, value), uR"(-false-)", uR"(Render())", __LINE__);

    content = uR"(-{var:6[key4]}-)";
    helper.Equal(Template::Render(content, value), uR"(-100-)", uR"(Render())", __LINE__);

    ////////////

    content = uR"(------{var:2})";
    helper.Equal(Template::Render(content, value), uR"(------true)", uR"(Render())", __LINE__);

    content = uR"(------{var:key7[0]})";
    helper.Equal(Template::Render(content, sub_value), uR"(------null)", uR"(Render())", __LINE__);

    content = uR"(------{var:key7[2][0]})";
    helper.Equal(Template::Render(content, sub_value), uR"(------Qentem)", uR"(Render())", __LINE__);

    content = uR"(------{var:6[key3]})";
    helper.Equal(Template::Render(content, value), uR"(------false)", uR"(Render())", __LINE__);

    content = uR"(------{var:6[key4]})";
    helper.Equal(Template::Render(content, value), uR"(------100)", uR"(Render())", __LINE__);

    ////////////

    content = uR"({var:2}------)";
    helper.Equal(Template::Render(content, value), uR"(true------)", uR"(Render())", __LINE__);

    content = uR"({var:key7[0]}------)";
    helper.Equal(Template::Render(content, sub_value), uR"(null------)", uR"(Render())", __LINE__);

    content = uR"({var:key7[2][0]}------)";
    helper.Equal(Template::Render(content, sub_value), uR"(Qentem------)", uR"(Render())", __LINE__);

    content = uR"({var:6[key3]}------)";
    helper.Equal(Template::Render(content, value), uR"(false------)", uR"(Render())", __LINE__);

    content = uR"({var:6[key4]}------)";
    helper.Equal(Template::Render(content, value), uR"(100------)", uR"(Render())", __LINE__);

    ////////////

    content = uR"(------{var:2}------)";
    helper.Equal(Template::Render(content, value), uR"(------true------)", uR"(Render())", __LINE__);

    content = uR"(------{var:key7[0]}------)";
    helper.Equal(Template::Render(content, sub_value), uR"(------null------)", uR"(Render())", __LINE__);

    content = uR"(------{var:key7[2][0]}------)";
    helper.Equal(Template::Render(content, sub_value), uR"(------Qentem------)", uR"(Render())", __LINE__);

    content = uR"(------{var:6[key3]}------)";
    helper.Equal(Template::Render(content, value), uR"(------false------)", uR"(Render())", __LINE__);

    content = uR"(------{var:6[key4]}------)";
    helper.Equal(Template::Render(content, value), uR"(------100------)", uR"(Render())", __LINE__);

    content = uR"({var:key7[2[0]})";
    helper.Equal(Template::Render(content, value), uR"({var:key7[2[0]})", uR"(Render())", __LINE__);

    content = uR"({var:6key3]})";
    helper.Equal(Template::Render(content, value), uR"({var:6key3]})", uR"(Render())", __LINE__);
}

static void TestVariableUTag2(TestHelper &helper) {
    const Value<char16_t> value = JSON::Parse(uR"(["A", "abc", true, 456, 1.5])");
    const char16_t       *content;

    content = uR"({var:0}{var:0})";
    helper.Equal(Template::Render(content, value), uR"(AA)", uR"(Render())", __LINE__);

    content = uR"({var:1}{var:0})";
    helper.Equal(Template::Render(content, value), uR"(abcA)", uR"(Render())", __LINE__);

    content = uR"({var:1}{var:2})";
    helper.Equal(Template::Render(content, value), uR"(abctrue)", uR"(Render())", __LINE__);

    content = uR"({var:2}{var:3}{var:2})";
    helper.Equal(Template::Render(content, value), uR"(true456true)", uR"(Render())", __LINE__);

    content = uR"({var:4}{var:4}{var:4})";
    helper.Equal(Template::Render(content, value), uR"(1.51.51.5)", uR"(Render())", __LINE__);
    ///

    content = uR"({var:0}-{var:0})";
    helper.Equal(Template::Render(content, value), uR"(A-A)", uR"(Render())", __LINE__);

    content = uR"({var:1}--{var:0})";
    helper.Equal(Template::Render(content, value), uR"(abc--A)", uR"(Render())", __LINE__);

    content = uR"({var:1}---{var:2})";
    helper.Equal(Template::Render(content, value), uR"(abc---true)", uR"(Render())", __LINE__);

    content = uR"({var:2}{var:3}--{var:2})";
    helper.Equal(Template::Render(content, value), uR"(true456--true)", uR"(Render())", __LINE__);

    content = uR"({var:4}--{var:4}{var:4})";
    helper.Equal(Template::Render(content, value), uR"(1.5--1.51.5)", uR"(Render())", __LINE__);

    content = uR"({var:4}--{var:4}--{var:4})";
    helper.Equal(Template::Render(content, value), uR"(1.5--1.5--1.5)", uR"(Render())", __LINE__);

    content = uR"({var:4}---{var:4}---{var:4})";
    helper.Equal(Template::Render(content, value), uR"(1.5---1.5---1.5)", uR"(Render())", __LINE__);
}

static void TestVariableUTag3(TestHelper &helper) {
    Value<char16_t> value;
    const char16_t *content;

    content = uR"({var:0})";
    helper.Equal(Template::Render(content, value), uR"({var:0})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({var:a})", value), uR"({var:a})", uR"(Render())", __LINE__);

    content = uR"({var:0[0]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value), uR"({var:0[0]})", uR"(Render())",
                 __LINE__);

    content = uR"({var:a[0]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value), uR"({var:a[0]})", uR"(Render())",
                 __LINE__);

    content = uR"({var:0[a]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value), uR"({var:0[a]})", uR"(Render())",
                 __LINE__);

    content = uR"({var:a[abc]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value), uR"({var:a[abc]})", uR"(Render())",
                 __LINE__);

    ////////////////

    value = JSON::Parse(uR"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = uR"({var:0})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value), uR"({var:0})", uR"(Render())",
                 __LINE__);

    content = uR"({var:0[0]})";
    helper.Equal(Template::Render(content, value), uR"({var:0[0]})", uR"(Render())", __LINE__);

    content = uR"({var:0[0][0]})";
    helper.Equal(Template::Render(content, value), uR"({var:0[0][0]})", uR"(Render())", __LINE__);

    /////

    content = uR"({var:1})";
    helper.Equal(Template::Render(content, value), uR"({var:1})", uR"(Render())", __LINE__);

    content = uR"({var:2})";
    helper.Equal(Template::Render(content, value), uR"({var:2})", uR"(Render())", __LINE__);

    content = uR"({var:1[a]})";
    helper.Equal(Template::Render(content, value), uR"({var:1[a]})", uR"(Render())", __LINE__);

    content = uR"({var:1[b]})";
    helper.Equal(Template::Render(content, value), uR"({var:1[b]})", uR"(Render())", __LINE__);

    ////

    value = JSON::Parse(uR"(["A", "abc", true, 456, 1.5])");

    content = uR"({var:0)";
    helper.Equal(Template::Render(content, value), uR"({var:0)", uR"(Render())", __LINE__);

    content = uR"(var:0})";
    helper.Equal(Template::Render(content, value), uR"(var:0})", uR"(Render())", __LINE__);

    content = uR"({v})";
    helper.Equal(Template::Render(content, value), uR"({v})", uR"(Render())", __LINE__);

    content = uR"({va})";
    helper.Equal(Template::Render(content, value), uR"({va})", uR"(Render())", __LINE__);

    content = uR"({var})";
    helper.Equal(Template::Render(content, value), uR"({var})", uR"(Render())", __LINE__);

    content = uR"({var:})";
    helper.Equal(Template::Render(content, value), uR"({var:})", uR"(Render())", __LINE__);

    content = uR"({v:0})";
    helper.Equal(Template::Render(content, value), uR"({v:0})", uR"(Render())", __LINE__);

    content = uR"({va:0})";
    helper.Equal(Template::Render(content, value), uR"({va:0})", uR"(Render())", __LINE__);
    ////

    content = uR"({var:0{var:0})";
    helper.Equal(Template::Render(content, value), uR"({var:0{var:0})", uR"(Render())", __LINE__);

    content = uR"(var:0{var:0})";
    helper.Equal(Template::Render(content, value), uR"(var:0A)", uR"(Render())", __LINE__);

    content = uR"(var:0}{var:0})";
    helper.Equal(Template::Render(content, value), uR"(var:0}A)", uR"(Render())", __LINE__);

    content = uR"({var:0{var:0}{var:0})";
    helper.Equal(Template::Render(content, value), uR"({var:0{var:0}A)", uR"(Render())", __LINE__);

    ////

    content = uR"({var:0{var:0})";
    helper.Equal(Template::Render(content, value), uR"({var:0{var:0})", uR"(Render())", __LINE__);

    content = uR"(var:0{var:0})";
    helper.Equal(Template::Render(content, value), uR"(var:0A)", uR"(Render())", __LINE__);

    content = uR"(var:0}{var:0})";
    helper.Equal(Template::Render(content, value), uR"(var:0}A)", uR"(Render())", __LINE__);

    content = uR"({var:0{var:0}{var:0})";
    helper.Equal(Template::Render(content, value), uR"({var:0{var:0}A)", uR"(Render())", __LINE__);

    ////

    content = uR"({{var:0})";
    helper.Equal(Template::Render(content, value), uR"({A)", uR"(Render())", __LINE__);

    content = uR"({{var:0}{var:0})";
    helper.Equal(Template::Render(content, value), uR"({AA)", uR"(Render())", __LINE__);

    content = uR"({v{var:0})";
    helper.Equal(Template::Render(content, value), uR"({vA)", uR"(Render())", __LINE__);

    content = uR"({v{var:0}{var:0})";
    helper.Equal(Template::Render(content, value), uR"({vAA)", uR"(Render())", __LINE__);

    content = uR"({va{var:0})";
    helper.Equal(Template::Render(content, value), uR"({vaA)", uR"(Render())", __LINE__);

    content = uR"({va{var:0}{var:0})";
    helper.Equal(Template::Render(content, value), uR"({vaAA)", uR"(Render())", __LINE__);

    content = uR"({var{var:0})";
    helper.Equal(Template::Render(content, value), uR"({varA)", uR"(Render())", __LINE__);

    content = uR"({var{var:0}{var:0})";
    helper.Equal(Template::Render(content, value), uR"({varAA)", uR"(Render())", __LINE__);

    ///

    content = uR"({-{var:0})";
    helper.Equal(Template::Render(content, value), uR"({-A)", uR"(Render())", __LINE__);

    content = uR"({-{var:0}{var:0})";
    helper.Equal(Template::Render(content, value), uR"({-AA)", uR"(Render())", __LINE__);

    content = uR"({v-{var:0})";
    helper.Equal(Template::Render(content, value), uR"({v-A)", uR"(Render())", __LINE__);

    content = uR"({v-{var:0}{var:0})";
    helper.Equal(Template::Render(content, value), uR"({v-AA)", uR"(Render())", __LINE__);

    content = uR"({va-{var:0})";
    helper.Equal(Template::Render(content, value), uR"({va-A)", uR"(Render())", __LINE__);

    content = uR"({va-{var:0}{var:0})";
    helper.Equal(Template::Render(content, value), uR"({va-AA)", uR"(Render())", __LINE__);

    content = uR"({var-{var:0})";
    helper.Equal(Template::Render(content, value), uR"({var-A)", uR"(Render())", __LINE__);

    content = uR"({var-{var:0}{var:0})";
    helper.Equal(Template::Render(content, value), uR"({var-AA)", uR"(Render())", __LINE__);

    //

    content = uR"({var-0})";
    helper.Equal(Template::Render(content, value), uR"({var-0})", uR"(Render())", __LINE__);

    content = uR"({var 0})";
    helper.Equal(Template::Render(content, value), uR"({var 0})", uR"(Render())", __LINE__);

    content = uR"({var:0 })";
    helper.Equal(Template::Render(content, value), uR"({var:0 })", uR"(Render())", __LINE__);

    content = uR"({var:0 )";
    helper.Equal(Template::Render(content, value), uR"({var:0 )", uR"(Render())", __LINE__);

    content = uR"({var:0)";
    helper.Equal(Template::Render(content, value), uR"({var:0)", uR"(Render())", __LINE__);

    content = uR"( {var-0})";
    helper.Equal(Template::Render(content, value), uR"( {var-0})", uR"(Render())", __LINE__);

    content = uR"( {var 0})";
    helper.Equal(Template::Render(content, value), uR"( {var 0})", uR"(Render())", __LINE__);

    content = uR"( {var:0 })";
    helper.Equal(Template::Render(content, value), uR"( {var:0 })", uR"(Render())", __LINE__);

    content = uR"( {var:0 )";
    helper.Equal(Template::Render(content, value), uR"( {var:0 )", uR"(Render())", __LINE__);

    content = uR"( {var:0)";
    helper.Equal(Template::Render(content, value), uR"( {var:0)", uR"(Render())", __LINE__);
}

static void TestVariableUTag4(TestHelper &helper) {
    Value<char16_t> value;

    value += uR"(<)";
    value += uR"(>)";
    value += uR"(&)";
    value += uR"(")";
    value += uR"(')";
    value += uR"(<>)";
    value += uR"(<&)";
    value += uR"(<&)";
    value += uR"(>")";
    value += uR"("')";
    value += uR"(<">)";
    value += uR"(<'>)";
    value += uR"(<&>)";
    value += uR"(&"&)";
    value += uR"("'")";
    value += uR"('<')";
    value += uR"('&')";
    value += uR"(<>&'")";
    value += uR"('"<>&)";
    value += uR"(<"&'>)";

    value += uR"(<<<<<)";
    value += uR"(>>>>>)";
    value += uR"(&&&&&)";
    value += uR"(""""")";
    value += uR"(''''')";

    value += uR"(A<)";
    value += uR"(A>)";
    value += uR"(A&)";
    value += uR"(A")";
    value += uR"(A')";
    value += uR"(<A)";
    value += uR"(>A)";
    value += uR"(&A)";
    value += uR"("A)";
    value += uR"('A)";
    value += uR"(A<A)";
    value += uR"(A>A)";
    value += uR"(A&A)";
    value += uR"(A"A)";
    value += uR"(A'A)";

    value += uR"(AA<AA)";
    value += uR"(AA>AA)";
    value += uR"(AA&AA)";
    value += uR"(AA"AA)";
    value += uR"(AA'AA)";

    value += uR"(AA<<<<AA)";
    value += uR"(AA>>>>AA)";
    value += uR"(AA&&&&AA)";
    value += uR"(AA""""AA)";
    value += uR"(AA''''AA)";

    value += uR"(<A>B'C"D&E'F"G<H>I&G"K)";
    value += uR"(AB"CD'EF<GH>IGK')";
    value += uR"("ABC'DEF<GHI>GK<)";
    value += uR"(A""BC<<DE>>FG''HI&&GK)";

    if constexpr (Config::AutoEscapeHTML) {
        helper.Equal(Template::Render(uR"({var:0})", value), uR"(&lt;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:1})", value), uR"(&gt;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:2})", value), uR"(&amp;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:3})", value), uR"(&quot;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:4})", value), uR"(&apos;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:5})", value), uR"(&lt;&gt;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:6})", value), uR"(&lt;&amp;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:7})", value), uR"(&lt;&amp;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:8})", value), uR"(&gt;&quot;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:9})", value), uR"(&quot;&apos;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:10})", value), uR"(&lt;&quot;&gt;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:11})", value), uR"(&lt;&apos;&gt;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:12})", value), uR"(&lt;&amp;&gt;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:13})", value), uR"(&amp;&quot;&amp;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:14})", value), uR"(&quot;&apos;&quot;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:15})", value), uR"(&apos;&lt;&apos;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:16})", value), uR"(&apos;&amp;&apos;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:17})", value), uR"(&lt;&gt;&amp;&apos;&quot;)", uR"(Render())",
                     __LINE__);
        helper.Equal(Template::Render(uR"({var:18})", value), uR"(&apos;&quot;&lt;&gt;&amp;)", uR"(Render())",
                     __LINE__);
        helper.Equal(Template::Render(uR"({var:19})", value), uR"(&lt;&quot;&amp;&apos;&gt;)", uR"(Render())",
                     __LINE__);
        helper.Equal(Template::Render(uR"({var:20})", value), uR"(&lt;&lt;&lt;&lt;&lt;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:21})", value), uR"(&gt;&gt;&gt;&gt;&gt;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:22})", value), uR"(&amp;&amp;&amp;&amp;&amp;)", uR"(Render())",
                     __LINE__);
        helper.Equal(Template::Render(uR"({var:23})", value), uR"(&quot;&quot;&quot;&quot;&quot;)", uR"(Render())",
                     __LINE__);
        helper.Equal(Template::Render(uR"({var:24})", value), uR"(&apos;&apos;&apos;&apos;&apos;)", uR"(Render())",
                     __LINE__);
        helper.Equal(Template::Render(uR"({var:25})", value), uR"(A&lt;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:26})", value), uR"(A&gt;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:27})", value), uR"(A&amp;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:28})", value), uR"(A&quot;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:29})", value), uR"(A&apos;)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:30})", value), uR"(&lt;A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:31})", value), uR"(&gt;A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:32})", value), uR"(&amp;A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:33})", value), uR"(&quot;A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:34})", value), uR"(&apos;A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:35})", value), uR"(A&lt;A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:36})", value), uR"(A&gt;A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:37})", value), uR"(A&amp;A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:38})", value), uR"(A&quot;A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:39})", value), uR"(A&apos;A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:40})", value), uR"(AA&lt;AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:41})", value), uR"(AA&gt;AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:42})", value), uR"(AA&amp;AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:43})", value), uR"(AA&quot;AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:44})", value), uR"(AA&apos;AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:45})", value), uR"(AA&lt;&lt;&lt;&lt;AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:46})", value), uR"(AA&gt;&gt;&gt;&gt;AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:47})", value), uR"(AA&amp;&amp;&amp;&amp;AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:48})", value), uR"(AA&quot;&quot;&quot;&quot;AA)", uR"(Render())",
                     __LINE__);
        helper.Equal(Template::Render(uR"({var:49})", value), uR"(AA&apos;&apos;&apos;&apos;AA)", uR"(Render())",
                     __LINE__);
        helper.Equal(Template::Render(uR"({var:50})", value),
                     uR"(&lt;A&gt;B&apos;C&quot;D&amp;E&apos;F&quot;G&lt;H&gt;I&amp;G&quot;K)", uR"(Render())",
                     __LINE__);
        helper.Equal(Template::Render(uR"({var:51})", value), uR"(AB&quot;CD&apos;EF&lt;GH&gt;IGK&apos;)",
                     uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:52})", value), uR"(&quot;ABC&apos;DEF&lt;GHI&gt;GK&lt;)", uR"(Render())",
                     __LINE__);
        helper.Equal(Template::Render(uR"({var:53})", value),
                     uR"(A&quot;&quot;BC&lt;&lt;DE&gt;&gt;FG&apos;&apos;HI&amp;&amp;GK)", uR"(Render())", __LINE__);
    } else {
        helper.Equal(Template::Render(uR"({var:0})", value), uR"(<)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:1})", value), uR"(>)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:2})", value), uR"(&)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:3})", value), uR"(")", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:4})", value), uR"(')", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:5})", value), uR"(<>)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:6})", value), uR"(<&)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:7})", value), uR"(<&)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:8})", value), uR"(>")", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:9})", value), uR"("')", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:10})", value), uR"(<">)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:11})", value), uR"(<'>)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:12})", value), uR"(<&>)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:13})", value), uR"(&"&)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:14})", value), uR"("'")", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:15})", value), uR"('<')", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:16})", value), uR"('&')", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:17})", value), uR"(<>&'")", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:18})", value), uR"('"<>&)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:19})", value), uR"(<"&'>)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:20})", value), uR"(<<<<<)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:21})", value), uR"(>>>>>)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:22})", value), uR"(&&&&&)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:23})", value), uR"(""""")", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:24})", value), uR"(''''')", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:25})", value), uR"(A<)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:26})", value), uR"(A>)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:27})", value), uR"(A&)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:28})", value), uR"(A")", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:29})", value), uR"(A')", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:30})", value), uR"(<A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:31})", value), uR"(>A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:32})", value), uR"(&A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:33})", value), uR"("A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:34})", value), uR"('A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:35})", value), uR"(A<A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:36})", value), uR"(A>A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:37})", value), uR"(A&A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:38})", value), uR"(A"A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:39})", value), uR"(A'A)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:40})", value), uR"(AA<AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:41})", value), uR"(AA>AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:42})", value), uR"(AA&AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:43})", value), uR"(AA"AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:44})", value), uR"(AA'AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:45})", value), uR"(AA<<<<AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:46})", value), uR"(AA>>>>AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:47})", value), uR"(AA&&&&AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:48})", value), uR"(AA""""AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:49})", value), uR"(AA''''AA)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:50})", value), uR"(<A>B'C"D&E'F"G<H>I&G"K)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:51})", value), uR"(AB"CD'EF<GH>IGK')", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:52})", value), uR"("ABC'DEF<GHI>GK<)", uR"(Render())", __LINE__);
        helper.Equal(Template::Render(uR"({var:53})", value), uR"(A""BC<<DE>>FG''HI&&GK)", uR"(Render())", __LINE__);
    }
}

static void TestRawVariableUTag1(TestHelper &helper) {
    const char16_t *content;

    Value<char16_t> value = JSON::Parse(uR"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    const Value<char16_t> sub_value =
        JSON::Parse(uR"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = uR"({raw:0})";
    helper.Equal(Template::Render(content, value), uR"(A)", uR"(Render())", __LINE__);

    content = uR"({raw:1})";
    helper.Equal(Template::Render(content, value), uR"(abc)", uR"(Render())", __LINE__);

    content = uR"({raw:2})";
    helper.Equal(Template::Render(content, value), uR"(true)", uR"(Render())", __LINE__);

    content = uR"({raw:3})";
    helper.Equal(Template::Render(content, value), uR"(456)", uR"(Render())", __LINE__);

    content = uR"({raw:4})";
    helper.Equal(Template::Render(content, value), uR"(1.5)", uR"(Render())", __LINE__);

    content = uR"({raw:5[0]})";
    helper.Equal(Template::Render(content, value), uR"(null)", uR"(Render())", __LINE__);

    content = uR"({raw:5[1]})";
    helper.Equal(Template::Render(content, value), uR"(false)", uR"(Render())", __LINE__);

    content = uR"({raw:5[2][0]})";
    helper.Equal(Template::Render(content, value), uR"(Qentem)", uR"(Render())", __LINE__);

    //////

    content = uR"({raw:key1})";
    helper.Equal(Template::Render(content, sub_value), uR"(a)", uR"(Render())", __LINE__);

    content = uR"({raw:key2})";
    helper.Equal(Template::Render(content, sub_value), uR"(ABC)", uR"(Render())", __LINE__);

    content = uR"({raw:key3})";
    helper.Equal(Template::Render(content, sub_value), uR"(false)", uR"(Render())", __LINE__);

    content = uR"({raw:key4})";
    helper.Equal(Template::Render(content, sub_value), uR"(100)", uR"(Render())", __LINE__);

    content = uR"({raw:key5})";
    helper.Equal(Template::Render(content, sub_value), uR"(1.5)", uR"(Render())", __LINE__);

    content = uR"({raw:key6[one]})";
    helper.Equal(Template::Render(content, sub_value), uR"(1)", uR"(Render())", __LINE__);

    content = uR"({raw:key7[0]})";
    helper.Equal(Template::Render(content, sub_value), uR"(null)", uR"(Render())", __LINE__);

    content = uR"({raw:key7[1]})";
    helper.Equal(Template::Render(content, sub_value), uR"(false)", uR"(Render())", __LINE__);

    content = uR"({raw:key7[2][0]})";
    helper.Equal(Template::Render(content, sub_value), uR"(Qentem)", uR"(Render())", __LINE__);

    //
    content = uR"({raw:6[key1]})";
    helper.Equal(Template::Render(content, value), uR"(a)", uR"(Render())", __LINE__);

    content = uR"({raw:6[key2]})";
    helper.Equal(Template::Render(content, value), uR"(ABC)", uR"(Render())", __LINE__);

    content = uR"({raw:6[key3]})";
    helper.Equal(Template::Render(content, value), uR"(false)", uR"(Render())", __LINE__);

    content = uR"({raw:6[key4]})";
    helper.Equal(Template::Render(content, value), uR"(100)", uR"(Render())", __LINE__);

    content = uR"({raw:6[key5]})";
    helper.Equal(Template::Render(content, value), uR"(1.5)", uR"(Render())", __LINE__);

    content = uR"({raw:6[key6][one]})";
    helper.Equal(Template::Render(content, value), uR"(1)", uR"(Render())", __LINE__);

    ////////////////

    content = uR"(-{raw:2})";
    helper.Equal(Template::Render(content, value), uR"(-true)", uR"(Render())", __LINE__);

    content = uR"(-{raw:key7[0]})";
    helper.Equal(Template::Render(content, sub_value), uR"(-null)", uR"(Render())", __LINE__);

    content = uR"(-{raw:key7[2][0]})";
    helper.Equal(Template::Render(content, sub_value), uR"(-Qentem)", uR"(Render())", __LINE__);

    content = uR"(-{raw:6[key3]})";
    helper.Equal(Template::Render(content, value), uR"(-false)", uR"(Render())", __LINE__);

    content = uR"(-{raw:6[key4]})";
    helper.Equal(Template::Render(content, value), uR"(-100)", uR"(Render())", __LINE__);
    ////////////

    content = uR"({raw:2}-)";
    helper.Equal(Template::Render(content, value), uR"(true-)", uR"(Render())", __LINE__);

    content = uR"({raw:key7[0]}-)";
    helper.Equal(Template::Render(content, sub_value), uR"(null-)", uR"(Render())", __LINE__);

    content = uR"({raw:key7[2][0]}-)";
    helper.Equal(Template::Render(content, sub_value), uR"(Qentem-)", uR"(Render())", __LINE__);

    content = uR"({raw:6[key3]}-)";
    helper.Equal(Template::Render(content, value), uR"(false-)", uR"(Render())", __LINE__);

    content = uR"({raw:6[key4]}-)";
    helper.Equal(Template::Render(content, value), uR"(100-)", uR"(Render())", __LINE__);

    ////////////

    content = uR"(-{raw:2}-)";
    helper.Equal(Template::Render(content, value), uR"(-true-)", uR"(Render())", __LINE__);

    content = uR"(-{raw:key7[0]}-)";
    helper.Equal(Template::Render(content, sub_value), uR"(-null-)", uR"(Render())", __LINE__);

    content = uR"(-{raw:key7[2][0]}-)";
    helper.Equal(Template::Render(content, sub_value), uR"(-Qentem-)", uR"(Render())", __LINE__);

    content = uR"(-{raw:6[key3]}-)";
    helper.Equal(Template::Render(content, value), uR"(-false-)", uR"(Render())", __LINE__);

    content = uR"(-{raw:6[key4]}-)";
    helper.Equal(Template::Render(content, value), uR"(-100-)", uR"(Render())", __LINE__);

    ////////////

    content = uR"(------{raw:2})";
    helper.Equal(Template::Render(content, value), uR"(------true)", uR"(Render())", __LINE__);

    content = uR"(------{raw:key7[0]})";
    helper.Equal(Template::Render(content, sub_value), uR"(------null)", uR"(Render())", __LINE__);

    content = uR"(------{raw:key7[2][0]})";
    helper.Equal(Template::Render(content, sub_value), uR"(------Qentem)", uR"(Render())", __LINE__);

    content = uR"(------{raw:6[key3]})";
    helper.Equal(Template::Render(content, value), uR"(------false)", uR"(Render())", __LINE__);

    content = uR"(------{raw:6[key4]})";
    helper.Equal(Template::Render(content, value), uR"(------100)", uR"(Render())", __LINE__);

    ////////////

    content = uR"({raw:2}------)";
    helper.Equal(Template::Render(content, value), uR"(true------)", uR"(Render())", __LINE__);

    content = uR"({raw:key7[0]}------)";
    helper.Equal(Template::Render(content, sub_value), uR"(null------)", uR"(Render())", __LINE__);

    content = uR"({raw:key7[2][0]}------)";
    helper.Equal(Template::Render(content, sub_value), uR"(Qentem------)", uR"(Render())", __LINE__);

    content = uR"({raw:6[key3]}------)";
    helper.Equal(Template::Render(content, value), uR"(false------)", uR"(Render())", __LINE__);

    content = uR"({raw:6[key4]}------)";
    helper.Equal(Template::Render(content, value), uR"(100------)", uR"(Render())", __LINE__);

    ////////////

    content = uR"(------{raw:2}------)";
    helper.Equal(Template::Render(content, value), uR"(------true------)", uR"(Render())", __LINE__);

    content = uR"(------{raw:key7[0]}------)";
    helper.Equal(Template::Render(content, sub_value), uR"(------null------)", uR"(Render())", __LINE__);

    content = uR"(------{raw:key7[2][0]}------)";
    helper.Equal(Template::Render(content, sub_value), uR"(------Qentem------)", uR"(Render())", __LINE__);

    content = uR"(------{raw:6[key3]}------)";
    helper.Equal(Template::Render(content, value), uR"(------false------)", uR"(Render())", __LINE__);

    content = uR"(------{raw:6[key4]}------)";
    helper.Equal(Template::Render(content, value), uR"(------100------)", uR"(Render())", __LINE__);

    content = uR"({raw:key7[2[0]})";
    helper.Equal(Template::Render(content, value), uR"({raw:key7[2[0]})", uR"(Render())", __LINE__);

    content = uR"({raw:6key3]})";
    helper.Equal(Template::Render(content, value), uR"({raw:6key3]})", uR"(Render())", __LINE__);
}

static void TestRawVariableUTag2(TestHelper &helper) {
    const Value<char16_t> value = JSON::Parse(uR"(["A", "abc", true, 456, 1.5])");
    const char16_t       *content;

    content = uR"({raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"(AA)", uR"(Render())", __LINE__);

    content = uR"({raw:1}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"(abcA)", uR"(Render())", __LINE__);

    content = uR"({raw:1}{raw:2})";
    helper.Equal(Template::Render(content, value), uR"(abctrue)", uR"(Render())", __LINE__);

    content = uR"({raw:2}{raw:3}{raw:2})";
    helper.Equal(Template::Render(content, value), uR"(true456true)", uR"(Render())", __LINE__);

    content = uR"({raw:4}{raw:4}{raw:4})";
    helper.Equal(Template::Render(content, value), uR"(1.51.51.5)", uR"(Render())", __LINE__);
    ///

    content = uR"({raw:0}-{raw:0})";
    helper.Equal(Template::Render(content, value), uR"(A-A)", uR"(Render())", __LINE__);

    content = uR"({raw:1}--{raw:0})";
    helper.Equal(Template::Render(content, value), uR"(abc--A)", uR"(Render())", __LINE__);

    content = uR"({raw:1}---{raw:2})";
    helper.Equal(Template::Render(content, value), uR"(abc---true)", uR"(Render())", __LINE__);

    content = uR"({raw:2}{raw:3}--{raw:2})";
    helper.Equal(Template::Render(content, value), uR"(true456--true)", uR"(Render())", __LINE__);

    content = uR"({raw:4}--{raw:4}{raw:4})";
    helper.Equal(Template::Render(content, value), uR"(1.5--1.51.5)", uR"(Render())", __LINE__);

    content = uR"({raw:4}--{raw:4}--{raw:4})";
    helper.Equal(Template::Render(content, value), uR"(1.5--1.5--1.5)", uR"(Render())", __LINE__);

    content = uR"({raw:4}---{raw:4}---{raw:4})";
    helper.Equal(Template::Render(content, value), uR"(1.5---1.5---1.5)", uR"(Render())", __LINE__);
}

static void TestRawVariableUTag3(TestHelper &helper) {
    Value<char16_t> value;
    const char16_t *content;

    content = uR"({raw:0})";
    helper.Equal(Template::Render(content, value), uR"({raw:0})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({raw:a})", value), uR"({raw:a})", uR"(Render())", __LINE__);

    content = uR"({raw:0[0]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value), uR"({raw:0[0]})", uR"(Render())",
                 __LINE__);

    content = uR"({raw:a[0]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value), uR"({raw:a[0]})", uR"(Render())",
                 __LINE__);

    content = uR"({raw:0[a]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value), uR"({raw:0[a]})", uR"(Render())",
                 __LINE__);

    content = uR"({raw:a[abc]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value), uR"({raw:a[abc]})", uR"(Render())",
                 __LINE__);

    ////////////////

    value = JSON::Parse(uR"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = uR"({raw:0})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value), uR"({raw:0})", uR"(Render())",
                 __LINE__);

    content = uR"({raw:0[0]})";
    helper.Equal(Template::Render(content, value), uR"({raw:0[0]})", uR"(Render())", __LINE__);

    content = uR"({raw:0[0][0]})";
    helper.Equal(Template::Render(content, value), uR"({raw:0[0][0]})", uR"(Render())", __LINE__);

    /////

    content = uR"({raw:1})";
    helper.Equal(Template::Render(content, value), uR"({raw:1})", uR"(Render())", __LINE__);

    content = uR"({raw:2})";
    helper.Equal(Template::Render(content, value), uR"({raw:2})", uR"(Render())", __LINE__);

    content = uR"({raw:1[a]})";
    helper.Equal(Template::Render(content, value), uR"({raw:1[a]})", uR"(Render())", __LINE__);

    content = uR"({raw:1[b]})";
    helper.Equal(Template::Render(content, value), uR"({raw:1[b]})", uR"(Render())", __LINE__);

    ////

    value = JSON::Parse(uR"(["A", "abc", true, 456, 1.5])");

    content = uR"({raw:0)";
    helper.Equal(Template::Render(content, value), uR"({raw:0)", uR"(Render())", __LINE__);

    content = uR"(raw:0})";
    helper.Equal(Template::Render(content, value), uR"(raw:0})", uR"(Render())", __LINE__);

    content = uR"({r})";
    helper.Equal(Template::Render(content, value), uR"({r})", uR"(Render())", __LINE__);

    content = uR"({ra})";
    helper.Equal(Template::Render(content, value), uR"({ra})", uR"(Render())", __LINE__);

    content = uR"({raw})";
    helper.Equal(Template::Render(content, value), uR"({raw})", uR"(Render())", __LINE__);

    content = uR"({raw:})";
    helper.Equal(Template::Render(content, value), uR"({raw:})", uR"(Render())", __LINE__);

    content = uR"({r:0})";
    helper.Equal(Template::Render(content, value), uR"({r:0})", uR"(Render())", __LINE__);

    content = uR"({ra:0})";
    helper.Equal(Template::Render(content, value), uR"({ra:0})", uR"(Render())", __LINE__);
    ////

    content = uR"({raw:0{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({raw:0{raw:0})", uR"(Render())", __LINE__);

    content = uR"(raw:0{raw:0})";
    helper.Equal(Template::Render(content, value), uR"(raw:0A)", uR"(Render())", __LINE__);

    content = uR"(raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"(raw:0}A)", uR"(Render())", __LINE__);

    content = uR"({raw:0{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({raw:0{raw:0}A)", uR"(Render())", __LINE__);

    ////

    content = uR"({raw:0{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({raw:0{raw:0})", uR"(Render())", __LINE__);

    content = uR"(raw:0{raw:0})";
    helper.Equal(Template::Render(content, value), uR"(raw:0A)", uR"(Render())", __LINE__);

    content = uR"(raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"(raw:0}A)", uR"(Render())", __LINE__);

    content = uR"({raw:0{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({raw:0{raw:0}A)", uR"(Render())", __LINE__);

    ////

    content = uR"({{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({A)", uR"(Render())", __LINE__);

    content = uR"({{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({AA)", uR"(Render())", __LINE__);

    content = uR"({r{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({rA)", uR"(Render())", __LINE__);

    content = uR"({r{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({rAA)", uR"(Render())", __LINE__);

    content = uR"({ra{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({raA)", uR"(Render())", __LINE__);

    content = uR"({ra{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({raAA)", uR"(Render())", __LINE__);

    content = uR"({raw{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({rawA)", uR"(Render())", __LINE__);

    content = uR"({raw{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({rawAA)", uR"(Render())", __LINE__);

    ///

    content = uR"({-{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({-A)", uR"(Render())", __LINE__);

    content = uR"({-{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({-AA)", uR"(Render())", __LINE__);

    content = uR"({r-{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({r-A)", uR"(Render())", __LINE__);

    content = uR"({r-{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({r-AA)", uR"(Render())", __LINE__);

    content = uR"({ra-{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({ra-A)", uR"(Render())", __LINE__);

    content = uR"({ra-{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({ra-AA)", uR"(Render())", __LINE__);

    content = uR"({raw-{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({raw-A)", uR"(Render())", __LINE__);

    content = uR"({raw-{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value), uR"({raw-AA)", uR"(Render())", __LINE__);

    //

    content = uR"({raw-0})";
    helper.Equal(Template::Render(content, value), uR"({raw-0})", uR"(Render())", __LINE__);

    content = uR"({raw 0})";
    helper.Equal(Template::Render(content, value), uR"({raw 0})", uR"(Render())", __LINE__);

    content = uR"({raw:0 })";
    helper.Equal(Template::Render(content, value), uR"({raw:0 })", uR"(Render())", __LINE__);

    content = uR"({raw:0 )";
    helper.Equal(Template::Render(content, value), uR"({raw:0 )", uR"(Render())", __LINE__);

    content = uR"({raw:0)";
    helper.Equal(Template::Render(content, value), uR"({raw:0)", uR"(Render())", __LINE__);

    content = uR"({ raw-0})";
    helper.Equal(Template::Render(content, value), uR"({ raw-0})", uR"(Render())", __LINE__);

    content = uR"({ raw 0})";
    helper.Equal(Template::Render(content, value), uR"({ raw 0})", uR"(Render())", __LINE__);

    content = uR"({ raw:0 })";
    helper.Equal(Template::Render(content, value), uR"({ raw:0 })", uR"(Render())", __LINE__);

    content = uR"({ raw:0 )";
    helper.Equal(Template::Render(content, value), uR"({ raw:0 )", uR"(Render())", __LINE__);

    content = uR"({ raw:0)";
    helper.Equal(Template::Render(content, value), uR"({ raw:0)", uR"(Render())", __LINE__);
}

static void TestRawVariableUTag4(TestHelper &helper) {
    Value<char16_t> value;

    value += uR"(<)";
    value += uR"(>)";
    value += uR"(&)";
    value += uR"(")";
    value += uR"(')";
    value += uR"(<>)";
    value += uR"(<&)";
    value += uR"(<&)";
    value += uR"(>")";
    value += uR"("')";
    value += uR"(<">)";
    value += uR"(<'>)";
    value += uR"(<&>)";
    value += uR"(&"&)";
    value += uR"("'")";
    value += uR"('<')";
    value += uR"('&')";
    value += uR"(<>&'")";
    value += uR"('"<>&)";
    value += uR"(<"&'>)";

    value += uR"(<<<<<)";
    value += uR"(>>>>>)";
    value += uR"(&&&&&)";
    value += uR"(""""")";
    value += uR"(''''')";

    value += uR"(A<)";
    value += uR"(A>)";
    value += uR"(A&)";
    value += uR"(A")";
    value += uR"(A')";
    value += uR"(<A)";
    value += uR"(>A)";
    value += uR"(&A)";
    value += uR"("A)";
    value += uR"('A)";
    value += uR"(A<A)";
    value += uR"(A>A)";
    value += uR"(A&A)";
    value += uR"(A"A)";
    value += uR"(A'A)";

    value += uR"(AA<AA)";
    value += uR"(AA>AA)";
    value += uR"(AA&AA)";
    value += uR"(AA"AA)";
    value += uR"(AA'AA)";

    value += uR"(AA<<<<AA)";
    value += uR"(AA>>>>AA)";
    value += uR"(AA&&&&AA)";
    value += uR"(AA""""AA)";
    value += uR"(AA''''AA)";

    value += uR"(<A>B'C"D&E'F"G<H>I&G"K)";
    value += uR"(AB"CD'EF<GH>IGK')";
    value += uR"("ABC'DEF<GHI>GK<)";
    value += uR"(A""BC<<DE>>FG''HI&&GK)";

    helper.Equal(Template::Render(uR"({raw:0})", value), uR"(<)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:1})", value), uR"(>)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:2})", value), uR"(&)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:3})", value), uR"(")", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:4})", value), uR"(')", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:5})", value), uR"(<>)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:6})", value), uR"(<&)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:7})", value), uR"(<&)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:8})", value), uR"(>")", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:9})", value), uR"("')", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:10})", value), uR"(<">)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:11})", value), uR"(<'>)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:12})", value), uR"(<&>)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:13})", value), uR"(&"&)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:14})", value), uR"("'")", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:15})", value), uR"('<')", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:16})", value), uR"('&')", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:17})", value), uR"(<>&'")", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:18})", value), uR"('"<>&)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:19})", value), uR"(<"&'>)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:20})", value), uR"(<<<<<)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:21})", value), uR"(>>>>>)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:22})", value), uR"(&&&&&)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:23})", value), uR"(""""")", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:24})", value), uR"(''''')", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:25})", value), uR"(A<)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:26})", value), uR"(A>)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:27})", value), uR"(A&)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:28})", value), uR"(A")", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:29})", value), uR"(A')", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:30})", value), uR"(<A)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:31})", value), uR"(>A)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:32})", value), uR"(&A)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:33})", value), uR"("A)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:34})", value), uR"('A)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:35})", value), uR"(A<A)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:36})", value), uR"(A>A)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:37})", value), uR"(A&A)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:38})", value), uR"(A"A)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:39})", value), uR"(A'A)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:40})", value), uR"(AA<AA)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:41})", value), uR"(AA>AA)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:42})", value), uR"(AA&AA)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:43})", value), uR"(AA"AA)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:44})", value), uR"(AA'AA)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:45})", value), uR"(AA<<<<AA)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:46})", value), uR"(AA>>>>AA)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:47})", value), uR"(AA&&&&AA)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:48})", value), uR"(AA""""AA)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:49})", value), uR"(AA''''AA)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:50})", value), uR"(<A>B'C"D&E'F"G<H>I&G"K)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:51})", value), uR"(AB"CD'EF<GH>IGK')", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:52})", value), uR"("ABC'DEF<GHI>GK<)", uR"(Render())", __LINE__);
    helper.Equal(Template::Render(uR"({raw:53})", value), uR"(A""BC<<DE>>FG''HI&&GK)", uR"(Render())", __LINE__);
}

static void TestMathUTag1(TestHelper &helper) {
    Value<char16_t> value;

    value[uR"(a1)"] = 5;
    value[uR"(a2)"] = true;
    value[uR"(a3)"] = nullptr;
    value[uR"(a4)"] = false;
    value[uR"(a5)"] = uR"(10)";
    value[uR"(a6)"] = uR"(20)";
    value[uR"(a7)"] = 6;
    value[uR"(a8)"] = 1;
    value[uR"(a9)"] = uR"(1)";

    helper.Equal(Template::Render(uR"({math:1+1})", value), uR"(2)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a1}+8})", value), uR"(13)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a7}+8})", value), uR"(14)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a7}+{var:a1}})", value), uR"(11)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a5}+{var:a1}})", value), uR"(15)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a1}+{var:a5}})", value), uR"(15)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a6}+{var:a5}})", value), uR"(30)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a6}*{var:a2}})", value), uR"(20)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a6}*{var:a4}})", value), uR"(0)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a6}*{var:a7}})", value), uR"(120)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a7}+{var:a6}})", value), uR"(26)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a1}+{var:a2}})", value), uR"(6)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a8}=={var:a2}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a2}=={var:a8}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a5}!={var:a1}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a2}!={var:a4}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a2}==true})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a9}=={var:a8}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a8}=={var:a9}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:1=={var:a8}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:6-5==({var:a9})})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:6-5==({var:a8})})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:null!={var:a3}})", value), uR"(0)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:({var:a3})==(0)})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a1}+{var:a7}})", value), uR"(11)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a1}+{var:a7}}{math:{var:a1}+{var:a7}})", value), uR"(1111)",
                 uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a1}+{var:a7}}*{math:{var:a1}+{var:a7}})", value), uR"(11*11)",
                 uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a1}+{var:a7}}##{math:{var:a1}+{var:a7}})", value), uR"(11##11)",
                 uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a1}+{var:a7}}&&&%%^^&&*{math:{var:a1}+{var:a7}})", value),
                 uR"(11&&&%%^^&&*11)", uR"(Render())", __LINE__);

    ///////////////////

    helper.Equal(Template::Render(uR"({math: {var:a1}+8})", value), uR"(13)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:  {var:a7}+8})", value), uR"(14)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:     {var:a7}+{var:a1}})", value), uR"(11)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a5} +{var:a1}})", value), uR"(15)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a1}  +{var:a5}})", value), uR"(15)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a6}    +{var:a5}})", value), uR"(30)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a6}* {var:a2}})", value), uR"(20)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a6}*  {var:a4}})", value), uR"(0)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a6}*      {var:a7}})", value), uR"(120)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a7}+{var:a6} })", value), uR"(26)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a1}+{var:a2}  })", value), uR"(6)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a8}=={var:a2}      })", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a2}=={var:a8}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math: {var:a5}!={var:a1} })", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:  {var:a2}!={var:a4}  })", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:    1=={var:a9}     })", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a9} == {var:a8}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a8}  ==  {var:a9}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:1==          {var:a8}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:6-5         ==1})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:1          ==            {var:a8}})", value), uR"(1)", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:     0     !=    ({var:a3})        })", value), uR"(0)", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:a3}       ==       null     })", value), uR"(1)", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:((8+1+{var:a8}))})", value), uR"(10)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:10==(8+1+{var:a8})})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:((8+1+{var:a8}))==9+1})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:(5*2)==((8+1+{var:a8}))})", value), uR"(1)", uR"(Render())", __LINE__);

    //////////////
    value.Reset();

    value += 5;
    value += true;
    value += nullptr;
    value += false;
    value += uR"(10)";
    value += uR"(20)";
    value += 6;
    value += 1;
    value += uR"(1)";
    value += uR"(Qentem)";

    helper.Equal(Template::Render(uR"({math:{var:0}+8})", value), uR"(13)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:6}+8})", value), uR"(14)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:6}+{var:0}})", value), uR"(11)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:4}+{var:0}})", value), uR"(15)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:0}+{var:4}})", value), uR"(15)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:5}+{var:4}})", value), uR"(30)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:5}*{var:1}})", value), uR"(20)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:5}*{var:3}})", value), uR"(0)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:5}*{var:6}})", value), uR"(120)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:6}+{var:5}})", value), uR"(26)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:0}+{var:1}})", value), uR"(6)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:7}=={var:1}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:1}=={var:7}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:4}!={var:0}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:1}!={var:3}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:({var:1})==({var:8})})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"(-{math:{var:8}=={var:7}})", value), uR"(-1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"(--{math:{var:7}=={var:8}})", value), uR"(--1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"(---{math:1=={var:7}})", value), uR"(---1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:1==({var:8})}-)", value), uR"(1-)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:6-5==({var:7})}--)", value), uR"(1--)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:0==({var:2})}---)", value), uR"(1---)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"(-{math:{var:2}!=null}-)", value), uR"(-0-)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"(--{math:Qente=={var:9}}--)", value), uR"(--0--)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"(---{math:Qente !={var:9}}---)", value), uR"(---1---)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:  Qentem   =={var:9}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:  Qentem!={var:9}})", value), uR"(0)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:9}   ==    Qente})", value), uR"(0)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:9} !=    Qente    })", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:  {var:9}   ==Qentem})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math: {var:9} !=Qentem})", value), uR"(0)", uR"(Render())", __LINE__);

    /////////

    helper.Equal(Template::Render(uR"({math: true == {var:1}})", value), uR"(1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math: {var:1} == true})", value), uR"(1)", uR"(Render())", __LINE__);
}

static void TestMathUTag2(TestHelper &helper) {
    Value<char16_t> value;

    value += Array<Value<char16_t>>();
    value += HArray<Value<char16_t>, char16_t>();
    value += 5;

    helper.Equal(Template::Render(uR"({math:{var:0}+8})", value), uR"({math:{var:0}+8})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:1}+8})", value), uR"({math:{var:1}+8})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:2}+{var:0}})", value), uR"({math:{var:2}+{var:0}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:2}+{var:0}})", value), uR"({math:{var:2}+{var:0}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:0}+{var:1}})", value), uR"({math:{var:0}+{var:1}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:1}+{var:2}})", value), uR"({math:{var:1}+{var:2}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:2}*{var:1}})", value), uR"({math:{var:2}*{var:1}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:0}*{var:1}})", value), uR"({math:{var:0}*{var:1}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:0}*{var:2}})", value), uR"({math:{var:0}*{var:2}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:2}+{var:0}})", value), uR"({math:{var:2}+{var:0}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:1}+{var:2}})", value), uR"({math:{var:1}+{var:2}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:0}=={var:1}})", value), uR"({math:{var:0}=={var:1}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:1}=={var:0}})", value), uR"({math:{var:1}=={var:0}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:0}!={var:2}})", value), uR"({math:{var:0}!={var:2}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:2}!={var:0}})", value), uR"({math:{var:2}!={var:0}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:1}=={var:2}})", value), uR"({math:{var:1}=={var:2}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:8}=={var:7}})", value), uR"({math:{var:8}=={var:7}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:7}=={var:2}})", value), uR"({math:{var:7}=={var:2}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:1=={var:7}})", value), uR"({math:1=={var:7}})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:(6-5)=={var:8}})", value), uR"({math:(6-5)=={var:8}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:(6-5)=={var:0}})", value), uR"({math:(6-5)=={var:0}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:0}=={var:8}})", value), uR"({math:{var:0}=={var:8}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:0}=={var:0}})", value), uR"({math:{var:0}=={var:0}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:0=={var:1}})", value), uR"({math:0=={var:1}})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:1}!=0})", value), uR"({math:{var:1}!=0})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:W={var:0}})", value), uR"({math:W={var:0}})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:W=={var:0}})", value), uR"({math:W=={var:0}})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:2}==c})", value), uR"({math:{var:2}==c})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:abc=={var:2}})", value), uR"({math:abc=={var:2}})", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math:sds})", value), uR"({math:sds})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:1)", value), uR"({math:1)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"(math:1})", value), uR"(math:1})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:{var:2})", value), uR"({math:5)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({{math:{var:2}+5})", value), uR"({10)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({m{var:2}})", value), uR"({m5})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({ma{var:2}})", value), uR"({ma5})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({mat{var:2}})", value), uR"({mat5})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math{var:2}})", value), uR"({math5})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math {var:2}})", value), uR"({math 5})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:})", value), uR"({math:})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math-{var:2}}{math:{var:2}+5})", value), uR"({math-5}10)", uR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(uR"({math-4}{math:{var:2}+5})", value), uR"({math-4}10)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math-4} {math:{var:2}+5})", value), uR"({math-4} 10)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:4)", value), uR"({math:4)", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:4    )", value), uR"({math:4    )", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:A==1+1})", value), uR"({math:A==1+1})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:(A)!=1+1})", value), uR"({math:(A)!=1+1})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:1+1==A})", value), uR"({math:1+1==A})", uR"(Render())", __LINE__);

    helper.Equal(Template::Render(uR"({math:1+1!=(A)})", value), uR"({math:1+1!=(A)})", uR"(Render())", __LINE__);
}

static void TestInlineIfUTag(TestHelper &helper) {
    Value<char16_t> value;
    const char16_t *content;

    value += 0;
    value += 1;
    value += -1;
    value += true;
    value += false;
    value += nullptr;
    value += uR"(ABC)";
    value += Array<Value<char16_t>>();

    content = uR"({if case="0" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(F)", uR"(Render())", __LINE__);

    content = uR"({if case="-1" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(F)", uR"(Render())", __LINE__);

    content = uR"({if case="0.1" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    content = uR"({if case="1" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    ////

    content = uR"({if case="0" true="T"})";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"({if case="-1" true="T"})";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"({if case="0.1" true="T"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    content = uR"({if case="1" true="T"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    ///

    content = uR"({if case="0" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(F)", uR"(Render())", __LINE__);

    content = uR"({if case="-1" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(F)", uR"(Render())", __LINE__);

    content = uR"({if case="0.1" false="F"})";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"({if case="1" false="F"})";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    /////

    content = uR"({if case="{var:0}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(F)", uR"(Render())", __LINE__);

    content = uR"({if case="{var:2}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(F)", uR"(Render())", __LINE__);

    content = uR"({if case="{raw:4}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(F)", uR"(Render())", __LINE__);

    content = uR"({if case="{var:5}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(F)", uR"(Render())", __LINE__);

    content = uR"({if case="{var:6}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"({if case="fas" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"({if case="{var:7}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"({if case="{var:20}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    ////
    content = uR"({if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    content = uR"({if case="{var:3}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    content = uR"({if case="{var:6}==ABC" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    /////////////////

    content = uR"({if case=" {var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    content = uR"({if case="{var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    content = uR"({if case=" {var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    content = uR"({if case="  {var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    content = uR"({if case="{var:1}  " true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    content = uR"({if case="  {var:1}  " true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    content = uR"({if case="      {var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    content = uR"({if case="{var:1}          " true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    content = uR"({if case="    {var:1}        " true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);
    ////

    content = uR"(-{if case=" {var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(-T)", uR"(Render())", __LINE__);

    content = uR"({if case=" {var:1} " true="T" false="F"}-)";
    helper.Equal(Template::Render(content, value), uR"(T-)", uR"(Render())", __LINE__);

    content = uR"(-{if case=" {var:1} " true="T" false="F"}-)";
    helper.Equal(Template::Render(content, value), uR"(-T-)", uR"(Render())", __LINE__);

    content = uR"(--{if case=" {var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(--T)", uR"(Render())", __LINE__);

    content = uR"({if case=" {var:1} " true="T" false="F"}--)";
    helper.Equal(Template::Render(content, value), uR"(T--)", uR"(Render())", __LINE__);

    content = uR"(--{if case=" {var:1} " true="T" false="F"}--)";
    helper.Equal(Template::Render(content, value), uR"(--T--)", uR"(Render())", __LINE__);

    content = uR"(---{if case=" {var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(---T)", uR"(Render())", __LINE__);

    content = uR"({if case=" {var:1} " true="T" false="F"}---)";
    helper.Equal(Template::Render(content, value), uR"(T---)", uR"(Render())", __LINE__);

    content = uR"(---{if case=" {var:1} " true="T" false="F"}---)";
    helper.Equal(Template::Render(content, value), uR"(---T---)", uR"(Render())", __LINE__);

    content = uR"({if case="1" true="T"}{if case="0" false="F"}{if case="1" true="T"})";
    helper.Equal(Template::Render(content, value), uR"(TFT)", uR"(Render())", __LINE__);

    ///////

    content = uR"({if case="{var:7}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"(T)", uR"(Render())", __LINE__);

    content = uR"({if case="01" true="{var:3}" false="{var:4}"}--)";
    helper.Equal(Template::Render(content, value), uR"(--)", uR"(Render())", __LINE__);

    content = uR"({if case="" true="c" false="d"})";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"({i)";
    helper.Equal(Template::Render(content, value), uR"({i)", uR"(Render())", __LINE__);

    content = uR"({if)";
    helper.Equal(Template::Render(content, value), uR"({if)", uR"(Render())", __LINE__);

    content = uR"({if})";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"({{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"({T)", uR"(Render())", __LINE__);

    content = uR"({i{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"({iT)", uR"(Render())", __LINE__);

    content = uR"({if{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"({ifT)", uR"(Render())", __LINE__);

    content = uR"({if case="{var:1}"                    put="F"})";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"({if{if case="1" true="T" false="F"}}{if case="1" true="T" false="F"})";

    helper.Equal(Template::Render(content, value), uR"(TT)", uR"(Render())", __LINE__);

    content = uR"({if{if case="{raw:1}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value), uR"({ifTT)", uR"(Render())", __LINE__);

    /////
    content = uR"({if case="0" true="{var:3}" false="{var:4}"})";
    helper.Equal(Template::Render(content, value), uR"(false)", uR"(Render())", __LINE__);

    content = uR"({if case="1" true="{var:3}" false="{var:4}"})";
    helper.Equal(Template::Render(content, value), uR"(true)", uR"(Render())", __LINE__);

    content = uR"({if case="0" true="{raw:3}{raw:3}" false="{var:4}{var:4}"})";
    helper.Equal(Template::Render(content, value), uR"(falsefalse)", uR"(Render())", __LINE__);

    content = uR"({if case="1000" true="{var:3}{var:3}" false="{var:4}{var:4}"})";
    helper.Equal(Template::Render(content, value), uR"(truetrue)", uR"(Render())", __LINE__);

    content = uR"({if case="0" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    helper.Equal(Template::Render(content, value), uR"(false---false)", uR"(Render())", __LINE__);

    content = uR"({if case="1" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    helper.Equal(Template::Render(content, value), uR"(true---true)", uR"(Render())", __LINE__);

    content = uR"({if case="0" true="{var:10}" false="{var:20}"})";
    helper.Equal(Template::Render(content, value), uR"({var:20})", uR"(Render())", __LINE__);

    content = uR"({if case="1" true="{var:10}" false="{var:20}"})";
    helper.Equal(Template::Render(content, value), uR"({var:10})", uR"(Render())", __LINE__);

    content = uR"({if case="1" true="1" false="0")";
    helper.Equal(Template::Render(content, value), uR"({if case="1" true="1" false="0")", uR"(Render())", __LINE__);

    content = uR"({if case="1" true="1" false="0")";
    helper.Equal(Template::Render(content, value), uR"({if case="1" true="1" false="0")", uR"(Render())", __LINE__);

    ///////
    Value<char16_t> value2;

    value2 += uR"(&)";
    value2 += uR"(")";
    value2 += 10;
    value2 += 5;
    value2 += 15;

    content = uR"({if case="1" true="{var:0}" false="{var:1}"})";

    if constexpr (Config::AutoEscapeHTML) {
        helper.Equal(Template::Render(content, value2), uR"(&amp;)", uR"(Render())", __LINE__);
    } else {
        helper.Equal(Template::Render(content, value2), uR"(&)", uR"(Render())", __LINE__);
    }

    content = uR"({if case="1" true="{raw:0}" false="{raw:1}"})";
    helper.Equal(Template::Render(content, value2), uR"(&)", uR"(Render())", __LINE__);

    content = uR"({if case="0" true="{var:0}" false="{var:1}"})";

    if constexpr (Config::AutoEscapeHTML) {
        helper.Equal(Template::Render(content, value2), uR"(&quot;)", uR"(Render())", __LINE__);
    } else {
        helper.Equal(Template::Render(content, value2), uR"(")", uR"(Render())", __LINE__);
    }

    content = uR"({if case="0" true="*{raw:0}*" false="-{raw:1}-"})";
    helper.Equal(Template::Render(content, value2), uR"(-"-)", uR"(Render())", __LINE__);

    content = uR"({if case="0" true="{raw:0}" false="{raw:1}"})";
    helper.Equal(Template::Render(content, value2), uR"(")", uR"(Render())", __LINE__);

    content =
        uR"({if case="{var:2}+{var:3} == {var:4}" true="{math:{var:3}+{var:4}}" false="{math: {var:2}+{var:4}}"})";
    helper.Equal(Template::Render(content, value2), uR"(20)", uR"(Render())", __LINE__);

    content =
        uR"({if case=" {var:4}-{var:3} != {var:2} " true="{math: {var:3}+{var:4} } " false=" {math: {var:2}+{var:4} } "})";
    helper.Equal(Template::Render(content, value2), uR"( 25 )", uR"(Render())", __LINE__);
}

static void TestLoopUTag1(TestHelper &helper) {
    Value<char16_t> value3;
    const char16_t *content;
    Value<char16_t> value1;

    value1 += 100;
    value1 += -50;
    value1 += uR"(Qentem)";
    value1 += true;
    value1 += false;
    value1 += nullptr;
    value1 += 3;

    content = uR"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value1), uR"(100, -50, Qentem, true, false, null, 3, )", uR"(Render())",
                 __LINE__);

    content = uR"(<loop value="loop1-value">{var:loop1-value}, {var:loop1-value} </loop>)";
    helper.Equal(Template::Render(content, value1),
                 uR"(100, 100 -50, -50 Qentem, Qentem true, true false, false null, null 3, 3 )", uR"(Render())",
                 __LINE__);

    ////////////////

    value1.Reset();
    value1 += 0;
    value1 += 1;

    content =
        uR"(<loop value="loop1-value"><loop value="loop2-value">({var:loop1-value}: {var:loop2-value}) </loop></loop>)";
    helper.Equal(Template::Render(content, value1), uR"((0: 0) (0: 1) (1: 0) (1: 1) )", uR"(Render())", __LINE__);

    content = uR"(<loop value="loop1-value"><loop value="loop2-value"><loop
                 value="loop3-value">({var:loop1-value}: {var:loop2-value}: {var:loop3-value}) </loop></loop></loop>)";
    helper.Equal(Template::Render(content, value1),
                 uR"((0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1) )", uR"(Render())",
                 __LINE__);

    content = uR"(<loop value="loop1-value"><loop value="loop2-value"><loop
                 value="loop3-value"><loop value="loop4-value">({var:loop1-value}: {var:loop2-value}: {var:loop3-value}: {var:loop4-value}) </loop></loop></loop></loop>)";
    helper.Equal(
        Template::Render(content, value1),
        uR"((0: 0: 0: 0) (0: 0: 0: 1) (0: 0: 1: 0) (0: 0: 1: 1) (0: 1: 0: 0) (0: 1: 0: 1) (0: 1: 1: 0) (0: 1: 1: 1) (1: 0: 0: 0) (1: 0: 0: 1) (1: 0: 1: 0) (1: 0: 1: 1) (1: 1: 0: 0) (1: 1: 0: 1) (1: 1: 1: 0) (1: 1: 1: 1) )",
        uR"(Render())", __LINE__);

    //////////////////////

    value1 = JSON::Parse(uR"([100, -50, "A", true, false, null])");
    const Value<char16_t> value2 =
        JSON::Parse(uR"({"k-1": 4, "k-2":1.5, "k-3":"ABC", "k-4":true, "k-5":false, "k-6":null})");

    //////////////////////
    value3[uR"(arr1)"] = value1;

    content = uR"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value1), uR"(100, -50, A, true, false, null, )", uR"(Render())", __LINE__);

    content = uR"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value2), uR"(4, 1.5, ABC, true, false, null, )", uR"(Render())", __LINE__);

    content = uR"(<loop set="arr1" value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3), uR"(100, -50, A, true, false, null, )", uR"(Render())", __LINE__);

    value3[uR"(arr1)"] = value2;

    content = uR"(<loop set="arr1" value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3), uR"(4, 1.5, ABC, true, false, null, )", uR"(Render())", __LINE__);

    //////////////////////
    value3[uR"(arr1)"] = value1;

    content = uR"(<loop value="loop1-value" >{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value1), uR"(100, -50, A, true, false, null, )", uR"(Render())", __LINE__);

    content = uR"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value2), uR"(4, 1.5, ABC, true, false, null, )", uR"(Render())", __LINE__);

    content = uR"(<loop value="loop1-value"set="arr1">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3), uR"(100, -50, A, true, false, null, )", uR"(Render())", __LINE__);

    value3[uR"(arr1)"] = value2;

    content = uR"(<loop set="arr1" value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3), uR"(4, 1.5, ABC, true, false, null, )", uR"(Render())", __LINE__);

    //////////////////////
    value3.Reset();
    value3[uR"(arr1)"][uR"(arr2)"][uR"(arr3)"] = value1;

    content = uR"(<loop set="arr1[arr2][arr3]" value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3), uR"(100, -50, A, true, false, null, )", uR"(Render())", __LINE__);

    value3.Reset();
    value3[0][0] += value2;

    content = uR"(<loop set="0[0][0]"value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3), uR"(4, 1.5, ABC, true, false, null, )", uR"(Render())", __LINE__);

    value3.Reset();
    value3[uR"(k1)"][0][uR"(k3)"] = value1;

    content = uR"(<loop value="loop1-value" set="k1[0][k3]">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3), uR"(100, -50, A, true, false, null, )", uR"(Render())", __LINE__);

    value3.Reset();
    value3[0][uR"(k2)"] += value2;

    content = uR"(<loop set="0[k2][0]"value="loop1-value">{var:loop1-value}, {var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3),
                 uR"(4, 4, 1.5, 1.5, ABC, ABC, true, true, false, false, null, null, )", uR"(Render())", __LINE__);

    value3 = JSON::Parse(uR"({"group":[[10],[20],[30]]})");

    content = uR"(<loop set="group" value="_Val1"><loop set="_Val1" value="_Val2">{var:_Val2}</loop></loop>)";
    helper.Equal(Template::Render(content, value3), uR"(102030)", uR"(Render())", __LINE__);

    value3 = JSON::Parse(uR"({"group":[1,2,3,4]})");

    content = uR"(<loop set="group" value="_Val">{var:_Val}</loop>)";
    helper.Equal(Template::Render(content, value3), uR"(1234)", uR"(Render())", __LINE__);

    value3  = JSON::Parse(uR"({"numbers":[1,2,3,4,5,6,7,8]})");
    content = uR"(A<loop set="numbers" value="l_id1">{var:l_id1}</loop>B)";
    helper.Equal(Template::Render(content, value3), uR"(A12345678B)", uR"(Render())", __LINE__);

    content = uR"(<loop set="numbers" value="l_id1">{var:l_id1[0]}</loop>)";
    helper.Equal(
        Template::Render(content, value3),
        uR"({var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]})",
        uR"(Render())", __LINE__);
}

static void TestLoopUTag2(TestHelper &helper) {
    Value<char16_t> value;
    const char16_t *content;

    content = uR"(<loop></loop>)";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"(<loop>abcd</loop>)";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"(<l</loop>)";
    helper.Equal(Template::Render(content, value), uR"(<l</loop>)", uR"(Render())", __LINE__);

    content = uR"(<lo</loop>)";
    helper.Equal(Template::Render(content, value), uR"(<lo</loop>)", uR"(Render())", __LINE__);

    content = uR"(<loo</loop>)";
    helper.Equal(Template::Render(content, value), uR"(<loo</loop>)", uR"(Render())", __LINE__);

    content = uR"(<loop></loop><loop>A</loop>)";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"(<loop value="a">{var:a}</loop>)";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"(<loop set="ss" value="a">{var:a}</loop>)";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"(<loop set="" value="a">{var:a}</loop>)";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    value[uR"(in)"] = Array<Value<char16_t>>();

    content = uR"(<loop value="v">{var:v}</loop>)";
    helper.Equal(Template::Render(content, value), uR"(in)", uR"(Render())", __LINE__);

    value.Reset();

    /////
    value.Reset();
    value[uR"(k1)"] = 10;
    value[uR"(k2)"] = 20;
    value[uR"(k3)"] = 30;

    value[uR"(k2)"].Reset();

    content = uR"(<loop value="v">{var:v}</loop>)";
    helper.Equal(Template::Render(content, value), uR"(1030)", uR"(Render())", __LINE__);

    value.RemoveIndex(1);

    content = uR"(<loop value="v">{var:v})";
    helper.Equal(Template::Render(content, value), uR"(<loop value="v">{var:v})", uR"(Render())", __LINE__);

    content = uR"(<loop value="v">{var:v}     )";
    helper.Equal(Template::Render(content, value), uR"(<loop value="v">{var:v}     )", uR"(Render())", __LINE__);

    value.Reset();
    value += 10;
    value += 20;
    value += 30;

    value.RemoveIndex(1);

    content = uR"(<loop value="v">{var:v}</loop>)";
    helper.Equal(Template::Render(content, value), uR"(1030)", uR"(Render())", __LINE__);

    value = JSON::Parse(uR"(
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
        uR"(<loop set="object" value="item">{var:item[var1]}{var:item[var2]}{var:item[var3]} {var:item[var4]}</loop>)";
    helper.Equal(Template::Render(content, value), uR"(value1value2value3 value4)", uR"(Render())", __LINE__);

    content = uR"(<loop set="array" value="item"> {var:item[0]} {var:item[1]} {var:item[2]} {var:item[3]} </loop>)";
    helper.Equal(Template::Render(content, value), uR"( value10 value20 value30 value40 )", uR"(Render())", __LINE__);

    content =
        uR"(<loop set="object" value="item">{var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]}</loop>)";
    helper.Equal(Template::Render(content, value),
                 uR"({var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]})", uR"(Render())", __LINE__);

    content =
        uR"(<loop set="array" value="item">{var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]}</loop>)";
    helper.Equal(Template::Render(content, value),
                 uR"({var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]})", uR"(Render())", __LINE__);

    value.RemoveIndex(0);
    content = uR"(<loop><l</loop>)";
    helper.Equal(Template::Render(content, value), uR"(<l)", uR"(Render())", __LINE__);

    value = JSON::Parse(uR"(
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

    content = uR"(<loop set="2020">{var:name}</loop>)";
    helper.Equal(Template::Render(content, value), uR"(some_valsome_valsome_val)", uR"(Render())", __LINE__);

    static constexpr unsigned int size_4 = (8 * 4);

    StringStream<char16_t> content2;
    StringStream<char16_t> output;
    Value<char16_t>        value2;

    for (unsigned int i = 0; i < size_4; i++) {
        value2 += i;
    }

    content2 += uR"(<loop value="loop1-value">A {var:loop1-value} B</loop>)";
    for (unsigned int i = 0; i < size_4; i++) {
        output += uR"(A )";
        Digit::NumberToString(output, i);
        output += uR"( B)";
    }

    helper.Equal(Template::Render(content2.First(), content2.Length(), value2), output, uR"(Render())", __LINE__);
}

static void TestLoopUTag3(TestHelper &helper) {
    const char16_t *content;
    Value<char16_t> value = JSON::Parse(uR"(
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
        uR"(<loop value="val1_" group="year111" sort="descend"><loop set="val1_" value="val2_"><loop set="val2_" value="val3_">{var:val3_}</loop></loop></loop>)";

    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content =
        uR"(<loop value="val1_" group="year" sort="descend"><loop set="val1_" value="val2_"><loop set="val2_" value="val3_">{var:val3_}</loop></loop></loop>)";

    helper.Equal(Template::Render(content, value), uR"(q11400q11450q11450q11100q11125q21200q22300q21200q22300q22300)",
                 uR"(Render())", __LINE__);

    content =
        uR"(<loop value="val1_" group="year" sort="descend"><loop set="val1_" value="val2_" group="quarter" sort="ascend"><loop set="val2_" value="val3_"><loop set="val3_" value="val4_">{var:val4_}</loop></loop></loop></loop>)";

    helper.Equal(Template::Render(content, value), uR"(1400145014501100112512002300120023002300)", uR"(Render())",
                 __LINE__);

    content =
        uR"(<loop value="val1_" group="year" sort="descend">-- {var:val1_}-<loop set="val1_" value="val2_" group="quarter" sort="ascend">{var:val2_}-<loop set="val2_" value="val3_" group="week" sort="ascend">{var:val2_}:<loop set="val3_" value="val4_"><loop set="val4_" value="val5_"> {var:val5_}</loop></loop></loop></loop></loop>)";

    helper.Equal(
        Template::Render(content, value),
        uR"(-- 2020-q1-1: 400 450 450-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2018-q2-1: 2002: 300-- 2017-q2-2: 300)",
        uR"(Render())", __LINE__);

    content =
        uR"(<loop value="val1_" group="year">-- {var:val1_}-<loop set="val1_" value="val2_" group="quarter">{var:val2_}-<loop set="val2_" value="val3_" group="week">{var:val2_}:<loop set="val3_" value="val4_"><loop set="val4_" value="val5_"> {var:val5_}</loop></loop></loop></loop></loop>)";

    helper.Equal(
        Template::Render(content, value),
        uR"(-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2017-q2-2: 300-- 2020-q1-1: 400 450 450-- 2018-q2-1: 2002: 300)",
        uR"(Render())", __LINE__);

    ////////////

    value.Reset();

    value += 4;
    value += 1;
    value += 3;
    value += 5;
    value += 2;
    value += 7;
    value += 6;

    content = uR"(<loop value="val1_" sort="a">{var:val1_}</loop>)";

    helper.Equal(Template::Render(content, value), uR"(1234567)", uR"(Render())", __LINE__);

    content = uR"(<loop value="val1_" sort="d">{var:val1_}</loop>)";

    helper.Equal(Template::Render(content, value), uR"(7654321)", uR"(Render())", __LINE__);
}

static void TestIfUTag1(TestHelper &helper) {
    Value<char16_t> value;
    const char16_t *content;

    value[uR"(name)"] = uR"(Qen)";
    value[uR"(t)"]    = true;
    value[uR"(f)"]    = false;
    value[uR"(n)"]    = nullptr;
    value[uR"(1)"]    = 1;
    value[uR"(one)"]  = uR"(1)";
    value[uR"(zero)"] = 0;

    content = uR"(<if case="1>0">{var:name}</if>)";
    helper.Equal(Template::Render(content, value), uR"(Qen)", uR"(Render())", __LINE__);

    content = uR"(#<if case="{var:one}">{var:name}</if>#)";
    helper.Equal(Template::Render(content, value), uR"(#Qen#)", uR"(Render())", __LINE__);

    content = uR"(##<if case="{var:zero}">{var:name}</if>##)";
    helper.Equal(Template::Render(content, value), uR"(####)", uR"(Render())", __LINE__);

    content = uR"(<if case="{var:1}">{var:name}1<else />{var:name}2</if>)";
    helper.Equal(Template::Render(content, value), uR"(Qen1)", uR"(Render())", __LINE__);

    content = uR"(<if case="{var:f}">{var:name}1<else />{var:name}2</if>)";
    helper.Equal(Template::Render(content, value), uR"(Qen2)", uR"(Render())", __LINE__);

    content = uR"(#<if case="{var:t}">G<if case="1">oo</if>d</if>!#)";
    helper.Equal(Template::Render(content, value), uR"(#Good!#)", uR"(Render())", __LINE__);

    content = uR"(###<if case="1">G<if case="1">o</if>o<if case="1">d!</if></if>###)";
    helper.Equal(Template::Render(content, value), uR"(###Good!###)", uR"(Render())", __LINE__);

    content = uR"(Be <if case="1">G<if case="1">oo<if case="1">d</if></if></if>!)";
    helper.Equal(Template::Render(content, value), uR"(Be Good!)", uR"(Render())", __LINE__);

    content = uR"(<if case="1">Good!<elseif case="0" />Bad!</if>)";
    helper.Equal(Template::Render(content, value), uR"(Good!)", uR"(Render())", __LINE__);

    content = uR"(<if case="{var:n}">Bad!<elseif case="1" />Good!</if>#)";
    helper.Equal(Template::Render(content, value), uR"(Good!#)", uR"(Render())", __LINE__);

    content = uR"(<if case="{var:f}">Bad!<elseif case="0" />Very Bad!</if>)";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"(#<if case="0">Bad!<elseif case="0" />Very Bad!<else />Very Good!</if>)";
    helper.Equal(Template::Render(content, value), uR"(#Very Good!)", uR"(Render())", __LINE__);

    content = uR"(<if case="1">a<else />b</if>)";
    helper.Equal(Template::Render(content, value), uR"(a)", uR"(Render())", __LINE__);

    content = uR"(<if case="0">a<else />b</if>)";
    helper.Equal(Template::Render(content, value), uR"(b)", uR"(Render())", __LINE__);

    content = uR"(<if case="0">a<else /><if case="1">b</if></if>)";
    helper.Equal(Template::Render(content, value), uR"(b)", uR"(Render())", __LINE__);

    content = uR"(<if case="1">a<else /><if case="1">b</if></if>)";
    helper.Equal(Template::Render(content, value), uR"(a)", uR"(Render())", __LINE__);

    content = uR"(<if case="0">a<if case="1">b</if>c</if>Empty)";
    helper.Equal(Template::Render(content, value), uR"(Empty)", uR"(Render())", __LINE__);

    content = uR"(<if case="1">a<else /><if case="1">b</if>c</if>===========)";
    helper.Equal(Template::Render(content, value), uR"(a===========)", uR"(Render())", __LINE__);

    content = uR"(===========<if case="1">a<if case="1">b</if><else />c</if>)";
    helper.Equal(Template::Render(content, value), uR"(===========ab)", uR"(Render())", __LINE__);

    content = uR"(<if case="1">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    helper.Equal(Template::Render(content, value), uR"(a)", uR"(Render())", __LINE__);

    content = uR"(<if case="0">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    helper.Equal(Template::Render(content, value), uR"(b)", uR"(Render())", __LINE__);

    content = uR"(===========<if case="0">a<elseif case="0" />b<elseif case="1" />c<else />d</if>===========)";
    helper.Equal(Template::Render(content, value), uR"(===========c===========)", uR"(Render())", __LINE__);

    content = uR"(<if case="0">a<elseif case="0" />b<elseif case="0" />c<else />d</if>)";
    helper.Equal(Template::Render(content, value), uR"(d)", uR"(Render())", __LINE__);

    content = uR"(<if case="1">a<if case="0">b<elseif case="0"/>c</if></if>)";
    helper.Equal(Template::Render(content, value), uR"(a)", uR"(Render())", __LINE__);

    content = uR"(<if case="1">a<if case="1">b<elseif case="0"/>c</if></if>)";
    helper.Equal(Template::Render(content, value), uR"(ab)", uR"(Render())", __LINE__);

    content = uR"(<if case="1"><if case="1">b<elseif case="c"/>c</if>a<else />c</if>)";
    helper.Equal(Template::Render(content, value), uR"(ba)", uR"(Render())", __LINE__);

    content = uR"(<if case="0">a<else />c<if case="1">b<elseif case="0"/>c</if></if>)";
    helper.Equal(Template::Render(content, value), uR"(cb)", uR"(Render())", __LINE__);

    content = uR"(<if_case="1"><if case="0">Bad1!<elseif case="0" />Bad2!</if>a</if>)";
    helper.Equal(Template::Render(content, value), uR"(a)", uR"(Render())", __LINE__);

    content = uR"(<if case="1">
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
    helper.Equal(String<char16_t>::Trim(Template::Render(content, value).GetString()), uR"(a)", uR"(Render())",
                 __LINE__);

    content = uR"(<if case="0">a
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
    helper.Equal(String<char16_t>::Trim(Template::Render(content, value).GetString()), uR"(b)", uR"(Render())",
                 __LINE__);

    content = uR"(<if case="0">
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
    helper.Equal(String<char16_t>::Trim(Template::Render(content, value).GetString()), uR"(c)", uR"(Render())",
                 __LINE__);

    content = uR"(<if case="0">a
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
    helper.Equal(String<char16_t>::Trim(Template::Render(content, value).GetString()), uR"(d)", uR"(Render())",
                 __LINE__);
}

static void TestIfUTag2(TestHelper &helper) {
    Value<char16_t> value;
    const char16_t *content;

    value[uR"(name)"] = uR"(Qentem)";

    content = uR"(<if case="1">{var:name})";
    helper.Equal(Template::Render(content, value), uR"(<if case="1">Qentem)", uR"(Render())", __LINE__);

    content = uR"(<if<if case="1">{var:name}</if>)";
    helper.Equal(Template::Render(content, value), uR"(<ifQentem)", uR"(Render())", __LINE__);

    content = uR"(<if case="1"><if case="1">{var:name}</if>)";
    helper.Equal(Template::Render(content, value), uR"(<if case="1">Qentem)", uR"(Render())", __LINE__);

    content = uR"(<if case="1"><if case="1"><if case="1">{var:name}</if></if>)";
    helper.Equal(Template::Render(content, value), uR"(<if case="1">Qentem)", uR"(Render())", __LINE__);

    content = uR"(<if case="ABC">{var:name}</if>)";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"(<if>{var:name}</if>)";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"(<if case="0"><elseif />{var:name}</if>)";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"(<iw case="0">{var:name}</if>)";
    helper.Equal(Template::Render(content, value), uR"(<iw case="0">Qentem</if>)", uR"(Render())", __LINE__);

    content = uR"(<if case="0"{var:name}</if>)";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"(<if case="0"><else {var:name}</if>)";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);
}

static void TestRenderU1(TestHelper &helper) {
    static constexpr unsigned int size_4 = (8 * 4);

    StringStream<char16_t> content;
    StringStream<char16_t> output;
    String<char16_t>       str;
    Value<char16_t>        value;

    for (unsigned int i = 0; i < size_4; i++) {
        value += i;
    }

    for (unsigned int i = 0; i < size_4; i++) {
        content += uR"({var:)";
        str.Reset();
        Digit::NumberToString(str, i);
        content += str;
        output += str;
        content += uR"(})";
    }

    helper.Equal(Template::Render(content.First(), content.Length(), value), output, uR"(Render())", __LINE__);

    content.Clear();
    output.Clear();

    unsigned int size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += uR"({var:)";
            str.Reset();
            Digit::NumberToString(str, i);
            content += str;
            content += uR"(})";

            output += str;
        } else {
            size += 8;

            content += uR"({math: 1 + )";
            str.Reset();
            Digit::NumberToString(str, i);
            content += str;
            content += uR"(})";

            Digit::NumberToString(output, 1U + i);
        }
    }

    helper.Equal(Template::Render(content.First(), content.Length(), value), output, uR"(Render())", __LINE__);

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += uR"({var:)";
            str.Reset();
            Digit::NumberToString(str, i);
            content += str;
            content += uR"(})";

            output += str;
        } else {
            size += 8;

            content += uR"({if case="{var:)";
            Digit::NumberToString(content, i);
            content += uR"(}<)";
            Digit::NumberToString(content, 1U + i);
            content += uR"(" true="yes"})";

            output += uR"(yes)";
        }
    }

    helper.Equal(Template::Render(content.First(), content.Length(), value), output, uR"(Render())", __LINE__);

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += uR"({var:)";
            str.Reset();
            Digit::NumberToString(str, i);
            content += str;
            content += uR"(})";

            output += str;
        } else {
            size += 8;
            content += uR"(<if case="1">A</if>)";
            output += uR"(A)";
        }
    }

    helper.Equal(Template::Render(content.First(), content.Length(), value), output, uR"(Render())", __LINE__);
}

static void TestRenderU2(TestHelper &helper) {
    Value<char16_t> value;
    const char16_t *content;

    value += 0;
    value += 1;
    value += 2;
    value += 5;
    value += 10;

    content = uR"(<loop value="loop1_val">{var:~loop1_val[0 </loop>)";
    helper.Equal(Template::Render(content, value),
                 uR"({var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 )",
                 uR"(Render())", __LINE__);

    content = uR"(<loop value="loop1_val">{var:loop1_val[]}</loop>)";
    helper.Equal(Template::Render(content, value),
                 uR"({var:loop1_val[]}{var:loop1_val[]}{var:loop1_val[]}{var:loop1_val[]}{var:loop1_val[]})",
                 uR"(Render())", __LINE__);

    content = uR"(<loop set="numbers" value="val_">{var:val_}</loop>)";
    helper.Equal(Template::Render(content, value), uR"()", uR"(Render())", __LINE__);

    content = uR"(<loop value="this_number"><if case="({var:this_number} % 2) == 1">{var:this_number},</if></loop>)";
    helper.Equal(Template::Render(content, value), uR"(1,5,)", uR"(Render())", __LINE__);

    content = uR"(<loop value="loop1_val">{if case="{var:loop1_val} < 5", true="{var:loop1_val}"}</loop>)";
    helper.Equal(Template::Render(content, value), uR"(012)", uR"(Render())", __LINE__);

    content = uR"(<loop value="loop1_val">{if case="{var:loop1_val} < 5", true="{var:4}"}</loop>)";
    helper.Equal(Template::Render(content, value), uR"(101010)", uR"(Render())", __LINE__);

    value = JSON::Parse(uR"([[[1,2,3]]])");

    content = uR"(<loop value="loop1_val">{var:loop1_val[0][2]}</loop>)";
    helper.Equal(Template::Render(content, value), uR"(3)", uR"(Render())", __LINE__);

    value = Qentem::JSON::Parse(uR"({"abc": [0,10,300], "xyz":[[1],[2],[3]]})");

    content = uR"(<loop set="xyz" value="lvar"> {var:lvar[0]} </loop>)";
    helper.Equal(Template::Render(content, value), uR"( 1  2  3 )", uR"(Render())", __LINE__);

    content = uR"(<loop set="xyz" value="lvar"><loop set="lvar" value="lvar2"> {math:{var:lvar2}+3} </loop></loop>)";
    helper.Equal(Template::Render(content, value), uR"( 4  5  6 )", uR"(Render())", __LINE__);
}

static int RunTemplateUTests() {
    TestHelper helper{"Template.hpp (16-bit char)", __FILE__};

    helper.PrintGroupName();

    helper.Test("Variable Tag Test 1", TestVariableUTag1);
    helper.Test("Variable Tag Test 2", TestVariableUTag2);
    helper.Test("Variable Tag Test 3", TestVariableUTag3);
    helper.Test("Variable Tag Test 4", TestVariableUTag4);

    helper.Test("Raw Variable Tag Test 1", TestRawVariableUTag1);
    helper.Test("Raw Variable Tag Test 2", TestRawVariableUTag2);
    helper.Test("Raw Variable Tag Test 3", TestRawVariableUTag3);
    helper.Test("Raw Variable Tag Test 4", TestRawVariableUTag4);

    helper.Test("Math Tag Test 1", TestMathUTag1);
    helper.Test("Math Tag Test 2", TestMathUTag2);

    helper.Test("Inline if Tag Test", TestInlineIfUTag);

    helper.Test("Loop Tag Test 1", TestLoopUTag1);
    helper.Test("Loop Tag Test 2", TestLoopUTag2);
    helper.Test("Loop Tag Test 3", TestLoopUTag3);

    helper.Test("If Tag Test 1", TestIfUTag1);
    helper.Test("If Tag Test 2", TestIfUTag2);

    helper.Test("Render Test 1", TestRenderU1);
    helper.Test("Render Test 2", TestRenderU2);

    return helper.EndTests();
}

} // namespace Qentem::Test

#endif
