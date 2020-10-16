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

#ifndef QENTEM_HARRAY_TESTS_H_
#define QENTEM_HARRAY_TESTS_H_

namespace Qentem {
namespace Test {

using HashArray = HArray<SizeT, char>;

static int TestHArray1() {
    HashArray numbers1;
    HashArray numbers2(8);

    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 0, "Capacity");
    EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_TO(numbers1.End(), nullptr, "End()", "null");

    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_TRUE(numbers2.Capacity() >= 8, "Capacity>= 8");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");

    numbers2.Reset();
    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_VALUE(numbers2.Capacity(), 0, "Capacity");
    EQ_TO(numbers2.First(), nullptr, "First()", "null");

    numbers1.Reserve(5);
    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 8, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");

    numbers1.Reserve(10);
    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 16, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");

    numbers1.Resize(18);
    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 32, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");

    numbers2.Resize(4);
    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_TRUE(numbers2.Capacity() >= 4, "Capacity>= 4");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");

    numbers2.Resize(5);
    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_VALUE(numbers2.Capacity(), 8, "Capacity");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");

    numbers2.Resize(2);
    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_TRUE(numbers2.Capacity() >= 2, "Capacity>= 2");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");

    numbers1.Reserve(0);
    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 0, "Capacity");
    EQ_TO(numbers1.First(), nullptr, "First()", "null");

    numbers2.Resize(0);
    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_VALUE(numbers2.Capacity(), 0, "Capacity");

    END_SUB_TEST;
}

static int TestHArray2() {
    HashArray                  numbers1(8);
    HashArray                  numbers2;
    const String<char> *       key;
    const char *               str_c;
    const HAItem<SizeT, char> *storage;

    numbers1["key1"] = 1;
    EQ_VALUE(numbers1.Size(), 1, "Size");
    EQ_TRUE(numbers1.Capacity() >= 8, "Capacity>= 8");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_TO(numbers1.End(), (numbers1.First() + 1), "End()", "null");
    EQ_VALUE(numbers1["key1"], 1, "key1");

    SizeT cap        = numbers1.Capacity();
    numbers1["key1"] = 20;
    EQ_VALUE(numbers1.Size(), 1, "Size");
    EQ_VALUE(numbers1.Capacity(), cap, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers1["key1"], 20, "key1");

    numbers1["key1"] = 300;
    EQ_VALUE(numbers1.Size(), 1, "Size");
    EQ_VALUE(numbers1.Capacity(), cap, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers1["key1"], 300, "key1");

    numbers1["key1"] = 4000;
    EQ_VALUE(numbers1.Size(), 1, "Size");
    EQ_VALUE(numbers1.Capacity(), cap, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers1["key1"], 4000, "key1");

    numbers1["key1"] = 10000;
    EQ_VALUE(numbers1.Size(), 1, "Size");
    EQ_VALUE(numbers1.Capacity(), cap, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers1["key1"], 10000, "key1");

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

    EQ_VALUE(numbers1.Size(), 7, "Size");
    EQ_VALUE(numbers1.Capacity(), cap, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers1["key1"], 10, "key1");
    EQ_VALUE(numbers1["key2"], 20, "key2");
    EQ_VALUE(numbers1["key3"], 30, "key3");
    EQ_VALUE(numbers1["key4"], 40, "key4");
    EQ_VALUE(numbers1["key5"], 50, "key5");
    EQ_VALUE(numbers1["key6"], 60, "key6");
    EQ_VALUE(numbers1["key7"], 70, "key7");

    key = numbers1.GetKey(0);
    NOT_EQ_TO(key, nullptr, "key", "null");
    EQ_TRUE(key->IsEqual("key1", 4), "(GetKey(0) == key1)");
    key = numbers1.GetKey(1);
    NOT_EQ_TO(key, nullptr, "key", "null");
    EQ_TRUE(key->IsEqual("key2", 4), "(GetKey(1) == key2)");
    key = numbers1.GetKey(2);
    NOT_EQ_TO(key, nullptr, "key", "null");
    EQ_TRUE(key->IsEqual("key3", 4), "(GetKey(2) == key3)");
    key = numbers1.GetKey(3);
    NOT_EQ_TO(key, nullptr, "key", "null");
    EQ_TRUE(key->IsEqual("key4", 4), "(GetKey(3) == key4)");
    key = numbers1.GetKey(4);
    NOT_EQ_TO(key, nullptr, "key", "null");
    EQ_TRUE(key->IsEqual("key5", 4), "(GetKey(4) == key5)");
    key = numbers1.GetKey(5);
    NOT_EQ_TO(key, nullptr, "key", "null");
    EQ_TRUE(key->IsEqual("key6", 4), "(GetKey(5) == key6)");
    key = numbers1.GetKey(6);
    NOT_EQ_TO(key, nullptr, "key", "null");
    EQ_TRUE(key->IsEqual("key7", 4), "(GetKey(6) == key7)");

    key   = numbers1.GetKey(5);
    str_c = key->First();
    // Test expanding
    numbers1["key8"] = 80;
    numbers1["key9"] = 90;
    EQ_VALUE(numbers1.Size(), 9, "Size");
    EQ_TRUE((numbers1.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers1["key1"], 10, "key1");
    EQ_VALUE(numbers1["key2"], 20, "key2");
    EQ_VALUE(numbers1["key3"], 30, "key3");
    EQ_VALUE(numbers1["key4"], 40, "key4");
    EQ_VALUE(numbers1["key5"], 50, "key5");
    EQ_VALUE(numbers1["key6"], 60, "key6");
    EQ_VALUE(numbers1["key7"], 70, "key7");
    EQ_VALUE(numbers1["key8"], 80, "key8");
    EQ_VALUE(numbers1["key9"], 90, "key9");

    const String<char> *key2 = numbers1.GetKey(5);
    NOT_EQ_TO(key2, key, "str", "GetKey(5)");
    NOT_EQ_TO(key2, nullptr, "key", "null");
    EQ_VALUE(key2->First(), str_c, "GetKey(5)->First()");

    storage  = numbers1.First();
    numbers2 = numbers1;
    EQ_VALUE(numbers1.Size(), 9, "Size");
    EQ_TRUE((numbers1.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers2.Size(), 9, "Size");
    EQ_TRUE((numbers2.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");
    NOT_EQ_TO(numbers2.First(), storage, "First()", "storage");
    NOT_EQ_TO(numbers1.GetKey(5), numbers2.GetKey(5), "numbers1.GetKey(5)",
              "numbers2.GetKey(5)");
    NOT_EQ_TO(key->First(), str_c, "str_c", "GetKey(5)->First()");

    numbers2 = static_cast<HashArray &&>(numbers1);
    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 0, "Capacity");
    EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers2.Size(), 9, "Size");
    EQ_TRUE((numbers2.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers2.First(), storage, "First()");
    EQ_VALUE(numbers2["key1"], 10, "key1");
    EQ_VALUE(numbers2["key2"], 20, "key2");
    EQ_VALUE(numbers2["key3"], 30, "key3");
    EQ_VALUE(numbers2["key4"], 40, "key4");
    EQ_VALUE(numbers2["key5"], 50, "key5");
    EQ_VALUE(numbers2["key6"], 60, "key6");
    EQ_VALUE(numbers2["key7"], 70, "key7");
    EQ_VALUE(numbers2["key8"], 80, "key8");
    EQ_VALUE(numbers2["key9"], 90, "key9");

    numbers1 = HashArray(numbers2);
    EQ_VALUE(numbers1.Size(), 9, "Size");
    EQ_TRUE((numbers1.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers2.Size(), 9, "Size");
    EQ_TRUE((numbers2.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");
    NOT_EQ_TO(numbers1.First(), storage, "First()", "storage");
    EQ_VALUE(numbers2.First(), storage, "First()");
    EQ_VALUE(numbers1["key1"], 10, "key1");
    EQ_VALUE(numbers1["key2"], 20, "key2");
    EQ_VALUE(numbers1["key3"], 30, "key3");
    EQ_VALUE(numbers1["key4"], 40, "key4");
    EQ_VALUE(numbers1["key5"], 50, "key5");
    EQ_VALUE(numbers1["key6"], 60, "key6");
    EQ_VALUE(numbers1["key7"], 70, "key7");
    EQ_VALUE(numbers1["key8"], 80, "key8");
    EQ_VALUE(numbers1["key9"], 90, "key9");

    END_SUB_TEST;
}

static int TestHArray3() {
    HashArray                  numbers1(8);
    HashArray                  numbers2;
    const String<char> *       key;
    const char *               str_c;
    const HAItem<SizeT, char> *storage;

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
    NOT_EQ_TO(key, nullptr, "key", "null");
    str_c = key->First();

    storage  = numbers2.First();
    numbers1 = static_cast<HashArray &&>(numbers2);

    EQ_VALUE(numbers1.Size(), 9, "Size");
    EQ_TRUE((numbers1.Capacity() >= 9), "(numbers1.Capacity() >= 9)");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers1.First(), storage, "First()");
    EQ_VALUE(numbers1["key1"], 10, "key1");
    EQ_VALUE(numbers1["key2"], 20, "key2");
    EQ_VALUE(numbers1["key3"], 30, "key3");
    EQ_VALUE(numbers1["key4"], 40, "key4");
    EQ_VALUE(numbers1["key5"], 50, "key5");
    EQ_VALUE(numbers1["key6"], 60, "key6");
    EQ_VALUE(numbers1["key7"], 70, "key7");
    EQ_VALUE(numbers1["key8"], 80, "key8");
    EQ_VALUE(numbers1["key9"], 90, "key9");
    EQ_TO(numbers1.GetKey(5), key, "key", "GetKey(5)");
    EQ_VALUE(numbers1.GetKey(5)->First(), str_c, "GetKey(5)->First()");

    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_VALUE(numbers2.Capacity(), 0, "Capacity");
    EQ_TO(numbers2.First(), nullptr, "First()", "null");

    END_SUB_TEST;
}

static int TestHArray4() {
    HashArray                  numbers1;
    HashArray                  numbers2;
    HashArray                  numbers3(3);
    const HAItem<SizeT, char> *storage;
    SizeT *                    value;

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
    EQ_VALUE(numbers1.Size(), 10, "Size");
    EQ_TRUE((numbers1.Capacity() >= 10), "(numbers1.Capacity() >= 10)");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers2.Size(), 6, "Size");
    EQ_TRUE((numbers2.Capacity() >= 6), "(numbers2.Capacity() >= 6)");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");
    NOT_EQ_TO(numbers2.First(), numbers1.First(), "First()", "null");
    EQ_VALUE(numbers1["key1"], 100, "key1");
    EQ_VALUE(numbers1["key2"], 20, "key2");
    EQ_VALUE(numbers1["key3"], 300, "key3");
    EQ_VALUE(numbers1["key4"], 40, "key4");
    EQ_VALUE(numbers1["key5"], 50, "key5");
    EQ_VALUE(numbers1["key6"], 600, "key6");
    EQ_VALUE(numbers1["key7"], 70, "key7");
    EQ_VALUE(numbers1["key8"], 800, "key8");
    EQ_VALUE(numbers1["key9"], 900, "key9");
    EQ_VALUE(numbers1["key10"], 1000, "key10");

    SizeT        index = 0;
    const SizeT *val   = numbers1.GetValue(index);
    NOT_EQ_TO(val, nullptr, "GetValue(index)", "null");
    EQ_VALUE(*val, 100, "0");

    val = numbers1.GetValue(++index);
    NOT_EQ_TO(val, nullptr, "GetValue(index)", "null");
    EQ_VALUE(*val, 20, "1");

    val = numbers1.GetValue(++index);
    NOT_EQ_TO(val, nullptr, "GetValue(index)", "null");
    EQ_VALUE(*val, 300, "2");

    val = numbers1.GetValue(++index);
    NOT_EQ_TO(val, nullptr, "GetValue(index)", "null");
    EQ_VALUE(*val, 40, "3");

    val = numbers1.GetValue(++index);
    NOT_EQ_TO(val, nullptr, "GetValue(index)", "null");
    EQ_VALUE(*val, 50, "4");

    val = numbers1.GetValue(++index);
    NOT_EQ_TO(val, nullptr, "GetValue(index)", "null");
    EQ_VALUE(*val, 600, "5");

    val = numbers1.GetValue(++index);
    NOT_EQ_TO(val, nullptr, "GetValue(index)", "null");
    EQ_VALUE(*val, 70, "6");

    val = numbers1.GetValue(++index);
    NOT_EQ_TO(val, nullptr, "GetValue(index)", "null");
    EQ_VALUE(*val, 800, "7");

    val = numbers1.GetValue(++index);
    NOT_EQ_TO(val, nullptr, "GetValue(index)", "null");
    EQ_VALUE(*val, 900, "8");

    val = numbers1.GetValue(++index);
    NOT_EQ_TO(val, nullptr, "GetValue(index)", "null");
    EQ_VALUE(*val, 1000, "9");

    val = numbers1.GetValue(99999);
    EQ_TO(val, nullptr, "GetValue(index)", "null");

    numbers1 = numbers3; // Restore
    numbers3 = numbers2; // Backup
    numbers2 += numbers1;
    EQ_VALUE(numbers2["key1"], 10, "key1");
    EQ_VALUE(numbers2["key2"], 20, "key2");
    EQ_VALUE(numbers2["key3"], 30, "key3");
    EQ_VALUE(numbers2["key4"], 40, "key4");
    EQ_VALUE(numbers2["key5"], 50, "key5");
    EQ_VALUE(numbers2["key6"], 60, "key6");
    EQ_VALUE(numbers2["key7"], 70, "key7");
    EQ_VALUE(numbers2["key8"], 80, "key8");
    EQ_VALUE(numbers2["key9"], 900, "key9");
    EQ_VALUE(numbers2["key10"], 1000, "key10");

    numbers2 = numbers3; // Restore
    numbers3 = numbers1; // Backup

    storage = numbers1.First();
    numbers2 += static_cast<HashArray &&>(numbers1);
    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 0, "Capacity");
    EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers2.Size(), 10, "Size");
    EQ_TRUE((numbers2.Capacity() >= 10), "(numbers2.Capacity() >= 10)");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");
    NOT_EQ_TO(numbers2.First(), storage, "First()", "storage");
    EQ_VALUE(numbers2["key1"], 10, "key1");
    EQ_VALUE(numbers2["key2"], 20, "key2");
    EQ_VALUE(numbers2["key3"], 30, "key3");
    EQ_VALUE(numbers2["key4"], 40, "key4");
    EQ_VALUE(numbers2["key5"], 50, "key5");
    EQ_VALUE(numbers2["key6"], 60, "key6");
    EQ_VALUE(numbers2["key7"], 70, "key7");
    EQ_VALUE(numbers2["key8"], 80, "key8");
    EQ_VALUE(numbers2["key9"], 900, "key9");
    EQ_VALUE(numbers2["key10"], 1000, "key10");

    // Addition of an empty array does nothing.
    numbers2 += HashArray(10);
    EQ_VALUE(numbers2.Size(), 10, "Size");

    numbers2.Resize(1);
    EQ_VALUE(numbers2["key1"], 10, "key1");

    EQ_TO(numbers2.GetKey(1), nullptr, "GetKey(1)", "null");

    numbers1.Reset();
    numbers1[""] = 555;
    EQ_VALUE(numbers1[""], 555, "empty key");

    numbers1.Reset();

    for (SizeT i = 0; i < 10; i++) {
        String<char> key = Digit<char>::NumberToString(i);

        numbers1.Insert(Digit<char>::NumberToString(i),
                        static_cast<SizeT &&>(SizeT{i}));
        value = numbers1.Find(key);
        NOT_EQ_TO(value, nullptr, "value", "null");
        EQ_VALUE(*value, i, key.First());
    }

    END_SUB_TEST;
}

static int TestHArray5() {
    HArray<SizeT, char> numbers1;
    String<char>        key;
    SizeT *             value;

    for (SizeT i = 1; i < 11; i++) {
        key = "k-";
        key += Digit<char>::NumberToString(i);

        numbers1[key] = i;

        value = numbers1.Find(key.First(), key.Length());
        NOT_EQ_TO(value, nullptr, "value", "null");
        EQ_VALUE(*value, i, key.First());
    }

    numbers1.Reset();

    for (SizeT i = 1; i < 1001; i++) {
        numbers1[Digit<char>::NumberToString(i)] = i;

        value = numbers1.Find(Digit<char>::NumberToString(i));
        NOT_EQ_TO(value, nullptr, "value", "null");
        EQ_VALUE(*value, i, key.First());
    }

    for (SizeT i = 1; i < 10; i++) {
        EQ_VALUE(numbers1[Digit<char>::NumberToString(i)], i, "key.First()");
    }

    value = numbers1.Find("10", 2);
    NOT_EQ_TO(value, nullptr, "value", "null");
    EQ_VALUE(*value, 10, "10");

    for (SizeT i = 1; i < 11; i++) {
        numbers1.Remove(Digit<char>::NumberToString(i));
    }

    for (SizeT i = 1; i < 11; i++) {
        EQ_TO(numbers1.Find(Digit<char>::NumberToString(i)), nullptr, "value",
              "null");
    }

    numbers1.Compress();
    EQ_VALUE(numbers1.Size(), 990, "Size");
    EQ_TRUE((numbers1.Capacity() >= 990), "(numbers1.Capacity() >= 990)");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");

    for (SizeT i = 1; i < 101; i++) {
        numbers1.Remove(Digit<char>::NumberToString(i));
    }

    for (SizeT i = 1; i < 101; i++) {
        EQ_TO(numbers1.Find(Digit<char>::NumberToString(i)), nullptr, "value",
              "null");
    }

    numbers1.Compress();
    EQ_VALUE(numbers1.Size(), 900, "Size");
    EQ_TRUE((numbers1.Capacity() >= 900), "(numbers1.Capacity() >= 900)");

    for (SizeT i = 101; i < 201; i++) {
        numbers1.Remove(Digit<char>::NumberToString(i));
    }

    for (SizeT i = 101; i < 201; i++) {
        EQ_TO(numbers1.Find(Digit<char>::NumberToString(i)), nullptr, "value",
              "null");
    }

    numbers1.Compress();
    EQ_VALUE(numbers1.Size(), 800, "Size");
    EQ_TRUE((numbers1.Capacity() >= 800), "(numbers1.Capacity() >= 800)");

    for (SizeT i = 0; i < 1001; i++) {
        key = Digit<char>::NumberToString(i);
        numbers1.Remove(key);
    }

    for (SizeT i = 0; i < 1001; i++) {
        EQ_TO(numbers1.Find(Digit<char>::NumberToString(i)), nullptr, "value",
              "null");
    }

    numbers1.Compress();
    EQ_VALUE(numbers1.Size(), 0, "Size");

    numbers1.Reserve(16);
    numbers1["a"] = 1;
    numbers1.Compress();
    EQ_VALUE(numbers1.Size(), 1, "Size");
    EQ_TRUE(numbers1.Capacity() >= 1, "Capacity() >= 1");

    END_SUB_TEST;
}

static int TestHArray6() {
    SizeT                             id;
    HArray<String<char>, char>        strings1;
    HArray<String<char>, char>        strings2;
    const HAItem<String<char>, char> *storage;

    String<char> key1("k-1");
    String<char> key2("k-2");

    String<char> str1("val1");
    String<char> str2("val2");

    const char *k_str1 = key1.First();
    const char *k_str2 = key2.First();
    const char *c_str1 = str1.First();
    const char *c_str2 = str2.First();

    strings1[static_cast<String<char> &&>(key1)] =
        static_cast<String<char> &&>(str1);
    strings1[static_cast<String<char> &&>(key2)] =
        static_cast<String<char> &&>(str2);

    EQ_VALUE(strings1.Size(), 2, "Size");
    NOT_EQ_TO(strings1.First(), nullptr, "First()", "null");
    NOT_EQ_TO(strings1.GetKey(0), nullptr, "GetKey(0)", "null");
    EQ_TO(strings1.GetKey(0)->First(), k_str1, "GetKey(0)->First()", "k_str1");
    EQ_TO(*(strings1.GetKey(0)), "k-1", "GetKey(0)", "k-1");
    NOT_EQ_TO(strings1.GetKey(1), nullptr, "GetKey(1)", "null");
    EQ_TO(strings1.GetKey(1)->First(), k_str2, "GetKey(1)->First()", "k_str2");
    EQ_TO(*(strings1.GetKey(1)), "k-2", "GetKey(1)", "k-2");

    id = 0;
    EQ_TO(strings1.GetValue(id)->First(), c_str1, "strings[0].First()",
          "c_str1");
    EQ_TO(strings1.GetValue(++id)->First(), c_str2, "strings[1].First()",
          "c_str2");

    strings2 += strings1;
    EQ_VALUE(strings2.Size(), 2, "Size");
    NOT_EQ_TO(strings2.First(), nullptr, "First()", "null");
    NOT_EQ_TO(strings2.GetKey(0), nullptr, "GetKey(0)", "null");
    NOT_EQ_TO(strings2.GetKey(0)->First(), k_str1, "GetKey(0)->First()",
              "k_str1");
    EQ_TO(*(strings2.GetKey(0)), "k-1", "GetKey(0)", "k-1");
    NOT_EQ_TO(strings2.GetKey(1), nullptr, "GetKey(1)", "null");
    NOT_EQ_TO(strings2.GetKey(1)->First(), k_str2, "GetKey(1)->First()",
              "k_str2");
    EQ_TO(*(strings2.GetKey(1)), "k-2", "GetKey(1)", "k-2");

    id = 0;
    NOT_EQ_TO(strings2.GetValue(id)->First(), c_str1, "strings[0].First()",
              "c_str1");
    NOT_EQ_TO(strings2.GetValue(++id)->First(), c_str2, "strings[1].First()",
              "c_str2");

    strings2.Reserve(2);
    storage = strings1.First();
    strings2 += static_cast<HArray<String<char>, char> &&>(strings1);
    EQ_VALUE(strings2.Size(), 2, "Size()");
    EQ_VALUE(strings2.Capacity(), 2, "Capacity()");
    NOT_EQ_TO(strings2.First(), nullptr, "First()", "null");
    NOT_EQ_TO(strings2.First(), storage, "First()", "storage");
    NOT_EQ_TO(strings2.GetKey(0), nullptr, "GetKey(0)", "null");
    EQ_TO(strings2.GetKey(0)->First(), k_str1, "GetKey(0)->First()", "k_str1");
    EQ_TO(*(strings2.GetKey(0)), "k-1", "GetKey(0)", "k-1");
    NOT_EQ_TO(strings2.GetKey(1), nullptr, "GetKey(1)", "null");
    EQ_TO(strings2.GetKey(1)->First(), k_str2, "GetKey(1)->First()", "k_str2");
    EQ_TO(*(strings2.GetKey(1)), "k-2", "GetKey(1)", "k-2");

    id = 0;
    EQ_TO(strings2.GetValue(id)->First(), c_str1, "strings[0].First()",
          "c_str1");
    EQ_TO(strings2.GetValue(++id)->First(), c_str2, "strings[1].First()",
          "c_str2");

    strings1 += static_cast<HArray<String<char>, char> &&>(strings2);
    strings1.Resize(10);
    storage = strings1.First();
    strings2.Reset();
    strings2 += static_cast<HArray<String<char>, char> &&>(strings1);
    EQ_VALUE(strings2.Size(), 2, "Size");
    EQ_VALUE(strings2.Capacity(), 2, "Capacity()");
    NOT_EQ_TO(strings2.First(), nullptr, "First()", "null");
    NOT_EQ_TO(strings2.First(), storage, "First()", "storage");
    NOT_EQ_TO(strings2.GetKey(0), nullptr, "GetKey(0)", "null");
    EQ_TO(strings2.GetKey(0)->First(), k_str1, "GetKey(0)->First()", "k_str1");
    EQ_TO(*(strings2.GetKey(0)), "k-1", "GetKey(0)", "k-1");
    NOT_EQ_TO(strings2.GetKey(1), nullptr, "GetKey(1)", "null");
    EQ_TO(strings2.GetKey(1)->First(), k_str2, "GetKey(1)->First()", "k_str2");
    EQ_TO(*(strings2.GetKey(1)), "k-2", "GetKey(1)", "k-2");

    id = 0;
    EQ_TO(strings2.GetValue(id)->First(), c_str1, "strings[0].First()",
          "c_str1");
    EQ_TO(strings2.GetValue(++id)->First(), c_str2, "strings[1].First()",
          "c_str2");

    END_SUB_TEST;
}

static int TestHArray7() {
    SizeT id = 1000;

    HArray<SizeT, char> numbers1(id);

    for (SizeT x = 0; x < id; x++) {
        numbers1[Digit<char>::NumberToString(x)] = x;
    }

    for (SizeT z = 0; z < id; z++) {
        numbers1.Remove(Digit<char>::NumberToString(z));
        EQ_TO(numbers1.GetKey(z), nullptr, "GetKey(id)->First()", "null");
    }

    numbers1.Resize((id + 1));
    EQ_VALUE(numbers1.Size(), 0, "Size()");

    numbers1.Reserve(id);
    for (SizeT y = 0; y < id; y++) {
        numbers1[Digit<char>::NumberToString(y)] = y;
    }

    do {
        --id;
        numbers1.Remove(Digit<char>::NumberToString(id));
        EQ_TO(numbers1.GetKey(id), nullptr, "GetKey(id)->First()", id);
    } while (id > 0);

    numbers1.Resize((id + 1));
    EQ_VALUE(numbers1.Size(), 0, "Size()");

    END_SUB_TEST;
}

static int TestHArray8() {
    SizeT id = 100;

    HArray<SizeT, char> list(id);

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");
        key += Digit<char>::NumberToString(i);

        list[key] = i;
    }

    for (SizeT i = 0; i < id; i++) {
        String<char> key1("k-");
        key1 += Digit<char>::NumberToString(i);

        String<char> key2 = Digit<char>::NumberToString(i);
        key2 += "-k";

        EQ_TRUE(list.Rename(key1, key2), "Rename()");
        EQ_TO(list.Find(key1), nullptr, "value", "null");
        EQ_FALSE(list.Rename(key1, key2), "Rename()");

        SizeT *y = list.Find(key2);
        SizeT  w = id + 1;

        if (y != nullptr) {
            w = *y;
        }

        EQ_TO(w, i, "value", i);
    }

    SizeT j = id;
    do {
        --j;
        String<char> key1("k-");
        key1 += Digit<char>::NumberToString(j);

        String<char> key2 = Digit<char>::NumberToString(j);
        key2 += "-k";

        EQ_TRUE(list.Rename(key2, key1), "Rename()");
    } while (j > 0);

    for (SizeT i = 0; i < id; i++) {
        String<char> key1("k-");
        key1 += Digit<char>::NumberToString(i);

        SizeT *y = list.Find(key1);
        SizeT  w = id + 1;

        if (y != nullptr) {
            w = *y;
        }

        EQ_TO(w, i, "value", i);
    }

    for (SizeT i = 0; i < id; i++) {
        String<char> key2 = Digit<char>::NumberToString(i);
        key2 += "-k";

        EQ_TO(list.Find(key2), nullptr, "value", "null");
    }

    END_SUB_TEST;
}

static int TestHArray9() {
    SizeT id = 10;

    HArray<SizeT, char>                list(id);
    const Qentem::HAItem<SizeT, char> *item;

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");
        key += Digit<char>::NumberToString(i);

        list[key] = i;
        item      = list.GetItem(i);

        NOT_EQ_TO(item, nullptr, "item", "null");
        EQ_TO(item->Key, key, "value", key.First());
        EQ_TO(item->Value, i, "value", i);
    }

    item = list.GetItem(id);
    EQ_TO(item, nullptr, "item", "null");

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");
        key += Digit<char>::NumberToString(i);

        list.RemoveIndex(i);

        EQ_TO(list.Find(key), nullptr, "value", "null");
    }

    list.Reset();

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");
        key += Digit<char>::NumberToString(i);
        list[key] = i;
    }

    for (SizeT i = 0; i < id; i++) {
        list.RemoveIndex(i);
        list.RemoveIndex(i); // Just to see if something goes wrong.
    }

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");
        key += Digit<char>::NumberToString(i);
        EQ_TO(list.Find(key), nullptr, "value", "null");
    }

    // No clear

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");
        key += Digit<char>::NumberToString(i);
        list[key] = i;
    }

    for (SizeT i = 0; i < id; i++) {
        list.RemoveIndex(i);
    }

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");
        key += Digit<char>::NumberToString(i);
        EQ_TO(list.Find(key), nullptr, "value", "null");
    }

    END_SUB_TEST;
}

static int TestHArray10() {
    SizeT id = 10;

    HArray<SizeT, char> list(id);

    for (SizeT i = 0; i < id; i++) {
        String<char> key("k-");
        key += Digit<char>::NumberToString(i);

        list[key] = i;

        const Qentem::HAItem<SizeT, char> *item = list.GetItem(key);

        NOT_EQ_TO(item, nullptr, "item", "null");
        EQ_VALUE(item->Value, i, "index");
    }

    String<char> key1("k-");
    key1 += Digit<char>::NumberToString(100);
    EQ_TO(list.GetItem(key1), nullptr, "GetItem(k-100)", "null");

    END_SUB_TEST;
}

static int RunHArrayTests() {
    STARTING_TEST("HArray.hpp");

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
