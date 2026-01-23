/*
 * Copyright (c) 2026 Hani Ammar
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

#ifndef QENTEM_OBJECT_POOL_TESTS_H
#define QENTEM_OBJECT_POOL_TESTS_H

#include "Qentem/QTest.hpp"
#include "Qentem/Array.hpp"
#include "Qentem/QPool.hpp"

namespace Qentem {
namespace Test {

static void TestQPool1(QTest &test) {
    Array<SizeT32 *>     items{32}; // Keep track of retrieved items
    QPool<SizeT32, 8U>   numbers1;  // 8-byte aligned pool
    QPool<SizeT32, 128U> numbers2;  // 128-byte aligned pool
    SizeT32             *num{nullptr};

    // Initially, the pool has no constructed objects and nothing in use
    test.IsEqual(numbers1.ConstructedCount(), 0U, __LINE__);
    test.IsEqual(numbers1.InUseCount(), 0U, __LINE__);

    // Retrieve a single item and verify construction and usage counters
    num = numbers1.Get();
    test.IsNotNull(num, __LINE__);
    items += num;

    test.IsEqual(numbers1.ConstructedCount(), 1U, __LINE__);
    test.IsEqual(numbers1.InUseCount(), 1U, __LINE__);

    // Return the object to the pool without destruction
    numbers1.Recycle(num);
    test.IsEqual(numbers1.ConstructedCount(), 1U, __LINE__);
    test.IsEqual(numbers1.InUseCount(), 0U, __LINE__);

    // Retrieve again and ensure we get the same item (LIFO reuse)
    num = numbers1.Get();
    test.IsEqual(numbers1.ConstructedCount(), 1U, __LINE__);
    test.IsEqual(numbers1.InUseCount(), 1U, __LINE__);
    test.IsEqual(num, *(items.Last()), __LINE__);

    numbers1.Recycle(num);
    test.IsEqual(numbers1.ConstructedCount(), 1U, __LINE__);
    test.IsEqual(numbers1.InUseCount(), 0U, __LINE__);

    items.Clear();

    // Allocate 32 items from both pools and validate alignment
    SizeT32 index = 0U;
    while (index < 32U) {
        ++index;
        num          = numbers1.Get();
        SizeT32 *tmp = numbers2.Get();

        test.IsNotNull(num, __LINE__);
        test.IsNotNull(tmp, __LINE__);

        // Verify counters increment correctly
        test.IsEqual(numbers1.ConstructedCount(), index, __LINE__);
        test.IsEqual(numbers1.InUseCount(), index, __LINE__);

        // Check alignment via bitwise AND
        test.IsEqual((reinterpret_cast<SystemLong>(num) & SystemLong{7}), SystemLong{0}, __LINE__);
        test.IsEqual((reinterpret_cast<SystemLong>(tmp) & SystemLong{127}), SystemLong{0}, __LINE__);

        items += num;
        *num = index; // store value for later verification
    }

    test.IsEqual(numbers1.InUseCount(), 32U, __LINE__);
    test.IsEqual(numbers1.ConstructedCount(), 32U, __LINE__);

    // Recycle all items and ensure in-use counter decrements correctly
    do {
        --index;
        numbers1.Recycle(items.Storage()[index]);
        test.IsEqual(numbers1.InUseCount(), index, __LINE__);
    } while (index != 0U);

    test.IsEqual(numbers1.ConstructedCount(), 32U, __LINE__);
    test.IsEqual(numbers1.InUseCount(), 0U, __LINE__);

    // Verify LIFO order: retrieving items yields the same stored values
    while (index < 32U) {
        ++index;
        test.IsEqual(index, *(numbers1.Get()), __LINE__);
    }

    test.IsEqual(numbers1.ConstructedCount(), 32U, __LINE__);
    test.IsEqual(numbers1.InUseCount(), 32U, __LINE__);

    // Reset pool: all objects destructed, memory released
    numbers1.Reset();
    test.IsEqual(numbers1.ConstructedCount(), 0U, __LINE__);
    test.IsEqual(numbers1.InUseCount(), 0U, __LINE__);
}

static int RunQPoolTests() {
    QTest test{"QPool.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("QPool Test 1", TestQPool1);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
