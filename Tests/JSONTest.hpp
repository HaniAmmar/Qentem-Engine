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
#include "JSON.hpp"

#ifndef QENTEM_JSON_TESTS_H
#define QENTEM_JSON_TESTS_H

namespace Qentem {
namespace Test {

static void TestParse1(QTest &helper) {
    Value<char>        value;
    StringStream<char> stream;
    const char        *content;

    ///////////
    content = R"([])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([ ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"( [])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"( [ ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([]    )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(     [])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(     []    )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(    [])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(    [ ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([    ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"( [true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([true])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"( [false ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([false])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([    null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([null])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([ 0 ]     )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([0])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([      "a"   ]          )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["a"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(        ["ABC"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["ABC"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([ [ ]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[          ]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([         [          ]           ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([ {}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{        }])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([ { } ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([ true,true ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([true,true])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([false, true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([false,true])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([null ,true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([null,true])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([-3 , true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([-3,true])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["a"       ,    true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["a",true])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([   "ABC",true       ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["ABC",true])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([       [] ,  true      ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[],true])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([ {} , true ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{},true])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([  true ,false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([true,false])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([false, false ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([false,false])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([ null ,false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([null,false])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([500,false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([500,false])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["a",false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["a",false])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([ "ABC" , false ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["ABC",false])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[]                      ,false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[],false])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{},                     false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{},false])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([true,null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([true,null])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([false,null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([false,null])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([null,null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([null,null])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([456.5,null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([456.5,null])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["a",null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["a",null])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["ABC",null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["ABC",null])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[],null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[],null])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{},null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{},null])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([true,"A"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([true,"A"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([false,"AB"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([false,"AB"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([null,"ABC"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([null,"ABC"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([-8.9,"ABC"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(2U), R"([-8.9,"ABC"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["a","ABCD"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["a","ABCD"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["ABC","ABCDE"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["ABC","ABCDE"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[],"ABCDEF"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[],"ABCDEF"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{},"ABCDEFG"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{},"ABCDEFG"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([true,[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([true,[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([false,[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([false,[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([null,[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([null,[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([10000,[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([10000,[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["ABC",[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["ABC",[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[],[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[],[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{},[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{},[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([true,{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([true,{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([false,{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([false,{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([null,{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([null,{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([-1000,{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([-1000,{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["ABC",{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["ABC",{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[],{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[],{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{},{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{},{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([true,{},false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([true,{},false])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([false,[],null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([false,[],null])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([null,{},"ABC"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([null,{},"ABC"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([null,789,"ABC"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([null,789,"ABC"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["ABC",[],{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["ABC",[],{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[],{},498])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[],{},498])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{},[],true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{},[],true])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([true,{},0,[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([true,{},0,[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([false,[],null,{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([false,[],null,{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([null,{},"ABC",[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([null,{},"ABC",[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["ABC",[],{},null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["ABC",[],{},null])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[],false,{},true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[],false,{},true])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{},null,[],[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{},null,[],[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([true,false,null,123,"ABC",[],{}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([true,false,null,123,"ABC",[],{}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{},[],"a",1.5,null,false,true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{},[],"a",1.5,null,false,true])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([["]"],["]"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([["]"],["]"]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([["[]"],["[]"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([["[]"],["[]"]])", "Stringify(stream)", __LINE__);
    stream.Clear();
}

static void TestParse2(QTest &helper) {
    Value<char>        value;
    StringStream<char> stream;
    const char        *content;

    content = R"({})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"( {} )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(  {}   )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({ })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({     })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"( {"A":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":true})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"( { "B":false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"B":false})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"( {"AA":null }            )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"AA":null})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"( {"a" :0} )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"a":0})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(      {"abc": "a"}        )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"abc":"a"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"ab" : "ABC"} )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"ab":"ABC"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({ "ABC" :[]}         )";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"ABC":[]})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(      { "ABC" :[          ]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"ABC":[]})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"5vn7b83y98t3wrupwmwa4ataw": {         } })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"5vn7b83y98t3wrupwmwa4ataw":{}})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({      "A":true,"B":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":true,"B":true})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":false,"b":true          })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":false,"b":true})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A"        :null,"BC":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":null,"BC":true})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":          -3,"AB":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":-3,"AB":true})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":"a"        ,"ABC":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":"a","ABC":true})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":"ABC" ,"1":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":"ABC","1":true})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"X":[], "123":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"X":[],"123":true})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"x":{},             "A":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"x":{},"A":true})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({     "A2"     :   true  ,   "A1"  :   false   })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A2":true,"A1":false})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A-" : false,"A123" : false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A-":false,"A123":false})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":null,             "B":false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":null,"B":false})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":500,"B":false             })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":500,"B":false})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({           "A":"a","B":false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":"a","B":false})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":          "ABC","B":false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":"ABC","B":false})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"X":[] ,   "A":false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"X":[],"A":false})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"X":{},"A"          :false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"X":{},"A":false})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":true,"W":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":true,"W":null})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":false,"@":           null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":false,"@":null})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":null,"#":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":null,"#":null})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":456.5,"H":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":456.5,"H":null})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":"a","Q":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":"a","Q":null})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":"ABC","e":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":"ABC","e":null})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"X":[],"n":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"X":[],"n":null})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"t":{},"A":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"t":{},"A":null})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"X":[          ],"n":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"X":[],"n":null})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"t":{         },"A":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"t":{},"A":null})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"e":true,"A":"A"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"e":true,"A":"A"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"m":false,"A":"AB"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"m":false,"A":"AB"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"i":null,"A":"ABC"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"i":null,"A":"ABC"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"s":-8.9,"A":"ABC"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(2), R"({"s":-8.9,"A":"ABC"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"-":"a","A":"ABCD"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"-":"a","A":"ABCD"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"g":"ABC","A":"ABCDE"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"g":"ABC","A":"ABCDE"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"o":[],"A":"ABCDEF"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"o":[],"A":"ABCDEF"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":{},"o":"ABCDEFG"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":{},"o":"ABCDEFG"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"d":true,"y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"d":true,"y":[]})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":false,"y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":false,"y":[]})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":null,"y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":null,"y":[]})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":10000,"y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":10000,"y":[]})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":"ABC","y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":"ABC","y":[]})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"X":[],"y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"X":[],"y":[]})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"X":{},"Y":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"X":{},"Y":[]})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":true,"y":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":true,"y":{}})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":false,"y":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":false,"y":{}})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":null,"y":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":null,"y":{}})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":-1000,"y":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":-1000,"y":{}})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":"ABC","y":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":"ABC","y":{}})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"C":[],"R":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"C":[],"R":{}})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"cc":{},"rr":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"cc":{},"rr":{}})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":true,"y":{},"AA":false})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":true,"y":{},"AA":false})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":false,"y":[],"B":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":false,"y":[],"B":null})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":null,"y":{},"ABC":"ABC"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":null,"y":{},"ABC":"ABC"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"t":null,"Y":789,"A":"ABC"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"t":null,"Y":789,"A":"ABC"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":"ABC","y":[],"key-u":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":"ABC","y":[],"key-u":{}})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"X":[],"Y":{},"key-u":498})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"X":[],"Y":{},"key-u":498})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"X":{},"y":[],"A":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"X":{},"y":[],"A":true})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"{}}":true,"y":{},"AA":0,"k-300":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"{}}":true,"y":{},"AA":0,"k-300":[]})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"B1":false,"y":[],"[A]":null,"k-300":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"B1":false,"y":[],"[A]":null,"k-300":{}})", "Stringify(stream)",
                 __LINE__);
    stream.Clear();

    content = R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})", "Stringify(stream)",
                 __LINE__);
    stream.Clear();

    content = R"({"x":"ABC","[]]":[],"key-u":{},"A":null})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"x":"ABC","[]]":[],"key-u":{},"A":null})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})", "Stringify(stream)",
                 __LINE__);
    stream.Clear();

    content = R"({"X":{},"A":null,"key-u":[],"k-300":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"X":{},"A":null,"key-u":[],"k-300":[]})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})",
                 "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream),
                 R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})",
                 "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"a":{"c":"}"},"b":{"d":"}"}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"a":{"c":"}"},"b":{"d":"}"}})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"a":{"c":"{}"},"b":{"d":"{}"}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"a":{"c":"{}"},"b":{"d":"{}"}})", "Stringify(stream)", __LINE__);
    stream.Clear();
}

static void TestParse3(QTest &helper) {
    Value<char>        value;
    StringStream<char> stream;
    const char        *content;

    content = R"([{"one":1}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{"one":1}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{"one":"a"}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{"one":"a"}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["a","b"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["a","b"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[1,2],[30]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[1,2],[30]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[[1]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[[1]]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[123]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[123]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[1,2]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[1,2]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([["a","b"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([["a","b"]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[1,2,3]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[1,2,3]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[[1,2,3]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[[1,2,3]]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[[[1,2,3]]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[[[1,2,3]]]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([123,"",456])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([123,"",456])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[1,"",2]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[1,"",2]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[123,456,""]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[123,456,""]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"a":[1],"b":1})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"a":[1],"b":1})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[],123,456])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[],123,456])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([123,456,[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([123,456,[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[[],1]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[[],1]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[123,456,[]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[123,456,[]]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([3,[4],5])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([3,[4],5])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"a":{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"a":{}})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[[{}]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[[{}]]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[[],[]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[[],[]]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["oo",[[],[],[{},[]]],"bk"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["oo",[[],[],[{},[]]],"bk"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{},[]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{},[]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["aa",[[{}],["ww","ee"]],"bb"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["aa",[[{}],["ww","ee"]],"bb"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"aa":44,"dd":"bb"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"aa":44,"dd":"bb"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"aa":[],"dd":"bb"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"aa":[],"dd":"bb"})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[],[[[]]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[],[[[]]]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[[["1","2","3"]]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[[["1","2","3"]]]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{"ab":{}}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{"ab":{}}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{"ab":{},"cd":""}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{"ab":{},"cd":""}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[[],[]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[[],[]]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"a":[],"c":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"a":[],"c":[]})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"\"bb\"":"\""})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"\"bb\"":"\""})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["\"bb\"","\""])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["\"bb\"","\""])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["s\""])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["s\""])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["\\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["\\"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["a\\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["a\\"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["\\a"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["\\a"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["a\\a"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(), R"(["a\\a"])", "Stringify(stream)", __LINE__);

    content = R"(["aaa\\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["aaa\\"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["\\aaa"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["\\aaa"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["aaa\\aaa"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["aaa\\aaa"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["c,"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["c,"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["]][[][]"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["]][[][]"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["k,","l"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["k,","l"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[[1],50]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[[1],50]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([["[,]",5]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([["[,]",5]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[["k,,,,,","l"]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[["k,,,,,","l"]]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["k,","e","W","u"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["k,","e","W","u"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{"a":{}},[{},[1,2,3],["aa","cc"]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{"a":{}},[{},[1,2,3],["aa","cc"]]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["s\"s\"s\"s\"s\"s\"s\"s\"s\"\\\\\\\\\\\\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["s\"s\"s\"s\"s\"s\"s\"s\"s\"\\\\\\\\\\\\"])", "Stringify(stream)",
                 __LINE__);
    stream.Clear();

    value = JSON::Parse(
        R"(["s\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""])");
    helper.Equal(
        value.Stringify(stream),
        R"(["s\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""])",
        "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"(["a\nb"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"(["a\nb"])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([["[]{}","A"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([["[]{}","A"]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([["[]{}",1]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([["[]{}",1]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{"[]{}":"A"}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{"[]{}":"A"}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([{"[]{}":1}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([{"[]{}":1}])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[["1","2","3"],"c","d"],"a",["1"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[["1","2","3"],"c","d"],"a",["1"]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[[1,2,3],40,50],4,[0.5],5])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[[1,2,3],40,50],4,[0.5],5])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[[[],30]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(), R"([[[[],30]]])", "Stringify(stream)", __LINE__);

    content = R"([[],[30]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[],[30]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([[1],[30]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([[1],[30]])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"({"a":{"b":"c"}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"({"a":{"b":"c"}})", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([12.1211212121212])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([12.1211212121212])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([-12.1211212121212])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([-12.1211212121212])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([3.123456789123456789123456789123456789123456789123456789123456789])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([3.12345678912346])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([3.123456789123456789123456789123456789123456789123456789123456789e+10])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(stream), R"([31234567891.2346])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([3.123456789123456789123456789123456789123456789123456789123456789e-10])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(6U), R"([3.12346e-10])", "Stringify(stream)", __LINE__);
    stream.Clear();

    content = R"([1.0e308])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.Equal(value.Stringify(6U), R"([1e+308])", "Stringify(stream)", __LINE__);
    stream.Clear();

    value = JSON::Parse(
        R"([                                                                 1,
                                                                                               2
                                                                                                          ])");
    helper.Equal(value.Stringify(stream), R"([1,2])", "Stringify(stream)", __LINE__);
    stream.Clear();
}

static void TestParse4(QTest &helper) {
    Value<char>         value;
    StringStream<char>  stream;
    const String<char> *key_ptr;
    const char         *content;

    content = R"(["\r"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(StringUtils::IsEqual("\r", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    content = R"({"x":   "\n"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(StringUtils::IsEqual("\n", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    content = R"({"\""   :"x"       })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("\"", key_ptr->First(), value[0].Length()), "IsEqual()", __LINE__);

    content = R"({    "\n":   "\f"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("\n", key_ptr->First(), value[0].Length()), "IsEqual()", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("\f", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    content = R"(["\/\/"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(StringUtils::IsEqual("//", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    content = R"({"x":"\r\r"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(StringUtils::IsEqual("\r\r", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    content = R"({"\b\b":"x"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("\b\b", key_ptr->First(), value[0].Length()), "IsEqual()", __LINE__);

    content = R"({"\\\\":"\"\""})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(R"(\\)", key_ptr->First(), value[0].Length()), "IsEqual()", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(R"("")", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    content = R"(["\t\t\t"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(StringUtils::IsEqual("\t\t\t", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    content = R"({"x":"\f\f\f"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(StringUtils::IsEqual("\f\f\f", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    content = R"({"\\\\\\":"x"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(R"(\\\)", key_ptr->First(), value[0].Length()), "IsEqual()", __LINE__);

    content = R"({"\/\/\/":"\n\n\n"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("///", key_ptr->First(), value[0].Length()), "IsEqual()", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("\n\n\n", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    content = R"([" \""])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(StringUtils::IsEqual(" \"", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    content = R"([" \t "])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(StringUtils::IsEqual(" \t ", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    content = R"(["\\ \\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(StringUtils::IsEqual("\\ \\", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    content = R"(["\"\\\/\b\f\n\r\t"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(StringUtils::IsEqual("\"\\/\b\f\n\r\t", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    content = R"({"x":"\t\r\n\f\b\/\\\""})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(StringUtils::IsEqual("\t\r\n\f\b/\\\"", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    content = R"({"\t\r\n\f\b\/\\\"":"\"\\\/\b\f\n\r\t"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("\t\r\n\f\b/\\\"", key_ptr->First(), value[0].Length()), "IsEqual()",
                      __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("\"\\/\b\f\n\r\t", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    content = R"(["\u003D"])";
    value   = JSON::Parse(content);
    helper.EqualsTrue(StringUtils::IsEqual("=", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    content = R"(["\u00a1"])";
    value   = JSON::Parse(content, 10);
    helper.EqualsTrue(StringUtils::IsEqual("¡", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    String<char> str(R"(["\u08A7"])");
    value = JSON::Parse(str.First(), str.Length());
    helper.EqualsTrue(StringUtils::IsEqual("ࢧ", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    str   = R"(["\ud802\uDE7B"])";
    value = JSON::Parse(str.First(), str.Length());
    helper.EqualsTrue(StringUtils::IsEqual("𐩻", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    str   = R"(["\uD83E\uFC59"])";
    value = JSON::Parse(str.First(), str.Length());
    helper.EqualsTrue(StringUtils::IsEqual("🡙", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    str   = R"(["\UD800\UDE83W\U003DW\UD800\UDE83\U00A1\UD83E\UFC59\U08A7"])";
    value = JSON::Parse(str.First(), str.Length());
    helper.EqualsTrue(StringUtils::IsEqual("𐊃W=W𐊃¡🡙ࢧ", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);
}

static void TestParse5(QTest &helper) {
    Value<char>        value;
    StringStream<char> stream;
    const char        *content;

    content = R"([1   ]    ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"a":1   }    })";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([f])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([t])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([n])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([falsE])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([truE])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([nulL])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([false-])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([true-])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([null-])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([1,])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([1,   ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([1   ,   ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([,])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["1",,"2"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([,1])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([,"1"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["1"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([["1"]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([  []]  ])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["w"],1])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["w" o  , 1])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"w": "a" o  , "b": "x"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["w" ] ,1])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["x"]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["aa"],"s"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["x":"s"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([{"x"::"s"}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x":})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([]")";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({}")";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({},)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([],)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([[[[[[[[[001]]]]]]]]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"a":{"b":{"c":001}}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([w])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({x:"x"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x":001})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x":4)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["x")";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([4,])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x" 4})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x" "4"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x" {}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x" []})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x",4})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([4,])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x":4,})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([4,,])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x":4,,})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x":"w" w})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x":"w" true})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["w" true])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["\w"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([""]")";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);
}

static void TestParse6(QTest &helper) {
    Value<char>        value;
    StringStream<char> stream;
    const char        *content;

    content = R"(["\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["""])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["a",)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"a":"c",)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse("[\"abc\n\"]");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["\"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({{}})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([123e])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);
    stream.Clear();

    content = R"([123e--4])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse("[\"\ta\"]");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["\uaaa"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["\uaa"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["\ua"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["\u"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["\uD83E\uFC5"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([[{} {}]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([{} {}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([[] []])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["w" "w"])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["w" {}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([4    {}])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({a "a":[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"a":a []})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"a"a :[]})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x":"w" "w"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x":"w" "u":"w"})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([{} 0])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([["x" 0]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([{} 1])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([[{} 0]])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([[] false])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["w" null])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"(["w" 3])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([4    4])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x":"w" 7})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"x":"w" 5})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({1)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([2)";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"a":h, "b": a})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"({"a":{})";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse("{\"\na\":{}");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse("[\"WHAT?\"}");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([1e])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([1e+])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([1e-])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([1e++])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    content = R"([1e--])";
    value   = JSON::Parse(stream, content, StringUtils::Count(content));
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);
}

static int RunJSONTests() {
    QTest helper{"JSON.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("Parse Test 1", TestParse1);
    helper.Test("Parse Test 2", TestParse2);
    helper.Test("Parse Test 3", TestParse3);
    helper.Test("Parse Test 4", TestParse4);
    helper.Test("Parse Test 5", TestParse5);
    helper.Test("Parse Test 6", TestParse6);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
