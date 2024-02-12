/*
 * Copyright (c) 2024 Hani Ammar
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

#ifndef QENTEM_TEMPLATE_U_TESTS_H
#define QENTEM_TEMPLATE_U_TESTS_H

#include "QTest.hpp"
#include "StringStream.hpp"
#include "JSON.hpp"
#include "Template.hpp"

namespace Qentem {
namespace Test {

static void TestVariableUTag1(QTest &test) {
    StringStream<char16_t> ss;
    const char16_t        *content;

    Value<char16_t> value = JSON::Parse(uR"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    const Value<char16_t> sub_value =
        JSON::Parse(uR"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = uR"({var:0})";
    test.IsEqual(Template::Render<StringStream<char16_t>>(content, value), uR"(A)", __LINE__);

    ss.Clear();

    content = uR"({var:1})";
    test.IsEqual(Template::Render(content, value, ss), uR"(abc)", __LINE__);
    ss.Clear();

    content = uR"({var:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(true)", __LINE__);
    ss.Clear();

    content = uR"({var:3})";
    test.IsEqual(Template::Render(content, value, ss), uR"(456)", __LINE__);
    ss.Clear();

    content = uR"({var:4})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1.5)", __LINE__);
    ss.Clear();

    content = uR"({var:5[0]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(null)", __LINE__);
    ss.Clear();

    content = uR"({var:5[1]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(false)", __LINE__);
    ss.Clear();

    content = uR"({var:5[2][0]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(Qentem)", __LINE__);
    ss.Clear();

    //////

    content = uR"({var:key1})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(a)", __LINE__);
    ss.Clear();

    content = uR"({var:key2})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(ABC)", __LINE__);
    ss.Clear();

    content = uR"({var:key3})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(false)", __LINE__);
    ss.Clear();

    content = uR"({var:key4})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(100)", __LINE__);
    ss.Clear();

    content = uR"({var:key5})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(1.5)", __LINE__);
    ss.Clear();

    content = uR"({var:key6[one]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(1)", __LINE__);
    ss.Clear();

    content = uR"({var:key7[0]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(null)", __LINE__);
    ss.Clear();

    content = uR"({var:key7[1]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(false)", __LINE__);
    ss.Clear();

    content = uR"({var:key7[2][0]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(Qentem)", __LINE__);
    ss.Clear();

    //
    content = uR"({var:6[key1]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(a)", __LINE__);
    ss.Clear();

    content = uR"({var:6[key2]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(ABC)", __LINE__);
    ss.Clear();

    content = uR"({var:6[key3]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(false)", __LINE__);
    ss.Clear();

    content = uR"({var:6[key4]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(100)", __LINE__);
    ss.Clear();

    content = uR"({var:6[key5]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1.5)", __LINE__);
    ss.Clear();

    content = uR"({var:6[key6][one]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1)", __LINE__);
    ss.Clear();

    ////////////////

    content = uR"(-{var:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(-true)", __LINE__);
    ss.Clear();

    content = uR"(-{var:key7[0]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(-null)", __LINE__);
    ss.Clear();

    content = uR"(-{var:key7[2][0]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(-Qentem)", __LINE__);
    ss.Clear();

    content = uR"(-{var:6[key3]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(-false)", __LINE__);
    ss.Clear();

    content = uR"(-{var:6[key4]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(-100)", __LINE__);
    ss.Clear();

    ////////////

    content = uR"({var:2}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(true-)", __LINE__);
    ss.Clear();

    content = uR"({var:key7[0]}-)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(null-)", __LINE__);
    ss.Clear();

    content = uR"({var:key7[2][0]}-)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(Qentem-)", __LINE__);
    ss.Clear();

    content = uR"({var:6[key3]}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(false-)", __LINE__);
    ss.Clear();

    content = uR"({var:6[key4]}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(100-)", __LINE__);
    ss.Clear();

    ////////////

    content = uR"(-{var:2}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(-true-)", __LINE__);
    ss.Clear();

    content = uR"(-{var:key7[0]}-)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(-null-)", __LINE__);
    ss.Clear();

    content = uR"(-{var:key7[2][0]}-)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(-Qentem-)", __LINE__);
    ss.Clear();

    content = uR"(-{var:6[key3]}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(-false-)", __LINE__);
    ss.Clear();

    content = uR"(-{var:6[key4]}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(-100-)", __LINE__);
    ss.Clear();

    ////////////

    content = uR"(------{var:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(------true)", __LINE__);
    ss.Clear();

    content = uR"(------{var:key7[0]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(------null)", __LINE__);
    ss.Clear();

    content = uR"(------{var:key7[2][0]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(------Qentem)", __LINE__);
    ss.Clear();

    content = uR"(------{var:6[key3]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(------false)", __LINE__);
    ss.Clear();

    content = uR"(------{var:6[key4]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(------100)", __LINE__);
    ss.Clear();

    ////////////

    content = uR"({var:2}------)";
    test.IsEqual(Template::Render(content, value, ss), uR"(true------)", __LINE__);
    ss.Clear();

    content = uR"({var:key7[0]}------)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(null------)", __LINE__);
    ss.Clear();

    content = uR"({var:key7[2][0]}------)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(Qentem------)", __LINE__);
    ss.Clear();

    content = uR"({var:6[key3]}------)";
    test.IsEqual(Template::Render(content, value, ss), uR"(false------)", __LINE__);
    ss.Clear();

    content = uR"({var:6[key4]}------)";
    test.IsEqual(Template::Render(content, value, ss), uR"(100------)", __LINE__);
    ss.Clear();

    ////////////

    content = uR"(------{var:2}------)";
    test.IsEqual(Template::Render(content, value, ss), uR"(------true------)", __LINE__);
    ss.Clear();

    content = uR"(------{var:key7[0]}------)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(------null------)", __LINE__);
    ss.Clear();

    content = uR"(------{var:key7[2][0]}------)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(------Qentem------)", __LINE__);
    ss.Clear();

    content = uR"(------{var:6[key3]}------)";
    test.IsEqual(Template::Render(content, value, ss), uR"(------false------)", __LINE__);
    ss.Clear();

    content = uR"(------{var:6[key4]}------)";
    test.IsEqual(Template::Render(content, value, ss), uR"(------100------)", __LINE__);
    ss.Clear();

    content = uR"({var:key7[2[0]})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:key7[2[0]})", __LINE__);
    ss.Clear();

    content = uR"({var:6key3]})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:6key3]})", __LINE__);
    ss.Clear();
}

static void TestVariableUTag2(QTest &test) {
    StringStream<char16_t> ss;
    const Value<char16_t>  value = JSON::Parse(uR"(["A", "abc", true, 456, 1.5])");
    const char16_t        *content;

    content = uR"({var:0}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(AA)", __LINE__);
    ss.Clear();

    content = uR"({var:1}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(abcA)", __LINE__);
    ss.Clear();

    content = uR"({var:1}{var:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(abctrue)", __LINE__);
    ss.Clear();

    content = uR"({var:2}{var:3}{var:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(true456true)", __LINE__);
    ss.Clear();

    content = uR"({var:4}{var:4}{var:4})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1.51.51.5)", __LINE__);
    ss.Clear();

    ///

    content = uR"({var:0}-{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(A-A)", __LINE__);
    ss.Clear();

    content = uR"({var:1}--{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(abc--A)", __LINE__);
    ss.Clear();

    content = uR"({var:1}---{var:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(abc---true)", __LINE__);
    ss.Clear();

    content = uR"({var:2}{var:3}--{var:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(true456--true)", __LINE__);
    ss.Clear();

    content = uR"({var:4}--{var:4}{var:4})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1.5--1.51.5)", __LINE__);
    ss.Clear();

    content = uR"({var:4}--{var:4}--{var:4})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1.5--1.5--1.5)", __LINE__);
    ss.Clear();

    content = uR"({var:4}---{var:4}---{var:4})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1.5---1.5---1.5)", __LINE__);
    ss.Clear();
}

static void TestVariableUTag3(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value;
    const char16_t        *content;

    content = uR"({var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:0})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({var:a})", value, ss), uR"({var:a})", __LINE__);
    ss.Clear();

    content = uR"({var:0[0]})";
    test.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), uR"({var:0[0]})", __LINE__);
    ss.Clear();

    content = uR"({var:a[0]})";
    test.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), uR"({var:a[0]})", __LINE__);
    ss.Clear();

    content = uR"({var:0[a]})";
    test.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), uR"({var:0[a]})", __LINE__);
    ss.Clear();

    content = uR"({var:a[abc]})";
    test.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), uR"({var:a[abc]})", __LINE__);
    ss.Clear();

    ////////////////

    value = JSON::Parse(uR"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = uR"({var:0})";
    test.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), uR"({var:0})", __LINE__);
    ss.Clear();

    content = uR"({var:0[0]})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:0[0]})", __LINE__);
    ss.Clear();

    content = uR"({var:0[0][0]})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:0[0][0]})", __LINE__);
    ss.Clear();

    /////

    content = uR"({var:1})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:1})", __LINE__);
    ss.Clear();

    content = uR"({var:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:2})", __LINE__);
    ss.Clear();

    content = uR"({var:1[a]})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:1[a]})", __LINE__);
    ss.Clear();

    content = uR"({var:1[b]})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:1[b]})", __LINE__);
    ss.Clear();

    ////

    value = JSON::Parse(uR"(["A", "abc", true, 456, 1.5])");

    content = uR"({var:0)";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:0)", __LINE__);
    ss.Clear();

    content = uR"(var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(var:0})", __LINE__);
    ss.Clear();

    content = uR"({v})";
    test.IsEqual(Template::Render(content, value, ss), uR"({v})", __LINE__);
    ss.Clear();

    content = uR"({va})";
    test.IsEqual(Template::Render(content, value, ss), uR"({va})", __LINE__);
    ss.Clear();

    content = uR"({var})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var})", __LINE__);
    ss.Clear();

    content = uR"({var:})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:})", __LINE__);
    ss.Clear();

    content = uR"({v:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({v:0})", __LINE__);
    ss.Clear();

    content = uR"({va:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({va:0})", __LINE__);
    ss.Clear();

    ////

    content = uR"({var:0{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:0{var:0})", __LINE__);
    ss.Clear();

    content = uR"(var:0{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(var:0A)", __LINE__);
    ss.Clear();

    content = uR"(var:0}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(var:0}A)", __LINE__);
    ss.Clear();

    content = uR"({var:0{var:0}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:0{var:0}A)", __LINE__);
    ss.Clear();

    ////

    content = uR"({var:0{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:0{var:0})", __LINE__);
    ss.Clear();

    content = uR"(var:0{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(var:0A)", __LINE__);
    ss.Clear();

    content = uR"(var:0}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(var:0}A)", __LINE__);
    ss.Clear();

    content = uR"({var:0{var:0}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:0{var:0}A)", __LINE__);
    ss.Clear();

    ////

    content = uR"({{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({A)", __LINE__);
    ss.Clear();

    content = uR"({{var:0}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({AA)", __LINE__);
    ss.Clear();

    content = uR"({v{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({vA)", __LINE__);
    ss.Clear();

    content = uR"({v{var:0}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({vAA)", __LINE__);
    ss.Clear();

    content = uR"({va{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({vaA)", __LINE__);
    ss.Clear();

    content = uR"({va{var:0}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({vaAA)", __LINE__);
    ss.Clear();

    content = uR"({var{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({varA)", __LINE__);
    ss.Clear();

    content = uR"({var{var:0}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({varAA)", __LINE__);
    ss.Clear();

    ///

    content = uR"({-{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({-A)", __LINE__);
    ss.Clear();

    content = uR"({-{var:0}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({-AA)", __LINE__);
    ss.Clear();

    content = uR"({v-{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({v-A)", __LINE__);
    ss.Clear();

    content = uR"({v-{var:0}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({v-AA)", __LINE__);
    ss.Clear();

    content = uR"({va-{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({va-A)", __LINE__);
    ss.Clear();

    content = uR"({va-{var:0}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({va-AA)", __LINE__);
    ss.Clear();

    content = uR"({var-{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var-A)", __LINE__);
    ss.Clear();

    content = uR"({var-{var:0}{var:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var-AA)", __LINE__);
    ss.Clear();

    //

    content = uR"({var-0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var-0})", __LINE__);
    ss.Clear();

    content = uR"({var 0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var 0})", __LINE__);
    ss.Clear();

    content = uR"({var:0 })";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:0 })", __LINE__);
    ss.Clear();

    content = uR"({var:0 )";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:0 )", __LINE__);
    ss.Clear();

    content = uR"({var:0)";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:0)", __LINE__);
    ss.Clear();

    content = uR"( {var-0})";
    test.IsEqual(Template::Render(content, value, ss), uR"( {var-0})", __LINE__);
    ss.Clear();

    content = uR"( {var 0})";
    test.IsEqual(Template::Render(content, value, ss), uR"( {var 0})", __LINE__);
    ss.Clear();

    content = uR"( {var:0 })";
    test.IsEqual(Template::Render(content, value, ss), uR"( {var:0 })", __LINE__);
    ss.Clear();

    content = uR"( {var:0 )";
    test.IsEqual(Template::Render(content, value, ss), uR"( {var:0 )", __LINE__);
    ss.Clear();

    content = uR"( {var:0)";
    test.IsEqual(Template::Render(content, value, ss), uR"( {var:0)", __LINE__);
    ss.Clear();
}

static void TestVariableUTag4(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value;

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

    if (Config::AutoEscapeHTML) {
        test.IsEqual(Template::Render(uR"({var:0})", value, ss), uR"(&lt;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:1})", value, ss), uR"(&gt;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:2})", value, ss), uR"(&amp;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:3})", value, ss), uR"(&quot;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:4})", value, ss), uR"(&apos;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:5})", value, ss), uR"(&lt;&gt;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:6})", value, ss), uR"(&lt;&amp;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:7})", value, ss), uR"(&lt;&amp;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:8})", value, ss), uR"(&gt;&quot;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:9})", value, ss), uR"(&quot;&apos;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:10})", value, ss), uR"(&lt;&quot;&gt;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:11})", value, ss), uR"(&lt;&apos;&gt;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:12})", value, ss), uR"(&lt;&amp;&gt;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:13})", value, ss), uR"(&amp;&quot;&amp;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:14})", value, ss), uR"(&quot;&apos;&quot;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:15})", value, ss), uR"(&apos;&lt;&apos;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:16})", value, ss), uR"(&apos;&amp;&apos;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:17})", value, ss), uR"(&lt;&gt;&amp;&apos;&quot;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:18})", value, ss), uR"(&apos;&quot;&lt;&gt;&amp;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:19})", value, ss), uR"(&lt;&quot;&amp;&apos;&gt;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:20})", value, ss), uR"(&lt;&lt;&lt;&lt;&lt;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:21})", value, ss), uR"(&gt;&gt;&gt;&gt;&gt;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:22})", value, ss), uR"(&amp;&amp;&amp;&amp;&amp;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:23})", value, ss), uR"(&quot;&quot;&quot;&quot;&quot;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:24})", value, ss), uR"(&apos;&apos;&apos;&apos;&apos;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:25})", value, ss), uR"(A&lt;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:26})", value, ss), uR"(A&gt;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:27})", value, ss), uR"(A&amp;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:28})", value, ss), uR"(A&quot;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:29})", value, ss), uR"(A&apos;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:30})", value, ss), uR"(&lt;A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:31})", value, ss), uR"(&gt;A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:32})", value, ss), uR"(&amp;A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:33})", value, ss), uR"(&quot;A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:34})", value, ss), uR"(&apos;A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:35})", value, ss), uR"(A&lt;A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:36})", value, ss), uR"(A&gt;A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:37})", value, ss), uR"(A&amp;A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:38})", value, ss), uR"(A&quot;A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:39})", value, ss), uR"(A&apos;A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:40})", value, ss), uR"(AA&lt;AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:41})", value, ss), uR"(AA&gt;AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:42})", value, ss), uR"(AA&amp;AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:43})", value, ss), uR"(AA&quot;AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:44})", value, ss), uR"(AA&apos;AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:45})", value, ss), uR"(AA&lt;&lt;&lt;&lt;AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:46})", value, ss), uR"(AA&gt;&gt;&gt;&gt;AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:47})", value, ss), uR"(AA&amp;&amp;&amp;&amp;AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:48})", value, ss), uR"(AA&quot;&quot;&quot;&quot;AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:49})", value, ss), uR"(AA&apos;&apos;&apos;&apos;AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:50})", value, ss),
                     uR"(&lt;A&gt;B&apos;C&quot;D&amp;E&apos;F&quot;G&lt;H&gt;I&amp;G&quot;K)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:51})", value, ss), uR"(AB&quot;CD&apos;EF&lt;GH&gt;IGK&apos;)",
                     __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:52})", value, ss), uR"(&quot;ABC&apos;DEF&lt;GHI&gt;GK&lt;)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:53})", value, ss),
                     uR"(A&quot;&quot;BC&lt;&lt;DE&gt;&gt;FG&apos;&apos;HI&amp;&amp;GK)", __LINE__);
        ss.Clear();

    } else {
        test.IsEqual(Template::Render(uR"({var:0})", value, ss), uR"(<)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:1})", value, ss), uR"(>)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:2})", value, ss), uR"(&)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:3})", value, ss), uR"(")", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:4})", value, ss), uR"(')", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:5})", value, ss), uR"(<>)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:6})", value, ss), uR"(<&)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:7})", value, ss), uR"(<&)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:8})", value, ss), uR"(>")", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:9})", value, ss), uR"("')", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:10})", value, ss), uR"(<">)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:11})", value, ss), uR"(<'>)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:12})", value, ss), uR"(<&>)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:13})", value, ss), uR"(&"&)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:14})", value, ss), uR"("'")", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:15})", value, ss), uR"('<')", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:16})", value, ss), uR"('&')", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:17})", value, ss), uR"(<>&'")", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:18})", value, ss), uR"('"<>&)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:19})", value, ss), uR"(<"&'>)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:20})", value, ss), uR"(<<<<<)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:21})", value, ss), uR"(>>>>>)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:22})", value, ss), uR"(&&&&&)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:23})", value, ss), uR"(""""")", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:24})", value, ss), uR"(''''')", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:25})", value, ss), uR"(A<)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:26})", value, ss), uR"(A>)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:27})", value, ss), uR"(A&)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:28})", value, ss), uR"(A")", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:29})", value, ss), uR"(A')", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:30})", value, ss), uR"(<A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:31})", value, ss), uR"(>A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:32})", value, ss), uR"(&A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:33})", value, ss), uR"("A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:34})", value, ss), uR"('A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:35})", value, ss), uR"(A<A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:36})", value, ss), uR"(A>A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:37})", value, ss), uR"(A&A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:38})", value, ss), uR"(A"A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:39})", value, ss), uR"(A'A)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:40})", value, ss), uR"(AA<AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:41})", value, ss), uR"(AA>AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:42})", value, ss), uR"(AA&AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:43})", value, ss), uR"(AA"AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:44})", value, ss), uR"(AA'AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:45})", value, ss), uR"(AA<<<<AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:46})", value, ss), uR"(AA>>>>AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:47})", value, ss), uR"(AA&&&&AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:48})", value, ss), uR"(AA""""AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:49})", value, ss), uR"(AA''''AA)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:50})", value, ss), uR"(<A>B'C"D&E'F"G<H>I&G"K)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:51})", value, ss), uR"(AB"CD'EF<GH>IGK')", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:52})", value, ss), uR"("ABC'DEF<GHI>GK<)", __LINE__);
        ss.Clear();

        test.IsEqual(Template::Render(uR"({var:53})", value, ss), uR"(A""BC<<DE>>FG''HI&&GK)", __LINE__);
        ss.Clear();
    }
}

static void TestRawVariableUTag1(QTest &test) {
    StringStream<char16_t> ss;
    const char16_t        *content;

    Value<char16_t> value = JSON::Parse(uR"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    const Value<char16_t> sub_value =
        JSON::Parse(uR"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = uR"({raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(A)", __LINE__);
    ss.Clear();

    content = uR"({raw:1})";
    test.IsEqual(Template::Render(content, value, ss), uR"(abc)", __LINE__);
    ss.Clear();

    content = uR"({raw:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(true)", __LINE__);
    ss.Clear();

    content = uR"({raw:3})";
    test.IsEqual(Template::Render(content, value, ss), uR"(456)", __LINE__);
    ss.Clear();

    content = uR"({raw:4})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1.5)", __LINE__);
    ss.Clear();

    content = uR"({raw:5[0]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(null)", __LINE__);
    ss.Clear();

    content = uR"({raw:5[1]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(false)", __LINE__);
    ss.Clear();

    content = uR"({raw:5[2][0]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(Qentem)", __LINE__);
    ss.Clear();

    //////

    content = uR"({raw:key1})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(a)", __LINE__);
    ss.Clear();

    content = uR"({raw:key2})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(ABC)", __LINE__);
    ss.Clear();

    content = uR"({raw:key3})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(false)", __LINE__);
    ss.Clear();

    content = uR"({raw:key4})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(100)", __LINE__);
    ss.Clear();

    content = uR"({raw:key5})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(1.5)", __LINE__);
    ss.Clear();

    content = uR"({raw:key6[one]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(1)", __LINE__);
    ss.Clear();

    content = uR"({raw:key7[0]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(null)", __LINE__);
    ss.Clear();

    content = uR"({raw:key7[1]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(false)", __LINE__);
    ss.Clear();

    content = uR"({raw:key7[2][0]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(Qentem)", __LINE__);
    ss.Clear();

    //
    content = uR"({raw:6[key1]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(a)", __LINE__);
    ss.Clear();

    content = uR"({raw:6[key2]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(ABC)", __LINE__);
    ss.Clear();

    content = uR"({raw:6[key3]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(false)", __LINE__);
    ss.Clear();

    content = uR"({raw:6[key4]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(100)", __LINE__);
    ss.Clear();

    content = uR"({raw:6[key5]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1.5)", __LINE__);
    ss.Clear();

    content = uR"({raw:6[key6][one]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1)", __LINE__);
    ss.Clear();

    ////////////////

    content = uR"(-{raw:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(-true)", __LINE__);
    ss.Clear();

    content = uR"(-{raw:key7[0]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(-null)", __LINE__);
    ss.Clear();

    content = uR"(-{raw:key7[2][0]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(-Qentem)", __LINE__);
    ss.Clear();

    content = uR"(-{raw:6[key3]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(-false)", __LINE__);
    ss.Clear();

    content = uR"(-{raw:6[key4]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(-100)", __LINE__);
    ss.Clear();

    ////////////

    content = uR"({raw:2}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(true-)", __LINE__);
    ss.Clear();

    content = uR"({raw:key7[0]}-)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(null-)", __LINE__);
    ss.Clear();

    content = uR"({raw:key7[2][0]}-)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(Qentem-)", __LINE__);
    ss.Clear();

    content = uR"({raw:6[key3]}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(false-)", __LINE__);
    ss.Clear();

    content = uR"({raw:6[key4]}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(100-)", __LINE__);
    ss.Clear();

    ////////////

    content = uR"(-{raw:2}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(-true-)", __LINE__);
    ss.Clear();

    content = uR"(-{raw:key7[0]}-)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(-null-)", __LINE__);
    ss.Clear();

    content = uR"(-{raw:key7[2][0]}-)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(-Qentem-)", __LINE__);
    ss.Clear();

    content = uR"(-{raw:6[key3]}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(-false-)", __LINE__);
    ss.Clear();

    content = uR"(-{raw:6[key4]}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(-100-)", __LINE__);
    ss.Clear();

    ////////////

    content = uR"(------{raw:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(------true)", __LINE__);
    ss.Clear();

    content = uR"(------{raw:key7[0]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(------null)", __LINE__);
    ss.Clear();

    content = uR"(------{raw:key7[2][0]})";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(------Qentem)", __LINE__);
    ss.Clear();

    content = uR"(------{raw:6[key3]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(------false)", __LINE__);
    ss.Clear();

    content = uR"(------{raw:6[key4]})";
    test.IsEqual(Template::Render(content, value, ss), uR"(------100)", __LINE__);
    ss.Clear();

    ////////////

    content = uR"({raw:2}------)";
    test.IsEqual(Template::Render(content, value, ss), uR"(true------)", __LINE__);
    ss.Clear();

    content = uR"({raw:key7[0]}------)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(null------)", __LINE__);
    ss.Clear();

    content = uR"({raw:key7[2][0]}------)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(Qentem------)", __LINE__);
    ss.Clear();

    content = uR"({raw:6[key3]}------)";
    test.IsEqual(Template::Render(content, value, ss), uR"(false------)", __LINE__);
    ss.Clear();

    content = uR"({raw:6[key4]}------)";
    test.IsEqual(Template::Render(content, value, ss), uR"(100------)", __LINE__);
    ss.Clear();

    ////////////

    content = uR"(------{raw:2}------)";
    test.IsEqual(Template::Render(content, value, ss), uR"(------true------)", __LINE__);
    ss.Clear();

    content = uR"(------{raw:key7[0]}------)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(------null------)", __LINE__);
    ss.Clear();

    content = uR"(------{raw:key7[2][0]}------)";
    test.IsEqual(Template::Render(content, sub_value, ss), uR"(------Qentem------)", __LINE__);
    ss.Clear();

    content = uR"(------{raw:6[key3]}------)";
    test.IsEqual(Template::Render(content, value, ss), uR"(------false------)", __LINE__);
    ss.Clear();

    content = uR"(------{raw:6[key4]}------)";
    test.IsEqual(Template::Render(content, value, ss), uR"(------100------)", __LINE__);
    ss.Clear();

    content = uR"({raw:key7[2[0]})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:key7[2[0]})", __LINE__);
    ss.Clear();

    content = uR"({raw:6key3]})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:6key3]})", __LINE__);
    ss.Clear();
}

static void TestRawVariableUTag2(QTest &test) {
    StringStream<char16_t> ss;
    const Value<char16_t>  value = JSON::Parse(uR"(["A", "abc", true, 456, 1.5])");
    const char16_t        *content;

    content = uR"({raw:0}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(AA)", __LINE__);
    ss.Clear();

    content = uR"({raw:1}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(abcA)", __LINE__);
    ss.Clear();

    content = uR"({raw:1}{raw:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(abctrue)", __LINE__);
    ss.Clear();

    content = uR"({raw:2}{raw:3}{raw:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(true456true)", __LINE__);
    ss.Clear();

    content = uR"({raw:4}{raw:4}{raw:4})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1.51.51.5)", __LINE__);
    ss.Clear();

    ///

    content = uR"({raw:0}-{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(A-A)", __LINE__);
    ss.Clear();

    content = uR"({raw:1}--{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(abc--A)", __LINE__);
    ss.Clear();

    content = uR"({raw:1}---{raw:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(abc---true)", __LINE__);
    ss.Clear();

    content = uR"({raw:2}{raw:3}--{raw:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"(true456--true)", __LINE__);
    ss.Clear();

    content = uR"({raw:4}--{raw:4}{raw:4})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1.5--1.51.5)", __LINE__);
    ss.Clear();

    content = uR"({raw:4}--{raw:4}--{raw:4})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1.5--1.5--1.5)", __LINE__);
    ss.Clear();

    content = uR"({raw:4}---{raw:4}---{raw:4})";
    test.IsEqual(Template::Render(content, value, ss), uR"(1.5---1.5---1.5)", __LINE__);
    ss.Clear();
}

static void TestRawVariableUTag3(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value;
    const char16_t        *content;

    content = uR"({raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:0})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:a})", value, ss), uR"({raw:a})", __LINE__);
    ss.Clear();

    content = uR"({raw:0[0]})";
    test.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), uR"({raw:0[0]})", __LINE__);
    ss.Clear();

    content = uR"({raw:a[0]})";
    test.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), uR"({raw:a[0]})", __LINE__);
    ss.Clear();

    content = uR"({raw:0[a]})";
    test.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), uR"({raw:0[a]})", __LINE__);
    ss.Clear();

    content = uR"({raw:a[abc]})";
    test.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), uR"({raw:a[abc]})", __LINE__);
    ss.Clear();

    ////////////////

    value = JSON::Parse(uR"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = uR"({raw:0})";
    test.IsEqual(Template::Render(content, StringUtils::Count(content), value, ss), uR"({raw:0})", __LINE__);
    ss.Clear();

    content = uR"({raw:0[0]})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:0[0]})", __LINE__);
    ss.Clear();

    content = uR"({raw:0[0][0]})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:0[0][0]})", __LINE__);
    ss.Clear();

    /////

    content = uR"({raw:1})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:1})", __LINE__);
    ss.Clear();

    content = uR"({raw:2})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:2})", __LINE__);
    ss.Clear();

    content = uR"({raw:1[a]})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:1[a]})", __LINE__);
    ss.Clear();

    content = uR"({raw:1[b]})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:1[b]})", __LINE__);
    ss.Clear();

    ////

    value = JSON::Parse(uR"(["A", "abc", true, 456, 1.5])");

    content = uR"({raw:0)";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:0)", __LINE__);
    ss.Clear();

    content = uR"(raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(raw:0})", __LINE__);
    ss.Clear();

    content = uR"({r})";
    test.IsEqual(Template::Render(content, value, ss), uR"({r})", __LINE__);
    ss.Clear();

    content = uR"({ra})";
    test.IsEqual(Template::Render(content, value, ss), uR"({ra})", __LINE__);
    ss.Clear();

    content = uR"({raw})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw})", __LINE__);
    ss.Clear();

    content = uR"({raw:})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:})", __LINE__);
    ss.Clear();

    content = uR"({r:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({r:0})", __LINE__);
    ss.Clear();

    content = uR"({ra:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({ra:0})", __LINE__);
    ss.Clear();

    ////

    content = uR"({raw:0{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:0{raw:0})", __LINE__);
    ss.Clear();

    content = uR"(raw:0{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(raw:0A)", __LINE__);
    ss.Clear();

    content = uR"(raw:0}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(raw:0}A)", __LINE__);
    ss.Clear();

    content = uR"({raw:0{raw:0}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:0{raw:0}A)", __LINE__);
    ss.Clear();

    ////

    content = uR"({raw:0{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:0{raw:0})", __LINE__);
    ss.Clear();

    content = uR"(raw:0{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(raw:0A)", __LINE__);
    ss.Clear();

    content = uR"(raw:0}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"(raw:0}A)", __LINE__);
    ss.Clear();

    content = uR"({raw:0{raw:0}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:0{raw:0}A)", __LINE__);
    ss.Clear();

    ////

    content = uR"({{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({A)", __LINE__);
    ss.Clear();

    content = uR"({{raw:0}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({AA)", __LINE__);
    ss.Clear();

    content = uR"({r{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({rA)", __LINE__);
    ss.Clear();

    content = uR"({r{raw:0}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({rAA)", __LINE__);
    ss.Clear();

    content = uR"({ra{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raA)", __LINE__);
    ss.Clear();

    content = uR"({ra{raw:0}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raAA)", __LINE__);
    ss.Clear();

    content = uR"({raw{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({rawA)", __LINE__);
    ss.Clear();

    content = uR"({raw{raw:0}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({rawAA)", __LINE__);
    ss.Clear();

    ///

    content = uR"({-{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({-A)", __LINE__);
    ss.Clear();

    content = uR"({-{raw:0}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({-AA)", __LINE__);
    ss.Clear();

    content = uR"({r-{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({r-A)", __LINE__);
    ss.Clear();

    content = uR"({r-{raw:0}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({r-AA)", __LINE__);
    ss.Clear();

    content = uR"({ra-{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({ra-A)", __LINE__);
    ss.Clear();

    content = uR"({ra-{raw:0}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({ra-AA)", __LINE__);
    ss.Clear();

    content = uR"({raw-{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw-A)", __LINE__);
    ss.Clear();

    content = uR"({raw-{raw:0}{raw:0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw-AA)", __LINE__);
    ss.Clear();

    //

    content = uR"({raw-0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw-0})", __LINE__);
    ss.Clear();

    content = uR"({raw 0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw 0})", __LINE__);
    ss.Clear();

    content = uR"({raw:0 })";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:0 })", __LINE__);
    ss.Clear();

    content = uR"({raw:0 )";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:0 )", __LINE__);
    ss.Clear();

    content = uR"({raw:0)";
    test.IsEqual(Template::Render(content, value, ss), uR"({raw:0)", __LINE__);
    ss.Clear();

    content = uR"({ raw-0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({ raw-0})", __LINE__);
    ss.Clear();

    content = uR"({ raw 0})";
    test.IsEqual(Template::Render(content, value, ss), uR"({ raw 0})", __LINE__);
    ss.Clear();

    content = uR"({ raw:0 })";
    test.IsEqual(Template::Render(content, value, ss), uR"({ raw:0 })", __LINE__);
    ss.Clear();

    content = uR"({ raw:0 )";
    test.IsEqual(Template::Render(content, value, ss), uR"({ raw:0 )", __LINE__);
    ss.Clear();

    content = uR"({ raw:0)";
    test.IsEqual(Template::Render(content, value, ss), uR"({ raw:0)", __LINE__);
    ss.Clear();
}

static void TestRawVariableUTag4(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value;

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

    test.IsEqual(Template::Render(uR"({raw:0})", value, ss), uR"(<)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:1})", value, ss), uR"(>)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:2})", value, ss), uR"(&)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:3})", value, ss), uR"(")", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:4})", value, ss), uR"(')", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:5})", value, ss), uR"(<>)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:6})", value, ss), uR"(<&)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:7})", value, ss), uR"(<&)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:8})", value, ss), uR"(>")", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:9})", value, ss), uR"("')", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:10})", value, ss), uR"(<">)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:11})", value, ss), uR"(<'>)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:12})", value, ss), uR"(<&>)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:13})", value, ss), uR"(&"&)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:14})", value, ss), uR"("'")", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:15})", value, ss), uR"('<')", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:16})", value, ss), uR"('&')", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:17})", value, ss), uR"(<>&'")", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:18})", value, ss), uR"('"<>&)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:19})", value, ss), uR"(<"&'>)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:20})", value, ss), uR"(<<<<<)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:21})", value, ss), uR"(>>>>>)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:22})", value, ss), uR"(&&&&&)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:23})", value, ss), uR"(""""")", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:24})", value, ss), uR"(''''')", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:25})", value, ss), uR"(A<)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:26})", value, ss), uR"(A>)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:27})", value, ss), uR"(A&)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:28})", value, ss), uR"(A")", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:29})", value, ss), uR"(A')", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:30})", value, ss), uR"(<A)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:31})", value, ss), uR"(>A)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:32})", value, ss), uR"(&A)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:33})", value, ss), uR"("A)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:34})", value, ss), uR"('A)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:35})", value, ss), uR"(A<A)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:36})", value, ss), uR"(A>A)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:37})", value, ss), uR"(A&A)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:38})", value, ss), uR"(A"A)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:39})", value, ss), uR"(A'A)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:40})", value, ss), uR"(AA<AA)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:41})", value, ss), uR"(AA>AA)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:42})", value, ss), uR"(AA&AA)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:43})", value, ss), uR"(AA"AA)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:44})", value, ss), uR"(AA'AA)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:45})", value, ss), uR"(AA<<<<AA)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:46})", value, ss), uR"(AA>>>>AA)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:47})", value, ss), uR"(AA&&&&AA)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:48})", value, ss), uR"(AA""""AA)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:49})", value, ss), uR"(AA''''AA)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:50})", value, ss), uR"(<A>B'C"D&E'F"G<H>I&G"K)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:51})", value, ss), uR"(AB"CD'EF<GH>IGK')", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:52})", value, ss), uR"("ABC'DEF<GHI>GK<)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({raw:53})", value, ss), uR"(A""BC<<DE>>FG''HI&&GK)", __LINE__);
    ss.Clear();
}

static void TestMathUTag1(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value;

    value[uR"(a1)"] = 5;
    value[uR"(a2)"] = true;
    value[uR"(a3)"] = nullptr;
    value[uR"(a4)"] = false;
    value[uR"(a5)"] = uR"(10)";
    value[uR"(a6)"] = uR"(20)";
    value[uR"(a7)"] = 6;
    value[uR"(a8)"] = 1;
    value[uR"(a9)"] = uR"(1)";

    test.IsEqual(Template::Render(uR"({math:1+1})", value, ss), uR"(2)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a1}+8})", value, ss), uR"(13)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a7}+8})", value, ss), uR"(14)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a7}+{var:a1}})", value, ss), uR"(11)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a5}+{var:a1}})", value, ss), uR"(15)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a1}+{var:a5}})", value, ss), uR"(15)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a6}+{var:a5}})", value, ss), uR"(30)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a6}*{var:a2}})", value, ss), uR"(20)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a6}*{var:a4}})", value, ss), uR"(0)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a6}*{var:a7}})", value, ss), uR"(120)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a7}+{var:a6}})", value, ss), uR"(26)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a1}+{var:a2}})", value, ss), uR"(6)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a8}=={var:a2}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a2}=={var:a8}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a5}!={var:a1}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a2}!={var:a4}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a2}==true})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a9}=={var:a8}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a8}=={var:a9}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:1=={var:a8}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:6-5==({var:a9})})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:6-5==({var:a8})})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:null!={var:a3}})", value, ss), uR"(0)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:({var:a3})==(0)})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a1}+{var:a7}})", value, ss), uR"(11)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a1}+{var:a7}}{math:{var:a1}+{var:a7}})", value, ss), uR"(1111)",
                 __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a1}+{var:a7}}*{math:{var:a1}+{var:a7}})", value, ss), uR"(11*11)",
                 __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a1}+{var:a7}}##{math:{var:a1}+{var:a7}})", value, ss), uR"(11##11)",
                 __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a1}+{var:a7}}&&&%%^^&&*{math:{var:a1}+{var:a7}})", value, ss),
                 uR"(11&&&%%^^&&*11)", __LINE__);
    ss.Clear();

    ///////////////////

    test.IsEqual(Template::Render(uR"({math: {var:a1}+8})", value, ss), uR"(13)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:  {var:a7}+8})", value, ss), uR"(14)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:     {var:a7}+{var:a1}})", value, ss), uR"(11)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a5} +{var:a1}})", value, ss), uR"(15)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a1}  +{var:a5}})", value, ss), uR"(15)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a6}    +{var:a5}})", value, ss), uR"(30)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a6}* {var:a2}})", value, ss), uR"(20)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a6}*  {var:a4}})", value, ss), uR"(0)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a6}*      {var:a7}})", value, ss), uR"(120)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a7}+{var:a6} })", value, ss), uR"(26)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a1}+{var:a2}  })", value, ss), uR"(6)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a8}=={var:a2}      })", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a2}=={var:a8}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math: {var:a5}!={var:a1} })", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:  {var:a2}!={var:a4}  })", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:    1=={var:a9}     })", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a9} == {var:a8}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a8}  ==  {var:a9}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:1==          {var:a8}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:6-5         ==1})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:1          ==            {var:a8}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:     0     !=    ({var:a3})        })", value, ss), uR"(0)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:a3}       ==       null     })", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:((8+1+{var:a8}))})", value, ss), uR"(10)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:10==(8+1+{var:a8})})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:((8+1+{var:a8}))==9+1})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:(5*2)==((8+1+{var:a8}))})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

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

    test.IsEqual(Template::Render(uR"({math:{var:0}+8})", value, ss), uR"(13)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:6}+8})", value, ss), uR"(14)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:6}+{var:0}})", value, ss), uR"(11)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:4}+{var:0}})", value, ss), uR"(15)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:0}+{var:4}})", value, ss), uR"(15)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:5}+{var:4}})", value, ss), uR"(30)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:5}*{var:1}})", value, ss), uR"(20)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:5}*{var:3}})", value, ss), uR"(0)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:5}*{var:6}})", value, ss), uR"(120)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:6}+{var:5}})", value, ss), uR"(26)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:0}+{var:1}})", value, ss), uR"(6)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:7}=={var:1}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:1}=={var:7}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:4}!={var:0}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:1}!={var:3}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:({var:1})==({var:8})})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"(-{math:{var:8}=={var:7}})", value, ss), uR"(-1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"(--{math:{var:7}=={var:8}})", value, ss), uR"(--1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"(---{math:1=={var:7}})", value, ss), uR"(---1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:1==({var:8})}-)", value, ss), uR"(1-)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:6-5==({var:7})}--)", value, ss), uR"(1--)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:0==({var:2})}---)", value, ss), uR"(1---)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"(-{math:{var:2}!=null}-)", value, ss), uR"(-0-)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"(--{math:Qente=={var:9}}--)", value, ss), uR"(--0--)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"(---{math:Qente !={var:9}}---)", value, ss), uR"(---1---)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:  Qentem   =={var:9}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:  Qentem!={var:9}})", value, ss), uR"(0)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:9}   ==    Qente})", value, ss), uR"(0)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:9} !=    Qente    })", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:  {var:9}   ==Qentem})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math: {var:9} !=Qentem})", value, ss), uR"(0)", __LINE__);
    ss.Clear();

    /////////

    test.IsEqual(Template::Render(uR"({math: true == {var:1}})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math: {var:1} == true})", value, ss), uR"(1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math: 8 /2})", value, ss), uR"(4)", __LINE__);
}

static void TestMathUTag2(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value;

    value += Array<Value<char16_t>>();
    value += HArray<Value<char16_t>, char16_t>();
    value += 5;

    test.IsEqual(Template::Render(uR"({math:{var:0}+8})", value, ss), uR"({math:{var:0}+8})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:1}+8})", value, ss), uR"({math:{var:1}+8})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:2}+{var:0}})", value, ss), uR"({math:{var:2}+{var:0}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:2}+{var:0}})", value, ss), uR"({math:{var:2}+{var:0}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:0}+{var:1}})", value, ss), uR"({math:{var:0}+{var:1}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:1}+{var:2}})", value, ss), uR"({math:{var:1}+{var:2}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:2}*{var:1}})", value, ss), uR"({math:{var:2}*{var:1}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:0}*{var:1}})", value, ss), uR"({math:{var:0}*{var:1}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:0}*{var:2}})", value, ss), uR"({math:{var:0}*{var:2}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:2}+{var:0}})", value, ss), uR"({math:{var:2}+{var:0}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:1}+{var:2}})", value, ss), uR"({math:{var:1}+{var:2}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:0}=={var:1}})", value, ss), uR"({math:{var:0}=={var:1}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:1}=={var:0}})", value, ss), uR"({math:{var:1}=={var:0}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:0}!={var:2}})", value, ss), uR"({math:{var:0}!={var:2}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:2}!={var:0}})", value, ss), uR"({math:{var:2}!={var:0}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:1}=={var:2}})", value, ss), uR"({math:{var:1}=={var:2}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:8}=={var:7}})", value, ss), uR"({math:{var:8}=={var:7}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:7}=={var:2}})", value, ss), uR"({math:{var:7}=={var:2}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:1=={var:7}})", value, ss), uR"({math:1=={var:7}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:(6-5)=={var:8}})", value, ss), uR"({math:(6-5)=={var:8}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:(6-5)=={var:0}})", value, ss), uR"({math:(6-5)=={var:0}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:0}=={var:8}})", value, ss), uR"({math:{var:0}=={var:8}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:0}=={var:0}})", value, ss), uR"({math:{var:0}=={var:0}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:0=={var:1}})", value, ss), uR"({math:0=={var:1}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:1}!=0})", value, ss), uR"({math:{var:1}!=0})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:W={var:0}})", value, ss), uR"({math:W={var:0}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:W=={var:0}})", value, ss), uR"({math:W=={var:0}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:2}==c})", value, ss), uR"({math:{var:2}==c})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:abc=={var:2}})", value, ss), uR"({math:abc=={var:2}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:sds})", value, ss), uR"({math:sds})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:1)", value, ss), uR"({math:1)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"(math:1})", value, ss), uR"(math:1})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:{var:2})", value, ss), uR"({math:5)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({{math:{var:2}+5})", value, ss), uR"({10)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({m{var:2}})", value, ss), uR"({m5})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({ma{var:2}})", value, ss), uR"({ma5})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({mat{var:2}})", value, ss), uR"({mat5})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math{var:2}})", value, ss), uR"({math5})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math {var:2}})", value, ss), uR"({math 5})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:})", value, ss), uR"({math:})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math-{var:2}}{math:{var:2}+5})", value, ss), uR"({math-5}10)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math-4}{math:{var:2}+5})", value, ss), uR"({math-4}10)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math-4} {math:{var:2}+5})", value, ss), uR"({math-4} 10)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:4)", value, ss), uR"({math:4)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:4    )", value, ss), uR"({math:4    )", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:A==1+1})", value, ss), uR"({math:A==1+1})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:(A)!=1+1})", value, ss), uR"({math:(A)!=1+1})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:1+1==A})", value, ss), uR"({math:1+1==A})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({math:1+1!=(A)})", value, ss), uR"({math:1+1!=(A)})", __LINE__);
    ss.Clear();
}

static void TestSuperVariableUTag1(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value;

    value[uR"(a)"]         = uR"(a)";
    value[uR"(b)"]         = uR"(bb)";
    value[uR"(c)"]         = uR"(ccc)";
    value[uR"(d)"]         = uR"(dddd)";
    value[uR"(4)"]         = uR"(e)";
    value[uR"(e)"]         = uR"(f)";
    value[uR"(ffffff)"]    = uR"(g)";
    value[uR"(g)"]         = 10;
    value[uR"(h)"]         = true;
    value[uR"(999999999)"] = uR"(8000000000007779999999)";

    value[uR"(x_y_z)"] = uR"(the value is {0})";

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:a}})", value, ss), uR"(the value is a)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"(..{svar:x_y_z, {var:d}}--)", value, ss), uR"(..the value is dddd--)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"(.{svar:x_y_z, {var:g}}-)", value, ss), uR"(.the value is 10-)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"(.{svar:x_y_z, {var:h}})", value, ss), uR"(.the value is true)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:999999999}}x)", value, ss),
                 uR"(the value is 8000000000007779999999x)", __LINE__);
    ss.Clear();

    value[uR"(x_y_z)"] = uR"({0})";

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:4}})", value, ss), uR"(e)", __LINE__);
    ss.Clear();

    value[uR"(x_y_z)"] = uR"({0}{0} {0} {0}{0}{0})";

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:ffffff}})", value, ss), uR"(gg g ggg)", __LINE__);
    ss.Clear();

    value[uR"(x_y_z)"] = uR"({0},{1}.{2}-{3}**{4}////{5}=={6} {7} {8} {9})";

    test.IsEqual(
        Template::Render(
            uR"({svar:x_y_z, {var:ffffff}, {var:a}, {var:4}, {var:999999999}, {var:g}, {var:h}, {var:c}, {var:d}, {var:b}, {var:e}})",
            value, ss),
        uR"(g,a.e-8000000000007779999999**10////true==ccc dddd bb f)", __LINE__);
    ss.Clear();

    value[uR"(a)"] = uR"('a)";
    value[uR"(b)"] = uR"(<>bb)";

    value[uR"(x_y_z)"] = uR"({0},{1}.{2}-{3}**{4}////{5}=={6} {7} {8} {9})";

    if (Config::AutoEscapeHTML) {
        test.IsEqual(
            Template::Render(
                uR"({svar:x_y_z, {var:ffffff}, {var:a}, {var:4}, {var:999999999}, {var:g}, {var:h}, {var:c}, {var:d}, {var:b}, {var:e}})",
                value, ss),
            uR"(g,&apos;a.e-8000000000007779999999**10////true==ccc dddd &lt;&gt;bb f)", __LINE__);
    } else {
        test.IsEqual(
            Template::Render(
                uR"({svar:x_y_z, {var:ffffff}, {var:a}, {var:4}, {var:999999999}, {var:g}, {var:h}, {var:c}, {var:d}, {var:b}, {var:e}})",
                value, ss),
            uR"(g,'a.e-8000000000007779999999**10////true==ccc dddd <>bb f)", __LINE__);
    }

    ss.Clear();

    test.IsEqual(
        Template::Render(
            uR"({svar:x_y_z, {raw:ffffff}, {raw:a}, {raw:4}, {raw:999999999}, {raw:g}, {raw:h}, {raw:c}, {raw:d}, {raw:b}, {raw:e}})",
            value, ss),
        uR"(g,'a.e-8000000000007779999999**10////true==ccc dddd <>bb f)", __LINE__);
    ss.Clear();

    value[uR"(a)"] = 10;
    value[uR"(b)"] = 5;
    value[uR"(c)"] = 3;

    value[uR"(x_y_z)"] = uR"({0} {1} {2} {3} {4})";

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {math:{var:a}+{var:b}+{var:c}},
    {math:{var:a}*{var:b}*{var:c}},
    {math:{var:a}-{var:b}-{var:c}},
    {math:{var:a}+{var:b}-{var:c}},
    {math:{var:a}/{var:b}+{var:c}}})",
                                  value, ss),
                 uR"(18 150 2 12 5)", __LINE__);
}

static void TestSuperVariableUTag2(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value;

    test.IsEqual(Template::Render(uR"({svar:x_y_z})", value, ss), uR"({svar:x_y_z})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:a}})", value, ss), uR"({svar:x_y_z, {var:a}})", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:x}, {var:y}, {var:z}})", value, ss),
                 uR"({svar:x_y_z, {var:x}, {var:y}, {var:z}})", __LINE__);
    ss.Clear();

    value[uR"(x_y_z)"] = uR"({0})";

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:x}})", value, ss), uR"({var:x})", __LINE__);
    ss.Clear();

    value[uR"(x_y_z)"] = uR"({0} {1})";

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:x}, {var:y}})", value, ss), uR"({var:x} {var:y})", __LINE__);
    ss.Clear();

    value[uR"(x_y_z)"] = uR"(-{0}-{1}-{2}-)";

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:x}, {var:y}, {var:z}})", value, ss),
                 uR"(-{var:x}-{var:y}-{var:z}-)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:x}})", value, ss), uR"(-{var:x}-{1}-{2}-)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:x}, {var:x}})", value, ss), uR"(-{var:x}-{var:x}-{2}-)",
                 __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:x}, {var:x}})", value, ss), uR"(-{var:x}-{var:x}-{2}-)",
                 __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:x}, {var:x}, {var:x}})", value, ss),
                 uR"(-{var:x}-{var:x}-{var:x}-)", __LINE__);
    ss.Clear();

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:x}, {var:y}, {var:z}})", value, ss),
                 uR"(-{var:x}-{var:y}-{var:z}-)", __LINE__);
    ss.Clear();

    value[uR"(x_y_z)"] = uR"(-{0-{1-{2-)";

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:x}, {var:y}, {var:z}})", value, ss), uR"(-{0-{1-{2-)",
                 __LINE__);
    ss.Clear();

    value[uR"(x_y_z)"] = uR"(-{{{-)";

    test.IsEqual(Template::Render(uR"({svar:x_y_z, {var:x}, {var:y}, {var:z}})", value, ss), uR"(-{{{-)", __LINE__);
    ss.Clear();

    value[uR"(x_y_z)"] = uR"({0}{1}{2}{3}{4}{5}{6}{7}{8}{9}{10}{11})";
    value[uR"(z)"]     = 1;

    test.IsEqual(
        Template::Render(
            uR"({svar:x_y_z, {var:z}, {var:z}, {var:z}, {var:z}, {var:z}, {var:z}, {var:z}, {var:z}, {var:z}, {var:z}, {var:z}, {var:z}})",
            value, ss),
        uR"(1111111111{10}{11})", __LINE__);
    ss.Clear();
}

static void TestInlineIfUTag(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value;
    const char16_t        *content;

    value += 0;
    value += 1;
    value += -1;
    value += true;
    value += false;
    value += nullptr;
    value += uR"(ABC)";
    value += Array<Value<char16_t>>();

    content = uR"({if case="0" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(F)", __LINE__);
    ss.Clear();

    content = uR"({if case='-1' true='T' false='F'})";
    test.IsEqual(Template::Render(content, value, ss), uR"(F)", __LINE__);
    ss.Clear();

    content = uR"({if case='0.1' true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    content = uR"({if case="1" true='T' false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    ////

    content = uR"({if case="0" true="T"})";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"({if case="-1" true="T"})";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"({if case="0.1" true="T"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    content = uR"({if case="1" true="T"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    ///

    content = uR"({if case="0" false='F'})";
    test.IsEqual(Template::Render(content, value, ss), uR"(F)", __LINE__);
    ss.Clear();

    content = uR"({if case="-1" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(F)", __LINE__);
    ss.Clear();

    content = uR"({if case='0.1' false='F'})";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"({if case='1' false='F'})";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    /////

    content = uR"({if case="{var:0}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(F)", __LINE__);
    ss.Clear();

    content = uR"({if case="{var:2}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(F)", __LINE__);
    ss.Clear();

    content = uR"({if case="{raw:4}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(F)", __LINE__);
    ss.Clear();

    content = uR"({if case="{var:5}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(F)", __LINE__);
    ss.Clear();

    content = uR"({if case="{var:5}" true="T" false="{F}"})";
    test.IsEqual(Template::Render(content, value, ss), uR"({F})", __LINE__);
    ss.Clear();

    content = uR"({if case="{var:6}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), u"T", __LINE__);
    ss.Clear();

    content = uR"({if case="fas" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"({if case="{var:7}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), u"F", __LINE__);
    ss.Clear();

    content = uR"({if case="{var:20}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), u"F", __LINE__);
    ss.Clear();

    ////
    content = uR"({if case="{var:1}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    content = uR"({if case="{var:3}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    content = uR"({if case="{var:6}==ABC" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    /////////////////

    content = uR"({if case=" {var:1}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    content = uR"({if case="{var:1} " true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    content = uR"({if case=" {var:1} " true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    content = uR"({if case="  {var:1}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    content = uR"({if case="{var:1}  " true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    content = uR"({if case="  {var:1}  " true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    content = uR"({if case="      {var:1}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    content = uR"({if case="{var:1}          " true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    content = uR"({if case="    {var:1}        " true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    ////

    content = uR"(-{if case=" {var:1} " true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(-T)", __LINE__);
    ss.Clear();

    content = uR"({if case=" {var:1} " true="T" false="F"}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(T-)", __LINE__);
    ss.Clear();

    content = uR"(-{if case=" {var:1} " true="T" false="F"}-)";
    test.IsEqual(Template::Render(content, value, ss), uR"(-T-)", __LINE__);
    ss.Clear();

    content = uR"(--{if case=" {var:1} " true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(--T)", __LINE__);
    ss.Clear();

    content = uR"({if case=" {var:1} " true="T" false="F"}--)";
    test.IsEqual(Template::Render(content, value, ss), uR"(T--)", __LINE__);
    ss.Clear();

    content = uR"(--{if case=" {var:1} " true="T" false="F"}--)";
    test.IsEqual(Template::Render(content, value, ss), uR"(--T--)", __LINE__);
    ss.Clear();

    content = uR"(---{if case=" {var:1} " true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(---T)", __LINE__);
    ss.Clear();

    content = uR"({if case=" {var:1} " true="T" false="F"}---)";
    test.IsEqual(Template::Render(content, value, ss), uR"(T---)", __LINE__);
    ss.Clear();

    content = uR"(---{if case=" {var:1} " true="T" false="F"}---)";
    test.IsEqual(Template::Render(content, value, ss), uR"(---T---)", __LINE__);
    ss.Clear();

    content = uR"({if case="1" true="T"}{if case="0" false="F"}{if case="1" true="T"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(TFT)", __LINE__);
    ss.Clear();

    ///////

    content = uR"({if case="{var:7}a" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(T)", __LINE__);
    ss.Clear();

    content = uR"({if case="01" true="{var:3}" false="{var:4}"}--)";
    test.IsEqual(Template::Render(content, value, ss), uR"(--)", __LINE__);
    ss.Clear();

    content = uR"({if case="" true="c" false="d"})";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"({i)";
    test.IsEqual(Template::Render(content, value, ss), uR"({i)", __LINE__);
    ss.Clear();

    content = uR"({if)";
    test.IsEqual(Template::Render(content, value, ss), uR"({if)", __LINE__);
    ss.Clear();

    content = uR"({if})";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"({{if case="{var:1}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"({T)", __LINE__);
    ss.Clear();

    content = uR"({i{if case="{var:1}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"({iT)", __LINE__);
    ss.Clear();

    content = uR"({if{if case="{var:1}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"({ifT)", __LINE__);
    ss.Clear();

    content = uR"({if case="{var:1}"                    put="F"})";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"({if}{if case="1" true="T" false="F"}{if case="1" true="T" false="F"})";

    test.IsEqual(Template::Render(content, value, ss), uR"(TT)", __LINE__);
    ss.Clear();

    content = uR"({if{if case="{raw:1}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"({ifTT)", __LINE__);
    ss.Clear();

    /////
    content = uR"({if case="0" true="{var:3}" false="{var:4}"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(false)", __LINE__);
    ss.Clear();

    content = uR"({if case="1" true="{var:3}" false="{var:4}"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(true)", __LINE__);
    ss.Clear();

    content = uR"({if case="0" true="{raw:3}{raw:3}" false="{var:4}{var:4}"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(falsefalse)", __LINE__);
    ss.Clear();

    content = uR"({if case="1000" true="{var:3}{var:3}" false="{var:4}{var:4}"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(truetrue)", __LINE__);
    ss.Clear();

    content = uR"({if case="0" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(false---false)", __LINE__);
    ss.Clear();

    content = uR"({if case="1" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    test.IsEqual(Template::Render(content, value, ss), uR"(true---true)", __LINE__);
    ss.Clear();

    content = uR"({if case="0" true="{var:10}" false="{var:20}"})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:20})", __LINE__);
    ss.Clear();

    content = uR"({if case="1" true="{var:10}" false="{var:20}"})";
    test.IsEqual(Template::Render(content, value, ss), uR"({var:10})", __LINE__);
    ss.Clear();

    content = uR"({if case="1" true="1" false="0")";
    test.IsEqual(Template::Render(content, value, ss), uR"({if case="1" true="1" false="0")", __LINE__);
    ss.Clear();

    content = uR"({if case="1" true="1" false="0")";
    test.IsEqual(Template::Render(content, value, ss), uR"({if case="1" true="1" false="0")", __LINE__);
    ss.Clear();

    ///////
    Value<char16_t> value2;

    value2 += uR"(&)";
    value2 += uR"(")";
    value2 += 10;
    value2 += 5;
    value2 += 15;

    content = uR"({if case="1" true="{var:0}" false="{var:1}"})";

    if (Config::AutoEscapeHTML) {
        test.IsEqual(Template::Render(content, value2, ss), uR"(&amp;)", __LINE__);
        ss.Clear();

    } else {
        test.IsEqual(Template::Render(content, value2, ss), uR"(&)", __LINE__);
        ss.Clear();
    }

    content = uR"({if case="1" true="{raw:0}" false="{raw:1}"})";
    test.IsEqual(Template::Render(content, value2, ss), uR"(&)", __LINE__);
    ss.Clear();

    content = uR"({if case="0" true="{var:0}" false="{var:1}"})";

    if (Config::AutoEscapeHTML) {
        test.IsEqual(Template::Render(content, value2, ss), uR"(&quot;)", __LINE__);
        ss.Clear();

    } else {
        test.IsEqual(Template::Render(content, value2, ss), uR"(")", __LINE__);
        ss.Clear();
    }

    content = uR"({if case="0" true="*{raw:0}*" false="-{raw:1}-"})";
    test.IsEqual(Template::Render(content, value2, ss), uR"(-"-)", __LINE__);
    ss.Clear();

    content = uR"({if case="0" true="{raw:0}" false="{raw:1}"})";
    test.IsEqual(Template::Render(content, value2, ss), uR"(")", __LINE__);
    ss.Clear();

    content =
        uR"({if case="{var:2}+{var:3} == {var:4}" true="{math:{var:3}+{var:4}}" false="{math: {var:2}+{var:4}}"})";
    test.IsEqual(Template::Render(content, value2, ss), uR"(20)", __LINE__);
    ss.Clear();

    content =
        uR"({if case=" {var:4}-{var:3} != {var:2} " true="{math: {var:3}+{var:4} } " false=" {math: {var:2}+{var:4} } "})";
    test.IsEqual(Template::Render(content, value2, ss), uR"( 25 )", __LINE__);
    ss.Clear();

    content = uR"({if case="1" true="{v}" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"({v})", __LINE__);
    ss.Clear();

    content = uR"({if case="1" true="{r}" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"({r})", __LINE__);
    ss.Clear();

    content = uR"({if case="1" true="{m}" false="F"})";
    test.IsEqual(Template::Render(content, value, ss), uR"({m})", __LINE__);
    ss.Clear();
}

static void TestLoopUTag1(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value1;
    Value<char16_t>        value3;
    const char16_t        *content;

    value1 += 100;
    value1 += -50;
    value1 += uR"(Qentem)";
    value1 += true;
    value1 += false;
    value1 += nullptr;
    value1 += 3;

    content = uR"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    test.IsEqual(Template::Render(content, value1, ss), uR"(100, -50, Qentem, true, false, null, 3, )", __LINE__);
    ss.Clear();

    content = uR"(<loop value="loop1-value">{var:loop1-value}, {var:loop1-value} </loop>)";
    test.IsEqual(Template::Render(content, value1, ss),
                 uR"(100, 100 -50, -50 Qentem, Qentem true, true false, false null, null 3, 3 )", __LINE__);
    ss.Clear();

    ////////////////

    value1.Reset();
    value1 += 0;
    value1 += 1;

    content =
        uR"(<loop value="loop1-value"><loop value="loop2-value">({var:loop1-value}: {var:loop2-value}) </loop></loop>)";
    test.IsEqual(Template::Render(content, value1, ss), uR"((0: 0) (0: 1) (1: 0) (1: 1) )", __LINE__);
    ss.Clear();

    content = uR"(<loop value="loop1-value"><loop value="loop2-value"><loop
                 value="loop3-value">({var:loop1-value}: {var:loop2-value}: {var:loop3-value}) </loop></loop></loop>)";
    test.IsEqual(Template::Render(content, value1, ss),
                 uR"((0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1) )", __LINE__);
    ss.Clear();

    content = uR"(<loop value='loop1-value'><loop value='loop2-value'><loop
                 value='loop3-value'><loop value='loop4-value'>({var:loop1-value}: {var:loop2-value}: {var:loop3-value}: {var:loop4-value}) </loop></loop></loop></loop>)";
    test.IsEqual(
        Template::Render(content, value1, ss),
        uR"((0: 0: 0: 0) (0: 0: 0: 1) (0: 0: 1: 0) (0: 0: 1: 1) (0: 1: 0: 0) (0: 1: 0: 1) (0: 1: 1: 0) (0: 1: 1: 1) (1: 0: 0: 0) (1: 0: 0: 1) (1: 0: 1: 0) (1: 0: 1: 1) (1: 1: 0: 0) (1: 1: 0: 1) (1: 1: 1: 0) (1: 1: 1: 1) )",
        __LINE__);
    ss.Clear();

    //////////////////////

    value1 = JSON::Parse(uR"([100, -50, "A", true, false, null])");
    const Value<char16_t> value2 =
        JSON::Parse(uR"({"k-1": 4, "k-2":1.5, "k-3":"ABC", "k-4":true, "k-5":false, "k-6":null})");

    //////////////////////
    value3[uR"(arr1)"] = value1;

    content = uR"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    test.IsEqual(Template::Render(content, value1, ss), uR"(100, -50, A, true, false, null, )", __LINE__);
    ss.Clear();

    content = uR"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    test.IsEqual(Template::Render(content, value2, ss), uR"(4, 1.5, ABC, true, false, null, )", __LINE__);
    ss.Clear();

    content = uR"(<loop set="arr1" value="loop1-value">{var:loop1-value}, </loop>)";
    test.IsEqual(Template::Render(content, value3, ss), uR"(100, -50, A, true, false, null, )", __LINE__);
    ss.Clear();

    value3[uR"(arr1)"] = value2;

    content = uR"(<loop set="arr1" value="loop1-value">{var:loop1-value}, </loop>)";
    test.IsEqual(Template::Render(content, value3, ss), uR"(4, 1.5, ABC, true, false, null, )", __LINE__);
    ss.Clear();

    //////////////////////
    value3[uR"(arr1)"] = value1;

    content = uR"(<loop value="loop1-value" >{var:loop1-value}, </loop>)";
    test.IsEqual(Template::Render(content, value1, ss), uR"(100, -50, A, true, false, null, )", __LINE__);
    ss.Clear();

    content = uR"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    test.IsEqual(Template::Render(content, value2, ss), uR"(4, 1.5, ABC, true, false, null, )", __LINE__);
    ss.Clear();

    content = uR"(<loop value="loop1-value"set="arr1">{var:loop1-value}, </loop>)";
    test.IsEqual(Template::Render(content, value3, ss), uR"(100, -50, A, true, false, null, )", __LINE__);
    ss.Clear();

    value3[uR"(arr1)"] = value2;

    content = uR"(<loop set='arr1' value="loop1-value">{var:loop1-value}, </loop>)";
    test.IsEqual(Template::Render(content, value3, ss), uR"(4, 1.5, ABC, true, false, null, )", __LINE__);
    ss.Clear();

    //////////////////////
    value3.Reset();
    value3[uR"(arr1)"][uR"(arr2)"][uR"(arr3)"] = value1;

    content = uR"(<loop set="arr1[arr2][arr3]" value="loop1-value">{var:loop1-value}, </loop>)";
    test.IsEqual(Template::Render(content, value3, ss), uR"(100, -50, A, true, false, null, )", __LINE__);
    ss.Clear();

    value3.Reset();
    value3[0][0] += value2;

    content = uR"(<loop set="0[0][0]"value="loop1-value">{var:loop1-value}, </loop>)";
    test.IsEqual(Template::Render(content, value3, ss), uR"(4, 1.5, ABC, true, false, null, )", __LINE__);
    ss.Clear();

    value3.Reset();
    value3[uR"(k1)"][0][uR"(k3)"] = value1;

    content = uR"(<loop value="loop1-value" set="k1[0][k3]">{var:loop1-value}, </loop>)";
    test.IsEqual(Template::Render(content, value3, ss), uR"(100, -50, A, true, false, null, )", __LINE__);
    ss.Clear();

    value3.Reset();
    value3[0][uR"(k2)"] += value2;

    content = uR"(<loop set="0[k2][0]"value="loop1-value">{var:loop1-value}, {var:loop1-value}, </loop>)";
    test.IsEqual(Template::Render(content, value3, ss),
                 uR"(4, 4, 1.5, 1.5, ABC, ABC, true, true, false, false, null, null, )", __LINE__);
    ss.Clear();

    value3 = JSON::Parse(uR"({"group":[[10],[20],[30]]})");

    content = uR"(<loop set="group" value="Val_1"><loop set="Val_1" value="Val_2">{var:Val_2}</loop></loop>)";
    test.IsEqual(Template::Render(content, value3, ss), uR"(102030)", __LINE__);
    ss.Clear();

    value3 = JSON::Parse(uR"({"group":[1,2,3,4]})");

    content = uR"(<loop set="group" value="Val_">{var:Val_}</loop>)";
    test.IsEqual(Template::Render(content, value3, ss), uR"(1234)", __LINE__);
    ss.Clear();

    value3  = JSON::Parse(uR"({"numbers":[1,2,3,4,5,6,7,8]})");
    content = uR"(A<loop set="numbers" value="l_id1">{var:l_id1}</loop>B)";
    test.IsEqual(Template::Render(content, value3, ss), uR"(A12345678B)", __LINE__);
    ss.Clear();

    content = uR"(<loop set="numbers" value="l_id1">{var:l_id1[0]}</loop>)";
    test.IsEqual(
        Template::Render(content, value3, ss),
        uR"({var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]})",
        __LINE__);
    ss.Clear();
}

static void TestLoopUTag2(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value;
    const char16_t        *content;

    content = uR"(<loop></loop>)";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"(<loop>abcd</loop>)";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"(<l</loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(<l</loop>)", __LINE__);
    ss.Clear();

    content = uR"(<lo</loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(<lo</loop>)", __LINE__);
    ss.Clear();

    content = uR"(<loo</loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(<loo</loop>)", __LINE__);
    ss.Clear();

    content = uR"(<loop></loop><loop>A</loop>)";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"(<loop value="a">{var:a}</loop>)";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"(<loop set="ss" value="a">{var:a}</loop>)";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"(<loop set="" value="a">{var:a}</loop>)";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    value[uR"(in)"] = Array<Value<char16_t>>();

    content = uR"(<loop value="v">{var:v}</loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(in)", __LINE__);
    ss.Clear();

    value.Reset();

    /////
    value.Reset();
    value[uR"(k1)"] = 10;
    value[uR"(k2)"] = 20;
    value[uR"(k3)"] = 30;

    value[uR"(k2)"].Reset();

    content = uR"(<loop value="v">{var:v}</loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(1030)", __LINE__);
    ss.Clear();

    value.RemoveIndex(1);

    content = uR"(<loop value="v">{var:v})";
    test.IsEqual(Template::Render(content, value, ss), uR"(<loop value="v">{var:v})", __LINE__);
    ss.Clear();

    content = uR"(<loop value="v">{var:v}     )";
    test.IsEqual(Template::Render(content, value, ss), uR"(<loop value="v">{var:v}     )", __LINE__);
    ss.Clear();

    value.Reset();
    value += 10;
    value += 20;
    value += 30;

    value.RemoveIndex(1);

    content = uR"(<loop value="v">{var:v}</loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(1030)", __LINE__);
    ss.Clear();

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
    test.IsEqual(Template::Render(content, value, ss), uR"(value1value2value3 value4)", __LINE__);
    ss.Clear();

    content = uR"(<loop set="array" value="item"> {var:item[0]} {var:item[1]} {var:item[2]} {var:item[3]} </loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"( value10 value20 value30 value40 )", __LINE__);
    ss.Clear();

    content =
        uR"(<loop set="object" value="item">{var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]}</loop>)";
    test.IsEqual(Template::Render(content, value, ss),
                 uR"({var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]})", __LINE__);
    ss.Clear();

    content =
        uR"(<loop set="array" value="item">{var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]}</loop>)";
    test.IsEqual(Template::Render(content, value, ss),
                 uR"({var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]})", __LINE__);
    ss.Clear();

    value.RemoveIndex(0);
    content = uR"(<loop><l</loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(<l)", __LINE__);
    ss.Clear();

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
    test.IsEqual(Template::Render(content, value, ss), uR"(some_valsome_valsome_val)", __LINE__);
    ss.Clear();

    constexpr SizeT32 size_4 = (8 * 4);

    StringStream<char16_t> content2;
    StringStream<char16_t> output;
    Value<char16_t>        value2;

    for (SizeT32 i = 0; i < size_4; i++) {
        value2 += i;
    }

    content2 += uR"(<loop value="loop1-value">A {var:loop1-value} B</loop>)";
    for (SizeT32 i = 0; i < size_4; i++) {
        output += uR"(A )";
        Digit::NumberToString(output, i);
        output += uR"( B)";
    }

    test.IsEqual(Template::Render(content2.First(), content2.Length(), value2, ss), output, __LINE__);
    ss.Clear();
}

static void TestLoopUTag3(QTest &test) {
    StringStream<char16_t> ss;
    const char16_t        *content;

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
        uR"(<loop value='val_1' group='year111' sort='descend'><loop set='val_1' value='val_2'><loop set='val_2' value='val_3'>{var:val_3}</loop></loop></loop>)";

    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content =
        uR"(<loop value='val_1' group="year" sort="descend"><loop set="val_1" value="val_2"><loop set="val_2" value="val_3">{var:val_3}</loop></loop></loop>)";

    test.IsEqual(Template::Render(content, value, ss),
                 uR"(q11400q11450q11450q11100q11125q21200q22300q21200q22300q22300)", __LINE__);
    ss.Clear();

    content =
        uR"(<loop value="val_1" group='year' sort="descend"><loop set="val_1" value="val_2" group="quarter" sort="ascend"><loop set="val_2" value="val_3"><loop set="val_3" value="val_4">{var:val_4}</loop></loop></loop></loop>)";

    test.IsEqual(Template::Render(content, value, ss), uR"(1400145014501100112512002300120023002300)", __LINE__);
    ss.Clear();

    content =
        uR"(<loop value="val_1" group="year" sort='descend'>-- {var:val_1}-<loop set="val_1" value="val_2" group="quarter" sort="ascend">{var:val_2}-<loop set="val_2" value="val_3" group="week" sort="ascend">{var:val_2}:<loop set="val_3" value="val_4"><loop set="val_4" value="val_5"> {var:val_5}</loop></loop></loop></loop></loop>)";

    test.IsEqual(
        Template::Render(content, value, ss),
        uR"(-- 2020-q1-1: 400 450 450-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2018-q2-1: 2002: 300-- 2017-q2-2: 300)",
        __LINE__);
    ss.Clear();

    content =
        uR"(<loop value="val_1" group="year">-- {var:val_1}-<loop set="val_1" value="val_2" group="quarter">{var:val_2}-<loop set="val_2" value="val_3" group="week">{var:val_2}:<loop set="val_3" value="val_4"><loop set="val_4" value="val_5"> {var:val_5}</loop></loop></loop></loop></loop>)";

    test.IsEqual(
        Template::Render(content, value, ss),
        uR"(-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2017-q2-2: 300-- 2020-q1-1: 400 450 450-- 2018-q2-1: 2002: 300)",
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

    content = uR"(<loop value="val_1" sort="a">{var:val_1}</loop>)";

    test.IsEqual(Template::Render(content, value, ss), uR"(1234567)", __LINE__);
    ss.Clear();

    content = uR"(<loop value="val_1" sort="d">{var:val_1}</loop>)";

    test.IsEqual(Template::Render(content, value, ss), uR"(7654321)", __LINE__);
    ss.Clear();
}

static void TestIfUTag1(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value;
    const char16_t        *content;

    value[uR"(name)"] = uR"(Qen)";
    value[uR"(t)"]    = true;
    value[uR"(f)"]    = false;
    value[uR"(n)"]    = nullptr;
    value[uR"(1)"]    = 1;
    value[uR"(one)"]  = uR"(1)";
    value[uR"(zero)"] = 0;

    content = uR"(<if case="1>0">{var:name}</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(Qen)", __LINE__);
    ss.Clear();

    content = uR"(#<if case="{var:one}">{var:name}</if>#)";
    test.IsEqual(Template::Render(content, value, ss), uR"(#Qen#)", __LINE__);
    ss.Clear();

    content = uR"(##<if case="{var:zero}">{var:name}</if>##)";
    test.IsEqual(Template::Render(content, value, ss), uR"(####)", __LINE__);
    ss.Clear();

    content = uR"(<if case="{var:1}">{var:name}1<else />{var:name}2</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(Qen1)", __LINE__);
    ss.Clear();

    content = uR"(<if case="{var:f}">{var:name}1<else />{var:name}2</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(Qen2)", __LINE__);
    ss.Clear();

    content = uR"(#<if case="{var:t}">G<if case="1">oo</if>d</if>!#)";
    test.IsEqual(Template::Render(content, value, ss), uR"(#Good!#)", __LINE__);
    ss.Clear();

    content = uR"(###<if case="1">G<if case="1">o</if>o<if case="1">d!</if></if>###)";
    test.IsEqual(Template::Render(content, value, ss), uR"(###Good!###)", __LINE__);
    ss.Clear();

    content = uR"(Be <if case="1">G<if case="1">oo<if case="1">d</if></if></if>!)";
    test.IsEqual(Template::Render(content, value, ss), uR"(Be Good!)", __LINE__);
    ss.Clear();

    content = uR"(<if case="1">Good!<elseif case="0" />Bad!</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(Good!)", __LINE__);
    ss.Clear();

    content = uR"(<if case="{var:n}">Bad!<else if case="1" />Good!</if>#)";
    test.IsEqual(Template::Render(content, value, ss), uR"(Good!#)", __LINE__);
    ss.Clear();

    content = uR"(<if case="{var:f}">Bad!<elseif case="0" />Very Bad!</if>)";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"(#<if case="0">Bad!<else  if case="0" />Very Bad!<else />Very Good!</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(#Very Good!)", __LINE__);
    ss.Clear();

    content = uR"(<if case="1">a<else />b</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(a)", __LINE__);
    ss.Clear();

    content = uR"(<if case="0">a<else />b</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(b)", __LINE__);
    ss.Clear();

    content = uR"(<if case="0">a<else /><if case="1">b</if></if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(b)", __LINE__);
    ss.Clear();

    content = uR"(<if case="1">a<else /><if case="1">b</if></if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(a)", __LINE__);
    ss.Clear();

    content = uR"(<if case="0">a<if case="1">b</if>c</if>Empty)";
    test.IsEqual(Template::Render(content, value, ss), uR"(Empty)", __LINE__);
    ss.Clear();

    content = uR"(<if case="1">a<else /><if case="1">b</if>c</if>===========)";
    test.IsEqual(Template::Render(content, value, ss), uR"(a===========)", __LINE__);
    ss.Clear();

    content = uR"(===========<if case="1">a<if case="1">b</if><else />c</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(===========ab)", __LINE__);
    ss.Clear();

    content = uR"(<if case="1">a<else if case="1" />b<elseif case="1" />c<else />d</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(a)", __LINE__);
    ss.Clear();

    content = uR"(<if case="0">a<elseif case="1" />b<else if case="1" />c<else />d</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(b)", __LINE__);
    ss.Clear();

    content = uR"(===========<if case="0">a<elseif case="0" />b<elseif case="1" />c<else />d</if>===========)";
    test.IsEqual(Template::Render(content, value, ss), uR"(===========c===========)", __LINE__);
    ss.Clear();

    content = uR"(<if case="0">a<elseif case="0" />b<elseif case="0" />c<else />d</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(d)", __LINE__);
    ss.Clear();

    content = uR"(<if case="1">a<if case="0">b<elseif case="0"/>c</if></if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(a)", __LINE__);
    ss.Clear();

    content = uR"(<if case="1">a<if case="1">b<elseif case="0"/>c</if></if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(ab)", __LINE__);
    ss.Clear();

    content = uR"(<if case="1"><if case="1">b<elseif case="c"/>c</if>a<else />c</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(ba)", __LINE__);
    ss.Clear();

    content = uR"(<if case="0">a<else />c<if case="1">b<elseif case="0"/>c</if></if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(cb)", __LINE__);
    ss.Clear();

    content = uR"(<if case="1"><if case="0">Bad1!<elseif case="0" />Bad2!</if>a</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(a)", __LINE__);
    ss.Clear();

    content = uR"(<if case="1">
            <if case="0">Bad1!
                <else if case="0">Bad2!
                <else if case="0">Bad3!
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
            <else>d
                <if case="1">Bad1!
                <elseif case="1" />Bad2!
                <elseif case="1" />Bad3!
                </if>
            </if>)";
    test.IsEqual(String<char16_t>::Trim(Template::Render(content, value, ss).GetString()), uR"(a)", __LINE__);
    ss.Clear();

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
    test.IsEqual(String<char16_t>::Trim(Template::Render(content, value, ss).GetString()), uR"(b)", __LINE__);
    ss.Clear();

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
    test.IsEqual(String<char16_t>::Trim(Template::Render(content, value, ss).GetString()), uR"(c)", __LINE__);
    ss.Clear();

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
    test.IsEqual(String<char16_t>::Trim(Template::Render(content, value, ss).GetString()), uR"(d)", __LINE__);
    ss.Clear();
}

static void TestIfUTag2(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value;
    const char16_t        *content;

    value[uR"(name)"] = uR"(Qentem)";

    content = uR"(<if case="1">{var:name})";
    test.IsEqual(Template::Render(content, value, ss), uR"(<if case="1">Qentem)", __LINE__);
    ss.Clear();

    content = uR"(<if<if case="1">{var:name}</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(<ifQentem)", __LINE__);
    ss.Clear();

    content = uR"(<if case="1"><if case="1">{var:name}</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(<if case="1">Qentem)", __LINE__);
    ss.Clear();

    content = uR"(<if case="1"><if case="1"><if case="1">{var:name}</if></if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(<if case="1">Qentem)", __LINE__);
    ss.Clear();

    content = uR"(<if case="ABC">{var:name}</if>)";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"(<if>{var:name}</if>)";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"(<if case="0"><elseif />{var:name}</if>)";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"(<iw case="0">{var:name}</if>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(<iw case="0">Qentem</if>)", __LINE__);
    ss.Clear();

    content = uR"(<if case="0"{var:name}</if>)";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"(<if case="0"><else {var:name}</if>)";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();
}

static void TestRenderU1(QTest &test) {
    constexpr SizeT32 size_4 = (8 * 4);

    StringStream<char16_t> ss;
    StringStream<char16_t> content;
    StringStream<char16_t> output;
    String<char16_t>       str;
    Value<char16_t>        value;

    for (SizeT32 i = 0; i < size_4; i++) {
        value += i;
    }

    for (SizeT32 i = 0; i < size_4; i++) {
        content += uR"({var:)";
        str.Reset();
        Digit::NumberToString(str, i);
        content += str;
        output += str;
        content += uR"(})";
    }

    test.IsEqual(Template::Render(content.First(), content.Length(), value, ss), output, __LINE__);
    ss.Clear();

    content.Clear();
    output.Clear();

    SizeT32 size = 8;
    for (SizeT32 i = 0, x = 1; i < size_4; i++, x++) {
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

    test.IsEqual(Template::Render(content.First(), content.Length(), value, ss), output, __LINE__);
    ss.Clear();

    content.Clear();
    output.Clear();

    size = 8;
    for (SizeT32 i = 0, x = 1; i < size_4; i++, x++) {
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

    test.IsEqual(Template::Render(content.First(), content.Length(), value, ss), output, __LINE__);
    ss.Clear();

    content.Clear();
    output.Clear();

    size = 8;
    for (SizeT32 i = 0, x = 1; i < size_4; i++, x++) {
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

    test.IsEqual(Template::Render(content.First(), content.Length(), value, ss), output, __LINE__);
    ss.Clear();
}

static void TestRenderU2(QTest &test) {
    StringStream<char16_t> ss;
    Value<char16_t>        value;
    const char16_t        *content;

    value += 0;
    value += 1;
    value += 2;
    value += 5;
    value += 10;

    content = uR"(<loop value="loop1_val">{var:~loop1_val[0 </loop>)";
    test.IsEqual(Template::Render(content, value, ss),
                 uR"({var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 )",
                 __LINE__);
    ss.Clear();

    content = uR"(<loop value="loop1_val">{var:loop1_val[]}</loop>)";
    test.IsEqual(Template::Render(content, value, ss),
                 uR"({var:loop1_val[]}{var:loop1_val[]}{var:loop1_val[]}{var:loop1_val[]}{var:loop1_val[]})", __LINE__);
    ss.Clear();

    content = uR"(<loop set="numbers" value="val_">{var:val_}</loop>)";
    test.IsEqual(Template::Render(content, value, ss), u"", __LINE__);
    ss.Clear();

    content = uR"(<loop value="this_number"><if case="({var:this_number} % 2) == 1">{var:this_number},</if></loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(1,5,)", __LINE__);
    ss.Clear();

    content = uR"(<loop value="loop1_val">{if case="{var:loop1_val} < 5", true="{var:loop1_val}"}</loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(012)", __LINE__);
    ss.Clear();

    content = uR"(<loop value="loop1_val">{if case="{var:loop1_val} < 5", true="{var:4}"}</loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(101010)", __LINE__);
    ss.Clear();

    value = JSON::Parse(uR"([[[1,2,3]]])");

    content = uR"(<loop value="loop1_val">{var:loop1_val[0][2]}</loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"(3)", __LINE__);
    ss.Clear();

    value = Qentem::JSON::Parse(uR"({"abc": [0,10,300], "xyz":[[1],[2],[3]]})");

    content = uR"(<loop set="xyz" value="lvar"> {var:lvar[0]} </loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"( 1  2  3 )", __LINE__);
    ss.Clear();

    content = uR"(<loop set="xyz" value="lvar"><loop set="lvar" value="lvar2"> {math:{var:lvar2}+3} </loop></loop>)";
    test.IsEqual(Template::Render(content, value, ss), uR"( 4  5  6 )", __LINE__);
    ss.Clear();
}

static int RunTemplateUTests() {
    QTest test{"Template.hpp (16-bit char)", __FILE__};

    test.PrintGroupName();

    test.Test("Variable Tag Test 1", TestVariableUTag1);
    test.Test("Variable Tag Test 2", TestVariableUTag2);
    test.Test("Variable Tag Test 3", TestVariableUTag3);
    test.Test("Variable Tag Test 4", TestVariableUTag4);

    test.Test("Raw Variable Tag Test 1", TestRawVariableUTag1);
    test.Test("Raw Variable Tag Test 2", TestRawVariableUTag2);
    test.Test("Raw Variable Tag Test 3", TestRawVariableUTag3);
    test.Test("Raw Variable Tag Test 4", TestRawVariableUTag4);

    test.Test("Math Tag Test 1", TestMathUTag1);
    test.Test("Math Tag Test 2", TestMathUTag2);

    test.Test("Super Variable Tag Test 1", TestSuperVariableUTag1);
    test.Test("Super Variable Tag Test 2", TestSuperVariableUTag2);

    test.Test("Inline if Tag Test", TestInlineIfUTag);

    test.Test("Loop Tag Test 1", TestLoopUTag1);
    test.Test("Loop Tag Test 2", TestLoopUTag2);
    test.Test("Loop Tag Test 3", TestLoopUTag3);

    test.Test("If Tag Test 1", TestIfUTag1);
    test.Test("If Tag Test 2", TestIfUTag2);

    test.Test("Render Test 1", TestRenderU1);
    test.Test("Render Test 2", TestRenderU2);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
