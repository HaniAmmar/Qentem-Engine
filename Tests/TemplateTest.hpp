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

#ifndef QENTEM_TEMPLATE_TESTS_H_
#define QENTEM_TEMPLATE_TESTS_H_

namespace Qentem {
namespace Test {

static int TestVariableTag1() {
    const char *content;

    Value<char> value = JSON::Parse(
        R"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    Value<char> sub_value = JSON::Parse(
        R"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = R"({var:0})";
    EQ_VALUE(Template::Render(content, &value), R"(A)", "Render()");

    content = R"({var:1})";
    EQ_VALUE(Template::Render(content, &value), R"(abc)", "Render()");

    content = R"({var:2})";
    EQ_VALUE(Template::Render(content, &value), R"(true)", "Render()");

    content = R"({var:3})";
    EQ_VALUE(Template::Render(content, &value), R"(456)", "Render()");

    content = R"({var:4})";
    EQ_VALUE(Template::Render(content, &value), R"(1.5)", "Render()");

    content = R"({var:5[0]})";
    EQ_VALUE(Template::Render(content, &value), R"(null)", "Render()");

    content = R"({var:5[1]})";
    EQ_VALUE(Template::Render(content, &value), R"(false)", "Render()");

    content = R"({var:5[2][0]})";
    EQ_VALUE(Template::Render(content, &value), R"(Qentem)", "Render()");

    //////

    content = R"({var:key1})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(a)", "Render()");

    content = R"({var:key2})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(ABC)", "Render()");

    content = R"({var:key3})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(false)", "Render()");

    content = R"({var:key4})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(100)", "Render()");

    content = R"({var:key5})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(1.5)", "Render()");

    content = R"({var:key6[one]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(1)", "Render()");

    content = R"({var:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(null)", "Render()");

    content = R"({var:key7[1]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(false)", "Render()");

    content = R"({var:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(Qentem)", "Render()");

    //
    content = R"({var:6[key1]})";
    EQ_VALUE(Template::Render(content, &value), R"(a)", "Render()");

    content = R"({var:6[key2]})";
    EQ_VALUE(Template::Render(content, &value), R"(ABC)", "Render()");

    content = R"({var:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), R"(false)", "Render()");

    content = R"({var:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), R"(100)", "Render()");

    content = R"({var:6[key5]})";
    EQ_VALUE(Template::Render(content, &value), R"(1.5)", "Render()");

    content = R"({var:6[key6][one]})";
    EQ_VALUE(Template::Render(content, &value), R"(1)", "Render()");

    ////////////////

    content = R"(-{var:2})";
    EQ_VALUE(Template::Render(content, &value), R"(-true)", "Render()");

    content = R"(-{var:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(-null)", "Render()");

    content = R"(-{var:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(-Qentem)", "Render()");

    content = R"(-{var:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), R"(-false)", "Render()");

    content = R"(-{var:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), R"(-100)", "Render()");
    ////////////

    content = R"({var:2}-)";
    EQ_VALUE(Template::Render(content, &value), R"(true-)", "Render()");

    content = R"({var:key7[0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(null-)", "Render()");

    content = R"({var:key7[2][0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(Qentem-)", "Render()");

    content = R"({var:6[key3]}-)";
    EQ_VALUE(Template::Render(content, &value), R"(false-)", "Render()");

    content = R"({var:6[key4]}-)";
    EQ_VALUE(Template::Render(content, &value), R"(100-)", "Render()");

    ////////////

    content = R"(-{var:2}-)";
    EQ_VALUE(Template::Render(content, &value), R"(-true-)", "Render()");

    content = R"(-{var:key7[0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(-null-)", "Render()");

    content = R"(-{var:key7[2][0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(-Qentem-)", "Render()");

    content = R"(-{var:6[key3]}-)";
    EQ_VALUE(Template::Render(content, &value), R"(-false-)", "Render()");

    content = R"(-{var:6[key4]}-)";
    EQ_VALUE(Template::Render(content, &value), R"(-100-)", "Render()");

    ////////////

    content = R"(------{var:2})";
    EQ_VALUE(Template::Render(content, &value), R"(------true)", "Render()");

    content = R"(------{var:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(------null)",
             "Render()");

    content = R"(------{var:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(------Qentem)",
             "Render()");

    content = R"(------{var:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), R"(------false)", "Render()");

    content = R"(------{var:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), R"(------100)", "Render()");

    ////////////

    content = R"({var:2}------)";
    EQ_VALUE(Template::Render(content, &value), R"(true------)", "Render()");

    content = R"({var:key7[0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(null------)",
             "Render()");

    content = R"({var:key7[2][0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(Qentem------)",
             "Render()");

    content = R"({var:6[key3]}------)";
    EQ_VALUE(Template::Render(content, &value), R"(false------)", "Render()");

    content = R"({var:6[key4]}------)";
    EQ_VALUE(Template::Render(content, &value), R"(100------)", "Render()");

    ////////////

    content = R"(------{var:2}------)";
    EQ_VALUE(Template::Render(content, &value), R"(------true------)",
             "Render()");

    content = R"(------{var:key7[0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(------null------)",
             "Render()");

    content = R"(------{var:key7[2][0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(------Qentem------)",
             "Render()");

    content = R"(------{var:6[key3]}------)";
    EQ_VALUE(Template::Render(content, &value), R"(------false------)",
             "Render()");

    content = R"(------{var:6[key4]}------)";
    EQ_VALUE(Template::Render(content, &value), R"(------100------)",
             "Render()");

    content = R"({var:key7[2[0]})";
    EQ_VALUE(Template::Render(content, &value), R"({var:key7[2[0]})",
             "Render()");

    content = R"({var:6key3]})";
    EQ_VALUE(Template::Render(content, &value), R"({var:6key3]})", "Render()");

    END_SUB_TEST;
}

static int TestVariableTag2() {
    Value<char> value = JSON::Parse(R"(["A", "abc", true, 456, 1.5])");
    const char *content;

    content = R"({var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"(AA)", "Render()");

    content = R"({var:1}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"(abcA)", "Render()");

    content = R"({var:1}{var:2})";
    EQ_VALUE(Template::Render(content, &value), R"(abctrue)", "Render()");

    content = R"({var:2}{var:3}{var:2})";
    EQ_VALUE(Template::Render(content, &value), R"(true456true)", "Render()");

    content = R"({var:4}{var:4}{var:4})";
    EQ_VALUE(Template::Render(content, &value), R"(1.51.51.5)", "Render()");
    ///

    content = R"({var:0}-{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"(A-A)", "Render()");

    content = R"({var:1}--{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"(abc--A)", "Render()");

    content = R"({var:1}---{var:2})";
    EQ_VALUE(Template::Render(content, &value), R"(abc---true)", "Render()");

    content = R"({var:2}{var:3}--{var:2})";
    EQ_VALUE(Template::Render(content, &value), R"(true456--true)", "Render()");

    content = R"({var:4}--{var:4}{var:4})";
    EQ_VALUE(Template::Render(content, &value), R"(1.5--1.51.5)", "Render()");

    content = R"({var:4}--{var:4}--{var:4})";
    EQ_VALUE(Template::Render(content, &value), R"(1.5--1.5--1.5)", "Render()");

    content = R"({var:4}---{var:4}---{var:4})";
    EQ_VALUE(Template::Render(content, &value), R"(1.5---1.5---1.5)",
             "Render()");

    END_SUB_TEST;
}

static int TestVariableTag3() {
    Value<char> value;
    const char *content;

    content = R"({var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({var:0})", "Render()");

    EQ_VALUE(Template::Render(R"({var:a})", &value), R"({var:a})", "Render()");

    content = R"({var:0[0]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             R"({var:0[0]})", "Render()");

    content = R"({var:a[0]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             R"({var:a[0]})", "Render()");

    content = R"({var:0[a]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             R"({var:0[a]})", "Render()");

    content = R"({var:a[abc]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             R"({var:a[abc]})", "Render()");

    ////////////////

    value = JSON::Parse(R"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = R"({var:0})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             R"({var:0})", "Render()");

    content = R"({var:0[0]})";
    EQ_VALUE(Template::Render(content, &value), R"({var:0[0]})", "Render()");

    content = R"({var:0[0][0]})";
    EQ_VALUE(Template::Render(content, &value), R"({var:0[0][0]})", "Render()");

    /////

    content = R"({var:1})";
    EQ_VALUE(Template::Render(content, &value), R"({var:1})", "Render()");

    content = R"({var:2})";
    EQ_VALUE(Template::Render(content, &value), R"({var:2})", "Render()");

    content = R"({var:1[a]})";
    EQ_VALUE(Template::Render(content, &value), R"({var:1[a]})", "Render()");

    content = R"({var:1[b]})";
    EQ_VALUE(Template::Render(content, &value), R"({var:1[b]})", "Render()");

    ////

    value = JSON::Parse(R"(["A", "abc", true, 456, 1.5])");

    content = R"({var:0)";
    EQ_VALUE(Template::Render(content, &value), R"({var:0)", "Render()");

    content = R"(var:0})";
    EQ_VALUE(Template::Render(content, &value), R"(var:0})", "Render()");

    content = R"({v})";
    EQ_VALUE(Template::Render(content, &value), R"({v})", "Render()");

    content = R"({va})";
    EQ_VALUE(Template::Render(content, &value), R"({va})", "Render()");

    content = R"({var})";
    EQ_VALUE(Template::Render(content, &value), R"({var})", "Render()");

    content = R"({var:})";
    EQ_VALUE(Template::Render(content, &value), R"({var:})", "Render()");

    content = R"({v:0})";
    EQ_VALUE(Template::Render(content, &value), R"({v:0})", "Render()");

    content = R"({va:0})";
    EQ_VALUE(Template::Render(content, &value), R"({va:0})", "Render()");
    ////

    content = R"({var:0{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({var:0{var:0})", "Render()");

    content = R"(var:0{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"(var:0A)", "Render()");

    content = R"(var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"(var:0}A)", "Render()");

    content = R"({var:0{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({var:0{var:0}A)",
             "Render()");

    ////

    content = R"({var:0{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({var:0{var:0})", "Render()");

    content = R"(var:0{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"(var:0A)", "Render()");

    content = R"(var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"(var:0}A)", "Render()");

    content = R"({var:0{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({var:0{var:0}A)",
             "Render()");

    ////

    content = R"({{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({A)", "Render()");

    content = R"({{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({AA)", "Render()");

    content = R"({v{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({vA)", "Render()");

    content = R"({v{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({vAA)", "Render()");

    content = R"({va{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({vaA)", "Render()");

    content = R"({va{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({vaAA)", "Render()");

    content = R"({var{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({varA)", "Render()");

    content = R"({var{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({varAA)", "Render()");

    ///

    content = R"({-{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({-A)", "Render()");

    content = R"({-{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({-AA)", "Render()");

    content = R"({v-{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({v-A)", "Render()");

    content = R"({v-{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({v-AA)", "Render()");

    content = R"({va-{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({va-A)", "Render()");

    content = R"({va-{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({va-AA)", "Render()");

    content = R"({var-{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({var-A)", "Render()");

    content = R"({var-{var:0}{var:0})";
    EQ_VALUE(Template::Render(content, &value), R"({var-AA)", "Render()");

    //

    content = R"({var-0})";
    EQ_VALUE(Template::Render(content, &value), R"({var-0})", "Render()");

    content = R"({var 0})";
    EQ_VALUE(Template::Render(content, &value), R"({var 0})", "Render()");

    content = R"({var:0 })";
    EQ_VALUE(Template::Render(content, &value), R"({var:0 })", "Render()");

    content = R"({var:0 )";
    EQ_VALUE(Template::Render(content, &value), R"({var:0 )", "Render()");

    content = R"({var:0)";
    EQ_VALUE(Template::Render(content, &value), R"({var:0)", "Render()");

    END_SUB_TEST;
}

static int TestVariableTag4() {
    Value<char> value;

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

#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
    EQ_VALUE(Template::Render(R"({var:0})", &value), R"(&lt;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:1})", &value), R"(&gt;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:2})", &value), R"(&amp;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:3})", &value), R"(&quot;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:4})", &value), R"(&apos;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:5})", &value), R"(&lt;&gt;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:6})", &value), R"(&lt;&amp;)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:7})", &value), R"(&lt;&amp;)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:8})", &value), R"(&gt;&quot;)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:9})", &value), R"(&quot;&apos;)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:10})", &value), R"(&lt;&quot;&gt;)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:11})", &value), R"(&lt;&apos;&gt;)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:12})", &value), R"(&lt;&amp;&gt;)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:13})", &value), R"(&amp;&quot;&amp;)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:14})", &value), R"(&quot;&apos;&quot;)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:15})", &value), R"(&apos;&lt;&apos;)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:16})", &value), R"(&apos;&amp;&apos;)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:17})", &value),
             R"(&lt;&gt;&amp;&apos;&quot;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:18})", &value),
             R"(&apos;&quot;&lt;&gt;&amp;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:19})", &value),
             R"(&lt;&quot;&amp;&apos;&gt;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:20})", &value), R"(&lt;&lt;&lt;&lt;&lt;)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:21})", &value), R"(&gt;&gt;&gt;&gt;&gt;)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:22})", &value),
             R"(&amp;&amp;&amp;&amp;&amp;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:23})", &value),
             R"(&quot;&quot;&quot;&quot;&quot;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:24})", &value),
             R"(&apos;&apos;&apos;&apos;&apos;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:25})", &value), R"(A&lt;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:26})", &value), R"(A&gt;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:27})", &value), R"(A&amp;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:28})", &value), R"(A&quot;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:29})", &value), R"(A&apos;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:30})", &value), R"(&lt;A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:31})", &value), R"(&gt;A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:32})", &value), R"(&amp;A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:33})", &value), R"(&quot;A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:34})", &value), R"(&apos;A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:35})", &value), R"(A&lt;A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:36})", &value), R"(A&gt;A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:37})", &value), R"(A&amp;A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:38})", &value), R"(A&quot;A)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:39})", &value), R"(A&apos;A)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:40})", &value), R"(AA&lt;AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:41})", &value), R"(AA&gt;AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:42})", &value), R"(AA&amp;AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:43})", &value), R"(AA&quot;AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:44})", &value), R"(AA&apos;AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:45})", &value), R"(AA&lt;&lt;&lt;&lt;AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:46})", &value), R"(AA&gt;&gt;&gt;&gt;AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:47})", &value),
             R"(AA&amp;&amp;&amp;&amp;AA)", "Render()");
    EQ_VALUE(Template::Render(R"({var:48})", &value),
             R"(AA&quot;&quot;&quot;&quot;AA)", "Render()");
    EQ_VALUE(Template::Render(R"({var:49})", &value),
             R"(AA&apos;&apos;&apos;&apos;AA)", "Render()");
    EQ_VALUE(
        Template::Render(R"({var:50})", &value),
        R"(&lt;A&gt;B&apos;C&quot;D&amp;E&apos;F&quot;G&lt;H&gt;I&amp;G&quot;K)",
        "Render()");
    EQ_VALUE(Template::Render(R"({var:51})", &value),
             R"(AB&quot;CD&apos;EF&lt;GH&gt;IGK&apos;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:52})", &value),
             R"(&quot;ABC&apos;DEF&lt;GHI&gt;GK&lt;)", "Render()");
    EQ_VALUE(Template::Render(R"({var:53})", &value),
             R"(A&quot;&quot;BC&lt;&lt;DE&gt;&gt;FG&apos;&apos;HI&amp;&amp;GK)",
             "Render()");
#else
    EQ_VALUE(Template::Render(R"({var:0})", &value), R"(<)", "Render()");
    EQ_VALUE(Template::Render(R"({var:1})", &value), R"(>)", "Render()");
    EQ_VALUE(Template::Render(R"({var:2})", &value), R"(&)", "Render()");
    EQ_VALUE(Template::Render(R"({var:3})", &value), R"(")", "Render()");
    EQ_VALUE(Template::Render(R"({var:4})", &value), R"(')", "Render()");
    EQ_VALUE(Template::Render(R"({var:5})", &value), R"(<>)", "Render()");
    EQ_VALUE(Template::Render(R"({var:6})", &value), R"(<&)", "Render()");
    EQ_VALUE(Template::Render(R"({var:7})", &value), R"(<&)", "Render()");
    EQ_VALUE(Template::Render(R"({var:8})", &value), R"(>")", "Render()");
    EQ_VALUE(Template::Render(R"({var:9})", &value), R"("')", "Render()");
    EQ_VALUE(Template::Render(R"({var:10})", &value), R"(<">)", "Render()");
    EQ_VALUE(Template::Render(R"({var:11})", &value), R"(<'>)", "Render()");
    EQ_VALUE(Template::Render(R"({var:12})", &value), R"(<&>)", "Render()");
    EQ_VALUE(Template::Render(R"({var:13})", &value), R"(&"&)", "Render()");
    EQ_VALUE(Template::Render(R"({var:14})", &value), R"("'")", "Render()");
    EQ_VALUE(Template::Render(R"({var:15})", &value), R"('<')", "Render()");
    EQ_VALUE(Template::Render(R"({var:16})", &value), R"('&')", "Render()");
    EQ_VALUE(Template::Render(R"({var:17})", &value), R"(<>&'")", "Render()");
    EQ_VALUE(Template::Render(R"({var:18})", &value), R"('"<>&)", "Render()");
    EQ_VALUE(Template::Render(R"({var:19})", &value), R"(<"&'>)", "Render()");
    EQ_VALUE(Template::Render(R"({var:20})", &value), R"(<<<<<)", "Render()");
    EQ_VALUE(Template::Render(R"({var:21})", &value), R"(>>>>>)", "Render()");
    EQ_VALUE(Template::Render(R"({var:22})", &value), R"(&&&&&)", "Render()");
    EQ_VALUE(Template::Render(R"({var:23})", &value), R"(""""")", "Render()");
    EQ_VALUE(Template::Render(R"({var:24})", &value), R"(''''')", "Render()");
    EQ_VALUE(Template::Render(R"({var:25})", &value), R"(A<)", "Render()");
    EQ_VALUE(Template::Render(R"({var:26})", &value), R"(A>)", "Render()");
    EQ_VALUE(Template::Render(R"({var:27})", &value), R"(A&)", "Render()");
    EQ_VALUE(Template::Render(R"({var:28})", &value), R"(A")", "Render()");
    EQ_VALUE(Template::Render(R"({var:29})", &value), R"(A')", "Render()");
    EQ_VALUE(Template::Render(R"({var:30})", &value), R"(<A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:31})", &value), R"(>A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:32})", &value), R"(&A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:33})", &value), R"("A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:34})", &value), R"('A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:35})", &value), R"(A<A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:36})", &value), R"(A>A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:37})", &value), R"(A&A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:38})", &value), R"(A"A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:39})", &value), R"(A'A)", "Render()");
    EQ_VALUE(Template::Render(R"({var:40})", &value), R"(AA<AA)", "Render()");
    EQ_VALUE(Template::Render(R"({var:41})", &value), R"(AA>AA)", "Render()");
    EQ_VALUE(Template::Render(R"({var:42})", &value), R"(AA&AA)", "Render()");
    EQ_VALUE(Template::Render(R"({var:43})", &value), R"(AA"AA)", "Render()");
    EQ_VALUE(Template::Render(R"({var:44})", &value), R"(AA'AA)", "Render()");
    EQ_VALUE(Template::Render(R"({var:45})", &value), R"(AA<<<<AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:46})", &value), R"(AA>>>>AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:47})", &value), R"(AA&&&&AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:48})", &value), R"(AA""""AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:49})", &value), R"(AA''''AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:50})", &value),
             R"(<A>B'C"D&E'F"G<H>I&G"K)", "Render()");
    EQ_VALUE(Template::Render(R"({var:51})", &value), R"(AB"CD'EF<GH>IGK')",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:52})", &value), R"("ABC'DEF<GHI>GK<)",
             "Render()");
    EQ_VALUE(Template::Render(R"({var:53})", &value),
             R"(A""BC<<DE>>FG''HI&&GK)", "Render()");
#endif

    END_SUB_TEST;
}

static int TestRawVariableTag1() {
    const char *content;

    Value<char> value = JSON::Parse(
        R"(["A", "abc", true, 456, 1.5,
           [null, false, ["Qentem"]]])");

    Value<char> sub_value = JSON::Parse(
        R"({"key1": "a", "key2": "ABC", "key3": false, "key4": 100, "key5": 1.5, "key6": {"one": 1}
        ,"key7": [null, false, ["Qentem"]]})");

    value += sub_value;

    content = R"({raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"(A)", "Render()");

    content = R"({raw:1})";
    EQ_VALUE(Template::Render(content, &value), R"(abc)", "Render()");

    content = R"({raw:2})";
    EQ_VALUE(Template::Render(content, &value), R"(true)", "Render()");

    content = R"({raw:3})";
    EQ_VALUE(Template::Render(content, &value), R"(456)", "Render()");

    content = R"({raw:4})";
    EQ_VALUE(Template::Render(content, &value), R"(1.5)", "Render()");

    content = R"({raw:5[0]})";
    EQ_VALUE(Template::Render(content, &value), R"(null)", "Render()");

    content = R"({raw:5[1]})";
    EQ_VALUE(Template::Render(content, &value), R"(false)", "Render()");

    content = R"({raw:5[2][0]})";
    EQ_VALUE(Template::Render(content, &value), R"(Qentem)", "Render()");

    //////

    content = R"({raw:key1})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(a)", "Render()");

    content = R"({raw:key2})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(ABC)", "Render()");

    content = R"({raw:key3})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(false)", "Render()");

    content = R"({raw:key4})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(100)", "Render()");

    content = R"({raw:key5})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(1.5)", "Render()");

    content = R"({raw:key6[one]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(1)", "Render()");

    content = R"({raw:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(null)", "Render()");

    content = R"({raw:key7[1]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(false)", "Render()");

    content = R"({raw:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(Qentem)", "Render()");

    //
    content = R"({raw:6[key1]})";
    EQ_VALUE(Template::Render(content, &value), R"(a)", "Render()");

    content = R"({raw:6[key2]})";
    EQ_VALUE(Template::Render(content, &value), R"(ABC)", "Render()");

    content = R"({raw:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), R"(false)", "Render()");

    content = R"({raw:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), R"(100)", "Render()");

    content = R"({raw:6[key5]})";
    EQ_VALUE(Template::Render(content, &value), R"(1.5)", "Render()");

    content = R"({raw:6[key6][one]})";
    EQ_VALUE(Template::Render(content, &value), R"(1)", "Render()");

    ////////////////

    content = R"(-{raw:2})";
    EQ_VALUE(Template::Render(content, &value), R"(-true)", "Render()");

    content = R"(-{raw:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(-null)", "Render()");

    content = R"(-{raw:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(-Qentem)", "Render()");

    content = R"(-{raw:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), R"(-false)", "Render()");

    content = R"(-{raw:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), R"(-100)", "Render()");
    ////////////

    content = R"({raw:2}-)";
    EQ_VALUE(Template::Render(content, &value), R"(true-)", "Render()");

    content = R"({raw:key7[0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(null-)", "Render()");

    content = R"({raw:key7[2][0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(Qentem-)", "Render()");

    content = R"({raw:6[key3]}-)";
    EQ_VALUE(Template::Render(content, &value), R"(false-)", "Render()");

    content = R"({raw:6[key4]}-)";
    EQ_VALUE(Template::Render(content, &value), R"(100-)", "Render()");

    ////////////

    content = R"(-{raw:2}-)";
    EQ_VALUE(Template::Render(content, &value), R"(-true-)", "Render()");

    content = R"(-{raw:key7[0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(-null-)", "Render()");

    content = R"(-{raw:key7[2][0]}-)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(-Qentem-)", "Render()");

    content = R"(-{raw:6[key3]}-)";
    EQ_VALUE(Template::Render(content, &value), R"(-false-)", "Render()");

    content = R"(-{raw:6[key4]}-)";
    EQ_VALUE(Template::Render(content, &value), R"(-100-)", "Render()");

    ////////////

    content = R"(------{raw:2})";
    EQ_VALUE(Template::Render(content, &value), R"(------true)", "Render()");

    content = R"(------{raw:key7[0]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(------null)",
             "Render()");

    content = R"(------{raw:key7[2][0]})";
    EQ_VALUE(Template::Render(content, &sub_value), R"(------Qentem)",
             "Render()");

    content = R"(------{raw:6[key3]})";
    EQ_VALUE(Template::Render(content, &value), R"(------false)", "Render()");

    content = R"(------{raw:6[key4]})";
    EQ_VALUE(Template::Render(content, &value), R"(------100)", "Render()");

    ////////////

    content = R"({raw:2}------)";
    EQ_VALUE(Template::Render(content, &value), R"(true------)", "Render()");

    content = R"({raw:key7[0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(null------)",
             "Render()");

    content = R"({raw:key7[2][0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(Qentem------)",
             "Render()");

    content = R"({raw:6[key3]}------)";
    EQ_VALUE(Template::Render(content, &value), R"(false------)", "Render()");

    content = R"({raw:6[key4]}------)";
    EQ_VALUE(Template::Render(content, &value), R"(100------)", "Render()");

    ////////////

    content = R"(------{raw:2}------)";
    EQ_VALUE(Template::Render(content, &value), R"(------true------)",
             "Render()");

    content = R"(------{raw:key7[0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(------null------)",
             "Render()");

    content = R"(------{raw:key7[2][0]}------)";
    EQ_VALUE(Template::Render(content, &sub_value), R"(------Qentem------)",
             "Render()");

    content = R"(------{raw:6[key3]}------)";
    EQ_VALUE(Template::Render(content, &value), R"(------false------)",
             "Render()");

    content = R"(------{raw:6[key4]}------)";
    EQ_VALUE(Template::Render(content, &value), R"(------100------)",
             "Render()");

    content = R"({raw:key7[2[0]})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:key7[2[0]})",
             "Render()");

    content = R"({raw:6key3]})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:6key3]})", "Render()");

    END_SUB_TEST;
}

static int TestRawVariableTag2() {
    Value<char> value = JSON::Parse(R"(["A", "abc", true, 456, 1.5])");
    const char *content;

    content = R"({raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"(AA)", "Render()");

    content = R"({raw:1}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"(abcA)", "Render()");

    content = R"({raw:1}{raw:2})";
    EQ_VALUE(Template::Render(content, &value), R"(abctrue)", "Render()");

    content = R"({raw:2}{raw:3}{raw:2})";
    EQ_VALUE(Template::Render(content, &value), R"(true456true)", "Render()");

    content = R"({raw:4}{raw:4}{raw:4})";
    EQ_VALUE(Template::Render(content, &value), R"(1.51.51.5)", "Render()");
    ///

    content = R"({raw:0}-{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"(A-A)", "Render()");

    content = R"({raw:1}--{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"(abc--A)", "Render()");

    content = R"({raw:1}---{raw:2})";
    EQ_VALUE(Template::Render(content, &value), R"(abc---true)", "Render()");

    content = R"({raw:2}{raw:3}--{raw:2})";
    EQ_VALUE(Template::Render(content, &value), R"(true456--true)", "Render()");

    content = R"({raw:4}--{raw:4}{raw:4})";
    EQ_VALUE(Template::Render(content, &value), R"(1.5--1.51.5)", "Render()");

    content = R"({raw:4}--{raw:4}--{raw:4})";
    EQ_VALUE(Template::Render(content, &value), R"(1.5--1.5--1.5)", "Render()");

    content = R"({raw:4}---{raw:4}---{raw:4})";
    EQ_VALUE(Template::Render(content, &value), R"(1.5---1.5---1.5)",
             "Render()");

    END_SUB_TEST;
}

static int TestRawVariableTag3() {
    Value<char> value;
    const char *content;

    content = R"({raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:0})", "Render()");

    EQ_VALUE(Template::Render(R"({raw:a})", &value), R"({raw:a})", "Render()");

    content = R"({raw:0[0]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             R"({raw:0[0]})", "Render()");

    content = R"({raw:a[0]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             R"({raw:a[0]})", "Render()");

    content = R"({raw:0[a]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             R"({raw:0[a]})", "Render()");

    content = R"({raw:a[abc]})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             R"({raw:a[abc]})", "Render()");

    ////////////////

    value = JSON::Parse(R"([[[]],{"a":["x"],"b":{"a":"X"}}])");

    content = R"({raw:0})";
    EQ_VALUE(Template::Render(content, StringUtils::Count(content), &value),
             R"({raw:0})", "Render()");

    content = R"({raw:0[0]})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:0[0]})", "Render()");

    content = R"({raw:0[0][0]})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:0[0][0]})", "Render()");

    /////

    content = R"({raw:1})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:1})", "Render()");

    content = R"({raw:2})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:2})", "Render()");

    content = R"({raw:1[a]})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:1[a]})", "Render()");

    content = R"({raw:1[b]})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:1[b]})", "Render()");

    ////

    value = JSON::Parse(R"(["A", "abc", true, 456, 1.5])");

    content = R"({raw:0)";
    EQ_VALUE(Template::Render(content, &value), R"({raw:0)", "Render()");

    content = R"(raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"(raw:0})", "Render()");

    content = R"({r})";
    EQ_VALUE(Template::Render(content, &value), R"({r})", "Render()");

    content = R"({ra})";
    EQ_VALUE(Template::Render(content, &value), R"({ra})", "Render()");

    content = R"({raw})";
    EQ_VALUE(Template::Render(content, &value), R"({raw})", "Render()");

    content = R"({raw:})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:})", "Render()");

    content = R"({r:0})";
    EQ_VALUE(Template::Render(content, &value), R"({r:0})", "Render()");

    content = R"({ra:0})";
    EQ_VALUE(Template::Render(content, &value), R"({ra:0})", "Render()");
    ////

    content = R"({raw:0{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:0{raw:0})", "Render()");

    content = R"(raw:0{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"(raw:0A)", "Render()");

    content = R"(raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"(raw:0}A)", "Render()");

    content = R"({raw:0{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:0{raw:0}A)",
             "Render()");

    ////

    content = R"({raw:0{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:0{raw:0})", "Render()");

    content = R"(raw:0{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"(raw:0A)", "Render()");

    content = R"(raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"(raw:0}A)", "Render()");

    content = R"({raw:0{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({raw:0{raw:0}A)",
             "Render()");

    ////

    content = R"({{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({A)", "Render()");

    content = R"({{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({AA)", "Render()");

    content = R"({r{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({rA)", "Render()");

    content = R"({r{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({rAA)", "Render()");

    content = R"({ra{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({raA)", "Render()");

    content = R"({ra{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({raAA)", "Render()");

    content = R"({raw{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({rawA)", "Render()");

    content = R"({raw{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({rawAA)", "Render()");

    ///

    content = R"({-{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({-A)", "Render()");

    content = R"({-{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({-AA)", "Render()");

    content = R"({r-{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({r-A)", "Render()");

    content = R"({r-{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({r-AA)", "Render()");

    content = R"({ra-{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({ra-A)", "Render()");

    content = R"({ra-{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({ra-AA)", "Render()");

    content = R"({raw-{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({raw-A)", "Render()");

    content = R"({raw-{raw:0}{raw:0})";
    EQ_VALUE(Template::Render(content, &value), R"({raw-AA)", "Render()");

    //

    content = R"({raw-0})";
    EQ_VALUE(Template::Render(content, &value), R"({raw-0})", "Render()");

    content = R"({raw 0})";
    EQ_VALUE(Template::Render(content, &value), R"({raw 0})", "Render()");

    content = R"({raw:0 })";
    EQ_VALUE(Template::Render(content, &value), R"({raw:0 })", "Render()");

    content = R"({raw:0 )";
    EQ_VALUE(Template::Render(content, &value), R"({raw:0 )", "Render()");

    content = R"({raw:0)";
    EQ_VALUE(Template::Render(content, &value), R"({raw:0)", "Render()");

    END_SUB_TEST;
}

static int TestRawVariableTag4() {
    Value<char> value;

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

    EQ_VALUE(Template::Render(R"({raw:0})", &value), R"(<)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:1})", &value), R"(>)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:2})", &value), R"(&)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:3})", &value), R"(")", "Render()");
    EQ_VALUE(Template::Render(R"({raw:4})", &value), R"(')", "Render()");
    EQ_VALUE(Template::Render(R"({raw:5})", &value), R"(<>)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:6})", &value), R"(<&)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:7})", &value), R"(<&)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:8})", &value), R"(>")", "Render()");
    EQ_VALUE(Template::Render(R"({raw:9})", &value), R"("')", "Render()");
    EQ_VALUE(Template::Render(R"({raw:10})", &value), R"(<">)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:11})", &value), R"(<'>)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:12})", &value), R"(<&>)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:13})", &value), R"(&"&)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:14})", &value), R"("'")", "Render()");
    EQ_VALUE(Template::Render(R"({raw:15})", &value), R"('<')", "Render()");
    EQ_VALUE(Template::Render(R"({raw:16})", &value), R"('&')", "Render()");
    EQ_VALUE(Template::Render(R"({raw:17})", &value), R"(<>&'")", "Render()");
    EQ_VALUE(Template::Render(R"({raw:18})", &value), R"('"<>&)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:19})", &value), R"(<"&'>)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:20})", &value), R"(<<<<<)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:21})", &value), R"(>>>>>)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:22})", &value), R"(&&&&&)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:23})", &value), R"(""""")", "Render()");
    EQ_VALUE(Template::Render(R"({raw:24})", &value), R"(''''')", "Render()");
    EQ_VALUE(Template::Render(R"({raw:25})", &value), R"(A<)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:26})", &value), R"(A>)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:27})", &value), R"(A&)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:28})", &value), R"(A")", "Render()");
    EQ_VALUE(Template::Render(R"({raw:29})", &value), R"(A')", "Render()");
    EQ_VALUE(Template::Render(R"({raw:30})", &value), R"(<A)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:31})", &value), R"(>A)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:32})", &value), R"(&A)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:33})", &value), R"("A)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:34})", &value), R"('A)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:35})", &value), R"(A<A)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:36})", &value), R"(A>A)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:37})", &value), R"(A&A)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:38})", &value), R"(A"A)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:39})", &value), R"(A'A)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:40})", &value), R"(AA<AA)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:41})", &value), R"(AA>AA)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:42})", &value), R"(AA&AA)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:43})", &value), R"(AA"AA)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:44})", &value), R"(AA'AA)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:45})", &value), R"(AA<<<<AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({raw:46})", &value), R"(AA>>>>AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({raw:47})", &value), R"(AA&&&&AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({raw:48})", &value), R"(AA""""AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({raw:49})", &value), R"(AA''''AA)",
             "Render()");
    EQ_VALUE(Template::Render(R"({raw:50})", &value),
             R"(<A>B'C"D&E'F"G<H>I&G"K)", "Render()");
    EQ_VALUE(Template::Render(R"({raw:51})", &value), R"(AB"CD'EF<GH>IGK')",
             "Render()");
    EQ_VALUE(Template::Render(R"({raw:52})", &value), R"("ABC'DEF<GHI>GK<)",
             "Render()");
    EQ_VALUE(Template::Render(R"({raw:53})", &value),
             R"(A""BC<<DE>>FG''HI&&GK)", "Render()");

    END_SUB_TEST;
}

static int TestMathTag1() {
    Value<char> value;

    value["a1"] = 5;
    value["a2"] = true;
    value["a3"] = nullptr;
    value["a4"] = false;
    value["a5"] = "10";
    value["a6"] = "20";
    value["a7"] = 6;
    value["a8"] = 1;
    value["a9"] = "1";
    value["eq"] = "(8+1+{var:a8})";

    EQ_VALUE(Template::Render("{math:1+1}", &value), "2", "Render()");

    EQ_VALUE(Template::Render("{math:{var:a1}+8}", &value), R"(13)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a7}+8}", &value), R"(14)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a7}+{var:a1}}", &value), R"(11)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a5}+{var:a1}}", &value), R"(15)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a1}+{var:a5}}", &value), R"(15)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a6}+{var:a5}}", &value), R"(30)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a6}*{var:a2}}", &value), R"(20)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a6}*{var:a4}}", &value), R"(0)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a6}*{var:a7}}", &value), R"(120)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a7}+{var:a6}}", &value), R"(26)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a1}+{var:a2}}", &value), R"(6)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a8}=={var:a2}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a2}=={var:a8}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a5}!={var:a1}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a2}!={var:a4}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a2}==true}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a9}=={var:a8}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a8}=={var:a9}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:1=={var:a8}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:6-5==({var:a9})}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:6-5==({var:a8})}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:null!={var:a3}}", &value), R"(0)",
             "Render()");

    EQ_VALUE(Template::Render("{math:({var:a3})==(0)}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a1}+{var:a7}}", &value), "11",
             "Render()");

    EQ_VALUE(Template::Render(
                 "{math:{var:a1}+{var:a7}}{math:{var:a1}+{var:a7}}", &value),
             "1111", "Render()");

    EQ_VALUE(Template::Render(
                 "{math:{var:a1}+{var:a7}}*{math:{var:a1}+{var:a7}}", &value),
             "11*11", "Render()");

    EQ_VALUE(Template::Render(
                 "{math:{var:a1}+{var:a7}}##{math:{var:a1}+{var:a7}}", &value),
             "11##11", "Render()");

    EQ_VALUE(Template::Render(
                 "{math:{var:a1}+{var:a7}}&&&%%^^&&*{math:{var:a1}+{var:a7}}",
                 &value),
             "11&&&%%^^&&*11", "Render()");

    ///////////////////

    EQ_VALUE(Template::Render("{math: {var:a1}+8}", &value), R"(13)",
             "Render()");

    EQ_VALUE(Template::Render("{math:  {var:a7}+8}", &value), R"(14)",
             "Render()");

    EQ_VALUE(Template::Render("{math:     {var:a7}+{var:a1}}", &value), R"(11)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a5} +{var:a1}}", &value), R"(15)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a1}  +{var:a5}}", &value), R"(15)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a6}    +{var:a5}}", &value), R"(30)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a6}* {var:a2}}", &value), R"(20)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a6}*  {var:a4}}", &value), R"(0)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a6}*      {var:a7}}", &value),
             R"(120)", "Render()");

    EQ_VALUE(Template::Render("{math:{var:a7}+{var:a6} }", &value), R"(26)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a1}+{var:a2}  }", &value), R"(6)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a8}=={var:a2}      }", &value),
             R"(1)", "Render()");

    EQ_VALUE(Template::Render("{math:{var:a2}=={var:a8}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math: {var:a5}!={var:a1} }", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:  {var:a2}!={var:a4}  }", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:    1=={var:a9}     }", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a9} == {var:a8}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:a8}  ==  {var:a9}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:1==          {var:a8}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:6-5         ==1}", &value), R"(1)",
             "Render()");

    EQ_VALUE(
        Template::Render("{math:1          ==            {var:a8}}", &value),
        R"(1)", "Render()");

    EQ_VALUE(
        Template::Render("{math:     0     !=    ({var:a3})        }", &value),
        R"(0)", "Render()");

    EQ_VALUE(
        Template::Render("{math:{var:a3}       ==       null     }", &value),
        R"(1)", "Render()");

    EQ_VALUE(Template::Render("{math:{var:eq}}", &value), R"(10)", "Render()");

    EQ_VALUE(Template::Render("{math:10==(8+1+{var:a8})}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:eq}==9+1}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:(5*2)=={var:eq}}", &value), R"(1)",
             "Render()");

    //////////////
    value.Reset();

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

    EQ_VALUE(Template::Render("{math:{var:0}+8}", &value), R"(13)", "Render()");

    EQ_VALUE(Template::Render("{math:{var:6}+8}", &value), R"(14)", "Render()");

    EQ_VALUE(Template::Render("{math:{var:6}+{var:0}}", &value), R"(11)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:4}+{var:0}}", &value), R"(15)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:0}+{var:4}}", &value), R"(15)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:5}+{var:4}}", &value), R"(30)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:5}*{var:1}}", &value), R"(20)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:5}*{var:3}}", &value), R"(0)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:5}*{var:6}}", &value), R"(120)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:6}+{var:5}}", &value), R"(26)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:0}+{var:1}}", &value), R"(6)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:7}=={var:1}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:1}=={var:7}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:4}!={var:0}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:1}!={var:3}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:({var:1})==({var:8})}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("-{math:{var:8}=={var:7}}", &value), R"(-1)",
             "Render()");

    EQ_VALUE(Template::Render("--{math:{var:7}=={var:8}}", &value), R"(--1)",
             "Render()");

    EQ_VALUE(Template::Render("---{math:1=={var:7}}", &value), R"(---1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:1==({var:8})}-", &value), R"(1-)",
             "Render()");

    EQ_VALUE(Template::Render("{math:6-5==({var:7})}--", &value), R"(1--)",
             "Render()");

    EQ_VALUE(Template::Render("{math:0==({var:2})}---", &value), R"(1---)",
             "Render()");

    EQ_VALUE(Template::Render("-{math:{var:2}!=null}-", &value), R"(-0-)",
             "Render()");

    EQ_VALUE(Template::Render("--{math:Qente=={var:9}}--", &value), R"(--0--)",
             "Render()");

    EQ_VALUE(Template::Render("---{math:Qente !={var:9}}---", &value),
             R"(---1---)", "Render()");

    EQ_VALUE(Template::Render("{math:  Qentem   =={var:9}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:  Qentem!={var:9}}", &value), R"(0)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:9}   ==    Qente}", &value), R"(0)",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:9} !=    Qente    }", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math:  {var:9}   ==Qentem}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math: {var:9} !=Qentem}", &value), R"(0)",
             "Render()");

    /////////

    EQ_VALUE(Template::Render("{math: true == {var:1}}", &value), R"(1)",
             "Render()");

    EQ_VALUE(Template::Render("{math: {var:1} == true}", &value), R"(1)",
             "Render()");

    END_SUB_TEST;
}

static int TestMathTag2() {
    Value<char> value;

    value[0] = Array<Value<char>>();
    value[1] = HArray<Value<char>, char>();
    value[2] = 5;

    EQ_VALUE(Template::Render("{math:{var:0}+8}", &value), "{math:{var:0}+8}",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:1}+8}", &value),
             R"({math:{var:1}+8})", "Render()");

    EQ_VALUE(Template::Render("{math:{var:2}+{var:0}}", &value),
             R"({math:{var:2}+{var:0}})", "Render()");

    EQ_VALUE(Template::Render("{math:{var:2}+{var:0}}", &value),
             R"({math:{var:2}+{var:0}})", "Render()");

    EQ_VALUE(Template::Render("{math:{var:0}+{var:1}}", &value),
             "{math:{var:0}+{var:1}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:1}+{var:2}}", &value),
             "{math:{var:1}+{var:2}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:2}*{var:1}}", &value),
             "{math:{var:2}*{var:1}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:0}*{var:1}}", &value),
             "{math:{var:0}*{var:1}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:0}*{var:2}}", &value),
             "{math:{var:0}*{var:2}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:2}+{var:0}}", &value),
             "{math:{var:2}+{var:0}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:1}+{var:2}}", &value),
             "{math:{var:1}+{var:2}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:0}=={var:1}}", &value),
             "{math:{var:0}=={var:1}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:1}=={var:0}}", &value),
             "{math:{var:1}=={var:0}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:0}!={var:2}}", &value),
             "{math:{var:0}!={var:2}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:2}!={var:0}}", &value),
             "{math:{var:2}!={var:0}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:1}=={var:2}}", &value),
             "{math:{var:1}=={var:2}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:8}=={var:7}}", &value),
             "{math:{var:8}=={var:7}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:7}=={var:2}}", &value),
             "{math:{var:7}=={var:2}}", "Render()");

    EQ_VALUE(Template::Render("{math:1=={var:7}}", &value), "{math:1=={var:7}}",
             "Render()");

    EQ_VALUE(Template::Render("{math:(6-5)=={var:8}}", &value),
             "{math:(6-5)=={var:8}}", "Render()");

    EQ_VALUE(Template::Render("{math:(6-5)=={var:0}}", &value),
             "{math:(6-5)=={var:0}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:0}=={var:8}}", &value),
             "{math:{var:0}=={var:8}}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:0}=={var:0}}", &value),
             "{math:{var:0}=={var:0}}", "Render()");

    EQ_VALUE(Template::Render("{math:0=={var:1}}", &value), "{math:0=={var:1}}",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:1}!=0}", &value), "{math:{var:1}!=0}",
             "Render()");

    EQ_VALUE(Template::Render("{math:W={var:0}}", &value), "{math:W={var:0}}",
             "Render()");

    EQ_VALUE(Template::Render("{math:W=={var:0}}", &value), "{math:W=={var:0}}",
             "Render()");

    EQ_VALUE(Template::Render("{math:{var:2}==c}", &value), "{math:{var:2}==c}",
             "Render()");

    EQ_VALUE(Template::Render("{math:abc=={var:2}}", &value),
             "{math:abc=={var:2}}", "Render()");

    EQ_VALUE(Template::Render("{math:sds}", &value), "{math:sds}", "Render()");

    EQ_VALUE(Template::Render("{math:1", &value), "{math:1", "Render()");

    EQ_VALUE(Template::Render("math:1}", &value), "math:1}", "Render()");

    EQ_VALUE(Template::Render("{math:{var:2}", &value), "{math:5", "Render()");

    EQ_VALUE(Template::Render("{{math:{var:2}+5}", &value), "{10", "Render()");

    EQ_VALUE(Template::Render("{m{var:2}}", &value), "{m5}", "Render()");

    EQ_VALUE(Template::Render("{ma{var:2}}", &value), "{ma5}", "Render()");

    EQ_VALUE(Template::Render("{mat{var:2}}", &value), "{mat5}", "Render()");

    EQ_VALUE(Template::Render("{math{var:2}}", &value), "{math5}", "Render()");

    EQ_VALUE(Template::Render("{math {var:2}}", &value), "{math 5}",
             "Render()");

    EQ_VALUE(Template::Render("{math:}", &value), "{math:}", "Render()");

    EQ_VALUE(Template::Render("{math-{var:2}}{math:{var:2}+5}", &value),
             "{math-5}10", "Render()");

    EQ_VALUE(Template::Render("{math-4}{math:{var:2}+5}", &value), "{math-4}10",
             "Render()");

    EQ_VALUE(Template::Render("{math-4} {math:{var:2}+5}", &value),
             "{math-4} 10", "Render()");

    EQ_VALUE(Template::Render("{math:4", &value), "{math:4", "Render()");

    EQ_VALUE(Template::Render("{math:4    ", &value), "{math:4    ",
             "Render()");

    EQ_VALUE(Template::Render("{math:A==1+1}", &value), "{math:A==1+1}",
             "Render()");

    EQ_VALUE(Template::Render("{math:(A)!=1+1}", &value), "{math:(A)!=1+1}",
             "Render()");

    EQ_VALUE(Template::Render("{math:1+1==A}", &value), "{math:1+1==A}",
             "Render()");

    EQ_VALUE(Template::Render("{math:1+1!=(A)}", &value), "{math:1+1!=(A)}",
             "Render()");

    END_SUB_TEST;
}

static int TestInlineIfTag() {
    Value<char> value;
    const char *content;

    value += 0;
    value += 1;
    value += -1;
    value += true;
    value += false;
    value += nullptr;
    value += "ABC";
    value += Array<Value<char>>();

    content = R"({if case="0" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "F", "Render()");

    content = R"({if case="-1" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "F", "Render()");

    content = R"({if case="0.1" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    content = R"({if case="1" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    ////

    content = R"({if case="0" true="T"})";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"({if case="-1" true="T"})";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"({if case="0.1" true="T"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    content = R"({if case="1" true="T"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    ///

    content = R"({if case="0" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "F", "Render()");

    content = R"({if case="-1" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "F", "Render()");

    content = R"({if case="0.1" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"({if case="1" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    /////

    content = R"({if case="{var:0}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "F", "Render()");

    content = R"({if case="{var:2}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "F", "Render()");

    content = R"({if case="{raw:4}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "F", "Render()");

    content = R"({if case="{var:5}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "F", "Render()");

    content = R"({if case="{var:6}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"({if case="fas" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"({if case="{var:7}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"({if case="{var:20}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    ////
    content = R"({if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    content = R"({if case="{var:3}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    content = R"({if case="{var:6}==ABC" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    /////////////////

    content = R"({if case=" {var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    content = R"({if case="{var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    content = R"({if case=" {var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    content = R"({if case="  {var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    content = R"({if case="{var:1}  " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    content = R"({if case="  {var:1}  " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    content = R"({if case="      {var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    content = R"({if case="{var:1}          " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    content = R"({if case="    {var:1}        " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");
    ////

    content = R"(-{if case=" {var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "-T", "Render()");

    content = R"({if case=" {var:1} " true="T" false="F"}-)";
    EQ_VALUE(Template::Render(content, &value), "T-", "Render()");

    content = R"(-{if case=" {var:1} " true="T" false="F"}-)";
    EQ_VALUE(Template::Render(content, &value), "-T-", "Render()");

    content = R"(--{if case=" {var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "--T", "Render()");

    content = R"({if case=" {var:1} " true="T" false="F"}--)";
    EQ_VALUE(Template::Render(content, &value), "T--", "Render()");

    content = R"(--{if case=" {var:1} " true="T" false="F"}--)";
    EQ_VALUE(Template::Render(content, &value), "--T--", "Render()");

    content = R"(---{if case=" {var:1} " true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "---T", "Render()");

    content = R"({if case=" {var:1} " true="T" false="F"}---)";
    EQ_VALUE(Template::Render(content, &value), "T---", "Render()");

    content = R"(---{if case=" {var:1} " true="T" false="F"}---)";
    EQ_VALUE(Template::Render(content, &value), "---T---", "Render()");

    content =
        R"({if case="1" true="T"}{if case="0" false="F"}{if case="1" true="T"})";
    EQ_VALUE(Template::Render(content, &value), "TFT", "Render()");

    ///////

    content =
        R"({if case="{var:7}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "T", "Render()");

    content = R"({if case="01" true="{var:3}" false="{var:4}"}--)";
    EQ_VALUE(Template::Render(content, &value), "--", "Render()");

    content = R"({if case="" true="c" false="d"})";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"({i)";
    EQ_VALUE(Template::Render(content, &value), "{i", "Render()");

    content = R"({if)";
    EQ_VALUE(Template::Render(content, &value), "{if", "Render()");

    content = R"({if})";
    EQ_VALUE(Template::Render(content, &value), "{if}", "Render()");

    content = R"({{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "{T", "Render()");

    content = R"({i{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "{iT", "Render()");

    content = R"({if{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), "{ifT", "Render()");

    content = R"({if case="{var:1}"                    put="F"})";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content =
        R"({if{if case="1" true="T" false="F"}}{if case="1" true="T" false="F"})";

    EQ_VALUE(Template::Render(content, &value), R"(TT)", "Render()");

    content =
        R"({if{if case="{raw:1}" true="T" false="F"}{if case="{var:1}" true="T" false="F"})";
    EQ_VALUE(Template::Render(content, &value), R"({ifTT)", "Render()");

    /////
    content = R"({if case="0" true="{var:3}" false="{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), "false", "Render()");

    content = R"({if case="1" true="{var:3}" false="{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), "true", "Render()");

    content = R"({if case="0" true="{raw:3}{raw:3}" false="{var:4}{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), "falsefalse", "Render()");

    content =
        R"({if case="1000" true="{var:3}{var:3}" false="{var:4}{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), "truetrue", "Render()");

    content =
        R"({if case="0" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), "false---false", "Render()");

    content =
        R"({if case="1" true="{var:3}---{var:3}" false="{var:4}---{var:4}"})";
    EQ_VALUE(Template::Render(content, &value), "true---true", "Render()");

    content = R"({if case="0" true="{var:10}" false="{var:20}"})";
    EQ_VALUE(Template::Render(content, &value), "{var:20}", "Render()");

    content = R"({if case="1" true="{var:10}" false="{var:20}"})";
    EQ_VALUE(Template::Render(content, &value), "{var:10}", "Render()");

    content = R"({if case="1" true="1" false="0")";
    EQ_VALUE(Template::Render(content, &value),
             R"({if case="1" true="1" false="0")", "Render()");

    content = R"({if case="1" true="1" false="0")";
    EQ_VALUE(Template::Render(content, &value),
             R"({if case="1" true="1" false="0")", "Render()");

    ///////
    Value<char> value2;

    value2 += "&";
    value2 += "\"";
    value2 += 10;
    value2 += 5;
    value2 += 15;

    content = R"({if case="1" true="{var:0}" false="{var:1}"})";
#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
    EQ_VALUE(Template::Render(content, &value2), R"(&amp;)", "Render()");
#else
    EQ_VALUE(Template::Render(content, &value2), R"(&)", "Render()");
#endif

    content = R"({if case="1" true="{raw:0}" false="{raw:1}"})";
    EQ_VALUE(Template::Render(content, &value2), R"(&)", "Render()");

    content = R"({if case="0" true="{var:0}" false="{var:1}"})";
#if defined(QENTEM_AUTOESCAPE_HTML) && (QENTEM_AUTOESCAPE_HTML == 1)
    EQ_VALUE(Template::Render(content, &value2), R"(&quot;)", "Render()");
#else
    EQ_VALUE(Template::Render(content, &value2), R"(")", "Render()");
#endif

    content = R"({if case="0" true="{raw:0}" false="{raw:1}"})";
    EQ_VALUE(Template::Render(content, &value2), R"(")", "Render()");

    content =
        R"({if case="{var:2}+{var:3} == {var:4}" true="{math:{var:3}+{var:4}}" false="{math: {var:2}+{var:4}}"})";
    EQ_VALUE(Template::Render(content, &value2), R"(20)", "Render()");

    content =
        R"({if case=" {var:4}-{var:3} != {var:2} " true="{math: {var:3}+{var:4} } " false=" {math: {var:2}+{var:4} } "})";
    EQ_VALUE(Template::Render(content, &value2), R"( 25 )", "Render()");

    END_SUB_TEST;
}

static int TestLoopTag1() {
    Value<char> value;
    const char *content;

    value += 100;
    value += -50;
    value += "Qentem";
    value += true;
    value += false;
    value += nullptr;
    value += 3;

    content = R"(<loop repeat="10">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "AAAAAAAAAA", "Render()");

    content = R"(<loop             repeat="1">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "A", "Render()");

    content = R"(<loop<loop repeat="1">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "<loopA", "Render()");

    content = R"(<loop repeat="3"         >ABC</loop>)";
    EQ_VALUE(Template::Render(content, &value), "ABCABCABC", "Render()");

    content = R"(-<loop repeat="3">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "-AAA", "Render()");

    content = R"(<loop repeat="3">A</loop>-)";
    EQ_VALUE(Template::Render(content, &value), "AAA-", "Render()");

    content = R"(-<loop repeat="3">A</loop>-)";
    EQ_VALUE(Template::Render(content, &value), "-AAA-", "Render()");

    content = R"(--<loop repeat="3">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "--AAA", "Render()");

    content = R"(<loop repeat="3">A</loop>--)";
    EQ_VALUE(Template::Render(content, &value), "AAA--", "Render()");

    content = R"(--<loop repeat="3">A</loop>--)";
    EQ_VALUE(Template::Render(content, &value), "--AAA--", "Render()");

    content = R"(---<loop repeat="3">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "---AAA", "Render()");

    content = R"(<loop repeat="3">A</loop>---)";
    EQ_VALUE(Template::Render(content, &value), "AAA---", "Render()");

    content = R"(---<loop repeat="3">A</loop>---)";
    EQ_VALUE(Template::Render(content, &value), "---AAA---", "Render()");

    content = R"(<loop repeat="2">A</loop><loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), "AABBB", "Render()");

    content = R"(<loop repeat="2">A</loop>-<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), "AA-BBB", "Render()");

    content = R"(<loop repeat="2">A</loop>--<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), "AA--BBB", "Render()");

    content = R"(<loop repeat="2">A</loop>---<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), "AA---BBB", "Render()");

    content =
        R"(<loop repeat="2">A</loop>            <loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), "AA            BBB",
             "Render()");

    ////

    content =
        R"(<loop repeat="4">CC</loop><loop repeat="2">A</loop><loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), "CCCCCCCCAABBB", "Render()");

    content =
        R"(<loop repeat="4">CC</loop>-<loop repeat="2">A</loop>-<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), "CCCCCCCC-AA-BBB", "Render()");

    content =
        R"(<loop repeat="4">CC</loop>--<loop repeat="2">A</loop>--<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), "CCCCCCCC--AA--BBB",
             "Render()");

    content =
        R"(<loop repeat="4">CC</loop>---<loop repeat="2">A</loop>---<loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value), "CCCCCCCC---AA---BBB",
             "Render()");

    content =
        R"(<loop repeat="4">CC</loop>     <loop repeat="2">A</loop>            <loop repeat="3">B</loop>)";
    EQ_VALUE(Template::Render(content, &value),
             "CCCCCCCC     AA            BBB", "Render()");

    ////////////////

    content = R"(<loop repeat="6"value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value),
             "100, -50, Qentem, true, false, null, ", "Render()");

    content =
        R"(<loop repeat="6" value="loop1-value">loop1-value, loop1-value </loop>)";
    EQ_VALUE(
        Template::Render(content, &value),
        "100, 100 -50, -50 Qentem, Qentem true, true false, false null, null ",
        "Render()");

    content =
        R"(<loop index="2" repeat="4" value="loop1-value">loop1-value{if
        case="loop1-value != null" true=", "}</loop>)";
    EQ_VALUE(Template::Render(content, &value), "Qentem, true, false, null",
             "Render()");

    ////////////////

    value.Reset();
    value += 0;
    value += 1;
    value += 2;

    content =
        R"(<loop repeat="3" value="loop1-value"><loop
        repeat="2" value="loop2-value">(loop1-value: loop2-value) </loop></loop>)";
    EQ_VALUE(Template::Render(content, &value),
             "(0: 0) (0: 1) (1: 0) (1: 1) (2: 0) (2: 1) ", "Render()");

    content =
        R"(<loop repeat="2" value="loop1-value"><loop repeat="2" value="loop2-value"><loop
        repeat="2" value="loop3-value">(loop1-value: loop2-value: loop3-value) </loop></loop></loop>)";
    EQ_VALUE(
        Template::Render(content, &value),
        "(0: 0: 0) (0: 0: 1) (0: 1: 0) (0: 1: 1) (1: 0: 0) (1: 0: 1) (1: 1: 0) (1: 1: 1) ",
        "Render()");

    END_SUB_TEST;
}

static int TestLoopTag2() {
    Value<char> value1;
    Value<char> value2;
    Value<char> value3;
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
    EQ_VALUE(Template::Render(content, &value1),
             "100, -50, A, true, false, null, ", "Render()");

    content = R"(<loop value="loop1-value" index="3">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value2), "true, false, null, ",
             "Render()");

    content = R"(<loop value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value2),
             "4, 1.5, ABC, true, false, null, ", "Render()");

    content = R"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             "100, -50, A, true, false, null, ", "Render()");

    value3["arr1"] = value2;

    content = R"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             "4, 1.5, ABC, true, false, null, ", "Render()");

    //////////////////////
    value3["arr1"] = value1;

    content = R"(<loop value="loop1-value" >loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value1),
             "100, -50, A, true, false, null, ", "Render()");

    content = R"(<loop value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value2),
             "4, 1.5, ABC, true, false, null, ", "Render()");

    content = R"(<loop value="loop1-value"set="arr1">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             "100, -50, A, true, false, null, ", "Render()");

    value3["arr1"] = value2;

    content = R"(<loop set="arr1" value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             "4, 1.5, ABC, true, false, null, ", "Render()");

    //////////////////////
    value3.Reset();
    value3["arr1"]["arr2"]["arr3"] = value1;

    content =
        R"(<loop set="arr1[arr2][arr3]" value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             "100, -50, A, true, false, null, ", "Render()");

    value3.Reset();
    value3[0][0][0] = value2;

    content = R"(<loop set="0[0][0]"value="loop1-value">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             "4, 1.5, ABC, true, false, null, ", "Render()");

    value3.Reset();
    value3["k1"][0]["k3"] = value1;

    content =
        R"(<loop value="loop1-value" set="k1[0][k3]">loop1-value, </loop>)";
    EQ_VALUE(Template::Render(content, &value3),
             "100, -50, A, true, false, null, ", "Render()");

    value3.Reset();
    value3[0]["k2"][0] = value2;

    content =
        R"(<loop set="0[k2][0]"key="loop1-key"value="loop1-value">loop1-value, loop1-value, </loop>)";
    EQ_VALUE(
        Template::Render(content, &value3),
        R"(4, 4, 1.5, 1.5, ABC, ABC, true, true, false, false, null, null, )",
        "Render()");

    value3 = JSON::Parse(R"({"group":[[10],[20],[30]]})");

    content =
        R"(<loop set="group" value="_Val1"><loop set="_Val1" value="_Val2">_Val2</loop></loop>)";
    EQ_VALUE(Template::Render(content, &value3), "102030", "Render()");

    value3 = JSON::Parse(R"({"group":[1,2,3,4]})");

    content = R"(<loop set="group" value="_Val" repeat="1">_Val</loop>)";
    EQ_VALUE(Template::Render(content, &value3), "1", "Render()");

    content = R"(<loop set="group" value="_Val" index="3">_Val</loop>)";
    EQ_VALUE(Template::Render(content, &value3), "4", "Render()");

    content =
        R"(<loop index="2" repeat="1" set="group" value="_Val">_Val</loop>)";
    EQ_VALUE(Template::Render(content, &value3), "3", "Render()");

    value3  = JSON::Parse(R"({"numbers":[1,2,3,4,5,6,7,8]})");
    content = R"(A<loop set="numbers" value="t">t</loop>B)";
    EQ_VALUE(Template::Render(content, &value3), "A12345678B", "Render()");

    content = R"(<loop set="numbers" value="t">t[0]</loop>)";
    EQ_VALUE(Template::Render(content, &value3), "", "Render()");

    END_SUB_TEST;
}

static int TestLoopTag3() {
    Value<char> value;
    const char *content;

    content = R"(<loop></loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"(<loop>abcd</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"(<l</loop>)";
    EQ_VALUE(Template::Render(content, &value), "<l</loop>", "Render()");

    content = R"(<lo</loop>)";
    EQ_VALUE(Template::Render(content, &value), "<lo</loop>", "Render()");

    content = R"(<loo</loop>)";
    EQ_VALUE(Template::Render(content, &value), "<loo</loop>", "Render()");

    content = R"(<loop></loop><loop repeat="2">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "AA", "Render()");

    content = R"(<loop key="a">a</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"(<loop value="a">a</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"(<loop set="ss" value="a">a</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"(<loop set="" value="a">a</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"(<loop repeat="2" A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"(<loop repeat="OOO">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"(<loop repeat="{var:10}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    value["in"] = Array<Value<char>>();

    content = R"(<loop repeat="{var:in}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"(<loop repeat="10" index="{var:in}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"(<loop repeat="10" index="{var:in">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"(<loop repeat="10" index="{var:100}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"(<loop repeat="10" index="O">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content = R"(<loop value="v">v</loop>)";
    EQ_VALUE(Template::Render(content, &value), "in", "Render()");

    value.Reset();
    value["in"] = 2;

    content = R"(<loop repeat="{var:in}">A</loop>)";
    EQ_VALUE(Template::Render(content, &value), "AA", "Render()");

    /////
    value.Reset();
    value["k1"] = 10;
    value["k2"] = 20;
    value["k3"] = 30;

    value["k2"].Reset();

    content = R"(<loop value="v">v</loop>)";
    EQ_VALUE(Template::Render(content, &value), "1030", "Render()");

    value.RemoveIndex(1);

    content = R"(<loop value="v">v)";
    EQ_VALUE(Template::Render(content, &value), R"(<loop value="v">v)",
             "Render()");

    content = R"(<loop value="v">v     )";
    EQ_VALUE(Template::Render(content, &value), R"(<loop value="v">v     )",
             "Render()");

    value.Reset();
    value += 10;
    value += 20;
    value += 30;

    value.RemoveIndex(1);

    content = R"(<loop value="v">v</loop>)";
    EQ_VALUE(Template::Render(content, &value), "1030", "Render()");

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
    EQ_VALUE(Template::Render(content, &value), "value1value2value3 value4",
             "Render()");

    content =
        R"(<loop set="array" value="item"> item[0] item[1] item[2] item[3] </loop>)";
    EQ_VALUE(Template::Render(content, &value),
             " value10 value20 value30 value40 ", "Render()");

    content =
        R"(<loop set="object" value="item">item[var11]item[var22]item[var33] item[var44]</loop>)";
    EQ_VALUE(Template::Render(content, &value), " ", "Render()");

    content =
        R"(<loop set="array" value="item">item[var11]item[var22]item[var33] item[var44]</loop>)";
    EQ_VALUE(Template::Render(content, &value), " ", "Render()");

    content = R"(<loop repeat="1"><l</loop>)";
    EQ_VALUE(Template::Render(content, &value), "<l", "Render()");

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
    EQ_VALUE(Template::Render(content, &value), "some_valsome_valsome_val",
             "Render()");

    END_SUB_TEST;
}

static int TestLoopTag4() {
    constexpr unsigned int size_4 = (8 * 4);

    StringStream<char> content;
    StringStream<char> output;
    String<char>       str;
    Value<char>        value;

    for (unsigned int i = 0; i < size_4; i++) {
        value += i;
    }

    content += R"(<loop repeat="1">)";
    for (unsigned int i = 0; i < size_4; i++) {
        content += "{var:";
        str = Digit<char>::NumberToString(i);
        content += str;
        content += "}";

        output += str;
    }
    content += R"(</loop>)";

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        "Render()");

    //////////////////////

    content.Clear();
    output.Clear();

    content += R"(<loop value="loop1-value">A loop1-value B</loop>)";
    for (unsigned int i = 0; i < size_4; i++) {
        output += "A ";
        Digit<char>::NumberToStringStream(output, i);
        output += " B";
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        "Render()");

    END_SUB_TEST;
}

static int TestLoopTag5() {
    Value<char> value;
    const char *content;

    value = JSON::Parse(R"(
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
        R"(<loop value="val1_" group="year111" sort="descend"><loop set="val1_" value="val2_"><loop set="val2_" value="val3_">val3_</loop></loop></loop>)";

    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content =
        R"(<loop value="val1_" group="year" sort="descend"><loop set="val1_" value="val2_"><loop set="val2_" value="val3_">val3_</loop></loop></loop>)";

    EQ_VALUE(Template::Render(content, &value),
             "q11400q11450q11450q11100q11125q21200q22300q21200q22300q22300",
             "Render()");

    content =
        R"(<loop value="val1_" group="year" sort="descend"><loop set="val1_" value="val2_" group="quarter" sort="ascend"><loop set="val2_" value="val3_"><loop set="val3_" value="val4_">val4_</loop></loop></loop></loop>)";

    EQ_VALUE(Template::Render(content, &value),
             "1400145014501100112512002300120023002300", "Render()");

    content =
        R"(<loop value="val1_" group="year" sort="descend">-- val1_-<loop set="val1_" value="val2_" group="quarter" sort="ascend">val2_-<loop set="val2_" value="val3_" group="week" sort="ascend">val2_:<loop set="val3_" value="val4_"><loop set="val4_" value="val5_"> val5_</loop></loop></loop></loop></loop>)";

    EQ_VALUE(
        Template::Render(content, &value),
        "-- 2020-q1-1: 400 450 450-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2018-q2-1: 2002: 300-- 2017-q2-2: 300",
        "Render()");

    content =
        R"(<loop value="val1_" group="year">-- val1_-<loop set="val1_" value="val2_" group="quarter">val2_-<loop set="val2_" value="val3_" group="week">val2_:<loop set="val3_" value="val4_"><loop set="val4_" value="val5_"> val5_</loop></loop></loop></loop></loop>)";

    EQ_VALUE(
        Template::Render(content, &value),
        "-- 2019-q1-1: 100 125q2-1: 2002: 300-- 2017-q2-2: 300-- 2020-q1-1: 400 450 450-- 2018-q2-1: 2002: 300",
        "Render()");

    ////////////

    value.Reset();

    value += 4;
    value += 1;
    value += 3;
    value += 5;
    value += 2;
    value += 7;
    value += 6;

    content = R"(<loop value="val1_" sort="a">val1_</loop>)";

    EQ_VALUE(Template::Render(content, &value), "1234567", "Render()");

    content = R"(<loop value="val1_" sort="d">val1_</loop>)";

    EQ_VALUE(Template::Render(content, &value), "7654321", "Render()");

    END_SUB_TEST;
}

static int TestIfTag1() {
    Value<char> value;
    const char *content;

    value["name"] = "Qen";
    value["t"]    = true;
    value["f"]    = false;
    value["n"]    = nullptr;
    value["1"]    = 1;
    value["one"]  = "1";
    value["zero"] = 0;

    content = R"(<if case="1>0">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), "Qen", "Render()");

    content = R"(#<if case="{var:one}">{var:name}</if>#)";
    EQ_VALUE(Template::Render(content, &value), "#Qen#", "Render()");

    content = R"(##<if case="{var:zero}">{var:name}</if>##)";
    EQ_VALUE(Template::Render(content, &value), "####", "Render()");

    content = R"(<if case="{var:1}">{var:name}1<else />{var:name}2</if>)";
    EQ_VALUE(Template::Render(content, &value), "Qen1", "Render()");

    content = R"(<if case="{var:f}">{var:name}1<else />{var:name}2</if>)";
    EQ_VALUE(Template::Render(content, &value), "Qen2", "Render()");

    content = R"(#<if case="{var:t}">G<if case="1">oo</if>d</if>!#)";
    EQ_VALUE(Template::Render(content, &value), "#Good!#", "Render()");

    content =
        R"(###<if case="1">G<if case="1">o</if>o<if case="1">d!</if></if>###)";
    EQ_VALUE(Template::Render(content, &value), "###Good!###", "Render()");

    content =
        R"(Be <if case="1">G<if case="1">oo<if case="1">d</if></if></if>!)";
    EQ_VALUE(Template::Render(content, &value), "Be Good!", "Render()");

    content = R"(<if case="1">Good!<elseif case="0" />Bad!</if>)";
    EQ_VALUE(Template::Render(content, &value), "Good!", "Render()");

    content = R"(<if case="{var:n}">Bad!<elseif case="1" />Good!</if>#)";
    EQ_VALUE(Template::Render(content, &value), "Good!#", "Render()");

    content = R"(<if case="{var:f}">Bad!<elseif case="0" />Very Bad!</if>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content =
        R"(#<if case="0">Bad!<elseif case="0" />Very Bad!<else />Very Good!</if>)";
    EQ_VALUE(Template::Render(content, &value), "#Very Good!", "Render()");

    content = R"(<if case="1">a<else />b</if>)";
    EQ_VALUE(Template::Render(content, &value), "a", "Render()");

    content = R"(<if case="0">a<else />b</if>)";
    EQ_VALUE(Template::Render(content, &value), "b", "Render()");

    content = R"(<if case="0">a<else /><if case="1">b</if></if>)";
    EQ_VALUE(Template::Render(content, &value), "b", "Render()");

    content = R"(<if case="1">a<else /><if case="1">b</if></if>)";
    EQ_VALUE(Template::Render(content, &value), "a", "Render()");

    content = R"(<if case="0">a<if case="1">b</if>c</if>Empty)";
    EQ_VALUE(Template::Render(content, &value), "Empty", "Render()");

    content = R"(<if case="1">a<else /><if case="1">b</if>c</if>===========)";
    EQ_VALUE(Template::Render(content, &value), "a===========", "Render()");

    content = R"(===========<if case="1">a<if case="1">b</if><else />c</if>)";
    EQ_VALUE(Template::Render(content, &value), "===========ab", "Render()");

    content =
        R"(<if case="1">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    EQ_VALUE(Template::Render(content, &value), "a", "Render()");

    content =
        R"(<if case="0">a<elseif case="1" />b<elseif case="1" />c<else />d</if>)";
    EQ_VALUE(Template::Render(content, &value), "b", "Render()");

    content =
        R"(===========<if case="0">a<elseif case="0" />b<elseif case="1" />c<else />d</if>===========)";
    EQ_VALUE(Template::Render(content, &value),
             "===========c===========", "Render()");

    content =
        R"(<if case="0">a<elseif case="0" />b<elseif case="0" />c<else />d</if>)";
    EQ_VALUE(Template::Render(content, &value), "d", "Render()");

    content = R"(<if case="1">a<if case="0">b<elseif case="0"/>c</if></if>)";
    EQ_VALUE(Template::Render(content, &value), "a", "Render()");

    content = R"(<if case="1">a<if case="1">b<elseif case="0"/>c</if></if>)";
    EQ_VALUE(Template::Render(content, &value), "ab", "Render()");

    content =
        R"(<if case="1"><if case="1">b<elseif case="c"/>c</if>a<else />c</if>)";
    EQ_VALUE(Template::Render(content, &value), "ba", "Render()");

    content =
        R"(<if case="0">a<else />c<if case="1">b<elseif case="0"/>c</if></if>)";
    EQ_VALUE(Template::Render(content, &value), "cb", "Render()");

    content =
        R"(<if_case="1"><if case="0">Bad1!<elseif case="0" />Bad2!</if>a</if>)";
    EQ_VALUE(Template::Render(content, &value), "a", "Render()");

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
    EQ_VALUE(String<char>::Trim(Template::Render(content, &value).GetString()),
             "a", "Render()");

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
    EQ_VALUE(String<char>::Trim(Template::Render(content, &value).GetString()),
             "b", "Render()");

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
    EQ_VALUE(String<char>::Trim(Template::Render(content, &value).GetString()),
             "c", "Render()");

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
    EQ_VALUE(String<char>::Trim(Template::Render(content, &value).GetString()),
             "d", "Render()");

    END_SUB_TEST;
}

static int TestIfTag2() {
    Value<char> value;
    const char *content;

    value["name"] = "Qentem";

    content = R"(<if case="1">{var:name})";
    EQ_VALUE(Template::Render(content, &value), R"(<if case="1">Qentem)",
             "Render()");

    content = R"(<if<if case="1">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), R"(<ifQentem)", "Render()");

    content = R"(<if case="1"><if case="1">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), R"(<if case="1">Qentem)",
             "Render()");

    content = R"(<if case="1"><if case="1"><if case="1">{var:name}</if></if>)";
    EQ_VALUE(Template::Render(content, &value), R"(<if case="1">Qentem)",
             "Render()");

    content = R"(<if case="ABC">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), R"()", "Render()");

    content = R"(<if>{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), R"()", "Render()");

    content = R"(<if case="0"><elseif />{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), R"()", "Render()");

    content = R"(<iw case="0">{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), R"(<iw case="0">Qentem</if>)",
             "Render()");

    content = R"(<if case="0"{var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), R"()", "Render()");

    content = R"(<if case="0"><else {var:name}</if>)";
    EQ_VALUE(Template::Render(content, &value), R"()", "Render()");

    END_SUB_TEST;
}

static int TestRender1() {
    constexpr unsigned int size_4 = (8 * 4);

    StringStream<char> content;
    StringStream<char> output;
    String<char>       str;
    Value<char>        value;

    for (unsigned int i = 0; i < size_4; i++) {
        value += i;
    }

    for (unsigned int i = 0; i < size_4; i++) {
        content += "{var:";
        str = Digit<char>::NumberToString(i);
        content += str;
        output += str;
        content += "}";
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        "Render()");

    content.Clear();
    output.Clear();

    unsigned int size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += "{var:";
            str = Digit<char>::NumberToString(i);
            content += str;
            content += "}";

            output += str;
        } else {
            size += 8;

            content += "{math: 1 + ";
            str = Digit<char>::NumberToString(i);
            content += str;
            content += "}";

            Digit<char>::NumberToStringStream(output, 1U + i);
        }
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        "Render()");

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += "{var:";
            str = Digit<char>::NumberToString(i);
            content += str;
            content += "}";

            output += str;
        } else {
            size += 8;

            content += R"({if case="{var:)";
            Digit<char>::NumberToStringStream(content, i);
            content += R"(}<)";
            Digit<char>::NumberToStringStream(content, 1U + i);
            content += R"(" true="yes"})";

            output += "yes";
        }
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        "Render()");

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += "{var:";
            str = Digit<char>::NumberToString(i);
            content += str;
            content += "}";

            output += str;
        } else {
            size += 8;
            content += R"(<loop repeat="1">A</loop>)";
            output += "A";
        }
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        "Render()");

    content.Clear();
    output.Clear();

    size = 8;
    for (unsigned int i = 0, x = 1; i < size_4; i++, x++) {
        if (x != size) {
            content += "{var:";
            str = Digit<char>::NumberToString(i);
            content += str;
            content += "}";

            output += str;
        } else {
            size += 8;
            content += R"(<if case="1">A</if>)";
            output += "A";
        }
    }

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        "Render()");

    content.Clear();
    output.Clear();

    constexpr unsigned int size_2_1 = (8 * 2) - 1;

    for (unsigned int i = 0; i < size_2_1; i++) {
        value += i;
    }

    content += R"(<loop repeat="1">)";
    for (unsigned int i = 0; i < size_2_1; i++) {
        content += "{var:";
        str = Digit<char>::NumberToString(i);
        content += str;
        output += str;
        content += "}";
    }
    content += R"(</loop>)";

    EQ_TRUE(
        (Template::Render(content.First(), content.Length(), &value) == output),
        "Render()");

    END_SUB_TEST;
}

static int TestRender2() {
    Value<char> value;
    const char *content;

    value[0] = 0;
    value[1] = 1;
    value[2] = 2;
    value[3] = 5;
    value[4] = 10;

    content = R"(<loop set="numbers" value="val_">val_</loop>)";
    EQ_VALUE(Template::Render(content, &value), "", "Render()");

    content =
        R"(<loop value="this_number"><if case="(this_number % 2) == 1">this_number</if></loop>)";
    EQ_VALUE(Template::Render(content, &value), "15", "Render()");

    content =
        R"(<loop value="loop1_val">{if case="loop1_val < 5", true="loop1_val"}</loop>)";
    EQ_VALUE(Template::Render(content, &value), R"(012)", "Render()");

    content =
        R"(<loop value="loop1_val">{if case="loop1_val < 5", true="{var:4}"}</loop>)";
    EQ_VALUE(Template::Render(content, &value), R"(101010)", "Render()");

    content = R"(<loop value="loop1_val">loop1_val[]</loop>)";
    EQ_VALUE(Template::Render(content, &value), R"()", "Render()");

    content = R"(<loop value="loop1_val">loop1_val[0 </loop>)";
    EQ_VALUE(
        Template::Render(content, &value),
        R"({var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 {var:~loop1_val[0 )",
        "Render()");

    value.Reset();
    value[0][0] += 1;
    value[0][0] += 2;
    value[0][0] += 3;

    content = R"(<loop value="loop1_val">loop1_val[0][2]</loop>)";
    EQ_VALUE(Template::Render(content, &value), R"(3)", "Render()");

    END_SUB_TEST;
}

static int RunTemplateTests() {
    STARTING_TEST("Template.hpp");

    START_TEST("Variable Tag Test 1", TestVariableTag1);
    START_TEST("Variable Tag Test 2", TestVariableTag2);
    START_TEST("Variable Tag Test 3", TestVariableTag3);
    START_TEST("Variable Tag Test 4", TestVariableTag4);

    START_TEST("Raw Variable Tag Test 1", TestRawVariableTag1);
    START_TEST("Raw Variable Tag Test 2", TestRawVariableTag2);
    START_TEST("Raw Variable Tag Test 3", TestRawVariableTag3);
    START_TEST("Raw Variable Tag Test 4", TestRawVariableTag4);

    START_TEST("Math Tag Test 1", TestMathTag1);
    START_TEST("Math Tag Test 2", TestMathTag2);

    START_TEST("Inline if Tag Test", TestInlineIfTag);

    START_TEST("Loop Tag Test 1", TestLoopTag1);
    START_TEST("Loop Tag Test 2", TestLoopTag2);
    START_TEST("Loop Tag Test 3", TestLoopTag3);
    START_TEST("Loop Tag Test 4", TestLoopTag4);
    START_TEST("Loop Tag Test 4", TestLoopTag5);

    START_TEST("If Tag Test 1", TestIfTag1);
    START_TEST("If Tag Test 2", TestIfTag2);

    START_TEST("Render Test 1", TestRender1);
    START_TEST("Render Test 2", TestRender2);

    END_TEST("Template.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
