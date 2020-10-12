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
    EQ_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"([ ])");
    EQ_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"( [])");
    EQ_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"( [ ])");
    EQ_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"([]    )");
    EQ_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"(     [])");
    EQ_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"(     []    )");
    EQ_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"(    [])");
    EQ_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"(    [ ])");
    EQ_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"([    ])");
    EQ_VALUE(value.Stringify(), R"([])", "Stringify()");

    value = JSON::Parse(R"( [true])");
    EQ_VALUE(value.Stringify(), R"([true])", "Stringify()");

    value = JSON::Parse(R"( [false ])");
    EQ_VALUE(value.Stringify(), R"([false])", "Stringify()");

    value = JSON::Parse(R"([    null])");
    EQ_VALUE(value.Stringify(), R"([null])", "Stringify()");

    value = JSON::Parse(R"([ 0 ]     )");
    EQ_VALUE(value.Stringify(), R"([0])", "Stringify()");

    value = JSON::Parse(R"([      "a"   ]          )");
    EQ_VALUE(value.Stringify(), R"(["a"])", "Stringify()");

    value = JSON::Parse(R"(        ["ABC"])");
    EQ_VALUE(value.Stringify(), R"(["ABC"])", "Stringify()");

    value = JSON::Parse(R"([[]])");
    EQ_VALUE(value.Stringify(), R"([[]])", "Stringify()");

    value = JSON::Parse(R"([ [ ]])");
    EQ_VALUE(value.Stringify(), R"([[]])", "Stringify()");

    value = JSON::Parse(R"([[          ]])");
    EQ_VALUE(value.Stringify(), R"([[]])", "Stringify()");

    value = JSON::Parse(R"([         [          ]           ])");
    EQ_VALUE(value.Stringify(), R"([[]])", "Stringify()");

    value = JSON::Parse(R"([{}])");
    EQ_VALUE(value.Stringify(), R"([{}])", "Stringify()");

    value = JSON::Parse(R"([ {}])");
    EQ_VALUE(value.Stringify(), R"([{}])", "Stringify()");

    value = JSON::Parse(R"([{        }])");
    EQ_VALUE(value.Stringify(), R"([{}])", "Stringify()");

    value = JSON::Parse(R"([ { } ])");
    EQ_VALUE(value.Stringify(), R"([{}])", "Stringify()");

    value = JSON::Parse(R"([ true,true ])");
    EQ_VALUE(value.Stringify(), R"([true,true])", "Stringify()");

    value = JSON::Parse(R"([false, true])");
    EQ_VALUE(value.Stringify(), R"([false,true])", "Stringify()");

    value = JSON::Parse(R"([null ,true])");
    EQ_VALUE(value.Stringify(), R"([null,true])", "Stringify()");

    value = JSON::Parse(R"([-3 , true])");
    EQ_VALUE(value.Stringify(), R"([-3,true])", "Stringify()");

    value = JSON::Parse(R"(["a"       ,    true])");
    EQ_VALUE(value.Stringify(), R"(["a",true])", "Stringify()");

    value = JSON::Parse(R"([   "ABC",true       ])");
    EQ_VALUE(value.Stringify(), R"(["ABC",true])", "Stringify()");

    value = JSON::Parse(R"([       [] ,  true      ])");
    EQ_VALUE(value.Stringify(), R"([[],true])", "Stringify()");

    value = JSON::Parse(R"([ {} , true ])");
    EQ_VALUE(value.Stringify(), R"([{},true])", "Stringify()");

    value = JSON::Parse(R"([  true ,false])");
    EQ_VALUE(value.Stringify(), R"([true,false])", "Stringify()");

    value = JSON::Parse(R"([false, false ])");
    EQ_VALUE(value.Stringify(), R"([false,false])", "Stringify()");

    value = JSON::Parse(R"([ null ,false])");
    EQ_VALUE(value.Stringify(), R"([null,false])", "Stringify()");

    value = JSON::Parse(R"([500,false])");
    EQ_VALUE(value.Stringify(), R"([500,false])", "Stringify()");

    value = JSON::Parse(R"(["a",false])");
    EQ_VALUE(value.Stringify(), R"(["a",false])", "Stringify()");

    value = JSON::Parse(R"([ "ABC" , false ])");
    EQ_VALUE(value.Stringify(), R"(["ABC",false])", "Stringify()");

    value = JSON::Parse(R"([[]                      ,false])");
    EQ_VALUE(value.Stringify(), R"([[],false])", "Stringify()");

    value = JSON::Parse(R"([{},                     false])");
    EQ_VALUE(value.Stringify(), R"([{},false])", "Stringify()");

    value = JSON::Parse(R"([true,null])");
    EQ_VALUE(value.Stringify(), R"([true,null])", "Stringify()");

    value = JSON::Parse(R"([false,null])");
    EQ_VALUE(value.Stringify(), R"([false,null])", "Stringify()");

    value = JSON::Parse(R"([null,null])");
    EQ_VALUE(value.Stringify(), R"([null,null])", "Stringify()");

    value = JSON::Parse(R"([456.5,null])");
    EQ_VALUE(value.Stringify(), R"([456.5,null])", "Stringify()");

    value = JSON::Parse(R"(["a",null])");
    EQ_VALUE(value.Stringify(), R"(["a",null])", "Stringify()");

    value = JSON::Parse(R"(["ABC",null])");
    EQ_VALUE(value.Stringify(), R"(["ABC",null])", "Stringify()");

    value = JSON::Parse(R"([[],null])");
    EQ_VALUE(value.Stringify(), R"([[],null])", "Stringify()");

    value = JSON::Parse(R"([{},null])");
    EQ_VALUE(value.Stringify(), R"([{},null])", "Stringify()");

    value = JSON::Parse(R"([true,"A"])");
    EQ_VALUE(value.Stringify(), R"([true,"A"])", "Stringify()");

    value = JSON::Parse(R"([false,"AB"])");
    EQ_VALUE(value.Stringify(), R"([false,"AB"])", "Stringify()");

    value = JSON::Parse(R"([null,"ABC"])");
    EQ_VALUE(value.Stringify(), R"([null,"ABC"])", "Stringify()");

    value = JSON::Parse(R"([-8.9,"ABC"])");
    EQ_VALUE(value.Stringify(), R"([-8.9,"ABC"])", "Stringify()");

    value = JSON::Parse(R"(["a","ABCD"])");
    EQ_VALUE(value.Stringify(), R"(["a","ABCD"])", "Stringify()");

    value = JSON::Parse(R"(["ABC","ABCDE"])");
    EQ_VALUE(value.Stringify(), R"(["ABC","ABCDE"])", "Stringify()");

    value = JSON::Parse(R"([[],"ABCDEF"])");
    EQ_VALUE(value.Stringify(), R"([[],"ABCDEF"])", "Stringify()");

    value = JSON::Parse(R"([{},"ABCDEFG"])");
    EQ_VALUE(value.Stringify(), R"([{},"ABCDEFG"])", "Stringify()");

    value = JSON::Parse(R"([true,[]])");
    EQ_VALUE(value.Stringify(), R"([true,[]])", "Stringify()");

    value = JSON::Parse(R"([false,[]])");
    EQ_VALUE(value.Stringify(), R"([false,[]])", "Stringify()");

    value = JSON::Parse(R"([null,[]])");
    EQ_VALUE(value.Stringify(), R"([null,[]])", "Stringify()");

    value = JSON::Parse(R"([10000,[]])");
    EQ_VALUE(value.Stringify(), R"([10000,[]])", "Stringify()");

    value = JSON::Parse(R"(["ABC",[]])");
    EQ_VALUE(value.Stringify(), R"(["ABC",[]])", "Stringify()");

    value = JSON::Parse(R"([[],[]])");
    EQ_VALUE(value.Stringify(), R"([[],[]])", "Stringify()");

    value = JSON::Parse(R"([{},[]])");
    EQ_VALUE(value.Stringify(), R"([{},[]])", "Stringify()");

    value = JSON::Parse(R"([true,{}])");
    EQ_VALUE(value.Stringify(), R"([true,{}])", "Stringify()");

    value = JSON::Parse(R"([false,{}])");
    EQ_VALUE(value.Stringify(), R"([false,{}])", "Stringify()");

    value = JSON::Parse(R"([null,{}])");
    EQ_VALUE(value.Stringify(), R"([null,{}])", "Stringify()");

    value = JSON::Parse(R"([-1000,{}])");
    EQ_VALUE(value.Stringify(), R"([-1000,{}])", "Stringify()");

    value = JSON::Parse(R"(["ABC",{}])");
    EQ_VALUE(value.Stringify(), R"(["ABC",{}])", "Stringify()");

    value = JSON::Parse(R"([[],{}])");
    EQ_VALUE(value.Stringify(), R"([[],{}])", "Stringify()");

    value = JSON::Parse(R"([{},{}])");
    EQ_VALUE(value.Stringify(), R"([{},{}])", "Stringify()");

    value = JSON::Parse(R"([true,{},false])");
    EQ_VALUE(value.Stringify(), R"([true,{},false])", "Stringify()");

    value = JSON::Parse(R"([false,[],null])");
    EQ_VALUE(value.Stringify(), R"([false,[],null])", "Stringify()");

    value = JSON::Parse(R"([null,{},"ABC"])");
    EQ_VALUE(value.Stringify(), R"([null,{},"ABC"])", "Stringify()");

    value = JSON::Parse(R"([null,789,"ABC"])");
    EQ_VALUE(value.Stringify(), R"([null,789,"ABC"])", "Stringify()");

    value = JSON::Parse(R"(["ABC",[],{}])");
    EQ_VALUE(value.Stringify(), R"(["ABC",[],{}])", "Stringify()");

    value = JSON::Parse(R"([[],{},498])");
    EQ_VALUE(value.Stringify(), R"([[],{},498])", "Stringify()");

    value = JSON::Parse(R"([{},[],true])");
    EQ_VALUE(value.Stringify(), R"([{},[],true])", "Stringify()");

    value = JSON::Parse(R"([true,{},0,[]])");
    EQ_VALUE(value.Stringify(), R"([true,{},0,[]])", "Stringify()");

    value = JSON::Parse(R"([false,[],null,{}])");
    EQ_VALUE(value.Stringify(), R"([false,[],null,{}])", "Stringify()");

    value = JSON::Parse(R"([null,{},"ABC",[]])");
    EQ_VALUE(value.Stringify(), R"([null,{},"ABC",[]])", "Stringify()");

    value = JSON::Parse(R"(["ABC",[],{},null])");
    EQ_VALUE(value.Stringify(), R"(["ABC",[],{},null])", "Stringify()");

    value = JSON::Parse(R"([[],false,{},true])");
    EQ_VALUE(value.Stringify(), R"([[],false,{},true])", "Stringify()");

    value = JSON::Parse(R"([{},null,[],[]])");
    EQ_VALUE(value.Stringify(), R"([{},null,[],[]])", "Stringify()");

    value = JSON::Parse(R"([true,false,null,123,"ABC",[],{}])");
    EQ_VALUE(value.Stringify(), R"([true,false,null,123,"ABC",[],{}])",
             "Stringify()");

    value = JSON::Parse(R"([{},[],"a",1.5,null,false,true])");
    EQ_VALUE(value.Stringify(), R"([{},[],"a",1.5,null,false,true])",
             "Stringify()");

    value = JSON::Parse(R"([["]"],["]"]])");
    EQ_VALUE(value.Stringify(), R"([["]"],["]"]])", "Stringify()");

    value = JSON::Parse(R"([["[]"],["[]"]])");
    EQ_VALUE(value.Stringify(), R"([["[]"],["[]"]])", "Stringify()");

    END_SUB_TEST;
}

static int TestParse2() {
    Value<char> value;

    value = JSON::Parse(R"({})");
    EQ_VALUE(value.Stringify(), R"({})", "Stringify()");

    value = JSON::Parse(R"( {} )");
    EQ_VALUE(value.Stringify(), R"({})", "Stringify()");

    value = JSON::Parse(R"(  {}   )");
    EQ_VALUE(value.Stringify(), R"({})", "Stringify()");

    value = JSON::Parse(R"({ })");
    EQ_VALUE(value.Stringify(), R"({})", "Stringify()");

    value = JSON::Parse(R"({     })");
    EQ_VALUE(value.Stringify(), R"({})", "Stringify()");

    value = JSON::Parse(R"( {"A":true})");
    EQ_VALUE(value.Stringify(), R"({"A":true})", "Stringify()");

    value = JSON::Parse(R"( { "B":false})");
    EQ_VALUE(value.Stringify(), R"({"B":false})", "Stringify()");

    value = JSON::Parse(R"( {"AA":null }            )");
    EQ_VALUE(value.Stringify(), R"({"AA":null})", "Stringify()");

    value = JSON::Parse(R"( {"a" :0} )");
    EQ_VALUE(value.Stringify(), R"({"a":0})", "Stringify()");

    value = JSON::Parse(R"(      {"abc": "a"}        )");
    EQ_VALUE(value.Stringify(), R"({"abc":"a"})", "Stringify()");

    value = JSON::Parse(R"({"ab" : "ABC"} )");
    EQ_VALUE(value.Stringify(), R"({"ab":"ABC"})", "Stringify()");

    value = JSON::Parse(R"({ "ABC" :[]}         )");
    EQ_VALUE(value.Stringify(), R"({"ABC":[]})", "Stringify()");

    value = JSON::Parse(R"(      { "ABC" :[          ]})");
    EQ_VALUE(value.Stringify(), R"({"ABC":[]})", "Stringify()");

    value = JSON::Parse(R"({"5vn7b83y98t3wrupwmwa4ataw": {         } })");
    EQ_VALUE(value.Stringify(), R"({"5vn7b83y98t3wrupwmwa4ataw":{}})",
             "Stringify()");

    value = JSON::Parse(R"({      "A":true,"B":true})");
    EQ_VALUE(value.Stringify(), R"({"A":true,"B":true})", "Stringify()");

    value = JSON::Parse(R"({"A":false,"b":true          })");
    EQ_VALUE(value.Stringify(), R"({"A":false,"b":true})", "Stringify()");

    value = JSON::Parse(R"({"A"        :null,"BC":true})");
    EQ_VALUE(value.Stringify(), R"({"A":null,"BC":true})", "Stringify()");

    value = JSON::Parse(R"({"A":          -3,"AB":true})");
    EQ_VALUE(value.Stringify(), R"({"A":-3,"AB":true})", "Stringify()");

    value = JSON::Parse(R"({"A":"a"        ,"ABC":true})");
    EQ_VALUE(value.Stringify(), R"({"A":"a","ABC":true})", "Stringify()");

    value = JSON::Parse(R"({"A":"ABC" ,"1":true})");
    EQ_VALUE(value.Stringify(), R"({"A":"ABC","1":true})", "Stringify()");

    value = JSON::Parse(R"({"X":[], "123":true})");
    EQ_VALUE(value.Stringify(), R"({"X":[],"123":true})", "Stringify()");

    value = JSON::Parse(R"({"x":{},             "A":true})");
    EQ_VALUE(value.Stringify(), R"({"x":{},"A":true})", "Stringify()");

    value = JSON::Parse(R"({     "A2"     :   true  ,   "A1"  :   false   })");
    EQ_VALUE(value.Stringify(), R"({"A2":true,"A1":false})", "Stringify()");

    value = JSON::Parse(R"({"A-" : false,"A123" : false})");
    EQ_VALUE(value.Stringify(), R"({"A-":false,"A123":false})", "Stringify()");

    value = JSON::Parse(R"({"A":null,             "B":false})");
    EQ_VALUE(value.Stringify(), R"({"A":null,"B":false})", "Stringify()");

    value = JSON::Parse(R"({"A":500,"B":false             })");
    EQ_VALUE(value.Stringify(), R"({"A":500,"B":false})", "Stringify()");

    value = JSON::Parse(R"({           "A":"a","B":false})");
    EQ_VALUE(value.Stringify(), R"({"A":"a","B":false})", "Stringify()");

    value = JSON::Parse(R"({"A":          "ABC","B":false})");
    EQ_VALUE(value.Stringify(), R"({"A":"ABC","B":false})", "Stringify()");

    value = JSON::Parse(R"({"X":[] ,   "A":false})");
    EQ_VALUE(value.Stringify(), R"({"X":[],"A":false})", "Stringify()");

    value = JSON::Parse(R"({"X":{},"A"          :false})");
    EQ_VALUE(value.Stringify(), R"({"X":{},"A":false})", "Stringify()");

    value = JSON::Parse(R"({"A":true,"W":null})");
    EQ_VALUE(value.Stringify(), R"({"A":true,"W":null})", "Stringify()");

    value = JSON::Parse(R"({"A":false,"@":           null})");
    EQ_VALUE(value.Stringify(), R"({"A":false,"@":null})", "Stringify()");

    value = JSON::Parse(R"({"A":null,"#":null})");
    EQ_VALUE(value.Stringify(), R"({"A":null,"#":null})", "Stringify()");

    value = JSON::Parse(R"({"A":456.5,"H":null})");
    EQ_VALUE(value.Stringify(), R"({"A":456.5,"H":null})", "Stringify()");

    value = JSON::Parse(R"({"A":"a","Q":null})");
    EQ_VALUE(value.Stringify(), R"({"A":"a","Q":null})", "Stringify()");

    value = JSON::Parse(R"({"A":"ABC","e":null})");
    EQ_VALUE(value.Stringify(), R"({"A":"ABC","e":null})", "Stringify()");

    value = JSON::Parse(R"({"X":[],"n":null})");
    EQ_VALUE(value.Stringify(), R"({"X":[],"n":null})", "Stringify()");

    value = JSON::Parse(R"({"t":{},"A":null})");
    EQ_VALUE(value.Stringify(), R"({"t":{},"A":null})", "Stringify()");

    value = JSON::Parse(R"({"X":[          ],"n":null})");
    EQ_VALUE(value.Stringify(), R"({"X":[],"n":null})", "Stringify()");

    value = JSON::Parse(R"({"t":{         },"A":null})");
    EQ_VALUE(value.Stringify(), R"({"t":{},"A":null})", "Stringify()");

    value = JSON::Parse(R"({"e":true,"A":"A"})");
    EQ_VALUE(value.Stringify(), R"({"e":true,"A":"A"})", "Stringify()");

    value = JSON::Parse(R"({"m":false,"A":"AB"})");
    EQ_VALUE(value.Stringify(), R"({"m":false,"A":"AB"})", "Stringify()");

    value = JSON::Parse(R"({"i":null,"A":"ABC"})");
    EQ_VALUE(value.Stringify(), R"({"i":null,"A":"ABC"})", "Stringify()");

    value = JSON::Parse(R"({"s":-8.9,"A":"ABC"})");
    EQ_VALUE(value.Stringify(), R"({"s":-8.9,"A":"ABC"})", "Stringify()");

    value = JSON::Parse(R"({"-":"a","A":"ABCD"})");
    EQ_VALUE(value.Stringify(), R"({"-":"a","A":"ABCD"})", "Stringify()");

    value = JSON::Parse(R"({"g":"ABC","A":"ABCDE"})");
    EQ_VALUE(value.Stringify(), R"({"g":"ABC","A":"ABCDE"})", "Stringify()");

    value = JSON::Parse(R"({"o":[],"A":"ABCDEF"})");
    EQ_VALUE(value.Stringify(), R"({"o":[],"A":"ABCDEF"})", "Stringify()");

    value = JSON::Parse(R"({"A":{},"o":"ABCDEFG"})");
    EQ_VALUE(value.Stringify(), R"({"A":{},"o":"ABCDEFG"})", "Stringify()");

    value = JSON::Parse(R"({"d":true,"y":[]})");
    EQ_VALUE(value.Stringify(), R"({"d":true,"y":[]})", "Stringify()");

    value = JSON::Parse(R"({"A":false,"y":[]})");
    EQ_VALUE(value.Stringify(), R"({"A":false,"y":[]})", "Stringify()");

    value = JSON::Parse(R"({"A":null,"y":[]})");
    EQ_VALUE(value.Stringify(), R"({"A":null,"y":[]})", "Stringify()");

    value = JSON::Parse(R"({"A":10000,"y":[]})");
    EQ_VALUE(value.Stringify(), R"({"A":10000,"y":[]})", "Stringify()");

    value = JSON::Parse(R"({"A":"ABC","y":[]})");
    EQ_VALUE(value.Stringify(), R"({"A":"ABC","y":[]})", "Stringify()");

    value = JSON::Parse(R"({"X":[],"y":[]})");
    EQ_VALUE(value.Stringify(), R"({"X":[],"y":[]})", "Stringify()");

    value = JSON::Parse(R"({"X":{},"Y":[]})");
    EQ_VALUE(value.Stringify(), R"({"X":{},"Y":[]})", "Stringify()");

    value = JSON::Parse(R"({"A":true,"y":{}})");
    EQ_VALUE(value.Stringify(), R"({"A":true,"y":{}})", "Stringify()");

    value = JSON::Parse(R"({"A":false,"y":{}})");
    EQ_VALUE(value.Stringify(), R"({"A":false,"y":{}})", "Stringify()");

    value = JSON::Parse(R"({"A":null,"y":{}})");
    EQ_VALUE(value.Stringify(), R"({"A":null,"y":{}})", "Stringify()");

    value = JSON::Parse(R"({"A":-1000,"y":{}})");
    EQ_VALUE(value.Stringify(), R"({"A":-1000,"y":{}})", "Stringify()");

    value = JSON::Parse(R"({"A":"ABC","y":{}})");
    EQ_VALUE(value.Stringify(), R"({"A":"ABC","y":{}})", "Stringify()");

    value = JSON::Parse(R"({"C":[],"R":{}})");
    EQ_VALUE(value.Stringify(), R"({"C":[],"R":{}})", "Stringify()");

    value = JSON::Parse(R"({"cc":{},"rr":{}})");
    EQ_VALUE(value.Stringify(), R"({"cc":{},"rr":{}})", "Stringify()");

    value = JSON::Parse(R"({"A":true,"y":{},"AA":false})");
    EQ_VALUE(value.Stringify(), R"({"A":true,"y":{},"AA":false})",
             "Stringify()");

    value = JSON::Parse(R"({"A":false,"y":[],"B":null})");
    EQ_VALUE(value.Stringify(), R"({"A":false,"y":[],"B":null})",
             "Stringify()");

    value = JSON::Parse(R"({"A":null,"y":{},"ABC":"ABC"})");
    EQ_VALUE(value.Stringify(), R"({"A":null,"y":{},"ABC":"ABC"})",
             "Stringify()");

    value = JSON::Parse(R"({"t":null,"Y":789,"A":"ABC"})");
    EQ_VALUE(value.Stringify(), R"({"t":null,"Y":789,"A":"ABC"})",
             "Stringify()");

    value = JSON::Parse(R"({"A":"ABC","y":[],"key-u":{}})");
    EQ_VALUE(value.Stringify(), R"({"A":"ABC","y":[],"key-u":{}})",
             "Stringify()");

    value = JSON::Parse(R"({"X":[],"Y":{},"key-u":498})");
    EQ_VALUE(value.Stringify(), R"({"X":[],"Y":{},"key-u":498})",
             "Stringify()");

    value = JSON::Parse(R"({"X":{},"y":[],"A":true})");
    EQ_VALUE(value.Stringify(), R"({"X":{},"y":[],"A":true})", "Stringify()");

    value = JSON::Parse(R"({"{}}":true,"y":{},"AA":0,"k-300":[]})");
    EQ_VALUE(value.Stringify(), R"({"{}}":true,"y":{},"AA":0,"k-300":[]})",
             "Stringify()");

    value = JSON::Parse(R"({"B1":false,"y":[],"[A]":null,"k-300":{}})");
    EQ_VALUE(value.Stringify(), R"({"B1":false,"y":[],"[A]":null,"k-300":{}})",
             "Stringify()");

    value = JSON::Parse(R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})");
    EQ_VALUE(value.Stringify(), R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})",
             "Stringify()");

    value = JSON::Parse(R"({"x":"ABC","[]]":[],"key-u":{},"A":null})");
    EQ_VALUE(value.Stringify(), R"({"x":"ABC","[]]":[],"key-u":{},"A":null})",
             "Stringify()");

    value = JSON::Parse(R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})");
    EQ_VALUE(value.Stringify(),
             R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})", "Stringify()");

    value = JSON::Parse(R"({"X":{},"A":null,"key-u":[],"k-300":[]})");
    EQ_VALUE(value.Stringify(), R"({"X":{},"A":null,"key-u":[],"k-300":[]})",
             "Stringify()");

    value = JSON::Parse(
        R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})");
    EQ_VALUE(value.Stringify(),
             R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})",
             "Stringify()");

    value = JSON::Parse(
        R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})");
    EQ_VALUE(
        value.Stringify(),
        R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})",
        "Stringify()");

    value = JSON::Parse(R"({"a":{"c":"}"},"b":{"d":"}"}})");
    EQ_VALUE(value.Stringify(), R"({"a":{"c":"}"},"b":{"d":"}"}})",
             "Stringify()");

    value = JSON::Parse(R"({"a":{"c":"{}"},"b":{"d":"{}"}})");
    EQ_VALUE(value.Stringify(), R"({"a":{"c":"{}"},"b":{"d":"{}"}})",
             "Stringify()");

    END_SUB_TEST;
}

static int TestParse3() {
    Value<char> value;

    value = JSON::Parse(R"([{"one":1}])");
    EQ_VALUE(value.Stringify(), R"([{"one":1}])", "Stringify()");

    value = JSON::Parse(R"([{"one":"a"}])");
    EQ_VALUE(value.Stringify(), R"([{"one":"a"}])", "Stringify()");

    value = JSON::Parse(R"(["a","b"])");
    EQ_VALUE(value.Stringify(), R"(["a","b"])", "Stringify()");

    value = JSON::Parse(R"([[1,2],[30]])");
    EQ_VALUE(value.Stringify(), R"([[1,2],[30]])", "Stringify()");

    value = JSON::Parse(R"([[[1]]])");
    EQ_VALUE(value.Stringify(), R"([[[1]]])", "Stringify()");

    value = JSON::Parse(R"([[123]])");
    EQ_VALUE(value.Stringify(), R"([[123]])", "Stringify()");

    value = JSON::Parse(R"([[1,2]])");
    EQ_VALUE(value.Stringify(), R"([[1,2]])", "Stringify()");

    value = JSON::Parse(R"([["a","b"]])");
    EQ_VALUE(value.Stringify(), R"([["a","b"]])", "Stringify()");

    value = JSON::Parse(R"([[1,2,3]])");
    EQ_VALUE(value.Stringify(), R"([[1,2,3]])", "Stringify()");

    value = JSON::Parse(R"([[[1,2,3]]])");
    EQ_VALUE(value.Stringify(), R"([[[1,2,3]]])", "Stringify()");

    value = JSON::Parse(R"([[[[1,2,3]]]])");
    EQ_VALUE(value.Stringify(), R"([[[[1,2,3]]]])", "Stringify()");

    value = JSON::Parse(R"([123,"",456])");
    EQ_VALUE(value.Stringify(), R"([123,"",456])", "Stringify()");

    value = JSON::Parse(R"([[1,"",2]])");
    EQ_VALUE(value.Stringify(), R"([[1,"",2]])", "Stringify()");

    value = JSON::Parse(R"([[123,456,""]])");
    EQ_VALUE(value.Stringify(), R"([[123,456,""]])", "Stringify()");

    value = JSON::Parse(R"({"a":[1],"b":1})");
    EQ_VALUE(value.Stringify(), R"({"a":[1],"b":1})", "Stringify()");

    value = JSON::Parse(R"([[],123,456])");
    EQ_VALUE(value.Stringify(), R"([[],123,456])", "Stringify()");

    value = JSON::Parse(R"([123,456,[]])");
    EQ_VALUE(value.Stringify(), R"([123,456,[]])", "Stringify()");

    value = JSON::Parse(R"([[[],1]])");
    EQ_VALUE(value.Stringify(), R"([[[],1]])", "Stringify()");

    value = JSON::Parse(R"([[123,456,[]]])");
    EQ_VALUE(value.Stringify(), R"([[123,456,[]]])", "Stringify()");

    value = JSON::Parse(R"([3,[4],5])");
    EQ_VALUE(value.Stringify(), R"([3,[4],5])", "Stringify()");

    value = JSON::Parse(R"({})");
    EQ_VALUE(value.Stringify(), R"({})", "Stringify()");

    value = JSON::Parse(R"({"a":{}})");
    EQ_VALUE(value.Stringify(), R"({"a":{}})", "Stringify()");

    value = JSON::Parse(R"([[[{}]]])");
    EQ_VALUE(value.Stringify(), R"([[[{}]]])", "Stringify()");

    value = JSON::Parse(R"([[[],[]]])");
    EQ_VALUE(value.Stringify(), R"([[[],[]]])", "Stringify()");

    value = JSON::Parse(R"(["oo",[[],[],[{},[]]],"bk"])");
    EQ_VALUE(value.Stringify(), R"(["oo",[[],[],[{},[]]],"bk"])",
             "Stringify()");

    value = JSON::Parse(R"([{},[]])");
    EQ_VALUE(value.Stringify(), R"([{},[]])", "Stringify()");

    value = JSON::Parse(R"(["aa",[[{}],["ww","ee"]],"bb"])");
    EQ_VALUE(value.Stringify(), R"(["aa",[[{}],["ww","ee"]],"bb"])",
             "Stringify()");

    value = JSON::Parse(R"({"aa":44,"dd":"bb"})");
    EQ_VALUE(value.Stringify(), R"({"aa":44,"dd":"bb"})", "Stringify()");

    value = JSON::Parse(R"({"aa":[],"dd":"bb"})");
    EQ_VALUE(value.Stringify(), R"({"aa":[],"dd":"bb"})", "Stringify()");

    value = JSON::Parse(R"([[],[[[]]]])");
    EQ_VALUE(value.Stringify(), R"([[],[[[]]]])", "Stringify()");

    value = JSON::Parse(R"([[[["1","2","3"]]]])");
    EQ_VALUE(value.Stringify(), R"([[[["1","2","3"]]]])", "Stringify()");

    value = JSON::Parse(R"([{"ab":{}}])");
    EQ_VALUE(value.Stringify(), R"([{"ab":{}}])", "Stringify()");

    value = JSON::Parse(R"([{"ab":{},"cd":""}])");
    EQ_VALUE(value.Stringify(), R"([{"ab":{},"cd":""}])", "Stringify()");

    value = JSON::Parse(R"([[[],[]]])");
    EQ_VALUE(value.Stringify(), R"([[[],[]]])", "Stringify()");

    value = JSON::Parse(R"({"a":[],"c":[]})");
    EQ_VALUE(value.Stringify(), R"({"a":[],"c":[]})", "Stringify()");

    value = JSON::Parse(R"({"\"bb\"":"\""})");
    EQ_VALUE(value.Stringify(), R"({"\"bb\"":"\""})", "Stringify()");

    value = JSON::Parse(R"(["\"bb\"","\""])");
    EQ_VALUE(value.Stringify(), R"(["\"bb\"","\""])", "Stringify()");

    value = JSON::Parse(R"(["s\""])");
    EQ_VALUE(value.Stringify(), R"(["s\""])", "Stringify()");

    value = JSON::Parse(R"(["\\"])");
    EQ_VALUE(value.Stringify(), R"(["\\"])", "Stringify()");

    value = JSON::Parse(R"(["a\\"])");
    EQ_VALUE(value.Stringify(), R"(["a\\"])", "Stringify()");

    value = JSON::Parse(R"(["\\a"])");
    EQ_VALUE(value.Stringify(), R"(["\\a"])", "Stringify()");

    value = JSON::Parse(R"(["a\\a"])");
    EQ_VALUE(value.Stringify(), R"(["a\\a"])", "Stringify()");

    value = JSON::Parse(R"(["aaa\\"])");
    EQ_VALUE(value.Stringify(), R"(["aaa\\"])", "Stringify()");

    value = JSON::Parse(R"(["\\aaa"])");
    EQ_VALUE(value.Stringify(), R"(["\\aaa"])", "Stringify()");

    value = JSON::Parse(R"(["aaa\\aaa"])");
    EQ_VALUE(value.Stringify(), R"(["aaa\\aaa"])", "Stringify()");

    value = JSON::Parse(R"(["c,"])");
    EQ_VALUE(value.Stringify(), R"(["c,"])", "Stringify()");

    value = JSON::Parse(R"(["]][[][]"])");
    EQ_VALUE(value.Stringify(), R"(["]][[][]"])", "Stringify()");

    value = JSON::Parse(R"(["k,","l"])");
    EQ_VALUE(value.Stringify(), R"(["k,","l"])", "Stringify()");

    value = JSON::Parse(R"([[[1],50]])");
    EQ_VALUE(value.Stringify(), R"([[[1],50]])", "Stringify()");

    value = JSON::Parse(R"([["[,]",5]])");
    EQ_VALUE(value.Stringify(), R"([["[,]",5]])", "Stringify()");

    value = JSON::Parse(R"([[["k,,,,,","l"]]])");
    EQ_VALUE(value.Stringify(), R"([[["k,,,,,","l"]]])", "Stringify()");

    value = JSON::Parse(R"(["k,","e","W","u"])");
    EQ_VALUE(value.Stringify(), R"(["k,","e","W","u"])", "Stringify()");

    value = JSON::Parse(R"([{"a":{}},[{},[1,2,3],["aa","cc"]]])");
    EQ_VALUE(value.Stringify(), R"([{"a":{}},[{},[1,2,3],["aa","cc"]]])",
             "Stringify()");

    value = JSON::Parse(R"(["s\"s\"s\"s\"s\"s\"s\"s\"s\"\\\\\\\\\\\\"])");
    EQ_VALUE(value.Stringify(),
             R"(["s\"s\"s\"s\"s\"s\"s\"s\"s\"\\\\\\\\\\\\"])", "Stringify()");

    value = JSON::Parse(
        R"(["s\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""])");
    EQ_VALUE(
        value.Stringify(),
        R"(["s\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""])",
        "Stringify()");

    value = JSON::Parse(R"(["a\nb"])");
    EQ_VALUE(value.Stringify(), R"(["a\nb"])", "Stringify()");

    value = JSON::Parse(R"([["[]{}","A"]])");
    EQ_VALUE(value.Stringify(), R"([["[]{}","A"]])", "Stringify()");

    value = JSON::Parse(R"([["[]{}",1]])");
    EQ_VALUE(value.Stringify(), R"([["[]{}",1]])", "Stringify()");

    value = JSON::Parse(R"([{"[]{}":"A"}])");
    EQ_VALUE(value.Stringify(), R"([{"[]{}":"A"}])", "Stringify()");

    value = JSON::Parse(R"([{"[]{}":1}])");
    EQ_VALUE(value.Stringify(), R"([{"[]{}":1}])", "Stringify()");

    value = JSON::Parse(R"([[["1","2","3"],"c","d"],"a",["1"]])");
    EQ_VALUE(value.Stringify(), R"([[["1","2","3"],"c","d"],"a",["1"]])",
             "Stringify()");

    value = JSON::Parse(R"([[[1,2,3],40,50],4,[0.5],5])");
    EQ_VALUE(value.Stringify(), R"([[[1,2,3],40,50],4,[0.5],5])",
             "Stringify()");

    value = JSON::Parse(R"([[[[],30]]])");
    EQ_VALUE(value.Stringify(), R"([[[[],30]]])", "Stringify()");

    value = JSON::Parse(R"([[],[30]])");
    EQ_VALUE(value.Stringify(), R"([[],[30]])", "Stringify()");

    value = JSON::Parse(R"([[1],[30]])");
    EQ_VALUE(value.Stringify(), R"([[1],[30]])", "Stringify()");

    value = JSON::Parse(R"({"a":{"b":"c"}})");
    EQ_VALUE(value.Stringify(), R"({"a":{"b":"c"}})", "Stringify()");

    value = JSON::Parse(
        R"([                                                                 1,
                                                                                               2
                                                                                                          ])");
    EQ_VALUE(value.Stringify(), R"([1,2])", "Stringify()");

    END_SUB_TEST;
}

static int TestParse4() {
    Value<char>         value;
    const String<char> *key_ptr;
    const char *        str1;

    value = JSON::Parse(R"(["\r"])");
    EQ_TRUE(
        StringUtils::IsEqual("\r", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    value = JSON::Parse(R"({"x":   "\n"})");
    EQ_TRUE(
        StringUtils::IsEqual("\n", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    value   = JSON::Parse(R"({"\""   :"x"       })");
    key_ptr = value.GetKey(0);
    NOT_EQ_TO(key_ptr, nullptr, "GetKey(0)", "null");
    EQ_TRUE(StringUtils::IsEqual("\"", key_ptr->First(), value[0].Length()),
            "IsEqual()");

    value   = JSON::Parse(R"({    "\n":   "\f"})");
    key_ptr = value.GetKey(0);
    NOT_EQ_TO(key_ptr, nullptr, "GetKey(0)", "null");
    EQ_TRUE(StringUtils::IsEqual("\n", key_ptr->First(), value[0].Length()),
            "IsEqual()");

    EQ_TRUE(
        StringUtils::IsEqual("\f", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    value = JSON::Parse(R"(["\/\/"])");
    EQ_TRUE(
        StringUtils::IsEqual("//", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    value = JSON::Parse(R"({"x":"\r\r"})");
    EQ_TRUE(StringUtils::IsEqual("\r\r", value[0].StringStorage(),
                                 value[0].Length()),
            "IsEqual()");

    value   = JSON::Parse(R"({"\b\b":"x"})");
    key_ptr = value.GetKey(0);
    NOT_EQ_TO(key_ptr, nullptr, "GetKey(0)", "null");
    EQ_TRUE(StringUtils::IsEqual("\b\b", key_ptr->First(), value[0].Length()),
            "IsEqual()");

    value   = JSON::Parse(R"({"\\\\":"\"\""})");
    key_ptr = value.GetKey(0);
    NOT_EQ_TO(key_ptr, nullptr, "GetKey(0)", "null");
    EQ_TRUE(StringUtils::IsEqual(R"(\\)", key_ptr->First(), value[0].Length()),
            "IsEqual()");

    EQ_TRUE(StringUtils::IsEqual(R"("")", value[0].StringStorage(),
                                 value[0].Length()),
            "IsEqual()");

    value = JSON::Parse(R"(["\t\t\t"])");
    EQ_TRUE(StringUtils::IsEqual("\t\t\t", value[0].StringStorage(),
                                 value[0].Length()),
            "IsEqual()");

    value = JSON::Parse(R"({"x":"\f\f\f"})");
    EQ_TRUE(StringUtils::IsEqual("\f\f\f", value[0].StringStorage(),
                                 value[0].Length()),
            "IsEqual()");

    value   = JSON::Parse(R"({"\\\\\\":"x"})");
    key_ptr = value.GetKey(0);
    NOT_EQ_TO(key_ptr, nullptr, "GetKey(0)", "null");
    EQ_TRUE(StringUtils::IsEqual(R"(\\\)", key_ptr->First(), value[0].Length()),
            "IsEqual()");

    value   = JSON::Parse(R"({"\/\/\/":"\n\n\n"})");
    key_ptr = value.GetKey(0);
    NOT_EQ_TO(key_ptr, nullptr, "GetKey(0)", "null");
    EQ_TRUE(StringUtils::IsEqual("///", key_ptr->First(), value[0].Length()),
            "IsEqual()");

    EQ_TRUE(StringUtils::IsEqual("\n\n\n", value[0].StringStorage(),
                                 value[0].Length()),
            "IsEqual()");

    value = JSON::Parse(R"([" \""])");
    EQ_TRUE(StringUtils::IsEqual(" \"", value[0].StringStorage(),
                                 value[0].Length()),
            "IsEqual()");

    value = JSON::Parse(R"([" \t "])");
    EQ_TRUE(StringUtils::IsEqual(" \t ", value[0].StringStorage(),
                                 value[0].Length()),
            "IsEqual()");

    value = JSON::Parse(R"(["\\ \\"])");
    EQ_TRUE(StringUtils::IsEqual("\\ \\", value[0].StringStorage(),
                                 value[0].Length()),
            "IsEqual()");

    value = JSON::Parse(R"(["\"\\\/\b\f\n\r\t"])");
    EQ_TRUE(StringUtils::IsEqual("\"\\/\b\f\n\r\t", value[0].StringStorage(),
                                 value[0].Length()),
            "IsEqual()");

    value = JSON::Parse(R"({"x":"\t\r\n\f\b\/\\\""})");
    EQ_TRUE(StringUtils::IsEqual("\t\r\n\f\b/\\\"", value[0].StringStorage(),
                                 value[0].Length()),
            "IsEqual()");

    value   = JSON::Parse(R"({"\t\r\n\f\b\/\\\"":"\"\\\/\b\f\n\r\t"})");
    key_ptr = value.GetKey(0);
    NOT_EQ_TO(key_ptr, nullptr, "GetKey(0)", "null");
    EQ_TRUE(StringUtils::IsEqual("\t\r\n\f\b/\\\"", key_ptr->First(),
                                 value[0].Length()),
            "IsEqual()");

    EQ_TRUE(StringUtils::IsEqual("\"\\/\b\f\n\r\t", value[0].StringStorage(),
                                 value[0].Length()),
            "IsEqual()");

    str1  = R"(["\u003D"])";
    value = JSON::Parse(str1);
    EQ_TRUE(
        StringUtils::IsEqual("=", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    str1  = R"(["\u00a1"])";
    value = JSON::Parse(str1, 10);
    EQ_TRUE(
        StringUtils::IsEqual("¡", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    String<char> str(R"(["\u08A7"])");
    value = JSON::Parse(str.First(), str.Length());
    EQ_TRUE(
        StringUtils::IsEqual("ࢧ", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    str   = R"(["\ud802\uDE7B"])";
    value = JSON::Parse(str.First(), str.Length());
    EQ_TRUE(
        StringUtils::IsEqual("𐩻", value[0].StringStorage(), value[0].Length()),
        "IsEqual()");

    str   = R"(["\uD83E\uFC59"])";
    value = JSON::Parse(str.First(), str.Length());
    EQ_TRUE(StringUtils::IsEqual("🡙", value[0].StringStorage(),
                                 value[0].Length()),
            "IsEqual()");

    str   = R"(["\UD800\UDE83W\U003DW\UD800\UDE83\U00A1\UD83E\UFC59\U08A7"])";
    value = JSON::Parse(str.First(), str.Length());
    EQ_TRUE(StringUtils::IsEqual("𐊃W=W𐊃¡🡙ࢧ",
                                 value[0].StringStorage(), value[0].Length()),
            "IsEqual()");

    END_SUB_TEST;
}

static int TestParse5() {
    Value<char> value;

    value = JSON::Parse(R"([1   ]    ])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"a":1   }    })");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([f])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([t])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([n])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([falsE])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([truE])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([nulL])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([false-])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([true-])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([null-])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([1,])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([1,   ])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([1   ,   ])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([,])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["1",,"2"])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([,1])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([,"1"])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["1"]])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([["1"]]])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([  []]  ])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w"],1])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" o  , 1])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"w": "a" o  , "b": "x"})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" ] ,1])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["x"]])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([]])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["aa"],"s"])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["x":"s"])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([{"x"::"s"}])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([]")");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({}")");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({},)");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([],)");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([[[[[[[[[001]]]]]]]]])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"a":{"b":{"c":001}}})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([w])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({x:"x"})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":001})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":4)");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["x")");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([4,])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x" 4})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x" "4"})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x" {}})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x" []})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x",4})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([4,])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":4,})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([4,,])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":4,,})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":"w" w})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":"w" true})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" true])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\w"])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([""]")");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    END_SUB_TEST;
}

static int TestParse6() {
    Value<char> value;

    value = JSON::Parse(R"(["\"])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["""])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["a",)");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"a":"c",)");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse("[\"abc\n\"]");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\"])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({{}})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({[]})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([123e])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([123e--4])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse("[\"\ta\"]");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\uaaa"])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\uaa"])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\ua"])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\u"])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["\uD83E\uFC5"])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([[{} {}]])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([{} {}])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([[] []])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" "w"])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" {}])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([4    {}])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({a "a":[]})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"a":a []})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"a"a :[]})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":"w" "w"})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":"w" "u":"w"})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([{} 0])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([["x" 0]])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([{} 1])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([[{} 0]])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([[] false])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" null])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"(["w" 3])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([4    4])");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":"w" 7})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"x":"w" 5})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({1)");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"([2)");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"a":h, "b": a})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse(R"({"a":{})");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse("{\"\na\":{}");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    value = JSON::Parse("[\"WHAT?\"}");
    EQ_TRUE(value.IsUndefined(), "value.IsUndefined()");

    END_SUB_TEST;
}

static int RunJSONTests() {
    STARTING_TEST("JSON.hpp");

    START_TEST("Parse Test 1", TestParse1);
    START_TEST("Parse Test 2", TestParse2);
    START_TEST("Parse Test 3", TestParse3);
    START_TEST("Parse Test 4", TestParse4);
    START_TEST("Parse Test 5", TestParse5);
    START_TEST("Parse Test 6", TestParse6);

    END_TEST("JSON.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
