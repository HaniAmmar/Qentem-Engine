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
#include "StringStream.hpp"
#include "JSON.hpp"
#include "Template.hpp"

#ifndef QENTEM_TEMPLATE_TESTS_H_
#define QENTEM_TEMPLATE_TESTS_H_

namespace Qentem::Test {

static void TestVariableTag1(TestHelper &helper) {
    StringStream<char> ss;

    const char *content;

    Value<char> value = JSON::Parse(R"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    const Value<char> sub_value =
        JSON::Parse(R"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = R"({var:0})";
    helper.Equal(Template::Render(content, value, ss), R"(A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:1})";
    helper.Equal(Template::Render(content, value, ss), R"(abc)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:2})";
    helper.Equal(Template::Render(content, value, ss), R"(true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:3})";
    helper.Equal(Template::Render(content, value, ss), R"(456)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:4})";
    helper.Equal(Template::Render(content, value, ss), R"(1.5)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:5[0]})";
    helper.Equal(Template::Render(content, value, ss), R"(null)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:5[1]})";
    helper.Equal(Template::Render(content, value, ss), R"(false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:5[2][0]})";
    helper.Equal(Template::Render(content, value, ss), R"(Qentem)", R"(Render())", __LINE__);
    ss.Clear();

    //////

    content = R"({var:key1})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(a)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:key2})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(ABC)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:key3})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:key4})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(100)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:key5})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(1.5)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:key6[one]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:key7[0]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(null)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:key7[1]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:key7[2][0]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(Qentem)", R"(Render())", __LINE__);
    ss.Clear();

    //
    content = R"({var:6[key1]})";
    helper.Equal(Template::Render(content, value, ss), R"(a)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:6[key2]})";
    helper.Equal(Template::Render(content, value, ss), R"(ABC)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:6[key3]})";
    helper.Equal(Template::Render(content, value, ss), R"(false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:6[key4]})";
    helper.Equal(Template::Render(content, value, ss), R"(100)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:6[key5]})";
    helper.Equal(Template::Render(content, value, ss), R"(1.5)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:6[key6][one]})";
    helper.Equal(Template::Render(content, value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    ////////////////

    content = R"(-{var:2})";
    helper.Equal(Template::Render(content, value, ss), R"(-true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{var:key7[0]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(-null)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{var:key7[2][0]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(-Qentem)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{var:6[key3]})";
    helper.Equal(Template::Render(content, value, ss), R"(-false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{var:6[key4]})";
    helper.Equal(Template::Render(content, value, ss), R"(-100)", R"(Render())", __LINE__);
    ss.Clear();

    ////////////

    content = R"({var:2}-)";
    helper.Equal(Template::Render(content, value, ss), R"(true-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:key7[0]}-)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(null-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:key7[2][0]}-)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(Qentem-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:6[key3]}-)";
    helper.Equal(Template::Render(content, value, ss), R"(false-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:6[key4]}-)";
    helper.Equal(Template::Render(content, value, ss), R"(100-)", R"(Render())", __LINE__);
    ss.Clear();

    ////////////

    content = R"(-{var:2}-)";
    helper.Equal(Template::Render(content, value, ss), R"(-true-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{var:key7[0]}-)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(-null-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{var:key7[2][0]}-)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(-Qentem-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{var:6[key3]}-)";
    helper.Equal(Template::Render(content, value, ss), R"(-false-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{var:6[key4]}-)";
    helper.Equal(Template::Render(content, value, ss), R"(-100-)", R"(Render())", __LINE__);
    ss.Clear();

    ////////////

    content = R"(------{var:2})";
    helper.Equal(Template::Render(content, value, ss), R"(------true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{var:key7[0]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(------null)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{var:key7[2][0]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(------Qentem)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{var:6[key3]})";
    helper.Equal(Template::Render(content, value, ss), R"(------false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{var:6[key4]})";
    helper.Equal(Template::Render(content, value, ss), R"(------100)", R"(Render())", __LINE__);
    ss.Clear();

    ////////////

    content = R"({var:2}------)";
    helper.Equal(Template::Render(content, value, ss), R"(true------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:key7[0]}------)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(null------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:key7[2][0]}------)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(Qentem------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:6[key3]}------)";
    helper.Equal(Template::Render(content, value, ss), R"(false------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:6[key4]}------)";
    helper.Equal(Template::Render(content, value, ss), R"(100------)", R"(Render())", __LINE__);
    ss.Clear();

    ////////////

    content = R"(------{var:2}------)";
    helper.Equal(Template::Render(content, value, ss), R"(------true------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{var:key7[0]}------)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(------null------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{var:key7[2][0]}------)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(------Qentem------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{var:6[key3]}------)";
    helper.Equal(Template::Render(content, value, ss), R"(------false------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{var:6[key4]}------)";
    helper.Equal(Template::Render(content, value, ss), R"(------100------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:key7[2[0]})";
    helper.Equal(Template::Render(content, value, ss), R"({var:key7[2[0]})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:6key3]})";
    helper.Equal(Template::Render(content, value, ss), R"({var:6key3]})", R"(Render())", __LINE__);
    ss.Clear();
}

static void TestVariableTag2(TestHelper &helper) {
    StringStream<char> ss;
    const Value<char>  value = JSON::Parse(R"(["A", "abc", true, 456, 1.5])");
    const char        *content;

    content = R"({var:0}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"(AA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:1}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"(abcA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:1}{var:2})";
    helper.Equal(Template::Render(content, value, ss), R"(abctrue)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:2}{var:3}{var:2})";
    helper.Equal(Template::Render(content, value, ss), R"(true456true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:4}{var:4}{var:4})";
    helper.Equal(Template::Render(content, value, ss), R"(1.51.51.5)", R"(Render())", __LINE__);
    ss.Clear();

    ///

    content = R"({var:0}-{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"(A-A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:1}--{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"(abc--A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:1}---{var:2})";
    helper.Equal(Template::Render(content, value, ss), R"(abc---true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:2}{var:3}--{var:2})";
    helper.Equal(Template::Render(content, value, ss), R"(true456--true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:4}--{var:4}{var:4})";
    helper.Equal(Template::Render(content, value, ss), R"(1.5--1.51.5)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:4}--{var:4}--{var:4})";
    helper.Equal(Template::Render(content, value, ss), R"(1.5--1.5--1.5)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:4}---{var:4}---{var:4})";
    helper.Equal(Template::Render(content, value, ss), R"(1.5---1.5---1.5)", R"(Render())", __LINE__);
    ss.Clear();
}

static void TestVariableTag3(TestHelper &helper) {
    StringStream<char> ss;
    Value<char>        value;
    const char        *content;

    content = R"({var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({var:0})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({var:a})", value, ss), R"({var:a})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:0[0]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value, ss), R"({var:0[0]})", R"(Render())",
                 __LINE__);
    ss.Clear();

    content = R"({var:a[0]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value, ss), R"({var:a[0]})", R"(Render())",
                 __LINE__);
    ss.Clear();

    content = R"({var:0[a]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value, ss), R"({var:0[a]})", R"(Render())",
                 __LINE__);
    ss.Clear();

    content = R"({var:a[abc]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value, ss), R"({var:a[abc]})", R"(Render())",
                 __LINE__);
    ss.Clear();

    ////////////////

    value = JSON::Parse(R"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = R"({var:0})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value, ss), R"({var:0})", R"(Render())",
                 __LINE__);
    ss.Clear();

    content = R"({var:0[0]})";
    helper.Equal(Template::Render(content, value, ss), R"({var:0[0]})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:0[0][0]})";
    helper.Equal(Template::Render(content, value, ss), R"({var:0[0][0]})", R"(Render())", __LINE__);
    ss.Clear();

    /////

    content = R"({var:1})";
    helper.Equal(Template::Render(content, value, ss), R"({var:1})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:2})";
    helper.Equal(Template::Render(content, value, ss), R"({var:2})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:1[a]})";
    helper.Equal(Template::Render(content, value, ss), R"({var:1[a]})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:1[b]})";
    helper.Equal(Template::Render(content, value, ss), R"({var:1[b]})", R"(Render())", __LINE__);
    ss.Clear();

    ////

    value = JSON::Parse(R"(["A", "abc", true, 456, 1.5])");

    content = R"({var:0)";
    helper.Equal(Template::Render(content, value, ss), R"({var:0)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(var:0})";
    helper.Equal(Template::Render(content, value, ss), R"(var:0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({v})";
    helper.Equal(Template::Render(content, value, ss), R"({v})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({va})";
    helper.Equal(Template::Render(content, value, ss), R"({va})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var})";
    helper.Equal(Template::Render(content, value, ss), R"({var})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:})";
    helper.Equal(Template::Render(content, value, ss), R"({var:})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({v:0})";
    helper.Equal(Template::Render(content, value, ss), R"({v:0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({va:0})";
    helper.Equal(Template::Render(content, value, ss), R"({va:0})", R"(Render())", __LINE__);
    ss.Clear();

    ////

    content = R"({var:0{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({var:0{var:0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(var:0{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"(var:0A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(var:0}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"(var:0}A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:0{var:0}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({var:0{var:0}A)", R"(Render())", __LINE__);
    ss.Clear();

    ////

    content = R"({var:0{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({var:0{var:0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(var:0{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"(var:0A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(var:0}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"(var:0}A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:0{var:0}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({var:0{var:0}A)", R"(Render())", __LINE__);
    ss.Clear();

    ////

    content = R"({{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({{var:0}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({AA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({v{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({vA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({v{var:0}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({vAA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({va{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({vaA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({va{var:0}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({vaAA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({varA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var{var:0}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({varAA)", R"(Render())", __LINE__);
    ss.Clear();

    ///

    content = R"({-{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({-A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({-{var:0}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({-AA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({v-{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({v-A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({v-{var:0}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({v-AA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({va-{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({va-A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({va-{var:0}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({va-AA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var-{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({var-A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var-{var:0}{var:0})";
    helper.Equal(Template::Render(content, value, ss), R"({var-AA)", R"(Render())", __LINE__);
    ss.Clear();

    //

    content = R"({var-0})";
    helper.Equal(Template::Render(content, value, ss), R"({var-0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var 0})";
    helper.Equal(Template::Render(content, value, ss), R"({var 0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:0 })";
    helper.Equal(Template::Render(content, value, ss), R"({var:0 })", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:0 )";
    helper.Equal(Template::Render(content, value, ss), R"({var:0 )", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({var:0)";
    helper.Equal(Template::Render(content, value, ss), R"({var:0)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"( {var-0})";
    helper.Equal(Template::Render(content, value, ss), R"( {var-0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"( {var 0})";
    helper.Equal(Template::Render(content, value, ss), R"( {var 0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"( {var:0 })";
    helper.Equal(Template::Render(content, value, ss), R"( {var:0 })", R"(Render())", __LINE__);
    ss.Clear();

    content = R"( {var:0 )";
    helper.Equal(Template::Render(content, value, ss), R"( {var:0 )", R"(Render())", __LINE__);
    ss.Clear();

    content = R"( {var:0)";
    helper.Equal(Template::Render(content, value, ss), R"( {var:0)", R"(Render())", __LINE__);
    ss.Clear();
}

static void TestVariableTag4(TestHelper &helper) {
    StringStream<char> ss;
    Value<char>        value;

    value += R"(<)";
    value += R"(>)";
    value += R"(&)";
    value += R"(")";
    value += R"(')";
    value += R"(<>)";
    value += R"(<&)";
    value += R"(<&)";
    value += R"(>")";
    value += R"("')";
    value += R"(<">)";
    value += R"(<'>)";
    value += R"(<&>)";
    value += R"(&"&)";
    value += R"("'")";
    value += R"('<')";
    value += R"('&')";
    value += R"(<>&'")";
    value += R"('"<>&)";
    value += R"(<"&'>)";

    value += R"(<<<<<)";
    value += R"(>>>>>)";
    value += R"(&&&&&)";
    value += R"(""""")";
    value += R"(''''')";

    value += R"(A<)";
    value += R"(A>)";
    value += R"(A&)";
    value += R"(A")";
    value += R"(A')";
    value += R"(<A)";
    value += R"(>A)";
    value += R"(&A)";
    value += R"("A)";
    value += R"('A)";
    value += R"(A<A)";
    value += R"(A>A)";
    value += R"(A&A)";
    value += R"(A"A)";
    value += R"(A'A)";

    value += R"(AA<AA)";
    value += R"(AA>AA)";
    value += R"(AA&AA)";
    value += R"(AA"AA)";
    value += R"(AA'AA)";

    value += R"(AA<<<<AA)";
    value += R"(AA>>>>AA)";
    value += R"(AA&&&&AA)";
    value += R"(AA""""AA)";
    value += R"(AA''''AA)";

    value += R"(<A>B'C"D&E'F"G<H>I&G"K)";
    value += R"(AB"CD'EF<GH>IGK')";
    value += R"("ABC'DEF<GHI>GK<)";
    value += R"(A""BC<<DE>>FG''HI&&GK)";

    if constexpr (Config::AutoEscapeHTML) {
        helper.Equal(Template::Render(R"({var:0})", value, ss), R"(&lt;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:1})", value, ss), R"(&gt;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:2})", value, ss), R"(&amp;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:3})", value, ss), R"(&quot;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:4})", value, ss), R"(&apos;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:5})", value, ss), R"(&lt;&gt;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:6})", value, ss), R"(&lt;&amp;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:7})", value, ss), R"(&lt;&amp;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:8})", value, ss), R"(&gt;&quot;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:9})", value, ss), R"(&quot;&apos;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:10})", value, ss), R"(&lt;&quot;&gt;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:11})", value, ss), R"(&lt;&apos;&gt;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:12})", value, ss), R"(&lt;&amp;&gt;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:13})", value, ss), R"(&amp;&quot;&amp;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:14})", value, ss), R"(&quot;&apos;&quot;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:15})", value, ss), R"(&apos;&lt;&apos;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:16})", value, ss), R"(&apos;&amp;&apos;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:17})", value, ss), R"(&lt;&gt;&amp;&apos;&quot;)", R"(Render())",
                     __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:18})", value, ss), R"(&apos;&quot;&lt;&gt;&amp;)", R"(Render())",
                     __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:19})", value, ss), R"(&lt;&quot;&amp;&apos;&gt;)", R"(Render())",
                     __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:20})", value, ss), R"(&lt;&lt;&lt;&lt;&lt;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:21})", value, ss), R"(&gt;&gt;&gt;&gt;&gt;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:22})", value, ss), R"(&amp;&amp;&amp;&amp;&amp;)", R"(Render())",
                     __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:23})", value, ss), R"(&quot;&quot;&quot;&quot;&quot;)", R"(Render())",
                     __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:24})", value, ss), R"(&apos;&apos;&apos;&apos;&apos;)", R"(Render())",
                     __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:25})", value, ss), R"(A&lt;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:26})", value, ss), R"(A&gt;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:27})", value, ss), R"(A&amp;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:28})", value, ss), R"(A&quot;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:29})", value, ss), R"(A&apos;)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:30})", value, ss), R"(&lt;A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:31})", value, ss), R"(&gt;A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:32})", value, ss), R"(&amp;A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:33})", value, ss), R"(&quot;A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:34})", value, ss), R"(&apos;A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:35})", value, ss), R"(A&lt;A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:36})", value, ss), R"(A&gt;A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:37})", value, ss), R"(A&amp;A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:38})", value, ss), R"(A&quot;A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:39})", value, ss), R"(A&apos;A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:40})", value, ss), R"(AA&lt;AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:41})", value, ss), R"(AA&gt;AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:42})", value, ss), R"(AA&amp;AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:43})", value, ss), R"(AA&quot;AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:44})", value, ss), R"(AA&apos;AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:45})", value, ss), R"(AA&lt;&lt;&lt;&lt;AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:46})", value, ss), R"(AA&gt;&gt;&gt;&gt;AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:47})", value, ss), R"(AA&amp;&amp;&amp;&amp;AA)", R"(Render())",
                     __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:48})", value, ss), R"(AA&quot;&quot;&quot;&quot;AA)", R"(Render())",
                     __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:49})", value, ss), R"(AA&apos;&apos;&apos;&apos;AA)", R"(Render())",
                     __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:50})", value, ss),
                     R"(&lt;A&gt;B&apos;C&quot;D&amp;E&apos;F&quot;G&lt;H&gt;I&amp;G&quot;K)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:51})", value, ss), R"(AB&quot;CD&apos;EF&lt;GH&gt;IGK&apos;)",
                     R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:52})", value, ss), R"(&quot;ABC&apos;DEF&lt;GHI&gt;GK&lt;)",
                     R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:53})", value, ss),
                     R"(A&quot;&quot;BC&lt;&lt;DE&gt;&gt;FG&apos;&apos;HI&amp;&amp;GK)", R"(Render())", __LINE__);
        ss.Clear();

    } else {
        helper.Equal(Template::Render(R"({var:0})", value, ss), R"(<)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:1})", value, ss), R"(>)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:2})", value, ss), R"(&)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:3})", value, ss), R"(")", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:4})", value, ss), R"(')", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:5})", value, ss), R"(<>)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:6})", value, ss), R"(<&)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:7})", value, ss), R"(<&)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:8})", value, ss), R"(>")", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:9})", value, ss), R"("')", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:10})", value, ss), R"(<">)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:11})", value, ss), R"(<'>)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:12})", value, ss), R"(<&>)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:13})", value, ss), R"(&"&)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:14})", value, ss), R"("'")", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:15})", value, ss), R"('<')", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:16})", value, ss), R"('&')", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:17})", value, ss), R"(<>&'")", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:18})", value, ss), R"('"<>&)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:19})", value, ss), R"(<"&'>)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:20})", value, ss), R"(<<<<<)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:21})", value, ss), R"(>>>>>)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:22})", value, ss), R"(&&&&&)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:23})", value, ss), R"(""""")", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:24})", value, ss), R"(''''')", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:25})", value, ss), R"(A<)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:26})", value, ss), R"(A>)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:27})", value, ss), R"(A&)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:28})", value, ss), R"(A")", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:29})", value, ss), R"(A')", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:30})", value, ss), R"(<A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:31})", value, ss), R"(>A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:32})", value, ss), R"(&A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:33})", value, ss), R"("A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:34})", value, ss), R"('A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:35})", value, ss), R"(A<A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:36})", value, ss), R"(A>A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:37})", value, ss), R"(A&A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:38})", value, ss), R"(A"A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:39})", value, ss), R"(A'A)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:40})", value, ss), R"(AA<AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:41})", value, ss), R"(AA>AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:42})", value, ss), R"(AA&AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:43})", value, ss), R"(AA"AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:44})", value, ss), R"(AA'AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:45})", value, ss), R"(AA<<<<AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:46})", value, ss), R"(AA>>>>AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:47})", value, ss), R"(AA&&&&AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:48})", value, ss), R"(AA""""AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:49})", value, ss), R"(AA''''AA)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:50})", value, ss), R"(<A>B'C"D&E'F"G<H>I&G"K)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:51})", value, ss), R"(AB"CD'EF<GH>IGK')", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:52})", value, ss), R"("ABC'DEF<GHI>GK<)", R"(Render())", __LINE__);
        ss.Clear();

        helper.Equal(Template::Render(R"({var:53})", value, ss), R"(A""BC<<DE>>FG''HI&&GK)", R"(Render())", __LINE__);
        ss.Clear();
    }
}

static void TestRawVariableTag1(TestHelper &helper) {
    StringStream<char> ss;
    const char        *content;

    Value<char> value = JSON::Parse(R"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    const Value<char> sub_value =
        JSON::Parse(R"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = R"({raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"(A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:1})";
    helper.Equal(Template::Render(content, value, ss), R"(abc)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:2})";
    helper.Equal(Template::Render(content, value, ss), R"(true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:3})";
    helper.Equal(Template::Render(content, value, ss), R"(456)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:4})";
    helper.Equal(Template::Render(content, value, ss), R"(1.5)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:5[0]})";
    helper.Equal(Template::Render(content, value, ss), R"(null)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:5[1]})";
    helper.Equal(Template::Render(content, value, ss), R"(false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:5[2][0]})";
    helper.Equal(Template::Render(content, value, ss), R"(Qentem)", R"(Render())", __LINE__);
    ss.Clear();

    //////

    content = R"({raw:key1})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(a)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:key2})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(ABC)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:key3})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:key4})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(100)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:key5})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(1.5)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:key6[one]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:key7[0]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(null)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:key7[1]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:key7[2][0]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(Qentem)", R"(Render())", __LINE__);
    ss.Clear();

    //
    content = R"({raw:6[key1]})";
    helper.Equal(Template::Render(content, value, ss), R"(a)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:6[key2]})";
    helper.Equal(Template::Render(content, value, ss), R"(ABC)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:6[key3]})";
    helper.Equal(Template::Render(content, value, ss), R"(false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:6[key4]})";
    helper.Equal(Template::Render(content, value, ss), R"(100)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:6[key5]})";
    helper.Equal(Template::Render(content, value, ss), R"(1.5)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:6[key6][one]})";
    helper.Equal(Template::Render(content, value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    ////////////////

    content = R"(-{raw:2})";
    helper.Equal(Template::Render(content, value, ss), R"(-true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{raw:key7[0]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(-null)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{raw:key7[2][0]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(-Qentem)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{raw:6[key3]})";
    helper.Equal(Template::Render(content, value, ss), R"(-false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{raw:6[key4]})";
    helper.Equal(Template::Render(content, value, ss), R"(-100)", R"(Render())", __LINE__);
    ss.Clear();

    ////////////

    content = R"({raw:2}-)";
    helper.Equal(Template::Render(content, value, ss), R"(true-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:key7[0]}-)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(null-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:key7[2][0]}-)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(Qentem-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:6[key3]}-)";
    helper.Equal(Template::Render(content, value, ss), R"(false-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:6[key4]}-)";
    helper.Equal(Template::Render(content, value, ss), R"(100-)", R"(Render())", __LINE__);
    ss.Clear();

    ////////////

    content = R"(-{raw:2}-)";
    helper.Equal(Template::Render(content, value, ss), R"(-true-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{raw:key7[0]}-)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(-null-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{raw:key7[2][0]}-)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(-Qentem-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{raw:6[key3]}-)";
    helper.Equal(Template::Render(content, value, ss), R"(-false-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{raw:6[key4]}-)";
    helper.Equal(Template::Render(content, value, ss), R"(-100-)", R"(Render())", __LINE__);
    ss.Clear();

    ////////////

    content = R"(------{raw:2})";
    helper.Equal(Template::Render(content, value, ss), R"(------true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{raw:key7[0]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(------null)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{raw:key7[2][0]})";
    helper.Equal(Template::Render(content, sub_value, ss), R"(------Qentem)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{raw:6[key3]})";
    helper.Equal(Template::Render(content, value, ss), R"(------false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{raw:6[key4]})";
    helper.Equal(Template::Render(content, value, ss), R"(------100)", R"(Render())", __LINE__);
    ss.Clear();

    ////////////

    content = R"({raw:2}------)";
    helper.Equal(Template::Render(content, value, ss), R"(true------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:key7[0]}------)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(null------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:key7[2][0]}------)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(Qentem------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:6[key3]}------)";
    helper.Equal(Template::Render(content, value, ss), R"(false------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:6[key4]}------)";
    helper.Equal(Template::Render(content, value, ss), R"(100------)", R"(Render())", __LINE__);
    ss.Clear();

    ////////////

    content = R"(------{raw:2}------)";
    helper.Equal(Template::Render(content, value, ss), R"(------true------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{raw:key7[0]}------)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(------null------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{raw:key7[2][0]}------)";
    helper.Equal(Template::Render(content, sub_value, ss), R"(------Qentem------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{raw:6[key3]}------)";
    helper.Equal(Template::Render(content, value, ss), R"(------false------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(------{raw:6[key4]}------)";
    helper.Equal(Template::Render(content, value, ss), R"(------100------)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:key7[2[0]})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:key7[2[0]})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:6key3]})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:6key3]})", R"(Render())", __LINE__);
    ss.Clear();
}

static void TestRawVariableTag2(TestHelper &helper) {
    StringStream<char> ss;
    const Value<char>  value = JSON::Parse(R"(["A", "abc", true, 456, 1.5])");
    const char        *content;

    content = R"({raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"(AA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:1}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"(abcA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:1}{raw:2})";
    helper.Equal(Template::Render(content, value, ss), R"(abctrue)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:2}{raw:3}{raw:2})";
    helper.Equal(Template::Render(content, value, ss), R"(true456true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:4}{raw:4}{raw:4})";
    helper.Equal(Template::Render(content, value, ss), R"(1.51.51.5)", R"(Render())", __LINE__);
    ss.Clear();

    ///

    content = R"({raw:0}-{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"(A-A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:1}--{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"(abc--A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:1}---{raw:2})";
    helper.Equal(Template::Render(content, value, ss), R"(abc---true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:2}{raw:3}--{raw:2})";
    helper.Equal(Template::Render(content, value, ss), R"(true456--true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:4}--{raw:4}{raw:4})";
    helper.Equal(Template::Render(content, value, ss), R"(1.5--1.51.5)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:4}--{raw:4}--{raw:4})";
    helper.Equal(Template::Render(content, value, ss), R"(1.5--1.5--1.5)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:4}---{raw:4}---{raw:4})";
    helper.Equal(Template::Render(content, value, ss), R"(1.5---1.5---1.5)", R"(Render())", __LINE__);
    ss.Clear();
}

static void TestRawVariableTag3(TestHelper &helper) {
    StringStream<char> ss;
    Value<char>        value;
    const char        *content;

    content = R"({raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:0})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:a})", value, ss), R"({raw:a})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:0[0]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value, ss), R"({raw:0[0]})", R"(Render())",
                 __LINE__);
    ss.Clear();

    content = R"({raw:a[0]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value, ss), R"({raw:a[0]})", R"(Render())",
                 __LINE__);
    ss.Clear();

    content = R"({raw:0[a]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value, ss), R"({raw:0[a]})", R"(Render())",
                 __LINE__);
    ss.Clear();

    content = R"({raw:a[abc]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value, ss), R"({raw:a[abc]})", R"(Render())",
                 __LINE__);
    ss.Clear();

    ////////////////

    value = JSON::Parse(R"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = R"({raw:0})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), value, ss), R"({raw:0})", R"(Render())",
                 __LINE__);
    ss.Clear();

    content = R"({raw:0[0]})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:0[0]})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:0[0][0]})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:0[0][0]})", R"(Render())", __LINE__);
    ss.Clear();

    /////

    content = R"({raw:1})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:1})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:2})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:2})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:1[a]})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:1[a]})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:1[b]})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:1[b]})", R"(Render())", __LINE__);
    ss.Clear();

    ////

    value = JSON::Parse(R"(["A", "abc", true, 456, 1.5])");

    content = R"({raw:0)";
    helper.Equal(Template::Render(content, value, ss), R"({raw:0)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"(raw:0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({r})";
    helper.Equal(Template::Render(content, value, ss), R"({r})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({ra})";
    helper.Equal(Template::Render(content, value, ss), R"({ra})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw})";
    helper.Equal(Template::Render(content, value, ss), R"({raw})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({r:0})";
    helper.Equal(Template::Render(content, value, ss), R"({r:0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({ra:0})";
    helper.Equal(Template::Render(content, value, ss), R"({ra:0})", R"(Render())", __LINE__);
    ss.Clear();

    ////

    content = R"({raw:0{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:0{raw:0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(raw:0{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"(raw:0A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"(raw:0}A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:0{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:0{raw:0}A)", R"(Render())", __LINE__);
    ss.Clear();

    ////

    content = R"({raw:0{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:0{raw:0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(raw:0{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"(raw:0A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"(raw:0}A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:0{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({raw:0{raw:0}A)", R"(Render())", __LINE__);
    ss.Clear();

    ////

    content = R"({{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({AA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({r{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({rA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({r{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({rAA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({ra{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({raA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({ra{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({raAA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({rawA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({rawAA)", R"(Render())", __LINE__);
    ss.Clear();

    ///

    content = R"({-{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({-A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({-{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({-AA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({r-{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({r-A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({r-{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({r-AA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({ra-{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({ra-A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({ra-{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({ra-AA)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw-{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({raw-A)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw-{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, value, ss), R"({raw-AA)", R"(Render())", __LINE__);
    ss.Clear();

    //

    content = R"({raw-0})";
    helper.Equal(Template::Render(content, value, ss), R"({raw-0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw 0})";
    helper.Equal(Template::Render(content, value, ss), R"({raw 0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:0 })";
    helper.Equal(Template::Render(content, value, ss), R"({raw:0 })", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:0 )";
    helper.Equal(Template::Render(content, value, ss), R"({raw:0 )", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({raw:0)";
    helper.Equal(Template::Render(content, value, ss), R"({raw:0)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({ raw-0})";
    helper.Equal(Template::Render(content, value, ss), R"({ raw-0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({ raw 0})";
    helper.Equal(Template::Render(content, value, ss), R"({ raw 0})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({ raw:0 })";
    helper.Equal(Template::Render(content, value, ss), R"({ raw:0 })", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({ raw:0 )";
    helper.Equal(Template::Render(content, value, ss), R"({ raw:0 )", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({ raw:0)";
    helper.Equal(Template::Render(content, value, ss), R"({ raw:0)", R"(Render())", __LINE__);
    ss.Clear();
}

static void TestRawVariableTag4(TestHelper &helper) {
    StringStream<char> ss;
    Value<char>        value;

    value += R"(<)";
    value += R"(>)";
    value += R"(&)";
    value += R"(")";
    value += R"(')";
    value += R"(<>)";
    value += R"(<&)";
    value += R"(<&)";
    value += R"(>")";
    value += R"("')";
    value += R"(<">)";
    value += R"(<'>)";
    value += R"(<&>)";
    value += R"(&"&)";
    value += R"("'")";
    value += R"('<')";
    value += R"('&')";
    value += R"(<>&'")";
    value += R"('"<>&)";
    value += R"(<"&'>)";

    value += R"(<<<<<)";
    value += R"(>>>>>)";
    value += R"(&&&&&)";
    value += R"(""""")";
    value += R"(''''')";

    value += R"(A<)";
    value += R"(A>)";
    value += R"(A&)";
    value += R"(A")";
    value += R"(A')";
    value += R"(<A)";
    value += R"(>A)";
    value += R"(&A)";
    value += R"("A)";
    value += R"('A)";
    value += R"(A<A)";
    value += R"(A>A)";
    value += R"(A&A)";
    value += R"(A"A)";
    value += R"(A'A)";

    value += R"(AA<AA)";
    value += R"(AA>AA)";
    value += R"(AA&AA)";
    value += R"(AA"AA)";
    value += R"(AA'AA)";

    value += R"(AA<<<<AA)";
    value += R"(AA>>>>AA)";
    value += R"(AA&&&&AA)";
    value += R"(AA""""AA)";
    value += R"(AA''''AA)";

    value += R"(<A>B'C"D&E'F"G<H>I&G"K)";
    value += R"(AB"CD'EF<GH>IGK')";
    value += R"("ABC'DEF<GHI>GK<)";
    value += R"(A""BC<<DE>>FG''HI&&GK)";

    helper.Equal(Template::Render(R"({raw:0})", value, ss), R"(<)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:1})", value, ss), R"(>)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:2})", value, ss), R"(&)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:3})", value, ss), R"(")", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:4})", value, ss), R"(')", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:5})", value, ss), R"(<>)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:6})", value, ss), R"(<&)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:7})", value, ss), R"(<&)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:8})", value, ss), R"(>")", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:9})", value, ss), R"("')", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:10})", value, ss), R"(<">)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:11})", value, ss), R"(<'>)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:12})", value, ss), R"(<&>)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:13})", value, ss), R"(&"&)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:14})", value, ss), R"("'")", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:15})", value, ss), R"('<')", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:16})", value, ss), R"('&')", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:17})", value, ss), R"(<>&'")", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:18})", value, ss), R"('"<>&)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:19})", value, ss), R"(<"&'>)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:20})", value, ss), R"(<<<<<)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:21})", value, ss), R"(>>>>>)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:22})", value, ss), R"(&&&&&)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:23})", value, ss), R"(""""")", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:24})", value, ss), R"(''''')", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:25})", value, ss), R"(A<)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:26})", value, ss), R"(A>)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:27})", value, ss), R"(A&)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:28})", value, ss), R"(A")", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:29})", value, ss), R"(A')", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:30})", value, ss), R"(<A)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:31})", value, ss), R"(>A)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:32})", value, ss), R"(&A)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:33})", value, ss), R"("A)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:34})", value, ss), R"('A)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:35})", value, ss), R"(A<A)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:36})", value, ss), R"(A>A)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:37})", value, ss), R"(A&A)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:38})", value, ss), R"(A"A)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:39})", value, ss), R"(A'A)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:40})", value, ss), R"(AA<AA)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:41})", value, ss), R"(AA>AA)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:42})", value, ss), R"(AA&AA)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:43})", value, ss), R"(AA"AA)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:44})", value, ss), R"(AA'AA)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:45})", value, ss), R"(AA<<<<AA)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:46})", value, ss), R"(AA>>>>AA)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:47})", value, ss), R"(AA&&&&AA)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:48})", value, ss), R"(AA""""AA)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:49})", value, ss), R"(AA''''AA)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:50})", value, ss), R"(<A>B'C"D&E'F"G<H>I&G"K)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:51})", value, ss), R"(AB"CD'EF<GH>IGK')", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:52})", value, ss), R"("ABC'DEF<GHI>GK<)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({raw:53})", value, ss), R"(A""BC<<DE>>FG''HI&&GK)", R"(Render())", __LINE__);
    ss.Clear();
}

static void TestMathTag1(TestHelper &helper) {
    StringStream<char> ss;
    Value<char>        value;

    value[R"(a1)"] = 5;
    value[R"(a2)"] = true;
    value[R"(a3)"] = nullptr;
    value[R"(a4)"] = false;
    value[R"(a5)"] = R"(10)";
    value[R"(a6)"] = R"(20)";
    value[R"(a7)"] = 6;
    value[R"(a8)"] = 1;
    value[R"(a9)"] = R"(1)";

    helper.Equal(Template::Render(R"({math:1+1})", value, ss), R"(2)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a1}+8})", value, ss), R"(13)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a7}+8})", value, ss), R"(14)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a7}+{var:a1}})", value, ss), R"(11)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a5}+{var:a1}})", value, ss), R"(15)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a1}+{var:a5}})", value, ss), R"(15)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a6}+{var:a5}})", value, ss), R"(30)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a6}*{var:a2}})", value, ss), R"(20)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a6}*{var:a4}})", value, ss), R"(0)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a6}*{var:a7}})", value, ss), R"(120)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a7}+{var:a6}})", value, ss), R"(26)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a1}+{var:a2}})", value, ss), R"(6)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a8}=={var:a2}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a2}=={var:a8}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a5}!={var:a1}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a2}!={var:a4}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a2}==true})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a9}=={var:a8}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a8}=={var:a9}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:1=={var:a8}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:6-5==({var:a9})})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:6-5==({var:a8})})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:null!={var:a3}})", value, ss), R"(0)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:({var:a3})==(0)})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a1}+{var:a7}})", value, ss), R"(11)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a1}+{var:a7}}{math:{var:a1}+{var:a7}})", value, ss), R"(1111)",
                 R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a1}+{var:a7}}*{math:{var:a1}+{var:a7}})", value, ss), R"(11*11)",
                 R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a1}+{var:a7}}##{math:{var:a1}+{var:a7}})", value, ss), R"(11##11)",
                 R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a1}+{var:a7}}&&&%%^^&&*{math:{var:a1}+{var:a7}})", value, ss),
                 R"(11&&&%%^^&&*11)", R"(Render())", __LINE__);
    ss.Clear();

    ///////////////////

    helper.Equal(Template::Render(R"({math: {var:a1}+8})", value, ss), R"(13)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:  {var:a7}+8})", value, ss), R"(14)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:     {var:a7}+{var:a1}})", value, ss), R"(11)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a5} +{var:a1}})", value, ss), R"(15)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a1}  +{var:a5}})", value, ss), R"(15)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a6}    +{var:a5}})", value, ss), R"(30)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a6}* {var:a2}})", value, ss), R"(20)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a6}*  {var:a4}})", value, ss), R"(0)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a6}*      {var:a7}})", value, ss), R"(120)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a7}+{var:a6} })", value, ss), R"(26)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a1}+{var:a2}  })", value, ss), R"(6)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a8}=={var:a2}      })", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a2}=={var:a8}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math: {var:a5}!={var:a1} })", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:  {var:a2}!={var:a4}  })", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:    1=={var:a9}     })", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a9} == {var:a8}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a8}  ==  {var:a9}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:1==          {var:a8}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:6-5         ==1})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:1          ==            {var:a8}})", value, ss), R"(1)", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:     0     !=    ({var:a3})        })", value, ss), R"(0)", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:a3}       ==       null     })", value, ss), R"(1)", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:((8+1+{var:a8}))})", value, ss), R"(10)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:10==(8+1+{var:a8})})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:((8+1+{var:a8}))==9+1})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:(5*2)==((8+1+{var:a8}))})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    //////////////
    value.Reset();

    value += 5;
    value += true;
    value += nullptr;
    value += false;
    value += R"(10)";
    value += R"(20)";
    value += 6;
    value += 1;
    value += R"(1)";
    value += R"(Qentem)";

    helper.Equal(Template::Render(R"({math:{var:0}+8})", value, ss), R"(13)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:6}+8})", value, ss), R"(14)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:6}+{var:0}})", value, ss), R"(11)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:4}+{var:0}})", value, ss), R"(15)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:0}+{var:4}})", value, ss), R"(15)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:5}+{var:4}})", value, ss), R"(30)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:5}*{var:1}})", value, ss), R"(20)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:5}*{var:3}})", value, ss), R"(0)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:5}*{var:6}})", value, ss), R"(120)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:6}+{var:5}})", value, ss), R"(26)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:0}+{var:1}})", value, ss), R"(6)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:7}=={var:1}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:1}=={var:7}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:4}!={var:0}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:1}!={var:3}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:({var:1})==({var:8})})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"(-{math:{var:8}=={var:7}})", value, ss), R"(-1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"(--{math:{var:7}=={var:8}})", value, ss), R"(--1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"(---{math:1=={var:7}})", value, ss), R"(---1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:1==({var:8})}-)", value, ss), R"(1-)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:6-5==({var:7})}--)", value, ss), R"(1--)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:0==({var:2})}---)", value, ss), R"(1---)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"(-{math:{var:2}!=null}-)", value, ss), R"(-0-)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"(--{math:Qente=={var:9}}--)", value, ss), R"(--0--)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"(---{math:Qente !={var:9}}---)", value, ss), R"(---1---)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:  Qentem   =={var:9}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:  Qentem!={var:9}})", value, ss), R"(0)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:9}   ==    Qente})", value, ss), R"(0)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:9} !=    Qente    })", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:  {var:9}   ==Qentem})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math: {var:9} !=Qentem})", value, ss), R"(0)", R"(Render())", __LINE__);
    ss.Clear();

    /////////

    helper.Equal(Template::Render(R"({math: true == {var:1}})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math: {var:1} == true})", value, ss), R"(1)", R"(Render())", __LINE__);
    ss.Clear();
}

static void TestMathTag2(TestHelper &helper) {
    StringStream<char> ss;
    Value<char>        value;

    value += Array<Value<char>>();
    value += HArray<Value<char>, char>();
    value += 5;

    helper.Equal(Template::Render(R"({math:{var:0}+8})", value, ss), R"({math:{var:0}+8})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:1}+8})", value, ss), R"({math:{var:1}+8})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:2}+{var:0}})", value, ss), R"({math:{var:2}+{var:0}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:2}+{var:0}})", value, ss), R"({math:{var:2}+{var:0}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:0}+{var:1}})", value, ss), R"({math:{var:0}+{var:1}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:1}+{var:2}})", value, ss), R"({math:{var:1}+{var:2}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:2}*{var:1}})", value, ss), R"({math:{var:2}*{var:1}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:0}*{var:1}})", value, ss), R"({math:{var:0}*{var:1}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:0}*{var:2}})", value, ss), R"({math:{var:0}*{var:2}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:2}+{var:0}})", value, ss), R"({math:{var:2}+{var:0}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:1}+{var:2}})", value, ss), R"({math:{var:1}+{var:2}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:0}=={var:1}})", value, ss), R"({math:{var:0}=={var:1}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:1}=={var:0}})", value, ss), R"({math:{var:1}=={var:0}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:0}!={var:2}})", value, ss), R"({math:{var:0}!={var:2}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:2}!={var:0}})", value, ss), R"({math:{var:2}!={var:0}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:1}=={var:2}})", value, ss), R"({math:{var:1}=={var:2}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:8}=={var:7}})", value, ss), R"({math:{var:8}=={var:7}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:7}=={var:2}})", value, ss), R"({math:{var:7}=={var:2}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:1=={var:7}})", value, ss), R"({math:1=={var:7}})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:(6-5)=={var:8}})", value, ss), R"({math:(6-5)=={var:8}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:(6-5)=={var:0}})", value, ss), R"({math:(6-5)=={var:0}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:0}=={var:8}})", value, ss), R"({math:{var:0}=={var:8}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:0}=={var:0}})", value, ss), R"({math:{var:0}=={var:0}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:0=={var:1}})", value, ss), R"({math:0=={var:1}})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:1}!=0})", value, ss), R"({math:{var:1}!=0})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:W={var:0}})", value, ss), R"({math:W={var:0}})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:W=={var:0}})", value, ss), R"({math:W=={var:0}})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:2}==c})", value, ss), R"({math:{var:2}==c})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:abc=={var:2}})", value, ss), R"({math:abc=={var:2}})", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:sds})", value, ss), R"({math:sds})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:1)", value, ss), R"({math:1)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"(math:1})", value, ss), R"(math:1})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:{var:2})", value, ss), R"({math:5)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({{math:{var:2}+5})", value, ss), R"({10)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({m{var:2}})", value, ss), R"({m5})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({ma{var:2}})", value, ss), R"({ma5})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({mat{var:2}})", value, ss), R"({mat5})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math{var:2}})", value, ss), R"({math5})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math {var:2}})", value, ss), R"({math 5})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:})", value, ss), R"({math:})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math-{var:2}}{math:{var:2}+5})", value, ss), R"({math-5}10)", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math-4}{math:{var:2}+5})", value, ss), R"({math-4}10)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math-4} {math:{var:2}+5})", value, ss), R"({math-4} 10)", R"(Render())",
                 __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:4)", value, ss), R"({math:4)", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:4    )", value, ss), R"({math:4    )", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:A==1+1})", value, ss), R"({math:A==1+1})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:(A)!=1+1})", value, ss), R"({math:(A)!=1+1})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:1+1==A})", value, ss), R"({math:1+1==A})", R"(Render())", __LINE__);
    ss.Clear();

    helper.Equal(Template::Render(R"({math:1+1!=(A)})", value, ss), R"({math:1+1!=(A)})", R"(Render())", __LINE__);
    ss.Clear();
}

static void TestInlineIfTag(TestHelper &helper) {
    StringStream<char> ss;
    Value<char>        value;
    const char        *content;

    value += 0;
    value += 1;
    value += -1;
    value += true;
    value += false;
    value += nullptr;
    value += R"(ABC)";
    value += Array<Value<char>>();

    content = R"({if case="0" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(F)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="-1" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(F)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="0.1" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="1" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    ////

    content = R"({if case="0" true="T"})";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="-1" true="T"})";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="0.1" true="T"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="1" true="T"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    ///

    content = R"({if case="0" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(F)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="-1" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(F)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="0.1" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="1" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    /////

    content = R"({if case="{var:0}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(F)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="{var:2}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(F)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="{raw:4}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(F)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="{var:5}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(F)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="{var:6}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="fas" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="{var:7}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="{var:20}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    ////
    content = R"({if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="{var:3}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="{var:6}==ABC" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    /////////////////

    content = R"({if case=" {var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="{var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case=" {var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="  {var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="{var:1}  " true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="  {var:1}  " true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="      {var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="{var:1}          " true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="    {var:1}        " true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    ////

    content = R"(-{if case=" {var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(-T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case=" {var:1} " true="T" false="F"}-)";
    helper.Equal(Template::Render(content, value, ss), R"(T-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(-{if case=" {var:1} " true="T" false="F"}-)";
    helper.Equal(Template::Render(content, value, ss), R"(-T-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(--{if case=" {var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(--T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case=" {var:1} " true="T" false="F"}--)";
    helper.Equal(Template::Render(content, value, ss), R"(T--)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(--{if case=" {var:1} " true="T" false="F"}--)";
    helper.Equal(Template::Render(content, value, ss), R"(--T--)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(---{if case=" {var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(---T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case=" {var:1} " true="T" false="F"}---)";
    helper.Equal(Template::Render(content, value, ss), R"(T---)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(---{if case=" {var:1} " true="T" false="F"}---)";
    helper.Equal(Template::Render(content, value, ss), R"(---T---)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="1" true="T"}{if case="0" false="F"}{if case="1" true="T"})";
    helper.Equal(Template::Render(content, value, ss), R"(TFT)", R"(Render())", __LINE__);
    ss.Clear();

    ///////

    content = R"({if case="{var:7}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"(T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="01" true="{var:3}" false="{var:4}"}--)";
    helper.Equal(Template::Render(content, value, ss), R"(--)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="" true="c" false="d"})";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({i)";
    helper.Equal(Template::Render(content, value, ss), R"({i)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if)";
    helper.Equal(Template::Render(content, value, ss), R"({if)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if})";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"({T)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({i{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"({iT)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"({ifT)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="{var:1}"                    put="F"})";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if{if case="1" true="T" false="F"}}{if case="1" true="T" false="F"})";

    helper.Equal(Template::Render(content, value, ss), R"(TT)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if{if case="{raw:1}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, value, ss), R"({ifTT)", R"(Render())", __LINE__);
    ss.Clear();

    /////
    content = R"({if case="0" true="{var:3}" false="{var:4}"})";
    helper.Equal(Template::Render(content, value, ss), R"(false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="1" true="{var:3}" false="{var:4}"})";
    helper.Equal(Template::Render(content, value, ss), R"(true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="0" true="{raw:3}{raw:3}" false="{var:4}{var:4}"})";
    helper.Equal(Template::Render(content, value, ss), R"(falsefalse)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="1000" true="{var:3}{var:3}" false="{var:4}{var:4}"})";
    helper.Equal(Template::Render(content, value, ss), R"(truetrue)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="0" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    helper.Equal(Template::Render(content, value, ss), R"(false---false)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="1" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    helper.Equal(Template::Render(content, value, ss), R"(true---true)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="0" true="{var:10}" false="{var:20}"})";
    helper.Equal(Template::Render(content, value, ss), R"({var:20})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="1" true="{var:10}" false="{var:20}"})";
    helper.Equal(Template::Render(content, value, ss), R"({var:10})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="1" true="1" false="0")";
    helper.Equal(Template::Render(content, value, ss), R"({if case="1" true="1" false="0")", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="1" true="1" false="0")";
    helper.Equal(Template::Render(content, value, ss), R"({if case="1" true="1" false="0")", R"(Render())", __LINE__);
    ss.Clear();

    ///////
    Value<char> value2;

    value2 += R"(&)";
    value2 += R"(")";
    value2 += 10;
    value2 += 5;
    value2 += 15;

    content = R"({if case="1" true="{var:0}" false="{var:1}"})";

    if constexpr (Config::AutoEscapeHTML) {
        helper.Equal(Template::Render(content, value2, ss), R"(&amp;)", R"(Render())", __LINE__);
        ss.Clear();

    } else {
        helper.Equal(Template::Render(content, value2, ss), R"(&)", R"(Render())", __LINE__);
        ss.Clear();
    }

    content = R"({if case="1" true="{raw:0}" false="{raw:1}"})";
    helper.Equal(Template::Render(content, value2, ss), R"(&)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="0" true="{var:0}" false="{var:1}"})";

    if constexpr (Config::AutoEscapeHTML) {
        helper.Equal(Template::Render(content, value2, ss), R"(&quot;)", R"(Render())", __LINE__);
        ss.Clear();

    } else {
        helper.Equal(Template::Render(content, value2, ss), R"(")", R"(Render())", __LINE__);
        ss.Clear();
    }

    content = R"({if case="0" true="*{raw:0}*" false="-{raw:1}-"})";
    helper.Equal(Template::Render(content, value2, ss), R"(-"-)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="0" true="{raw:0}" false="{raw:1}"})";
    helper.Equal(Template::Render(content, value2, ss), R"(")", R"(Render())", __LINE__);
    ss.Clear();

    content = R"({if case="{var:2}+{var:3} == {var:4}" true="{math:{var:3}+{var:4}}" false="{math: {var:2}+{var:4}}"})";
    helper.Equal(Template::Render(content, value2, ss), R"(20)", R"(Render())", __LINE__);
    ss.Clear();

    content =
        R"({if case=" {var:4}-{var:3} != {var:2} " true="{math: {var:3}+{var:4} } " false=" {math: {var:2}+{var:4} } "})";
    helper.Equal(Template::Render(content, value2, ss), R"( 25 )", R"(Render())", __LINE__);
    ss.Clear();
}

static void TestLoopTag1(TestHelper &helper) {
    StringStream<char> ss;
    Value<char>        value3;
    const char        *content;
    Value<char>        value1;

    value1 += 100;
    value1 += -50;
    value1 += R"(Qentem)";
    value1 += true;
    value1 += false;
    value1 += nullptr;
    value1 += 3;

    content = R"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value1, ss), R"(100, -50, Qentem, true, false, null, 3, )", R"(Render())",
                 __LINE__);
    ss.Clear();

    content = R"(<loop value="loop1-value">{var:loop1-value}, {var:loop1-value} </loop>)";
    helper.Equal(Template::Render(content, value1, ss),
                 R"(100, 100 -50, -50 Qentem, Qentem true, true false, false null, null 3, 3 )", R"(Render())",
                 __LINE__);
    ss.Clear();

    ////////////////

    value1.Reset();
    value1 += 0;
    value1 += 1;

    content =
        R"(<loop value="loop1-value"><loop value="loop2-value">({var:loop1-value}: {var:loop2-value}) </loop></loop>)";
    helper.Equal(Template::Render(content, value1, ss), R"((0: 0) (0: 1) (1: 0) (1: 1) )", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop value="loop1-value"><loop value="loop2-value"><loop
                 value="loop3-value">({var:loop1-value}: {var:loop2-value}: {var:loop3-value}) </loop></loop></loop>)";
    helper.Equal(Template::Render(content, value1, ss),
                 R"((0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1) )", R"(Render())",
                 __LINE__);
    ss.Clear();

    content = R"(<loop value="loop1-value"><loop value="loop2-value"><loop
                 value="loop3-value"><loop value="loop4-value">({var:loop1-value}: {var:loop2-value}: {var:loop3-value}: {var:loop4-value}) </loop></loop></loop></loop>)";
    helper.Equal(
        Template::Render(content, value1, ss),
        R"((0: 0: 0: 0) (0: 0: 0: 1) (0: 0: 1: 0) (0: 0: 1: 1) (0: 1: 0: 0) (0: 1: 0: 1) (0: 1: 1: 0) (0: 1: 1: 1) (1: 0: 0: 0) (1: 0: 0: 1) (1: 0: 1: 0) (1: 0: 1: 1) (1: 1: 0: 0) (1: 1: 0: 1) (1: 1: 1: 0) (1: 1: 1: 1) )",
        R"(Render())", __LINE__);
    ss.Clear();

    //////////////////////

    value1 = JSON::Parse(R"([100, -50, "A", true, false, null])");
    const Value<char> value2 =
        JSON::Parse(R"({"k-1": 4, "k-2":1.5, "k-3":"ABC", "k-4":true, "k-5":false, "k-6":null})");

    //////////////////////
    value3[R"(arr1)"] = value1;

    content = R"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value1, ss), R"(100, -50, A, true, false, null, )", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value2, ss), R"(4, 1.5, ABC, true, false, null, )", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop set="arr1" value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3, ss), R"(100, -50, A, true, false, null, )", R"(Render())", __LINE__);
    ss.Clear();

    value3[R"(arr1)"] = value2;

    content = R"(<loop set="arr1" value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3, ss), R"(4, 1.5, ABC, true, false, null, )", R"(Render())", __LINE__);
    ss.Clear();

    //////////////////////
    value3[R"(arr1)"] = value1;

    content = R"(<loop value="loop1-value" >{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value1, ss), R"(100, -50, A, true, false, null, )", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value2, ss), R"(4, 1.5, ABC, true, false, null, )", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop value="loop1-value"set="arr1">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3, ss), R"(100, -50, A, true, false, null, )", R"(Render())", __LINE__);
    ss.Clear();

    value3[R"(arr1)"] = value2;

    content = R"(<loop set="arr1" value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3, ss), R"(4, 1.5, ABC, true, false, null, )", R"(Render())", __LINE__);
    ss.Clear();

    //////////////////////
    value3.Reset();
    value3[R"(arr1)"][R"(arr2)"][R"(arr3)"] = value1;

    content = R"(<loop set="arr1[arr2][arr3]" value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3, ss), R"(100, -50, A, true, false, null, )", R"(Render())", __LINE__);
    ss.Clear();

    value3.Reset();
    value3[0][0] += value2;

    content = R"(<loop set="0[0][0]"value="loop1-value">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3, ss), R"(4, 1.5, ABC, true, false, null, )", R"(Render())", __LINE__);
    ss.Clear();

    value3.Reset();
    value3[R"(k1)"][0][R"(k3)"] = value1;

    content = R"(<loop value="loop1-value" set="k1[0][k3]">{var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3, ss), R"(100, -50, A, true, false, null, )", R"(Render())", __LINE__);
    ss.Clear();

    value3.Reset();
    value3[0][R"(k2)"] += value2;

    content = R"(<loop set="0[k2][0]"value="loop1-value">{var:loop1-value}, {var:loop1-value}, </loop>)";
    helper.Equal(Template::Render(content, value3, ss),
                 R"(4, 4, 1.5, 1.5, ABC, ABC, true, true, false, false, null, null, )", R"(Render())", __LINE__);
    ss.Clear();

    value3 = JSON::Parse(R"({"group":[[10],[20],[30]]})");

    content = R"(<loop set="group" value="_Val1"><loop set="_Val1" value="_Val2">{var:_Val2}</loop></loop>)";
    helper.Equal(Template::Render(content, value3, ss), R"(102030)", R"(Render())", __LINE__);
    ss.Clear();

    value3 = JSON::Parse(R"({"group":[1,2,3,4]})");

    content = R"(<loop set="group" value="_Val">{var:_Val}</loop>)";
    helper.Equal(Template::Render(content, value3, ss), R"(1234)", R"(Render())", __LINE__);
    ss.Clear();

    value3  = JSON::Parse(R"({"numbers":[1,2,3,4,5,6,7,8]})");
    content = R"(A<loop set="numbers" value="l_id1">{var:l_id1}</loop>B)";
    helper.Equal(Template::Render(content, value3, ss), R"(A12345678B)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop set="numbers" value="l_id1">{var:l_id1[0]}</loop>)";
    helper.Equal(
        Template::Render(content, value3, ss),
        R"({var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]}{var:l_id1[0]})",
        R"(Render())", __LINE__);
    ss.Clear();
}

static void TestLoopTag2(TestHelper &helper) {
    StringStream<char> ss;
    Value<char>        value;
    const char        *content;

    content = R"(<loop></loop>)";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop>abcd</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<l</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"(<l</loop>)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<lo</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"(<lo</loop>)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loo</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"(<loo</loop>)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop></loop><loop>A</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop value="a">{var:a}</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop set="ss" value="a">{var:a}</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop set="" value="a">{var:a}</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    value[R"(in)"] = Array<Value<char>>();

    content = R"(<loop value="v">{var:v}</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"(in)", R"(Render())", __LINE__);
    ss.Clear();

    value.Reset();

    /////
    value.Reset();
    value[R"(k1)"] = 10;
    value[R"(k2)"] = 20;
    value[R"(k3)"] = 30;

    value[R"(k2)"].Reset();

    content = R"(<loop value="v">{var:v}</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"(1030)", R"(Render())", __LINE__);
    ss.Clear();

    value.RemoveIndex(1);

    content = R"(<loop value="v">{var:v})";
    helper.Equal(Template::Render(content, value, ss), R"(<loop value="v">{var:v})", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop value="v">{var:v}     )";
    helper.Equal(Template::Render(content, value, ss), R"(<loop value="v">{var:v}     )", R"(Render())", __LINE__);
    ss.Clear();

    value.Reset();
    value += 10;
    value += 20;
    value += 30;

    value.RemoveIndex(1);

    content = R"(<loop value="v">{var:v}</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"(1030)", R"(Render())", __LINE__);
    ss.Clear();

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
        R"(<loop set="object" value="item">{var:item[var1]}{var:item[var2]}{var:item[var3]} {var:item[var4]}</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"(value1value2value3 value4)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop set="array" value="item"> {var:item[0]} {var:item[1]} {var:item[2]} {var:item[3]} </loop>)";
    helper.Equal(Template::Render(content, value, ss), R"( value10 value20 value30 value40 )", R"(Render())", __LINE__);
    ss.Clear();

    content =
        R"(<loop set="object" value="item">{var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]}</loop>)";
    helper.Equal(Template::Render(content, value, ss),
                 R"({var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]})", R"(Render())", __LINE__);
    ss.Clear();

    content =
        R"(<loop set="array" value="item">{var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]}</loop>)";
    helper.Equal(Template::Render(content, value, ss),
                 R"({var:item[var11]}{var:item[var22]}{var:item[var33]} {var:item[var44]})", R"(Render())", __LINE__);
    ss.Clear();

    value.RemoveIndex(0);
    content = R"(<loop><l</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"(<l)", R"(Render())", __LINE__);
    ss.Clear();

    value = JSON::Parse(R"(
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

    content = R"(<loop set="2020">{var:name}</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"(some_valsome_valsome_val)", R"(Render())", __LINE__);
    ss.Clear();

    constexpr SizeT32 size_4 = (8 * 4);

    StringStream<char> content2;
    StringStream<char> output;
    Value<char>        value2;

    for (SizeT32 i = 0; i < size_4; i++) {
        value2 += i;
    }

    content2 += R"(<loop value="loop1-value">A {var:loop1-value} B</loop>)";
    for (SizeT32 i = 0; i < size_4; i++) {
        output += R"(A )";
        Digit::NumberToString(output, i);
        output += R"( B)";
    }

    helper.Equal(Template::Render(content2.First(), content2.Length(), value2, ss), output, R"(Render())", __LINE__);
    ss.Clear();
}

static void TestLoopTag3(TestHelper &helper) {
    StringStream<char> ss;
    const char        *content;
    Value<char>        value = JSON::Parse(R"(
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
        R"(<loop value="val1_" group="year111" sort="descend"><loop set="val1_" value="val2_"><loop set="val2_" value="val3_">{var:val3_}</loop></loop></loop>)";

    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content =
        R"(<loop value="val1_" group="year" sort="descend"><loop set="val1_" value="val2_"><loop set="val2_" value="val3_">{var:val3_}</loop></loop></loop>)";

    helper.Equal(Template::Render(content, value, ss),
                 R"(q11400q11450q11450q11100q11125q21200q22300q21200q22300q22300)", R"(Render())", __LINE__);
    ss.Clear();

    content =
        R"(<loop value="val1_" group="year" sort="descend"><loop set="val1_" value="val2_" group="quarter" sort="ascend"><loop set="val2_" value="val3_"><loop set="val3_" value="val4_">{var:val4_}</loop></loop></loop></loop>)";

    helper.Equal(Template::Render(content, value, ss), R"(1400145014501100112512002300120023002300)", R"(Render())",
                 __LINE__);
    ss.Clear();

    content =
        R"(<loop value="val1_" group="year" sort="descend">-- {var:val1_}-<loop set="val1_" value="val2_" group="quarter" sort="ascend">{var:val2_}-<loop set="val2_" value="val3_" group="week" sort="ascend">{var:val2_}:<loop set="val3_" value="val4_"><loop set="val4_" value="val5_"> {var:val5_}</loop></loop></loop></loop></loop>)";

    helper.Equal(
        Template::Render(content, value, ss),
        R"(-- 2020-q1-1: 400 450 450-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2018-q2-1: 2002: 300-- 2017-q2-2: 300)",
        R"(Render())", __LINE__);
    ss.Clear();

    content =
        R"(<loop value="val1_" group="year">-- {var:val1_}-<loop set="val1_" value="val2_" group="quarter">{var:val2_}-<loop set="val2_" value="val3_" group="week">{var:val2_}:<loop set="val3_" value="val4_"><loop set="val4_" value="val5_"> {var:val5_}</loop></loop></loop></loop></loop>)";

    helper.Equal(
        Template::Render(content, value, ss),
        R"(-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2017-q2-2: 300-- 2020-q1-1: 400 450 450-- 2018-q2-1: 2002: 300)",
        R"(Render())", __LINE__);
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

    content = R"(<loop value="val1_" sort="a">{var:val1_}</loop>)";

    helper.Equal(Template::Render(content, value, ss), R"(1234567)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop value="val1_" sort="d">{var:val1_}</loop>)";

    helper.Equal(Template::Render(content, value, ss), R"(7654321)", R"(Render())", __LINE__);
    ss.Clear();
}

static void TestIfTag1(TestHelper &helper) {
    StringStream<char> ss;
    Value<char>        value;
    const char        *content;

    value[R"(name)"] = R"(Qen)";
    value[R"(t)"]    = true;
    value[R"(f)"]    = false;
    value[R"(n)"]    = nullptr;
    value[R"(1)"]    = 1;
    value[R"(one)"]  = R"(1)";
    value[R"(zero)"] = 0;

    content = R"(<if case="1>0">{var:name}</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(Qen)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(#<if case="{var:one}">{var:name}</if>#)";
    helper.Equal(Template::Render(content, value, ss), R"(#Qen#)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(##<if case="{var:zero}">{var:name}</if>##)";
    helper.Equal(Template::Render(content, value, ss), R"(####)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="{var:1}">{var:name}1<else />{var:name}2</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(Qen1)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="{var:f}">{var:name}1<else />{var:name}2</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(Qen2)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(#<if case="{var:t}">G<if case="1">oo</if>d</if>!#)";
    helper.Equal(Template::Render(content, value, ss), R"(#Good!#)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(###<if case="1">G<if case="1">o</if>o<if case="1">d!</if></if>###)";
    helper.Equal(Template::Render(content, value, ss), R"(###Good!###)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(Be <if case="1">G<if case="1">oo<if case="1">d</if></if></if>!)";
    helper.Equal(Template::Render(content, value, ss), R"(Be Good!)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="1">Good!<elseif case="0" />Bad!</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(Good!)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="{var:n}">Bad!<elseif case="1" />Good!</if>#)";
    helper.Equal(Template::Render(content, value, ss), R"(Good!#)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="{var:f}">Bad!<elseif case="0" />Very Bad!</if>)";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(#<if case="0">Bad!<elseif case="0" />Very Bad!<else />Very Good!</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(#Very Good!)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="1">a<else />b</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(a)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="0">a<else />b</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(b)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="0">a<else /><if case="1">b</if></if>)";
    helper.Equal(Template::Render(content, value, ss), R"(b)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="1">a<else /><if case="1">b</if></if>)";
    helper.Equal(Template::Render(content, value, ss), R"(a)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="0">a<if case="1">b</if>c</if>Empty)";
    helper.Equal(Template::Render(content, value, ss), R"(Empty)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="1">a<else /><if case="1">b</if>c</if>===========)";
    helper.Equal(Template::Render(content, value, ss), R"(a===========)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(===========<if case="1">a<if case="1">b</if><else />c</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(===========ab)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="1">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(a)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="0">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(b)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(===========<if case="0">a<elseif case="0" />b<elseif case="1" />c<else />d</if>===========)";
    helper.Equal(Template::Render(content, value, ss), R"(===========c===========)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="0">a<elseif case="0" />b<elseif case="0" />c<else />d</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(d)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="1">a<if case="0">b<elseif case="0"/>c</if></if>)";
    helper.Equal(Template::Render(content, value, ss), R"(a)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="1">a<if case="1">b<elseif case="0"/>c</if></if>)";
    helper.Equal(Template::Render(content, value, ss), R"(ab)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="1"><if case="1">b<elseif case="c"/>c</if>a<else />c</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(ba)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="0">a<else />c<if case="1">b<elseif case="0"/>c</if></if>)";
    helper.Equal(Template::Render(content, value, ss), R"(cb)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if_case="1"><if case="0">Bad1!<elseif case="0" />Bad2!</if>a</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(a)", R"(Render())", __LINE__);
    ss.Clear();

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
    helper.Equal(String<char>::Trim(Template::Render(content, value, ss).GetString()), R"(a)", R"(Render())", __LINE__);
    ss.Clear();

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
    helper.Equal(String<char>::Trim(Template::Render(content, value, ss).GetString()), R"(b)", R"(Render())", __LINE__);
    ss.Clear();

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
    helper.Equal(String<char>::Trim(Template::Render(content, value, ss).GetString()), R"(c)", R"(Render())", __LINE__);
    ss.Clear();

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
    helper.Equal(String<char>::Trim(Template::Render(content, value, ss).GetString()), R"(d)", R"(Render())", __LINE__);
    ss.Clear();
}

static void TestIfTag2(TestHelper &helper) {
    StringStream<char> ss;
    Value<char>        value;
    const char        *content;

    value[R"(name)"] = R"(Qentem)";

    content = R"(<if case="1">{var:name})";
    helper.Equal(Template::Render(content, value, ss), R"(<if case="1">Qentem)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if<if case="1">{var:name}</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(<ifQentem)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="1"><if case="1">{var:name}</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(<if case="1">Qentem)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="1"><if case="1"><if case="1">{var:name}</if></if>)";
    helper.Equal(Template::Render(content, value, ss), R"(<if case="1">Qentem)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="ABC">{var:name}</if>)";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if>{var:name}</if>)";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="0"><elseif />{var:name}</if>)";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<iw case="0">{var:name}</if>)";
    helper.Equal(Template::Render(content, value, ss), R"(<iw case="0">Qentem</if>)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="0"{var:name}</if>)";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<if case="0"><else {var:name}</if>)";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();
}

static void TestRender1(TestHelper &helper) {
    constexpr SizeT32 size_4 = (8 * 4);

    StringStream<char> ss;
    StringStream<char> content;
    StringStream<char> output;
    String<char>       str;
    Value<char>        value;

    for (SizeT32 i = 0; i < size_4; i++) {
        value += i;
    }

    for (SizeT32 i = 0; i < size_4; i++) {
        content += R"({var:)";
        str.Reset();
        Digit::NumberToString(str, i);
        content += str;
        output += str;
        content += R"(})";
    }

    helper.Equal(Template::Render(content.First(), content.Length(), value, ss), output, R"(Render())", __LINE__);
    ss.Clear();

    content.Clear();
    output.Clear();

    SizeT32 size = 8;
    for (SizeT32 i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += R"({var:)";
            str.Reset();
            Digit::NumberToString(str, i);
            content += str;
            content += R"(})";

            output += str;
        } else {
            size += 8;

            content += R"({math: 1 + )";
            str.Reset();
            Digit::NumberToString(str, i);
            content += str;
            content += R"(})";

            Digit::NumberToString(output, 1U + i);
        }
    }

    helper.Equal(Template::Render(content.First(), content.Length(), value, ss), output, R"(Render())", __LINE__);
    ss.Clear();

    content.Clear();
    output.Clear();

    size = 8;
    for (SizeT32 i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += R"({var:)";
            str.Reset();
            Digit::NumberToString(str, i);
            content += str;
            content += R"(})";

            output += str;
        } else {
            size += 8;

            content += R"({if case="{var:)";
            Digit::NumberToString(content, i);
            content += R"(}<)";
            Digit::NumberToString(content, 1U + i);
            content += R"(" true="yes"})";

            output += R"(yes)";
        }
    }

    helper.Equal(Template::Render(content.First(), content.Length(), value, ss), output, R"(Render())", __LINE__);
    ss.Clear();

    content.Clear();
    output.Clear();

    size = 8;
    for (SizeT32 i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += R"({var:)";
            str.Reset();
            Digit::NumberToString(str, i);
            content += str;
            content += R"(})";

            output += str;
        } else {
            size += 8;
            content += R"(<if case="1">A</if>)";
            output += R"(A)";
        }
    }

    helper.Equal(Template::Render(content.First(), content.Length(), value, ss), output, R"(Render())", __LINE__);
    ss.Clear();
}

static void TestRender2(TestHelper &helper) {
    StringStream<char> ss;
    Value<char>        value;
    const char        *content;

    value += 0;
    value += 1;
    value += 2;
    value += 5;
    value += 10;

    content = R"(<loop value="loop1_val">{var:~loop1_val[0 </loop>)";
    helper.Equal(Template::Render(content, value, ss),
                 R"({var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 )",
                 R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop value="loop1_val">{var:loop1_val[]}</loop>)";
    helper.Equal(Template::Render(content, value, ss),
                 R"({var:loop1_val[]}{var:loop1_val[]}{var:loop1_val[]}{var:loop1_val[]}{var:loop1_val[]})",
                 R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop set="numbers" value="val_">{var:val_}</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"()", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop value="this_number"><if case="({var:this_number} % 2) == 1">{var:this_number},</if></loop>)";
    helper.Equal(Template::Render(content, value, ss), R"(1,5,)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop value="loop1_val">{if case="{var:loop1_val} < 5", true="{var:loop1_val}"}</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"(012)", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop value="loop1_val">{if case="{var:loop1_val} < 5", true="{var:4}"}</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"(101010)", R"(Render())", __LINE__);
    ss.Clear();

    value = JSON::Parse(R"([[[1,2,3]]])");

    content = R"(<loop value="loop1_val">{var:loop1_val[0][2]}</loop>)";
    helper.Equal(Template::Render(content, value, ss), R"(3)", R"(Render())", __LINE__);
    ss.Clear();

    value = Qentem::JSON::Parse(R"({"abc": [0,10,300], "xyz":[[1],[2],[3]]})");

    content = R"(<loop set="xyz" value="lvar"> {var:lvar[0]} </loop>)";
    helper.Equal(Template::Render(content, value, ss), R"( 1  2  3 )", R"(Render())", __LINE__);
    ss.Clear();

    content = R"(<loop set="xyz" value="lvar"><loop set="lvar" value="lvar2"> {math:{var:lvar2}+3} </loop></loop>)";
    helper.Equal(Template::Render(content, value, ss), R"( 4  5  6 )", R"(Render())", __LINE__);
    ss.Clear();
}

static int RunTemplateTests() {
    TestHelper helper{"Template.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("Variable Tag Test 1", TestVariableTag1);
    helper.Test("Variable Tag Test 2", TestVariableTag2);
    helper.Test("Variable Tag Test 3", TestVariableTag3);
    helper.Test("Variable Tag Test 4", TestVariableTag4);

    helper.Test("Raw Variable Tag Test 1", TestRawVariableTag1);
    helper.Test("Raw Variable Tag Test 2", TestRawVariableTag2);
    helper.Test("Raw Variable Tag Test 3", TestRawVariableTag3);
    helper.Test("Raw Variable Tag Test 4", TestRawVariableTag4);

    helper.Test("Math Tag Test 1", TestMathTag1);
    helper.Test("Math Tag Test 2", TestMathTag2);

    helper.Test("Inline if Tag Test", TestInlineIfTag);

    helper.Test("Loop Tag Test 1", TestLoopTag1);
    helper.Test("Loop Tag Test 2", TestLoopTag2);
    helper.Test("Loop Tag Test 3", TestLoopTag3);

    helper.Test("If Tag Test 1", TestIfTag1);
    helper.Test("If Tag Test 2", TestIfTag2);

    helper.Test("Render Test 1", TestRender1);
    helper.Test("Render Test 2", TestRender2);

    return helper.EndTests();
}

} // namespace Qentem::Test

#endif
