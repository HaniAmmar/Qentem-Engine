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

#ifndef QENTEM_VALUE_TESTS_H_
#define QENTEM_VALUE_TESTS_H_

namespace Qentem {
namespace Test {
using VString       = String<char>;
using ValueC        = Value<char>;
using VStringStream = StringStream<char>;
using VHArray       = HArray<ValueC, char>;
using VArray        = Array<ValueC>;

template <typename Stream_T_>
static Stream_T_ &operator<<(Stream_T_ &out, ValueType type) {
    switch (type) {
        case ValueType::Undefined:
            out << "Undefined";
            break;
        case ValueType::Object:
            out << "Object";
            break;
        case ValueType::Array:
            out << "Array";
            break;
        case ValueType::String:
            out << "String";
            break;
        case ValueType::UInt64:
            out << "UInt64";
            break;
        case ValueType::Int64:
            out << "Int64";
            break;
        case ValueType::Double:
            out << "Double";
            break;
        case ValueType::True:
            out << "True";
            break;
        case ValueType::False:
            out << "False";
            break;
        case ValueType::Null:
            out << "Null";
            break;
    }

    return out;
}

static int TestEmptyValue() {
    ValueC value1;

    VStringStream ss_var;
    VString       str_var;
    const char *  c_str_var;
    SizeT         c_str_len;
    double        num_var;
    bool          bool_var;

    EQ_TRUE(value1.IsUndefined(), "isUndefined()");
    EQ_FALSE(value1.IsObject(), "IsObject()");
    EQ_FALSE(value1.IsArray(), "IsArray()");
    EQ_FALSE(value1.IsString(), "IsString()");
    EQ_FALSE(value1.IsNumber(), "IsNumber()");
    EQ_FALSE(value1.IsTrue(), "IsTrue()");
    EQ_FALSE(value1.IsFalse(), "IsFalse()");
    EQ_FALSE(value1.IsNull(), "IsNull()");
    EQ_VALUE(value1.Type(), ValueType::Undefined, "Type()");
    EQ_VALUE(value1.Size(), 0, "Size()");
    EQ_TO(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    EQ_TO(value1.GetValue(10), nullptr, "GetValue(10)", "null");
    EQ_TO(value1.GetValue("", 0), nullptr,
          "GetValue("
          ", 0)",
          "null");
    EQ_TO(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    EQ_FALSE(value1.InsertKey(ss_var, 0), "InsertKey(0)");
    EQ_FALSE(value1.SetCharAndLength(c_str_var, c_str_len),
             "SetCharAndLength()");
    EQ_TO(value1.GetKey(10), nullptr, "GetKey(10)", "null");
    EQ_TO(value1.GetObject(), nullptr, "GetObject()", "null");
    EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetString(), nullptr, "GetString()", "null");
    EQ_TO(value1.StringStorage(), nullptr, "StringStorage()", "null");
    EQ_VALUE(value1.Length(), 0, "Length()");
    EQ_FALSE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(value1.GetNumber(), 0, "GetNumber()");
    EQ_FALSE(value1.SetNumber(num_var), "SetNumber()");
    EQ_FALSE(value1.GetBool(bool_var), "GetBool()");
    EQ_VALUE(value1.Stringify(), "", "Stringify()");

    value1 = ValueC{ValueType::Object};
    EQ_TRUE(value1.IsObject(), "IsObject()");

    value1 = ValueC{ValueType::Array};
    EQ_TRUE(value1.IsArray(), "IsArray()");

    value1 = ValueC{ValueType::String};
    EQ_TRUE(value1.IsString(), "IsString()");

    value1 = ValueC{ValueType::UInt64};
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_TRUE(value1.IsUInt64(), "IsUInt64()");

    value1 = ValueC{ValueType::Int64};
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_TRUE(value1.IsInt64(), "IsInt64()");

    value1 = ValueC{ValueType::Double};
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_TRUE(value1.IsDouble(), "IsDouble()");

    END_SUB_TEST;
}

static int TestTrueValue() {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    VString       str_var;
    const char *  c_str_var;
    SizeT         c_str_len;
    double        num_var;
    bool          bool_var;

    value1 = true;
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    EQ_VALUE(value1.Type(), ValueType::True, "Type()");
    EQ_VALUE(value1.Size(), 0, "Size()");
    EQ_TO(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    EQ_TO(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    EQ_FALSE(value1.InsertKey(ss_var, 0), "InsertKey(0)");
    EQ_TRUE(value1.SetCharAndLength(c_str_var, c_str_len),
            "SetCharAndLength()");
    EQ_TRUE(StringUtils::IsEqual("true", c_str_var, c_str_len),
            "value1.SetCharAndLength()");
    EQ_TO(value1.GetObject(), nullptr, "GetObject()", "null");
    EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetString(), nullptr, "GetString()", "null");
    EQ_TO(value1.StringStorage(), nullptr, "StringStorage()", "null");
    EQ_VALUE(value1.Length(), 0, "Length()");
    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "true", "str_var");
    EQ_TRUE(value1.InsertString(ss_var), "InsertString()");
    EQ_VALUE(ss_var, "true", "ss_var");
    EQ_VALUE(value1.GetNumber(), 1, "GetNumber()");
    EQ_TRUE(value1.SetNumber(num_var), "SetNumber()");
    EQ_VALUE(num_var, 1, "num_var");
    EQ_TRUE(value1.GetBool(bool_var), "GetBool()");
    EQ_TRUE(bool_var, "bool_var");
    EQ_VALUE(value1.Stringify(), "", "Stringify()");
    ss_var.Reset();

    value1.Reset();
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    value1 = true;
    value2 = true;
    value2 = value1;
    EQ_TRUE(value2.IsTrue(), "IsTrue()");
    EQ_TRUE(value1.IsTrue(), "IsTrue()");

    value1 = true;
    value2 = ValueC{value1};
    EQ_TRUE(value2.IsTrue(), "IsTrue()");

    value2.Reset();
    value2 = static_cast<ValueC &&>(value1);
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");
    EQ_TRUE(value2.IsTrue(), "IsTrue()");

    value1 = true;
    ValueC value3(static_cast<ValueC &&>(value1));
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    EQ_TRUE(value3.IsTrue(), "IsTrue()");

    value3 = true;
    EQ_TRUE(value3.IsTrue(), "IsTrue()");

    value3 = ValueC{ValueType::True};
    EQ_TRUE(value3.IsTrue(), "IsTrue()");

    END_SUB_TEST;
}

static int TestFalseValue() {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    VString       str_var;
    const char *  c_str_var;
    SizeT         c_str_len;
    double        num_var;
    bool          bool_var;

    value1 = false;
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    EQ_VALUE(value1.Type(), ValueType::False, "Type()");
    EQ_VALUE(value1.Size(), 0, "Size()");
    EQ_TO(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    EQ_TO(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    EQ_FALSE(value1.InsertKey(ss_var, 0), "InsertKey(0)");
    EQ_TRUE(value1.SetCharAndLength(c_str_var, c_str_len),
            "SetCharAndLength()");
    EQ_TRUE(StringUtils::IsEqual("false", c_str_var, c_str_len),
            "value1.SetCharAndLength()");
    EQ_TO(value1.GetObject(), nullptr, "GetObject()", "null");
    EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetString(), nullptr, "GetString()", "null");
    EQ_TO(value1.StringStorage(), nullptr, "StringStorage()", "null");
    EQ_VALUE(value1.Length(), 0, "Length()");
    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "false", "str_var");
    EQ_TRUE(value1.InsertString(ss_var), "InsertString()");
    EQ_VALUE(ss_var, "false", "ss_var");
    EQ_VALUE(value1.GetNumber(), 0, "GetNumber()");
    EQ_TRUE(value1.SetNumber(num_var), "SetNumber()");
    EQ_VALUE(num_var, 0, "num_var");
    EQ_TRUE(value1.GetBool(bool_var), "GetBool()");
    EQ_FALSE(bool_var, "bool_var");
    EQ_VALUE(value1.Stringify(), "", "Stringify()");
    ss_var.Reset();

    value1.Reset();
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    value1 = false;
    value2 = false;
    value2 = value1;
    EQ_TRUE(value2.IsFalse(), "IsFalse()");
    EQ_TRUE(value1.IsFalse(), "IsFalse()");

    value1 = false;
    value2 = ValueC{value1};
    EQ_TRUE(value2.IsFalse(), "IsFalse()");

    value2.Reset();
    value2 = static_cast<ValueC &&>(value1);
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");
    EQ_TRUE(value2.IsFalse(), "IsFalse()");

    value1 = false;
    ValueC value3(static_cast<ValueC &&>(value1));
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    EQ_TRUE(value3.IsFalse(), "IsFalse()");

    value3 = false;
    EQ_TRUE(value3.IsFalse(), "IsFalse()");

    value3 = ValueC{ValueType::False};
    EQ_TRUE(value3.IsFalse(), "IsFalse()");

    END_SUB_TEST;
}

static int TestNullValue() {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    VString       str_var;
    const char *  c_str_var;
    SizeT         c_str_len;
    double        num_var;
    bool          bool_var;

    value1 = nullptr;
    EQ_TRUE(value1.IsNull(), "IsNull()");
    EQ_VALUE(value1.Type(), ValueType::Null, "Type()");
    EQ_VALUE(value1.Size(), 0, "Size()");
    EQ_TO(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    EQ_TO(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    EQ_FALSE(value1.InsertKey(ss_var, 0), "InsertKey(0)");
    EQ_TRUE(value1.SetCharAndLength(c_str_var, c_str_len),
            "SetCharAndLength()");
    EQ_TRUE(StringUtils::IsEqual("null", c_str_var, c_str_len),
            "value1.SetCharAndLength()");
    EQ_TO(value1.GetObject(), nullptr, "GetObject()", "null");
    EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetString(), nullptr, "GetString()", "null");
    EQ_TO(value1.StringStorage(), nullptr, "StringStorage()", "null");
    EQ_VALUE(value1.Length(), 0, "Length()");
    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "null", "str_var");
    EQ_TRUE(value1.InsertString(ss_var), "InsertString()");
    EQ_VALUE(ss_var, "null", "ss_var");
    EQ_VALUE(value1.GetNumber(), 0, "GetNumber()");
    EQ_TRUE(value1.SetNumber(num_var), "SetNumber()");
    EQ_VALUE(num_var, 0, "num_var");
    EQ_TRUE(value1.GetBool(bool_var), "GetBool()");
    EQ_FALSE(bool_var, "bool_var");
    EQ_VALUE(value1.Stringify(), "", "Stringify()");
    ss_var.Reset();

    value1.Reset();
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    value1 = nullptr;
    value2 = nullptr;
    value2 = value1;
    EQ_TRUE(value2.IsNull(), "IsNull()");
    EQ_TRUE(value1.IsNull(), "IsNull()");

    value1 = nullptr;
    value2 = ValueC{value1};
    EQ_TRUE(value2.IsNull(), "IsNull()");

    value2.Reset();
    value2 = static_cast<ValueC &&>(value1);
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");
    EQ_TRUE(value2.IsNull(), "IsNull()");

    value1 = nullptr;
    ValueC value3(static_cast<ValueC &&>(value1));
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    EQ_TRUE(value3.IsNull(), "IsNull()");

    value3 = nullptr;
    EQ_TRUE(value3.IsNull(), "IsNull()");

    value3 = ValueC{ValueType::Null};
    EQ_TRUE(value3.IsNull(), "IsNull()");

    END_SUB_TEST;
}

static int TestNumberValue1() {
    using vu_int = unsigned int;

    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    VString       str_var;
    const char *  c_str_var;
    SizeT         c_str_len;
    double        num_var;
    bool          bool_var;

    value1 = 33;
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.Size(), 0, "Size()");
    EQ_TO(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    EQ_TO(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    EQ_TO(value1.InsertKey(ss_var, 0), false, "InsertKey(0)", "false");
    EQ_TO(value1.SetCharAndLength(c_str_var, c_str_len), false,
          "SetCharAndLength()", "false");
    EQ_TO(value1.GetObject(), nullptr, "GetObject()", "null");
    EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetString(), nullptr, "GetString()", "null");
    EQ_TO(value1.StringStorage(), nullptr, "StringStorage()", "null");
    EQ_VALUE(value1.Length(), 0, "Length()");
    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "33", "str_var");
    EQ_TRUE(value1.InsertString(ss_var), "InsertString()");
    EQ_VALUE(ss_var, "33", "ss_var");
    EQ_VALUE(value1.GetNumber(), 33, "GetNumber()");
    EQ_TRUE(value1.SetNumber(num_var), "SetNumber()");
    EQ_VALUE(num_var, 33, "num_var");
    EQ_TRUE(value1.GetBool(bool_var), "GetBool()");
    EQ_TRUE(bool_var, "bool_var");
    EQ_VALUE(value1.Stringify(), "", "Stringify()");
    ss_var.Reset();

    value1.Reset();
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    value1 = 45;
    value2 = -10;
    value2 = value1;
    EQ_TRUE(value2.IsNumber(), "IsNumber()");
    EQ_TRUE(value2.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "45", "str_var");
    EQ_VALUE(value2.GetNumber(), 45, "GetNumber()");
    EQ_TRUE(value2.SetNumber(num_var), "SetNumber()");
    EQ_VALUE(num_var, 45, "num_var");
    EQ_TRUE(value2.GetBool(bool_var), "GetBool()");
    EQ_TRUE(bool_var, "bool_var");
    EQ_VALUE(value1.GetNumber(), 45, "GetNumber()");

    value1.Reset();

    value1 = vu_int{10};
    value2 = ValueC{value1};
    EQ_VALUE(value2.GetNumber(), 10, "GetNumber()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");

    value2.Reset();
    value2 = static_cast<ValueC &&>(value1);
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");
    EQ_VALUE(value2.GetNumber(), 10, "GetNumber()");

    value2 = 1213;
    EQ_VALUE(value2.GetNumber(), 1213, "GetNumber()");

    value2 = 10;
    value2 = double(785);
    EQ_VALUE(value2.GetNumber(), 785, "GetNumber()");

    value2 = vu_int{0};
    EQ_TRUE(value2.GetBool(bool_var), "GetBool()");
    EQ_FALSE(bool_var, "bool_var");

    value2 = int{-8};
    EQ_TRUE(value2.GetBool(bool_var), "GetBool()");
    EQ_FALSE(bool_var, "bool_var");

    value2 = ValueC{double{3.75}};
    EQ_TRUE(value2.IsNumber(), "IsNumber()");
    EQ_VALUE(value2.GetNumber(), 3.75, "GetNumber()");

    END_SUB_TEST;
}

static int TestNumberValue2() {
    using vu_short     = unsigned short;
    using vu_int       = unsigned int;
    using vu_long      = unsigned long;
    using vu_long_long = unsigned long long;

    ValueC  value1;
    VString str_var;
    bool    bool_var;

    double        double_var;
    long          long_var;
    unsigned long ulong_var;

    /////////////////// unsigned

    value1 = ValueC{vu_short{10}};
    EQ_VALUE(value1.Type(), ValueType::UInt64, "Type()");
    EQ_TRUE(value1.IsUInt64(), "IsUInt64()");
    EQ_FALSE(value1.IsInt64(), "IsInt64()");
    EQ_FALSE(value1.IsDouble(), "IsDouble()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), 10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), 10, "GetInt64()");
    EQ_VALUE(value1.GetUInt64(), 10, "GetUInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, 10, "double_var");
    EQ_VALUE(long_var, 10, "long_var");
    EQ_VALUE(ulong_var, 10, "ulong_var");
    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "10", "str_var");
    EQ_TRUE(value1.GetBool(bool_var), "GetBool()");
    EQ_TRUE(bool_var, "bool_var");
    value1.Reset();

    value1 = ValueC{vu_int{10}};
    EQ_TRUE(value1.IsUInt64(), "IsUInt64()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), 10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), 10, "GetInt64()");
    EQ_VALUE(value1.GetUInt64(), 10, "GetUInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, 10, "double_var");
    EQ_VALUE(long_var, 10, "long_var");
    EQ_VALUE(ulong_var, 10, "ulong_var");
    value1.Reset();

    value1 = ValueC{vu_long{10}};
    EQ_TRUE(value1.IsUInt64(), "IsUInt64()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), 10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), 10, "GetInt64()");
    EQ_VALUE(value1.GetUInt64(), 10, "GetUInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, 10, "double_var");
    EQ_VALUE(long_var, 10, "long_var");
    EQ_VALUE(ulong_var, 10, "ulong_var");
    value1.Reset();

    value1 = ValueC{vu_long_long{10}};
    EQ_TRUE(value1.IsUInt64(), "IsUInt64()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), 10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), 10, "GetInt64()");
    EQ_VALUE(value1.GetUInt64(), 10, "GetUInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, 10, "double_var");
    EQ_VALUE(long_var, 10, "long_var");
    EQ_VALUE(ulong_var, 10, "ulong_var");
    value1.Reset();

    END_SUB_TEST;
}

static int TestNumberValue3() {
    using v_long_long = long long;

    ValueC  value1;
    VString str_var;
    bool    bool_var;

    double        double_var;
    long          long_var;
    unsigned long ulong_var;

    /////////////////// signed

    value1 = ValueC{short{-10}};
    EQ_VALUE(value1.Type(), ValueType::Int64, "Type()");
    EQ_FALSE(value1.IsUInt64(), "IsUInt64()");
    EQ_TRUE(value1.IsInt64(), "IsInt64()");
    EQ_FALSE(value1.IsDouble(), "IsDouble()");
    EQ_VALUE(value1.GetNumber(), -10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), -10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), -10, "GetInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, -10, "double_var");
    EQ_VALUE(long_var, -10, "long_var");
    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "-10", "str_var");
    EQ_TRUE(value1.GetBool(bool_var), "GetBool()");
    EQ_FALSE(bool_var, "bool_var");
    value1.Reset();

    value1 = ValueC{short{10}};
    EQ_TRUE(value1.IsInt64(), "IsInt64()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), 10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), 10, "GetInt64()");
    EQ_VALUE(value1.GetUInt64(), 10, "GetUInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, 10, "double_var");
    EQ_VALUE(long_var, 10, "long_var");
    EQ_VALUE(ulong_var, 10, "ulong_var");
    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "10", "str_var");
    value1.Reset();

    value1 = ValueC{int{-10}};
    EQ_TRUE(value1.IsInt64(), "IsInt64()");
    EQ_VALUE(value1.GetNumber(), -10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), -10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), -10, "GetInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, -10, "double_var");
    EQ_VALUE(long_var, -10, "long_var");
    value1.Reset();

    value1 = ValueC{int{10}};
    EQ_TRUE(value1.IsInt64(), "IsInt64()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), 10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), 10, "GetInt64()");
    EQ_VALUE(value1.GetUInt64(), 10, "GetUInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, 10, "double_var");
    EQ_VALUE(long_var, 10, "long_var");
    EQ_VALUE(ulong_var, 10, "ulong_var");
    value1.Reset();

    value1 = ValueC{long{-10}};
    EQ_TRUE(value1.IsInt64(), "IsInt64()");
    EQ_VALUE(value1.GetNumber(), -10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), -10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), -10, "GetInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, -10, "double_var");
    EQ_VALUE(long_var, -10, "long_var");
    value1.Reset();

    value1 = ValueC{long{10}};
    EQ_TRUE(value1.IsInt64(), "IsInt64()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), 10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), 10, "GetInt64()");
    EQ_VALUE(value1.GetUInt64(), 10, "GetUInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, 10, "double_var");
    EQ_VALUE(long_var, 10, "long_var");
    EQ_VALUE(ulong_var, 10, "ulong_var");
    value1.Reset();

    value1 = ValueC{long{-10}};
    EQ_TRUE(value1.IsInt64(), "IsInt64()");
    EQ_VALUE(value1.GetNumber(), -10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), -10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), -10, "GetInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, -10, "double_var");
    EQ_VALUE(long_var, -10, "long_var");
    value1.Reset();

    value1 = ValueC{v_long_long{10}};
    EQ_TRUE(value1.IsInt64(), "IsInt64()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), 10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), 10, "GetInt64()");
    EQ_VALUE(value1.GetUInt64(), 10, "GetUInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, 10, "double_var");
    EQ_VALUE(long_var, 10, "long_var");
    EQ_VALUE(ulong_var, 10, "ulong_var");
    value1.Reset();

    END_SUB_TEST;
}

static int TestNumberValue4() {
    ValueC  value1;
    VString str_var;
    bool    bool_var;

    double        double_var;
    long          long_var;
    unsigned long ulong_var;

    /////////////////// float

    value1 = ValueC{float{10.5}};
    EQ_VALUE(value1.Type(), ValueType::Double, "Type()");
    EQ_FALSE(value1.IsUInt64(), "IsUInt64()");
    EQ_FALSE(value1.IsInt64(), "IsInt64()");
    EQ_TRUE(value1.IsDouble(), "IsDouble()");
    EQ_VALUE(value1.GetNumber(), 10.5, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), 10.5, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), 10, "GetInt64()");
    EQ_VALUE(value1.GetUInt64(), 10, "GetUInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, 10.5, "double_var");
    EQ_VALUE(long_var, 10, "long_var");
    EQ_VALUE(ulong_var, 10, "ulong_var");
    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "10.5", "str_var");
    EQ_TRUE(value1.GetBool(bool_var), "GetBool()");
    EQ_TRUE(bool_var, "bool_var");
    value1.Reset();

    value1 = ValueC{float{-10.5}};
    EQ_TRUE(value1.IsDouble(), "IsDouble()");
    EQ_VALUE(value1.GetNumber(), -10.5, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), -10.5, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), -10, "GetInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, -10.5, "double_var");
    EQ_VALUE(long_var, -10, "long_var");
    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "-10.5", "str_var");
    EQ_TRUE(value1.GetBool(bool_var), "GetBool()");
    EQ_FALSE(bool_var, "bool_var");
    value1.Reset();

    value1 = ValueC{float{10}};
    EQ_TRUE(value1.IsDouble(), "IsDouble()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), 10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), 10, "GetInt64()");
    EQ_VALUE(value1.GetUInt64(), 10, "GetUInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, 10, "double_var");
    EQ_VALUE(long_var, 10, "long_var");
    EQ_VALUE(ulong_var, 10, "ulong_var");
    value1.Reset();

    value1 = ValueC{float{-10}};
    EQ_TRUE(value1.IsDouble(), "IsDouble()");
    EQ_VALUE(value1.GetNumber(), -10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), -10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), -10, "GetInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, -10, "double_var");
    EQ_VALUE(long_var, -10, "long_var");
    value1.Reset();

    value1 = ValueC{double{10.5}};
    EQ_TRUE(value1.IsDouble(), "IsDouble()");
    EQ_VALUE(value1.GetNumber(), 10.5, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), 10.5, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), 10, "GetInt64()");
    EQ_VALUE(value1.GetUInt64(), 10, "GetUInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, 10.5, "double_var");
    EQ_VALUE(long_var, 10, "long_var");
    EQ_VALUE(ulong_var, 10, "ulong_var");
    value1.Reset();

    value1 = ValueC{double{-10.5}};
    EQ_TRUE(value1.IsDouble(), "IsDouble()");
    EQ_VALUE(value1.GetNumber(), -10.5, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), -10.5, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), -10, "GetInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, -10.5, "double_var");
    EQ_VALUE(long_var, -10, "long_var");
    value1.Reset();

    value1 = ValueC{double{10}};
    EQ_TRUE(value1.IsDouble(), "IsDouble()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), 10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), 10, "GetInt64()");
    EQ_VALUE(value1.GetUInt64(), 10, "GetUInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, 10, "double_var");
    EQ_VALUE(long_var, 10, "long_var");
    EQ_VALUE(ulong_var, 10, "ulong_var");
    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "10", "str_var");
    EQ_TRUE(value1.GetBool(bool_var), "GetBool()");
    EQ_TRUE(bool_var, "bool_var");
    value1.Reset();

    value1 = ValueC{double{-10}};
    EQ_TRUE(value1.IsDouble(), "IsDouble()");
    EQ_VALUE(value1.GetNumber(), -10, "GetNumber()");
    EQ_VALUE(value1.GetDouble(), -10, "GetDouble()");
    EQ_VALUE(value1.GetInt64(), -10, "GetInt64()");
    EQ_TRUE(value1.SetNumber(double_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(long_var), "SetNumber()");
    EQ_TRUE(value1.SetNumber(ulong_var), "SetNumber()");
    EQ_VALUE(double_var, -10, "double_var");
    EQ_VALUE(long_var, -10, "long_var");
    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "-10", "str_var");
    EQ_TRUE(value1.GetBool(bool_var), "GetBool()");
    EQ_FALSE(bool_var, "bool_var");
    value1.Reset();
    END_SUB_TEST;
}

static int TestNumberValue5() {
    using vu_short     = unsigned short;
    using vu_int       = unsigned int;
    using vu_long      = unsigned long;
    using vu_long_long = unsigned long long;

    using v_long_long = long long;

    ValueC value1;

    value1 = vu_short{10};
    EQ_TRUE(value1.IsUInt64(), "IsUInt64()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");

    value1 = -10;
    EQ_TRUE(value1.IsInt64(), "IsInt64()");
    EQ_VALUE(value1.GetNumber(), -10, "GetNumber()");

    value1 = -10.5;
    EQ_TRUE(value1.IsDouble(), "IsDouble()");
    EQ_VALUE(value1.GetNumber(), -10.5, "GetNumber()");

    value1 = vu_int{10};
    EQ_TRUE(value1.IsUInt64(), "IsUInt64()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");

    value1 = float{10};
    EQ_TRUE(value1.IsDouble(), "IsDouble()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");

    value1 = int{10};
    EQ_TRUE(value1.IsInt64(), "IsInt64()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");

    value1 = vu_long{10};
    EQ_TRUE(value1.IsUInt64(), "IsUInt64()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");

    value1 = long{-10};
    EQ_TRUE(value1.IsInt64(), "IsInt64()");
    EQ_VALUE(value1.GetNumber(), -10, "GetNumber()");

    value1 = vu_long_long{10};
    EQ_TRUE(value1.IsUInt64(), "IsUInt64()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");

    value1 = double{-10};
    EQ_TRUE(value1.IsDouble(), "IsDouble()");
    EQ_VALUE(value1.GetNumber(), -10, "GetNumber()");

    value1 = v_long_long{10};
    EQ_TRUE(value1.IsInt64(), "IsInt64()");
    EQ_VALUE(value1.GetNumber(), 10, "GetNumber()");

    END_SUB_TEST;
}

static int TestStringValue() {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    VString       str_var;
    const char *  c_str_var;
    SizeT         c_str_len;
    double        num_var;
    bool          bool_var;

    value1 = "-ABCDEF0123456789ABCDEF0123456789-";
    EQ_TRUE(value1.IsString(), "IsString()");
    EQ_VALUE(value1.Type(), ValueType::String, "Type()");
    EQ_VALUE(value1.Size(), 0, "Size()");
    EQ_TO(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    EQ_TO(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    EQ_TO(value1.InsertKey(ss_var, 0), false, "InsertKey(0)", "false");
    EQ_TO(value1.SetCharAndLength(c_str_var, c_str_len), true,
          "SetCharAndLength()", "true");
    EQ_TO(StringUtils::IsEqual(value1.StringStorage(), c_str_var, c_str_len),
          true, "value1.StringStorage()", "value1.SetCharAndLength()");
    EQ_TO(value1.GetObject(), nullptr, "GetObject()", "null");
    EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetString(), nullptr, "GetString()", "null");
    EQ_VALUE(*(value1.GetString()), "-ABCDEF0123456789ABCDEF0123456789-",
             "GetString()");
    NOT_EQ_TO(value1.StringStorage(), nullptr, "StringStorage()", "null");
    EQ_TRUE(StringUtils::IsEqual(value1.StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "IsEqual()");
    EQ_VALUE(value1.Length(), 34, "Length()");
    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "-ABCDEF0123456789ABCDEF0123456789-", "str_var");
    EQ_TRUE(value1.InsertString(ss_var), "InsertString()");
    EQ_VALUE(ss_var, "-ABCDEF0123456789ABCDEF0123456789-", "ss_var");
    EQ_VALUE(value1.GetNumber(), 0, "GetNumber()");
    EQ_FALSE(value1.SetNumber(num_var), "SetNumber()");
    EQ_FALSE(value1.GetBool(bool_var), "GetBool()");
    EQ_VALUE(value1.Stringify(), "", "Stringify()");
    ss_var.Reset();

    value1.Reset();
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    value1 = "45";
    value2 = "-50";
    EQ_TRUE(value2.SetNumber(num_var), "SetNumber()");
    EQ_VALUE(num_var, -50, "num_var");
    EQ_VALUE(value2.GetNumber(), -50, "GetNumber()");

    value2 = value1;
    EQ_TRUE(value1.IsString(), "IsString()");
    NOT_EQ_TO(value1.GetString(), nullptr, "GetString()", "null");
    EQ_VALUE(*(value1.GetString()), "45", "GetString()");
    NOT_EQ_TO(value1.StringStorage(), nullptr, "StringStorage()", "null");
    EQ_TRUE(StringUtils::IsEqual(value1.StringStorage(), "45", 2), "IsEqual()");
    EQ_VALUE(value1.Length(), 2, "Length()");
    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "45", "str_var");
    EQ_TRUE(value1.SetNumber(num_var), "SetNumber()");
    EQ_VALUE(num_var, 45, "num_var");
    EQ_VALUE(value1.GetNumber(), 45, "GetNumber()");
    EQ_FALSE(value1.GetBool(bool_var), "GetBool()");
    EQ_TRUE(value2.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "45", "str_var");

    value1 = "true";
    value2 = ValueC{value1};
    EQ_TRUE(value2.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "true", "str_var");
    EQ_VALUE(value2.GetNumber(), 0, "GetNumber()");
    bool_var = false;
    EQ_TRUE(value2.GetBool(bool_var), "GetBool()");
    EQ_TRUE(bool_var, "bool_var");

    EQ_TRUE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "true", "str_var");

    value2.Reset();
    value2 = static_cast<ValueC &&>(value1);
    EQ_TRUE(value2.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "true", "str_var");

    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    value1 = "false";
    ValueC value3(static_cast<ValueC &&>(value1));
    EQ_VALUE(value3.Length(), 5, "Length()");
    EQ_TRUE(value3.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "false", "str_var");
    bool_var = true;
    EQ_TRUE(value3.GetBool(bool_var), "GetBool()");
    EQ_FALSE(bool_var, "bool_var");

    str_var           = "qen";
    const char *c_str = str_var.First();
    value3            = str_var; // Copy of a string
    EQ_TRUE(value3.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "qen", "str_var");
    NOT_EQ_TO(value3.StringStorage(), c_str, "str_var.First()", "c_str");

    value3 = ValueC{VString("ABC")};
    EQ_TRUE(value3.IsString(), "IsString()");
    NOT_EQ_TO(value3.GetString(), nullptr, "GetString()", "null");
    EQ_VALUE(*(value3.GetString()), "ABC", "GetString()");

    value3 = VString("123");
    EQ_TRUE(value3.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "123", "str_var");

    value3 = 321;
    value3 = "321";
    EQ_TRUE(value3.IsString(), "IsString()");
    EQ_TRUE(value3.SetString(str_var), "SetString()");
    EQ_VALUE(str_var, "321", "str_var");

    END_SUB_TEST;
}

static int TestArrayValue() {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    VArray        arr_var;
    const ValueC *storage;
    VString       str_var;
    const char *  c_str_var;
    SizeT         c_str_len;
    double        num_var;
    bool          bool_var;

    arr_var.ResizeAndInitialize(5);
    storage = arr_var.First();

    value1 = arr_var; // Copy.
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Type(), ValueType::Array, "Type()");
    EQ_VALUE(value1.Size(), 5, "Size()");
    EQ_TO(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    EQ_TO(value1.GetValue(4), nullptr, "GetValue(4)", "null");
    EQ_TO(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    EQ_TO(value1.InsertKey(ss_var, 0), false, "InsertKey(0)", "false");
    EQ_TO(value1.SetCharAndLength(c_str_var, c_str_len), false,
          "SetCharAndLength()", "false");
    EQ_TO(value1.GetObject(), nullptr, "GetObject()", "null");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), storage, "GetArray()->First()",
              "storage");
    EQ_TO(value1.GetString(), nullptr, "GetString()", "null");
    EQ_TO(value1.StringStorage(), nullptr, "StringStorage()", "null");
    EQ_VALUE(value1.Length(), 0, "Length()");
    EQ_FALSE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(value1.GetNumber(), 0, "GetNumber()");
    EQ_FALSE(value1.SetNumber(num_var), "SetNumber()");
    EQ_FALSE(value1.GetBool(bool_var), "GetBool()");
    EQ_VALUE(value1.Stringify(), "[]", "Stringify()");

    arr_var.Reset();
    value1 = arr_var;
    EQ_VALUE(value1.Stringify(), "[]", "Stringify()");

    value1.Reset();
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    arr_var.Reset();
    arr_var.ResizeAndInitialize(10);
    storage = arr_var.First();

    VArray arr_var2;
    arr_var2.ResizeAndInitialize(10);

    value1 = static_cast<VArray &&>(arr_var); // Move
    value2 = static_cast<VArray &&>(arr_var2);
    value2 = value1;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 10, "Size()");
    EQ_TO(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    EQ_TO(value1.GetValue(9), nullptr, "GetValue(9)", "null");
    EQ_TO(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value1.GetArray()->First(), storage, "GetArray()->First()",
          "storage");

    EQ_VALUE(value2.Size(), 10, "Size()");
    NOT_EQ_TO(value2.GetArray()->First(), storage, "GetArray()->First()",
              "storage");

    arr_var.Reset();
    arr_var.ResizeAndInitialize(7);
    storage = arr_var.First();
    value1  = static_cast<VArray &&>(arr_var);

    value2 = ValueC{value1};
    EQ_TRUE(value2.IsArray(), "IsArray()");
    EQ_VALUE(value2.Size(), 7, "Size()");
    NOT_EQ_TO(value2.GetArray()->First(), storage, "GetArray()->First()",
              "storage");

    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 7, "Size()");
    EQ_TO(value1.GetArray()->First(), storage, "GetArray()->First()",
          "storage");

    value2.Reset();
    value2 = static_cast<ValueC &&>(value1);
    EQ_TO(value2.GetArray()->First(), storage, "GetArray()->First()",
          "storage");

    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    arr_var.Reset();
    arr_var.ResizeAndInitialize(7);
    storage = arr_var.First();
    value1  = static_cast<VArray &&>(arr_var);

    ValueC value3(static_cast<ValueC &&>(value1));
    EQ_TO(value3.GetArray()->First(), storage, "GetArray()->First()",
          "storage");

    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    arr_var.Reset();
    arr_var.ResizeAndInitialize(3);
    storage = arr_var.First();
    value3  = arr_var; // Copy
    EQ_VALUE(value3.Size(), 3, "Size()");
    NOT_EQ_TO(value3.GetArray()->First(), storage, "GetArray()->First()",
              "storage");

    arr_var.Reset();
    arr_var.ResizeAndInitialize(13);
    storage = arr_var.First();
    value3  = static_cast<VArray &&>(arr_var); // Move
    EQ_TO(value3.GetArray()->First(), storage, "GetArray()->First()",
          "storage");

    value3 = ValueC{VArray(3)};
    EQ_TRUE(value3.IsArray(), "IsArray()");
    NOT_EQ_TO(value3.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value3.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_VALUE(value3.GetArray()->Capacity(), 3, "GetArray()->Capacity()");

    value1[0] = 1;

    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 1, "Size()");
    EQ_VALUE(value1[0].GetNumber(), 1, R"(value1[0].GetNumber())");

    value1.Reset();

    value1[0] = 1;
    value1[1] = 2;
    value1[2] = 3;

    EQ_VALUE(value1.Size(), 3, "value1.Size()");

    value2 = value1;
    EQ_VALUE(value2.Size(), 3, "value2.Size()");
    EQ_VALUE(value2[0].GetNumber(), 1, "value2[0].GetNumber()");
    EQ_VALUE(value2[1].GetNumber(), 2, "value2[1].GetNumber()");
    EQ_VALUE(value2[2].GetNumber(), 3, "value2[2].GetNumber()");

    value1[0] = 11;
    value1[1] = 22;
    str_var   = "Qen";
    c_str_var = str_var.First();
    value1[2] = static_cast<VString &&>(str_var);

    value2 = value1;
    EQ_VALUE(value2.Size(), 3, "value2.Size()");
    EQ_VALUE(value2[0].GetNumber(), 11, "value2[0].GetNumber()");
    EQ_VALUE(value2[1].GetNumber(), 22, "value2[1].GetNumber()");
    NOT_EQ_TO(value2[2].StringStorage(), c_str_var, "value2[2].StringStorage()",
              "c_str_var");
    EQ_TO(StringUtils::IsEqual(value2[2].StringStorage(), "Qen", 3), true,
          "value2[2].StringStorage()", "Qen");

    value2[3] = 44;
    value2[4] = 55;
    EQ_VALUE(value2.Size(), 5, "value2.Size()");
    EQ_VALUE(value2[0].GetNumber(), 11, "value2[0].GetNumber()");
    EQ_VALUE(value2[1].GetNumber(), 22, "value2[1].GetNumber()");
    NOT_EQ_TO(value2[2].StringStorage(), c_str_var, "value2[2].StringStorage()",
              "c_str_var");
    EQ_TO(StringUtils::IsEqual(value2[2].StringStorage(), "Qen", 3), true,
          "value2[2].StringStorage()", "Qen");
    EQ_VALUE(value2[3].GetNumber(), 44, "value2[3].GetNumber()");
    EQ_VALUE(value2[4].GetNumber(), 55, "value2[4].GetNumber()");

    value2 = value1;
    EQ_VALUE(value2.Size(), 3, "value2.Size()");
    EQ_VALUE(value2[0].GetNumber(), 11, "value2[0].GetNumber()");
    EQ_VALUE(value2[1].GetNumber(), 22, "value2[1].GetNumber()");
    NOT_EQ_TO(value2[2].StringStorage(), c_str_var, "value2[2].StringStorage()",
              "c_str_var");
    EQ_TO(StringUtils::IsEqual(value2[2].StringStorage(), "Qen", 3), true,
          "value2[2].StringStorage()", "Qen");
    EQ_TO(value2.GetValue(3), nullptr, "value2.GetValue(3)", "null");
    EQ_TO(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null");

    ////////////////////
    arr_var.ResizeAndInitialize(4);
    arr_var[0] = 10;
    arr_var[1] = 20;
    arr_var[2] = 30;
    str_var    = "-ABCDEF0123456789ABCDEF0123456789-";
    c_str_var  = str_var.First();
    arr_var[3] = static_cast<VString &&>(str_var);

    value2 = arr_var;
    EQ_VALUE(value2.Size(), 4, "value2.Size()");
    EQ_VALUE(value2[0].GetNumber(), 10, "value2[0].GetNumber()");
    EQ_VALUE(value2[1].GetNumber(), 20, "value2[1].GetNumber()");
    EQ_VALUE(value2[2].GetNumber(), 30, "value2[1].GetNumber()");
    NOT_EQ_TO(value2[3].StringStorage(), c_str_var, "value2[3].StringStorage()",
              "c_str_var");
    EQ_TO(StringUtils::IsEqual(value2[3].StringStorage(),
                               "-ABCDEF0123456789ABCDEF0123456789-", 34),
          true, "value2[3].StringStorage()",
          "-ABCDEF0123456789ABCDEF0123456789-");
    EQ_TO(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null");

    ////////////////////

    arr_var.Clear();
    arr_var.ResizeAndInitialize(10);
    arr_var.GoBackTo(5);
    value2 = static_cast<VArray &&>(arr_var);
    EQ_VALUE(value2.Size(), 5, "value2.Size()");

    value2 += 1;

    value2.Compress();
    NOT_EQ_TO(value2.GetArray(), nullptr, "GetArray()", "null");
    EQ_VALUE(value2.GetArray()->Capacity(), 1, "Capacity()");
    EQ_VALUE(value2.GetArray()->Size(), 1, "Size()");

    arr_var.Reset();
    arr_var.Reserve(10);
    value2 = static_cast<VArray &&>(arr_var);
    value2.Compress();
    NOT_EQ_TO(value2.GetArray(), nullptr, "GetArray()", "null");
    EQ_VALUE(value2.GetArray()->Capacity(), 0, "Capacity()");
    EQ_VALUE(value2.GetArray()->Size(), 0, "Size()");

    END_SUB_TEST;
}

static int TestObjectValue1() {
    ValueC value1;
    ValueC value2;

    using ObjectItem_ = HAItem<ValueC, char>;

    VHArray            h_arr_var;
    const ObjectItem_ *storage;
    VStringStream      ss_var;
    VString            str_var;
    const char *       c_str_var;
    SizeT              c_str_len;
    double             num_var;
    bool               bool_var;

    for (unsigned int i = 0; i < 5; i++) {
        VString key("Key_");
        key += Digit<char>::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();

    value1 = h_arr_var; // Copy.
    EQ_TRUE(value1.IsObject(), "IsObject()");
    EQ_VALUE(value1.Type(), ValueType::Object, "Type()");
    EQ_VALUE(value1.Size(), 5, "Size()");
    NOT_EQ_TO(value1.GetValue(0), nullptr, "GetValue(0)", "null");
    NOT_EQ_TO(value1.GetValue(4), nullptr, "GetValue(4)", "null");
    NOT_EQ_TO(value1.GetKey(0), nullptr, "GetKey(0)", "null");
    NOT_EQ_TO(value1.GetKey(4), nullptr, "GetKey(4)", "null");
    NOT_EQ_TO(value1.InsertKey(ss_var, 0), false, "InsertKey(0)", "false");
    NOT_EQ_TO(value1.InsertKey(ss_var, 4), false, "InsertKey(4)", "false");
    EQ_TO(value1.SetCharAndLength(c_str_var, c_str_len), false,
          "SetCharAndLength()", "false");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetObject()->First(), storage, "GetArray()->First()",
              "storage");
    EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetString(), nullptr, "GetString()", "null");
    EQ_TO(value1.StringStorage(), nullptr, "StringStorage()", "null");
    EQ_VALUE(value1.Length(), 0, "Length()");
    EQ_FALSE(value1.SetString(str_var), "SetString()");
    EQ_VALUE(value1.GetNumber(), 0, "GetNumber()");
    EQ_FALSE(value1.SetNumber(num_var), "SetNumber()");
    EQ_FALSE(value1.GetBool(bool_var), "GetBool()");

    h_arr_var.Reset();
    value1 = h_arr_var;
    EQ_VALUE(value1.Stringify(), "{}", "Stringify()");

    value1.Reset();
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    value2 = h_arr_var;

    h_arr_var.Reset();
    for (unsigned int i = 0; i < 10; i++) {
        VString key("Key_");
        key += Digit<char>::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();

    value1 = static_cast<VHArray &&>(h_arr_var); // Move
    value2 = value1;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    EQ_VALUE(value1.Size(), 10, "Size()");
    EQ_TO(value1.GetObject()->First(), storage, "GetArray()->First()",
          "storage");

    value1 = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    EQ_VALUE(value1.Stringify(), "{}", "Stringify()");

    EQ_TRUE(value2.IsObject(), "IsObject()");
    NOT_EQ_TO(value2.GetObject()->First(), storage, "GetArray()->First()",
              "storage");

    h_arr_var.Reset();
    // Testing empty values
    for (unsigned int i = 0; i < 10; i++) {
        VString key("Key_");
        key += Digit<char>::NumberToString(i);
        h_arr_var[key];
    }

    value2 = h_arr_var;
    EQ_VALUE(value2.Size(), 10, "Size()");

    h_arr_var.Reset();
    for (SizeT i = 0; i < 7; i++) {
        VString key("Key_");
        key += Digit<char>::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value1  = static_cast<VHArray &&>(h_arr_var);

    value2 = ValueC{value1};
    EQ_TRUE(value2.IsObject(), "IsObject()");
    EQ_VALUE(value2.Size(), 7, "Size()");
    NOT_EQ_TO(value2.GetObject()->First(), storage, "GetArray()->First()",
              "storage");

    h_arr_var.Reset();
    value2 = h_arr_var;
    EQ_TRUE(value2.IsObject(), "IsObject()");
    EQ_TRUE(value1.IsObject(), "IsObject()");
    EQ_TO(value1.GetObject()->First(), storage, "GetArray()->First()",
          "storage");

    h_arr_var.Reset();
    value1 = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");

    for (unsigned int i = 0; i < 7; i++) {
        VString key("Key_");
        key += Digit<char>::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value1  = static_cast<VHArray &&>(h_arr_var);

    value2.Reset();
    value2 = static_cast<ValueC &&>(value1);
    EQ_TRUE(value2.IsObject(), "IsObject()");
    EQ_TO(value2.GetObject()->First(), storage, "GetArray()->First()",
          "storage");

    h_arr_var.Reset();
    value2 = h_arr_var;
    EQ_VALUE(value2.Stringify(), "{}", "Stringify()");
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    END_SUB_TEST;
}

static int TestObjectValue2() {
    ValueC value1;
    ValueC value2;

    using ObjectItem_ = HAItem<ValueC, char>;

    VHArray            h_arr_var;
    const ObjectItem_ *storage;
    VStringStream      ss_var;
    VString            str_var;
    const char *       c_str_var;

    h_arr_var.Reset();
    for (unsigned int i = 0; i < 7; i++) {
        VString key("Key_");
        key += Digit<char>::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value1  = static_cast<VHArray &&>(h_arr_var);

    ValueC value3(static_cast<ValueC &&>(value1));
    EQ_TRUE(value3.IsObject(), "IsObject()");
    EQ_VALUE(value3.Size(), 7, "Size()");
    EQ_TO(value3.GetObject()->First(), storage, "GetArray()->First()",
          "storage");

    h_arr_var.Reset();
    value3 = h_arr_var;
    EQ_VALUE(value3.Stringify(), "{}", "Stringify()");
    EQ_TRUE(value1.IsUndefined(), "isUndefined()");

    h_arr_var.Reset();
    for (unsigned int i = 0; i < 3; i++) {
        VString key("Key_");
        key += Digit<char>::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value3  = h_arr_var; // Copy
    EQ_VALUE(value3.Size(), 3, "Size()");
    NOT_EQ_TO(value3.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value3.GetObject()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value3.GetObject()->First(), storage, "GetArray()->First()",
              "storage");

    for (unsigned int i = 0; i < 13; i++) {
        VString key("Key_");
        key += Digit<char>::NumberToString(i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value3  = static_cast<VHArray &&>(h_arr_var); // Move
    EQ_TRUE(value3.IsObject(), "IsObject()");
    EQ_VALUE(value3.Size(), 13, "Size()");
    NOT_EQ_TO(value3.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value3.GetObject()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value3.GetObject()->First(), storage, "GetArray()->First()",
          "storage");

    value3 = ValueC{VHArray(4)};
    EQ_TRUE(value3.IsObject(), "IsObject()");
    NOT_EQ_TO(value3.GetObject(), nullptr, "GetObject()", "null");
    NOT_EQ_TO(value3.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    EQ_VALUE(value3.GetObject()->Capacity(), 4, "GetArray()->Capacity()");

    value1.Reset();
    value1[""] = 1;

    EQ_TRUE(value1.IsObject(), "IsObject()");
    EQ_VALUE(value1.Size(), 1, "Size()");
    EQ_VALUE(value1[""].GetNumber(), 1, R"(value1[""].GetNumber())");

    value1.Reset();

    value1["k1"] = 1;
    value1["k2"] = 2;
    value1["k3"] = 3;

    EQ_VALUE(value1.Size(), 3, "value1.Size()");

    value3 = value1;
    EQ_VALUE(value3.Size(), 3, "value3.Size()");
    EQ_VALUE(value3["k1"].GetNumber(), 1, "value3[\"k1\"].GetNumber()");
    EQ_VALUE(value3["k2"].GetNumber(), 2, "value3[\"k2\"].GetNumber()");
    EQ_VALUE(value3["k3"].GetNumber(), 3, "value3[\"k3\"].GetNumber()");

    value1["k1"] = 11;
    value1["k2"] = 22;
    str_var      = "Qen";
    c_str_var    = str_var.First();
    value1["k3"] = static_cast<VString &&>(str_var);

    value3 = value1;
    EQ_VALUE(value3.Size(), 3, "value3.Size()");
    EQ_VALUE(value3["k1"].GetNumber(), 11, "value3[\"k1\"].GetNumber()");
    EQ_VALUE(value3["k2"].GetNumber(), 22, "value3[\"k2\"].GetNumber()");
    NOT_EQ_TO(value3["k3"].StringStorage(), c_str_var,
              "value3[\"k3\"].StringStorage()", "c_str_var");
    EQ_TO(StringUtils::IsEqual(value3["k3"].StringStorage(), "Qen", 3), true,
          "value3[\"k3\"].StringStorage()", "Qen");

    value3["k4"] = 44;
    value3["k5"] = 55;
    EQ_VALUE(value3.Size(), 5, "value3.Size()");
    EQ_VALUE(value3["k1"].GetNumber(), 11, "value3[\"k1\"].GetNumber()");
    EQ_VALUE(value3["k2"].GetNumber(), 22, "value3[\"k2\"].GetNumber()");
    NOT_EQ_TO(value3["k3"].StringStorage(), c_str_var,
              "value3[\"k3\"].StringStorage()", "c_str_var");
    EQ_TO(StringUtils::IsEqual(value3["k3"].StringStorage(), "Qen", 3), true,
          "value3[\"k3\"].StringStorage()", "Qen");
    EQ_VALUE(value3["k4"].GetNumber(), 44, "value3[\"k4\"].GetNumber()");
    EQ_VALUE(value3["k5"].GetNumber(), 55, "value3[\"k5\"].GetNumber()");

    value3 = value1;
    EQ_VALUE(value3.Size(), 3, "value3.Size()");
    EQ_VALUE(value3["k1"].GetNumber(), 11, "value3[\"k1\"].GetNumber()");
    EQ_VALUE(value3["k2"].GetNumber(), 22, "value3[\"k2\"].GetNumber()");
    EQ_TO(StringUtils::IsEqual(value3["k3"].StringStorage(), "Qen", 3), true,
          "value3[\"k3\"].StringStorage()", "Qen");
    EQ_TO(value3.GetValue("k4", 2), nullptr, "value3.GetValue(\"k4\", 2)",
          "null");
    EQ_TO(value3.GetValue(3), nullptr, "value3.GetValue(3)", "null");
    EQ_TO(value3.GetValue("k5", 2), nullptr, "value3.GetValue(\"k5\", 2)",
          "null");
    EQ_TO(value3.GetValue(4), nullptr, "value3.GetValue(4)", "null");

    ////////////////////
    h_arr_var[VString("w1")] = 10;
    h_arr_var[VString("w2")] = 20;
    h_arr_var[VString("w3")] = 30;
    str_var                  = "-ABCDEF0123456789ABCDEF0123456789-";
    c_str_var                = str_var.First();
    h_arr_var[VString("w4")] = static_cast<VString &&>(str_var);

    value3 = h_arr_var;
    EQ_VALUE(value3.Size(), 4, "value3.Size()");
    EQ_VALUE(value3["w1"].GetNumber(), 10, "[\"w1\"].GetNumber()");
    EQ_VALUE(value3["w2"].GetNumber(), 20, "[\"w2\"].GetNumber()");
    EQ_VALUE(value3["w3"].GetNumber(), 30, "[\"w3\"].GetNumber()");
    NOT_EQ_TO(value3["w4"].StringStorage(), c_str_var,
              "value3[\"w4\"].StringStorage()", "c_str_var");
    EQ_TO(StringUtils::IsEqual(value3["w4"].StringStorage(),
                               "-ABCDEF0123456789ABCDEF0123456789-", 34),
          true, "value3[\"w4\"].StringStorage()", "Qen");
    EQ_TO(value3.GetValue("k5", 2), nullptr, "value3.GetValue(\"k5\", 2)",
          "null");
    EQ_TO(value3.GetValue(4), nullptr, "value3.GetValue(4)", "null");
    ////////////////////

    h_arr_var.Reserve(10);
    h_arr_var[VString("w1")] = 10;
    h_arr_var[VString("w2")] = 20;
    h_arr_var[VString("w3")] = 30;

    value3 = static_cast<VHArray &&>(h_arr_var);
    EQ_VALUE(value3.Size(), 3, "value3.Size()");

    value3.Compress();
    NOT_EQ_TO(value3.GetObject(), nullptr, "GetArray()", "null");
    EQ_VALUE(value3.GetObject()->Size(), 3, "Size()");
    EQ_TRUE(value3.GetObject()->Capacity() >= 3,
            "GetObject()->Capacity() >= 3");

    END_SUB_TEST;
}

static int TestMoveValue1() {
    ValueC value1;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray                     h_arr_var;
    const HAItem<ValueC, char> *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = true;

    value1 = false;
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = true;

    value1 = nullptr;
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = true;

    value1 = 11;
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 11, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = true;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = static_cast<VString &&>(str_var);
    EQ_TRUE(value1.IsString(), "IsString()");
    EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
          "c_str_var");
    value1.Reset();

    /////////////
    value1 = true;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = static_cast<VArray &&>(arr_var);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
          "storage");
    value1.Reset();

    /////////////
    value1 = true;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = static_cast<VHArray &&>(h_arr_var);
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    EQ_TO(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = false;

    value1 = true;
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = false;

    value1 = nullptr;
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = false;

    value1 = -90;
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), -90, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = false;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = static_cast<VString &&>(str_var);
    EQ_TRUE(value1.IsString(), "IsString()");
    EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
          "c_str_var");
    value1.Reset();

    /////////////
    value1 = false;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = static_cast<VArray &&>(arr_var);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
          "storage");
    value1.Reset();

    /////////////
    value1 = false;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = static_cast<VHArray &&>(h_arr_var);
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    EQ_TO(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = nullptr;

    value1 = true;
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = nullptr;

    value1 = false;
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = nullptr;

    value1 = 7.5;
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 7.5, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = nullptr;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = static_cast<VString &&>(str_var);
    EQ_TRUE(value1.IsString(), "IsString()");
    EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
          "c_str_var");
    value1.Reset();

    /////////////
    value1 = nullptr;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = static_cast<VArray &&>(arr_var);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
          "storage");
    value1.Reset();

    /////////////
    value1 = nullptr;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = static_cast<VHArray &&>(h_arr_var);
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    EQ_TO(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = 13;

    value1 = true;
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = 40;

    value1 = false;
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = 33;

    value1 = nullptr;
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = 47;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = static_cast<VString &&>(str_var);
    EQ_TRUE(value1.IsString(), "IsString()");
    EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
          "c_str_var");
    value1.Reset();

    /////////////
    value1 = 10e10;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = static_cast<VArray &&>(arr_var);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
          "storage");
    value1.Reset();

    /////////////
    value1 = 9.1;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = static_cast<VHArray &&>(h_arr_var);
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    EQ_TO(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value1 = true;
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value1 = false;
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value1 = nullptr;
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value1 = 4;
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 4, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = static_cast<VArray &&>(arr_var);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
          "storage");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = static_cast<VHArray &&>(h_arr_var);
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    EQ_TO(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VArray(1);

    value1 = true;
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value1 = false;
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value1 = nullptr;
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value1 = 33;
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 33, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = static_cast<VString &&>(str_var);
    EQ_TRUE(value1.IsString(), "IsString()");
    EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
          "c_str_var");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = static_cast<VHArray &&>(h_arr_var);
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    EQ_TO(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestMoveValue2() {
    ValueC value1;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray h_arr_var;

    ////////////////////////////////////////////

    value1 = VHArray(1);

    value1 = true;
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value1 = false;
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value1 = nullptr;
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value1 = 33;
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 33, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = static_cast<VString &&>(str_var);
    EQ_TRUE(value1.IsString(), "IsString()");
    EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
          "c_str_var");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = static_cast<VArray &&>(arr_var);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestMoveValue3() {
    ValueC value1;
    ValueC value2;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray                     h_arr_var;
    const HAItem<ValueC, char> *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = true;

    value2 = false;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = true;

    value2 = nullptr;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = true;

    value2 = 11;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 11, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = true;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = static_cast<VString &&>(str_var);
    value1    = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsString(), "IsString()");
    EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
          "c_str_var");
    value1.Reset();

    /////////////
    value1 = true;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);
    value1      = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
          "storage");
    value1.Reset();

    /////////////
    value1 = true;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = static_cast<VHArray &&>(h_arr_var);
    value1        = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    EQ_TO(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = false;

    value2 = true;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = false;

    value2 = nullptr;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = false;

    value2 = -90;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), -90, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = false;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = static_cast<VString &&>(str_var);
    value1    = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsString(), "IsString()");
    EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
          "c_str_var");
    value1.Reset();

    /////////////
    value1 = false;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);
    value1      = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
          "storage");
    value1.Reset();

    /////////////
    value1 = false;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = static_cast<VHArray &&>(h_arr_var);
    value1        = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    EQ_TO(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = nullptr;

    value2 = true;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = nullptr;

    value2 = false;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = nullptr;

    value2 = 7.5;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 7.5, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = nullptr;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = static_cast<VString &&>(str_var);
    value1    = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsString(), "IsString()");
    EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
          "c_str_var");
    value1.Reset();

    /////////////
    value1 = nullptr;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);
    value1      = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
          "storage");
    value1.Reset();

    /////////////
    value1 = nullptr;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = static_cast<VHArray &&>(h_arr_var);
    value1        = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    EQ_TO(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = 13;

    value2 = true;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = 40;

    value2 = false;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = 33;

    value2 = nullptr;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = 47;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = static_cast<VString &&>(str_var);
    value1    = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsString(), "IsString()");
    EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
          "c_str_var");
    value1.Reset();

    /////////////
    value1 = 10e10;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);
    value1      = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
          "storage");
    value1.Reset();

    /////////////
    value1 = 9.1;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = static_cast<VHArray &&>(h_arr_var);
    value1        = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    EQ_TO(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = true;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = false;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = nullptr;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = 4;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 4, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);
    value1      = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
          "storage");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = static_cast<VHArray &&>(h_arr_var);
    value1        = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    EQ_TO(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VArray(1);

    value2 = true;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = false;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = nullptr;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = 33;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 33, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = static_cast<VString &&>(str_var);
    value1    = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsString(), "IsString()");
    EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
          "c_str_var");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = static_cast<VHArray &&>(h_arr_var);
    value1        = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    EQ_TO(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestMoveValue4() {
    ValueC value1;
    ValueC value2;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray h_arr_var;

    ////////////////////////////////////////////

    value1 = VHArray(1);

    value2 = true;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = false;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = nullptr;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = 33;
    value1 = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 33, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = static_cast<VString &&>(str_var);
    value1    = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsString(), "IsString()");
    EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
          "c_str_var");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);
    value1      = static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
          "storage");
    value1.Reset();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestCopyValue1() {
    ValueC value1;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray                     h_arr_var;
    const HAItem<ValueC, char> *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = true;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    EQ_TRUE(value1.IsString(), "IsString()");
    NOT_EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
              "c_str_var");
    value1.Reset();

    /////////////
    value1 = true;

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null");
    value1.Reset();
    value1 = true;

    // Has values
    arr_var = VArray();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    value1.Reset();

    /////////////
    value1 = true;

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null");
    value1.Reset();
    value1 = true;

    // Has values
    h_arr_var = VHArray();

    for (unsigned int i = 0; i < 3; i++) {
        h_arr_var[Digit<char>::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.First();
    value1        = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    NOT_EQ_TO(value1.GetObject()->First(), h_arr_storage,
              "GetObject()->First()", "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = false;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    EQ_TRUE(value1.IsString(), "IsString()");
    NOT_EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
              "c_str_var");
    value1.Reset();

    /////////////
    value1 = false;

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null");
    value1.Reset();
    value1 = false;

    // Has values
    arr_var = VArray();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    value1.Reset();

    /////////////
    value1 = false;

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null");
    value1.Reset();
    value1 = false;

    // Has values
    h_arr_var = VHArray();

    for (unsigned int i = 0; i < 3; i++) {
        h_arr_var[Digit<char>::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.First();
    value1        = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    NOT_EQ_TO(value1.GetObject()->First(), h_arr_storage,
              "GetObject()->First()", "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = nullptr;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    EQ_TRUE(value1.IsString(), "IsString()");
    NOT_EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
              "c_str_var");
    value1.Reset();

    /////////////
    value1 = nullptr;

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null");
    value1.Reset();
    value1 = nullptr;

    // Has values
    arr_var = VArray();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    value1.Reset();

    /////////////
    value1 = nullptr;

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null");
    value1.Reset();
    value1 = nullptr;

    // Has values
    h_arr_var = VHArray();

    for (unsigned int i = 0; i < 3; i++) {
        h_arr_var[Digit<char>::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.First();
    value1        = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    NOT_EQ_TO(value1.GetObject()->First(), h_arr_storage,
              "GetObject()->First()", "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = 23;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    EQ_TRUE(value1.IsString(), "IsString()");
    NOT_EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
              "c_str_var");
    value1.Reset();

    /////////////
    value1 = 33;

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null");
    value1.Reset();
    value1 = 33;

    // Has values
    arr_var = VArray();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    value1.Reset();

    /////////////
    value1 = 34;

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null");
    value1.Reset();
    value1 = 34;

    // Has values
    h_arr_var = VHArray();

    for (unsigned int i = 0; i < 3; i++) {
        h_arr_var[Digit<char>::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.First();
    value1        = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    NOT_EQ_TO(value1.GetObject()->First(), h_arr_storage,
              "GetObject()->First()", "storage");
    value1.Reset();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestCopyValue2() {
    ValueC value1;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray                     h_arr_var;
    const HAItem<ValueC, char> *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null");
    value1.Reset();
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // Has values
    arr_var = VArray();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null");
    value1.Reset();
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // Has values
    h_arr_var = VHArray();

    for (unsigned int i = 0; i < 3; i++) {
        h_arr_var[Digit<char>::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.First();
    value1        = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    NOT_EQ_TO(value1.GetObject()->First(), h_arr_storage,
              "GetObject()->First()", "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    EQ_TRUE(value1.IsString(), "IsString()");
    NOT_EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
              "c_str_var");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null");
    value1.Reset();
    value1 = VArray(1);

    // Has values
    h_arr_var = VHArray();

    for (unsigned int i = 0; i < 3; i++) {
        h_arr_var[Digit<char>::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.First();
    value1        = h_arr_var;
    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    NOT_EQ_TO(value1.GetObject()->First(), h_arr_storage,
              "GetObject()->First()", "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VHArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    EQ_TRUE(value1.IsString(), "IsString()");
    NOT_EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
              "c_str_var");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null");
    value1.Reset();
    value1 = VHArray(1);

    // Has values
    arr_var = VArray();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    value1.Reset();
    ////////////////////////////////////////////
    END_SUB_TEST;
}

static int TestCopyValue3() {
    ValueC value1;
    ValueC value2;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray                     h_arr_var;
    const HAItem<ValueC, char> *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = true;

    value2 = false;
    value1 = value2;
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = true;

    value2 = nullptr;
    value1 = value2;
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = true;

    value2 = 11;
    value1 = value2;
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 11, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = true;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = static_cast<VString &&>(str_var);
    value1    = value2;
    EQ_TRUE(value1.IsString(), "IsString()");
    NOT_EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
              "c_str_var");
    value1.Reset();

    /////////////
    value1 = true;

    // No values
    value2 = VArray(1);
    value1 = value2;

    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null");
    value1.Reset();
    value1 = true;

    // Has values
    arr_var = VArray();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);
    value1      = value2;

    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    value1.Reset();

    /////////////
    value1 = true;

    // No values
    value2 = VHArray(1);
    value1 = value2;

    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null");
    value1.Reset();
    value1 = true;

    // Has values
    h_arr_var = VHArray();

    for (unsigned int i = 0; i < 3; i++) {
        h_arr_var[Digit<char>::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.First();
    value2        = static_cast<VHArray &&>(h_arr_var);
    value1        = value2;

    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    NOT_EQ_TO(value1.GetObject()->First(), h_arr_storage,
              "GetObject()->First()", "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = false;

    value2 = true;
    value1 = value2;
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = false;

    value2 = nullptr;
    value1 = value2;
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = false;

    value2 = -90;
    value1 = value2;
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), -90, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = false;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = static_cast<VString &&>(str_var);
    value1    = value2;
    EQ_TRUE(value1.IsString(), "IsString()");
    NOT_EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
              "c_str_var");
    value1.Reset();

    /////////////
    value1 = false;

    // No values
    value2 = VArray(1);
    value1 = value2;

    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null");
    value1.Reset();
    value1 = false;

    // Has values
    arr_var = VArray();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);
    value1      = value2;

    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    value1.Reset();

    /////////////
    value1 = false;

    // No values
    value2 = VHArray(1);
    value1 = value2;

    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null");
    value1.Reset();
    value1 = false;

    // Has values
    h_arr_var = VHArray();

    for (unsigned int i = 0; i < 3; i++) {
        h_arr_var[Digit<char>::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.First();
    value2        = static_cast<VHArray &&>(h_arr_var);
    value1        = value2;

    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    NOT_EQ_TO(value1.GetObject()->First(), h_arr_storage,
              "GetObject()->First()", "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = nullptr;

    value2 = true;
    value1 = value2;
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = nullptr;

    value2 = false;
    value1 = value2;
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = nullptr;

    value2 = 7.5;
    value1 = value2;
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 7.5, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = nullptr;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = static_cast<VString &&>(str_var);
    value1    = value2;
    EQ_TRUE(value1.IsString(), "IsString()");
    NOT_EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
              "c_str_var");
    value1.Reset();

    /////////////
    value1 = nullptr;

    // No values
    value2 = VArray(1);
    value1 = value2;

    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null");
    value1.Reset();
    value1 = nullptr;

    // Has values
    arr_var = VArray();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);
    value1      = value2;

    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    value1.Reset();

    /////////////
    value1 = nullptr;

    // No values
    value2 = VHArray(1);
    value1 = value2;

    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null");
    value1.Reset();
    value1 = nullptr;

    // Has values
    h_arr_var = VHArray();

    for (unsigned int i = 0; i < 3; i++) {
        h_arr_var[Digit<char>::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.First();
    value2        = static_cast<VHArray &&>(h_arr_var);
    value1        = value2;

    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    NOT_EQ_TO(value1.GetObject()->First(), h_arr_storage,
              "GetObject()->First()", "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = 13;

    value2 = true;
    value1 = value2;
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = 40;

    value2 = false;
    value1 = value2;
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = 33;

    value2 = nullptr;
    value1 = value2;
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = 47;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = static_cast<VString &&>(str_var);
    value1    = value2;
    EQ_TRUE(value1.IsString(), "IsString()");
    NOT_EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
              "c_str_var");
    value1.Reset();

    /////////////
    value1 = 10e10;

    // No values
    value2 = VArray(1);
    value1 = value2;

    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null");
    value1.Reset();
    value1 = 10e10;

    // Has values
    arr_var = VArray();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);
    value1      = value2;

    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    value1.Reset();

    /////////////
    value1 = 9.1;

    // No values
    value2 = VHArray(1);
    value1 = value2;

    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null");
    value1.Reset();
    value1 = 9.1;

    // Has values
    h_arr_var = VHArray();

    for (unsigned int i = 0; i < 3; i++) {
        h_arr_var[Digit<char>::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.First();
    value2        = static_cast<VHArray &&>(h_arr_var);
    value1        = value2;

    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    NOT_EQ_TO(value1.GetObject()->First(), h_arr_storage,
              "GetObject()->First()", "storage");
    value1.Reset();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestCopyValue4() {
    ValueC value1;
    ValueC value2;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray                     h_arr_var;
    const HAItem<ValueC, char> *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = true;
    value1 = value2;
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = false;
    value1 = value2;
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = nullptr;
    value1 = value2;
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = 4;
    value1 = value2;
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 4, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // No values
    value2 = VArray(1);
    value1 = value2;

    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null");
    value1.Reset();
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // Has values
    arr_var = VArray();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);
    value1      = value2;

    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // No values
    value2 = VHArray(1);
    value1 = value2;

    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null");
    value1.Reset();
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // Has values
    h_arr_var = VHArray();

    for (unsigned int i = 0; i < 3; i++) {
        h_arr_var[Digit<char>::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.First();
    value2        = static_cast<VHArray &&>(h_arr_var);
    value1        = value2;

    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    NOT_EQ_TO(value1.GetObject()->First(), h_arr_storage,
              "GetObject()->First()", "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VArray(1);

    value2 = true;
    value1 = value2;
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = false;
    value1 = value2;
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = nullptr;
    value1 = value2;
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = 33;
    value1 = value2;
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 33, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = static_cast<VString &&>(str_var);
    value1    = value2;
    EQ_TRUE(value1.IsString(), "IsString()");
    NOT_EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
              "c_str_var");
    value1.Reset();

    /////////////
    value1 = VArray(1);

    // No values
    value2 = VHArray(1);
    value1 = value2;

    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null");
    value1.Reset();
    value1 = VArray(1);

    // Has values
    h_arr_var = VHArray();

    for (unsigned int i = 0; i < 3; i++) {
        h_arr_var[Digit<char>::NumberToString(i)] = i;
    }

    h_arr_storage = h_arr_var.First();
    value2        = static_cast<VHArray &&>(h_arr_var);
    value1        = value2;

    EQ_TRUE(value1.IsObject(), "IsObject()");
    NOT_EQ_TO(value1.GetObject(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetObject()->First(), nullptr, "GetObject()->First()",
              "null");
    NOT_EQ_TO(value1.GetObject()->First(), h_arr_storage,
              "GetObject()->First()", "storage");
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VHArray(1);

    value2 = true;
    value1 = value2;
    EQ_TRUE(value1.IsTrue(), "IsTrue()");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = false;
    value1 = value2;
    EQ_TRUE(value1.IsFalse(), "IsFalse()");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = nullptr;
    value1 = value2;
    EQ_TRUE(value1.IsNull(), "IsNull()");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = 33;
    value1 = value2;
    EQ_TRUE(value1.IsNumber(), "IsNumber()");
    EQ_VALUE(value1.GetNumber(), 33, "GetNumber()");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = static_cast<VString &&>(str_var);
    value1    = value2;
    EQ_TRUE(value1.IsString(), "IsString()");
    NOT_EQ_TO(value1.StringStorage(), c_str_var, "value1.StringStorage()",
              "c_str_var");
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    // No values
    value2 = VArray(1);
    value1 = value2;

    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null");
    value1.Reset();
    value1 = VHArray(1);

    // Has values
    arr_var = VArray();
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);
    value1      = value2;

    EQ_TRUE(value1.IsArray(), "IsArray()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    value1.Reset();

    ////////////////////////////////////////////

    END_SUB_TEST;
}

static int TestIndexOperator1() {
    using vu_int       = unsigned int;
    using vu_long_long = unsigned long long;

    ValueC  value;
    VString str1("D");
    VString str2("DEFG");
    bool    did_throw;

    try {
        did_throw = false;
        value[10] = 5;
    } catch (...) {
        did_throw = true;
    }

    EQ_TRUE(did_throw, "did_throw");

    value[0] = 5;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_VALUE(value[0].GetNumber(), 5, "value[0]");

    try {
        did_throw = false;
        value[2]  = 12;
    } catch (...) {
        did_throw = true;
    }

    EQ_TRUE(did_throw, "did_throw");

    try {
        did_throw   = false;
        value[str2] = 120;
    } catch (...) {
        did_throw = true;
    }

    EQ_TRUE(did_throw, "did_throw");

    try {
        did_throw           = false;
        value[VString("W")] = 120;
    } catch (...) {
        did_throw = true;
    }

    EQ_TRUE(did_throw, "did_throw");

    try {
        did_throw    = false;
        value["WYX"] = 120;
    } catch (...) {
        did_throw = true;
    }

    EQ_TRUE(did_throw, "did_throw");

    value[0] = 20;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_VALUE(value[0].GetNumber(), 20, "value[0]");

    value[1] = 30;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 2, "Size()");
    EQ_VALUE(value[0].GetNumber(), 20, "value[0]");
    EQ_VALUE(value[1].GetNumber(), 30, "value[1]");

    value[0]               = 50;
    value[1]               = 100;
    value[2]               = 200;
    value[int{3}]          = 300;
    value[vu_long_long{4}] = 400;
    value[vu_int{5}]       = 500;

    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 6, "Size()");
    EQ_VALUE(value[0].GetNumber(), 50, "value[0]");
    EQ_VALUE(value[1].GetNumber(), 100, "value[1]");
    EQ_VALUE(value[2].GetNumber(), 200, "value[2]");
    EQ_VALUE(value[3].GetNumber(), 300, "value[3]");
    EQ_VALUE(value[4].GetNumber(), 400, "value[4]");
    EQ_VALUE(value[5].GetNumber(), 500, "value[5]");
    NOT_EQ_TO(value.GetValue(0), nullptr, "GetValue(0)", "null");
    NOT_EQ_TO(value.GetValue(1), nullptr, "GetValue(1)", "null");
    NOT_EQ_TO(value.GetValue(2), nullptr, "GetValue(2)", "null");
    NOT_EQ_TO(value.GetValue(3), nullptr, "GetValue(3)", "null");
    NOT_EQ_TO(value.GetValue(4), nullptr, "GetValue(4)", "null");
    NOT_EQ_TO(value.GetValue(5), nullptr, "GetValue(5)", "null");
    NOT_EQ_TO(value.GetValue("5", 1), nullptr, "GetValue(\"5\")", "null");
    EQ_VALUE(value.GetValue(0)->GetNumber(), 50, "GetValue(0)");
    EQ_VALUE(value.GetValue(1)->GetNumber(), 100, "GetValue(1)");
    EQ_VALUE(value.GetValue(2)->GetNumber(), 200, "GetValue(2)");
    EQ_VALUE(value.GetValue(3)->GetNumber(), 300, "GetValue(3)");
    EQ_VALUE(value.GetValue(4)->GetNumber(), 400, "GetValue(4)");
    EQ_VALUE(value.GetValue(5)->GetNumber(), 500, "GetValue(5)");
    try {
        did_throw = false;
        value[7]  = "cc";
    } catch (...) {
        did_throw = true;
    }

    EQ_TRUE(did_throw, "did_throw");
    EQ_TO(value.GetValue(6), nullptr, "GetValue(6)", "null");

    //////////////////////////////////////////
    value.Reset();

    value["A"] = 7.5;
    EQ_TRUE(value.IsObject(), "IsObject()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_VALUE(value[0].GetNumber(), 7.5, "value[0]");
    EQ_VALUE(value["A"].GetNumber(), 7.5, "value[\"A\"]");

    try {
        did_throw = false;
        value[1]  = 5;
    } catch (...) {
        did_throw = true;
    }

    EQ_TRUE(did_throw, "did_throw");

    try {
        did_throw = false;
        value[10] = 5;
    } catch (...) {
        did_throw = true;
    }

    EQ_TRUE(did_throw, "did_throw");

    value["A"] = 59;
    EQ_TRUE(value.IsObject(), "IsObject()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_VALUE(value[0].GetNumber(), 59, "value[0]");
    EQ_VALUE(value["A"].GetNumber(), 59, "value[\"A\"]");

    value["B"] = 60;
    EQ_TRUE(value.IsObject(), "IsObject()");
    EQ_VALUE(value.Size(), 2, "Size()");
    EQ_VALUE(value[0].GetNumber(), 59, "value[0]");
    EQ_VALUE(value["A"].GetNumber(), 59, "value[\"A\"]");
    EQ_VALUE(value[1].GetNumber(), 60, "value[1]");
    EQ_VALUE(value["B"].GetNumber(), 60, "value[\"B\"]");

    value[0]                   = 50;
    value["B"]                 = 100;
    value[VString("C")]        = 200;
    value[str1]                = 300;
    value["EFEFE"]             = 400;
    value[VString("FGHIGKLM")] = 500;

    EQ_TRUE(value.IsObject(), "IsObject()");
    EQ_VALUE(value.Size(), 6, "Size()");
    EQ_VALUE(value[0].GetNumber(), 50, "value[0]");
    EQ_VALUE(value[1].GetNumber(), 100, "value[1]");
    EQ_VALUE(value[2].GetNumber(), 200, "value[2]");
    EQ_VALUE(value[3].GetNumber(), 300, "value[3]");
    EQ_VALUE(value[4].GetNumber(), 400, "value[4]");
    EQ_VALUE(value[5].GetNumber(), 500, "value[5]");
    EQ_VALUE(value["A"].GetNumber(), 50, "value[\"A\"]");
    EQ_VALUE(value["B"].GetNumber(), 100, "value[\"B\"]");
    EQ_VALUE(value["C"].GetNumber(), 200, "value[\"C\"]");
    EQ_VALUE(value["D"].GetNumber(), 300, "value[\"D\"]");
    EQ_VALUE(value["EFEFE"].GetNumber(), 400, "value[\"EFEFE\"]");
    EQ_VALUE(value["FGHIGKLM"].GetNumber(), 500, "value[\"FGHIGKLM\"]");
    NOT_EQ_TO(value.GetValue(0), nullptr, "GetValue(0)", "null");
    NOT_EQ_TO(value.GetValue(1), nullptr, "GetValue(1)", "null");
    NOT_EQ_TO(value.GetValue(2), nullptr, "GetValue(2)", "null");
    NOT_EQ_TO(value.GetValue(3), nullptr, "GetValue(3)", "null");
    NOT_EQ_TO(value.GetValue(4), nullptr, "GetValue(4)", "null");
    NOT_EQ_TO(value.GetValue(5), nullptr, "GetValue(5)", "null");
    EQ_VALUE(value.GetValue(0)->GetNumber(), 50, "GetValue(0)");
    EQ_VALUE(value.GetValue(1)->GetNumber(), 100, "GetValue(1)");
    EQ_VALUE(value.GetValue(2)->GetNumber(), 200, "GetValue(2)");
    EQ_VALUE(value.GetValue(3)->GetNumber(), 300, "GetValue(3)");
    EQ_VALUE(value.GetValue(4)->GetNumber(), 400, "GetValue(4)");
    EQ_VALUE(value.GetValue(5)->GetNumber(), 500, "GetValue(5)");

    NOT_EQ_TO(value.GetKey(0), nullptr, "GetKey(0)", "null");
    NOT_EQ_TO(value.GetKey(1), nullptr, "GetKey(1)", "null");
    NOT_EQ_TO(value.GetKey(2), nullptr, "GetKey(2)", "null");
    NOT_EQ_TO(value.GetKey(3), nullptr, "GetKey(3)", "null");
    NOT_EQ_TO(value.GetKey(4), nullptr, "GetKey(4)", "null");
    NOT_EQ_TO(value.GetKey(5), nullptr, "GetKey(5)", "null");

    EQ_TRUE(value.GetKey(0)->IsEqual("A", 1), "GetKey(0)->IsEqual()");
    EQ_TRUE(value.GetKey(1)->IsEqual("B", 1), "GetKey(1)->IsEqual()");
    EQ_TRUE(value.GetKey(2)->IsEqual("C", 1), "GetKey(2)->IsEqual()");
    EQ_TRUE(value.GetKey(3)->IsEqual("D", 1), "GetKey(3)->IsEqual()");
    EQ_TRUE(value.GetKey(4)->IsEqual("EFEFE", 5), "GetKey(4)->IsEqual()");
    EQ_TRUE(value.GetKey(5)->IsEqual("FGHIGKLM", 8), "GetKey(5)->IsEqual()");

    try {
        did_throw = false;
        value[6]  = "W";
    } catch (...) {
        did_throw = true;
    }

    EQ_TRUE(did_throw, "did_throw");
    EQ_TO(value.GetValue(6), nullptr, "GetValue(6)", "null");

    value.Reset();

    value[VString("C")] = 4;
    EQ_TRUE(value.IsObject(), "IsObject()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_VALUE(value[0].GetNumber(), 4, "value[0]");
    EQ_VALUE(value[VString("C")].GetNumber(), 4, "value[\"C\"]");

    value.Reset();

    value[str2] = 5;
    EQ_TRUE(value.IsObject(), "IsObject()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_VALUE(value[0].GetNumber(), 5, "value[0]");
    EQ_VALUE(value[str2].GetNumber(), 5, "value[\"C\"]");

    //////////////////////////////////////////

    END_SUB_TEST;
}

static int TestIndexOperator2() {
    ValueC  value;
    ValueC *value_ptr;

    value["A"]    = 10;
    value["BB"]   = 20;
    value["CCCC"] = 30;

    value_ptr = value.GetValue("A", 1);
    NOT_EQ_TO(value_ptr, nullptr, "GetValue(\"A\", 1)", "null");
    EQ_VALUE(value_ptr->GetNumber(), 10, "GetValue(\"A\", 1)");

    value_ptr = value.GetValue("ABCDEF", 1);
    NOT_EQ_TO(value_ptr, nullptr, "GetValue(\"ABCDEF\", 1)", "null");
    EQ_VALUE(value_ptr->GetNumber(), 10, "GetValue(\"ABCDEF\", 1)");

    value_ptr = value.GetValue("BB", 2);
    NOT_EQ_TO(value_ptr, nullptr, "GetValue(\"BB\", 2)", "null");
    EQ_VALUE(value_ptr->GetNumber(), 20, "GetValue(\"BB\", 2)");

    value_ptr = value.GetValue("CCCC", 4);
    NOT_EQ_TO(value_ptr, nullptr, "GetValue(\"CCCC\", 4)", "null");
    EQ_VALUE(value_ptr->GetNumber(), 30, "GetValue(\"CCCC\", 4)");
    ////////////////////
    EQ_TO(value.GetValue("a", 1), nullptr, "GetValue(\"a\", 1)", "null");
    EQ_TO(value.GetValue("ABCDEF", 6), nullptr, "GetValue(\"ABCDEF\", 1)",
          "null");
    EQ_TO(value.GetValue("b", 1), nullptr, "GetValue(\"b\", 1)", "null");
    EQ_TO(value.GetValue("bb", 2), nullptr, "GetValue(\"bb\", 2)", "null");

    EQ_TO(value.GetValue("CCC", 3), nullptr, "GetValue(\"CCCC\", 3)", "null");
    ////////////////////

    END_SUB_TEST;
}

static int TestAddition1() {
    using vu_int       = unsigned int;
    using vu_long_long = unsigned long long;

    ValueC  value;
    VString str1("D");
    VString str2("DEFG");
    bool    did_throw;

    try {
        did_throw = false;
        value[10] = 5;
    } catch (...) {
        did_throw = true;
    }

    EQ_TRUE(did_throw, "did_throw");

    value += 20;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_VALUE(value[0].GetNumber(), 20, "value[0]");

    try {
        did_throw = false;
        value[2]  = 12;
    } catch (...) {
        did_throw = true;
    }

    value += 30;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 2, "Size()");
    EQ_VALUE(value[0].GetNumber(), 20, "value[0]");
    EQ_VALUE(value[1].GetNumber(), 30, "value[1]");

    value[0] = 50;
    value[1] = 100;
    value += int{200};
    value += vu_int{300};
    value += vu_long_long{400};
    value += double{500};

    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 6, "Size()");
    EQ_VALUE(value[0].GetNumber(), 50, "value[0]");
    EQ_VALUE(value[1].GetNumber(), 100, "value[1]");
    EQ_VALUE(value[2].GetNumber(), 200, "value[2]");
    EQ_VALUE(value[3].GetNumber(), 300, "value[3]");
    EQ_VALUE(value[4].GetNumber(), 400, "value[4]");
    EQ_VALUE(value[5].GetNumber(), 500, "value[5]");

    try {
        did_throw = false;
        value[7]  = "cc";
    } catch (...) {
        did_throw = true;
    }

    EQ_TRUE(did_throw, "did_throw");

    //////////////////////////////////////////

    END_SUB_TEST;
}

static int TestAddition2() {
    ValueC  value;
    VArray  arr_var;
    VString str;

    /////////////////

    value += true;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_TRUE(value[0].IsTrue(), "value[0].IsTrue()");

    value += true;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 2, "Size()");
    EQ_TRUE(value[0].IsTrue(), "value[0].IsTrue()");
    EQ_TRUE(value[1].IsTrue(), "value[1].IsTrue()");

    value.Reset();
    /////////////////

    value += false;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_TRUE(value[0].IsFalse(), "value[0].IsFalse()");

    value += false;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 2, "Size()");
    EQ_TRUE(value[0].IsFalse(), "value[0].IsFalse()");
    EQ_TRUE(value[1].IsFalse(), "value[1].IsFalse()");

    value.Reset();
    /////////////////

    value += nullptr;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_TRUE(value[0].IsNull(), "value[0].IsNull()");

    value += nullptr;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 2, "Size()");
    EQ_TRUE(value[0].IsNull(), "value[0].IsNull()");
    EQ_TRUE(value[1].IsNull(), "value[1].IsNull()");

    value.Reset();
    /////////////////

    value += "-ABCDEF0123456789ABCDEF0123456789-";
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_TRUE(value[0].IsString(), "value[0].IsString()");
    EQ_TRUE(StringUtils::IsEqual(value[0].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");

    value += "^ABCDEF0123456789ABCDEF0123456789^";
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 2, "Size()");
    EQ_TRUE(value[0].IsString(), "value[0].IsString()");
    EQ_TRUE(value[1].IsString(), "value[1].IsString()");
    EQ_TRUE(StringUtils::IsEqual(value[0].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");
    EQ_TRUE(StringUtils::IsEqual(value[1].StringStorage(),
                                 "^ABCDEF0123456789ABCDEF0123456789^", 34),
            "StringUtils::IsEqual");
    value.Reset();
    /////////////////
    str               = VString("-ABCDEF0123456789ABCDEF0123456789-");
    const char *c_str = str.First();
    value += static_cast<VString &&>(str);
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_TRUE(value[0].IsString(), "value[0].IsString()");
    EQ_TO(value[0].StringStorage(), c_str, "value[0].StringStorage()", "c_str");
    EQ_TRUE(StringUtils::IsEqual(value[0].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");

    str                = VString("#0123456789ABCDEF0123456789ABCDEF#");
    const char *c_str2 = str.First();
    value += static_cast<VString &&>(str);
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 2, "Size()");
    EQ_TRUE(value[0].IsString(), "value[0].IsString()");
    EQ_TRUE(value[1].IsString(), "value[1].IsString()");
    EQ_TO(value[0].StringStorage(), c_str, "value[0].StringStorage()", "c_str");
    EQ_TO(value[1].StringStorage(), c_str2, "value[1].StringStorage()",
          "c_str2");
    EQ_TRUE(StringUtils::IsEqual(value[0].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");
    EQ_TRUE(StringUtils::IsEqual(value[1].StringStorage(),
                                 "#0123456789ABCDEF0123456789ABCDEF#", 34),
            "StringUtils::IsEqual");
    value.Reset();
    /////////////////

    str   = VString("-ABCDEF0123456789ABCDEF0123456789-");
    c_str = str.First();
    value += str;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_TRUE(value[0].IsString(), "value[0].IsString()");
    NOT_EQ_TO(value[0].StringStorage(), c_str, "value[0].StringStorage()",
              "c_str");
    EQ_TRUE(StringUtils::IsEqual(value[0].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");

    str    = VString("#0123456789ABCDEF0123456789ABCDEF#");
    c_str2 = str.First();
    value += str;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 2, "Size()");
    EQ_TRUE(value[0].IsString(), "value[0].IsString()");
    EQ_TRUE(value[1].IsString(), "value[1].IsString()");
    NOT_EQ_TO(value[0].StringStorage(), c_str, "value[0].StringStorage()",
              "c_str");
    NOT_EQ_TO(value[1].StringStorage(), c_str2, "value[1].StringStorage()",
              "c_str2");
    EQ_TRUE(StringUtils::IsEqual(value[0].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");
    EQ_TRUE(StringUtils::IsEqual(value[1].StringStorage(),
                                 "#0123456789ABCDEF0123456789ABCDEF#", 34),
            "StringUtils::IsEqual");
    value.Reset();

    //////////////

    value += VHArray();
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    EQ_TRUE(value[0].IsObject(), "value[0].IsObject()");

    value += VHArray();
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 2, "Size()");
    EQ_TRUE(value[0].IsObject(), "value[0].IsObject()");
    EQ_TRUE(value[1].IsObject(), "value[1].IsObject()");

    //////////////////////////////////////////

    END_SUB_TEST;
}

static int TestAddition3() {
    ValueC        value;
    VArray        arr_var;
    const ValueC *arr_storage;
    VString       str;
    const char *  c_str;
    const char *  c_str2;
    /////////////////
    arr_var = VArray(1);
    value += arr_var;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    NOT_EQ_TO(value.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");

    value.Reset();

    arr_var = VArray(3);
    value += arr_var;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    NOT_EQ_TO(value.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");

    value.Reset();
    ///

    arr_var += ValueC{false};
    arr_var += ValueC{true};
    arr_var += ValueC{VString("-ABCDEF0123456789ABCDEF0123456789-")};
    c_str       = arr_var[2].StringStorage();
    arr_storage = arr_var.First();

    value += arr_var;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 3, "Size()");
    NOT_EQ_TO(value.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    EQ_TRUE(value[0].IsFalse(), "value[0].IsFalse()");
    EQ_TRUE(value[1].IsTrue(), "value[1].IsTrue()");
    EQ_TRUE(value[2].IsString(), "value[2].IsString()");
    NOT_EQ_TO(value[2].StringStorage(), c_str, "value[2].StringStorage()",
              "c_str");
    EQ_TRUE(StringUtils::IsEqual(value[2].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");

    arr_var.Reset();
    arr_var += ValueC{nullptr};
    arr_var += ValueC{14};
    arr_var += ValueC{VString("^ABCDEF0123456789ABCDEF0123456789^")};
    c_str2      = arr_var[2].StringStorage();
    arr_storage = arr_var.First();

    value += arr_var;
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 6, "Size()");
    NOT_EQ_TO(value.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    EQ_TRUE(value[0].IsFalse(), "value[0].IsFalse()");
    EQ_TRUE(value[1].IsTrue(), "value[1].IsTrue()");
    EQ_TRUE(value[2].IsString(), "value[2].IsString()");
    NOT_EQ_TO(value[2].StringStorage(), c_str, "value[2].StringStorage()",
              "c_str");
    EQ_TRUE(StringUtils::IsEqual(value[2].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");
    EQ_TRUE(value[3].IsNull(), "value[3].IsNull()");
    EQ_TRUE(value[4].IsNumber(), "value1[4].IsNumber()");
    EQ_VALUE(value[4].GetNumber(), 14, "value[4].GetNumber()");
    EQ_TRUE(value[5].IsString(), "value[5].IsString()");
    NOT_EQ_TO(value[5].StringStorage(), c_str2, "value[5].StringStorage()",
              "c_str2");
    EQ_TRUE(StringUtils::IsEqual(value[5].StringStorage(),
                                 "^ABCDEF0123456789ABCDEF0123456789^", 34),
            "StringUtils::IsEqual");

    value.Reset();
    /////////////////
    value += VArray(1);
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    NOT_EQ_TO(value.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");

    value.Reset();

    value += VArray(3);
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 1, "Size()");
    NOT_EQ_TO(value.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");

    value.Reset();
    ///

    arr_var.Reset();
    arr_var += ValueC{ValueType::False};
    arr_var += ValueC{ValueType::True};
    arr_var += ValueC{VString("#0123456789ABCDEF0123456789ABCDEF#")};
    c_str       = arr_var[2].StringStorage();
    arr_storage = arr_var.First();

    value += static_cast<VArray &&>(arr_var);
    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 3, "Size()");
    NOT_EQ_TO(value.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    EQ_TO(value.GetArray()->First(), arr_storage, "GetArray()->First()",
          "storage");
    EQ_TRUE(value[0].IsFalse(), "value[0].IsFalse()");
    EQ_TRUE(value[1].IsTrue(), "value[1].IsTrue()");
    EQ_TRUE(value[2].IsString(), "value[2].IsString()");
    EQ_TO(value[2].StringStorage(), c_str, "value[2].StringStorage()", "c_str");
    EQ_TRUE(StringUtils::IsEqual(value[2].StringStorage(),
                                 "#0123456789ABCDEF0123456789ABCDEF#", 34),
            "StringUtils::IsEqual");

    arr_var += ValueC{nullptr};
    arr_var += ValueC{14};
    arr_var += ValueC{VString("-ABCDEF0123456789ABCDEF0123456789-")};
    c_str2      = arr_var[2].StringStorage();
    arr_storage = arr_var.First();

    value += static_cast<VArray &&>(arr_var);
    EQ_TO(arr_var.First(), nullptr, "arr_var.First()", "null");

    EQ_TRUE(value.IsArray(), "IsArray()");
    EQ_VALUE(value.Size(), 6, "Size()");
    NOT_EQ_TO(value.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    EQ_TRUE(value[0].IsFalse(), "value[0].IsFalse()");
    EQ_TRUE(value[1].IsTrue(), "value[1].IsTrue()");
    EQ_TRUE(value[2].IsString(), "value[2].IsString()");
    EQ_TO(value[2].StringStorage(), c_str, "value[2].StringStorage()", "c_str");
    EQ_TRUE(StringUtils::IsEqual(value[2].StringStorage(),
                                 "#0123456789ABCDEF0123456789ABCDEF#", 34),
            "StringUtils::IsEqual");
    EQ_TRUE(value[3].IsNull(), "value[3].IsNull()");
    EQ_TRUE(value[4].IsNumber(), "value1[4].IsNumber()");
    EQ_VALUE(value[4].GetNumber(), 14, "value[4].GetNumber()");
    EQ_TRUE(value[5].IsString(), "value[5].IsString()");
    EQ_TO(value[5].StringStorage(), c_str2, "value[5].StringStorage()",
          "c_str2");
    EQ_TRUE(StringUtils::IsEqual(value[5].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");
    //////////////////////////////////////////

    END_SUB_TEST;
}

static int TestAddition4() {
    ValueC        value1;
    ValueC        value2;
    VArray        arr_var;
    const ValueC *arr_storage;
    VString       str;

    /////////////////

    value2 = true;
    value1 += value2;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 1, "Size()");
    EQ_TRUE(value1[0].IsTrue(), "value1[0].IsTrue()");

    value2 = true;
    value1 += value2;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 2, "Size()");
    EQ_TRUE(value1[0].IsTrue(), "value1[0].IsTrue()");
    EQ_TRUE(value1[1].IsTrue(), "value1[1].IsTrue()");

    value1.Reset();
    /////////////////

    value2 = false;
    value1 += value2;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 1, "Size()");
    EQ_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");

    value2 = false;
    value1 += value2;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 2, "Size()");
    EQ_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");
    EQ_TRUE(value1[1].IsFalse(), "value1[1].IsFalse()");

    value1.Reset();
    /////////////////

    value2 = nullptr;
    value1 += value2;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 1, "Size()");
    EQ_TRUE(value1[0].IsNull(), "value1[0].IsNull()");

    value2 = nullptr;
    value1 += value2;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 2, "Size()");
    EQ_TRUE(value1[0].IsNull(), "value1[0].IsNull()");
    EQ_TRUE(value1[1].IsNull(), "value1[1].IsNull()");

    value1.Reset();
    /////////////////
    str               = VString("-ABCDEF0123456789ABCDEF0123456789-");
    const char *c_str = str.First();
    value2            = static_cast<VString &&>(str);

    value1 += value2;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 1, "Size()");
    EQ_TRUE(value1[0].IsString(), "value1[0].IsString()");
    NOT_EQ_TO(value1[0].StringStorage(), c_str, "value1[0].StringStorage()",
              "c_str");
    EQ_TRUE(StringUtils::IsEqual(value1[0].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");

    str                = VString("#0123456789ABCDEF0123456789ABCDEF#");
    const char *c_str2 = str.First();
    value2             = static_cast<VString &&>(str);

    value1 += value2;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 2, "Size()");
    EQ_TRUE(value1[0].IsString(), "value1[0].IsString()");
    EQ_TRUE(value1[1].IsString(), "value1[1].IsString()");
    NOT_EQ_TO(value1[0].StringStorage(), c_str, "value1[0].StringStorage()",
              "c_str");
    NOT_EQ_TO(value1[1].StringStorage(), c_str2, "value1[1].StringStorage()",
              "c_str2");
    EQ_TRUE(StringUtils::IsEqual(value1[0].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");
    EQ_TRUE(StringUtils::IsEqual(value1[1].StringStorage(),
                                 "#0123456789ABCDEF0123456789ABCDEF#", 34),
            "StringUtils::IsEqual");
    value1.Reset();
    /////////////////

    value2 = VArray(1);
    value1 += value2;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 1, "Size()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");

    value1.Reset();

    value2 = VArray(10);
    value1 += value2;
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 1, "Size()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");

    value1.Reset();

    value2 = VArray(1);
    value1.Merge(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 0, "Size()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");

    value2 = VArray(10);
    value1.Merge(static_cast<ValueC &&>(value2));
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 0, "Size()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");

    value1.Reset();
    ///

    arr_var += ValueC{ValueType::False};
    arr_var += ValueC{ValueType::True};
    arr_var += ValueC{VString("-ABCDEF0123456789ABCDEF0123456789-")};
    c_str = arr_var[2].StringStorage();

    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);

    value1.Merge(static_cast<ValueC &&>(value2));
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 3, "Size()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    EQ_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");
    EQ_TRUE(value1[1].IsTrue(), "value1[1].IsTrue()");
    EQ_TRUE(value1[2].IsString(), "value1[2].IsString()");
    EQ_TO(value1[2].StringStorage(), c_str, "value1[2].StringStorage()",
          "c_str");
    EQ_TRUE(StringUtils::IsEqual(value1[2].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");

    arr_var.Reset();
    arr_var += ValueC{ValueType::Null};
    arr_var += ValueC{14};
    arr_var += ValueC{VString("^ABCDEF0123456789ABCDEF0123456789^")};
    c_str2      = arr_var[2].StringStorage();
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);

    value1.Merge(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 6, "Size()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    EQ_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");
    EQ_TRUE(value1[1].IsTrue(), "value1[1].IsTrue()");
    EQ_TRUE(value1[2].IsString(), "value1[2].IsString()");
    EQ_TO(value1[2].StringStorage(), c_str, "value1[0].StringStorage()",
          "c_str");
    EQ_TRUE(StringUtils::IsEqual(value1[2].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");
    EQ_TRUE(value1[3].IsNull(), "value1[3].IsNull()");
    EQ_TRUE(value1[4].IsNumber(), "value1[4].IsNumber()");
    EQ_VALUE(value1[4].GetNumber(), 14, "value1[4].GetNumber()");
    EQ_TRUE(value1[5].IsString(), "value1[5].IsString()");
    NOT_EQ_TO(value1[5].StringStorage(), c_str2, "value1[5].StringStorage()",
              "c_str2");
    EQ_TRUE(StringUtils::IsEqual(value1[5].StringStorage(),
                                 "^ABCDEF0123456789ABCDEF0123456789^", 34),
            "StringUtils::IsEqual");

    value1.Reset();

    //////////////////////////////////////////

    END_SUB_TEST;
}

static int TestAddition5() {
    ValueC        value1;
    ValueC        value2;
    VArray        arr_var;
    const ValueC *arr_storage;
    VString       str;

    /////////////////

    value2 = true;
    value2 += true;
    value2 += false;
    value2 += nullptr;
    value2 += "v";
    value2 += 4;
    value2 += VArray(1);
    value2 += VHArray(1);

    EQ_TRUE(value2.IsTrue(), "value2.IsTrue()");

    value1 += static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 1, "Size()");
    EQ_TRUE(value1[0].IsTrue(), "value1[0].IsTrue()");

    value2 = true;
    value1 += static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 2, "Size()");
    EQ_TRUE(value1[0].IsTrue(), "value1[0].IsTrue()");
    EQ_TRUE(value1[1].IsTrue(), "value1[1].IsTrue()");

    value1.Reset();
    /////////////////

    value2 = false;

    value1 += static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 1, "Size()");
    EQ_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");

    value2 = false;
    value1 += static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 2, "Size()");
    EQ_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");
    EQ_TRUE(value1[1].IsFalse(), "value1[1].IsFalse()");

    value1.Reset();
    /////////////////

    value2 = nullptr;
    value1 += static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 1, "Size()");
    EQ_TRUE(value1[0].IsNull(), "value1[0].IsNull()");

    value2 = nullptr;
    value1 += static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 2, "Size()");
    EQ_TRUE(value1[0].IsNull(), "value1[0].IsNull()");
    EQ_TRUE(value1[1].IsNull(), "value1[1].IsNull()");

    value1.Reset();
    /////////////////
    str               = VString("-ABCDEF0123456789ABCDEF0123456789-");
    const char *c_str = str.First();
    value2            = static_cast<VString &&>(str);

    value1 += static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 1, "Size()");
    EQ_TRUE(value1[0].IsString(), "value1[0].IsString()");
    EQ_TO(value1[0].StringStorage(), c_str, "value1[0].StringStorage()",
          "c_str");
    EQ_TRUE(StringUtils::IsEqual(value1[0].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");

    str                = VString("#0123456789ABCDEF0123456789ABCDEF#");
    const char *c_str2 = str.First();
    value2             = static_cast<VString &&>(str);

    value1 += static_cast<ValueC &&>(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 2, "Size()");
    EQ_TRUE(value1[0].IsString(), "value1[0].IsString()");
    EQ_TRUE(value1[1].IsString(), "value1[1].IsString()");
    EQ_TO(value1[0].StringStorage(), c_str, "value1[0].StringStorage()",
          "c_str");
    EQ_TO(value1[1].StringStorage(), c_str2, "value1[1].StringStorage()",
          "c_str2");
    EQ_TRUE(StringUtils::IsEqual(value1[0].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");
    EQ_TRUE(StringUtils::IsEqual(value1[1].StringStorage(),
                                 "#0123456789ABCDEF0123456789ABCDEF#", 34),
            "StringUtils::IsEqual");
    value1.Reset();
    /////////////////

    value2 = VArray(1);
    value1.Merge(value2);
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 0, "Size()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value2.GetArray(), nullptr, "GetArray()", "null");

    value2 = VArray(1);
    value1.Merge(static_cast<ValueC &&>(value2));
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 0, "Size()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    EQ_TO(value2.GetArray(), nullptr, "GetArray()", "null");

    value1.Reset();
    ///

    arr_var += ValueC{ValueType::False};
    arr_var += ValueC{ValueType::True};
    arr_var += ValueC{VString("-ABCDEF0123456789ABCDEF0123456789-")};
    c_str       = arr_var[2].StringStorage();
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);

    value1.Merge(static_cast<ValueC &&>(value2));
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 3, "Size()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    EQ_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");
    EQ_TRUE(value1[1].IsTrue(), "value1[1].IsTrue()");
    EQ_TRUE(value1[2].IsString(), "value1[2].IsString()");
    EQ_TO(value1[2].StringStorage(), c_str, "value1[0].StringStorage()",
          "c_str");
    EQ_TRUE(StringUtils::IsEqual(value1[2].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");

    arr_var.Reset();
    arr_var += ValueC{ValueType::Null};
    arr_var += ValueC{14};
    arr_var += ValueC{VString("#0123456789ABCDEF0123456789ABCDEF#")};
    c_str2      = arr_var[2].StringStorage();
    arr_storage = arr_var.First();
    value2      = static_cast<VArray &&>(arr_var);

    value1.Merge(static_cast<ValueC &&>(value2));
    EQ_TRUE(value2.IsUndefined(), "value2.IsUndefined()");
    EQ_TRUE(value1.IsArray(), "IsArray()");
    EQ_VALUE(value1.Size(), 6, "Size()");
    NOT_EQ_TO(value1.GetArray(), nullptr, "GetArray()", "null");
    NOT_EQ_TO(value1.GetArray()->First(), nullptr, "GetArray()->First()",
              "null");
    NOT_EQ_TO(value1.GetArray()->First(), arr_storage, "GetArray()->First()",
              "storage");
    EQ_TRUE(value1[0].IsFalse(), "value1[0].IsFalse()");
    EQ_TRUE(value1[1].IsTrue(), "value1[1].IsTrue()");
    EQ_TRUE(value1[2].IsString(), "value1[2].IsString()");
    EQ_TO(value1[2].StringStorage(), c_str, "value1[0].StringStorage()",
          "c_str");
    EQ_TRUE(StringUtils::IsEqual(value1[2].StringStorage(),
                                 "-ABCDEF0123456789ABCDEF0123456789-", 34),
            "StringUtils::IsEqual");
    EQ_TRUE(value1[3].IsNull(), "value1[3].IsNull()");
    EQ_TRUE(value1[4].IsNumber(), "value1[4].IsNumber()");
    EQ_VALUE(value1[4].GetNumber(), 14, "value1[3].GetNumber()");
    EQ_TRUE(value1[5].IsString(), "value1[0].IsString()");
    EQ_TO(value1[5].StringStorage(), c_str2, "value1[1].StringStorage()",
          "c_str2");
    EQ_TRUE(StringUtils::IsEqual(value1[5].StringStorage(),
                                 "#0123456789ABCDEF0123456789ABCDEF#", 34),
            "StringUtils::IsEqual");
    value1.Reset();
    //////////////////////////////////////////

    END_SUB_TEST;
}

static int TestAddition6() {
    ValueC                      value1;
    ValueC                      value2;
    VString                     str_var;
    const char *                str_c1;
    const char *                str_c2;
    VHArray                     h_arr_var;
    const HAItem<ValueC, char> *h_arr_storage1 = nullptr;
    const HAItem<ValueC, char> *h_arr_storage2 = nullptr;

    value1["k1"] = 11;
    value1["k2"] = 22;
    str_var      = "*ABCDEF0123456789ABCDEF0123456789*";
    str_c1       = str_var.First();
    value1["k3"] = static_cast<VString &&>(str_var);

    if (value1.GetObject() != nullptr) {
        h_arr_storage1 = value1.GetObject()->First();
    }

    h_arr_var[VString("w1")] = 10;
    h_arr_var[VString("w2")] = 20;
    h_arr_var[VString("w3")] = 30;
    str_var                  = "-ABCDEF0123456789ABCDEF0123456789-";
    str_c2                   = str_var.First();
    h_arr_var[VString("w4")] = static_cast<VString &&>(str_var);
    h_arr_storage2           = h_arr_var.First();

    value2 = VHArray();   // Setting to object type.
    value2.Merge(value1); // Copy
    EQ_VALUE(value2.Size(), 3, "value2.Size()");
    NOT_EQ_TO(value2.GetObject(), nullptr, "GetObject()", "null");
    NOT_EQ_TO(value2.GetObject()->First(), h_arr_storage1,
              "GetArray()->First()", "storage");
    EQ_VALUE(value2["k1"].GetNumber(), 11, "value2[\"k1\"].GetNumber()");
    EQ_VALUE(value2["k2"].GetNumber(), 22, "value2[\"k2\"].GetNumber()");
    NOT_EQ_TO(value2["k3"].StringStorage(), str_c1,
              "value2[\"k3\"].StringStorage()", "str_c");
    EQ_TO(StringUtils::IsEqual(value2["k3"].StringStorage(),
                               "*ABCDEF0123456789ABCDEF0123456789*", 34),
          true, "value2[\"k3\"].StringStorage()", "IsEqual");

    value2 = VHArray();  // Clearing and  Setting to object type.
    value2 += h_arr_var; // Copy
    EQ_VALUE(value2.Size(), 4, "value2.Size()");
    NOT_EQ_TO(value2.GetObject(), nullptr, "GetObject()", "null");
    NOT_EQ_TO(value2.GetObject()->First(), h_arr_storage2,
              "GetArray()->First()", "storage");
    EQ_VALUE(value2["w1"].GetNumber(), 10, "[\"w1\"].GetNumber()");
    EQ_VALUE(value2["w2"].GetNumber(), 20, "[\"w2\"].GetNumber()");
    EQ_VALUE(value2["w3"].GetNumber(), 30, "[\"w3\"].GetNumber()");
    NOT_EQ_TO(value2["w4"].StringStorage(), str_c2,
              "value2[\"w4\"].StringStorage()", "str_c");
    EQ_TO(StringUtils::IsEqual(value2["w4"].StringStorage(),
                               "-ABCDEF0123456789ABCDEF0123456789-", 34),
          true, "value2[\"w4\"].StringStorage()", "IsEqual");
    EQ_TO(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null");

    ////

    value2 = VHArray(); // Clearing and  Setting to object type.
    value2.Merge(static_cast<ValueC &&>(value1)); // Move
    EQ_TRUE(value1.IsUndefined(), "value1.IsUndefined()");
    EQ_VALUE(value2.Size(), 3, "value2.Size()");
    NOT_EQ_TO(value2.GetObject(), nullptr, "GetObject()", "null");
    NOT_EQ_TO(value2.GetObject()->First(), h_arr_storage2,
              "GetArray()->First()", "storage");
    EQ_VALUE(value2["k1"].GetNumber(), 11, "value2[\"k1\"].GetNumber()");
    EQ_VALUE(value2["k2"].GetNumber(), 22, "value2[\"k2\"].GetNumber()");
    EQ_TO(value2["k3"].StringStorage(), str_c1,
          "value2[\"k3\"].StringStorage()", "str_c1");
    EQ_TO(StringUtils::IsEqual(value2["k3"].StringStorage(),
                               "*ABCDEF0123456789ABCDEF0123456789*", 34),
          true, "value2[\"k3\"].StringStorage()", "IsEqual");

    value1 = value2; // Copying back the values.
    str_c1 = value1["k3"].StringStorage();

    value2 = VHArray(); // Clearing and  Setting to object type.
    value2 += static_cast<VHArray &&>(h_arr_var); // Move
    EQ_TO(h_arr_var.First(), nullptr, "h_arr_var.First()", "null");
    EQ_VALUE(value2.Size(), 4, "value2.Size()");
    NOT_EQ_TO(value2.GetObject(), nullptr, "GetObject()", "null");
    NOT_EQ_TO(value2.GetObject()->First(), h_arr_storage2,
              "GetArray()->First()", "storage");
    EQ_VALUE(value2["w1"].GetNumber(), 10, "[\"w1\"].GetNumber()");
    EQ_VALUE(value2["w2"].GetNumber(), 20, "[\"w2\"].GetNumber()");
    EQ_VALUE(value2["w3"].GetNumber(), 30, "[\"w3\"].GetNumber()");
    EQ_TO(value2["w4"].StringStorage(), str_c2,
          "value2[\"w4\"].StringStorage()", "str_c2");
    EQ_TO(StringUtils::IsEqual(value2["w4"].StringStorage(),
                               "-ABCDEF0123456789ABCDEF0123456789-", 34),
          true, "value2[\"w4\"].StringStorage()", "IsEqual");
    EQ_TO(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null");

    if (value2.GetObject() != nullptr) {
        h_arr_var = *(value2.GetObject()); // Copying back the values.
        str_c2    = h_arr_var[VString("w4")].StringStorage();
    }

    ////////
    value2 = VHArray();   // Clearing and  Setting to object type.
    value2.Merge(value1); // Copy
    value2 += h_arr_var;  // Copy
    EQ_VALUE(value2.Size(), 7, "value2.Size()");
    EQ_VALUE(value2["w1"].GetNumber(), 10, "[\"w1\"].GetNumber()");
    EQ_VALUE(value2["w2"].GetNumber(), 20, "[\"w2\"].GetNumber()");
    EQ_VALUE(value2["w3"].GetNumber(), 30, "[\"w3\"].GetNumber()");
    NOT_EQ_TO(value2["w4"].StringStorage(), str_c2,
              "value2[\"w4\"].StringStorage()", "str_c2");
    EQ_TO(StringUtils::IsEqual(value2["w4"].StringStorage(),
                               "-ABCDEF0123456789ABCDEF0123456789-", 34),
          true, "value2[\"w4\"].StringStorage()", "IsEqual");
    EQ_VALUE(value2["k1"].GetNumber(), 11, "value2[\"k1\"].GetNumber()");
    EQ_VALUE(value2["k2"].GetNumber(), 22, "value2[\"k2\"].GetNumber()");
    NOT_EQ_TO(value2["k3"].StringStorage(), str_c1,
              "value2[\"k3\"].StringStorage()", "str_c1");
    EQ_TO(StringUtils::IsEqual(value2["k3"].StringStorage(),
                               "*ABCDEF0123456789ABCDEF0123456789*", 34),
          true, "value2[\"k3\"].StringStorage()", "IsEqual");

    ////
    value2       = VHArray(); // Clearing and  Setting to object type.
    value2["w0"] = 5;
    value2["w1"] = 100;
    value2["w2"] = 200;
    value2["w4"] = 400;
    value2["w5"] = 500;
    value2["w6"] = 600;
    value2 += h_arr_var;  // Copy
    value2.Merge(value1); // Copy

    EQ_VALUE(value2["w0"].GetNumber(), 5, "[\"w0\"].GetNumber()");
    EQ_VALUE(value2["w1"].GetNumber(), 10, "[\"w1\"].GetNumber()");
    EQ_VALUE(value2["w2"].GetNumber(), 20, "[\"w2\"].GetNumber()");
    EQ_VALUE(value2["w3"].GetNumber(), 30, "[\"w3\"].GetNumber()");
    NOT_EQ_TO(value2["w4"].StringStorage(), str_c2,
              "value2[\"w4\"].StringStorage()", "str_c2");
    EQ_TO(StringUtils::IsEqual(value2["w4"].StringStorage(),
                               "-ABCDEF0123456789ABCDEF0123456789-", 34),
          true, "value2[\"w4\"].StringStorage()", "IsEqual");
    EQ_VALUE(value2["w5"].GetNumber(), 500, "[\"w5\"].GetNumber()");
    EQ_VALUE(value2["w6"].GetNumber(), 600, "[\"w6\"].GetNumber()");
    EQ_VALUE(value2["k1"].GetNumber(), 11, "value2[\"k1\"].GetNumber()");
    EQ_VALUE(value2["k2"].GetNumber(), 22, "value2[\"k2\"].GetNumber()");
    NOT_EQ_TO(value2["k3"].StringStorage(), str_c1,
              "value2[\"k3\"].StringStorage()", "str_c1");
    EQ_TO(StringUtils::IsEqual(value2["k3"].StringStorage(),
                               "*ABCDEF0123456789ABCDEF0123456789*", 34),
          true, "value2[\"k3\"].StringStorage()", "IsEqual");

    value2       = VHArray(); // Clearing and  Setting to object type.
    value2["w0"] = 5;
    value2["w1"] = 100;
    value2["w2"] = 200;
    value2["w4"] = 400;
    value2["w5"] = 500;
    value2["w6"] = 600;
    value2.Merge(static_cast<ValueC &&>(value1)); // Move
    value2 += static_cast<VHArray &&>(h_arr_var); // Move
    EQ_VALUE(value2["w0"].GetNumber(), 5, "[\"w0\"].GetNumber()");
    EQ_VALUE(value2["w1"].GetNumber(), 10, "[\"w1\"].GetNumber()");
    EQ_VALUE(value2["w2"].GetNumber(), 20, "[\"w2\"].GetNumber()");
    EQ_VALUE(value2["w3"].GetNumber(), 30, "[\"w3\"].GetNumber()");
    EQ_TO(value2["w4"].StringStorage(), str_c2,
          "value2[\"w4\"].StringStorage()", "str_c2");
    EQ_TO(StringUtils::IsEqual(value2["w4"].StringStorage(),
                               "-ABCDEF0123456789ABCDEF0123456789-", 34),
          true, "value2[\"w4\"].StringStorage()", "IsEqual");
    EQ_VALUE(value2["w5"].GetNumber(), 500, "[\"w5\"].GetNumber()");
    EQ_VALUE(value2["w6"].GetNumber(), 600, "[\"w6\"].GetNumber()");
    EQ_VALUE(value2["k1"].GetNumber(), 11, "value2[\"k1\"].GetNumber()");
    EQ_VALUE(value2["k2"].GetNumber(), 22, "value2[\"k2\"].GetNumber()");
    EQ_TO(value2["k3"].StringStorage(), str_c1,
          "value2[\"k3\"].StringStorage()", "str_c1");
    EQ_TO(StringUtils::IsEqual(value2["k3"].StringStorage(),
                               "*ABCDEF0123456789ABCDEF0123456789*", 34),
          true, "value2[\"k3\"].StringStorage()", "IsEqual");
    /////////////////////////////

    END_SUB_TEST;
}

static int TestStringify1() {
    ValueC value;

    ///////////
    value = VArray();
    EQ_VALUE(value.Stringify(), "[]", "Stringify()");

    value.Reset();
    value += true;
    EQ_VALUE(value.Stringify(), "[true]", "Stringify()");

    value.Reset();
    value += false;
    EQ_VALUE(value.Stringify(), "[false]", "Stringify()");

    value.Reset();
    value += nullptr;
    EQ_VALUE(value.Stringify(), "[null]", "Stringify()");

    value.Reset();
    value += 0;
    EQ_VALUE(value.Stringify(), "[0]", "Stringify()");

    value.Reset();
    value += "a";
    EQ_VALUE(value.Stringify(), R"(["a"])", "Stringify()");

    value.Reset();
    value += "ABC";
    EQ_VALUE(value.Stringify(), R"(["ABC"])", "Stringify()");

    value.Reset();
    value += VArray();
    EQ_VALUE(value.Stringify(), "[[]]", "Stringify()");

    value.Reset();
    value += VHArray();
    EQ_VALUE(value.Stringify(), "[{}]", "Stringify()");
    ///////////

    value.Reset();
    value += true;
    value += true;
    EQ_VALUE(value.Stringify(), "[true,true]", "Stringify()");

    value.Reset();
    value += false;
    value += true;
    EQ_VALUE(value.Stringify(), "[false,true]", "Stringify()");

    value.Reset();
    value += nullptr;
    value += true;
    EQ_VALUE(value.Stringify(), "[null,true]", "Stringify()");

    value.Reset();
    value += -3;
    value += true;
    EQ_VALUE(value.Stringify(), "[-3,true]", "Stringify()");

    value.Reset();
    value += "a";
    value += true;
    EQ_VALUE(value.Stringify(), R"(["a",true])", "Stringify()");

    value.Reset();
    value += "ABC";
    value += true;
    EQ_VALUE(value.Stringify(), R"(["ABC",true])", "Stringify()");

    value.Reset();
    value += VArray();
    value += true;
    EQ_VALUE(value.Stringify(), "[[],true]", "Stringify()");

    value.Reset();
    value += VHArray();
    value += true;
    EQ_VALUE(value.Stringify(), "[{},true]", "Stringify()");
    ///////////

    value.Reset();
    value += true;
    value += false;
    EQ_VALUE(value.Stringify(), "[true,false]", "Stringify()");

    value.Reset();
    value += false;
    value += false;
    EQ_VALUE(value.Stringify(), "[false,false]", "Stringify()");

    value.Reset();
    value += nullptr;
    value += false;
    EQ_VALUE(value.Stringify(), "[null,false]", "Stringify()");

    value.Reset();
    value += 500;
    value += false;
    EQ_VALUE(value.Stringify(), "[500,false]", "Stringify()");

    value.Reset();
    value += VString("a");
    value += false;
    EQ_VALUE(value.Stringify(), R"(["a",false])", "Stringify()");

    value.Reset();
    VString str("ABC");
    value += str;
    value += false;
    EQ_VALUE(value.Stringify(), R"(["ABC",false])", "Stringify()");

    value.Reset();
    value += VArray();
    value += false;
    EQ_VALUE(value.Stringify(), "[[],false]", "Stringify()");

    value.Reset();
    value += VHArray();
    value += false;
    EQ_VALUE(value.Stringify(), "[{},false]", "Stringify()");
    ///////////

    value.Reset();
    value += true;
    value += nullptr;
    EQ_VALUE(value.Stringify(), "[true,null]", "Stringify()");

    value.Reset();
    value += false;
    value += nullptr;
    EQ_VALUE(value.Stringify(), "[false,null]", "Stringify()");

    value.Reset();
    value += nullptr;
    value += nullptr;
    EQ_VALUE(value.Stringify(), "[null,null]", "Stringify()");

    value.Reset();
    value += 456.5;
    value += nullptr;
    EQ_VALUE(value.Stringify(), "[456.5,null]", "Stringify()");

    value.Reset();
    value += "a";
    value += nullptr;
    EQ_VALUE(value.Stringify(), R"(["a",null])", "Stringify()");

    value.Reset();
    value += "ABC";
    value += nullptr;
    EQ_VALUE(value.Stringify(), R"(["ABC",null])", "Stringify()");

    value.Reset();
    value += VArray();
    value += nullptr;
    EQ_VALUE(value.Stringify(), "[[],null]", "Stringify()");

    value.Reset();
    value += VHArray();
    value += nullptr;
    EQ_VALUE(value.Stringify(), "[{},null]", "Stringify()");
    ///////////

    value.Reset();
    value += true;
    value += "A";
    EQ_VALUE(value.Stringify(), R"([true,"A"])", "Stringify()");

    value.Reset();
    value += false;
    value += "AB";
    EQ_VALUE(value.Stringify(), R"([false,"AB"])", "Stringify()");

    value.Reset();
    value += nullptr;
    value += "ABC";
    EQ_VALUE(value.Stringify(), R"([null,"ABC"])", "Stringify()");

    value.Reset();
    value += -8.9;
    value += "ABC";
    EQ_VALUE(value.Stringify(), R"([-8.9,"ABC"])", "Stringify()");

    value.Reset();
    value += "a";
    value += "ABCD";
    EQ_VALUE(value.Stringify(), R"(["a","ABCD"])", "Stringify()");

    value.Reset();
    value += "ABC";
    value += "ABCDE";
    EQ_VALUE(value.Stringify(), R"(["ABC","ABCDE"])", "Stringify()");

    value.Reset();
    value += VArray();
    value += "ABCDEF";
    EQ_VALUE(value.Stringify(), R"([[],"ABCDEF"])", "Stringify()");

    value.Reset();
    value += VHArray();
    value += "ABCDEFG";
    EQ_VALUE(value.Stringify(), R"([{},"ABCDEFG"])", "Stringify()");
    ///////////

    value.Reset();
    value += true;
    value += VArray();
    EQ_VALUE(value.Stringify(), "[true,[]]", "Stringify()");

    value.Reset();
    value += false;
    value += VArray();
    EQ_VALUE(value.Stringify(), "[false,[]]", "Stringify()");

    value.Reset();
    value += nullptr;
    value += VArray();
    EQ_VALUE(value.Stringify(), "[null,[]]", "Stringify()");

    value.Reset();
    value += 10000;
    value += VArray();
    EQ_VALUE(value.Stringify(), "[10000,[]]", "Stringify()");

    value.Reset();
    value += "ABC";
    value += VArray();
    EQ_VALUE(value.Stringify(), R"(["ABC",[]])", "Stringify()");

    value.Reset();
    value += VArray();
    value += VArray();
    EQ_VALUE(value.Stringify(), "[[],[]]", "Stringify()");

    value.Reset();
    value += VHArray();
    value += VArray();
    EQ_VALUE(value.Stringify(), "[{},[]]", "Stringify()");
    ///////////

    value.Reset();
    value += true;
    value += VHArray();
    EQ_VALUE(value.Stringify(), "[true,{}]", "Stringify()");

    value.Reset();
    value += false;
    value += VHArray();
    EQ_VALUE(value.Stringify(), "[false,{}]", "Stringify()");

    value.Reset();
    value += nullptr;
    value += VHArray();
    EQ_VALUE(value.Stringify(), "[null,{}]", "Stringify()");

    value.Reset();
    value += -1000;
    value += VHArray();
    EQ_VALUE(value.Stringify(), "[-1000,{}]", "Stringify()");

    value.Reset();
    value += "ABC";
    value += VHArray();
    EQ_VALUE(value.Stringify(), R"(["ABC",{}])", "Stringify()");

    value.Reset();
    value += VArray();
    value += VHArray();
    EQ_VALUE(value.Stringify(), "[[],{}]", "Stringify()");

    value.Reset();
    value += VHArray();
    value += VHArray();
    EQ_VALUE(value.Stringify(), "[{},{}]", "Stringify()");
    ///////////

    value.Reset();
    value += true;
    value += VHArray();
    value += false;
    EQ_VALUE(value.Stringify(), "[true,{},false]", "Stringify()");

    value.Reset();
    value += false;
    value += VArray();
    value += nullptr;
    EQ_VALUE(value.Stringify(), "[false,[],null]", "Stringify()");

    value.Reset();
    value += nullptr;
    value += VHArray();
    value += "ABC";
    EQ_VALUE(value.Stringify(), R"([null,{},"ABC"])", "Stringify()");

    value.Reset();
    value += nullptr;
    value += 789;
    value += "ABC";
    EQ_VALUE(value.Stringify(), R"([null,789,"ABC"])", "Stringify()");

    value.Reset();
    value += "ABC";
    value += VArray();
    value += VHArray();
    EQ_VALUE(value.Stringify(), R"(["ABC",[],{}])", "Stringify()");

    value.Reset();
    value += VArray();
    value += VHArray();
    value[2] = 498;
    EQ_VALUE(value.Stringify(), "[[],{},498]", "Stringify()");

    value.Reset();
    value += VHArray();
    value += VArray();
    value += true;
    EQ_VALUE(value.Stringify(), "[{},[],true]", "Stringify()");
    ///////////

    value.Reset();
    value += true;
    value += VHArray();
    value += 0;
    value += VArray();
    EQ_VALUE(value.Stringify(), "[true,{},0,[]]", "Stringify()");

    value.Reset();
    value += false;
    value += VArray();
    value += nullptr;
    value += VHArray();
    EQ_VALUE(value.Stringify(), "[false,[],null,{}]", "Stringify()");

    value.Reset();
    value += nullptr;
    value += VHArray();
    value += "ABC";
    value += VArray();
    EQ_VALUE(value.Stringify(), R"([null,{},"ABC",[]])", "Stringify()");

    value.Reset();
    value += "ABC";
    value += VArray();
    value += VHArray();
    value += nullptr;
    EQ_VALUE(value.Stringify(), R"(["ABC",[],{},null])", "Stringify()");

    value.Reset();
    value += VArray();
    value += false;
    value += VHArray();
    value += true;
    EQ_VALUE(value.Stringify(), "[[],false,{},true]", "Stringify()");

    value.Reset();
    value += VHArray();
    value += nullptr;
    value += VArray();
    value += VArray();
    EQ_VALUE(value.Stringify(), "[{},null,[],[]]", "Stringify()");

    ///////////
    value.Reset();
    value += true;
    value += false;
    value += nullptr;
    value += 123;
    value += "ABC";
    value += VArray();
    value += VHArray();
    EQ_VALUE(value.Stringify(), R"([true,false,null,123,"ABC",[],{}])",
             "Stringify()");

    ///////////
    value.Reset();
    value += VHArray();
    value += VArray();
    value += "a";
    value += 1.5;
    value += nullptr;
    value += false;
    value += true;
    EQ_VALUE(value.Stringify(), R"([{},[],"a",1.5,null,false,true])",
             "Stringify()");
    ///////////////////////////////////////
    END_SUB_TEST;
}

static int TestStringify2() {
    ValueC value;

    ///////////
    value = VHArray();
    EQ_VALUE(value.Stringify(), R"({})", "Stringify()");

    value.Reset();
    value["A"] = true;
    EQ_VALUE(value.Stringify(), R"({"A":true})", "Stringify()");

    value.Reset();
    value["B"] = false;
    EQ_VALUE(value.Stringify(), R"({"B":false})", "Stringify()");

    value.Reset();
    value["AA"] = nullptr;
    EQ_VALUE(value.Stringify(), R"({"AA":null})", "Stringify()");

    value.Reset();
    value["a"] = 0;
    EQ_VALUE(value.Stringify(), R"({"a":0})", "Stringify()");

    value.Reset();
    value["abc"] = "a";
    EQ_VALUE(value.Stringify(), R"({"abc":"a"})", "Stringify()");

    value.Reset();
    value["ab"] = "ABC";
    EQ_VALUE(value.Stringify(), R"({"ab":"ABC"})", "Stringify()");

    value.Reset();
    value["ABC"] = VArray();
    EQ_VALUE(value.Stringify(), R"({"ABC":[]})", "Stringify()");

    value.Reset();
    value["5vn7b83y98t3wrupwmwa4ataw"] = VHArray();
    EQ_VALUE(value.Stringify(), R"({"5vn7b83y98t3wrupwmwa4ataw":{}})",
             "Stringify()");
    ///////////

    value.Reset();
    value["A"] = true;
    value["B"] = true;
    EQ_VALUE(value.Stringify(), R"({"A":true,"B":true})", "Stringify()");

    value.Reset();
    value["A"] = false;
    value["b"] = true;
    EQ_VALUE(value.Stringify(), R"({"A":false,"b":true})", "Stringify()");

    value.Reset();
    value["A"]  = nullptr;
    value["BC"] = true;
    EQ_VALUE(value.Stringify(), R"({"A":null,"BC":true})", "Stringify()");

    value.Reset();
    value["A"]  = -3;
    value["AB"] = true;
    EQ_VALUE(value.Stringify(), R"({"A":-3,"AB":true})", "Stringify()");

    value.Reset();
    value["A"]   = "a";
    value["ABC"] = true;
    EQ_VALUE(value.Stringify(), R"({"A":"a","ABC":true})", "Stringify()");

    value.Reset();
    value["A"] = "ABC";
    value["1"] = true;
    EQ_VALUE(value.Stringify(), R"({"A":"ABC","1":true})", "Stringify()");

    value.Reset();
    value["X"]   = VArray();
    value["123"] = true;
    EQ_VALUE(value.Stringify(), R"({"X":[],"123":true})", "Stringify()");

    value.Reset();
    value["x"] = VHArray();
    value["A"] = true;
    EQ_VALUE(value.Stringify(), R"({"x":{},"A":true})", "Stringify()");
    ///////////

    value.Reset();
    value["A2"] = true;
    value["A1"] = false;
    EQ_VALUE(value.Stringify(), R"({"A2":true,"A1":false})", "Stringify()");

    value.Reset();
    value["A-"]   = false;
    value["A123"] = false;
    EQ_VALUE(value.Stringify(), R"({"A-":false,"A123":false})", "Stringify()");

    value.Reset();
    value["A"] = nullptr;
    value["B"] = false;
    EQ_VALUE(value.Stringify(), R"({"A":null,"B":false})", "Stringify()");

    value.Reset();
    value["A"] = 500;
    value["B"] = false;
    EQ_VALUE(value.Stringify(), R"({"A":500,"B":false})", "Stringify()");

    value.Reset();
    value["A"] = VString("a");
    value["B"] = false;
    EQ_VALUE(value.Stringify(), R"({"A":"a","B":false})", "Stringify()");

    value.Reset();
    VString str("ABC");
    value["A"] = str;
    value["B"] = false;
    EQ_VALUE(value.Stringify(), R"({"A":"ABC","B":false})", "Stringify()");

    value.Reset();
    value["X"] = VArray();
    value["A"] = false;
    EQ_VALUE(value.Stringify(), R"({"X":[],"A":false})", "Stringify()");

    value.Reset();
    value["X"] = VHArray();
    value["A"] = false;
    EQ_VALUE(value.Stringify(), R"({"X":{},"A":false})", "Stringify()");
    ///////////

    value.Reset();
    value["A"] = true;
    value["W"] = nullptr;
    EQ_VALUE(value.Stringify(), R"({"A":true,"W":null})", "Stringify()");

    value.Reset();
    value["A"] = false;
    value["@"] = nullptr;
    EQ_VALUE(value.Stringify(), R"({"A":false,"@":null})", "Stringify()");

    value.Reset();
    value["A"] = nullptr;
    value["#"] = nullptr;
    EQ_VALUE(value.Stringify(), R"({"A":null,"#":null})", "Stringify()");

    value.Reset();
    value["A"] = 456.5;
    value["H"] = nullptr;
    EQ_VALUE(value.Stringify(), R"({"A":456.5,"H":null})", "Stringify()");

    value.Reset();
    value["A"] = "a";
    value["Q"] = nullptr;
    EQ_VALUE(value.Stringify(), R"({"A":"a","Q":null})", "Stringify()");

    value.Reset();
    value["A"] = "ABC";
    value["e"] = nullptr;
    EQ_VALUE(value.Stringify(), R"({"A":"ABC","e":null})", "Stringify()");

    value.Reset();
    value["X"] = VArray();
    value["n"] = nullptr;
    EQ_VALUE(value.Stringify(), R"({"X":[],"n":null})", "Stringify()");

    value.Reset();
    value["t"] = VHArray();
    value["A"] = nullptr;
    EQ_VALUE(value.Stringify(), R"({"t":{},"A":null})", "Stringify()");
    ///////////

    value.Reset();
    value["e"] = true;
    value["A"] = "A";
    EQ_VALUE(value.Stringify(), R"({"e":true,"A":"A"})", "Stringify()");

    value.Reset();
    value["m"] = false;
    value["A"] = "AB";
    EQ_VALUE(value.Stringify(), R"({"m":false,"A":"AB"})", "Stringify()");

    value.Reset();
    value["i"] = nullptr;
    value["A"] = "ABC";
    EQ_VALUE(value.Stringify(), R"({"i":null,"A":"ABC"})", "Stringify()");

    value.Reset();
    value["s"] = -8.9;
    value["A"] = "ABC";
    EQ_VALUE(value.Stringify(), R"({"s":-8.9,"A":"ABC"})", "Stringify()");

    value.Reset();
    value["-"] = "a";
    value["A"] = "ABCD";
    EQ_VALUE(value.Stringify(), R"({"-":"a","A":"ABCD"})", "Stringify()");

    value.Reset();
    value["g"] = "ABC";
    value["A"] = "ABCDE";
    EQ_VALUE(value.Stringify(), R"({"g":"ABC","A":"ABCDE"})", "Stringify()");

    value.Reset();
    value["o"] = VArray();
    value["A"] = "ABCDEF";
    EQ_VALUE(value.Stringify(), R"({"o":[],"A":"ABCDEF"})", "Stringify()");

    value.Reset();
    value["A"] = VHArray();
    value["o"] = "ABCDEFG";
    EQ_VALUE(value.Stringify(), R"({"A":{},"o":"ABCDEFG"})", "Stringify()");
    ///////////

    value.Reset();
    value["d"] = true;
    value["y"] = VArray();
    EQ_VALUE(value.Stringify(), R"({"d":true,"y":[]})", "Stringify()");

    value.Reset();
    value["A"] = false;
    value["y"] = VArray();
    EQ_VALUE(value.Stringify(), R"({"A":false,"y":[]})", "Stringify()");

    value.Reset();
    value["A"] = nullptr;
    value["y"] = VArray();
    EQ_VALUE(value.Stringify(), R"({"A":null,"y":[]})", "Stringify()");

    value.Reset();
    value["A"] = 10000;
    value["y"] = VArray();
    EQ_VALUE(value.Stringify(), R"({"A":10000,"y":[]})", "Stringify()");

    value.Reset();
    value["A"] = "ABC";
    value["y"] = VArray();
    EQ_VALUE(value.Stringify(), R"({"A":"ABC","y":[]})", "Stringify()");

    value.Reset();
    value["X"] = VArray();
    value["y"] = VArray();
    EQ_VALUE(value.Stringify(), R"({"X":[],"y":[]})", "Stringify()");

    value.Reset();
    value["X"] = VHArray();
    value["Y"] = VArray();
    EQ_VALUE(value.Stringify(), R"({"X":{},"Y":[]})", "Stringify()");
    ///////////

    value.Reset();
    value["A"] = true;
    value["y"] = VHArray();
    EQ_VALUE(value.Stringify(), R"({"A":true,"y":{}})", "Stringify()");

    value.Reset();
    value["A"] = false;
    value["y"] = VHArray();
    EQ_VALUE(value.Stringify(), R"({"A":false,"y":{}})", "Stringify()");

    value.Reset();
    value["A"] = nullptr;
    value["y"] = VHArray();
    EQ_VALUE(value.Stringify(), R"({"A":null,"y":{}})", "Stringify()");

    value.Reset();
    value["A"] = -1000;
    value["y"] = VHArray();
    EQ_VALUE(value.Stringify(), R"({"A":-1000,"y":{}})", "Stringify()");

    value.Reset();
    value["A"] = "ABC";
    value["y"] = VHArray();
    EQ_VALUE(value.Stringify(), R"({"A":"ABC","y":{}})", "Stringify()");

    value.Reset();
    value["C"] = VArray();
    value["R"] = VHArray();
    EQ_VALUE(value.Stringify(), R"({"C":[],"R":{}})", "Stringify()");

    value.Reset();
    value["cc"] = VHArray();
    value["rr"] = VHArray();
    EQ_VALUE(value.Stringify(), R"({"cc":{},"rr":{}})", "Stringify()");
    ///////////

    value.Reset();
    value["A"]  = true;
    value["y"]  = VHArray();
    value["AA"] = false;
    EQ_VALUE(value.Stringify(), R"({"A":true,"y":{},"AA":false})",
             "Stringify()");

    value.Reset();
    value["A"] = false;
    value["y"] = VArray();
    value["B"] = nullptr;
    EQ_VALUE(value.Stringify(), R"({"A":false,"y":[],"B":null})",
             "Stringify()");

    value.Reset();
    value["A"]   = nullptr;
    value["y"]   = VHArray();
    value["ABC"] = "ABC";
    EQ_VALUE(value.Stringify(), R"({"A":null,"y":{},"ABC":"ABC"})",
             "Stringify()");

    value.Reset();
    value["t"] = nullptr;
    value["Y"] = 789;
    value["A"] = "ABC";
    EQ_VALUE(value.Stringify(), R"({"t":null,"Y":789,"A":"ABC"})",
             "Stringify()");

    value.Reset();
    value["A"]     = "ABC";
    value["y"]     = VArray();
    value["key-u"] = VHArray();
    EQ_VALUE(value.Stringify(), R"({"A":"ABC","y":[],"key-u":{}})",
             "Stringify()");

    value.Reset();
    value["X"]     = VArray();
    value["Y"]     = VHArray();
    value["key-u"] = 498;
    EQ_VALUE(value.Stringify(), R"({"X":[],"Y":{},"key-u":498})",
             "Stringify()");

    value.Reset();
    value["X"] = VHArray();
    value["y"] = VArray();
    value["A"] = true;
    EQ_VALUE(value.Stringify(), R"({"X":{},"y":[],"A":true})", "Stringify()");
    ///////////

    value.Reset();
    value["{}}"]   = true;
    value["y"]     = VHArray();
    value["AA"]    = 0;
    value["k-300"] = VArray();
    EQ_VALUE(value.Stringify(), R"({"{}}":true,"y":{},"AA":0,"k-300":[]})",
             "Stringify()");

    value.Reset();
    value["B1"]    = false;
    value["y"]     = VArray();
    value["[A]"]   = nullptr;
    value["k-300"] = VHArray();
    EQ_VALUE(value.Stringify(), R"({"B1":false,"y":[],"[A]":null,"k-300":{}})",
             "Stringify()");

    value.Reset();
    value["{A}"]   = nullptr;
    value["y"]     = VHArray();
    value["AA"]    = "ABC";
    value["k-300"] = VArray();
    EQ_VALUE(value.Stringify(), R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})",
             "Stringify()");

    value.Reset();
    value["x"]     = "ABC";
    value["[]]"]   = VArray();
    value["key-u"] = VHArray();
    value["A"]     = nullptr;
    EQ_VALUE(value.Stringify(), R"({"x":"ABC","[]]":[],"key-u":{},"A":null})",
             "Stringify()");

    value.Reset();
    value["X"]       = VArray();
    value["CCCCccc"] = false;
    value["key-u"]   = VHArray();
    value["A"]       = true;
    EQ_VALUE(value.Stringify(),
             R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})", "Stringify()");

    value.Reset();
    value["X"]     = VHArray();
    value["A"]     = nullptr;
    value["key-u"] = VArray();
    value["k-300"] = VArray();
    EQ_VALUE(value.Stringify(), R"({"X":{},"A":null,"key-u":[],"k-300":[]})",
             "Stringify()");

    ///////////
    value.Reset();
    value["A"] = true;
    value["B"] = false;
    value["C"] = nullptr;
    value["D"] = 123;
    value["E"] = "ABC";
    value["F"] = VArray();
    value["G"] = VHArray();
    EQ_VALUE(value.Stringify(),
             R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})",
             "Stringify()");

    ///////////
    value.Reset();
    value["A"]       = VHArray();
    value["BB"]      = VArray();
    value["CCC"]     = "a";
    value["DDDD"]    = 1.5;
    value["EEEEE"]   = nullptr;
    value["FFFFFF"]  = false;
    value["GGGGGGG"] = true;
    EQ_VALUE(
        value.Stringify(),
        R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})",
        "Stringify()");
    ///////////////////////////////////////

    END_SUB_TEST;
}

static int TestStringify3() {
    ValueC value;

    value[0] += true;
    value[0] += false;
    value[0] += nullptr;
    value[0] += 0;
    value[0] += "ABC";
    value[0] += VArray();
    value[0] += VHArray();
    EQ_VALUE(value.Stringify(), R"([[true,false,null,0,"ABC",[],{}]])",
             "Stringify()");

    value         = VArray();
    value[0]["a"] = true;
    value[0]["0"] = false;
    value[0]["1"] = nullptr;
    value[0]["V"] = 0;
    value[0]["B"] = "a";
    value[0]["2"] = VArray();
    value[0]["6"] = VHArray();
    EQ_VALUE(value.Stringify(),
             R"([{"a":true,"0":false,"1":null,"V":0,"B":"a","2":[],"6":{}}])",
             "Stringify()");

    value.Reset();
    value["o"] += true;
    value["o"] += false;
    value["o"] += nullptr;
    value["o"] += 0;
    value["o"] += "ABC";
    value["o"] += VArray();
    value["o"] += VHArray();
    EQ_VALUE(value.Stringify(), R"({"o":[true,false,null,0,"ABC",[],{}]})",
             "Stringify()");

    value.Reset();
    value["i"]["a"] = true;
    value["i"]["0"] = false;
    value["i"]["1"] = nullptr;
    value["i"]["V"] = 0;
    value["i"]["B"] = "a";
    value["i"]["2"] = VArray();
    value["i"]["6"] = VHArray();
    EQ_VALUE(
        value.Stringify(),
        R"({"i":{"a":true,"0":false,"1":null,"V":0,"B":"a","2":[],"6":{}}})",
        "Stringify()");

    ////
    END_SUB_TEST;
}

static int TestStringify4() {
    ValueC value;

    value["\"\\/\b\f\n\r\t"] = "\t\r\n\f\b/\\\"";
    EQ_VALUE(value.Stringify(), R"({"\"\\\/\b\f\n\r\t":"\t\r\n\f\b\/\\\""})",
             "value.Stringify()");

    value.Reset();
    value += "\"\\/\b\f\n\r\t";
    EQ_VALUE(value.Stringify(), R"(["\"\\\/\b\f\n\r\t"])", "value.Stringify()");

    END_SUB_TEST;
}

static int TestDeleteValue() {
    using vu_int       = unsigned int;
    using vu_long_long = unsigned long long;

    ValueC value;

    value[0] = 1;
    value.RemoveIndex(int{0});
    EQ_TO(value.GetValue(0), nullptr, "GetValue(0)", "null");
    EQ_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = "c";
    value.RemoveIndex(vu_int{0});
    EQ_TO(value.GetValue(0), nullptr, "GetValue(0)", "null");
    EQ_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = VArray();
    value.RemoveIndex(vu_long_long{0});
    EQ_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = false;
    value[1] = true;
    value.RemoveIndex(0);
    EQ_VALUE(value.Stringify(), R"([true])", "value.Stringify()");
    value.RemoveIndex(1);
    EQ_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = "abc";
    value[1] = nullptr;
    value.RemoveIndex(1);
    EQ_VALUE(value.Stringify(), R"(["abc"])", "value.Stringify()");
    value.RemoveIndex(0);
    EQ_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = false;
    value[1] = true;
    value[2] = nullptr;
    value.RemoveIndex(1);
    EQ_VALUE(value.Stringify(), R"([false,null])", "value.Stringify()");
    value.RemoveIndex(0);
    EQ_VALUE(value.Stringify(), R"([null])", "value.Stringify()");

    value.RemoveIndex(2);
    EQ_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = false;
    value[1] = true;
    value[2] = nullptr;
    value[1].Reset();
    EQ_VALUE(value.Stringify(), R"([false,null])", "value.Stringify()");
    value[0].Reset();
    EQ_VALUE(value.Stringify(), R"([null])", "value.Stringify()");

    value[2].Reset();
    EQ_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value[0] = "a";
    value[1] = VArray();
    value[2] = VHArray();
    value.RemoveIndex(2);
    EQ_VALUE(value.Stringify(), R"(["a",[]])", "value.Stringify()");
    value.RemoveIndex(1);
    EQ_VALUE(value.Stringify(), R"(["a"])", "value.Stringify()");

    value.RemoveIndex(0);
    EQ_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    /////////
    value.Reset();

    value["A"] = 1;
    value.RemoveIndex(0);
    EQ_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value["A"] = "c";
    value.Remove("A");
    EQ_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value["A"] = VArray();
    value.Remove("A");
    EQ_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value.Reset();
    value["A"]  = false;
    value["bb"] = true;
    value.Remove("A");
    EQ_TO(value.GetKey(0), nullptr, "GetKey(0)", "null");
    EQ_TO(value.GetValue(0), nullptr, "GetValue(0)", "null");
    EQ_VALUE(value.Stringify(), R"({"bb":true})", "value.Stringify()");
    value.RemoveIndex(1);
    EQ_TO(value.GetKey(0), nullptr, "GetKey(0)", "null");
    EQ_TO(value.GetValue(1), nullptr, "GetValue(1)", "null");
    EQ_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value["A"]  = "abc";
    value["bb"] = nullptr;
    value.Remove("bb");
    EQ_VALUE(value.Stringify(), R"({"A":"abc"})", "value.Stringify()");
    value.Remove("A");
    EQ_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value["A"]   = false;
    value["bb"]  = true;
    value["AAA"] = nullptr;
    value.Remove("bb");
    EQ_VALUE(value.Stringify(), R"({"A":false,"AAA":null})",
             "value.Stringify()");
    value.Remove("A");
    EQ_VALUE(value.Stringify(), R"({"AAA":null})", "value.Stringify()");

    value.Remove("AAA");
    EQ_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value["A"]   = "a";
    value["bb"]  = VHArray();
    value["AAA"] = VArray();
    value.Remove("AAA");
    EQ_VALUE(value.Stringify(), R"({"A":"a","bb":{}})", "value.Stringify()");
    value.Remove("bb");
    EQ_VALUE(value.Stringify(), R"({"A":"a"})", value.Stringify().First());

    value.Remove("A");
    EQ_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    value["A"]   = "a";
    value["bb"]  = VHArray();
    value["AAA"] = VArray();
    value["AAA"].Reset();
    EQ_VALUE(value.Stringify(), R"({"A":"a","bb":{}})", "value.Stringify()");
    value["bb"].Reset();
    EQ_VALUE(value.Stringify(), R"({"A":"a"})", "value.Stringify()");

    value["A"].Reset();
    EQ_VALUE(value.Stringify(), R"({})", "value.Stringify()");

    END_SUB_TEST;
}

static int TestSortValue() {
    ValueC value;

    value["2019"] = 0;
    value["2016"] = 0;
    value["2017"] = 0;
    value["2020"] = 0;
    value["2015"] = 0;
    value["2021"] = 0;
    value["2018"] = 0;

    value.Sort();

    EQ_VALUE(
        value.Stringify(),
        R"({"2015":0,"2016":0,"2017":0,"2018":0,"2019":0,"2020":0,"2021":0})",
        "value.Stringify()");

    value.Reset();

    value["2019"] = 0;
    value["2016"] = 0;
    value["2017"] = 0;
    value["2020"] = 0;
    value["2015"] = 0;
    value["2021"] = 0;
    value["2018"] = 0;

    value.Sort(false);

    EQ_VALUE(
        value.Stringify(),
        R"({"2021":0,"2020":0,"2019":0,"2018":0,"2017":0,"2016":0,"2015":0})",
        "value.Stringify()");

    value.Reset();

    value["2019"] = 0;
    value["2016"] = 0;
    value["2017"] = 0;
    value["2020"] = 0;
    value["2015"] = 0;
    value["2021"] = 0;

    value.Sort();

    EQ_VALUE(value.Stringify(),
             R"({"2015":0,"2016":0,"2017":0,"2019":0,"2020":0,"2021":0})",
             "value.Stringify()");

    value.Reset();

    value["2019"] = 0;
    value["2016"] = 0;
    value["2017"] = 0;
    value["2020"] = 0;
    value["2015"] = 0;
    value["2021"] = 0;

    value.Sort(false);

    EQ_VALUE(value.Stringify(),
             R"({"2021":0,"2020":0,"2019":0,"2017":0,"2016":0,"2015":0})",
             "value.Stringify()");

    END_SUB_TEST;
}

static int TestGroupValue() {
    ValueC value;

    value += VHArray();
    value += VHArray();
    value += VHArray();
    value += VHArray();
    value += VHArray();
    value += VHArray();
    value += VHArray();

    value[0]["year"] = 2019;
    value[1]["year"] = 2020;
    value[2]["year"] = 2017;
    value[3]["year"] = 2020;
    value[4]["year"] = 2018;
    value[5]["year"] = 2020;
    value[6]["year"] = 2018;

    value[0]["month"] = 4;
    value[1]["month"] = 5;
    value[2]["month"] = 1;
    value[3]["month"] = 6;
    value[4]["month"] = 2;
    value[5]["month"] = 7;
    value[6]["month"] = 3;

    ValueC value2;
    value.GroupBy(value2, "year");
    value2.Sort();

    EQ_VALUE(
        value2.Stringify(),
        R"({"2017":[{"month":1}],"2018":[{"month":2},{"month":3}],"2019":[{"month":4}],"2020":[{"month":5},{"month":6},{"month":7}]})",
        "value2.Stringify()");

    value2.Sort(false);

    EQ_VALUE(
        value2.Stringify(),
        R"({"2020":[{"month":5},{"month":6},{"month":7}],"2019":[{"month":4}],"2018":[{"month":2},{"month":3}],"2017":[{"month":1}]})",
        "value2.Stringify()");

    value.Reset();
    value2.Reset();
    value.GroupBy(value2, "year");
    EQ_VALUE(value2.Stringify(), R"()", "value2.Stringify()");

    value[0]["year1"] = 2019;
    value[1]["year1"] = 2020;
    value[2]["year1"] = 2017;
    value[3]["year1"] = 2020;
    value[4]["year1"] = 2018;
    value[5]["year1"] = 2020;
    value[6]["year1"] = 2018;

    value[0]["month"] = 4;
    value[1]["month"] = 5;
    value[2]["month"] = 1;
    value[3]["month"] = 6;
    value[4]["month"] = 2;
    value[5]["month"] = 7;
    value[6]["month"] = 3;

    value.GroupBy(value2, "year");
    EQ_VALUE(value2.Stringify(), R"({})", "value2.Stringify()");

    ////
    value.Reset();

    value[0]["year"] = 2019;
    value[1]["year"] = 2020;
    value[2]["year"];

    value[0]["month"] = 4;
    value[1]["month"] = 5;
    value[2]["month"] = 1;

    EQ_FALSE(value.GroupBy(value2, "year"), "value2.Stringify()");

    value[2].Reset();
    value.GroupBy(value2, "year");
    EQ_FALSE(value.GroupBy(value2, "year"), "value2.Stringify()");

    value[2]["year"] = VHArray();
    EQ_FALSE(value.GroupBy(value2, "year"), "value2.Stringify()");

    ///////////////////

    value.Reset();
    value2.Reset();

    value2 += unsigned(4);
    value2 += unsigned(1);
    value2 += unsigned(3);
    value2 += unsigned(5);
    value2 += unsigned(2);
    value2 += unsigned(7);
    value2 += unsigned(6);

    value = value2;
    value.Sort();

    EQ_VALUE(value.Stringify(), R"([1,2,3,4,5,6,7])", "value.Stringify()");

    value = value2;

    value.Sort(false);

    EQ_VALUE(value.Stringify(), R"([7,6,5,4,3,2,1])", "value.Stringify()");

    //////////////////////

    value.Reset();
    value2.Reset();

    value2 += 5.4;
    value2 += "str";
    value2 += VArray();
    value2 += nullptr;
    value2 += true;
    value2 += VHArray();
    value2 += false;

    value = value2;
    value.Sort();

    EQ_VALUE(value.Stringify(), R"([{},[],"str",5.4,true,false,null])",
             "value.Stringify()");

    value = value2;
    value.Sort(false);

    EQ_VALUE(value.Stringify(), R"([null,false,true,5.4,"str",[],{}])",
             "value.Stringify()");

    ///////////////////

    value.Reset();
    value2.Reset();

    value2 += -4;
    value2 += -1;
    value2 += -3;
    value2 += -5;
    value2 += -2;
    value2 += -7;
    value2 += -6;

    value = value2;
    value.Sort();

    EQ_VALUE(value.Stringify(), R"([-7,-6,-5,-4,-3,-2,-1])",
             "value.Stringify()");

    value = value2;

    value.Sort(false);

    EQ_VALUE(value.Stringify(), R"([-1,-2,-3,-4,-5,-6,-7])",
             "value.Stringify()");

    ///////////////////

    value.Reset();
    value2.Reset();

    value2 += 4.5;
    value2 += 1.5;
    value2 += 3.5;
    value2 += 5.5;
    value2 += 2.5;
    value2 += 7.5;
    value2 += 6.5;

    value = value2;
    value.Sort();

    EQ_VALUE(value.Stringify(), R"([1.5,2.5,3.5,4.5,5.5,6.5,7.5])",
             "value.Stringify()");

    value = value2;

    value.Sort(false);

    EQ_VALUE(value.Stringify(), R"([7.5,6.5,5.5,4.5,3.5,2.5,1.5])",
             "value.Stringify()");

    ///////////////////

    value.Reset();
    value2.Reset();

    value2 += "b";
    value2 += "a";
    value2 += "g";
    value2 += "f";
    value2 += "c";
    value2 += "e";
    value2 += "d";

    value = value2;
    value.Sort();

    EQ_VALUE(value.Stringify(), R"(["a","b","c","d","e","f","g"])",
             "value.Stringify()");

    value = value2;

    value.Sort(false);

    EQ_VALUE(value.Stringify(), R"(["g","f","e","d","c","b","a"])",
             "value.Stringify()");

    ///////////////////

    value.Reset();
    value2.Reset();

    value2 += 0;
    value2 += 0;
    value2 += 0;
    value[0] = value2;

    value2 += 0;
    value2 += 0;
    value[1] = value2;

    value.Sort();

    EQ_VALUE(value.Stringify(), R"([[0,0,0],[0,0,0,0,0]])",
             "value.Stringify()");

    value.Reset();
    value2.Reset();

    value2 += 0;
    value2 += 0;
    value2 += 0;
    value[0] = value2;

    value2 += 0;
    value2 += 0;
    value[1] = value2;

    value.Sort(false);

    EQ_VALUE(value.Stringify(), R"([[0,0,0,0,0],[0,0,0]])",
             "value.Stringify()");

    ///////////////////

    value.Reset();
    value2.Reset();

    value2["a"] = 0;
    value2["b"] = 0;
    value2["c"] = 0;
    value[0]    = value2;

    value2["d"] = 0;
    value2["e"] = 0;
    value[1]    = value2;

    value.Sort();

    EQ_VALUE(value.Stringify(),
             R"([{"a":0,"b":0,"c":0},{"a":0,"b":0,"c":0,"d":0,"e":0}])",
             "value.Stringify()");

    value.Reset();
    value2.Reset();

    value2["a"] = 0;
    value2["b"] = 0;
    value2["c"] = 0;
    value[0]    = value2;

    value2["d"] = 0;
    value2["e"] = 0;
    value[1]    = value2;

    value.Sort(false);

    EQ_VALUE(value.Stringify(),
             R"([{"a":0,"b":0,"c":0,"d":0,"e":0},{"a":0,"b":0,"c":0}])",
             "value.Stringify()");

    //////////////////////

    ///////////////////

    value.Reset();
    value2.Reset();

    value2 += true;
    value2 += true;

    value = value2;
    value.Sort();

    EQ_VALUE(value.Stringify(), R"([true,true])", "value.Stringify()");

    value = value2;

    value.Sort(false);

    EQ_VALUE(value.Stringify(), R"([true,true])", "value.Stringify()");

    //////////
    value.Reset();
    value2.Reset();

    value2 += false;
    value2 += false;

    value = value2;
    value.Sort();

    EQ_VALUE(value.Stringify(), R"([false,false])", "value.Stringify()");

    value = value2;

    value.Sort(false);

    EQ_VALUE(value.Stringify(), R"([false,false])", "value.Stringify()");
    ///////////
    value.Reset();
    value2.Reset();

    value2 += nullptr;
    value2 += nullptr;

    value = value2;
    value.Sort();

    EQ_VALUE(value.Stringify(), R"([null,null])", "value.Stringify()");

    value = value2;

    value.Sort(false);

    EQ_VALUE(value.Stringify(), R"([null,null])", "value.Stringify()");
    ///////////
    value.RemoveIndex(0);
    value.RemoveIndex(1);

    value.Sort();

    EQ_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    value.Sort(false);

    EQ_VALUE(value.Stringify(), R"([])", "value.Stringify()");

    ///////////////////

    END_SUB_TEST;
}

static int RunValueTests() {
    STARTING_TEST("Value.hpp");

    START_TEST("Empty Value Test", TestEmptyValue);
    START_TEST("True Value Test", TestTrueValue);
    START_TEST("False Value Test", TestFalseValue);
    START_TEST("Null Value Test", TestNullValue);
    START_TEST("Number Value Test 1", TestNumberValue1);
    START_TEST("Number Value Test 2", TestNumberValue2);
    START_TEST("Number Value Test 3", TestNumberValue3);
    START_TEST("Number Value Test 4", TestNumberValue4);
    START_TEST("Number Value Test 5", TestNumberValue5);
    START_TEST("String Value Test", TestStringValue);
    START_TEST("Array Value Test", TestArrayValue);

    START_TEST("Object Value Test 1", TestObjectValue1);
    START_TEST("Object Value Test 2", TestObjectValue2);

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

    START_TEST("Sort Value Test", TestSortValue);
    START_TEST("Group Value Test", TestGroupValue);

    END_TEST("Value.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
