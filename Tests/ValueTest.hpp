/*
 * Copyright (c) 2024 Hani Ammar
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

#ifndef QENTEM_VALUE_TESTS_H
#define QENTEM_VALUE_TESTS_H

#include "QTest.hpp"
#include "Value.hpp"

namespace Qentem {
namespace Test {

using VString       = String<char>;
using ValueC        = Value<char>;
using VStringStream = StringStream<char>;
using VHArray       = HArray<String<char>, ValueC>;
using VArray        = Array<ValueC>;
using ObjectItem    = HAItem_T<String<char>, ValueC>;

static void TestEmptyValue(QTest &test) {
    ValueC value1;

    VStringStream ss_var;
    QNumber64     num_var;
    const char   *c_str_var;
    SizeT         c_str_len;
    bool          bool_var;

    test.IsTrue(value1.GetNumberType() == QNumberType::NotANumber, __LINE__);
    test.IsTrue(value1.IsUndefined(), __LINE__);
    test.IsFalse(value1.IsObject(), __LINE__);
    test.IsFalse(value1.IsArray(), __LINE__);
    test.IsFalse(value1.IsString(), __LINE__);
    test.IsFalse(value1.IsNumber(), __LINE__);
    test.IsFalse(value1.IsTrue(), __LINE__);
    test.IsFalse(value1.IsFalse(), __LINE__);
    test.IsFalse(value1.IsNull(), __LINE__);
    test.IsTrue(value1.Type() == ValueType::Undefined, __LINE__);
    test.IsEqual(value1.Size(), 0U, __LINE__);
    test.IsNull(value1.GetValueAt(0), __LINE__);
    test.IsNull(value1.GetValueAt(10), __LINE__);
    test.IsNull(value1.GetValue("", 0), __LINE__);
    test.IsNull(value1.GetKey(0), __LINE__);
    test.IsFalse(value1.CopyKeyByIndexTo(ss_var, 0), __LINE__);
    test.IsFalse(value1.SetCharAndLength(c_str_var, c_str_len), __LINE__);
    test.IsNull(value1.GetKey(10), __LINE__);
    test.IsNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetString(), __LINE__);
    test.IsNull(value1.StringStorage(), __LINE__);
    test.IsEqual(value1.Length(), 0U, __LINE__);
    test.IsFalse(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(value1.GetNumber(), 0.0, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::NotANumber, __LINE__);
    test.IsFalse(value1.SetBool(bool_var), __LINE__);
    test.IsEqual(value1.Stringify(), "", __LINE__);

    value1 = ValueC{ValueType::Object};
    test.IsTrue(value1.IsObject(), __LINE__);

    value1 = ValueC{ValueType::Array};
    test.IsTrue(value1.IsArray(), __LINE__);

    value1 = ValueC{ValueType::String};
    test.IsTrue(value1.IsString(), __LINE__);

    value1 = ValueC{ValueType::UIntLong};
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsTrue(value1.IsUInt64(), __LINE__);

    value1 = ValueC{ValueType::IntLong};
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsTrue(value1.IsInt64(), __LINE__);

    value1 = ValueC{ValueType::Double};
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsTrue(value1.IsDouble(), __LINE__);
}

static void TestTrueValue(QTest &test) {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    QNumber64     num_var;
    const char   *c_str_var;
    SizeT         c_str_len;
    bool          bool_var;

    value1 = true;
    test.IsTrue(value1.IsTrue(), __LINE__);
    test.IsTrue(value1.GetNumberType() == QNumberType::NotANumber, __LINE__);
    test.IsTrue(value1.Type() == ValueType::True, __LINE__);
    test.IsEqual(value1.Size(), 0U, __LINE__);
    test.IsNull(value1.GetValue(0), __LINE__);
    test.IsNull(value1.GetKey(0), __LINE__);
    test.IsFalse(value1.CopyKeyByIndexTo(ss_var, 0), __LINE__);
    test.IsTrue(value1.SetCharAndLength(c_str_var, c_str_len), __LINE__);
    test.IsTrue(StringUtils::IsEqual("true", c_str_var, c_str_len), __LINE__);
    test.IsNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetString(), __LINE__);
    test.IsNull(value1.StringStorage(), __LINE__);
    test.IsEqual(value1.Length(), 0U, __LINE__);
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "true", __LINE__);
    test.IsEqual(value1.GetNumber(), 1.0, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Natural, __LINE__);
    test.IsEqual(num_var.Natural, 1U, __LINE__);
    test.IsTrue(value1.SetBool(bool_var), __LINE__);
    test.IsTrue(bool_var, __LINE__);
    test.IsEqual(value1.Stringify(), "", __LINE__);
    ss_var.Reset();

    value1.Reset();
    test.IsTrue(value1.IsUndefined(), __LINE__);

    value1 = true;
    value2 = true;
    value2 = value1;
    test.IsTrue(value2.IsTrue(), __LINE__);
    test.IsTrue(value1.IsTrue(), __LINE__);

    value1 = true;
    value2 = ValueC{value1};
    test.IsTrue(value2.IsTrue(), __LINE__);

    value2.Reset();
    value2 = Memory::Move(value1);
    test.IsTrue(value1.IsUndefined(), __LINE__);
    test.IsTrue(value2.IsTrue(), __LINE__);

    value1 = true;
    ValueC value3{Memory::Move(value1)};
    test.IsTrue(value1.IsUndefined(), __LINE__);

    test.IsTrue(value3.IsTrue(), __LINE__);

    value3 = true;
    test.IsTrue(value3.IsTrue(), __LINE__);

    value3 = ValueC{ValueType::True};
    test.IsTrue(value3.IsTrue(), __LINE__);
}

static void TestFalseValue(QTest &test) {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    QNumber64     num_var;
    const char   *c_str_var;
    SizeT         c_str_len;
    bool          bool_var;

    value1 = false;
    test.IsTrue(value1.IsFalse(), __LINE__);
    test.IsTrue(value1.GetNumberType() == QNumberType::NotANumber, __LINE__);
    test.IsTrue(value1.Type() == ValueType::False, __LINE__);
    test.IsEqual(value1.Size(), 0U, __LINE__);
    test.IsNull(value1.GetValue(0), __LINE__);
    test.IsNull(value1.GetKey(0), __LINE__);
    test.IsFalse(value1.CopyKeyByIndexTo(ss_var, 0), __LINE__);
    test.IsTrue(value1.SetCharAndLength(c_str_var, c_str_len), __LINE__);
    test.IsTrue(StringUtils::IsEqual("false", c_str_var, c_str_len), __LINE__);
    test.IsNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetString(), __LINE__);
    test.IsNull(value1.StringStorage(), __LINE__);
    test.IsEqual(value1.Length(), 0U, __LINE__);
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "false", __LINE__);
    test.IsEqual(value1.GetNumber(), 0.0, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Natural, __LINE__);
    test.IsEqual(num_var.Natural, 0U, __LINE__);
    test.IsTrue(value1.SetBool(bool_var), __LINE__);
    test.IsFalse(bool_var, __LINE__);
    test.IsEqual(value1.Stringify(), "", __LINE__);
    ss_var.Reset();

    value1.Reset();
    test.IsTrue(value1.IsUndefined(), __LINE__);

    value1 = false;
    value2 = false;
    value2 = value1;
    test.IsTrue(value2.IsFalse(), __LINE__);
    test.IsTrue(value1.IsFalse(), __LINE__);

    value1 = false;
    value2 = ValueC{value1};
    test.IsTrue(value2.IsFalse(), __LINE__);

    value2.Reset();
    value2 = Memory::Move(value1);
    test.IsTrue(value1.IsUndefined(), __LINE__);
    test.IsTrue(value2.IsFalse(), __LINE__);

    value1 = false;
    ValueC value3(Memory::Move(value1));
    test.IsTrue(value1.IsUndefined(), __LINE__);

    test.IsTrue(value3.IsFalse(), __LINE__);

    value3 = false;
    test.IsTrue(value3.IsFalse(), __LINE__);

    value3 = ValueC{ValueType::False};
    test.IsTrue(value3.IsFalse(), __LINE__);
}

static void TestNullValue(QTest &test) {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    QNumber64     num_var;
    const char   *c_str_var;
    SizeT         c_str_len;
    bool          bool_var;

    value1 = nullptr;
    test.IsTrue(value1.IsNull(), __LINE__);
    test.IsTrue(value1.GetNumberType() == QNumberType::NotANumber, __LINE__);
    test.IsTrue(value1.Type() == ValueType::Null, __LINE__);
    test.IsEqual(value1.Size(), 0U, __LINE__);
    test.IsNull(value1.GetValue(0), __LINE__);
    test.IsNull(value1.GetKey(0), __LINE__);
    test.IsFalse(value1.CopyKeyByIndexTo(ss_var, 0), __LINE__);
    test.IsTrue(value1.SetCharAndLength(c_str_var, c_str_len), __LINE__);
    test.IsTrue(StringUtils::IsEqual("null", c_str_var, c_str_len), __LINE__);
    test.IsNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetString(), __LINE__);
    test.IsNull(value1.StringStorage(), __LINE__);
    test.IsEqual(value1.Length(), 0U, __LINE__);
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "null", __LINE__);
    test.IsEqual(value1.GetNumber(), 0.0, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Natural, __LINE__);
    test.IsEqual(num_var.Natural, 0U, __LINE__);
    test.IsTrue(value1.SetBool(bool_var), __LINE__);
    test.IsFalse(bool_var, __LINE__);
    test.IsEqual(value1.Stringify(), "", __LINE__);
    ss_var.Reset();

    value1.Reset();
    test.IsTrue(value1.IsUndefined(), __LINE__);

    value1 = nullptr;
    value2 = nullptr;
    value2 = value1;
    test.IsTrue(value2.IsNull(), __LINE__);
    test.IsTrue(value1.IsNull(), __LINE__);

    value1 = nullptr;
    value2 = ValueC{value1};
    test.IsTrue(value2.IsNull(), __LINE__);

    value2.Reset();
    value2 = Memory::Move(value1);
    test.IsTrue(value1.IsUndefined(), __LINE__);
    test.IsTrue(value2.IsNull(), __LINE__);

    value1 = nullptr;
    ValueC value3(Memory::Move(value1));
    test.IsTrue(value1.IsUndefined(), __LINE__);

    test.IsTrue(value3.IsNull(), __LINE__);

    value3 = nullptr;
    test.IsTrue(value3.IsNull(), __LINE__);

    value3 = ValueC{ValueType::Null};
    test.IsTrue(value3.IsNull(), __LINE__);
}

static void TestNumberValue1(QTest &test) {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    QNumber64     num_var;
    const char   *c_str_var;
    SizeT         c_str_len;
    bool          bool_var;

    value1 = 33;
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsTrue(value1.GetNumberType() == QNumberType::Integer, __LINE__);
    test.IsEqual(value1.Size(), 0U, __LINE__);
    test.IsNull(value1.GetValue(0), __LINE__);
    test.IsNull(value1.GetKey(0), __LINE__);
    test.IsFalse(value1.CopyKeyByIndexTo(ss_var, 0), __LINE__);
    test.IsFalse(value1.SetCharAndLength(c_str_var, c_str_len), __LINE__);
    test.IsNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetString(), __LINE__);
    test.IsNull(value1.StringStorage(), __LINE__);
    test.IsEqual(value1.Length(), 0U, __LINE__);
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "33", __LINE__);
    test.IsEqual(value1.GetNumber(), 33.0, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Integer, __LINE__);
    test.IsEqual(num_var.Integer, 33, __LINE__);
    test.IsTrue(value1.SetBool(bool_var), __LINE__);
    test.IsTrue(bool_var, __LINE__);
    test.IsEqual(value1.Stringify(), "", __LINE__);

    value1.Reset();
    test.IsTrue(value1.IsUndefined(), __LINE__);

    value1 = 45;
    value2 = -10;
    value2 = value1;
    test.IsTrue(value2.IsNumber(), __LINE__);
    test.IsTrue(value2.GetNumberType() == QNumberType::Integer, __LINE__);
    ss_var.Clear();
    test.IsTrue(value2.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "45", __LINE__);
    test.IsEqual(value2.GetNumber(), 45.0, __LINE__);
    test.IsTrue(value2.SetNumber(num_var) == QNumberType::Integer, __LINE__);
    test.IsEqual(num_var.Integer, 45, __LINE__);
    test.IsTrue(value2.SetBool(bool_var), __LINE__);
    test.IsTrue(bool_var, __LINE__);
    test.IsEqual(value1.GetNumber(), 45U, __LINE__);

    value1.Reset();

    value1 = SizeT32{10};
    value2 = ValueC{value1};
    test.IsEqual(value2.GetNumber(), 10.0, __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);

    value2.Reset();
    value2 = Memory::Move(value1);
    test.IsTrue(value1.IsUndefined(), __LINE__);
    test.IsEqual(value2.GetNumber(), 10.0, __LINE__);

    value2 = 1213;
    test.IsEqual(value2.GetNumber(), 1213.0, __LINE__);

    value2 = 10;
    value2 = double{785};
    test.IsEqual(value2.GetNumber(), 785.0, __LINE__);

    value2 = SizeT32{0};
    test.IsTrue(value2.SetBool(bool_var), __LINE__);
    test.IsFalse(bool_var, __LINE__);

    value2 = -8;
    test.IsTrue(value2.SetBool(bool_var), __LINE__);
    test.IsFalse(bool_var, __LINE__);

    value2 = ValueC{double{3.75}};
    test.IsTrue(value2.IsNumber(), __LINE__);
    test.IsEqual(value2.GetNumber(), 3.75, __LINE__);
}

static void TestNumberValue2(QTest &test) {
    using vu_long = unsigned long;

    ValueC        value1;
    VStringStream ss_var;
    QNumber64     num_var;
    bool          bool_var;

    /////////////////// unsigned

    value1 = ValueC{SizeT16{10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Natural, __LINE__);
    test.IsTrue(value1.Type() == ValueType::UIntLong, __LINE__);
    test.IsTrue(value1.IsUInt64(), __LINE__);
    test.IsFalse(value1.IsInt64(), __LINE__);
    test.IsFalse(value1.IsDouble(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), 10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), 10, __LINE__);
    test.IsEqual(value1.GetUInt64(), 10U, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Natural, __LINE__);
    test.IsEqual(num_var.Natural, 10U, __LINE__);
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "10", __LINE__);
    test.IsTrue(value1.SetBool(bool_var), __LINE__);
    test.IsTrue(bool_var, __LINE__);
    value1.Reset();

    value1 = ValueC{SizeT32{10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Natural, __LINE__);
    test.IsTrue(value1.IsUInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), 10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), 10, __LINE__);
    test.IsEqual(value1.GetUInt64(), 10U, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Natural, __LINE__);
    test.IsEqual(num_var.Natural, 10U, __LINE__);
    value1.Reset();

    value1 = ValueC{vu_long{10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Natural, __LINE__);
    test.IsTrue(value1.IsUInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), 10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), 10, __LINE__);
    test.IsEqual(value1.GetUInt64(), 10U, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Natural, __LINE__);
    test.IsEqual(num_var.Natural, 10U, __LINE__);
    value1.Reset();

    value1 = ValueC{SizeT64{10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Natural, __LINE__);
    test.IsTrue(value1.IsUInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), 10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), 10, __LINE__);
    test.IsEqual(value1.GetUInt64(), 10U, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Natural, __LINE__);
    test.IsEqual(num_var.Natural, 10U, __LINE__);
    value1.Reset();
}

static void TestNumberValue3(QTest &test) {
    ValueC        value1;
    VStringStream ss_var;
    QNumber64     num_var;
    bool          bool_var;

    /////////////////// signed

    value1 = ValueC{short{-10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Integer, __LINE__);
    test.IsTrue(value1.Type() == ValueType::IntLong, __LINE__);
    test.IsFalse(value1.IsUInt64(), __LINE__);
    test.IsTrue(value1.IsInt64(), __LINE__);
    test.IsFalse(value1.IsDouble(), __LINE__);
    test.IsEqual(value1.GetNumber(), -10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), -10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), -10, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Integer, __LINE__);
    test.IsEqual(num_var.Integer, -10, __LINE__);
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "-10", __LINE__);
    test.IsTrue(value1.SetBool(bool_var), __LINE__);
    test.IsFalse(bool_var, __LINE__);
    value1.Reset();

    value1 = ValueC{short{10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Integer, __LINE__);
    test.IsTrue(value1.IsInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), 10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), 10, __LINE__);
    test.IsEqual(value1.GetUInt64(), 10U, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Integer, __LINE__);
    test.IsEqual(num_var.Integer, 10, __LINE__);
    ss_var.Clear();
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "10", __LINE__);
    value1.Reset();

    value1 = ValueC{-10};
    test.IsTrue(value1.GetNumberType() == QNumberType::Integer, __LINE__);
    test.IsTrue(value1.IsInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), -10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), -10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), -10, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Integer, __LINE__);
    test.IsEqual(num_var.Integer, -10, __LINE__);
    value1.Reset();

    value1 = ValueC{10};
    test.IsTrue(value1.GetNumberType() == QNumberType::Integer, __LINE__);
    test.IsTrue(value1.IsInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), 10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), 10, __LINE__);
    test.IsEqual(value1.GetUInt64(), 10U, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Integer, __LINE__);
    test.IsEqual(num_var.Integer, 10U, __LINE__);
    value1.Reset();

    value1 = ValueC{long{-10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Integer, __LINE__);
    test.IsTrue(value1.IsInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), -10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), -10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), -10, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Integer, __LINE__);
    test.IsEqual(num_var.Integer, -10, __LINE__);
    value1.Reset();

    value1 = ValueC{long{10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Integer, __LINE__);
    test.IsTrue(value1.IsInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), 10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), 10, __LINE__);
    test.IsEqual(value1.GetUInt64(), 10U, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Integer, __LINE__);
    test.IsEqual(num_var.Integer, 10U, __LINE__);
    value1.Reset();

    value1 = ValueC{long{-10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Integer, __LINE__);
    test.IsTrue(value1.IsInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), -10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), -10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), -10, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Integer, __LINE__);
    test.IsEqual(num_var.Integer, -10, __LINE__);
    value1.Reset();

    value1 = ValueC{SizeT64I{10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Integer, __LINE__);
    test.IsTrue(value1.IsInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), 10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), 10, __LINE__);
    test.IsEqual(value1.GetUInt64(), 10U, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Integer, __LINE__);
    test.IsEqual(num_var.Integer, 10U, __LINE__);
    value1.Reset();
}

static void TestNumberValue4(QTest &test) {
    ValueC        value1;
    VStringStream ss_var;
    QNumber64     num_var;
    bool          bool_var;

    /////////////////// float

    value1 = ValueC{float{10.5}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Real, __LINE__);
    test.IsTrue(value1.Type() == ValueType::Double, __LINE__);
    test.IsFalse(value1.IsUInt64(), __LINE__);
    test.IsFalse(value1.IsInt64(), __LINE__);
    test.IsTrue(value1.IsDouble(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.5, __LINE__);
    test.IsEqual(value1.GetDouble(), 10.5, __LINE__);
    test.IsEqual(value1.GetInt64(), 10, __LINE__);
    test.IsEqual(value1.GetUInt64(), 10U, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Real, __LINE__);
    test.IsEqual(num_var.Real, 10.5, __LINE__);
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "10.5", __LINE__);
    test.IsTrue(value1.SetBool(bool_var), __LINE__);
    test.IsTrue(bool_var, __LINE__);
    value1.Reset();

    value1 = ValueC{float{-10.5}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Real, __LINE__);
    test.IsTrue(value1.IsDouble(), __LINE__);
    test.IsEqual(value1.GetNumber(), -10.5, __LINE__);
    test.IsEqual(value1.GetDouble(), -10.5, __LINE__);
    test.IsEqual(value1.GetInt64(), -10, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Real, __LINE__);
    test.IsEqual(num_var.Real, -10.5, __LINE__);
    ss_var.Clear();
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "-10.5", __LINE__);
    test.IsTrue(value1.SetBool(bool_var), __LINE__);
    test.IsFalse(bool_var, __LINE__);
    value1.Reset();

    value1 = ValueC{float{10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Real, __LINE__);
    test.IsTrue(value1.IsDouble(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), 10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), 10, __LINE__);
    test.IsEqual(value1.GetUInt64(), 10U, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Real, __LINE__);
    test.IsEqual(num_var.Real, 10U, __LINE__);
    value1.Reset();

    value1 = ValueC{float{-10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Real, __LINE__);
    test.IsTrue(value1.IsDouble(), __LINE__);
    test.IsEqual(value1.GetNumber(), -10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), -10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), -10, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Real, __LINE__);
    test.IsEqual(num_var.Real, -10, __LINE__);
    value1.Reset();

    value1 = ValueC{double{10.5}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Real, __LINE__);
    test.IsTrue(value1.IsDouble(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.5, __LINE__);
    test.IsEqual(value1.GetDouble(), 10.5, __LINE__);
    test.IsEqual(value1.GetInt64(), 10, __LINE__);
    test.IsEqual(value1.GetUInt64(), 10U, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Real, __LINE__);
    test.IsEqual(num_var.Real, 10.5, __LINE__);
    value1.Reset();

    value1 = ValueC{double{-10.5}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Real, __LINE__);
    test.IsTrue(value1.IsDouble(), __LINE__);
    test.IsEqual(value1.GetNumber(), -10.5, __LINE__);
    test.IsEqual(value1.GetDouble(), -10.5, __LINE__);
    test.IsEqual(value1.GetInt64(), -10, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Real, __LINE__);
    test.IsEqual(num_var.Real, -10.5, __LINE__);
    value1.Reset();

    value1 = ValueC{double{10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Real, __LINE__);
    test.IsTrue(value1.IsDouble(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), 10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), 10, __LINE__);
    test.IsEqual(value1.GetUInt64(), 10U, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Real, __LINE__);
    test.IsEqual(num_var.Real, 10.0, __LINE__);
    ss_var.Clear();
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "10", __LINE__);
    test.IsTrue(value1.SetBool(bool_var), __LINE__);
    test.IsTrue(bool_var, __LINE__);
    value1.Reset();

    value1 = ValueC{double{-10}};
    test.IsTrue(value1.GetNumberType() == QNumberType::Real, __LINE__);
    test.IsTrue(value1.IsDouble(), __LINE__);
    test.IsEqual(value1.GetNumber(), -10.0, __LINE__);
    test.IsEqual(value1.GetDouble(), -10.0, __LINE__);
    test.IsEqual(value1.GetInt64(), -10, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Real, __LINE__);
    test.IsEqual(num_var.Real, -10, __LINE__);
    ss_var.Clear();
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "-10", __LINE__);
    test.IsTrue(value1.SetBool(bool_var), __LINE__);
    test.IsFalse(bool_var, __LINE__);
    value1.Reset();
}

static void TestNumberValue5(QTest &test) {
    using vu_long = unsigned long;

    ValueC value1;

    value1 = SizeT16{10};
    test.IsTrue(value1.GetNumberType() == QNumberType::Natural, __LINE__);
    test.IsTrue(value1.IsUInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);

    value1 = -10;
    test.IsTrue(value1.GetNumberType() == QNumberType::Integer, __LINE__);
    test.IsTrue(value1.IsInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), -10.0, __LINE__);

    value1 = -10.5;
    test.IsTrue(value1.GetNumberType() == QNumberType::Real, __LINE__);
    test.IsTrue(value1.IsDouble(), __LINE__);
    test.IsEqual(value1.GetNumber(), -10.5, __LINE__);

    value1 = SizeT32{10};
    test.IsTrue(value1.GetNumberType() == QNumberType::Natural, __LINE__);
    test.IsTrue(value1.IsUInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);

    value1 = float{10};
    test.IsTrue(value1.GetNumberType() == QNumberType::Real, __LINE__);
    test.IsTrue(value1.IsDouble(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);

    value1 = 10;
    test.IsTrue(value1.GetNumberType() == QNumberType::Integer, __LINE__);
    test.IsTrue(value1.IsInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);

    value1 = vu_long{10};
    test.IsTrue(value1.GetNumberType() == QNumberType::Natural, __LINE__);
    test.IsTrue(value1.IsUInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);

    value1 = long{-10};
    test.IsTrue(value1.GetNumberType() == QNumberType::Integer, __LINE__);
    test.IsTrue(value1.IsInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), -10.0, __LINE__);

    value1 = SizeT64{10};
    test.IsTrue(value1.GetNumberType() == QNumberType::Natural, __LINE__);
    test.IsTrue(value1.IsUInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);

    value1 = double{-10};
    test.IsTrue(value1.GetNumberType() == QNumberType::Real, __LINE__);
    test.IsTrue(value1.IsDouble(), __LINE__);
    test.IsEqual(value1.GetNumber(), -10.0, __LINE__);

    value1 = SizeT64I{10};
    test.IsTrue(value1.GetNumberType() == QNumberType::Integer, __LINE__);
    test.IsTrue(value1.IsInt64(), __LINE__);
    test.IsEqual(value1.GetNumber(), 10.0, __LINE__);
}

static void TestStringValue(QTest &test) {
    ValueC value1;
    ValueC value2;

    VStringStream ss_var;
    VString       str_var;
    QNumber64     num_var;
    const char   *c_str_var;
    SizeT         c_str_len;
    bool          bool_var;

    value1 = "-ABCDEF0123456789ABCDEF0123456789-";
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsTrue(value1.GetNumberType() == QNumberType::NotANumber, __LINE__);
    test.IsTrue(value1.Type() == ValueType::String, __LINE__);
    test.IsEqual(value1.Size(), SizeT{0}, __LINE__);
    test.IsEqual(value1.Length(), SizeT{34}, __LINE__);
    test.IsNull(value1.GetValue(0), __LINE__);
    test.IsNull(value1.GetKey(0), __LINE__);
    test.IsFalse(value1.CopyKeyByIndexTo(ss_var, 0), __LINE__);
    test.IsTrue(value1.SetCharAndLength(c_str_var, c_str_len), __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1.StringStorage(), c_str_var, c_str_len), __LINE__);
    test.IsNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetArray(), __LINE__);
    test.IsEqual(value1.GetUInt64(), SizeT64{0}, __LINE__);
    test.IsEqual(value1.GetInt64(), 0, __LINE__);
    test.IsEqual(value1.GetDouble(), 0.0, __LINE__);
    test.IsNotNull(value1.GetString(), __LINE__);
    test.IsEqual(*(value1.GetString()), "-ABCDEF0123456789ABCDEF0123456789-", __LINE__);
    test.IsNotNull(value1.StringStorage(), __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1.StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsEqual(value1.Length(), 34U, __LINE__);
    ss_var.Clear();
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "-ABCDEF0123456789ABCDEF0123456789-", __LINE__);
    ss_var.Clear();
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "-ABCDEF0123456789ABCDEF0123456789-", __LINE__);
    test.IsEqual(value1.GetNumber(), 0.0, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::NotANumber, __LINE__);
    test.IsFalse(value1.SetBool(bool_var), __LINE__);
    test.IsEqual(value1.Stringify(), "", __LINE__);

    value1.Reset();
    test.IsTrue(value1.IsUndefined(), __LINE__);

    value1 = "45";
    value2 = "-50";
    test.IsTrue(value2.SetNumber(num_var) == QNumberType::Integer, __LINE__);
    test.IsEqual(num_var.Integer, -50, __LINE__);
    test.IsEqual(value2.GetNumber(), -50.0, __LINE__);

    value2 = value1;
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsNotNull(value1.GetString(), __LINE__);
    test.IsEqual(*(value1.GetString()), "45", __LINE__);
    test.IsNotNull(value1.StringStorage(), __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1.StringStorage(), "45", 2), __LINE__);
    test.IsEqual(value1.Length(), 2U, __LINE__);
    ss_var.Clear();
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "45", __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::Natural, __LINE__);
    test.IsEqual(num_var.Natural, 45U, __LINE__);
    test.IsEqual(value1.GetNumber(), 45.0, __LINE__);
    test.IsFalse(value1.SetBool(bool_var), __LINE__);
    ss_var.Clear();
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "45", __LINE__);

    value1 = "true";
    value2 = ValueC{value1};
    ss_var.Clear();
    test.IsTrue(value2.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "true", __LINE__);
    test.IsEqual(value2.GetNumber(), 0.0, __LINE__);
    bool_var = false;
    test.IsTrue(value2.SetBool(bool_var), __LINE__);
    test.IsTrue(bool_var, __LINE__);

    ss_var.Clear();
    test.IsTrue(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "true", __LINE__);

    value2.Reset();
    value2 = Memory::Move(value1);
    ss_var.Clear();
    test.IsTrue(value2.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "true", __LINE__);

    test.IsTrue(value1.IsUndefined(), __LINE__);

    value1 = "false";
    ValueC value3(Memory::Move(value1));
    test.IsEqual(value3.Length(), 5U, __LINE__);
    ss_var.Clear();
    test.IsTrue(value3.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "false", __LINE__);
    bool_var = true;
    test.IsTrue(value3.SetBool(bool_var), __LINE__);
    test.IsFalse(bool_var, __LINE__);

    str_var           = "qen";
    const char *c_str = str_var.First();
    value3            = str_var;
    ss_var.Clear();
    test.IsTrue(value3.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "qen", __LINE__);
    test.IsNotEqual(value3.StringStorage(), c_str, __LINE__);

    value3 = ValueC{VString("ABC")};
    test.IsTrue(value3.IsString(), __LINE__);
    test.IsNotNull(value3.GetString(), __LINE__);
    test.IsEqual(*(value3.GetString()), "ABC", __LINE__);

    value3 = VString("123");
    ss_var.Clear();
    test.IsTrue(value3.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "123", __LINE__);

    value3 = 321.25;
    value3 = "321.25";
    test.IsTrue(value3.IsString(), __LINE__);
    ss_var.Clear();
    test.IsTrue(value3.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(ss_var, "321.25", __LINE__);
}

static void TestArrayValue(QTest &test) {
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

    arr_var.ResizeWithDefaultInit(5);
    storage = arr_var.First();

    value1 = arr_var; // Copy.
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsTrue(value1.GetNumberType() == QNumberType::NotANumber, __LINE__);
    test.IsTrue(value1.Type() == ValueType::Array, __LINE__);
    test.IsEqual(value1.Size(), 5U, __LINE__);
    test.IsNull(value1.GetValueAt(0), __LINE__);
    test.IsNull(value1.GetValueAt(4), __LINE__);
    test.IsNull(value1.GetKey(0), __LINE__);
    test.IsFalse(value1.CopyKeyByIndexTo(ss_var, 0), __LINE__);
    test.IsFalse(value1.SetCharAndLength(c_str_var, c_str_len), __LINE__);
    test.IsNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), storage, __LINE__);
    test.IsNull(value1.GetString(), __LINE__);
    test.IsNull(value1.StringStorage(), __LINE__);
    test.IsEqual(value1.Length(), 0U, __LINE__);
    test.IsFalse(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(value1.GetNumber(), 0.0, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::NotANumber, __LINE__);
    test.IsFalse(value1.SetBool(bool_var), __LINE__);
    test.IsEqual(value1.Stringify(), "[]", __LINE__);

    arr_var.Reset();
    value1 = arr_var;
    test.IsEqual(value1.Stringify(), "[]", __LINE__);

    value1.Reset();
    test.IsTrue(value1.IsUndefined(), __LINE__);

    arr_var.Reset();
    arr_var.ResizeWithDefaultInit(10);
    storage = arr_var.First();

    VArray arr_var2;
    arr_var2.ResizeWithDefaultInit(10);

    value1 = Memory::Move(arr_var); // Move
    value2 = Memory::Move(arr_var2);
    value2 = value1;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 10U, __LINE__);
    test.IsNull(value1.GetValueAt(0), __LINE__);
    test.IsNull(value1.GetValueAt(9), __LINE__);
    test.IsNull(value1.GetKey(0), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), storage, __LINE__);

    test.IsEqual(value2.Size(), 10U, __LINE__);
    test.IsNotNull(value2.GetArray(), __LINE__);
    test.IsNotEqual(value2.GetArray()->First(), storage, __LINE__);

    arr_var.Reset();
    arr_var.ResizeWithDefaultInit(7);
    storage = arr_var.First();
    value1  = Memory::Move(arr_var);

    value2 = ValueC{value1};
    test.IsTrue(value2.IsArray(), __LINE__);
    test.IsEqual(value2.Size(), 7U, __LINE__);
    test.IsNotNull(value2.GetArray(), __LINE__);
    test.IsNotEqual(value2.GetArray()->First(), storage, __LINE__);

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 7U, __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), storage, __LINE__);

    value2.Reset();
    value2 = Memory::Move(value1);
    test.IsNotNull(value2.GetArray(), __LINE__);
    test.IsEqual(value2.GetArray()->First(), storage, __LINE__);

    test.IsTrue(value1.IsUndefined(), __LINE__);

    arr_var.Reset();
    arr_var.ResizeWithDefaultInit(7);
    storage = arr_var.First();
    value1  = Memory::Move(arr_var);

    ValueC value3(Memory::Move(value1));
    test.IsNotNull(value3.GetArray(), __LINE__);
    test.IsEqual(value3.GetArray()->First(), storage, __LINE__);

    test.IsTrue(value1.IsUndefined(), __LINE__);

    arr_var.Reset();
    arr_var.ResizeWithDefaultInit(3);
    storage = arr_var.First();
    value3  = arr_var; // Copy
    test.IsEqual(value3.Size(), 3U, __LINE__);
    test.IsNotNull(value3.GetArray(), __LINE__);
    test.IsNotEqual(value3.GetArray()->First(), storage, __LINE__);

    arr_var.Reset();
    arr_var.ResizeWithDefaultInit(13);
    storage = arr_var.First();
    value3  = Memory::Move(arr_var); // Move
    test.IsNotNull(value3.GetArray(), __LINE__);
    test.IsEqual(value3.GetArray()->First(), storage, __LINE__);

    value3 = ValueC{VArray(3)};
    test.IsTrue(value3.IsArray(), __LINE__);
    test.IsNotNull(value3.GetArray(), __LINE__);
    test.IsNotNull(value3.GetArray()->First(), __LINE__);
    test.IsEqual(value3.GetArray()->Capacity(), 3U, __LINE__);

    value1[0] = 1;

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 1U, __LINE__);
    test.IsEqual(value1[0].GetNumber(), 1.0, __LINE__);

    value1.Reset();

    value1[0] = 1;
    value1[1] = 2;
    value1[2] = 3;

    test.IsEqual(value1.Size(), 3U, __LINE__);

    value2 = value1;
    test.IsEqual(value2.Size(), 3U, __LINE__);
    test.IsEqual(value2[0].GetNumber(), 1.0, __LINE__);
    test.IsEqual(value2[1].GetNumber(), 2.0, __LINE__);
    test.IsEqual(value2[2].GetNumber(), 3.0, __LINE__);

    value1[0] = 11;
    value1[1] = 22;
    str_var   = "Qen";
    c_str_var = str_var.First();
    value1[2] = Memory::Move(str_var);

    value2 = value1;
    test.IsEqual(value2.Size(), 3U, __LINE__);
    test.IsEqual(value2[0].GetNumber(), 11.0, __LINE__);
    test.IsEqual(value2[1].GetNumber(), 22.0, __LINE__);
    test.IsNotEqual(value2[2].StringStorage(), c_str_var, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2[2].StringStorage(), "Qen", 3), __LINE__);

    value2[3] = 44;
    value2[4] = 55;
    test.IsEqual(value2.Size(), 5U, __LINE__);
    test.IsEqual(value2[0].GetNumber(), 11.0, __LINE__);
    test.IsEqual(value2[1].GetNumber(), 22.0, __LINE__);
    test.IsNotEqual(value2[2].StringStorage(), c_str_var, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2[2].StringStorage(), "Qen", 3), __LINE__);
    test.IsEqual(value2[3].GetNumber(), 44.0, __LINE__);
    test.IsEqual(value2[4].GetNumber(), 55.0, __LINE__);

    value2 = value1;
    test.IsEqual(value2.Size(), 3U, __LINE__);
    test.IsEqual(value2[0].GetNumber(), 11.0, __LINE__);
    test.IsEqual(value2[1].GetNumber(), 22.0, __LINE__);
    test.IsNotEqual(value2[2].StringStorage(), c_str_var, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2[2].StringStorage(), "Qen", 3), __LINE__);
    test.IsNull(value2.GetValueAt(3), __LINE__);
    test.IsNull(value2.GetValueAt(4), __LINE__);

    ////////////////////
    str_var   = "-ABCDEF0123456789ABCDEF0123456789-";
    c_str_var = str_var.First();

    arr_var.ResizeWithDefaultInit(4);
    ValueC *val_ptr = arr_var.Storage();

    val_ptr[0] = 10;
    val_ptr[1] = 20;
    val_ptr[2] = 30;
    val_ptr[3] = Memory::Move(str_var);

    value2 = arr_var;
    test.IsEqual(value2.Size(), 4U, __LINE__);
    test.IsEqual(value2[0].GetNumber(), 10.0, __LINE__);
    test.IsEqual(value2[1].GetNumber(), 20.0, __LINE__);
    test.IsEqual(value2[2].GetNumber(), 30.0, __LINE__);
    test.IsNotEqual(value2[3].StringStorage(), c_str_var, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2[3].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsNull(value2.GetValueAt(4), __LINE__);

    ////////////////////

    arr_var.Clear();
    arr_var.Resize(5);
    value2 = Memory::Move(arr_var);
    test.IsNotNull(value2.GetArray(), __LINE__);
    test.IsEqual(value2.GetArray()->Capacity(), 5U, __LINE__);

    value2 += 1;

    value2.Compress();
    test.IsNotNull(value2.GetArray(), __LINE__);
    test.IsEqual(value2.GetArray()->Capacity(), 1U, __LINE__);
    test.IsEqual(value2.GetArray()->Size(), 1U, __LINE__);

    arr_var.Reset();
    arr_var.Reserve(10);
    value2 = Memory::Move(arr_var);
    value2.Compress();
    test.IsNotNull(value2.GetArray(), __LINE__);
    test.IsEqual(value2.GetArray()->Capacity(), 0U, __LINE__);
    test.IsEqual(value2.GetArray()->Size(), 0U, __LINE__);
}

static void TestObjectValue1(QTest &test) {
    ValueC value1;
    ValueC value2;

    VHArray           h_arr_var;
    const ObjectItem *storage;
    VStringStream     ss_var;
    QNumber64         num_var;
    const char       *c_str_var;
    SizeT             c_str_len;
    bool              bool_var;

    for (SizeT32 i = 0; i < 5; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();

    value1 = h_arr_var; // Copy.
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsTrue(value1.GetNumberType() == QNumberType::NotANumber, __LINE__);
    test.IsTrue(value1.Type() == ValueType::Object, __LINE__);
    test.IsEqual(value1.Size(), 5U, __LINE__);
    test.IsNotNull(value1.GetValueAt(0), __LINE__);
    test.IsNotNull(value1.GetValueAt(4), __LINE__);
    test.IsNotNull(value1.GetKey(0), __LINE__);
    test.IsNotNull(value1.GetKey(4), __LINE__);
    test.IsTrue(value1.CopyKeyByIndexTo(ss_var, 0), __LINE__);
    test.IsTrue(value1.CopyKeyByIndexTo(ss_var, 4), __LINE__);
    test.IsFalse(value1.SetCharAndLength(c_str_var, c_str_len), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsNotEqual(value1.GetObject()->First(), storage, __LINE__);
    test.IsNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetString(), __LINE__);
    test.IsNull(value1.StringStorage(), __LINE__);
    test.IsEqual(value1.Length(), 0U, __LINE__);
    test.IsFalse(value1.CopyValueTo(ss_var), __LINE__);
    test.IsEqual(value1.GetNumber(), 0.0, __LINE__);
    test.IsTrue(value1.SetNumber(num_var) == QNumberType::NotANumber, __LINE__);
    test.IsFalse(value1.SetBool(bool_var), __LINE__);

    h_arr_var.Reset();
    value1 = h_arr_var;
    test.IsEqual(value1.Stringify(), "{}", __LINE__);

    value1.Reset();
    test.IsTrue(value1.IsUndefined(), __LINE__);

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
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsEqual(value1.Size(), 10U, __LINE__);
    test.IsEqual(value1.GetObject()->First(), storage, __LINE__);

    value1 = h_arr_var;
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsEqual(value1.Stringify(), "{}", __LINE__);

    test.IsTrue(value2.IsObject(), __LINE__);
    test.IsNotEqual(value2.GetObject()->First(), storage, __LINE__);

    h_arr_var.Reset();
    // Testing empty values
    for (SizeT32 i = 0; i < 10; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key];
    }

    value2 = h_arr_var;
    test.IsEqual(value2.Size(), 10U, __LINE__);

    h_arr_var.Reset();
    for (SizeT i = 0; i < 7; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value1  = Memory::Move(h_arr_var);

    value2 = ValueC{value1};
    test.IsTrue(value2.IsObject(), __LINE__);
    test.IsEqual(value2.Size(), 7U, __LINE__);
    test.IsNotEqual(value2.GetObject()->First(), storage, __LINE__);

    h_arr_var.Reset();
    value2 = h_arr_var;
    test.IsTrue(value2.IsObject(), __LINE__);
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsEqual(value1.GetObject()->First(), storage, __LINE__);

    h_arr_var.Reset();
    value1 = h_arr_var;
    test.IsTrue(value1.IsObject(), __LINE__);

    for (SizeT32 i = 0; i < 7; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value1  = Memory::Move(h_arr_var);

    value2.Reset();
    value2 = Memory::Move(value1);
    test.IsTrue(value2.IsObject(), __LINE__);
    test.IsEqual(value2.GetObject()->First(), storage, __LINE__);

    h_arr_var.Reset();
    value2 = h_arr_var;
    test.IsEqual(value2.Stringify(), "{}", __LINE__);
    test.IsTrue(value1.IsUndefined(), __LINE__);
}

static void TestObjectValue2(QTest &test) {
    ValueC            value1;
    VHArray           h_arr_var;
    const ObjectItem *storage;
    VString           str_var;
    const char       *c_str_var;

    h_arr_var.Reset();
    for (SizeT32 i = 0; i < 7; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value1  = Memory::Move(h_arr_var);

    ValueC value3(Memory::Move(value1));
    test.IsTrue(value3.IsObject(), __LINE__);
    test.IsEqual(value3.Size(), 7U, __LINE__);
    test.IsEqual(value3.GetObject()->First(), storage, __LINE__);

    h_arr_var.Reset();
    value3 = h_arr_var;
    test.IsEqual(value3.Stringify(), "{}", __LINE__);
    test.IsTrue(value1.IsUndefined(), __LINE__);

    h_arr_var.Reset();
    for (SizeT32 i = 0; i < 3; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value3  = h_arr_var; // Copy
    test.IsEqual(value3.Size(), 3U, __LINE__);
    test.IsNotNull(value3.GetObject(), __LINE__);
    test.IsNotNull(value3.GetObject()->First(), __LINE__);
    test.IsNotEqual(value3.GetObject()->First(), storage, __LINE__);

    for (SizeT32 i = 0; i < 13; i++) {
        VString key("Key_");
        Digit::NumberToString(key, i);
        h_arr_var[key] = i;
    }

    storage = h_arr_var.First();
    value3  = Memory::Move(h_arr_var); // Move
    test.IsTrue(value3.IsObject(), __LINE__);
    test.IsEqual(value3.Size(), 13U, __LINE__);
    test.IsNotNull(value3.GetObject(), __LINE__);
    test.IsNotNull(value3.GetObject()->First(), __LINE__);
    test.IsEqual(value3.GetObject()->First(), storage, __LINE__);

    value3 = ValueC{VHArray(4)};
    test.IsTrue(value3.IsObject(), __LINE__);
    test.IsNotNull(value3.GetObject(), __LINE__);
    test.IsNotNull(value3.GetObject()->First(), __LINE__);
    test.IsEqual(value3.GetObject()->Capacity(), 4U, __LINE__);

    value1.Reset();
    value1[""] = 1;

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsEqual(value1.Size(), 1U, __LINE__);
    test.IsEqual(value1[""].GetNumber(), 1.0, __LINE__);

    value1.Reset();

    value1["k1"] = 1;
    value1["k2"] = 2;
    value1["k3"] = 3;

    test.IsEqual(value1.Size(), 3U, __LINE__);

    value3 = value1;
    test.IsEqual(value3.Size(), 3U, __LINE__);
    test.IsEqual(value3["k1"].GetNumber(), 1.0, __LINE__);
    test.IsEqual(value3["k2"].GetNumber(), 2.0, __LINE__);
    test.IsEqual(value3["k3"].GetNumber(), 3.0, __LINE__);

    value1["k1"] = 11;
    value1["k2"] = 22;
    str_var      = "Qen";
    c_str_var    = str_var.First();
    value1["k3"] = Memory::Move(str_var);

    value3 = value1;
    test.IsEqual(value3.Size(), 3U, __LINE__);
    test.IsEqual(value3["k1"].GetNumber(), 11.0, __LINE__);
    test.IsEqual(value3["k2"].GetNumber(), 22.0, __LINE__);
    test.IsNotEqual(value3["k3"].StringStorage(), c_str_var, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value3["k3"].StringStorage(), "Qen", 3), __LINE__);

    value3["k4"] = 44;
    value3["k5"] = 55;
    test.IsEqual(value3.Size(), 5U, __LINE__);
    test.IsEqual(value3["k1"].GetNumber(), 11.0, __LINE__);
    test.IsEqual(value3["k2"].GetNumber(), 22.0, __LINE__);
    test.IsNotEqual(value3["k3"].StringStorage(), c_str_var, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value3["k3"].StringStorage(), "Qen", 3), __LINE__);
    test.IsEqual(value3["k4"].GetNumber(), 44U, __LINE__);
    test.IsEqual(value3["k5"].GetNumber(), 55U, __LINE__);

    value3 = value1;
    test.IsEqual(value3.Size(), 3U, __LINE__);
    test.IsEqual(value3["k1"].GetNumber(), 11.0, __LINE__);
    test.IsEqual(value3["k2"].GetNumber(), 22.0, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value3["k3"].StringStorage(), "Qen", 3), __LINE__);
    test.IsNull(value3.GetValue("k4", 2), __LINE__);
    test.IsNull(value3.GetValueAt(3), __LINE__);
    test.IsNull(value3.GetValue("k5", 2), __LINE__);
    test.IsNull(value3.GetValueAt(4), __LINE__);

    ////////////////////
    h_arr_var[VString("w1")] = 10;
    h_arr_var[VString("w2")] = 20;
    h_arr_var[VString("w3")] = 30;
    str_var                  = "-ABCDEF0123456789ABCDEF0123456789-";
    c_str_var                = str_var.First();
    h_arr_var[VString("w4")] = Memory::Move(str_var);

    value3 = h_arr_var;
    test.IsEqual(value3.Size(), 4U, __LINE__);
    test.IsEqual(value3["w1"].GetNumber(), 10.0, __LINE__);
    test.IsEqual(value3["w2"].GetNumber(), 20.0, __LINE__);
    test.IsEqual(value3["w3"].GetNumber(), 30.0, __LINE__);
    test.IsNotEqual(value3["w4"].StringStorage(), c_str_var, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value3["w4"].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsNull(value3.GetValue("k5", 2), __LINE__);
    test.IsNull(value3.GetValueAt(4), __LINE__);
    ////////////////////

    h_arr_var.Reserve(10);
    h_arr_var[VString("w1")] = 10;
    h_arr_var[VString("w2")] = 20;
    h_arr_var[VString("w3")] = 30;

    value3 = Memory::Move(h_arr_var);
    test.IsEqual(value3.Size(), 3U, __LINE__);

    value3.Compress();
    test.IsNotNull(value3.GetObject(), __LINE__);
    test.IsEqual(value3.GetObject()->Size(), 3U, __LINE__);
    test.IsTrue(value3.GetObject()->Capacity() >= 3U, __LINE__);
}

static void TestMoveValue1(QTest &test) {
    ValueC value1;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray           h_arr_var;
    const ObjectItem *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = true;

    value1 = false;
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    value1 = nullptr;
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    value1 = 11;
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 11.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = Memory::Move(str_var);
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = Memory::Move(arr_var);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = Memory::Move(h_arr_var);
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = false;

    value1 = true;
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    value1 = nullptr;
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    value1 = -90;
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), -90.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = Memory::Move(str_var);
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = Memory::Move(arr_var);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = Memory::Move(h_arr_var);
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = nullptr;

    value1 = true;
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    value1 = false;
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    value1 = 7.5;
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 7.5, __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = Memory::Move(str_var);
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = Memory::Move(arr_var);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = Memory::Move(h_arr_var);
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = 13;

    value1 = true;
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = 40;

    value1 = false;
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = 33;

    value1 = nullptr;
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = 47;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = Memory::Move(str_var);
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = 10e10;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = Memory::Move(arr_var);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = 9.1;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = Memory::Move(h_arr_var);
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value1 = true;
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value1 = false;
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value1 = nullptr;
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value1 = 4;
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 4.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = Memory::Move(arr_var);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = Memory::Move(h_arr_var);
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VArray(1);

    value1 = true;
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value1 = false;
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value1 = nullptr;
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value1 = 33;
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 33.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = Memory::Move(str_var);
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value1        = Memory::Move(h_arr_var);
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestMoveValue2(QTest &test) {
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
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value1 = false;
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value1 = nullptr;
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value1 = 33;
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 33.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = Memory::Move(str_var);
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value1      = Memory::Move(arr_var);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestMoveValue3(QTest &test) {
    ValueC value1;
    ValueC value2;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray           h_arr_var;
    const ObjectItem *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = true;

    value2 = false;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    value2 = nullptr;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    value2 = 11;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 11.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = Memory::Move(value2);
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = Memory::Move(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = Memory::Move(value2);
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = false;

    value2 = true;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    value2 = nullptr;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    value2 = -90;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), -90.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = Memory::Move(value2);
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = Memory::Move(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = Memory::Move(value2);
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = nullptr;

    value2 = true;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    value2 = false;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    value2 = 7.5;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 7.5, __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = Memory::Move(value2);
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = Memory::Move(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = Memory::Move(value2);
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = 13;

    value2 = true;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = 40;

    value2 = false;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = 33;

    value2 = nullptr;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = 47;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = Memory::Move(value2);
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = 10e10;

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = Memory::Move(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = 9.1;

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = Memory::Move(value2);
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = true;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = false;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = nullptr;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = 4;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 4.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = Memory::Move(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = Memory::Move(value2);
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VArray(1);

    value2 = true;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = false;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = nullptr;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = 33;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 33.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = Memory::Move(value2);
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    h_arr_var     = VHArray(1);
    h_arr_storage = h_arr_var.First();
    value2        = Memory::Move(h_arr_var);
    value1        = Memory::Move(value2);
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestMoveValue4(QTest &test) {
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
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = false;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = nullptr;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = 33;
    value1 = Memory::Move(value2);
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 33.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = Memory::Move(value2);
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    arr_var     = VArray(1);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = Memory::Move(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestCopyValue1(QTest &test) {
    ValueC value1;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray           h_arr_var;
    const ObjectItem *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = true;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsNotEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetArray()->First(), __LINE__);
    value1.Reset();
    value1 = true;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeWithDefaultInit(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetObject()->First(), __LINE__);
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
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsNotEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = false;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsNotEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetArray()->First(), __LINE__);
    value1.Reset();
    value1 = false;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeWithDefaultInit(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetObject()->First(), __LINE__);
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
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsNotEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = nullptr;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsNotEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetArray()->First(), __LINE__);
    value1.Reset();
    value1 = nullptr;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeWithDefaultInit(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetObject()->First(), __LINE__);
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
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsNotEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = 23;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsNotEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = 33;

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetArray()->First(), __LINE__);
    value1.Reset();
    value1 = 33;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeWithDefaultInit(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = 34;

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetObject()->First(), __LINE__);
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
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsNotEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestCopyValue2(QTest &test) {
    ValueC value1;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray           h_arr_var;
    const ObjectItem *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetArray()->First(), __LINE__);
    value1.Reset();
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // Has values
    arr_var = VArray{};
    arr_var.ResizeWithDefaultInit(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetObject()->First(), __LINE__);
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
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsNotEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsNotEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    // No values
    h_arr_var = VHArray(1);
    value1    = h_arr_var;
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetObject()->First(), __LINE__);
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
    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsNotEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VHArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value1    = str_var;
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsNotEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    // No values
    arr_var = VArray(1);
    value1  = arr_var;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetArray()->First(), __LINE__);
    value1.Reset();
    value1 = VHArray(1);

    // Has values
    arr_var = VArray{};
    arr_var.ResizeWithDefaultInit(3);
    arr_storage = arr_var.First();
    value1      = arr_var;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();
    ////////////////////////////////////////////
}

static void TestCopyValue3(QTest &test) {
    ValueC value1;
    ValueC value2;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray           h_arr_var;
    const ObjectItem *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = true;

    value2 = false;
    value1 = value2;
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    value2 = nullptr;
    value1 = value2;
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    value2 = 11;
    value1 = value2;
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 11.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = value2;
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsNotEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    // No values
    value2 = VArray(1);
    value1 = value2;

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetArray()->First(), __LINE__);
    value1.Reset();
    value1 = true;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeWithDefaultInit(3);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = value2;

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = true;

    // No values
    value2 = VHArray(1);
    value1 = value2;

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetObject()->First(), __LINE__);
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

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsNotEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = false;

    value2 = true;
    value1 = value2;
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    value2 = nullptr;
    value1 = value2;
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    value2 = -90;
    value1 = value2;
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), -90.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = value2;
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsNotEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    // No values
    value2 = VArray(1);
    value1 = value2;

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetArray()->First(), __LINE__);
    value1.Reset();
    value1 = false;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeWithDefaultInit(3);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = value2;

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = false;

    // No values
    value2 = VHArray(1);
    value1 = value2;

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetObject()->First(), __LINE__);
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

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsNotEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = nullptr;

    value2 = true;
    value1 = value2;
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    value2 = false;
    value1 = value2;
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    value2 = 7.5;
    value1 = value2;
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 7.5, __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = value2;
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsNotEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    // No values
    value2 = VArray(1);
    value1 = value2;

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetArray()->First(), __LINE__);
    value1.Reset();
    value1 = nullptr;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeWithDefaultInit(3);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = value2;

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = nullptr;

    // No values
    value2 = VHArray(1);
    value1 = value2;

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetObject()->First(), __LINE__);
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

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsNotEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = 13;

    value2 = true;
    value1 = value2;
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = 40;

    value2 = false;
    value1 = value2;
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = 33;

    value2 = nullptr;
    value1 = value2;
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = 47;

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = value2;
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsNotEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = 10e10;

    // No values
    value2 = VArray(1);
    value1 = value2;

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetArray()->First(), __LINE__);
    value1.Reset();
    value1 = 10e10;

    // Has values
    arr_var = VArray{};
    arr_var.ResizeWithDefaultInit(3);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = value2;

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = 9.1;

    // No values
    value2 = VHArray(1);
    value1 = value2;

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetObject()->First(), __LINE__);
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

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsNotEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestCopyValue4(QTest &test) {
    ValueC value1;
    ValueC value2;

    // true
    // false
    // null

    VString     str_var;
    const char *c_str_var; // = str_var.First();

    VArray        arr_var;
    const ValueC *arr_storage; // = arr_var.First();

    VHArray           h_arr_var;
    const ObjectItem *h_arr_storage; // = h_arr_var.First();

    ////////////////////////////////////////////

    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = true;
    value1 = value2;
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = false;
    value1 = value2;
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = nullptr;
    value1 = value2;
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    value2 = 4;
    value1 = value2;
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 4.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // No values
    value2 = VArray(1);
    value1 = value2;

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetArray()->First(), __LINE__);
    value1.Reset();
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // Has values
    arr_var = VArray{};
    arr_var.ResizeWithDefaultInit(3);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = value2;

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    /////////////
    value1 = VString{"-ABCDEF0123456789ABCDEF0123456789-"};

    // No values
    value2 = VHArray(1);
    value1 = value2;

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetObject()->First(), __LINE__);
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

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsNotEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VArray(1);

    value2 = true;
    value1 = value2;
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = false;
    value1 = value2;
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = nullptr;
    value1 = value2;
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    value2 = 33;
    value1 = value2;
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 33.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = value2;
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsNotEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = VArray(1);

    // No values
    value2 = VHArray(1);
    value1 = value2;

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNull(value1.GetObject()->First(), __LINE__);
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

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsNotNull(value1.GetObject(), __LINE__);
    test.IsNotNull(value1.GetObject()->First(), __LINE__);
    test.IsNotEqual(value1.GetObject()->First(), h_arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////

    value1 = VHArray(1);

    value2 = true;
    value1 = value2;
    test.IsTrue(value1.IsTrue(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = false;
    value1 = value2;
    test.IsTrue(value1.IsFalse(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = nullptr;
    value1 = value2;
    test.IsTrue(value1.IsNull(), __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    value2 = 33;
    value1 = value2;
    test.IsTrue(value1.IsNumber(), __LINE__);
    test.IsEqual(value1.GetNumber(), 33.0, __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    str_var   = VString{"-ABCDEF0123456789ABCDEF0123456789-"};
    c_str_var = str_var.First();
    value2    = Memory::Move(str_var);
    value1    = value2;
    test.IsTrue(value1.IsString(), __LINE__);
    test.IsNotEqual(value1.StringStorage(), c_str_var, __LINE__);
    value1.Reset();

    /////////////
    value1 = VHArray(1);

    // No values
    value2 = VArray(1);
    value1 = value2;

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNull(value1.GetArray()->First(), __LINE__);
    value1.Reset();
    value1 = VHArray(1);

    // Has values
    arr_var = VArray{};
    arr_var.ResizeWithDefaultInit(3);
    arr_storage = arr_var.First();
    value2      = Memory::Move(arr_var);
    value1      = value2;

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    value1.Reset();

    ////////////////////////////////////////////
}

static void TestIndexOperator1(QTest &test) {
    ValueC        value;
    const VString str1("D");
    const VString str2("DEFG");

    value = VHArray(1); // Test changing type

    value[0] = 5;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsEqual(value[0].GetNumber(), 5.0, __LINE__);

    value[0] = 20;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsEqual(value[0].GetNumber(), 20.0, __LINE__);

    value[1] = 30;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 2U, __LINE__);
    test.IsEqual(value[0].GetNumber(), 20.0, __LINE__);
    test.IsEqual(value[1].GetNumber(), 30.0, __LINE__);

    value[0]          = 50;
    value[1]          = 100;
    value[2]          = 200;
    value[3]          = 300;
    value[SizeT64{4}] = 400;
    value[SizeT32{5}] = 500;

    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 6U, __LINE__);
    test.IsEqual(value[0].GetNumber(), 50.0, __LINE__);
    test.IsEqual(value[1].GetNumber(), 100.0, __LINE__);
    test.IsEqual(value[2].GetNumber(), 200.0, __LINE__);
    test.IsEqual(value[3].GetNumber(), 300.0, __LINE__);
    test.IsEqual(value[4].GetNumber(), 400.0, __LINE__);
    test.IsEqual(value[5].GetNumber(), 500.0, __LINE__);
    test.IsNotNull(value.GetValueAt(0), __LINE__);
    test.IsNotNull(value.GetValueAt(1), __LINE__);
    test.IsNotNull(value.GetValueAt(2), __LINE__);
    test.IsNotNull(value.GetValueAt(3), __LINE__);
    test.IsNotNull(value.GetValueAt(4), __LINE__);
    test.IsNotNull(value.GetValueAt(5), __LINE__);
    test.IsNotNull(value.GetValue("5", 1), __LINE__);
    test.IsNull(value.GetValue("50", 2), __LINE__);
    test.IsEqual(value.GetValueAt(0)->GetNumber(), 50.0, __LINE__);
    test.IsEqual(value.GetValueAt(1)->GetNumber(), 100.0, __LINE__);
    test.IsEqual(value.GetValueAt(2)->GetNumber(), 200.0, __LINE__);
    test.IsEqual(value.GetValueAt(3)->GetNumber(), 300.0, __LINE__);
    test.IsEqual(value.GetValueAt(4)->GetNumber(), 400.0, __LINE__);
    test.IsEqual(value.GetValueAt(5)->GetNumber(), 500.0, __LINE__);
    test.IsNull(value.GetValueAt(6), __LINE__);

    //////////////////////////////////////////
    value.Reset();

    value["A"] = 7.5;
    test.IsTrue(value.IsObject(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsEqual(value[0].GetNumber(), 7.5, __LINE__);
    test.IsEqual(value["A"].GetNumber(), 7.5, __LINE__);

    value["A"] = 59;
    test.IsTrue(value.IsObject(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsEqual(value[0].GetNumber(), 59.0, __LINE__);
    test.IsEqual(value["A"].GetNumber(), 59.0, __LINE__);

    value["B"] = 60;
    test.IsTrue(value.IsObject(), __LINE__);
    test.IsEqual(value.Size(), 2U, __LINE__);
    test.IsEqual(value[0].GetNumber(), 59.0, __LINE__);
    test.IsEqual(value["A"].GetNumber(), 59.0, __LINE__);
    test.IsEqual(value[1].GetNumber(), 60.0, __LINE__);
    test.IsEqual(value["B"].GetNumber(), 60.0, __LINE__);

    value[0]                   = 50;
    value["B"]                 = 100;
    value[VString("C")]        = 200;
    value[str1]                = 300;
    value["EFEFE"]             = 400;
    value[VString("FGHIGKLM")] = 500;

    test.IsTrue(value.IsObject(), __LINE__);
    test.IsEqual(value.Size(), 6U, __LINE__);
    test.IsEqual(value[0].GetNumber(), 50.0, __LINE__);
    test.IsEqual(value[1].GetNumber(), 100.0, __LINE__);
    test.IsEqual(value[2].GetNumber(), 200.0, __LINE__);
    test.IsEqual(value[3].GetNumber(), 300.0, __LINE__);
    test.IsEqual(value[4].GetNumber(), 400.0, __LINE__);
    test.IsEqual(value[5].GetNumber(), 500.0, __LINE__);
    test.IsEqual(value["A"].GetNumber(), 50.0, __LINE__);
    test.IsEqual(value["B"].GetNumber(), 100.0, __LINE__);
    test.IsEqual(value["C"].GetNumber(), 200.0, __LINE__);
    test.IsEqual(value["D"].GetNumber(), 300.0, __LINE__);
    test.IsEqual(value["EFEFE"].GetNumber(), 400.0, __LINE__);
    test.IsEqual(value["FGHIGKLM"].GetNumber(), 500.0, __LINE__);
    test.IsNotNull(value.GetValueAt(0), __LINE__);
    test.IsNotNull(value.GetValueAt(1), __LINE__);
    test.IsNotNull(value.GetValueAt(2), __LINE__);
    test.IsNotNull(value.GetValueAt(3), __LINE__);
    test.IsNotNull(value.GetValueAt(4), __LINE__);
    test.IsNotNull(value.GetValueAt(5), __LINE__);
    test.IsEqual(value.GetValueAt(0)->GetNumber(), 50.0, __LINE__);
    test.IsEqual(value.GetValueAt(1)->GetNumber(), 100.0, __LINE__);
    test.IsEqual(value.GetValueAt(2)->GetNumber(), 200.0, __LINE__);
    test.IsEqual(value.GetValueAt(3)->GetNumber(), 300.0, __LINE__);
    test.IsEqual(value.GetValueAt(4)->GetNumber(), 400.0, __LINE__);
    test.IsEqual(value.GetValueAt(5)->GetNumber(), 500.0, __LINE__);

    test.IsNotNull(value.GetKey(0), __LINE__);
    test.IsNotNull(value.GetKey(1), __LINE__);
    test.IsNotNull(value.GetKey(2), __LINE__);
    test.IsNotNull(value.GetKey(3), __LINE__);
    test.IsNotNull(value.GetKey(4), __LINE__);
    test.IsNotNull(value.GetKey(5), __LINE__);

    test.IsTrue(value.GetKey(0)->IsEqual("A", 1), __LINE__);
    test.IsTrue(value.GetKey(1)->IsEqual("B", 1), __LINE__);
    test.IsTrue(value.GetKey(2)->IsEqual("C", 1), __LINE__);
    test.IsTrue(value.GetKey(3)->IsEqual("D", 1), __LINE__);
    test.IsTrue(value.GetKey(4)->IsEqual("EFEFE", 5), __LINE__);
    test.IsTrue(value.GetKey(5)->IsEqual("FGHIGKLM", 8), __LINE__);

    test.IsNull(value.GetValueAt(6), __LINE__);

    value.Reset();

    value[VString("C")] = 4;
    test.IsTrue(value.IsObject(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsEqual(value[0].GetNumber(), 4.0, __LINE__);
    test.IsEqual(value[VString("C")].GetNumber(), 4.0, __LINE__);

    value.Reset();

    value[str2] = 5;
    test.IsTrue(value.IsObject(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsEqual(value[0].GetNumber(), 5.0, __LINE__);
    test.IsEqual(value[str2].GetNumber(), 5.0, __LINE__);

    //////////////////////////////////////////
}

static void TestIndexOperator2(QTest &test) {
    ValueC  value;
    ValueC *value_ptr;

    value["A"]    = 10;
    value["BB"]   = 20;
    value["CCCC"] = 30;

    value_ptr = value.GetValue("A", 1);
    test.IsNotNull(value_ptr, __LINE__);
    test.IsEqual(value_ptr->GetNumber(), 10.0, __LINE__);

    value_ptr = value.GetValue("ABCDEF", 1);
    test.IsNotNull(value_ptr, __LINE__);
    test.IsEqual(value_ptr->GetNumber(), 10.0, __LINE__);

    value_ptr = value.GetValue("BB", 2);
    test.IsNotNull(value_ptr, __LINE__);
    test.IsEqual(value_ptr->GetNumber(), 20.0, __LINE__);

    value_ptr = value.GetValue("CCCC", 4);
    test.IsNotNull(value_ptr, __LINE__);
    test.IsEqual(value_ptr->GetNumber(), 30.0, __LINE__);
    ////////////////////
    test.IsNull(value.GetValue("a", 1), __LINE__);
    test.IsNull(value.GetValue("ABCDEF", 6), __LINE__);
    test.IsNull(value.GetValue("b", 1), __LINE__);
    test.IsNull(value.GetValue("bb", 2), __LINE__);
    test.IsNull(value.GetValue("CCC", 3), __LINE__);
    ////////////////////
}

static void TestAddition1(QTest &test) {
    ValueC        value;
    const VString str1("D");
    const VString str2("DEFG");

    value += 20;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsEqual(value[0].GetNumber(), 20.0, __LINE__);

    value += 30;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 2U, __LINE__);
    test.IsEqual(value[0].GetNumber(), 20.0, __LINE__);
    test.IsEqual(value[1].GetNumber(), 30.0, __LINE__);

    value[0] = 50;
    value[1] = 100;
    value += 200;
    value += SizeT32{300};
    value += SizeT64{400};
    value += double{500};

    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 6U, __LINE__);
    test.IsEqual(value[0].GetNumber(), 50.0, __LINE__);
    test.IsEqual(value[1].GetNumber(), 100.0, __LINE__);
    test.IsEqual(value[2].GetNumber(), 200.0, __LINE__);
    test.IsEqual(value[3].GetNumber(), 300.0, __LINE__);
    test.IsEqual(value[4].GetNumber(), 400.0, __LINE__);
    test.IsEqual(value[5].GetNumber(), 500.0, __LINE__);

    value.Reset();

    value[10] = 100;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 11U, __LINE__);
    test.IsTrue(value[3].IsUndefined(), __LINE__);
    test.IsTrue(value[4].IsUndefined(), __LINE__);
    test.IsTrue(value[5].IsUndefined(), __LINE__);
    test.IsEqual(value[10].GetNumber(), 100.0, __LINE__);
    //////////////////////////////////////////
}

static void TestAddition2(QTest &test) {
    ValueC  value;
    VString str;

    /////////////////

    value += true;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsTrue(value[0].IsTrue(), __LINE__);

    value += true;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 2U, __LINE__);
    test.IsTrue(value[0].IsTrue(), __LINE__);
    test.IsTrue(value[1].IsTrue(), __LINE__);

    value.Reset();
    /////////////////

    value += false;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsTrue(value[0].IsFalse(), __LINE__);

    value += false;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 2U, __LINE__);
    test.IsTrue(value[0].IsFalse(), __LINE__);
    test.IsTrue(value[1].IsFalse(), __LINE__);

    value.Reset();
    /////////////////

    value += nullptr;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsTrue(value[0].IsNull(), __LINE__);

    value += nullptr;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 2U, __LINE__);
    test.IsTrue(value[0].IsNull(), __LINE__);
    test.IsTrue(value[1].IsNull(), __LINE__);

    value.Reset();
    /////////////////

    value += "-ABCDEF0123456789ABCDEF0123456789-";
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsTrue(value[0].IsString(), __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);

    value += "^ABCDEF0123456789ABCDEF0123456789^";
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 2U, __LINE__);
    test.IsTrue(value[0].IsString(), __LINE__);
    test.IsTrue(value[1].IsString(), __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[1].StringStorage(), "^ABCDEF0123456789ABCDEF0123456789^", 34), __LINE__);
    value.Reset();
    /////////////////
    str               = VString("-ABCDEF0123456789ABCDEF0123456789-");
    const char *c_str = str.First();
    value += Memory::Move(str);
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsTrue(value[0].IsString(), __LINE__);
    test.IsEqual(value[0].StringStorage(), c_str, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);

    str                = VString("#0123456789ABCDEF0123456789ABCDEF#");
    const char *c_str2 = str.First();
    value += Memory::Move(str);
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 2U, __LINE__);
    test.IsTrue(value[0].IsString(), __LINE__);
    test.IsTrue(value[1].IsString(), __LINE__);
    test.IsEqual(value[0].StringStorage(), c_str, __LINE__);
    test.IsEqual(value[1].StringStorage(), c_str2, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[1].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34), __LINE__);
    value.Reset();
    /////////////////

    str   = VString("-ABCDEF0123456789ABCDEF0123456789-");
    c_str = str.First();
    value += str;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsTrue(value[0].IsString(), __LINE__);
    test.IsNotEqual(value[0].StringStorage(), c_str, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);

    str    = VString("#0123456789ABCDEF0123456789ABCDEF#");
    c_str2 = str.First();
    value += str;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 2U, __LINE__);
    test.IsTrue(value[0].IsString(), __LINE__);
    test.IsTrue(value[1].IsString(), __LINE__);
    test.IsNotEqual(value[0].StringStorage(), c_str, __LINE__);
    test.IsNotEqual(value[1].StringStorage(), c_str2, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[1].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34), __LINE__);
    value.Reset();

    //////////////

    value += VHArray{};
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsTrue(value[0].IsObject(), __LINE__);

    value += VHArray{};
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 2U, __LINE__);
    test.IsTrue(value[0].IsObject(), __LINE__);
    test.IsTrue(value[1].IsObject(), __LINE__);

    //////////////////////////////////////////
}

static void TestAddition3(QTest &test) {
    ValueC        value;
    VArray        arr_var;
    const ValueC *arr_storage;
    const char   *c_str;
    const char   *c_str2;
    /////////////////
    arr_var = VArray(1);
    value += arr_var;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsNotNull(value.GetArray(), __LINE__);
    test.IsNotNull(value.GetArray()->First(), __LINE__);

    value.Reset();

    arr_var = VArray(3);
    value += arr_var;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsNotNull(value.GetArray(), __LINE__);
    test.IsNotNull(value.GetArray()->First(), __LINE__);

    value.Reset();
    ///

    arr_var += ValueC{false};
    arr_var += ValueC{true};
    arr_var += ValueC{VString("-ABCDEF0123456789ABCDEF0123456789-")};
    arr_storage = arr_var.First();
    c_str       = arr_storage[2].StringStorage();

    value += arr_var;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 3U, __LINE__);
    test.IsNotNull(value.GetArray(), __LINE__);
    test.IsNotNull(value.GetArray()->First(), __LINE__);
    test.IsNotEqual(value.GetArray()->First(), arr_storage, __LINE__);
    test.IsTrue(value[0].IsFalse(), __LINE__);
    test.IsTrue(value[1].IsTrue(), __LINE__);
    test.IsTrue(value[2].IsString(), __LINE__);
    test.IsNotEqual(value[2].StringStorage(), c_str, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[2].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);

    arr_var.Reset();
    arr_var += ValueC{nullptr};
    arr_var += ValueC{14};
    arr_var += ValueC{VString("^ABCDEF0123456789ABCDEF0123456789^")};
    arr_storage = arr_var.First();
    c_str2      = arr_storage[2].StringStorage();

    value += arr_var;
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 6U, __LINE__);
    test.IsNotNull(value.GetArray(), __LINE__);
    test.IsNotNull(value.GetArray()->First(), __LINE__);
    test.IsNotEqual(value.GetArray()->First(), arr_storage, __LINE__);
    test.IsTrue(value[0].IsFalse(), __LINE__);
    test.IsTrue(value[1].IsTrue(), __LINE__);
    test.IsTrue(value[2].IsString(), __LINE__);
    test.IsNotEqual(value[2].StringStorage(), c_str, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[2].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsTrue(value[3].IsNull(), __LINE__);
    test.IsTrue(value[4].IsNumber(), __LINE__);
    test.IsEqual(value[4].GetNumber(), 14.0, __LINE__);
    test.IsTrue(value[5].IsString(), __LINE__);
    test.IsNotEqual(value[5].StringStorage(), c_str2, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[5].StringStorage(), "^ABCDEF0123456789ABCDEF0123456789^", 34), __LINE__);

    value.Reset();
    /////////////////
    value += VArray(1);
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsNotNull(value.GetArray(), __LINE__);
    test.IsNotNull(value.GetArray()->First(), __LINE__);

    value.Reset();

    value += VArray(3);
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 1U, __LINE__);
    test.IsNotNull(value.GetArray(), __LINE__);
    test.IsNotNull(value.GetArray()->First(), __LINE__);

    value.Reset();
    arr_var.Reset();

    arr_var += ValueC{ValueType::False};
    arr_var += ValueC{ValueType::True};
    arr_var += ValueC{VString("#0123456789ABCDEF0123456789ABCDEF#")};
    arr_storage = arr_var.First();
    c_str       = arr_storage[2].StringStorage();

    value += Memory::Move(arr_var);
    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 3U, __LINE__);
    test.IsNotNull(value.GetArray(), __LINE__);
    test.IsNotNull(value.GetArray()->First(), __LINE__);
    test.IsEqual(value.GetArray()->First(), arr_storage, __LINE__);
    test.IsTrue(value[0].IsFalse(), __LINE__);
    test.IsTrue(value[1].IsTrue(), __LINE__);
    test.IsTrue(value[2].IsString(), __LINE__);
    test.IsEqual(value[2].StringStorage(), c_str, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[2].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34), __LINE__);

    arr_var += ValueC{nullptr};
    arr_var += ValueC{14};
    arr_var += ValueC{VString("-ABCDEF0123456789ABCDEF0123456789-")};
    arr_storage = arr_var.First();
    c_str2      = arr_storage[2].StringStorage();

    value += Memory::Move(arr_var);
    test.IsNull(arr_var.First(), __LINE__);

    test.IsTrue(value.IsArray(), __LINE__);
    test.IsEqual(value.Size(), 6U, __LINE__);
    test.IsNotNull(value.GetArray(), __LINE__);
    test.IsNotNull(value.GetArray()->First(), __LINE__);
    test.IsNotEqual(value.GetArray()->First(), arr_storage, __LINE__);
    test.IsTrue(value[0].IsFalse(), __LINE__);
    test.IsTrue(value[1].IsTrue(), __LINE__);
    test.IsTrue(value[2].IsString(), __LINE__);
    test.IsEqual(value[2].StringStorage(), c_str, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[2].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34), __LINE__);
    test.IsTrue(value[3].IsNull(), __LINE__);
    test.IsTrue(value[4].IsNumber(), __LINE__);
    test.IsEqual(value[4].GetNumber(), 14.0, __LINE__);
    test.IsTrue(value[5].IsString(), __LINE__);
    test.IsEqual(value[5].StringStorage(), c_str2, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value[5].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    //////////////////////////////////////////
}

static void TestAddition4(QTest &test) {
    ValueC        value1;
    ValueC        value2;
    VArray        arr_var;
    const ValueC *arr_storage;
    VString       str;

    /////////////////

    value2 = true;
    value1 += value2;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 1U, __LINE__);
    test.IsTrue(value1[0].IsTrue(), __LINE__);

    value2 = true;
    value1 += value2;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 2U, __LINE__);
    test.IsTrue(value1[0].IsTrue(), __LINE__);
    test.IsTrue(value1[1].IsTrue(), __LINE__);

    value1.Reset();
    /////////////////

    value2 = false;
    value1 += value2;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 1U, __LINE__);
    test.IsTrue(value1[0].IsFalse(), __LINE__);

    value2 = false;
    value1 += value2;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 2U, __LINE__);
    test.IsTrue(value1[0].IsFalse(), __LINE__);
    test.IsTrue(value1[1].IsFalse(), __LINE__);

    value1.Reset();
    /////////////////

    value2 = nullptr;
    value1 += value2;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 1U, __LINE__);
    test.IsTrue(value1[0].IsNull(), __LINE__);

    value2 = nullptr;
    value1 += value2;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 2U, __LINE__);
    test.IsTrue(value1[0].IsNull(), __LINE__);
    test.IsTrue(value1[1].IsNull(), __LINE__);

    value1.Reset();
    /////////////////
    str               = VString("-ABCDEF0123456789ABCDEF0123456789-");
    const char *c_str = str.First();
    value2            = Memory::Move(str);

    value1 += value2;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 1U, __LINE__);
    test.IsTrue(value1[0].IsString(), __LINE__);
    test.IsNotEqual(value1[0].StringStorage(), c_str, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);

    str                = VString("#0123456789ABCDEF0123456789ABCDEF#");
    const char *c_str2 = str.First();
    value2             = Memory::Move(str);

    value1 += value2;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 2U, __LINE__);
    test.IsTrue(value1[0].IsString(), __LINE__);
    test.IsTrue(value1[1].IsString(), __LINE__);
    test.IsNotEqual(value1[0].StringStorage(), c_str, __LINE__);
    test.IsNotEqual(value1[1].StringStorage(), c_str2, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1[1].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34), __LINE__);
    value1.Reset();
    /////////////////

    value2 = VArray(1);
    value1 += value2;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 1U, __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);

    value1.Reset();

    value2 = VArray(10);
    value1 += value2;
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 1U, __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);

    value1.Reset();

    value2 = VArray(1);
    value1.Merge(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 0U, __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);

    value2 = VArray(10);
    value1.Merge(Memory::Move(value2));
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 0U, __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);

    value1.Reset();
    ///

    arr_var += ValueC{ValueType::False};
    arr_var += ValueC{ValueType::True};
    arr_var += ValueC{VString("-ABCDEF0123456789ABCDEF0123456789-")};
    arr_storage = arr_var.First();
    c_str       = arr_storage[2].StringStorage();
    value2      = Memory::Move(arr_var);

    value1.Merge(Memory::Move(value2));
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 3U, __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    test.IsTrue(value1[0].IsFalse(), __LINE__);
    test.IsTrue(value1[1].IsTrue(), __LINE__);
    test.IsTrue(value1[2].IsString(), __LINE__);
    test.IsEqual(value1[2].StringStorage(), c_str, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1[2].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);

    arr_var.Reset();
    arr_var += ValueC{ValueType::Null};
    arr_var += ValueC{14};
    arr_var += ValueC{VString("^ABCDEF0123456789ABCDEF0123456789^")};
    arr_storage = arr_var.First();
    c_str2      = arr_storage[2].StringStorage();
    value2      = Memory::Move(arr_var);

    value1.Merge(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 6U, __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    test.IsTrue(value1[0].IsFalse(), __LINE__);
    test.IsTrue(value1[1].IsTrue(), __LINE__);
    test.IsTrue(value1[2].IsString(), __LINE__);
    test.IsEqual(value1[2].StringStorage(), c_str, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1[2].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsTrue(value1[3].IsNull(), __LINE__);
    test.IsTrue(value1[4].IsNumber(), __LINE__);
    test.IsEqual(value1[4].GetNumber(), 14.0, __LINE__);
    test.IsTrue(value1[5].IsString(), __LINE__);
    test.IsNotEqual(value1[5].StringStorage(), c_str2, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1[5].StringStorage(), "^ABCDEF0123456789ABCDEF0123456789^", 34), __LINE__);

    value1.Reset();

    //////////////////////////////////////////
}

static void TestAddition5(QTest &test) {
    ValueC        value1;
    ValueC        value2;
    VArray        arr_var;
    const ValueC *arr_storage;
    VString       str;

    /////////////////

    value2 = true;
    test.IsTrue(value2.IsTrue(), __LINE__);

    value1 += Memory::Move(value2);

    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 1U, __LINE__);
    test.IsTrue(value1[0].IsTrue(), __LINE__);

    value2 += true;
    value2 += false;
    value2 += nullptr;
    value2 += "v";
    value2 += 4;
    value2 += VArray(1);
    value2 += VHArray(1);

    value2 = true;
    value1 += Memory::Move(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 2U, __LINE__);
    test.IsTrue(value1[0].IsTrue(), __LINE__);
    test.IsTrue(value1[1].IsTrue(), __LINE__);

    value1.Reset();
    /////////////////

    value2 = false;

    value1 += Memory::Move(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 1U, __LINE__);
    test.IsTrue(value1[0].IsFalse(), __LINE__);

    value2 = false;
    value1 += Memory::Move(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 2U, __LINE__);
    test.IsTrue(value1[0].IsFalse(), __LINE__);
    test.IsTrue(value1[1].IsFalse(), __LINE__);

    value1.Reset();
    /////////////////

    value2 = nullptr;
    value1 += Memory::Move(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 1U, __LINE__);
    test.IsTrue(value1[0].IsNull(), __LINE__);

    value2 = nullptr;
    value1 += Memory::Move(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 2U, __LINE__);
    test.IsTrue(value1[0].IsNull(), __LINE__);
    test.IsTrue(value1[1].IsNull(), __LINE__);

    value1.Reset();
    /////////////////
    str               = VString("-ABCDEF0123456789ABCDEF0123456789-");
    const char *c_str = str.First();
    value2            = Memory::Move(str);

    value1 += Memory::Move(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 1U, __LINE__);
    test.IsTrue(value1[0].IsString(), __LINE__);
    test.IsEqual(value1[0].StringStorage(), c_str, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);

    str                = VString("#0123456789ABCDEF0123456789ABCDEF#");
    const char *c_str2 = str.First();
    value2             = Memory::Move(str);

    value1 += Memory::Move(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 2U, __LINE__);
    test.IsTrue(value1[0].IsString(), __LINE__);
    test.IsTrue(value1[1].IsString(), __LINE__);
    test.IsEqual(value1[0].StringStorage(), c_str, __LINE__);
    test.IsEqual(value1[1].StringStorage(), c_str2, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1[0].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1[1].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34), __LINE__);
    value1.Reset();
    /////////////////

    value2 = VArray(1);
    value1.Merge(value2);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 0U, __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value2.GetArray(), __LINE__);

    value2 = VArray(1);
    value1.Merge(Memory::Move(value2));
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 0U, __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNull(value2.GetArray(), __LINE__);

    value1.Reset();
    ///

    arr_var += ValueC{ValueType::False};
    arr_var += ValueC{ValueType::True};
    arr_var += ValueC{VString("-ABCDEF0123456789ABCDEF0123456789-")};
    arr_storage = arr_var.First();
    c_str       = arr_storage[2].StringStorage();
    value2      = Memory::Move(arr_var);

    value1.Merge(Memory::Move(value2));
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 3U, __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    test.IsTrue(value1[0].IsFalse(), __LINE__);
    test.IsTrue(value1[1].IsTrue(), __LINE__);
    test.IsTrue(value1[2].IsString(), __LINE__);
    test.IsEqual(value1[2].StringStorage(), c_str, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1[2].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);

    arr_var.Reset();
    arr_var += ValueC{ValueType::Null};
    arr_var += ValueC{14};
    arr_var += ValueC{VString("#0123456789ABCDEF0123456789ABCDEF#")};
    arr_storage = arr_var.First();
    c_str2      = arr_storage[2].StringStorage();
    value2      = Memory::Move(arr_var);

    value1.Merge(Memory::Move(value2));
    test.IsTrue(value2.IsUndefined(), __LINE__);
    test.IsTrue(value1.IsArray(), __LINE__);
    test.IsEqual(value1.Size(), 6U, __LINE__);
    test.IsNotNull(value1.GetArray(), __LINE__);
    test.IsNotNull(value1.GetArray()->First(), __LINE__);
    test.IsNotEqual(value1.GetArray()->First(), arr_storage, __LINE__);
    test.IsTrue(value1[0].IsFalse(), __LINE__);
    test.IsTrue(value1[1].IsTrue(), __LINE__);
    test.IsTrue(value1[2].IsString(), __LINE__);
    test.IsEqual(value1[2].StringStorage(), c_str, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1[2].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsTrue(value1[3].IsNull(), __LINE__);
    test.IsTrue(value1[4].IsNumber(), __LINE__);
    test.IsEqual(value1[4].GetNumber(), 14.0, __LINE__);
    test.IsTrue(value1[5].IsString(), __LINE__);
    test.IsEqual(value1[5].StringStorage(), c_str2, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value1[5].StringStorage(), "#0123456789ABCDEF0123456789ABCDEF#", 34), __LINE__);
    value1.Reset();
    //////////////////////////////////////////
}

static void TestAddition6(QTest &test) {
    ValueC            value1;
    ValueC            value2;
    VString           str_var;
    const char       *str_c1;
    const char       *str_c2;
    VHArray           h_arr_var;
    const ObjectItem *h_arr_storage1 = nullptr;
    const ObjectItem *h_arr_storage2 = nullptr;

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
    test.IsEqual(value2.Size(), 3U, __LINE__);
    test.IsNotNull(value2.GetObject(), __LINE__);
    test.IsNotEqual(value2.GetObject()->First(), h_arr_storage1, __LINE__);
    test.IsEqual(value2["k1"].GetNumber(), 11.0, __LINE__);
    test.IsEqual(value2["k2"].GetNumber(), 22.0, __LINE__);
    test.IsNotEqual(value2["k3"].StringStorage(), str_c1, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2["k3"].StringStorage(), "*ABCDEF0123456789ABCDEF0123456789*", 34), __LINE__);

    value2 = VHArray{};  // Clearing and  Setting to object type.
    value2 += h_arr_var; // Copy
    test.IsEqual(value2.Size(), 4U, __LINE__);
    test.IsNotNull(value2.GetObject(), __LINE__);
    test.IsNotEqual(value2.GetObject()->First(), h_arr_storage2, __LINE__);
    test.IsEqual(value2["w1"].GetNumber(), 10.0, __LINE__);
    test.IsEqual(value2["w2"].GetNumber(), 20.0, __LINE__);
    test.IsEqual(value2["w3"].GetNumber(), 30.0, __LINE__);
    test.IsNotEqual(value2["w4"].StringStorage(), str_c2, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2["w4"].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsNull(value2.GetValueAt(4), __LINE__);

    ////

    value2 = VHArray{};                 // Clearing and  Setting to object type.
    value2.Merge(Memory::Move(value1)); // Move
    test.IsTrue(value1.IsUndefined(), __LINE__);
    test.IsEqual(value2.Size(), 3U, __LINE__);
    test.IsNotNull(value2.GetObject(), __LINE__);
    test.IsNotEqual(value2.GetObject()->First(), h_arr_storage2, __LINE__);
    test.IsEqual(value2["k1"].GetNumber(), 11.0, __LINE__);
    test.IsEqual(value2["k2"].GetNumber(), 22.0, __LINE__);
    test.IsEqual(value2["k3"].StringStorage(), str_c1, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2["k3"].StringStorage(), "*ABCDEF0123456789ABCDEF0123456789*", 34), __LINE__);

    value1 = value2; // Copying back the values.
    str_c1 = value1["k3"].StringStorage();

    value2 = VHArray{};                // Clearing and  Setting to object type.
    value2 += Memory::Move(h_arr_var); // Move
    test.IsNull(h_arr_var.First(), __LINE__);
    test.IsEqual(value2.Size(), 4U, __LINE__);
    test.IsNotNull(value2.GetObject(), __LINE__);
    test.IsNotEqual(value2.GetObject()->First(), h_arr_storage2, __LINE__);
    test.IsEqual(value2["w1"].GetNumber(), 10.0, __LINE__);
    test.IsEqual(value2["w2"].GetNumber(), 20.0, __LINE__);
    test.IsEqual(value2["w3"].GetNumber(), 30.0, __LINE__);
    test.IsEqual(value2["w4"].StringStorage(), str_c2, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2["w4"].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsNull(value2.GetValueAt(4), __LINE__);

    if (value2.GetObject() != nullptr) {
        h_arr_var = *(value2.GetObject()); // Copying back the values.
        str_c2    = h_arr_var[VString("w4")].StringStorage();
    }

    ////////
    value2 = VHArray{};   // Clearing and  Setting to object type.
    value2.Merge(value1); // Copy
    value2 += h_arr_var;  // Copy
    test.IsEqual(value2.Size(), 7U, __LINE__);
    test.IsEqual(value2["w1"].GetNumber(), 10.0, __LINE__);
    test.IsEqual(value2["w2"].GetNumber(), 20.0, __LINE__);
    test.IsEqual(value2["w3"].GetNumber(), 30.0, __LINE__);
    test.IsNotEqual(value2["w4"].StringStorage(), str_c2, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2["w4"].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsEqual(value2["k1"].GetNumber(), 11.0, __LINE__);
    test.IsEqual(value2["k2"].GetNumber(), 22.0, __LINE__);
    test.IsNotEqual(value2["k3"].StringStorage(), str_c1, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2["k3"].StringStorage(), "*ABCDEF0123456789ABCDEF0123456789*", 34), __LINE__);

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

    test.IsEqual(value2["w0"].GetNumber(), 5.0, __LINE__);
    test.IsEqual(value2["w1"].GetNumber(), 10.0, __LINE__);
    test.IsEqual(value2["w2"].GetNumber(), 20.0, __LINE__);
    test.IsEqual(value2["w3"].GetNumber(), 30.0, __LINE__);
    test.IsNotEqual(value2["w4"].StringStorage(), str_c2, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2["w4"].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsEqual(value2["w5"].GetNumber(), 500.0, __LINE__);
    test.IsEqual(value2["w6"].GetNumber(), 600.0, __LINE__);
    test.IsEqual(value2["k1"].GetNumber(), 11.0, __LINE__);
    test.IsEqual(value2["k2"].GetNumber(), 22.0, __LINE__);
    test.IsNotEqual(value2["k3"].StringStorage(), str_c1, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2["k3"].StringStorage(), "*ABCDEF0123456789ABCDEF0123456789*", 34), __LINE__);

    value2       = VHArray{}; // Clearing and  Setting to object type.
    value2["w0"] = 5;
    value2["w1"] = 100;
    value2["w2"] = 200;
    value2["w4"] = 400;
    value2["w5"] = 500;
    value2["w6"] = 600;
    value2.Merge(Memory::Move(value1)); // Move
    value2 += Memory::Move(h_arr_var);  // Move
    test.IsEqual(value2["w0"].GetNumber(), 5.0, __LINE__);
    test.IsEqual(value2["w1"].GetNumber(), 10.0, __LINE__);
    test.IsEqual(value2["w2"].GetNumber(), 20.0, __LINE__);
    test.IsEqual(value2["w3"].GetNumber(), 30.0, __LINE__);
    test.IsEqual(value2["w4"].StringStorage(), str_c2, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2["w4"].StringStorage(), "-ABCDEF0123456789ABCDEF0123456789-", 34), __LINE__);
    test.IsEqual(value2["w5"].GetNumber(), 500.0, __LINE__);
    test.IsEqual(value2["w6"].GetNumber(), 600.0, __LINE__);
    test.IsEqual(value2["k1"].GetNumber(), 11.0, __LINE__);
    test.IsEqual(value2["k2"].GetNumber(), 22.0, __LINE__);
    test.IsEqual(value2["k3"].StringStorage(), str_c1, __LINE__);
    test.IsTrue(StringUtils::IsEqual(value2["k3"].StringStorage(), "*ABCDEF0123456789ABCDEF0123456789*", 34), __LINE__);
    /////////////////////////////

    value1.Reset();
    value2.Reset();

    value1["k1"]  = 10;
    value1["k2"]  = 2;
    value1["k3"]  = 30;
    value1["k7"]  = 7;
    value1["k8"]  = 80;
    value1["k10"] = 100;

    ValueC value3 = value1;

    value2["k0"] = 99;
    value2["k1"] = 1;
    value2["k3"] = 3;
    value2["k4"] = 4;
    value2["k5"] = 5;
    value2["k6"] = 6;
    value2["k8"] = 8;
    value2["k9"] = 9;

    value1 += value2;

    test.IsTrue(value2.IsObject(), __LINE__);
    test.IsEqual(value2.Size(), 8U, __LINE__);
    test.IsEqual(value2["k0"].GetNumber(), 99, __LINE__);
    test.IsEqual(value2["k1"].GetNumber(), 1, __LINE__);
    test.IsEqual(value2["k3"].GetNumber(), 3, __LINE__);
    test.IsEqual(value2["k4"].GetNumber(), 4, __LINE__);
    test.IsEqual(value2["k5"].GetNumber(), 5, __LINE__);
    test.IsEqual(value2["k6"].GetNumber(), 6, __LINE__);
    test.IsEqual(value2["k8"].GetNumber(), 8, __LINE__);
    test.IsEqual(value2["k9"].GetNumber(), 9, __LINE__);

    test.IsTrue(value1.IsObject(), __LINE__);
    test.IsEqual(value1.Size(), 11U, __LINE__);
    test.IsEqual(value1["k0"].GetNumber(), 99, __LINE__);
    test.IsEqual(value1["k1"].GetNumber(), 1, __LINE__);
    test.IsEqual(value1["k2"].GetNumber(), 2, __LINE__);
    test.IsEqual(value1["k3"].GetNumber(), 3, __LINE__);
    test.IsEqual(value1["k4"].GetNumber(), 4, __LINE__);
    test.IsEqual(value1["k5"].GetNumber(), 5, __LINE__);
    test.IsEqual(value1["k6"].GetNumber(), 6, __LINE__);
    test.IsEqual(value1["k7"].GetNumber(), 7, __LINE__);
    test.IsEqual(value1["k8"].GetNumber(), 8, __LINE__);
    test.IsEqual(value1["k9"].GetNumber(), 9, __LINE__);
    test.IsEqual(value1["k10"].GetNumber(), 100, __LINE__);

    value3 += Memory::Move(value2);

    test.IsEqual(value2.Size(), 0U, __LINE__);
    test.IsTrue(value2.IsUndefined(), __LINE__);
    test.IsTrue(value3.IsObject(), __LINE__);
    test.IsEqual(value3.Size(), 11U, __LINE__);
    test.IsEqual(value3["k0"].GetNumber(), 99, __LINE__);
    test.IsEqual(value3["k1"].GetNumber(), 1, __LINE__);
    test.IsEqual(value3["k2"].GetNumber(), 2, __LINE__);
    test.IsEqual(value3["k3"].GetNumber(), 3, __LINE__);
    test.IsEqual(value3["k4"].GetNumber(), 4, __LINE__);
    test.IsEqual(value3["k5"].GetNumber(), 5, __LINE__);
    test.IsEqual(value3["k6"].GetNumber(), 6, __LINE__);
    test.IsEqual(value3["k7"].GetNumber(), 7, __LINE__);
    test.IsEqual(value3["k8"].GetNumber(), 8, __LINE__);
    test.IsEqual(value3["k9"].GetNumber(), 9, __LINE__);
    test.IsEqual(value3["k10"].GetNumber(), 100, __LINE__);
}

static void TestStringify1(QTest &test) {
    StringStream<char> ss;
    ValueC             value;

    ///////////
    value = VArray{};
    test.IsEqual(value.Stringify(ss), "[]", __LINE__);
    ss.Clear();

    value.Reset();
    value += true;
    test.IsEqual(value.Stringify(ss), "[true]", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    test.IsEqual(value.Stringify(ss), "[false]", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    test.IsEqual(value.Stringify(ss), "[null]", __LINE__);
    ss.Clear();

    value.Reset();
    value += 0;
    test.IsEqual(value.Stringify(ss), "[0]", __LINE__);
    ss.Clear();

    value.Reset();
    value += "a";
    test.IsEqual(value.Stringify(ss), R"(["a"])", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    test.IsEqual(value.Stringify(ss), R"(["ABC"])", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    test.IsEqual(value.Stringify(ss), "[[]]", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    test.IsEqual(value.Stringify(ss), "[{}]", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += true;
    test.IsEqual(value.Stringify(ss), "[true,true]", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += true;
    test.IsEqual(value.Stringify(ss), "[false,true]", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += true;
    test.IsEqual(value.Stringify(ss), "[null,true]", __LINE__);
    ss.Clear();

    value.Reset();
    value += -3;
    value += true;
    test.IsEqual(value.Stringify(ss), "[-3,true]", __LINE__);
    ss.Clear();

    value.Reset();
    value += "a";
    value += true;
    test.IsEqual(value.Stringify(ss), R"(["a",true])", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += true;
    test.IsEqual(value.Stringify(ss), R"(["ABC",true])", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += true;
    test.IsEqual(value.Stringify(ss), "[[],true]", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += true;
    test.IsEqual(value.Stringify(ss), "[{},true]", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += false;
    test.IsEqual(value.Stringify(ss), "[true,false]", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += false;
    test.IsEqual(value.Stringify(ss), "[false,false]", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += false;
    test.IsEqual(value.Stringify(ss), "[null,false]", __LINE__);
    ss.Clear();

    value.Reset();
    value += 500;
    value += false;
    test.IsEqual(value.Stringify(ss), "[500,false]", __LINE__);
    ss.Clear();

    value.Reset();
    value += VString("a");
    value += false;
    test.IsEqual(value.Stringify(ss), R"(["a",false])", __LINE__);
    ss.Clear();

    value.Reset();
    const VString str("ABC");
    value += str;
    value += false;
    test.IsEqual(value.Stringify(ss), R"(["ABC",false])", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += false;
    test.IsEqual(value.Stringify(ss), "[[],false]", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += false;
    test.IsEqual(value.Stringify(ss), "[{},false]", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += nullptr;
    test.IsEqual(value.Stringify(ss), "[true,null]", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += nullptr;
    test.IsEqual(value.Stringify(ss), "[false,null]", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += nullptr;
    test.IsEqual(value.Stringify(ss), "[null,null]", __LINE__);
    ss.Clear();

    value.Reset();
    value += 456.5;
    value += nullptr;
    test.IsEqual(value.Stringify(ss), "[456.5,null]", __LINE__);
    ss.Clear();

    value.Reset();
    value += "a";
    value += nullptr;
    test.IsEqual(value.Stringify(ss), R"(["a",null])", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += nullptr;
    test.IsEqual(value.Stringify(ss), R"(["ABC",null])", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += nullptr;
    test.IsEqual(value.Stringify(ss), "[[],null]", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += nullptr;
    test.IsEqual(value.Stringify(ss), "[{},null]", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += "A";
    test.IsEqual(value.Stringify(ss), R"([true,"A"])", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += "AB";
    test.IsEqual(value.Stringify(ss), R"([false,"AB"])", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += "ABC";
    test.IsEqual(value.Stringify(ss), R"([null,"ABC"])", __LINE__);
    ss.Clear();

    value.Reset();
    value += -8.9;
    value += "ABC";
    test.IsEqual(value.Stringify(ss, 2U), R"([-8.9,"ABC"])", __LINE__);
    ss.Clear();

    value.Reset();
    value += "a";
    value += "ABCD";
    test.IsEqual(value.Stringify(ss), R"(["a","ABCD"])", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += "ABCDE";
    test.IsEqual(value.Stringify(ss), R"(["ABC","ABCDE"])", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += "ABCDEF";
    test.IsEqual(value.Stringify(ss), R"([[],"ABCDEF"])", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += "ABCDEFG";
    test.IsEqual(value.Stringify(ss), R"([{},"ABCDEFG"])", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += VArray{};
    test.IsEqual(value.Stringify(ss), "[true,[]]", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += VArray{};
    test.IsEqual(value.Stringify(ss), "[false,[]]", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += VArray{};
    test.IsEqual(value.Stringify(ss), "[null,[]]", __LINE__);
    ss.Clear();

    value.Reset();
    value += 10000;
    value += VArray{};
    test.IsEqual(value.Stringify(ss), "[10000,[]]", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += VArray{};
    test.IsEqual(value.Stringify(ss), R"(["ABC",[]])", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += VArray{};
    test.IsEqual(value.Stringify(ss), "[[],[]]", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += VArray{};
    test.IsEqual(value.Stringify(ss), "[{},[]]", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += VHArray{};
    test.IsEqual(value.Stringify(ss), "[true,{}]", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += VHArray{};
    test.IsEqual(value.Stringify(ss), "[false,{}]", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += VHArray{};
    test.IsEqual(value.Stringify(ss), "[null,{}]", __LINE__);
    ss.Clear();

    value.Reset();
    value += -1000;
    value += VHArray{};
    test.IsEqual(value.Stringify(ss), "[-1000,{}]", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += VHArray{};
    test.IsEqual(value.Stringify(ss), R"(["ABC",{}])", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += VHArray{};
    test.IsEqual(value.Stringify(ss), "[[],{}]", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += VHArray{};
    test.IsEqual(value.Stringify(ss), "[{},{}]", __LINE__);
    ss.Clear();
    ///////////

    value.Reset();
    value += true;
    value += VHArray{};
    value += false;
    test.IsEqual(value.Stringify(ss), "[true,{},false]", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += VArray{};
    value += nullptr;
    test.IsEqual(value.Stringify(ss), "[false,[],null]", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += VHArray{};
    value += "ABC";
    test.IsEqual(value.Stringify(ss), R"([null,{},"ABC"])", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += 789;
    value += "ABC";
    test.IsEqual(value.Stringify(ss), R"([null,789,"ABC"])", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += VArray{};
    value += VHArray{};
    test.IsEqual(value.Stringify(ss), R"(["ABC",[],{}])", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += VHArray{};
    value[2] = 498;
    test.IsEqual(value.Stringify(ss), "[[],{},498]", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += VArray{};
    value += true;
    test.IsEqual(value.Stringify(ss), "[{},[],true]", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value += true;
    value += VHArray{};
    value += 0;
    value += VArray{};
    test.IsEqual(value.Stringify(ss), "[true,{},0,[]]", __LINE__);
    ss.Clear();

    value.Reset();
    value += false;
    value += VArray{};
    value += nullptr;
    value += VHArray{};
    test.IsEqual(value.Stringify(ss), "[false,[],null,{}]", __LINE__);
    ss.Clear();

    value.Reset();
    value += nullptr;
    value += VHArray{};
    value += "ABC";
    value += VArray{};
    test.IsEqual(value.Stringify(ss), R"([null,{},"ABC",[]])", __LINE__);
    ss.Clear();

    value.Reset();
    value += "ABC";
    value += VArray{};
    value += VHArray{};
    value += nullptr;
    test.IsEqual(value.Stringify(ss), R"(["ABC",[],{},null])", __LINE__);
    ss.Clear();

    value.Reset();
    value += VArray{};
    value += false;
    value += VHArray{};
    value += true;
    test.IsEqual(value.Stringify(ss), "[[],false,{},true]", __LINE__);
    ss.Clear();

    value.Reset();
    value += VHArray{};
    value += nullptr;
    value += VArray{};
    value += VArray{};
    test.IsEqual(value.Stringify(ss), "[{},null,[],[]]", __LINE__);
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
    test.IsEqual(value.Stringify(ss), R"([true,false,null,123,"ABC",[],{}])", __LINE__);
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
    test.IsEqual(value.Stringify(ss), R"([{},[],"a",1.5,null,false,true])", __LINE__);
    ///////////////////////////////////////
}

static void TestStringify2(QTest &test) {
    StringStream<char> ss;
    ValueC             value;

    ///////////
    value = VHArray{};
    test.IsEqual(value.Stringify(ss), R"({})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = true;
    test.IsEqual(value.Stringify(ss), R"({"A":true})", __LINE__);
    ss.Clear();

    value.Reset();
    value["B"] = false;
    test.IsEqual(value.Stringify(ss), R"({"B":false})", __LINE__);
    ss.Clear();

    value.Reset();
    value["AA"] = nullptr;
    test.IsEqual(value.Stringify(ss), R"({"AA":null})", __LINE__);
    ss.Clear();

    value.Reset();
    value["a"] = 0;
    test.IsEqual(value.Stringify(ss), R"({"a":0})", __LINE__);
    ss.Clear();

    value.Reset();
    value["abc"] = "a";
    test.IsEqual(value.Stringify(ss), R"({"abc":"a"})", __LINE__);
    ss.Clear();

    value.Reset();
    value["ab"] = "ABC";
    test.IsEqual(value.Stringify(ss), R"({"ab":"ABC"})", __LINE__);
    ss.Clear();

    value.Reset();
    value["ABC"] = VArray{};
    test.IsEqual(value.Stringify(ss), R"({"ABC":[]})", __LINE__);
    ss.Clear();

    value.Reset();
    value["5vn7b83y98t3wrupwmwa4ataw"] = VHArray{};
    test.IsEqual(value.Stringify(ss), R"({"5vn7b83y98t3wrupwmwa4ataw":{}})", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["A"] = true;
    value["B"] = true;
    test.IsEqual(value.Stringify(ss), R"({"A":true,"B":true})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = false;
    value["b"] = true;
    test.IsEqual(value.Stringify(ss), R"({"A":false,"b":true})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"]  = nullptr;
    value["BC"] = true;
    test.IsEqual(value.Stringify(ss), R"({"A":null,"BC":true})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"]  = -3;
    value["AB"] = true;
    test.IsEqual(value.Stringify(ss), R"({"A":-3,"AB":true})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"]   = "a";
    value["ABC"] = true;
    test.IsEqual(value.Stringify(ss), R"({"A":"a","ABC":true})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = "ABC";
    value["1"] = true;
    test.IsEqual(value.Stringify(ss), R"({"A":"ABC","1":true})", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"]   = VArray{};
    value["123"] = true;
    test.IsEqual(value.Stringify(ss), R"({"X":[],"123":true})", __LINE__);
    ss.Clear();

    value.Reset();
    value["x"] = VHArray{};
    value["A"] = true;
    test.IsEqual(value.Stringify(ss), R"({"x":{},"A":true})", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["A2"] = true;
    value["A1"] = false;
    test.IsEqual(value.Stringify(ss), R"({"A2":true,"A1":false})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A-"]   = false;
    value["A123"] = false;
    test.IsEqual(value.Stringify(ss), R"({"A-":false,"A123":false})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = nullptr;
    value["B"] = false;
    test.IsEqual(value.Stringify(ss), R"({"A":null,"B":false})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = 500;
    value["B"] = false;
    test.IsEqual(value.Stringify(ss), R"({"A":500,"B":false})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = VString("a");
    value["B"] = false;
    test.IsEqual(value.Stringify(ss), R"({"A":"a","B":false})", __LINE__);
    ss.Clear();

    value.Reset();
    const VString str("ABC");
    value["A"] = str;
    value["B"] = false;
    test.IsEqual(value.Stringify(ss), R"({"A":"ABC","B":false})", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"] = VArray{};
    value["A"] = false;
    test.IsEqual(value.Stringify(ss), R"({"X":[],"A":false})", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"] = VHArray{};
    value["A"] = false;
    test.IsEqual(value.Stringify(ss), R"({"X":{},"A":false})", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["A"] = true;
    value["W"] = nullptr;
    test.IsEqual(value.Stringify(ss), R"({"A":true,"W":null})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = false;
    value["@"] = nullptr;
    test.IsEqual(value.Stringify(ss), R"({"A":false,"@":null})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = nullptr;
    value["#"] = nullptr;
    test.IsEqual(value.Stringify(ss), R"({"A":null,"#":null})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = 456.5;
    value["H"] = nullptr;
    test.IsEqual(value.Stringify(ss), R"({"A":456.5,"H":null})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = "a";
    value["Q"] = nullptr;
    test.IsEqual(value.Stringify(ss), R"({"A":"a","Q":null})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = "ABC";
    value["e"] = nullptr;
    test.IsEqual(value.Stringify(ss), R"({"A":"ABC","e":null})", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"] = VArray{};
    value["n"] = nullptr;
    test.IsEqual(value.Stringify(ss), R"({"X":[],"n":null})", __LINE__);
    ss.Clear();

    value.Reset();
    value["t"] = VHArray{};
    value["A"] = nullptr;
    test.IsEqual(value.Stringify(ss), R"({"t":{},"A":null})", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["e"] = true;
    value["A"] = "A";
    test.IsEqual(value.Stringify(ss), R"({"e":true,"A":"A"})", __LINE__);
    ss.Clear();

    value.Reset();
    value["m"] = false;
    value["A"] = "AB";
    test.IsEqual(value.Stringify(ss), R"({"m":false,"A":"AB"})", __LINE__);
    ss.Clear();

    value.Reset();
    value["i"] = nullptr;
    value["A"] = "ABC";
    test.IsEqual(value.Stringify(ss), R"({"i":null,"A":"ABC"})", __LINE__);
    ss.Clear();

    value.Reset();
    value["s"] = -8.9;
    value["A"] = "ABC";
    test.IsEqual(value.Stringify(ss, 2U), R"({"s":-8.9,"A":"ABC"})", __LINE__);
    ss.Clear();

    value.Reset();
    value["-"] = "a";
    value["A"] = "ABCD";
    test.IsEqual(value.Stringify(ss), R"({"-":"a","A":"ABCD"})", __LINE__);
    ss.Clear();

    value.Reset();
    value["g"] = "ABC";
    value["A"] = "ABCDE";
    test.IsEqual(value.Stringify(ss), R"({"g":"ABC","A":"ABCDE"})", __LINE__);
    ss.Clear();

    value.Reset();
    value["o"] = VArray{};
    value["A"] = "ABCDEF";
    test.IsEqual(value.Stringify(ss), R"({"o":[],"A":"ABCDEF"})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = VHArray{};
    value["o"] = "ABCDEFG";
    test.IsEqual(value.Stringify(ss), R"({"A":{},"o":"ABCDEFG"})", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["d"] = true;
    value["y"] = VArray{};
    test.IsEqual(value.Stringify(ss), R"({"d":true,"y":[]})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = false;
    value["y"] = VArray{};
    test.IsEqual(value.Stringify(ss), R"({"A":false,"y":[]})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = nullptr;
    value["y"] = VArray{};
    test.IsEqual(value.Stringify(ss), R"({"A":null,"y":[]})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = 10000;
    value["y"] = VArray{};
    test.IsEqual(value.Stringify(ss), R"({"A":10000,"y":[]})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = "ABC";
    value["y"] = VArray{};
    test.IsEqual(value.Stringify(ss), R"({"A":"ABC","y":[]})", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"] = VArray{};
    value["y"] = VArray{};
    test.IsEqual(value.Stringify(ss), R"({"X":[],"y":[]})", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"] = VHArray{};
    value["Y"] = VArray{};
    test.IsEqual(value.Stringify(ss), R"({"X":{},"Y":[]})", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["A"] = true;
    value["y"] = VHArray{};
    test.IsEqual(value.Stringify(ss), R"({"A":true,"y":{}})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = false;
    value["y"] = VHArray{};
    test.IsEqual(value.Stringify(ss), R"({"A":false,"y":{}})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = nullptr;
    value["y"] = VHArray{};
    test.IsEqual(value.Stringify(ss), R"({"A":null,"y":{}})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = -1000;
    value["y"] = VHArray{};
    test.IsEqual(value.Stringify(ss), R"({"A":-1000,"y":{}})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = "ABC";
    value["y"] = VHArray{};
    test.IsEqual(value.Stringify(ss), R"({"A":"ABC","y":{}})", __LINE__);
    ss.Clear();

    value.Reset();
    value["C"] = VArray{};
    value["R"] = VHArray{};
    test.IsEqual(value.Stringify(ss), R"({"C":[],"R":{}})", __LINE__);
    ss.Clear();

    value.Reset();
    value["cc"] = VHArray{};
    value["rr"] = VHArray{};
    test.IsEqual(value.Stringify(ss), R"({"cc":{},"rr":{}})", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["A"]  = true;
    value["y"]  = VHArray{};
    value["AA"] = false;
    test.IsEqual(value.Stringify(ss), R"({"A":true,"y":{},"AA":false})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"] = false;
    value["y"] = VArray{};
    value["B"] = nullptr;
    test.IsEqual(value.Stringify(ss), R"({"A":false,"y":[],"B":null})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"]   = nullptr;
    value["y"]   = VHArray{};
    value["ABC"] = "ABC";
    test.IsEqual(value.Stringify(ss), R"({"A":null,"y":{},"ABC":"ABC"})", __LINE__);
    ss.Clear();

    value.Reset();
    value["t"] = nullptr;
    value["Y"] = 789;
    value["A"] = "ABC";
    test.IsEqual(value.Stringify(ss), R"({"t":null,"Y":789,"A":"ABC"})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"]     = "ABC";
    value["y"]     = VArray{};
    value["key-u"] = VHArray{};
    test.IsEqual(value.Stringify(ss), R"({"A":"ABC","y":[],"key-u":{}})", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"]     = VArray{};
    value["Y"]     = VHArray{};
    value["key-u"] = 498;
    test.IsEqual(value.Stringify(ss), R"({"X":[],"Y":{},"key-u":498})", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"] = VHArray{};
    value["y"] = VArray{};
    value["A"] = true;
    test.IsEqual(value.Stringify(ss), R"({"X":{},"y":[],"A":true})", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value["{}}"]   = true;
    value["y"]     = VHArray{};
    value["AA"]    = 0;
    value["k-300"] = VArray{};
    test.IsEqual(value.Stringify(ss), R"({"{}}":true,"y":{},"AA":0,"k-300":[]})", __LINE__);
    ss.Clear();

    value.Reset();
    value["B1"]    = false;
    value["y"]     = VArray{};
    value["[A]"]   = nullptr;
    value["k-300"] = VHArray{};
    test.IsEqual(value.Stringify(ss), R"({"B1":false,"y":[],"[A]":null,"k-300":{}})", __LINE__);
    ss.Clear();

    value.Reset();
    value["{A}"]   = nullptr;
    value["y"]     = VHArray{};
    value["AA"]    = "ABC";
    value["k-300"] = VArray{};
    test.IsEqual(value.Stringify(ss), R"({"{A}":null,"y":{},"AA":"ABC","k-300":[]})", __LINE__);
    ss.Clear();

    value.Reset();
    value["x"]     = "ABC";
    value["[]]"]   = VArray{};
    value["key-u"] = VHArray{};
    value["A"]     = nullptr;
    test.IsEqual(value.Stringify(ss), R"({"x":"ABC","[]]":[],"key-u":{},"A":null})", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"]       = VArray{};
    value["CCCCccc"] = false;
    value["key-u"]   = VHArray{};
    value["A"]       = true;
    test.IsEqual(value.Stringify(ss), R"({"X":[],"CCCCccc":false,"key-u":{},"A":true})", __LINE__);
    ss.Clear();

    value.Reset();
    value["X"]     = VHArray{};
    value["A"]     = nullptr;
    value["key-u"] = VArray{};
    value["k-300"] = VArray{};
    test.IsEqual(value.Stringify(ss), R"({"X":{},"A":null,"key-u":[],"k-300":[]})", __LINE__);

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
    test.IsEqual(value.Stringify(ss), R"({"A":true,"B":false,"C":null,"D":123,"E":"ABC","F":[],"G":{}})", __LINE__);

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
    test.IsEqual(value.Stringify(ss),
                 R"({"A":{},"BB":[],"CCC":"a","DDDD":1.5,"EEEEE":null,"FFFFFF":false,"GGGGGGG":true})", __LINE__);
    ///////////////////////////////////////
}

static void TestStringify3(QTest &test) {
    StringStream<char> ss;
    ValueC             value;

    value[0] += true;
    value[0] += false;
    value[0] += nullptr;
    value[0] += 0;
    value[0] += "ABC";
    value[0] += VArray{};
    value[0] += VHArray{};
    test.IsEqual(value.Stringify(ss), R"([[true,false,null,0,"ABC",[],{}]])", __LINE__);
    ss.Clear();

    value         = VArray{};
    value[0]["a"] = true;
    value[0]["0"] = false;
    value[0]["1"] = nullptr;
    value[0]["V"] = 0;
    value[0]["B"] = "a";
    value[0]["2"] = VArray{};
    value[0]["6"] = VHArray{};
    test.IsEqual(value.Stringify(ss), R"([{"a":true,"0":false,"1":null,"V":0,"B":"a","2":[],"6":{}}])", __LINE__);
    ss.Clear();

    value.Reset();
    value["o"] += true;
    value["o"] += false;
    value["o"] += nullptr;
    value["o"] += 0;
    value["o"] += "ABC";
    value["o"] += VArray{};
    value["o"] += VHArray{};
    test.IsEqual(value.Stringify(ss), R"({"o":[true,false,null,0,"ABC",[],{}]})", __LINE__);
    ss.Clear();

    value.Reset();
    value["i"]["a"] = true;
    value["i"]["0"] = false;
    value["i"]["1"] = nullptr;
    value["i"]["V"] = 0;
    value["i"]["B"] = "a";
    value["i"]["2"] = VArray{};
    value["i"]["6"] = VHArray{};
    test.IsEqual(value.Stringify(ss), R"({"i":{"a":true,"0":false,"1":null,"V":0,"B":"a","2":[],"6":{}}})", __LINE__);
}

static void TestStringify4(QTest &test) {
    StringStream<char> ss;
    ValueC             value;

    value["\"\\/\b\f\n\r\t"] = "\t\r\n\f\b/\\\"";
    test.IsEqual(value.Stringify(ss), R"({"\"\\\/\b\f\n\r\t":"\t\r\n\f\b\/\\\""})", __LINE__);
    ss.Clear();

    value.Reset();
    value += "\"\\/\b\f\n\r\t";
    test.IsEqual(value.Stringify(ss), R"(["\"\\\/\b\f\n\r\t"])", __LINE__);
}

static void TestDeleteValue(QTest &test) {
    StringStream<char> ss;
    ValueC             value;

    value[0] = 1;
    value.RemoveIndex(0);
    test.IsNull(value.GetValue(0), __LINE__);
    test.IsEqual(value.Stringify(ss), R"([])", __LINE__);
    ss.Clear();

    value[0] = "c";
    value.RemoveIndex(SizeT32{0});
    test.IsNull(value.GetValue(0), __LINE__);
    test.IsEqual(value.Stringify(ss), R"([])", __LINE__);
    ss.Clear();

    value[0] = VArray{};
    value.RemoveIndex(SizeT64{0});
    test.IsEqual(value.Stringify(ss), R"([])", __LINE__);
    ss.Clear();

    value[0] = false;
    value[1] = true;
    value.RemoveIndex(0);
    test.IsEqual(value.Stringify(ss), R"([true])", __LINE__);
    ss.Clear();
    value.RemoveIndex(1);
    test.IsEqual(value.Stringify(ss), R"([])", __LINE__);
    ss.Clear();

    value[0] = "abc";
    value[1] = nullptr;
    value.RemoveIndex(1);
    test.IsEqual(value.Stringify(ss), R"(["abc"])", __LINE__);
    ss.Clear();
    value.RemoveIndex(0);
    test.IsEqual(value.Stringify(ss), R"([])", __LINE__);
    ss.Clear();

    value[0] = false;
    value[1] = true;
    value[2] = nullptr;
    value.RemoveIndex(1);
    test.IsEqual(value.Stringify(ss), R"([false,null])", __LINE__);
    ss.Clear();
    value.RemoveIndex(0);
    test.IsEqual(value.Stringify(ss), R"([null])", __LINE__);
    ss.Clear();

    value.RemoveIndex(2);
    test.IsEqual(value.Stringify(ss), R"([])", __LINE__);
    ss.Clear();

    value[0] = false;
    value[1] = true;
    value[2] = nullptr;
    value[1].Reset();
    test.IsEqual(value.Stringify(ss), R"([false,null])", __LINE__);
    ss.Clear();
    value[0].Reset();
    test.IsEqual(value.Stringify(ss), R"([null])", __LINE__);
    ss.Clear();

    value[2].Reset();
    test.IsEqual(value.Stringify(ss), R"([])", __LINE__);
    ss.Clear();

    value[0] = "a";
    value[1] = VArray{};
    value[2] = VHArray{};
    value.RemoveIndex(2);
    test.IsEqual(value.Stringify(ss), R"(["a",[]])", __LINE__);
    ss.Clear();
    value.RemoveIndex(1);
    test.IsEqual(value.Stringify(ss), R"(["a"])", __LINE__);
    ss.Clear();

    value.RemoveIndex(0);
    test.IsEqual(value.Stringify(ss), R"([])", __LINE__);
    ss.Clear();
    /////////

    value.Reset();

    value["A"] = 1;
    value.RemoveIndex(0);
    test.IsEqual(value.Stringify(ss), R"({})", __LINE__);
    ss.Clear();

    value["A"] = "c";
    value.Remove("A");
    test.IsEqual(value.Stringify(ss), R"({})", __LINE__);
    ss.Clear();

    value["A"] = VArray{};
    value.Remove("A");
    test.IsEqual(value.Stringify(ss), R"({})", __LINE__);
    ss.Clear();

    value.Reset();
    value["A"]  = false;
    value["bb"] = true;
    value.Remove("A");
    test.IsNull(value.GetKey(0), __LINE__);
    test.IsNull(value.GetValueAt(0), __LINE__);
    test.IsEqual(value.Stringify(ss), R"({"bb":true})", __LINE__);
    ss.Clear();
    value.RemoveIndex(1);
    test.IsNull(value.GetKey(0), __LINE__);
    test.IsNull(value.GetValueAt(1), __LINE__);
    test.IsEqual(value.Stringify(ss), R"({})", __LINE__);
    ss.Clear();

    value["A"]  = "abc";
    value["bb"] = nullptr;
    value.Remove("bb");
    test.IsEqual(value.Stringify(ss), R"({"A":"abc"})", __LINE__);
    ss.Clear();
    value.Remove("A");
    test.IsEqual(value.Stringify(ss), R"({})", __LINE__);
    ss.Clear();

    value["A"]   = false;
    value["bb"]  = true;
    value["AAA"] = nullptr;
    value.Remove("bb");
    test.IsEqual(value.Stringify(ss), R"({"A":false,"AAA":null})", __LINE__);
    ss.Clear();
    value.Remove("A");
    test.IsEqual(value.Stringify(ss), R"({"AAA":null})", __LINE__);
    ss.Clear();

    value.Remove("AAA");
    test.IsEqual(value.Stringify(ss), R"({})", __LINE__);
    ss.Clear();

    value["A"]   = "a";
    value["bb"]  = VHArray{};
    value["AAA"] = VArray{};
    value.Remove("AAA");
    test.IsEqual(value.Stringify(ss), R"({"A":"a","bb":{}})", __LINE__);
    ss.Clear();
    value.Remove("bb");
    test.IsEqual(value.Stringify(ss), R"({"A":"a"})", __LINE__);
    ss.Clear();

    value.Remove("A");
    test.IsEqual(value.Stringify(ss), R"({})", __LINE__);
    ss.Clear();

    value["A"]   = "a";
    value["bb"]  = VHArray{};
    value["AAA"] = VArray{};
    value["AAA"].Reset();
    test.IsEqual(value.Stringify(ss), R"({"A":"a","bb":{}})", __LINE__);
    ss.Clear();
    value["bb"].Reset();
    test.IsEqual(value.Stringify(ss), R"({"A":"a"})", __LINE__);
    ss.Clear();

    value["A"].Reset();
    test.IsEqual(value.Stringify(ss), R"({})", __LINE__);
}

static void TestCompressValue(QTest &test) {
    ValueC value;

    value[0] = 1;
    value[1] = 1;
    value[2] = 1;
    value[3] = 1;
    value[4] = 1;

    const VArray *arr = value.GetArray();

    test.IsNotNull(arr, __LINE__);
    if (arr != nullptr) {
        test.IsTrue(arr->Capacity() > arr->Size(), __LINE__);
    }

    value.Compress();

    arr = value.GetArray();

    test.IsNotNull(arr, __LINE__);
    if (arr != nullptr) {
        test.IsEqual(arr->Capacity(), arr->Size(), __LINE__);
    }

    value.Reset();

    value["key1"][0] = 1;

    value["key2"][0] = 1;
    value["key2"][1] = 1;
    value["key2"][2] = 1;

    value["key3"][0] = 1;
    value["key3"][1] = 1;
    value["key3"][2] = 1;
    value["key3"][3] = 1;
    value["key3"][4] = 1;

    arr = value["key1"].GetArray();
    test.IsNotNull(arr, __LINE__);
    if (arr != nullptr) {
        test.IsTrue(arr->Capacity() > arr->Size(), __LINE__);
    }
    arr = value["key2"].GetArray();
    test.IsNotNull(arr, __LINE__);
    if (arr != nullptr) {
        test.IsTrue(arr->Capacity() > arr->Size(), __LINE__);
    }
    arr = value["key3"].GetArray();
    test.IsNotNull(arr, __LINE__);
    if (arr != nullptr) {
        test.IsTrue(arr->Capacity() > arr->Size(), __LINE__);
    }

    value["key4"][0] = 1;
    value["key5"][0] = 1;
    value["key6"][0] = 1;
    value["key7"][0] = 1;
    value["key8"][0] = 1;
    value["key9"][0] = 1;

    value.Remove("key4");
    value.Remove("key5");
    value.Remove("key6");
    value.Remove("key7");
    value.Remove("key8");
    value.Remove("key9");

    const VHArray *obj = value.GetObject();
    test.IsNotNull(obj, __LINE__);
    if (obj != nullptr) {
        test.IsTrue(obj->Size() > obj->ActualSize(), __LINE__);
    }
    value.Compress();

    obj = value.GetObject();
    test.IsNotNull(obj, __LINE__);
    if (obj != nullptr) {
        test.IsEqual(obj->Size(), obj->ActualSize(), __LINE__);
    }

    arr = value["key1"].GetArray();
    test.IsNotNull(arr, __LINE__);
    if (arr != nullptr) {
        test.IsEqual(arr->Capacity(), arr->Size(), __LINE__);
    }
    arr = value["key2"].GetArray();
    test.IsNotNull(arr, __LINE__);
    if (arr != nullptr) {
        test.IsEqual(arr->Capacity(), arr->Size(), __LINE__);
    }
    arr = value["key3"].GetArray();
    test.IsNotNull(arr, __LINE__);
    if (arr != nullptr) {
        test.IsEqual(arr->Capacity(), arr->Size(), __LINE__);
    }

    value.Reset();

    value[0]["key1"] = 1;
    value[0]["key2"] = 1;
    value[0]["key3"] = 1;
    value[0]["key4"] = 1;
    value[0].Remove("key3");
    value[0].Remove("key4");

    obj = value[0].GetObject();
    test.IsNotNull(obj, __LINE__);
    if (obj != nullptr) {
        test.IsTrue(obj->Size() > obj->ActualSize(), __LINE__);
    }

    value[1]["key1"] = 1;
    value[1]["key2"] = 1;
    value[1]["key3"] = 1;
    value[1]["key4"] = 1;
    value[1]["key5"] = 1;
    value[1]["key6"] = 1;
    value[1]["key7"] = 1;
    value[1]["key8"] = 1;
    value[1].Remove("key5");
    value[1].Remove("key6");
    value[1].Remove("key7");
    value[1].Remove("key8");

    obj = value[0].GetObject();
    test.IsNotNull(obj, __LINE__);
    if (obj != nullptr) {
        test.IsTrue(obj->Size() > obj->ActualSize(), __LINE__);
    }

    obj = value[1].GetObject();
    test.IsNotNull(obj, __LINE__);
    if (obj != nullptr) {
        test.IsTrue(obj->Size() > obj->ActualSize(), __LINE__);
    }

    value[2] = 0;
    value.RemoveIndex(2U);
    value.Compress();

    arr = value.GetArray();
    test.IsNotNull(arr, __LINE__);
    if (arr != nullptr) {
        test.IsEqual(arr->Capacity(), arr->Size(), __LINE__);
    }

    obj = value[0].GetObject();
    test.IsNotNull(obj, __LINE__);
    if (obj != nullptr) {
        test.IsEqual(obj->Size(), obj->ActualSize(), __LINE__);
    }

    obj = value[1].GetObject();
    test.IsNotNull(obj, __LINE__);
    if (obj != nullptr) {
        test.IsEqual(obj->Size(), obj->ActualSize(), __LINE__);
    }
}

static void TestSortValue(QTest &test) {
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

    test.IsEqual(value.Stringify(ss), R"({"2015":0,"2016":0,"2017":0,"2018":0,"2019":0,"2020":0,"2021":0})", __LINE__);
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

    test.IsEqual(value.Stringify(ss), R"({"2021":0,"2020":0,"2019":0,"2018":0,"2017":0,"2016":0,"2015":0})", __LINE__);
    ss.Clear();

    value.Reset();

    value["2019"] = 0;
    value["2016"] = 0;
    value["2017"] = 0;
    value["2020"] = 0;
    value["2015"] = 0;
    value["2021"] = 0;

    value.Sort();

    test.IsEqual(value.Stringify(ss), R"({"2015":0,"2016":0,"2017":0,"2019":0,"2020":0,"2021":0})", __LINE__);
    ss.Clear();

    value.Reset();

    value["2019"] = 0;
    value["2016"] = 0;
    value["2017"] = 0;
    value["2020"] = 0;
    value["2015"] = 0;
    value["2021"] = 0;

    value.Sort(false);

    test.IsEqual(value.Stringify(ss), R"({"2021":0,"2020":0,"2019":0,"2017":0,"2016":0,"2015":0})", __LINE__);
}

static void TestGroupValue(QTest &test) {
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

    test.IsEqual(
        value2.Stringify(ss),
        R"({"2017":[{"month":1}],"2018":[{"month":2},{"month":3}],"2019":[{"month":4}],"2020":[{"month":5},{"month":6},{"month":7}]})",
        __LINE__);
    ss.Clear();

    value2.Sort(false);

    test.IsEqual(
        value2.Stringify(ss),
        R"({"2020":[{"month":5},{"month":6},{"month":7}],"2019":[{"month":4}],"2018":[{"month":2},{"month":3}],"2017":[{"month":1}]})",
        __LINE__);
    ss.Clear();

    value.Reset();
    value2.Reset();
    value.GroupBy(value2, "year");
    test.IsEqual(value2.Stringify(ss), R"()", __LINE__);
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
    test.IsEqual(value2.Stringify(ss), R"({})", __LINE__);
    ss.Clear();
    ////

    value.Reset();

    value[0]["year"] = 2019;
    value[1]["year"] = 2020;
    value[2]["year"];

    value[0]["month"] = 4;
    value[1]["month"] = 5;
    value[2]["month"] = 1;

    test.IsFalse(value.GroupBy(value2, "year"), __LINE__);

    value[2].Reset();
    value.GroupBy(value2, "year");
    test.IsFalse(value.GroupBy(value2, "year"), __LINE__);

    value[2]["year"] = VHArray{};
    test.IsFalse(value.GroupBy(value2, "year"), __LINE__);

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

    test.IsEqual(value.Stringify(ss), R"([1,2,3,4,5,6,7])", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    test.IsEqual(value.Stringify(ss), R"([7,6,5,4,3,2,1])", __LINE__);
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

    test.IsEqual(value.Stringify(ss, 2U), R"([{},[],"str",5.4,true,false,null])", __LINE__);
    ss.Clear();

    value = value2;
    value.Sort(false);

    test.IsEqual(value.Stringify(ss, 2U), R"([null,false,true,5.4,"str",[],{}])", __LINE__);
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

    test.IsEqual(value.Stringify(ss), R"([-7,-6,-5,-4,-3,-2,-1])", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    test.IsEqual(value.Stringify(ss), R"([-1,-2,-3,-4,-5,-6,-7])", __LINE__);
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

    test.IsEqual(value.Stringify(ss), R"([1.5,2.5,3.5,4.5,5.5,6.5,7.5])", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    test.IsEqual(value.Stringify(ss), R"([7.5,6.5,5.5,4.5,3.5,2.5,1.5])", __LINE__);
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

    test.IsEqual(value.Stringify(ss), R"(["a","b","c","d","e","f","g"])", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    test.IsEqual(value.Stringify(ss), R"(["g","f","e","d","c","b","a"])", __LINE__);
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

    test.IsEqual(value.Stringify(ss), R"([[0,0,0],[0,0,0,0,0]])", __LINE__);
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

    test.IsEqual(value.Stringify(ss), R"([[0,0,0,0,0],[0,0,0]])", __LINE__);

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

    test.IsEqual(value.Stringify(ss), R"([{"a":0,"b":0,"c":0},{"a":0,"b":0,"c":0,"d":0,"e":0}])", __LINE__);
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

    test.IsEqual(value.Stringify(ss), R"([{"a":0,"b":0,"c":0,"d":0,"e":0},{"a":0,"b":0,"c":0}])", __LINE__);

    //////////////////////

    ///////////////////
    ss.Clear();

    value.Reset();
    value2.Reset();

    value2 += true;
    value2 += true;

    value = value2;
    value.Sort();

    test.IsEqual(value.Stringify(ss), R"([true,true])", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    test.IsEqual(value.Stringify(ss), R"([true,true])", __LINE__);
    ss.Clear();

    //////////

    value.Reset();
    value2.Reset();

    value2 += false;
    value2 += false;

    value = value2;
    value.Sort();

    test.IsEqual(value.Stringify(ss), R"([false,false])", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    test.IsEqual(value.Stringify(ss), R"([false,false])", __LINE__);
    ///////////
    ss.Clear();

    value.Reset();
    value2.Reset();

    value2 += nullptr;
    value2 += nullptr;

    value = value2;
    value.Sort();

    test.IsEqual(value.Stringify(ss), R"([null,null])", __LINE__);
    ss.Clear();

    value = value2;

    value.Sort(false);

    test.IsEqual(value.Stringify(ss), R"([null,null])", __LINE__);
    ss.Clear();
    ///////////
    value.RemoveIndex(0);
    value.RemoveIndex(1);

    value.Sort();

    test.IsEqual(value.Stringify(ss), R"([])", __LINE__);
    ss.Clear();

    value.Sort(false);

    test.IsEqual(value.Stringify(ss), R"([])", __LINE__);
    ///////////////////
}

static int RunValueTests() {
    QTest test{"Value.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("Empty Value Test", TestEmptyValue);
    test.Test("True Value Test", TestTrueValue);
    test.Test("False Value Test", TestFalseValue);
    test.Test("Null Value Test", TestNullValue);
    test.Test("Number Value Test 1", TestNumberValue1);
    test.Test("Number Value Test 2", TestNumberValue2);
    test.Test("Number Value Test 3", TestNumberValue3);
    test.Test("Number Value Test 4", TestNumberValue4);
    test.Test("Number Value Test 5", TestNumberValue5);
    test.Test("String Value Test", TestStringValue);
    test.Test("Array Value Test", TestArrayValue);

    test.Test("Object Value Test 1", TestObjectValue1);
    test.Test("Object Value Test 2", TestObjectValue2);

    test.Test("Move Value Test 1", TestMoveValue1);
    test.Test("Move Value Test 2", TestMoveValue2);
    test.Test("Move Value Test 3", TestMoveValue3);
    test.Test("Move Value Test 4", TestMoveValue4);

    test.Test("Copy Value Test 1", TestCopyValue1);
    test.Test("Copy Value Test 2", TestCopyValue2);
    test.Test("Copy Value Test 3", TestCopyValue3);
    test.Test("Copy Value Test 4", TestCopyValue4);

    test.Test("Index Operator Test 1", TestIndexOperator1);
    test.Test("Index Operator Test 2", TestIndexOperator2);

    test.Test("Addition Test 1", TestAddition1);
    test.Test("Addition Test 2", TestAddition2);
    test.Test("Addition Test 3", TestAddition3);
    test.Test("Addition Test 4", TestAddition4);
    test.Test("Addition Test 5", TestAddition5);
    test.Test("Addition Test 6", TestAddition6);

    test.Test("Stringify Test 1", TestStringify1);
    test.Test("Stringify Test 2", TestStringify2);
    test.Test("Stringify Test 3", TestStringify3);
    test.Test("Stringify Test 4", TestStringify4);

    test.Test("Delete Value Test", TestDeleteValue);
    test.Test("Compress Value Test", TestCompressValue);

    test.Test("Sort Value Test", TestSortValue);
    test.Test("Group Value Test", TestGroupValue);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
