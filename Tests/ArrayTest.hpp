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

#ifndef QENTEM_ARRAYTEST_H_
#define QENTEM_ARRAYTEST_H_

namespace Qentem {
namespace Test {

static int TestArray1() {
    const UInt *storage;
    Array<UInt> numbers1;
    Array<UInt> numbers2(8);

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
    storage = numbers1.Storage();
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

    storage = numbers1.Storage();
    numbers1.Resize(4);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 4, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(numbers1.Storage(), storage, "Storage()", "storage");

    numbers2.Resize(5);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 5, "Capacity");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");

    numbers2.Resize(2);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 2, "Capacity");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");

    numbers2.Resize(0);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");

    numbers1.SetCapacity(0);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    numbers1.Clear();
    numbers2.Clear();

    numbers1.ResizeAndInitialize(8);
    storage = numbers1.Storage();
    SHOULD_EQUAL_VALUE(numbers1.Size(), 8, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 8, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    numbers1.ResizeAndInitialize(4);
    SHOULD_NOT_EQUAL(numbers1.Storage(), storage, "Storage()", "storage");

    storage  = numbers1.Storage();
    numbers2 = numbers1;
    SHOULD_EQUAL_VALUE(numbers1.Size(), 4, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 4, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers2.Size(), 4, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 4, "Capacity");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(numbers2.Storage(), storage, "Storage()", "storage");

    numbers1.ResizeAndInitialize(16);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 16, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 16, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    storage  = numbers1.Storage();
    numbers2 = numbers1;
    SHOULD_EQUAL_VALUE(numbers2.Size(), 16, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 16, "Capacity");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(numbers2.Storage(), storage, "Storage()", "storage");

    numbers2 = static_cast<Array<UInt> &&>(numbers1);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 16, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 16, "Capacity");
    SHOULD_EQUAL_VALUE(numbers2.Storage(), storage, "Storage()");

    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    storage  = numbers1.Storage();
    numbers1 = Array<UInt>(numbers2);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 16, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 16, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(numbers1.Storage(), storage, "Storage()", "storage");

    storage  = numbers2.Storage();
    numbers1 = Array<UInt>(static_cast<Array<UInt> &&>(numbers2));
    SHOULD_EQUAL_VALUE(numbers1.Size(), 16, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 16, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers1.Storage(), storage, "Storage()");
    SHOULD_EQUAL_VALUE(numbers2.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");

    numbers1.SoftClear();
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 16, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    numbers1.ResizeAndInitialize(3);
    numbers2.ResizeAndInitialize(5);
    storage = numbers2.Storage();

    numbers1 += numbers2;
    SHOULD_EQUAL_VALUE(numbers1.Size(), 8, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 8, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(numbers1.Storage(), storage, "Storage()", "storage");

    storage = numbers1.Storage();
    numbers1 += static_cast<Array<UInt> &&>(numbers2);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 13, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 13, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(numbers1.Storage(), storage, "Storage()", "storage");
    SHOULD_EQUAL_VALUE(numbers2.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");

    numbers2.ResizeAndInitialize(5);
    numbers1.Resize(18);
    storage = numbers1.Storage();
    numbers1 += static_cast<Array<UInt> &&>(numbers2);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 18, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 18, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers1.Storage(), storage, "Storage()");

    storage = numbers1.Storage();
    numbers2 += static_cast<Array<UInt> &&>(numbers1);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 18, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 18, "Capacity");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(numbers2.Storage(), storage, "Storage()");

    UInt *tmp = numbers2.Eject();
    SHOULD_EQUAL_VALUE(numbers2.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL_VALUE(tmp, storage, "Storage()");

    delete tmp;

    END_SUB_TEST;
}

static int TestArray2() {
    const UInt *storage;
    Array<UInt> numbers1;
    Array<UInt> numbers2;

    numbers2.SetCapacity(4);
    numbers1.ResizeAndInitialize(4);
    storage = numbers2.Storage();
    numbers2.Add(numbers1);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 4, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 4, "Capacity");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL(numbers2.Storage(), storage, "Storage()", "storage");

    numbers1.SetCapacity(10);
    numbers2.ResizeAndInitialize(4);
    numbers2.Add(numbers1);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 4, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 4, "Capacity");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");

    numbers1.ResizeAndInitialize(4);
    numbers2.Add(numbers1);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 8, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 8, "Capacity");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");

    storage = numbers1.Storage();
    numbers2.Add(static_cast<Array<UInt> &&>(numbers1));
    SHOULD_EQUAL_VALUE(numbers2.Size(), 12, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 12, "Capacity");
    SHOULD_NOT_EQUAL(numbers2.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(numbers2.Storage(), storage, "Storage()", "storage");

    numbers1.SetCapacity(16);

    numbers1.SoftResize(5);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 5, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 16, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    numbers1.SoftResize(0);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 16, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    numbers1.SoftResize(16);
    SHOULD_EQUAL_VALUE(numbers1.Size(), 16, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 16, "Capacity");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    numbers1.SoftResize(0);
    numbers1.Compress();
    SHOULD_EQUAL_VALUE(numbers1.Size(), 0, "Size");
    SHOULD_EQUAL_VALUE(numbers1.Capacity(), 0, "Capacity");
    SHOULD_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    bool did_throw;

    try {
        did_throw = false;
        numbers1.SoftResize(160);
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    END_SUB_TEST;
}

static int TestArray3() {
    Array<UInt>   numbers1;
    Array<UInt>   numbers2(8);
    Array<String> strings;

    for (UInt i = 0; i < 8; i++) {
        numbers1 += i;
    }

    SHOULD_EQUAL_VALUE(numbers1.Size(), 8, "Size");
    SHOULD_EQUAL_TRUE((numbers1.Capacity() >= 8), "(numbers1.Capacity() >= 8)");
    SHOULD_NOT_EQUAL(numbers1.Storage(), nullptr, "Storage()", "null");

    bool did_throw;

    try {
        did_throw = false;
        numbers2[0];
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    try {
        did_throw = false;
        numbers2[7];
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    try {
        did_throw = false;
        numbers1[8];
    } catch (...) {
        did_throw = true;
    }

    SHOULD_EQUAL_TRUE(did_throw, "did_throw");

    numbers2.Add(numbers1[0]).Add(numbers1[1]);
    for (UInt i = 2; i < 8; i++) {
        numbers2.Add(numbers1[i]);
        SHOULD_EQUAL_VALUE(numbers2[i], numbers1[i], "numbers2[i]");
    }

    SHOULD_EQUAL_VALUE(numbers2.Size(), 8, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 8, "Capacity");

    numbers2.Resize(16);
    SHOULD_EQUAL_VALUE(numbers2.Size(), 8, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 16, "Capacity");

    numbers2.Compress();
    SHOULD_EQUAL_VALUE(numbers2.Size(), 8, "Size");
    SHOULD_EQUAL_VALUE(numbers2.Capacity(), 8, "Capacity");

    // Checking move
    String str1("val1");
    String str2("val2");

    const char *str1_cstr = str1.Char();
    const char *str2_cstr = str2.Char();

    strings += static_cast<String &&>(str1);
    SHOULD_EQUAL(strings[0].Char(), str1_cstr, "strings[0].Char()",
                 "str1_cstr");

    strings.Add(static_cast<String &&>(str2));
    SHOULD_EQUAL(strings[1].Char(), str2_cstr, "strings[1].Char()",
                 "str2_cstr");

    strings += str1;
    SHOULD_NOT_EQUAL(strings[2].Char(), str1_cstr, "strings[2].Char()",
                     "str1_cstr");

    strings.Add(str2);
    SHOULD_NOT_EQUAL(strings[3].Char(), str2_cstr, "strings[3].Char()",
                     "str2_cstr");

    // Checking move after expanding.
    for (UInt i = 0; i < 16; i++) {
        strings += String("");
    }

    SHOULD_EQUAL(strings[0].Char(), str1_cstr, "strings[0].Char()",
                 "str1_cstr");

    SHOULD_EQUAL(strings[1].Char(), str2_cstr, "strings[1].Char()",
                 "str2_cstr");

    END_SUB_TEST;
}

static int TestArray4() {
    Array<String> strings1;
    Array<String> strings2;

    String str1("val1");
    String str2("val2");

    const char *str1_cstr = str1.Char();
    const char *str2_cstr = str2.Char();

    strings1 += static_cast<String &&>(str1);
    strings1 += static_cast<String &&>(str2);

    strings2 += strings1;
    SHOULD_EQUAL_VALUE(strings2.Size(), 2, "Size");
    SHOULD_NOT_EQUAL(strings2.Storage(), nullptr, "Storage()", "null");
    SHOULD_NOT_EQUAL(strings2[0].Char(), str1_cstr, "strings[0].Char()",
                     "str1_cstr");
    SHOULD_NOT_EQUAL(strings2[1].Char(), str2_cstr, "strings[1].Char()",
                     "str2_cstr");

    strings2.SetCapacity(2);
    strings2 += static_cast<Array<String> &&>(strings1);
    SHOULD_EQUAL_VALUE(strings2.Size(), 2, "Size");
    SHOULD_NOT_EQUAL(strings2.Storage(), nullptr, "Storage()", "null");
    SHOULD_EQUAL(strings2[0].Char(), str1_cstr, "strings[0].Char()",
                 "str1_cstr");
    SHOULD_EQUAL(strings2[1].Char(), str2_cstr, "strings[1].Char()",
                 "str2_cstr");

    END_SUB_TEST;
}

static int RunArrayTests() {
    STARTING_TEST("Array.hpp");

    START_TEST("Array Test 1", TestArray1);
    START_TEST("Array Test 2", TestArray2);
    START_TEST("Array Test 3", TestArray3);
    START_TEST("Array Test 4", TestArray4);

    END_TEST("Array.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
