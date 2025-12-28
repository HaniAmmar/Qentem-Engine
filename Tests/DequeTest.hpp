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

#ifndef QENTEM_DEQUE_TESTS_H
#define QENTEM_DEQUE_TESTS_H

#include "Qentem/QTest.hpp"
#include "Qentem/Deque.hpp"
#include "Qentem/String.hpp"

namespace Qentem {
namespace Test {

static void TestDeque1(QTest &test) {
    const SizeT *storage;
    Deque<SizeT> numbers1;
    Deque<SizeT> numbers2{8};

    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);
    test.IsTrue(numbers1.IsEmpty(), __LINE__);
    test.IsFalse(numbers1.IsNotEmpty(), __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);
    test.IsNull(numbers1.Last(), __LINE__);

    test.IsEqual(numbers2.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers2.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNull(numbers2.First(), __LINE__);

    numbers2.Reset();
    test.IsEqual(numbers2.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers2.Capacity(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNull(numbers2.First(), __LINE__);

    numbers1.Reserve(5);
    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);

    numbers1.Reserve(10);
    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{16}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);

    numbers1.Resize(18);
    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{32}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);

    storage = numbers1.First();
    numbers1.Resize(4);
    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);
    test.IsNotEqual(numbers1.Storage(), storage, __LINE__);

    numbers2.Resize(5);
    test.IsEqual(numbers2.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers2.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNull(numbers2.First(), __LINE__);

    numbers2.Resize(2);
    test.IsEqual(numbers2.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers2.Capacity(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNull(numbers2.First(), __LINE__);

    numbers2.Resize(0);
    test.IsEqual(numbers2.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers2.Capacity(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNull(numbers2.First(), __LINE__);

    numbers1.Reserve(0);
    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);

    numbers1.Reset();
    numbers2.Reset();

    numbers1.ResizeWithDefaultInit(8);
    storage = numbers1.First();
    test.IsEqual(numbers1.Size(), SizeT{8}, __LINE__);
    test.IsFalse(numbers1.IsEmpty(), __LINE__);
    test.IsTrue(numbers1.IsNotEmpty(), __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);

    numbers1.ResizeWithDefaultInit(4);
    test.IsNotEqual(numbers1.First(), storage, __LINE__);

    numbers1.ResizeWithDefaultInit(16);
    test.IsEqual(numbers1.Size(), SizeT{16}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{16}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);

    storage  = numbers1.First();
    numbers2 = QUtility::Move(numbers1);
    test.IsEqual(numbers2.Size(), SizeT{16}, __LINE__);
    test.IsEqual(numbers2.Capacity(), SizeT{16}, __LINE__);
    test.IsEqual(numbers2.First(), storage, __LINE__);

    test.IsNull(numbers1.Storage(), __LINE__);
    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{0}, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);

    numbers1 = Deque<SizeT>(QUtility::Move(numbers2));
    test.IsEqual(numbers1.Size(), SizeT{16}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{16}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers1.First(), storage, __LINE__);
    test.IsNull(numbers2.Storage(), __LINE__);
    test.IsEqual(numbers2.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers2.Capacity(), SizeT{0}, __LINE__);
    test.IsNull(numbers2.First(), __LINE__);

    storage = numbers1.First();
    numbers1.Clear();
    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{16}, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);
    test.IsEqual(numbers1.Storage(), storage, __LINE__);

    numbers1.ResizeWithDefaultInit(3);
    numbers2.ResizeWithDefaultInit(5);
    storage = numbers2.First();
}

static void TestDeque2(QTest &test) {
    Deque<SizeT> numbers1;
    Deque<SizeT> numbers2{8};
    SizeT       *storage;

    auto &item = numbers1.Insert(10);

    test.IsEqual(numbers1.Size(), SizeT{1}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{10}, __LINE__);
    test.IsEqual(numbers1.First(), &item, __LINE__);
    test.IsEqual(numbers1.Last(), &item, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{10}, __LINE__);

    auto &item2 = numbers1.Insert(20);

    test.IsEqual(numbers1.Size(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{20}, __LINE__);
    test.IsEqual(numbers1.First(), &item, __LINE__);
    test.IsEqual(numbers1.Last(), &item2, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{20}, __LINE__);

    storage = numbers1.Storage();
    numbers1 += SizeT{30};

    test.IsEqual(numbers1.Size(), SizeT{3}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{30}, __LINE__);
    test.IsNotEqual(numbers1.First(), storage, __LINE__);
    test.IsNotEqual(numbers1.First(), &item, __LINE__);
    test.IsNotEqual(numbers1.Last(), &item2, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{20}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{30}, __LINE__);

    storage = numbers1.Storage();
    numbers1 += SizeT{40};
    numbers1 += SizeT{50};
    numbers1 += SizeT{60};
    numbers1 += SizeT{70};
    numbers1 += SizeT{80};

    test.IsEqual(numbers1.Size(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{80}, __LINE__);
    test.IsNotEqual(numbers1.First(), storage, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{20}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{30}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{40}, __LINE__);
    test.IsEqual(*(numbers1.Get(4)), SizeT{50}, __LINE__);
    test.IsEqual(*(numbers1.Get(5)), SizeT{60}, __LINE__);
    test.IsEqual(*(numbers1.Get(6)), SizeT{70}, __LINE__);
    test.IsEqual(*(numbers1.Get(7)), SizeT{80}, __LINE__);

    numbers1.Dequeue();

    test.IsEqual(numbers1.Size(), SizeT{7}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{1}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{20}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{80}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{20}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{30}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{40}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{50}, __LINE__);
    test.IsEqual(*(numbers1.Get(4)), SizeT{60}, __LINE__);
    test.IsEqual(*(numbers1.Get(5)), SizeT{70}, __LINE__);
    test.IsEqual(*(numbers1.Get(6)), SizeT{80}, __LINE__);

    numbers1.Dequeue();

    test.IsEqual(numbers1.Size(), SizeT{6}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{2}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{30}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{80}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{30}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{40}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{50}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{60}, __LINE__);
    test.IsEqual(*(numbers1.Get(4)), SizeT{70}, __LINE__);
    test.IsEqual(*(numbers1.Get(5)), SizeT{80}, __LINE__);

    numbers1.Dequeue();

    test.IsEqual(numbers1.Size(), SizeT{5}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{3}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{40}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{80}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{40}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{50}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{60}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{70}, __LINE__);
    test.IsEqual(*(numbers1.Get(4)), SizeT{80}, __LINE__);

    numbers1.Dequeue();

    test.IsEqual(numbers1.Size(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{4}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{50}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{80}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{50}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{60}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{70}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{80}, __LINE__);

    numbers1.Dequeue();

    test.IsEqual(numbers1.Size(), SizeT{3}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{5}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{60}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{80}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{60}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{70}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{80}, __LINE__);

    numbers1.Dequeue();

    test.IsEqual(numbers1.Size(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{6}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{70}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{80}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{70}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{80}, __LINE__);

    numbers1.Dequeue();

    test.IsEqual(numbers1.Size(), SizeT{1}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{7}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{80}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{80}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{80}, __LINE__);

    numbers1.Dequeue();
    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{8}, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);
    test.IsNull(numbers1.Last(), __LINE__);
    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);

    numbers1 += SizeT{10};
    numbers1 += SizeT{20};
    numbers1 += SizeT{30};
    numbers1 += SizeT{40};
    numbers1 += SizeT{50};

    numbers1.Dequeue();
    numbers1.Dequeue();
    numbers1 += SizeT{10};

    test.IsEqual(numbers1.Size(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{10}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{30}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{10}, __LINE__);

    numbers1 += SizeT{5};

    test.IsEqual(numbers1.Size(), SizeT{5}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{30}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{5}, __LINE__);

    numbers1.Dequeue();
    numbers1.Dequeue();
    numbers1.Dequeue();
    numbers1.Dequeue();

    test.IsEqual(numbers1.Size(), SizeT{1}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{14}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{5}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{5}, __LINE__);

    numbers1 += SizeT{10};
    numbers1 += SizeT{20};
    numbers1 += SizeT{30};
    numbers1 += SizeT{40};
    numbers1 += SizeT{50};

    test.IsEqual(numbers1.Size(), SizeT{6}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{5}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{50}, __LINE__);

    numbers1.Dequeue();

    numbers1 += SizeT{60};
    numbers1 += SizeT{70};
    numbers1 += SizeT{80};
    numbers1 += SizeT{90};

    test.IsEqual(numbers1.Size(), SizeT{9}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{16}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{90}, __LINE__);

    numbers1.Reset();

    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);

    numbers1 += SizeT{10};
    numbers1 += SizeT{20};
    numbers1 += SizeT{30};
    numbers1 += SizeT{40};

    numbers1.Dequeue();
    numbers1.Dequeue();
    numbers1.Dequeue();

    numbers1 += SizeT{50};

    test.IsEqual(numbers1.Size(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{3}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{40}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{50}, __LINE__);

    numbers1 += SizeT{60};

    test.IsEqual(numbers1.Size(), SizeT{3}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{40}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{60}, __LINE__);

    numbers1 += SizeT{70};

    test.IsEqual(numbers1.Size(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{40}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{70}, __LINE__);

    numbers1 += SizeT{80};

    test.IsEqual(numbers1.Size(), SizeT{5}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{40}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{80}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{40}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{50}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{60}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{70}, __LINE__);
    test.IsEqual(*(numbers1.Get(4)), SizeT{80}, __LINE__);

    numbers1.Resize(0);

    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);
    test.IsNull(numbers1.Last(), __LINE__);
}

static void TestDeque3(QTest &test) {
    Deque<SizeT> numbers1;
    Deque<SizeT> numbers2{8};

    numbers1 += SizeT{10};
    numbers1 += SizeT{20};
    numbers1 += SizeT{30};
    numbers1 += SizeT{40};
    numbers1 += SizeT{50};
    numbers1 += SizeT{60};
    numbers1 += SizeT{70};
    numbers1 += SizeT{80};

    numbers1.DequeueBack();

    test.IsEqual(numbers1.Size(), SizeT{7}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{70}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{20}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{30}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{40}, __LINE__);
    test.IsEqual(*(numbers1.Get(4)), SizeT{50}, __LINE__);
    test.IsEqual(*(numbers1.Get(5)), SizeT{60}, __LINE__);
    test.IsEqual(*(numbers1.Get(6)), SizeT{70}, __LINE__);

    numbers1.DequeueBack();

    test.IsEqual(numbers1.Size(), SizeT{6}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{60}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{20}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{30}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{40}, __LINE__);
    test.IsEqual(*(numbers1.Get(4)), SizeT{50}, __LINE__);
    test.IsEqual(*(numbers1.Get(5)), SizeT{60}, __LINE__);

    numbers1.DequeueBack();

    test.IsEqual(numbers1.Size(), SizeT{5}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{50}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{20}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{30}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{40}, __LINE__);
    test.IsEqual(*(numbers1.Get(4)), SizeT{50}, __LINE__);

    numbers1.DequeueBack();

    test.IsEqual(numbers1.Size(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{40}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{20}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{30}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{40}, __LINE__);

    numbers1.DequeueBack();

    test.IsEqual(numbers1.Size(), SizeT{3}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{30}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{20}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{30}, __LINE__);

    numbers1.DequeueBack();

    test.IsEqual(numbers1.Size(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{20}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{20}, __LINE__);

    numbers1.DequeueBack();

    test.IsEqual(numbers1.Size(), SizeT{1}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{10}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{10}, __LINE__);

    numbers1.DequeueBack();

    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);
    test.IsNull(numbers1.Last(), __LINE__);
    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);

    numbers1 += SizeT{10};
    numbers1 += SizeT{20};
    numbers1 += SizeT{30};
    numbers1 += SizeT{40};
    numbers1 += SizeT{50};

    numbers1.DequeueBack();
    numbers1.DequeueBack();

    numbers1 += SizeT{40};

    test.IsEqual(numbers1.Size(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{40}, __LINE__);

    numbers1 += SizeT{5};

    test.IsEqual(numbers1.Size(), SizeT{5}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{5}, __LINE__);

    numbers1.DequeueBack();
    numbers1.DequeueBack();
    numbers1.DequeueBack();
    numbers1.DequeueBack();

    test.IsEqual(numbers1.Size(), SizeT{1}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{10}, __LINE__);

    numbers1 += SizeT{20};
    numbers1 += SizeT{30};
    numbers1 += SizeT{40};
    numbers1 += SizeT{50};
    numbers1 += SizeT{60};

    test.IsEqual(numbers1.Size(), SizeT{6}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{60}, __LINE__);

    numbers1.DequeueBack();

    numbers1 += SizeT{60};
    numbers1 += SizeT{70};
    numbers1 += SizeT{80};
    numbers1 += SizeT{90};

    test.IsEqual(numbers1.Size(), SizeT{9}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{16}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{90}, __LINE__);

    numbers1.Reset();

    numbers1 += SizeT{10};
    numbers1 += SizeT{20};
    numbers1 += SizeT{30};
    numbers1 += SizeT{40};

    numbers1.DequeueBack();
    numbers1.DequeueBack();
    numbers1.DequeueBack();

    numbers1 += SizeT{50};

    test.IsEqual(numbers1.Size(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{50}, __LINE__);

    numbers1 += SizeT{60};

    test.IsEqual(numbers1.Size(), SizeT{3}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{60}, __LINE__);

    numbers1 += SizeT{70};

    test.IsEqual(numbers1.Size(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{70}, __LINE__);

    numbers1 += SizeT{80};

    test.IsEqual(numbers1.Size(), SizeT{5}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{80}, __LINE__);

    test.IsEqual(*(numbers1.Get(0)), SizeT{10}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{50}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{60}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{70}, __LINE__);
    test.IsEqual(*(numbers1.Get(4)), SizeT{80}, __LINE__);
}

static void TestDeque4(QTest &test) {
    Deque<SizeT>        numbers1;
    Deque<SizeT>        numbers2{8};
    Deque<String<char>> strings;

    for (SizeT i = 0; i < 8; i++) {
        numbers1 += i;
    }

    const SizeT *storage1 = numbers1.Storage();
    const SizeT *storage2 = numbers1.Storage();

    test.IsEqual(numbers1.Size(), SizeT{8}, __LINE__);
    test.IsTrue((numbers1.Capacity() >= SizeT{8}), __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);

    SizeT sum = 0;
    for (SizeT i = 0; i < 8; i++) {
        SizeT *item = numbers1.Get(i);
        if (item != nullptr) {
            sum += *item;
        }
    }

    test.IsEqual(sum, 28U, __LINE__);

    numbers2.Insert(storage1[0]);
    numbers2.Insert(storage1[1]);

    for (SizeT i = 2; i < 8; i++) {
        numbers2.Insert(storage1[i]);
        test.IsEqual(storage2[i], storage1[i], __LINE__);
    }

    test.IsEqual(numbers2.Size(), SizeT{8}, __LINE__);
    test.IsEqual(numbers2.Capacity(), SizeT{8}, __LINE__);

    numbers2.Resize(16);
    test.IsEqual(numbers2.Size(), SizeT{8}, __LINE__);
    test.IsEqual(numbers2.Capacity(), SizeT{16}, __LINE__);

    storage2 = numbers2.Storage();
    numbers2.Clear();
    test.IsEqual(numbers2.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers2.Capacity(), SizeT{16}, __LINE__);
    test.IsEqual(numbers2.Storage(), storage2, __LINE__);

    numbers2.Reset();
    test.IsEqual(numbers2.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers2.Capacity(), SizeT{0}, __LINE__);

    // Checking move
    String<char> str1("val--------------------------------1");
    String<char> str2("val--------------------------------2");

    const char *str1_cstr = str1.First();
    const char *str2_cstr = str2.First();

    strings += QUtility::Move(str1);

    test.IsEqual(strings.Storage()[0].First(), str1_cstr, __LINE__);

    strings.Insert(QUtility::Move(str2));
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

    numbers1.Clear();
    numbers1 += SizeT{6};
    numbers1 += SizeT{7};
    numbers1 += SizeT{5};
    numbers1 += SizeT{1};
    numbers1 += SizeT{3};
    numbers1 += SizeT{4};
    numbers1 += SizeT{2};

    test.IsEqual(numbers1.Size(), 7U, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    const SizeT size = numbers1.Size();
    sum              = 0;

    for (SizeT i = 0; i < size; i++) {
        SizeT *item = numbers1.First();

        numbers1.Dequeue();

        if (item != nullptr) {
            sum += *item;
        }
    }

    test.IsEqual(sum, 28U, __LINE__);
}

static void TestDeque5(QTest &test) {
    Deque<SizeT> numbers1;

    numbers1 += SizeT{1};
    numbers1 += SizeT{2};
    numbers1 += SizeT{3};
    numbers1 += SizeT{4};

    numbers1.Dequeue();
    numbers1.Dequeue();

    numbers1 += SizeT{5};
    numbers1 += SizeT{6};

    numbers1.Clear();

    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNull(numbers1.First(), __LINE__);
    test.IsNull(numbers1.Last(), __LINE__);

    ///////////////

    numbers1 += SizeT{1};
    numbers1 += SizeT{2};
    numbers1 += SizeT{3};
    numbers1 += SizeT{4};

    numbers1.Dequeue();

    test.IsEqual(numbers1.PopCount(), SizeT{1}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{2}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{4}, __LINE__);

    numbers1 += SizeT{5};

    numbers1.Resize(1);

    test.IsEqual(numbers1.Size(), SizeT{1}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{2}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{2}, __LINE__);

    ////////////////////
    numbers1.Reserve(4);

    test.IsEqual(numbers1.Size(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);

    numbers1 += SizeT{1};
    numbers1 += SizeT{2};
    numbers1 += SizeT{3};
    numbers1 += SizeT{4};

    numbers1.Resize(3);

    test.IsEqual(numbers1.Size(), SizeT{3}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{1}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{3}, __LINE__);

    numbers1.Resize(2);

    test.IsEqual(numbers1.Size(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{1}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{2}, __LINE__);

    ////////////////
    numbers1.Reset();
    numbers1.ResizeWithDefaultInit(2);

    test.IsEqual(numbers1.Size(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{2}, __LINE__);
    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{0}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{0}, __LINE__);

    numbers1.Reset();

    numbers1 += SizeT{1};
    numbers1 += SizeT{2};

    numbers1.ResizeWithDefaultInit(4);

    test.IsEqual(numbers1.Size(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{1}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{2}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{0}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{0}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{0}, __LINE__);

    numbers1 += SizeT{3};
    numbers1 += SizeT{4};

    test.IsEqual(numbers1.Size(), SizeT{6}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsNotNull(numbers1.First(), __LINE__);
    test.IsNotNull(numbers1.Last(), __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{1}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{2}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{0}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{0}, __LINE__);
    test.IsEqual(*(numbers1.Get(4)), SizeT{3}, __LINE__);
    test.IsEqual(*(numbers1.Get(5)), SizeT{4}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{4}, __LINE__);

    /////////////////////
    numbers1.Reset();

    numbers1 += SizeT{1};
    numbers1 += SizeT{2};
    numbers1 += SizeT{3};
    numbers1 += SizeT{4};

    numbers1.Dequeue();

    numbers1 += SizeT{5};

    numbers1.ResizeWithDefaultInit(8);

    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Size(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);
    test.IsEqual(*(numbers1.First()), SizeT{2}, __LINE__);
    test.IsEqual(*(numbers1.Last()), SizeT{0}, __LINE__);
    test.IsEqual(*(numbers1.Get(0)), SizeT{2}, __LINE__);
    test.IsEqual(*(numbers1.Get(1)), SizeT{3}, __LINE__);
    test.IsEqual(*(numbers1.Get(2)), SizeT{4}, __LINE__);
    test.IsEqual(*(numbers1.Get(3)), SizeT{5}, __LINE__);
    test.IsEqual(*(numbers1.Get(4)), SizeT{0}, __LINE__);
    test.IsEqual(*(numbers1.Get(5)), SizeT{0}, __LINE__);
    test.IsEqual(*(numbers1.Get(6)), SizeT{0}, __LINE__);
    test.IsEqual(*(numbers1.Get(7)), SizeT{0}, __LINE__);

    numbers1.Compress();

    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Size(), SizeT{8}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{8}, __LINE__);

    numbers1.Dequeue();
    numbers1.Dequeue();
    numbers1.Dequeue();
    numbers1.Dequeue();

    numbers1.Compress();

    test.IsEqual(numbers1.PopCount(), SizeT{0}, __LINE__);
    test.IsEqual(numbers1.Size(), SizeT{4}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);

    numbers1.Dequeue();

    numbers1.Compress();

    test.IsEqual(numbers1.PopCount(), SizeT{1}, __LINE__);
    test.IsEqual(numbers1.Size(), SizeT{3}, __LINE__);
    test.IsEqual(numbers1.Capacity(), SizeT{4}, __LINE__);
}

static int RunDequeTests() {
    QTest test{"Deque.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("Deque Test 1", TestDeque1);
    test.Test("Deque Test 2", TestDeque2);
    test.Test("Deque Test 3", TestDeque3);
    test.Test("Deque Test 4", TestDeque4);
    test.Test("Deque Test 5", TestDeque5);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
