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

#include "Digit.hpp"
#include "HArray.hpp"
#include "TestHelper.hpp"

#ifndef QENTEM_HARRAYTEST_H_
#define QENTEM_HARRAYTEST_H_

namespace Qentem {
namespace Test {

static int TestHash() {
    using H_ARR = HArray<UInt>;
    ULong hash;
    ULong hash2;
    ULong hash3;
    ULong hash4;

    hash = H_ARR::Hash("", 0);
    SHOULD_NOT_EQUAL(hash, 0, "hash", "0");

    hash  = H_ARR::Hash("1", 1);
    hash2 = H_ARR::Hash("0", 1);
    SHOULD_NOT_EQUAL(hash, 0, "hash", "0");
    SHOULD_NOT_EQUAL(hash2, 0, "hash2", "0");
    SHOULD_NOT_EQUAL(hash, hash2, "hash", "hash2");

    hash3 = H_ARR::Hash("10", 2);
    hash4 = H_ARR::Hash("01", 2);
    SHOULD_NOT_EQUAL(hash3, 0, "hash3", "0");
    SHOULD_NOT_EQUAL(hash4, 0, "hash4", "0");
    SHOULD_NOT_EQUAL(hash, hash3, "hash", "hash3");
    SHOULD_NOT_EQUAL(hash2, hash3, "hash2", "hash3");
    SHOULD_NOT_EQUAL(hash, hash4, "hash", "hash4");
    SHOULD_NOT_EQUAL(hash2, hash4, "hash2", "hash4");
    SHOULD_NOT_EQUAL(hash3, hash4, "hash3", "hash4");

    hash  = H_ARR::Hash("100", 3);
    hash2 = H_ARR::Hash("001", 3);
    SHOULD_NOT_EQUAL(hash, 0, "hash", "0");
    SHOULD_NOT_EQUAL(hash2, 0, "hash2", "0");
    SHOULD_NOT_EQUAL(hash, hash3, "hash", "hash3");
    SHOULD_NOT_EQUAL(hash2, hash3, "hash2", "hash3");
    SHOULD_NOT_EQUAL(hash, hash4, "hash", "hash4");
    SHOULD_NOT_EQUAL(hash2, hash4, "hash2", "hash4");

    hash  = H_ARR::Hash("abc", 3);
    hash2 = H_ARR::Hash("cba", 3);
    SHOULD_NOT_EQUAL(hash, hash2, "hash", "hash2");
    SHOULD_NOT_EQUAL(hash, 0, "hash", "0");
    SHOULD_NOT_EQUAL(hash2, 0, "hash2", "0");

    END_SUB_TEST;
}

static int TestHArray1() {
    HArray<UInt> numbers1;
    HArray<UInt> numbers2(8);

    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    SHOULD_EQUAL_VALUE(numbers2.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 8, "Capacity");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");

    numbers2.Clear();
    SHOULD_EQUAL_VALUE(numbers2.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");

    numbers1.SetCapacity(5);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 5, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    numbers1.SetCapacity(10);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 10, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    numbers1.Resize(18);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 18, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    numbers2.Resize(4);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 4, "Capacity");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");

    numbers2.Resize(5);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 5, "Capacity");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");

    numbers2.Resize(2);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 2, "Capacity");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");

    numbers1.SetCapacity(0);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    numbers2.Resize(0);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 0, "Capacity");

    END_SUB_TEST;
}

static int TestHArray2() {
    HArray<UInt>        numbers1(8);
    HArray<UInt>        numbers2;
    const String *      key;
    const char *        str_c;
    const HAItem<UInt> *storage;

    numbers1["key1"] = 1;
    SHOULD_EQUAL_VALUE(numbers1.Size(), 1, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 8, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers1["key1"], 1, "key1");

    numbers1["key1"] = 20;
    SHOULD_EQUAL_VALUE(numbers1.Size(), 1, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 8, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers1["key1"], 20, "key1");

    numbers1["key1"] = 300;
    SHOULD_EQUAL_VALUE(numbers1.Size(), 1, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 8, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers1["key1"], 300, "key1");

    numbers1["key1"] = 4000;
    SHOULD_EQUAL_VALUE(numbers1.Size(), 1, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 8, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers1["key1"], 4000, "key1");

    numbers1["key1"] = 10000;
    SHOULD_EQUAL_VALUE(numbers1.Size(), 1, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 8, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers1["key1"], 10000, "key1");

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
    numbers1["key7"] = 70;
    numbers1["key8"] = 80;

    SHOULD_EQUAL_VALUE(numbers1.Size(), 8, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 8, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers1["key1"], 10, "key1");
    SHOULD_EQUAL_VALUE(numbers1["key2"], 20, "key2");
    SHOULD_EQUAL_VALUE(numbers1["key3"], 30, "key3");
    SHOULD_EQUAL_VALUE(numbers1["key4"], 40, "key4");
    SHOULD_EQUAL_VALUE(numbers1["key5"], 50, "key5");
    SHOULD_EQUAL_VALUE(numbers1["key6"], 60, "key6");
    SHOULD_EQUAL_VALUE(numbers1["key7"], 70, "key7");
    SHOULD_EQUAL_VALUE(numbers1["key8"], 80, "key8");

    key = numbers1.GetKey(0);
    SHOULD_NOT_EQUAL(key, nullptr, "key", "null");
    SHOULD_EQUAL_TRUE(key->Compare("key1", 4), "(GetKey(0) == key1)");
    key = numbers1.GetKey(1);
    SHOULD_NOT_EQUAL(key, nullptr, "key", "null");
    SHOULD_EQUAL_TRUE(key->Compare("key2", 4), "(GetKey(1) == key2)");
    key = numbers1.GetKey(2);
    SHOULD_NOT_EQUAL(key, nullptr, "key", "null");
    SHOULD_EQUAL_TRUE(key->Compare("key3", 4), "(GetKey(2) == key3)");
    key = numbers1.GetKey(3);
    SHOULD_NOT_EQUAL(key, nullptr, "key", "null");
    SHOULD_EQUAL_TRUE(key->Compare("key4", 4), "(GetKey(3) == key4)");
    key = numbers1.GetKey(4);
    SHOULD_NOT_EQUAL(key, nullptr, "key", "null");
    SHOULD_EQUAL_TRUE(key->Compare("key5", 4), "(GetKey(4) == key5)");
    key = numbers1.GetKey(5);
    SHOULD_NOT_EQUAL(key, nullptr, "key", "null");
    SHOULD_EQUAL_TRUE(key->Compare("key6", 4), "(GetKey(5) == key6)");
    key = numbers1.GetKey(6);
    SHOULD_NOT_EQUAL(key, nullptr, "key", "null");
    SHOULD_EQUAL_TRUE(key->Compare("key7", 4), "(GetKey(6) == key7)");
    key = numbers1.GetKey(7);
    SHOULD_NOT_EQUAL(key, nullptr, "key", "null");
    SHOULD_EQUAL_TRUE(key->Compare("key8", 4), "(GetKey(7) == key8)");

    key   = numbers1.GetKey(5);
    str_c = key->Char();
    // Test expanding
    numbers1["key9"] =
        90; // Capacity was at 8, any more items should expand the array.
    SHOULD_EQUAL_VALUE(numbers1.Size(), 9, "Size");
    SHOULD_EQUAL_TRUE((numbers1.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers1["key1"], 10, "key1");
    SHOULD_EQUAL_VALUE(numbers1["key2"], 20, "key2");
    SHOULD_EQUAL_VALUE(numbers1["key3"], 30, "key3");
    SHOULD_EQUAL_VALUE(numbers1["key4"], 40, "key4");
    SHOULD_EQUAL_VALUE(numbers1["key5"], 50, "key5");
    SHOULD_EQUAL_VALUE(numbers1["key6"], 60, "key6");
    SHOULD_EQUAL_VALUE(numbers1["key7"], 70, "key7");
    SHOULD_EQUAL_VALUE(numbers1["key8"], 80, "key8");
    SHOULD_EQUAL_VALUE(numbers1["key9"], 90, "key9");

    const String *key2 = numbers1.GetKey(5);
    SHOULD_NOT_EQUAL(key2, key, "str", "GetKey(5)");
    SHOULD_NOT_EQUAL(key2, nullptr, "key", "null");
    SHOULD_EQUAL_VALUE(key2->Char(), str_c, "GetKey(5)->Char()");

    storage  = numbers1.Storage();
    numbers2 = numbers1;
    SHOULD_EQUAL_VALUE(numbers1.Size(), 9, "Size");
    SHOULD_EQUAL_TRUE((numbers1.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers2.Size(), 9, "Size");
    SHOULD_EQUAL_TRUE((numbers2.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(numbers2.Storage(), storage, "Storage()", "storage");
    SHOULD_NOT_EQUAL(numbers1.GetKey(5), numbers2.GetKey(5),
                     "numbers1.GetKey(5)", "numbers2.GetKey(5)");
    SHOULD_NOT_EQUAL(key->Char(), str_c, "str_c", "GetKey(5)->Char()");

    numbers2 = static_cast<HArray<UInt> &&>(numbers1);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers2.Size(), 9, "Size");
    SHOULD_EQUAL_TRUE((numbers2.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers2.Storage(), storage, "Storage()");
    SHOULD_EQUAL_VALUE(numbers2["key1"], 10, "key1");
    SHOULD_EQUAL_VALUE(numbers2["key2"], 20, "key2");
    SHOULD_EQUAL_VALUE(numbers2["key3"], 30, "key3");
    SHOULD_EQUAL_VALUE(numbers2["key4"], 40, "key4");
    SHOULD_EQUAL_VALUE(numbers2["key5"], 50, "key5");
    SHOULD_EQUAL_VALUE(numbers2["key6"], 60, "key6");
    SHOULD_EQUAL_VALUE(numbers2["key7"], 70, "key7");
    SHOULD_EQUAL_VALUE(numbers2["key8"], 80, "key8");
    SHOULD_EQUAL_VALUE(numbers2["key9"], 90, "key9");

    numbers1 = HArray<UInt>(numbers2);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 9, "Size");
    SHOULD_EQUAL_TRUE((numbers1.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers2.Size(), 9, "Size");
    SHOULD_EQUAL_TRUE((numbers2.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(numbers1.Storage(), storage, "Storage()", "storage");
    SHOULD_EQUAL_VALUE(numbers2.Storage(), storage, "Storage()");
    SHOULD_EQUAL_VALUE(numbers1["key1"], 10, "key1");
    SHOULD_EQUAL_VALUE(numbers1["key2"], 20, "key2");
    SHOULD_EQUAL_VALUE(numbers1["key3"], 30, "key3");
    SHOULD_EQUAL_VALUE(numbers1["key4"], 40, "key4");
    SHOULD_EQUAL_VALUE(numbers1["key5"], 50, "key5");
    SHOULD_EQUAL_VALUE(numbers1["key6"], 60, "key6");
    SHOULD_EQUAL_VALUE(numbers1["key7"], 70, "key7");
    SHOULD_EQUAL_VALUE(numbers1["key8"], 80, "key8");
    SHOULD_EQUAL_VALUE(numbers1["key9"], 90, "key9");

    END_SUB_TEST;
}

static int TestHArray3() {
    HArray<UInt>        numbers1(8);
    HArray<UInt>        numbers2;
    const String *      key;
    const char *        str_c;
    const HAItem<UInt> *storage;

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
    SHOULD_NOT_EQUAL(key, nullptr, "key", "null");
    str_c = key->Char();

    storage  = numbers2.Storage();
    numbers1 = static_cast<HArray<UInt> &&>(numbers2);

    SHOULD_EQUAL_VALUE(numbers1.Size(), 9, "Size");
    SHOULD_EQUAL_TRUE((numbers1.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers1.Storage(), storage, "Storage()");
    SHOULD_EQUAL_VALUE(numbers1["key1"], 10, "key1");
    SHOULD_EQUAL_VALUE(numbers1["key2"], 20, "key2");
    SHOULD_EQUAL_VALUE(numbers1["key3"], 30, "key3");
    SHOULD_EQUAL_VALUE(numbers1["key4"], 40, "key4");
    SHOULD_EQUAL_VALUE(numbers1["key5"], 50, "key5");
    SHOULD_EQUAL_VALUE(numbers1["key6"], 60, "key6");
    SHOULD_EQUAL_VALUE(numbers1["key7"], 70, "key7");
    SHOULD_EQUAL_VALUE(numbers1["key8"], 80, "key8");
    SHOULD_EQUAL_VALUE(numbers1["key9"], 90, "key9");
    SHOULD_EQUAL(numbers1.GetKey(5), key, "key", "GetKey(5)");
    SHOULD_EQUAL_VALUE(numbers1.GetKey(5)->Char(), str_c, "GetKey(5)->Char()");

    SHOULD_EQUAL_VALUE(numbers2.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");

    END_SUB_TEST;
}

static int TestHArray4() {
    HArray<UInt>        numbers1;
    HArray<UInt>        numbers2;
    HArray<UInt>        numbers3(3);
    const HAItem<UInt> *storage;

    numbers1["key1"] = 10;
    numbers1["key2"] = 20;
    numbers1["key3"] = 30;
    numbers1["key4"] = 40;
    numbers1["key5"] = 50;
    numbers1["key6"] = 60;
    numbers1["key7"] = 70;
    numbers1["key8"] = 80;

    numbers2["key1"]  = 100;
    numbers2["key3"]  = 300;
    numbers2["key6"]  = 600;
    numbers2["key8"]  = 800;
    numbers2["key9"]  = 900;
    numbers2["key10"] = 1000;

    numbers3 = numbers1; // Backup

    numbers1 += numbers2;
    SHOULD_EQUAL_VALUE(numbers1.Size(), 10, "Size");
    SHOULD_EQUAL_TRUE((numbers1.Capacity() >= 10),
                      "(numbers1.Capacity() >= 10)");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers2.Size(), 6, "Size");
    SHOULD_EQUAL_TRUE((numbers2.Capacity() >= 6), "(numbers2.Capacity() >= 6)");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(numbers2.Storage(), numbers1.Storage(), "Storage()",
                     "null");
    SHOULD_EQUAL_VALUE(numbers1["key1"], 100, "key1");
    SHOULD_EQUAL_VALUE(numbers1["key2"], 20, "key2");
    SHOULD_EQUAL_VALUE(numbers1["key3"], 300, "key3");
    SHOULD_EQUAL_VALUE(numbers1["key4"], 40, "key4");
    SHOULD_EQUAL_VALUE(numbers1["key5"], 50, "key5");
    SHOULD_EQUAL_VALUE(numbers1["key6"], 600, "key6");
    SHOULD_EQUAL_VALUE(numbers1["key7"], 70, "key7");
    SHOULD_EQUAL_VALUE(numbers1["key8"], 800, "key8");
    SHOULD_EQUAL_VALUE(numbers1["key9"], 900, "key9");
    SHOULD_EQUAL_VALUE(numbers1["key10"], 1000, "key10");

    UInt        index = 0;
    const UInt *val   = numbers1.GetValue(index);
    SHOULD_NOT_EQUAL(val, nullptr, "GetValue(index)", "null");
    SHOULD_EQUAL_VALUE(*val, 100, "0");

    val = numbers1.GetValue(++index);
    SHOULD_NOT_EQUAL(val, nullptr, "GetValue(index)", "null");
    SHOULD_EQUAL_VALUE(*val, 20, "1");

    val = numbers1.GetValue(++index);
    SHOULD_NOT_EQUAL(val, nullptr, "GetValue(index)", "null");
    SHOULD_EQUAL_VALUE(*val, 300, "2");

    val = numbers1.GetValue(++index);
    SHOULD_NOT_EQUAL(val, nullptr, "GetValue(index)", "null");
    SHOULD_EQUAL_VALUE(*val, 40, "3");

    val = numbers1.GetValue(++index);
    SHOULD_NOT_EQUAL(val, nullptr, "GetValue(index)", "null");
    SHOULD_EQUAL_VALUE(*val, 50, "4");

    val = numbers1.GetValue(++index);
    SHOULD_NOT_EQUAL(val, nullptr, "GetValue(index)", "null");
    SHOULD_EQUAL_VALUE(*val, 600, "5");

    val = numbers1.GetValue(++index);
    SHOULD_NOT_EQUAL(val, nullptr, "GetValue(index)", "null");
    SHOULD_EQUAL_VALUE(*val, 70, "6");

    val = numbers1.GetValue(++index);
    SHOULD_NOT_EQUAL(val, nullptr, "GetValue(index)", "null");
    SHOULD_EQUAL_VALUE(*val, 800, "7");

    val = numbers1.GetValue(++index);
    SHOULD_NOT_EQUAL(val, nullptr, "GetValue(index)", "null");
    SHOULD_EQUAL_VALUE(*val, 900, "8");

    val = numbers1.GetValue(++index);
    SHOULD_NOT_EQUAL(val, nullptr, "GetValue(index)", "null");
    SHOULD_EQUAL_VALUE(*val, 1000, "9");

    val = numbers1.GetValue(99999);
    SHOULD_EQUAL(val, nullptr, "GetValue(index)", "null");

    numbers1 = numbers3; // Restore
    numbers3 = numbers2; // Backup
    numbers2 += numbers1;
    SHOULD_EQUAL_VALUE(numbers2["key1"], 10, "key1");
    SHOULD_EQUAL_VALUE(numbers2["key2"], 20, "key2");
    SHOULD_EQUAL_VALUE(numbers2["key3"], 30, "key3");
    SHOULD_EQUAL_VALUE(numbers2["key4"], 40, "key4");
    SHOULD_EQUAL_VALUE(numbers2["key5"], 50, "key5");
    SHOULD_EQUAL_VALUE(numbers2["key6"], 60, "key6");
    SHOULD_EQUAL_VALUE(numbers2["key7"], 70, "key7");
    SHOULD_EQUAL_VALUE(numbers2["key8"], 80, "key8");
    SHOULD_EQUAL_VALUE(numbers2["key9"], 900, "key9");
    SHOULD_EQUAL_VALUE(numbers2["key10"], 1000, "key10");

    numbers2 = numbers3; // Restore
    numbers3 = numbers1; // Backup

    storage = numbers1.Storage();
    numbers2 += static_cast<HArray<UInt> &&>(numbers1);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers2.Size(), 10, "Size");
    SHOULD_EQUAL_TRUE((numbers2.Capacity() >= 10),
                      "(numbers2.Capacity() >= 10)");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(numbers2.Storage(), storage, "Storage()", "storage");
    SHOULD_EQUAL_VALUE(numbers2["key1"], 10, "key1");
    SHOULD_EQUAL_VALUE(numbers2["key2"], 20, "key2");
    SHOULD_EQUAL_VALUE(numbers2["key3"], 30, "key3");
    SHOULD_EQUAL_VALUE(numbers2["key4"], 40, "key4");
    SHOULD_EQUAL_VALUE(numbers2["key5"], 50, "key5");
    SHOULD_EQUAL_VALUE(numbers2["key6"], 60, "key6");
    SHOULD_EQUAL_VALUE(numbers2["key7"], 70, "key7");
    SHOULD_EQUAL_VALUE(numbers2["key8"], 80, "key8");
    SHOULD_EQUAL_VALUE(numbers2["key9"], 900, "key9");
    SHOULD_EQUAL_VALUE(numbers2["key10"], 1000, "key10");

    // Addition of an empty array does nothing.
    numbers2 += HArray<UInt>(10);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 10, "Size");
    String srt1;

    bool did_throw = false;

    try {
        numbers2[static_cast<String &&>(srt1)];
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    numbers2.Resize(1);
    SHOULD_EQUAL_VALUE(numbers2["key1"], 10, "key1");

    SHOULD_EQUAL(numbers2.GetKey(1), nullptr, "GetKey(1)", "null");

    try {
        did_throw                = false;
        numbers1.Add(nullptr, 0) = 10;
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    numbers1.Clear();
    numbers1[""] = 555;
    SHOULD_EQUAL_VALUE(numbers1[""], 555, "empty key");

    END_SUB_TEST;
}

static int TestHArray5() {
    HArray<ULong> numbers1;
    String        key;
    ULong *       value;

    for (ULong i = 1; i < 11; i++) {
        key = "k-";
        key += Digit::NumberToString(i);

        numbers1.Add(key.Char(), key.Length()) = i;

        value = numbers1.Find(key.Char(), key.Length());
        SHOULD_NOT_EQUAL(value, nullptr, "value", "null");
        SHOULD_EQUAL_VALUE(*value, i, key.Char());
    }

    numbers1.Clear();

    for (ULong i = 1; i < 1001; i++) {
        numbers1[Digit::NumberToString(i)] = i;

        value = numbers1.Find(Digit::NumberToString(i));
        SHOULD_NOT_EQUAL(value, nullptr, "value", "null");
        SHOULD_EQUAL_VALUE(*value, i, key.Char());
    }

    for (ULong i = 1; i < 10; i++) {
        SHOULD_EQUAL_VALUE(numbers1[Digit::NumberToString(i)], i, "key.Char()");
    }

    value = numbers1.Find("10", 2);
    SHOULD_NOT_EQUAL(value, nullptr, "value", "null");
    SHOULD_EQUAL_VALUE(*value, 10, "10");

    for (ULong i = 1; i < 11; i++) {
        numbers1.Delete(Digit::NumberToString(i));
    }

    for (ULong i = 1; i < 11; i++) {
        SHOULD_EQUAL(numbers1.Find(Digit::NumberToString(i)), nullptr, "value",
                     "null");
    }

    numbers1.Compress();
    SHOULD_EQUAL_VALUE(numbers1.Size(), 990, "Size");
    SHOULD_EQUAL_TRUE((numbers1.Capacity() >= 990),
                      "(numbers1.Capacity() >= 990)");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    for (ULong i = 1; i < 101; i++) {
        numbers1.Delete(Digit::NumberToString(i));
    }

    for (ULong i = 1; i < 101; i++) {
        SHOULD_EQUAL(numbers1.Find(Digit::NumberToString(i)), nullptr, "value",
                     "null");
    }

    numbers1.Compress();
    SHOULD_EQUAL_VALUE(numbers1.Size(), 900, "Size");
    SHOULD_EQUAL_TRUE((numbers1.Capacity() >= 900),
                      "(numbers1.Capacity() >= 900)");

    for (ULong i = 101; i < 201; i++) {
        key = Digit::NumberToString(i);
        numbers1.Delete(key.Char(), key.Length());
    }

    for (ULong i = 101; i < 201; i++) {
        SHOULD_EQUAL(numbers1.Find(Digit::NumberToString(i)), nullptr, "value",
                     "null");
    }

    numbers1.Compress();
    SHOULD_EQUAL_VALUE(numbers1.Size(), 800, "Size");
    SHOULD_EQUAL_TRUE((numbers1.Capacity() >= 800),
                      "(numbers1.Capacity() >= 800)");

    for (ULong i = 0; i < 1001; i++) {
        key = Digit::NumberToString(i);
        numbers1.Delete(key);
    }

    for (ULong i = 0; i < 1001; i++) {
        SHOULD_EQUAL(numbers1.Find(Digit::NumberToString(i)), nullptr, "value",
                     "null");
    }

    numbers1.Compress();
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");

    numbers1.SetCapacity(16);
    numbers1["a"] = 1;
    numbers1.Compress();
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 1, "Capacity");

    END_SUB_TEST;
}

static int TestHArray6() {
    ULong                 id;
    HArray<String>        strings1;
    HArray<String>        strings2;
    const HAItem<String> *storage;

    String key1("k-1");
    String key2("k-2");

    String str1("val1");
    String str2("val2");

    const char *k_str1 = key1.Char();
    const char *k_str2 = key2.Char();
    const char *c_str1 = str1.Char();
    const char *c_str2 = str2.Char();

    strings1[static_cast<String &&>(key1)] = static_cast<String &&>(str1);
    strings1[static_cast<String &&>(key2)] = static_cast<String &&>(str2);

    SHOULD_EQUAL_VALUE(strings1.Size(), 2, "Size");
    SHOULD_NOT_EQUAL(strings1.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(strings1.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(strings1.GetKey(0)->Char(), k_str1, "GetKey(0)->Char()",
                 "k_str1");
    SHOULD_EQUAL(*(strings1.GetKey(0)), "k-1", "GetKey(0)", "k-1");
    SHOULD_NOT_EQUAL(strings1.GetKey(1), nullptr, "GetKey(1)", "null");
    SHOULD_EQUAL(strings1.GetKey(1)->Char(), k_str2, "GetKey(1)->Char()",
                 "k_str2");
    SHOULD_EQUAL(*(strings1.GetKey(1)), "k-2", "GetKey(1)", "k-2");

    id = 0;
    SHOULD_EQUAL(strings1.GetValue(id)->Char(), c_str1, "strings[0].Char()",
                 "c_str1");
    SHOULD_EQUAL(strings1.GetValue(++id)->Char(), c_str2, "strings[1].Char()",
                 "c_str2");

    strings2 += strings1;
    SHOULD_EQUAL_VALUE(strings2.Size(), 2, "Size");
    SHOULD_NOT_EQUAL(strings2.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(strings2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_NOT_EQUAL(strings2.GetKey(0)->Char(), k_str1, "GetKey(0)->Char()",
                     "k_str1");
    SHOULD_EQUAL(*(strings2.GetKey(0)), "k-1", "GetKey(0)", "k-1");
    SHOULD_NOT_EQUAL(strings2.GetKey(1), nullptr, "GetKey(1)", "null");
    SHOULD_NOT_EQUAL(strings2.GetKey(1)->Char(), k_str2, "GetKey(1)->Char()",
                     "k_str2");
    SHOULD_EQUAL(*(strings2.GetKey(1)), "k-2", "GetKey(1)", "k-2");

    id = 0;
    SHOULD_NOT_EQUAL(strings2.GetValue(id)->Char(), c_str1, "strings[0].Char()",
                     "c_str1");
    SHOULD_NOT_EQUAL(strings2.GetValue(++id)->Char(), c_str2,
                     "strings[1].Char()", "c_str2");

    strings2.SetCapacity(2);
    storage = strings1.Storage();
    strings2 += static_cast<HArray<String> &&>(strings1);
    SHOULD_EQUAL_VALUE(strings2.Size(), 2, "Size()");
    SHOULD_EQUAL_VALUE(strings2.Capacity(), 2, "Capacity()");
    SHOULD_NOT_EQUAL(strings2.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(strings2.Storage(), storage, "Storage()", "storage");
    SHOULD_NOT_EQUAL(strings2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(strings2.GetKey(0)->Char(), k_str1, "GetKey(0)->Char()",
                 "k_str1");
    SHOULD_EQUAL(*(strings2.GetKey(0)), "k-1", "GetKey(0)", "k-1");
    SHOULD_NOT_EQUAL(strings2.GetKey(1), nullptr, "GetKey(1)", "null");
    SHOULD_EQUAL(strings2.GetKey(1)->Char(), k_str2, "GetKey(1)->Char()",
                 "k_str2");
    SHOULD_EQUAL(*(strings2.GetKey(1)), "k-2", "GetKey(1)", "k-2");

    id = 0;
    SHOULD_EQUAL(strings2.GetValue(id)->Char(), c_str1, "strings[0].Char()",
                 "c_str1");
    SHOULD_EQUAL(strings2.GetValue(++id)->Char(), c_str2, "strings[1].Char()",
                 "c_str2");

    strings1 += static_cast<HArray<String> &&>(strings2);
    strings1.Resize(10);
    storage = strings1.Storage();
    strings2.Clear();
    strings2 += static_cast<HArray<String> &&>(strings1);
    SHOULD_EQUAL_VALUE(strings2.Size(), 2, "Size");
    SHOULD_EQUAL_VALUE(strings2.Capacity(), 2, "Capacity()");
    SHOULD_NOT_EQUAL(strings2.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(strings2.Storage(), storage, "Storage()", "storage");
    SHOULD_NOT_EQUAL(strings2.GetKey(0), nullptr, "GetKey(0)", "null");
    SHOULD_EQUAL(strings2.GetKey(0)->Char(), k_str1, "GetKey(0)->Char()",
                 "k_str1");
    SHOULD_EQUAL(*(strings2.GetKey(0)), "k-1", "GetKey(0)", "k-1");
    SHOULD_NOT_EQUAL(strings2.GetKey(1), nullptr, "GetKey(1)", "null");
    SHOULD_EQUAL(strings2.GetKey(1)->Char(), k_str2, "GetKey(1)->Char()",
                 "k_str2");
    SHOULD_EQUAL(*(strings2.GetKey(1)), "k-2", "GetKey(1)", "k-2");

    id = 0;
    SHOULD_EQUAL(strings2.GetValue(id)->Char(), c_str1, "strings[0].Char()",
                 "c_str1");
    SHOULD_EQUAL(strings2.GetValue(++id)->Char(), c_str2, "strings[1].Char()",
                 "c_str2");

    END_SUB_TEST;
}

static int TestHArray7() {
    ULong id = 1000;

    HArray<ULong> numbers1(id);

    for (ULong x = 0; x < id; x++) {
        numbers1[Digit::NumberToString(x)] = x;
    }

    for (ULong z = 0; z < id; z++) {
        numbers1.Delete(Digit::NumberToString(z));
        SHOULD_EQUAL(numbers1.GetKey(z), nullptr, "GetKey(id)->Char()", "null");
    }

    numbers1.Resize((id + 1));
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size()");

    numbers1.SetCapacity(id);
    for (ULong y = 0; y < id; y++) {
        numbers1[Digit::NumberToString(y)] = y;
    }

    do {
        --id;
        numbers1.Delete(Digit::NumberToString(id));
        SHOULD_EQUAL(numbers1.GetKey(id), nullptr, "GetKey(id)->Char()", id);
    } while (id > 0);

    numbers1.Resize((id + 1));
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size()");

    END_SUB_TEST;
}

static int TestHArray8() {
    UInt id = 10;

    HArray<UInt> list(id);

    for (UInt i = 0; i < id; i++) {
        String key("k-");
        key += Digit::NumberToString(i);

        list[key] = i;
    }

    for (UInt i = 0; i < id; i++) {
        String key1("k-");
        key1 += Digit::NumberToString(i);

        String key2 = Digit::NumberToString(i);
        key2 += "-k";

        SHOULD_EQUAL_TRUE(list.Rename(key1, key2), "Rename()");
        SHOULD_EQUAL(list.Find(key1), nullptr, "value", "null");
        SHOULD_EQUAL_TRUE((list.Rename(key1, key2) == false), "Rename()");

        UInt *y = list.Find(key2);
        UInt  w = id + 1;

        if (y != nullptr) {
            w = *y;
        }

        SHOULD_EQUAL(w, i, "value", i);
    }

    UInt j = id;
    do {
        --j;
        String key1("k-");
        key1 += Digit::NumberToString(j);

        String key2 = Digit::NumberToString(j);
        key2 += "-k";

        SHOULD_EQUAL_TRUE(list.Rename(key2, key1), "Rename()");
    } while (j > 0);

    for (UInt i = 0; i < id; i++) {
        String key1("k-");
        key1 += Digit::NumberToString(i);

        UInt *y = list.Find(key1);
        UInt  w = id + 1;

        if (y != nullptr) {
            w = *y;
        }

        SHOULD_EQUAL(w, i, "value", i);
    }

    for (UInt i = 0; i < id; i++) {
        String key2 = Digit::NumberToString(i);
        key2 += "-k";

        SHOULD_EQUAL(list.Find(key2), nullptr, "value", "null");
    }

    END_SUB_TEST;
}

static int TestHArray9() {
    UInt id = 10;

    HArray<UInt>                list(id);
    const Qentem::HAItem<UInt> *item;

    for (UInt i = 0; i < id; i++) {
        String key("k-");
        key += Digit::NumberToString(i);

        list[key] = i;
        item      = list.GetItem(i);

        SHOULD_NOT_EQUAL(item, nullptr, "item", "null");
        SHOULD_EQUAL(item->Key, key, "value", key.Char());
        SHOULD_EQUAL(item->Value, i, "value", i);
    }

    item = list.GetItem(id);
    SHOULD_EQUAL(item, nullptr, "item", "null");

    for (UInt i = 0; i < id; i++) {
        String key("k-");
        key += Digit::NumberToString(i);

        list.DeleteIndex(i);

        SHOULD_EQUAL(list.Find(key), nullptr, "value", "null");
    }

    list.Clear();

    for (UInt i = 0; i < id; i++) {
        String key("k-");
        key += Digit::NumberToString(i);
        list[key] = i;
    }

    for (UInt i = 0; i < id; i++) {
        list.DeleteIndex(i);
        list.DeleteIndex(i); // Just to see if something goes wrong.
    }

    for (UInt i = 0; i < id; i++) {
        String key("k-");
        key += Digit::NumberToString(i);
        SHOULD_EQUAL(list.Find(key), nullptr, "value", "null");
    }

    // No clear

    for (UInt i = 0; i < id; i++) {
        String key("k-");
        key += Digit::NumberToString(i);
        list[key] = i;
    }

    for (UInt i = 0; i < id; i++) {
        list.DeleteIndex(i);
    }

    for (UInt i = 0; i < id; i++) {
        String key("k-");
        key += Digit::NumberToString(i);
        SHOULD_EQUAL(list.Find(key), nullptr, "value", "null");
    }

    END_SUB_TEST;
}

static int TestHArray10() {
    UInt id = 10;

    HArray<UInt> list(id);

    for (UInt i = 0; i < id; i++) {
        String key("k-");
        key += Digit::NumberToString(i);

        list[key] = i;

        const Qentem::HAItem<UInt> *item = list.GetItem(key);

        SHOULD_NOT_EQUAL(item, nullptr, "item", "null");
        SHOULD_EQUAL_VALUE(item->Value, i, "index");
    }

    END_SUB_TEST;
}

static int RunHArrayTests() {
    STARTING_TEST("HArray.hpp");

    START_TEST("Hash Test", TestHash);
    START_TEST("HArray Test 1", TestHArray1);
    START_TEST("HArray Test 2", TestHArray2);
    START_TEST("HArray Test 3", TestHArray3);
    START_TEST("HArray Test 4", TestHArray4);
    START_TEST("HArray Test 5", TestHArray5);
    START_TEST("HArray Test 6", TestHArray6);
    START_TEST("HArray Test 7", TestHArray7);
    START_TEST("HArray Test 8", TestHArray8);
    START_TEST("HArray Test 9", TestHArray9);
    START_TEST("HArray Test 10", TestHArray10);

    END_TEST("HArray.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
