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

#include "Array.hpp"
#include "String.hpp"
#include "TestHelper.hpp"

#ifndef QENTEM_ARRAY_TESTS_H_
#define QENTEM_ARRAY_TESTS_H_

namespace Qentem {
namespace Test {

static int TestArray1() {
    const SizeT *storage;
    Array<SizeT> numbers1;
    Array<SizeT> numbers2(8);

    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_TRUE(numbers1.IsEmpty(), "IsEmpty");
    EQ_FALSE(numbers1.IsNotEmpty(), "IsNotEmpty");
    EQ_VALUE(numbers1.Capacity(), 0, "Capacity");
    EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_TO(numbers1.Last(), nullptr, "Last()", "null");

    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_VALUE(numbers2.Capacity(), 8, "Capacity");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");

    numbers2.Reset();
    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_VALUE(numbers2.Capacity(), 0, "Capacity");
    EQ_TO(numbers2.First(), nullptr, "First()", "null");

    numbers1.Reserve(5);
    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 5, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");

    numbers1.Reserve(10);
    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 10, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");

    numbers1.Resize(18);
    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 18, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");

    storage = numbers1.First();
    numbers1.Resize(4);
    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 4, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    NOT_EQ_TO(numbers1.First(), storage, "First()", "storage");

    numbers2.Resize(5);
    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_VALUE(numbers2.Capacity(), 5, "Capacity");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");

    numbers2.Resize(2);
    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_VALUE(numbers2.Capacity(), 2, "Capacity");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");

    numbers2.Resize(0);
    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_VALUE(numbers2.Capacity(), 0, "Capacity");
    EQ_TO(numbers2.First(), nullptr, "First()", "null");

    numbers1.Reserve(0);
    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 0, "Capacity");
    EQ_TO(numbers1.First(), nullptr, "First()", "null");

    numbers1.Reset();
    numbers2.Reset();

    numbers1.ResizeAndInitialize(8);
    storage = numbers1.First();
    EQ_VALUE(numbers1.Size(), 8, "Size");
    EQ_FALSE(numbers1.IsEmpty(), "IsEmpty");
    EQ_TRUE(numbers1.IsNotEmpty(), "IsNotEmpty");
    EQ_VALUE(numbers1.Capacity(), 8, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");

    numbers1.ResizeAndInitialize(4);
    NOT_EQ_TO(numbers1.First(), storage, "First()", "storage");

    storage  = numbers1.First();
    numbers2 = numbers1;
    EQ_VALUE(numbers1.Size(), 4, "Size");
    EQ_VALUE(numbers1.Capacity(), 4, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers2.Size(), 4, "Size");
    EQ_VALUE(numbers2.Capacity(), 4, "Capacity");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");
    NOT_EQ_TO(numbers2.First(), storage, "First()", "storage");

    numbers1.ResizeAndInitialize(16);
    EQ_VALUE(numbers1.Size(), 16, "Size");
    EQ_VALUE(numbers1.Capacity(), 16, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");

    storage  = numbers1.First();
    numbers2 = numbers1;
    EQ_VALUE(numbers2.Size(), 16, "Size");
    EQ_VALUE(numbers2.Capacity(), 16, "Capacity");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");
    NOT_EQ_TO(numbers2.First(), storage, "First()", "storage");

    numbers2 = static_cast<Array<SizeT> &&>(numbers1);
    EQ_VALUE(numbers2.Size(), 16, "Size");
    EQ_VALUE(numbers2.Capacity(), 16, "Capacity");
    EQ_VALUE(numbers2.First(), storage, "First()");

    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 0, "Capacity");
    EQ_TO(numbers1.First(), nullptr, "First()", "null");

    storage  = numbers1.First();
    numbers1 = Array<SizeT>(numbers2);
    EQ_VALUE(numbers1.Size(), 16, "Size");
    EQ_VALUE(numbers1.Capacity(), 16, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    NOT_EQ_TO(numbers1.First(), storage, "First()", "storage");

    storage  = numbers2.First();
    numbers1 = Array<SizeT>(static_cast<Array<SizeT> &&>(numbers2));
    EQ_VALUE(numbers1.Size(), 16, "Size");
    EQ_VALUE(numbers1.Capacity(), 16, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers1.First(), storage, "First()");
    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_VALUE(numbers2.Capacity(), 0, "Capacity");
    EQ_TO(numbers2.First(), nullptr, "First()", "null");

    numbers1.Clear();
    EQ_VALUE(numbers1.Size(), 0, "Size");
    EQ_VALUE(numbers1.Capacity(), 16, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");

    numbers1.ResizeAndInitialize(3);
    numbers2.ResizeAndInitialize(5);
    storage = numbers2.First();

    numbers1 += numbers2;
    EQ_VALUE(numbers1.Size(), 8, "Size");
    EQ_VALUE(numbers1.Capacity(), 8, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    NOT_EQ_TO(numbers1.First(), storage, "First()", "storage");

    storage = numbers1.First();
    numbers1 += static_cast<Array<SizeT> &&>(numbers2);
    EQ_VALUE(numbers1.Size(), 13, "Size");
    EQ_VALUE(numbers1.Capacity(), 13, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    NOT_EQ_TO(numbers1.First(), storage, "First()", "storage");
    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_VALUE(numbers2.Capacity(), 0, "Capacity");
    EQ_TO(numbers2.First(), nullptr, "First()", "null");

    numbers2.ResizeAndInitialize(5);
    numbers1.Resize(18);
    storage = numbers1.First();
    numbers1 += static_cast<Array<SizeT> &&>(numbers2);
    EQ_VALUE(numbers1.Size(), 18, "Size");
    EQ_VALUE(numbers1.Capacity(), 18, "Capacity");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers1.First(), storage, "First()");

    storage = numbers1.First();
    numbers2 += static_cast<Array<SizeT> &&>(numbers1);
    EQ_VALUE(numbers2.Size(), 18, "Size");
    EQ_VALUE(numbers2.Capacity(), 18, "Capacity");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");
    EQ_VALUE(numbers2.First(), storage, "First()");

    SizeT *tmp = numbers2.Eject();
    EQ_VALUE(numbers2.Size(), 0, "Size");
    EQ_VALUE(numbers2.Capacity(), 0, "Capacity");
    EQ_TO(numbers2.First(), nullptr, "First()", "null");
    EQ_VALUE(tmp, storage, "First()");

    Memory::Deallocate(tmp);

    END_SUB_TEST;
}

static int TestArray2() {
    const SizeT *storage;
    Array<SizeT> numbers1;
    Array<SizeT> numbers2;

    numbers2.Reserve(4);
    numbers1.ResizeAndInitialize(4);
    storage = numbers2.First();
    numbers2.Insert(numbers1);
    EQ_VALUE(numbers2.Size(), 4, "Size");
    EQ_VALUE(numbers2.Capacity(), 4, "Capacity");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");
    EQ_TO(numbers2.First(), storage, "First()", "storage");
    EQ_TO(numbers2.Last(), (storage + 3), "Last()", "(storage + 3)");
    NOT_EQ_TO(numbers1.Last(), nullptr, "First()", "null");

    numbers1.Reserve(10);
    numbers2.ResizeAndInitialize(4);
    numbers2.Insert(numbers1);
    EQ_VALUE(numbers2.Size(), 4, "Size");
    EQ_VALUE(numbers2.Capacity(), 4, "Capacity");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");

    numbers1.ResizeAndInitialize(4);
    numbers2.Insert(numbers1);
    EQ_VALUE(numbers2.Size(), 8, "Size");
    EQ_VALUE(numbers2.Capacity(), 8, "Capacity");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");

    storage = numbers1.First();
    numbers2.Insert(static_cast<Array<SizeT> &&>(numbers1));
    EQ_VALUE(numbers2.Size(), 12, "Size");
    EQ_VALUE(numbers2.Capacity(), 12, "Capacity");
    NOT_EQ_TO(numbers2.First(), nullptr, "First()", "null");
    NOT_EQ_TO(numbers2.First(), storage, "First()", "storage");

    END_SUB_TEST;
}

static int TestArray3() {
    Array<SizeT>        numbers1;
    Array<SizeT>        numbers2(8);
    Array<String<char>> strings;

    for (SizeT i = 0; i < 8; i++) {
        numbers1 += i;
    }

    const SizeT *storage1 = numbers1.Storage();
    const SizeT *storage2 = numbers1.Storage();

    EQ_VALUE(numbers1.Size(), 8, "Size");
    EQ_TRUE((numbers1.Capacity() >= 8), "(numbers1.Capacity() >= 8)");
    NOT_EQ_TO(numbers1.First(), nullptr, "First()", "null");

    numbers2.Insert(storage1[0]).Insert(storage1[1]);

    for (SizeT i = 2; i < 8; i++) {
        numbers2.Insert(storage1[i]);
        EQ_VALUE(storage2[i], storage1[i], "storage2[i]");
    }

    EQ_VALUE(numbers2.Size(), 8, "Size");
    EQ_VALUE(numbers2.Capacity(), 8, "Capacity");

    numbers2.Resize(16);
    EQ_VALUE(numbers2.Size(), 8, "Size");
    EQ_VALUE(numbers2.Capacity(), 16, "Capacity");

    numbers2.Compress();
    EQ_VALUE(numbers2.Size(), 8, "Size");
    EQ_VALUE(numbers2.Capacity(), 8, "Capacity");

    // Checking move
    String<char> str1("val--------------------------------1");
    String<char> str2("val--------------------------------2");

    const char *str1_cstr = str1.First();
    const char *str2_cstr = str2.First();

    strings += static_cast<String<char> &&>(str1);
    String<char> *storage_str = strings.Storage();

    EQ_TO(storage_str[0].First(), str1_cstr, "storage_str[0].First()", "str1_cstr");

    strings.Insert(static_cast<String<char> &&>(str2));
    EQ_TO(storage_str[1].First(), str2_cstr, "storage_str[1].First()", "str2_cstr");

    strings += str1;
    NOT_EQ_TO(storage_str[2].First(), str1_cstr, "storage_str[2].First()", "str1_cstr");

    strings.Insert(str2);
    NOT_EQ_TO(storage_str[3].First(), str2_cstr, "storage_str[3].First()", "str2_cstr");

    // Checking move after expanding.
    for (SizeT i = 0; i < 16; i++) {
        strings += String<char>("");
    }

    storage_str = strings.Storage();
    EQ_TO(storage_str[0].First(), str1_cstr, "storage_str[0].First()", "str1_cstr");

    EQ_TO(storage_str[1].First(), str2_cstr, "storage_str[1].First()", "str2_cstr");

    END_SUB_TEST;
}

static int TestArray4() {
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

    EQ_VALUE(strings2.Size(), 2, "Size");
    NOT_EQ_TO(strings2.First(), nullptr, "First()", "null");
    NOT_EQ_TO(storage2[0].First(), str1_cstr, "storage2[0].First()", "str1_cstr");
    NOT_EQ_TO(storage2[1].First(), str2_cstr, "storage2[1].First()", "str2_cstr");

    strings2.Reserve(2);
    strings2 += static_cast<Array<String<char>> &&>(strings1);
    storage2 = strings2.Storage();

    EQ_VALUE(strings2.Size(), 2, "Size");
    NOT_EQ_TO(strings2.First(), nullptr, "First()", "null");
    EQ_TO(storage2[0].First(), str1_cstr, "storage2[0].First()", "str1_cstr");
    EQ_TO(storage2[1].First(), str2_cstr, "storage2[1].First()", "str2_cstr");

    END_SUB_TEST;
}

static int TestArraySort() {
    Array<int> numbers;

    numbers += 3;
    numbers += 0;
    numbers += 5;
    numbers += 1;
    numbers += 4;
    numbers += 2;

    numbers.Sort();

    int *storage = numbers.Storage();

    EQ_VALUE(storage[0], 0, "storage[0]");
    EQ_VALUE(storage[1], 1, "storage[1]");
    EQ_VALUE(storage[2], 2, "storage[2]");
    EQ_VALUE(storage[3], 3, "storage[3]");
    EQ_VALUE(storage[4], 4, "storage[4]");
    EQ_VALUE(storage[5], 5, "storage[5]");

    numbers.Sort(false);
    storage = numbers.Storage();

    EQ_VALUE(storage[0], 5, "storage[0]");
    EQ_VALUE(storage[1], 4, "storage[1]");
    EQ_VALUE(storage[2], 3, "storage[2]");
    EQ_VALUE(storage[3], 2, "storage[3]");
    EQ_VALUE(storage[4], 1, "storage[4]");
    EQ_VALUE(storage[5], 0, "storage[5]");

    numbers.Reset();

    numbers += 3;
    numbers += 0;
    numbers += 5;
    numbers += 1;
    numbers += 4;
    numbers += 2;

    numbers.Sort(false);
    storage = numbers.Storage();

    EQ_VALUE(storage[0], 5, "storage[0]");
    EQ_VALUE(storage[1], 4, "storage[1]");
    EQ_VALUE(storage[2], 3, "storage[2]");
    EQ_VALUE(storage[3], 2, "storage[3]");
    EQ_VALUE(storage[4], 1, "storage[4]");
    EQ_VALUE(storage[5], 0, "storage[5]");

    numbers.Reset();

    numbers += 0;
    numbers += 1;
    numbers += 2;
    numbers += 3;
    numbers += 4;
    numbers += 5;

    numbers.Sort();
    storage = numbers.Storage();

    EQ_VALUE(storage[0], 0, "storage[0]");
    EQ_VALUE(storage[1], 1, "storage[1]");
    EQ_VALUE(storage[2], 2, "storage[2]");
    EQ_VALUE(storage[3], 3, "storage[3]");
    EQ_VALUE(storage[4], 4, "storage[4]");
    EQ_VALUE(storage[5], 5, "storage[5]");

    numbers.Sort(false);
    storage = numbers.Storage();

    EQ_VALUE(storage[0], 5, "storage[0]");
    EQ_VALUE(storage[1], 4, "storage[1]");
    EQ_VALUE(storage[2], 3, "storage[2]");
    EQ_VALUE(storage[3], 2, "storage[3]");
    EQ_VALUE(storage[4], 1, "storage[4]");
    EQ_VALUE(storage[5], 0, "storage[5]");

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

    EQ_VALUE(storage_str[0], "2017", "storage_str[0]");
    EQ_VALUE(storage_str[1], "2018", "storage_str[1]");
    EQ_VALUE(storage_str[2], "2018", "storage_str[2]");
    EQ_VALUE(storage_str[3], "2019", "storage_str[3]");
    EQ_VALUE(storage_str[4], "2020", "storage_str[4]");
    EQ_VALUE(storage_str[5], "2021", "storage_str[5]");
    EQ_VALUE(storage_str[6], "2021", "storage_str[6]");

    strings.Sort(false);
    storage_str = strings.Storage();

    EQ_VALUE(storage_str[0], "2021", "storage_str[0]");
    EQ_VALUE(storage_str[1], "2021", "storage_str[1]");
    EQ_VALUE(storage_str[2], "2020", "storage_str[2]");
    EQ_VALUE(storage_str[3], "2019", "storage_str[3]");
    EQ_VALUE(storage_str[4], "2018", "storage_str[4]");
    EQ_VALUE(storage_str[5], "2018", "storage_str[5]");
    EQ_VALUE(storage_str[6], "2017", "storage_str[6]");

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

    EQ_VALUE(storage_str[0], "2017", "storage_str[0]");
    EQ_VALUE(storage_str[1], "2018", "storage_str[1]");
    EQ_VALUE(storage_str[2], "2018", "storage_str[2]");
    EQ_VALUE(storage_str[3], "2019", "storage_str[3]");
    EQ_VALUE(storage_str[4], "2020", "storage_str[4]");
    EQ_VALUE(storage_str[5], "2021", "storage_str[5]");
    EQ_VALUE(storage_str[6], "2021", "storage_str[6]");

    strings.Sort(false);
    storage_str = strings.Storage();

    EQ_VALUE(storage_str[0], "2021", "storage_str[0]");
    EQ_VALUE(storage_str[1], "2021", "storage_str[1]");
    EQ_VALUE(storage_str[2], "2020", "storage_str[2]");
    EQ_VALUE(storage_str[3], "2019", "storage_str[3]");
    EQ_VALUE(storage_str[4], "2018", "storage_str[4]");
    EQ_VALUE(storage_str[5], "2018", "storage_str[5]");
    EQ_VALUE(storage_str[6], "2017", "storage_str[6]");

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

    EQ_VALUE(storage_str[0], "2021", "storage_str[0]");
    EQ_VALUE(storage_str[1], "2021", "storage_str[1]");
    EQ_VALUE(storage_str[2], "2020", "storage_str[2]");
    EQ_VALUE(storage_str[3], "2019", "storage_str[3]");
    EQ_VALUE(storage_str[4], "2018", "storage_str[4]");
    EQ_VALUE(storage_str[5], "2018", "storage_str[5]");
    EQ_VALUE(storage_str[6], "2017", "storage_str[6]");

    END_SUB_TEST;
}

static int RunArrayTests() {
    STARTING_TEST("Array.hpp");

    START_TEST("Array Test 1", TestArray1);
    START_TEST("Array Test 2", TestArray2);
    START_TEST("Array Test 3", TestArray3);
    START_TEST("Array Test 4", TestArray4);
    START_TEST("Array Sort Test", TestArraySort);

    END_TEST("Array.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
