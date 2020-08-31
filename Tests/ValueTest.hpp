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
#include "Value.hpp"

#ifndef QENTEM_VALUETEST_H_
#define QENTEM_VALUETEST_H_

namespace Qentem {
namespace Test {

using Qentem::Value;
using Qentem::ValueType;

static int TestEmptyValue1() {
    Value value1;
    Value value2;

    String str_var;
    double num_var;
    bool   bool_var;

    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetValue(10), nullptr, "GetValue(10)", "null");
    SHOULD_EQUAL(value1.GetValue("", 0), nullptr,
                 "GetValue("
                 ", 0)",
                 "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetKey(10), nullptr, "GetKey(10)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value2 = value1;
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetValue(10), nullptr, "GetValue(10)", "null");
    SHOULD_EQUAL(value1.GetValue("", 0), nullptr,
                 "GetValue("
                 ", 0)",
                 "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetKey(10), nullptr, "GetKey(10)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value2.IsUndefined(), "isUndefined()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsString(), "IsString()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsNumber(), "IsNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsTrue(), "IsTrue()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsFalse(), "IsFalse()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsNull(), "IsNull()");
    SHOULD_EQUAL(value2.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetValue(10), nullptr, "GetValue(10)", "null");
    SHOULD_EQUAL(value2.GetValue("", 0), nullptr,
                 "GetValue("
                 ", 0)",
                 "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetKey(10), nullptr, "GetKey(10)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestEmptyValue2() {
    Value value1;
    Value value2;

    String str_var;
    double num_var;
    bool   bool_var;

    value2 = static_cast<Value &&>(value1);
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetValue(10), nullptr, "GetValue(10)", "null");
    SHOULD_EQUAL(value1.GetValue("", 0), nullptr,
                 "GetValue("
                 ", 0)",
                 "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetKey(10), nullptr, "GetKey(10)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value2.IsUndefined(), "isUndefined()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsString(), "IsString()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsNumber(), "IsNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsTrue(), "IsTrue()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsFalse(), "IsFalse()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsNull(), "IsNull()");
    SHOULD_EQUAL(value2.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetValue(10), nullptr, "GetValue(10)", "null");
    SHOULD_EQUAL(value2.GetValue("", 0), nullptr,
                 "GetValue("
                 ", 0)",
                 "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetKey(10), nullptr, "GetKey(10)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    Value value3(value1);
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetValue(10), nullptr, "GetValue(10)", "null");
    SHOULD_EQUAL(value1.GetValue("", 0), nullptr,
                 "GetValue("
                 ", 0)",
                 "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetKey(10), nullptr, "GetKey(10)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value3.IsUndefined(), "isUndefined()");
    SHOULD_NOT_EQUAL_TRUE(value3.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL_TRUE(value3.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL_TRUE(value3.IsString(), "IsString()");
    SHOULD_NOT_EQUAL_TRUE(value3.IsNumber(), "IsNumber()");
    SHOULD_NOT_EQUAL_TRUE(value3.IsTrue(), "IsTrue()");
    SHOULD_NOT_EQUAL_TRUE(value3.IsFalse(), "IsFalse()");
    SHOULD_NOT_EQUAL_TRUE(value3.IsNull(), "IsNull()");
    SHOULD_EQUAL(value3.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value3.Size(), 0, "Type()");
    SHOULD_EQUAL(value3.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value3.GetValue(10), nullptr, "GetValue(10)", "null");
    SHOULD_EQUAL(value3.GetValue("", 0), nullptr,
                 "GetValue("
                 ", 0)",
                 "null");
    SHOULD_EQUAL(value3.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value3.GetKey(10), nullptr, "GetKey(10)", "null");
    SHOULD_EQUAL(value3.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value3.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value3.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value3.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value3.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value3.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value3.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value3.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value3.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value3.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestEmptyValue3() {
    Value value1;

    StringStream ss_var;
    String       str_var;
    double       num_var;
    bool         bool_var;

    Value value2(static_cast<Value &&>(value1));
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    SHOULD_NOT_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetValue(10), nullptr, "GetValue(10)", "null");
    SHOULD_EQUAL(value1.GetValue("", 0), nullptr,
                 "GetValue("
                 ", 0)",
                 "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetKey(10), nullptr, "GetKey(10)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.InsertString(ss_var), "GetString(ss_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");
    SHOULD_EQUAL_TRUE(value2.IsUndefined(), "isUndefined()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsString(), "IsString()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsNumber(), "IsNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsTrue(), "IsTrue()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsFalse(), "IsFalse()");
    SHOULD_NOT_EQUAL_TRUE(value2.IsNull(), "IsNull()");
    SHOULD_EQUAL(value2.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetValue(10), nullptr, "GetValue(10)", "null");
    SHOULD_EQUAL(value2.GetValue("", 0), nullptr,
                 "GetValue("
                 ", 0)",
                 "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetKey(10), nullptr, "GetKey(10)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    double d_var = static_cast<double>(value2);
    SHOULD_EQUAL_VALUE(d_var, 0, "static_cast<double>");

    const char *c_var = static_cast<const char *>(value2);
    SHOULD_EQUAL(c_var, nullptr, "static_cast<const char *>", "null");

    bool b_var = static_cast<bool>(value2);
    SHOULD_NOT_EQUAL_TRUE(b_var, "static_cast<bool>");

    value2 = Value{ValueType::Object};
    SHOULD_EQUAL_TRUE(value2.IsUndefined(), "isUndefined()");

    value2 = Value{ValueType::Array};
    SHOULD_EQUAL_TRUE(value2.IsUndefined(), "isUndefined()");

    value2 = Value{ValueType::String};
    SHOULD_EQUAL_TRUE(value2.IsUndefined(), "isUndefined()");

    value2 = Value{ValueType::Number};
    SHOULD_EQUAL_TRUE(value2.IsUndefined(), "isUndefined()");

    END_SUB_TEST;
}

static int TestTrueValue1() {
    Value value1;
    Value value2;

    StringStream ss_var;
    String       str_var;
    double       num_var;
    bool         bool_var;

    value1 = true;
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    SHOULD_EQUAL(value1.Type(), ValueType::True, "Type()", "True");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "true", "str_var");
    SHOULD_EQUAL_TRUE(value1.InsertString(ss_var), "GetString(ss_var)");
    SHOULD_EQUAL_VALUE(ss_var.GetString(), "true", "ss_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 1, "num_var");
    SHOULD_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value1.Clear();
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value1 = true;
    value2 = true;
    value2 = value1;
    SHOULD_EQUAL_TRUE(value2.IsTrue(), "IsTrue()");
    SHOULD_EQUAL(value2.Type(), ValueType::True, "Type()", "True");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "true", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 1, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    SHOULD_EQUAL(value1.Type(), ValueType::True, "Type()", "True");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "true", "str_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 1, "num_var");
    SHOULD_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestTrueValue2() {
    Value value1;
    Value value2;

    String str_var;
    double num_var;
    bool   bool_var;

    value1 = true;
    value2 = Value(value1);
    SHOULD_EQUAL_TRUE(value2.IsTrue(), "IsTrue()");
    SHOULD_EQUAL(value2.Type(), ValueType::True, "Type()", "True");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "true", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 1, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    SHOULD_EQUAL(value1.Type(), ValueType::True, "Type()", "True");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "true", "str_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 1, "num_var");
    SHOULD_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value2.Clear();
    value2 = static_cast<Value &&>(value1);
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value2.IsTrue(), "IsTrue()");
    SHOULD_EQUAL(value2.Type(), ValueType::True, "Type()", "True");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "true", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 1, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestTrueValue3() {
    Value value1;

    String str_var;
    double num_var;
    bool   bool_var;

    value1 = true;
    Value value2(static_cast<Value &&>(value1));
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value2.IsTrue(), "IsTrue()");
    SHOULD_EQUAL(value2.Type(), ValueType::True, "Type()", "True");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "true", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 1, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    value2 = true;
    SHOULD_EQUAL_TRUE(value2.IsTrue(), "IsTrue()");

    double d_var = static_cast<double>(value2);
    SHOULD_EQUAL_VALUE(d_var, 0, "static_cast<double>");

    const char *c_var = static_cast<const char *>(value2);
    SHOULD_EQUAL(c_var, nullptr, "static_cast<const char *>", "null");

    bool b_var = static_cast<bool>(value2);
    SHOULD_EQUAL_TRUE(b_var, "static_cast<bool>");

    value2 = Value{ValueType::True};
    SHOULD_EQUAL_TRUE(value2.IsTrue(), "IsTrue()");

    END_SUB_TEST;
}

static int TestFalseValue1() {
    Value value1;
    Value value2;

    StringStream ss_var;
    String       str_var;
    double       num_var;
    bool         bool_var;

    value1 = false;
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    SHOULD_EQUAL(value1.Type(), ValueType::False, "Type()", "False");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "false", "str_var");
    SHOULD_EQUAL_TRUE(value1.InsertString(ss_var), "GetString(ss_var)");
    SHOULD_EQUAL_VALUE(ss_var.GetString(), "false", "ss_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value1.Clear();
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value1 = false;
    value2 = false;
    value2 = value1;
    SHOULD_EQUAL_TRUE(value2.IsFalse(), "IsFalse()");
    SHOULD_EQUAL(value2.Type(), ValueType::False, "Type()", "False");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "false", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    SHOULD_EQUAL(value1.Type(), ValueType::False, "Type()", "False");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "false", "str_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestFalseValue2() {
    Value value1;
    Value value2;

    String str_var;
    double num_var;
    bool   bool_var;

    value1 = false;
    value2 = Value(value1);
    SHOULD_EQUAL_TRUE(value2.IsFalse(), "IsFalse()");
    SHOULD_EQUAL(value2.Type(), ValueType::False, "Type()", "False");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "false", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    SHOULD_EQUAL(value1.Type(), ValueType::False, "Type()", "False");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "false", "str_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value2.Clear();
    value2 = static_cast<Value &&>(value1);
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value2.IsFalse(), "IsFalse()");
    SHOULD_EQUAL(value2.Type(), ValueType::False, "Type()", "False");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "false", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestFalseValue3() {
    Value value1;

    String str_var;
    double num_var;
    bool   bool_var;

    value1 = false;
    Value value2(static_cast<Value &&>(value1));
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value2.IsFalse(), "IsFalse()");
    SHOULD_EQUAL(value2.Type(), ValueType::False, "Type()", "False");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "false", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    value2 = false;
    SHOULD_EQUAL_TRUE(value2.IsFalse(), "IsFalse()");

    double d_var = static_cast<double>(value2);
    SHOULD_EQUAL_VALUE(d_var, 0, "static_cast<double>");

    const char *c_var = static_cast<const char *>(value2);
    SHOULD_EQUAL(c_var, nullptr, "static_cast<const char *>", "null");

    bool b_var = static_cast<bool>(value2);
    SHOULD_NOT_EQUAL_TRUE(b_var, "static_cast<bool>");

    value2 = Value{ValueType::False};
    SHOULD_EQUAL_TRUE(value2.IsFalse(), "IsFalse()");

    END_SUB_TEST;
}

static int TestNullValue1() {
    Value value1;
    Value value2;

    StringStream ss_var;
    String       str_var;
    double       num_var;
    bool         bool_var;

    value1 = nullptr;
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    SHOULD_EQUAL(value1.Type(), ValueType::Null, "Type()", "Null");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "null", "str_var");
    SHOULD_EQUAL_TRUE(value1.InsertString(ss_var), "GetString(ss_var)");
    SHOULD_EQUAL_VALUE(ss_var.GetString(), "null", "ss_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value1.Clear();
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value1 = nullptr;
    value2 = nullptr;
    value2 = value1;
    SHOULD_EQUAL_TRUE(value2.IsNull(), "IsNull()");
    SHOULD_EQUAL(value2.Type(), ValueType::Null, "Type()", "Null");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "null", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    SHOULD_EQUAL(value1.Type(), ValueType::Null, "Type()", "Null");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "null", "str_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestNullValue2() {
    Value value1;
    Value value2;

    String str_var;
    double num_var;
    bool   bool_var;

    value1 = nullptr;
    value2 = Value(value1);
    SHOULD_EQUAL_TRUE(value2.IsNull(), "IsNull()");
    SHOULD_EQUAL(value2.Type(), ValueType::Null, "Type()", "Null");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "null", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    SHOULD_EQUAL(value1.Type(), ValueType::Null, "Type()", "Null");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "null", "str_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value2.Clear();
    value2 = static_cast<Value &&>(value1);
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value2.IsNull(), "IsNull()");
    SHOULD_EQUAL(value2.Type(), ValueType::Null, "Type()", "Null");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "null", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestNullValue3() {
    Value value1;

    String str_var;
    double num_var;
    bool   bool_var;

    value1 = nullptr;
    Value value2(static_cast<Value &&>(value1));
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value2.IsNull(), "IsNull()");
    SHOULD_EQUAL(value2.Type(), ValueType::Null, "Type()", "Null");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "null", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    value2 = nullptr;
    SHOULD_EQUAL_TRUE(value2.IsNull(), "IsNull()");

    double d_var = static_cast<double>(value2);
    SHOULD_EQUAL_VALUE(d_var, 0, "static_cast<double>");

    const char *c_var = static_cast<const char *>(value2);
    SHOULD_EQUAL(c_var, nullptr, "static_cast<const char *>", "null");

    bool b_var = static_cast<bool>(value2);
    SHOULD_NOT_EQUAL_TRUE(b_var, "static_cast<bool>");

    value2 = Value{ValueType::Null};
    SHOULD_EQUAL_TRUE(value2.IsNull(), "IsNull()");

    END_SUB_TEST;
}

static int TestNumberValue1() {
    Value value1;
    Value value2;

    StringStream ss_var;
    String       str_var;
    double       num_var;
    bool         bool_var;

    value1 = 33;
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL(value1.Type(), ValueType::Number, "Type()", "Number");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "33", "str_var");
    SHOULD_EQUAL_TRUE(value1.InsertString(ss_var), "GetString(ss_var)");
    SHOULD_EQUAL_VALUE(ss_var.GetString(), "33", "ss_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 33, "GetNumber()");
    SHOULD_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 33, "num_var");
    SHOULD_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value1.Clear();
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value1 = 45;
    value2 = -10;
    value2 = value1;
    SHOULD_EQUAL_TRUE(value2.IsNumber(), "IsNumber()");
    SHOULD_EQUAL(value2.Type(), ValueType::Number, "Type()", "Number");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "45", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 45, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 45, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL(value1.Type(), ValueType::Number, "Type()", "Number");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "45", "str_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 45, "GetNumber()");
    SHOULD_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 45, "num_var");
    SHOULD_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestNumberValue2() {
    Value value1;
    Value value2;

    String str_var;
    double num_var;
    bool   bool_var;

    value1 = 10;
    value2 = Value(value1);
    SHOULD_EQUAL_TRUE(value2.IsNumber(), "IsNumber()");
    SHOULD_EQUAL(value2.Type(), ValueType::Number, "Type()", "Number");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "10", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 10, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 10, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL(value1.Type(), ValueType::Number, "Type()", "Number");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "10", "str_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 10, "GetNumber()");
    SHOULD_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 10, "num_var");
    SHOULD_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value2.Clear();
    value2 = static_cast<Value &&>(value1);
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value2.IsNumber(), "IsNumber()");
    SHOULD_EQUAL(value2.Type(), ValueType::Number, "Type()", "Number");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "10", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 10, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 10, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestNumberValue3() {
    Value value1;

    String str_var;
    double num_var;
    bool   bool_var;

    value1 = ULong{13};
    Value value2(static_cast<Value &&>(value1));
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value2.IsNumber(), "IsNumber()");
    SHOULD_EQUAL(value2.Type(), ValueType::Number, "Type()", "Number");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "13", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 13, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 13, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    value2 = 1213;
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 1213, "GetNumber()");

    value2 = 785;
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 785, "GetNumber()");

    value2 += 105;
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 890, "GetNumber()");

    value2 -= 90;
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 800, "GetNumber()");

    value2 /= 80;
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 10, "GetNumber()");

    value2 *= 3;
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 30, "GetNumber()");

    double d_var = static_cast<double>(value2);
    SHOULD_EQUAL_VALUE(d_var, 30, "static_cast<double>");

    const char *c_var = static_cast<const char *>(value2);
    SHOULD_EQUAL(c_var, nullptr, "static_cast<const char *>", "null");

    bool b_var = static_cast<bool>(value2);
    SHOULD_NOT_EQUAL_TRUE(b_var, "static_cast<bool>");

    value2 = UInt{0};
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");

    value2 = int{-8};
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");

    value2 = Value{double{3.75}};
    SHOULD_EQUAL_TRUE(value2.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 3.75, "GetNumber()");

    END_SUB_TEST;
}

static int TestStringValue1() {
    Value value1;
    Value value2;

    StringStream ss_var;
    String       str_var;
    double       num_var;
    bool         bool_var;

    value1 = "Qentem";
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Type(), ValueType::String, "Type()", "String");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL_VALUE(*(value1.GetString()), "Qentem", "GetString()");
    SHOULD_NOT_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1.Char(), "Qentem", 6), "Compare()");
    SHOULD_EQUAL_VALUE(value1.Length(), 6, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "Qentem", "str_var");
    SHOULD_EQUAL_TRUE(value1.InsertString(ss_var), "GetString(ss_var)");
    SHOULD_EQUAL_VALUE(ss_var.GetString(), "Qentem", "ss_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value1.Clear();
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value1 = "45";
    value2 = "-50";
    value2 = value1;
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Type(), ValueType::String, "Type()", "String");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL_VALUE(*(value1.GetString()), "45", "GetString()");
    SHOULD_NOT_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1.Char(), "45", 2), "Compare()");
    SHOULD_EQUAL_VALUE(value1.Length(), 2, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "45", "str_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 45, "num_var");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value2.IsString(), "IsString()");
    SHOULD_EQUAL(value2.Type(), ValueType::String, "Type()", "String");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL_VALUE(*(value2.GetString()), "45", "GetString()");
    SHOULD_NOT_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_TRUE(Memory::Compare(value2.Char(), "45", 2), "Compare()");
    SHOULD_EQUAL_VALUE(value2.Length(), 2, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "45", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 45, "num_var");
    SHOULD_NOT_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestStringValue2() {
    Value value1;
    Value value2;

    String str_var;
    double num_var;
    bool   bool_var;

    value1 = "true";
    value2 = Value(value1);
    SHOULD_EQUAL_TRUE(value2.IsString(), "IsString()");
    SHOULD_EQUAL(value2.Type(), ValueType::String, "Type()", "String");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL_VALUE(*(value2.GetString()), "true", "GetString()");
    SHOULD_NOT_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_TRUE(Memory::Compare(value2.Char(), "true", 4), "Compare()");
    SHOULD_EQUAL_VALUE(value2.Length(), 4, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "true", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    bool_var = false;
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Type(), ValueType::String, "Type()", "String");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL_VALUE(*(value1.GetString()), "true", "GetString()");
    SHOULD_NOT_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1.Char(), "true", 4), "Compare()");
    SHOULD_EQUAL_VALUE(value1.Length(), 4, "Length()");
    SHOULD_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "true", "str_var");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value2.Clear();
    value2 = static_cast<Value &&>(value1);
    SHOULD_EQUAL_TRUE(value2.IsString(), "IsString()");
    SHOULD_EQUAL(value2.Type(), ValueType::String, "Type()", "String");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL_VALUE(*(value2.GetString()), "true", "GetString()");
    SHOULD_NOT_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_TRUE(Memory::Compare(value2.Char(), "true", 4), "Compare()");
    SHOULD_EQUAL_VALUE(value2.Length(), 4, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "true", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestStringValue3() {
    Value value1;

    String str_var;
    double num_var;
    bool   bool_var;

    value1 = "false";
    Value value2(static_cast<Value &&>(value1));
    SHOULD_EQUAL_TRUE(value2.IsString(), "IsString()");
    SHOULD_EQUAL(value2.Type(), ValueType::String, "Type()", "String");
    SHOULD_EQUAL_VALUE(value2.Size(), 0, "Type()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL_VALUE(*(value2.GetString()), "false", "GetString()");
    SHOULD_NOT_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_TRUE(Memory::Compare(value2.Char(), "false", 5), "Compare()");
    SHOULD_EQUAL_VALUE(value2.Length(), 5, "Length()");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "false", "str_var");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_EQUAL_VALUE(num_var, 0, "num_var");
    bool_var = true;
    SHOULD_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_NOT_EQUAL_TRUE(bool_var, "bool_var");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    str_var           = "qen";
    const char *c_str = str_var.Char();
    value2            = str_var; // Copy of a string
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "qen", "str_var");
    SHOULD_NOT_EQUAL(value2.Char(), c_str, "str_var.Char()", "c_str");

    str_var = "Qen";
    c_str   = str_var.Char();
    value2  = static_cast<String &&>(str_var); // Move
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "Qen", "str_var");
    SHOULD_EQUAL(value2.Char(), c_str, "str_var.Char()", "c_str");

    value2 += "tem";
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "Qentem", "str_var");

    str_var = " 3";
    value2 += str_var;
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "Qentem 3", "str_var");

    double d_var = static_cast<double>(value2);
    SHOULD_EQUAL_VALUE(d_var, 0, "static_cast<double>");

    const char *c_var = static_cast<const char *>(value2);
    SHOULD_NOT_EQUAL(c_var, nullptr, "static_cast<const char *>", "null");
    SHOULD_EQUAL(value2.Char(), c_var, "str_var.Char()", "c_str");

    bool b_var = static_cast<bool>(value2);
    SHOULD_NOT_EQUAL_TRUE(b_var, "static_cast<bool>");

    String *str_p = HAllocator::Allocate(String("ABC"));
    value2        = Value{str_p};
    SHOULD_EQUAL_TRUE(value2.IsString(), "IsString()");
    SHOULD_NOT_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.GetString(), str_p, "GetString()", "str_p");
    SHOULD_EQUAL_VALUE(*(value2.GetString()), "ABC", "GetString()");

    value2 = String("123");
    SHOULD_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(str_var, "123", "str_var");
    d_var = static_cast<double>(value2);
    SHOULD_EQUAL_VALUE(d_var, 0, "static_cast<double>");

    END_SUB_TEST;
}

static int TestArrayValue1() {
    Value value1;
    Value value2;

    Array<Value> arr_var;
    const Value *storage;
    String       str_var;
    double       num_var;
    bool         bool_var;

    arr_var.ResizeAndInitialize(5);
    storage = arr_var.Storage();

    value1 = arr_var; // Copy.
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL(value1.Type(), ValueType::Array, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value1.Size(), 5, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetValue(4), nullptr, "GetValue(4)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), storage,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "[]", "Stringify()");

    value1.Clear();
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    arr_var.ResizeAndInitialize(10);
    storage = arr_var.Storage();

    Array<Value> arr_var2;
    arr_var2.ResizeAndInitialize(10);

    value1 = static_cast<Array<Value> &&>(arr_var); // Move
    value2 = static_cast<Array<Value> &&>(arr_var2);
    value2 = value1;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL(value1.Type(), ValueType::Array, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value1.Size(), 10, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetValue(9), nullptr, "GetValue(9)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), storage, "GetArray()->Storage()",
                 "storage");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "[]", "Stringify()");

    SHOULD_EQUAL_TRUE(value2.IsArray(), "IsArray()");
    SHOULD_EQUAL(value2.Type(), ValueType::Array, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value2.Size(), 10, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetValue(4), nullptr, "GetValue(4)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_NOT_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value2.GetArray()->Storage(), storage,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "[]", "Stringify()");

    END_SUB_TEST;
}

static int TestArrayValue2() {
    Value value1;
    Value value2;

    Array<Value> arr_var;
    const Value *storage;
    String       str_var;
    double       num_var;
    bool         bool_var;

    arr_var.ResizeAndInitialize(7);
    storage = arr_var.Storage();
    value1  = static_cast<Array<Value> &&>(arr_var);

    value2 = Value(value1);
    SHOULD_EQUAL_TRUE(value2.IsArray(), "IsArray()");
    SHOULD_EQUAL(value2.Type(), ValueType::Array, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value2.Size(), 7, "Size()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetValue(6), nullptr, "GetValue(6)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_NOT_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value2.GetArray()->Storage(), storage,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "[]", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL(value1.Type(), ValueType::Array, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value1.Size(), 7, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetValue(6), nullptr, "GetValue(6)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), storage, "GetArray()->Storage()",
                 "storage");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "[]", "Stringify()");

    value2.Clear();
    value2 = static_cast<Value &&>(value1);
    SHOULD_EQUAL_TRUE(value2.IsArray(), "IsArray()");
    SHOULD_EQUAL(value2.Type(), ValueType::Array, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value2.Size(), 7, "Size()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetValue(6), nullptr, "GetValue(6)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_NOT_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value2.GetArray()->Storage(), storage, "GetArray()->Storage()",
                 "storage");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "[]", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestArrayValue3() {
    Value value1;

    Array<Value> arr_var;
    const Value *storage;
    String       str_var;
    double       num_var;
    bool         bool_var;

    arr_var.ResizeAndInitialize(7);
    storage = arr_var.Storage();
    value1  = static_cast<Array<Value> &&>(arr_var);

    Value value2(static_cast<Value &&>(value1));
    SHOULD_EQUAL_TRUE(value2.IsArray(), "IsArray()");
    SHOULD_EQUAL(value2.Type(), ValueType::Array, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value2.Size(), 7, "Size()");
    SHOULD_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value2.GetValue(6), nullptr, "GetValue(6)", "null");
    SHOULD_EQUAL(value2.GetValue(10), nullptr, "GetValue(6)", "null");
    SHOULD_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_NOT_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value2.GetArray()->Storage(), storage, "GetArray()->Storage()",
                 "storage");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "[]", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    arr_var.ResizeAndInitialize(3);
    storage = arr_var.Storage();
    value2  = arr_var; // Copy
    SHOULD_EQUAL_TRUE(value2.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value2.Size(), 3, "Size()");
    SHOULD_NOT_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value2.GetArray()->Storage(), storage,
                     "GetArray()->Storage()", "storage");

    arr_var.ResizeAndInitialize(13);
    storage = arr_var.Storage();
    value2  = static_cast<Array<Value> &&>(arr_var); // Move
    SHOULD_NOT_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_TRUE(value2.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value2.Size(), 13, "Size()");
    SHOULD_NOT_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value2.GetArray()->Storage(), storage, "GetArray()->Storage()",
                 "storage");

    double d_var = static_cast<double>(value2);
    SHOULD_EQUAL_VALUE(d_var, 0, "static_cast<double>");

    const char *c_var = static_cast<const char *>(value2);
    SHOULD_EQUAL(c_var, nullptr, "static_cast<const char *>", "null");

    bool b_var = static_cast<bool>(value2);
    SHOULD_NOT_EQUAL_TRUE(b_var, "static_cast<bool>");

    Array<Value> *arr_p = HAllocator::Allocate(Array<Value>(3));
    value2              = Value{arr_p};
    SHOULD_EQUAL_TRUE(value2.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetArray(), arr_p, "GetArray()", "arr_p");
    SHOULD_NOT_EQUAL(value2.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL_VALUE(value2.GetArray()->Capacity(), 3,
                       "GetArray()->Capacity()");

    END_SUB_TEST;
}

static int TestArrayValue4() {
    Value        value1;
    Value        value2;
    String       str_var;
    const char * str_c;
    Array<Value> arr_var;

    value1[0] = 1;

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 1, "Size()");
    SHOULD_EQUAL_VALUE(value1[0].GetNumber(), 1, R"(value1[0].GetNumber())");

    value1.Clear();

    value1[0] = 1;
    value1[1] = 2;
    value1[2] = 3;

    SHOULD_EQUAL_VALUE(value1.Size(), 3, "value1.Size()");

    value2 = value1;
    SHOULD_EQUAL_VALUE(value2.Size(), 3, "value2.Size()");
    SHOULD_EQUAL_VALUE(value2[0].GetNumber(), 1, "value2[0].GetNumber()");
    SHOULD_EQUAL_VALUE(value2[1].GetNumber(), 2, "value2[1].GetNumber()");
    SHOULD_EQUAL_VALUE(value2[2].GetNumber(), 3, "value2[2].GetNumber()");

    value1[0] = 11;
    value1[1] = 22;
    str_var   = "Qen";
    str_c     = str_var.Char();
    value1[2] = static_cast<String &&>(str_var);

    value2 = value1;
    SHOULD_EQUAL_VALUE(value2.Size(), 3, "value2.Size()");
    SHOULD_EQUAL_VALUE(value2[0].GetNumber(), 11, "value2[0].GetNumber()");
    SHOULD_EQUAL_VALUE(value2[1].GetNumber(), 22, "value2[1].GetNumber()");
    SHOULD_NOT_EQUAL(value2[2].Char(), str_c, "value2[2].Char()", "str_c");
    SHOULD_EQUAL(Memory::Compare(value2[2].Char(), "Qen", 3), true,
                 "value2[2].Char()", "Qen");

    value2[3] = 44;
    value2[4] = 55;
    SHOULD_EQUAL_VALUE(value2.Size(), 5, "value2.Size()");
    SHOULD_EQUAL_VALUE(value2[0].GetNumber(), 11, "value2[0].GetNumber()");
    SHOULD_EQUAL_VALUE(value2[1].GetNumber(), 22, "value2[1].GetNumber()");
    SHOULD_NOT_EQUAL(value2[2].Char(), str_c, "value2[2].Char()", "str_c");
    SHOULD_EQUAL(Memory::Compare(value2[2].Char(), "Qen", 3), true,
                 "value2[2].Char()", "Qen");
    SHOULD_EQUAL_VALUE(value2[3].GetNumber(), 44, "value2[3].GetNumber()");
    SHOULD_EQUAL_VALUE(value2[4].GetNumber(), 55, "value2[4].GetNumber()");

    value2 = value1;
    SHOULD_EQUAL_VALUE(value2.Size(), 3, "value2.Size()");
    SHOULD_EQUAL_VALUE(value2[0].GetNumber(), 11, "value2[0].GetNumber()");
    SHOULD_EQUAL_VALUE(value2[1].GetNumber(), 22, "value2[1].GetNumber()");
    SHOULD_NOT_EQUAL(value2[2].Char(), str_c, "value2[2].Char()", "str_c");
    SHOULD_EQUAL(Memory::Compare(value2[2].Char(), "Qen", 3), true,
                 "value2[2].Char()", "Qen");
    SHOULD_EQUAL(value2.GetValue(3), nullptr, "value2.GetValue(3)", "null");
    SHOULD_EQUAL(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null");

    ////////////////////
    arr_var.ResizeAndInitialize(4);
    arr_var[0] = 10;
    arr_var[1] = 20;
    arr_var[2] = 30;
    str_var    = "Qentem";
    str_c      = str_var.Char();
    arr_var[3] = static_cast<String &&>(str_var);

    value2 = arr_var;
    SHOULD_EQUAL_VALUE(value2.Size(), 4, "value2.Size()");
    SHOULD_EQUAL_VALUE(value2[0].GetNumber(), 10, "value2[0].GetNumber()");
    SHOULD_EQUAL_VALUE(value2[1].GetNumber(), 20, "value2[1].GetNumber()");
    SHOULD_EQUAL_VALUE(value2[2].GetNumber(), 30, "value2[1].GetNumber()");
    SHOULD_NOT_EQUAL(value2[3].Char(), str_c, "value2[3].Char()", "str_c");
    SHOULD_EQUAL(Memory::Compare(value2[3].Char(), "Qentem", 6), true,
                 "value2[3].Char()", "Qentem");
    SHOULD_EQUAL(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null");

    ////////////////////

    arr_var.SetCapacity(10);
    arr_var.SoftResize(5);
    value2 = static_cast<Array<Value> &&>(arr_var);
    SHOULD_EQUAL_VALUE(value2.Size(), 5, "value2.Size()");

    value2.Compress();
    SHOULD_NOT_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL_VALUE(value2.GetArray()->Capacity(), 5, "Capacity()");

    END_SUB_TEST;
}

static int TestObjectValue1() {
    Value value1;
    Value value2;

    using ObjectItem_ = HAItem<Value>;

    HArray<Value>      h_arr_var;
    const ObjectItem_ *storage;
    String             str_var;
    double             num_var;
    bool               bool_var;

    for (UInt i = 0; i < 5; i++) {
        String key("Key_");
        key += Digit::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.Storage();

    value1 = h_arr_var; // Copy.
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_EQUAL(value1.Type(), ValueType::Object, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value1.Size(), 5, "Size()");
    SHOULD_NOT_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_NOT_EQUAL(value1.GetValue(4), nullptr, "GetValue(4)", "null");
    SHOULD_NOT_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_NOT_EQUAL(value1.GetKey(4), nullptr, "GetKey(4)", "null");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), storage,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    h_arr_var.Clear();
    value1 = h_arr_var;
    SHOULD_EQUAL_VALUE(value1.Stringify(), "{}", "Stringify()");

    value1.Clear();
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    value2 = h_arr_var;

    h_arr_var.Clear();
    for (UInt i = 0; i < 10; i++) {
        String key("Key_");
        key += Digit::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.Storage();

    value1 = static_cast<HArray<Value> &&>(h_arr_var); // Move
    value2 = value1;
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_EQUAL(value1.Type(), ValueType::Object, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value1.Size(), 10, "Size()");
    SHOULD_NOT_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_NOT_EQUAL(value1.GetValue(9), nullptr, "GetValue(9)", "null");
    SHOULD_NOT_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_NOT_EQUAL(value1.GetKey(9), nullptr, "GetKey(9)", "null");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), storage,
                 "GetArray()->Storage()", "storage");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    value1 = h_arr_var;
    SHOULD_EQUAL_VALUE(value1.Stringify(), "{}", "Stringify()");

    SHOULD_EQUAL_TRUE(value2.IsObject(), "IsObject()");
    SHOULD_EQUAL(value2.Type(), ValueType::Object, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value2.Size(), 10, "Size()");
    SHOULD_NOT_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_NOT_EQUAL(value2.GetValue(9), nullptr, "GetValue(9)", "null");
    SHOULD_NOT_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_NOT_EQUAL(value2.GetKey(9), nullptr, "GetKey(9)", "null");
    SHOULD_EQUAL(value2.GetKey(10), nullptr, "GetKey(10)", "null");
    SHOULD_NOT_EQUAL(value2.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), storage,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");

    h_arr_var.Clear();
    // Testing empty values
    for (UInt i = 0; i < 10; i++) {
        String key("Key_");
        key += Digit::NumberToString(i);
        h_arr_var[key];
    }

    value2 = h_arr_var;
    SHOULD_EQUAL_VALUE(value2.Size(), 10, "Size()");
    SHOULD_EQUAL_VALUE(value2.Stringify(), "{}", "Stringify()");

    END_SUB_TEST;
}

static int TestObjectValue2() {
    Value value1;
    Value value2;

    using ObjectItem_ = HAItem<Value>;

    HArray<Value>      h_arr_var;
    const ObjectItem_ *storage;
    String             str_var;
    double             num_var;
    bool               bool_var;

    for (ULong i = 0; i < 7; i++) {
        String key("Key_");
        key += Digit::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.Storage();
    value1  = static_cast<HArray<Value> &&>(h_arr_var);

    value2 = Value(value1);
    SHOULD_EQUAL_TRUE(value2.IsObject(), "IsObject()");
    SHOULD_EQUAL(value2.Type(), ValueType::Object, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value2.Size(), 7, "Size()");
    SHOULD_NOT_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_NOT_EQUAL(value2.GetValue(6), nullptr, "GetValue(6)", "null");
    SHOULD_NOT_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_NOT_EQUAL(value2.GetKey(6), nullptr, "GetKey(6)", "null");
    SHOULD_NOT_EQUAL(value2.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), storage,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    h_arr_var.Clear();
    value2 = h_arr_var;
    SHOULD_EQUAL_VALUE(value2.Stringify(), "{}", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_EQUAL(value1.Type(), ValueType::Object, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value1.Size(), 7, "Size()");
    SHOULD_NOT_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_NOT_EQUAL(value1.GetValue(6), nullptr, "GetValue(6)", "null");
    SHOULD_NOT_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_NOT_EQUAL(value1.GetKey(6), nullptr, "GetKey(6)", "null");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), storage,
                 "GetArray()->Storage()", "storage");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    h_arr_var.Clear();
    value1 = h_arr_var;
    SHOULD_EQUAL_VALUE(value1.Stringify(), "{}", "Stringify()");

    for (UInt i = 0; i < 7; i++) {
        String key("Key_");
        key += Digit::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.Storage();
    value1  = static_cast<HArray<Value> &&>(h_arr_var);

    value2.Clear();
    value2 = static_cast<Value &&>(value1);
    SHOULD_EQUAL_TRUE(value2.IsObject(), "IsObject()");
    SHOULD_EQUAL(value2.Type(), ValueType::Object, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value2.Size(), 7, "Size()");
    SHOULD_NOT_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_NOT_EQUAL(value2.GetValue(6), nullptr, "GetValue(6)", "null");
    SHOULD_NOT_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_NOT_EQUAL(value2.GetKey(6), nullptr, "GetKey(6)", "null");
    SHOULD_NOT_EQUAL(value2.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value2.GetObject()->Storage(), storage,
                 "GetArray()->Storage()", "storage");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    h_arr_var.Clear();
    value2 = h_arr_var;
    SHOULD_EQUAL_VALUE(value2.Stringify(), "{}", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    END_SUB_TEST;
}

static int TestObjectValue3() {
    Value value1;

    using ObjectItem_ = HAItem<Value>;

    HArray<Value>      h_arr_var;
    const ObjectItem_ *storage;
    String             str_var;
    double             num_var;
    bool               bool_var;

    for (UInt i = 0; i < 7; i++) {
        String key("Key_");
        key += Digit::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.Storage();
    value1  = static_cast<HArray<Value> &&>(h_arr_var);

    Value value2(static_cast<Value &&>(value1));
    SHOULD_EQUAL_TRUE(value2.IsObject(), "IsObject()");
    SHOULD_EQUAL(value2.Type(), ValueType::Object, "Type()", "Array");
    SHOULD_EQUAL_VALUE(value2.Size(), 7, "Size()");
    SHOULD_NOT_EQUAL(value2.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_NOT_EQUAL(value2.GetValue(6), nullptr, "GetValue(6)", "null");
    SHOULD_NOT_EQUAL(value2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_NOT_EQUAL(value2.GetKey(6), nullptr, "GetKey(6)", "null");
    SHOULD_NOT_EQUAL(value2.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value2.GetObject()->Storage(), storage,
                 "GetArray()->Storage()", "storage");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value2.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value2.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value2.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value2.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value2.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value2.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value2.GetBool(bool_var), "GetBool(bool_var)");
    h_arr_var.Clear();
    value2 = h_arr_var;
    SHOULD_EQUAL_VALUE(value2.Stringify(), "{}", "Stringify()");

    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "isUndefined()");
    SHOULD_EQUAL(value1.Type(), ValueType::Undefined, "Type()", "Undefined");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_EQUAL(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value1.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetString(), nullptr, "GetString()", "null");
    SHOULD_EQUAL(value1.Char(), nullptr, "Char()", "null");
    SHOULD_EQUAL_VALUE(value1.Length(), 0, "Length()");
    SHOULD_NOT_EQUAL_TRUE(value1.SetString(str_var), "GetString(str_var)");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 0, "GetNumber()");
    SHOULD_NOT_EQUAL_TRUE(value1.GetNumber(num_var), "GetNumber(num_var)");
    SHOULD_NOT_EQUAL_TRUE(value1.GetBool(bool_var), "GetBool(bool_var)");
    SHOULD_EQUAL_VALUE(value1.Stringify(), "", "Stringify()");

    h_arr_var.Clear();
    for (UInt i = 0; i < 3; i++) {
        String key("Key_");
        key += Digit::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.Storage();
    value2  = h_arr_var; // Copy
    SHOULD_EQUAL_VALUE(value2.Size(), 3, "Size()");
    SHOULD_NOT_EQUAL(value2.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), storage,
                     "GetArray()->Storage()", "storage");

    for (UInt i = 0; i < 13; i++) {
        String key("Key_");
        key += Digit::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.Storage();
    value2  = static_cast<HArray<Value> &&>(h_arr_var); // Move
    SHOULD_EQUAL_TRUE(value2.IsObject(), "IsObject()");
    SHOULD_EQUAL_VALUE(value2.Size(), 13, "Size()");
    SHOULD_NOT_EQUAL(value2.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value2.GetObject()->Storage(), storage,
                 "GetArray()->Storage()", "storage");

    double d_var = static_cast<double>(value2);
    SHOULD_EQUAL_VALUE(d_var, 0, "static_cast<double>");

    const char *c_var = static_cast<const char *>(value2);
    SHOULD_EQUAL(c_var, nullptr, "static_cast<const char *>", "null");

    bool b_var = static_cast<bool>(value2);
    SHOULD_NOT_EQUAL_TRUE(b_var, "static_cast<bool>");

    HArray<Value> *h_arr_p = HAllocator::Allocate(HArray<Value>(4));
    value2                 = Value{h_arr_p};
    SHOULD_EQUAL_TRUE(value2.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_EQUAL(value2.GetObject(), h_arr_p, "GetObject()", "h_arr_p");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_EQUAL_VALUE(value2.GetObject()->Capacity(), 4,
                       "GetArray()->Capacity()");

    END_SUB_TEST;
}

static int TestObjectValue4() {
    Value         value1;
    Value         value2;
    String        str_var;
    const char *  str_c;
    HArray<Value> h_arr_var;

    value1[""] = 1;

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_EQUAL_VALUE(value1.Size(), 1, "Size()");
    SHOULD_EQUAL_VALUE(value1[""].GetNumber(), 1, R"(value1[""].GetNumber())");

    value1.Clear();

    value1["k1"] = 1;
    value1["k2"] = 2;
    value1["k3"] = 3;

    SHOULD_EQUAL_VALUE(value1.Size(), 3, "value1.Size()");

    value2 = value1;
    SHOULD_EQUAL_VALUE(value2.Size(), 3, "value2.Size()");
    SHOULD_EQUAL_VALUE(value2["k1"].GetNumber(), 1,
                       "value2[\"k1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["k2"].GetNumber(), 2,
                       "value2[\"k2\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["k3"].GetNumber(), 3,
                       "value2[\"k3\"].GetNumber()");

    value1["k1"] = 11;
    value1["k2"] = 22;
    str_var      = "Qen";
    str_c        = str_var.Char();
    value1["k3"] = static_cast<String &&>(str_var);

    value2 = value1;
    SHOULD_EQUAL_VALUE(value2.Size(), 3, "value2.Size()");
    SHOULD_EQUAL_VALUE(value2["k1"].GetNumber(), 11,
                       "value2[\"k1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["k2"].GetNumber(), 22,
                       "value2[\"k2\"].GetNumber()");
    SHOULD_NOT_EQUAL(value2["k3"].Char(), str_c, "value2[\"k3\"].Char()",
                     "str_c");
    SHOULD_EQUAL(Memory::Compare(value2["k3"].Char(), "Qen", 3), true,
                 "value2[\"k3\"].Char()", "Qen");

    value2["k4"] = 44;
    value2["k5"] = 55;
    SHOULD_EQUAL_VALUE(value2.Size(), 5, "value2.Size()");
    SHOULD_EQUAL_VALUE(value2["k1"].GetNumber(), 11,
                       "value2[\"k1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["k2"].GetNumber(), 22,
                       "value2[\"k2\"].GetNumber()");
    SHOULD_NOT_EQUAL(value2["k3"].Char(), str_c, "value2[\"k3\"].Char()",
                     "str_c");
    SHOULD_EQUAL(Memory::Compare(value2["k3"].Char(), "Qen", 3), true,
                 "value2[\"k3\"].Char()", "Qen");
    SHOULD_EQUAL_VALUE(value2["k4"].GetNumber(), 44,
                       "value2[\"k4\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["k5"].GetNumber(), 55,
                       "value2[\"k5\"].GetNumber()");

    value2 = value1;
    SHOULD_EQUAL_VALUE(value2.Size(), 3, "value2.Size()");
    SHOULD_EQUAL_VALUE(value2["k1"].GetNumber(), 11,
                       "value2[\"k1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["k2"].GetNumber(), 22,
                       "value2[\"k2\"].GetNumber()");
    SHOULD_EQUAL(Memory::Compare(value2["k3"].Char(), "Qen", 3), true,
                 "value2[\"k3\"].Char()", "Qen");
    SHOULD_EQUAL(value2.GetValue("k4", 2), nullptr,
                 "value2.GetValue(\"k4\", 2)", "null");
    SHOULD_EQUAL(value2.GetValue(3), nullptr, "value2.GetValue(3)", "null");
    SHOULD_EQUAL(value2.GetValue("k5", 2), nullptr,
                 "value2.GetValue(\"k5\", 2)", "null");
    SHOULD_EQUAL(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null");

    ////////////////////
    h_arr_var[String("w1")] = 10;
    h_arr_var[String("w2")] = 20;
    h_arr_var[String("w3")] = 30;
    str_var                 = "Qentem";
    str_c                   = str_var.Char();
    h_arr_var[String("w4")] = static_cast<String &&>(str_var);

    value2 = h_arr_var;
    SHOULD_EQUAL_VALUE(value2.Size(), 4, "value2.Size()");
    SHOULD_EQUAL_VALUE(value2["w1"].GetNumber(), 10, "[\"w1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w2"].GetNumber(), 20, "[\"w2\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w3"].GetNumber(), 30, "[\"w3\"].GetNumber()");
    SHOULD_NOT_EQUAL(value2["w4"].Char(), str_c, "value2[\"w4\"].Char()",
                     "str_c");
    SHOULD_EQUAL(Memory::Compare(value2["w4"].Char(), "Qentem", 6), true,
                 "value2[\"w4\"].Char()", "Qen");
    SHOULD_EQUAL(value2.GetValue("k5", 2), nullptr,
                 "value2.GetValue(\"k5\", 2)", "null");
    SHOULD_EQUAL(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null");
    ////////////////////

    h_arr_var.SetCapacity(10);
    h_arr_var[String("w1")] = 10;
    h_arr_var[String("w2")] = 20;
    h_arr_var[String("w3")] = 30;

    value2 = static_cast<HArray<Value> &&>(h_arr_var);
    SHOULD_EQUAL_VALUE(value2.Size(), 3, "value2.Size()");

    value2.Compress();
    SHOULD_NOT_EQUAL(value2.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL_VALUE(value2.GetObject()->Capacity(), 3, "Capacity()");

    END_SUB_TEST;
}

static int TestMoveValue1() {
    Value value1;

    // true
    // false
    // null

    String      str_var;
    const char *c_str_var; // = str_var.Char();

    Array<Value> arr_var;
    const Value *arr_storage; // = arr_var.Storage();

    HArray<Value>        h_arr_var;
    const HAItem<Value> *h_arr_storage; // = h_arr_var.Storage();

    ////////////////////////////////////////////

    value1 = true;

    value1 = false;
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = true;

    value1 = nullptr;
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = true;

    value1 = 11;
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 11, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = true;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value1    = static_cast<String &&>(str_var);
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = true;

    arr_var     = Array<Value>(1);
    arr_storage = arr_var.Storage();
    value1      = static_cast<Array<Value> &&>(arr_var);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), arr_storage,
                 "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = true;

    h_arr_var     = HArray<Value>(1);
    h_arr_storage = h_arr_var.Storage();
    value1        = static_cast<HArray<Value> &&>(h_arr_var);
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                 "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = false;

    value1 = true;
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = false;

    value1 = nullptr;
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = false;

    value1 = -90;
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), -90, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = false;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value1    = static_cast<String &&>(str_var);
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = false;

    arr_var     = Array<Value>(1);
    arr_storage = arr_var.Storage();
    value1      = static_cast<Array<Value> &&>(arr_var);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), arr_storage,
                 "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = false;

    h_arr_var     = HArray<Value>(1);
    h_arr_storage = h_arr_var.Storage();
    value1        = static_cast<HArray<Value> &&>(h_arr_var);
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                 "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = nullptr;

    value1 = true;
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = nullptr;

    value1 = false;
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = nullptr;

    value1 = 7.5;
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 7.5, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = nullptr;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value1    = static_cast<String &&>(str_var);
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = nullptr;

    arr_var     = Array<Value>(1);
    arr_storage = arr_var.Storage();
    value1      = static_cast<Array<Value> &&>(arr_var);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), arr_storage,
                 "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = nullptr;

    h_arr_var     = HArray<Value>(1);
    h_arr_storage = h_arr_var.Storage();
    value1        = static_cast<HArray<Value> &&>(h_arr_var);
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                 "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = 13;

    value1 = true;
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = 40;

    value1 = false;
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = 33;

    value1 = nullptr;
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = 47;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value1    = static_cast<String &&>(str_var);
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = 10e10;

    arr_var     = Array<Value>(1);
    arr_storage = arr_var.Storage();
    value1      = static_cast<Array<Value> &&>(arr_var);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), arr_storage,
                 "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = 9.1;

    h_arr_var     = HArray<Value>(1);
    h_arr_storage = h_arr_var.Storage();
    value1        = static_cast<HArray<Value> &&>(h_arr_var);
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                 "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = String(1);

    value1 = true;
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = String(1);

    value1 = false;
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = String(1);

    value1 = nullptr;
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = String(1);

    value1 = 4;
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 4, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = String(1);

    arr_var     = Array<Value>(1);
    arr_storage = arr_var.Storage();
    value1      = static_cast<Array<Value> &&>(arr_var);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), arr_storage,
                 "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = String(1);

    h_arr_var     = HArray<Value>(1);
    h_arr_storage = h_arr_var.Storage();
    value1        = static_cast<HArray<Value> &&>(h_arr_var);
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                 "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = Array<Value>(1);

    value1 = true;
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    value1 = false;
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    value1 = nullptr;
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    value1 = 33;
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 33, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    str_var   = String(1);
    c_str_var = str_var.Char();
    value1    = static_cast<String &&>(str_var);
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    h_arr_var     = HArray<Value>(1);
    h_arr_storage = h_arr_var.Storage();
    value1        = static_cast<HArray<Value> &&>(h_arr_var);
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                 "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestMoveValue2() {
    Value value1;

    // true
    // false
    // null

    String      str_var;
    const char *c_str_var; // = str_var.Char();

    Array<Value> arr_var;
    const Value *arr_storage; // = arr_var.Storage();

    HArray<Value> h_arr_var;

    ////////////////////////////////////////////

    value1 = HArray<Value>(1);

    value1 = true;
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    value1 = false;
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    value1 = nullptr;
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    value1 = 33;
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 33, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    str_var   = String(1);
    c_str_var = str_var.Char();
    value1    = static_cast<String &&>(str_var);
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    arr_var     = Array<Value>(1);
    arr_storage = arr_var.Storage();
    value1      = static_cast<Array<Value> &&>(arr_var);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), arr_storage,
                 "GetArray()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestMoveValue3() {
    Value value1;
    Value value2;

    // true
    // false
    // null

    String      str_var;
    const char *c_str_var; // = str_var.Char();

    Array<Value> arr_var;
    const Value *arr_storage; // = arr_var.Storage();

    HArray<Value>        h_arr_var;
    const HAItem<Value> *h_arr_storage; // = h_arr_var.Storage();

    ////////////////////////////////////////////

    value1 = true;

    value2 = false;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = true;

    value2 = nullptr;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = true;

    value2 = 11;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 11, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = true;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value2    = static_cast<String &&>(str_var);
    value1    = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = true;

    arr_var     = Array<Value>(1);
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);
    value1      = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), arr_storage,
                 "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = true;

    h_arr_var     = HArray<Value>(1);
    h_arr_storage = h_arr_var.Storage();
    value2        = static_cast<HArray<Value> &&>(h_arr_var);
    value1        = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                 "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = false;

    value2 = true;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = false;

    value2 = nullptr;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = false;

    value2 = -90;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), -90, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = false;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value2    = static_cast<String &&>(str_var);
    value1    = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = false;

    arr_var     = Array<Value>(1);
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);
    value1      = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), arr_storage,
                 "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = false;

    h_arr_var     = HArray<Value>(1);
    h_arr_storage = h_arr_var.Storage();
    value2        = static_cast<HArray<Value> &&>(h_arr_var);
    value1        = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                 "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = nullptr;

    value2 = true;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = nullptr;

    value2 = false;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = nullptr;

    value2 = 7.5;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 7.5, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = nullptr;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value2    = static_cast<String &&>(str_var);
    value1    = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = nullptr;

    arr_var     = Array<Value>(1);
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);
    value1      = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), arr_storage,
                 "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = nullptr;

    h_arr_var     = HArray<Value>(1);
    h_arr_storage = h_arr_var.Storage();
    value2        = static_cast<HArray<Value> &&>(h_arr_var);
    value1        = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                 "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = 13;

    value2 = true;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = 40;

    value2 = false;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = 33;

    value2 = nullptr;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = 47;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value2    = static_cast<String &&>(str_var);
    value1    = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = 10e10;

    arr_var     = Array<Value>(1);
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);
    value1      = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), arr_storage,
                 "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = 9.1;

    h_arr_var     = HArray<Value>(1);
    h_arr_storage = h_arr_var.Storage();
    value2        = static_cast<HArray<Value> &&>(h_arr_var);
    value1        = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                 "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = String(1);

    value2 = true;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = String(1);

    value2 = false;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = String(1);

    value2 = nullptr;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = String(1);

    value2 = 4;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 4, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = String(1);

    arr_var     = Array<Value>(1);
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);
    value1      = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), arr_storage,
                 "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = String(1);

    h_arr_var     = HArray<Value>(1);
    h_arr_storage = h_arr_var.Storage();
    value2        = static_cast<HArray<Value> &&>(h_arr_var);
    value1        = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                 "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = Array<Value>(1);

    value2 = true;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    value2 = false;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    value2 = nullptr;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    value2 = 33;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 33, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    str_var   = String(1);
    c_str_var = str_var.Char();
    value2    = static_cast<String &&>(str_var);
    value1    = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    h_arr_var     = HArray<Value>(1);
    h_arr_storage = h_arr_var.Storage();
    value2        = static_cast<HArray<Value> &&>(h_arr_var);
    value1        = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                 "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestMoveValue4() {
    Value value1;
    Value value2;

    // true
    // false
    // null

    String      str_var;
    const char *c_str_var; // = str_var.Char();

    Array<Value> arr_var;
    const Value *arr_storage; // = arr_var.Storage();

    HArray<Value> h_arr_var;

    ////////////////////////////////////////////

    value1 = HArray<Value>(1);

    value2 = true;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    value2 = false;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    value2 = nullptr;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    value2 = 33;
    value1 = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 33, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    str_var   = String(1);
    c_str_var = str_var.Char();
    value2    = static_cast<String &&>(str_var);
    value1    = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    arr_var     = Array<Value>(1);
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);
    value1      = static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), arr_storage,
                 "GetArray()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestCopyValue1() {
    Value value1;

    // true
    // false
    // null

    String      str_var;
    const char *c_str_var; // = str_var.Char();

    Array<Value> arr_var;
    const Value *arr_storage; // = arr_var.Storage();

    HArray<Value>        h_arr_var;
    const HAItem<Value> *h_arr_storage; // = h_arr_var.Storage();

    ////////////////////////////////////////////

    value1 = true;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value1    = str_var;
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = true;

    // No values
    arr_var = Array<Value>(1);
    value1  = arr_var;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");
    value1.Clear();
    value1 = true;

    // Has values
    arr_var = Array<Value>();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.Storage();
    value1      = arr_var;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = true;

    // No values
    h_arr_var = HArray<Value>(1);
    value1    = h_arr_var;
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), nullptr,
                 "GetObject()->Storage()", "null");
    value1.Clear();
    value1 = true;

    // Has values
    h_arr_var = HArray<Value>();

    for (UInt i = 0; i < 3; i++) {
        h_arr_var[Digit::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.Storage();
    value1        = h_arr_var;
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                     "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = false;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value1    = str_var;
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = false;

    // No values
    arr_var = Array<Value>(1);
    value1  = arr_var;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");
    value1.Clear();
    value1 = false;

    // Has values
    arr_var = Array<Value>();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.Storage();
    value1      = arr_var;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = false;

    // No values
    h_arr_var = HArray<Value>(1);
    value1    = h_arr_var;
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), nullptr,
                 "GetObject()->Storage()", "null");
    value1.Clear();
    value1 = false;

    // Has values
    h_arr_var = HArray<Value>();

    for (UInt i = 0; i < 3; i++) {
        h_arr_var[Digit::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.Storage();
    value1        = h_arr_var;
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                     "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = nullptr;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value1    = str_var;
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = nullptr;

    // No values
    arr_var = Array<Value>(1);
    value1  = arr_var;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");
    value1.Clear();
    value1 = nullptr;

    // Has values
    arr_var = Array<Value>();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.Storage();
    value1      = arr_var;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = nullptr;

    // No values
    h_arr_var = HArray<Value>(1);
    value1    = h_arr_var;
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), nullptr,
                 "GetObject()->Storage()", "null");
    value1.Clear();
    value1 = nullptr;

    // Has values
    h_arr_var = HArray<Value>();

    for (UInt i = 0; i < 3; i++) {
        h_arr_var[Digit::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.Storage();
    value1        = h_arr_var;
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                     "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = 23;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value1    = str_var;
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = 33;

    // No values
    arr_var = Array<Value>(1);
    value1  = arr_var;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");
    value1.Clear();
    value1 = 33;

    // Has values
    arr_var = Array<Value>();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.Storage();
    value1      = arr_var;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = 34;

    // No values
    h_arr_var = HArray<Value>(1);
    value1    = h_arr_var;
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), nullptr,
                 "GetObject()->Storage()", "null");
    value1.Clear();
    value1 = 34;

    // Has values
    h_arr_var = HArray<Value>();

    for (UInt i = 0; i < 3; i++) {
        h_arr_var[Digit::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.Storage();
    value1        = h_arr_var;
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                     "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestCopyValue2() {
    Value value1;

    // true
    // false
    // null

    String      str_var;
    const char *c_str_var; // = str_var.Char();

    Array<Value> arr_var;
    const Value *arr_storage; // = arr_var.Storage();

    HArray<Value>        h_arr_var;
    const HAItem<Value> *h_arr_storage; // = h_arr_var.Storage();

    ////////////////////////////////////////////
    value1 = String(1);

    // No values
    arr_var = Array<Value>(1);
    value1  = arr_var;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");
    value1.Clear();
    value1 = String(1);

    // Has values
    arr_var = Array<Value>();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.Storage();
    value1      = arr_var;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = String(1);

    // No values
    h_arr_var = HArray<Value>(1);
    value1    = h_arr_var;
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), nullptr,
                 "GetObject()->Storage()", "null");
    value1.Clear();
    value1 = String(1);

    // Has values
    h_arr_var = HArray<Value>();

    for (UInt i = 0; i < 3; i++) {
        h_arr_var[Digit::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.Storage();
    value1        = h_arr_var;
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                     "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = Array<Value>(1);

    str_var   = String(1);
    c_str_var = str_var.Char();
    value1    = str_var;
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    // No values
    h_arr_var = HArray<Value>(1);
    value1    = h_arr_var;
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), nullptr,
                 "GetObject()->Storage()", "null");
    value1.Clear();
    value1 = Array<Value>(1);

    // Has values
    h_arr_var = HArray<Value>();

    for (UInt i = 0; i < 3; i++) {
        h_arr_var[Digit::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.Storage();
    value1        = h_arr_var;
    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                     "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = HArray<Value>(1);

    str_var   = String(1);
    c_str_var = str_var.Char();
    value1    = str_var;
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    // No values
    arr_var = Array<Value>(1);
    value1  = arr_var;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");
    value1.Clear();
    value1 = HArray<Value>(1);

    // Has values
    arr_var = Array<Value>();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.Storage();
    value1      = arr_var;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    value1.Clear();
    ////////////////////////////////////////////
    END_SUB_TEST;
}

static int TestCopyValue3() {
    Value value1;
    Value value2;

    // true
    // false
    // null

    String      str_var;
    const char *c_str_var; // = str_var.Char();

    Array<Value> arr_var;
    const Value *arr_storage; // = arr_var.Storage();

    HArray<Value>        h_arr_var;
    const HAItem<Value> *h_arr_storage; // = h_arr_var.Storage();

    ////////////////////////////////////////////

    value1 = true;

    value2 = false;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = true;

    value2 = nullptr;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = true;

    value2 = 11;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 11, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = true;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value2    = static_cast<String &&>(str_var);
    value1    = value2;
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = true;

    // No values
    value2 = Array<Value>(1);
    value1 = value2;

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");
    value1.Clear();
    value1 = true;

    // Has values
    arr_var = Array<Value>();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);
    value1      = value2;

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = true;

    // No values
    value2 = HArray<Value>(1);
    value1 = value2;

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), nullptr,
                 "GetObject()->Storage()", "null");
    value1.Clear();
    value1 = true;

    // Has values
    h_arr_var = HArray<Value>();

    for (UInt i = 0; i < 3; i++) {
        h_arr_var[Digit::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.Storage();
    value2        = static_cast<HArray<Value> &&>(h_arr_var);
    value1        = value2;

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                     "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = false;

    value2 = true;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = false;

    value2 = nullptr;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = false;

    value2 = -90;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), -90, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = false;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value2    = static_cast<String &&>(str_var);
    value1    = value2;
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = false;

    // No values
    value2 = Array<Value>(1);
    value1 = value2;

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");
    value1.Clear();
    value1 = false;

    // Has values
    arr_var = Array<Value>();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);
    value1      = value2;

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = false;

    // No values
    value2 = HArray<Value>(1);
    value1 = value2;

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), nullptr,
                 "GetObject()->Storage()", "null");
    value1.Clear();
    value1 = false;

    // Has values
    h_arr_var = HArray<Value>();

    for (UInt i = 0; i < 3; i++) {
        h_arr_var[Digit::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.Storage();
    value2        = static_cast<HArray<Value> &&>(h_arr_var);
    value1        = value2;

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                     "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = nullptr;

    value2 = true;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = nullptr;

    value2 = false;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = nullptr;

    value2 = 7.5;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 7.5, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = nullptr;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value2    = static_cast<String &&>(str_var);
    value1    = value2;
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = nullptr;

    // No values
    value2 = Array<Value>(1);
    value1 = value2;

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");
    value1.Clear();
    value1 = nullptr;

    // Has values
    arr_var = Array<Value>();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);
    value1      = value2;

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = nullptr;

    // No values
    value2 = HArray<Value>(1);
    value1 = value2;

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), nullptr,
                 "GetObject()->Storage()", "null");
    value1.Clear();
    value1 = nullptr;

    // Has values
    h_arr_var = HArray<Value>();

    for (UInt i = 0; i < 3; i++) {
        h_arr_var[Digit::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.Storage();
    value2        = static_cast<HArray<Value> &&>(h_arr_var);
    value1        = value2;

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                     "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = 13;

    value2 = true;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = 40;

    value2 = false;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = 33;

    value2 = nullptr;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = 47;

    str_var   = String(1);
    c_str_var = str_var.Char();
    value2    = static_cast<String &&>(str_var);
    value1    = value2;
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = 10e10;

    // No values
    value2 = Array<Value>(1);
    value1 = value2;

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");
    value1.Clear();
    value1 = 10e10;

    // Has values
    arr_var = Array<Value>();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);
    value1      = value2;

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = 9.1;

    // No values
    value2 = HArray<Value>(1);
    value1 = value2;

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), nullptr,
                 "GetObject()->Storage()", "null");
    value1.Clear();
    value1 = 9.1;

    // Has values
    h_arr_var = HArray<Value>();

    for (UInt i = 0; i < 3; i++) {
        h_arr_var[Digit::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.Storage();
    value2        = static_cast<HArray<Value> &&>(h_arr_var);
    value1        = value2;

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                     "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestCopyValue4() {
    Value value1;
    Value value2;

    // true
    // false
    // null

    String      str_var;
    const char *c_str_var; // = str_var.Char();

    Array<Value> arr_var;
    const Value *arr_storage; // = arr_var.Storage();

    HArray<Value>        h_arr_var;
    const HAItem<Value> *h_arr_storage; // = h_arr_var.Storage();

    ////////////////////////////////////////////

    value1 = String(1);

    value2 = true;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = String(1);

    value2 = false;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = String(1);

    value2 = nullptr;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = String(1);

    value2 = 4;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 4, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = String(1);

    // No values
    value2 = Array<Value>(1);
    value1 = value2;

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");
    value1.Clear();
    value1 = String(1);

    // Has values
    arr_var = Array<Value>();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);
    value1      = value2;

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    value1.Clear();

    /////////////
    value1 = String(1);

    // No values
    value2 = HArray<Value>(1);
    value1 = value2;

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), nullptr,
                 "GetObject()->Storage()", "null");
    value1.Clear();
    value1 = String(1);

    // Has values
    h_arr_var = HArray<Value>();

    for (UInt i = 0; i < 3; i++) {
        h_arr_var[Digit::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.Storage();
    value2        = static_cast<HArray<Value> &&>(h_arr_var);
    value1        = value2;

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                     "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = Array<Value>(1);

    value2 = true;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    value2 = false;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    value2 = nullptr;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    value2 = 33;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 33, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    str_var   = String(1);
    c_str_var = str_var.Char();
    value2    = static_cast<String &&>(str_var);
    value1    = value2;
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = Array<Value>(1);

    // No values
    value2 = HArray<Value>(1);
    value1 = value2;

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetObject()->Storage(), nullptr,
                 "GetObject()->Storage()", "null");
    value1.Clear();
    value1 = Array<Value>(1);

    // Has values
    h_arr_var = HArray<Value>();

    for (UInt i = 0; i < 3; i++) {
        h_arr_var[Digit::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.Storage();
    value2        = static_cast<HArray<Value> &&>(h_arr_var);
    value1        = value2;

    SHOULD_EQUAL_TRUE(value1.IsObject(), "IsObject()");
    SHOULD_NOT_EQUAL(value1.GetObject(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), nullptr,
                     "GetObject()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetObject()->Storage(), h_arr_storage,
                     "GetObject()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    value1 = HArray<Value>(1);

    value2 = true;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    value2 = false;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    value2 = nullptr;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsNull(), "IsNull()");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    value2 = 33;
    value1 = value2;
    SHOULD_EQUAL_TRUE(value1.IsNumber(), "IsNumber()");
    SHOULD_EQUAL_VALUE(value1.GetNumber(), 33, "GetNumber()");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    str_var   = String(1);
    c_str_var = str_var.Char();
    value2    = static_cast<String &&>(str_var);
    value1    = value2;
    SHOULD_EQUAL_TRUE(value1.IsString(), "IsString()");
    SHOULD_NOT_EQUAL(value1.Char(), c_str_var, "value1.Char()", "c_str_var");
    value1.Clear();

    /////////////
    value1 = HArray<Value>(1);

    // No values
    value2 = Array<Value>(1);
    value1 = value2;

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value1.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");
    value1.Clear();
    value1 = HArray<Value>(1);

    // Has values
    arr_var = Array<Value>();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);
    value1      = value2;

    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    value1.Clear();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestIndexOperator1() {
    Value  value;
    String str1("D");
    String str2("DEFG");
    bool   did_throw;

    try {
        did_throw = false;
        value[10] = 5;
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    value[0] = 5;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_VALUE(value[0].GetNumber(), 5, "value[0]");

    try {
        did_throw = false;
        value[2]  = 12;
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    try {
        did_throw   = false;
        value[str2] = 120;
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    try {
        did_throw          = false;
        value[String("W")] = 120;
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    try {
        did_throw    = false;
        value["WYX"] = 120;
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    value[0] = 20;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_VALUE(value[0].GetNumber(), 20, "value[0]");

    value[1] = 30;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 2, "Size()");
    SHOULD_EQUAL_VALUE(value[0].GetNumber(), 20, "value[0]");
    SHOULD_EQUAL_VALUE(value[1].GetNumber(), 30, "value[1]");

    value[0]        = 50;
    value[1]        = 100;
    value[2]        = 200;
    value[int{3}]   = 300;
    value[ULong{4}] = 400;
    value[UInt{5}]  = 500;

    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 6, "Size()");
    SHOULD_EQUAL_VALUE(value[0].GetNumber(), 50, "value[0]");
    SHOULD_EQUAL_VALUE(value[1].GetNumber(), 100, "value[1]");
    SHOULD_EQUAL_VALUE(value[2].GetNumber(), 200, "value[2]");
    SHOULD_EQUAL_VALUE(value[3].GetNumber(), 300, "value[3]");
    SHOULD_EQUAL_VALUE(value[4].GetNumber(), 400, "value[4]");
    SHOULD_EQUAL_VALUE(value[5].GetNumber(), 500, "value[5]");
    SHOULD_NOT_EQUAL(value.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_NOT_EQUAL(value.GetValue(1), nullptr, "GetValue(1)", "null");
    SHOULD_NOT_EQUAL(value.GetValue(2), nullptr, "GetValue(2)", "null");
    SHOULD_NOT_EQUAL(value.GetValue(3), nullptr, "GetValue(3)", "null");
    SHOULD_NOT_EQUAL(value.GetValue(4), nullptr, "GetValue(4)", "null");
    SHOULD_NOT_EQUAL(value.GetValue(5), nullptr, "GetValue(5)", "null");
    SHOULD_EQUAL(value.GetValue("5", 1), nullptr, "GetValue(\"5\")", "null");
    SHOULD_EQUAL_VALUE(value.GetValue(0)->GetNumber(), 50, "GetValue(0)");
    SHOULD_EQUAL_VALUE(value.GetValue(1)->GetNumber(), 100, "GetValue(1)");
    SHOULD_EQUAL_VALUE(value.GetValue(2)->GetNumber(), 200, "GetValue(2)");
    SHOULD_EQUAL_VALUE(value.GetValue(3)->GetNumber(), 300, "GetValue(3)");
    SHOULD_EQUAL_VALUE(value.GetValue(4)->GetNumber(), 400, "GetValue(4)");
    SHOULD_EQUAL_VALUE(value.GetValue(5)->GetNumber(), 500, "GetValue(5)");
    try {
        did_throw = false;
        value[7]  = "cc";
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");
    SHOULD_EQUAL(value.GetValue(6), nullptr, "GetValue(6)", "null");

    //////////////////////////////////////////
    value.Clear();

    value["A"] = 7.5;
    SHOULD_EQUAL_TRUE(value.IsObject(), "IsObject()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_VALUE(value[0].GetNumber(), 7.5, "value[0]");
    SHOULD_EQUAL_VALUE(value["A"].GetNumber(), 7.5, "value[\"A\"]");

    try {
        did_throw = false;
        value[1]  = 5;
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    try {
        did_throw = false;
        value[10] = 5;
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    value["A"] = 59;
    SHOULD_EQUAL_TRUE(value.IsObject(), "IsObject()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_VALUE(value[0].GetNumber(), 59, "value[0]");
    SHOULD_EQUAL_VALUE(value["A"].GetNumber(), 59, "value[\"A\"]");

    value["B"] = 60;
    SHOULD_EQUAL_TRUE(value.IsObject(), "IsObject()");
    SHOULD_EQUAL_VALUE(value.Size(), 2, "Size()");
    SHOULD_EQUAL_VALUE(value[0].GetNumber(), 59, "value[0]");
    SHOULD_EQUAL_VALUE(value["A"].GetNumber(), 59, "value[\"A\"]");
    SHOULD_EQUAL_VALUE(value[1].GetNumber(), 60, "value[1]");
    SHOULD_EQUAL_VALUE(value["B"].GetNumber(), 60, "value[\"B\"]");

    value[0]                  = 50;
    value["B"]                = 100;
    value[String("C")]        = 200;
    value[str1]               = 300;
    value["EFEFE"]            = 400;
    value[String("FGHIGKLM")] = 500;

    SHOULD_EQUAL_TRUE(value.IsObject(), "IsObject()");
    SHOULD_EQUAL_VALUE(value.Size(), 6, "Size()");
    SHOULD_EQUAL_VALUE(value[0].GetNumber(), 50, "value[0]");
    SHOULD_EQUAL_VALUE(value[1].GetNumber(), 100, "value[1]");
    SHOULD_EQUAL_VALUE(value[2].GetNumber(), 200, "value[2]");
    SHOULD_EQUAL_VALUE(value[3].GetNumber(), 300, "value[3]");
    SHOULD_EQUAL_VALUE(value[4].GetNumber(), 400, "value[4]");
    SHOULD_EQUAL_VALUE(value[5].GetNumber(), 500, "value[5]");
    SHOULD_EQUAL_VALUE(value["A"].GetNumber(), 50, "value[\"A\"]");
    SHOULD_EQUAL_VALUE(value["B"].GetNumber(), 100, "value[\"B\"]");
    SHOULD_EQUAL_VALUE(value["C"].GetNumber(), 200, "value[\"C\"]");
    SHOULD_EQUAL_VALUE(value["D"].GetNumber(), 300, "value[\"D\"]");
    SHOULD_EQUAL_VALUE(value["EFEFE"].GetNumber(), 400, "value[\"EFEFE\"]");
    SHOULD_EQUAL_VALUE(value["FGHIGKLM"].GetNumber(), 500,
                       "value[\"FGHIGKLM\"]");
    SHOULD_NOT_EQUAL(value.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_NOT_EQUAL(value.GetValue(1), nullptr, "GetValue(1)", "null");
    SHOULD_NOT_EQUAL(value.GetValue(2), nullptr, "GetValue(2)", "null");
    SHOULD_NOT_EQUAL(value.GetValue(3), nullptr, "GetValue(3)", "null");
    SHOULD_NOT_EQUAL(value.GetValue(4), nullptr, "GetValue(4)", "null");
    SHOULD_NOT_EQUAL(value.GetValue(5), nullptr, "GetValue(5)", "null");
    SHOULD_EQUAL_VALUE(value.GetValue(0)->GetNumber(), 50, "GetValue(0)");
    SHOULD_EQUAL_VALUE(value.GetValue(1)->GetNumber(), 100, "GetValue(1)");
    SHOULD_EQUAL_VALUE(value.GetValue(2)->GetNumber(), 200, "GetValue(2)");
    SHOULD_EQUAL_VALUE(value.GetValue(3)->GetNumber(), 300, "GetValue(3)");
    SHOULD_EQUAL_VALUE(value.GetValue(4)->GetNumber(), 400, "GetValue(4)");
    SHOULD_EQUAL_VALUE(value.GetValue(5)->GetNumber(), 500, "GetValue(5)");

    SHOULD_NOT_EQUAL(value.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_NOT_EQUAL(value.GetKey(1), nullptr, "GetKey(1)", "null");
    SHOULD_NOT_EQUAL(value.GetKey(2), nullptr, "GetKey(2)", "null");
    SHOULD_NOT_EQUAL(value.GetKey(3), nullptr, "GetKey(3)", "null");
    SHOULD_NOT_EQUAL(value.GetKey(4), nullptr, "GetKey(4)", "null");
    SHOULD_NOT_EQUAL(value.GetKey(5), nullptr, "GetKey(5)", "null");

    SHOULD_EQUAL_TRUE(value.GetKey(0)->Compare("A", 1), "GetKey(0)->Compare()");
    SHOULD_EQUAL_TRUE(value.GetKey(1)->Compare("B", 1), "GetKey(1)->Compare()");
    SHOULD_EQUAL_TRUE(value.GetKey(2)->Compare("C", 1), "GetKey(2)->Compare()");
    SHOULD_EQUAL_TRUE(value.GetKey(3)->Compare("D", 1), "GetKey(3)->Compare()");
    SHOULD_EQUAL_TRUE(value.GetKey(4)->Compare("EFEFE", 5),
                      "GetKey(4)->Compare()");
    SHOULD_EQUAL_TRUE(value.GetKey(5)->Compare("FGHIGKLM", 8),
                      "GetKey(5)->Compare()");

    try {
        did_throw = false;
        value[6]  = "W";
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");
    SHOULD_EQUAL(value.GetValue(6), nullptr, "GetValue(6)", "null");

    value.Clear();

    value[String("C")] = 4;
    SHOULD_EQUAL_TRUE(value.IsObject(), "IsObject()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_VALUE(value[0].GetNumber(), 4, "value[0]");
    SHOULD_EQUAL_VALUE(value[String("C")].GetNumber(), 4, "value[\"C\"]");

    value.Clear();

    value[str2] = 5;
    SHOULD_EQUAL_TRUE(value.IsObject(), "IsObject()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_VALUE(value[0].GetNumber(), 5, "value[0]");
    SHOULD_EQUAL_VALUE(value[str2].GetNumber(), 5, "value[\"C\"]");

    //////////////////////////////////////////

    END_SUB_TEST;
}

static int TestIndexOperator2() {
    Value  value;
    Value *value_ptr;

    value["A"]    = 10;
    value["BB"]   = 20;
    value["CCCC"] = 30;

    value_ptr = value.GetValue("A", 1);
    SHOULD_NOT_EQUAL(value_ptr, nullptr, "GetValue(\"A\", 1)", "null");
    SHOULD_EQUAL_VALUE(value_ptr->GetNumber(), 10, "GetValue(\"A\", 1)");

    value_ptr = value.GetValue("ABCDEF", 1);
    SHOULD_NOT_EQUAL(value_ptr, nullptr, "GetValue(\"ABCDEF\", 1)", "null");
    SHOULD_EQUAL_VALUE(value_ptr->GetNumber(), 10, "GetValue(\"ABCDEF\", 1)");

    value_ptr = value.GetValue("BB", 2);
    SHOULD_NOT_EQUAL(value_ptr, nullptr, "GetValue(\"BB\", 2)", "null");
    SHOULD_EQUAL_VALUE(value_ptr->GetNumber(), 20, "GetValue(\"BB\", 2)");

    value_ptr = value.GetValue("CCCC", 4);
    SHOULD_NOT_EQUAL(value_ptr, nullptr, "GetValue(\"CCCC\", 4)", "null");
    SHOULD_EQUAL_VALUE(value_ptr->GetNumber(), 30, "GetValue(\"CCCC\", 4)");
    ////////////////////
    SHOULD_EQUAL(value.GetValue("a", 1), nullptr, "GetValue(\"a\", 1)", "null");
    SHOULD_EQUAL(value.GetValue("ABCDEF", 6), nullptr,
                 "GetValue(\"ABCDEF\", 1)", "null");
    SHOULD_EQUAL(value.GetValue("b", 1), nullptr, "GetValue(\"b\", 1)", "null");
    SHOULD_EQUAL(value.GetValue("bb", 2), nullptr, "GetValue(\"bb\", 2)",
                 "null");

    SHOULD_EQUAL(value.GetValue("CCC", 3), nullptr, "GetValue(\"CCCC\", 3)",
                 "null");
    ////////////////////

    END_SUB_TEST;
}

static int TestAddition1() {
    Value  value;
    String str1("D");
    String str2("DEFG");
    bool   did_throw;

    try {
        did_throw = false;
        value[10] = 5;
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    value += 20;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_VALUE(value[0].GetNumber(), 20, "value[0]");

    try {
        did_throw = false;
        value[2]  = 12;
    } catch (...) {
        did_throw = true;
    }

    value += 30;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 2, "Size()");
    SHOULD_EQUAL_VALUE(value[0].GetNumber(), 20, "value[0]");
    SHOULD_EQUAL_VALUE(value[1].GetNumber(), 30, "value[1]");

    value[0] = 50;
    value[1] = 100;
    value += int{200};
    value += UInt{300};
    value += ULong{400};
    value += double{500};

    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 6, "Size()");
    SHOULD_EQUAL_VALUE(value[0].GetNumber(), 50, "value[0]");
    SHOULD_EQUAL_VALUE(value[1].GetNumber(), 100, "value[1]");
    SHOULD_EQUAL_VALUE(value[2].GetNumber(), 200, "value[2]");
    SHOULD_EQUAL_VALUE(value[3].GetNumber(), 300, "value[3]");
    SHOULD_EQUAL_VALUE(value[4].GetNumber(), 400, "value[4]");
    SHOULD_EQUAL_VALUE(value[5].GetNumber(), 500, "value[5]");

    try {
        did_throw = false;
        value[7]  = "cc";
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    //////////////////////////////////////////

    END_SUB_TEST;
}

static int TestAddition2() {
    Value        value;
    Array<Value> arr_var;
    String       str;

    /////////////////

    value += true;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsTrue(), "value[0].IsTrue()");

    value += true;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsTrue(), "value[0].IsTrue()");
    SHOULD_EQUAL_TRUE(value[1].IsTrue(), "value[1].IsTrue()");

    value.Clear();
    /////////////////

    value += false;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsFalse(), "value[0].IsFalse()");

    value += false;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsFalse(), "value[0].IsFalse()");
    SHOULD_EQUAL_TRUE(value[1].IsFalse(), "value[1].IsFalse()");

    value.Clear();
    /////////////////

    value += nullptr;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsNull(), "value[0].IsNull()");

    value += nullptr;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsNull(), "value[0].IsNull()");
    SHOULD_EQUAL_TRUE(value[1].IsNull(), "value[1].IsNull()");

    value.Clear();
    /////////////////

    value += "Qentem";
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsString(), "value[0].IsString()");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[0].Char(), "Qentem", 6),
                      "Memory::Compare");

    value += "Hani";
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsString(), "value[0].IsString()");
    SHOULD_EQUAL_TRUE(value[1].IsString(), "value[1].IsString()");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[0].Char(), "Qentem", 6),
                      "Memory::Compare");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[1].Char(), "Hani", 4),
                      "Memory::Compare");
    value.Clear();
    /////////////////
    str               = String("Qentem");
    const char *c_str = str.Char();
    value += static_cast<String &&>(str);
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsString(), "value[0].IsString()");
    SHOULD_EQUAL(value[0].Char(), c_str, "value[0].Char()", "c_str");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[0].Char(), "Qentem", 6),
                      "Memory::Compare");

    str                = String("Hani");
    const char *c_str2 = str.Char();
    value += static_cast<String &&>(str);
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsString(), "value[0].IsString()");
    SHOULD_EQUAL_TRUE(value[1].IsString(), "value[1].IsString()");
    SHOULD_EQUAL(value[0].Char(), c_str, "value[0].Char()", "c_str");
    SHOULD_EQUAL(value[1].Char(), c_str2, "value[1].Char()", "c_str2");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[0].Char(), "Qentem", 6),
                      "Memory::Compare");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[1].Char(), "Hani", 4),
                      "Memory::Compare");
    value.Clear();
    /////////////////

    str   = String("Qentem");
    c_str = str.Char();
    value += str;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsString(), "value[0].IsString()");
    SHOULD_NOT_EQUAL(value[0].Char(), c_str, "value[0].Char()", "c_str");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[0].Char(), "Qentem", 6),
                      "Memory::Compare");

    str    = String("Hani");
    c_str2 = str.Char();
    value += str;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsString(), "value[0].IsString()");
    SHOULD_EQUAL_TRUE(value[1].IsString(), "value[1].IsString()");
    SHOULD_NOT_EQUAL(value[0].Char(), c_str, "value[0].Char()", "c_str");
    SHOULD_NOT_EQUAL(value[1].Char(), c_str2, "value[1].Char()", "c_str2");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[0].Char(), "Qentem", 6),
                      "Memory::Compare");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[1].Char(), "Hani", 4),
                      "Memory::Compare");
    value.Clear();

    //////////////

    value += HArray<Value>();
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsObject(), "value[0].IsObject()");

    value += HArray<Value>();
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value[0].IsObject(), "value[0].IsObject()");
    SHOULD_EQUAL_TRUE(value[1].IsObject(), "value[1].IsObject()");

    //////////////////////////////////////////

    END_SUB_TEST;
}

static int TestAddition3() {
    Value        value;
    Array<Value> arr_var;
    const Value *arr_storage;
    String       str;
    const char * c_str;
    const char * c_str2;
    /////////////////
    arr_var = Array<Value>(1);
    value += arr_var;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 0, "Size()");
    SHOULD_NOT_EQUAL(value.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");

    arr_var = Array<Value>(3);
    value += arr_var;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 0, "Size()");
    SHOULD_NOT_EQUAL(value.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_EQUAL(value.GetArray()->Storage(), nullptr, "GetArray()->Storage()",
                 "null");

    value.Clear();
    ///

    arr_var += Value{ValueType::False};
    arr_var += Value{ValueType::True};
    arr_var += Value{HAllocator::Allocate(String("Qentem"))};
    c_str       = arr_var[2].Char();
    arr_storage = arr_var.Storage();

    value += arr_var;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 3, "Size()");
    SHOULD_NOT_EQUAL(value.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL_TRUE(value[0].IsFalse(), "value[0].IsFalse()");
    SHOULD_EQUAL_TRUE(value[1].IsTrue(), "value[1].IsTrue()");
    SHOULD_EQUAL_TRUE(value[2].IsString(), "value[2].IsString()");
    SHOULD_NOT_EQUAL(value[2].Char(), c_str, "value[2].Char()", "c_str");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[2].Char(), "Qentem", 6),
                      "Memory::Compare");

    arr_var.Clear();
    arr_var += Value{ValueType::Null};
    arr_var += Value{14};
    arr_var += Value{HAllocator::Allocate(String("Hani"))};
    c_str2      = arr_var[2].Char();
    arr_storage = arr_var.Storage();

    value += arr_var;
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 6, "Size()");
    SHOULD_NOT_EQUAL(value.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL_TRUE(value[0].IsFalse(), "value[0].IsFalse()");
    SHOULD_EQUAL_TRUE(value[1].IsTrue(), "value[1].IsTrue()");
    SHOULD_EQUAL_TRUE(value[2].IsString(), "value[2].IsString()");
    SHOULD_NOT_EQUAL(value[2].Char(), c_str, "value[2].Char()", "c_str");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[2].Char(), "Qentem", 6),
                      "Memory::Compare");
    SHOULD_EQUAL_TRUE(value[3].IsNull(), "value[3].IsNull()");
    SHOULD_EQUAL_TRUE(value[4].IsNumber(), "value1[4].IsNumber()");
    SHOULD_EQUAL_VALUE(value[4].GetNumber(), 14, "value[4].GetNumber()");
    SHOULD_EQUAL_TRUE(value[5].IsString(), "value[5].IsString()");
    SHOULD_NOT_EQUAL(value[5].Char(), c_str2, "value[5].Char()", "c_str2");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[5].Char(), "Hani", 4),
                      "Memory::Compare");

    value.Clear();
    /////////////////
    value += Array<Value>(1);
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 0, "Size()");
    SHOULD_NOT_EQUAL(value.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");

    value += Array<Value>(3);
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 0, "Size()");
    SHOULD_NOT_EQUAL(value.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");

    value.Clear();
    ///

    arr_var.Clear();
    arr_var += Value{ValueType::False};
    arr_var += Value{ValueType::True};
    arr_var += Value{HAllocator::Allocate(String("Qentem"))};
    c_str       = arr_var[2].Char();
    arr_storage = arr_var.Storage();

    value += static_cast<Array<Value> &&>(arr_var);
    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 3, "Size()");
    SHOULD_NOT_EQUAL(value.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value.GetArray()->Storage(), arr_storage,
                 "GetArray()->Storage()", "storage");
    SHOULD_EQUAL_TRUE(value[0].IsFalse(), "value[0].IsFalse()");
    SHOULD_EQUAL_TRUE(value[1].IsTrue(), "value[1].IsTrue()");
    SHOULD_EQUAL_TRUE(value[2].IsString(), "value[2].IsString()");
    SHOULD_EQUAL(value[2].Char(), c_str, "value[2].Char()", "c_str");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[2].Char(), "Qentem", 6),
                      "Memory::Compare");

    arr_var += Value{ValueType::Null};
    arr_var += Value{14};
    arr_var += Value{HAllocator::Allocate(String("Hani"))};
    c_str2      = arr_var[2].Char();
    arr_storage = arr_var.Storage();

    value += static_cast<Array<Value> &&>(arr_var);
    SHOULD_EQUAL(arr_var.Storage(), nullptr, "arr_var.Storage()", "null");

    SHOULD_EQUAL_TRUE(value.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value.Size(), 6, "Size()");
    SHOULD_NOT_EQUAL(value.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL_TRUE(value[0].IsFalse(), "value[0].IsFalse()");
    SHOULD_EQUAL_TRUE(value[1].IsTrue(), "value[1].IsTrue()");
    SHOULD_EQUAL_TRUE(value[2].IsString(), "value[2].IsString()");
    SHOULD_EQUAL(value[2].Char(), c_str, "value[2].Char()", "c_str");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[2].Char(), "Qentem", 6),
                      "Memory::Compare");
    SHOULD_EQUAL_TRUE(value[3].IsNull(), "value[3].IsNull()");
    SHOULD_EQUAL_TRUE(value[4].IsNumber(), "value1[4].IsNumber()");
    SHOULD_EQUAL_VALUE(value[4].GetNumber(), 14, "value[4].GetNumber()");
    SHOULD_EQUAL_TRUE(value[5].IsString(), "value[5].IsString()");
    SHOULD_EQUAL(value[5].Char(), c_str2, "value[5].Char()", "c_str2");
    SHOULD_EQUAL_TRUE(Memory::Compare(value[5].Char(), "Hani", 4),
                      "Memory::Compare");
    //////////////////////////////////////////

    END_SUB_TEST;
}

static int TestAddition4() {
    Value        value1;
    Value        value2;
    Array<Value> arr_var;
    const Value *arr_storage;
    String       str;

    /////////////////

    value2 = true;
    value1 += value2;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsTrue(), "value1[0].IsTrue()");

    value2 = true;
    value1 += value2;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsTrue(), "value1[0].IsTrue()");
    SHOULD_EQUAL_TRUE(value1[1].IsTrue(), "value1[1].IsTrue()");

    value1.Clear();
    /////////////////

    value2 = false;
    value1 += value2;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");

    value2 = false;
    value1 += value2;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");
    SHOULD_EQUAL_TRUE(value1[1].IsFalse(), "value1[1].IsFalse()");

    value1.Clear();
    /////////////////

    value2 = nullptr;
    value1 += value2;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsNull(), "value1[0].IsNull()");

    value2 = nullptr;
    value1 += value2;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsNull(), "value1[0].IsNull()");
    SHOULD_EQUAL_TRUE(value1[1].IsNull(), "value1[1].IsNull()");

    value1.Clear();
    /////////////////
    str               = String("Qentem");
    const char *c_str = str.Char();
    value2            = static_cast<String &&>(str);

    value1 += value2;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsString(), "value1[0].IsString()");
    SHOULD_NOT_EQUAL(value1[0].Char(), c_str, "value1[0].Char()", "c_str");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1[0].Char(), "Qentem", 6),
                      "Memory::Compare");

    str                = String("Hani");
    const char *c_str2 = str.Char();
    value2             = static_cast<String &&>(str);

    value1 += value2;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsString(), "value1[0].IsString()");
    SHOULD_EQUAL_TRUE(value1[1].IsString(), "value1[1].IsString()");
    SHOULD_NOT_EQUAL(value1[0].Char(), c_str, "value1[0].Char()", "c_str");
    SHOULD_NOT_EQUAL(value1[1].Char(), c_str2, "value1[1].Char()", "c_str2");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1[0].Char(), "Qentem", 6),
                      "Memory::Compare");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1[1].Char(), "Hani", 4),
                      "Memory::Compare");
    value1.Clear();
    /////////////////

    value2 = Array<Value>(1);
    value1 += value2;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");

    value2 = Array<Value>(10);
    value1 += value2;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");

    value1.Clear();
    ///

    arr_var += Value{ValueType::False};
    arr_var += Value{ValueType::True};
    arr_var += Value{HAllocator::Allocate(String("Qentem"))};
    c_str = arr_var[2].Char();

    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);

    value1 += value2;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 3, "Size()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");
    SHOULD_EQUAL_TRUE(value1[1].IsTrue(), "value1[1].IsTrue()");
    SHOULD_EQUAL_TRUE(value1[2].IsString(), "value1[2].IsString()");
    SHOULD_NOT_EQUAL(value1[2].Char(), c_str, "value1[0].Char()", "c_str");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1[2].Char(), "Qentem", 6),
                      "Memory::Compare");

    arr_var.Clear();
    arr_var += Value{ValueType::Null};
    arr_var += Value{14};
    arr_var += Value{HAllocator::Allocate(String("Hani"))};
    c_str2      = arr_var[2].Char();
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);

    value1 += value2;
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 6, "Size()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");
    SHOULD_EQUAL_TRUE(value1[1].IsTrue(), "value1[1].IsTrue()");
    SHOULD_EQUAL_TRUE(value1[2].IsString(), "value1[2].IsString()");
    SHOULD_NOT_EQUAL(value1[2].Char(), c_str, "value1[0].Char()", "c_str");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1[2].Char(), "Qentem", 6),
                      "Memory::Compare");
    SHOULD_EQUAL_TRUE(value1[3].IsNull(), "value1[3].IsNull()");
    SHOULD_EQUAL_TRUE(value1[4].IsNumber(), "value1[4].IsNumber()");
    SHOULD_EQUAL_VALUE(value1[4].GetNumber(), 14, "value1[4].GetNumber()");
    SHOULD_EQUAL_TRUE(value1[5].IsString(), "value1[5].IsString()");
    SHOULD_NOT_EQUAL(value1[5].Char(), c_str2, "value1[5].Char()", "c_str2");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1[5].Char(), "Hani", 4),
                      "Memory::Compare");

    value1.Clear();

    //////////////////////////////////////////

    END_SUB_TEST;
}

static int TestAddition5() {
    Value        value1;
    Value        value2;
    Array<Value> arr_var;
    const Value *arr_storage;
    String       str;

    /////////////////

    value2 = true;
    value2 += true;
    value2 += false;
    value2 += nullptr;
    value2 += "v";
    value2 += 4;
    value2 += Array<Value>(1);
    value2 += HArray<Value>(1);

    SHOULD_EQUAL_TRUE(value2.IsTrue(), "value2.IsTrue()");

    value1 += static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsTrue(), "value1[0].IsTrue()");

    value2 = true;
    value1 += static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsTrue(), "value1[0].IsTrue()");
    SHOULD_EQUAL_TRUE(value1[1].IsTrue(), "value1[1].IsTrue()");

    value1.Clear();
    /////////////////

    value2 = false;

    value1 += static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");

    value2 = false;
    value1 += static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");
    SHOULD_EQUAL_TRUE(value1[1].IsFalse(), "value1[1].IsFalse()");

    value1.Clear();
    /////////////////

    value2 = nullptr;
    value1 += static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsNull(), "value1[0].IsNull()");

    value2 = nullptr;
    value1 += static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsNull(), "value1[0].IsNull()");
    SHOULD_EQUAL_TRUE(value1[1].IsNull(), "value1[1].IsNull()");

    value1.Clear();
    /////////////////
    str               = String("Qentem");
    const char *c_str = str.Char();
    value2            = static_cast<String &&>(str);

    value1 += static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 1, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsString(), "value1[0].IsString()");
    SHOULD_EQUAL(value1[0].Char(), c_str, "value1[0].Char()", "c_str");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1[0].Char(), "Qentem", 6),
                      "Memory::Compare");

    str                = String("Hani");
    const char *c_str2 = str.Char();
    value2             = static_cast<String &&>(str);

    value1 += static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 2, "Size()");
    SHOULD_EQUAL_TRUE(value1[0].IsString(), "value1[0].IsString()");
    SHOULD_EQUAL_TRUE(value1[1].IsString(), "value1[1].IsString()");
    SHOULD_EQUAL(value1[0].Char(), c_str, "value1[0].Char()", "c_str");
    SHOULD_EQUAL(value1[1].Char(), c_str2, "value1[1].Char()", "c_str2");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1[0].Char(), "Qentem", 6),
                      "Memory::Compare");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1[1].Char(), "Hani", 4),
                      "Memory::Compare");
    value1.Clear();
    /////////////////

    value2 = Array<Value>(1);
    value1 += static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");

    value2 = Array<Value>(1);
    value1 += static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 0, "Size()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_EQUAL(value2.GetArray(), nullptr, "GetArray()", "null");

    value1.Clear();
    ///

    arr_var += Value{ValueType::False};
    arr_var += Value{ValueType::True};
    arr_var += Value{HAllocator::Allocate(String("Qentem"))};
    c_str       = arr_var[2].Char();
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);

    value1 += static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 3, "Size()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");
    SHOULD_EQUAL_TRUE(value1[1].IsTrue(), "value1[1].IsTrue()");
    SHOULD_EQUAL_TRUE(value1[2].IsString(), "value1[2].IsString()");
    SHOULD_EQUAL(value1[2].Char(), c_str, "value1[0].Char()", "c_str");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1[2].Char(), "Qentem", 6),
                      "Memory::Compare");

    arr_var.Clear();
    arr_var += Value{ValueType::Null};
    arr_var += Value{14};
    arr_var += Value{HAllocator::Allocate(String("Hani"))};
    c_str2      = arr_var[2].Char();
    arr_storage = arr_var.Storage();
    value2      = static_cast<Array<Value> &&>(arr_var);

    value1 += static_cast<Value &&>(value2);
    SHOULD_EQUAL_TRUE(value2.IsUndefined(), "value2.IsUndefined()");
    SHOULD_EQUAL_TRUE(value1.IsArray(), "IsArray()");
    SHOULD_EQUAL_VALUE(value1.Size(), 6, "Size()");
    SHOULD_NOT_EQUAL(value1.GetArray(), nullptr, "GetArray()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), nullptr,
                     "GetArray()->Storage()", "null");
    SHOULD_NOT_EQUAL(value1.GetArray()->Storage(), arr_storage,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");
    SHOULD_EQUAL_TRUE(value1[1].IsTrue(), "value1[1].IsTrue()");
    SHOULD_EQUAL_TRUE(value1[2].IsString(), "value1[2].IsString()");
    SHOULD_EQUAL(value1[2].Char(), c_str, "value1[0].Char()", "c_str");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1[2].Char(), "Qentem", 6),
                      "Memory::Compare");
    SHOULD_EQUAL_TRUE(value1[3].IsNull(), "value1[3].IsNull()");
    SHOULD_EQUAL_TRUE(value1[4].IsNumber(), "value1[4].IsNumber()");
    SHOULD_EQUAL_VALUE(value1[4].GetNumber(), 14, "value1[3].GetNumber()");
    SHOULD_EQUAL_TRUE(value1[5].IsString(), "value1[0].IsString()");
    SHOULD_EQUAL(value1[5].Char(), c_str2, "value1[1].Char()", "c_str2");
    SHOULD_EQUAL_TRUE(Memory::Compare(value1[5].Char(), "Hani", 4),
                      "Memory::Compare");
    value1.Clear();
    //////////////////////////////////////////

    END_SUB_TEST;
}

static int TestAddition6() {
    Value                value1;
    Value                value2;
    String               str_var;
    const char *         str_c1;
    const char *         str_c2;
    HArray<Value>        h_arr_var;
    const HAItem<Value> *h_arr_storage1 = nullptr;
    const HAItem<Value> *h_arr_storage2 = nullptr;

    value1["k1"] = 11;
    value1["k2"] = 22;
    str_var      = "Qen";
    str_c1       = str_var.Char();
    value1["k3"] = static_cast<String &&>(str_var);

    if (value1.GetObject() != nullptr) {
        h_arr_storage1 = value1.GetObject()->Storage();
    }

    h_arr_var[String("w1")] = 10;
    h_arr_var[String("w2")] = 20;
    h_arr_var[String("w3")] = 30;
    str_var                 = "Qentem";
    str_c2                  = str_var.Char();
    h_arr_var[String("w4")] = static_cast<String &&>(str_var);
    h_arr_storage2          = h_arr_var.Storage();

    value2 = HArray<Value>(); // Setting to object type.
    value2 += value1;         // Copy
    SHOULD_EQUAL_VALUE(value2.Size(), 3, "value2.Size()");
    SHOULD_NOT_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), h_arr_storage1,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL_VALUE(value2["k1"].GetNumber(), 11,
                       "value2[\"k1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["k2"].GetNumber(), 22,
                       "value2[\"k2\"].GetNumber()");
    SHOULD_NOT_EQUAL(value2["k3"].Char(), str_c1, "value2[\"k3\"].Char()",
                     "str_c");
    SHOULD_EQUAL(Memory::Compare(value2["k3"].Char(), "Qen", 3), true,
                 "value2[\"k3\"].Char()", "Qen");

    value2 = HArray<Value>(); // Clearing and  Setting to object type.
    value2 += h_arr_var;      // Copy
    SHOULD_EQUAL_VALUE(value2.Size(), 4, "value2.Size()");
    SHOULD_NOT_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), h_arr_storage2,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL_VALUE(value2["w1"].GetNumber(), 10, "[\"w1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w2"].GetNumber(), 20, "[\"w2\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w3"].GetNumber(), 30, "[\"w3\"].GetNumber()");
    SHOULD_NOT_EQUAL(value2["w4"].Char(), str_c2, "value2[\"w4\"].Char()",
                     "str_c");
    SHOULD_EQUAL(Memory::Compare(value2["w4"].Char(), "Qentem", 6), true,
                 "value2[\"w4\"].Char()", "Qen");
    SHOULD_EQUAL(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null");

    ////

    value2 = HArray<Value>(); // Clearing and  Setting to object type.
    value2 += static_cast<Value &&>(value1); // Move
    SHOULD_EQUAL_TRUE(value1.IsUndefined(), "value1.IsUndefined()");
    SHOULD_EQUAL_VALUE(value2.Size(), 3, "value2.Size()");
    SHOULD_NOT_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), h_arr_storage2,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL_VALUE(value2["k1"].GetNumber(), 11,
                       "value2[\"k1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["k2"].GetNumber(), 22,
                       "value2[\"k2\"].GetNumber()");
    SHOULD_EQUAL(value2["k3"].Char(), str_c1, "value2[\"k3\"].Char()",
                 "str_c1");
    SHOULD_EQUAL(Memory::Compare(value2["k3"].Char(), "Qen", 3), true,
                 "value2[\"k3\"].Char()", "Qen");

    value1 = value2; // Copying back the values.
    str_c1 = value1["k3"].Char();

    value2 = HArray<Value>(); // Clearing and  Setting to object type.
    value2 += static_cast<HArray<Value> &&>(h_arr_var); // Move
    SHOULD_EQUAL(h_arr_var.Storage(), nullptr, "h_arr_var.Storage()", "null");
    SHOULD_EQUAL_VALUE(value2.Size(), 4, "value2.Size()");
    SHOULD_NOT_EQUAL(value2.GetObject(), nullptr, "GetObject()", "null");
    SHOULD_NOT_EQUAL(value2.GetObject()->Storage(), h_arr_storage2,
                     "GetArray()->Storage()", "storage");
    SHOULD_EQUAL_VALUE(value2["w1"].GetNumber(), 10, "[\"w1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w2"].GetNumber(), 20, "[\"w2\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w3"].GetNumber(), 30, "[\"w3\"].GetNumber()");
    SHOULD_EQUAL(value2["w4"].Char(), str_c2, "value2[\"w4\"].Char()",
                 "str_c2");
    SHOULD_EQUAL(Memory::Compare(value2["w4"].Char(), "Qentem", 6), true,
                 "value2[\"w4\"].Char()", "Qen");
    SHOULD_EQUAL(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null");

    if (value2.GetObject() != nullptr) {
        h_arr_var = *(value2.GetObject()); // Copying back the values.
        str_c2    = h_arr_var[String("w4")].Char();
    }

    ////////
    value2 = HArray<Value>(); // Clearing and  Setting to object type.
    value2 += value1;         // Copy
    value2 += h_arr_var;      // Copy
    SHOULD_EQUAL_VALUE(value2.Size(), 7, "value2.Size()");
    SHOULD_EQUAL_VALUE(value2["w1"].GetNumber(), 10, "[\"w1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w2"].GetNumber(), 20, "[\"w2\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w3"].GetNumber(), 30, "[\"w3\"].GetNumber()");
    SHOULD_NOT_EQUAL(value2["w4"].Char(), str_c2, "value2[\"w4\"].Char()",
                     "str_c2");
    SHOULD_EQUAL(Memory::Compare(value2["w4"].Char(), "Qentem", 6), true,
                 "value2[\"w4\"].Char()", "Qen");
    SHOULD_EQUAL_VALUE(value2["k1"].GetNumber(), 11,
                       "value2[\"k1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["k2"].GetNumber(), 22,
                       "value2[\"k2\"].GetNumber()");
    SHOULD_NOT_EQUAL(value2["k3"].Char(), str_c1, "value2[\"k3\"].Char()",
                     "str_c1");
    SHOULD_EQUAL(Memory::Compare(value2["k3"].Char(), "Qen", 3), true,
                 "value2[\"k3\"].Char()", "Qen");

    ////
    value2       = HArray<Value>(); // Clearing and  Setting to object type.
    value2["w0"] = 5;
    value2["w1"] = 100;
    value2["w2"] = 200;
    value2["w4"] = 400;
    value2["w5"] = 500;
    value2["w6"] = 600;
    value2 += h_arr_var; // Copy
    value2 += value1;    // Copy

    SHOULD_EQUAL_VALUE(value2["w0"].GetNumber(), 5, "[\"w0\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w1"].GetNumber(), 10, "[\"w1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w2"].GetNumber(), 20, "[\"w2\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w3"].GetNumber(), 30, "[\"w3\"].GetNumber()");
    SHOULD_NOT_EQUAL(value2["w4"].Char(), str_c2, "value2[\"w4\"].Char()",
                     "str_c2");
    SHOULD_EQUAL(Memory::Compare(value2["w4"].Char(), "Qentem", 6), true,
                 "value2[\"w4\"].Char()", "Qen");
    SHOULD_EQUAL_VALUE(value2["w5"].GetNumber(), 500, "[\"w5\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w6"].GetNumber(), 600, "[\"w6\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["k1"].GetNumber(), 11,
                       "value2[\"k1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["k2"].GetNumber(), 22,
                       "value2[\"k2\"].GetNumber()");
    SHOULD_NOT_EQUAL(value2["k3"].Char(), str_c1, "value2[\"k3\"].Char()",
                     "str_c1");
    SHOULD_EQUAL(Memory::Compare(value2["k3"].Char(), "Qen", 3), true,
                 "value2[\"k3\"].Char()", "Qen");

    value2       = HArray<Value>(); // Clearing and  Setting to object type.
    value2["w0"] = 5;
    value2["w1"] = 100;
    value2["w2"] = 200;
    value2["w4"] = 400;
    value2["w5"] = 500;
    value2["w6"] = 600;
    value2 += static_cast<Value &&>(value1);            // Move
    value2 += static_cast<HArray<Value> &&>(h_arr_var); // Move
    SHOULD_EQUAL_VALUE(value2["w0"].GetNumber(), 5, "[\"w0\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w1"].GetNumber(), 10, "[\"w1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w2"].GetNumber(), 20, "[\"w2\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w3"].GetNumber(), 30, "[\"w3\"].GetNumber()");
    SHOULD_EQUAL(value2["w4"].Char(), str_c2, "value2[\"w4\"].Char()",
                 "str_c2");
    SHOULD_EQUAL(Memory::Compare(value2["w4"].Char(), "Qentem", 6), true,
                 "value2[\"w4\"].Char()", "Qen");
    SHOULD_EQUAL_VALUE(value2["w5"].GetNumber(), 500, "[\"w5\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["w6"].GetNumber(), 600, "[\"w6\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["k1"].GetNumber(), 11,
                       "value2[\"k1\"].GetNumber()");
    SHOULD_EQUAL_VALUE(value2["k2"].GetNumber(), 22,
                       "value2[\"k2\"].GetNumber()");
    SHOULD_EQUAL(value2["k3"].Char(), str_c1, "value2[\"k3\"].Char()",
                 "str_c1");
    SHOULD_EQUAL(Memory::Compare(value2["k3"].Char(), "Qen", 3), true,
                 "value2[\"k3\"].Char()", "Qen");
    /////////////////////////////

    END_SUB_TEST;
}

static int TestStringify1() {
    Value value;

    ///////////
    value = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[]", "Stringify()");

    value.Clear();
    value += true;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[true]", "Stringify()");

    value.Clear();
    value += false;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[false]", "Stringify()");

    value.Clear();
    value += nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[null]", "Stringify()");

    value.Clear();
    value += 0;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[0]", "Stringify()");

    value.Clear();
    value += "a";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a"])", "Stringify()");

    value.Clear();
    value += "ABC";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC"])", "Stringify()");

    value.Clear();
    value[0] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[[]]", "Stringify()");

    value.Clear();
    value[0] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[{}]", "Stringify()");
    ///////////

    value.Clear();
    value += true;
    value += true;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[true,true]", "Stringify()");

    value.Clear();
    value += false;
    value += true;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[false,true]", "Stringify()");

    value.Clear();
    value += nullptr;
    value += true;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[null,true]", "Stringify()");

    value.Clear();
    value += -3;
    value += true;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[-3,true]", "Stringify()");

    value.Clear();
    value += "a";
    value += true;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a",true])", "Stringify()");

    value.Clear();
    value += "ABC";
    value += true;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",true])", "Stringify()");

    value.Clear();
    value[0] = Array<Value>();
    value += true;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[[],true]", "Stringify()");

    value.Clear();
    value[0] = HArray<Value>();
    value += true;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[{},true]", "Stringify()");
    ///////////

    value.Clear();
    value += true;
    value += false;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[true,false]", "Stringify()");

    value.Clear();
    value += false;
    value += false;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[false,false]", "Stringify()");

    value.Clear();
    value += nullptr;
    value += false;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[null,false]", "Stringify()");

    value.Clear();
    value += 500;
    value += false;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[500,false]", "Stringify()");

    value.Clear();
    value += String("a");
    value += false;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a",false])", "Stringify()");

    value.Clear();
    String str("ABC");
    value += str;
    value += false;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",false])", "Stringify()");

    value.Clear();
    value[0] = Array<Value>();
    value += false;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[[],false]", "Stringify()");

    value.Clear();
    value[0] = HArray<Value>();
    value += false;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[{},false]", "Stringify()");
    ///////////

    value.Clear();
    value += true;
    value += nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[true,null]", "Stringify()");

    value.Clear();
    value += false;
    value += nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[false,null]", "Stringify()");

    value.Clear();
    value += nullptr;
    value += nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[null,null]", "Stringify()");

    value.Clear();
    value += 456.5;
    value += nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[456.5,null]", "Stringify()");

    value.Clear();
    value += "a";
    value += nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a",null])", "Stringify()");

    value.Clear();
    value += "ABC";
    value += nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",null])", "Stringify()");

    value.Clear();
    value[0] = Array<Value>();
    value += nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[[],null]", "Stringify()");

    value.Clear();
    value[0] = HArray<Value>();
    value += nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[{},null]", "Stringify()");
    ///////////

    value.Clear();
    value += true;
    value += "A";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([true,"A"])", "Stringify()");

    value.Clear();
    value += false;
    value += "AB";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([false,"AB"])", "Stringify()");

    value.Clear();
    value += nullptr;
    value += "ABC";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null,"ABC"])", "Stringify()");

    value.Clear();
    value += -8.9;
    value += "ABC";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([-8.9,"ABC"])", "Stringify()");

    value.Clear();
    value += "a";
    value += "ABCD";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a","ABCD"])", "Stringify()");

    value.Clear();
    value += "ABC";
    value += "ABCDE";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC","ABCDE"])", "Stringify()");

    value.Clear();
    value[0] = Array<Value>();
    value += "ABCDEF";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([[],"ABCDEF"])", "Stringify()");

    value.Clear();
    value[0] = HArray<Value>();
    value += "ABCDEFG";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{},"ABCDEFG"])", "Stringify()");
    ///////////

    value.Clear();
    value += true;
    value[1] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[true,[]]", "Stringify()");

    value.Clear();
    value += false;
    value[1] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[false,[]]", "Stringify()");

    value.Clear();
    value += nullptr;
    value[1] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[null,[]]", "Stringify()");

    value.Clear();
    value += 10000;
    value[1] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[10000,[]]", "Stringify()");

    value.Clear();
    value += "ABC";
    value[1] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",[]])", "Stringify()");

    value.Clear();
    value[0] = Array<Value>();
    value[1] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[[],[]]", "Stringify()");

    value.Clear();
    value[0] = HArray<Value>();
    value[1] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[{},[]]", "Stringify()");
    ///////////

    value.Clear();
    value += true;
    value[1] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[true,{}]", "Stringify()");

    value.Clear();
    value += false;
    value[1] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[false,{}]", "Stringify()");

    value.Clear();
    value += nullptr;
    value[1] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[null,{}]", "Stringify()");

    value.Clear();
    value += -1000;
    value[1] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[-1000,{}]", "Stringify()");

    value.Clear();
    value += "ABC";
    value[1] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",{}])", "Stringify()");

    value.Clear();
    value[0] = Array<Value>();
    value[1] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[[],{}]", "Stringify()");

    value.Clear();
    value[0] = HArray<Value>();
    value[1] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[{},{}]", "Stringify()");
    ///////////

    value.Clear();
    value += true;
    value[1] = HArray<Value>();
    value += false;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[true,{},false]", "Stringify()");

    value.Clear();
    value += false;
    value[1] = Array<Value>();
    value += nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[false,[],null]", "Stringify()");

    value.Clear();
    value += nullptr;
    value[1] = HArray<Value>();
    value += "ABC";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null,{},"ABC"])", "Stringify()");

    value.Clear();
    value += nullptr;
    value[1] = 789;
    value += "ABC";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null,789,"ABC"])", "Stringify()");

    value.Clear();
    value += "ABC";
    value[1] = Array<Value>();
    value[2] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",[],{}])", "Stringify()");

    value.Clear();
    value[0] = Array<Value>();
    value[1] = HArray<Value>();
    value[2] = 498;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[[],{},498]", "Stringify()");

    value.Clear();
    value[0] = HArray<Value>();
    value[1] = Array<Value>();
    value += true;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[{},[],true]", "Stringify()");
    ///////////

    value.Clear();
    value += true;
    value[1] = HArray<Value>();
    value += 0;
    value[3] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[true,{},0,[]]", "Stringify()");

    value.Clear();
    value += false;
    value[1] = Array<Value>();
    value += nullptr;
    value[3] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[false,[],null,{}]", "Stringify()");

    value.Clear();
    value += nullptr;
    value[1] = HArray<Value>();
    value += "ABC";
    value[3] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null,{},"ABC",[]])",
                       "Stringify()");

    value.Clear();
    value += "ABC";
    value[1] = Array<Value>();
    value[2] = HArray<Value>();
    value += nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["ABC",[],{},null])",
                       "Stringify()");

    value.Clear();
    value[0] = Array<Value>();
    value += false;
    value[2] = HArray<Value>();
    value += true;
    SHOULD_EQUAL_VALUE(value.Stringify(), "[[],false,{},true]", "Stringify()");

    value.Clear();
    value[0] = HArray<Value>();
    value += nullptr;
    value[2] = Array<Value>();
    value[3] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), "[{},null,[],[]]", "Stringify()");

    ///////////
    value.Clear();
    value += true;
    value += false;
    value += nullptr;
    value += 123;
    value += "ABC";
    value[5] = Array<Value>();
    value[6] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"([true,false,null,123,"ABC",[],{}])", "Stringify()");

    ///////////
    value.Clear();
    value[0] = HArray<Value>();
    value[1] = Array<Value>();
    value += "a";
    value += 1.5;
    value += nullptr;
    value += false;
    value += true;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([{},[],"a",1.5,null,false,true])",
                       "Stringify()");
    ///////////////////////////////////////
    END_SUB_TEST;
}

static int TestStringify2() {
    Value value;

    ///////////
    value = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "Stringify()");

    value.Clear();
    value["A"] = true;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":true})", "Stringify()");

    value.Clear();
    value["B"] = false;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"B":false})", "Stringify()");

    value.Clear();
    value["AA"] = nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"AA":null})", "Stringify()");

    value.Clear();
    value["a"] = 0;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"a":0})", "Stringify()");

    value.Clear();
    value["abc"] = "a";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"abc":"a"})", "Stringify()");

    value.Clear();
    value["ab"] = "ABC";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"ab":"ABC"})", "Stringify()");

    value.Clear();
    value["ABC"] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"ABC":[]})", "Stringify()");

    value.Clear();
    value["5vn7b83y98t3wrupwmwa4ataw"] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"5vn7b83y98t3wrupwmwa4ataw":{}})",
                       "Stringify()");
    ///////////

    value.Clear();
    value["A"] = true;
    value["B"] = true;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":true,"B":true})",
                       "Stringify()");

    value.Clear();
    value["A"] = false;
    value["b"] = true;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":false,"b":true})",
                       "Stringify()");

    value.Clear();
    value["A"]  = nullptr;
    value["BC"] = true;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":null,"BC":true})",
                       "Stringify()");

    value.Clear();
    value["A"]  = -3;
    value["AB"] = true;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":-3,"AB":true})",
                       "Stringify()");

    value.Clear();
    value["A"]   = "a";
    value["ABC"] = true;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"a","ABC":true})",
                       "Stringify()");

    value.Clear();
    value["A"] = "ABC";
    value["1"] = true;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"ABC","1":true})",
                       "Stringify()");

    value.Clear();
    value["X"]   = Array<Value>();
    value["123"] = true;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":[],"123":true})",
                       "Stringify()");

    value.Clear();
    value["x"] = HArray<Value>();
    value["A"] = true;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"x":{},"A":true})",
                       "Stringify()");
    ///////////

    value.Clear();
    value["A2"] = true;
    value["A1"] = false;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A2":true,"A1":false})",
                       "Stringify()");

    value.Clear();
    value["A-"]   = false;
    value["A123"] = false;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A-":false,"A123":false})",
                       "Stringify()");

    value.Clear();
    value["A"] = nullptr;
    value["B"] = false;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":null,"B":false})",
                       "Stringify()");

    value.Clear();
    value["A"] = 500;
    value["B"] = false;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":500,"B":false})",
                       "Stringify()");

    value.Clear();
    value["A"] = String("a");
    value["B"] = false;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"a","B":false})",
                       "Stringify()");

    value.Clear();
    String str("ABC");
    value["A"] = str;
    value["B"] = false;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"ABC","B":false})",
                       "Stringify()");

    value.Clear();
    value["X"] = Array<Value>();
    value["A"] = false;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":[],"A":false})",
                       "Stringify()");

    value.Clear();
    value["X"] = HArray<Value>();
    value["A"] = false;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":{},"A":false})",
                       "Stringify()");
    ///////////

    value.Clear();
    value["A"] = true;
    value["W"] = nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":true,"W":null})",
                       "Stringify()");

    value.Clear();
    value["A"] = false;
    value["@"] = nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":false,"@":null})",
                       "Stringify()");

    value.Clear();
    value["A"] = nullptr;
    value["#"] = nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":null,"#":null})",
                       "Stringify()");

    value.Clear();
    value["A"] = 456.5;
    value["H"] = nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":456.5,"H":null})",
                       "Stringify()");

    value.Clear();
    value["A"] = "a";
    value["Q"] = nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"a","Q":null})",
                       "Stringify()");

    value.Clear();
    value["A"] = "ABC";
    value["e"] = nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"ABC","e":null})",
                       "Stringify()");

    value.Clear();
    value["X"] = Array<Value>();
    value["n"] = nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":[],"n":null})",
                       "Stringify()");

    value.Clear();
    value["t"] = HArray<Value>();
    value["A"] = nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"t":{},"A":null})",
                       "Stringify()");
    ///////////

    value.Clear();
    value["e"] = true;
    value["A"] = "A";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"e":true,"A":"A"})",
                       "Stringify()");

    value.Clear();
    value["m"] = false;
    value["A"] = "AB";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"m":false,"A":"AB"})",
                       "Stringify()");

    value.Clear();
    value["i"] = nullptr;
    value["A"] = "ABC";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"i":null,"A":"ABC"})",
                       "Stringify()");

    value.Clear();
    value["s"] = -8.9;
    value["A"] = "ABC";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"s":-8.9,"A":"ABC"})",
                       "Stringify()");

    value.Clear();
    value["-"] = "a";
    value["A"] = "ABCD";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"-":"a","A":"ABCD"})",
                       "Stringify()");

    value.Clear();
    value["g"] = "ABC";
    value["A"] = "ABCDE";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"g":"ABC","A":"ABCDE"})",
                       "Stringify()");

    value.Clear();
    value["o"] = Array<Value>();
    value["A"] = "ABCDEF";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"o":[],"A":"ABCDEF"})",
                       "Stringify()");

    value.Clear();
    value["A"] = HArray<Value>();
    value["o"] = "ABCDEFG";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":{},"o":"ABCDEFG"})",
                       "Stringify()");
    ///////////

    value.Clear();
    value["d"] = true;
    value["y"] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"d":true,"y":[]})",
                       "Stringify()");

    value.Clear();
    value["A"] = false;
    value["y"] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":false,"y":[]})",
                       "Stringify()");

    value.Clear();
    value["A"] = nullptr;
    value["y"] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":null,"y":[]})",
                       "Stringify()");

    value.Clear();
    value["A"] = 10000;
    value["y"] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":10000,"y":[]})",
                       "Stringify()");

    value.Clear();
    value["A"] = "ABC";
    value["y"] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"ABC","y":[]})",
                       "Stringify()");

    value.Clear();
    value["X"] = Array<Value>();
    value["y"] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":[],"y":[]})", "Stringify()");

    value.Clear();
    value["X"] = HArray<Value>();
    value["Y"] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":{},"Y":[]})", "Stringify()");
    ///////////

    value.Clear();
    value["A"] = true;
    value["y"] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":true,"y":{}})",
                       "Stringify()");

    value.Clear();
    value["A"] = false;
    value["y"] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":false,"y":{}})",
                       "Stringify()");

    value.Clear();
    value["A"] = nullptr;
    value["y"] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":null,"y":{}})",
                       "Stringify()");

    value.Clear();
    value["A"] = -1000;
    value["y"] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":-1000,"y":{}})",
                       "Stringify()");

    value.Clear();
    value["A"] = "ABC";
    value["y"] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"ABC","y":{}})",
                       "Stringify()");

    value.Clear();
    value["C"] = Array<Value>();
    value["R"] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"C":[],"R":{}})", "Stringify()");

    value.Clear();
    value["cc"] = HArray<Value>();
    value["rr"] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"cc":{},"rr":{}})",
                       "Stringify()");
    ///////////

    value.Clear();
    value["A"]  = true;
    value["y"]  = HArray<Value>();
    value["AA"] = false;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":true,"y":{},"AA":false})",
                       "Stringify()");

    value.Clear();
    value["A"] = false;
    value["y"] = Array<Value>();
    value["B"] = nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":false,"y":[],"B":null})",
                       "Stringify()");

    value.Clear();
    value["A"]   = nullptr;
    value["y"]   = HArray<Value>();
    value["ABC"] = "ABC";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":null,"y":{},"ABC":"ABC"})",
                       "Stringify()");

    value.Clear();
    value["t"] = nullptr;
    value["Y"] = 789;
    value["A"] = "ABC";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"t":null,"Y":789,"A":"ABC"})",
                       "Stringify()");

    value.Clear();
    value["A"]     = "ABC";
    value["y"]     = Array<Value>();
    value["key-u"] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"ABC","y":[],"key-u":{}})",
                       "Stringify()");

    value.Clear();
    value["X"]     = Array<Value>();
    value["Y"]     = HArray<Value>();
    value["key-u"] = 498;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":[],"Y":{},"key-u":498})",
                       "Stringify()");

    value.Clear();
    value["X"] = HArray<Value>();
    value["y"] = Array<Value>();
    value["A"] = true;
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"X":{},"y":[],"A":true})",
                       "Stringify()");
    ///////////

    value.Clear();
    value["{}}"]   = true;
    value["y"]     = HArray<Value>();
    value["AA"]    = 0;
    value["k-300"] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"{}}":true,"y":{},"AA":0,"k-300":[]})",
                       "Stringify()");

    value.Clear();
    value["B1"]    = false;
    value["y"]     = Array<Value>();
    value["[A]"]   = nullptr;
    value["k-300"] = HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"B1":false,"y":[],"[A]":null,"k-300":{}})",
                       "Stringify()");

    value.Clear();
    value["{A}"]   = nullptr;
    value["y"]     = HArray<Value>();
    value["AA"]    = "ABC";
    value["k-300"] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})",
                       "Stringify()");

    value.Clear();
    value["x"]     = "ABC";
    value["[]]"]   = Array<Value>();
    value["key-u"] = HArray<Value>();
    value["A"]     = nullptr;
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"x":"ABC","[]]":[],"key-u":{},"A":null})",
                       "Stringify()");

    value.Clear();
    value["X"]       = Array<Value>();
    value["CCCCccc"] = false;
    value["key-u"]   = HArray<Value>();
    value["A"]       = true;
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})",
                       "Stringify()");

    value.Clear();
    value["X"]     = HArray<Value>();
    value["A"]     = nullptr;
    value["key-u"] = Array<Value>();
    value["k-300"] = Array<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"X":{},"A":null,"key-u":[],"k-300":[]})",
                       "Stringify()");

    ///////////
    value.Clear();
    value["A"] = true;
    value["B"] = false;
    value["C"] = nullptr;
    value["D"] = 123;
    value["E"] = "ABC";
    value["F"] = Array<Value>();
    value["G"] = HArray<Value>();
    SHOULD_EQUAL_VALUE(
        value.Stringify(),
        R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})",
        "Stringify()");

    ///////////
    value.Clear();
    value["A"]       = HArray<Value>();
    value["BB"]      = Array<Value>();
    value["CCC"]     = "a";
    value["DDDD"]    = 1.5;
    value["EEEEE"]   = nullptr;
    value["FFFFFF"]  = false;
    value["GGGGGGG"] = true;
    SHOULD_EQUAL_VALUE(
        value.Stringify(),
        R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})",
        "Stringify()");
    ///////////////////////////////////////

    END_SUB_TEST;
}

static int TestStringify3() {
    Value value;

    value[0] += true;
    value[0] += false;
    value[0] += nullptr;
    value[0] += 0;
    value[0] += "ABC";
    value[0][5] = Array<Value>();
    value[0] += HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"([[true,false,null,0,"ABC",[],{}]])", "Stringify()");

    value         = Array<Value>();
    value[0]["a"] = true;
    value[0]["0"] = false;
    value[0]["1"] = nullptr;
    value[0]["V"] = 0;
    value[0]["B"] = "a";
    value[0]["2"] = Array<Value>();
    value[0]["6"] = HArray<Value>();
    SHOULD_EQUAL_VALUE(
        value.Stringify(),
        R"([{"a":true,"0":false,"1":null,"V":0,"B":"a","2":[],"6":{}}])",
        "Stringify()");

    value.Clear();
    value["o"] += true;
    value["o"] += false;
    value["o"] += nullptr;
    value["o"] += 0;
    value["o"] += "ABC";
    value["o"][5] = Array<Value>();
    value["o"] += HArray<Value>();
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"o":[true,false,null,0,"ABC",[],{}]})",
                       "Stringify()");

    value.Clear();
    value["i"]["a"] = true;
    value["i"]["0"] = false;
    value["i"]["1"] = nullptr;
    value["i"]["V"] = 0;
    value["i"]["B"] = "a";
    value["i"]["2"] = Array<Value>();
    value["i"]["6"] = HArray<Value>();
    SHOULD_EQUAL_VALUE(
        value.Stringify(),
        R"({"i":{"a":true,"0":false,"1":null,"V":0,"B":"a","2":[],"6":{}}})",
        "Stringify()");

    ////
    END_SUB_TEST;
}

static int TestStringify4() {
    Value        value;
    String       str;
    StringStream ss;
    /*
     * "
     * \
     * /
     * \b
     * \f
     * \n
     * \r
     * \t
     */

    str = "\"";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\")", "ss.GetString()");

    str = "\\";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\\)", "ss.GetString()");

    str = "/";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\/)", "ss.GetString()");

    str = "\b";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\b)", "ss.GetString()");

    str = "\f";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\f)", "ss.GetString()");

    str = "\n";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\n)", "ss.GetString()");

    str = "\r";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\r)", "ss.GetString()");

    str = "\t";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\t)", "ss.GetString()");
    //////////

    str = "\"\"";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\"\")", "ss.GetString()");

    str = "\\\\";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\\\\)", "ss.GetString()");

    str = "//";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\/\/)", "ss.GetString()");

    str = "\b\b";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\b\b)", "ss.GetString()");

    str = "\f\f";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\f\f)", "ss.GetString()");

    str = "\n\n";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\n\n)", "ss.GetString()");

    str = "\r\r";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\r\r)", "ss.GetString()");

    str = "\t\t";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\t\t)", "ss.GetString()");
    //////////

    str = R"(""")";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\"\"\")", "ss.GetString()");

    str = R"(\\\)";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\\\\\\)", "ss.GetString()");

    str = "///";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\/\/\/)", "ss.GetString()");

    str = "\b\b\b";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\b\b\b)", "ss.GetString()");

    str = "\f\f\f";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\f\f\f)", "ss.GetString()");

    str = "\n\n\n";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\n\n\n)", "ss.GetString()");

    str = "\r\r\r";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\r\r\r)", "ss.GetString()");

    str = "\t\t\t";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\t\t\t)", "ss.GetString()");
    //////////

    str = " \"";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \")", "ss.GetString()");

    str = " \\";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \\)", "ss.GetString()");

    str = " /";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \/)", "ss.GetString()");

    str = " \b";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \b)", "ss.GetString()");

    str = " \f";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \f)", "ss.GetString()");

    str = " \n";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \n)", "ss.GetString()");

    str = " \r";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \r)", "ss.GetString()");

    str = " \t";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \t)", "ss.GetString()");
    //////////

    str = "\" ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\" )", "ss.GetString()");

    str = "\\ ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\\ )", "ss.GetString()");

    str = "/ ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\/ )", "ss.GetString()");

    str = "\b ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\b )", "ss.GetString()");

    str = "\f ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\f )", "ss.GetString()");

    str = "\n ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\n )", "ss.GetString()");

    str = "\r ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\r )", "ss.GetString()");

    str = "\t ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\t )", "ss.GetString()");
    //////////

    str = " \" ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \" )", "ss.GetString()");

    str = " \\ ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \\ )", "ss.GetString()");

    str = " / ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \/ )", "ss.GetString()");

    str = " \b ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \b )", "ss.GetString()");

    str = " \f ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \f )", "ss.GetString()");

    str = " \n ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \n )", "ss.GetString()");

    str = " \r ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \r )", "ss.GetString()");

    str = " \t ";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"( \t )", "ss.GetString()");
    //////////

    str = R"(" ")";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\" \")", "ss.GetString()");

    str = R"(\ \)";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\\ \\)", "ss.GetString()");

    str = "/ /";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\/ \/)", "ss.GetString()");

    str = "\b \b";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\b \b)", "ss.GetString()");

    str = "\f \f";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\f \f)", "ss.GetString()");

    str = "\n \n";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\n \n)", "ss.GetString()");

    str = "\r \r";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\r \r)", "ss.GetString()");

    str = "\t \t";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\t \t)", "ss.GetString()");

    //////////
    str = "\"\\/\b\f\n\r\t";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\"\\\/\b\f\n\r\t)", "ss.GetString()");

    str = "\t\r\n\f\b/\\\"";
    JSON::EscapeString(str.Char(), str.Length(), ss);
    SHOULD_EQUAL_VALUE(ss.GetString(), R"(\t\r\n\f\b\/\\\")", "ss.GetString()");

    value["\"\\/\b\f\n\r\t"] = "\t\r\n\f\b/\\\"";
    SHOULD_EQUAL_VALUE(value.Stringify(),
                       R"({"\"\\\/\b\f\n\r\t":"\t\r\n\f\b\/\\\""})",
                       "value.Stringify()");

    value.Clear();
    value += "\"\\/\b\f\n\r\t";
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["\"\\\/\b\f\n\r\t"])",
                       "value.Stringify()");

    END_SUB_TEST;
}

static int TestDeleteValue() {
    Value value;

    value[0] = 1;
    value.Delete(int{0});
    SHOULD_EQUAL(value.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = "c";
    value.Delete(UInt{0});
    SHOULD_EQUAL(value.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = Array<Value>();
    value.Delete(ULong{0});
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = false;
    value[1] = true;
    value.Delete(0);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([true])", "value.Stringify()");
    value.Delete(1);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = "abc";
    value[1] = nullptr;
    value.Delete(1);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["abc"])", "value.Stringify()");
    value.Delete(0);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = false;
    value[1] = true;
    value[2] = nullptr;
    value.Delete(1);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([false,null])",
                       "value.Stringify()");
    value.Delete(0);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null])", "value.Stringify()");

    value.Delete(2);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = false;
    value[1] = true;
    value[2] = nullptr;
    value[1].Clear();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([false,null])",
                       "value.Stringify()");
    value[0].Clear();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([null])", "value.Stringify()");

    value[2].Clear();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = "a";
    value[1] = Array<Value>();
    value[2] = HArray<Value>();
    value.Delete(2);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a",[]])", "value.Stringify()");
    value.Delete(1);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"(["a"])", "value.Stringify()");

    value.Delete(0);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    /////////
    value.Clear();

    value["A"] = 1;
    value.Delete(0);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value["A"] = "c";
    value.Delete("A", 1);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value["A"] = Array<Value>();
    value.Delete("A", 1);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value.Clear();
    value["A"]  = false;
    value["bb"] = true;
    value.Delete("A", 1);
    SHOULD_EQUAL(value.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value.GetValue(0), nullptr, "GetValue(0)", "null");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"bb":true})",
                       "value.Stringify()");
    value.Delete(1);
    SHOULD_EQUAL(value.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(value.GetValue(1), nullptr, "GetValue(1)", "null");
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value["A"]  = "abc";
    value["bb"] = nullptr;
    value.Delete("bb", 2);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"abc"})",
                       "value.Stringify()");
    value.Delete("A", 1);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value["A"]   = false;
    value["bb"]  = true;
    value["AAA"] = nullptr;
    value.Delete("bb", 2);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":false,"AAA":null})",
                       "value.Stringify()");
    value.Delete("A", 1);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"AAA":null})",
                       "value.Stringify()");

    value.Delete("AAA", 3);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value["A"]   = "a";
    value["bb"]  = HArray<Value>();
    value["AAA"] = Array<Value>();
    value.Delete("AAA", 3);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"a","bb":{}})",
                       "value.Stringify()");
    value.Delete("bb", 2);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"a"})",
                       value.Stringify().Char());

    value.Delete("A", 1);
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value["A"]   = "a";
    value["bb"]  = HArray<Value>();
    value["AAA"] = Array<Value>();
    value["AAA"].Clear();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"a","bb":{}})",
                       "value.Stringify()");
    value["bb"].Clear();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({"A":"a"})", "value.Stringify()");

    value["A"].Clear();
    SHOULD_EQUAL_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    END_SUB_TEST;
}

static int RunValueTests() {
    STARTING_TEST("Value.hpp");

    START_TEST("Empty Value Test 1", TestEmptyValue1);
    START_TEST("Empty Value Test 2", TestEmptyValue2);
    START_TEST("Empty Value Test 3", TestEmptyValue3);

    START_TEST("True Value Test 1", TestTrueValue1);
    START_TEST("True Value Test 2", TestTrueValue2);
    START_TEST("True Value Test 3", TestTrueValue3);

    START_TEST("False Value Test 1", TestFalseValue1);
    START_TEST("False Value Test 2", TestFalseValue2);
    START_TEST("False Value Test 3", TestFalseValue3);

    START_TEST("Null Value Test 1", TestNullValue1);
    START_TEST("Null Value Test 2", TestNullValue2);
    START_TEST("Null Value Test 3", TestNullValue3);

    START_TEST("Number Value Test 1", TestNumberValue1);
    START_TEST("Number Value Test 2", TestNumberValue2);
    START_TEST("Number Value Test 3", TestNumberValue3);

    START_TEST("String Value Test 1", TestStringValue1);
    START_TEST("String Value Test 2", TestStringValue2);
    START_TEST("String Value Test 3", TestStringValue3);

    START_TEST("Array Value Test 1", TestArrayValue1);
    START_TEST("Array Value Test 2", TestArrayValue2);
    START_TEST("Array Value Test 3", TestArrayValue3);
    START_TEST("Array Value Test 4", TestArrayValue4);

    START_TEST("Object Value Test 1", TestObjectValue1);
    START_TEST("Object Value Test 2", TestObjectValue2);
    START_TEST("Object Value Test 3", TestObjectValue3);
    START_TEST("Object Value Test 4", TestObjectValue4);

    START_TEST("Move Value Test 1", TestMoveValue1);
    START_TEST("Move Value Test 2", TestMoveValue2);
    START_TEST("Move Value Test 3", TestMoveValue3);
    START_TEST("Move Value Test 4", TestMoveValue4);

    START_TEST("Copy Value Test 1", TestCopyValue1);
    START_TEST("Copy Value Test 2", TestCopyValue2);
    START_TEST("Copy Value Test 3", TestCopyValue3);
    START_TEST("Copy Value Test 4", TestCopyValue4);

    START_TEST("Index Operator Test 1", TestIndexOperator1);
    START_TEST("Index Operator Test 2", TestIndexOperator2);

    START_TEST("Addition Test 1", TestAddition1);
    START_TEST("Addition Test 2", TestAddition2);
    START_TEST("Addition Test 3", TestAddition3);
    START_TEST("Addition Test 4", TestAddition4);
    START_TEST("Addition Test 5", TestAddition5);
    START_TEST("Addition Test 6", TestAddition6);

    START_TEST("Stringify Test 1", TestStringify1);
    START_TEST("Stringify Test 2", TestStringify2);
    START_TEST("Stringify Test 3", TestStringify3);
    START_TEST("Stringify Test 4", TestStringify4);

    START_TEST("Delete Value Test", TestDeleteValue);

    END_TEST("Value.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
