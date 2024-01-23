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

#ifndef QENTEM_HARRAY_TESTS_H
#define QENTEM_HARRAY_TESTS_H

#include "QTest.hpp"
#include "Digit.hpp"
#include "HArray.hpp"

namespace Qentem {
namespace Test {

using HashArray = HArray<SizeT32, char>;

static void TestHArray1(QTest &test) {
    HashArray numbers1;
    HashArray numbers2(8);

    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 0U, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);
    test.IsNull(numbers1.End(), __LINE__);

    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsTrue(numbers2.Capacity() >= 8U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);

    numbers2.Reset();
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 0U, __LINE__);
    test.IsNull(numbers2.First(), __LINE__);

    numbers1.Reserve(5);
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 8U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);

    numbers1.Reserve(10);
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 16U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);

    numbers1.Resize(18);
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 32U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);

    numbers2.Resize(4);
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsTrue(numbers2.Capacity() >= 4U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);

    numbers2.Resize(5);
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 8U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);

    numbers2.Resize(2);
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsTrue(numbers2.Capacity() >= 2U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);

    numbers1.Reserve(0);
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 0U, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);

    numbers2.Resize(0);
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 0U, __LINE__);

    numbers1.Reset();
    numbers2.Reset();

    numbers2 = numbers1;
}

static void TestHArray2(QTest &test) {
    HashArray                      numbers1(8);
    HashArray                      numbers2;
    const String<char>            *key;
    const char                    *str_c;
    const HAItem_T<SizeT32, char> *storage;

    numbers1["key1"] = 1;
    test.IsEqual(numbers1.Size(), 1U, __LINE__);
    test.IsTrue(numbers1.Capacity() >= 8U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers1.End(), (numbers1.First() + 1), __LINE__);
    test.IsEqual(numbers1["key1"], 1U, __LINE__);

    const SizeT cap  = numbers1.Capacity();
    numbers1["key1"] = 20;
    test.IsEqual(numbers1.Size(), 1U, __LINE__);
    test.IsEqual(numbers1.Capacity(), cap, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers1["key1"], 20U, __LINE__);

    numbers1["key1"] = 300;
    test.IsEqual(numbers1.Size(), 1U, __LINE__);
    test.IsEqual(numbers1.Capacity(), cap, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers1["key1"], 300U, __LINE__);

    numbers1["key1"] = 4000;
    test.IsEqual(numbers1.Size(), 1U, __LINE__);
    test.IsEqual(numbers1.Capacity(), cap, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers1["key1"], 4000U, __LINE__);

    numbers1["key1"] = 10000;
    test.IsEqual(numbers1.Size(), 1U, __LINE__);
    test.IsEqual(numbers1.Capacity(), cap, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers1["key1"], 10000U, __LINE__);

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

    test.IsEqual(numbers1.Size(), 7U, __LINE__);
    test.IsEqual(numbers1.Capacity(), cap, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers1["key1"], 10U, __LINE__);
    test.IsEqual(numbers1["key2"], 20U, __LINE__);
    test.IsEqual(numbers1["key3"], 30U, __LINE__);
    test.IsEqual(numbers1["key4"], 40U, __LINE__);
    test.IsEqual(numbers1["key5"], 50U, __LINE__);
    test.IsEqual(numbers1["key6"], 60U, __LINE__);
    test.IsEqual(numbers1["ABCDEF0123456789ABCDEF0123456789"], 70U, __LINE__);

    key = numbers1.GetKey(0);
    test.IsNotNull(key, __LINE__);
    test.IsTrue(key->IsEqual("key1", 4), __LINE__);
    key = numbers1.GetKey(1);
    test.IsNotNull(key, __LINE__);
    test.IsTrue(key->IsEqual("key2", 4), __LINE__);
    key = numbers1.GetKey(2);
    test.IsNotNull(key, __LINE__);
    test.IsTrue(key->IsEqual("key3", 4), __LINE__);
    key = numbers1.GetKey(3);
    test.IsNotNull(key, __LINE__);
    test.IsTrue(key->IsEqual("key4", 4), __LINE__);
    key = numbers1.GetKey(4);
    test.IsNotNull(key, __LINE__);
    test.IsTrue(key->IsEqual("key5", 4), __LINE__);
    key = numbers1.GetKey(5);
    test.IsNotNull(key, __LINE__);
    test.IsTrue(key->IsEqual("key6", 4), __LINE__);
    key = numbers1.GetKey(6);
    test.IsNotNull(key, __LINE__);
    test.IsTrue(key->IsEqual("ABCDEF0123456789ABCDEF0123456789", 32), __LINE__);

    key   = numbers1.GetKey(6);
    str_c = key->First();
    // Test expanding
    numbers1["key8"] = 80;
    numbers1["key9"] = 90;
    test.IsEqual(numbers1.Size(), 9U, __LINE__);
    test.IsTrue((numbers1.Capacity() >= 9), __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers1["key1"], 10U, __LINE__);
    test.IsEqual(numbers1["key2"], 20U, __LINE__);
    test.IsEqual(numbers1["key3"], 30U, __LINE__);
    test.IsEqual(numbers1["key4"], 40U, __LINE__);
    test.IsEqual(numbers1["key5"], 50U, __LINE__);
    test.IsEqual(numbers1["key6"], 60U, __LINE__);
    test.IsEqual(numbers1["ABCDEF0123456789ABCDEF0123456789"], 70U, __LINE__);
    test.IsEqual(numbers1["key8"], 80U, __LINE__);
    test.IsEqual(numbers1["key9"], 90U, __LINE__);

    const String<char> *key2 = numbers1.GetKey(6);
    test.IsNotEqual(key2, key, __LINE__);
    test.IsNotNull(key2, __LINE__);
    test.IsEqual(key2->First(), str_c, __LINE__);

    storage  = numbers1.First();
    numbers2 = numbers1;
    test.IsEqual(numbers1.Size(), 9U, __LINE__);
    test.IsTrue((numbers1.Capacity() >= 9), __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers2.Size(), 9U, __LINE__);
    test.IsTrue((numbers2.Capacity() >= 9), __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);
    test.IsNotEqual(numbers2.First(), storage, __LINE__);
    test.IsNotEqual(numbers1.GetKey(6), numbers2.GetKey(6), __LINE__);
    test.IsNotEqual(numbers2.GetKey(6)->First(), str_c, __LINE__);

    numbers2 = Memory::Move(numbers1);
    test.IsNull(numbers1.Storage(), __LINE__);
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 0U, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers2.Size(), 9U, __LINE__);
    test.IsTrue((numbers2.Capacity() >= 9), __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);
    test.IsEqual(numbers2.First(), storage, __LINE__);
    test.IsEqual(numbers2["key1"], 10U, __LINE__);
    test.IsEqual(numbers2["key2"], 20U, __LINE__);
    test.IsEqual(numbers2["key3"], 30U, __LINE__);
    test.IsEqual(numbers2["key4"], 40U, __LINE__);
    test.IsEqual(numbers2["key5"], 50U, __LINE__);
    test.IsEqual(numbers2["key6"], 60U, __LINE__);
    test.IsEqual(numbers2["ABCDEF0123456789ABCDEF0123456789"], 70U, __LINE__);
    test.IsEqual(numbers2["key8"], 80U, __LINE__);
    test.IsEqual(numbers2["key9"], 90U, __LINE__);

    numbers1 = HashArray(numbers2);
    test.IsEqual(numbers1.Size(), 9U, __LINE__);
    test.IsTrue((numbers1.Capacity() >= 9), __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers2.Size(), 9U, __LINE__);
    test.IsTrue((numbers2.Capacity() >= 9), __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);
    test.IsNotEqual(numbers1.First(), storage, __LINE__);
    test.IsEqual(numbers2.First(), storage, __LINE__);
    test.IsEqual(numbers1["key1"], 10U, __LINE__);
    test.IsEqual(numbers1["key2"], 20U, __LINE__);
    test.IsEqual(numbers1["key3"], 30U, __LINE__);
    test.IsEqual(numbers1["key4"], 40U, __LINE__);
    test.IsEqual(numbers1["key5"], 50U, __LINE__);
    test.IsEqual(numbers1["key6"], 60U, __LINE__);
    test.IsEqual(numbers1["ABCDEF0123456789ABCDEF0123456789"], 70U, __LINE__);
    test.IsEqual(numbers1["key8"], 80U, __LINE__);
    test.IsEqual(numbers1["key9"], 90U, __LINE__);
}

static void TestHArray3(QTest &test) {
    HashArray                      numbers1(8);
    HashArray                      numbers2;
    const String<char>            *key;
    const char                    *str_c;
    const HAItem_T<SizeT32, char> *storage;

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
    test.IsNotNull(key, __LINE__);
    str_c = key->First();

    storage  = numbers2.First();
    numbers1 = Memory::Move(numbers2);

    test.IsEqual(numbers1.Size(), 9U, __LINE__);
    test.IsTrue((numbers1.Capacity() >= 9), __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers1.First(), storage, __LINE__);
    test.IsEqual(numbers1["key1"], 10U, __LINE__);
    test.IsEqual(numbers1["key2"], 20U, __LINE__);
    test.IsEqual(numbers1["key3"], 30U, __LINE__);
    test.IsEqual(numbers1["key4"], 40U, __LINE__);
    test.IsEqual(numbers1["key5"], 50U, __LINE__);
    test.IsEqual(numbers1["key6"], 60U, __LINE__);
    test.IsEqual(numbers1["key7"], 70U, __LINE__);
    test.IsEqual(numbers1["key8"], 80U, __LINE__);
    test.IsEqual(numbers1["key9"], 90U, __LINE__);
    test.IsEqual(numbers1.GetKey(5), key, __LINE__);
    test.IsEqual(numbers1.GetKey(5)->First(), str_c, __LINE__);

    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 0U, __LINE__);
    test.IsNull(numbers2.First(), __LINE__);
}

static void TestHArray4(QTest &test) {
    HashArray                      numbers1;
    HashArray                      numbers2;
    HashArray                      numbers3(3);
    const HAItem_T<SizeT32, char> *storage;
    SizeT32                       *value;

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
    test.IsEqual(key_index, 0U, __LINE__);
    numbers1.GetKeyIndex(key_index, "key2", 4);
    test.IsEqual(key_index, 1U, __LINE__);
    numbers1.GetKeyIndex(key_index, "key3", 4);
    test.IsEqual(key_index, 2U, __LINE__);
    numbers1.GetKeyIndex(key_index, "key4", 4);
    test.IsEqual(key_index, 3U, __LINE__);
    numbers1.GetKeyIndex(key_index, "key5", 4);
    test.IsEqual(key_index, 4U, __LINE__);
    numbers1.GetKeyIndex(key_index, "key6", 4);
    test.IsEqual(key_index, 5U, __LINE__);
    numbers1.GetKeyIndex(key_index, "key7", 4);
    test.IsEqual(key_index, 6U, __LINE__);
    numbers1.GetKeyIndex(key_index, "key8", 4);
    test.IsEqual(key_index, 7U, __LINE__);

    numbers2["key1"]  = 100;
    numbers2["key3"]  = 300;
    numbers2["key6"]  = 600;
    numbers2["key8"]  = 800;
    numbers2["key9"]  = 900;
    numbers2["key10"] = 1000;

    numbers3 = numbers1; // Backup

    numbers1 += numbers2;
    test.IsEqual(numbers1.Size(), 10U, __LINE__);
    test.IsTrue((numbers1.Capacity() >= 10), __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers2.Size(), 6U, __LINE__);
    test.IsTrue((numbers2.Capacity() >= 6), __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);
    test.IsNotEqual(numbers2.First(), numbers1.First(), __LINE__);
    test.IsEqual(numbers1["key1"], 100U, __LINE__);
    test.IsEqual(numbers1["key2"], 20U, __LINE__);
    test.IsEqual(numbers1["key3"], 300U, __LINE__);
    test.IsEqual(numbers1["key4"], 40U, __LINE__);
    test.IsEqual(numbers1["key5"], 50U, __LINE__);
    test.IsEqual(numbers1["key6"], 600U, __LINE__);
    test.IsEqual(numbers1["key7"], 70U, __LINE__);
    test.IsEqual(numbers1["key8"], 800U, __LINE__);
    test.IsEqual(numbers1["key9"], 900U, __LINE__);
    test.IsEqual(numbers1["key10"], 1000U, __LINE__);

    SizeT          index = 0;
    const SizeT32 *val   = numbers1.GetValue(index);
    test.IsNotNull(val, __LINE__);
    test.IsEqual(*val, 100U, __LINE__);

    val = numbers1.GetValue(++index);
    test.IsNotNull(val, __LINE__);
    test.IsEqual(*val, 20U, __LINE__);

    val = numbers1.GetValue(++index);
    test.IsNotNull(val, __LINE__);
    test.IsEqual(*val, 300U, __LINE__);

    val = numbers1.GetValue(++index);
    test.IsNotNull(val, __LINE__);
    test.IsEqual(*val, 40U, __LINE__);

    val = numbers1.GetValue(++index);
    test.IsNotNull(val, __LINE__);
    test.IsEqual(*val, 50U, __LINE__);

    val = numbers1.GetValue(++index);
    test.IsNotNull(val, __LINE__);
    test.IsEqual(*val, 600U, __LINE__);

    val = numbers1.GetValue(++index);
    test.IsNotNull(val, __LINE__);
    test.IsEqual(*val, 70U, __LINE__);

    val = numbers1.GetValue(++index);
    test.IsNotNull(val, __LINE__);
    test.IsEqual(*val, 800U, __LINE__);

    val = numbers1.GetValue(++index);
    test.IsNotNull(val, __LINE__);
    test.IsEqual(*val, 900U, __LINE__);

    val = numbers1.GetValue(++index);
    test.IsNotNull(val, __LINE__);
    test.IsEqual(*val, 1000U, __LINE__);

    val = numbers1.GetValue(SizeT{255});
    test.IsNull(val, __LINE__);

    numbers1 = numbers3; // Restore
    numbers3 = numbers2; // Backup
    numbers2 += numbers1;
    test.IsEqual(numbers2["key1"], 10U, __LINE__);
    test.IsEqual(numbers2["key2"], 20U, __LINE__);
    test.IsEqual(numbers2["key3"], 30U, __LINE__);
    test.IsEqual(numbers2["key4"], 40U, __LINE__);
    test.IsEqual(numbers2["key5"], 50U, __LINE__);
    test.IsEqual(numbers2["key6"], 60U, __LINE__);
    test.IsEqual(numbers2["key7"], 70U, __LINE__);
    test.IsEqual(numbers2["key8"], 80U, __LINE__);
    test.IsEqual(numbers2["key9"], 900U, __LINE__);
    test.IsEqual(numbers2["key10"], 1000U, __LINE__);

    numbers2 = numbers3; // Restore
    numbers3 = numbers1; // Backup

    storage = numbers1.First();
    numbers2 += Memory::Move(numbers1);
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 0U, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers2.Size(), 10U, __LINE__);
    test.IsTrue((numbers2.Capacity() >= 10), __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);
    test.IsNotEqual(numbers2.First(), storage, __LINE__);
    test.IsEqual(numbers2["key1"], 10U, __LINE__);
    test.IsEqual(numbers2["key2"], 20U, __LINE__);
    test.IsEqual(numbers2["key3"], 30U, __LINE__);
    test.IsEqual(numbers2["key4"], 40U, __LINE__);
    test.IsEqual(numbers2["key5"], 50U, __LINE__);
    test.IsEqual(numbers2["key6"], 60U, __LINE__);
    test.IsEqual(numbers2["key7"], 70U, __LINE__);
    test.IsEqual(numbers2["key8"], 80U, __LINE__);
    test.IsEqual(numbers2["key9"], 900U, __LINE__);
    test.IsEqual(numbers2["key10"], 1000U, __LINE__);

    // Addition of an empty array does nothing.
    numbers2 += HashArray(10);
    test.IsEqual(numbers2.Size(), 10U, __LINE__);

    numbers2.Resize(1);
    test.IsEqual(numbers2["key1"], 10U, __LINE__);

    test.IsNull(numbers2.GetKey(1), __LINE__);

    numbers1.Reset();
    numbers1[""] = 555;
    test.IsEqual(numbers1[""], 555U, __LINE__);

    numbers1.Reset();

    for (SizeT32 i = 0; i < 10; i++) {
        String<char> key;
        String<char> key2;

        Digit::NumberToString(key, i);
        key2 = key;

        numbers1.Insert(Memory::Move(key2), Memory::Move(i));
        value = numbers1.GetValue(key);
        test.IsNotNull(value, __LINE__);
        test.IsEqual(*value, i, __LINE__);
    }
}

static void TestHArray5(QTest &test) {
    HArray<SizeT, char> numbers1;
    SizeT              *value;

    for (SizeT i = 1; i <= 10; i++) {
        String<char> key{"k-", 2};

        Digit::NumberToString(key, i);
        numbers1[key] = i;
        value         = numbers1.GetValue(key.First(), key.Length());
        test.IsNotNull(value, __LINE__);
        test.IsEqual(*value, i, __LINE__);
    }

    numbers1.Reset();

    for (SizeT i = 1; i <= 120; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        numbers1[key] = i;
        value         = numbers1.GetValue(key);
        test.IsNotNull(value, __LINE__);
        test.IsEqual(*value, i, __LINE__);
    }

    for (SizeT i = 1; i < 10; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        test.IsEqual(numbers1[key], i, __LINE__);
    }

    value = numbers1.GetValue("10", 2);
    test.IsNotNull(value, __LINE__);
    test.IsEqual(*value, 10U, __LINE__);

    for (SizeT i = 1; i <= 10; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        numbers1.Remove(key);
    }

    for (SizeT i = 1; i <= 10; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        test.IsNull(numbers1.GetValue(key), __LINE__);
    }

    numbers1.Compress();
    test.IsEqual(numbers1.Size(), 110U, __LINE__);
    test.IsTrue((numbers1.Capacity() >= 110U), __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);

    for (SizeT i = 1; i <= 100; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        numbers1.Remove(key);
    }

    for (SizeT i = 1; i <= 100; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        test.IsNull(numbers1.GetValue(key), __LINE__);
    }

    numbers1.Compress();
    test.IsEqual(numbers1.Size(), 20U, __LINE__);
    test.IsTrue((numbers1.Capacity() >= 20), __LINE__);

    for (SizeT i = 100; i <= 105; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        numbers1.Remove(key);
    }

    for (SizeT i = 100; i <= 105; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        test.IsNull(numbers1.GetValue(key), __LINE__);
    }

    test.IsEqual(numbers1.ActualSize(), 15U, __LINE__);
    test.IsTrue((numbers1.Capacity() >= 20U), __LINE__);

    for (SizeT i = 0; i <= 120; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        numbers1.Remove(key);
    }

    for (SizeT i = 0; i <= 120; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        test.IsNull(numbers1.GetValue(key), __LINE__);
    }

    numbers1.Compress();
    test.IsEqual(numbers1.Size(), 0U, __LINE__);

    numbers1.Reserve(16);
    numbers1["a"] = 1;
    numbers1.Compress();
    test.IsEqual(numbers1.Size(), 1U, __LINE__);
    test.IsTrue(numbers1.Capacity() >= 1U, __LINE__);
}

static void TestHArray6(QTest &test) {
    SizeT                               id;
    HArray<String<char>, char>          strings1;
    HArray<String<char>, char>          strings2;
    const HAItem_T<String<char>, char> *storage;

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

    test.IsEqual(strings1.Size(), 2U, __LINE__);
    test.IsNotNull(strings1.First(), __LINE__);
    test.IsNotNull(strings1.GetKey(0), __LINE__);
    test.IsEqual(strings1.GetKey(0)->First(), k_str1, __LINE__);
    test.IsEqual(*(strings1.GetKey(0)), "k-1-ABCDEF0123456789ABCDEF0123456789", __LINE__);
    test.IsNotNull(strings1.GetKey(1), __LINE__);
    test.IsEqual(strings1.GetKey(1)->First(), k_str2, __LINE__);
    test.IsEqual(*(strings1.GetKey(1)), "k-2-ABCDEF0123456789ABCDEF0123456789", __LINE__);

    id = 0;
    test.IsEqual(strings1.GetValue(id)->First(), c_str1, __LINE__);
    test.IsEqual(strings1.GetValue(++id)->First(), c_str2, __LINE__);

    strings2 += strings1;
    test.IsEqual(strings2.Size(), 2U, __LINE__);
    test.IsNotNull(strings2.First(), __LINE__);
    test.IsNotNull(strings2.GetKey(0), __LINE__);
    test.IsNotEqual(strings2.GetKey(0)->First(), k_str1, __LINE__);
    test.IsEqual(*(strings2.GetKey(0)), "k-1-ABCDEF0123456789ABCDEF0123456789", __LINE__);
    test.IsNotNull(strings2.GetKey(1), __LINE__);
    test.IsNotEqual(strings2.GetKey(1)->First(), k_str2, __LINE__);
    test.IsEqual(*(strings2.GetKey(1)), "k-2-ABCDEF0123456789ABCDEF0123456789", __LINE__);

    id = 0;
    test.IsNotEqual(strings2.GetValue(id)->First(), c_str1, __LINE__);
    test.IsNotEqual(strings2.GetValue(++id)->First(), c_str2, __LINE__);

    strings2.Reserve(2);
    storage = strings1.First();
    strings2 += Memory::Move(strings1);
    test.IsEqual(strings2.Size(), 2U, __LINE__);
    test.IsEqual(strings2.Capacity(), 2U, __LINE__);
    test.IsNotNull(strings2.First(), __LINE__);
    test.IsNotEqual(strings2.First(), storage, __LINE__);
    test.IsNotNull(strings2.GetKey(0), __LINE__);
    test.IsEqual(strings2.GetKey(0)->First(), k_str1, __LINE__);
    test.IsEqual(*(strings2.GetKey(0)), "k-1-ABCDEF0123456789ABCDEF0123456789", __LINE__);
    test.IsNotNull(strings2.GetKey(1), __LINE__);
    test.IsEqual(strings2.GetKey(1)->First(), k_str2, __LINE__);
    test.IsEqual(*(strings2.GetKey(1)), "k-2-ABCDEF0123456789ABCDEF0123456789", __LINE__);

    id = 0;
    test.IsEqual(strings2.GetValue(id)->First(), c_str1, __LINE__);
    test.IsEqual(strings2.GetValue(++id)->First(), c_str2, __LINE__);

    strings1 += Memory::Move(strings2);
    strings1.Resize(10);
    storage = strings1.First();
    strings2.Reset();
    strings2 += Memory::Move(strings1);
    test.IsEqual(strings2.Size(), 2U, __LINE__);
    test.IsEqual(strings2.Capacity(), 2U, __LINE__);
    test.IsNotNull(strings2.First(), __LINE__);
    test.IsNotEqual(strings2.First(), storage, __LINE__);
    test.IsNotNull(strings2.GetKey(0), __LINE__);
    test.IsEqual(strings2.GetKey(0)->First(), k_str1, __LINE__);
    test.IsEqual(*(strings2.GetKey(0)), "k-1-ABCDEF0123456789ABCDEF0123456789", __LINE__);
    test.IsNotNull(strings2.GetKey(1), __LINE__);
    test.IsEqual(strings2.GetKey(1)->First(), k_str2, __LINE__);
    test.IsEqual(*(strings2.GetKey(1)), "k-2-ABCDEF0123456789ABCDEF0123456789", __LINE__);

    id = 0;
    test.IsEqual(strings2.GetValue(id)->First(), c_str1, __LINE__);
    test.IsEqual(strings2.GetValue(++id)->First(), c_str2, __LINE__);
}

static void TestHArray7(QTest &test) {
    SizeT id = 200;

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
        test.IsNull(numbers1.GetKey(z), __LINE__);
    }

    numbers1.Resize((id + 1));
    test.IsEqual(numbers1.Size(), 0U, __LINE__);

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
        test.IsNull(numbers1.GetKey(id), __LINE__);
    } while (id > 0);

    numbers1.Resize((id + 1));
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    ////////////////////////////
    numbers1.Reset();
    numbers1.Reserve(16);
    for (SizeT y = 0; y < 16; y++) {
        String<char> key;

        Digit::NumberToString(key, y);
        numbers1[key] = y;
    }

    const HAItem_T<SizeT, char> *storage = numbers1.Storage();
    numbers1.Clear();
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.ActualSize(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 16U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNull(numbers1.Last(), __LINE__);
    test.IsEqual(numbers1.Storage(), storage, __LINE__);

    for (SizeT y = 0; y < 16; y++) {
        String<char> key;

        Digit::NumberToString(key, y);
        numbers1[key] = y;
    }

    test.IsEqual(numbers1.Size(), 16U, __LINE__);
    test.IsEqual(numbers1.ActualSize(), 16U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 16U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);

    numbers1.Clear();
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.ActualSize(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 16U, __LINE__);
    test.IsEqual(numbers1.Storage(), storage, __LINE__);

    numbers1.Reset();
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.ActualSize(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 0U, __LINE__);
}

static void TestHArray8(QTest &test) {
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

        test.IsTrue(list.Rename(key1, key2), __LINE__);
        test.IsNull(list.GetValue(key1), __LINE__);
        test.IsFalse(list.Rename(key1, key2), __LINE__);

        const SizeT *y = list.GetValue(key2);
        SizeT        w = id + 1;

        if (y != nullptr) {
            w = *y;
        }

        test.IsEqual(w, i, __LINE__);
    }

    SizeT j = id;
    do {
        --j;
        String<char> key1("k-");
        String<char> key2;

        Digit::NumberToString(key1, j);
        Digit::NumberToString(key2, j);
        key2 += "-k";

        test.IsTrue(list.Rename(key2, key1), __LINE__);
    } while (j > 0);

    for (SizeT i = 0; i < id; i++) {
        String<char> key1("k-");

        Digit::NumberToString(key1, i);
        const SizeT *y = list.GetValue(key1);
        SizeT        w = id + 1;

        if (y != nullptr) {
            w = *y;
        }

        test.IsEqual(w, i, __LINE__);
    }

    for (SizeT i = 0; i < id; i++) {
        String<char> key2;

        Digit::NumberToString(key2, i);
        key2 += "-k";

        test.IsNull(list.GetValue(key2), __LINE__);
    }
}

static void TestHArray9(QTest &test) {
    constexpr SizeT id = 10;

    HArray<SizeT, char>          list(id);
    const HAItem_T<SizeT, char> *item;
    const HAItem_T<SizeT, char> *item2;
    const SizeT                 *value;

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");

        Digit::NumberToString(key, i);

        list[key] = i;
        item      = list.GetItem(i);
        item2     = list.GetItem(item->Key.First(), item->Key.Length(), item->Hash);
        value     = list.GetValue(item->Key.First(), item->Key.Length(), item->Hash);

        test.IsNotNull(item, __LINE__);
        test.IsEqual(item->Key, key, __LINE__);
        test.IsEqual(item->Value, i, __LINE__);
        test.IsNotNull(item2, __LINE__);
        test.IsEqual(item, item2, __LINE__);
        test.IsNotNull(value, __LINE__);
        test.IsEqual(item2->Value, *value, __LINE__);

        item2 = list.GetItem(item->Key.First(), item->Key.Length(), i);
        value = list.GetValue(item->Key.First(), item->Key.Length(), i);
        test.IsNull(item2, __LINE__);
        test.IsNull(value, __LINE__);
    }

    item = list.GetItem(id);
    test.IsNull(item, __LINE__);

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");

        Digit::NumberToString(key, i);
        list.RemoveIndex(i);

        test.IsNull(list.GetValue(key), __LINE__);
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
        test.IsNull(list.GetValue(key), __LINE__);
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
        test.IsNull(list.GetValue(key), __LINE__);
    }
}

static void TestHArray10(QTest &test) {
    constexpr SizeT id = 10;

    HArray<SizeT, char> list(id);

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");

        Digit::NumberToString(key, i);
        list[key] = i;

        const HAItem_T<SizeT, char> *item = list.GetItem(key);

        test.IsNotNull(item, __LINE__);
        test.IsEqual(item->Value, i, __LINE__);
    }

    String<char> key1("k-");

    Digit::NumberToString(key1, 100);
    test.IsNull(list.GetItem(key1), __LINE__);
}

static void TestHArraySort(QTest &test) {
    HashArray strings;

    strings["2017"] += 2017;
    strings["2020"] += 2020;
    strings["2019"] += 2019;
    strings["2018"] += 2018;
    strings["2021"] += 2021;
    strings["2016"] += 2016;
    strings["2022"] += 2022;

    strings.Sort();

    test.IsNotNull(strings.GetValue(0), __LINE__);
    test.IsEqual(*(strings.GetValue(0)), 2016U, __LINE__);
    test.IsNotNull(strings.GetValue(1), __LINE__);
    test.IsEqual(*(strings.GetValue(1)), 2017U, __LINE__);
    test.IsNotNull(strings.GetValue(2), __LINE__);
    test.IsEqual(*(strings.GetValue(2)), 2018U, __LINE__);
    test.IsNotNull(strings.GetValue(3), __LINE__);
    test.IsEqual(*(strings.GetValue(3)), 2019U, __LINE__);
    test.IsNotNull(strings.GetValue(4), __LINE__);
    test.IsEqual(*(strings.GetValue(4)), 2020U, __LINE__);
    test.IsNotNull(strings.GetValue(5), __LINE__);
    test.IsEqual(*(strings.GetValue(5)), 2021U, __LINE__);
    test.IsNotNull(strings.GetValue(6), __LINE__);
    test.IsEqual(*(strings.GetValue(6)), 2022U, __LINE__);

    test.IsEqual(strings["2016"], 2016U, __LINE__);
    test.IsEqual(strings["2017"], 2017U, __LINE__);
    test.IsEqual(strings["2018"], 2018U, __LINE__);
    test.IsEqual(strings["2019"], 2019U, __LINE__);
    test.IsEqual(strings["2020"], 2020U, __LINE__);
    test.IsEqual(strings["2021"], 2021U, __LINE__);
    test.IsEqual(strings["2022"], 2022U, __LINE__);

    strings.Sort(false);

    test.IsEqual(strings["2022"], 2022U, __LINE__);
    test.IsEqual(strings["2021"], 2021U, __LINE__);
    test.IsEqual(strings["2020"], 2020U, __LINE__);
    test.IsEqual(strings["2019"], 2019U, __LINE__);
    test.IsEqual(strings["2018"], 2018U, __LINE__);
    test.IsEqual(strings["2017"], 2017U, __LINE__);
    test.IsEqual(strings["2016"], 2016U, __LINE__);

    strings.Reset();

    strings["2020"] = 2020;
    strings["2017"] = 2017;
    strings["2019"] = 2019;
    strings["2018"] = 2018;
    strings["2021"] = 2021;
    strings["2018"] = 2018;

    strings.Sort();

    test.IsEqual(strings["2017"], 2017U, __LINE__);
    test.IsEqual(strings["2018"], 2018U, __LINE__);
    test.IsEqual(strings["2018"], 2018U, __LINE__);
    test.IsEqual(strings["2019"], 2019U, __LINE__);
    test.IsEqual(strings["2020"], 2020U, __LINE__);
    test.IsEqual(strings["2021"], 2021U, __LINE__);

    strings.Sort(false);

    test.IsEqual(strings["2021"], 2021U, __LINE__);
    test.IsEqual(strings["2020"], 2020U, __LINE__);
    test.IsEqual(strings["2019"], 2019U, __LINE__);
    test.IsEqual(strings["2018"], 2018U, __LINE__);
    test.IsEqual(strings["2018"], 2018U, __LINE__);
    test.IsEqual(strings["2017"], 2017U, __LINE__);

    strings.Reset();

    strings["2020"] = 2020;
    strings["2017"] = 2017;
    strings["2019"] = 2019;
    strings["2018"] = 2018;
    strings["2021"] = 2021;
    strings["2018"] = 2018;

    strings.Sort(false);

    test.IsEqual(strings["2021"], 2021U, __LINE__);
    test.IsEqual(strings["2020"], 2020U, __LINE__);
    test.IsEqual(strings["2019"], 2019U, __LINE__);
    test.IsEqual(strings["2018"], 2018U, __LINE__);
    test.IsEqual(strings["2018"], 2018U, __LINE__);
    test.IsEqual(strings["2017"], 2017U, __LINE__);

    SizeT total = 0;

    for (const auto &item : strings) {
        total = SizeT(total + item.Value);
    }

    test.IsEqual(total, 10095U, __LINE__);

    for (auto &item : strings) {
        item.Value += 5;
    }

    test.IsEqual(strings["2021"], 2026U, __LINE__);
    test.IsEqual(strings["2020"], 2025U, __LINE__);
    test.IsEqual(strings["2019"], 2024U, __LINE__);
    test.IsEqual(strings["2018"], 2023U, __LINE__);
    test.IsEqual(strings["2018"], 2023U, __LINE__);
    test.IsEqual(strings["2017"], 2022U, __LINE__);
}

static int RunHArrayTests() {
    QTest test{"HArray.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("HArray Test 1", TestHArray1);
    test.Test("HArray Test 2", TestHArray2);
    test.Test("HArray Test 3", TestHArray3);
    test.Test("HArray Test 4", TestHArray4);
    test.Test("HArray Test 5", TestHArray5);
    test.Test("HArray Test 6", TestHArray6);
    test.Test("HArray Test 7", TestHArray7);
    test.Test("HArray Test 8", TestHArray8);
    test.Test("HArray Test 9", TestHArray9);
    test.Test("HArray Test 10", TestHArray10);
    test.Test("HArray Sort Test", TestHArraySort);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
