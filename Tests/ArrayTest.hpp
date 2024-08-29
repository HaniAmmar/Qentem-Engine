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

#ifndef QENTEM_ARRAY_TESTS_H
#define QENTEM_ARRAY_TESTS_H

#include "QTest.hpp"
#include "Array.hpp"
#include "String.hpp"

namespace Qentem {
namespace Test {

static void TestArray1(QTest &test) {
    const SizeT *storage;
    Array<SizeT> numbers1;
    Array<SizeT> numbers2(8);

    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsTrue(numbers1.IsEmpty(), __LINE__);
    test.IsFalse(numbers1.IsNotEmpty(), __LINE__);
    test.IsEqual(numbers1.Capacity(), 0U, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);
    test.IsNull(numbers1.Last(), __LINE__);

    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 8U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);

    numbers2.Reset();
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 0U, __LINE__);
    test.IsNull(numbers2.First(), __LINE__);

    numbers1.Reserve(5);
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 5U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);

    numbers1.Reserve(10);
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 10U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);

    numbers1.Resize(18);
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 18U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);

    storage = numbers1.First();
    numbers1.Resize(4);
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 4U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotEqual(numbers1.First(), storage, __LINE__);

    numbers2.Resize(5);
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 5U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);

    numbers2.Resize(2);
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 2U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);

    numbers2.Resize(0);
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 0U, __LINE__);
    test.IsNull(numbers2.First(), __LINE__);

    numbers1.Reserve(0);
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 0U, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);

    numbers1.Reset();
    numbers2.Reset();

    numbers1.ResizeAndInitialize(8);
    storage = numbers1.First();
    test.IsEqual(numbers1.Size(), 8U, __LINE__);
    test.IsFalse(numbers1.IsEmpty(), __LINE__);
    test.IsTrue(numbers1.IsNotEmpty(), __LINE__);
    test.IsEqual(numbers1.Capacity(), 8U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);

    numbers1.ResizeAndInitialize(4);
    test.IsNotEqual(numbers1.First(), storage, __LINE__);

    storage  = numbers1.First();
    numbers2 = numbers1;
    test.IsEqual(numbers1.Size(), 4U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 4U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers2.Size(), 4U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 4U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);
    test.IsNotEqual(numbers2.First(), storage, __LINE__);

    numbers1.ResizeAndInitialize(16);
    test.IsEqual(numbers1.Size(), 16U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 16U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);

    storage  = numbers1.First();
    numbers2 = numbers1;
    test.IsEqual(numbers2.Size(), 16U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 16U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);
    test.IsNotEqual(numbers2.First(), storage, __LINE__);

    numbers2 = Memory::Move(numbers1);
    test.IsEqual(numbers2.Size(), 16U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 16U, __LINE__);
    test.IsEqual(numbers2.First(), storage, __LINE__);

    test.IsNull(numbers1.Storage(), __LINE__);
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 0U, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);

    storage  = numbers1.First();
    numbers1 = Array<SizeT>(numbers2);
    test.IsEqual(numbers1.Size(), 16U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 16U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotEqual(numbers1.First(), storage, __LINE__);

    storage  = numbers2.First();
    numbers1 = Array<SizeT>(Memory::Move(numbers2));
    test.IsEqual(numbers1.Size(), 16U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 16U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers1.First(), storage, __LINE__);
    test.IsNull(numbers2.Storage(), __LINE__);
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 0U, __LINE__);
    test.IsNull(numbers2.First(), __LINE__);

    storage = numbers1.First();
    numbers1.Clear();
    test.IsEqual(numbers1.Size(), 0U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 16U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers1.First(), storage, __LINE__);

    numbers1.ResizeAndInitialize(3);
    numbers2.ResizeAndInitialize(5);
    storage = numbers2.First();

    numbers1 += numbers2;
    test.IsEqual(numbers1.Size(), 8U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 8U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotEqual(numbers1.First(), storage, __LINE__);

    storage = numbers1.First();
    numbers1 += Memory::Move(numbers2);
    test.IsEqual(numbers1.Size(), 13U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 13U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotEqual(numbers1.First(), storage, __LINE__);
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 0U, __LINE__);
    test.IsNull(numbers2.First(), __LINE__);

    numbers2.ResizeAndInitialize(5);
    numbers1.Resize(18);
    storage = numbers1.First();
    numbers1 += Memory::Move(numbers2);
    test.IsEqual(numbers1.Size(), 18U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 18U, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers1.First(), storage, __LINE__);

    storage = numbers1.First();
    numbers2 += Memory::Move(numbers1);
    test.IsEqual(numbers2.Size(), 18U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 18U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);
    test.IsEqual(numbers2.First(), storage, __LINE__);

    SizeT *tmp = numbers2.Detach();
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 0U, __LINE__);
    test.IsNull(numbers2.First(), __LINE__);
    test.IsEqual(tmp, storage, __LINE__);

    Memory::Deallocate(tmp);
}

static void TestArray2(QTest &test) {
    const SizeT *storage;
    Array<SizeT> numbers1;
    Array<SizeT> numbers2;
    Array<SizeT> numbers3;

    numbers2.Reserve(4);
    numbers1.ResizeAndInitialize(4);
    storage = numbers2.First();
    numbers2.Insert(numbers1);
    test.IsEqual(numbers2.Size(), 4U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 4U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);
    test.IsEqual(numbers2.First(), storage, __LINE__);
    test.IsEqual(numbers2.Last(), (storage + 3), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);

    numbers1.Reserve(10);
    numbers2.ResizeAndInitialize(4);
    numbers2.Insert(numbers1);
    test.IsEqual(numbers2.Size(), 4U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 4U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);

    numbers1.ResizeAndInitialize(4);
    numbers2.Insert(numbers1);
    test.IsEqual(numbers2.Size(), 8U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 8U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);

    numbers3.Reserve(8U, true);
    test.IsEqual(numbers3.Size(), 8U, __LINE__);
    test.IsEqual(numbers3.Capacity(), 8U, __LINE__);
    test.IsNotNull(numbers3.First(), __LINE__);

    storage = numbers1.First();
    numbers2.Insert(Memory::Move(numbers1));
    test.IsEqual(numbers2.Size(), 12U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 12U, __LINE__);
    test.IsNotNull(numbers2.First(), __LINE__);
    test.IsNotEqual(numbers2.First(), storage, __LINE__);
}

static void TestArray3(QTest &test) {
    Array<SizeT>        numbers1;
    Array<SizeT>        numbers2(8);
    Array<String<char>> strings;

    for (SizeT i = 0; i < 8; i++) {
        numbers1 += i;
    }

    const SizeT *storage1 = numbers1.Storage();
    const SizeT *storage2 = numbers1.Storage();

    test.IsEqual(numbers1.Size(), 8U, __LINE__);
    test.IsTrue((numbers1.Capacity() >= 8U), __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);

    SizeT sum = 0;
    for (const SizeT &num : numbers1) {
        sum += num;
    }

    test.IsEqual(sum, 28U, __LINE__);

    numbers2.Insert(storage1[0]);
    numbers2.Insert(storage1[1]);

    for (SizeT i = 2; i < 8; i++) {
        numbers2.Insert(storage1[i]);
        test.IsEqual(storage2[i], storage1[i], __LINE__);
    }

    test.IsEqual(numbers2.Size(), 8U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 8U, __LINE__);

    numbers2.Resize(16);
    test.IsEqual(numbers2.Size(), 8U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 16U, __LINE__);

    numbers2.Compress();
    test.IsEqual(numbers2.Size(), 8U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 8U, __LINE__);

    storage2 = numbers2.Storage();
    numbers2.Clear();
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 8U, __LINE__);
    test.IsEqual(numbers2.Storage(), storage2, __LINE__);

    numbers2.Reset();
    test.IsEqual(numbers2.Size(), 0U, __LINE__);
    test.IsEqual(numbers2.Capacity(), 0U, __LINE__);

    // Checking move
    String<char> str1("val--------------------------------1");
    String<char> str2("val--------------------------------2");

    const char *str1_cstr = str1.First();
    const char *str2_cstr = str2.First();

    strings += Memory::Move(str1);

    test.IsEqual(strings.Storage()[0].First(), str1_cstr, __LINE__);

    strings.Insert(Memory::Move(str2));
    test.IsEqual(strings.Storage()[1].First(), str2_cstr, __LINE__);

    strings += str1;
    test.IsNotEqual(strings.Storage()[2].First(), str1_cstr, __LINE__);

    strings.Insert(str2);
    test.IsNotEqual(strings.Storage()[3].First(), str2_cstr, __LINE__);

    // Checking move after expanding.
    for (SizeT i = 0; i < 16; i++) {
        strings += String<char>("");
    }

    test.IsEqual(strings.Storage()[0].First(), str1_cstr, __LINE__);
    test.IsEqual(strings.Storage()[1].First(), str2_cstr, __LINE__);

    ///////////////////////////
    numbers1.Clear();

    SizeT &nu1 = numbers1.Insert(5);
    ++nu1;

    test.IsEqual(*(numbers1.First()), SizeT{6}, __LINE__);

    /////////////////////
    numbers1.Clear();
    numbers1 += 1;

    for (SizeT &num : numbers1) {
        num += 10;
    }

    test.IsEqual(*(numbers1.First()), SizeT{11}, __LINE__);
    /////////////////////
    const Array<SizeT> numbers_init{8U, true};
    test.IsEqual(numbers_init.Size(), 8U, __LINE__);

    /////////////////////

    numbers1.Clear();
    numbers1 += 6;
    numbers1 += 7;
    numbers1 += 5;
    numbers1 += 1;
    numbers1 += 3;
    numbers1 += 4;
    numbers1 += 2;

    SizeT *storage = numbers1.Storage();
    numbers1.Swap(storage[3], storage[6]);
    numbers1.Swap(storage[0], storage[6]);
    numbers1.Swap(storage[3], storage[1]);
    numbers1.Swap(storage[2], storage[4]);
    numbers1.Swap(storage[3], storage[5]);
    numbers1.Swap(storage[6], storage[5]);

    test.IsEqual(storage[0], 1U, __LINE__);
    test.IsEqual(storage[1], 2U, __LINE__);
    test.IsEqual(storage[2], 3U, __LINE__);
    test.IsEqual(storage[3], 4U, __LINE__);
    test.IsEqual(storage[4], 5U, __LINE__);
    test.IsEqual(storage[5], 6U, __LINE__);
    test.IsEqual(storage[6], 7U, __LINE__);
}

static void TestArray4(QTest &test) {
    Array<String<char>> strings1;
    Array<String<char>> strings2;

    String<char> str1("val--------------------------------1");
    String<char> str2("val--------------------------------2");

    const char *str1_cstr = str1.First();
    const char *str2_cstr = str2.First();

    strings1 += Memory::Move(str1);
    strings1 += Memory::Move(str2);

    strings2 += strings1;
    String<char> *storage2 = strings2.Storage();

    test.IsEqual(strings2.Size(), 2U, __LINE__);
    test.IsNotNull(strings2.First(), __LINE__);
    test.IsNotEqual(storage2[0].First(), str1_cstr, __LINE__);
    test.IsNotEqual(storage2[1].First(), str2_cstr, __LINE__);

    strings2.Reserve(2);
    strings2 += Memory::Move(strings1);
    storage2 = strings2.Storage();

    test.IsEqual(strings2.Size(), 2U, __LINE__);
    test.IsNotNull(strings2.First(), __LINE__);
    test.IsEqual(storage2[0].First(), str1_cstr, __LINE__);
    test.IsEqual(storage2[1].First(), str2_cstr, __LINE__);

    strings2.Reset();

    strings2.Expect(0);
    test.IsEqual(strings2.Capacity(), 0U, __LINE__);

    strings2.Expect(1);
    test.IsEqual(strings2.Capacity(), 1U, __LINE__);

    strings2.Expect(2);
    test.IsEqual(strings2.Capacity(), 2U, __LINE__);

    strings2.Expect(3);
    test.IsEqual(strings2.Capacity(), 3U, __LINE__);

    strings2.Expect(4);
    test.IsEqual(strings2.Capacity(), 4U, __LINE__);

    strings2.Expect(5);
    test.IsEqual(strings2.Capacity(), 5U, __LINE__);

    strings2.Expect(10);
    test.IsEqual(strings2.Capacity(), 10U, __LINE__);

    strings2.Expect(11);
    test.IsEqual(strings2.Capacity(), 11U, __LINE__);
}

static void TestArraySort(QTest &test) {
    Array<SizeT32> numbers;

    numbers += 3;
    numbers += 0;
    numbers += 5;
    numbers += 1;
    numbers += 4;
    numbers += 2;

    numbers.Sort();

    SizeT32 *storage = numbers.Storage();

    test.IsEqual(storage[0], 0U, __LINE__);
    test.IsEqual(storage[1], 1U, __LINE__);
    test.IsEqual(storage[2], 2U, __LINE__);
    test.IsEqual(storage[3], 3U, __LINE__);
    test.IsEqual(storage[4], 4U, __LINE__);
    test.IsEqual(storage[5], 5U, __LINE__);

    numbers.Sort(false);
    storage = numbers.Storage();

    test.IsEqual(storage[0], 5U, __LINE__);
    test.IsEqual(storage[1], 4U, __LINE__);
    test.IsEqual(storage[2], 3U, __LINE__);
    test.IsEqual(storage[3], 2U, __LINE__);
    test.IsEqual(storage[4], 1U, __LINE__);
    test.IsEqual(storage[5], 0U, __LINE__);

    numbers.Reset();

    numbers += 3;
    numbers += 0;
    numbers += 5;
    numbers += 1;
    numbers += 4;
    numbers += 2;

    numbers.Sort(false);
    storage = numbers.Storage();

    test.IsEqual(storage[0], 5U, __LINE__);
    test.IsEqual(storage[1], 4U, __LINE__);
    test.IsEqual(storage[2], 3U, __LINE__);
    test.IsEqual(storage[3], 2U, __LINE__);
    test.IsEqual(storage[4], 1U, __LINE__);
    test.IsEqual(storage[5], 0U, __LINE__);

    numbers.Reset();

    numbers += 0;
    numbers += 1;
    numbers += 2;
    numbers += 3;
    numbers += 4;
    numbers += 5;

    numbers.Sort();
    storage = numbers.Storage();

    test.IsEqual(storage[0], 0U, __LINE__);
    test.IsEqual(storage[1], 1U, __LINE__);
    test.IsEqual(storage[2], 2U, __LINE__);
    test.IsEqual(storage[3], 3U, __LINE__);
    test.IsEqual(storage[4], 4U, __LINE__);
    test.IsEqual(storage[5], 5U, __LINE__);

    numbers.Sort(false);
    storage = numbers.Storage();

    test.IsEqual(storage[0], 5U, __LINE__);
    test.IsEqual(storage[1], 4U, __LINE__);
    test.IsEqual(storage[2], 3U, __LINE__);
    test.IsEqual(storage[3], 2U, __LINE__);
    test.IsEqual(storage[4], 1U, __LINE__);
    test.IsEqual(storage[5], 0U, __LINE__);

    Array<String<char>> strings;

    strings += String<char>("2017");
    strings += String<char>("2020");
    strings += String<char>("2019");
    strings += String<char>("2018");
    strings += String<char>("2021");
    strings += String<char>("2018");
    strings += String<char>("2021");

    strings.Sort();
    String<char> *storage_str = strings.Storage();

    test.IsEqual(storage_str[0], "2017", __LINE__);
    test.IsEqual(storage_str[1], "2018", __LINE__);
    test.IsEqual(storage_str[2], "2018", __LINE__);
    test.IsEqual(storage_str[3], "2019", __LINE__);
    test.IsEqual(storage_str[4], "2020", __LINE__);
    test.IsEqual(storage_str[5], "2021", __LINE__);
    test.IsEqual(storage_str[6], "2021", __LINE__);

    strings.Sort(false);
    storage_str = strings.Storage();

    test.IsEqual(storage_str[0], "2021", __LINE__);
    test.IsEqual(storage_str[1], "2021", __LINE__);
    test.IsEqual(storage_str[2], "2020", __LINE__);
    test.IsEqual(storage_str[3], "2019", __LINE__);
    test.IsEqual(storage_str[4], "2018", __LINE__);
    test.IsEqual(storage_str[5], "2018", __LINE__);
    test.IsEqual(storage_str[6], "2017", __LINE__);

    strings.Reset();

    strings += String<char>("2020");
    strings += String<char>("2017");
    strings += String<char>("2019");
    strings += String<char>("2018");
    strings += String<char>("2021");
    strings += String<char>("2021");
    strings += String<char>("2018");

    strings.Sort();
    storage_str = strings.Storage();

    test.IsEqual(storage_str[0], "2017", __LINE__);
    test.IsEqual(storage_str[1], "2018", __LINE__);
    test.IsEqual(storage_str[2], "2018", __LINE__);
    test.IsEqual(storage_str[3], "2019", __LINE__);
    test.IsEqual(storage_str[4], "2020", __LINE__);
    test.IsEqual(storage_str[5], "2021", __LINE__);
    test.IsEqual(storage_str[6], "2021", __LINE__);

    strings.Sort(false);
    storage_str = strings.Storage();

    test.IsEqual(storage_str[0], "2021", __LINE__);
    test.IsEqual(storage_str[1], "2021", __LINE__);
    test.IsEqual(storage_str[2], "2020", __LINE__);
    test.IsEqual(storage_str[3], "2019", __LINE__);
    test.IsEqual(storage_str[4], "2018", __LINE__);
    test.IsEqual(storage_str[5], "2018", __LINE__);
    test.IsEqual(storage_str[6], "2017", __LINE__);

    strings.Reset();

    strings += String<char>("2020");
    strings += String<char>("2017");
    strings += String<char>("2019");
    strings += String<char>("2018");
    strings += String<char>("2021");
    strings += String<char>("2021");
    strings += String<char>("2018");

    strings.Sort(false);
    storage_str = strings.Storage();

    test.IsEqual(storage_str[0], "2021", __LINE__);
    test.IsEqual(storage_str[1], "2021", __LINE__);
    test.IsEqual(storage_str[2], "2020", __LINE__);
    test.IsEqual(storage_str[3], "2019", __LINE__);
    test.IsEqual(storage_str[4], "2018", __LINE__);
    test.IsEqual(storage_str[5], "2018", __LINE__);
    test.IsEqual(storage_str[6], "2017", __LINE__);
}

static int RunArrayTests() {
    QTest test{"Array.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("Array Test 1", TestArray1);
    test.Test("Array Test 2", TestArray2);
    test.Test("Array Test 3", TestArray3);
    test.Test("Array Test 4", TestArray4);
    test.Test("Array Sort Test", TestArraySort);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
