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

#ifndef QENTEM_RESERVER_TESTS_H
#define QENTEM_RESERVER_TESTS_H

#include "QTest.hpp"
#include "Reserver.hpp"

namespace Qentem {
namespace Test {

QENTEM_MAYBE_UNUSED
static void TestReserver1(QTest &test) {
    constexpr SizeT32 size      = (sizeof(void *) * 2);
    constexpr SizeT32 page_size = 4096;

    ReserverCore<(sizeof(void *) * 2), page_size> r{};

    void *var1;
    void *var2;
    void *var3;
    void *var4;

    var1 = r.Reserve(size);
    var2 = r.Reserve(size);
    var3 = r.Reserve(page_size * 2);
    var4 = r.Reserve(page_size * 2);
    r.Release(var3, page_size * 2);
    r.Release(var1, size);
    r.Release(var2, size);
    r.Release(var4, page_size * 2);

    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);
}

QENTEM_MAYBE_UNUSED
static void TestReserver2(QTest &test) {
    constexpr SizeT32 size      = (sizeof(void *) * 2);
    constexpr SizeT32 page_size = 4096;

    ReserverCore<(sizeof(void *) * 2), page_size> r{};

    void *var1;
    void *var2;
    void *var3;
    void *var4;

    var1 = r.Reserve(size);
    var2 = r.Reserve(size);
    var3 = r.Reserve(page_size * 2);
    var4 = r.Reserve(page_size * 2);
    r.Release(var3, page_size * 2);
    r.Release(var1, size);
    r.Release(var2, size);
    r.Release(var4, page_size * 2);
    var1 = r.Reserve(page_size);
    r.Release(var1, page_size);

    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);
}

QENTEM_MAYBE_UNUSED
static void TestReserver3(QTest &test) {
    ReserverCore<> r{};
    void          *var1;
    void          *var2;
    void          *var3;
    void          *var4;
    void          *var5;
    void          *var6;
    void          *var7;
    void          *var8;
    void          *var9;
    void          *var10;

    var1 = r.Reserve(16);
    var2 = r.Reserve(32);
    r.Release(var1, 16);
    var4 = r.Reserve(64);
    r.Release(var2, 32);
    var3 = r.Reserve(128);
    r.Release(var4, 64);
    var5 = r.Reserve(256);
    r.Release(var3, 128);
    var6 = r.Reserve(512);
    r.Release(var5, 256);
    var7 = r.Reserve(1024);
    r.Release(var6, 512);
    var8 = r.Reserve(2048);
    r.Release(var7, 1024);
    var9 = r.Reserve(4096);
    r.Release(var8, 2048);
    var10 = r.Reserve(8192);
    r.Release(var9, 4096);
    r.Release(var10, 8192);

    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);
}

static int RunReserverTests() {
    QTest test{"Reserver.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("Reserver Test 1", TestReserver1);
    test.Test("Reserver Test 2", TestReserver2);
    test.Test("Reserver Test 3", TestReserver3);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
