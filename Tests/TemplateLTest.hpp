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
#include "StringStream.hpp"
#include "JSON.hpp"
#include "Template.hpp"

#ifndef QENTEM_TEMPLATE_L_TESTS_H
#define QENTEM_TEMPLATE_L_TESTS_H

namespace Qentem {
namespace Test {

static void TestVariableLTag1(QTest &helper) {
    StringStream<wchar_t> ss;
    const wchar_t        *content;

    Value<wchar_t> value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    const Value<wchar_t> sub_value =
        JSON::Parse(LR"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = LR"({var:0})";
    helper.IsEqual(Template::Render<StringStream<wchar_t>>(content, value), LR"(A)", __LINE__);
    ss.Clear();

    content = LR"({var:1})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(abc)", __LINE__);
    ss.Clear();

    content = LR"({var:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(true)", __LINE__);
    ss.Clear();

    content = LR"({var:3})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(456)", __LINE__);
    ss.Clear();

    content = LR"({var:4})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1.5)", __LINE__);
    ss.Clear();

    content = LR"({var:5[0]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(null)", __LINE__);
    ss.Clear();

    content = LR"({var:5[1]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(false)", __LINE__);
    ss.Clear();

    content = LR"({var:5[2][0]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(Qentem)", __LINE__);
    ss.Clear();

    //////

    content = LR"({var:key1})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(a)", __LINE__);
    ss.Clear();

    content = LR"({var:key2})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(ABC)", __LINE__);
    ss.Clear();

    content = LR"({var:key3})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(false)", __LINE__);
    ss.Clear();

    content = LR"({var:key4})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(100)", __LINE__);
    ss.Clear();

    content = LR"({var:key5})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(1.5)", __LINE__);
    ss.Clear();

    content = LR"({var:key6[one]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(1)", __LINE__);
    ss.Clear();

    content = LR"({var:key7[0]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(null)", __LINE__);
    ss.Clear();

    content = LR"({var:key7[1]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(false)", __LINE__);
    ss.Clear();

    content = LR"({var:key7[2][0]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(Qentem)", __LINE__);
    ss.Clear();

    //
    content = LR"({var:6[key1]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(a)", __LINE__);
    ss.Clear();

    content = LR"({var:6[key2]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(ABC)", __LINE__);
    ss.Clear();

    content = LR"({var:6[key3]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(false)", __LINE__);
    ss.Clear();

    content = LR"({var:6[key4]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(100)", __LINE__);
    ss.Clear();

    content = LR"({var:6[key5]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1.5)", __LINE__);
    ss.Clear();

    content = LR"({var:6[key6][one]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1)", __LINE__);
    ss.Clear();

    ////////////////

    content = LR"(-{var:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-true)", __LINE__);
    ss.Clear();

    content = LR"(-{var:key7[0]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(-null)", __LINE__);
    ss.Clear();

    content = LR"(-{var:key7[2][0]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(-Qentem)", __LINE__);
    ss.Clear();

    content = LR"(-{var:6[key3]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-false)", __LINE__);
    ss.Clear();

    content = LR"(-{var:6[key4]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-100)", __LINE__);
    ss.Clear();

    ////////////

    content = LR"({var:2}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(true-)", __LINE__);
    ss.Clear();

    content = LR"({var:key7[0]}-)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(null-)", __LINE__);
    ss.Clear();

    content = LR"({var:key7[2][0]}-)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(Qentem-)", __LINE__);
    ss.Clear();

    content = LR"({var:6[key3]}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(false-)", __LINE__);
    ss.Clear();

    content = LR"({var:6[key4]}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(100-)", __LINE__);
    ss.Clear();

    ////////////

    content = LR"(-{var:2}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-true-)", __LINE__);
    ss.Clear();

    content = LR"(-{var:key7[0]}-)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(-null-)", __LINE__);
    ss.Clear();

    content = LR"(-{var:key7[2][0]}-)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(-Qentem-)", __LINE__);
    ss.Clear();

    content = LR"(-{var:6[key3]}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-false-)", __LINE__);
    ss.Clear();

    content = LR"(-{var:6[key4]}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-100-)", __LINE__);
    ss.Clear();

    ////////////

    content = LR"(------{var:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(------true)", __LINE__);
    ss.Clear();

    content = LR"(------{var:key7[0]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(------null)", __LINE__);
    ss.Clear();

    content = LR"(------{var:key7[2][0]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(------Qentem)", __LINE__);
    ss.Clear();

    content = LR"(------{var:6[key3]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(------false)", __LINE__);
    ss.Clear();

    content = LR"(------{var:6[key4]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(------100)", __LINE__);
    ss.Clear();

    ////////////

    content = LR"({var:2}------)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(true------)", __LINE__);
    ss.Clear();

    content = LR"({var:key7[0]}------)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(null------)", __LINE__);
    ss.Clear();

    content = LR"({var:key7[2][0]}------)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(Qentem------)", __LINE__);
    ss.Clear();

    content = LR"({var:6[key3]}------)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(false------)", __LINE__);
    ss.Clear();

    content = LR"({var:6[key4]}------)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(100------)", __LINE__);
    ss.Clear();

    ////////////

    content = LR"(------{var:2}------)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(------true------)", __LINE__);
    ss.Clear();

    content = LR"(------{var:key7[0]}------)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(------null------)", __LINE__);
    ss.Clear();

    content = LR"(------{var:key7[2][0]}------)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(------Qentem------)", __LINE__);
    ss.Clear();

    content = LR"(------{var:6[key3]}------)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(------false------)", __LINE__);
    ss.Clear();

    content = LR"(------{var:6[key4]}------)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(------100------)", __LINE__);
    ss.Clear();

    content = LR"({var:key7[2[0]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:key7[2[0]})", __LINE__);
    ss.Clear();

    content = LR"({var:6key3]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:6key3]})", __LINE__);
    ss.Clear();
}

static void TestVariableLTag2(QTest &helper) {
    StringStream<wchar_t> ss;
    const Value<wchar_t>  value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5])");
    const wchar_t        *content;

    content = LR"({var:0}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(AA)", __LINE__);
    ss.Clear();

    content = LR"({var:1}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(abcA)", __LINE__);
    ss.Clear();

    content = LR"({var:1}{var:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(abctrue)", __LINE__);
    ss.Clear();

    content = LR"({var:2}{var:3}{var:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(true456true)", __LINE__);
    ss.Clear();

    content = LR"({var:4}{var:4}{var:4})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1.51.51.5)", __LINE__);
    ss.Clear();

    ///

    content = LR"({var:0}-{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(A-A)", __LINE__);
    ss.Clear();

    content = LR"({var:1}--{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(abc--A)", __LINE__);
    ss.Clear();

    content = LR"({var:1}---{var:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(abc---true)", __LINE__);
    ss.Clear();

    content = LR"({var:2}{var:3}--{var:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(true456--true)", __LINE__);
    ss.Clear();

    content = LR"({var:4}--{var:4}{var:4})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1.5--1.51.5)", __LINE__);
    ss.Clear();

    content = LR"({var:4}--{var:4}--{var:4})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1.5--1.5--1.5)", __LINE__);
    ss.Clear();

    content = LR"({var:4}---{var:4}---{var:4})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1.5---1.5---1.5)", __LINE__);
    ss.Clear();
}

static void TestVariableLTag3(QTest &helper) {
    StringStream<wchar_t> ss;
    Value<wchar_t>        value;
    const wchar_t        *content;

    content = LR"({var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:0})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({var:a})", value, ss), LR"({var:a})", __LINE__);
    ss.Clear();

    content = LR"({var:0[0]})";
    helper.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), LR"({var:0[0]})", __LINE__);
    ss.Clear();

    content = LR"({var:a[0]})";
    helper.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), LR"({var:a[0]})", __LINE__);
    ss.Clear();

    content = LR"({var:0[a]})";
    helper.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), LR"({var:0[a]})", __LINE__);
    ss.Clear();

    content = LR"({var:a[abc]})";
    helper.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), LR"({var:a[abc]})", __LINE__);
    ss.Clear();

    ////////////////

    value = JSON::Parse(LR"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = LR"({var:0})";
    helper.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), LR"({var:0})", __LINE__);
    ss.Clear();

    content = LR"({var:0[0]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:0[0]})", __LINE__);
    ss.Clear();

    content = LR"({var:0[0][0]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:0[0][0]})", __LINE__);
    ss.Clear();

    /////

    content = LR"({var:1})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:1})", __LINE__);
    ss.Clear();

    content = LR"({var:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:2})", __LINE__);
    ss.Clear();

    content = LR"({var:1[a]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:1[a]})", __LINE__);
    ss.Clear();

    content = LR"({var:1[b]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:1[b]})", __LINE__);
    ss.Clear();

    ////

    value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5])");

    content = LR"({var:0)";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:0)", __LINE__);
    ss.Clear();

    content = LR"(var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(var:0})", __LINE__);
    ss.Clear();

    content = LR"({v})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({v})", __LINE__);
    ss.Clear();

    content = LR"({va})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({va})", __LINE__);
    ss.Clear();

    content = LR"({var})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var})", __LINE__);
    ss.Clear();

    content = LR"({var:})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:})", __LINE__);
    ss.Clear();

    content = LR"({v:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({v:0})", __LINE__);
    ss.Clear();

    content = LR"({va:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({va:0})", __LINE__);
    ss.Clear();

    ////

    content = LR"({var:0{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:0{var:0})", __LINE__);
    ss.Clear();

    content = LR"(var:0{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(var:0A)", __LINE__);
    ss.Clear();

    content = LR"(var:0}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(var:0}A)", __LINE__);
    ss.Clear();

    content = LR"({var:0{var:0}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:0{var:0}A)", __LINE__);
    ss.Clear();

    ////

    content = LR"({var:0{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:0{var:0})", __LINE__);
    ss.Clear();

    content = LR"(var:0{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(var:0A)", __LINE__);
    ss.Clear();

    content = LR"(var:0}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(var:0}A)", __LINE__);
    ss.Clear();

    content = LR"({var:0{var:0}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:0{var:0}A)", __LINE__);
    ss.Clear();

    ////

    content = LR"({{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({A)", __LINE__);
    ss.Clear();

    content = LR"({{var:0}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({AA)", __LINE__);
    ss.Clear();

    content = LR"({v{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({vA)", __LINE__);
    ss.Clear();

    content = LR"({v{var:0}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({vAA)", __LINE__);
    ss.Clear();

    content = LR"({va{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({vaA)", __LINE__);
    ss.Clear();

    content = LR"({va{var:0}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({vaAA)", __LINE__);
    ss.Clear();

    content = LR"({var{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({varA)", __LINE__);
    ss.Clear();

    content = LR"({var{var:0}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({varAA)", __LINE__);
    ss.Clear();

    ///

    content = LR"({-{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({-A)", __LINE__);
    ss.Clear();

    content = LR"({-{var:0}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({-AA)", __LINE__);
    ss.Clear();

    content = LR"({v-{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({v-A)", __LINE__);
    ss.Clear();

    content = LR"({v-{var:0}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({v-AA)", __LINE__);
    ss.Clear();

    content = LR"({va-{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({va-A)", __LINE__);
    ss.Clear();

    content = LR"({va-{var:0}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({va-AA)", __LINE__);
    ss.Clear();

    content = LR"({var-{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var-A)", __LINE__);
    ss.Clear();

    content = LR"({var-{var:0}{var:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var-AA)", __LINE__);
    ss.Clear();

    //

    content = LR"({var-0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var-0})", __LINE__);
    ss.Clear();

    content = LR"({var 0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var 0})", __LINE__);
    ss.Clear();

    content = LR"({var:0 })";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:0 })", __LINE__);
    ss.Clear();

    content = LR"({var:0 )";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:0 )", __LINE__);
    ss.Clear();

    content = LR"({var:0)";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:0)", __LINE__);
    ss.Clear();

    content = LR"( {var-0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"( {var-0})", __LINE__);
    ss.Clear();

    content = LR"( {var 0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"( {var 0})", __LINE__);
    ss.Clear();

    content = LR"( {var:0 })";
    helper.IsEqual(Template::Render(content, value, ss), LR"( {var:0 })", __LINE__);
    ss.Clear();

    content = LR"( {var:0 )";
    helper.IsEqual(Template::Render(content, value, ss), LR"( {var:0 )", __LINE__);
    ss.Clear();

    content = LR"( {var:0)";
    helper.IsEqual(Template::Render(content, value, ss), LR"( {var:0)", __LINE__);
    ss.Clear();
}

static void TestVariableLTag4(QTest &helper) {
    StringStream<wchar_t> ss;
    Value<wchar_t>        value;

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

    if (Config::AutoEscapeHTML) {
        helper.IsEqual(Template::Render(LR"({var:0})", value, ss), LR"(&lt;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:1})", value, ss), LR"(&gt;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:2})", value, ss), LR"(&amp;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:3})", value, ss), LR"(&quot;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:4})", value, ss), LR"(&apos;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:5})", value, ss), LR"(&lt;&gt;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:6})", value, ss), LR"(&lt;&amp;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:7})", value, ss), LR"(&lt;&amp;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:8})", value, ss), LR"(&gt;&quot;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:9})", value, ss), LR"(&quot;&apos;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:10})", value, ss), LR"(&lt;&quot;&gt;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:11})", value, ss), LR"(&lt;&apos;&gt;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:12})", value, ss), LR"(&lt;&amp;&gt;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:13})", value, ss), LR"(&amp;&quot;&amp;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:14})", value, ss), LR"(&quot;&apos;&quot;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:15})", value, ss), LR"(&apos;&lt;&apos;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:16})", value, ss), LR"(&apos;&amp;&apos;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:17})", value, ss), LR"(&lt;&gt;&amp;&apos;&quot;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:18})", value, ss), LR"(&apos;&quot;&lt;&gt;&amp;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:19})", value, ss), LR"(&lt;&quot;&amp;&apos;&gt;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:20})", value, ss), LR"(&lt;&lt;&lt;&lt;&lt;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:21})", value, ss), LR"(&gt;&gt;&gt;&gt;&gt;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:22})", value, ss), LR"(&amp;&amp;&amp;&amp;&amp;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:23})", value, ss), LR"(&quot;&quot;&quot;&quot;&quot;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:24})", value, ss), LR"(&apos;&apos;&apos;&apos;&apos;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:25})", value, ss), LR"(A&lt;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:26})", value, ss), LR"(A&gt;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:27})", value, ss), LR"(A&amp;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:28})", value, ss), LR"(A&quot;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:29})", value, ss), LR"(A&apos;)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:30})", value, ss), LR"(&lt;A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:31})", value, ss), LR"(&gt;A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:32})", value, ss), LR"(&amp;A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:33})", value, ss), LR"(&quot;A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:34})", value, ss), LR"(&apos;A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:35})", value, ss), LR"(A&lt;A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:36})", value, ss), LR"(A&gt;A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:37})", value, ss), LR"(A&amp;A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:38})", value, ss), LR"(A&quot;A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:39})", value, ss), LR"(A&apos;A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:40})", value, ss), LR"(AA&lt;AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:41})", value, ss), LR"(AA&gt;AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:42})", value, ss), LR"(AA&amp;AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:43})", value, ss), LR"(AA&quot;AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:44})", value, ss), LR"(AA&apos;AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:45})", value, ss), LR"(AA&lt;&lt;&lt;&lt;AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:46})", value, ss), LR"(AA&gt;&gt;&gt;&gt;AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:47})", value, ss), LR"(AA&amp;&amp;&amp;&amp;AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:48})", value, ss), LR"(AA&quot;&quot;&quot;&quot;AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:49})", value, ss), LR"(AA&apos;&apos;&apos;&apos;AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:50})", value, ss),
                       LR"(&lt;A&gt;B&apos;C&quot;D&amp;E&apos;F&quot;G&lt;H&gt;I&amp;G&quot;K)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:51})", value, ss), LR"(AB&quot;CD&apos;EF&lt;GH&gt;IGK&apos;)",
                       __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:52})", value, ss), LR"(&quot;ABC&apos;DEF&lt;GHI&gt;GK&lt;)",
                       __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:53})", value, ss),
                       LR"(A&quot;&quot;BC&lt;&lt;DE&gt;&gt;FG&apos;&apos;HI&amp;&amp;GK)", __LINE__);
        ss.Clear();

    } else {
        helper.IsEqual(Template::Render(LR"({var:0})", value, ss), LR"(<)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:1})", value, ss), LR"(>)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:2})", value, ss), LR"(&)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:3})", value, ss), LR"(")", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:4})", value, ss), LR"(')", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:5})", value, ss), LR"(<>)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:6})", value, ss), LR"(<&)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:7})", value, ss), LR"(<&)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:8})", value, ss), LR"(>")", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:9})", value, ss), LR"("')", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:10})", value, ss), LR"(<">)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:11})", value, ss), LR"(<'>)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:12})", value, ss), LR"(<&>)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:13})", value, ss), LR"(&"&)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:14})", value, ss), LR"("'")", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:15})", value, ss), LR"('<')", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:16})", value, ss), LR"('&')", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:17})", value, ss), LR"(<>&'")", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:18})", value, ss), LR"('"<>&)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:19})", value, ss), LR"(<"&'>)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:20})", value, ss), LR"(<<<<<)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:21})", value, ss), LR"(>>>>>)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:22})", value, ss), LR"(&&&&&)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:23})", value, ss), LR"(""""")", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:24})", value, ss), LR"(''''')", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:25})", value, ss), LR"(A<)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:26})", value, ss), LR"(A>)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:27})", value, ss), LR"(A&)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:28})", value, ss), LR"(A")", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:29})", value, ss), LR"(A')", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:30})", value, ss), LR"(<A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:31})", value, ss), LR"(>A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:32})", value, ss), LR"(&A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:33})", value, ss), LR"("A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:34})", value, ss), LR"('A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:35})", value, ss), LR"(A<A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:36})", value, ss), LR"(A>A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:37})", value, ss), LR"(A&A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:38})", value, ss), LR"(A"A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:39})", value, ss), LR"(A'A)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:40})", value, ss), LR"(AA<AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:41})", value, ss), LR"(AA>AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:42})", value, ss), LR"(AA&AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:43})", value, ss), LR"(AA"AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:44})", value, ss), LR"(AA'AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:45})", value, ss), LR"(AA<<<<AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:46})", value, ss), LR"(AA>>>>AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:47})", value, ss), LR"(AA&&&&AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:48})", value, ss), LR"(AA""""AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:49})", value, ss), LR"(AA''''AA)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:50})", value, ss), LR"(<A>B'C"D&E'F"G<H>I&G"K)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:51})", value, ss), LR"(AB"CD'EF<GH>IGK')", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:52})", value, ss), LR"("ABC'DEF<GHI>GK<)", __LINE__);
        ss.Clear();

        helper.IsEqual(Template::Render(LR"({var:53})", value, ss), LR"(A""BC<<DE>>FG''HI&&GK)", __LINE__);
        ss.Clear();
    }
}

static void TestRawVariableLTag1(QTest &helper) {
    StringStream<wchar_t> ss;
    const wchar_t        *content;

    Value<wchar_t> value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    const Value<wchar_t> sub_value =
        JSON::Parse(LR"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = LR"({raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(A)", __LINE__);
    ss.Clear();

    content = LR"({raw:1})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(abc)", __LINE__);
    ss.Clear();

    content = LR"({raw:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(true)", __LINE__);
    ss.Clear();

    content = LR"({raw:3})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(456)", __LINE__);
    ss.Clear();

    content = LR"({raw:4})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1.5)", __LINE__);
    ss.Clear();

    content = LR"({raw:5[0]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(null)", __LINE__);
    ss.Clear();

    content = LR"({raw:5[1]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(false)", __LINE__);
    ss.Clear();

    content = LR"({raw:5[2][0]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(Qentem)", __LINE__);
    ss.Clear();

    //////

    content = LR"({raw:key1})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(a)", __LINE__);
    ss.Clear();

    content = LR"({raw:key2})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(ABC)", __LINE__);
    ss.Clear();

    content = LR"({raw:key3})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(false)", __LINE__);
    ss.Clear();

    content = LR"({raw:key4})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(100)", __LINE__);
    ss.Clear();

    content = LR"({raw:key5})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(1.5)", __LINE__);
    ss.Clear();

    content = LR"({raw:key6[one]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(1)", __LINE__);
    ss.Clear();

    content = LR"({raw:key7[0]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(null)", __LINE__);
    ss.Clear();

    content = LR"({raw:key7[1]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(false)", __LINE__);
    ss.Clear();

    content = LR"({raw:key7[2][0]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(Qentem)", __LINE__);
    ss.Clear();

    //
    content = LR"({raw:6[key1]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(a)", __LINE__);
    ss.Clear();

    content = LR"({raw:6[key2]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(ABC)", __LINE__);
    ss.Clear();

    content = LR"({raw:6[key3]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(false)", __LINE__);
    ss.Clear();

    content = LR"({raw:6[key4]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(100)", __LINE__);
    ss.Clear();

    content = LR"({raw:6[key5]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1.5)", __LINE__);
    ss.Clear();

    content = LR"({raw:6[key6][one]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1)", __LINE__);
    ss.Clear();

    ////////////////

    content = LR"(-{raw:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-true)", __LINE__);
    ss.Clear();

    content = LR"(-{raw:key7[0]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(-null)", __LINE__);
    ss.Clear();

    content = LR"(-{raw:key7[2][0]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(-Qentem)", __LINE__);
    ss.Clear();

    content = LR"(-{raw:6[key3]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-false)", __LINE__);
    ss.Clear();

    content = LR"(-{raw:6[key4]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-100)", __LINE__);
    ss.Clear();

    ////////////

    content = LR"({raw:2}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(true-)", __LINE__);
    ss.Clear();

    content = LR"({raw:key7[0]}-)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(null-)", __LINE__);
    ss.Clear();

    content = LR"({raw:key7[2][0]}-)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(Qentem-)", __LINE__);
    ss.Clear();

    content = LR"({raw:6[key3]}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(false-)", __LINE__);
    ss.Clear();

    content = LR"({raw:6[key4]}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(100-)", __LINE__);
    ss.Clear();

    ////////////

    content = LR"(-{raw:2}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-true-)", __LINE__);
    ss.Clear();

    content = LR"(-{raw:key7[0]}-)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(-null-)", __LINE__);
    ss.Clear();

    content = LR"(-{raw:key7[2][0]}-)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(-Qentem-)", __LINE__);
    ss.Clear();

    content = LR"(-{raw:6[key3]}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-false-)", __LINE__);
    ss.Clear();

    content = LR"(-{raw:6[key4]}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-100-)", __LINE__);
    ss.Clear();

    ////////////

    content = LR"(------{raw:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(------true)", __LINE__);
    ss.Clear();

    content = LR"(------{raw:key7[0]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(------null)", __LINE__);
    ss.Clear();

    content = LR"(------{raw:key7[2][0]})";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(------Qentem)", __LINE__);
    ss.Clear();

    content = LR"(------{raw:6[key3]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(------false)", __LINE__);
    ss.Clear();

    content = LR"(------{raw:6[key4]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(------100)", __LINE__);
    ss.Clear();

    ////////////

    content = LR"({raw:2}------)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(true------)", __LINE__);
    ss.Clear();

    content = LR"({raw:key7[0]}------)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(null------)", __LINE__);
    ss.Clear();

    content = LR"({raw:key7[2][0]}------)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(Qentem------)", __LINE__);
    ss.Clear();

    content = LR"({raw:6[key3]}------)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(false------)", __LINE__);
    ss.Clear();

    content = LR"({raw:6[key4]}------)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(100------)", __LINE__);
    ss.Clear();

    ////////////

    content = LR"(------{raw:2}------)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(------true------)", __LINE__);
    ss.Clear();

    content = LR"(------{raw:key7[0]}------)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(------null------)", __LINE__);
    ss.Clear();

    content = LR"(------{raw:key7[2][0]}------)";
    helper.IsEqual(Template::Render(content, sub_value, ss), LR"(------Qentem------)", __LINE__);
    ss.Clear();

    content = LR"(------{raw:6[key3]}------)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(------false------)", __LINE__);
    ss.Clear();

    content = LR"(------{raw:6[key4]}------)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(------100------)", __LINE__);
    ss.Clear();

    content = LR"({raw:key7[2[0]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:key7[2[0]})", __LINE__);
    ss.Clear();

    content = LR"({raw:6key3]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:6key3]})", __LINE__);
    ss.Clear();
}

static void TestRawVariableLTag2(QTest &helper) {
    StringStream<wchar_t> ss;
    const Value<wchar_t>  value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5])");
    const wchar_t        *content;

    content = LR"({raw:0}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(AA)", __LINE__);
    ss.Clear();

    content = LR"({raw:1}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(abcA)", __LINE__);
    ss.Clear();

    content = LR"({raw:1}{raw:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(abctrue)", __LINE__);
    ss.Clear();

    content = LR"({raw:2}{raw:3}{raw:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(true456true)", __LINE__);
    ss.Clear();

    content = LR"({raw:4}{raw:4}{raw:4})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1.51.51.5)", __LINE__);
    ss.Clear();

    ///

    content = LR"({raw:0}-{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(A-A)", __LINE__);
    ss.Clear();

    content = LR"({raw:1}--{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(abc--A)", __LINE__);
    ss.Clear();

    content = LR"({raw:1}---{raw:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(abc---true)", __LINE__);
    ss.Clear();

    content = LR"({raw:2}{raw:3}--{raw:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(true456--true)", __LINE__);
    ss.Clear();

    content = LR"({raw:4}--{raw:4}{raw:4})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1.5--1.51.5)", __LINE__);
    ss.Clear();

    content = LR"({raw:4}--{raw:4}--{raw:4})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1.5--1.5--1.5)", __LINE__);
    ss.Clear();

    content = LR"({raw:4}---{raw:4}---{raw:4})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1.5---1.5---1.5)", __LINE__);
    ss.Clear();
}

static void TestRawVariableLTag3(QTest &helper) {
    StringStream<wchar_t> ss;
    Value<wchar_t>        value;
    const wchar_t        *content;

    content = LR"({raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:0})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:a})", value, ss), LR"({raw:a})", __LINE__);
    ss.Clear();

    content = LR"({raw:0[0]})";
    helper.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), LR"({raw:0[0]})", __LINE__);
    ss.Clear();

    content = LR"({raw:a[0]})";
    helper.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), LR"({raw:a[0]})", __LINE__);
    ss.Clear();

    content = LR"({raw:0[a]})";
    helper.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), LR"({raw:0[a]})", __LINE__);
    ss.Clear();

    content = LR"({raw:a[abc]})";
    helper.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), LR"({raw:a[abc]})", __LINE__);
    ss.Clear();

    ////////////////

    value = JSON::Parse(LR"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = LR"({raw:0})";
    helper.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), LR"({raw:0})", __LINE__);
    ss.Clear();

    content = LR"({raw:0[0]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:0[0]})", __LINE__);
    ss.Clear();

    content = LR"({raw:0[0][0]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:0[0][0]})", __LINE__);
    ss.Clear();

    /////

    content = LR"({raw:1})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:1})", __LINE__);
    ss.Clear();

    content = LR"({raw:2})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:2})", __LINE__);
    ss.Clear();

    content = LR"({raw:1[a]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:1[a]})", __LINE__);
    ss.Clear();

    content = LR"({raw:1[b]})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:1[b]})", __LINE__);
    ss.Clear();

    ////

    value = JSON::Parse(LR"(["A", "abc", true, 456, 1.5])");

    content = LR"({raw:0)";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:0)", __LINE__);
    ss.Clear();

    content = LR"(raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(raw:0})", __LINE__);
    ss.Clear();

    content = LR"({r})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({r})", __LINE__);
    ss.Clear();

    content = LR"({ra})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({ra})", __LINE__);
    ss.Clear();

    content = LR"({raw})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw})", __LINE__);
    ss.Clear();

    content = LR"({raw:})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:})", __LINE__);
    ss.Clear();

    content = LR"({r:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({r:0})", __LINE__);
    ss.Clear();

    content = LR"({ra:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({ra:0})", __LINE__);
    ss.Clear();

    ////

    content = LR"({raw:0{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:0{raw:0})", __LINE__);
    ss.Clear();

    content = LR"(raw:0{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(raw:0A)", __LINE__);
    ss.Clear();

    content = LR"(raw:0}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(raw:0}A)", __LINE__);
    ss.Clear();

    content = LR"({raw:0{raw:0}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:0{raw:0}A)", __LINE__);
    ss.Clear();

    ////

    content = LR"({raw:0{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:0{raw:0})", __LINE__);
    ss.Clear();

    content = LR"(raw:0{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(raw:0A)", __LINE__);
    ss.Clear();

    content = LR"(raw:0}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(raw:0}A)", __LINE__);
    ss.Clear();

    content = LR"({raw:0{raw:0}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:0{raw:0}A)", __LINE__);
    ss.Clear();

    ////

    content = LR"({{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({A)", __LINE__);
    ss.Clear();

    content = LR"({{raw:0}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({AA)", __LINE__);
    ss.Clear();

    content = LR"({r{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({rA)", __LINE__);
    ss.Clear();

    content = LR"({r{raw:0}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({rAA)", __LINE__);
    ss.Clear();

    content = LR"({ra{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raA)", __LINE__);
    ss.Clear();

    content = LR"({ra{raw:0}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raAA)", __LINE__);
    ss.Clear();

    content = LR"({raw{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({rawA)", __LINE__);
    ss.Clear();

    content = LR"({raw{raw:0}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({rawAA)", __LINE__);
    ss.Clear();

    ///

    content = LR"({-{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({-A)", __LINE__);
    ss.Clear();

    content = LR"({-{raw:0}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({-AA)", __LINE__);
    ss.Clear();

    content = LR"({r-{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({r-A)", __LINE__);
    ss.Clear();

    content = LR"({r-{raw:0}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({r-AA)", __LINE__);
    ss.Clear();

    content = LR"({ra-{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({ra-A)", __LINE__);
    ss.Clear();

    content = LR"({ra-{raw:0}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({ra-AA)", __LINE__);
    ss.Clear();

    content = LR"({raw-{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw-A)", __LINE__);
    ss.Clear();

    content = LR"({raw-{raw:0}{raw:0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw-AA)", __LINE__);
    ss.Clear();

    //

    content = LR"({raw-0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw-0})", __LINE__);
    ss.Clear();

    content = LR"({raw 0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw 0})", __LINE__);
    ss.Clear();

    content = LR"({raw:0 })";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:0 })", __LINE__);
    ss.Clear();

    content = LR"({raw:0 )";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:0 )", __LINE__);
    ss.Clear();

    content = LR"({raw:0)";
    helper.IsEqual(Template::Render(content, value, ss), LR"({raw:0)", __LINE__);
    ss.Clear();

    content = LR"({ raw-0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({ raw-0})", __LINE__);
    ss.Clear();

    content = LR"({ raw 0})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({ raw 0})", __LINE__);
    ss.Clear();

    content = LR"({ raw:0 })";
    helper.IsEqual(Template::Render(content, value, ss), LR"({ raw:0 })", __LINE__);
    ss.Clear();

    content = LR"({ raw:0 )";
    helper.IsEqual(Template::Render(content, value, ss), LR"({ raw:0 )", __LINE__);
    ss.Clear();

    content = LR"({ raw:0)";
    helper.IsEqual(Template::Render(content, value, ss), LR"({ raw:0)", __LINE__);
    ss.Clear();
}

static void TestRawVariableLTag4(QTest &helper) {
    StringStream<wchar_t> ss;
    Value<wchar_t>        value;

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

    helper.IsEqual(Template::Render(LR"({raw:0})", value, ss), LR"(<)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:1})", value, ss), LR"(>)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:2})", value, ss), LR"(&)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:3})", value, ss), LR"(")", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:4})", value, ss), LR"(')", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:5})", value, ss), LR"(<>)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:6})", value, ss), LR"(<&)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:7})", value, ss), LR"(<&)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:8})", value, ss), LR"(>")", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:9})", value, ss), LR"("')", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:10})", value, ss), LR"(<">)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:11})", value, ss), LR"(<'>)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:12})", value, ss), LR"(<&>)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:13})", value, ss), LR"(&"&)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:14})", value, ss), LR"("'")", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:15})", value, ss), LR"('<')", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:16})", value, ss), LR"('&')", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:17})", value, ss), LR"(<>&'")", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:18})", value, ss), LR"('"<>&)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:19})", value, ss), LR"(<"&'>)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:20})", value, ss), LR"(<<<<<)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:21})", value, ss), LR"(>>>>>)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:22})", value, ss), LR"(&&&&&)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:23})", value, ss), LR"(""""")", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:24})", value, ss), LR"(''''')", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:25})", value, ss), LR"(A<)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:26})", value, ss), LR"(A>)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:27})", value, ss), LR"(A&)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:28})", value, ss), LR"(A")", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:29})", value, ss), LR"(A')", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:30})", value, ss), LR"(<A)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:31})", value, ss), LR"(>A)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:32})", value, ss), LR"(&A)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:33})", value, ss), LR"("A)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:34})", value, ss), LR"('A)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:35})", value, ss), LR"(A<A)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:36})", value, ss), LR"(A>A)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:37})", value, ss), LR"(A&A)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:38})", value, ss), LR"(A"A)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:39})", value, ss), LR"(A'A)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:40})", value, ss), LR"(AA<AA)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:41})", value, ss), LR"(AA>AA)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:42})", value, ss), LR"(AA&AA)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:43})", value, ss), LR"(AA"AA)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:44})", value, ss), LR"(AA'AA)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:45})", value, ss), LR"(AA<<<<AA)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:46})", value, ss), LR"(AA>>>>AA)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:47})", value, ss), LR"(AA&&&&AA)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:48})", value, ss), LR"(AA""""AA)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:49})", value, ss), LR"(AA''''AA)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:50})", value, ss), LR"(<A>B'C"D&E'F"G<H>I&G"K)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:51})", value, ss), LR"(AB"CD'EF<GH>IGK')", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:52})", value, ss), LR"("ABC'DEF<GHI>GK<)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({raw:53})", value, ss), LR"(A""BC<<DE>>FG''HI&&GK)", __LINE__);
    ss.Clear();
}

static void TestMathLTag1(QTest &helper) {
    StringStream<wchar_t> ss;
    Value<wchar_t>        value;

    value[LR"(a1)"] = 5;
    value[LR"(a2)"] = true;
    value[LR"(a3)"] = nullptr;
    value[LR"(a4)"] = false;
    value[LR"(a5)"] = LR"(10)";
    value[LR"(a6)"] = LR"(20)";
    value[LR"(a7)"] = 6;
    value[LR"(a8)"] = 1;
    value[LR"(a9)"] = LR"(1)";

    helper.IsEqual(Template::Render(LR"({math:1+1})", value, ss), LR"(2)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a1}+8})", value, ss), LR"(13)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a7}+8})", value, ss), LR"(14)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a7}+{var:a1}})", value, ss), LR"(11)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a5}+{var:a1}})", value, ss), LR"(15)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a1}+{var:a5}})", value, ss), LR"(15)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a6}+{var:a5}})", value, ss), LR"(30)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a6}*{var:a2}})", value, ss), LR"(20)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a6}*{var:a4}})", value, ss), LR"(0)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a6}*{var:a7}})", value, ss), LR"(120)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a7}+{var:a6}})", value, ss), LR"(26)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a1}+{var:a2}})", value, ss), LR"(6)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a8}=={var:a2}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a2}=={var:a8}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a5}!={var:a1}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a2}!={var:a4}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a2}==true})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a9}=={var:a8}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a8}=={var:a9}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:1=={var:a8}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:6-5==({var:a9})})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:6-5==({var:a8})})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:null!={var:a3}})", value, ss), LR"(0)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:({var:a3})==(0)})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a1}+{var:a7}})", value, ss), LR"(11)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a1}+{var:a7}}{math:{var:a1}+{var:a7}})", value, ss), LR"(1111)",
                   __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a1}+{var:a7}}*{math:{var:a1}+{var:a7}})", value, ss), LR"(11*11)",
                   __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a1}+{var:a7}}##{math:{var:a1}+{var:a7}})", value, ss), LR"(11##11)",
                   __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a1}+{var:a7}}&&&%%^^&&*{math:{var:a1}+{var:a7}})", value, ss),
                   LR"(11&&&%%^^&&*11)", __LINE__);
    ss.Clear();

    ///////////////////

    helper.IsEqual(Template::Render(LR"({math: {var:a1}+8})", value, ss), LR"(13)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:  {var:a7}+8})", value, ss), LR"(14)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:     {var:a7}+{var:a1}})", value, ss), LR"(11)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a5} +{var:a1}})", value, ss), LR"(15)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a1}  +{var:a5}})", value, ss), LR"(15)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a6}    +{var:a5}})", value, ss), LR"(30)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a6}* {var:a2}})", value, ss), LR"(20)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a6}*  {var:a4}})", value, ss), LR"(0)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a6}*      {var:a7}})", value, ss), LR"(120)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a7}+{var:a6} })", value, ss), LR"(26)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a1}+{var:a2}  })", value, ss), LR"(6)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a8}=={var:a2}      })", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a2}=={var:a8}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math: {var:a5}!={var:a1} })", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:  {var:a2}!={var:a4}  })", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:    1=={var:a9}     })", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a9} == {var:a8}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a8}  ==  {var:a9}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:1==          {var:a8}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:6-5         ==1})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:1          ==            {var:a8}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:     0     !=    ({var:a3})        })", value, ss), LR"(0)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:a3}       ==       null     })", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:((8+1+{var:a8}))})", value, ss), LR"(10)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:10==(8+1+{var:a8})})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:((8+1+{var:a8}))==9+1})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:(5*2)==((8+1+{var:a8}))})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

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

    helper.IsEqual(Template::Render(LR"({math:{var:0}+8})", value, ss), LR"(13)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:6}+8})", value, ss), LR"(14)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:6}+{var:0}})", value, ss), LR"(11)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:4}+{var:0}})", value, ss), LR"(15)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:0}+{var:4}})", value, ss), LR"(15)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:5}+{var:4}})", value, ss), LR"(30)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:5}*{var:1}})", value, ss), LR"(20)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:5}*{var:3}})", value, ss), LR"(0)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:5}*{var:6}})", value, ss), LR"(120)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:6}+{var:5}})", value, ss), LR"(26)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:0}+{var:1}})", value, ss), LR"(6)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:7}=={var:1}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:1}=={var:7}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:4}!={var:0}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:1}!={var:3}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:({var:1})==({var:8})})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"(-{math:{var:8}=={var:7}})", value, ss), LR"(-1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"(--{math:{var:7}=={var:8}})", value, ss), LR"(--1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"(---{math:1=={var:7}})", value, ss), LR"(---1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:1==({var:8})}-)", value, ss), LR"(1-)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:6-5==({var:7})}--)", value, ss), LR"(1--)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:0==({var:2})}---)", value, ss), LR"(1---)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"(-{math:{var:2}!=null}-)", value, ss), LR"(-0-)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"(--{math:Qente=={var:9}}--)", value, ss), LR"(--0--)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"(---{math:Qente !={var:9}}---)", value, ss), LR"(---1---)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:  Qentem   =={var:9}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:  Qentem!={var:9}})", value, ss), LR"(0)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:9}   ==    Qente})", value, ss), LR"(0)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:9} !=    Qente    })", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:  {var:9}   ==Qentem})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math: {var:9} !=Qentem})", value, ss), LR"(0)", __LINE__);
    ss.Clear();

    /////////

    helper.IsEqual(Template::Render(LR"({math: true == {var:1}})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math: {var:1} == true})", value, ss), LR"(1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math: 8 /2})", value, ss), LR"(4)", __LINE__);
}

static void TestMathLTag2(QTest &helper) {
    StringStream<wchar_t> ss;
    Value<wchar_t>        value;

    value += Array<Value<wchar_t>>();
    value += HArray<Value<wchar_t>, wchar_t>();
    value += 5;

    helper.IsEqual(Template::Render(LR"({math:{var:0}+8})", value, ss), LR"({math:{var:0}+8})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:1}+8})", value, ss), LR"({math:{var:1}+8})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:2}+{var:0}})", value, ss), LR"({math:{var:2}+{var:0}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:2}+{var:0}})", value, ss), LR"({math:{var:2}+{var:0}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:0}+{var:1}})", value, ss), LR"({math:{var:0}+{var:1}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:1}+{var:2}})", value, ss), LR"({math:{var:1}+{var:2}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:2}*{var:1}})", value, ss), LR"({math:{var:2}*{var:1}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:0}*{var:1}})", value, ss), LR"({math:{var:0}*{var:1}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:0}*{var:2}})", value, ss), LR"({math:{var:0}*{var:2}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:2}+{var:0}})", value, ss), LR"({math:{var:2}+{var:0}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:1}+{var:2}})", value, ss), LR"({math:{var:1}+{var:2}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:0}=={var:1}})", value, ss), LR"({math:{var:0}=={var:1}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:1}=={var:0}})", value, ss), LR"({math:{var:1}=={var:0}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:0}!={var:2}})", value, ss), LR"({math:{var:0}!={var:2}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:2}!={var:0}})", value, ss), LR"({math:{var:2}!={var:0}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:1}=={var:2}})", value, ss), LR"({math:{var:1}=={var:2}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:8}=={var:7}})", value, ss), LR"({math:{var:8}=={var:7}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:7}=={var:2}})", value, ss), LR"({math:{var:7}=={var:2}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:1=={var:7}})", value, ss), LR"({math:1=={var:7}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:(6-5)=={var:8}})", value, ss), LR"({math:(6-5)=={var:8}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:(6-5)=={var:0}})", value, ss), LR"({math:(6-5)=={var:0}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:0}=={var:8}})", value, ss), LR"({math:{var:0}=={var:8}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:0}=={var:0}})", value, ss), LR"({math:{var:0}=={var:0}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:0=={var:1}})", value, ss), LR"({math:0=={var:1}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:1}!=0})", value, ss), LR"({math:{var:1}!=0})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:W={var:0}})", value, ss), LR"({math:W={var:0}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:W=={var:0}})", value, ss), LR"({math:W=={var:0}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:2}==c})", value, ss), LR"({math:{var:2}==c})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:abc=={var:2}})", value, ss), LR"({math:abc=={var:2}})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:sds})", value, ss), LR"({math:sds})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:1)", value, ss), LR"({math:1)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"(math:1})", value, ss), LR"(math:1})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:{var:2})", value, ss), LR"({math:5)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({{math:{var:2}+5})", value, ss), LR"({10)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({m{var:2}})", value, ss), LR"({m5})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({ma{var:2}})", value, ss), LR"({ma5})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({mat{var:2}})", value, ss), LR"({mat5})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math{var:2}})", value, ss), LR"({math5})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math {var:2}})", value, ss), LR"({math 5})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:})", value, ss), LR"({math:})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math-{var:2}}{math:{var:2}+5})", value, ss), LR"({math-5}10)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math-4}{math:{var:2}+5})", value, ss), LR"({math-4}10)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math-4} {math:{var:2}+5})", value, ss), LR"({math-4} 10)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:4)", value, ss), LR"({math:4)", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:4    )", value, ss), LR"({math:4    )", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:A==1+1})", value, ss), LR"({math:A==1+1})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:(A)!=1+1})", value, ss), LR"({math:(A)!=1+1})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:1+1==A})", value, ss), LR"({math:1+1==A})", __LINE__);
    ss.Clear();

    helper.IsEqual(Template::Render(LR"({math:1+1!=(A)})", value, ss), LR"({math:1+1!=(A)})", __LINE__);
    ss.Clear();
}

static void TestInlineIfLTag(QTest &helper) {
    StringStream<wchar_t> ss;
    Value<wchar_t>        value;
    const wchar_t        *content;

    value += 0;
    value += 1;
    value += -1;
    value += true;
    value += false;
    value += nullptr;
    value += LR"(ABC)";
    value += Array<Value<wchar_t>>();

    content = LR"({if case="0" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(F)", __LINE__);
    ss.Clear();

    content = LR"({if case="-1" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(F)", __LINE__);
    ss.Clear();

    content = LR"({if case="0.1" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    content = LR"({if case="1" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    ////

    content = LR"({if case="0" true="T"})";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"({if case="-1" true="T"})";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"({if case="0.1" true="T"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    content = LR"({if case="1" true="T"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    ///

    content = LR"({if case="0" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(F)", __LINE__);
    ss.Clear();

    content = LR"({if case="-1" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(F)", __LINE__);
    ss.Clear();

    content = LR"({if case="0.1" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"({if case="1" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    /////

    content = LR"({if case="{var:0}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(F)", __LINE__);
    ss.Clear();

    content = LR"({if case="{var:2}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(F)", __LINE__);
    ss.Clear();

    content = LR"({if case="{raw:4}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(F)", __LINE__);
    ss.Clear();

    content = LR"({if case="{var:5}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(F)", __LINE__);
    ss.Clear();

    content = LR"({if case="{var:5}" true="T" false="{F}"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({F})", __LINE__);
    ss.Clear();

    content = LR"({if case="{var:6}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"({if case="fas" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"({if case="{var:7}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"({if case="{var:20}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    ////
    content = LR"({if case="{var:1}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    content = LR"({if case="{var:3}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    content = LR"({if case="{var:6}==ABC" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    /////////////////

    content = LR"({if case=" {var:1}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    content = LR"({if case="{var:1} " true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    content = LR"({if case=" {var:1} " true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    content = LR"({if case="  {var:1}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    content = LR"({if case="{var:1}  " true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    content = LR"({if case="  {var:1}  " true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    content = LR"({if case="      {var:1}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    content = LR"({if case="{var:1}          " true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    content = LR"({if case="    {var:1}        " true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    ////

    content = LR"(-{if case=" {var:1} " true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-T)", __LINE__);
    ss.Clear();

    content = LR"({if case=" {var:1} " true="T" false="F"}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T-)", __LINE__);
    ss.Clear();

    content = LR"(-{if case=" {var:1} " true="T" false="F"}-)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(-T-)", __LINE__);
    ss.Clear();

    content = LR"(--{if case=" {var:1} " true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(--T)", __LINE__);
    ss.Clear();

    content = LR"({if case=" {var:1} " true="T" false="F"}--)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T--)", __LINE__);
    ss.Clear();

    content = LR"(--{if case=" {var:1} " true="T" false="F"}--)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(--T--)", __LINE__);
    ss.Clear();

    content = LR"(---{if case=" {var:1} " true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(---T)", __LINE__);
    ss.Clear();

    content = LR"({if case=" {var:1} " true="T" false="F"}---)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T---)", __LINE__);
    ss.Clear();

    content = LR"(---{if case=" {var:1} " true="T" false="F"}---)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(---T---)", __LINE__);
    ss.Clear();

    content = LR"({if case="1" true="T"}{if case="0" false="F"}{if case="1" true="T"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(TFT)", __LINE__);
    ss.Clear();

    ///////

    content = LR"({if case="{var:7}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(T)", __LINE__);
    ss.Clear();

    content = LR"({if case="01" true="{var:3}" false="{var:4}"}--)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(--)", __LINE__);
    ss.Clear();

    content = LR"({if case="" true="c" false="d"})";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"({i)";
    helper.IsEqual(Template::Render(content, value, ss), LR"({i)", __LINE__);
    ss.Clear();

    content = LR"({if)";
    helper.IsEqual(Template::Render(content, value, ss), LR"({if)", __LINE__);
    ss.Clear();

    content = LR"({if})";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"({{if case="{var:1}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({T)", __LINE__);
    ss.Clear();

    content = LR"({i{if case="{var:1}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({iT)", __LINE__);
    ss.Clear();

    content = LR"({if{if case="{var:1}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({ifT)", __LINE__);
    ss.Clear();

    content = LR"({if case="{var:1}"                    put="F"})";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"({if{if case="1" true="T" false="F"}}{if case="1" true="T" false="F"})";

    helper.IsEqual(Template::Render(content, value, ss), LR"(TT)", __LINE__);
    ss.Clear();

    content = LR"({if{if case="{raw:1}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({ifTT)", __LINE__);
    ss.Clear();

    /////
    content = LR"({if case="0" true="{var:3}" false="{var:4}"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(false)", __LINE__);
    ss.Clear();

    content = LR"({if case="1" true="{var:3}" false="{var:4}"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(true)", __LINE__);
    ss.Clear();

    content = LR"({if case="0" true="{raw:3}{raw:3}" false="{var:4}{var:4}"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(falsefalse)", __LINE__);
    ss.Clear();

    content = LR"({if case="1000" true="{var:3}{var:3}" false="{var:4}{var:4}"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(truetrue)", __LINE__);
    ss.Clear();

    content = LR"({if case="0" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(false---false)", __LINE__);
    ss.Clear();

    content = LR"({if case="1" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(true---true)", __LINE__);
    ss.Clear();

    content = LR"({if case="0" true="{var:10}" false="{var:20}"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:20})", __LINE__);
    ss.Clear();

    content = LR"({if case="1" true="{var:10}" false="{var:20}"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({var:10})", __LINE__);
    ss.Clear();

    content = LR"({if case="1" true="1" false="0")";
    helper.IsEqual(Template::Render(content, value, ss), LR"({if case="1" true="1" false="0")", __LINE__);
    ss.Clear();

    content = LR"({if case="1" true="1" false="0")";
    helper.IsEqual(Template::Render(content, value, ss), LR"({if case="1" true="1" false="0")", __LINE__);
    ss.Clear();

    ///////
    Value<wchar_t> value2;

    value2 += LR"(&)";
    value2 += LR"(")";
    value2 += 10;
    value2 += 5;
    value2 += 15;

    content = LR"({if case="1" true="{var:0}" false="{var:1}"})";

    if (Config::AutoEscapeHTML) {
        helper.IsEqual(Template::Render(content, value2, ss), LR"(&amp;)", __LINE__);
        ss.Clear();

    } else {
        helper.IsEqual(Template::Render(content, value2, ss), LR"(&)", __LINE__);
        ss.Clear();
    }

    content = LR"({if case="1" true="{raw:0}" false="{raw:1}"})";
    helper.IsEqual(Template::Render(content, value2, ss), LR"(&)", __LINE__);
    ss.Clear();

    content = LR"({if case="0" true="{var:0}" false="{var:1}"})";

    if (Config::AutoEscapeHTML) {
        helper.IsEqual(Template::Render(content, value2, ss), LR"(&quot;)", __LINE__);
        ss.Clear();

    } else {
        helper.IsEqual(Template::Render(content, value2, ss), LR"(")", __LINE__);
        ss.Clear();
    }

    content = LR"({if case="0" true="*{raw:0}*" false="-{raw:1}-"})";
    helper.IsEqual(Template::Render(content, value2, ss), LR"(-"-)", __LINE__);
    ss.Clear();

    content = LR"({if case="0" true="{raw:0}" false="{raw:1}"})";
    helper.IsEqual(Template::Render(content, value2, ss), LR"(")", __LINE__);
    ss.Clear();

    content =
        LR"({if case="{var:2}+{var:3} == {var:4}" true="{math:{var:3}+{var:4}}" false="{math: {var:2}+{var:4}}"})";
    helper.IsEqual(Template::Render(content, value2, ss), LR"(20)", __LINE__);
    ss.Clear();

    content =
        LR"({if case=" {var:4}-{var:3} != {var:2} " true="{math: {var:3}+{var:4} } " false=" {math: {var:2}+{var:4} } "})";
    helper.IsEqual(Template::Render(content, value2, ss), LR"( 25 )", __LINE__);
    ss.Clear();

    content = LR"({if case="1" true="{v}" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({v})", __LINE__);
    ss.Clear();

    content = LR"({if case="1" true="{r}" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({r})", __LINE__);
    ss.Clear();

    content = LR"({if case="1" true="{m}" false="F"})";
    helper.IsEqual(Template::Render(content, value, ss), LR"({m})", __LINE__);
    ss.Clear();
}

static void TestLoopLTag1(QTest &helper) {
    StringStream<wchar_t> ss;
    Value<wchar_t>        value1;
    Value<wchar_t>        value3;
    const wchar_t        *content;

    value1 += 100;
    value1 += -50;
    value1 += LR"(Qentem)";
    value1 += true;
    value1 += false;
    value1 += nullptr;
    value1 += 3;

    content = LR"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    helper.IsEqual(Template::Render(content, value1, ss), LR"(100, -50, Qentem, true, false, null, 3, )", __LINE__);
    ss.Clear();

    content = LR"(<loop value="loop1-value">{var:loop1-value}, {var:loop1-value} </loop>)";
    helper.IsEqual(Template::Render(content, value1, ss),
                   LR"(100, 100 -50, -50 Qentem, Qentem true, true false, false null, null 3, 3 )", __LINE__);
    ss.Clear();

    ////////////////

    value1.Reset();
    value1 += 0;
    value1 += 1;

    content =
        LR"(<loop value="loop1-value"><loop value="loop2-value">({var:loop1-value}: {var:loop2-value}) </loop></loop>)";
    helper.IsEqual(Template::Render(content, value1, ss), LR"((0: 0) (0: 1) (1: 0) (1: 1) )", __LINE__);
    ss.Clear();

    content = LR"(<loop value="loop1-value"><loop value="loop2-value"><loop
                 value="loop3-value">({var:loop1-value}: {var:loop2-value}: {var:loop3-value}) </loop></loop></loop>)";
    helper.IsEqual(Template::Render(content, value1, ss),
                   LR"((0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1) )", __LINE__);
    ss.Clear();

    content = LR"(<loop value="loop1-value"><loop value="loop2-value"><loop
                 value="loop3-value"><loop value="loop4-value">({var:loop1-value}: {var:loop2-value}: {var:loop3-value}: {var:loop4-value}) </loop></loop></loop></loop>)";
    helper.IsEqual(
        Template::Render(content, value1, ss),
        LR"((0: 0: 0: 0) (0: 0: 0: 1) (0: 0: 1: 0) (0: 0: 1: 1) (0: 1: 0: 0) (0: 1: 0: 1) (0: 1: 1: 0) (0: 1: 1: 1) (1: 0: 0: 0) (1: 0: 0: 1) (1: 0: 1: 0) (1: 0: 1: 1) (1: 1: 0: 0) (1: 1: 0: 1) (1: 1: 1: 0) (1: 1: 1: 1) )",
        __LINE__);
    ss.Clear();

    //////////////////////

    value1 = JSON::Parse(LR"([100, -50, "A", true, false, null])");
    const Value<wchar_t> value2 =
        JSON::Parse(LR"({"k-1": 4, "k-2":1.5, "k-3":"ABC", "k-4":true, "k-5":false, "k-6":null})");

    //////////////////////
    value3[LR"(arr1)"] = value1;

    content = LR"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    helper.IsEqual(Template::Render(content, value1, ss), LR"(100, -50, A, true, false, null, )", __LINE__);
    ss.Clear();

    content = LR"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    helper.IsEqual(Template::Render(content, value2, ss), LR"(4, 1.5, ABC, true, false, null, )", __LINE__);
    ss.Clear();

    content = LR"(<loop set="arr1" value="loop1-value">{var:loop1-value}, </loop>)";
    helper.IsEqual(Template::Render(content, value3, ss), LR"(100, -50, A, true, false, null, )", __LINE__);
    ss.Clear();

    value3[LR"(arr1)"] = value2;

    content = LR"(<loop set="arr1" value="loop1-value">{var:loop1-value}, </loop>)";
    helper.IsEqual(Template::Render(content, value3, ss), LR"(4, 1.5, ABC, true, false, null, )", __LINE__);
    ss.Clear();

    //////////////////////
    value3[LR"(arr1)"] = value1;

    content = LR"(<loop value="loop1-value" >{var:loop1-value}, </loop>)";
    helper.IsEqual(Template::Render(content, value1, ss), LR"(100, -50, A, true, false, null, )", __LINE__);
    ss.Clear();

    content = LR"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    helper.IsEqual(Template::Render(content, value2, ss), LR"(4, 1.5, ABC, true, false, null, )", __LINE__);
    ss.Clear();

    content = LR"(<loop value="loop1-value"set="arr1">{var:loop1-value}, </loop>)";
    helper.IsEqual(Template::Render(content, value3, ss), LR"(100, -50, A, true, false, null, )", __LINE__);
    ss.Clear();

    value3[LR"(arr1)"] = value2;

    content = LR"(<loop set="arr1" value="loop1-value">{var:loop1-value}, </loop>)";
    helper.IsEqual(Template::Render(content, value3, ss), LR"(4, 1.5, ABC, true, false, null, )", __LINE__);
    ss.Clear();

    //////////////////////
    value3.Reset();
    value3[LR"(arr1)"][LR"(arr2)"][LR"(arr3)"] = value1;

    content = LR"(<loop set="arr1[arr2][arr3]" value="loop1-value">{var:loop1-value}, </loop>)";
    helper.IsEqual(Template::Render(content, value3, ss), LR"(100, -50, A, true, false, null, )", __LINE__);
    ss.Clear();

    value3.Reset();
    value3[0][0] += value2;

    content = LR"(<loop set="0[0][0]"value="loop1-value">{var:loop1-value}, </loop>)";
    helper.IsEqual(Template::Render(content, value3, ss), LR"(4, 1.5, ABC, true, false, null, )", __LINE__);
    ss.Clear();

    value3.Reset();
    value3[LR"(k1)"][0][LR"(k3)"] = value1;

    content = LR"(<loop value="loop1-value" set="k1[0][k3]">{var:loop1-value}, </loop>)";
    helper.IsEqual(Template::Render(content, value3, ss), LR"(100, -50, A, true, false, null, )", __LINE__);
    ss.Clear();

    value3.Reset();
    value3[0][LR"(k2)"] += value2;

    content = LR"(<loop set="0[k2][0]"value="loop1-value">{var:loop1-value}, {var:loop1-value}, </loop>)";
    helper.IsEqual(Template::Render(content, value3, ss),
                   LR"(4, 4, 1.5, 1.5, ABC, ABC, true, true, false, false, null, null, )", __LINE__);
    ss.Clear();

    value3 = JSON::Parse(LR"({"group":[[10],[20],[30]]})");

    content = LR"(<loop set="group" value="_Val1"><loop set="_Val1" value="_Val2">{var:_Val2}</loop></loop>)";
    helper.IsEqual(Template::Render(content, value3, ss), LR"(102030)", __LINE__);
    ss.Clear();

    value3 = JSON::Parse(LR"({"group":[1,2,3,4]})");

    content = LR"(<loop set="group" value="_Val">{var:_Val}</loop>)";
    helper.IsEqual(Template::Render(content, value3, ss), LR"(1234)", __LINE__);
    ss.Clear();

    value3  = JSON::Parse(LR"({"numbers":[1,2,3,4,5,6,7,8]})");
    content = LR"(A<loop set="numbers" value="l_id1">{var:l_id1}</loop>B)";
    helper.IsEqual(Template::Render(content, value3, ss), LR"(A12345678B)", __LINE__);
    ss.Clear();

    content = LR"(<loop set="numbers" value="l_id1">{var:l_id1[0]}</loop>)";
    helper.IsEqual(
        Template::Render(content, value3, ss),
        LR"({var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]})",
        __LINE__);
    ss.Clear();
}

static void TestLoopLTag2(QTest &helper) {
    StringStream<wchar_t> ss;
    Value<wchar_t>        value;
    const wchar_t        *content;

    content = LR"(<loop></loop>)";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"(<loop>abcd</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"(<l</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(<l</loop>)", __LINE__);
    ss.Clear();

    content = LR"(<lo</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(<lo</loop>)", __LINE__);
    ss.Clear();

    content = LR"(<loo</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(<loo</loop>)", __LINE__);
    ss.Clear();

    content = LR"(<loop></loop><loop>A</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"(<loop value="a">{var:a}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"(<loop set="ss" value="a">{var:a}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"(<loop set="" value="a">{var:a}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    value[LR"(in)"] = Array<Value<wchar_t>>();

    content = LR"(<loop value="v">{var:v}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(in)", __LINE__);
    ss.Clear();

    value.Reset();

    /////
    value.Reset();
    value[LR"(k1)"] = 10;
    value[LR"(k2)"] = 20;
    value[LR"(k3)"] = 30;

    value[LR"(k2)"].Reset();

    content = LR"(<loop value="v">{var:v}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1030)", __LINE__);
    ss.Clear();

    value.RemoveIndex(1);

    content = LR"(<loop value="v">{var:v})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(<loop value="v">{var:v})", __LINE__);
    ss.Clear();

    content = LR"(<loop value="v">{var:v}     )";
    helper.IsEqual(Template::Render(content, value, ss), LR"(<loop value="v">{var:v}     )", __LINE__);
    ss.Clear();

    value.Reset();
    value += 10;
    value += 20;
    value += 30;

    value.RemoveIndex(1);

    content = LR"(<loop value="v">{var:v}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1030)", __LINE__);
    ss.Clear();

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
        LR"(<loop set="object" value="item">{var:item[var1]}{var:item[var2]}{var:item[var3]} {var:item[var4]}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(value1value2value3 value4)", __LINE__);
    ss.Clear();

    content = LR"(<loop set="array" value="item"> {var:item[0]} {var:item[1]} {var:item[2]} {var:item[3]} </loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"( value10 value20 value30 value40 )", __LINE__);
    ss.Clear();

    content =
        LR"(<loop set="object" value="item">{var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss),
                   LR"({var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]})", __LINE__);
    ss.Clear();

    content =
        LR"(<loop set="array" value="item">{var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss),
                   LR"({var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]})", __LINE__);
    ss.Clear();

    value.RemoveIndex(0);
    content = LR"(<loop><l</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(<l)", __LINE__);
    ss.Clear();

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
    helper.IsEqual(Template::Render(content, value, ss), LR"(some_valsome_valsome_val)", __LINE__);
    ss.Clear();

    constexpr SizeT32 size_4 = (8 * 4);

    StringStream<wchar_t> content2;
    StringStream<wchar_t> output;
    Value<wchar_t>        value2;

    for (SizeT32 i = 0; i < size_4; i++) {
        value2 += i;
    }

    content2 += LR"(<loop value="loop1-value">A {var:loop1-value} B</loop>)";
    for (SizeT32 i = 0; i < size_4; i++) {
        output += LR"(A )";
        Digit::NumberToString(output, i);
        output += LR"( B)";
    }

    helper.IsEqual(Template::Render(content2.First(), content2.Length(), value2, ss), output, __LINE__);
    ss.Clear();
}

static void TestLoopLTag3(QTest &helper) {
    StringStream<wchar_t> ss;
    const wchar_t        *content;

    Value<wchar_t> value = JSON::Parse(LR"(
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
        LR"(<loop value="_val1" group="year111" sort="descend"><loop set="_val1" value="_val2"><loop set="_val2" value="_val3">{var:_val3}</loop></loop></loop>)";

    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content =
        LR"(<loop value="_val1" group="year" sort="descend"><loop set="_val1" value="_val2"><loop set="_val2" value="_val3">{var:_val3}</loop></loop></loop>)";

    helper.IsEqual(Template::Render(content, value, ss),
                   LR"(q11400q11450q11450q11100q11125q21200q22300q21200q22300q22300)", __LINE__);
    ss.Clear();

    content =
        LR"(<loop value="_val1" group="year" sort="descend"><loop set="_val1" value="_val2" group="quarter" sort="ascend"><loop set="_val2" value="_val3"><loop set="_val3" value="_val4">{var:_val4}</loop></loop></loop></loop>)";

    helper.IsEqual(Template::Render(content, value, ss), LR"(1400145014501100112512002300120023002300)", __LINE__);
    ss.Clear();

    content =
        LR"(<loop value="_val1" group="year" sort="descend">-- {var:_val1}-<loop set="_val1" value="_val2" group="quarter" sort="ascend">{var:_val2}-<loop set="_val2" value="_val3" group="week" sort="ascend">{var:_val2}:<loop set="_val3" value="_val4"><loop set="_val4" value="_val5"> {var:_val5}</loop></loop></loop></loop></loop>)";

    helper.IsEqual(
        Template::Render(content, value, ss),
        LR"(-- 2020-q1-1: 400 450 450-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2018-q2-1: 2002: 300-- 2017-q2-2: 300)",
        __LINE__);
    ss.Clear();

    content =
        LR"(<loop value="_val1" group="year">-- {var:_val1}-<loop set="_val1" value="_val2" group="quarter">{var:_val2}-<loop set="_val2" value="_val3" group="week">{var:_val2}:<loop set="_val3" value="_val4"><loop set="_val4" value="_val5"> {var:_val5}</loop></loop></loop></loop></loop>)";

    helper.IsEqual(
        Template::Render(content, value, ss),
        LR"(-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2017-q2-2: 300-- 2020-q1-1: 400 450 450-- 2018-q2-1: 2002: 300)",
        __LINE__);
    ss.Clear();

    ////////////

    value.Reset();

    value += 4;
    value += 1;
    value += 3;
    value += 5;
    value += 2;
    value += 7;
    value += 6;

    content = LR"(<loop value="_val1" sort="a">{var:_val1}</loop>)";

    helper.IsEqual(Template::Render(content, value, ss), LR"(1234567)", __LINE__);
    ss.Clear();

    content = LR"(<loop value="_val1" sort="d">{var:_val1}</loop>)";

    helper.IsEqual(Template::Render(content, value, ss), LR"(7654321)", __LINE__);
    ss.Clear();
}

static void TestIfLTag1(QTest &helper) {
    StringStream<wchar_t> ss;
    Value<wchar_t>        value;
    const wchar_t        *content;

    value[LR"(name)"] = LR"(Qen)";
    value[LR"(t)"]    = true;
    value[LR"(f)"]    = false;
    value[LR"(n)"]    = nullptr;
    value[LR"(1)"]    = 1;
    value[LR"(one)"]  = LR"(1)";
    value[LR"(zero)"] = 0;

    content = LR"(<if case="1>0">{var:name}</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(Qen)", __LINE__);
    ss.Clear();

    content = LR"(#<if case="{var:one}">{var:name}</if>#)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(#Qen#)", __LINE__);
    ss.Clear();

    content = LR"(##<if case="{var:zero}">{var:name}</if>##)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(####)", __LINE__);
    ss.Clear();

    content = LR"(<if case="{var:1}">{var:name}1<else />{var:name}2</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(Qen1)", __LINE__);
    ss.Clear();

    content = LR"(<if case="{var:f}">{var:name}1<else />{var:name}2</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(Qen2)", __LINE__);
    ss.Clear();

    content = LR"(#<if case="{var:t}">G<if case="1">oo</if>d</if>!#)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(#Good!#)", __LINE__);
    ss.Clear();

    content = LR"(###<if case="1">G<if case="1">o</if>o<if case="1">d!</if></if>###)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(###Good!###)", __LINE__);
    ss.Clear();

    content = LR"(Be <if case="1">G<if case="1">oo<if case="1">d</if></if></if>!)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(Be Good!)", __LINE__);
    ss.Clear();

    content = LR"(<if case="1">Good!<elseif case="0" />Bad!</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(Good!)", __LINE__);
    ss.Clear();

    content = LR"(<if case="{var:n}">Bad!<elseif case="1" />Good!</if>#)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(Good!#)", __LINE__);
    ss.Clear();

    content = LR"(<if case="{var:f}">Bad!<elseif case="0" />Very Bad!</if>)";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"(#<if case="0">Bad!<elseif case="0" />Very Bad!<else />Very Good!</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(#Very Good!)", __LINE__);
    ss.Clear();

    content = LR"(<if case="1">a<else />b</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(a)", __LINE__);
    ss.Clear();

    content = LR"(<if case="0">a<else />b</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(b)", __LINE__);
    ss.Clear();

    content = LR"(<if case="0">a<else /><if case="1">b</if></if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(b)", __LINE__);
    ss.Clear();

    content = LR"(<if case="1">a<else /><if case="1">b</if></if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(a)", __LINE__);
    ss.Clear();

    content = LR"(<if case="0">a<if case="1">b</if>c</if>Empty)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(Empty)", __LINE__);
    ss.Clear();

    content = LR"(<if case="1">a<else /><if case="1">b</if>c</if>===========)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(a===========)", __LINE__);
    ss.Clear();

    content = LR"(===========<if case="1">a<if case="1">b</if><else />c</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(===========ab)", __LINE__);
    ss.Clear();

    content = LR"(<if case="1">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(a)", __LINE__);
    ss.Clear();

    content = LR"(<if case="0">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(b)", __LINE__);
    ss.Clear();

    content = LR"(===========<if case="0">a<elseif case="0" />b<elseif case="1" />c<else />d</if>===========)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(===========c===========)", __LINE__);
    ss.Clear();

    content = LR"(<if case="0">a<elseif case="0" />b<elseif case="0" />c<else />d</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(d)", __LINE__);
    ss.Clear();

    content = LR"(<if case="1">a<if case="0">b<elseif case="0"/>c</if></if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(a)", __LINE__);
    ss.Clear();

    content = LR"(<if case="1">a<if case="1">b<elseif case="0"/>c</if></if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(ab)", __LINE__);
    ss.Clear();

    content = LR"(<if case="1"><if case="1">b<elseif case="c"/>c</if>a<else />c</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(ba)", __LINE__);
    ss.Clear();

    content = LR"(<if case="0">a<else />c<if case="1">b<elseif case="0"/>c</if></if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(cb)", __LINE__);
    ss.Clear();

    content = LR"(<if_case="1"><if case="0">Bad1!<elseif case="0" />Bad2!</if>a</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(a)", __LINE__);
    ss.Clear();

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
    helper.IsEqual(String<wchar_t>::Trim(Template::Render(content, value, ss).GetString()), LR"(a)", __LINE__);
    ss.Clear();

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
    helper.IsEqual(String<wchar_t>::Trim(Template::Render(content, value, ss).GetString()), LR"(b)", __LINE__);
    ss.Clear();

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
    helper.IsEqual(String<wchar_t>::Trim(Template::Render(content, value, ss).GetString()), LR"(c)", __LINE__);
    ss.Clear();

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
    helper.IsEqual(String<wchar_t>::Trim(Template::Render(content, value, ss).GetString()), LR"(d)", __LINE__);
    ss.Clear();
}

static void TestIfLTag2(QTest &helper) {
    StringStream<wchar_t> ss;
    Value<wchar_t>        value;
    const wchar_t        *content;

    value[LR"(name)"] = LR"(Qentem)";

    content = LR"(<if case="1">{var:name})";
    helper.IsEqual(Template::Render(content, value, ss), LR"(<if case="1">Qentem)", __LINE__);
    ss.Clear();

    content = LR"(<if<if case="1">{var:name}</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(<ifQentem)", __LINE__);
    ss.Clear();

    content = LR"(<if case="1"><if case="1">{var:name}</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(<if case="1">Qentem)", __LINE__);
    ss.Clear();

    content = LR"(<if case="1"><if case="1"><if case="1">{var:name}</if></if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(<if case="1">Qentem)", __LINE__);
    ss.Clear();

    content = LR"(<if case="ABC">{var:name}</if>)";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"(<if>{var:name}</if>)";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"(<if case="0"><elseif />{var:name}</if>)";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"(<iw case="0">{var:name}</if>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(<iw case="0">Qentem</if>)", __LINE__);
    ss.Clear();

    content = LR"(<if case="0"{var:name}</if>)";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"(<if case="0"><else {var:name}</if>)";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();
}

static void TestRenderL1(QTest &helper) {
    constexpr SizeT32 size_4 = (8 * 4);

    StringStream<wchar_t> ss;
    StringStream<wchar_t> content;
    StringStream<wchar_t> output;
    String<wchar_t>       str;
    Value<wchar_t>        value;

    for (SizeT32 i = 0; i < size_4; i++) {
        value += i;
    }

    for (SizeT32 i = 0; i < size_4; i++) {
        content += LR"({var:)";
        str.Reset();
        Digit::NumberToString(str, i);
        content += str;
        output += str;
        content += LR"(})";
    }

    helper.IsEqual(Template::Render(content.First(), content.Length(), value, ss), output, __LINE__);
    ss.Clear();

    content.Clear();
    output.Clear();

    SizeT32 size = 8;
    for (SizeT32 i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += LR"({var:)";
            str.Reset();
            Digit::NumberToString(str, i);
            content += str;
            content += LR"(})";

            output += str;
        } else {
            size += 8;

            content += LR"({math: 1 + )";
            str.Reset();
            Digit::NumberToString(str, i);
            content += str;
            content += LR"(})";

            Digit::NumberToString(output, 1U + i);
        }
    }

    helper.IsEqual(Template::Render(content.First(), content.Length(), value, ss), output, __LINE__);
    ss.Clear();

    content.Clear();
    output.Clear();

    size = 8;
    for (SizeT32 i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += LR"({var:)";
            str.Reset();
            Digit::NumberToString(str, i);
            content += str;
            content += LR"(})";

            output += str;
        } else {
            size += 8;

            content += LR"({if case="{var:)";
            Digit::NumberToString(content, i);
            content += LR"(}<)";
            Digit::NumberToString(content, 1U + i);
            content += LR"(" true="yes"})";

            output += LR"(yes)";
        }
    }

    helper.IsEqual(Template::Render(content.First(), content.Length(), value, ss), output, __LINE__);
    ss.Clear();

    content.Clear();
    output.Clear();

    size = 8;
    for (SizeT32 i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += LR"({var:)";
            str.Reset();
            Digit::NumberToString(str, i);
            content += str;
            content += LR"(})";

            output += str;
        } else {
            size += 8;
            content += LR"(<if case="1">A</if>)";
            output += LR"(A)";
        }
    }

    helper.IsEqual(Template::Render(content.First(), content.Length(), value, ss), output, __LINE__);
    ss.Clear();
}

static void TestRenderL2(QTest &helper) {
    StringStream<wchar_t> ss;
    Value<wchar_t>        value;
    const wchar_t        *content;

    value += 0;
    value += 1;
    value += 2;
    value += 5;
    value += 10;

    content = LR"(<loop value="loop1_val">{var:~loop1_val[0 </loop>)";
    helper.IsEqual(Template::Render(content, value, ss),
                   LR"({var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 )",
                   __LINE__);
    ss.Clear();

    content = LR"(<loop value="loop1_val">{var:loop1_val[]}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss),
                   LR"({var:loop1_val[]}{var:loop1_val[]}{var:loop1_val[]}{var:loop1_val[]}{var:loop1_val[]})",
                   __LINE__);
    ss.Clear();

    content = LR"(<loop set="numbers" value="_val">{var:_val}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), L"", __LINE__);
    ss.Clear();

    content = LR"(<loop value="this_number"><if case="({var:this_number} % 2) == 1">{var:this_number},</if></loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(1,5,)", __LINE__);
    ss.Clear();

    content = LR"(<loop value="loop1_val">{if case="{var:loop1_val} < 5", true="{var:loop1_val}"}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(012)", __LINE__);
    ss.Clear();

    content = LR"(<loop value="loop1_val">{if case="{var:loop1_val} < 5", true="{var:4}"}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(101010)", __LINE__);
    ss.Clear();

    value = JSON::Parse(LR"([[[1,2,3]]])");

    content = LR"(<loop value="loop1_val">{var:loop1_val[0][2]}</loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"(3)", __LINE__);
    ss.Clear();

    value = Qentem::JSON::Parse(LR"({"abc": [0,10,300], "xyz":[[1],[2],[3]]})");

    content = LR"(<loop set="xyz" value="lvar"> {var:lvar[0]} </loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"( 1  2  3 )", __LINE__);
    ss.Clear();

    content = LR"(<loop set="xyz" value="lvar"><loop set="lvar" value="lvar2"> {math:{var:lvar2}+3} </loop></loop>)";
    helper.IsEqual(Template::Render(content, value, ss), LR"( 4  5  6 )", __LINE__);
    ss.Clear();
}

static int RunTemplateLTests() {
    QTest helper{"Template.hpp (Wide char)", __FILE__};

    helper.PrintGroupName();

    helper.Test("Variable Tag Test 1", TestVariableLTag1);
    helper.Test("Variable Tag Test 2", TestVariableLTag2);
    helper.Test("Variable Tag Test 3", TestVariableLTag3);
    helper.Test("Variable Tag Test 4", TestVariableLTag4);

    helper.Test("Raw Variable Tag Test 1", TestRawVariableLTag1);
    helper.Test("Raw Variable Tag Test 2", TestRawVariableLTag2);
    helper.Test("Raw Variable Tag Test 3", TestRawVariableLTag3);
    helper.Test("Raw Variable Tag Test 4", TestRawVariableLTag4);

    helper.Test("Math Tag Test 1", TestMathLTag1);
    helper.Test("Math Tag Test 2", TestMathLTag2);

    helper.Test("Inline if Tag Test", TestInlineIfLTag);

    helper.Test("Loop Tag Test 1", TestLoopLTag1);
    helper.Test("Loop Tag Test 2", TestLoopLTag2);
    helper.Test("Loop Tag Test 3", TestLoopLTag3);

    helper.Test("If Tag Test 1", TestIfLTag1);
    helper.Test("If Tag Test 2", TestIfLTag2);

    helper.Test("Render Test 1", TestRenderL1);
    helper.Test("Render Test 2", TestRenderL2);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
