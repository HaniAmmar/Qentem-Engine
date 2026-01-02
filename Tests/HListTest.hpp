/*
 * Copyright (c) 2025 Hani Ammar
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

#ifndef QENTEM_HLIST_TESTS_H
#define QENTEM_HLIST_TESTS_H

#include "Qentem/QTest.hpp"
#include "Qentem/HList.hpp"
#include "Qentem/String.hpp"
#include "Qentem/Digit.hpp"

namespace Qentem {
namespace Test {

static void TestHList1(QTest &test) {
    HList<String<char>> numbers1;
    HList<String<char>> numbers2;

    for (SizeT i = 1; i <= 10; i++) {
        String<char> key{"k-", 2};

        Digit::NumberToString(key, i);
        numbers1.Insert(key);
        test.IsTrue(numbers1.Has(key), __LINE__);
    }

    numbers1.Reset();

    for (SizeT i = 1; i <= 120; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        numbers1.Insert(key);
        test.IsTrue(numbers1.Has(key), __LINE__);
    }

    numbers2 = QUtility::Move(numbers1);

    for (SizeT i = 1; i < 10; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        test.IsTrue(numbers2.Has(key), __LINE__);
    }

    numbers1 = QUtility::Move(numbers2);

    HList<String<char>> numbers3{numbers1};

    for (SizeT i = 1; i <= 120; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        test.IsTrue(numbers3.Has(key), __LINE__);
    }

    for (SizeT i = 1; i <= 10; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        numbers1.Remove(key);
    }

    for (SizeT i = 1; i <= 10; i++) {
        String<char> key;

        Digit::NumberToString(key, i);
        test.IsFalse(numbers1.Has(key), __LINE__);
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
        test.IsFalse(numbers1.Has(key), __LINE__);
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
        test.IsFalse(numbers1.Has(key), __LINE__);
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
        test.IsFalse(numbers1.Has(key), __LINE__);
    }

    numbers1.Compress();
    test.IsEqual(numbers1.Size(), 0U, __LINE__);

    numbers1.Reserve(16);
    numbers1["a"];
    numbers1.Compress();
    test.IsEqual(numbers1.Size(), 1U, __LINE__);
    test.IsEqual(numbers1.Capacity(), 16U, __LINE__);
}

static void TestHListNumeric(QTest &test) {
    HList<unsigned> numbers(8);

    // Test insertions and lookups
    numbers[0U];
    numbers[10U];
    numbers[20U];
    numbers[30U];

    test.IsEqual(numbers.Size(), SizeT{4}, __LINE__);
    test.IsTrue(numbers.Capacity() >= SizeT{4}, __LINE__);
    test.IsTrue(numbers.Has(0U), __LINE__);
    test.IsTrue(numbers.Has(10U), __LINE__);
    test.IsTrue(numbers.Has(20U), __LINE__);
    test.IsTrue(numbers.Has(30U), __LINE__);

    // Test updating values
    numbers[10U];
    test.IsEqual(numbers.Size(), SizeT{4}, __LINE__);
    test.IsTrue(numbers.Has(10U), __LINE__);

    // Test resizing/expanding
    numbers[50U];
    numbers.Insert(60U);
    numbers[70U];
    numbers.Insert(80U);
    numbers[90U];

    test.IsEqual(numbers.Size(), SizeT{9}, __LINE__);
    test.IsTrue(numbers.Capacity() >= SizeT{9}, __LINE__);
    test.IsTrue(numbers.Has(0U), __LINE__);
    test.IsTrue(numbers.Has(10U), __LINE__);
    test.IsTrue(numbers.Has(20U), __LINE__);
    test.IsTrue(numbers.Has(30U), __LINE__);
    test.IsTrue(numbers.Has(50U), __LINE__);
    test.IsTrue(numbers.Has(60U), __LINE__);
    test.IsTrue(numbers.Has(70U), __LINE__);
    test.IsTrue(numbers.Has(80U), __LINE__);
    test.IsTrue(numbers.Has(90U), __LINE__);

    numbers.Remove(70U);
    test.IsFalse(numbers.Has(70U), __LINE__);

    test.IsEqual(numbers.Size(), SizeT{9}, __LINE__);

    numbers.Reorder();

    test.IsEqual(numbers.Size(), SizeT{8}, __LINE__);
    test.IsEqual(numbers.Capacity(), SizeT{16}, __LINE__);
    test.IsTrue(numbers.Has(0U), __LINE__);
    test.IsTrue(numbers.Has(10U), __LINE__);
    test.IsTrue(numbers.Has(20U), __LINE__);
    test.IsTrue(numbers.Has(30U), __LINE__);
    test.IsTrue(numbers.Has(50U), __LINE__);
    test.IsTrue(numbers.Has(60U), __LINE__);
    test.IsTrue(numbers.Has(80U), __LINE__);
    test.IsTrue(numbers.Has(90U), __LINE__);

    numbers.Remove(0U);
    test.IsEqual(numbers.Size(), SizeT{8}, __LINE__);
    test.IsEqual(numbers.Capacity(), SizeT{16}, __LINE__);
    test.IsFalse(numbers.Has(0U), __LINE__);

    numbers.Reorder();

    test.IsEqual(numbers.Size(), SizeT{7}, __LINE__);
    test.IsEqual(numbers.Capacity(), SizeT{16}, __LINE__);
    test.IsTrue(numbers.Has(10U), __LINE__);
    test.IsTrue(numbers.Has(20U), __LINE__);
    test.IsTrue(numbers.Has(30U), __LINE__);
    test.IsTrue(numbers.Has(50U), __LINE__);
    test.IsTrue(numbers.Has(60U), __LINE__);
    test.IsTrue(numbers.Has(80U), __LINE__);
    test.IsTrue(numbers.Has(90U), __LINE__);

    numbers.Remove(10U);
    numbers.Remove(60U);
    numbers.Remove(90U);

    numbers.Reorder();

    test.IsEqual(numbers.Size(), SizeT{4}, __LINE__);
    test.IsEqual(numbers.Capacity(), SizeT{16}, __LINE__);
    test.IsTrue(numbers.Has(20U), __LINE__);
    test.IsTrue(numbers.Has(30U), __LINE__);
    test.IsTrue(numbers.Has(50U), __LINE__);
    test.IsTrue(numbers.Has(80U), __LINE__);

    numbers.Remove(80U);
    test.IsFalse(numbers.Has(80U), __LINE__);

    numbers.Reorder();
    test.IsTrue(numbers.Has(20U), __LINE__);
    test.IsTrue(numbers.Has(30U), __LINE__);
    test.IsTrue(numbers.Has(50U), __LINE__);

    numbers.Remove(20U);
    test.IsTrue(numbers.Has(30U), __LINE__);
    test.IsTrue(numbers.Has(50U), __LINE__);

    numbers.Reorder();

    test.IsEqual(numbers.Size(), SizeT{2}, __LINE__);
    test.IsEqual(numbers.Capacity(), SizeT{16}, __LINE__);

    numbers.Remove(30U);
    numbers.Remove(50U);

    numbers.Reorder();

    test.IsEqual(numbers.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers.Capacity(), SizeT{16}, __LINE__);

    numbers.Clear();

    numbers[10U];
    numbers[20U];
    numbers[30U];
    numbers[40U];

    numbers.Remove(40U);

    test.IsEqual(numbers.Size(), SizeT{4}, __LINE__);

    numbers.Reorder();

    test.IsEqual(numbers.Size(), SizeT{3}, __LINE__);

    numbers[40U];
    test.IsEqual(numbers.Size(), SizeT{4}, __LINE__);

    numbers.Remove(10U);
    numbers.Remove(40U);

    numbers.Reorder();

    test.IsEqual(numbers.Size(), SizeT{2}, __LINE__);

    numbers[10U];
    numbers[40U];

    test.IsEqual(numbers.Size(), SizeT{4}, __LINE__);

    numbers.Remove(10U);
    numbers.Remove(20U);
    numbers.Remove(30U);

    numbers.Reorder();

    test.IsEqual(numbers.Size(), SizeT{1}, __LINE__);

    numbers.Clear();

    numbers[10U];
    numbers[20U];
    numbers.Remove(10U);

    numbers.Reorder();

    test.IsEqual(numbers.Size(), SizeT{1}, __LINE__);

    numbers.Clear();
    numbers.Reorder();
    test.IsEqual(numbers.Size(), SizeT{0}, __LINE__);

    numbers.Insert(10U);
    numbers.Insert(20U);
    numbers.Insert(30U);
    numbers.Insert(40U);

    numbers.Reorder();
    test.IsEqual(numbers.Size(), SizeT{4}, __LINE__);
}

static int RunHListTests() {
    QTest test{"HList.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("HList Test 1", TestHList1);
    test.Test("HList Numeric Test", TestHListNumeric);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
