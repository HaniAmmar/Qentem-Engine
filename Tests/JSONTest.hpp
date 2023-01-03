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

#ifndef QENTEM_JSON_TESTS_H_
#define QENTEM_JSON_TESTS_H_

namespace Qentem {
namespace Test {

namespace JSON = Qentem::JSON;

static void TestParse1(TestHelper &helper) {
    Value<char> value;

    ///////////
    value = JSON::Parse(R"([])");
    helper.Equal(value.Stringify(), R"([])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([ ])");
    helper.Equal(value.Stringify(), R"([])", "Stringify()", __LINE__);

    value = JSON::Parse(R"( [])");
    helper.Equal(value.Stringify(), R"([])", "Stringify()", __LINE__);

    value = JSON::Parse(R"( [ ])");
    helper.Equal(value.Stringify(), R"([])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([]    )");
    helper.Equal(value.Stringify(), R"([])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(     [])");
    helper.Equal(value.Stringify(), R"([])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(     []    )");
    helper.Equal(value.Stringify(), R"([])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(    [])");
    helper.Equal(value.Stringify(), R"([])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(    [ ])");
    helper.Equal(value.Stringify(), R"([])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([    ])");
    helper.Equal(value.Stringify(), R"([])", "Stringify()", __LINE__);

    value = JSON::Parse(R"( [true])");
    helper.Equal(value.Stringify(), R"([true])", "Stringify()", __LINE__);

    value = JSON::Parse(R"( [false ])");
    helper.Equal(value.Stringify(), R"([false])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([    null])");
    helper.Equal(value.Stringify(), R"([null])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([ 0 ]     )");
    helper.Equal(value.Stringify(), R"([0])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([      "a"   ]          )");
    helper.Equal(value.Stringify(), R"(["a"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(        ["ABC"])");
    helper.Equal(value.Stringify(), R"(["ABC"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[]])");
    helper.Equal(value.Stringify(), R"([[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([ [ ]])");
    helper.Equal(value.Stringify(), R"([[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[          ]])");
    helper.Equal(value.Stringify(), R"([[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([         [          ]           ])");
    helper.Equal(value.Stringify(), R"([[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{}])");
    helper.Equal(value.Stringify(), R"([{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([ {}])");
    helper.Equal(value.Stringify(), R"([{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{        }])");
    helper.Equal(value.Stringify(), R"([{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([ { } ])");
    helper.Equal(value.Stringify(), R"([{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([ true,true ])");
    helper.Equal(value.Stringify(), R"([true,true])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([false, true])");
    helper.Equal(value.Stringify(), R"([false,true])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([null ,true])");
    helper.Equal(value.Stringify(), R"([null,true])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([-3 , true])");
    helper.Equal(value.Stringify(), R"([-3,true])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["a"       ,    true])");
    helper.Equal(value.Stringify(), R"(["a",true])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([   "ABC",true       ])");
    helper.Equal(value.Stringify(), R"(["ABC",true])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([       [] ,  true      ])");
    helper.Equal(value.Stringify(), R"([[],true])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([ {} , true ])");
    helper.Equal(value.Stringify(), R"([{},true])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([  true ,false])");
    helper.Equal(value.Stringify(), R"([true,false])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([false, false ])");
    helper.Equal(value.Stringify(), R"([false,false])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([ null ,false])");
    helper.Equal(value.Stringify(), R"([null,false])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([500,false])");
    helper.Equal(value.Stringify(), R"([500,false])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["a",false])");
    helper.Equal(value.Stringify(), R"(["a",false])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([ "ABC" , false ])");
    helper.Equal(value.Stringify(), R"(["ABC",false])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[]                      ,false])");
    helper.Equal(value.Stringify(), R"([[],false])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{},                     false])");
    helper.Equal(value.Stringify(), R"([{},false])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([true,null])");
    helper.Equal(value.Stringify(), R"([true,null])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([false,null])");
    helper.Equal(value.Stringify(), R"([false,null])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([null,null])");
    helper.Equal(value.Stringify(), R"([null,null])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([456.5,null])");
    helper.Equal(value.Stringify(), R"([456.5,null])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["a",null])");
    helper.Equal(value.Stringify(), R"(["a",null])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["ABC",null])");
    helper.Equal(value.Stringify(), R"(["ABC",null])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[],null])");
    helper.Equal(value.Stringify(), R"([[],null])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{},null])");
    helper.Equal(value.Stringify(), R"([{},null])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([true,"A"])");
    helper.Equal(value.Stringify(), R"([true,"A"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([false,"AB"])");
    helper.Equal(value.Stringify(), R"([false,"AB"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([null,"ABC"])");
    helper.Equal(value.Stringify(), R"([null,"ABC"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([-8.9,"ABC"])");
    helper.Equal(value.Stringify(), R"([-8.9,"ABC"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["a","ABCD"])");
    helper.Equal(value.Stringify(), R"(["a","ABCD"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["ABC","ABCDE"])");
    helper.Equal(value.Stringify(), R"(["ABC","ABCDE"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[],"ABCDEF"])");
    helper.Equal(value.Stringify(), R"([[],"ABCDEF"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{},"ABCDEFG"])");
    helper.Equal(value.Stringify(), R"([{},"ABCDEFG"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([true,[]])");
    helper.Equal(value.Stringify(), R"([true,[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([false,[]])");
    helper.Equal(value.Stringify(), R"([false,[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([null,[]])");
    helper.Equal(value.Stringify(), R"([null,[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([10000,[]])");
    helper.Equal(value.Stringify(), R"([10000,[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["ABC",[]])");
    helper.Equal(value.Stringify(), R"(["ABC",[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[],[]])");
    helper.Equal(value.Stringify(), R"([[],[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{},[]])");
    helper.Equal(value.Stringify(), R"([{},[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([true,{}])");
    helper.Equal(value.Stringify(), R"([true,{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([false,{}])");
    helper.Equal(value.Stringify(), R"([false,{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([null,{}])");
    helper.Equal(value.Stringify(), R"([null,{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([-1000,{}])");
    helper.Equal(value.Stringify(), R"([-1000,{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["ABC",{}])");
    helper.Equal(value.Stringify(), R"(["ABC",{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[],{}])");
    helper.Equal(value.Stringify(), R"([[],{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{},{}])");
    helper.Equal(value.Stringify(), R"([{},{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([true,{},false])");
    helper.Equal(value.Stringify(), R"([true,{},false])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([false,[],null])");
    helper.Equal(value.Stringify(), R"([false,[],null])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([null,{},"ABC"])");
    helper.Equal(value.Stringify(), R"([null,{},"ABC"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([null,789,"ABC"])");
    helper.Equal(value.Stringify(), R"([null,789,"ABC"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["ABC",[],{}])");
    helper.Equal(value.Stringify(), R"(["ABC",[],{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[],{},498])");
    helper.Equal(value.Stringify(), R"([[],{},498])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{},[],true])");
    helper.Equal(value.Stringify(), R"([{},[],true])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([true,{},0,[]])");
    helper.Equal(value.Stringify(), R"([true,{},0,[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([false,[],null,{}])");
    helper.Equal(value.Stringify(), R"([false,[],null,{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([null,{},"ABC",[]])");
    helper.Equal(value.Stringify(), R"([null,{},"ABC",[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["ABC",[],{},null])");
    helper.Equal(value.Stringify(), R"(["ABC",[],{},null])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[],false,{},true])");
    helper.Equal(value.Stringify(), R"([[],false,{},true])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{},null,[],[]])");
    helper.Equal(value.Stringify(), R"([{},null,[],[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([true,false,null,123,"ABC",[],{}])");
    helper.Equal(value.Stringify(), R"([true,false,null,123,"ABC",[],{}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{},[],"a",1.5,null,false,true])");
    helper.Equal(value.Stringify(), R"([{},[],"a",1.5,null,false,true])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([["]"],["]"]])");
    helper.Equal(value.Stringify(), R"([["]"],["]"]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([["[]"],["[]"]])");
    helper.Equal(value.Stringify(), R"([["[]"],["[]"]])", "Stringify()", __LINE__);
}

static void TestParse2(TestHelper &helper) {
    Value<char> value;

    value = JSON::Parse(R"({})");
    helper.Equal(value.Stringify(), R"({})", "Stringify()", __LINE__);

    value = JSON::Parse(R"( {} )");
    helper.Equal(value.Stringify(), R"({})", "Stringify()", __LINE__);

    value = JSON::Parse(R"(  {}   )");
    helper.Equal(value.Stringify(), R"({})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({ })");
    helper.Equal(value.Stringify(), R"({})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({     })");
    helper.Equal(value.Stringify(), R"({})", "Stringify()", __LINE__);

    value = JSON::Parse(R"( {"A":true})");
    helper.Equal(value.Stringify(), R"({"A":true})", "Stringify()", __LINE__);

    value = JSON::Parse(R"( { "B":false})");
    helper.Equal(value.Stringify(), R"({"B":false})", "Stringify()", __LINE__);

    value = JSON::Parse(R"( {"AA":null }            )");
    helper.Equal(value.Stringify(), R"({"AA":null})", "Stringify()", __LINE__);

    value = JSON::Parse(R"( {"a" :0} )");
    helper.Equal(value.Stringify(), R"({"a":0})", "Stringify()", __LINE__);

    value = JSON::Parse(R"(      {"abc": "a"}        )");
    helper.Equal(value.Stringify(), R"({"abc":"a"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"ab" : "ABC"} )");
    helper.Equal(value.Stringify(), R"({"ab":"ABC"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({ "ABC" :[]}         )");
    helper.Equal(value.Stringify(), R"({"ABC":[]})", "Stringify()", __LINE__);

    value = JSON::Parse(R"(      { "ABC" :[          ]})");
    helper.Equal(value.Stringify(), R"({"ABC":[]})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"5vn7b83y98t3wrupwmwa4ataw": {         } })");
    helper.Equal(value.Stringify(), R"({"5vn7b83y98t3wrupwmwa4ataw":{}})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({      "A":true,"B":true})");
    helper.Equal(value.Stringify(), R"({"A":true,"B":true})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":false,"b":true          })");
    helper.Equal(value.Stringify(), R"({"A":false,"b":true})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A"        :null,"BC":true})");
    helper.Equal(value.Stringify(), R"({"A":null,"BC":true})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":          -3,"AB":true})");
    helper.Equal(value.Stringify(), R"({"A":-3,"AB":true})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":"a"        ,"ABC":true})");
    helper.Equal(value.Stringify(), R"({"A":"a","ABC":true})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":"ABC" ,"1":true})");
    helper.Equal(value.Stringify(), R"({"A":"ABC","1":true})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"X":[], "123":true})");
    helper.Equal(value.Stringify(), R"({"X":[],"123":true})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"x":{},             "A":true})");
    helper.Equal(value.Stringify(), R"({"x":{},"A":true})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({     "A2"     :   true  ,   "A1"  :   false   })");
    helper.Equal(value.Stringify(), R"({"A2":true,"A1":false})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A-" : false,"A123" : false})");
    helper.Equal(value.Stringify(), R"({"A-":false,"A123":false})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":null,             "B":false})");
    helper.Equal(value.Stringify(), R"({"A":null,"B":false})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":500,"B":false             })");
    helper.Equal(value.Stringify(), R"({"A":500,"B":false})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({           "A":"a","B":false})");
    helper.Equal(value.Stringify(), R"({"A":"a","B":false})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":          "ABC","B":false})");
    helper.Equal(value.Stringify(), R"({"A":"ABC","B":false})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"X":[] ,   "A":false})");
    helper.Equal(value.Stringify(), R"({"X":[],"A":false})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"X":{},"A"          :false})");
    helper.Equal(value.Stringify(), R"({"X":{},"A":false})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":true,"W":null})");
    helper.Equal(value.Stringify(), R"({"A":true,"W":null})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":false,"@":           null})");
    helper.Equal(value.Stringify(), R"({"A":false,"@":null})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":null,"#":null})");
    helper.Equal(value.Stringify(), R"({"A":null,"#":null})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":456.5,"H":null})");
    helper.Equal(value.Stringify(), R"({"A":456.5,"H":null})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":"a","Q":null})");
    helper.Equal(value.Stringify(), R"({"A":"a","Q":null})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":"ABC","e":null})");
    helper.Equal(value.Stringify(), R"({"A":"ABC","e":null})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"X":[],"n":null})");
    helper.Equal(value.Stringify(), R"({"X":[],"n":null})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"t":{},"A":null})");
    helper.Equal(value.Stringify(), R"({"t":{},"A":null})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"X":[          ],"n":null})");
    helper.Equal(value.Stringify(), R"({"X":[],"n":null})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"t":{         },"A":null})");
    helper.Equal(value.Stringify(), R"({"t":{},"A":null})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"e":true,"A":"A"})");
    helper.Equal(value.Stringify(), R"({"e":true,"A":"A"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"m":false,"A":"AB"})");
    helper.Equal(value.Stringify(), R"({"m":false,"A":"AB"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"i":null,"A":"ABC"})");
    helper.Equal(value.Stringify(), R"({"i":null,"A":"ABC"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"s":-8.9,"A":"ABC"})");
    helper.Equal(value.Stringify(), R"({"s":-8.9,"A":"ABC"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"-":"a","A":"ABCD"})");
    helper.Equal(value.Stringify(), R"({"-":"a","A":"ABCD"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"g":"ABC","A":"ABCDE"})");
    helper.Equal(value.Stringify(), R"({"g":"ABC","A":"ABCDE"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"o":[],"A":"ABCDEF"})");
    helper.Equal(value.Stringify(), R"({"o":[],"A":"ABCDEF"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":{},"o":"ABCDEFG"})");
    helper.Equal(value.Stringify(), R"({"A":{},"o":"ABCDEFG"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"d":true,"y":[]})");
    helper.Equal(value.Stringify(), R"({"d":true,"y":[]})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":false,"y":[]})");
    helper.Equal(value.Stringify(), R"({"A":false,"y":[]})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":null,"y":[]})");
    helper.Equal(value.Stringify(), R"({"A":null,"y":[]})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":10000,"y":[]})");
    helper.Equal(value.Stringify(), R"({"A":10000,"y":[]})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":"ABC","y":[]})");
    helper.Equal(value.Stringify(), R"({"A":"ABC","y":[]})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"X":[],"y":[]})");
    helper.Equal(value.Stringify(), R"({"X":[],"y":[]})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"X":{},"Y":[]})");
    helper.Equal(value.Stringify(), R"({"X":{},"Y":[]})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":true,"y":{}})");
    helper.Equal(value.Stringify(), R"({"A":true,"y":{}})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":false,"y":{}})");
    helper.Equal(value.Stringify(), R"({"A":false,"y":{}})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":null,"y":{}})");
    helper.Equal(value.Stringify(), R"({"A":null,"y":{}})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":-1000,"y":{}})");
    helper.Equal(value.Stringify(), R"({"A":-1000,"y":{}})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":"ABC","y":{}})");
    helper.Equal(value.Stringify(), R"({"A":"ABC","y":{}})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"C":[],"R":{}})");
    helper.Equal(value.Stringify(), R"({"C":[],"R":{}})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"cc":{},"rr":{}})");
    helper.Equal(value.Stringify(), R"({"cc":{},"rr":{}})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":true,"y":{},"AA":false})");
    helper.Equal(value.Stringify(), R"({"A":true,"y":{},"AA":false})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":false,"y":[],"B":null})");
    helper.Equal(value.Stringify(), R"({"A":false,"y":[],"B":null})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":null,"y":{},"ABC":"ABC"})");
    helper.Equal(value.Stringify(), R"({"A":null,"y":{},"ABC":"ABC"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"t":null,"Y":789,"A":"ABC"})");
    helper.Equal(value.Stringify(), R"({"t":null,"Y":789,"A":"ABC"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":"ABC","y":[],"key-u":{}})");
    helper.Equal(value.Stringify(), R"({"A":"ABC","y":[],"key-u":{}})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"X":[],"Y":{},"key-u":498})");
    helper.Equal(value.Stringify(), R"({"X":[],"Y":{},"key-u":498})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"X":{},"y":[],"A":true})");
    helper.Equal(value.Stringify(), R"({"X":{},"y":[],"A":true})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"{}}":true,"y":{},"AA":0,"k-300":[]})");
    helper.Equal(value.Stringify(), R"({"{}}":true,"y":{},"AA":0,"k-300":[]})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"B1":false,"y":[],"[A]":null,"k-300":{}})");
    helper.Equal(value.Stringify(), R"({"B1":false,"y":[],"[A]":null,"k-300":{}})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})");
    helper.Equal(value.Stringify(), R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"x":"ABC","[]]":[],"key-u":{},"A":null})");
    helper.Equal(value.Stringify(), R"({"x":"ABC","[]]":[],"key-u":{},"A":null})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})");
    helper.Equal(value.Stringify(), R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"X":{},"A":null,"key-u":[],"k-300":[]})");
    helper.Equal(value.Stringify(), R"({"X":{},"A":null,"key-u":[],"k-300":[]})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})");
    helper.Equal(value.Stringify(), R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})", "Stringify()",
                 __LINE__);

    value = JSON::Parse(R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})");
    helper.Equal(value.Stringify(),
                 R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})", "Stringify()",
                 __LINE__);

    value = JSON::Parse(R"({"a":{"c":"}"},"b":{"d":"}"}})");
    helper.Equal(value.Stringify(), R"({"a":{"c":"}"},"b":{"d":"}"}})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"a":{"c":"{}"},"b":{"d":"{}"}})");
    helper.Equal(value.Stringify(), R"({"a":{"c":"{}"},"b":{"d":"{}"}})", "Stringify()", __LINE__);
}

static void TestParse3(TestHelper &helper) {
    Value<char> value;

    value = JSON::Parse(R"([{"one":1}])");
    helper.Equal(value.Stringify(), R"([{"one":1}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{"one":"a"}])");
    helper.Equal(value.Stringify(), R"([{"one":"a"}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["a","b"])");
    helper.Equal(value.Stringify(), R"(["a","b"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[1,2],[30]])");
    helper.Equal(value.Stringify(), R"([[1,2],[30]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[[1]]])");
    helper.Equal(value.Stringify(), R"([[[1]]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[123]])");
    helper.Equal(value.Stringify(), R"([[123]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[1,2]])");
    helper.Equal(value.Stringify(), R"([[1,2]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([["a","b"]])");
    helper.Equal(value.Stringify(), R"([["a","b"]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[1,2,3]])");
    helper.Equal(value.Stringify(), R"([[1,2,3]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[[1,2,3]]])");
    helper.Equal(value.Stringify(), R"([[[1,2,3]]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[[[1,2,3]]]])");
    helper.Equal(value.Stringify(), R"([[[[1,2,3]]]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([123,"",456])");
    helper.Equal(value.Stringify(), R"([123,"",456])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[1,"",2]])");
    helper.Equal(value.Stringify(), R"([[1,"",2]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[123,456,""]])");
    helper.Equal(value.Stringify(), R"([[123,456,""]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"a":[1],"b":1})");
    helper.Equal(value.Stringify(), R"({"a":[1],"b":1})", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[],123,456])");
    helper.Equal(value.Stringify(), R"([[],123,456])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([123,456,[]])");
    helper.Equal(value.Stringify(), R"([123,456,[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[[],1]])");
    helper.Equal(value.Stringify(), R"([[[],1]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[123,456,[]]])");
    helper.Equal(value.Stringify(), R"([[123,456,[]]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([3,[4],5])");
    helper.Equal(value.Stringify(), R"([3,[4],5])", "Stringify()", __LINE__);

    value = JSON::Parse(R"({})");
    helper.Equal(value.Stringify(), R"({})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"a":{}})");
    helper.Equal(value.Stringify(), R"({"a":{}})", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[[{}]]])");
    helper.Equal(value.Stringify(), R"([[[{}]]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[[],[]]])");
    helper.Equal(value.Stringify(), R"([[[],[]]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["oo",[[],[],[{},[]]],"bk"])");
    helper.Equal(value.Stringify(), R"(["oo",[[],[],[{},[]]],"bk"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{},[]])");
    helper.Equal(value.Stringify(), R"([{},[]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["aa",[[{}],["ww","ee"]],"bb"])");
    helper.Equal(value.Stringify(), R"(["aa",[[{}],["ww","ee"]],"bb"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"aa":44,"dd":"bb"})");
    helper.Equal(value.Stringify(), R"({"aa":44,"dd":"bb"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"aa":[],"dd":"bb"})");
    helper.Equal(value.Stringify(), R"({"aa":[],"dd":"bb"})", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[],[[[]]]])");
    helper.Equal(value.Stringify(), R"([[],[[[]]]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[[["1","2","3"]]]])");
    helper.Equal(value.Stringify(), R"([[[["1","2","3"]]]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{"ab":{}}])");
    helper.Equal(value.Stringify(), R"([{"ab":{}}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{"ab":{},"cd":""}])");
    helper.Equal(value.Stringify(), R"([{"ab":{},"cd":""}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[[],[]]])");
    helper.Equal(value.Stringify(), R"([[[],[]]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"a":[],"c":[]})");
    helper.Equal(value.Stringify(), R"({"a":[],"c":[]})", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"\"bb\"":"\""})");
    helper.Equal(value.Stringify(), R"({"\"bb\"":"\""})", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["\"bb\"","\""])");
    helper.Equal(value.Stringify(), R"(["\"bb\"","\""])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["s\""])");
    helper.Equal(value.Stringify(), R"(["s\""])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["\\"])");
    helper.Equal(value.Stringify(), R"(["\\"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["a\\"])");
    helper.Equal(value.Stringify(), R"(["a\\"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["\\a"])");
    helper.Equal(value.Stringify(), R"(["\\a"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["a\\a"])");
    helper.Equal(value.Stringify(), R"(["a\\a"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["aaa\\"])");
    helper.Equal(value.Stringify(), R"(["aaa\\"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["\\aaa"])");
    helper.Equal(value.Stringify(), R"(["\\aaa"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["aaa\\aaa"])");
    helper.Equal(value.Stringify(), R"(["aaa\\aaa"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["c,"])");
    helper.Equal(value.Stringify(), R"(["c,"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["]][[][]"])");
    helper.Equal(value.Stringify(), R"(["]][[][]"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["k,","l"])");
    helper.Equal(value.Stringify(), R"(["k,","l"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[[1],50]])");
    helper.Equal(value.Stringify(), R"([[[1],50]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([["[,]",5]])");
    helper.Equal(value.Stringify(), R"([["[,]",5]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[["k,,,,,","l"]]])");
    helper.Equal(value.Stringify(), R"([[["k,,,,,","l"]]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["k,","e","W","u"])");
    helper.Equal(value.Stringify(), R"(["k,","e","W","u"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{"a":{}},[{},[1,2,3],["aa","cc"]]])");
    helper.Equal(value.Stringify(), R"([{"a":{}},[{},[1,2,3],["aa","cc"]]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"(["s\"s\"s\"s\"s\"s\"s\"s\"s\"\\\\\\\\\\\\"])");
    helper.Equal(value.Stringify(), R"(["s\"s\"s\"s\"s\"s\"s\"s\"s\"\\\\\\\\\\\\"])", "Stringify()", __LINE__);

    value = JSON::Parse(
        R"(["s\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""])");
    helper.Equal(
        value.Stringify(),
        R"(["s\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""])",
        "Stringify()", __LINE__);

    value = JSON::Parse(R"(["a\nb"])");
    helper.Equal(value.Stringify(), R"(["a\nb"])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([["[]{}","A"]])");
    helper.Equal(value.Stringify(), R"([["[]{}","A"]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([["[]{}",1]])");
    helper.Equal(value.Stringify(), R"([["[]{}",1]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{"[]{}":"A"}])");
    helper.Equal(value.Stringify(), R"([{"[]{}":"A"}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([{"[]{}":1}])");
    helper.Equal(value.Stringify(), R"([{"[]{}":1}])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[["1","2","3"],"c","d"],"a",["1"]])");
    helper.Equal(value.Stringify(), R"([[["1","2","3"],"c","d"],"a",["1"]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[[1,2,3],40,50],4,[0.5],5])");
    helper.Equal(value.Stringify(), R"([[[1,2,3],40,50],4,[0.5],5])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[[[],30]]])");
    helper.Equal(value.Stringify(), R"([[[[],30]]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[],[30]])");
    helper.Equal(value.Stringify(), R"([[],[30]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"([[1],[30]])");
    helper.Equal(value.Stringify(), R"([[1],[30]])", "Stringify()", __LINE__);

    value = JSON::Parse(R"({"a":{"b":"c"}})");
    helper.Equal(value.Stringify(), R"({"a":{"b":"c"}})", "Stringify()", __LINE__);

    value = JSON::Parse(
        R"([                                                                 1,
                                                                                               2
                                                                                                          ])");
    helper.Equal(value.Stringify(), R"([1,2])", "Stringify()", __LINE__);
}

static void TestParse4(TestHelper &helper) {
    Value<char>         value;
    const String<char> *key_ptr;
    const char         *str1;

    value = JSON::Parse(R"(["\r"])");
    helper.EqualsTrue(StringUtils::IsEqual("\r", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    value = JSON::Parse(R"({"x":   "\n"})");
    helper.EqualsTrue(StringUtils::IsEqual("\n", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    value   = JSON::Parse(R"({"\""   :"x"       })");
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("\"", key_ptr->First(), value[0].Length()), "IsEqual()", __LINE__);

    value   = JSON::Parse(R"({    "\n":   "\f"})");
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("\n", key_ptr->First(), value[0].Length()), "IsEqual()", __LINE__);

    helper.EqualsTrue(StringUtils::IsEqual("\f", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    value = JSON::Parse(R"(["\/\/"])");
    helper.EqualsTrue(StringUtils::IsEqual("//", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    value = JSON::Parse(R"({"x":"\r\r"})");
    helper.EqualsTrue(StringUtils::IsEqual("\r\r", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    value   = JSON::Parse(R"({"\b\b":"x"})");
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("\b\b", key_ptr->First(), value[0].Length()), "IsEqual()", __LINE__);

    value   = JSON::Parse(R"({"\\\\":"\"\""})");
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(R"(\\)", key_ptr->First(), value[0].Length()), "IsEqual()", __LINE__);

    helper.EqualsTrue(StringUtils::IsEqual(R"("")", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    value = JSON::Parse(R"(["\t\t\t"])");
    helper.EqualsTrue(StringUtils::IsEqual("\t\t\t", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    value = JSON::Parse(R"({"x":"\f\f\f"})");
    helper.EqualsTrue(StringUtils::IsEqual("\f\f\f", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    value   = JSON::Parse(R"({"\\\\\\":"x"})");
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(R"(\\\)", key_ptr->First(), value[0].Length()), "IsEqual()", __LINE__);

    value   = JSON::Parse(R"({"\/\/\/":"\n\n\n"})");
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("///", key_ptr->First(), value[0].Length()), "IsEqual()", __LINE__);

    helper.EqualsTrue(StringUtils::IsEqual("\n\n\n", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    value = JSON::Parse(R"([" \""])");
    helper.EqualsTrue(StringUtils::IsEqual(" \"", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    value = JSON::Parse(R"([" \t "])");
    helper.EqualsTrue(StringUtils::IsEqual(" \t ", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    value = JSON::Parse(R"(["\\ \\"])");
    helper.EqualsTrue(StringUtils::IsEqual("\\ \\", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    value = JSON::Parse(R"(["\"\\\/\b\f\n\r\t"])");
    helper.EqualsTrue(StringUtils::IsEqual("\"\\/\b\f\n\r\t", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    value = JSON::Parse(R"({"x":"\t\r\n\f\b\/\\\""})");
    helper.EqualsTrue(StringUtils::IsEqual("\t\r\n\f\b/\\\"", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    value   = JSON::Parse(R"({"\t\r\n\f\b\/\\\"":"\"\\\/\b\f\n\r\t"})");
    key_ptr = value.GetKey(0);
    helper.NotEqual(key_ptr, nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("\t\r\n\f\b/\\\"", key_ptr->First(), value[0].Length()), "IsEqual()",
                      __LINE__);

    helper.EqualsTrue(StringUtils::IsEqual("\"\\/\b\f\n\r\t", value[0].StringStorage(), value[0].Length()), "IsEqual()",
                      __LINE__);

    str1  = R"(["\u003D"])";
    value = JSON::Parse(str1);
    helper.EqualsTrue(StringUtils::IsEqual("=", value[0].StringStorage(), value[0].Length()), "IsEqual()", __LINE__);

    str1  = R"(["\u00a1"])";
    value = JSON::Parse(str1, 10);
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
    helper.EqualsTrue(StringUtils::IsEqual("𐊃W=W𐊃¡🡙ࢧ", value[0].StringStorage(), value[0].Length()),
                      "IsEqual()", __LINE__);
}

static void TestParse5(TestHelper &helper) {
    Value<char> value;

    value = JSON::Parse(R"([1   ]    ])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"a":1   }    })");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([f])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([t])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([n])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([falsE])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([truE])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([nulL])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([false-])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([true-])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([null-])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([1,])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([1,   ])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([1   ,   ])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([,])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["1",,"2"])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([,1])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([,"1"])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["1"]])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([["1"]]])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([  []]  ])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["w"],1])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["w" o  , 1])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"w": "a" o  , "b": "x"})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["w" ] ,1])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["x"]])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([]])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["aa"],"s"])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["x":"s"])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([{"x"::"s"}])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x":})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([]")");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({}")");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({},)");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([],)");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([[[[[[[[[001]]]]]]]]])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"a":{"b":{"c":001}}})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([w])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({x:"x"})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x":001})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x":4)");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["x")");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([4,])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x" 4})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x" "4"})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x" {}})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x" []})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x",4})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([4,])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x":4,})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([4,,])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x":4,,})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x":"w" w})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x":"w" true})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["w" true])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["\w"])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([""]")");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);
}

static void TestParse6(TestHelper &helper) {
    Value<char> value;

    value = JSON::Parse(R"(["\"])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["""])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["a",)");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"a":"c",)");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse("[\"abc\n\"]");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["\"])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({{}})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({[]})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([123e])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([123e--4])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse("[\"\ta\"]");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["\uaaa"])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["\uaa"])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["\ua"])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["\u"])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["\uD83E\uFC5"])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([[{} {}]])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([{} {}])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([[] []])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["w" "w"])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["w" {}])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([4    {}])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({a "a":[]})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"a":a []})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"a"a :[]})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x":"w" "w"})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x":"w" "u":"w"})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([{} 0])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([["x" 0]])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([{} 1])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([[{} 0]])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([[] false])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["w" null])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"(["w" 3])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([4    4])");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x":"w" 7})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"x":"w" 5})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({1)");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"([2)");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"a":h, "b": a})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse(R"({"a":{})");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse("{\"\na\":{}");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);

    value = JSON::Parse("[\"WHAT?\"}");
    helper.EqualsTrue(value.IsUndefined(), "value.IsUndefined()", __LINE__);
}

static int RunJSONTests() {
    TestHelper helper{"JSON.hpp", __FILE__};

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
