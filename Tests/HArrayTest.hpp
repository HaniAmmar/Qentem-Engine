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

#include "TestHelper.hpp"
#include "Digit.hpp"
#include "HArray.hpp"

#ifndef QENTEM_HARRAY_TESTS_H_
#define QENTEM_HARRAY_TESTS_H_

namespace Qentem::Test {

using HashArray = HArray<unsigned int, char>;

static void TestHArray1(TestHelper &helper) {
    HashArray numbers1;
    HashArray numbers2(8);

    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers1.End(), nullptr, "End()", "null", __LINE__);

    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.EqualsTrue(numbers2.Capacity() >= 8U, "Capacity>= 8", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);

    numbers2.Reset();
    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers2.First(), nullptr, "First()", "null", __LINE__);

    numbers1.Reserve(5);
    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 8U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);

    numbers1.Reserve(10);
    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 16U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);

    numbers1.Resize(18);
    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 32U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);

    numbers2.Resize(4);
    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.EqualsTrue(numbers2.Capacity() >= 4U, "Capacity>= 4", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);

    numbers2.Resize(5);
    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 8U, "Capacity", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);

    numbers2.Resize(2);
    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.EqualsTrue(numbers2.Capacity() >= 2U, "Capacity>= 2", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);

    numbers1.Reserve(0);
    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers1.First(), nullptr, "First()", "null", __LINE__);

    numbers2.Resize(0);
    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 0U, "Capacity", __LINE__);

    numbers1.Reset();
    numbers2.Reset();

    numbers2 = numbers1;
}

static void TestHArray2(TestHelper &helper) {
    HashArray                            numbers1(8);
    HashArray                            numbers2;
    const String<char>                  *key;
    const char                          *str_c;
    const HAItem_T_<unsigned int, char> *storage;

    numbers1["key1"] = 1;
    helper.Equal(numbers1.Size(), 1U, "Size", __LINE__);
    helper.EqualsTrue(numbers1.Capacity() >= 8U, "Capacity>= 8", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers1.End(), (numbers1.First() + 1), "End()", "null", __LINE__);
    helper.Equal(numbers1["key1"], 1U, "key1", __LINE__);

    const SizeT cap  = numbers1.Capacity();
    numbers1["key1"] = 20;
    helper.Equal(numbers1.Size(), 1U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), cap, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers1["key1"], 20U, "key1", __LINE__);

    numbers1["key1"] = 300;
    helper.Equal(numbers1.Size(), 1U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), cap, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers1["key1"], 300U, "key1", __LINE__);

    numbers1["key1"] = 4000;
    helper.Equal(numbers1.Size(), 1U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), cap, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers1["key1"], 4000U, "key1", __LINE__);

    numbers1["key1"] = 10000;
    helper.Equal(numbers1.Size(), 1U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), cap, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers1["key1"], 10000U, "key1", __LINE__);

    numbers1["key1"] = 19990;
    numbers1["key2"] = 20000;
    numbers1["key1"] = 18880;
    numbers1["key2"] = 27770;
    numbers1["key3"] = 39990;
    numbers1["key1"] = 17770;
    numbers1["key2"] = 23660;
    numbers1["key4"] = 40;
    numbers1["key1"] = 10;
    numbers1["key2"] = 20;
    numbers1["key3"] = 30;
    numbers1["key5"] = 50;
    numbers1["key6"] = 60;

    numbers1["ABCDEF0123456789ABCDEF0123456789"] = 70;

    helper.Equal(numbers1.Size(), 7U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), cap, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers1["key1"], 10U, "key1", __LINE__);
    helper.Equal(numbers1["key2"], 20U, "key2", __LINE__);
    helper.Equal(numbers1["key3"], 30U, "key3", __LINE__);
    helper.Equal(numbers1["key4"], 40U, "key4", __LINE__);
    helper.Equal(numbers1["key5"], 50U, "key5", __LINE__);
    helper.Equal(numbers1["key6"], 60U, "key6", __LINE__);
    helper.Equal(numbers1["ABCDEF0123456789ABCDEF0123456789"], 70U, "ABCDEF0123456789ABCDEF0123456789", __LINE__);

    key = numbers1.GetKey(0);
    helper.NotEqual(key, nullptr, "key", "null", __LINE__);
    helper.EqualsTrue(key->IsEqual("key1", 4), "(GetKey(0) == key1)", __LINE__);
    key = numbers1.GetKey(1);
    helper.NotEqual(key, nullptr, "key", "null", __LINE__);
    helper.EqualsTrue(key->IsEqual("key2", 4), "(GetKey(1) == key2)", __LINE__);
    key = numbers1.GetKey(2);
    helper.NotEqual(key, nullptr, "key", "null", __LINE__);
    helper.EqualsTrue(key->IsEqual("key3", 4), "(GetKey(2) == key3)", __LINE__);
    key = numbers1.GetKey(3);
    helper.NotEqual(key, nullptr, "key", "null", __LINE__);
    helper.EqualsTrue(key->IsEqual("key4", 4), "(GetKey(3) == key4)", __LINE__);
    key = numbers1.GetKey(4);
    helper.NotEqual(key, nullptr, "key", "null", __LINE__);
    helper.EqualsTrue(key->IsEqual("key5", 4), "(GetKey(4) == key5)", __LINE__);
    key = numbers1.GetKey(5);
    helper.NotEqual(key, nullptr, "key", "null", __LINE__);
    helper.EqualsTrue(key->IsEqual("key6", 4), "(GetKey(5) == key6)", __LINE__);
    key = numbers1.GetKey(6);
    helper.NotEqual(key, nullptr, "key", "null", __LINE__);
    helper.EqualsTrue(key->IsEqual("ABCDEF0123456789ABCDEF0123456789", 32), "(GetKey(6) == key7)", __LINE__);

    key   = numbers1.GetKey(6);
    str_c = key->First();
    // Test expanding
    numbers1["key8"] = 80;
    numbers1["key9"] = 90;
    helper.Equal(numbers1.Size(), 9U, "Size", __LINE__);
    helper.EqualsTrue((numbers1.Capacity() >= 9), "(numbers1.Capacity() >= 9)", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers1["key1"], 10U, "key1", __LINE__);
    helper.Equal(numbers1["key2"], 20U, "key2", __LINE__);
    helper.Equal(numbers1["key3"], 30U, "key3", __LINE__);
    helper.Equal(numbers1["key4"], 40U, "key4", __LINE__);
    helper.Equal(numbers1["key5"], 50U, "key5", __LINE__);
    helper.Equal(numbers1["key6"], 60U, "key6", __LINE__);
    helper.Equal(numbers1["ABCDEF0123456789ABCDEF0123456789"], 70U, "ABCDEF0123456789ABCDEF0123456789", __LINE__);
    helper.Equal(numbers1["key8"], 80U, "key8", __LINE__);
    helper.Equal(numbers1["key9"], 90U, "key9", __LINE__);

    const String<char> *key2 = numbers1.GetKey(6);
    helper.NotEqual(key2, key, "str", "GetKey(6)", __LINE__);
    helper.NotEqual(key2, nullptr, "key", "null", __LINE__);
    helper.Equal(key2->First(), str_c, "GetKey(6)->First()", __LINE__);

    storage  = numbers1.First();
    numbers2 = numbers1;
    helper.Equal(numbers1.Size(), 9U, "Size", __LINE__);
    helper.EqualsTrue((numbers1.Capacity() >= 9), "(numbers1.Capacity() >= 9)", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers2.Size(), 9U, "Size", __LINE__);
    helper.EqualsTrue((numbers2.Capacity() >= 9), "(numbers1.Capacity() >= 9)", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(numbers2.First(), storage, "First()", "storage", __LINE__);
    helper.NotEqual(numbers1.GetKey(6), numbers2.GetKey(6), "numbers1.GetKey(6)", "numbers2.GetKey(6)", __LINE__);
    helper.NotEqual(numbers2.GetKey(6)->First(), str_c, "str_c", "GetKey(6)->First()", __LINE__);

    numbers2 = Memory::Move(numbers1);
    helper.Equal(numbers1.Storage(), nullptr, "Storage()", "null", __LINE__);
    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers2.Size(), 9U, "Size", __LINE__);
    helper.EqualsTrue((numbers2.Capacity() >= 9), "(numbers1.Capacity() >= 9)", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers2.First(), storage, "First()", __LINE__);
    helper.Equal(numbers2["key1"], 10U, "key1", __LINE__);
    helper.Equal(numbers2["key2"], 20U, "key2", __LINE__);
    helper.Equal(numbers2["key3"], 30U, "key3", __LINE__);
    helper.Equal(numbers2["key4"], 40U, "key4", __LINE__);
    helper.Equal(numbers2["key5"], 50U, "key5", __LINE__);
    helper.Equal(numbers2["key6"], 60U, "key6", __LINE__);
    helper.Equal(numbers2["ABCDEF0123456789ABCDEF0123456789"], 70U, "ABCDEF0123456789ABCDEF0123456789", __LINE__);
    helper.Equal(numbers2["key8"], 80U, "key8", __LINE__);
    helper.Equal(numbers2["key9"], 90U, "key9", __LINE__);

    numbers1 = HashArray(numbers2);
    helper.Equal(numbers1.Size(), 9U, "Size", __LINE__);
    helper.EqualsTrue((numbers1.Capacity() >= 9), "(numbers1.Capacity() >= 9)", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers2.Size(), 9U, "Size", __LINE__);
    helper.EqualsTrue((numbers2.Capacity() >= 9), "(numbers1.Capacity() >= 9)", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(numbers1.First(), storage, "First()", "storage", __LINE__);
    helper.Equal(numbers2.First(), storage, "First()", __LINE__);
    helper.Equal(numbers1["key1"], 10U, "key1", __LINE__);
    helper.Equal(numbers1["key2"], 20U, "key2", __LINE__);
    helper.Equal(numbers1["key3"], 30U, "key3", __LINE__);
    helper.Equal(numbers1["key4"], 40U, "key4", __LINE__);
    helper.Equal(numbers1["key5"], 50U, "key5", __LINE__);
    helper.Equal(numbers1["key6"], 60U, "key6", __LINE__);
    helper.Equal(numbers1["ABCDEF0123456789ABCDEF0123456789"], 70U, "ABCDEF0123456789ABCDEF0123456789", __LINE__);
    helper.Equal(numbers1["key8"], 80U, "key8", __LINE__);
    helper.Equal(numbers1["key9"], 90U, "key9", __LINE__);
}

static void TestHArray3(TestHelper &helper) {
    HashArray                            numbers1(8);
    HashArray                            numbers2;
    const String<char>                  *key;
    const char                          *str_c;
    const HAItem_T_<unsigned int, char> *storage;

    numbers2["key4"] = 40;
    numbers2["key1"] = 10;
    numbers2["key2"] = 20;
    numbers2["key3"] = 30;
    numbers2["key5"] = 50;
    numbers2["key6"] = 60;
    numbers2["key7"] = 70;
    numbers2["key8"] = 80;
    numbers2["key9"] = 90;

    key = numbers2.GetKey(5);
    helper.NotEqual(key, nullptr, "key", "null", __LINE__);
    str_c = key->First();

    storage  = numbers2.First();
    numbers1 = Memory::Move(numbers2);

    helper.Equal(numbers1.Size(), 9U, "Size", __LINE__);
    helper.EqualsTrue((numbers1.Capacity() >= 9), "(numbers1.Capacity() >= 9)", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers1.First(), storage, "First()", __LINE__);
    helper.Equal(numbers1["key1"], 10U, "key1", __LINE__);
    helper.Equal(numbers1["key2"], 20U, "key2", __LINE__);
    helper.Equal(numbers1["key3"], 30U, "key3", __LINE__);
    helper.Equal(numbers1["key4"], 40U, "key4", __LINE__);
    helper.Equal(numbers1["key5"], 50U, "key5", __LINE__);
    helper.Equal(numbers1["key6"], 60U, "key6", __LINE__);
    helper.Equal(numbers1["key7"], 70U, "key7", __LINE__);
    helper.Equal(numbers1["key8"], 80U, "key8", __LINE__);
    helper.Equal(numbers1["key9"], 90U, "key9", __LINE__);
    helper.Equal(numbers1.GetKey(5), key, "key", "GetKey(5)", __LINE__);
    helper.Equal(numbers1.GetKey(5)->First(), str_c, "GetKey(5)->First()", __LINE__);

    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers2.First(), nullptr, "First()", "null", __LINE__);
}

static void TestHArray4(TestHelper &helper) {
    HashArray                            numbers1;
    HashArray                            numbers2;
    HashArray                            numbers3(3);
    const HAItem_T_<unsigned int, char> *storage;
    unsigned int                        *value;

    numbers1["key1"] = 10;
    numbers1["key2"] = 20;
    numbers1["key3"] = 30;
    numbers1["key4"] = 40;
    numbers1["key5"] = 50;
    numbers1["key6"] = 60;
    numbers1["key7"] = 70;
    numbers1["key8"] = 80;

    SizeT key_index = 100;

    numbers1.GetKeyIndex(key_index, "key1", 4);
    helper.Equal(key_index, 0U, "key1 Index", __LINE__);
    numbers1.GetKeyIndex(key_index, "key2", 4);
    helper.Equal(key_index, 1U, "key2 Index", __LINE__);
    numbers1.GetKeyIndex(key_index, "key3", 4);
    helper.Equal(key_index, 2U, "key3 Index", __LINE__);
    numbers1.GetKeyIndex(key_index, "key4", 4);
    helper.Equal(key_index, 3U, "key4 Index", __LINE__);
    numbers1.GetKeyIndex(key_index, "key5", 4);
    helper.Equal(key_index, 4U, "key5 Index", __LINE__);
    numbers1.GetKeyIndex(key_index, "key6", 4);
    helper.Equal(key_index, 5U, "key6 Index", __LINE__);
    numbers1.GetKeyIndex(key_index, "key7", 4);
    helper.Equal(key_index, 6U, "key7 Index", __LINE__);
    numbers1.GetKeyIndex(key_index, "key8", 4);
    helper.Equal(key_index, 7U, "key8 Index", __LINE__);

    numbers2["key1"]  = 100;
    numbers2["key3"]  = 300;
    numbers2["key6"]  = 600;
    numbers2["key8"]  = 800;
    numbers2["key9"]  = 900;
    numbers2["key10"] = 1000;

    numbers3 = numbers1; // Backup

    numbers1 += numbers2;
    helper.Equal(numbers1.Size(), 10U, "Size", __LINE__);
    helper.EqualsTrue((numbers1.Capacity() >= 10), "(numbers1.Capacity() >= 10)", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers2.Size(), 6U, "Size", __LINE__);
    helper.EqualsTrue((numbers2.Capacity() >= 6), "(numbers2.Capacity() >= 6)", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(numbers2.First(), numbers1.First(), "First()", "null", __LINE__);
    helper.Equal(numbers1["key1"], 100U, "key1", __LINE__);
    helper.Equal(numbers1["key2"], 20U, "key2", __LINE__);
    helper.Equal(numbers1["key3"], 300U, "key3", __LINE__);
    helper.Equal(numbers1["key4"], 40U, "key4", __LINE__);
    helper.Equal(numbers1["key5"], 50U, "key5", __LINE__);
    helper.Equal(numbers1["key6"], 600U, "key6", __LINE__);
    helper.Equal(numbers1["key7"], 70U, "key7", __LINE__);
    helper.Equal(numbers1["key8"], 800U, "key8", __LINE__);
    helper.Equal(numbers1["key9"], 900U, "key9", __LINE__);
    helper.Equal(numbers1["key10"], 1000U, "key10", __LINE__);

    SizeT               index = 0;
    const unsigned int *val   = numbers1.GetValue(index);
    helper.NotEqual(val, nullptr, "GetValue(index)", "null", __LINE__);
    helper.Equal(*val, 100U, "0", __LINE__);

    val = numbers1.GetValue(++index);
    helper.NotEqual(val, nullptr, "GetValue(index)", "null", __LINE__);
    helper.Equal(*val, 20U, "1", __LINE__);

    val = numbers1.GetValue(++index);
    helper.NotEqual(val, nullptr, "GetValue(index)", "null", __LINE__);
    helper.Equal(*val, 300U, "2", __LINE__);

    val = numbers1.GetValue(++index);
    helper.NotEqual(val, nullptr, "GetValue(index)", "null", __LINE__);
    helper.Equal(*val, 40U, "3", __LINE__);

    val = numbers1.GetValue(++index);
    helper.NotEqual(val, nullptr, "GetValue(index)", "null", __LINE__);
    helper.Equal(*val, 50U, "4", __LINE__);

    val = numbers1.GetValue(++index);
    helper.NotEqual(val, nullptr, "GetValue(index)", "null", __LINE__);
    helper.Equal(*val, 600U, "5", __LINE__);

    val = numbers1.GetValue(++index);
    helper.NotEqual(val, nullptr, "GetValue(index)", "null", __LINE__);
    helper.Equal(*val, 70U, "6", __LINE__);

    val = numbers1.GetValue(++index);
    helper.NotEqual(val, nullptr, "GetValue(index)", "null", __LINE__);
    helper.Equal(*val, 800U, "7", __LINE__);

    val = numbers1.GetValue(++index);
    helper.NotEqual(val, nullptr, "GetValue(index)", "null", __LINE__);
    helper.Equal(*val, 900U, "8", __LINE__);

    val = numbers1.GetValue(++index);
    helper.NotEqual(val, nullptr, "GetValue(index)", "null", __LINE__);
    helper.Equal(*val, 1000U, "9", __LINE__);

    val = numbers1.GetValue(SizeT{65530});
    helper.Equal(val, nullptr, "GetValue(index)", "null", __LINE__);

    numbers1 = numbers3; // Restore
    numbers3 = numbers2; // Backup
    numbers2 += numbers1;
    helper.Equal(numbers2["key1"], 10U, "key1", __LINE__);
    helper.Equal(numbers2["key2"], 20U, "key2", __LINE__);
    helper.Equal(numbers2["key3"], 30U, "key3", __LINE__);
    helper.Equal(numbers2["key4"], 40U, "key4", __LINE__);
    helper.Equal(numbers2["key5"], 50U, "key5", __LINE__);
    helper.Equal(numbers2["key6"], 60U, "key6", __LINE__);
    helper.Equal(numbers2["key7"], 70U, "key7", __LINE__);
    helper.Equal(numbers2["key8"], 80U, "key8", __LINE__);
    helper.Equal(numbers2["key9"], 900U, "key9", __LINE__);
    helper.Equal(numbers2["key10"], 1000U, "key10", __LINE__);

    numbers2 = numbers3; // Restore
    numbers3 = numbers1; // Backup

    storage = numbers1.First();
    numbers2 += Memory::Move(numbers1);
    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers2.Size(), 10U, "Size", __LINE__);
    helper.EqualsTrue((numbers2.Capacity() >= 10), "(numbers2.Capacity() >= 10)", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(numbers2.First(), storage, "First()", "storage", __LINE__);
    helper.Equal(numbers2["key1"], 10U, "key1", __LINE__);
    helper.Equal(numbers2["key2"], 20U, "key2", __LINE__);
    helper.Equal(numbers2["key3"], 30U, "key3", __LINE__);
    helper.Equal(numbers2["key4"], 40U, "key4", __LINE__);
    helper.Equal(numbers2["key5"], 50U, "key5", __LINE__);
    helper.Equal(numbers2["key6"], 60U, "key6", __LINE__);
    helper.Equal(numbers2["key7"], 70U, "key7", __LINE__);
    helper.Equal(numbers2["key8"], 80U, "key8", __LINE__);
    helper.Equal(numbers2["key9"], 900U, "key9", __LINE__);
    helper.Equal(numbers2["key10"], 1000U, "key10", __LINE__);

    // Addition of an empty array does nothing.
    numbers2 += HashArray(10);
    helper.Equal(numbers2.Size(), 10U, "Size", __LINE__);

    numbers2.Resize(1);
    helper.Equal(numbers2["key1"], 10U, "key1", __LINE__);

    helper.Equal(numbers2.GetKey(1), nullptr, "GetKey(1)", "null", __LINE__);

    numbers1.Reset();
    numbers1[""] = 555;
    helper.Equal(numbers1[""], 555U, "empty key", __LINE__);

    numbers1.Reset();

    for (unsigned int i = 0; i < 10; i++) {
        String<char> key;
        String<char> key2;

        Digit::NumberToString(key, i);
        key2 = key;

        numbers1.Insert(Memory::Move(key2), Memory::Move(i));
        value = numbers1.GetValue(key);
        helper.NotEqual(value, nullptr, "value", "null", __LINE__);
        helper.Equal(*value, i, key.First(), __LINE__);
    }
}

static void TestHArray5(TestHelper &helper) {
    HArray<SizeT, char> numbers1;
    SizeT              *value;

    for (SizeT i = 1; i < 11; i++) {
        String<char> key{"k-", 2};

        Digit::NumberToString(key, i);
        numbers1[key] = i;
        value         = numbers1.GetValue(key.First(), key.Length());
        helper.NotEqual(value, nullptr, "value", "null", __LINE__);
        helper.Equal(*value, i, key.First(), __LINE__);
    }

    numbers1.Reset();

    for (SizeT i = 1; i < 1001; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        numbers1[key] = i;
        value         = numbers1.GetValue(key);
        helper.NotEqual(value, nullptr, "value", "null", __LINE__);
        helper.Equal(*value, i, key.First(), __LINE__);
    }

    for (SizeT i = 1; i < 10; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        helper.Equal(numbers1[key], i, "key.First()", __LINE__);
    }

    value = numbers1.GetValue("10", 2);
    helper.NotEqual(value, nullptr, "value", "null", __LINE__);
    helper.Equal(*value, 10U, "10", __LINE__);

    for (SizeT i = 1; i < 11; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        numbers1.Remove(key);
    }

    for (SizeT i = 1; i < 11; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        helper.Equal(numbers1.GetValue(key), nullptr, "value", "null", __LINE__);
    }

    numbers1.Compress();
    helper.Equal(numbers1.Size(), 990U, "Size", __LINE__);
    helper.EqualsTrue((numbers1.Capacity() >= 990), "(numbers1.Capacity() >= 990)", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);

    for (SizeT i = 1; i < 101; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        numbers1.Remove(key);
    }

    for (SizeT i = 1; i < 101; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        helper.Equal(numbers1.GetValue(key), nullptr, "value", "null", __LINE__);
    }

    numbers1.Compress();
    helper.Equal(numbers1.Size(), 900U, "Size", __LINE__);
    helper.EqualsTrue((numbers1.Capacity() >= 900), "(numbers1.Capacity() >= 900)", __LINE__);

    for (SizeT i = 101; i < 201; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        numbers1.Remove(key);
    }

    for (SizeT i = 101; i < 201; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        helper.Equal(numbers1.GetValue(key), nullptr, "value", "null", __LINE__);
    }

    numbers1.Compress();
    helper.Equal(numbers1.Size(), 800U, "Size", __LINE__);
    helper.EqualsTrue((numbers1.Capacity() >= 800), "(numbers1.Capacity() >= 800)", __LINE__);

    for (SizeT i = 0; i < 1001; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        numbers1.Remove(key);
    }

    for (SizeT i = 0; i < 1001; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        helper.Equal(numbers1.GetValue(key), nullptr, "value", "null", __LINE__);
    }

    numbers1.Compress();
    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);

    numbers1.Reserve(16);
    numbers1["a"] = 1;
    numbers1.Compress();
    helper.Equal(numbers1.Size(), 1U, "Size", __LINE__);
    helper.EqualsTrue(numbers1.Capacity() >= 1U, "Capacity() >= 1", __LINE__);
}

static void TestHArray6(TestHelper &helper) {
    SizeT                                id;
    HArray<String<char>, char>           strings1;
    HArray<String<char>, char>           strings2;
    const HAItem_T_<String<char>, char> *storage;

    String<char> key1("k-1-ABCDEF0123456789ABCDEF0123456789");
    String<char> key2("k-2-ABCDEF0123456789ABCDEF0123456789");

    String<char> str1("val1-ABCDEF0123456789ABCDEF0123456789");
    String<char> str2("val2-ABCDEF0123456789ABCDEF0123456789");

    const char *k_str1 = key1.First();
    const char *k_str2 = key2.First();
    const char *c_str1 = str1.First();
    const char *c_str2 = str2.First();

    strings1[Memory::Move(key1)] = Memory::Move(str1);
    strings1[Memory::Move(key2)] = Memory::Move(str2);

    helper.Equal(strings1.Size(), 2U, "Size", __LINE__);
    helper.NotEqual(strings1.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(strings1.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.Equal(strings1.GetKey(0)->First(), k_str1, "GetKey(0)->First()", "k_str1", __LINE__);
    helper.Equal(*(strings1.GetKey(0)), "k-1-ABCDEF0123456789ABCDEF0123456789", "GetKey(0)", "k-1", __LINE__);
    helper.NotEqual(strings1.GetKey(1), nullptr, "GetKey(1)", "null", __LINE__);
    helper.Equal(strings1.GetKey(1)->First(), k_str2, "GetKey(1)->First()", "k_str2", __LINE__);
    helper.Equal(*(strings1.GetKey(1)), "k-2-ABCDEF0123456789ABCDEF0123456789", "GetKey(1)", "k-2", __LINE__);

    id = 0;
    helper.Equal(strings1.GetValue(id)->First(), c_str1, "strings[0].First()", "c_str1", __LINE__);
    helper.Equal(strings1.GetValue(++id)->First(), c_str2, "strings[1].First()", "c_str2", __LINE__);

    strings2 += strings1;
    helper.Equal(strings2.Size(), 2U, "Size", __LINE__);
    helper.NotEqual(strings2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(strings2.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.NotEqual(strings2.GetKey(0)->First(), k_str1, "GetKey(0)->First()", "k_str1", __LINE__);
    helper.Equal(*(strings2.GetKey(0)), "k-1-ABCDEF0123456789ABCDEF0123456789", "GetKey(0)", "k-1", __LINE__);
    helper.NotEqual(strings2.GetKey(1), nullptr, "GetKey(1)", "null", __LINE__);
    helper.NotEqual(strings2.GetKey(1)->First(), k_str2, "GetKey(1)->First()", "k_str2", __LINE__);
    helper.Equal(*(strings2.GetKey(1)), "k-2-ABCDEF0123456789ABCDEF0123456789", "GetKey(1)", "k-2", __LINE__);

    id = 0;
    helper.NotEqual(strings2.GetValue(id)->First(), c_str1, "strings[0].First()", "c_str1", __LINE__);
    helper.NotEqual(strings2.GetValue(++id)->First(), c_str2, "strings[1].First()", "c_str2", __LINE__);

    strings2.Reserve(2);
    storage = strings1.First();
    strings2 += Memory::Move(strings1);
    helper.Equal(strings2.Size(), 2U, "Size()", __LINE__);
    helper.Equal(strings2.Capacity(), 2U, "Capacity()", __LINE__);
    helper.NotEqual(strings2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(strings2.First(), storage, "First()", "storage", __LINE__);
    helper.NotEqual(strings2.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.Equal(strings2.GetKey(0)->First(), k_str1, "GetKey(0)->First()", "k_str1", __LINE__);
    helper.Equal(*(strings2.GetKey(0)), "k-1-ABCDEF0123456789ABCDEF0123456789", "GetKey(0)", "k-1", __LINE__);
    helper.NotEqual(strings2.GetKey(1), nullptr, "GetKey(1)", "null", __LINE__);
    helper.Equal(strings2.GetKey(1)->First(), k_str2, "GetKey(1)->First()", "k_str2", __LINE__);
    helper.Equal(*(strings2.GetKey(1)), "k-2-ABCDEF0123456789ABCDEF0123456789", "GetKey(1)", "k-2", __LINE__);

    id = 0;
    helper.Equal(strings2.GetValue(id)->First(), c_str1, "strings[0].First()", "c_str1", __LINE__);
    helper.Equal(strings2.GetValue(++id)->First(), c_str2, "strings[1].First()", "c_str2", __LINE__);

    strings1 += Memory::Move(strings2);
    strings1.Resize(10);
    storage = strings1.First();
    strings2.Reset();
    strings2 += Memory::Move(strings1);
    helper.Equal(strings2.Size(), 2U, "Size", __LINE__);
    helper.Equal(strings2.Capacity(), 2U, "Capacity()", __LINE__);
    helper.NotEqual(strings2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(strings2.First(), storage, "First()", "storage", __LINE__);
    helper.NotEqual(strings2.GetKey(0), nullptr, "GetKey(0)", "null", __LINE__);
    helper.Equal(strings2.GetKey(0)->First(), k_str1, "GetKey(0)->First()", "k_str1", __LINE__);
    helper.Equal(*(strings2.GetKey(0)), "k-1-ABCDEF0123456789ABCDEF0123456789", "GetKey(0)", "k-1", __LINE__);
    helper.NotEqual(strings2.GetKey(1), nullptr, "GetKey(1)", "null", __LINE__);
    helper.Equal(strings2.GetKey(1)->First(), k_str2, "GetKey(1)->First()", "k_str2", __LINE__);
    helper.Equal(*(strings2.GetKey(1)), "k-2-ABCDEF0123456789ABCDEF0123456789", "GetKey(1)", "k-2", __LINE__);

    id = 0;
    helper.Equal(strings2.GetValue(id)->First(), c_str1, "strings[0].First()", "c_str1", __LINE__);
    helper.Equal(strings2.GetValue(++id)->First(), c_str2, "strings[1].First()", "c_str2", __LINE__);
}

static void TestHArray7(TestHelper &helper) {
    SizeT id = 1000;

    HArray<SizeT, char> numbers1(id);

    for (SizeT x = 0; x < id; x++) {
        String<char> key;

        Digit::NumberToString(key, x);
        numbers1[key] = x;
    }

    for (SizeT z = 0; z < id; z++) {
        String<char> key;

        Digit::NumberToString(key, z);
        numbers1.Remove(key);
        helper.Equal(numbers1.GetKey(z), nullptr, "GetKey(id)->First()", "null", __LINE__);
    }

    numbers1.Resize((id + 1));
    helper.Equal(numbers1.Size(), 0U, "Size()", __LINE__);

    numbers1.Reserve(id);
    for (SizeT y = 0; y < id; y++) {
        String<char> key;

        Digit::NumberToString(key, y);
        numbers1[key] = y;
    }

    do {
        --id;
        String<char> key;
        Digit::NumberToString(key, id);

        numbers1.Remove(key);
        helper.Equal(numbers1.GetKey(id), nullptr, "GetKey(id)->First()", id, __LINE__);
    } while (id > 0);

    numbers1.Resize((id + 1));
    helper.Equal(numbers1.Size(), 0U, "Size()", __LINE__);
}

static void TestHArray8(TestHelper &helper) {
    constexpr SizeT id = 100;

    HArray<SizeT, char> list(id);

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");

        Digit::NumberToString(key, i);
        list[key] = i;
    }

    for (SizeT i = 0; i < id; i++) {
        String<char> key1("k-");
        String<char> key2;

        Digit::NumberToString(key1, i);
        Digit::NumberToString(key2, i);
        key2 += "-k";

        helper.EqualsTrue(list.Rename(key1, key2), "Rename()", __LINE__);
        helper.Equal(list.GetValue(key1), nullptr, "value", "null", __LINE__);
        helper.EqualsFalse(list.Rename(key1, key2), "Rename()", __LINE__);

        const SizeT *y = list.GetValue(key2);
        SizeT        w = id + 1;

        if (y != nullptr) {
            w = *y;
        }

        helper.Equal(w, i, "value", i, __LINE__);
    }

    SizeT j = id;
    do {
        --j;
        String<char> key1("k-");
        String<char> key2;

        Digit::NumberToString(key1, j);
        Digit::NumberToString(key2, j);
        key2 += "-k";

        helper.EqualsTrue(list.Rename(key2, key1), "Rename()", __LINE__);
    } while (j > 0);

    for (SizeT i = 0; i < id; i++) {
        String<char> key1("k-");

        Digit::NumberToString(key1, i);
        const SizeT *y = list.GetValue(key1);
        SizeT        w = id + 1;

        if (y != nullptr) {
            w = *y;
        }

        helper.Equal(w, i, "value", i, __LINE__);
    }

    for (SizeT i = 0; i < id; i++) {
        String<char> key2;

        Digit::NumberToString(key2, i);
        key2 += "-k";

        helper.Equal(list.GetValue(key2), nullptr, "value", "null", __LINE__);
    }
}

static void TestHArray9(TestHelper &helper) {
    constexpr SizeT id = 10;

    HArray<SizeT, char>                   list(id);
    const Qentem::HAItem_T_<SizeT, char> *item;
    const Qentem::HAItem_T_<SizeT, char> *item2;
    const SizeT                          *value;

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");

        Digit::NumberToString(key, i);

        list[key] = i;
        item      = list.GetItem(i);
        item2     = list.GetItem(item->Key.First(), item->Key.Length(), item->Hash);
        value     = list.GetValue(item->Key.First(), item->Key.Length(), item->Hash);

        helper.NotEqual(item, nullptr, "item", "null", __LINE__);
        helper.Equal(item->Key, key, "value", key.First(), __LINE__);
        helper.Equal(item->Value, i, "value", i, __LINE__);
        helper.NotEqual(item2, nullptr, "value", "null", __LINE__);
        helper.Equal(item, item2, "item", item, __LINE__);
        helper.NotEqual(value, nullptr, "value", "null", __LINE__);
        helper.Equal(item2->Value, *value, "value", item2->Value, __LINE__);

        item2 = list.GetItem(item->Key.First(), item->Key.Length(), i);
        value = list.GetValue(item->Key.First(), item->Key.Length(), i);
        helper.Equal(item2, nullptr, "value", "null", __LINE__);
        helper.Equal(value, nullptr, "value", "null", __LINE__);
    }

    item = list.GetItem(id);
    helper.Equal(item, nullptr, "item", "null", __LINE__);

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");

        Digit::NumberToString(key, i);
        list.RemoveIndex(i);

        helper.Equal(list.GetValue(key), nullptr, "value", "null", __LINE__);
    }

    list.Reset();

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");

        Digit::NumberToString(key, i);
        list[key] = i;
    }

    for (SizeT i = 0; i < id; i++) {
        list.RemoveIndex(i);
        list.RemoveIndex(i); // Just to see if something goes wrong.
    }

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");

        Digit::NumberToString(key, i);
        helper.Equal(list.GetValue(key), nullptr, "value", "null", __LINE__);
    }

    // No clear

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");

        Digit::NumberToString(key, i);
        list[key] = i;
    }

    for (SizeT i = 0; i < id; i++) {
        list.RemoveIndex(i);
    }

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");

        Digit::NumberToString(key, i);
        helper.Equal(list.GetValue(key), nullptr, "value", "null", __LINE__);
    }
}

static void TestHArray10(TestHelper &helper) {
    constexpr SizeT id = 10;

    HArray<SizeT, char> list(id);

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");

        Digit::NumberToString(key, i);
        list[key] = i;

        const Qentem::HAItem_T_<SizeT, char> *item = list.GetItem(key);

        helper.NotEqual(item, nullptr, "item", "null", __LINE__);
        helper.Equal(item->Value, i, "index", __LINE__);
    }

    String<char> key1("k-");

    Digit::NumberToString(key1, 100);
    helper.Equal(list.GetItem(key1), nullptr, "GetItem(k-100)", "null", __LINE__);
}

static void TestHArraySort(TestHelper &helper) {
    HashArray strings;

    strings["2017"] += 2017;
    strings["2020"] += 2020;
    strings["2019"] += 2019;
    strings["2018"] += 2018;
    strings["2021"] += 2021;
    strings["2016"] += 2016;
    strings["2022"] += 2022;

    strings.Sort();

    helper.NotEqual(strings.GetValue(0), nullptr, "GetValue(0)", "null", __LINE__);
    helper.Equal(*(strings.GetValue(0)), 2016U, "strings[2016]", __LINE__);
    helper.NotEqual(strings.GetValue(1), nullptr, "GetValue(1)", "null", __LINE__);
    helper.Equal(*(strings.GetValue(1)), 2017U, "strings[2017]", __LINE__);
    helper.NotEqual(strings.GetValue(2), nullptr, "GetValue(2)", "null", __LINE__);
    helper.Equal(*(strings.GetValue(2)), 2018U, "strings[2018]", __LINE__);
    helper.NotEqual(strings.GetValue(3), nullptr, "GetValue(3)", "null", __LINE__);
    helper.Equal(*(strings.GetValue(3)), 2019U, "strings[2019]", __LINE__);
    helper.NotEqual(strings.GetValue(4), nullptr, "GetValue(4)", "null", __LINE__);
    helper.Equal(*(strings.GetValue(4)), 2020U, "strings[2020]", __LINE__);
    helper.NotEqual(strings.GetValue(5), nullptr, "GetValue(5)", "null", __LINE__);
    helper.Equal(*(strings.GetValue(5)), 2021U, "strings[2021]", __LINE__);
    helper.NotEqual(strings.GetValue(6), nullptr, "GetValue(6)", "null", __LINE__);
    helper.Equal(*(strings.GetValue(6)), 2022U, "strings[2022]", __LINE__);

    helper.Equal(strings["2016"], 2016U, "strings[2016]", __LINE__);
    helper.Equal(strings["2017"], 2017U, "strings[2017]", __LINE__);
    helper.Equal(strings["2018"], 2018U, "strings[2018]", __LINE__);
    helper.Equal(strings["2019"], 2019U, "strings[2019]", __LINE__);
    helper.Equal(strings["2020"], 2020U, "strings[2020]", __LINE__);
    helper.Equal(strings["2021"], 2021U, "strings[2021]", __LINE__);
    helper.Equal(strings["2022"], 2022U, "strings[2022]", __LINE__);

    strings.Sort(false);

    helper.Equal(strings["2022"], 2022U, "strings[2022]", __LINE__);
    helper.Equal(strings["2021"], 2021U, "strings[2021]", __LINE__);
    helper.Equal(strings["2020"], 2020U, "strings[2020]", __LINE__);
    helper.Equal(strings["2019"], 2019U, "strings[2019]", __LINE__);
    helper.Equal(strings["2018"], 2018U, "strings[2018]", __LINE__);
    helper.Equal(strings["2017"], 2017U, "strings[2017]", __LINE__);
    helper.Equal(strings["2016"], 2016U, "strings[2016]", __LINE__);

    strings.Reset();

    strings["2020"] = 2020;
    strings["2017"] = 2017;
    strings["2019"] = 2019;
    strings["2018"] = 2018;
    strings["2021"] = 2021;
    strings["2018"] = 2018;

    strings.Sort();

    helper.Equal(strings["2017"], 2017U, "strings[2017]", __LINE__);
    helper.Equal(strings["2018"], 2018U, "strings[2018]", __LINE__);
    helper.Equal(strings["2018"], 2018U, "strings[2018]", __LINE__);
    helper.Equal(strings["2019"], 2019U, "strings[2019]", __LINE__);
    helper.Equal(strings["2020"], 2020U, "strings[2020]", __LINE__);
    helper.Equal(strings["2021"], 2021U, "strings[2021]", __LINE__);

    strings.Sort(false);

    helper.Equal(strings["2021"], 2021U, "strings[2021]", __LINE__);
    helper.Equal(strings["2020"], 2020U, "strings[2020]", __LINE__);
    helper.Equal(strings["2019"], 2019U, "strings[2019]", __LINE__);
    helper.Equal(strings["2018"], 2018U, "strings[2018]", __LINE__);
    helper.Equal(strings["2018"], 2018U, "strings[2018]", __LINE__);
    helper.Equal(strings["2017"], 2017U, "strings[2017]", __LINE__);

    strings.Reset();

    strings["2020"] = 2020;
    strings["2017"] = 2017;
    strings["2019"] = 2019;
    strings["2018"] = 2018;
    strings["2021"] = 2021;
    strings["2018"] = 2018;

    strings.Sort(false);

    helper.Equal(strings["2021"], 2021U, "strings[2021]", __LINE__);
    helper.Equal(strings["2020"], 2020U, "strings[2020]", __LINE__);
    helper.Equal(strings["2019"], 2019U, "strings[2019]", __LINE__);
    helper.Equal(strings["2018"], 2018U, "strings[2018]", __LINE__);
    helper.Equal(strings["2018"], 2018U, "strings[2018]", __LINE__);
    helper.Equal(strings["2017"], 2017U, "strings[2017]", __LINE__);

    SizeT total = 0;

    for (const auto &item : strings) {
        total += item.Value;
    }

    helper.Equal(total, 10095U, "total", __LINE__);

    for (auto &item : strings) {
        item.Value += 5;
    }

    helper.Equal(strings["2021"], 2026U, "strings[2021]", __LINE__);
    helper.Equal(strings["2020"], 2025U, "strings[2020]", __LINE__);
    helper.Equal(strings["2019"], 2024U, "strings[2019]", __LINE__);
    helper.Equal(strings["2018"], 2023U, "strings[2018]", __LINE__);
    helper.Equal(strings["2018"], 2023U, "strings[2018]", __LINE__);
    helper.Equal(strings["2017"], 2022U, "strings[2017]", __LINE__);
}

static int RunHArrayTests() {
    TestHelper helper{"HArray.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("HArray Test 1", TestHArray1);
    helper.Test("HArray Test 2", TestHArray2);
    helper.Test("HArray Test 3", TestHArray3);
    helper.Test("HArray Test 4", TestHArray4);
    helper.Test("HArray Test 5", TestHArray5);
    helper.Test("HArray Test 6", TestHArray6);
    helper.Test("HArray Test 7", TestHArray7);
    helper.Test("HArray Test 8", TestHArray8);
    helper.Test("HArray Test 9", TestHArray9);
    helper.Test("HArray Test 10", TestHArray10);
    helper.Test("HArray Sort Test", TestHArraySort);

    return helper.EndTests();
}

} // namespace Qentem::Test

#endif
