/*
 * Copyright (c) 2026 Hani Ammar
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

#ifndef QENTEM_JSON_TESTS_H
#define QENTEM_JSON_TESTS_H

#include "Qentem/QTest.hpp"
#include "Qentem/JSON.hpp"

namespace Qentem {
namespace Test {

static void TestParse1(QTest &test) {
    Value<char>        value;
    StringStream<char> stream;
    const char        *content;

    ///////////
    content = R"([])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([])", __LINE__);
    stream.Clear();

    content = R"([ ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([])", __LINE__);
    stream.Clear();

    content = R"( [])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([])", __LINE__);
    stream.Clear();

    content = R"( [ ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([])", __LINE__);
    stream.Clear();

    content = R"([]    )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([])", __LINE__);
    stream.Clear();

    content = R"(     [])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([])", __LINE__);
    stream.Clear();

    content = R"(     []    )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([])", __LINE__);
    stream.Clear();

    content = R"(    [])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([])", __LINE__);
    stream.Clear();

    content = R"(    [ ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([])", __LINE__);
    stream.Clear();

    content = R"([    ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([])", __LINE__);
    stream.Clear();

    content = R"( [true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([true])", __LINE__);
    stream.Clear();

    content = R"( [false ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([false])", __LINE__);
    stream.Clear();

    content = R"([    null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([null])", __LINE__);
    stream.Clear();

    content = R"([ 0 ]     )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([0])", __LINE__);
    stream.Clear();

    content = R"([      "a"   ]          )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["a"])", __LINE__);
    stream.Clear();

    content = R"(        ["ABC"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["ABC"])", __LINE__);
    stream.Clear();

    content = R"([[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[]])", __LINE__);
    stream.Clear();

    content = R"([ [ ]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[]])", __LINE__);
    stream.Clear();

    content = R"([[          ]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[]])", __LINE__);
    stream.Clear();

    content = R"([         [          ]           ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[]])", __LINE__);
    stream.Clear();

    content = R"([{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{}])", __LINE__);
    stream.Clear();

    content = R"([ {}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{}])", __LINE__);
    stream.Clear();

    content = R"([{        }])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{}])", __LINE__);
    stream.Clear();

    content = R"([ { } ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{}])", __LINE__);
    stream.Clear();

    content = R"([ true,true ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([true,true])", __LINE__);
    stream.Clear();

    content = R"([false, true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([false,true])", __LINE__);
    stream.Clear();

    content = R"([null ,true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([null,true])", __LINE__);
    stream.Clear();

    content = R"([-3 , true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([-3,true])", __LINE__);
    stream.Clear();

    content = R"(["a"       ,    true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["a",true])", __LINE__);
    stream.Clear();

    content = R"([   "ABC",true       ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["ABC",true])", __LINE__);
    stream.Clear();

    content = R"([       [] ,  true      ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[],true])", __LINE__);
    stream.Clear();

    content = R"([ {} , true ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{},true])", __LINE__);
    stream.Clear();

    content = R"([  true ,false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([true,false])", __LINE__);
    stream.Clear();

    content = R"([false, false ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([false,false])", __LINE__);
    stream.Clear();

    content = R"([ null ,false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([null,false])", __LINE__);
    stream.Clear();

    content = R"([500,false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([500,false])", __LINE__);
    stream.Clear();

    content = R"(["a",false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["a",false])", __LINE__);
    stream.Clear();

    content = R"([ "ABC" , false ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["ABC",false])", __LINE__);
    stream.Clear();

    content = R"([[]                      ,false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[],false])", __LINE__);
    stream.Clear();

    content = R"([{},                     false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{},false])", __LINE__);
    stream.Clear();

    content = R"([true,null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([true,null])", __LINE__);
    stream.Clear();

    content = R"([false,null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([false,null])", __LINE__);
    stream.Clear();

    content = R"([null,null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([null,null])", __LINE__);
}

static void TestParse2(QTest &test) {
    Value<char>        value;
    StringStream<char> stream;
    const char        *content;

    content = R"([456.5,null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([456.5,null])", __LINE__);
    stream.Clear();

    content = R"(["a",null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["a",null])", __LINE__);
    stream.Clear();

    content = R"(["ABC",null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["ABC",null])", __LINE__);
    stream.Clear();

    content = R"([[],null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[],null])", __LINE__);
    stream.Clear();

    content = R"([{},null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{},null])", __LINE__);
    stream.Clear();

    content = R"([true,"A"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([true,"A"])", __LINE__);
    stream.Clear();

    content = R"([false,"AB"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([false,"AB"])", __LINE__);
    stream.Clear();

    content = R"([null,"ABC"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([null,"ABC"])", __LINE__);
    stream.Clear();

    content = R"([-8.9,"ABC"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(2U), R"([-8.9,"ABC"])", __LINE__);
    stream.Clear();

    content = R"(["a","ABCD"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["a","ABCD"])", __LINE__);
    stream.Clear();

    content = R"(["ABC","ABCDE"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["ABC","ABCDE"])", __LINE__);
    stream.Clear();

    content = R"([[],"ABCDEF"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[],"ABCDEF"])", __LINE__);
    stream.Clear();

    content = R"([{},"ABCDEFG"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{},"ABCDEFG"])", __LINE__);
    stream.Clear();

    content = R"([true,[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([true,[]])", __LINE__);
    stream.Clear();

    content = R"([false,[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([false,[]])", __LINE__);
    stream.Clear();

    content = R"([null,[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([null,[]])", __LINE__);
    stream.Clear();

    content = R"([10000,[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([10000,[]])", __LINE__);
    stream.Clear();

    content = R"(["ABC",[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["ABC",[]])", __LINE__);
    stream.Clear();

    content = R"([[],[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[],[]])", __LINE__);
    stream.Clear();

    content = R"([{},[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{},[]])", __LINE__);
    stream.Clear();

    content = R"([true,{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([true,{}])", __LINE__);
    stream.Clear();

    content = R"([false,{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([false,{}])", __LINE__);
    stream.Clear();

    content = R"([null,{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([null,{}])", __LINE__);
    stream.Clear();

    content = R"([-1000,{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([-1000,{}])", __LINE__);
    stream.Clear();

    content = R"(["ABC",{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["ABC",{}])", __LINE__);
    stream.Clear();

    content = R"([[],{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[],{}])", __LINE__);
    stream.Clear();

    content = R"([{},{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{},{}])", __LINE__);
    stream.Clear();

    content = R"([true,{},false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([true,{},false])", __LINE__);
    stream.Clear();

    content = R"([false,[],null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([false,[],null])", __LINE__);
    stream.Clear();

    content = R"([null,{},"ABC"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([null,{},"ABC"])", __LINE__);
    stream.Clear();

    content = R"([null,789,"ABC"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([null,789,"ABC"])", __LINE__);
    stream.Clear();

    content = R"(["ABC",[],{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["ABC",[],{}])", __LINE__);
    stream.Clear();

    content = R"([[],{},498])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[],{},498])", __LINE__);
    stream.Clear();

    content = R"([{},[],true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{},[],true])", __LINE__);
    stream.Clear();

    content = R"([true,{},0,[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([true,{},0,[]])", __LINE__);
    stream.Clear();

    content = R"([false,[],null,{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([false,[],null,{}])", __LINE__);
    stream.Clear();

    content = R"([null,{},"ABC",[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([null,{},"ABC",[]])", __LINE__);
    stream.Clear();

    content = R"(["ABC",[],{},null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["ABC",[],{},null])", __LINE__);
    stream.Clear();

    content = R"([[],false,{},true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[],false,{},true])", __LINE__);
    stream.Clear();

    content = R"([{},null,[],[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{},null,[],[]])", __LINE__);
    stream.Clear();

    content = R"([true,false,null,123,"ABC",[],{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([true,false,null,123,"ABC",[],{}])", __LINE__);
    stream.Clear();

    content = R"([{},[],"a",1.5,null,false,true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{},[],"a",1.5,null,false,true])", __LINE__);
    stream.Clear();

    content = R"([["]"],["]"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([["]"],["]"]])", __LINE__);
    stream.Clear();

    content = R"([["[]"],["[]"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([["[]"],["[]"]])", __LINE__);
    stream.Clear();
}

static void TestParse3(QTest &test) {
    Value<char>        value;
    StringStream<char> stream;
    const char        *content;

    content = R"({})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({})", __LINE__);
    stream.Clear();

    content = R"( {} )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({})", __LINE__);
    stream.Clear();

    content = R"(  {}   )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({})", __LINE__);
    stream.Clear();

    content = R"({ })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({})", __LINE__);
    stream.Clear();

    content = R"({     })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({})", __LINE__);
    stream.Clear();

    content = R"( {"A":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":true})", __LINE__);
    stream.Clear();

    content = R"( { "B":false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"B":false})", __LINE__);
    stream.Clear();

    content = R"( {"AA":null }            )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"AA":null})", __LINE__);
    stream.Clear();

    content = R"( {"a" :0} )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"a":0})", __LINE__);
    stream.Clear();

    content = R"(      {"abc": "a"}        )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"abc":"a"})", __LINE__);
    stream.Clear();

    content = R"({"ab" : "ABC"} )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"ab":"ABC"})", __LINE__);
    stream.Clear();

    content = R"({ "ABC" :[]}         )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"ABC":[]})", __LINE__);
    stream.Clear();

    content = R"(      { "ABC" :[          ]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"ABC":[]})", __LINE__);
    stream.Clear();

    content = R"({"5vn7b83y98t3wrupwmwa4ataw": {         } })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"5vn7b83y98t3wrupwmwa4ataw":{}})", __LINE__);
    stream.Clear();

    content = R"({      "A":true,"B":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":true,"B":true})", __LINE__);
    stream.Clear();

    content = R"({"A":false,"b":true          })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":false,"b":true})", __LINE__);
    stream.Clear();

    content = R"({"A"        :null,"BC":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":null,"BC":true})", __LINE__);
    stream.Clear();

    content = R"({"A":          -3,"AB":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":-3,"AB":true})", __LINE__);
    stream.Clear();

    content = R"({"A":"a"        ,"ABC":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":"a","ABC":true})", __LINE__);
    stream.Clear();

    content = R"({"A":"ABC" ,"1":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":"ABC","1":true})", __LINE__);
    stream.Clear();

    content = R"({"X":[], "123":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"X":[],"123":true})", __LINE__);
    stream.Clear();

    content = R"({"x":{},             "A":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"x":{},"A":true})", __LINE__);
    stream.Clear();

    content = R"({     "A2"     :   true  ,   "A1"  :   false   })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A2":true,"A1":false})", __LINE__);
    stream.Clear();

    content = R"({"A-" : false,"A123" : false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A-":false,"A123":false})", __LINE__);
    stream.Clear();

    content = R"({"A":null,             "B":false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":null,"B":false})", __LINE__);
    stream.Clear();

    content = R"({"A":500,"B":false             })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":500,"B":false})", __LINE__);
    stream.Clear();

    content = R"({           "A":"a","B":false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":"a","B":false})", __LINE__);
    stream.Clear();

    content = R"({"A":          "ABC","B":false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":"ABC","B":false})", __LINE__);
    stream.Clear();

    content = R"({"X":[] ,   "A":false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"X":[],"A":false})", __LINE__);
    stream.Clear();

    content = R"({"X":{},"A"          :false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"X":{},"A":false})", __LINE__);
    stream.Clear();

    content = R"({"A":true,"W":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":true,"W":null})", __LINE__);
    stream.Clear();

    content = R"({"A":false,"@":           null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":false,"@":null})", __LINE__);
    stream.Clear();

    content = R"({"A":null,"#":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":null,"#":null})", __LINE__);
    stream.Clear();

    content = R"({"A":456.5,"H":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":456.5,"H":null})", __LINE__);
    stream.Clear();

    content = R"({"A":"a","Q":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":"a","Q":null})", __LINE__);
    stream.Clear();

    content = R"({"A":"ABC","e":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":"ABC","e":null})", __LINE__);
    stream.Clear();

    content = R"({"X":[],"n":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"X":[],"n":null})", __LINE__);
    stream.Clear();

    content = R"({"t":{},"A":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"t":{},"A":null})", __LINE__);
    stream.Clear();

    content = R"({"X":[          ],"n":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"X":[],"n":null})", __LINE__);
    stream.Clear();

    content = R"({"t":{         },"A":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"t":{},"A":null})", __LINE__);
}

static void TestParse4(QTest &test) {
    Value<char>        value;
    StringStream<char> stream;
    const char        *content;

    content = R"({"e":true,"A":"A"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"e":true,"A":"A"})", __LINE__);
    stream.Clear();

    content = R"({"m":false,"A":"AB"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"m":false,"A":"AB"})", __LINE__);
    stream.Clear();

    content = R"({"i":null,"A":"ABC"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"i":null,"A":"ABC"})", __LINE__);
    stream.Clear();

    content = R"({"s":-8.9,"A":"ABC"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(2), R"({"s":-8.9,"A":"ABC"})", __LINE__);
    stream.Clear();

    content = R"({"-":"a","A":"ABCD"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"-":"a","A":"ABCD"})", __LINE__);
    stream.Clear();

    content = R"({"g":"ABC","A":"ABCDE"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"g":"ABC","A":"ABCDE"})", __LINE__);
    stream.Clear();

    content = R"({"o":[],"A":"ABCDEF"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"o":[],"A":"ABCDEF"})", __LINE__);
    stream.Clear();

    content = R"({"A":{},"o":"ABCDEFG"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":{},"o":"ABCDEFG"})", __LINE__);
    stream.Clear();

    content = R"({"d":true,"y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"d":true,"y":[]})", __LINE__);
    stream.Clear();

    content = R"({"A":false,"y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":false,"y":[]})", __LINE__);
    stream.Clear();

    content = R"({"A":null,"y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":null,"y":[]})", __LINE__);
    stream.Clear();

    content = R"({"A":10000,"y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":10000,"y":[]})", __LINE__);
    stream.Clear();

    content = R"({"A":"ABC","y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":"ABC","y":[]})", __LINE__);
    stream.Clear();

    content = R"({"X":[],"y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"X":[],"y":[]})", __LINE__);
    stream.Clear();

    content = R"({"X":{},"Y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"X":{},"Y":[]})", __LINE__);
    stream.Clear();

    content = R"({"A":true,"y":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":true,"y":{}})", __LINE__);
    stream.Clear();

    content = R"({"A":false,"y":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":false,"y":{}})", __LINE__);
    stream.Clear();

    content = R"({"A":null,"y":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":null,"y":{}})", __LINE__);
    stream.Clear();

    content = R"({"A":-1000,"y":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":-1000,"y":{}})", __LINE__);
    stream.Clear();

    content = R"({"A":"ABC","y":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":"ABC","y":{}})", __LINE__);
    stream.Clear();

    content = R"({"C":[],"R":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"C":[],"R":{}})", __LINE__);
    stream.Clear();

    content = R"({"cc":{},"rr":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"cc":{},"rr":{}})", __LINE__);
    stream.Clear();

    content = R"({"A":true,"y":{},"AA":false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":true,"y":{},"AA":false})", __LINE__);
    stream.Clear();

    content = R"({"A":false,"y":[],"B":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":false,"y":[],"B":null})", __LINE__);
    stream.Clear();

    content = R"({"A":null,"y":{},"ABC":"ABC"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":null,"y":{},"ABC":"ABC"})", __LINE__);
    stream.Clear();

    content = R"({"t":null,"Y":789,"A":"ABC"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"t":null,"Y":789,"A":"ABC"})", __LINE__);
    stream.Clear();

    content = R"({"A":"ABC","y":[],"key-u":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":"ABC","y":[],"key-u":{}})", __LINE__);
    stream.Clear();

    content = R"({"X":[],"Y":{},"key-u":498})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"X":[],"Y":{},"key-u":498})", __LINE__);
    stream.Clear();

    content = R"({"X":{},"y":[],"A":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"X":{},"y":[],"A":true})", __LINE__);
    stream.Clear();

    content = R"({"{}}":true,"y":{},"AA":0,"k-300":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"{}}":true,"y":{},"AA":0,"k-300":[]})", __LINE__);
    stream.Clear();

    content = R"({"B1":false,"y":[],"[A]":null,"k-300":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"B1":false,"y":[],"[A]":null,"k-300":{}})", __LINE__);
    stream.Clear();

    content = R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})", __LINE__);
    stream.Clear();

    content = R"({"x":"ABC","[]]":[],"key-u":{},"A":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"x":"ABC","[]]":[],"key-u":{},"A":null})", __LINE__);
    stream.Clear();

    content = R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})", __LINE__);
    stream.Clear();

    content = R"({"X":{},"A":null,"key-u":[],"k-300":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"X":{},"A":null,"key-u":[],"k-300":[]})", __LINE__);
    stream.Clear();

    content = R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})", __LINE__);
    stream.Clear();

    content = R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream),
                 R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})", __LINE__);
    stream.Clear();

    content = R"({"a":{"c":"}"},"b":{"d":"}"}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"a":{"c":"}"},"b":{"d":"}"}})", __LINE__);
    stream.Clear();

    content = R"({"a":{"c":"{}"},"b":{"d":"{}"}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"a":{"c":"{}"},"b":{"d":"{}"}})", __LINE__);
}

static void TestParse5(QTest &test) {
    Value<char>        value;
    StringStream<char> stream;
    const char        *content;

    content = R"([{"one":1}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{"one":1}])", __LINE__);
    stream.Clear();

    content = R"([{"one":"a"}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{"one":"a"}])", __LINE__);
    stream.Clear();

    content = R"(["a","b"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["a","b"])", __LINE__);
    stream.Clear();

    content = R"([[1,2],[30]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[1,2],[30]])", __LINE__);
    stream.Clear();

    content = R"([[[1]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[[1]]])", __LINE__);
    stream.Clear();

    content = R"([[123]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[123]])", __LINE__);
    stream.Clear();

    content = R"([[1,2]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[1,2]])", __LINE__);
    stream.Clear();

    content = R"([["a","b"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([["a","b"]])", __LINE__);
    stream.Clear();

    content = R"([[1,2,3]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[1,2,3]])", __LINE__);
    stream.Clear();

    content = R"([[[1,2,3]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[[1,2,3]]])", __LINE__);
    stream.Clear();

    content = R"([[[[1,2,3]]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[[[1,2,3]]]])", __LINE__);
    stream.Clear();

    content = R"([123,"",456])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([123,"",456])", __LINE__);
    stream.Clear();

    content = R"([[1,"",2]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[1,"",2]])", __LINE__);
    stream.Clear();

    content = R"([[123,456,""]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[123,456,""]])", __LINE__);
    stream.Clear();

    content = R"({"a":[1],"b":1})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"a":[1],"b":1})", __LINE__);
    stream.Clear();

    content = R"([[],123,456])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[],123,456])", __LINE__);
    stream.Clear();

    content = R"([123,456,[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([123,456,[]])", __LINE__);
    stream.Clear();

    content = R"([[[],1]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[[],1]])", __LINE__);
    stream.Clear();

    content = R"([[123,456,[]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[123,456,[]]])", __LINE__);
    stream.Clear();

    content = R"([3,[4],5])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([3,[4],5])", __LINE__);
    stream.Clear();

    content = R"({})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({})", __LINE__);
    stream.Clear();

    content = R"({"a":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"a":{}})", __LINE__);
    stream.Clear();

    content = R"([[[{}]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[[{}]]])", __LINE__);
    stream.Clear();

    content = R"([[[],[]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[[],[]]])", __LINE__);
    stream.Clear();

    content = R"(["oo",[[],[],[{},[]]],"bk"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["oo",[[],[],[{},[]]],"bk"])", __LINE__);
    stream.Clear();

    content = R"([{},[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{},[]])", __LINE__);
    stream.Clear();

    content = R"(["aa",[[{}],["ww","ee"]],"bb"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["aa",[[{}],["ww","ee"]],"bb"])", __LINE__);
    stream.Clear();

    content = R"({"aa":44,"dd":"bb"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"aa":44,"dd":"bb"})", __LINE__);
    stream.Clear();

    content = R"({"aa":[],"dd":"bb"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"aa":[],"dd":"bb"})", __LINE__);
    stream.Clear();

    content = R"([[],[[[]]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[],[[[]]]])", __LINE__);
    stream.Clear();

    content = R"([[[["1","2","3"]]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[[["1","2","3"]]]])", __LINE__);
    stream.Clear();

    content = R"([{"ab":{}}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{"ab":{}}])", __LINE__);
    stream.Clear();

    content = R"([{"ab":{},"cd":""}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{"ab":{},"cd":""}])", __LINE__);
    stream.Clear();

    content = R"([[[],[]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[[],[]]])", __LINE__);
    stream.Clear();

    content = R"({"a":[],"c":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"a":[],"c":[]})", __LINE__);
    stream.Clear();

    content = R"({"\"bb\"":"\""})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"\"bb\"":"\""})", __LINE__);
    stream.Clear();

    content = R"(["\"bb\"","\""])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["\"bb\"","\""])", __LINE__);
    stream.Clear();

    content = R"(["s\""])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["s\""])", __LINE__);
    stream.Clear();

    content = R"(["\\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["\\"])", __LINE__);
}

static void TestParse6(QTest &test) {
    Value<char>        value;
    StringStream<char> stream;
    const char        *content;

    content = R"(["a\\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["a\\"])", __LINE__);
    stream.Clear();

    content = R"(["\\a"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["\\a"])", __LINE__);
    stream.Clear();

    content = R"(["a\\a"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(), R"(["a\\a"])", __LINE__);

    content = R"(["aaa\\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["aaa\\"])", __LINE__);
    stream.Clear();

    content = R"(["\\aaa"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["\\aaa"])", __LINE__);
    stream.Clear();

    content = R"(["aaa\\aaa"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["aaa\\aaa"])", __LINE__);
    stream.Clear();

    content = R"(["c,"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["c,"])", __LINE__);
    stream.Clear();

    content = R"(["]][[][]"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["]][[][]"])", __LINE__);
    stream.Clear();

    content = R"(["k,","l"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["k,","l"])", __LINE__);
    stream.Clear();

    content = R"([[[1],50]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[[1],50]])", __LINE__);
    stream.Clear();

    content = R"([["[,]",5]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([["[,]",5]])", __LINE__);
    stream.Clear();

    content = R"([[["k,,,,,","l"]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[["k,,,,,","l"]]])", __LINE__);
    stream.Clear();

    content = R"(["k,","e","W","u"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["k,","e","W","u"])", __LINE__);
    stream.Clear();

    content = R"([{"a":{}},[{},[1,2,3],["aa","cc"]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{"a":{}},[{},[1,2,3],["aa","cc"]]])", __LINE__);
    stream.Clear();

    content = R"(["s\"s\"s\"s\"s\"s\"s\"s\"s\"\\\\\\\\\\\\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["s\"s\"s\"s\"s\"s\"s\"s\"s\"\\\\\\\\\\\\"])", __LINE__);
    stream.Clear();

    value = JSON::Parse(
        R"(["s\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""])");
    test.IsEqual(
        value.Stringify(stream),
        R"(["s\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""])",
        __LINE__);
    stream.Clear();

    content = R"(["a\nb"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"(["a\nb"])", __LINE__);
    stream.Clear();

    content = R"([["[]{}","A"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([["[]{}","A"]])", __LINE__);
    stream.Clear();

    content = R"([["[]{}",1]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([["[]{}",1]])", __LINE__);
    stream.Clear();

    content = R"([{"[]{}":"A"}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{"[]{}":"A"}])", __LINE__);
    stream.Clear();

    content = R"([{"[]{}":1}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([{"[]{}":1}])", __LINE__);
    stream.Clear();

    content = R"([[["1","2","3"],"c","d"],"a",["1"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[["1","2","3"],"c","d"],"a",["1"]])", __LINE__);
    stream.Clear();

    content = R"([[[1,2,3],40,50],4,[0.5],5])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[[1,2,3],40,50],4,[0.5],5])", __LINE__);
    stream.Clear();

    content = R"([[[[],30]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(), R"([[[[],30]]])", __LINE__);

    content = R"([[],[30]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[],[30]])", __LINE__);
    stream.Clear();

    content = R"([[1],[30]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([[1],[30]])", __LINE__);
    stream.Clear();

    content = R"({"a":{"b":"c"}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"({"a":{"b":"c"}})", __LINE__);
    stream.Clear();

    content = R"([12.1211212121212])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([12.1211212121212])", __LINE__);
    stream.Clear();

    content = R"([-12.1211212121212])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([-12.1211212121212])", __LINE__);
    stream.Clear();

    content = R"([3.123456789123456789123456789123456789123456789123456789123456789])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([3.12345678912346])", __LINE__);
    stream.Clear();

    content = R"([3.123456789123456789123456789123456789123456789123456789123456789e+10])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(stream), R"([31234567891.2346])", __LINE__);
    stream.Clear();

    content = R"([3.123456789123456789123456789123456789123456789123456789123456789e-10])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(6U), R"([3.12346e-10])", __LINE__);
    stream.Clear();

    content = R"([1.0e308])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(6U), R"([1e+308])", __LINE__);
    stream.Clear();

    value = JSON::Parse(
        R"([                                                                 1,
                                                                                               2
                                                                                                          ])");
    test.IsEqual(value.Stringify(stream), R"([1,2])", __LINE__);
    stream.Clear();

    content = R"([0xAAAA, 0xFFFFFFFFFFFFFFFF, 0Xabcdef])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsEqual(value.Stringify(), R"([43690,18446744073709551615,11259375])", __LINE__);
}

static void TestParse7(QTest &test) {
    Value<char>         value;
    StringStream<char>  stream;
    const String<char> *key_ptr;
    const char         *content;

    content = R"(["\r"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(StringUtils::IsEqual("\r", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"({"x":   "\n"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(StringUtils::IsEqual("\n", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"({"\""   :"x"       })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKeyAt(0);
    test.IsNotNull(key_ptr, __LINE__);
    test.IsTrue(StringUtils::IsEqual("\"", key_ptr->First(), value[0].Length()), __LINE__);

    content = R"({    "\n":   "\f"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKeyAt(0);
    test.IsNotNull(key_ptr, __LINE__);
    test.IsTrue(StringUtils::IsEqual("\n", key_ptr->First(), value[0].Length()), __LINE__);
    test.IsTrue(StringUtils::IsEqual("\f", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"(["\/\/"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(StringUtils::IsEqual("//", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"({"x":"\r\r"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(StringUtils::IsEqual("\r\r", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"({"\b\b":"x"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKeyAt(0);
    test.IsNotNull(key_ptr, __LINE__);
    test.IsTrue(StringUtils::IsEqual("\b\b", key_ptr->First(), value[0].Length()), __LINE__);

    content = R"({"\\\\":"\"\""})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKeyAt(0);
    test.IsNotNull(key_ptr, __LINE__);
    test.IsTrue(StringUtils::IsEqual(R"(\\)", key_ptr->First(), value[0].Length()), __LINE__);
    test.IsTrue(StringUtils::IsEqual(R"("")", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"(["\t\t\t"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(StringUtils::IsEqual("\t\t\t", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"({"x":"\f\f\f"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(StringUtils::IsEqual("\f\f\f", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"({"\\\\\\":"x"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKeyAt(0);
    test.IsNotNull(key_ptr, __LINE__);
    test.IsTrue(StringUtils::IsEqual(R"(\\\)", key_ptr->First(), value[0].Length()), __LINE__);

    content = R"({"\/\/\/":"\n\n\n"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKeyAt(0);
    test.IsNotNull(key_ptr, __LINE__);
    test.IsTrue(StringUtils::IsEqual("///", key_ptr->First(), value[0].Length()), __LINE__);
    test.IsTrue(StringUtils::IsEqual("\n\n\n", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"([" \""])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(StringUtils::IsEqual(" \"", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"([" \t "])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(StringUtils::IsEqual(" \t ", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"(["\\ \\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(StringUtils::IsEqual("\\ \\", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"(["\"\\\/\b\f\n\r\t"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(StringUtils::IsEqual("\"\\/\b\f\n\r\t", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"({"x":"\t\r\n\f\b\/\\\""})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(StringUtils::IsEqual("\t\r\n\f\b/\\\"", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"({"\t\r\n\f\b\/\\\"":"\"\\\/\b\f\n\r\t"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKeyAt(0);
    test.IsNotNull(key_ptr, __LINE__);
    test.IsTrue(StringUtils::IsEqual("\t\r\n\f\b/\\\"", key_ptr->First(), value[0].Length()), __LINE__);
    test.IsTrue(StringUtils::IsEqual("\"\\/\b\f\n\r\t", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"(["\u003D"])";
    value   = JSON::Parse(content);
    test.IsTrue(StringUtils::IsEqual("=", value[0].StringStorage(), value[0].Length()), __LINE__);

    content = R"(["\u00a1"])";
    value   = JSON::Parse(content, 10);
    test.IsTrue(StringUtils::IsEqual("¡", value[0].StringStorage(), value[0].Length()), __LINE__);

    String<char> str(R"(["\u08A7"])");
    value = JSON::Parse(str.First(), str.Length());
    test.IsTrue(StringUtils::IsEqual("ࢧ", value[0].StringStorage(), value[0].Length()), __LINE__);

    str   = R"(["\ud802\uDE7B"])";
    value = JSON::Parse(str.First(), str.Length());
    test.IsTrue(StringUtils::IsEqual("𐩻", value[0].StringStorage(), value[0].Length()), __LINE__);

    str   = R"(["\uD83E\uFC59"])";
    value = JSON::Parse(str.First(), str.Length());
    test.IsTrue(StringUtils::IsEqual("🡙", value[0].StringStorage(), value[0].Length()), __LINE__);

    str   = R"(["\UD800\UDE83W\U003DW\UD800\UDE83\U00A1\UD83E\UFC59\U08A7"])";
    value = JSON::Parse(str.First(), str.Length());
    test.IsTrue(StringUtils::IsEqual("𐊃W=W𐊃¡🡙ࢧ", value[0].StringStorage(), value[0].Length()), __LINE__);
}

static void TestParse8(QTest &test) {
    Value<char>        value;
    StringStream<char> stream;
    const char        *content;

    content = R"([1   ]    ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"a":1   }    })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([f])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([t])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([n])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([falsE])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([truE])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([nulL])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([false-])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([true-])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([null-])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([1,])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([1,   ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([1   ,   ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([,])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["1",,"2"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([,1])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([,"1"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["1"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([["1"]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([  []]  ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["w"],1])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["w" o  , 1])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"w": "a" o  , "b": "x"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["w" ] ,1])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["x"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["aa"],"s"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["x":"s"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([{"x"::"s"}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x":})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([]")";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({}")";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({},)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([],)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([[[[[[[[[001]]]]]]]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"a":{"b":{"c":001}}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([w])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({x:"x"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x":001})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x":4)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["x")";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([4,])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x" 4})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x" "4"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x" {}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x" []})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x",4})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([4,])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x":4,})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([4,,])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x":4,,})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x":"w" w})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x":"w" true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["w" true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["\w"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([""]")";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);
}

static void TestParse9(QTest &test) {
    Value<char>        value;
    StringStream<char> stream;
    const char        *content;

    content = R"(["\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["""])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["a",)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"a":"c",)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    value = JSON::Parse("[\"abc\n\"]");
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([123e])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);
    stream.Clear();

    content = R"([123e--4])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    value = JSON::Parse("[\"\ta\"]");
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["\uaaa"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["\uaa"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["\ua"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["\u"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["\uD83E\uFC5"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([[{} {}]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([{} {}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([[] []])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["w" "w"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["w" {}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([4    {}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({a "a":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"a":a []})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"a"a :[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x":"w" "w"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x":"w" "u":"w"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([{} 0])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([["x" 0]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([{} 1])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([[{} 0]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([[] false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["w" null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"(["w" 3])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([4    4])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x":"w" 7})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"x":"w" 5})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({1)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([2)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"a":h, "b": a})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"({"a":{})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    value = JSON::Parse("{\"\na\":{}");
    test.IsTrue(value.IsUndefined(), __LINE__);

    value = JSON::Parse("[\"WHAT?\"}");
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([1e])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([1e+])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([1e-])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([1e++])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);

    content = R"([1e--])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    test.IsTrue(value.IsUndefined(), __LINE__);
}

template <typename String_T>
static void TestStripComments(QTest &test) {
    Value<char>        value;
    String_T           in;
    StringStream<char> out;

    in = R"([1,2,3,4,5,6,7,8,9,10])";
    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,2,3,4,5,6,7,8,9,10])", __LINE__);
    out.Clear();

    in = R"(
    [1,//2,
    3,4,5,6,7,8,9,10]
    )";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());

    test.IsEqual(value.Stringify(out), R"([1,3,4,5,6,7,8,9,10])", __LINE__);
    out.Clear();

    in = R"(
    [1,//2,
    3,4,5,6
    //,7
    ,8,9,10]
    )";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,3,4,5,6,8,9,10])", __LINE__);
    out.Clear();

    in = R"(
    [1,//2,
    3,4,5,6
    //,7
    ,8,9
    //,10
    ]
    )";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,3,4,5,6,8,9])", __LINE__);
    out.Clear();

    in = R"(["//"])";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"(["\/\/"])", __LINE__);
    out.Clear();

    in = R"(["//", "//"])";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"(["\/\/","\/\/"])", __LINE__);
    out.Clear();

    in = R"(["//\"", "//\\"])";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"(["\/\/\"","\/\/\\"])", __LINE__);
    out.Clear();

    in = R"(["//\\\"", "//\\"])";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"(["\/\/\\\"","\/\/\\"])", __LINE__);
    out.Clear();

    in = R"([1,"//","//3",4,5,"6//"
    //,7
    ,8,9
    //,10
    ]
    )";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,"\/\/","\/\/3",4,5,"6\/\/",8,9])", __LINE__);
    out.Clear();

    in = R"([1,2,3/*,4,5,6*/,7,8,9])";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,2,3,7,8,9])", __LINE__);
    out.Clear();

    in = R"([1,2,3/*,4,5,6*/,7/*ooo*/,8,9])";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,2,3,7,8,9])", __LINE__);
    out.Clear();

    in = R"([1/*ooo*/,2,3,7,8/*ooo*/,9])";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,2,3,7,8,9])", __LINE__);
    out.Clear();

    in = R"(/*ooo*/[1,2,3,7,8,9])";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,2,3,7,8,9])", __LINE__);
    out.Clear();

    in = R"([1,2,3,7,8,9]/*ooo*/)";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,2,3,7,8,9])", __LINE__);
    out.Clear();

    in = R"(/*ooo*/[1,2,3,7,8,9]/*ooo*/)";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,2,3,7,8,9])", __LINE__);
    out.Clear();

    in = R"([/*o*/1,/*o*/2,/*o*/3,7,8,9])";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,2,3,7,8,9])", __LINE__);
    out.Clear();

    in = R"(/*ooo*/[1,/*ooo*/2,/*ooo*/3,/*ooo*/7,8,9]/*ooo*/)";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,2,3,7,8,9])", __LINE__);
    out.Clear();

    in = R"(/*ooo*/[1,
    /*ooo*/2,/*ooo*/3,
    //ddddd
    /*ooo*/7,8,
    //9]
    10]
    /*ooo*/)";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,2,3,7,8,10])", __LINE__);
    out.Clear();

    in = R"(["/*A*/"])";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"(["\/*A*\/"])", __LINE__);
    out.Clear();

    in = R"(["/*A*/"/*ggg*/])";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"(["\/*A*\/"])", __LINE__);
    out.Clear();

    in = R"(["B/*"/*A*/])";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"(["B\/*"])", __LINE__);
    out.Clear();

    in = R"([1, 2, 3] /* unterminated)";

    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"([1,2,3])", __LINE__);
    out.Clear();

    in = R"([1, 2, 3 /* unterminated)";
    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsTrue(value.IsUndefined(), __LINE__);
    out.Clear();

    in = R"([1, // first\n // second\n 2])";
    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsTrue(value.IsUndefined(), __LINE__);
    out.Clear();

    in = R"(// just a comment)";
    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsTrue(value.IsUndefined(), __LINE__);
    out.Clear();

    in = R"(/* just a block comment */)";
    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsTrue(value.IsUndefined(), __LINE__);
    out.Clear();

    in = R"(["//", /*foo*/ 1, 2, "//"])";
    StringUtils::StripComments(in.Storage(), in.Length());
    value = JSON::Parse(out, in.First(), in.Length());
    test.IsEqual(value.Stringify(out), R"(["\/\/",1,2,"\/\/"])", __LINE__);
}

static int RunJSONTests() {
    QTest test{"JSON.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("Parse Test 1", TestParse1);
    test.Test("Parse Test 2", TestParse2);
    test.Test("Parse Test 3", TestParse3);
    test.Test("Parse Test 4", TestParse4);
    test.Test("Parse Test 5", TestParse5);
    test.Test("Parse Test 6", TestParse6);
    test.Test("Parse Test 7", TestParse7);
    test.Test("Parse Test 8", TestParse8);
    test.Test("Parse Test 9", TestParse9);
    test.Test("StripComments Test 1", TestStripComments<StringStream<char>>);
    test.Test("StripComments Test 2", TestStripComments<String<char>>);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
