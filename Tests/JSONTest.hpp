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
#include "TestHelper.hpp"

#ifndef QENTEM_JSON_TESTS_H_
#define QENTEM_JSON_TESTS_H_

namespace Qentem {
namespace Test {

namespace JSON = Qentem::JSON;

static int TestParse1() {
    Value<char> value;

    ///////////
    value = JSON::Parse(R"([])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"([ ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"( [])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"( [ ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"([]    )");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"(     [])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"(     []    )");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"(    [])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"(    [ ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"([    ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"( [true])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([true])", "Stringify()");

    value = JSON::Parse(R"( [false ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([false])", "Stringify()");

    value = JSON::Parse(R"([    null])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null])", "Stringify()");

    value = JSON::Parse(R"([ 0 ]     )");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([0])", "Stringify()");

    value = JSON::Parse(R"([      "a"   ]          )");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a"])", "Stringify()");

    value = JSON::Parse(R"(        ["ABC"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC"])", "Stringify()");

    value = JSON::Parse(R"([[]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[]])", "Stringify()");

    value = JSON::Parse(R"([ [ ]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[]])", "Stringify()");

    value = JSON::Parse(R"([[          ]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[]])", "Stringify()");

    value = JSON::Parse(R"([         [          ]           ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[]])", "Stringify()");

    value = JSON::Parse(R"([{}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{}])", "Stringify()");

    value = JSON::Parse(R"([ {}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{}])", "Stringify()");

    value = JSON::Parse(R"([{        }])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{}])", "Stringify()");

    value = JSON::Parse(R"([ { } ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{}])", "Stringify()");

    value = JSON::Parse(R"([ true,true ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([true,true])", "Stringify()");

    value = JSON::Parse(R"([false, true])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([false,true])", "Stringify()");

    value = JSON::Parse(R"([null ,true])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null,true])", "Stringify()");

    value = JSON::Parse(R"([-3 , true])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([-3,true])", "Stringify()");

    value = JSON::Parse(R"(["a"       ,    true])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a",true])", "Stringify()");

    value = JSON::Parse(R"([   "ABC",true       ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",true])", "Stringify()");

    value = JSON::Parse(R"([       [] ,  true      ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[],true])", "Stringify()");

    value = JSON::Parse(R"([ {} , true ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{},true])", "Stringify()");

    value = JSON::Parse(R"([  true ,false])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([true,false])", "Stringify()");

    value = JSON::Parse(R"([false, false ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([false,false])", "Stringify()");

    value = JSON::Parse(R"([ null ,false])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null,false])", "Stringify()");

    value = JSON::Parse(R"([500,false])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([500,false])", "Stringify()");

    value = JSON::Parse(R"(["a",false])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a",false])", "Stringify()");

    value = JSON::Parse(R"([ "ABC" , false ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",false])", "Stringify()");

    value = JSON::Parse(R"([[]                      ,false])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[],false])", "Stringify()");

    value = JSON::Parse(R"([{},                     false])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{},false])", "Stringify()");

    value = JSON::Parse(R"([true,null])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([true,null])", "Stringify()");

    value = JSON::Parse(R"([false,null])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([false,null])", "Stringify()");

    value = JSON::Parse(R"([null,null])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null,null])", "Stringify()");

    value = JSON::Parse(R"([456.5,null])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([456.5,null])", "Stringify()");

    value = JSON::Parse(R"(["a",null])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a",null])", "Stringify()");

    value = JSON::Parse(R"(["ABC",null])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",null])", "Stringify()");

    value = JSON::Parse(R"([[],null])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[],null])", "Stringify()");

    value = JSON::Parse(R"([{},null])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{},null])", "Stringify()");

    value = JSON::Parse(R"([true,"A"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([true,"A"])", "Stringify()");

    value = JSON::Parse(R"([false,"AB"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([false,"AB"])", "Stringify()");

    value = JSON::Parse(R"([null,"ABC"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null,"ABC"])", "Stringify()");

    value = JSON::Parse(R"([-8.9,"ABC"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([-8.9,"ABC"])", "Stringify()");

    value = JSON::Parse(R"(["a","ABCD"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a","ABCD"])", "Stringify()");

    value = JSON::Parse(R"(["ABC","ABCDE"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC","ABCDE"])", "Stringify()");

    value = JSON::Parse(R"([[],"ABCDEF"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[],"ABCDEF"])", "Stringify()");

    value = JSON::Parse(R"([{},"ABCDEFG"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{},"ABCDEFG"])", "Stringify()");

    value = JSON::Parse(R"([true,[]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([true,[]])", "Stringify()");

    value = JSON::Parse(R"([false,[]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([false,[]])", "Stringify()");

    value = JSON::Parse(R"([null,[]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null,[]])", "Stringify()");

    value = JSON::Parse(R"([10000,[]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([10000,[]])", "Stringify()");

    value = JSON::Parse(R"(["ABC",[]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",[]])", "Stringify()");

    value = JSON::Parse(R"([[],[]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[],[]])", "Stringify()");

    value = JSON::Parse(R"([{},[]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{},[]])", "Stringify()");

    value = JSON::Parse(R"([true,{}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([true,{}])", "Stringify()");

    value = JSON::Parse(R"([false,{}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([false,{}])", "Stringify()");

    value = JSON::Parse(R"([null,{}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null,{}])", "Stringify()");

    value = JSON::Parse(R"([-1000,{}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([-1000,{}])", "Stringify()");

    value = JSON::Parse(R"(["ABC",{}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",{}])", "Stringify()");

    value = JSON::Parse(R"([[],{}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[],{}])", "Stringify()");

    value = JSON::Parse(R"([{},{}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{},{}])", "Stringify()");

    value = JSON::Parse(R"([true,{},false])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([true,{},false])", "Stringify()");

    value = JSON::Parse(R"([false,[],null])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([false,[],null])", "Stringify()");

    value = JSON::Parse(R"([null,{},"ABC"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null,{},"ABC"])", "Stringify()");

    value = JSON::Parse(R"([null,789,"ABC"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null,789,"ABC"])", "Stringify()");

    value = JSON::Parse(R"(["ABC",[],{}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",[],{}])", "Stringify()");

    value = JSON::Parse(R"([[],{},498])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[],{},498])", "Stringify()");

    value = JSON::Parse(R"([{},[],true])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{},[],true])", "Stringify()");

    value = JSON::Parse(R"([true,{},0,[]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([true,{},0,[]])", "Stringify()");

    value = JSON::Parse(R"([false,[],null,{}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([false,[],null,{}])",
                       "Stringify()");

    value = JSON::Parse(R"([null,{},"ABC",[]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null,{},"ABC",[]])",
                       "Stringify()");

    value = JSON::Parse(R"(["ABC",[],{},null])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",[],{},null])",
                       "Stringify()");

    value = JSON::Parse(R"([[],false,{},true])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[],false,{},true])",
                       "Stringify()");

    value = JSON::Parse(R"([{},null,[],[]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{},null,[],[]])", "Stringify()");

    value = JSON::Parse(R"([true,false,null,123,"ABC",[],{}])");
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"([true,false,null,123,"ABC",[],{}])", "Stringify()");

    value = JSON::Parse(R"([{},[],"a",1.5,null,false,true])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{},[],"a",1.5,null,false,true])",
                       "Stringify()");

    value = JSON::Parse(R"([["]"],["]"]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([["]"],["]"]])", "Stringify()");

    value = JSON::Parse(R"([["[]"],["[]"]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([["[]"],["[]"]])", "Stringify()");

    END_SUB_TEST;
}

static int TestParse2() {
    Value<char> value;

    value = JSON::Parse(R"({})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "Stringify()");

    value = JSON::Parse(R"( {} )");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "Stringify()");

    value = JSON::Parse(R"(  {}   )");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "Stringify()");

    value = JSON::Parse(R"({ })");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "Stringify()");

    value = JSON::Parse(R"({     })");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "Stringify()");

    value = JSON::Parse(R"( {"A":true})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":true})", "Stringify()");

    value = JSON::Parse(R"( { "B":false})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"B":false})", "Stringify()");

    value = JSON::Parse(R"( {"AA":null }            )");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"AA":null})", "Stringify()");

    value = JSON::Parse(R"( {"a" :0} )");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"a":0})", "Stringify()");

    value = JSON::Parse(R"(      {"abc": "a"}        )");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"abc":"a"})", "Stringify()");

    value = JSON::Parse(R"({"ab" : "ABC"} )");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"ab":"ABC"})", "Stringify()");

    value = JSON::Parse(R"({ "ABC" :[]}         )");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"ABC":[]})", "Stringify()");

    value = JSON::Parse(R"(      { "ABC" :[          ]})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"ABC":[]})", "Stringify()");

    value = JSON::Parse(R"({"5vn7b83y98t3wrupwmwa4ataw": {         } })");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"5vn7b83y98t3wrupwmwa4ataw":{}})",
                       "Stringify()");

    value = JSON::Parse(R"({      "A":true,"B":true})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":true,"B":true})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":false,"b":true          })");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":false,"b":true})",
                       "Stringify()");

    value = JSON::Parse(R"({"A"        :null,"BC":true})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":null,"BC":true})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":          -3,"AB":true})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":-3,"AB":true})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":"a"        ,"ABC":true})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"a","ABC":true})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":"ABC" ,"1":true})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"ABC","1":true})",
                       "Stringify()");

    value = JSON::Parse(R"({"X":[], "123":true})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":[],"123":true})",
                       "Stringify()");

    value = JSON::Parse(R"({"x":{},             "A":true})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"x":{},"A":true})",
                       "Stringify()");

    value = JSON::Parse(R"({     "A2"     :   true  ,   "A1"  :   false   })");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A2":true,"A1":false})",
                       "Stringify()");

    value = JSON::Parse(R"({"A-" : false,"A123" : false})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A-":false,"A123":false})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":null,             "B":false})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":null,"B":false})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":500,"B":false             })");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":500,"B":false})",
                       "Stringify()");

    value = JSON::Parse(R"({           "A":"a","B":false})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"a","B":false})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":          "ABC","B":false})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"ABC","B":false})",
                       "Stringify()");

    value = JSON::Parse(R"({"X":[] ,   "A":false})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":[],"A":false})",
                       "Stringify()");

    value = JSON::Parse(R"({"X":{},"A"          :false})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":{},"A":false})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":true,"W":null})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":true,"W":null})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":false,"@":           null})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":false,"@":null})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":null,"#":null})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":null,"#":null})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":456.5,"H":null})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":456.5,"H":null})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":"a","Q":null})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"a","Q":null})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":"ABC","e":null})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"ABC","e":null})",
                       "Stringify()");

    value = JSON::Parse(R"({"X":[],"n":null})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":[],"n":null})",
                       "Stringify()");

    value = JSON::Parse(R"({"t":{},"A":null})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"t":{},"A":null})",
                       "Stringify()");

    value = JSON::Parse(R"({"X":[          ],"n":null})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":[],"n":null})",
                       "Stringify()");

    value = JSON::Parse(R"({"t":{         },"A":null})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"t":{},"A":null})",
                       "Stringify()");

    value = JSON::Parse(R"({"e":true,"A":"A"})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"e":true,"A":"A"})",
                       "Stringify()");

    value = JSON::Parse(R"({"m":false,"A":"AB"})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"m":false,"A":"AB"})",
                       "Stringify()");

    value = JSON::Parse(R"({"i":null,"A":"ABC"})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"i":null,"A":"ABC"})",
                       "Stringify()");

    value = JSON::Parse(R"({"s":-8.9,"A":"ABC"})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"s":-8.9,"A":"ABC"})",
                       "Stringify()");

    value = JSON::Parse(R"({"-":"a","A":"ABCD"})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"-":"a","A":"ABCD"})",
                       "Stringify()");

    value = JSON::Parse(R"({"g":"ABC","A":"ABCDE"})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"g":"ABC","A":"ABCDE"})",
                       "Stringify()");

    value = JSON::Parse(R"({"o":[],"A":"ABCDEF"})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"o":[],"A":"ABCDEF"})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":{},"o":"ABCDEFG"})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":{},"o":"ABCDEFG"})",
                       "Stringify()");

    value = JSON::Parse(R"({"d":true,"y":[]})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"d":true,"y":[]})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":false,"y":[]})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":false,"y":[]})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":null,"y":[]})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":null,"y":[]})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":10000,"y":[]})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":10000,"y":[]})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":"ABC","y":[]})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"ABC","y":[]})",
                       "Stringify()");

    value = JSON::Parse(R"({"X":[],"y":[]})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":[],"y":[]})", "Stringify()");

    value = JSON::Parse(R"({"X":{},"Y":[]})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":{},"Y":[]})", "Stringify()");

    value = JSON::Parse(R"({"A":true,"y":{}})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":true,"y":{}})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":false,"y":{}})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":false,"y":{}})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":null,"y":{}})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":null,"y":{}})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":-1000,"y":{}})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":-1000,"y":{}})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":"ABC","y":{}})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"ABC","y":{}})",
                       "Stringify()");

    value = JSON::Parse(R"({"C":[],"R":{}})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"C":[],"R":{}})", "Stringify()");

    value = JSON::Parse(R"({"cc":{},"rr":{}})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"cc":{},"rr":{}})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":true,"y":{},"AA":false})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":true,"y":{},"AA":false})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":false,"y":[],"B":null})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":false,"y":[],"B":null})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":null,"y":{},"ABC":"ABC"})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":null,"y":{},"ABC":"ABC"})",
                       "Stringify()");

    value = JSON::Parse(R"({"t":null,"Y":789,"A":"ABC"})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"t":null,"Y":789,"A":"ABC"})",
                       "Stringify()");

    value = JSON::Parse(R"({"A":"ABC","y":[],"key-u":{}})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"ABC","y":[],"key-u":{}})",
                       "Stringify()");

    value = JSON::Parse(R"({"X":[],"Y":{},"key-u":498})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":[],"Y":{},"key-u":498})",
                       "Stringify()");

    value = JSON::Parse(R"({"X":{},"y":[],"A":true})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":{},"y":[],"A":true})",
                       "Stringify()");

    value = JSON::Parse(R"({"{}}":true,"y":{},"AA":0,"k-300":[]})");
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"{}}":true,"y":{},"AA":0,"k-300":[]})",
                       "Stringify()");

    value = JSON::Parse(R"({"B1":false,"y":[],"[A]":null,"k-300":{}})");
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"B1":false,"y":[],"[A]":null,"k-300":{}})",
                       "Stringify()");

    value = JSON::Parse(R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})");
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})",
                       "Stringify()");

    value = JSON::Parse(R"({"x":"ABC","[]]":[],"key-u":{},"A":null})");
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"x":"ABC","[]]":[],"key-u":{},"A":null})",
                       "Stringify()");

    value = JSON::Parse(R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})");
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})",
                       "Stringify()");

    value = JSON::Parse(R"({"X":{},"A":null,"key-u":[],"k-300":[]})");
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"X":{},"A":null,"key-u":[],"k-300":[]})",
                       "Stringify()");

    value = JSON::Parse(
        R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})");
    SHOULD_EQUAL_VALUE(
        value.Stringify(),
        R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})",
        "Stringify()");

    value = JSON::Parse(
        R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})");
    SHOULD_EQUAL_VALUE(
        value.Stringify(),
        R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})",
        "Stringify()");

    value = JSON::Parse(R"({"a":{"c":"}"},"b":{"d":"}"}})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"a":{"c":"}"},"b":{"d":"}"}})",
                       "Stringify()");

    value = JSON::Parse(R"({"a":{"c":"{}"},"b":{"d":"{}"}})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"a":{"c":"{}"},"b":{"d":"{}"}})",
                       "Stringify()");

    END_SUB_TEST;
}

static int TestParse3() {
    Value<char> value;

    value = JSON::Parse(R"([{"one":1}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{"one":1}])", "Stringify()");

    value = JSON::Parse(R"([{"one":"a"}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{"one":"a"}])", "Stringify()");

    value = JSON::Parse(R"(["a","b"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a","b"])", "Stringify()");

    value = JSON::Parse(R"([[1,2],[30]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[1,2],[30]])", "Stringify()");

    value = JSON::Parse(R"([[[1]]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[[1]]])", "Stringify()");

    value = JSON::Parse(R"([[123]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[123]])", "Stringify()");

    value = JSON::Parse(R"([[1,2]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[1,2]])", "Stringify()");

    value = JSON::Parse(R"([["a","b"]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([["a","b"]])", "Stringify()");

    value = JSON::Parse(R"([[1,2,3]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[1,2,3]])", "Stringify()");

    value = JSON::Parse(R"([[[1,2,3]]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[[1,2,3]]])", "Stringify()");

    value = JSON::Parse(R"([[[[1,2,3]]]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[[[1,2,3]]]])", "Stringify()");

    value = JSON::Parse(R"([123,"",456])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([123,"",456])", "Stringify()");

    value = JSON::Parse(R"([[1,"",2]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[1,"",2]])", "Stringify()");

    value = JSON::Parse(R"([[123,456,""]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[123,456,""]])", "Stringify()");

    value = JSON::Parse(R"({"a":[1],"b":1})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"a":[1],"b":1})", "Stringify()");

    value = JSON::Parse(R"([[],123,456])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[],123,456])", "Stringify()");

    value = JSON::Parse(R"([123,456,[]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([123,456,[]])", "Stringify()");

    value = JSON::Parse(R"([[[],1]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[[],1]])", "Stringify()");

    value = JSON::Parse(R"([[123,456,[]]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[123,456,[]]])", "Stringify()");

    value = JSON::Parse(R"([3,[4],5])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([3,[4],5])", "Stringify()");

    value = JSON::Parse(R"({})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "Stringify()");

    value = JSON::Parse(R"({"a":{}})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"a":{}})", "Stringify()");

    value = JSON::Parse(R"([[[{}]]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[[{}]]])", "Stringify()");

    value = JSON::Parse(R"([[[],[]]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[[],[]]])", "Stringify()");

    value = JSON::Parse(R"(["oo",[[],[],[{},[]]],"bk"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["oo",[[],[],[{},[]]],"bk"])",
                       "Stringify()");

    value = JSON::Parse(R"([{},[]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{},[]])", "Stringify()");

    value = JSON::Parse(R"(["aa",[[{}],["ww","ee"]],"bb"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["aa",[[{}],["ww","ee"]],"bb"])",
                       "Stringify()");

    value = JSON::Parse(R"({"aa":44,"dd":"bb"})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"aa":44,"dd":"bb"})",
                       "Stringify()");

    value = JSON::Parse(R"({"aa":[],"dd":"bb"})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"aa":[],"dd":"bb"})",
                       "Stringify()");

    value = JSON::Parse(R"([[],[[[]]]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[],[[[]]]])", "Stringify()");

    value = JSON::Parse(R"([[[["1","2","3"]]]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[[["1","2","3"]]]])",
                       "Stringify()");

    value = JSON::Parse(R"([{"ab":{}}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{"ab":{}}])", "Stringify()");

    value = JSON::Parse(R"([{"ab":{},"cd":""}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{"ab":{},"cd":""}])",
                       "Stringify()");

    value = JSON::Parse(R"([[[],[]]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[[],[]]])", "Stringify()");

    value = JSON::Parse(R"({"a":[],"c":[]})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"a":[],"c":[]})", "Stringify()");

    value = JSON::Parse(R"({"\"bb\"":"\""})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"\"bb\"":"\""})", "Stringify()");

    value = JSON::Parse(R"(["\"bb\"","\""])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["\"bb\"","\""])", "Stringify()");

    value = JSON::Parse(R"(["s\""])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["s\""])", "Stringify()");

    value = JSON::Parse(R"(["\\"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["\\"])", "Stringify()");

    value = JSON::Parse(R"(["a\\"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a\\"])", "Stringify()");

    value = JSON::Parse(R"(["\\a"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["\\a"])", "Stringify()");

    value = JSON::Parse(R"(["a\\a"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a\\a"])", "Stringify()");

    value = JSON::Parse(R"(["aaa\\"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["aaa\\"])", "Stringify()");

    value = JSON::Parse(R"(["\\aaa"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["\\aaa"])", "Stringify()");

    value = JSON::Parse(R"(["aaa\\aaa"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["aaa\\aaa"])", "Stringify()");

    value = JSON::Parse(R"(["c,"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["c,"])", "Stringify()");

    value = JSON::Parse(R"(["]][[][]"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["]][[][]"])", "Stringify()");

    value = JSON::Parse(R"(["k,","l"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["k,","l"])", "Stringify()");

    value = JSON::Parse(R"([[[1],50]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[[1],50]])", "Stringify()");

    value = JSON::Parse(R"([["[,]",5]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([["[,]",5]])", "Stringify()");

    value = JSON::Parse(R"([[["k,,,,,","l"]]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[["k,,,,,","l"]]])",
                       "Stringify()");

    value = JSON::Parse(R"(["k,","e","W","u"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["k,","e","W","u"])",
                       "Stringify()");

    value = JSON::Parse(R"([{"a":{}},[{},[1,2,3],["aa","cc"]]])");
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"([{"a":{}},[{},[1,2,3],["aa","cc"]]])", "Stringify()");

    value = JSON::Parse(R"(["s\"s\"s\"s\"s\"s\"s\"s\"s\"\\\\\\\\\\\\"])");
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"(["s\"s\"s\"s\"s\"s\"s\"s\"s\"\\\\\\\\\\\\"])",
                       "Stringify()");

    value = JSON::Parse(
        R"(["s\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""])");
    SHOULD_EQUAL_VALUE(
        value.Stringify(),
        R"(["s\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""])",
        "Stringify()");

    value = JSON::Parse(R"(["a\nb"])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a\nb"])", "Stringify()");

    value = JSON::Parse(R"([["[]{}","A"]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([["[]{}","A"]])", "Stringify()");

    value = JSON::Parse(R"([["[]{}",1]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([["[]{}",1]])", "Stringify()");

    value = JSON::Parse(R"([{"[]{}":"A"}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{"[]{}":"A"}])", "Stringify()");

    value = JSON::Parse(R"([{"[]{}":1}])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{"[]{}":1}])", "Stringify()");

    value = JSON::Parse(R"([[["1","2","3"],"c","d"],"a",["1"]])");
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"([[["1","2","3"],"c","d"],"a",["1"]])", "Stringify()");

    value = JSON::Parse(R"([[[1,2,3],40,50],4,[0.5],5])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[[1,2,3],40,50],4,[0.5],5])",
                       "Stringify()");

    value = JSON::Parse(R"([[[[],30]]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[[[],30]]])", "Stringify()");

    value = JSON::Parse(R"([[],[30]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[],[30]])", "Stringify()");

    value = JSON::Parse(R"([[1],[30]])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[1],[30]])", "Stringify()");

    value = JSON::Parse(R"({"a":{"b":"c"}})");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"a":{"b":"c"}})", "Stringify()");

    value = JSON::Parse(
        R"([                                                                 1,
                                                                                               2
                                                                                                          ])");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([1,2])", "Stringify()");

    END_SUB_TEST;
}

static int TestParse4() {
    Value<char>         value;
    const String<char> *key_ptr;
    const char *        str1;

    value = JSON::Parse(R"(["\r"])");
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual("\r", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    value = JSON::Parse(R"({"x":   "\n"})");
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual("\n", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    value   = JSON::Parse(R"({"\""   :"x"       })");
    key_ptr = value.GetKey(0);
    SHOULD_NOT_EQUAL(key_ptr, nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual("\"", key_ptr->First(), value[0].Length()),
        "IsEqual()");

    value   = JSON::Parse(R"({    "\n":   "\f"})");
    key_ptr = value.GetKey(0);
    SHOULD_NOT_EQUAL(key_ptr, nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual("\n", key_ptr->First(), value[0].Length()),
        "IsEqual()");

    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual("\f", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    value = JSON::Parse(R"(["\/\/"])");
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual("//", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    value = JSON::Parse(R"({"x":"\r\r"})");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("\r\r", value[0].StringStorage(),
                                           value[0].Length()),
                      "IsEqual()");

    value   = JSON::Parse(R"({"\b\b":"x"})");
    key_ptr = value.GetKey(0);
    SHOULD_NOT_EQUAL(key_ptr, nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual("\b\b", key_ptr->First(), value[0].Length()),
        "IsEqual()");

    value   = JSON::Parse(R"({"\\\\":"\"\""})");
    key_ptr = value.GetKey(0);
    SHOULD_NOT_EQUAL(key_ptr, nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual(R"(\\)", key_ptr->First(), value[0].Length()),
        "IsEqual()");

    SHOULD_EQUAL_TRUE(StringUtils::IsEqual(R"("")", value[0].StringStorage(),
                                           value[0].Length()),
                      "IsEqual()");

    value = JSON::Parse(R"(["\t\t\t"])");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("\t\t\t", value[0].StringStorage(),
                                           value[0].Length()),
                      "IsEqual()");

    value = JSON::Parse(R"({"x":"\f\f\f"})");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("\f\f\f", value[0].StringStorage(),
                                           value[0].Length()),
                      "IsEqual()");

    value   = JSON::Parse(R"({"\\\\\\":"x"})");
    key_ptr = value.GetKey(0);
    SHOULD_NOT_EQUAL(key_ptr, nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual(R"(\\\)", key_ptr->First(), value[0].Length()),
        "IsEqual()");

    value   = JSON::Parse(R"({"\/\/\/":"\n\n\n"})");
    key_ptr = value.GetKey(0);
    SHOULD_NOT_EQUAL(key_ptr, nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual("///", key_ptr->First(), value[0].Length()),
        "IsEqual()");

    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("\n\n\n", value[0].StringStorage(),
                                           value[0].Length()),
                      "IsEqual()");

    value = JSON::Parse(R"([" \""])");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual(" \"", value[0].StringStorage(),
                                           value[0].Length()),
                      "IsEqual()");

    value = JSON::Parse(R"([" \t "])");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual(" \t ", value[0].StringStorage(),
                                           value[0].Length()),
                      "IsEqual()");

    value = JSON::Parse(R"(["\\ \\"])");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("\\ \\", value[0].StringStorage(),
                                           value[0].Length()),
                      "IsEqual()");

    value = JSON::Parse(R"(["\"\\\/\b\f\n\r\t"])");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("\"\\/\b\f\n\r\t",
                                           value[0].StringStorage(),
                                           value[0].Length()),
                      "IsEqual()");

    value = JSON::Parse(R"({"x":"\t\r\n\f\b\/\\\""})");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("\t\r\n\f\b/\\\"",
                                           value[0].StringStorage(),
                                           value[0].Length()),
                      "IsEqual()");

    value   = JSON::Parse(R"({"\t\r\n\f\b\/\\\"":"\"\\\/\b\f\n\r\t"})");
    key_ptr = value.GetKey(0);
    SHOULD_NOT_EQUAL(key_ptr, nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("\t\r\n\f\b/\\\"", key_ptr->First(),
                                           value[0].Length()),
                      "IsEqual()");

    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("\"\\/\b\f\n\r\t",
                                           value[0].StringStorage(),
                                           value[0].Length()),
                      "IsEqual()");

    str1  = R"(["\u003D"])";
    value = JSON::Parse(str1);
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual("=", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    str1  = R"(["\u00a1"])";
    value = JSON::Parse(str1, 10);
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual("¡", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    String<char> str(R"(["\u08A7"])");
    value = JSON::Parse(str.First(), str.Length());
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual("ࢧ", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    str   = R"(["\ud802\uDE7B"])";
    value = JSON::Parse(str.First(), str.Length());
    SHOULD_EQUAL_TRUE(
        StringUtils::IsEqual("𐩻", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    str   = R"(["\uD83E\uFC59"])";
    value = JSON::Parse(str.First(), str.Length());
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("🡙", value[0].StringStorage(),
                                           value[0].Length()),
                      "IsEqual()");

    str   = R"(["\UD800\UDE83W\U003DW\UD800\UDE83\U00A1\UD83E\UFC59\U08A7"])";
    value = JSON::Parse(str.First(), str.Length());
    SHOULD_EQUAL_TRUE(StringUtils::IsEqual("𐊃W=W𐊃¡🡙ࢧ",
                                           value[0].StringStorage(),
                                           value[0].Length()),
                      "IsEqual()");

    END_SUB_TEST;
}

QENTEM_MAYBE_UNUSED_
static int TestParse5() {
    Value<char> value;

    value = JSON::Parse(R"([1   ]    ])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"a":1   }    })");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([f])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([t])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([n])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([falsE])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([truE])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([nulL])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([false-])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([true-])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([null-])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([1,])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([1,   ])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([1   ,   ])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([,])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["1",,"2"])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([,1])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([,"1"])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["1"]])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([["1"]]])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([  []]  ])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w"],1])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" o  , 1])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"w": "a" o  , "b": "x"})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" ] ,1])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["x"]])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([]])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["aa"],"s"])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["x":"s"])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([{"x"::"s"}])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([]")");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({}")");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({},)");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([],)");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([[[[[[[[[001]]]]]]]]])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"a":{"b":{"c":001}}})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([w])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({x:"x"})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":001})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":4)");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["x")");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([4,])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x" 4})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x" "4"})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x" {}})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x" []})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x",4})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":4,})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([4,,])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":4,,})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":"w" w})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":"w" true})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" true])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\w"])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([""]")");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\"])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["""])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["a",)");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"a":"c",)");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse("[\"abc\n\"]");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\"])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({{}})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({[]})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([123e])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([123e--4])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse("[\"\ta\"]");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\uaaa"])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\uaa"])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\ua"])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\u"])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\uD83E\uFC5"])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([[{} {}]])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([{} {}])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([[] []])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" "w"])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" {}])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([4    {}])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({a "a":[]})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"a":a []})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"a"a :[]})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":"w" "w"})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":"w" "u":"w"})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([{} 0])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([["x" 0]])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([{} 1])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([[{} 0]])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([[] false])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" null])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" 3])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([4    4])");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":"w" 7})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":"w" 5})");
    SHOULD_EQUAL_TRUE(value.IsUndefined(), "value.IsUndefined()");

    END_SUB_TEST;
}

static int RunJSONTests() {
    STARTING_TEST("JSON.hpp");

    START_TEST("Parse Test 1", TestParse1);
    START_TEST("Parse Test 2", TestParse2);
    START_TEST("Parse Test 3", TestParse3);
    START_TEST("Parse Test 4", TestParse4);
    START_TEST("Parse Test 5", TestParse5);

    END_TEST("JSON.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
