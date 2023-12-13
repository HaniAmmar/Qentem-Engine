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

static void TestEmptyValue(QTest &helper) {
    ValueC value1;

    VStringStream ss_var;
    QNumber64     num_var;
    const char   *c_str_var;
    SizeT         c_str_len;
    bool          bool_var;

    helper.EqualsTrue(value1.GetNumberType() == QNumberType::NotANumber, "NumberType = NotANumber", __LINE__);
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);
    helper.EqualsFalse(value1.IsObject(), "IsObject()", __LINE__);
    helper.EqualsFalse(value1.IsArray(), "IsArray()", __LINE__);
    helper.EqualsFalse(value1.IsString(), "IsString()", __LINE__);
    helper.EqualsFalse(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.EqualsFalse(value1.IsTrue(), "IsTrue()", __LINE__);
    helper.EqualsFalse(value1.IsFalse(), "IsFalse()", __LINE__);
    helper.EqualsFalse(value1.IsNull(), "IsNull()", __LINE__);
    helper.EqualsTrue(value1.Type() == ValueType::Undefined, "Undefined", __LINE__);
    helper.Equal(value1.Size(), 0U, "Size()", __LINE__);
    helper.Equal(value1.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.Equal(value1.GetValue(10), nullptr, "GetValue(10)", "null", __LINE__);
    helper.Equal(value1.GetValue("", 0), nullptr,
                 "GetValue("
                 ", 0)",
                 "null", __LINE__);
    helper.Equal(value1.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsFalse(value1.CopyKeyByIndexTo(ss_var, 0), "CopyKeyByIndexTo(0)", __LINE__);
    helper.EqualsFalse(value1.SetCharAndLength(c_str_var, c_str_len), "SetCharAndLength()", __LINE__);
    helper.Equal(value1.GetKey(10), nullptr, "GetKey(10)", "null", __LINE__);
    helper.Equal(value1.GetObject(), nullptr, "GetObject()", "null", __LINE__);
    helper.Equal(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetString(), nullptr, "GetString()", "null", __LINE__);
    helper.Equal(value1.StringStorage(), nullptr, "StringStorage()", "null", __LINE__);
    helper.Equal(value1.Length(), 0U, "Length()", __LINE__);
    helper.EqualsFalse(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(value1.GetNumber(), 0.0, "GetNumber()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::NotANumber, "NotANumber", __LINE__);
    helper.EqualsFalse(value1.GetBool(bool_var), "GetBool()", __LINE__);
    helper.Equal(value1.Stringify(), "", "Stringify()", __LINE__);

    value1 = ValueC{ValueType::Object};
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);

    value1 = ValueC{ValueType::Array};
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);

    value1 = ValueC{ValueType::String};
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);

    value1 = ValueC{ValueType::UIntLong};
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.EqualsTrue(value1.IsUInt64(), "IsUInt64()", __LINE__);

    value1 = ValueC{ValueType::IntLong};
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.EqualsTrue(value1.IsInt64(), "IsInt64()", __LINE__);

    value1 = ValueC{ValueType::Double};
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.EqualsTrue(value1.IsDouble(), "IsDouble()", __LINE__);
}

static void TestTrueValue(QTest &helper) {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    QNumber64     num_var;
    const char   *c_str_var;
    SizeT         c_str_len;
    bool          bool_var;

    value1 = true;
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::NotANumber, "NumberType = NotANumber", __LINE__);
    helper.EqualsTrue(value1.Type() == ValueType::True, "True", __LINE__);
    helper.Equal(value1.Size(), 0U, "Size()", __LINE__);
    helper.Equal(value1.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.Equal(value1.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsFalse(value1.CopyKeyByIndexTo(ss_var, 0), "CopyKeyByIndexTo(0)", __LINE__);
    helper.EqualsTrue(value1.SetCharAndLength(c_str_var, c_str_len), "SetCharAndLength()", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("true", c_str_var, c_str_len), "value1.SetCharAndLength()", __LINE__);
    helper.Equal(value1.GetObject(), nullptr, "GetObject()", "null", __LINE__);
    helper.Equal(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetString(), nullptr, "GetString()", "null", __LINE__);
    helper.Equal(value1.StringStorage(), nullptr, "StringStorage()", "null", __LINE__);
    helper.Equal(value1.Length(), 0U, "Length()", __LINE__);
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "true", "ss_var", __LINE__);
    helper.Equal(value1.GetNumber(), 1.0, "GetNumber()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Natural, "Natural", __LINE__);
    helper.Equal(num_var.Natural, 1U, "num_var", __LINE__);
    helper.EqualsTrue(value1.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsTrue(bool_var, "bool_var", __LINE__);
    helper.Equal(value1.Stringify(), "", "Stringify()", __LINE__);
    ss_var.Reset();

    value1.Reset();
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    value1 = true;
    value2 = true;
    value2 = value1;
    helper.EqualsTrue(value2.IsTrue(), "IsTrue()", __LINE__);
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);

    value1 = true;
    value2 = ValueC{value1};
    helper.EqualsTrue(value2.IsTrue(), "IsTrue()", __LINE__);

    value2.Reset();
    value2 = Memory::Move(value1);
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);
    helper.EqualsTrue(value2.IsTrue(), "IsTrue()", __LINE__);

    value1 = true;
    ValueC value3{Memory::Move(value1)};
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    helper.EqualsTrue(value3.IsTrue(), "IsTrue()", __LINE__);

    value3 = true;
    helper.EqualsTrue(value3.IsTrue(), "IsTrue()", __LINE__);

    value3 = ValueC{ValueType::True};
    helper.EqualsTrue(value3.IsTrue(), "IsTrue()", __LINE__);
}

static void TestFalseValue(QTest &helper) {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    QNumber64     num_var;
    const char   *c_str_var;
    SizeT         c_str_len;
    bool          bool_var;

    value1 = false;
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::NotANumber, "NumberType = NotANumber", __LINE__);
    helper.EqualsTrue(value1.Type() == ValueType::False, "False", __LINE__);
    helper.Equal(value1.Size(), 0U, "Size()", __LINE__);
    helper.Equal(value1.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.Equal(value1.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsFalse(value1.CopyKeyByIndexTo(ss_var, 0), "CopyKeyByIndexTo(0)", __LINE__);
    helper.EqualsTrue(value1.SetCharAndLength(c_str_var, c_str_len), "SetCharAndLength()", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("false", c_str_var, c_str_len), "value1.SetCharAndLength()", __LINE__);
    helper.Equal(value1.GetObject(), nullptr, "GetObject()", "null", __LINE__);
    helper.Equal(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetString(), nullptr, "GetString()", "null", __LINE__);
    helper.Equal(value1.StringStorage(), nullptr, "StringStorage()", "null", __LINE__);
    helper.Equal(value1.Length(), 0U, "Length()", __LINE__);
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "false", "ss_var", __LINE__);
    helper.Equal(value1.GetNumber(), 0.0, "GetNumber()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Natural, "Natural", __LINE__);
    helper.Equal(num_var.Natural, 0U, "num_var", __LINE__);
    helper.EqualsTrue(value1.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsFalse(bool_var, "bool_var", __LINE__);
    helper.Equal(value1.Stringify(), "", "Stringify()", __LINE__);
    ss_var.Reset();

    value1.Reset();
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    value1 = false;
    value2 = false;
    value2 = value1;
    helper.EqualsTrue(value2.IsFalse(), "IsFalse()", __LINE__);
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);

    value1 = false;
    value2 = ValueC{value1};
    helper.EqualsTrue(value2.IsFalse(), "IsFalse()", __LINE__);

    value2.Reset();
    value2 = Memory::Move(value1);
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);
    helper.EqualsTrue(value2.IsFalse(), "IsFalse()", __LINE__);

    value1 = false;
    ValueC value3(Memory::Move(value1));
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    helper.EqualsTrue(value3.IsFalse(), "IsFalse()", __LINE__);

    value3 = false;
    helper.EqualsTrue(value3.IsFalse(), "IsFalse()", __LINE__);

    value3 = ValueC{ValueType::False};
    helper.EqualsTrue(value3.IsFalse(), "IsFalse()", __LINE__);
}

static void TestNullValue(QTest &helper) {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    QNumber64     num_var;
    const char   *c_str_var;
    SizeT         c_str_len;
    bool          bool_var;

    value1 = nullptr;
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::NotANumber, "NumberType = NotANumber", __LINE__);
    helper.EqualsTrue(value1.Type() == ValueType::Null, "Null", __LINE__);
    helper.Equal(value1.Size(), 0U, "Size()", __LINE__);
    helper.Equal(value1.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.Equal(value1.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.EqualsFalse(value1.CopyKeyByIndexTo(ss_var, 0), "CopyKeyByIndexTo(0)", __LINE__);
    helper.EqualsTrue(value1.SetCharAndLength(c_str_var, c_str_len), "SetCharAndLength()", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual("null", c_str_var, c_str_len), "value1.SetCharAndLength()", __LINE__);
    helper.Equal(value1.GetObject(), nullptr, "GetObject()", "null", __LINE__);
    helper.Equal(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetString(), nullptr, "GetString()", "null", __LINE__);
    helper.Equal(value1.StringStorage(), nullptr, "StringStorage()", "null", __LINE__);
    helper.Equal(value1.Length(), 0U, "Length()", __LINE__);
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "null", "ss_var", __LINE__);
    helper.Equal(value1.GetNumber(), 0.0, "GetNumber()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Natural, "Natural", __LINE__);
    helper.Equal(num_var.Natural, 0U, "num_var", __LINE__);
    helper.EqualsTrue(value1.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsFalse(bool_var, "bool_var", __LINE__);
    helper.Equal(value1.Stringify(), "", "Stringify()", __LINE__);
    ss_var.Reset();

    value1.Reset();
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    value1 = nullptr;
    value2 = nullptr;
    value2 = value1;
    helper.EqualsTrue(value2.IsNull(), "IsNull()", __LINE__);
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);

    value1 = nullptr;
    value2 = ValueC{value1};
    helper.EqualsTrue(value2.IsNull(), "IsNull()", __LINE__);

    value2.Reset();
    value2 = Memory::Move(value1);
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);
    helper.EqualsTrue(value2.IsNull(), "IsNull()", __LINE__);

    value1 = nullptr;
    ValueC value3(Memory::Move(value1));
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    helper.EqualsTrue(value3.IsNull(), "IsNull()", __LINE__);

    value3 = nullptr;
    helper.EqualsTrue(value3.IsNull(), "IsNull()", __LINE__);

    value3 = ValueC{ValueType::Null};
    helper.EqualsTrue(value3.IsNull(), "IsNull()", __LINE__);
}

static void TestNumberValue1(QTest &helper) {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    QNumber64     num_var;
    const char   *c_str_var;
    SizeT         c_str_len;
    bool          bool_var;

    value1 = 33;
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    helper.Equal(value1.Size(), 0U, "Size()", __LINE__);
    helper.Equal(value1.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.Equal(value1.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.Equal(value1.CopyKeyByIndexTo(ss_var, 0), false, "CopyKeyByIndexTo(0)", "false", __LINE__);
    helper.Equal(value1.SetCharAndLength(c_str_var, c_str_len), false, "SetCharAndLength()", "false", __LINE__);
    helper.Equal(value1.GetObject(), nullptr, "GetObject()", "null", __LINE__);
    helper.Equal(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetString(), nullptr, "GetString()", "null", __LINE__);
    helper.Equal(value1.StringStorage(), nullptr, "StringStorage()", "null", __LINE__);
    helper.Equal(value1.Length(), 0U, "Length()", __LINE__);
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "33", "ss_var", __LINE__);
    helper.Equal(value1.GetNumber(), 33.0, "GetNumber()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Integer, "Integer", __LINE__);
    helper.Equal(num_var.Integer, 33, "num_var", __LINE__);
    helper.EqualsTrue(value1.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsTrue(bool_var, "bool_var", __LINE__);
    helper.Equal(value1.Stringify(), "", "Stringify()", __LINE__);

    value1.Reset();
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    value1 = 45;
    value2 = -10;
    value2 = value1;
    helper.EqualsTrue(value2.IsNumber(), "IsNumber()", __LINE__);
    helper.EqualsTrue(value2.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    ss_var.Clear();
    helper.EqualsTrue(value2.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "45", "ss_var", __LINE__);
    helper.Equal(value2.GetNumber(), 45.0, "GetNumber()", __LINE__);
    helper.EqualsTrue(value2.SetNumber(num_var) == QNumberType::Integer, "Integer", __LINE__);
    helper.Equal(num_var.Integer, 45, "num_var", __LINE__);
    helper.EqualsTrue(value2.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsTrue(bool_var, "bool_var", __LINE__);
    helper.Equal(value1.GetNumber(), 45U, "GetNumber()", __LINE__);

    value1.Reset();

    value1 = SizeT32{10};
    value2 = ValueC{value1};
    helper.Equal(value2.GetNumber(), 10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);

    value2.Reset();
    value2 = Memory::Move(value1);
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);
    helper.Equal(value2.GetNumber(), 10.0, "GetNumber()", __LINE__);

    value2 = 1213;
    helper.Equal(value2.GetNumber(), 1213.0, "GetNumber()", __LINE__);

    value2 = 10;
    value2 = double{785};
    helper.Equal(value2.GetNumber(), 785.0, "GetNumber()", __LINE__);

    value2 = SizeT32{0};
    helper.EqualsTrue(value2.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsFalse(bool_var, "bool_var", __LINE__);

    value2 = int{-8};
    helper.EqualsTrue(value2.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsFalse(bool_var, "bool_var", __LINE__);

    value2 = ValueC{double{3.75}};
    helper.EqualsTrue(value2.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value2.GetNumber(), 3.75, "GetNumber()", __LINE__);
}

static void TestNumberValue2(QTest &helper) {
    using vu_long = unsigned long;

    ValueC        value1;
    VStringStream ss_var;
    QNumber64     num_var;
    bool          bool_var;

    /////////////////// unsigned

    value1 = ValueC{SizeT16{10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Natural, "NumberType = Natural", __LINE__);
    helper.EqualsTrue(value1.Type() == ValueType::UIntLong, "UIntLong", __LINE__);
    helper.EqualsTrue(value1.IsUInt64(), "IsUInt64()", __LINE__);
    helper.EqualsFalse(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.EqualsFalse(value1.IsDouble(), "IsDouble()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), 10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), 10, "GetInt64()", __LINE__);
    helper.Equal(value1.GetUInt64(), 10U, "GetUInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Natural, "Natural", __LINE__);
    helper.Equal(num_var.Natural, 10U, "ulong_var", __LINE__);
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "10", "ss_var", __LINE__);
    helper.EqualsTrue(value1.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsTrue(bool_var, "bool_var", __LINE__);
    value1.Reset();

    value1 = ValueC{SizeT32{10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Natural, "NumberType = Natural", __LINE__);
    helper.EqualsTrue(value1.IsUInt64(), "IsUInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), 10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), 10, "GetInt64()", __LINE__);
    helper.Equal(value1.GetUInt64(), 10U, "GetUInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Natural, "Natural", __LINE__);
    helper.Equal(num_var.Natural, 10U, "ulong_var", __LINE__);
    value1.Reset();

    value1 = ValueC{vu_long{10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Natural, "NumberType = Natural", __LINE__);
    helper.EqualsTrue(value1.IsUInt64(), "IsUInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), 10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), 10, "GetInt64()", __LINE__);
    helper.Equal(value1.GetUInt64(), 10U, "GetUInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Natural, "Natural", __LINE__);
    helper.Equal(num_var.Natural, 10U, "num_var", __LINE__);
    value1.Reset();

    value1 = ValueC{SizeT64{10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Natural, "NumberType = Natural", __LINE__);
    helper.EqualsTrue(value1.IsUInt64(), "IsUInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), 10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), 10, "GetInt64()", __LINE__);
    helper.Equal(value1.GetUInt64(), 10U, "GetUInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Natural, "Natural", __LINE__);
    helper.Equal(num_var.Natural, 10U, "num_var", __LINE__);
    value1.Reset();
}

static void TestNumberValue3(QTest &helper) {
    ValueC        value1;
    VStringStream ss_var;
    QNumber64     num_var;
    bool          bool_var;

    /////////////////// signed

    value1 = ValueC{short{-10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    helper.EqualsTrue(value1.Type() == ValueType::IntLong, "IntLong", __LINE__);
    helper.EqualsFalse(value1.IsUInt64(), "IsUInt64()", __LINE__);
    helper.EqualsTrue(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.EqualsFalse(value1.IsDouble(), "IsDouble()", __LINE__);
    helper.Equal(value1.GetNumber(), -10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), -10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), -10, "GetInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Integer, "Integer", __LINE__);
    helper.Equal(num_var.Integer, -10, "num_var", __LINE__);
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "-10", "ss_var", __LINE__);
    helper.EqualsTrue(value1.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsFalse(bool_var, "bool_var", __LINE__);
    value1.Reset();

    value1 = ValueC{short{10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    helper.EqualsTrue(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), 10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), 10, "GetInt64()", __LINE__);
    helper.Equal(value1.GetUInt64(), 10U, "GetUInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Integer, "Integer", __LINE__);
    helper.Equal(num_var.Integer, 10, "num_var", __LINE__);
    ss_var.Clear();
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "10", "ss_var", __LINE__);
    value1.Reset();

    value1 = ValueC{int{-10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    helper.EqualsTrue(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), -10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), -10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), -10, "GetInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Integer, "Integer", __LINE__);
    helper.Equal(num_var.Integer, -10, "num_var", __LINE__);
    value1.Reset();

    value1 = ValueC{int{10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    helper.EqualsTrue(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), 10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), 10, "GetInt64()", __LINE__);
    helper.Equal(value1.GetUInt64(), 10U, "GetUInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Integer, "Integer", __LINE__);
    helper.Equal(num_var.Integer, 10U, "num_var", __LINE__);
    value1.Reset();

    value1 = ValueC{long{-10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    helper.EqualsTrue(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), -10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), -10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), -10, "GetInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Integer, "Integer", __LINE__);
    helper.Equal(num_var.Integer, -10, "num_var", __LINE__);
    value1.Reset();

    value1 = ValueC{long{10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    helper.EqualsTrue(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), 10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), 10, "GetInt64()", __LINE__);
    helper.Equal(value1.GetUInt64(), 10U, "GetUInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Integer, "Integer", __LINE__);
    helper.Equal(num_var.Integer, 10U, "num_var", __LINE__);
    value1.Reset();

    value1 = ValueC{long{-10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    helper.EqualsTrue(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), -10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), -10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), -10, "GetInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Integer, "Integer", __LINE__);
    helper.Equal(num_var.Integer, -10, "num_var", __LINE__);
    value1.Reset();

    value1 = ValueC{SizeT64I{10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    helper.EqualsTrue(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), 10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), 10, "GetInt64()", __LINE__);
    helper.Equal(value1.GetUInt64(), 10U, "GetUInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Integer, "Integer", __LINE__);
    helper.Equal(num_var.Integer, 10U, "num_var", __LINE__);
    value1.Reset();
}

static void TestNumberValue4(QTest &helper) {
    ValueC        value1;
    VStringStream ss_var;
    QNumber64     num_var;
    bool          bool_var;

    /////////////////// float

    value1 = ValueC{float{10.5}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Real, "NumberType = Real", __LINE__);
    helper.EqualsTrue(value1.Type() == ValueType::Double, "Double", __LINE__);
    helper.EqualsFalse(value1.IsUInt64(), "IsUInt64()", __LINE__);
    helper.EqualsFalse(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.EqualsTrue(value1.IsDouble(), "IsDouble()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.5, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), 10.5, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), 10, "GetInt64()", __LINE__);
    helper.Equal(value1.GetUInt64(), 10U, "GetUInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Real, "Real", __LINE__);
    helper.Equal(num_var.Real, 10.5, "num_var", __LINE__);
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "10.5", "ss_var", __LINE__);
    helper.EqualsTrue(value1.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsTrue(bool_var, "bool_var", __LINE__);
    value1.Reset();

    value1 = ValueC{float{-10.5}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Real, "NumberType = Real", __LINE__);
    helper.EqualsTrue(value1.IsDouble(), "IsDouble()", __LINE__);
    helper.Equal(value1.GetNumber(), -10.5, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), -10.5, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), -10, "GetInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Real, "Real", __LINE__);
    helper.Equal(num_var.Real, -10.5, "num_var", __LINE__);
    ss_var.Clear();
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "-10.5", "ss_var", __LINE__);
    helper.EqualsTrue(value1.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsFalse(bool_var, "bool_var", __LINE__);
    value1.Reset();

    value1 = ValueC{float{10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Real, "NumberType = Real", __LINE__);
    helper.EqualsTrue(value1.IsDouble(), "IsDouble()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), 10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), 10, "GetInt64()", __LINE__);
    helper.Equal(value1.GetUInt64(), 10U, "GetUInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Real, "Real", __LINE__);
    helper.Equal(num_var.Real, 10U, "num_var", __LINE__);
    value1.Reset();

    value1 = ValueC{float{-10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Real, "NumberType = Real", __LINE__);
    helper.EqualsTrue(value1.IsDouble(), "IsDouble()", __LINE__);
    helper.Equal(value1.GetNumber(), -10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), -10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), -10, "GetInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Real, "Real", __LINE__);
    helper.Equal(num_var.Real, -10, "num_var", __LINE__);
    value1.Reset();

    value1 = ValueC{double{10.5}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Real, "NumberType = Real", __LINE__);
    helper.EqualsTrue(value1.IsDouble(), "IsDouble()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.5, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), 10.5, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), 10, "GetInt64()", __LINE__);
    helper.Equal(value1.GetUInt64(), 10U, "GetUInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Real, "Real", __LINE__);
    helper.Equal(num_var.Real, 10.5, "num_var", __LINE__);
    value1.Reset();

    value1 = ValueC{double{-10.5}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Real, "NumberType = Real", __LINE__);
    helper.EqualsTrue(value1.IsDouble(), "IsDouble()", __LINE__);
    helper.Equal(value1.GetNumber(), -10.5, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), -10.5, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), -10, "GetInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Real, "Real", __LINE__);
    helper.Equal(num_var.Real, -10.5, "num_var", __LINE__);
    value1.Reset();

    value1 = ValueC{double{10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Real, "NumberType = Real", __LINE__);
    helper.EqualsTrue(value1.IsDouble(), "IsDouble()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), 10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), 10, "GetInt64()", __LINE__);
    helper.Equal(value1.GetUInt64(), 10U, "GetUInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Real, "Real", __LINE__);
    helper.Equal(num_var.Real, 10.0, "num_var", __LINE__);
    ss_var.Clear();
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "10", "ss_var", __LINE__);
    helper.EqualsTrue(value1.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsTrue(bool_var, "bool_var", __LINE__);
    value1.Reset();

    value1 = ValueC{double{-10}};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Real, "NumberType = Real", __LINE__);
    helper.EqualsTrue(value1.IsDouble(), "IsDouble()", __LINE__);
    helper.Equal(value1.GetNumber(), -10.0, "GetNumber()", __LINE__);
    helper.Equal(value1.GetDouble(), -10.0, "GetDouble()", __LINE__);
    helper.Equal(value1.GetInt64(), -10, "GetInt64()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Real, "Real", __LINE__);
    helper.Equal(num_var.Real, -10, "num_var", __LINE__);
    ss_var.Clear();
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "-10", "ss_var", __LINE__);
    helper.EqualsTrue(value1.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsFalse(bool_var, "bool_var", __LINE__);
    value1.Reset();
}

static void TestNumberValue5(QTest &helper) {
    using vu_long = unsigned long;

    ValueC value1;

    value1 = SizeT16{10};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Natural, "NumberType = Natural", __LINE__);
    helper.EqualsTrue(value1.IsUInt64(), "IsUInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);

    value1 = -10;
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    helper.EqualsTrue(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), -10.0, "GetNumber()", __LINE__);

    value1 = -10.5;
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Real, "NumberType = Real", __LINE__);
    helper.EqualsTrue(value1.IsDouble(), "IsDouble()", __LINE__);
    helper.Equal(value1.GetNumber(), -10.5, "GetNumber()", __LINE__);

    value1 = SizeT32{10};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Natural, "NumberType = Natural", __LINE__);
    helper.EqualsTrue(value1.IsUInt64(), "IsUInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);

    value1 = float{10};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Real, "NumberType = Real", __LINE__);
    helper.EqualsTrue(value1.IsDouble(), "IsDouble()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);

    value1 = int{10};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    helper.EqualsTrue(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);

    value1 = vu_long{10};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Natural, "NumberType = Natural", __LINE__);
    helper.EqualsTrue(value1.IsUInt64(), "IsUInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);

    value1 = long{-10};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    helper.EqualsTrue(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), -10.0, "GetNumber()", __LINE__);

    value1 = SizeT64{10};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Natural, "NumberType = Natural", __LINE__);
    helper.EqualsTrue(value1.IsUInt64(), "IsUInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);

    value1 = double{-10};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Real, "NumberType = Real", __LINE__);
    helper.EqualsTrue(value1.IsDouble(), "IsDouble()", __LINE__);
    helper.Equal(value1.GetNumber(), -10.0, "GetNumber()", __LINE__);

    value1 = SizeT64I{10};
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::Integer, "NumberType = Integer", __LINE__);
    helper.EqualsTrue(value1.IsInt64(), "IsInt64()", __LINE__);
    helper.Equal(value1.GetNumber(), 10.0, "GetNumber()", __LINE__);
}

static void TestStringValue(QTest &helper) {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    VString       str_var;
    QNumber64     num_var;
    const char   *c_str_var;
    SizeT         c_str_len;
    bool          bool_var;

    value1 = "-ABCDEF0123456789ABCDEF0123456789-";
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::NotANumber, "NumberType = NotANumber", __LINE__);
    helper.EqualsTrue(value1.Type() == ValueType::String, "String", __LINE__);
    helper.Equal(value1.Size(), 0U, "Size()", __LINE__);
    helper.Equal(value1.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.Equal(value1.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.Equal(value1.CopyKeyByIndexTo(ss_var, 0), false, "CopyKeyByIndexTo(0)", "false", __LINE__);
    helper.Equal(value1.SetCharAndLength(c_str_var, c_str_len), true, "SetCharAndLength()", "true", __LINE__);
    helper.Equal(StringUtils::IsEqual(value1.StringStorage(), c_str_var, c_str_len), true, "value1.StringStorage()",
                 "value1.SetCharAndLength()", __LINE__);
    helper.Equal(value1.GetObject(), nullptr, "GetObject()", "null", __LINE__);
    helper.Equal(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetUInt64(), SizeT64{0}, "GetUInt64()", "null", __LINE__);
    helper.Equal(value1.GetInt64(), 0, "GetInt64()", "null", __LINE__);
    helper.Equal(value1.GetDouble(), 0.0, "GetDouble()", "null", __LINE__);
    helper.NotEqual(value1.GetString(), nullptr, "GetString()", "null", __LINE__);
    helper.Equal(*(value1.GetString()), "-ABCDEF0123456789ABCDEF0123456789-", "GetString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), nullptr, "StringStorage()", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1.StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "IsEqual()", __LINE__);
    helper.Equal(value1.Length(), 34U, "Length()", __LINE__);
    ss_var.Clear();
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "-ABCDEF0123456789ABCDEF0123456789-", "ss_var", __LINE__);
    ss_var.Clear();
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "-ABCDEF0123456789ABCDEF0123456789-", "ss_var", __LINE__);
    helper.Equal(value1.GetNumber(), 0.0, "GetNumber()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::NotANumber, "NotANumber", __LINE__);
    helper.EqualsFalse(value1.GetBool(bool_var), "GetBool()", __LINE__);
    helper.Equal(value1.Stringify(), "", "Stringify()", __LINE__);

    value1.Reset();
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    value1 = "45";
    value2 = "-50";
    helper.EqualsTrue(value2.SetNumber(num_var) == QNumberType::Integer, "Integer", __LINE__);
    helper.Equal(num_var.Integer, -50, "num_var", __LINE__);
    helper.Equal(value2.GetNumber(), -50.0, "GetNumber()", __LINE__);

    value2 = value1;
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value1.GetString(), nullptr, "GetString()", "null", __LINE__);
    helper.Equal(*(value1.GetString()), "45", "GetString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), nullptr, "StringStorage()", "null", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1.StringStorage(), "45", 2), "IsEqual()", __LINE__);
    helper.Equal(value1.Length(), 2U, "Length()", __LINE__);
    ss_var.Clear();
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "45", "ss_var", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::Natural, "Natural", __LINE__);
    helper.Equal(num_var.Natural, 45U, "num_var", __LINE__);
    helper.Equal(value1.GetNumber(), 45.0, "GetNumber()", __LINE__);
    helper.EqualsFalse(value1.GetBool(bool_var), "GetBool()", __LINE__);
    ss_var.Clear();
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "45", "ss_var", __LINE__);

    value1 = "true";
    value2 = ValueC{value1};
    ss_var.Clear();
    helper.EqualsTrue(value2.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "true", "ss_var", __LINE__);
    helper.Equal(value2.GetNumber(), 0.0, "GetNumber()", __LINE__);
    bool_var = false;
    helper.EqualsTrue(value2.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsTrue(bool_var, "bool_var", __LINE__);

    ss_var.Clear();
    helper.EqualsTrue(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "true", "ss_var", __LINE__);

    value2.Reset();
    value2 = Memory::Move(value1);
    ss_var.Clear();
    helper.EqualsTrue(value2.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "true", "ss_var", __LINE__);

    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    value1 = "false";
    ValueC value3(Memory::Move(value1));
    helper.Equal(value3.Length(), 5U, "Length()", __LINE__);
    ss_var.Clear();
    helper.EqualsTrue(value3.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "false", "ss_var", __LINE__);
    bool_var = true;
    helper.EqualsTrue(value3.GetBool(bool_var), "GetBool()", __LINE__);
    helper.EqualsFalse(bool_var, "bool_var", __LINE__);

    str_var           = "qen";
    const char *c_str = str_var.First();
    value3            = str_var;
    ss_var.Clear();
    helper.EqualsTrue(value3.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "qen", "ss_var", __LINE__);
    helper.NotEqual(value3.StringStorage(), c_str, "str_var.First()", "c_str", __LINE__);

    value3 = ValueC{VString("ABC")};
    helper.EqualsTrue(value3.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value3.GetString(), nullptr, "GetString()", "null", __LINE__);
    helper.Equal(*(value3.GetString()), "ABC", "GetString()", __LINE__);

    value3 = VString("123");
    ss_var.Clear();
    helper.EqualsTrue(value3.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "123", "ss_var", __LINE__);

    value3 = 321.25;
    value3 = "321.25";
    helper.EqualsTrue(value3.IsString(), "IsString()", __LINE__);
    ss_var.Clear();
    helper.EqualsTrue(value3.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(ss_var, "321.25", "ss_var", __LINE__);
}

static void TestArrayValue(QTest &helper) {
    ValueC value1;
    ValueC value2;

    VArray        arr_var;
    const ValueC *storage;
    VStringStream ss_var;
    VString       str_var;
    QNumber64     num_var;
    const char   *c_str_var;
    SizeT         c_str_len;
    bool          bool_var;

    arr_var.ResizeAndInitialize(5);
    storage = arr_var.First();

    value1 = arr_var; // Copy.
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::NotANumber, "NumberType = NotANumber", __LINE__);
    helper.EqualsTrue(value1.Type() == ValueType::Array, "Array", __LINE__);
    helper.Equal(value1.Size(), 5U, "Size()", __LINE__);
    helper.Equal(value1.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.Equal(value1.GetValue(4), nullptr, "GetValue(4)", "null", __LINE__);
    helper.Equal(value1.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.Equal(value1.CopyKeyByIndexTo(ss_var, 0), false, "CopyKeyByIndexTo(0)", "false", __LINE__);
    helper.Equal(value1.SetCharAndLength(c_str_var, c_str_len), false, "SetCharAndLength()", "false", __LINE__);
    helper.Equal(value1.GetObject(), nullptr, "GetObject()", "null", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), storage, "GetArray()->First()", "storage", __LINE__);
    helper.Equal(value1.GetString(), nullptr, "GetString()", "null", __LINE__);
    helper.Equal(value1.StringStorage(), nullptr, "StringStorage()", "null", __LINE__);
    helper.Equal(value1.Length(), 0U, "Length()", __LINE__);
    helper.EqualsFalse(value1.CopyValueTo(ss_var), "CopyValueTo()", __LINE__);
    helper.Equal(value1.GetNumber(), 0.0, "GetNumber()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::NotANumber, "NotANumber", __LINE__);
    helper.EqualsFalse(value1.GetBool(bool_var), "GetBool()", __LINE__);
    helper.Equal(value1.Stringify(), "[]", "Stringify()", __LINE__);

    arr_var.Reset();
    value1 = arr_var;
    helper.Equal(value1.Stringify(), "[]", "Stringify()", __LINE__);

    value1.Reset();
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    arr_var.Reset();
    arr_var.ResizeAndInitialize(10);
    storage = arr_var.First();

    VArray arr_var2;
    arr_var2.ResizeAndInitialize(10);

    value1 = Memory::Move(arr_var); // Move
    value2 = Memory::Move(arr_var2);
    value2 = value1;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 10U, "Size()", __LINE__);
    helper.Equal(value1.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.Equal(value1.GetValue(9), nullptr, "GetValue(9)", "null", __LINE__);
    helper.Equal(value1.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    helper.Equal(value2.Size(), 10U, "Size()", __LINE__);
    helper.NotEqual(value2.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value2.GetArray()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    arr_var.Reset();
    arr_var.ResizeAndInitialize(7);
    storage = arr_var.First();
    value1  = Memory::Move(arr_var);

    value2 = ValueC{value1};
    helper.EqualsTrue(value2.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value2.Size(), 7U, "Size()", __LINE__);
    helper.NotEqual(value2.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value2.GetArray()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 7U, "Size()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    value2.Reset();
    value2 = Memory::Move(value1);
    helper.NotEqual(value2.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value2.GetArray()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    arr_var.Reset();
    arr_var.ResizeAndInitialize(7);
    storage = arr_var.First();
    value1  = Memory::Move(arr_var);

    ValueC value3(Memory::Move(value1));
    helper.NotEqual(value3.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value3.GetArray()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    arr_var.Reset();
    arr_var.ResizeAndInitialize(3);
    storage = arr_var.First();
    value3  = arr_var; // Copy
    helper.Equal(value3.Size(), 3U, "Size()", __LINE__);
    helper.NotEqual(value3.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value3.GetArray()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    arr_var.Reset();
    arr_var.ResizeAndInitialize(13);
    storage = arr_var.First();
    value3  = Memory::Move(arr_var); // Move
    helper.NotEqual(value3.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value3.GetArray()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    value3 = ValueC{VArray(3)};
    helper.EqualsTrue(value3.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value3.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value3.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value3.GetArray()->Capacity(), 3U, "GetArray()->Capacity()", __LINE__);

    value1[0] = 1;

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 1U, "Size()", __LINE__);
    helper.Equal(value1[0].GetNumber(), 1.0, R"(value1[0].GetNumber())", __LINE__);

    value1.Reset();

    value1[0] = 1;
    value1[1] = 2;
    value1[2] = 3;

    helper.Equal(value1.Size(), 3U, "value1.Size()", __LINE__);

    value2 = value1;
    helper.Equal(value2.Size(), 3U, "value2.Size()", __LINE__);
    helper.Equal(value2[0].GetNumber(), 1.0, "value2[0].GetNumber()", __LINE__);
    helper.Equal(value2[1].GetNumber(), 2.0, "value2[1].GetNumber()", __LINE__);
    helper.Equal(value2[2].GetNumber(), 3.0, "value2[2].GetNumber()", __LINE__);

    value1[0] = 11;
    value1[1] = 22;
    str_var   = "Qen";
    c_str_var = str_var.First();
    value1[2] = Memory::Move(str_var);

    value2 = value1;
    helper.Equal(value2.Size(), 3U, "value2.Size()", __LINE__);
    helper.Equal(value2[0].GetNumber(), 11.0, "value2[0].GetNumber()", __LINE__);
    helper.Equal(value2[1].GetNumber(), 22.0, "value2[1].GetNumber()", __LINE__);
    helper.NotEqual(value2[2].StringStorage(), c_str_var, "value2[2].StringStorage()", "c_str_var", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2[2].StringStorage(), "Qen", 3), true, "value2[2].StringStorage()", "Qen",
                 __LINE__);

    value2[3] = 44;
    value2[4] = 55;
    helper.Equal(value2.Size(), 5U, "value2.Size()", __LINE__);
    helper.Equal(value2[0].GetNumber(), 11.0, "value2[0].GetNumber()", __LINE__);
    helper.Equal(value2[1].GetNumber(), 22.0, "value2[1].GetNumber()", __LINE__);
    helper.NotEqual(value2[2].StringStorage(), c_str_var, "value2[2].StringStorage()", "c_str_var", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2[2].StringStorage(), "Qen", 3), true, "value2[2].StringStorage()", "Qen",
                 __LINE__);
    helper.Equal(value2[3].GetNumber(), 44.0, "value2[3].GetNumber()", __LINE__);
    helper.Equal(value2[4].GetNumber(), 55.0, "value2[4].GetNumber()", __LINE__);

    value2 = value1;
    helper.Equal(value2.Size(), 3U, "value2.Size()", __LINE__);
    helper.Equal(value2[0].GetNumber(), 11.0, "value2[0].GetNumber()", __LINE__);
    helper.Equal(value2[1].GetNumber(), 22.0, "value2[1].GetNumber()", __LINE__);
    helper.NotEqual(value2[2].StringStorage(), c_str_var, "value2[2].StringStorage()", "c_str_var", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2[2].StringStorage(), "Qen", 3), true, "value2[2].StringStorage()", "Qen",
                 __LINE__);
    helper.Equal(value2.GetValue(3), nullptr, "value2.GetValue(3)", "null", __LINE__);
    helper.Equal(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null", __LINE__);

    ////////////////////
    str_var   = "-ABCDEF0123456789ABCDEF0123456789-";
    c_str_var = str_var.First();

    arr_var.ResizeAndInitialize(4);
    ValueC *val_ptr = arr_var.Storage();

    val_ptr[0] = 10;
    val_ptr[1] = 20;
    val_ptr[2] = 30;
    val_ptr[3] = Memory::Move(str_var);

    value2 = arr_var;
    helper.Equal(value2.Size(), 4U, "value2.Size()", __LINE__);
    helper.Equal(value2[0].GetNumber(), 10.0, "value2[0].GetNumber()", __LINE__);
    helper.Equal(value2[1].GetNumber(), 20.0, "value2[1].GetNumber()", __LINE__);
    helper.Equal(value2[2].GetNumber(), 30.0, "value2[1].GetNumber()", __LINE__);
    helper.NotEqual(value2[3].StringStorage(), c_str_var, "value2[3].StringStorage()", "c_str_var", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2[3].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), true,
                 "value2[3].StringStorage()", "-ABCDEF0123456789ABCDEF0123456789-", __LINE__);
    helper.Equal(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null", __LINE__);

    ////////////////////

    arr_var.Clear();
    arr_var.Resize(5);
    value2 = Memory::Move(arr_var);
    helper.NotEqual(value2.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value2.GetArray()->Capacity(), 5U, "value2.Size()", __LINE__);

    value2 += 1;

    value2.Compress();
    helper.NotEqual(value2.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value2.GetArray()->Capacity(), 1U, "Capacity()", __LINE__);
    helper.Equal(value2.GetArray()->Size(), 1U, "Size()", __LINE__);

    arr_var.Reset();
    arr_var.Reserve(10);
    value2 = Memory::Move(arr_var);
    value2.Compress();
    helper.NotEqual(value2.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value2.GetArray()->Capacity(), 0U, "Capacity()", __LINE__);
    helper.Equal(value2.GetArray()->Size(), 0U, "Size()", __LINE__);
}

static void TestObjectValue1(QTest &helper) {
    ValueC value1;
    ValueC value2;

    using ObjectItem_ = HAItem_T_<ValueC, char>;

    VHArray            h_arr_var;
    const ObjectItem_ *storage;
    VStringStream      ss_var;
    QNumber64          num_var;
    const char        *c_str_var;
    SizeT              c_str_len;
    bool               bool_var;

    for (SizeT32 i = 0; i < 5; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();

    value1 = h_arr_var; // Copy.
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.EqualsTrue(value1.GetNumberType() == QNumberType::NotANumber, "NumberType = NotANumber", __LINE__);
    helper.EqualsTrue(value1.Type() == ValueType::Object, "Object", __LINE__);
    helper.Equal(value1.Size(), 5U, "Size()", __LINE__);
    helper.NotEqual(value1.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.NotEqual(value1.GetValue(4), nullptr, "GetValue(4)", "null", __LINE__);
    helper.NotEqual(value1.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.NotEqual(value1.GetKey(4), nullptr, "GetKey(4)", "null", __LINE__);
    helper.NotEqual(value1.CopyKeyByIndexTo(ss_var, 0), false, "CopyKeyByIndexTo(0)", "false", __LINE__);
    helper.NotEqual(value1.CopyKeyByIndexTo(ss_var, 4), false, "CopyKeyByIndexTo(4)", "false", __LINE__);
    helper.Equal(value1.SetCharAndLength(c_str_var, c_str_len), false, "SetCharAndLength()", "false", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), storage, "GetArray()->First()", "storage", __LINE__);
    helper.Equal(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetString(), nullptr, "GetString()", "null", __LINE__);
    helper.Equal(value1.StringStorage(), nullptr, "StringStorage()", "null", __LINE__);
    helper.Equal(value1.Length(), 0U, "Length()", __LINE__);
    helper.EqualsFalse(value1.CopyValueTo(ss_var), "SetString()", __LINE__);
    helper.Equal(value1.GetNumber(), 0.0, "GetNumber()", __LINE__);
    helper.EqualsTrue(value1.SetNumber(num_var) == QNumberType::NotANumber, "NotANumber", __LINE__);
    helper.EqualsFalse(value1.GetBool(bool_var), "GetBool()", __LINE__);

    h_arr_var.Reset();
    value1 = h_arr_var;
    helper.Equal(value1.Stringify(), "{}", "Stringify()", __LINE__);

    value1.Reset();
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    value2 = h_arr_var;

    h_arr_var.Reset();
    for (SizeT32 i = 0; i < 10; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();

    value1 = Memory::Move(h_arr_var); // Move
    value2 = value1;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value1.Size(), 10U, "Size()", __LINE__);
    helper.Equal(value1.GetObject()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    value1 = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value1.Stringify(), "{}", "Stringify()", __LINE__);

    helper.EqualsTrue(value2.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value2.GetObject()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    h_arr_var.Reset();
    // Testing empty values
    for (SizeT32 i = 0; i < 10; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key];
    }

    value2 = h_arr_var;
    helper.Equal(value2.Size(), 10U, "Size()", __LINE__);

    h_arr_var.Reset();
    for (SizeT i = 0; i < 7; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value1  = Memory::Move(h_arr_var);

    value2 = ValueC{value1};
    helper.EqualsTrue(value2.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value2.Size(), 7U, "Size()", __LINE__);
    helper.NotEqual(value2.GetObject()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    h_arr_var.Reset();
    value2 = h_arr_var;
    helper.EqualsTrue(value2.IsObject(), "IsObject()", __LINE__);
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value1.GetObject()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    h_arr_var.Reset();
    value1 = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);

    for (SizeT32 i = 0; i < 7; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value1  = Memory::Move(h_arr_var);

    value2.Reset();
    value2 = Memory::Move(value1);
    helper.EqualsTrue(value2.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value2.GetObject()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    h_arr_var.Reset();
    value2 = h_arr_var;
    helper.Equal(value2.Stringify(), "{}", "Stringify()", __LINE__);
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);
}

static void TestObjectValue2(QTest &helper) {
    ValueC value1;

    using ObjectItem_ = HAItem_T_<ValueC, char>;

    VHArray            h_arr_var;
    const ObjectItem_ *storage;
    VString            str_var;
    const char        *c_str_var;

    h_arr_var.Reset();
    for (SizeT32 i = 0; i < 7; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value1  = Memory::Move(h_arr_var);

    ValueC value3(Memory::Move(value1));
    helper.EqualsTrue(value3.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value3.Size(), 7U, "Size()", __LINE__);
    helper.Equal(value3.GetObject()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    h_arr_var.Reset();
    value3 = h_arr_var;
    helper.Equal(value3.Stringify(), "{}", "Stringify()", __LINE__);
    helper.EqualsTrue(value1.IsUndefined(), "isUndefined()", __LINE__);

    h_arr_var.Reset();
    for (SizeT32 i = 0; i < 3; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value3  = h_arr_var; // Copy
    helper.Equal(value3.Size(), 3U, "Size()", __LINE__);
    helper.NotEqual(value3.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value3.GetObject()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value3.GetObject()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    for (SizeT32 i = 0; i < 13; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value3  = Memory::Move(h_arr_var); // Move
    helper.EqualsTrue(value3.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value3.Size(), 13U, "Size()", __LINE__);
    helper.NotEqual(value3.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value3.GetObject()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value3.GetObject()->First(), storage, "GetArray()->First()", "storage", __LINE__);

    value3 = ValueC{VHArray(4)};
    helper.EqualsTrue(value3.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value3.GetObject(), nullptr, "GetObject()", "null", __LINE__);
    helper.NotEqual(value3.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.Equal(value3.GetObject()->Capacity(), 4U, "GetArray()->Capacity()", __LINE__);

    value1.Reset();
    value1[""] = 1;

    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value1.Size(), 1U, "Size()", __LINE__);
    helper.Equal(value1[""].GetNumber(), 1.0, R"(value1[""].GetNumber())", __LINE__);

    value1.Reset();

    value1["k1"] = 1;
    value1["k2"] = 2;
    value1["k3"] = 3;

    helper.Equal(value1.Size(), 3U, "value1.Size()", __LINE__);

    value3 = value1;
    helper.Equal(value3.Size(), 3U, "value3.Size()", __LINE__);
    helper.Equal(value3["k1"].GetNumber(), 1.0, "value3[\"k1\"].GetNumber()", __LINE__);
    helper.Equal(value3["k2"].GetNumber(), 2.0, "value3[\"k2\"].GetNumber()", __LINE__);
    helper.Equal(value3["k3"].GetNumber(), 3.0, "value3[\"k3\"].GetNumber()", __LINE__);

    value1["k1"] = 11;
    value1["k2"] = 22;
    str_var      = "Qen";
    c_str_var    = str_var.First();
    value1["k3"] = Memory::Move(str_var);

    value3 = value1;
    helper.Equal(value3.Size(), 3U, "value3.Size()", __LINE__);
    helper.Equal(value3["k1"].GetNumber(), 11.0, "value3[\"k1\"].GetNumber()", __LINE__);
    helper.Equal(value3["k2"].GetNumber(), 22.0, "value3[\"k2\"].GetNumber()", __LINE__);
    helper.NotEqual(value3["k3"].StringStorage(), c_str_var, "value3[\"k3\"].StringStorage()", "c_str_var", __LINE__);
    helper.Equal(StringUtils::IsEqual(value3["k3"].StringStorage(), "Qen", 3), true, "value3[\"k3\"].StringStorage()",
                 "Qen", __LINE__);

    value3["k4"] = 44;
    value3["k5"] = 55;
    helper.Equal(value3.Size(), 5U, "value3.Size()", __LINE__);
    helper.Equal(value3["k1"].GetNumber(), 11.0, "value3[\"k1\"].GetNumber()", __LINE__);
    helper.Equal(value3["k2"].GetNumber(), 22.0, "value3[\"k2\"].GetNumber()", __LINE__);
    helper.NotEqual(value3["k3"].StringStorage(), c_str_var, "value3[\"k3\"].StringStorage()", "c_str_var", __LINE__);
    helper.Equal(StringUtils::IsEqual(value3["k3"].StringStorage(), "Qen", 3), true, "value3[\"k3\"].StringStorage()",
                 "Qen", __LINE__);
    helper.Equal(value3["k4"].GetNumber(), 44U, "value3[\"k4\"].GetNumber()", __LINE__);
    helper.Equal(value3["k5"].GetNumber(), 55U, "value3[\"k5\"].GetNumber()", __LINE__);

    value3 = value1;
    helper.Equal(value3.Size(), 3U, "value3.Size()", __LINE__);
    helper.Equal(value3["k1"].GetNumber(), 11.0, "value3[\"k1\"].GetNumber()", __LINE__);
    helper.Equal(value3["k2"].GetNumber(), 22.0, "value3[\"k2\"].GetNumber()", __LINE__);
    helper.Equal(StringUtils::IsEqual(value3["k3"].StringStorage(), "Qen", 3), true, "value3[\"k3\"].StringStorage()",
                 "Qen", __LINE__);
    helper.Equal(value3.GetValue("k4", 2), nullptr, "value3.GetValue(\"k4\", 2)", "null", __LINE__);
    helper.Equal(value3.GetValue(3), nullptr, "value3.GetValue(3)", "null", __LINE__);
    helper.Equal(value3.GetValue("k5", 2), nullptr, "value3.GetValue(\"k5\", 2)", "null", __LINE__);
    helper.Equal(value3.GetValue(4), nullptr, "value3.GetValue(4)", "null", __LINE__);

    ////////////////////
    h_arr_var[VString("w1")] = 10;
    h_arr_var[VString("w2")] = 20;
    h_arr_var[VString("w3")] = 30;
    str_var                  = "-ABCDEF0123456789ABCDEF0123456789-";
    c_str_var                = str_var.First();
    h_arr_var[VString("w4")] = Memory::Move(str_var);

    value3 = h_arr_var;
    helper.Equal(value3.Size(), 4U, "value3.Size()", __LINE__);
    helper.Equal(value3["w1"].GetNumber(), 10.0, "[\"w1\"].GetNumber()", __LINE__);
    helper.Equal(value3["w2"].GetNumber(), 20.0, "[\"w2\"].GetNumber()", __LINE__);
    helper.Equal(value3["w3"].GetNumber(), 30.0, "[\"w3\"].GetNumber()", __LINE__);
    helper.NotEqual(value3["w4"].StringStorage(), c_str_var, "value3[\"w4\"].StringStorage()", "c_str_var", __LINE__);
    helper.Equal(StringUtils::IsEqual(value3["w4"].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), true,
                 "value3[\"w4\"].StringStorage()", "Qen", __LINE__);
    helper.Equal(value3.GetValue("k5", 2), nullptr, "value3.GetValue(\"k5\", 2)", "null", __LINE__);
    helper.Equal(value3.GetValue(4), nullptr, "value3.GetValue(4)", "null", __LINE__);
    ////////////////////

    h_arr_var.Reserve(10);
    h_arr_var[VString("w1")] = 10;
    h_arr_var[VString("w2")] = 20;
    h_arr_var[VString("w3")] = 30;

    value3 = Memory::Move(h_arr_var);
    helper.Equal(value3.Size(), 3U, "value3.Size()", __LINE__);

    value3.Compress();
    helper.NotEqual(value3.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value3.GetObject()->Size(), 3U, "Size()", __LINE__);
    helper.EqualsTrue(value3.GetObject()->Capacity() >= 3U, "GetObject()->Capacity() >= 3", __LINE__);
}

static void TestMoveValue1(QTest &helper) {
    ValueC value1;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray                        h_arr_var;
    const HAItem_T_<ValueC, char> *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = true;

    value1 = false;
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    value1 = nullptr;
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    value1 = 11;
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 11.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = Memory::Move(str_var);
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.Equal(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = Memory::Move(arr_var);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = Memory::Move(h_arr_var);
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = false;

    value1 = true;
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    value1 = nullptr;
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    value1 = -90;
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), -90.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = Memory::Move(str_var);
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.Equal(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = Memory::Move(arr_var);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = Memory::Move(h_arr_var);
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = nullptr;

    value1 = true;
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    value1 = false;
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    value1 = 7.5;
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 7.5, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = Memory::Move(str_var);
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.Equal(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = Memory::Move(arr_var);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = Memory::Move(h_arr_var);
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = 13;

    value1 = true;
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = 40;

    value1 = false;
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = 33;

    value1 = nullptr;
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = 47;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = Memory::Move(str_var);
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.Equal(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = 10e10;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = Memory::Move(arr_var);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = 9.1;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = Memory::Move(h_arr_var);
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value1 = true;
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value1 = false;
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value1 = nullptr;
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value1 = 4;
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 4.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = Memory::Move(arr_var);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = Memory::Move(h_arr_var);
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VArray(1);

    value1 = true;
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value1 = false;
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value1 = nullptr;
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value1 = 33;
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 33.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = Memory::Move(str_var);
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.Equal(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = Memory::Move(h_arr_var);
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestMoveValue2(QTest &helper) {
    ValueC value1;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    ////////////////////////////////////////////

    value1 = VHArray(1);

    value1 = true;
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value1 = false;
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value1 = nullptr;
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value1 = 33;
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 33.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = Memory::Move(str_var);
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.Equal(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = Memory::Move(arr_var);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestMoveValue3(QTest &helper) {
    ValueC value1;
    ValueC value2;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray                        h_arr_var;
    const HAItem_T_<ValueC, char> *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = true;

    value2 = false;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    value2 = nullptr;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    value2 = 11;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 11.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = Memory::Move(value2);
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.Equal(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = Memory::Move(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = Memory::Move(value2);
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = false;

    value2 = true;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    value2 = nullptr;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    value2 = -90;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), -90.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = Memory::Move(value2);
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.Equal(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = Memory::Move(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = Memory::Move(value2);
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = nullptr;

    value2 = true;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    value2 = false;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    value2 = 7.5;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 7.5, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = Memory::Move(value2);
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.Equal(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = Memory::Move(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = Memory::Move(value2);
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = 13;

    value2 = true;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = 40;

    value2 = false;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = 33;

    value2 = nullptr;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = 47;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = Memory::Move(value2);
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.Equal(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = 10e10;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = Memory::Move(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = 9.1;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = Memory::Move(value2);
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = true;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = false;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = nullptr;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = 4;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 4.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = Memory::Move(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = Memory::Move(value2);
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VArray(1);

    value2 = true;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = false;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = nullptr;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = 33;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 33.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = Memory::Move(value2);
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.Equal(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = Memory::Move(value2);
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestMoveValue4(QTest &helper) {
    ValueC value1;
    ValueC value2;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    ////////////////////////////////////////////

    value1 = VHArray(1);

    value2 = true;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = false;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = nullptr;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = 33;
    value1 = Memory::Move(value2);
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 33.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = Memory::Move(value2);
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.Equal(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = Memory::Move(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestCopyValue1(QTest &helper) {
    ValueC value1;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray                        h_arr_var;
    const HAItem_T_<ValueC, char> *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = true;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    value1.Reset();
    value1 = true;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    value1.Reset();
    value1 = true;

    // Has values
    h_arr_var = VHArray{};

    for (SizeT32 i = 0; i < 3; i++) {
        VString key;

        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    h_arr_storage = h_arr_var.First();
    value1        = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = false;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    value1.Reset();
    value1 = false;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    value1.Reset();
    value1 = false;

    // Has values
    h_arr_var = VHArray{};

    for (SizeT32 i = 0; i < 3; i++) {
        VString key;

        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    h_arr_storage = h_arr_var.First();
    value1        = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = nullptr;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    value1.Reset();
    value1 = nullptr;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    value1.Reset();
    value1 = nullptr;

    // Has values
    h_arr_var = VHArray{};

    for (SizeT32 i = 0; i < 3; i++) {
        VString key;

        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    h_arr_storage = h_arr_var.First();
    value1        = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = 23;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = 33;

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    value1.Reset();
    value1 = 33;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = 34;

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    value1.Reset();
    value1 = 34;

    // Has values
    h_arr_var = VHArray{};

    for (SizeT32 i = 0; i < 3; i++) {
        VString key;

        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    h_arr_storage = h_arr_var.First();
    value1        = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestCopyValue2(QTest &helper) {
    ValueC value1;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray                        h_arr_var;
    const HAItem_T_<ValueC, char> *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    value1.Reset();
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // Has values
    arr_var = VArray{};
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    value1.Reset();
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // Has values
    h_arr_var = VHArray{};

    for (SizeT32 i = 0; i < 3; i++) {
        VString key;

        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    h_arr_storage = h_arr_var.First();
    value1        = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    value1.Reset();
    value1 = VArray(1);

    // Has values
    h_arr_var = VHArray{};

    for (SizeT32 i = 0; i < 3; i++) {
        VString key;

        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    h_arr_storage = h_arr_var.First();
    value1        = h_arr_var;
    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VHArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    value1.Reset();
    value1 = VHArray(1);

    // Has values
    arr_var = VArray{};
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();
    ////////////////////////////////////////////
}

static void TestCopyValue3(QTest &helper) {
    ValueC value1;
    ValueC value2;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray                        h_arr_var;
    const HAItem_T_<ValueC, char> *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = true;

    value2 = false;
    value1 = value2;
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    value2 = nullptr;
    value1 = value2;
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    value2 = 11;
    value1 = value2;
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 11.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = value2;
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    // No values
    value2 = VArray(1);
    value1 = value2;

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    value1.Reset();
    value1 = true;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = value2;

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    // No values
    value2 = VHArray(1);
    value1 = value2;

    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    value1.Reset();
    value1 = true;

    // Has values
    h_arr_var = VHArray{};

    for (SizeT32 i = 0; i < 3; i++) {
        VString key;

        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = value2;

    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = false;

    value2 = true;
    value1 = value2;
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    value2 = nullptr;
    value1 = value2;
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    value2 = -90;
    value1 = value2;
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), -90.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = value2;
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    // No values
    value2 = VArray(1);
    value1 = value2;

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    value1.Reset();
    value1 = false;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = value2;

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    // No values
    value2 = VHArray(1);
    value1 = value2;

    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    value1.Reset();
    value1 = false;

    // Has values
    h_arr_var = VHArray{};

    for (SizeT32 i = 0; i < 3; i++) {
        VString key;

        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = value2;

    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = nullptr;

    value2 = true;
    value1 = value2;
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    value2 = false;
    value1 = value2;
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    value2 = 7.5;
    value1 = value2;
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 7.5, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = value2;
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    // No values
    value2 = VArray(1);
    value1 = value2;

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    value1.Reset();
    value1 = nullptr;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = value2;

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    // No values
    value2 = VHArray(1);
    value1 = value2;

    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    value1.Reset();
    value1 = nullptr;

    // Has values
    h_arr_var = VHArray{};

    for (SizeT32 i = 0; i < 3; i++) {
        VString key;

        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = value2;

    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = 13;

    value2 = true;
    value1 = value2;
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = 40;

    value2 = false;
    value1 = value2;
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = 33;

    value2 = nullptr;
    value1 = value2;
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = 47;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = value2;
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = 10e10;

    // No values
    value2 = VArray(1);
    value1 = value2;

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    value1.Reset();
    value1 = 10e10;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = value2;

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = 9.1;

    // No values
    value2 = VHArray(1);
    value1 = value2;

    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    value1.Reset();
    value1 = 9.1;

    // Has values
    h_arr_var = VHArray{};

    for (SizeT32 i = 0; i < 3; i++) {
        VString key;

        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = value2;

    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestCopyValue4(QTest &helper) {
    ValueC value1;
    ValueC value2;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray                        h_arr_var;
    const HAItem_T_<ValueC, char> *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = true;
    value1 = value2;
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = false;
    value1 = value2;
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = nullptr;
    value1 = value2;
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = 4;
    value1 = value2;
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 4.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // No values
    value2 = VArray(1);
    value1 = value2;

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    value1.Reset();
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // Has values
    arr_var = VArray{};
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = value2;

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // No values
    value2 = VHArray(1);
    value1 = value2;

    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    value1.Reset();
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // Has values
    h_arr_var = VHArray{};

    for (SizeT32 i = 0; i < 3; i++) {
        VString key;

        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = value2;

    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VArray(1);

    value2 = true;
    value1 = value2;
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = false;
    value1 = value2;
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = nullptr;
    value1 = value2;
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = 33;
    value1 = value2;
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 33.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = value2;
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    // No values
    value2 = VHArray(1);
    value1 = value2;

    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    value1.Reset();
    value1 = VArray(1);

    // Has values
    h_arr_var = VHArray{};

    for (SizeT32 i = 0; i < 3; i++) {
        VString key;

        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = value2;

    helper.EqualsTrue(value1.IsObject(), "IsObject()", __LINE__);
    helper.NotEqual(value1.GetObject(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), nullptr, "GetObject()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetObject()->First(), h_arr_storage, "GetObject()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VHArray(1);

    value2 = true;
    value1 = value2;
    helper.EqualsTrue(value1.IsTrue(), "IsTrue()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = false;
    value1 = value2;
    helper.EqualsTrue(value1.IsFalse(), "IsFalse()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = nullptr;
    value1 = value2;
    helper.EqualsTrue(value1.IsNull(), "IsNull()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = 33;
    value1 = value2;
    helper.EqualsTrue(value1.IsNumber(), "IsNumber()", __LINE__);
    helper.Equal(value1.GetNumber(), 33.0, "GetNumber()", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = value2;
    helper.EqualsTrue(value1.IsString(), "IsString()", __LINE__);
    helper.NotEqual(value1.StringStorage(), c_str_var, "value1.StringStorage()", "c_str_var", __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    // No values
    value2 = VArray(1);
    value1 = value2;

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    value1.Reset();
    value1 = VHArray(1);

    // Has values
    arr_var = VArray{};
    arr_var.ResizeAndInitialize(3);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = value2;

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestIndexOperator1(QTest &helper) {
    ValueC        value;
    const VString str1("D");
    const VString str2("DEFG");

    value = VHArray(1); // Test changing type

    value[0] = 5;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.Equal(value[0].GetNumber(), 5.0, "value[0]", __LINE__);

    value[0] = 20;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.Equal(value[0].GetNumber(), 20.0, "value[0]", __LINE__);

    value[1] = 30;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 2U, "Size()", __LINE__);
    helper.Equal(value[0].GetNumber(), 20.0, "value[0]", __LINE__);
    helper.Equal(value[1].GetNumber(), 30.0, "value[1]", __LINE__);

    value[0]          = 50;
    value[1]          = 100;
    value[2]          = 200;
    value[int{3}]     = 300;
    value[SizeT64{4}] = 400;
    value[SizeT32{5}] = 500;

    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 6U, "Size()", __LINE__);
    helper.Equal(value[0].GetNumber(), 50.0, "value[0]", __LINE__);
    helper.Equal(value[1].GetNumber(), 100.0, "value[1]", __LINE__);
    helper.Equal(value[2].GetNumber(), 200.0, "value[2]", __LINE__);
    helper.Equal(value[3].GetNumber(), 300.0, "value[3]", __LINE__);
    helper.Equal(value[4].GetNumber(), 400.0, "value[4]", __LINE__);
    helper.Equal(value[5].GetNumber(), 500.0, "value[5]", __LINE__);
    helper.NotEqual(value.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.NotEqual(value.GetValue(1), nullptr, "GetValue(1)", "null", __LINE__);
    helper.NotEqual(value.GetValue(2), nullptr, "GetValue(2)", "null", __LINE__);
    helper.NotEqual(value.GetValue(3), nullptr, "GetValue(3)", "null", __LINE__);
    helper.NotEqual(value.GetValue(4), nullptr, "GetValue(4)", "null", __LINE__);
    helper.NotEqual(value.GetValue(5), nullptr, "GetValue(5)", "null", __LINE__);
    helper.NotEqual(value.GetValue("5", 1), nullptr, "GetValue(\"5\")", "null", __LINE__);
    helper.Equal(value.GetValue("50", 2), nullptr, "GetValue(\"50\")", "null", __LINE__);
    helper.Equal(value.GetValue(0)->GetNumber(), 50.0, "GetValue(0)", __LINE__);
    helper.Equal(value.GetValue(1)->GetNumber(), 100.0, "GetValue(1)", __LINE__);
    helper.Equal(value.GetValue(2)->GetNumber(), 200.0, "GetValue(2)", __LINE__);
    helper.Equal(value.GetValue(3)->GetNumber(), 300.0, "GetValue(3)", __LINE__);
    helper.Equal(value.GetValue(4)->GetNumber(), 400.0, "GetValue(4)", __LINE__);
    helper.Equal(value.GetValue(5)->GetNumber(), 500.0, "GetValue(5)", __LINE__);
    helper.Equal(value.GetValue(6), nullptr, "GetValue(6)", "null", __LINE__);

    //////////////////////////////////////////
    value.Reset();

    value["A"] = 7.5;
    helper.EqualsTrue(value.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.Equal(value[0].GetNumber(), 7.5, "value[0]", __LINE__);
    helper.Equal(value["A"].GetNumber(), 7.5, "value[\"A\"]", __LINE__);

    value["A"] = 59;
    helper.EqualsTrue(value.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.Equal(value[0].GetNumber(), 59.0, "value[0]", __LINE__);
    helper.Equal(value["A"].GetNumber(), 59.0, "value[\"A\"]", __LINE__);

    value["B"] = 60;
    helper.EqualsTrue(value.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value.Size(), 2U, "Size()", __LINE__);
    helper.Equal(value[0].GetNumber(), 59.0, "value[0]", __LINE__);
    helper.Equal(value["A"].GetNumber(), 59.0, "value[\"A\"]", __LINE__);
    helper.Equal(value[1].GetNumber(), 60.0, "value[1]", __LINE__);
    helper.Equal(value["B"].GetNumber(), 60.0, "value[\"B\"]", __LINE__);

    value[0]                   = 50;
    value["B"]                 = 100;
    value[VString("C")]        = 200;
    value[str1]                = 300;
    value["EFEFE"]             = 400;
    value[VString("FGHIGKLM")] = 500;

    helper.EqualsTrue(value.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value.Size(), 6U, "Size()", __LINE__);
    helper.Equal(value[0].GetNumber(), 50.0, "value[0]", __LINE__);
    helper.Equal(value[1].GetNumber(), 100.0, "value[1]", __LINE__);
    helper.Equal(value[2].GetNumber(), 200.0, "value[2]", __LINE__);
    helper.Equal(value[3].GetNumber(), 300.0, "value[3]", __LINE__);
    helper.Equal(value[4].GetNumber(), 400.0, "value[4]", __LINE__);
    helper.Equal(value[5].GetNumber(), 500.0, "value[5]", __LINE__);
    helper.Equal(value["A"].GetNumber(), 50.0, "value[\"A\"]", __LINE__);
    helper.Equal(value["B"].GetNumber(), 100.0, "value[\"B\"]", __LINE__);
    helper.Equal(value["C"].GetNumber(), 200.0, "value[\"C\"]", __LINE__);
    helper.Equal(value["D"].GetNumber(), 300.0, "value[\"D\"]", __LINE__);
    helper.Equal(value["EFEFE"].GetNumber(), 400.0, "value[\"EFEFE\"]", __LINE__);
    helper.Equal(value["FGHIGKLM"].GetNumber(), 500.0, "value[\"FGHIGKLM\"]", __LINE__);
    helper.NotEqual(value.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.NotEqual(value.GetValue(1), nullptr, "GetValue(1)", "null", __LINE__);
    helper.NotEqual(value.GetValue(2), nullptr, "GetValue(2)", "null", __LINE__);
    helper.NotEqual(value.GetValue(3), nullptr, "GetValue(3)", "null", __LINE__);
    helper.NotEqual(value.GetValue(4), nullptr, "GetValue(4)", "null", __LINE__);
    helper.NotEqual(value.GetValue(5), nullptr, "GetValue(5)", "null", __LINE__);
    helper.Equal(value.GetValue(0)->GetNumber(), 50.0, "GetValue(0)", __LINE__);
    helper.Equal(value.GetValue(1)->GetNumber(), 100.0, "GetValue(1)", __LINE__);
    helper.Equal(value.GetValue(2)->GetNumber(), 200.0, "GetValue(2)", __LINE__);
    helper.Equal(value.GetValue(3)->GetNumber(), 300.0, "GetValue(3)", __LINE__);
    helper.Equal(value.GetValue(4)->GetNumber(), 400.0, "GetValue(4)", __LINE__);
    helper.Equal(value.GetValue(5)->GetNumber(), 500.0, "GetValue(5)", __LINE__);

    helper.NotEqual(value.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.NotEqual(value.GetKey(1), nullptr, "GetKey(1)", "null", __LINE__);
    helper.NotEqual(value.GetKey(2), nullptr, "GetKey(2)", "null", __LINE__);
    helper.NotEqual(value.GetKey(3), nullptr, "GetKey(3)", "null", __LINE__);
    helper.NotEqual(value.GetKey(4), nullptr, "GetKey(4)", "null", __LINE__);
    helper.NotEqual(value.GetKey(5), nullptr, "GetKey(5)", "null", __LINE__);

    helper.EqualsTrue(value.GetKey(0)->IsEqual("A", 1), "GetKey(0)->IsEqual()", __LINE__);
    helper.EqualsTrue(value.GetKey(1)->IsEqual("B", 1), "GetKey(1)->IsEqual()", __LINE__);
    helper.EqualsTrue(value.GetKey(2)->IsEqual("C", 1), "GetKey(2)->IsEqual()", __LINE__);
    helper.EqualsTrue(value.GetKey(3)->IsEqual("D", 1), "GetKey(3)->IsEqual()", __LINE__);
    helper.EqualsTrue(value.GetKey(4)->IsEqual("EFEFE", 5), "GetKey(4)->IsEqual()", __LINE__);
    helper.EqualsTrue(value.GetKey(5)->IsEqual("FGHIGKLM", 8), "GetKey(5)->IsEqual()", __LINE__);

    helper.Equal(value.GetValue(6), nullptr, "GetValue(6)", "null", __LINE__);

    value.Reset();

    value[VString("C")] = 4;
    helper.EqualsTrue(value.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.Equal(value[0].GetNumber(), 4.0, "value[0]", __LINE__);
    helper.Equal(value[VString("C")].GetNumber(), 4.0, "value[\"C\"]", __LINE__);

    value.Reset();

    value[str2] = 5;
    helper.EqualsTrue(value.IsObject(), "IsObject()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.Equal(value[0].GetNumber(), 5.0, "value[0]", __LINE__);
    helper.Equal(value[str2].GetNumber(), 5.0, "value[\"C\"]", __LINE__);

    //////////////////////////////////////////
}

static void TestIndexOperator2(QTest &helper) {
    ValueC  value;
    ValueC *value_ptr;

    value["A"]    = 10;
    value["BB"]   = 20;
    value["CCCC"] = 30;

    value_ptr = value.GetValue("A", 1);
    helper.NotEqual(value_ptr, nullptr, "GetValue(\"A\", 1)", "null", __LINE__);
    helper.Equal(value_ptr->GetNumber(), 10.0, "GetValue(\"A\", 1)", __LINE__);

    value_ptr = value.GetValue("ABCDEF", 1);
    helper.NotEqual(value_ptr, nullptr, "GetValue(\"ABCDEF\", 1)", "null", __LINE__);
    helper.Equal(value_ptr->GetNumber(), 10.0, "GetValue(\"ABCDEF\", 1)", __LINE__);

    value_ptr = value.GetValue("BB", 2);
    helper.NotEqual(value_ptr, nullptr, "GetValue(\"BB\", 2)", "null", __LINE__);
    helper.Equal(value_ptr->GetNumber(), 20.0, "GetValue(\"BB\", 2)", __LINE__);

    value_ptr = value.GetValue("CCCC", 4);
    helper.NotEqual(value_ptr, nullptr, "GetValue(\"CCCC\", 4)", "null", __LINE__);
    helper.Equal(value_ptr->GetNumber(), 30.0, "GetValue(\"CCCC\", 4)", __LINE__);
    ////////////////////
    helper.Equal(value.GetValue("a", 1), nullptr, "GetValue(\"a\", 1)", "null", __LINE__);
    helper.Equal(value.GetValue("ABCDEF", 6), nullptr, "GetValue(\"ABCDEF\", 1)", "null", __LINE__);
    helper.Equal(value.GetValue("b", 1), nullptr, "GetValue(\"b\", 1)", "null", __LINE__);
    helper.Equal(value.GetValue("bb", 2), nullptr, "GetValue(\"bb\", 2)", "null", __LINE__);

    helper.Equal(value.GetValue("CCC", 3), nullptr, "GetValue(\"CCCC\", 3)", "null", __LINE__);
    ////////////////////
}

static void TestAddition1(QTest &helper) {
    ValueC        value;
    const VString str1("D");
    const VString str2("DEFG");

    value += 20;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.Equal(value[0].GetNumber(), 20.0, "value[0]", __LINE__);

    value += 30;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 2U, "Size()", __LINE__);
    helper.Equal(value[0].GetNumber(), 20.0, "value[0]", __LINE__);
    helper.Equal(value[1].GetNumber(), 30.0, "value[1]", __LINE__);

    value[0] = 50;
    value[1] = 100;
    value += int{200};
    value += SizeT32{300};
    value += SizeT64{400};
    value += double{500};

    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 6U, "Size()", __LINE__);
    helper.Equal(value[0].GetNumber(), 50.0, "value[0]", __LINE__);
    helper.Equal(value[1].GetNumber(), 100.0, "value[1]", __LINE__);
    helper.Equal(value[2].GetNumber(), 200.0, "value[2]", __LINE__);
    helper.Equal(value[3].GetNumber(), 300.0, "value[3]", __LINE__);
    helper.Equal(value[4].GetNumber(), 400.0, "value[4]", __LINE__);
    helper.Equal(value[5].GetNumber(), 500.0, "value[5]", __LINE__);

    value.Reset();

    value[10] = 100;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 11U, "Size()", __LINE__);
    helper.EqualsTrue(value[3].IsUndefined(), "value[3]", __LINE__);
    helper.EqualsTrue(value[4].IsUndefined(), "value[4]", __LINE__);
    helper.EqualsTrue(value[5].IsUndefined(), "value[5]", __LINE__);
    helper.Equal(value[10].GetNumber(), 100.0, "value[10]", __LINE__);
    //////////////////////////////////////////
}

static void TestAddition2(QTest &helper) {
    ValueC  value;
    VString str;

    /////////////////

    value += true;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsTrue(), "value[0].IsTrue()", __LINE__);

    value += true;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsTrue(), "value[0].IsTrue()", __LINE__);
    helper.EqualsTrue(value[1].IsTrue(), "value[1].IsTrue()", __LINE__);

    value.Reset();
    /////////////////

    value += false;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsFalse(), "value[0].IsFalse()", __LINE__);

    value += false;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsFalse(), "value[0].IsFalse()", __LINE__);
    helper.EqualsTrue(value[1].IsFalse(), "value[1].IsFalse()", __LINE__);

    value.Reset();
    /////////////////

    value += nullptr;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsNull(), "value[0].IsNull()", __LINE__);

    value += nullptr;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsNull(), "value[0].IsNull()", __LINE__);
    helper.EqualsTrue(value[1].IsNull(), "value[1].IsNull()", __LINE__);

    value.Reset();
    /////////////////

    value += "-ABCDEF0123456789ABCDEF0123456789-";
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsString(), "value[0].IsString()", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);

    value += "^ABCDEF0123456789ABCDEF0123456789^";
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsString(), "value[0].IsString()", __LINE__);
    helper.EqualsTrue(value[1].IsString(), "value[1].IsString()", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[1].StringStorage(), "^ABCDEF0123456789ABCDEF0123456789^", 34),
                      "StringUtils::IsEqual", __LINE__);
    value.Reset();
    /////////////////
    str               = VString("-ABCDEF0123456789ABCDEF0123456789-");
    const char *c_str = str.First();
    value += Memory::Move(str);
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsString(), "value[0].IsString()", __LINE__);
    helper.Equal(value[0].StringStorage(), c_str, "value[0].StringStorage()", "c_str", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);

    str                = VString("#0123456789ABCDEF0123456789ABCDEF#");
    const char *c_str2 = str.First();
    value += Memory::Move(str);
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsString(), "value[0].IsString()", __LINE__);
    helper.EqualsTrue(value[1].IsString(), "value[1].IsString()", __LINE__);
    helper.Equal(value[0].StringStorage(), c_str, "value[0].StringStorage()", "c_str", __LINE__);
    helper.Equal(value[1].StringStorage(), c_str2, "value[1].StringStorage()", "c_str2", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[1].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34),
                      "StringUtils::IsEqual", __LINE__);
    value.Reset();
    /////////////////

    str   = VString("-ABCDEF0123456789ABCDEF0123456789-");
    c_str = str.First();
    value += str;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsString(), "value[0].IsString()", __LINE__);
    helper.NotEqual(value[0].StringStorage(), c_str, "value[0].StringStorage()", "c_str", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);

    str    = VString("#0123456789ABCDEF0123456789ABCDEF#");
    c_str2 = str.First();
    value += str;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsString(), "value[0].IsString()", __LINE__);
    helper.EqualsTrue(value[1].IsString(), "value[1].IsString()", __LINE__);
    helper.NotEqual(value[0].StringStorage(), c_str, "value[0].StringStorage()", "c_str", __LINE__);
    helper.NotEqual(value[1].StringStorage(), c_str2, "value[1].StringStorage()", "c_str2", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[1].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34),
                      "StringUtils::IsEqual", __LINE__);
    value.Reset();

    //////////////

    value += VHArray{};
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsObject(), "value[0].IsObject()", __LINE__);

    value += VHArray{};
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value[0].IsObject(), "value[0].IsObject()", __LINE__);
    helper.EqualsTrue(value[1].IsObject(), "value[1].IsObject()", __LINE__);

    //////////////////////////////////////////
}

static void TestAddition3(QTest &helper) {
    ValueC        value;
    VArray        arr_var;
    const ValueC *arr_storage;
    const char   *c_str;
    const char   *c_str2;
    /////////////////
    arr_var = VArray(1);
    value += arr_var;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.NotEqual(value.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);

    value.Reset();

    arr_var = VArray(3);
    value += arr_var;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.NotEqual(value.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);

    value.Reset();
    ///

    arr_var += ValueC{false};
    arr_var += ValueC{true};
    arr_var += ValueC{VString("-ABCDEF0123456789ABCDEF0123456789-")};
    arr_storage = arr_var.First();
    c_str       = arr_storage[2].StringStorage();

    value += arr_var;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 3U, "Size()", __LINE__);
    helper.NotEqual(value.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    helper.EqualsTrue(value[0].IsFalse(), "value[0].IsFalse()", __LINE__);
    helper.EqualsTrue(value[1].IsTrue(), "value[1].IsTrue()", __LINE__);
    helper.EqualsTrue(value[2].IsString(), "value[2].IsString()", __LINE__);
    helper.NotEqual(value[2].StringStorage(), c_str, "value[2].StringStorage()", "c_str", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[2].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);

    arr_var.Reset();
    arr_var += ValueC{nullptr};
    arr_var += ValueC{14};
    arr_var += ValueC{VString("^ABCDEF0123456789ABCDEF0123456789^")};
    arr_storage = arr_var.First();
    c_str2      = arr_storage[2].StringStorage();

    value += arr_var;
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 6U, "Size()", __LINE__);
    helper.NotEqual(value.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    helper.EqualsTrue(value[0].IsFalse(), "value[0].IsFalse()", __LINE__);
    helper.EqualsTrue(value[1].IsTrue(), "value[1].IsTrue()", __LINE__);
    helper.EqualsTrue(value[2].IsString(), "value[2].IsString()", __LINE__);
    helper.NotEqual(value[2].StringStorage(), c_str, "value[2].StringStorage()", "c_str", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[2].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);
    helper.EqualsTrue(value[3].IsNull(), "value[3].IsNull()", __LINE__);
    helper.EqualsTrue(value[4].IsNumber(), "value1[4].IsNumber()", __LINE__);
    helper.Equal(value[4].GetNumber(), 14.0, "value[4].GetNumber()", __LINE__);
    helper.EqualsTrue(value[5].IsString(), "value[5].IsString()", __LINE__);
    helper.NotEqual(value[5].StringStorage(), c_str2, "value[5].StringStorage()", "c_str2", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[5].StringStorage(), "^ABCDEF0123456789ABCDEF0123456789^", 34),
                      "StringUtils::IsEqual", __LINE__);

    value.Reset();
    /////////////////
    value += VArray(1);
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.NotEqual(value.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);

    value.Reset();

    value += VArray(3);
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 1U, "Size()", __LINE__);
    helper.NotEqual(value.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);

    value.Reset();
    arr_var.Reset();

    arr_var += ValueC{ValueType::False};
    arr_var += ValueC{ValueType::True};
    arr_var += ValueC{VString("#0123456789ABCDEF0123456789ABCDEF#")};
    arr_storage = arr_var.First();
    c_str       = arr_storage[2].StringStorage();

    value += Memory::Move(arr_var);
    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 3U, "Size()", __LINE__);
    helper.NotEqual(value.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.Equal(value.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    helper.EqualsTrue(value[0].IsFalse(), "value[0].IsFalse()", __LINE__);
    helper.EqualsTrue(value[1].IsTrue(), "value[1].IsTrue()", __LINE__);
    helper.EqualsTrue(value[2].IsString(), "value[2].IsString()", __LINE__);
    helper.Equal(value[2].StringStorage(), c_str, "value[2].StringStorage()", "c_str", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[2].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34),
                      "StringUtils::IsEqual", __LINE__);

    arr_var += ValueC{nullptr};
    arr_var += ValueC{14};
    arr_var += ValueC{VString("-ABCDEF0123456789ABCDEF0123456789-")};
    arr_storage = arr_var.First();
    c_str2      = arr_storage[2].StringStorage();

    value += Memory::Move(arr_var);
    helper.Equal(arr_var.First(), nullptr, "arr_var.First()", "null", __LINE__);

    helper.EqualsTrue(value.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value.Size(), 6U, "Size()", __LINE__);
    helper.NotEqual(value.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    helper.EqualsTrue(value[0].IsFalse(), "value[0].IsFalse()", __LINE__);
    helper.EqualsTrue(value[1].IsTrue(), "value[1].IsTrue()", __LINE__);
    helper.EqualsTrue(value[2].IsString(), "value[2].IsString()", __LINE__);
    helper.Equal(value[2].StringStorage(), c_str, "value[2].StringStorage()", "c_str", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[2].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34),
                      "StringUtils::IsEqual", __LINE__);
    helper.EqualsTrue(value[3].IsNull(), "value[3].IsNull()", __LINE__);
    helper.EqualsTrue(value[4].IsNumber(), "value1[4].IsNumber()", __LINE__);
    helper.Equal(value[4].GetNumber(), 14.0, "value[4].GetNumber()", __LINE__);
    helper.EqualsTrue(value[5].IsString(), "value[5].IsString()", __LINE__);
    helper.Equal(value[5].StringStorage(), c_str2, "value[5].StringStorage()", "c_str2", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value[5].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);
    //////////////////////////////////////////
}

static void TestAddition4(QTest &helper) {
    ValueC        value1;
    ValueC        value2;
    VArray        arr_var;
    const ValueC *arr_storage;
    VString       str;

    /////////////////

    value2 = true;
    value1 += value2;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsTrue(), "value1[0].IsTrue()", __LINE__);

    value2 = true;
    value1 += value2;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsTrue(), "value1[0].IsTrue()", __LINE__);
    helper.EqualsTrue(value1[1].IsTrue(), "value1[1].IsTrue()", __LINE__);

    value1.Reset();
    /////////////////

    value2 = false;
    value1 += value2;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsFalse(), "value1[0].IsFalse()", __LINE__);

    value2 = false;
    value1 += value2;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsFalse(), "value1[0].IsFalse()", __LINE__);
    helper.EqualsTrue(value1[1].IsFalse(), "value1[1].IsFalse()", __LINE__);

    value1.Reset();
    /////////////////

    value2 = nullptr;
    value1 += value2;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsNull(), "value1[0].IsNull()", __LINE__);

    value2 = nullptr;
    value1 += value2;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsNull(), "value1[0].IsNull()", __LINE__);
    helper.EqualsTrue(value1[1].IsNull(), "value1[1].IsNull()", __LINE__);

    value1.Reset();
    /////////////////
    str               = VString("-ABCDEF0123456789ABCDEF0123456789-");
    const char *c_str = str.First();
    value2            = Memory::Move(str);

    value1 += value2;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsString(), "value1[0].IsString()", __LINE__);
    helper.NotEqual(value1[0].StringStorage(), c_str, "value1[0].StringStorage()", "c_str", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);

    str                = VString("#0123456789ABCDEF0123456789ABCDEF#");
    const char *c_str2 = str.First();
    value2             = Memory::Move(str);

    value1 += value2;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsString(), "value1[0].IsString()", __LINE__);
    helper.EqualsTrue(value1[1].IsString(), "value1[1].IsString()", __LINE__);
    helper.NotEqual(value1[0].StringStorage(), c_str, "value1[0].StringStorage()", "c_str", __LINE__);
    helper.NotEqual(value1[1].StringStorage(), c_str2, "value1[1].StringStorage()", "c_str2", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1[1].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34),
                      "StringUtils::IsEqual", __LINE__);
    value1.Reset();
    /////////////////

    value2 = VArray(1);
    value1 += value2;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 1U, "Size()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);

    value1.Reset();

    value2 = VArray(10);
    value1 += value2;
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 1U, "Size()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);

    value1.Reset();

    value2 = VArray(1);
    value1.Merge(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 0U, "Size()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);

    value2 = VArray(10);
    value1.Merge(Memory::Move(value2));
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 0U, "Size()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);

    value1.Reset();
    ///

    arr_var += ValueC{ValueType::False};
    arr_var += ValueC{ValueType::True};
    arr_var += ValueC{VString("-ABCDEF0123456789ABCDEF0123456789-")};
    arr_storage = arr_var.First();
    c_str       = arr_storage[2].StringStorage();
    value2      = Memory::Move(arr_var);

    value1.Merge(Memory::Move(value2));
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 3U, "Size()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    helper.EqualsTrue(value1[0].IsFalse(), "value1[0].IsFalse()", __LINE__);
    helper.EqualsTrue(value1[1].IsTrue(), "value1[1].IsTrue()", __LINE__);
    helper.EqualsTrue(value1[2].IsString(), "value1[2].IsString()", __LINE__);
    helper.Equal(value1[2].StringStorage(), c_str, "value1[2].StringStorage()", "c_str", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1[2].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);

    arr_var.Reset();
    arr_var += ValueC{ValueType::Null};
    arr_var += ValueC{14};
    arr_var += ValueC{VString("^ABCDEF0123456789ABCDEF0123456789^")};
    arr_storage = arr_var.First();
    c_str2      = arr_storage[2].StringStorage();
    value2      = Memory::Move(arr_var);

    value1.Merge(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 6U, "Size()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    helper.EqualsTrue(value1[0].IsFalse(), "value1[0].IsFalse()", __LINE__);
    helper.EqualsTrue(value1[1].IsTrue(), "value1[1].IsTrue()", __LINE__);
    helper.EqualsTrue(value1[2].IsString(), "value1[2].IsString()", __LINE__);
    helper.Equal(value1[2].StringStorage(), c_str, "value1[0].StringStorage()", "c_str", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1[2].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);
    helper.EqualsTrue(value1[3].IsNull(), "value1[3].IsNull()", __LINE__);
    helper.EqualsTrue(value1[4].IsNumber(), "value1[4].IsNumber()", __LINE__);
    helper.Equal(value1[4].GetNumber(), 14.0, "value1[4].GetNumber()", __LINE__);
    helper.EqualsTrue(value1[5].IsString(), "value1[5].IsString()", __LINE__);
    helper.NotEqual(value1[5].StringStorage(), c_str2, "value1[5].StringStorage()", "c_str2", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1[5].StringStorage(), "^ABCDEF0123456789ABCDEF0123456789^", 34),
                      "StringUtils::IsEqual", __LINE__);

    value1.Reset();

    //////////////////////////////////////////
}

static void TestAddition5(QTest &helper) {
    ValueC        value1;
    ValueC        value2;
    VArray        arr_var;
    const ValueC *arr_storage;
    VString       str;

    /////////////////

    value2 = true;
    helper.EqualsTrue(value2.IsTrue(), "value2.IsTrue()", __LINE__);

    value1 += Memory::Move(value2);

    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsTrue(), "value1[0].IsTrue()", __LINE__);

    value2 += true;
    value2 += false;
    value2 += nullptr;
    value2 += "v";
    value2 += 4;
    value2 += VArray(1);
    value2 += VHArray(1);

    value2 = true;
    value1 += Memory::Move(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsTrue(), "value1[0].IsTrue()", __LINE__);
    helper.EqualsTrue(value1[1].IsTrue(), "value1[1].IsTrue()", __LINE__);

    value1.Reset();
    /////////////////

    value2 = false;

    value1 += Memory::Move(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsFalse(), "value1[0].IsFalse()", __LINE__);

    value2 = false;
    value1 += Memory::Move(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsFalse(), "value1[0].IsFalse()", __LINE__);
    helper.EqualsTrue(value1[1].IsFalse(), "value1[1].IsFalse()", __LINE__);

    value1.Reset();
    /////////////////

    value2 = nullptr;
    value1 += Memory::Move(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsNull(), "value1[0].IsNull()", __LINE__);

    value2 = nullptr;
    value1 += Memory::Move(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsNull(), "value1[0].IsNull()", __LINE__);
    helper.EqualsTrue(value1[1].IsNull(), "value1[1].IsNull()", __LINE__);

    value1.Reset();
    /////////////////
    str               = VString("-ABCDEF0123456789ABCDEF0123456789-");
    const char *c_str = str.First();
    value2            = Memory::Move(str);

    value1 += Memory::Move(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 1U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsString(), "value1[0].IsString()", __LINE__);
    helper.Equal(value1[0].StringStorage(), c_str, "value1[0].StringStorage()", "c_str", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);

    str                = VString("#0123456789ABCDEF0123456789ABCDEF#");
    const char *c_str2 = str.First();
    value2             = Memory::Move(str);

    value1 += Memory::Move(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 2U, "Size()", __LINE__);
    helper.EqualsTrue(value1[0].IsString(), "value1[0].IsString()", __LINE__);
    helper.EqualsTrue(value1[1].IsString(), "value1[1].IsString()", __LINE__);
    helper.Equal(value1[0].StringStorage(), c_str, "value1[0].StringStorage()", "c_str", __LINE__);
    helper.Equal(value1[1].StringStorage(), c_str2, "value1[1].StringStorage()", "c_str2", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1[1].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34),
                      "StringUtils::IsEqual", __LINE__);
    value1.Reset();
    /////////////////

    value2 = VArray(1);
    value1.Merge(value2);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 0U, "Size()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value2.GetArray(), nullptr, "GetArray()", "null", __LINE__);

    value2 = VArray(1);
    value1.Merge(Memory::Move(value2));
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 0U, "Size()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.Equal(value2.GetArray(), nullptr, "GetArray()", "null", __LINE__);

    value1.Reset();
    ///

    arr_var += ValueC{ValueType::False};
    arr_var += ValueC{ValueType::True};
    arr_var += ValueC{VString("-ABCDEF0123456789ABCDEF0123456789-")};
    arr_storage = arr_var.First();
    c_str       = arr_storage[2].StringStorage();
    value2      = Memory::Move(arr_var);

    value1.Merge(Memory::Move(value2));
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 3U, "Size()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    helper.EqualsTrue(value1[0].IsFalse(), "value1[0].IsFalse()", __LINE__);
    helper.EqualsTrue(value1[1].IsTrue(), "value1[1].IsTrue()", __LINE__);
    helper.EqualsTrue(value1[2].IsString(), "value1[2].IsString()", __LINE__);
    helper.Equal(value1[2].StringStorage(), c_str, "value1[0].StringStorage()", "c_str", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1[2].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);

    arr_var.Reset();
    arr_var += ValueC{ValueType::Null};
    arr_var += ValueC{14};
    arr_var += ValueC{VString("#0123456789ABCDEF0123456789ABCDEF#")};
    arr_storage = arr_var.First();
    c_str2      = arr_storage[2].StringStorage();
    value2      = Memory::Move(arr_var);

    value1.Merge(Memory::Move(value2));
    helper.EqualsTrue(value2.IsUndefined(), "value2.IsUndefined()", __LINE__);
    helper.EqualsTrue(value1.IsArray(), "IsArray()", __LINE__);
    helper.Equal(value1.Size(), 6U, "Size()", __LINE__);
    helper.NotEqual(value1.GetArray(), nullptr, "GetArray()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), nullptr, "GetArray()->First()", "null", __LINE__);
    helper.NotEqual(value1.GetArray()->First(), arr_storage, "GetArray()->First()", "storage", __LINE__);
    helper.EqualsTrue(value1[0].IsFalse(), "value1[0].IsFalse()", __LINE__);
    helper.EqualsTrue(value1[1].IsTrue(), "value1[1].IsTrue()", __LINE__);
    helper.EqualsTrue(value1[2].IsString(), "value1[2].IsString()", __LINE__);
    helper.Equal(value1[2].StringStorage(), c_str, "value1[0].StringStorage()", "c_str", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1[2].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34),
                      "StringUtils::IsEqual", __LINE__);
    helper.EqualsTrue(value1[3].IsNull(), "value1[3].IsNull()", __LINE__);
    helper.EqualsTrue(value1[4].IsNumber(), "value1[4].IsNumber()", __LINE__);
    helper.Equal(value1[4].GetNumber(), 14.0, "value1[3].GetNumber()", __LINE__);
    helper.EqualsTrue(value1[5].IsString(), "value1[0].IsString()", __LINE__);
    helper.Equal(value1[5].StringStorage(), c_str2, "value1[1].StringStorage()", "c_str2", __LINE__);
    helper.EqualsTrue(StringUtils::IsEqual(value1[5].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34),
                      "StringUtils::IsEqual", __LINE__);
    value1.Reset();
    //////////////////////////////////////////
}

static void TestAddition6(QTest &helper) {
    ValueC                         value1;
    ValueC                         value2;
    VString                        str_var;
    const char                    *str_c1;
    const char                    *str_c2;
    VHArray                        h_arr_var;
    const HAItem_T_<ValueC, char> *h_arr_storage1 = nullptr;
    const HAItem_T_<ValueC, char> *h_arr_storage2 = nullptr;

    value1["k1"] = 11;
    value1["k2"] = 22;
    str_var      = "*ABCDEF0123456789ABCDEF0123456789*";
    str_c1       = str_var.First();
    value1["k3"] = Memory::Move(str_var);

    if (value1.GetObject() != nullptr) {
        h_arr_storage1 = value1.GetObject()->First();
    }

    h_arr_var[VString("w1")] = 10;
    h_arr_var[VString("w2")] = 20;
    h_arr_var[VString("w3")] = 30;
    str_var                  = "-ABCDEF0123456789ABCDEF0123456789-";
    str_c2                   = str_var.First();
    h_arr_var[VString("w4")] = Memory::Move(str_var);
    h_arr_storage2           = h_arr_var.First();

    value2 = VHArray{};   // Setting to object type.
    value2.Merge(value1); // Copy
    helper.Equal(value2.Size(), 3U, "value2.Size()", __LINE__);
    helper.NotEqual(value2.GetObject(), nullptr, "GetObject()", "null", __LINE__);
    helper.NotEqual(value2.GetObject()->First(), h_arr_storage1, "GetArray()->First()", "storage", __LINE__);
    helper.Equal(value2["k1"].GetNumber(), 11.0, "value2[\"k1\"].GetNumber()", __LINE__);
    helper.Equal(value2["k2"].GetNumber(), 22.0, "value2[\"k2\"].GetNumber()", __LINE__);
    helper.NotEqual(value2["k3"].StringStorage(), str_c1, "value2[\"k3\"].StringStorage()", "str_c", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2["k3"].StringStorage(), "*ABCDEF0123456789ABCDEF0123456789*", 34), true,
                 "value2[\"k3\"].StringStorage()", "IsEqual", __LINE__);

    value2 = VHArray{};  // Clearing and  Setting to object type.
    value2 += h_arr_var; // Copy
    helper.Equal(value2.Size(), 4U, "value2.Size()", __LINE__);
    helper.NotEqual(value2.GetObject(), nullptr, "GetObject()", "null", __LINE__);
    helper.NotEqual(value2.GetObject()->First(), h_arr_storage2, "GetArray()->First()", "storage", __LINE__);
    helper.Equal(value2["w1"].GetNumber(), 10.0, "[\"w1\"].GetNumber()", __LINE__);
    helper.Equal(value2["w2"].GetNumber(), 20.0, "[\"w2\"].GetNumber()", __LINE__);
    helper.Equal(value2["w3"].GetNumber(), 30.0, "[\"w3\"].GetNumber()", __LINE__);
    helper.NotEqual(value2["w4"].StringStorage(), str_c2, "value2[\"w4\"].StringStorage()", "str_c", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2["w4"].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), true,
                 "value2[\"w4\"].StringStorage()", "IsEqual", __LINE__);
    helper.Equal(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null", __LINE__);

    ////

    value2 = VHArray{};                 // Clearing and  Setting to object type.
    value2.Merge(Memory::Move(value1)); // Move
    helper.EqualsTrue(value1.IsUndefined(), "value1.IsUndefined()", __LINE__);
    helper.Equal(value2.Size(), 3U, "value2.Size()", __LINE__);
    helper.NotEqual(value2.GetObject(), nullptr, "GetObject()", "null", __LINE__);
    helper.NotEqual(value2.GetObject()->First(), h_arr_storage2, "GetArray()->First()", "storage", __LINE__);
    helper.Equal(value2["k1"].GetNumber(), 11.0, "value2[\"k1\"].GetNumber()", __LINE__);
    helper.Equal(value2["k2"].GetNumber(), 22.0, "value2[\"k2\"].GetNumber()", __LINE__);
    helper.Equal(value2["k3"].StringStorage(), str_c1, "value2[\"k3\"].StringStorage()", "str_c1", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2["k3"].StringStorage(), "*ABCDEF0123456789ABCDEF0123456789*", 34), true,
                 "value2[\"k3\"].StringStorage()", "IsEqual", __LINE__);

    value1 = value2; // Copying back the values.
    str_c1 = value1["k3"].StringStorage();

    value2 = VHArray{};                // Clearing and  Setting to object type.
    value2 += Memory::Move(h_arr_var); // Move
    helper.Equal(h_arr_var.First(), nullptr, "h_arr_var.First()", "null", __LINE__);
    helper.Equal(value2.Size(), 4U, "value2.Size()", __LINE__);
    helper.NotEqual(value2.GetObject(), nullptr, "GetObject()", "null", __LINE__);
    helper.NotEqual(value2.GetObject()->First(), h_arr_storage2, "GetArray()->First()", "storage", __LINE__);
    helper.Equal(value2["w1"].GetNumber(), 10.0, "[\"w1\"].GetNumber()", __LINE__);
    helper.Equal(value2["w2"].GetNumber(), 20.0, "[\"w2\"].GetNumber()", __LINE__);
    helper.Equal(value2["w3"].GetNumber(), 30.0, "[\"w3\"].GetNumber()", __LINE__);
    helper.Equal(value2["w4"].StringStorage(), str_c2, "value2[\"w4\"].StringStorage()", "str_c2", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2["w4"].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), true,
                 "value2[\"w4\"].StringStorage()", "IsEqual", __LINE__);
    helper.Equal(value2.GetValue(4), nullptr, "value2.GetValue(4)", "null", __LINE__);

    if (value2.GetObject() != nullptr) {
        h_arr_var = *(value2.GetObject()); // Copying back the values.
        str_c2    = h_arr_var[VString("w4")].StringStorage();
    }

    ////////
    value2 = VHArray{};   // Clearing and  Setting to object type.
    value2.Merge(value1); // Copy
    value2 += h_arr_var;  // Copy
    helper.Equal(value2.Size(), 7U, "value2.Size()", __LINE__);
    helper.Equal(value2["w1"].GetNumber(), 10.0, "[\"w1\"].GetNumber()", __LINE__);
    helper.Equal(value2["w2"].GetNumber(), 20.0, "[\"w2\"].GetNumber()", __LINE__);
    helper.Equal(value2["w3"].GetNumber(), 30.0, "[\"w3\"].GetNumber()", __LINE__);
    helper.NotEqual(value2["w4"].StringStorage(), str_c2, "value2[\"w4\"].StringStorage()", "str_c2", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2["w4"].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), true,
                 "value2[\"w4\"].StringStorage()", "IsEqual", __LINE__);
    helper.Equal(value2["k1"].GetNumber(), 11.0, "value2[\"k1\"].GetNumber()", __LINE__);
    helper.Equal(value2["k2"].GetNumber(), 22.0, "value2[\"k2\"].GetNumber()", __LINE__);
    helper.NotEqual(value2["k3"].StringStorage(), str_c1, "value2[\"k3\"].StringStorage()", "str_c1", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2["k3"].StringStorage(), "*ABCDEF0123456789ABCDEF0123456789*", 34), true,
                 "value2[\"k3\"].StringStorage()", "IsEqual", __LINE__);

    ////
    value2       = VHArray{}; // Clearing and  Setting to object type.
    value2["w0"] = 5;
    value2["w1"] = 100;
    value2["w2"] = 200;
    value2["w4"] = 400;
    value2["w5"] = 500;
    value2["w6"] = 600;
    value2 += h_arr_var;  // Copy
    value2.Merge(value1); // Copy

    helper.Equal(value2["w0"].GetNumber(), 5.0, "[\"w0\"].GetNumber()", __LINE__);
    helper.Equal(value2["w1"].GetNumber(), 10.0, "[\"w1\"].GetNumber()", __LINE__);
    helper.Equal(value2["w2"].GetNumber(), 20.0, "[\"w2\"].GetNumber()", __LINE__);
    helper.Equal(value2["w3"].GetNumber(), 30.0, "[\"w3\"].GetNumber()", __LINE__);
    helper.NotEqual(value2["w4"].StringStorage(), str_c2, "value2[\"w4\"].StringStorage()", "str_c2", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2["w4"].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), true,
                 "value2[\"w4\"].StringStorage()", "IsEqual", __LINE__);
    helper.Equal(value2["w5"].GetNumber(), 500.0, "[\"w5\"].GetNumber()", __LINE__);
    helper.Equal(value2["w6"].GetNumber(), 600.0, "[\"w6\"].GetNumber()", __LINE__);
    helper.Equal(value2["k1"].GetNumber(), 11.0, "value2[\"k1\"].GetNumber()", __LINE__);
    helper.Equal(value2["k2"].GetNumber(), 22.0, "value2[\"k2\"].GetNumber()", __LINE__);
    helper.NotEqual(value2["k3"].StringStorage(), str_c1, "value2[\"k3\"].StringStorage()", "str_c1", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2["k3"].StringStorage(), "*ABCDEF0123456789ABCDEF0123456789*", 34), true,
                 "value2[\"k3\"].StringStorage()", "IsEqual", __LINE__);

    value2       = VHArray{}; // Clearing and  Setting to object type.
    value2["w0"] = 5;
    value2["w1"] = 100;
    value2["w2"] = 200;
    value2["w4"] = 400;
    value2["w5"] = 500;
    value2["w6"] = 600;
    value2.Merge(Memory::Move(value1)); // Move
    value2 += Memory::Move(h_arr_var);  // Move
    helper.Equal(value2["w0"].GetNumber(), 5.0, "[\"w0\"].GetNumber()", __LINE__);
    helper.Equal(value2["w1"].GetNumber(), 10.0, "[\"w1\"].GetNumber()", __LINE__);
    helper.Equal(value2["w2"].GetNumber(), 20.0, "[\"w2\"].GetNumber()", __LINE__);
    helper.Equal(value2["w3"].GetNumber(), 30.0, "[\"w3\"].GetNumber()", __LINE__);
    helper.Equal(value2["w4"].StringStorage(), str_c2, "value2[\"w4\"].StringStorage()", "str_c2", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2["w4"].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), true,
                 "value2[\"w4\"].StringStorage()", "IsEqual", __LINE__);
    helper.Equal(value2["w5"].GetNumber(), 500.0, "[\"w5\"].GetNumber()", __LINE__);
    helper.Equal(value2["w6"].GetNumber(), 600.0, "[\"w6\"].GetNumber()", __LINE__);
    helper.Equal(value2["k1"].GetNumber(), 11.0, "value2[\"k1\"].GetNumber()", __LINE__);
    helper.Equal(value2["k2"].GetNumber(), 22.0, "value2[\"k2\"].GetNumber()", __LINE__);
    helper.Equal(value2["k3"].StringStorage(), str_c1, "value2[\"k3\"].StringStorage()", "str_c1", __LINE__);
    helper.Equal(StringUtils::IsEqual(value2["k3"].StringStorage(), "*ABCDEF0123456789ABCDEF0123456789*", 34), true,
                 "value2[\"k3\"].StringStorage()", "IsEqual", __LINE__);
    /////////////////////////////
}

static void TestStringify1(QTest &helper) {
    StringStream<char> ss;
    ValueC             value;

    ///////////
    value = VArray{};
    helper.Equal(value.Stringify(ss), "[]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += true;
    helper.Equal(value.Stringify(ss), "[true]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    helper.Equal(value.Stringify(ss), "[false]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    helper.Equal(value.Stringify(ss), "[null]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += 0;
    helper.Equal(value.Stringify(ss), "[0]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += "a";
    helper.Equal(value.Stringify(ss), R"(["a"])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    helper.Equal(value.Stringify(ss), R"(["ABC"])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    helper.Equal(value.Stringify(ss), "[[]]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    helper.Equal(value.Stringify(ss), "[{}]", "Stringify()", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += true;
    helper.Equal(value.Stringify(ss), "[true,true]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += true;
    helper.Equal(value.Stringify(ss), "[false,true]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += true;
    helper.Equal(value.Stringify(ss), "[null,true]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += -3;
    value += true;
    helper.Equal(value.Stringify(ss), "[-3,true]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += "a";
    value += true;
    helper.Equal(value.Stringify(ss), R"(["a",true])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += true;
    helper.Equal(value.Stringify(ss), R"(["ABC",true])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += true;
    helper.Equal(value.Stringify(ss), "[[],true]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += true;
    helper.Equal(value.Stringify(ss), "[{},true]", "Stringify()", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += false;
    helper.Equal(value.Stringify(ss), "[true,false]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += false;
    helper.Equal(value.Stringify(ss), "[false,false]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += false;
    helper.Equal(value.Stringify(ss), "[null,false]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += 500;
    value += false;
    helper.Equal(value.Stringify(ss), "[500,false]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VString("a");
    value += false;
    helper.Equal(value.Stringify(ss), R"(["a",false])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    const VString str("ABC");
    value += str;
    value += false;
    helper.Equal(value.Stringify(ss), R"(["ABC",false])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += false;
    helper.Equal(value.Stringify(ss), "[[],false]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += false;
    helper.Equal(value.Stringify(ss), "[{},false]", "Stringify()", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += nullptr;
    helper.Equal(value.Stringify(ss), "[true,null]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += nullptr;
    helper.Equal(value.Stringify(ss), "[false,null]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += nullptr;
    helper.Equal(value.Stringify(ss), "[null,null]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += 456.5;
    value += nullptr;
    helper.Equal(value.Stringify(ss), "[456.5,null]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += "a";
    value += nullptr;
    helper.Equal(value.Stringify(ss), R"(["a",null])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += nullptr;
    helper.Equal(value.Stringify(ss), R"(["ABC",null])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += nullptr;
    helper.Equal(value.Stringify(ss), "[[],null]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += nullptr;
    helper.Equal(value.Stringify(ss), "[{},null]", "Stringify()", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += "A";
    helper.Equal(value.Stringify(ss), R"([true,"A"])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += "AB";
    helper.Equal(value.Stringify(ss), R"([false,"AB"])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += "ABC";
    helper.Equal(value.Stringify(ss), R"([null,"ABC"])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += -8.9;
    value += "ABC";
    helper.Equal(value.Stringify(ss, 2U), R"([-8.9,"ABC"])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += "a";
    value += "ABCD";
    helper.Equal(value.Stringify(ss), R"(["a","ABCD"])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += "ABCDE";
    helper.Equal(value.Stringify(ss), R"(["ABC","ABCDE"])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += "ABCDEF";
    helper.Equal(value.Stringify(ss), R"([[],"ABCDEF"])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += "ABCDEFG";
    helper.Equal(value.Stringify(ss), R"([{},"ABCDEFG"])", "Stringify()", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += VArray{};
    helper.Equal(value.Stringify(ss), "[true,[]]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += VArray{};
    helper.Equal(value.Stringify(ss), "[false,[]]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += VArray{};
    helper.Equal(value.Stringify(ss), "[null,[]]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += 10000;
    value += VArray{};
    helper.Equal(value.Stringify(ss), "[10000,[]]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += VArray{};
    helper.Equal(value.Stringify(ss), R"(["ABC",[]])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += VArray{};
    helper.Equal(value.Stringify(ss), "[[],[]]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += VArray{};
    helper.Equal(value.Stringify(ss), "[{},[]]", "Stringify()", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += VHArray{};
    helper.Equal(value.Stringify(ss), "[true,{}]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += VHArray{};
    helper.Equal(value.Stringify(ss), "[false,{}]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += VHArray{};
    helper.Equal(value.Stringify(ss), "[null,{}]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += -1000;
    value += VHArray{};
    helper.Equal(value.Stringify(ss), "[-1000,{}]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += VHArray{};
    helper.Equal(value.Stringify(ss), R"(["ABC",{}])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += VHArray{};
    helper.Equal(value.Stringify(ss), "[[],{}]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += VHArray{};
    helper.Equal(value.Stringify(ss), "[{},{}]", "Stringify()", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += VHArray{};
    value += false;
    helper.Equal(value.Stringify(ss), "[true,{},false]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += VArray{};
    value += nullptr;
    helper.Equal(value.Stringify(ss), "[false,[],null]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += VHArray{};
    value += "ABC";
    helper.Equal(value.Stringify(ss), R"([null,{},"ABC"])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += 789;
    value += "ABC";
    helper.Equal(value.Stringify(ss), R"([null,789,"ABC"])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += VArray{};
    value += VHArray{};
    helper.Equal(value.Stringify(ss), R"(["ABC",[],{}])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += VHArray{};
    value[2] = 498;
    helper.Equal(value.Stringify(ss), "[[],{},498]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += VArray{};
    value += true;
    helper.Equal(value.Stringify(ss), "[{},[],true]", "Stringify()", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value += true;
    value += VHArray{};
    value += 0;
    value += VArray{};
    helper.Equal(value.Stringify(ss), "[true,{},0,[]]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += VArray{};
    value += nullptr;
    value += VHArray{};
    helper.Equal(value.Stringify(ss), "[false,[],null,{}]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += VHArray{};
    value += "ABC";
    value += VArray{};
    helper.Equal(value.Stringify(ss), R"([null,{},"ABC",[]])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += VArray{};
    value += VHArray{};
    value += nullptr;
    helper.Equal(value.Stringify(ss), R"(["ABC",[],{},null])", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += false;
    value += VHArray{};
    value += true;
    helper.Equal(value.Stringify(ss), "[[],false,{},true]", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += nullptr;
    value += VArray{};
    value += VArray{};
    helper.Equal(value.Stringify(ss), "[{},null,[],[]]", "Stringify()", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += false;
    value += nullptr;
    value += 123;
    value += "ABC";
    value += VArray{};
    value += VHArray{};
    helper.Equal(value.Stringify(ss), R"([true,false,null,123,"ABC",[],{}])", "Stringify()", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += VHArray{};
    value += VArray{};
    value += "a";
    value += 1.5;
    value += nullptr;
    value += false;
    value += true;
    helper.Equal(value.Stringify(ss), R"([{},[],"a",1.5,null,false,true])", "Stringify()", __LINE__);
    ///////////////////////////////////////
}

static void TestStringify2(QTest &helper) {
    StringStream<char> ss;
    ValueC             value;

    ///////////
    value = VHArray{};
    helper.Equal(value.Stringify(ss), R"({})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = true;
    helper.Equal(value.Stringify(ss), R"({"A":true})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["B"] = false;
    helper.Equal(value.Stringify(ss), R"({"B":false})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["AA"] = nullptr;
    helper.Equal(value.Stringify(ss), R"({"AA":null})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["a"] = 0;
    helper.Equal(value.Stringify(ss), R"({"a":0})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["abc"] = "a";
    helper.Equal(value.Stringify(ss), R"({"abc":"a"})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["ab"] = "ABC";
    helper.Equal(value.Stringify(ss), R"({"ab":"ABC"})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["ABC"] = VArray{};
    helper.Equal(value.Stringify(ss), R"({"ABC":[]})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["5vn7b83y98t3wrupwmwa4ataw"] = VHArray{};
    helper.Equal(value.Stringify(ss), R"({"5vn7b83y98t3wrupwmwa4ataw":{}})", "Stringify()", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["A"] = true;
    value["B"] = true;
    helper.Equal(value.Stringify(ss), R"({"A":true,"B":true})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = false;
    value["b"] = true;
    helper.Equal(value.Stringify(ss), R"({"A":false,"b":true})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"]  = nullptr;
    value["BC"] = true;
    helper.Equal(value.Stringify(ss), R"({"A":null,"BC":true})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"]  = -3;
    value["AB"] = true;
    helper.Equal(value.Stringify(ss), R"({"A":-3,"AB":true})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"]   = "a";
    value["ABC"] = true;
    helper.Equal(value.Stringify(ss), R"({"A":"a","ABC":true})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = "ABC";
    value["1"] = true;
    helper.Equal(value.Stringify(ss), R"({"A":"ABC","1":true})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"]   = VArray{};
    value["123"] = true;
    helper.Equal(value.Stringify(ss), R"({"X":[],"123":true})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["x"] = VHArray{};
    value["A"] = true;
    helper.Equal(value.Stringify(ss), R"({"x":{},"A":true})", "Stringify()", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["A2"] = true;
    value["A1"] = false;
    helper.Equal(value.Stringify(ss), R"({"A2":true,"A1":false})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A-"]   = false;
    value["A123"] = false;
    helper.Equal(value.Stringify(ss), R"({"A-":false,"A123":false})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = nullptr;
    value["B"] = false;
    helper.Equal(value.Stringify(ss), R"({"A":null,"B":false})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = 500;
    value["B"] = false;
    helper.Equal(value.Stringify(ss), R"({"A":500,"B":false})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = VString("a");
    value["B"] = false;
    helper.Equal(value.Stringify(ss), R"({"A":"a","B":false})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    const VString str("ABC");
    value["A"] = str;
    value["B"] = false;
    helper.Equal(value.Stringify(ss), R"({"A":"ABC","B":false})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"] = VArray{};
    value["A"] = false;
    helper.Equal(value.Stringify(ss), R"({"X":[],"A":false})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"] = VHArray{};
    value["A"] = false;
    helper.Equal(value.Stringify(ss), R"({"X":{},"A":false})", "Stringify()", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["A"] = true;
    value["W"] = nullptr;
    helper.Equal(value.Stringify(ss), R"({"A":true,"W":null})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = false;
    value["@"] = nullptr;
    helper.Equal(value.Stringify(ss), R"({"A":false,"@":null})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = nullptr;
    value["#"] = nullptr;
    helper.Equal(value.Stringify(ss), R"({"A":null,"#":null})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = 456.5;
    value["H"] = nullptr;
    helper.Equal(value.Stringify(ss), R"({"A":456.5,"H":null})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = "a";
    value["Q"] = nullptr;
    helper.Equal(value.Stringify(ss), R"({"A":"a","Q":null})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = "ABC";
    value["e"] = nullptr;
    helper.Equal(value.Stringify(ss), R"({"A":"ABC","e":null})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"] = VArray{};
    value["n"] = nullptr;
    helper.Equal(value.Stringify(ss), R"({"X":[],"n":null})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["t"] = VHArray{};
    value["A"] = nullptr;
    helper.Equal(value.Stringify(ss), R"({"t":{},"A":null})", "Stringify()", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["e"] = true;
    value["A"] = "A";
    helper.Equal(value.Stringify(ss), R"({"e":true,"A":"A"})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["m"] = false;
    value["A"] = "AB";
    helper.Equal(value.Stringify(ss), R"({"m":false,"A":"AB"})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["i"] = nullptr;
    value["A"] = "ABC";
    helper.Equal(value.Stringify(ss), R"({"i":null,"A":"ABC"})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["s"] = -8.9;
    value["A"] = "ABC";
    helper.Equal(value.Stringify(ss, 2U), R"({"s":-8.9,"A":"ABC"})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["-"] = "a";
    value["A"] = "ABCD";
    helper.Equal(value.Stringify(ss), R"({"-":"a","A":"ABCD"})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["g"] = "ABC";
    value["A"] = "ABCDE";
    helper.Equal(value.Stringify(ss), R"({"g":"ABC","A":"ABCDE"})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["o"] = VArray{};
    value["A"] = "ABCDEF";
    helper.Equal(value.Stringify(ss), R"({"o":[],"A":"ABCDEF"})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = VHArray{};
    value["o"] = "ABCDEFG";
    helper.Equal(value.Stringify(ss), R"({"A":{},"o":"ABCDEFG"})", "Stringify()", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["d"] = true;
    value["y"] = VArray{};
    helper.Equal(value.Stringify(ss), R"({"d":true,"y":[]})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = false;
    value["y"] = VArray{};
    helper.Equal(value.Stringify(ss), R"({"A":false,"y":[]})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = nullptr;
    value["y"] = VArray{};
    helper.Equal(value.Stringify(ss), R"({"A":null,"y":[]})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = 10000;
    value["y"] = VArray{};
    helper.Equal(value.Stringify(ss), R"({"A":10000,"y":[]})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = "ABC";
    value["y"] = VArray{};
    helper.Equal(value.Stringify(ss), R"({"A":"ABC","y":[]})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"] = VArray{};
    value["y"] = VArray{};
    helper.Equal(value.Stringify(ss), R"({"X":[],"y":[]})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"] = VHArray{};
    value["Y"] = VArray{};
    helper.Equal(value.Stringify(ss), R"({"X":{},"Y":[]})", "Stringify()", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["A"] = true;
    value["y"] = VHArray{};
    helper.Equal(value.Stringify(ss), R"({"A":true,"y":{}})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = false;
    value["y"] = VHArray{};
    helper.Equal(value.Stringify(ss), R"({"A":false,"y":{}})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = nullptr;
    value["y"] = VHArray{};
    helper.Equal(value.Stringify(ss), R"({"A":null,"y":{}})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = -1000;
    value["y"] = VHArray{};
    helper.Equal(value.Stringify(ss), R"({"A":-1000,"y":{}})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = "ABC";
    value["y"] = VHArray{};
    helper.Equal(value.Stringify(ss), R"({"A":"ABC","y":{}})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["C"] = VArray{};
    value["R"] = VHArray{};
    helper.Equal(value.Stringify(ss), R"({"C":[],"R":{}})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["cc"] = VHArray{};
    value["rr"] = VHArray{};
    helper.Equal(value.Stringify(ss), R"({"cc":{},"rr":{}})", "Stringify()", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["A"]  = true;
    value["y"]  = VHArray{};
    value["AA"] = false;
    helper.Equal(value.Stringify(ss), R"({"A":true,"y":{},"AA":false})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = false;
    value["y"] = VArray{};
    value["B"] = nullptr;
    helper.Equal(value.Stringify(ss), R"({"A":false,"y":[],"B":null})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"]   = nullptr;
    value["y"]   = VHArray{};
    value["ABC"] = "ABC";
    helper.Equal(value.Stringify(ss), R"({"A":null,"y":{},"ABC":"ABC"})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["t"] = nullptr;
    value["Y"] = 789;
    value["A"] = "ABC";
    helper.Equal(value.Stringify(ss), R"({"t":null,"Y":789,"A":"ABC"})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"]     = "ABC";
    value["y"]     = VArray{};
    value["key-u"] = VHArray{};
    helper.Equal(value.Stringify(ss), R"({"A":"ABC","y":[],"key-u":{}})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"]     = VArray{};
    value["Y"]     = VHArray{};
    value["key-u"] = 498;
    helper.Equal(value.Stringify(ss), R"({"X":[],"Y":{},"key-u":498})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"] = VHArray{};
    value["y"] = VArray{};
    value["A"] = true;
    helper.Equal(value.Stringify(ss), R"({"X":{},"y":[],"A":true})", "Stringify()", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["{}}"]   = true;
    value["y"]     = VHArray{};
    value["AA"]    = 0;
    value["k-300"] = VArray{};
    helper.Equal(value.Stringify(ss), R"({"{}}":true,"y":{},"AA":0,"k-300":[]})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["B1"]    = false;
    value["y"]     = VArray{};
    value["[A]"]   = nullptr;
    value["k-300"] = VHArray{};
    helper.Equal(value.Stringify(ss), R"({"B1":false,"y":[],"[A]":null,"k-300":{}})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["{A}"]   = nullptr;
    value["y"]     = VHArray{};
    value["AA"]    = "ABC";
    value["k-300"] = VArray{};
    helper.Equal(value.Stringify(ss), R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["x"]     = "ABC";
    value["[]]"]   = VArray{};
    value["key-u"] = VHArray{};
    value["A"]     = nullptr;
    helper.Equal(value.Stringify(ss), R"({"x":"ABC","[]]":[],"key-u":{},"A":null})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"]       = VArray{};
    value["CCCCccc"] = false;
    value["key-u"]   = VHArray{};
    value["A"]       = true;
    helper.Equal(value.Stringify(ss), R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"]     = VHArray{};
    value["A"]     = nullptr;
    value["key-u"] = VArray{};
    value["k-300"] = VArray{};
    helper.Equal(value.Stringify(ss), R"({"X":{},"A":null,"key-u":[],"k-300":[]})", "Stringify()", __LINE__);

    ///////////
    ss.Clear();

    value.Reset();
    value["A"] = true;
    value["B"] = false;
    value["C"] = nullptr;
    value["D"] = 123;
    value["E"] = "ABC";
    value["F"] = VArray{};
    value["G"] = VHArray{};
    helper.Equal(value.Stringify(ss), R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})", "Stringify()",
                 __LINE__);

    ///////////
    ss.Clear();

    value.Reset();
    value["A"]       = VHArray{};
    value["BB"]      = VArray{};
    value["CCC"]     = "a";
    value["DDDD"]    = 1.5;
    value["EEEEE"]   = nullptr;
    value["FFFFFF"]  = false;
    value["GGGGGGG"] = true;
    helper.Equal(value.Stringify(ss),
                 R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})", "Stringify()",
                 __LINE__);
    ///////////////////////////////////////
}

static void TestStringify3(QTest &helper) {
    StringStream<char> ss;
    ValueC             value;

    value[0] += true;
    value[0] += false;
    value[0] += nullptr;
    value[0] += 0;
    value[0] += "ABC";
    value[0] += VArray{};
    value[0] += VHArray{};
    helper.Equal(value.Stringify(ss), R"([[true,false,null,0,"ABC",[],{}]])", "Stringify()", __LINE__);
    ss.Clear();

    value         = VArray{};
    value[0]["a"] = true;
    value[0]["0"] = false;
    value[0]["1"] = nullptr;
    value[0]["V"] = 0;
    value[0]["B"] = "a";
    value[0]["2"] = VArray{};
    value[0]["6"] = VHArray{};
    helper.Equal(value.Stringify(ss), R"([{"a":true,"0":false,"1":null,"V":0,"B":"a","2":[],"6":{}}])", "Stringify()",
                 __LINE__);
    ss.Clear();

    value.Reset();
    value["o"] += true;
    value["o"] += false;
    value["o"] += nullptr;
    value["o"] += 0;
    value["o"] += "ABC";
    value["o"] += VArray{};
    value["o"] += VHArray{};
    helper.Equal(value.Stringify(ss), R"({"o":[true,false,null,0,"ABC",[],{}]})", "Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["i"]["a"] = true;
    value["i"]["0"] = false;
    value["i"]["1"] = nullptr;
    value["i"]["V"] = 0;
    value["i"]["B"] = "a";
    value["i"]["2"] = VArray{};
    value["i"]["6"] = VHArray{};
    helper.Equal(value.Stringify(ss), R"({"i":{"a":true,"0":false,"1":null,"V":0,"B":"a","2":[],"6":{}}})",
                 "Stringify()", __LINE__);
}

static void TestStringify4(QTest &helper) {
    StringStream<char> ss;
    ValueC             value;

    value["\"\\/\b\f\n\r\t"] = "\t\r\n\f\b/\\\"";
    helper.Equal(value.Stringify(ss), R"({"\"\\\/\b\f\n\r\t":"\t\r\n\f\b\/\\\""})", "value.Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value += "\"\\/\b\f\n\r\t";
    helper.Equal(value.Stringify(ss), R"(["\"\\\/\b\f\n\r\t"])", "value.Stringify()", __LINE__);
}

static void TestDeleteValue(QTest &helper) {
    StringStream<char> ss;
    ValueC             value;

    value[0] = 1;
    value.RemoveIndex(int{0});
    helper.Equal(value.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.Equal(value.Stringify(ss), R"([])", "value.Stringify()", __LINE__);
    ss.Clear();

    value[0] = "c";
    value.RemoveIndex(SizeT32{0});
    helper.Equal(value.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.Equal(value.Stringify(ss), R"([])", "value.Stringify()", __LINE__);
    ss.Clear();

    value[0] = VArray{};
    value.RemoveIndex(SizeT64{0});
    helper.Equal(value.Stringify(ss), R"([])", "value.Stringify()", __LINE__);
    ss.Clear();

    value[0] = false;
    value[1] = true;
    value.RemoveIndex(0);
    helper.Equal(value.Stringify(ss), R"([true])", "value.Stringify()", __LINE__);
    ss.Clear();
    value.RemoveIndex(1);
    helper.Equal(value.Stringify(ss), R"([])", "value.Stringify()", __LINE__);
    ss.Clear();

    value[0] = "abc";
    value[1] = nullptr;
    value.RemoveIndex(1);
    helper.Equal(value.Stringify(ss), R"(["abc"])", "value.Stringify()", __LINE__);
    ss.Clear();
    value.RemoveIndex(0);
    helper.Equal(value.Stringify(ss), R"([])", "value.Stringify()", __LINE__);
    ss.Clear();

    value[0] = false;
    value[1] = true;
    value[2] = nullptr;
    value.RemoveIndex(1);
    helper.Equal(value.Stringify(ss), R"([false,null])", "value.Stringify()", __LINE__);
    ss.Clear();
    value.RemoveIndex(0);
    helper.Equal(value.Stringify(ss), R"([null])", "value.Stringify()", __LINE__);
    ss.Clear();

    value.RemoveIndex(2);
    helper.Equal(value.Stringify(ss), R"([])", "value.Stringify()", __LINE__);
    ss.Clear();

    value[0] = false;
    value[1] = true;
    value[2] = nullptr;
    value[1].Reset();
    helper.Equal(value.Stringify(ss), R"([false,null])", "value.Stringify()", __LINE__);
    ss.Clear();
    value[0].Reset();
    helper.Equal(value.Stringify(ss), R"([null])", "value.Stringify()", __LINE__);
    ss.Clear();

    value[2].Reset();
    helper.Equal(value.Stringify(ss), R"([])", "value.Stringify()", __LINE__);
    ss.Clear();

    value[0] = "a";
    value[1] = VArray{};
    value[2] = VHArray{};
    value.RemoveIndex(2);
    helper.Equal(value.Stringify(ss), R"(["a",[]])", "value.Stringify()", __LINE__);
    ss.Clear();
    value.RemoveIndex(1);
    helper.Equal(value.Stringify(ss), R"(["a"])", "value.Stringify()", __LINE__);
    ss.Clear();

    value.RemoveIndex(0);
    helper.Equal(value.Stringify(ss), R"([])", "value.Stringify()", __LINE__);
    ss.Clear();
    /////////

    value.Reset();

    value["A"] = 1;
    value.RemoveIndex(0);
    helper.Equal(value.Stringify(ss), R"({})", "value.Stringify()", __LINE__);
    ss.Clear();

    value["A"] = "c";
    value.Remove("A");
    helper.Equal(value.Stringify(ss), R"({})", "value.Stringify()", __LINE__);
    ss.Clear();

    value["A"] = VArray{};
    value.Remove("A");
    helper.Equal(value.Stringify(ss), R"({})", "value.Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"]  = false;
    value["bb"] = true;
    value.Remove("A");
    helper.Equal(value.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.Equal(value.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.Equal(value.Stringify(ss), R"({"bb":true})", "value.Stringify()", __LINE__);
    ss.Clear();
    value.RemoveIndex(1);
    helper.Equal(value.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.Equal(value.GetValue(1), nullptr, "GetValue(1)", "null", __LINE__);
    helper.Equal(value.Stringify(ss), R"({})", "value.Stringify()", __LINE__);
    ss.Clear();

    value["A"]  = "abc";
    value["bb"] = nullptr;
    value.Remove("bb");
    helper.Equal(value.Stringify(ss), R"({"A":"abc"})", "value.Stringify()", __LINE__);
    ss.Clear();
    value.Remove("A");
    helper.Equal(value.Stringify(ss), R"({})", "value.Stringify()", __LINE__);
    ss.Clear();

    value["A"]   = false;
    value["bb"]  = true;
    value["AAA"] = nullptr;
    value.Remove("bb");
    helper.Equal(value.Stringify(ss), R"({"A":false,"AAA":null})", "value.Stringify()", __LINE__);
    ss.Clear();
    value.Remove("A");
    helper.Equal(value.Stringify(ss), R"({"AAA":null})", "value.Stringify()", __LINE__);
    ss.Clear();

    value.Remove("AAA");
    helper.Equal(value.Stringify(ss), R"({})", "value.Stringify()", __LINE__);
    ss.Clear();

    value["A"]   = "a";
    value["bb"]  = VHArray{};
    value["AAA"] = VArray{};
    value.Remove("AAA");
    helper.Equal(value.Stringify(ss), R"({"A":"a","bb":{}})", "value.Stringify()", __LINE__);
    ss.Clear();
    value.Remove("bb");
    helper.Equal(value.Stringify(ss), R"({"A":"a"})", "value.Stringify()", __LINE__);
    ss.Clear();

    value.Remove("A");
    helper.Equal(value.Stringify(ss), R"({})", "value.Stringify()", __LINE__);
    ss.Clear();

    value["A"]   = "a";
    value["bb"]  = VHArray{};
    value["AAA"] = VArray{};
    value["AAA"].Reset();
    helper.Equal(value.Stringify(ss), R"({"A":"a","bb":{}})", "value.Stringify()", __LINE__);
    ss.Clear();
    value["bb"].Reset();
    helper.Equal(value.Stringify(ss), R"({"A":"a"})", "value.Stringify()", __LINE__);
    ss.Clear();

    value["A"].Reset();
    helper.Equal(value.Stringify(ss), R"({})", "value.Stringify()", __LINE__);
}

static void TestSortValue(QTest &helper) {
    StringStream<char> ss;
    ValueC             value;

    value["2019"] = 0;
    value["2016"] = 0;
    value["2017"] = 0;
    value["2020"] = 0;
    value["2015"] = 0;
    value["2021"] = 0;
    value["2018"] = 0;

    value.Sort();

    helper.Equal(value.Stringify(ss), R"({"2015":0,"2016":0,"2017":0,"2018":0,"2019":0,"2020":0,"2021":0})",
                 "value.Stringify()", __LINE__);
    ss.Clear();

    value.Reset();

    value["2019"] = 0;
    value["2016"] = 0;
    value["2017"] = 0;
    value["2020"] = 0;
    value["2015"] = 0;
    value["2021"] = 0;
    value["2018"] = 0;

    value.Sort(false);

    helper.Equal(value.Stringify(ss), R"({"2021":0,"2020":0,"2019":0,"2018":0,"2017":0,"2016":0,"2015":0})",
                 "value.Stringify()", __LINE__);
    ss.Clear();

    value.Reset();

    value["2019"] = 0;
    value["2016"] = 0;
    value["2017"] = 0;
    value["2020"] = 0;
    value["2015"] = 0;
    value["2021"] = 0;

    value.Sort();

    helper.Equal(value.Stringify(ss), R"({"2015":0,"2016":0,"2017":0,"2019":0,"2020":0,"2021":0})", "value.Stringify()",
                 __LINE__);
    ss.Clear();

    value.Reset();

    value["2019"] = 0;
    value["2016"] = 0;
    value["2017"] = 0;
    value["2020"] = 0;
    value["2015"] = 0;
    value["2021"] = 0;

    value.Sort(false);

    helper.Equal(value.Stringify(ss), R"({"2021":0,"2020":0,"2019":0,"2017":0,"2016":0,"2015":0})", "value.Stringify()",
                 __LINE__);
}

static void TestGroupValue(QTest &helper) {
    StringStream<char> ss;
    ValueC             value;

    value += VHArray{};
    value += VHArray{};
    value += VHArray{};
    value += VHArray{};
    value += VHArray{};
    value += VHArray{};
    value += VHArray{};

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

    helper.Equal(
        value2.Stringify(ss),
        R"({"2017":[{"month":1}],"2018":[{"month":2},{"month":3}],"2019":[{"month":4}],"2020":[{"month":5},{"month":6},{"month":7}]})",
        "value2.Stringify()", __LINE__);
    ss.Clear();

    value2.Sort(false);

    helper.Equal(
        value2.Stringify(ss),
        R"({"2020":[{"month":5},{"month":6},{"month":7}],"2019":[{"month":4}],"2018":[{"month":2},{"month":3}],"2017":[{"month":1}]})",
        "value2.Stringify()", __LINE__);
    ss.Clear();

    value.Reset();
    value2.Reset();
    value.GroupBy(value2, "year");
    helper.Equal(value2.Stringify(ss), R"()", "value2.Stringify()", __LINE__);
    ss.Clear();

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
    helper.Equal(value2.Stringify(ss), R"({})", "value2.Stringify()", __LINE__);
    ss.Clear();
    ////

    value.Reset();

    value[0]["year"] = 2019;
    value[1]["year"] = 2020;
    value[2]["year"];

    value[0]["month"] = 4;
    value[1]["month"] = 5;
    value[2]["month"] = 1;

    helper.EqualsFalse(value.GroupBy(value2, "year"), "value2.Stringify()", __LINE__);

    value[2].Reset();
    value.GroupBy(value2, "year");
    helper.EqualsFalse(value.GroupBy(value2, "year"), "value2.Stringify()", __LINE__);

    value[2]["year"] = VHArray{};
    helper.EqualsFalse(value.GroupBy(value2, "year"), "value2.Stringify()", __LINE__);

    ///////////////////
    ss.Clear();

    value.Reset();
    value2.Reset();

    value2 += unsigned{4};
    value2 += unsigned{1};
    value2 += unsigned{3};
    value2 += unsigned{5};
    value2 += unsigned{2};
    value2 += unsigned{7};
    value2 += unsigned{6};

    value = value2;
    value.Sort();

    helper.Equal(value.Stringify(ss), R"([1,2,3,4,5,6,7])", "value.Stringify()", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    helper.Equal(value.Stringify(ss), R"([7,6,5,4,3,2,1])", "value.Stringify()", __LINE__);
    ss.Clear();

    //////////////////////

    value.Reset();
    value2.Reset();

    value2 += 5.4;
    value2 += "str";
    value2 += VArray{};
    value2 += nullptr;
    value2 += true;
    value2 += VHArray{};
    value2 += false;

    value = value2;
    value.Sort();

    helper.Equal(value.Stringify(ss, 2U), R"([{},[],"str",5.4,true,false,null])", "value.Stringify()", __LINE__);
    ss.Clear();

    value = value2;
    value.Sort(false);

    helper.Equal(value.Stringify(ss, 2U), R"([null,false,true,5.4,"str",[],{}])", "value.Stringify()", __LINE__);
    ss.Clear();

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

    helper.Equal(value.Stringify(ss), R"([-7,-6,-5,-4,-3,-2,-1])", "value.Stringify()", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    helper.Equal(value.Stringify(ss), R"([-1,-2,-3,-4,-5,-6,-7])", "value.Stringify()", __LINE__);
    ss.Clear();

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

    helper.Equal(value.Stringify(ss), R"([1.5,2.5,3.5,4.5,5.5,6.5,7.5])", "value.Stringify()", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    helper.Equal(value.Stringify(ss), R"([7.5,6.5,5.5,4.5,3.5,2.5,1.5])", "value.Stringify()", __LINE__);
    ss.Clear();

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

    helper.Equal(value.Stringify(ss), R"(["a","b","c","d","e","f","g"])", "value.Stringify()", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    helper.Equal(value.Stringify(ss), R"(["g","f","e","d","c","b","a"])", "value.Stringify()", __LINE__);
    ss.Clear();

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

    helper.Equal(value.Stringify(ss), R"([[0,0,0],[0,0,0,0,0]])", "value.Stringify()", __LINE__);
    ss.Clear();

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

    helper.Equal(value.Stringify(ss), R"([[0,0,0,0,0],[0,0,0]])", "value.Stringify()", __LINE__);

    ///////////////////
    ss.Clear();

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

    helper.Equal(value.Stringify(ss), R"([{"a":0,"b":0,"c":0},{"a":0,"b":0,"c":0,"d":0,"e":0}])", "value.Stringify()",
                 __LINE__);
    ss.Clear();

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

    helper.Equal(value.Stringify(ss), R"([{"a":0,"b":0,"c":0,"d":0,"e":0},{"a":0,"b":0,"c":0}])", "value.Stringify()",
                 __LINE__);

    //////////////////////

    ///////////////////
    ss.Clear();

    value.Reset();
    value2.Reset();

    value2 += true;
    value2 += true;

    value = value2;
    value.Sort();

    helper.Equal(value.Stringify(ss), R"([true,true])", "value.Stringify()", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    helper.Equal(value.Stringify(ss), R"([true,true])", "value.Stringify()", __LINE__);
    ss.Clear();

    //////////

    value.Reset();
    value2.Reset();

    value2 += false;
    value2 += false;

    value = value2;
    value.Sort();

    helper.Equal(value.Stringify(ss), R"([false,false])", "value.Stringify()", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    helper.Equal(value.Stringify(ss), R"([false,false])", "value.Stringify()", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value2.Reset();

    value2 += nullptr;
    value2 += nullptr;

    value = value2;
    value.Sort();

    helper.Equal(value.Stringify(ss), R"([null,null])", "value.Stringify()", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    helper.Equal(value.Stringify(ss), R"([null,null])", "value.Stringify()", __LINE__);
    ss.Clear();
    ///////////
    value.RemoveIndex(0);
    value.RemoveIndex(1);

    value.Sort();

    helper.Equal(value.Stringify(ss), R"([])", "value.Stringify()", __LINE__);
    ss.Clear();

    value.Sort(false);

    helper.Equal(value.Stringify(ss), R"([])", "value.Stringify()", __LINE__);
    ///////////////////
}

static int RunValueTests() {
    QTest helper{"Value.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("Empty Value Test", TestEmptyValue);
    helper.Test("True Value Test", TestTrueValue);
    helper.Test("False Value Test", TestFalseValue);
    helper.Test("Null Value Test", TestNullValue);
    helper.Test("Number Value Test 1", TestNumberValue1);
    helper.Test("Number Value Test 2", TestNumberValue2);
    helper.Test("Number Value Test 3", TestNumberValue3);
    helper.Test("Number Value Test 4", TestNumberValue4);
    helper.Test("Number Value Test 5", TestNumberValue5);
    helper.Test("String Value Test", TestStringValue);
    helper.Test("Array Value Test", TestArrayValue);

    helper.Test("Object Value Test 1", TestObjectValue1);
    helper.Test("Object Value Test 2", TestObjectValue2);

    helper.Test("Move Value Test 1", TestMoveValue1);
    helper.Test("Move Value Test 2", TestMoveValue2);
    helper.Test("Move Value Test 3", TestMoveValue3);
    helper.Test("Move Value Test 4", TestMoveValue4);

    helper.Test("Copy Value Test 1", TestCopyValue1);
    helper.Test("Copy Value Test 2", TestCopyValue2);
    helper.Test("Copy Value Test 3", TestCopyValue3);
    helper.Test("Copy Value Test 4", TestCopyValue4);

    helper.Test("Index Operator Test 1", TestIndexOperator1);
    helper.Test("Index Operator Test 2", TestIndexOperator2);

    helper.Test("Addition Test 1", TestAddition1);
    helper.Test("Addition Test 2", TestAddition2);
    helper.Test("Addition Test 3", TestAddition3);
    helper.Test("Addition Test 4", TestAddition4);
    helper.Test("Addition Test 5", TestAddition5);
    helper.Test("Addition Test 6", TestAddition6);

    helper.Test("Stringify Test 1", TestStringify1);
    helper.Test("Stringify Test 2", TestStringify2);
    helper.Test("Stringify Test 3", TestStringify3);
    helper.Test("Stringify Test 4", TestStringify4);

    helper.Test("Delete Value Test", TestDeleteValue);

    helper.Test("Sort Value Test", TestSortValue);
    helper.Test("Group Value Test", TestGroupValue);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
