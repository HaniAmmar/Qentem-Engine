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

#include "JSON.hpp"
#include "Template.hpp"

#ifndef QENTEM_TEMPLATE_L_TESTS_H_
#define QENTEM_TEMPLATE_L_TESTS_H_

namespace Qentem {
namespace Test {

void TestVariableLTag1(TestHelper &helper) {
    const char32_t *content;

    Value<char32_t> value = JSON::Parse(UR"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    Value<char32_t> sub_value =
        JSON::Parse(UR"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = UR"({var:0})";
    helper.Equal(Template::Render(content, &value), UR"(A)", UR"(Render())", __LINE__);

    content = UR"({var:1})";
    helper.Equal(Template::Render(content, &value), UR"(abc)", UR"(Render())", __LINE__);

    content = UR"({var:2})";
    helper.Equal(Template::Render(content, &value), UR"(true)", UR"(Render())", __LINE__);

    content = UR"({var:3})";
    helper.Equal(Template::Render(content, &value), UR"(456)", UR"(Render())", __LINE__);

    content = UR"({var:4})";
    helper.Equal(Template::Render(content, &value), UR"(1.5)", UR"(Render())", __LINE__);

    content = UR"({var:5[0]})";
    helper.Equal(Template::Render(content, &value), UR"(null)", UR"(Render())", __LINE__);

    content = UR"({var:5[1]})";
    helper.Equal(Template::Render(content, &value), UR"(false)", UR"(Render())", __LINE__);

    content = UR"({var:5[2][0]})";
    helper.Equal(Template::Render(content, &value), UR"(Qentem)", UR"(Render())", __LINE__);

    //////

    content = UR"({var:key1})";
    helper.Equal(Template::Render(content, &sub_value), UR"(a)", UR"(Render())", __LINE__);

    content = UR"({var:key2})";
    helper.Equal(Template::Render(content, &sub_value), UR"(ABC)", UR"(Render())", __LINE__);

    content = UR"({var:key3})";
    helper.Equal(Template::Render(content, &sub_value), UR"(false)", UR"(Render())", __LINE__);

    content = UR"({var:key4})";
    helper.Equal(Template::Render(content, &sub_value), UR"(100)", UR"(Render())", __LINE__);

    content = UR"({var:key5})";
    helper.Equal(Template::Render(content, &sub_value), UR"(1.5)", UR"(Render())", __LINE__);

    content = UR"({var:key6[one]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(1)", UR"(Render())", __LINE__);

    content = UR"({var:key7[0]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(null)", UR"(Render())", __LINE__);

    content = UR"({var:key7[1]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(false)", UR"(Render())", __LINE__);

    content = UR"({var:key7[2][0]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(Qentem)", UR"(Render())", __LINE__);

    //
    content = UR"({var:6[key1]})";
    helper.Equal(Template::Render(content, &value), UR"(a)", UR"(Render())", __LINE__);

    content = UR"({var:6[key2]})";
    helper.Equal(Template::Render(content, &value), UR"(ABC)", UR"(Render())", __LINE__);

    content = UR"({var:6[key3]})";
    helper.Equal(Template::Render(content, &value), UR"(false)", UR"(Render())", __LINE__);

    content = UR"({var:6[key4]})";
    helper.Equal(Template::Render(content, &value), UR"(100)", UR"(Render())", __LINE__);

    content = UR"({var:6[key5]})";
    helper.Equal(Template::Render(content, &value), UR"(1.5)", UR"(Render())", __LINE__);

    content = UR"({var:6[key6][one]})";
    helper.Equal(Template::Render(content, &value), UR"(1)", UR"(Render())", __LINE__);

    ////////////////

    content = UR"(-{var:2})";
    helper.Equal(Template::Render(content, &value), UR"(-true)", UR"(Render())", __LINE__);

    content = UR"(-{var:key7[0]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(-null)", UR"(Render())", __LINE__);

    content = UR"(-{var:key7[2][0]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(-Qentem)", UR"(Render())", __LINE__);

    content = UR"(-{var:6[key3]})";
    helper.Equal(Template::Render(content, &value), UR"(-false)", UR"(Render())", __LINE__);

    content = UR"(-{var:6[key4]})";
    helper.Equal(Template::Render(content, &value), UR"(-100)", UR"(Render())", __LINE__);
    ////////////

    content = UR"({var:2}-)";
    helper.Equal(Template::Render(content, &value), UR"(true-)", UR"(Render())", __LINE__);

    content = UR"({var:key7[0]}-)";
    helper.Equal(Template::Render(content, &sub_value), UR"(null-)", UR"(Render())", __LINE__);

    content = UR"({var:key7[2][0]}-)";
    helper.Equal(Template::Render(content, &sub_value), UR"(Qentem-)", UR"(Render())", __LINE__);

    content = UR"({var:6[key3]}-)";
    helper.Equal(Template::Render(content, &value), UR"(false-)", UR"(Render())", __LINE__);

    content = UR"({var:6[key4]}-)";
    helper.Equal(Template::Render(content, &value), UR"(100-)", UR"(Render())", __LINE__);

    ////////////

    content = UR"(-{var:2}-)";
    helper.Equal(Template::Render(content, &value), UR"(-true-)", UR"(Render())", __LINE__);

    content = UR"(-{var:key7[0]}-)";
    helper.Equal(Template::Render(content, &sub_value), UR"(-null-)", UR"(Render())", __LINE__);

    content = UR"(-{var:key7[2][0]}-)";
    helper.Equal(Template::Render(content, &sub_value), UR"(-Qentem-)", UR"(Render())", __LINE__);

    content = UR"(-{var:6[key3]}-)";
    helper.Equal(Template::Render(content, &value), UR"(-false-)", UR"(Render())", __LINE__);

    content = UR"(-{var:6[key4]}-)";
    helper.Equal(Template::Render(content, &value), UR"(-100-)", UR"(Render())", __LINE__);

    ////////////

    content = UR"(------{var:2})";
    helper.Equal(Template::Render(content, &value), UR"(------true)", UR"(Render())", __LINE__);

    content = UR"(------{var:key7[0]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(------null)", UR"(Render())", __LINE__);

    content = UR"(------{var:key7[2][0]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(------Qentem)", UR"(Render())", __LINE__);

    content = UR"(------{var:6[key3]})";
    helper.Equal(Template::Render(content, &value), UR"(------false)", UR"(Render())", __LINE__);

    content = UR"(------{var:6[key4]})";
    helper.Equal(Template::Render(content, &value), UR"(------100)", UR"(Render())", __LINE__);

    ////////////

    content = UR"({var:2}------)";
    helper.Equal(Template::Render(content, &value), UR"(true------)", UR"(Render())", __LINE__);

    content = UR"({var:key7[0]}------)";
    helper.Equal(Template::Render(content, &sub_value), UR"(null------)", UR"(Render())", __LINE__);

    content = UR"({var:key7[2][0]}------)";
    helper.Equal(Template::Render(content, &sub_value), UR"(Qentem------)", UR"(Render())", __LINE__);

    content = UR"({var:6[key3]}------)";
    helper.Equal(Template::Render(content, &value), UR"(false------)", UR"(Render())", __LINE__);

    content = UR"({var:6[key4]}------)";
    helper.Equal(Template::Render(content, &value), UR"(100------)", UR"(Render())", __LINE__);

    ////////////

    content = UR"(------{var:2}------)";
    helper.Equal(Template::Render(content, &value), UR"(------true------)", UR"(Render())", __LINE__);

    content = UR"(------{var:key7[0]}------)";
    helper.Equal(Template::Render(content, &sub_value), UR"(------null------)", UR"(Render())", __LINE__);

    content = UR"(------{var:key7[2][0]}------)";
    helper.Equal(Template::Render(content, &sub_value), UR"(------Qentem------)", UR"(Render())", __LINE__);

    content = UR"(------{var:6[key3]}------)";
    helper.Equal(Template::Render(content, &value), UR"(------false------)", UR"(Render())", __LINE__);

    content = UR"(------{var:6[key4]}------)";
    helper.Equal(Template::Render(content, &value), UR"(------100------)", UR"(Render())", __LINE__);

    content = UR"({var:key7[2[0]})";
    helper.Equal(Template::Render(content, &value), UR"({var:key7[2[0]})", UR"(Render())", __LINE__);

    content = UR"({var:6key3]})";
    helper.Equal(Template::Render(content, &value), UR"({var:6key3]})", UR"(Render())", __LINE__);

    content = UR"({var:~2})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({var:~~~~~~~2})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({var:~2]})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({var:~[2]]})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({var:~~~~~~~~2]})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({var:~~~~~~~~[2]})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);
}

void TestVariableLTag2(TestHelper &helper) {
    Value<char32_t> value = JSON::Parse(UR"(["A", "abc", true, 456, 1.5])");
    const char32_t *content;

    content = UR"({var:0}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"(AA)", UR"(Render())", __LINE__);

    content = UR"({var:1}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"(abcA)", UR"(Render())", __LINE__);

    content = UR"({var:1}{var:2})";
    helper.Equal(Template::Render(content, &value), UR"(abctrue)", UR"(Render())", __LINE__);

    content = UR"({var:2}{var:3}{var:2})";
    helper.Equal(Template::Render(content, &value), UR"(true456true)", UR"(Render())", __LINE__);

    content = UR"({var:4}{var:4}{var:4})";
    helper.Equal(Template::Render(content, &value), UR"(1.51.51.5)", UR"(Render())", __LINE__);
    ///

    content = UR"({var:0}-{var:0})";
    helper.Equal(Template::Render(content, &value), UR"(A-A)", UR"(Render())", __LINE__);

    content = UR"({var:1}--{var:0})";
    helper.Equal(Template::Render(content, &value), UR"(abc--A)", UR"(Render())", __LINE__);

    content = UR"({var:1}---{var:2})";
    helper.Equal(Template::Render(content, &value), UR"(abc---true)", UR"(Render())", __LINE__);

    content = UR"({var:2}{var:3}--{var:2})";
    helper.Equal(Template::Render(content, &value), UR"(true456--true)", UR"(Render())", __LINE__);

    content = UR"({var:4}--{var:4}{var:4})";
    helper.Equal(Template::Render(content, &value), UR"(1.5--1.51.5)", UR"(Render())", __LINE__);

    content = UR"({var:4}--{var:4}--{var:4})";
    helper.Equal(Template::Render(content, &value), UR"(1.5--1.5--1.5)", UR"(Render())", __LINE__);

    content = UR"({var:4}---{var:4}---{var:4})";
    helper.Equal(Template::Render(content, &value), UR"(1.5---1.5---1.5)", UR"(Render())", __LINE__);
}

void TestVariableLTag3(TestHelper &helper) {
    Value<char32_t> value;
    const char32_t *content;

    content = UR"({var:0})";
    helper.Equal(Template::Render(content, &value), UR"({var:0})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({var:a})", &value), UR"({var:a})", UR"(Render())", __LINE__);

    content = UR"({var:0[0]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), &value), UR"({var:0[0]})", UR"(Render())",
                 __LINE__);

    content = UR"({var:a[0]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), &value), UR"({var:a[0]})", UR"(Render())",
                 __LINE__);

    content = UR"({var:0[a]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), &value), UR"({var:0[a]})", UR"(Render())",
                 __LINE__);

    content = UR"({var:a[abc]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), &value), UR"({var:a[abc]})", UR"(Render())",
                 __LINE__);

    ////////////////

    value = JSON::Parse(UR"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = UR"({var:0})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), &value), UR"({var:0})", UR"(Render())",
                 __LINE__);

    content = UR"({var:0[0]})";
    helper.Equal(Template::Render(content, &value), UR"({var:0[0]})", UR"(Render())", __LINE__);

    content = UR"({var:0[0][0]})";
    helper.Equal(Template::Render(content, &value), UR"({var:0[0][0]})", UR"(Render())", __LINE__);

    /////

    content = UR"({var:1})";
    helper.Equal(Template::Render(content, &value), UR"({var:1})", UR"(Render())", __LINE__);

    content = UR"({var:2})";
    helper.Equal(Template::Render(content, &value), UR"({var:2})", UR"(Render())", __LINE__);

    content = UR"({var:1[a]})";
    helper.Equal(Template::Render(content, &value), UR"({var:1[a]})", UR"(Render())", __LINE__);

    content = UR"({var:1[b]})";
    helper.Equal(Template::Render(content, &value), UR"({var:1[b]})", UR"(Render())", __LINE__);

    ////

    value = JSON::Parse(UR"(["A", "abc", true, 456, 1.5])");

    content = UR"({var:0)";
    helper.Equal(Template::Render(content, &value), UR"({var:0)", UR"(Render())", __LINE__);

    content = UR"(var:0})";
    helper.Equal(Template::Render(content, &value), UR"(var:0})", UR"(Render())", __LINE__);

    content = UR"({v})";
    helper.Equal(Template::Render(content, &value), UR"({v})", UR"(Render())", __LINE__);

    content = UR"({va})";
    helper.Equal(Template::Render(content, &value), UR"({va})", UR"(Render())", __LINE__);

    content = UR"({var})";
    helper.Equal(Template::Render(content, &value), UR"({var})", UR"(Render())", __LINE__);

    content = UR"({var:})";
    helper.Equal(Template::Render(content, &value), UR"({var:})", UR"(Render())", __LINE__);

    content = UR"({v:0})";
    helper.Equal(Template::Render(content, &value), UR"({v:0})", UR"(Render())", __LINE__);

    content = UR"({va:0})";
    helper.Equal(Template::Render(content, &value), UR"({va:0})", UR"(Render())", __LINE__);
    ////

    content = UR"({var:0{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({var:0{var:0})", UR"(Render())", __LINE__);

    content = UR"(var:0{var:0})";
    helper.Equal(Template::Render(content, &value), UR"(var:0A)", UR"(Render())", __LINE__);

    content = UR"(var:0}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"(var:0}A)", UR"(Render())", __LINE__);

    content = UR"({var:0{var:0}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({var:0{var:0}A)", UR"(Render())", __LINE__);

    ////

    content = UR"({var:0{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({var:0{var:0})", UR"(Render())", __LINE__);

    content = UR"(var:0{var:0})";
    helper.Equal(Template::Render(content, &value), UR"(var:0A)", UR"(Render())", __LINE__);

    content = UR"(var:0}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"(var:0}A)", UR"(Render())", __LINE__);

    content = UR"({var:0{var:0}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({var:0{var:0}A)", UR"(Render())", __LINE__);

    ////

    content = UR"({{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({A)", UR"(Render())", __LINE__);

    content = UR"({{var:0}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({AA)", UR"(Render())", __LINE__);

    content = UR"({v{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({vA)", UR"(Render())", __LINE__);

    content = UR"({v{var:0}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({vAA)", UR"(Render())", __LINE__);

    content = UR"({va{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({vaA)", UR"(Render())", __LINE__);

    content = UR"({va{var:0}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({vaAA)", UR"(Render())", __LINE__);

    content = UR"({var{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({varA)", UR"(Render())", __LINE__);

    content = UR"({var{var:0}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({varAA)", UR"(Render())", __LINE__);

    ///

    content = UR"({-{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({-A)", UR"(Render())", __LINE__);

    content = UR"({-{var:0}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({-AA)", UR"(Render())", __LINE__);

    content = UR"({v-{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({v-A)", UR"(Render())", __LINE__);

    content = UR"({v-{var:0}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({v-AA)", UR"(Render())", __LINE__);

    content = UR"({va-{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({va-A)", UR"(Render())", __LINE__);

    content = UR"({va-{var:0}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({va-AA)", UR"(Render())", __LINE__);

    content = UR"({var-{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({var-A)", UR"(Render())", __LINE__);

    content = UR"({var-{var:0}{var:0})";
    helper.Equal(Template::Render(content, &value), UR"({var-AA)", UR"(Render())", __LINE__);

    //

    content = UR"({var-0})";
    helper.Equal(Template::Render(content, &value), UR"({var-0})", UR"(Render())", __LINE__);

    content = UR"({var 0})";
    helper.Equal(Template::Render(content, &value), UR"({var 0})", UR"(Render())", __LINE__);

    content = UR"({var:0 })";
    helper.Equal(Template::Render(content, &value), UR"({var:0 })", UR"(Render())", __LINE__);

    content = UR"({var:0 )";
    helper.Equal(Template::Render(content, &value), UR"({var:0 )", UR"(Render())", __LINE__);

    content = UR"({var:0)";
    helper.Equal(Template::Render(content, &value), UR"({var:0)", UR"(Render())", __LINE__);

    content = UR"( {var-0})";
    helper.Equal(Template::Render(content, &value), UR"( {var-0})", UR"(Render())", __LINE__);

    content = UR"( {var 0})";
    helper.Equal(Template::Render(content, &value), UR"( {var 0})", UR"(Render())", __LINE__);

    content = UR"( {var:0 })";
    helper.Equal(Template::Render(content, &value), UR"( {var:0 })", UR"(Render())", __LINE__);

    content = UR"( {var:0 )";
    helper.Equal(Template::Render(content, &value), UR"( {var:0 )", UR"(Render())", __LINE__);

    content = UR"( {var:0)";
    helper.Equal(Template::Render(content, &value), UR"( {var:0)", UR"(Render())", __LINE__);
}

void TestVariableLTag4(TestHelper &helper) {
    Value<char32_t> value;

    value += UR"(<)";
    value += UR"(>)";
    value += UR"(&)";
    value += UR"(")";
    value += UR"(')";
    value += UR"(<>)";
    value += UR"(<&)";
    value += UR"(<&)";
    value += UR"(>")";
    value += UR"("')";
    value += UR"(<">)";
    value += UR"(<'>)";
    value += UR"(<&>)";
    value += UR"(&"&)";
    value += UR"("'")";
    value += UR"('<')";
    value += UR"('&')";
    value += UR"(<>&'")";
    value += UR"('"<>&)";
    value += UR"(<"&'>)";

    value += UR"(<<<<<)";
    value += UR"(>>>>>)";
    value += UR"(&&&&&)";
    value += UR"(""""")";
    value += UR"(''''')";

    value += UR"(A<)";
    value += UR"(A>)";
    value += UR"(A&)";
    value += UR"(A")";
    value += UR"(A')";
    value += UR"(<A)";
    value += UR"(>A)";
    value += UR"(&A)";
    value += UR"("A)";
    value += UR"('A)";
    value += UR"(A<A)";
    value += UR"(A>A)";
    value += UR"(A&A)";
    value += UR"(A"A)";
    value += UR"(A'A)";

    value += UR"(AA<AA)";
    value += UR"(AA>AA)";
    value += UR"(AA&AA)";
    value += UR"(AA"AA)";
    value += UR"(AA'AA)";

    value += UR"(AA<<<<AA)";
    value += UR"(AA>>>>AA)";
    value += UR"(AA&&&&AA)";
    value += UR"(AA""""AA)";
    value += UR"(AA''''AA)";

    value += UR"(<A>B'C"D&E'F"G<H>I&G"K)";
    value += UR"(AB"CD'EF<GH>IGK')";
    value += UR"("ABC'DEF<GHI>GK<)";
    value += UR"(A""BC<<DE>>FG''HI&&GK)";

#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
    helper.Equal(Template::Render(UR"({var:0})", &value), UR"(&lt;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:1})", &value), UR"(&gt;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:2})", &value), UR"(&amp;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:3})", &value), UR"(&quot;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:4})", &value), UR"(&apos;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:5})", &value), UR"(&lt;&gt;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:6})", &value), UR"(&lt;&amp;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:7})", &value), UR"(&lt;&amp;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:8})", &value), UR"(&gt;&quot;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:9})", &value), UR"(&quot;&apos;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:10})", &value), UR"(&lt;&quot;&gt;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:11})", &value), UR"(&lt;&apos;&gt;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:12})", &value), UR"(&lt;&amp;&gt;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:13})", &value), UR"(&amp;&quot;&amp;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:14})", &value), UR"(&quot;&apos;&quot;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:15})", &value), UR"(&apos;&lt;&apos;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:16})", &value), UR"(&apos;&amp;&apos;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:17})", &value), UR"(&lt;&gt;&amp;&apos;&quot;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:18})", &value), UR"(&apos;&quot;&lt;&gt;&amp;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:19})", &value), UR"(&lt;&quot;&amp;&apos;&gt;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:20})", &value), UR"(&lt;&lt;&lt;&lt;&lt;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:21})", &value), UR"(&gt;&gt;&gt;&gt;&gt;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:22})", &value), UR"(&amp;&amp;&amp;&amp;&amp;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:23})", &value), UR"(&quot;&quot;&quot;&quot;&quot;)", UR"(Render())",
                 __LINE__);
    helper.Equal(Template::Render(UR"({var:24})", &value), UR"(&apos;&apos;&apos;&apos;&apos;)", UR"(Render())",
                 __LINE__);
    helper.Equal(Template::Render(UR"({var:25})", &value), UR"(A&lt;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:26})", &value), UR"(A&gt;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:27})", &value), UR"(A&amp;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:28})", &value), UR"(A&quot;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:29})", &value), UR"(A&apos;)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:30})", &value), UR"(&lt;A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:31})", &value), UR"(&gt;A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:32})", &value), UR"(&amp;A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:33})", &value), UR"(&quot;A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:34})", &value), UR"(&apos;A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:35})", &value), UR"(A&lt;A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:36})", &value), UR"(A&gt;A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:37})", &value), UR"(A&amp;A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:38})", &value), UR"(A&quot;A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:39})", &value), UR"(A&apos;A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:40})", &value), UR"(AA&lt;AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:41})", &value), UR"(AA&gt;AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:42})", &value), UR"(AA&amp;AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:43})", &value), UR"(AA&quot;AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:44})", &value), UR"(AA&apos;AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:45})", &value), UR"(AA&lt;&lt;&lt;&lt;AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:46})", &value), UR"(AA&gt;&gt;&gt;&gt;AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:47})", &value), UR"(AA&amp;&amp;&amp;&amp;AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:48})", &value), UR"(AA&quot;&quot;&quot;&quot;AA)", UR"(Render())",
                 __LINE__);
    helper.Equal(Template::Render(UR"({var:49})", &value), UR"(AA&apos;&apos;&apos;&apos;AA)", UR"(Render())",
                 __LINE__);
    helper.Equal(Template::Render(UR"({var:50})", &value),
                 UR"(&lt;A&gt;B&apos;C&quot;D&amp;E&apos;F&quot;G&lt;H&gt;I&amp;G&quot;K)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:51})", &value), UR"(AB&quot;CD&apos;EF&lt;GH&gt;IGK&apos;)", UR"(Render())",
                 __LINE__);
    helper.Equal(Template::Render(UR"({var:52})", &value), UR"(&quot;ABC&apos;DEF&lt;GHI&gt;GK&lt;)", UR"(Render())",
                 __LINE__);
    helper.Equal(Template::Render(UR"({var:53})", &value),
                 UR"(A&quot;&quot;BC&lt;&lt;DE&gt;&gt;FG&apos;&apos;HI&amp;&amp;GK)", UR"(Render())", __LINE__);
#else
    helper.Equal(Template::Render(UR"({var:0})", &value), UR"(<)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:1})", &value), UR"(>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:2})", &value), UR"(&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:3})", &value), UR"(")", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:4})", &value), UR"(')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:5})", &value), UR"(<>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:6})", &value), UR"(<&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:7})", &value), UR"(<&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:8})", &value), UR"(>")", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:9})", &value), UR"("')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:10})", &value), UR"(<">)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:11})", &value), UR"(<'>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:12})", &value), UR"(<&>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:13})", &value), UR"(&"&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:14})", &value), UR"("'")", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:15})", &value), UR"('<')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:16})", &value), UR"('&')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:17})", &value), UR"(<>&'")", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:18})", &value), UR"('"<>&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:19})", &value), UR"(<"&'>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:20})", &value), UR"(<<<<<)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:21})", &value), UR"(>>>>>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:22})", &value), UR"(&&&&&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:23})", &value), UR"(""""")", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:24})", &value), UR"(''''')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:25})", &value), UR"(A<)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:26})", &value), UR"(A>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:27})", &value), UR"(A&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:28})", &value), UR"(A")", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:29})", &value), UR"(A')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:30})", &value), UR"(<A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:31})", &value), UR"(>A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:32})", &value), UR"(&A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:33})", &value), UR"("A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:34})", &value), UR"('A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:35})", &value), UR"(A<A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:36})", &value), UR"(A>A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:37})", &value), UR"(A&A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:38})", &value), UR"(A"A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:39})", &value), UR"(A'A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:40})", &value), UR"(AA<AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:41})", &value), UR"(AA>AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:42})", &value), UR"(AA&AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:43})", &value), UR"(AA"AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:44})", &value), UR"(AA'AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:45})", &value), UR"(AA<<<<AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:46})", &value), UR"(AA>>>>AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:47})", &value), UR"(AA&&&&AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:48})", &value), UR"(AA""""AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:49})", &value), UR"(AA''''AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:50})", &value), UR"(<A>B'C"D&E'F"G<H>I&G"K)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:51})", &value), UR"(AB"CD'EF<GH>IGK')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:52})", &value), UR"("ABC'DEF<GHI>GK<)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({var:53})", &value), UR"(A""BC<<DE>>FG''HI&&GK)", UR"(Render())", __LINE__);
#endif
}

void TestRawVariableLTag1(TestHelper &helper) {
    const char32_t *content;

    Value<char32_t> value = JSON::Parse(UR"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    Value<char32_t> sub_value =
        JSON::Parse(UR"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = UR"({raw:0})";
    helper.Equal(Template::Render(content, &value), UR"(A)", UR"(Render())", __LINE__);

    content = UR"({raw:1})";
    helper.Equal(Template::Render(content, &value), UR"(abc)", UR"(Render())", __LINE__);

    content = UR"({raw:2})";
    helper.Equal(Template::Render(content, &value), UR"(true)", UR"(Render())", __LINE__);

    content = UR"({raw:3})";
    helper.Equal(Template::Render(content, &value), UR"(456)", UR"(Render())", __LINE__);

    content = UR"({raw:4})";
    helper.Equal(Template::Render(content, &value), UR"(1.5)", UR"(Render())", __LINE__);

    content = UR"({raw:5[0]})";
    helper.Equal(Template::Render(content, &value), UR"(null)", UR"(Render())", __LINE__);

    content = UR"({raw:5[1]})";
    helper.Equal(Template::Render(content, &value), UR"(false)", UR"(Render())", __LINE__);

    content = UR"({raw:5[2][0]})";
    helper.Equal(Template::Render(content, &value), UR"(Qentem)", UR"(Render())", __LINE__);

    //////

    content = UR"({raw:key1})";
    helper.Equal(Template::Render(content, &sub_value), UR"(a)", UR"(Render())", __LINE__);

    content = UR"({raw:key2})";
    helper.Equal(Template::Render(content, &sub_value), UR"(ABC)", UR"(Render())", __LINE__);

    content = UR"({raw:key3})";
    helper.Equal(Template::Render(content, &sub_value), UR"(false)", UR"(Render())", __LINE__);

    content = UR"({raw:key4})";
    helper.Equal(Template::Render(content, &sub_value), UR"(100)", UR"(Render())", __LINE__);

    content = UR"({raw:key5})";
    helper.Equal(Template::Render(content, &sub_value), UR"(1.5)", UR"(Render())", __LINE__);

    content = UR"({raw:key6[one]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(1)", UR"(Render())", __LINE__);

    content = UR"({raw:key7[0]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(null)", UR"(Render())", __LINE__);

    content = UR"({raw:key7[1]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(false)", UR"(Render())", __LINE__);

    content = UR"({raw:key7[2][0]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(Qentem)", UR"(Render())", __LINE__);

    //
    content = UR"({raw:6[key1]})";
    helper.Equal(Template::Render(content, &value), UR"(a)", UR"(Render())", __LINE__);

    content = UR"({raw:6[key2]})";
    helper.Equal(Template::Render(content, &value), UR"(ABC)", UR"(Render())", __LINE__);

    content = UR"({raw:6[key3]})";
    helper.Equal(Template::Render(content, &value), UR"(false)", UR"(Render())", __LINE__);

    content = UR"({raw:6[key4]})";
    helper.Equal(Template::Render(content, &value), UR"(100)", UR"(Render())", __LINE__);

    content = UR"({raw:6[key5]})";
    helper.Equal(Template::Render(content, &value), UR"(1.5)", UR"(Render())", __LINE__);

    content = UR"({raw:6[key6][one]})";
    helper.Equal(Template::Render(content, &value), UR"(1)", UR"(Render())", __LINE__);

    ////////////////

    content = UR"(-{raw:2})";
    helper.Equal(Template::Render(content, &value), UR"(-true)", UR"(Render())", __LINE__);

    content = UR"(-{raw:key7[0]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(-null)", UR"(Render())", __LINE__);

    content = UR"(-{raw:key7[2][0]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(-Qentem)", UR"(Render())", __LINE__);

    content = UR"(-{raw:6[key3]})";
    helper.Equal(Template::Render(content, &value), UR"(-false)", UR"(Render())", __LINE__);

    content = UR"(-{raw:6[key4]})";
    helper.Equal(Template::Render(content, &value), UR"(-100)", UR"(Render())", __LINE__);
    ////////////

    content = UR"({raw:2}-)";
    helper.Equal(Template::Render(content, &value), UR"(true-)", UR"(Render())", __LINE__);

    content = UR"({raw:key7[0]}-)";
    helper.Equal(Template::Render(content, &sub_value), UR"(null-)", UR"(Render())", __LINE__);

    content = UR"({raw:key7[2][0]}-)";
    helper.Equal(Template::Render(content, &sub_value), UR"(Qentem-)", UR"(Render())", __LINE__);

    content = UR"({raw:6[key3]}-)";
    helper.Equal(Template::Render(content, &value), UR"(false-)", UR"(Render())", __LINE__);

    content = UR"({raw:6[key4]}-)";
    helper.Equal(Template::Render(content, &value), UR"(100-)", UR"(Render())", __LINE__);

    ////////////

    content = UR"(-{raw:2}-)";
    helper.Equal(Template::Render(content, &value), UR"(-true-)", UR"(Render())", __LINE__);

    content = UR"(-{raw:key7[0]}-)";
    helper.Equal(Template::Render(content, &sub_value), UR"(-null-)", UR"(Render())", __LINE__);

    content = UR"(-{raw:key7[2][0]}-)";
    helper.Equal(Template::Render(content, &sub_value), UR"(-Qentem-)", UR"(Render())", __LINE__);

    content = UR"(-{raw:6[key3]}-)";
    helper.Equal(Template::Render(content, &value), UR"(-false-)", UR"(Render())", __LINE__);

    content = UR"(-{raw:6[key4]}-)";
    helper.Equal(Template::Render(content, &value), UR"(-100-)", UR"(Render())", __LINE__);

    ////////////

    content = UR"(------{raw:2})";
    helper.Equal(Template::Render(content, &value), UR"(------true)", UR"(Render())", __LINE__);

    content = UR"(------{raw:key7[0]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(------null)", UR"(Render())", __LINE__);

    content = UR"(------{raw:key7[2][0]})";
    helper.Equal(Template::Render(content, &sub_value), UR"(------Qentem)", UR"(Render())", __LINE__);

    content = UR"(------{raw:6[key3]})";
    helper.Equal(Template::Render(content, &value), UR"(------false)", UR"(Render())", __LINE__);

    content = UR"(------{raw:6[key4]})";
    helper.Equal(Template::Render(content, &value), UR"(------100)", UR"(Render())", __LINE__);

    ////////////

    content = UR"({raw:2}------)";
    helper.Equal(Template::Render(content, &value), UR"(true------)", UR"(Render())", __LINE__);

    content = UR"({raw:key7[0]}------)";
    helper.Equal(Template::Render(content, &sub_value), UR"(null------)", UR"(Render())", __LINE__);

    content = UR"({raw:key7[2][0]}------)";
    helper.Equal(Template::Render(content, &sub_value), UR"(Qentem------)", UR"(Render())", __LINE__);

    content = UR"({raw:6[key3]}------)";
    helper.Equal(Template::Render(content, &value), UR"(false------)", UR"(Render())", __LINE__);

    content = UR"({raw:6[key4]}------)";
    helper.Equal(Template::Render(content, &value), UR"(100------)", UR"(Render())", __LINE__);

    ////////////

    content = UR"(------{raw:2}------)";
    helper.Equal(Template::Render(content, &value), UR"(------true------)", UR"(Render())", __LINE__);

    content = UR"(------{raw:key7[0]}------)";
    helper.Equal(Template::Render(content, &sub_value), UR"(------null------)", UR"(Render())", __LINE__);

    content = UR"(------{raw:key7[2][0]}------)";
    helper.Equal(Template::Render(content, &sub_value), UR"(------Qentem------)", UR"(Render())", __LINE__);

    content = UR"(------{raw:6[key3]}------)";
    helper.Equal(Template::Render(content, &value), UR"(------false------)", UR"(Render())", __LINE__);

    content = UR"(------{raw:6[key4]}------)";
    helper.Equal(Template::Render(content, &value), UR"(------100------)", UR"(Render())", __LINE__);

    content = UR"({raw:key7[2[0]})";
    helper.Equal(Template::Render(content, &value), UR"({raw:key7[2[0]})", UR"(Render())", __LINE__);

    content = UR"({raw:6key3]})";
    helper.Equal(Template::Render(content, &value), UR"({raw:6key3]})", UR"(Render())", __LINE__);
}

void TestRawVariableLTag2(TestHelper &helper) {
    Value<char32_t> value = JSON::Parse(UR"(["A", "abc", true, 456, 1.5])");
    const char32_t *content;

    content = UR"({raw:0}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"(AA)", UR"(Render())", __LINE__);

    content = UR"({raw:1}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"(abcA)", UR"(Render())", __LINE__);

    content = UR"({raw:1}{raw:2})";
    helper.Equal(Template::Render(content, &value), UR"(abctrue)", UR"(Render())", __LINE__);

    content = UR"({raw:2}{raw:3}{raw:2})";
    helper.Equal(Template::Render(content, &value), UR"(true456true)", UR"(Render())", __LINE__);

    content = UR"({raw:4}{raw:4}{raw:4})";
    helper.Equal(Template::Render(content, &value), UR"(1.51.51.5)", UR"(Render())", __LINE__);
    ///

    content = UR"({raw:0}-{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"(A-A)", UR"(Render())", __LINE__);

    content = UR"({raw:1}--{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"(abc--A)", UR"(Render())", __LINE__);

    content = UR"({raw:1}---{raw:2})";
    helper.Equal(Template::Render(content, &value), UR"(abc---true)", UR"(Render())", __LINE__);

    content = UR"({raw:2}{raw:3}--{raw:2})";
    helper.Equal(Template::Render(content, &value), UR"(true456--true)", UR"(Render())", __LINE__);

    content = UR"({raw:4}--{raw:4}{raw:4})";
    helper.Equal(Template::Render(content, &value), UR"(1.5--1.51.5)", UR"(Render())", __LINE__);

    content = UR"({raw:4}--{raw:4}--{raw:4})";
    helper.Equal(Template::Render(content, &value), UR"(1.5--1.5--1.5)", UR"(Render())", __LINE__);

    content = UR"({raw:4}---{raw:4}---{raw:4})";
    helper.Equal(Template::Render(content, &value), UR"(1.5---1.5---1.5)", UR"(Render())", __LINE__);
}

void TestRawVariableLTag3(TestHelper &helper) {
    Value<char32_t> value;
    const char32_t *content;

    content = UR"({raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({raw:0})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({raw:a})", &value), UR"({raw:a})", UR"(Render())", __LINE__);

    content = UR"({raw:0[0]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), &value), UR"({raw:0[0]})", UR"(Render())",
                 __LINE__);

    content = UR"({raw:a[0]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), &value), UR"({raw:a[0]})", UR"(Render())",
                 __LINE__);

    content = UR"({raw:0[a]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), &value), UR"({raw:0[a]})", UR"(Render())",
                 __LINE__);

    content = UR"({raw:a[abc]})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), &value), UR"({raw:a[abc]})", UR"(Render())",
                 __LINE__);

    ////////////////

    value = JSON::Parse(UR"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = UR"({raw:0})";
    helper.Equal(Template::Render(content, StringUtils::Count(content), &value), UR"({raw:0})", UR"(Render())",
                 __LINE__);

    content = UR"({raw:0[0]})";
    helper.Equal(Template::Render(content, &value), UR"({raw:0[0]})", UR"(Render())", __LINE__);

    content = UR"({raw:0[0][0]})";
    helper.Equal(Template::Render(content, &value), UR"({raw:0[0][0]})", UR"(Render())", __LINE__);

    /////

    content = UR"({raw:1})";
    helper.Equal(Template::Render(content, &value), UR"({raw:1})", UR"(Render())", __LINE__);

    content = UR"({raw:2})";
    helper.Equal(Template::Render(content, &value), UR"({raw:2})", UR"(Render())", __LINE__);

    content = UR"({raw:1[a]})";
    helper.Equal(Template::Render(content, &value), UR"({raw:1[a]})", UR"(Render())", __LINE__);

    content = UR"({raw:1[b]})";
    helper.Equal(Template::Render(content, &value), UR"({raw:1[b]})", UR"(Render())", __LINE__);

    ////

    value = JSON::Parse(UR"(["A", "abc", true, 456, 1.5])");

    content = UR"({raw:0)";
    helper.Equal(Template::Render(content, &value), UR"({raw:0)", UR"(Render())", __LINE__);

    content = UR"(raw:0})";
    helper.Equal(Template::Render(content, &value), UR"(raw:0})", UR"(Render())", __LINE__);

    content = UR"({r})";
    helper.Equal(Template::Render(content, &value), UR"({r})", UR"(Render())", __LINE__);

    content = UR"({ra})";
    helper.Equal(Template::Render(content, &value), UR"({ra})", UR"(Render())", __LINE__);

    content = UR"({raw})";
    helper.Equal(Template::Render(content, &value), UR"({raw})", UR"(Render())", __LINE__);

    content = UR"({raw:})";
    helper.Equal(Template::Render(content, &value), UR"({raw:})", UR"(Render())", __LINE__);

    content = UR"({r:0})";
    helper.Equal(Template::Render(content, &value), UR"({r:0})", UR"(Render())", __LINE__);

    content = UR"({ra:0})";
    helper.Equal(Template::Render(content, &value), UR"({ra:0})", UR"(Render())", __LINE__);
    ////

    content = UR"({raw:0{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({raw:0{raw:0})", UR"(Render())", __LINE__);

    content = UR"(raw:0{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"(raw:0A)", UR"(Render())", __LINE__);

    content = UR"(raw:0}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"(raw:0}A)", UR"(Render())", __LINE__);

    content = UR"({raw:0{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({raw:0{raw:0}A)", UR"(Render())", __LINE__);

    ////

    content = UR"({raw:0{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({raw:0{raw:0})", UR"(Render())", __LINE__);

    content = UR"(raw:0{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"(raw:0A)", UR"(Render())", __LINE__);

    content = UR"(raw:0}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"(raw:0}A)", UR"(Render())", __LINE__);

    content = UR"({raw:0{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({raw:0{raw:0}A)", UR"(Render())", __LINE__);

    ////

    content = UR"({{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({A)", UR"(Render())", __LINE__);

    content = UR"({{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({AA)", UR"(Render())", __LINE__);

    content = UR"({r{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({rA)", UR"(Render())", __LINE__);

    content = UR"({r{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({rAA)", UR"(Render())", __LINE__);

    content = UR"({ra{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({raA)", UR"(Render())", __LINE__);

    content = UR"({ra{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({raAA)", UR"(Render())", __LINE__);

    content = UR"({raw{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({rawA)", UR"(Render())", __LINE__);

    content = UR"({raw{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({rawAA)", UR"(Render())", __LINE__);

    ///

    content = UR"({-{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({-A)", UR"(Render())", __LINE__);

    content = UR"({-{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({-AA)", UR"(Render())", __LINE__);

    content = UR"({r-{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({r-A)", UR"(Render())", __LINE__);

    content = UR"({r-{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({r-AA)", UR"(Render())", __LINE__);

    content = UR"({ra-{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({ra-A)", UR"(Render())", __LINE__);

    content = UR"({ra-{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({ra-AA)", UR"(Render())", __LINE__);

    content = UR"({raw-{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({raw-A)", UR"(Render())", __LINE__);

    content = UR"({raw-{raw:0}{raw:0})";
    helper.Equal(Template::Render(content, &value), UR"({raw-AA)", UR"(Render())", __LINE__);

    //

    content = UR"({raw-0})";
    helper.Equal(Template::Render(content, &value), UR"({raw-0})", UR"(Render())", __LINE__);

    content = UR"({raw 0})";
    helper.Equal(Template::Render(content, &value), UR"({raw 0})", UR"(Render())", __LINE__);

    content = UR"({raw:0 })";
    helper.Equal(Template::Render(content, &value), UR"({raw:0 })", UR"(Render())", __LINE__);

    content = UR"({raw:0 )";
    helper.Equal(Template::Render(content, &value), UR"({raw:0 )", UR"(Render())", __LINE__);

    content = UR"({raw:0)";
    helper.Equal(Template::Render(content, &value), UR"({raw:0)", UR"(Render())", __LINE__);

    content = UR"({ raw-0})";
    helper.Equal(Template::Render(content, &value), UR"({ raw-0})", UR"(Render())", __LINE__);

    content = UR"({ raw 0})";
    helper.Equal(Template::Render(content, &value), UR"({ raw 0})", UR"(Render())", __LINE__);

    content = UR"({ raw:0 })";
    helper.Equal(Template::Render(content, &value), UR"({ raw:0 })", UR"(Render())", __LINE__);

    content = UR"({ raw:0 )";
    helper.Equal(Template::Render(content, &value), UR"({ raw:0 )", UR"(Render())", __LINE__);

    content = UR"({ raw:0)";
    helper.Equal(Template::Render(content, &value), UR"({ raw:0)", UR"(Render())", __LINE__);
}

void TestRawVariableLTag4(TestHelper &helper) {
    Value<char32_t> value;

    value += UR"(<)";
    value += UR"(>)";
    value += UR"(&)";
    value += UR"(")";
    value += UR"(')";
    value += UR"(<>)";
    value += UR"(<&)";
    value += UR"(<&)";
    value += UR"(>")";
    value += UR"("')";
    value += UR"(<">)";
    value += UR"(<'>)";
    value += UR"(<&>)";
    value += UR"(&"&)";
    value += UR"("'")";
    value += UR"('<')";
    value += UR"('&')";
    value += UR"(<>&'")";
    value += UR"('"<>&)";
    value += UR"(<"&'>)";

    value += UR"(<<<<<)";
    value += UR"(>>>>>)";
    value += UR"(&&&&&)";
    value += UR"(""""")";
    value += UR"(''''')";

    value += UR"(A<)";
    value += UR"(A>)";
    value += UR"(A&)";
    value += UR"(A")";
    value += UR"(A')";
    value += UR"(<A)";
    value += UR"(>A)";
    value += UR"(&A)";
    value += UR"("A)";
    value += UR"('A)";
    value += UR"(A<A)";
    value += UR"(A>A)";
    value += UR"(A&A)";
    value += UR"(A"A)";
    value += UR"(A'A)";

    value += UR"(AA<AA)";
    value += UR"(AA>AA)";
    value += UR"(AA&AA)";
    value += UR"(AA"AA)";
    value += UR"(AA'AA)";

    value += UR"(AA<<<<AA)";
    value += UR"(AA>>>>AA)";
    value += UR"(AA&&&&AA)";
    value += UR"(AA""""AA)";
    value += UR"(AA''''AA)";

    value += UR"(<A>B'C"D&E'F"G<H>I&G"K)";
    value += UR"(AB"CD'EF<GH>IGK')";
    value += UR"("ABC'DEF<GHI>GK<)";
    value += UR"(A""BC<<DE>>FG''HI&&GK)";

    helper.Equal(Template::Render(UR"({raw:0})", &value), UR"(<)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:1})", &value), UR"(>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:2})", &value), UR"(&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:3})", &value), UR"(")", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:4})", &value), UR"(')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:5})", &value), UR"(<>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:6})", &value), UR"(<&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:7})", &value), UR"(<&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:8})", &value), UR"(>")", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:9})", &value), UR"("')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:10})", &value), UR"(<">)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:11})", &value), UR"(<'>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:12})", &value), UR"(<&>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:13})", &value), UR"(&"&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:14})", &value), UR"("'")", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:15})", &value), UR"('<')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:16})", &value), UR"('&')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:17})", &value), UR"(<>&'")", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:18})", &value), UR"('"<>&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:19})", &value), UR"(<"&'>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:20})", &value), UR"(<<<<<)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:21})", &value), UR"(>>>>>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:22})", &value), UR"(&&&&&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:23})", &value), UR"(""""")", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:24})", &value), UR"(''''')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:25})", &value), UR"(A<)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:26})", &value), UR"(A>)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:27})", &value), UR"(A&)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:28})", &value), UR"(A")", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:29})", &value), UR"(A')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:30})", &value), UR"(<A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:31})", &value), UR"(>A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:32})", &value), UR"(&A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:33})", &value), UR"("A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:34})", &value), UR"('A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:35})", &value), UR"(A<A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:36})", &value), UR"(A>A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:37})", &value), UR"(A&A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:38})", &value), UR"(A"A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:39})", &value), UR"(A'A)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:40})", &value), UR"(AA<AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:41})", &value), UR"(AA>AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:42})", &value), UR"(AA&AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:43})", &value), UR"(AA"AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:44})", &value), UR"(AA'AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:45})", &value), UR"(AA<<<<AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:46})", &value), UR"(AA>>>>AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:47})", &value), UR"(AA&&&&AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:48})", &value), UR"(AA""""AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:49})", &value), UR"(AA''''AA)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:50})", &value), UR"(<A>B'C"D&E'F"G<H>I&G"K)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:51})", &value), UR"(AB"CD'EF<GH>IGK')", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:52})", &value), UR"("ABC'DEF<GHI>GK<)", UR"(Render())", __LINE__);
    helper.Equal(Template::Render(UR"({raw:53})", &value), UR"(A""BC<<DE>>FG''HI&&GK)", UR"(Render())", __LINE__);
}

void TestMathLTag1(TestHelper &helper) {
    Value<char32_t> value;

    value[UR"(a1)"] = 5;
    value[UR"(a2)"] = true;
    value[UR"(a3)"] = nullptr;
    value[UR"(a4)"] = false;
    value[UR"(a5)"] = UR"(10)";
    value[UR"(a6)"] = UR"(20)";
    value[UR"(a7)"] = 6;
    value[UR"(a8)"] = 1;
    value[UR"(a9)"] = UR"(1)";
    value[UR"(eq)"] = UR"((8+1+{var:a8}))";

    helper.Equal(Template::Render(UR"({math:1+1})", &value), UR"(2)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a1}+8})", &value), UR"(13)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a7}+8})", &value), UR"(14)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a7}+{var:a1}})", &value), UR"(11)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a5}+{var:a1}})", &value), UR"(15)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a1}+{var:a5}})", &value), UR"(15)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a6}+{var:a5}})", &value), UR"(30)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a6}*{var:a2}})", &value), UR"(20)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a6}*{var:a4}})", &value), UR"(0)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a6}*{var:a7}})", &value), UR"(120)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a7}+{var:a6}})", &value), UR"(26)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a1}+{var:a2}})", &value), UR"(6)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a8}=={var:a2}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a2}=={var:a8}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a5}!={var:a1}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a2}!={var:a4}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a2}==true})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a9}=={var:a8}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a8}=={var:a9}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:1=={var:a8}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:6-5==({var:a9})})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:6-5==({var:a8})})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:null!={var:a3}})", &value), UR"(0)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:({var:a3})==(0)})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a1}+{var:a7}})", &value), UR"(11)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a1}+{var:a7}}{math:{var:a1}+{var:a7}})", &value), UR"(1111)",
                 UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a1}+{var:a7}}*{math:{var:a1}+{var:a7}})", &value), UR"(11*11)",
                 UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a1}+{var:a7}}##{math:{var:a1}+{var:a7}})", &value), UR"(11##11)",
                 UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a1}+{var:a7}}&&&%%^^&&*{math:{var:a1}+{var:a7}})", &value),
                 UR"(11&&&%%^^&&*11)", UR"(Render())", __LINE__);

    ///////////////////

    helper.Equal(Template::Render(UR"({math: {var:a1}+8})", &value), UR"(13)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:  {var:a7}+8})", &value), UR"(14)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:     {var:a7}+{var:a1}})", &value), UR"(11)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a5} +{var:a1}})", &value), UR"(15)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a1}  +{var:a5}})", &value), UR"(15)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a6}    +{var:a5}})", &value), UR"(30)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a6}* {var:a2}})", &value), UR"(20)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a6}*  {var:a4}})", &value), UR"(0)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a6}*      {var:a7}})", &value), UR"(120)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a7}+{var:a6} })", &value), UR"(26)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a1}+{var:a2}  })", &value), UR"(6)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a8}=={var:a2}      })", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a2}=={var:a8}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math: {var:a5}!={var:a1} })", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:  {var:a2}!={var:a4}  })", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:    1=={var:a9}     })", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a9} == {var:a8}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a8}  ==  {var:a9}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:1==          {var:a8}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:6-5         ==1})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:1          ==            {var:a8}})", &value), UR"(1)", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:     0     !=    ({var:a3})        })", &value), UR"(0)", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:a3}       ==       null     })", &value), UR"(1)", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:eq}})", &value), UR"(10)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:10==(8+1+{var:a8})})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:eq}==9+1})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:(5*2)=={var:eq}})", &value), UR"(1)", UR"(Render())", __LINE__);

    //////////////
    value.Reset();

    value += 5;
    value += true;
    value += nullptr;
    value += false;
    value += UR"(10)";
    value += UR"(20)";
    value += 6;
    value += 1;
    value += UR"(1)";
    value += UR"(Qentem)";

    helper.Equal(Template::Render(UR"({math:{var:0}+8})", &value), UR"(13)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:6}+8})", &value), UR"(14)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:6}+{var:0}})", &value), UR"(11)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:4}+{var:0}})", &value), UR"(15)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:0}+{var:4}})", &value), UR"(15)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:5}+{var:4}})", &value), UR"(30)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:5}*{var:1}})", &value), UR"(20)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:5}*{var:3}})", &value), UR"(0)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:5}*{var:6}})", &value), UR"(120)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:6}+{var:5}})", &value), UR"(26)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:0}+{var:1}})", &value), UR"(6)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:7}=={var:1}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:1}=={var:7}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:4}!={var:0}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:1}!={var:3}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:({var:1})==({var:8})})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"(-{math:{var:8}=={var:7}})", &value), UR"(-1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"(--{math:{var:7}=={var:8}})", &value), UR"(--1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"(---{math:1=={var:7}})", &value), UR"(---1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:1==({var:8})}-)", &value), UR"(1-)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:6-5==({var:7})}--)", &value), UR"(1--)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:0==({var:2})}---)", &value), UR"(1---)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"(-{math:{var:2}!=null}-)", &value), UR"(-0-)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"(--{math:Qente=={var:9}}--)", &value), UR"(--0--)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"(---{math:Qente !={var:9}}---)", &value), UR"(---1---)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:  Qentem   =={var:9}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:  Qentem!={var:9}})", &value), UR"(0)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:9}   ==    Qente})", &value), UR"(0)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:9} !=    Qente    })", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:  {var:9}   ==Qentem})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math: {var:9} !=Qentem})", &value), UR"(0)", UR"(Render())", __LINE__);

    /////////

    helper.Equal(Template::Render(UR"({math: true == {var:1}})", &value), UR"(1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math: {var:1} == true})", &value), UR"(1)", UR"(Render())", __LINE__);
}

void TestMathLTag2(TestHelper &helper) {
    Value<char32_t> value;

    value += Array<Value<char32_t>>();
    value += HArray<Value<char32_t>, char32_t>();
    value += 5;

    helper.Equal(Template::Render(UR"({math:{var:0}+8})", &value), UR"({math:{var:0}+8})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:1}+8})", &value), UR"({math:{var:1}+8})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:2}+{var:0}})", &value), UR"({math:{var:2}+{var:0}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:2}+{var:0}})", &value), UR"({math:{var:2}+{var:0}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:0}+{var:1}})", &value), UR"({math:{var:0}+{var:1}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:1}+{var:2}})", &value), UR"({math:{var:1}+{var:2}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:2}*{var:1}})", &value), UR"({math:{var:2}*{var:1}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:0}*{var:1}})", &value), UR"({math:{var:0}*{var:1}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:0}*{var:2}})", &value), UR"({math:{var:0}*{var:2}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:2}+{var:0}})", &value), UR"({math:{var:2}+{var:0}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:1}+{var:2}})", &value), UR"({math:{var:1}+{var:2}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:0}=={var:1}})", &value), UR"({math:{var:0}=={var:1}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:1}=={var:0}})", &value), UR"({math:{var:1}=={var:0}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:0}!={var:2}})", &value), UR"({math:{var:0}!={var:2}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:2}!={var:0}})", &value), UR"({math:{var:2}!={var:0}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:1}=={var:2}})", &value), UR"({math:{var:1}=={var:2}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:8}=={var:7}})", &value), UR"({math:{var:8}=={var:7}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:7}=={var:2}})", &value), UR"({math:{var:7}=={var:2}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:1=={var:7}})", &value), UR"({math:1=={var:7}})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:(6-5)=={var:8}})", &value), UR"({math:(6-5)=={var:8}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:(6-5)=={var:0}})", &value), UR"({math:(6-5)=={var:0}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:0}=={var:8}})", &value), UR"({math:{var:0}=={var:8}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:0}=={var:0}})", &value), UR"({math:{var:0}=={var:0}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:0=={var:1}})", &value), UR"({math:0=={var:1}})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:1}!=0})", &value), UR"({math:{var:1}!=0})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:W={var:0}})", &value), UR"({math:W={var:0}})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:W=={var:0}})", &value), UR"({math:W=={var:0}})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:2}==c})", &value), UR"({math:{var:2}==c})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:abc=={var:2}})", &value), UR"({math:abc=={var:2}})", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:sds})", &value), UR"({math:sds})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:1)", &value), UR"({math:1)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"(math:1})", &value), UR"(math:1})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:{var:2})", &value), UR"({math:5)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({{math:{var:2}+5})", &value), UR"({10)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({m{var:2}})", &value), UR"({m5})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({ma{var:2}})", &value), UR"({ma5})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({mat{var:2}})", &value), UR"({mat5})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math{var:2}})", &value), UR"({math5})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math {var:2}})", &value), UR"({math 5})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:})", &value), UR"({math:})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math-{var:2}}{math:{var:2}+5})", &value), UR"({math-5}10)", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math-4}{math:{var:2}+5})", &value), UR"({math-4}10)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math-4} {math:{var:2}+5})", &value), UR"({math-4} 10)", UR"(Render())",
                 __LINE__);

    helper.Equal(Template::Render(UR"({math:4)", &value), UR"({math:4)", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:4    )", &value), UR"({math:4    )", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:A==1+1})", &value), UR"({math:A==1+1})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:(A)!=1+1})", &value), UR"({math:(A)!=1+1})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:1+1==A})", &value), UR"({math:1+1==A})", UR"(Render())", __LINE__);

    helper.Equal(Template::Render(UR"({math:1+1!=(A)})", &value), UR"({math:1+1!=(A)})", UR"(Render())", __LINE__);
}

void TestInlineIfLTag(TestHelper &helper) {
    Value<char32_t> value;
    const char32_t *content;

    value += 0;
    value += 1;
    value += -1;
    value += true;
    value += false;
    value += nullptr;
    value += UR"(ABC)";
    value += Array<Value<char32_t>>();

    content = UR"({if case="0" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(F)", UR"(Render())", __LINE__);

    content = UR"({if case="-1" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(F)", UR"(Render())", __LINE__);

    content = UR"({if case="0.1" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    content = UR"({if case="1" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    ////

    content = UR"({if case="0" true="T"})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({if case="-1" true="T"})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({if case="0.1" true="T"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    content = UR"({if case="1" true="T"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    ///

    content = UR"({if case="0" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(F)", UR"(Render())", __LINE__);

    content = UR"({if case="-1" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(F)", UR"(Render())", __LINE__);

    content = UR"({if case="0.1" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({if case="1" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    /////

    content = UR"({if case="{var:0}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(F)", UR"(Render())", __LINE__);

    content = UR"({if case="{var:2}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(F)", UR"(Render())", __LINE__);

    content = UR"({if case="{raw:4}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(F)", UR"(Render())", __LINE__);

    content = UR"({if case="{var:5}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(F)", UR"(Render())", __LINE__);

    content = UR"({if case="{var:6}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({if case="fas" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({if case="{var:7}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({if case="{var:20}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    ////
    content = UR"({if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    content = UR"({if case="{var:3}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    content = UR"({if case="{var:6}==ABC" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    /////////////////

    content = UR"({if case=" {var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    content = UR"({if case="{var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    content = UR"({if case=" {var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    content = UR"({if case="  {var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    content = UR"({if case="{var:1}  " true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    content = UR"({if case="  {var:1}  " true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    content = UR"({if case="      {var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    content = UR"({if case="{var:1}          " true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    content = UR"({if case="    {var:1}        " true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);
    ////

    content = UR"(-{if case=" {var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(-T)", UR"(Render())", __LINE__);

    content = UR"({if case=" {var:1} " true="T" false="F"}-)";
    helper.Equal(Template::Render(content, &value), UR"(T-)", UR"(Render())", __LINE__);

    content = UR"(-{if case=" {var:1} " true="T" false="F"}-)";
    helper.Equal(Template::Render(content, &value), UR"(-T-)", UR"(Render())", __LINE__);

    content = UR"(--{if case=" {var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(--T)", UR"(Render())", __LINE__);

    content = UR"({if case=" {var:1} " true="T" false="F"}--)";
    helper.Equal(Template::Render(content, &value), UR"(T--)", UR"(Render())", __LINE__);

    content = UR"(--{if case=" {var:1} " true="T" false="F"}--)";
    helper.Equal(Template::Render(content, &value), UR"(--T--)", UR"(Render())", __LINE__);

    content = UR"(---{if case=" {var:1} " true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(---T)", UR"(Render())", __LINE__);

    content = UR"({if case=" {var:1} " true="T" false="F"}---)";
    helper.Equal(Template::Render(content, &value), UR"(T---)", UR"(Render())", __LINE__);

    content = UR"(---{if case=" {var:1} " true="T" false="F"}---)";
    helper.Equal(Template::Render(content, &value), UR"(---T---)", UR"(Render())", __LINE__);

    content = UR"({if case="1" true="T"}{if case="0" false="F"}{if case="1" true="T"})";
    helper.Equal(Template::Render(content, &value), UR"(TFT)", UR"(Render())", __LINE__);

    ///////

    content = UR"({if case="{var:7}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"(T)", UR"(Render())", __LINE__);

    content = UR"({if case="01" true="{var:3}" false="{var:4}"}--)";
    helper.Equal(Template::Render(content, &value), UR"(--)", UR"(Render())", __LINE__);

    content = UR"({if case="" true="c" false="d"})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({i)";
    helper.Equal(Template::Render(content, &value), UR"({i)", UR"(Render())", __LINE__);

    content = UR"({if)";
    helper.Equal(Template::Render(content, &value), UR"({if)", UR"(Render())", __LINE__);

    content = UR"({if})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"({T)", UR"(Render())", __LINE__);

    content = UR"({i{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"({iT)", UR"(Render())", __LINE__);

    content = UR"({if{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"({ifT)", UR"(Render())", __LINE__);

    content = UR"({if case="{var:1}"                    put="F"})";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"({if{if case="1" true="T" false="F"}}{if case="1" true="T" false="F"})";

    helper.Equal(Template::Render(content, &value), UR"(TT)", UR"(Render())", __LINE__);

    content = UR"({if{if case="{raw:1}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    helper.Equal(Template::Render(content, &value), UR"({ifTT)", UR"(Render())", __LINE__);

    /////
    content = UR"({if case="0" true="{var:3}" false="{var:4}"})";
    helper.Equal(Template::Render(content, &value), UR"(false)", UR"(Render())", __LINE__);

    content = UR"({if case="1" true="{var:3}" false="{var:4}"})";
    helper.Equal(Template::Render(content, &value), UR"(true)", UR"(Render())", __LINE__);

    content = UR"({if case="0" true="{raw:3}{raw:3}" false="{var:4}{var:4}"})";
    helper.Equal(Template::Render(content, &value), UR"(falsefalse)", UR"(Render())", __LINE__);

    content = UR"({if case="1000" true="{var:3}{var:3}" false="{var:4}{var:4}"})";
    helper.Equal(Template::Render(content, &value), UR"(truetrue)", UR"(Render())", __LINE__);

    content = UR"({if case="0" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    helper.Equal(Template::Render(content, &value), UR"(false---false)", UR"(Render())", __LINE__);

    content = UR"({if case="1" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    helper.Equal(Template::Render(content, &value), UR"(true---true)", UR"(Render())", __LINE__);

    content = UR"({if case="0" true="{var:10}" false="{var:20}"})";
    helper.Equal(Template::Render(content, &value), UR"({var:20})", UR"(Render())", __LINE__);

    content = UR"({if case="1" true="{var:10}" false="{var:20}"})";
    helper.Equal(Template::Render(content, &value), UR"({var:10})", UR"(Render())", __LINE__);

    content = UR"({if case="1" true="1" false="0")";
    helper.Equal(Template::Render(content, &value), UR"({if case="1" true="1" false="0")", UR"(Render())", __LINE__);

    content = UR"({if case="1" true="1" false="0")";
    helper.Equal(Template::Render(content, &value), UR"({if case="1" true="1" false="0")", UR"(Render())", __LINE__);

    ///////
    Value<char32_t> value2;

    value2 += UR"(&)";
    value2 += UR"(")";
    value2 += 10;
    value2 += 5;
    value2 += 15;

    content = UR"({if case="1" true="{var:0}" false="{var:1}"})";
#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
    helper.Equal(Template::Render(content, &value2), UR"(&amp;)", UR"(Render())", __LINE__);
#else
    helper.Equal(Template::Render(content, &value2), UR"(&)", UR"(Render())", __LINE__);
#endif

    content = UR"({if case="1" true="{raw:0}" false="{raw:1}"})";
    helper.Equal(Template::Render(content, &value2), UR"(&)", UR"(Render())", __LINE__);

    content = UR"({if case="0" true="{var:0}" false="{var:1}"})";
#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
    helper.Equal(Template::Render(content, &value2), UR"(&quot;)", UR"(Render())", __LINE__);
#else
    helper.Equal(Template::Render(content, &value2), UR"(")", UR"(Render())", __LINE__);
#endif

    content = UR"({if case="0" true="{raw:0}" false="{raw:1}"})";
    helper.Equal(Template::Render(content, &value2), UR"(")", UR"(Render())", __LINE__);

    content =
        UR"({if case="{var:2}+{var:3} == {var:4}" true="{math:{var:3}+{var:4}}" false="{math: {var:2}+{var:4}}"})";
    helper.Equal(Template::Render(content, &value2), UR"(20)", UR"(Render())", __LINE__);

    content =
        UR"({if case=" {var:4}-{var:3} != {var:2} " true="{math: {var:3}+{var:4} } " false=" {math: {var:2}+{var:4} } "})";
    helper.Equal(Template::Render(content, &value2), UR"( 25 )", UR"(Render())", __LINE__);
}

void TestLoopLTag1(TestHelper &helper) {
    Value<char32_t> value;
    const char32_t *content;

    value += 100;
    value += -50;
    value += UR"(Qentem)";
    value += true;
    value += false;
    value += nullptr;
    value += 3;

    content = UR"(<loop repeat="10">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(AAAAAAAAAA)", UR"(Render())", __LINE__);

    content = UR"(<loop             repeat="1">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(A)", UR"(Render())", __LINE__);

    content = UR"(<loop<loop repeat="1">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(<loopA)", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="3"         >ABC</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(ABCABCABC)", UR"(Render())", __LINE__);

    content = UR"(-<loop repeat="3">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(-AAA)", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="3">A</loop>-)";
    helper.Equal(Template::Render(content, &value), UR"(AAA-)", UR"(Render())", __LINE__);

    content = UR"(-<loop repeat="3">A</loop>-)";
    helper.Equal(Template::Render(content, &value), UR"(-AAA-)", UR"(Render())", __LINE__);

    content = UR"(--<loop repeat="3">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(--AAA)", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="3">A</loop>--)";
    helper.Equal(Template::Render(content, &value), UR"(AAA--)", UR"(Render())", __LINE__);

    content = UR"(--<loop repeat="3">A</loop>--)";
    helper.Equal(Template::Render(content, &value), UR"(--AAA--)", UR"(Render())", __LINE__);

    content = UR"(---<loop repeat="3">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(---AAA)", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="3">A</loop>---)";
    helper.Equal(Template::Render(content, &value), UR"(AAA---)", UR"(Render())", __LINE__);

    content = UR"(---<loop repeat="3">A</loop>---)";
    helper.Equal(Template::Render(content, &value), UR"(---AAA---)", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="2">A</loop><loop repeat="3">B</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(AABBB)", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="2">A</loop>-<loop repeat="3">B</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(AA-BBB)", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="2">A</loop>--<loop repeat="3">B</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(AA--BBB)", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="2">A</loop>---<loop repeat="3">B</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(AA---BBB)", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="2">A</loop>            <loop repeat="3">B</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(AA            BBB)", UR"(Render())", __LINE__);

    ////

    content = UR"(<loop repeat="4">CC</loop><loop repeat="2">A</loop><loop repeat="3">B</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(CCCCCCCCAABBB)", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="4">CC</loop>-<loop repeat="2">A</loop>-<loop repeat="3">B</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(CCCCCCCC-AA-BBB)", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="4">CC</loop>--<loop repeat="2">A</loop>--<loop repeat="3">B</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(CCCCCCCC--AA--BBB)", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="4">CC</loop>---<loop repeat="2">A</loop>---<loop repeat="3">B</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(CCCCCCCC---AA---BBB)", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="4">CC</loop>     <loop repeat="2">A</loop>            <loop repeat="3">B</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(CCCCCCCC     AA            BBB)", UR"(Render())", __LINE__);

    ////////////////

    content = UR"(<loop repeat="6"value="loop1-value">loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value), UR"(100, -50, Qentem, true, false, null, )", UR"(Render())",
                 __LINE__);

    content = UR"(<loop repeat="6" value="loop1-value">loop1-value, loop1-value </loop>)";
    helper.Equal(Template::Render(content, &value),
                 UR"(100, 100 -50, -50 Qentem, Qentem true, true false, false null, null )", UR"(Render())", __LINE__);

    content = UR"(<loop index="2" repeat="4" value="loop1-value">loop1-value{if
        case="loop1-value != null" true=", "}</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(Qentem, true, false, null)", UR"(Render())", __LINE__);

    ////////////////

    value.Reset();
    value += 0;
    value += 1;
    value += 2;

    content = UR"(<loop repeat="3" value="loop1-value"><loop
        repeat="2" value="loop2-value">(loop1-value: loop2-value) </loop></loop>)";
    helper.Equal(Template::Render(content, &value), UR"((0: 0) (0: 1) (1: 0) (1: 1) (2: 0) (2: 1) )", UR"(Render())",
                 __LINE__);

    content = UR"(<loop repeat="2" value="loop1-value"><loop repeat="2" value="loop2-value"><loop
        repeat="2" value="loop3-value">(loop1-value: loop2-value: loop3-value) </loop></loop></loop>)";
    helper.Equal(Template::Render(content, &value),
                 UR"((0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1) )", UR"(Render())",
                 __LINE__);
}

void TestLoopLTag2(TestHelper &helper) {
    Value<char32_t> value3;
    const char32_t *content;

    Value<char32_t> value1 = JSON::Parse(UR"([100, -50, "A", true, false, null])");
    Value<char32_t> value2 = JSON::Parse(UR"({"k-1": 4, "k-2":1.5, "k-3":"ABC", "k-4":true, "k-5":false, "k-6":null})");

    //////////////////////
    value3[UR"(arr1)"] = value1;

    content = UR"(<loop value="loop1-value">loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value1), UR"(100, -50, A, true, false, null, )", UR"(Render())", __LINE__);

    content = UR"(<loop value="loop1-value" index="3">loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value2), UR"(true, false, null, )", UR"(Render())", __LINE__);

    content = UR"(<loop value="loop1-value">loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value2), UR"(4, 1.5, ABC, true, false, null, )", UR"(Render())", __LINE__);

    content = UR"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value3), UR"(100, -50, A, true, false, null, )", UR"(Render())", __LINE__);

    value3[UR"(arr1)"] = value2;

    content = UR"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value3), UR"(4, 1.5, ABC, true, false, null, )", UR"(Render())", __LINE__);

    //////////////////////
    value3[UR"(arr1)"] = value1;

    content = UR"(<loop value="loop1-value" >loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value1), UR"(100, -50, A, true, false, null, )", UR"(Render())", __LINE__);

    content = UR"(<loop value="loop1-value">loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value2), UR"(4, 1.5, ABC, true, false, null, )", UR"(Render())", __LINE__);

    content = UR"(<loop value="loop1-value"set="arr1">loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value3), UR"(100, -50, A, true, false, null, )", UR"(Render())", __LINE__);

    value3[UR"(arr1)"] = value2;

    content = UR"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value3), UR"(4, 1.5, ABC, true, false, null, )", UR"(Render())", __LINE__);

    //////////////////////
    value3.Reset();
    value3[UR"(arr1)"][UR"(arr2)"][UR"(arr3)"] = value1;

    content = UR"(<loop set="arr1[arr2][arr3]" value="loop1-value">loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value3), UR"(100, -50, A, true, false, null, )", UR"(Render())", __LINE__);

    value3.Reset();
    value3[0][0] += value2;

    content = UR"(<loop set="0[0][0]"value="loop1-value">loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value3), UR"(4, 1.5, ABC, true, false, null, )", UR"(Render())", __LINE__);

    value3.Reset();
    value3[UR"(k1)"][0][UR"(k3)"] = value1;

    content = UR"(<loop value="loop1-value" set="k1[0][k3]">loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value3), UR"(100, -50, A, true, false, null, )", UR"(Render())", __LINE__);

    value3.Reset();
    value3[0][UR"(k2)"] += value2;

    content = UR"(<loop set="0[k2][0]"key="loop1-key"value="loop1-value">loop1-value, loop1-value, </loop>)";
    helper.Equal(Template::Render(content, &value3),
                 UR"(4, 4, 1.5, 1.5, ABC, ABC, true, true, false, false, null, null, )", UR"(Render())", __LINE__);

    value3 = JSON::Parse(UR"({"group":[[10],[20],[30]]})");

    content = UR"(<loop set="group" value="_Val1"><loop set="_Val1" value="_Val2">_Val2</loop></loop>)";
    helper.Equal(Template::Render(content, &value3), UR"(102030)", UR"(Render())", __LINE__);

    value3 = JSON::Parse(UR"({"group":[1,2,3,4]})");

    content = UR"(<loop set="group" value="_Val" repeat="1">_Val</loop>)";
    helper.Equal(Template::Render(content, &value3), UR"(1)", UR"(Render())", __LINE__);

    content = UR"(<loop set="group" value="_Val" index="3">_Val</loop>)";
    helper.Equal(Template::Render(content, &value3), UR"(4)", UR"(Render())", __LINE__);

    content = UR"(<loop index="2" repeat="1" set="group" value="_Val">_Val</loop>)";
    helper.Equal(Template::Render(content, &value3), UR"(3)", UR"(Render())", __LINE__);

    value3  = JSON::Parse(UR"({"numbers":[1,2,3,4,5,6,7,8]})");
    content = UR"(A<loop set="numbers" value="t">t</loop>B)";
    helper.Equal(Template::Render(content, &value3), UR"(A12345678B)", UR"(Render())", __LINE__);

    content = UR"(<loop set="numbers" value="t">t[0]</loop>)";
    helper.Equal(Template::Render(content, &value3), UR"()", UR"(Render())", __LINE__);
}

void TestLoopLTag3(TestHelper &helper) {
    Value<char32_t> value;
    const char32_t *content;

    content = UR"(<loop></loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop>abcd</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<l</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(<l</loop>)", UR"(Render())", __LINE__);

    content = UR"(<lo</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(<lo</loop>)", UR"(Render())", __LINE__);

    content = UR"(<loo</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(<loo</loop>)", UR"(Render())", __LINE__);

    content = UR"(<loop></loop><loop repeat="2">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(AA)", UR"(Render())", __LINE__);

    content = UR"(<loop key="a">a</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop value="a">a</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop set="ss" value="a">a</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop set="" value="a">a</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="2" A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="OOO">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="{var:10}">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    value[UR"(in)"] = Array<Value<char32_t>>();

    content = UR"(<loop repeat="{var:in}">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="10" index="{var:in}">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="10" index="{var:in">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="10" index="{var:100}">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="10" index="O">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop value="v">v</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(in)", UR"(Render())", __LINE__);

    value.Reset();
    value[UR"(in)"] = 2;

    content = UR"(<loop repeat="{var:in}">A</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(AA)", UR"(Render())", __LINE__);

    /////
    value.Reset();
    value[UR"(k1)"] = 10;
    value[UR"(k2)"] = 20;
    value[UR"(k3)"] = 30;

    value[UR"(k2)"].Reset();

    content = UR"(<loop value="v">v</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(1030)", UR"(Render())", __LINE__);

    value.RemoveIndex(1);

    content = UR"(<loop value="v">v)";
    helper.Equal(Template::Render(content, &value), UR"(<loop value="v">v)", UR"(Render())", __LINE__);

    content = UR"(<loop value="v">v     )";
    helper.Equal(Template::Render(content, &value), UR"(<loop value="v">v     )", UR"(Render())", __LINE__);

    value.Reset();
    value += 10;
    value += 20;
    value += 30;

    value.RemoveIndex(1);

    content = UR"(<loop value="v">v</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(1030)", UR"(Render())", __LINE__);

    value = JSON::Parse(UR"(
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

    content = UR"(<loop set="object" value="item">item[var1]item[var2]item[var3] item[var4]</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(value1value2value3 value4)", UR"(Render())", __LINE__);

    content = UR"(<loop set="array" value="item"> item[0] item[1] item[2] item[3] </loop>)";
    helper.Equal(Template::Render(content, &value), UR"( value10 value20 value30 value40 )", UR"(Render())", __LINE__);

    content = UR"(<loop set="object" value="item">item[var11]item[var22]item[var33] item[var44]</loop>)";
    helper.Equal(Template::Render(content, &value), UR"( )", UR"(Render())", __LINE__);

    content = UR"(<loop set="array" value="item">item[var11]item[var22]item[var33] item[var44]</loop>)";
    helper.Equal(Template::Render(content, &value), UR"( )", UR"(Render())", __LINE__);

    content = UR"(<loop repeat="1"><l</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(<l)", UR"(Render())", __LINE__);

    value = JSON::Parse(UR"(
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

    content = UR"(<loop set="2020">{var:name}</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(some_valsome_valsome_val)", UR"(Render())", __LINE__);
}

void TestLoopLTag4(TestHelper &helper) {
    constexpr unsigned int size_4 = (8 * 4);

    StringStream<char32_t> content;
    StringStream<char32_t> output;
    String<char32_t>       str;
    Value<char32_t>        value;

    for (unsigned int i = 0; i < size_4; i++) {
        value += i;
    }

    content += UR"(<loop repeat="1">)";
    for (unsigned int i = 0; i < size_4; i++) {
        content += UR"({var:)";
        str = Digit<char32_t>::NumberToString(i);
        content += str;
        content += UR"(})";

        output += str;
    }
    content += UR"(</loop>)";

    helper.EqualsTrue((Template::Render(content.First(), content.Length(), &value) == output), UR"(Render())",
                      __LINE__);

    //////////////////////

    content.Clear();
    output.Clear();

    content += UR"(<loop value="loop1-value">A loop1-value B</loop>)";
    for (unsigned int i = 0; i < size_4; i++) {
        output += UR"(A )";
        Digit<char32_t>::NumberToStringStream(output, i);
        output += UR"( B)";
    }

    helper.EqualsTrue((Template::Render(content.First(), content.Length(), &value) == output), UR"(Render())",
                      __LINE__);
}

void TestLoopLTag5(TestHelper &helper) {
    Value<char32_t> value;
    const char32_t *content;

    value = JSON::Parse(UR"(
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
        UR"(<loop value="val1_" group="year111" sort="descend"><loop set="val1_" value="val2_"><loop set="val2_" value="val3_">val3_</loop></loop></loop>)";

    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content =
        UR"(<loop value="val1_" group="year" sort="descend"><loop set="val1_" value="val2_"><loop set="val2_" value="val3_">val3_</loop></loop></loop>)";

    helper.Equal(Template::Render(content, &value), UR"(q11400q11450q11450q11100q11125q21200q22300q21200q22300q22300)",
                 UR"(Render())", __LINE__);

    content =
        UR"(<loop value="val1_" group="year" sort="descend"><loop set="val1_" value="val2_" group="quarter" sort="ascend"><loop set="val2_" value="val3_"><loop set="val3_" value="val4_">val4_</loop></loop></loop></loop>)";

    helper.Equal(Template::Render(content, &value), UR"(1400145014501100112512002300120023002300)", UR"(Render())",
                 __LINE__);

    content =
        UR"(<loop value="val1_" group="year" sort="descend">-- val1_-<loop set="val1_" value="val2_" group="quarter" sort="ascend">val2_-<loop set="val2_" value="val3_" group="week" sort="ascend">val2_:<loop set="val3_" value="val4_"><loop set="val4_" value="val5_"> val5_</loop></loop></loop></loop></loop>)";

    helper.Equal(
        Template::Render(content, &value),
        UR"(-- 2020-q1-1: 400 450 450-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2018-q2-1: 2002: 300-- 2017-q2-2: 300)",
        UR"(Render())", __LINE__);

    content =
        UR"(<loop value="val1_" group="year">-- val1_-<loop set="val1_" value="val2_" group="quarter">val2_-<loop set="val2_" value="val3_" group="week">val2_:<loop set="val3_" value="val4_"><loop set="val4_" value="val5_"> val5_</loop></loop></loop></loop></loop>)";

    helper.Equal(
        Template::Render(content, &value),
        UR"(-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2017-q2-2: 300-- 2020-q1-1: 400 450 450-- 2018-q2-1: 2002: 300)",
        UR"(Render())", __LINE__);

    ////////////

    value.Reset();

    value += 4;
    value += 1;
    value += 3;
    value += 5;
    value += 2;
    value += 7;
    value += 6;

    content = UR"(<loop value="val1_" sort="a">val1_</loop>)";

    helper.Equal(Template::Render(content, &value), UR"(1234567)", UR"(Render())", __LINE__);

    content = UR"(<loop value="val1_" sort="d">val1_</loop>)";

    helper.Equal(Template::Render(content, &value), UR"(7654321)", UR"(Render())", __LINE__);
}

void TestIfLTag1(TestHelper &helper) {
    Value<char32_t> value;
    const char32_t *content;

    value[UR"(name)"] = UR"(Qen)";
    value[UR"(t)"]    = true;
    value[UR"(f)"]    = false;
    value[UR"(n)"]    = nullptr;
    value[UR"(1)"]    = 1;
    value[UR"(one)"]  = UR"(1)";
    value[UR"(zero)"] = 0;

    content = UR"(<if case="1>0">{var:name}</if>)";
    helper.Equal(Template::Render(content, &value), UR"(Qen)", UR"(Render())", __LINE__);

    content = UR"(#<if case="{var:one}">{var:name}</if>#)";
    helper.Equal(Template::Render(content, &value), UR"(#Qen#)", UR"(Render())", __LINE__);

    content = UR"(##<if case="{var:zero}">{var:name}</if>##)";
    helper.Equal(Template::Render(content, &value), UR"(####)", UR"(Render())", __LINE__);

    content = UR"(<if case="{var:1}">{var:name}1<else />{var:name}2</if>)";
    helper.Equal(Template::Render(content, &value), UR"(Qen1)", UR"(Render())", __LINE__);

    content = UR"(<if case="{var:f}">{var:name}1<else />{var:name}2</if>)";
    helper.Equal(Template::Render(content, &value), UR"(Qen2)", UR"(Render())", __LINE__);

    content = UR"(#<if case="{var:t}">G<if case="1">oo</if>d</if>!#)";
    helper.Equal(Template::Render(content, &value), UR"(#Good!#)", UR"(Render())", __LINE__);

    content = UR"(###<if case="1">G<if case="1">o</if>o<if case="1">d!</if></if>###)";
    helper.Equal(Template::Render(content, &value), UR"(###Good!###)", UR"(Render())", __LINE__);

    content = UR"(Be <if case="1">G<if case="1">oo<if case="1">d</if></if></if>!)";
    helper.Equal(Template::Render(content, &value), UR"(Be Good!)", UR"(Render())", __LINE__);

    content = UR"(<if case="1">Good!<elseif case="0" />Bad!</if>)";
    helper.Equal(Template::Render(content, &value), UR"(Good!)", UR"(Render())", __LINE__);

    content = UR"(<if case="{var:n}">Bad!<elseif case="1" />Good!</if>#)";
    helper.Equal(Template::Render(content, &value), UR"(Good!#)", UR"(Render())", __LINE__);

    content = UR"(<if case="{var:f}">Bad!<elseif case="0" />Very Bad!</if>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(#<if case="0">Bad!<elseif case="0" />Very Bad!<else />Very Good!</if>)";
    helper.Equal(Template::Render(content, &value), UR"(#Very Good!)", UR"(Render())", __LINE__);

    content = UR"(<if case="1">a<else />b</if>)";
    helper.Equal(Template::Render(content, &value), UR"(a)", UR"(Render())", __LINE__);

    content = UR"(<if case="0">a<else />b</if>)";
    helper.Equal(Template::Render(content, &value), UR"(b)", UR"(Render())", __LINE__);

    content = UR"(<if case="0">a<else /><if case="1">b</if></if>)";
    helper.Equal(Template::Render(content, &value), UR"(b)", UR"(Render())", __LINE__);

    content = UR"(<if case="1">a<else /><if case="1">b</if></if>)";
    helper.Equal(Template::Render(content, &value), UR"(a)", UR"(Render())", __LINE__);

    content = UR"(<if case="0">a<if case="1">b</if>c</if>Empty)";
    helper.Equal(Template::Render(content, &value), UR"(Empty)", UR"(Render())", __LINE__);

    content = UR"(<if case="1">a<else /><if case="1">b</if>c</if>===========)";
    helper.Equal(Template::Render(content, &value), UR"(a===========)", UR"(Render())", __LINE__);

    content = UR"(===========<if case="1">a<if case="1">b</if><else />c</if>)";
    helper.Equal(Template::Render(content, &value), UR"(===========ab)", UR"(Render())", __LINE__);

    content = UR"(<if case="1">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    helper.Equal(Template::Render(content, &value), UR"(a)", UR"(Render())", __LINE__);

    content = UR"(<if case="0">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    helper.Equal(Template::Render(content, &value), UR"(b)", UR"(Render())", __LINE__);

    content = UR"(===========<if case="0">a<elseif case="0" />b<elseif case="1" />c<else />d</if>===========)";
    helper.Equal(Template::Render(content, &value), UR"(===========c===========)", UR"(Render())", __LINE__);

    content = UR"(<if case="0">a<elseif case="0" />b<elseif case="0" />c<else />d</if>)";
    helper.Equal(Template::Render(content, &value), UR"(d)", UR"(Render())", __LINE__);

    content = UR"(<if case="1">a<if case="0">b<elseif case="0"/>c</if></if>)";
    helper.Equal(Template::Render(content, &value), UR"(a)", UR"(Render())", __LINE__);

    content = UR"(<if case="1">a<if case="1">b<elseif case="0"/>c</if></if>)";
    helper.Equal(Template::Render(content, &value), UR"(ab)", UR"(Render())", __LINE__);

    content = UR"(<if case="1"><if case="1">b<elseif case="c"/>c</if>a<else />c</if>)";
    helper.Equal(Template::Render(content, &value), UR"(ba)", UR"(Render())", __LINE__);

    content = UR"(<if case="0">a<else />c<if case="1">b<elseif case="0"/>c</if></if>)";
    helper.Equal(Template::Render(content, &value), UR"(cb)", UR"(Render())", __LINE__);

    content = UR"(<if_case="1"><if case="0">Bad1!<elseif case="0" />Bad2!</if>a</if>)";
    helper.Equal(Template::Render(content, &value), UR"(a)", UR"(Render())", __LINE__);

    content = UR"(<if case="1">
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
    helper.Equal(String<char32_t>::Trim(Template::Render(content, &value).GetString()), UR"(a)", UR"(Render())",
                 __LINE__);

    content = UR"(<if case="0">a
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
    helper.Equal(String<char32_t>::Trim(Template::Render(content, &value).GetString()), UR"(b)", UR"(Render())",
                 __LINE__);

    content = UR"(<if case="0">
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
    helper.Equal(String<char32_t>::Trim(Template::Render(content, &value).GetString()), UR"(c)", UR"(Render())",
                 __LINE__);

    content = UR"(<if case="0">a
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
    helper.Equal(String<char32_t>::Trim(Template::Render(content, &value).GetString()), UR"(d)", UR"(Render())",
                 __LINE__);
}

void TestIfLTag2(TestHelper &helper) {
    Value<char32_t> value;
    const char32_t *content;

    value[UR"(name)"] = UR"(Qentem)";

    content = UR"(<if case="1">{var:name})";
    helper.Equal(Template::Render(content, &value), UR"(<if case="1">Qentem)", UR"(Render())", __LINE__);

    content = UR"(<if<if case="1">{var:name}</if>)";
    helper.Equal(Template::Render(content, &value), UR"(<ifQentem)", UR"(Render())", __LINE__);

    content = UR"(<if case="1"><if case="1">{var:name}</if>)";
    helper.Equal(Template::Render(content, &value), UR"(<if case="1">Qentem)", UR"(Render())", __LINE__);

    content = UR"(<if case="1"><if case="1"><if case="1">{var:name}</if></if>)";
    helper.Equal(Template::Render(content, &value), UR"(<if case="1">Qentem)", UR"(Render())", __LINE__);

    content = UR"(<if case="ABC">{var:name}</if>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<if>{var:name}</if>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<if case="0"><elseif />{var:name}</if>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<iw case="0">{var:name}</if>)";
    helper.Equal(Template::Render(content, &value), UR"(<iw case="0">Qentem</if>)", UR"(Render())", __LINE__);

    content = UR"(<if case="0"{var:name}</if>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<if case="0"><else {var:name}</if>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);
}

void TestRenderL1(TestHelper &helper) {
    constexpr unsigned int size_4 = (8 * 4);

    StringStream<char32_t> content;
    StringStream<char32_t> output;
    String<char32_t>       str;
    Value<char32_t>        value;

    for (unsigned int i = 0; i < size_4; i++) {
        value += i;
    }

    for (unsigned int i = 0; i < size_4; i++) {
        content += UR"({var:)";
        str = Digit<char32_t>::NumberToString(i);
        content += str;
        output += str;
        content += UR"(})";
    }

    helper.EqualsTrue((Template::Render(content.First(), content.Length(), &value) == output), UR"(Render())",
                      __LINE__);

    content.Clear();
    output.Clear();

    unsigned int size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += UR"({var:)";
            str = Digit<char32_t>::NumberToString(i);
            content += str;
            content += UR"(})";

            output += str;
        } else {
            size += 8;

            content += UR"({math: 1 + )";
            str = Digit<char32_t>::NumberToString(i);
            content += str;
            content += UR"(})";

            Digit<char32_t>::NumberToStringStream(output, 1U + i);
        }
    }

    helper.EqualsTrue((Template::Render(content.First(), content.Length(), &value) == output), UR"(Render())",
                      __LINE__);

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += UR"({var:)";
            str = Digit<char32_t>::NumberToString(i);
            content += str;
            content += UR"(})";

            output += str;
        } else {
            size += 8;

            content += UR"({if case="{var:)";
            Digit<char32_t>::NumberToStringStream(content, i);
            content += UR"(}<)";
            Digit<char32_t>::NumberToStringStream(content, 1U + i);
            content += UR"(" true="yes"})";

            output += UR"(yes)";
        }
    }

    helper.EqualsTrue((Template::Render(content.First(), content.Length(), &value) == output), UR"(Render())",
                      __LINE__);

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += UR"({var:)";
            str = Digit<char32_t>::NumberToString(i);
            content += str;
            content += UR"(})";

            output += str;
        } else {
            size += 8;
            content += UR"(<loop repeat="1">A</loop>)";
            output += UR"(A)";
        }
    }

    helper.EqualsTrue((Template::Render(content.First(), content.Length(), &value) == output), UR"(Render())",
                      __LINE__);

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += UR"({var:)";
            str = Digit<char32_t>::NumberToString(i);
            content += str;
            content += UR"(})";

            output += str;
        } else {
            size += 8;
            content += UR"(<if case="1">A</if>)";
            output += UR"(A)";
        }
    }

    helper.EqualsTrue((Template::Render(content.First(), content.Length(), &value) == output), UR"(Render())",
                      __LINE__);

    content.Clear();
    output.Clear();

    constexpr unsigned int size_2_1 = (8 * 2) - 1;

    for (unsigned int i = 0; i < size_2_1; i++) {
        value += i;
    }

    content += UR"(<loop repeat="1">)";
    for (unsigned int i = 0; i < size_2_1; i++) {
        content += UR"({var:)";
        str = Digit<char32_t>::NumberToString(i);
        content += str;
        output += str;
        content += UR"(})";
    }
    content += UR"(</loop>)";

    helper.EqualsTrue((Template::Render(content.First(), content.Length(), &value) == output), UR"(Render())",
                      __LINE__);
}

void TestRenderL2(TestHelper &helper) {
    Value<char32_t> value;
    const char32_t *content;

    value += 0;
    value += 1;
    value += 2;
    value += 5;
    value += 10;

    content = UR"(<loop set="numbers" value="val_">val_</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop value="this_number"><if case="(this_number % 2) == 1">this_number</if></loop>)";
    helper.Equal(Template::Render(content, &value), UR"(15)", UR"(Render())", __LINE__);

    content = UR"(<loop value="loop1_val">{if case="loop1_val < 5", true="loop1_val"}</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(012)", UR"(Render())", __LINE__);

    content = UR"(<loop value="loop1_val">{if case="loop1_val < 5", true="{var:4}"}</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(101010)", UR"(Render())", __LINE__);

    content = UR"(<loop value="loop1_val">loop1_val[]</loop>)";
    helper.Equal(Template::Render(content, &value), UR"()", UR"(Render())", __LINE__);

    content = UR"(<loop value="loop1_val">loop1_val[0 </loop>)";
    helper.Equal(Template::Render(content, &value),
                 UR"({var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 )",
                 UR"(Render())", __LINE__);

    value = JSON::Parse(UR"([[[1,2,3]]])");

    content = UR"(<loop value="loop1_val">loop1_val[0][2]</loop>)";
    helper.Equal(Template::Render(content, &value), UR"(3)", UR"(Render())", __LINE__);
}

static int RunTemplateLTests() {
    TestHelper helper{"Template.hpp (32-bit character)", __FILE__};

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
    helper.Test("Loop Tag Test 4", TestLoopLTag4);
    helper.Test("Loop Tag Test 5", TestLoopLTag5);

    helper.Test("If Tag Test 1", TestIfLTag1);
    helper.Test("If Tag Test 2", TestIfLTag2);

    helper.Test("Render Test 1", TestRenderL1);
    helper.Test("Render Test 2", TestRenderL2);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
