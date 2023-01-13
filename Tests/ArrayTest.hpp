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

#include "Array.hpp"
#include "String.hpp"

#ifndef QENTEM_ARRAY_TESTS_H_
#define QENTEM_ARRAY_TESTS_H_

namespace Qentem {
namespace Test {

static void TestArray1(TestHelper &helper) {
    const SizeT *storage;
    Array<SizeT> numbers1;
    Array<SizeT> numbers2(8);

    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.EqualsTrue(numbers1.IsEmpty(), "IsEmpty", __LINE__);
    helper.EqualsFalse(numbers1.IsNotEmpty(), "IsNotEmpty", __LINE__);
    helper.Equal(numbers1.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers1.Last(), nullptr, "Last()", "null", __LINE__);

    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 8U, "Capacity", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);

    numbers2.Reset();
    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers2.First(), nullptr, "First()", "null", __LINE__);

    numbers1.Reserve(5);
    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 5U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);

    numbers1.Reserve(10);
    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 10U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);

    numbers1.Resize(18);
    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 18U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);

    storage = numbers1.First();
    numbers1.Resize(4);
    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 4U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(numbers1.First(), storage, "First()", "storage", __LINE__);

    numbers2.Resize(5);
    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 5U, "Capacity", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);

    numbers2.Resize(2);
    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 2U, "Capacity", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);

    numbers2.Resize(0);
    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers2.First(), nullptr, "First()", "null", __LINE__);

    numbers1.Reserve(0);
    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers1.First(), nullptr, "First()", "null", __LINE__);

    numbers1.Reset();
    numbers2.Reset();

    numbers1.ResizeAndInitialize(8);
    storage = numbers1.First();
    helper.Equal(numbers1.Size(), 8U, "Size", __LINE__);
    helper.EqualsFalse(numbers1.IsEmpty(), "IsEmpty", __LINE__);
    helper.EqualsTrue(numbers1.IsNotEmpty(), "IsNotEmpty", __LINE__);
    helper.Equal(numbers1.Capacity(), 8U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);

    numbers1.ResizeAndInitialize(4);
    helper.NotEqual(numbers1.First(), storage, "First()", "storage", __LINE__);

    storage  = numbers1.First();
    numbers2 = numbers1;
    helper.Equal(numbers1.Size(), 4U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 4U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers2.Size(), 4U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 4U, "Capacity", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(numbers2.First(), storage, "First()", "storage", __LINE__);

    numbers1.ResizeAndInitialize(16);
    helper.Equal(numbers1.Size(), 16U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 16U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);

    storage  = numbers1.First();
    numbers2 = numbers1;
    helper.Equal(numbers2.Size(), 16U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 16U, "Capacity", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(numbers2.First(), storage, "First()", "storage", __LINE__);

    numbers2 = static_cast<Array<SizeT> &&>(numbers1);
    helper.Equal(numbers2.Size(), 16U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 16U, "Capacity", __LINE__);
    helper.Equal(numbers2.First(), storage, "First()", __LINE__);

    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers1.First(), nullptr, "First()", "null", __LINE__);

    storage  = numbers1.First();
    numbers1 = Array<SizeT>(numbers2);
    helper.Equal(numbers1.Size(), 16U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 16U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(numbers1.First(), storage, "First()", "storage", __LINE__);

    storage  = numbers2.First();
    numbers1 = Array<SizeT>(static_cast<Array<SizeT> &&>(numbers2));
    helper.Equal(numbers1.Size(), 16U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 16U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers1.First(), storage, "First()", __LINE__);
    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers2.First(), nullptr, "First()", "null", __LINE__);

    numbers1.Clear();
    helper.Equal(numbers1.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 16U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);

    numbers1.ResizeAndInitialize(3);
    numbers2.ResizeAndInitialize(5);
    storage = numbers2.First();

    numbers1 += numbers2;
    helper.Equal(numbers1.Size(), 8U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 8U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(numbers1.First(), storage, "First()", "storage", __LINE__);

    storage = numbers1.First();
    numbers1 += static_cast<Array<SizeT> &&>(numbers2);
    helper.Equal(numbers1.Size(), 13U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 13U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(numbers1.First(), storage, "First()", "storage", __LINE__);
    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers2.First(), nullptr, "First()", "null", __LINE__);

    numbers2.ResizeAndInitialize(5);
    numbers1.Resize(18);
    storage = numbers1.First();
    numbers1 += static_cast<Array<SizeT> &&>(numbers2);
    helper.Equal(numbers1.Size(), 18U, "Size", __LINE__);
    helper.Equal(numbers1.Capacity(), 18U, "Capacity", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers1.First(), storage, "First()", __LINE__);

    storage = numbers1.First();
    numbers2 += static_cast<Array<SizeT> &&>(numbers1);
    helper.Equal(numbers2.Size(), 18U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 18U, "Capacity", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers2.First(), storage, "First()", __LINE__);

    SizeT *tmp = numbers2.Detach();
    helper.Equal(numbers2.Size(), 0U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 0U, "Capacity", __LINE__);
    helper.Equal(numbers2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(tmp, storage, "First()", __LINE__);

    Memory::Deallocate(tmp);
}

static void TestArray2(TestHelper &helper) {
    const SizeT *storage;
    Array<SizeT> numbers1;
    Array<SizeT> numbers2;

    numbers2.Reserve(4);
    numbers1.ResizeAndInitialize(4);
    storage = numbers2.First();
    numbers2.Insert(numbers1);
    helper.Equal(numbers2.Size(), 4U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 4U, "Capacity", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(numbers2.First(), storage, "First()", "storage", __LINE__);
    helper.Equal(numbers2.Last(), (storage + 3), "Last()", "(storage + 3)", __LINE__);
    helper.NotEqual(numbers1.Last(), nullptr, "First()", "null", __LINE__);

    numbers1.Reserve(10);
    numbers2.ResizeAndInitialize(4);
    numbers2.Insert(numbers1);
    helper.Equal(numbers2.Size(), 4U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 4U, "Capacity", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);

    numbers1.ResizeAndInitialize(4);
    numbers2.Insert(numbers1);
    helper.Equal(numbers2.Size(), 8U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 8U, "Capacity", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);

    storage = numbers1.First();
    numbers2.Insert(static_cast<Array<SizeT> &&>(numbers1));
    helper.Equal(numbers2.Size(), 12U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 12U, "Capacity", __LINE__);
    helper.NotEqual(numbers2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(numbers2.First(), storage, "First()", "storage", __LINE__);
}

static void TestArray3(TestHelper &helper) {
    Array<SizeT>        numbers1;
    Array<SizeT>        numbers2(8);
    Array<String<char>> strings;

    for (SizeT i = 0; i < 8; i++) {
        numbers1 += i;
    }

    const SizeT *storage1 = numbers1.Storage();
    const SizeT *storage2 = numbers1.Storage();

    helper.Equal(numbers1.Size(), 8U, "Size", __LINE__);
    helper.EqualsTrue((numbers1.Capacity() >= 8), "(numbers1.Capacity() >= 8)", __LINE__);
    helper.NotEqual(numbers1.First(), nullptr, "First()", "null", __LINE__);

    SizeT sum = 0;
    for (const SizeT &num : numbers1) {
        sum += num;
    }

    helper.Equal(sum, 28U, "sum", __LINE__);

    numbers2.Insert(storage1[0]);
    numbers2.Insert(storage1[1]);

    for (SizeT i = 2; i < 8; i++) {
        numbers2.Insert(storage1[i]);
        helper.Equal(storage2[i], storage1[i], "storage2[i]", __LINE__);
    }

    helper.Equal(numbers2.Size(), 8U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 8U, "Capacity", __LINE__);

    numbers2.Resize(16);
    helper.Equal(numbers2.Size(), 8U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 16U, "Capacity", __LINE__);

    numbers2.Compress();
    helper.Equal(numbers2.Size(), 8U, "Size", __LINE__);
    helper.Equal(numbers2.Capacity(), 8U, "Capacity", __LINE__);

    // Checking move
    String<char> str1("val--------------------------------1");
    String<char> str2("val--------------------------------2");

    const char *str1_cstr = str1.First();
    const char *str2_cstr = str2.First();

    strings += static_cast<String<char> &&>(str1);

    helper.Equal(strings.Storage()[0].First(), str1_cstr, "storage_str[0].First()", "str1_cstr", __LINE__);

    strings.Insert(static_cast<String<char> &&>(str2));
    helper.Equal(strings.Storage()[1].First(), str2_cstr, "storage_str[1].First()", "str2_cstr", __LINE__);

    strings += str1;
    helper.NotEqual(strings.Storage()[2].First(), str1_cstr, "storage_str[2].First()", "str1_cstr", __LINE__);

    strings.Insert(str2);
    helper.NotEqual(strings.Storage()[3].First(), str2_cstr, "storage_str[3].First()", "str2_cstr", __LINE__);

    // Checking move after expanding.
    for (SizeT i = 0; i < 16; i++) {
        strings += String<char>("");
    }

    helper.Equal(strings.Storage()[0].First(), str1_cstr, "storage_str[0].First()", "str1_cstr", __LINE__);
    helper.Equal(strings.Storage()[1].First(), str2_cstr, "storage_str[1].First()", "str2_cstr", __LINE__);
}

static void TestArray4(TestHelper &helper) {
    Array<String<char>> strings1;
    Array<String<char>> strings2;

    String<char> str1("val--------------------------------1");
    String<char> str2("val--------------------------------2");

    const char *str1_cstr = str1.First();
    const char *str2_cstr = str2.First();

    strings1 += static_cast<String<char> &&>(str1);
    strings1 += static_cast<String<char> &&>(str2);

    strings2 += strings1;
    String<char> *storage2 = strings2.Storage();

    helper.Equal(strings2.Size(), 2U, "Size", __LINE__);
    helper.NotEqual(strings2.First(), nullptr, "First()", "null", __LINE__);
    helper.NotEqual(storage2[0].First(), str1_cstr, "storage2[0].First()", "str1_cstr", __LINE__);
    helper.NotEqual(storage2[1].First(), str2_cstr, "storage2[1].First()", "str2_cstr", __LINE__);

    strings2.Reserve(2);
    strings2 += static_cast<Array<String<char>> &&>(strings1);
    storage2 = strings2.Storage();

    helper.Equal(strings2.Size(), 2U, "Size", __LINE__);
    helper.NotEqual(strings2.First(), nullptr, "First()", "null", __LINE__);
    helper.Equal(storage2[0].First(), str1_cstr, "storage2[0].First()", "str1_cstr", __LINE__);
    helper.Equal(storage2[1].First(), str2_cstr, "storage2[1].First()", "str2_cstr", __LINE__);
}

static void TestArraySort(TestHelper &helper) {
    Array<unsigned int> numbers;

    numbers += 3;
    numbers += 0;
    numbers += 5;
    numbers += 1;
    numbers += 4;
    numbers += 2;

    numbers.Sort();

    unsigned int *storage = numbers.Storage();

    helper.Equal(storage[0], 0U, "storage[0]", __LINE__);
    helper.Equal(storage[1], 1U, "storage[1]", __LINE__);
    helper.Equal(storage[2], 2U, "storage[2]", __LINE__);
    helper.Equal(storage[3], 3U, "storage[3]", __LINE__);
    helper.Equal(storage[4], 4U, "storage[4]", __LINE__);
    helper.Equal(storage[5], 5U, "storage[5]", __LINE__);

    numbers.Sort(false);
    storage = numbers.Storage();

    helper.Equal(storage[0], 5U, "storage[0]", __LINE__);
    helper.Equal(storage[1], 4U, "storage[1]", __LINE__);
    helper.Equal(storage[2], 3U, "storage[2]", __LINE__);
    helper.Equal(storage[3], 2U, "storage[3]", __LINE__);
    helper.Equal(storage[4], 1U, "storage[4]", __LINE__);
    helper.Equal(storage[5], 0U, "storage[5]", __LINE__);

    numbers.Reset();

    numbers += 3;
    numbers += 0;
    numbers += 5;
    numbers += 1;
    numbers += 4;
    numbers += 2;

    numbers.Sort(false);
    storage = numbers.Storage();

    helper.Equal(storage[0], 5U, "storage[0]", __LINE__);
    helper.Equal(storage[1], 4U, "storage[1]", __LINE__);
    helper.Equal(storage[2], 3U, "storage[2]", __LINE__);
    helper.Equal(storage[3], 2U, "storage[3]", __LINE__);
    helper.Equal(storage[4], 1U, "storage[4]", __LINE__);
    helper.Equal(storage[5], 0U, "storage[5]", __LINE__);

    numbers.Reset();

    numbers += 0;
    numbers += 1;
    numbers += 2;
    numbers += 3;
    numbers += 4;
    numbers += 5;

    numbers.Sort();
    storage = numbers.Storage();

    helper.Equal(storage[0], 0U, "storage[0]", __LINE__);
    helper.Equal(storage[1], 1U, "storage[1]", __LINE__);
    helper.Equal(storage[2], 2U, "storage[2]", __LINE__);
    helper.Equal(storage[3], 3U, "storage[3]", __LINE__);
    helper.Equal(storage[4], 4U, "storage[4]", __LINE__);
    helper.Equal(storage[5], 5U, "storage[5]", __LINE__);

    numbers.Sort(false);
    storage = numbers.Storage();

    helper.Equal(storage[0], 5U, "storage[0]", __LINE__);
    helper.Equal(storage[1], 4U, "storage[1]", __LINE__);
    helper.Equal(storage[2], 3U, "storage[2]", __LINE__);
    helper.Equal(storage[3], 2U, "storage[3]", __LINE__);
    helper.Equal(storage[4], 1U, "storage[4]", __LINE__);
    helper.Equal(storage[5], 0U, "storage[5]", __LINE__);

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

    helper.Equal(storage_str[0], "2017", "storage_str[0]", __LINE__);
    helper.Equal(storage_str[1], "2018", "storage_str[1]", __LINE__);
    helper.Equal(storage_str[2], "2018", "storage_str[2]", __LINE__);
    helper.Equal(storage_str[3], "2019", "storage_str[3]", __LINE__);
    helper.Equal(storage_str[4], "2020", "storage_str[4]", __LINE__);
    helper.Equal(storage_str[5], "2021", "storage_str[5]", __LINE__);
    helper.Equal(storage_str[6], "2021", "storage_str[6]", __LINE__);

    strings.Sort(false);
    storage_str = strings.Storage();

    helper.Equal(storage_str[0], "2021", "storage_str[0]", __LINE__);
    helper.Equal(storage_str[1], "2021", "storage_str[1]", __LINE__);
    helper.Equal(storage_str[2], "2020", "storage_str[2]", __LINE__);
    helper.Equal(storage_str[3], "2019", "storage_str[3]", __LINE__);
    helper.Equal(storage_str[4], "2018", "storage_str[4]", __LINE__);
    helper.Equal(storage_str[5], "2018", "storage_str[5]", __LINE__);
    helper.Equal(storage_str[6], "2017", "storage_str[6]", __LINE__);

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

    helper.Equal(storage_str[0], "2017", "storage_str[0]", __LINE__);
    helper.Equal(storage_str[1], "2018", "storage_str[1]", __LINE__);
    helper.Equal(storage_str[2], "2018", "storage_str[2]", __LINE__);
    helper.Equal(storage_str[3], "2019", "storage_str[3]", __LINE__);
    helper.Equal(storage_str[4], "2020", "storage_str[4]", __LINE__);
    helper.Equal(storage_str[5], "2021", "storage_str[5]", __LINE__);
    helper.Equal(storage_str[6], "2021", "storage_str[6]", __LINE__);

    strings.Sort(false);
    storage_str = strings.Storage();

    helper.Equal(storage_str[0], "2021", "storage_str[0]", __LINE__);
    helper.Equal(storage_str[1], "2021", "storage_str[1]", __LINE__);
    helper.Equal(storage_str[2], "2020", "storage_str[2]", __LINE__);
    helper.Equal(storage_str[3], "2019", "storage_str[3]", __LINE__);
    helper.Equal(storage_str[4], "2018", "storage_str[4]", __LINE__);
    helper.Equal(storage_str[5], "2018", "storage_str[5]", __LINE__);
    helper.Equal(storage_str[6], "2017", "storage_str[6]", __LINE__);

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

    helper.Equal(storage_str[0], "2021", "storage_str[0]", __LINE__);
    helper.Equal(storage_str[1], "2021", "storage_str[1]", __LINE__);
    helper.Equal(storage_str[2], "2020", "storage_str[2]", __LINE__);
    helper.Equal(storage_str[3], "2019", "storage_str[3]", __LINE__);
    helper.Equal(storage_str[4], "2018", "storage_str[4]", __LINE__);
    helper.Equal(storage_str[5], "2018", "storage_str[5]", __LINE__);
    helper.Equal(storage_str[6], "2017", "storage_str[6]", __LINE__);
}

static int RunArrayTests() {
    TestHelper helper{"Array.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("Array Test 1", TestArray1);
    helper.Test("Array Test 2", TestArray2);
    helper.Test("Array Test 3", TestArray3);
    helper.Test("Array Test 4", TestArray4);
    helper.Test("Array Sort Test", TestArraySort);

    return helper.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
