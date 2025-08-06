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

static void TestReserverShrink(QTest &test) {
    // Initialize Reserver with 4 KiB block size and 16-byte alignment.
    ReserverCore<16, (4 * 1024)> r{};

    char *var1;
    char *var2;

    // Allocate 48 bytes and shrink it to 16 bytes.
    var1 = static_cast<char *>(r.Reserve(48));
    test.IsTrue(r.Shrink(var1, 48, 16), __LINE__);

    // Allocate 16 bytes — it should reuse the tail of the shrunk region.
    var2 = static_cast<char *>(r.Reserve(16));
    test.IsEqual(var1 + 16, var2, __LINE__); // Should follow immediately.

    // Clean up both regions.
    r.Release(var1, 16);
    r.Release(var2, 16);

    // Confirm memory is fully released, and only one block remains.
    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);

    // Full allocation from the top block.
    SystemIntType max  = r.GetBlocks().First()->UsableSize();
    SystemIntType half = max / 2;
    var1               = static_cast<char *>(r.Reserve(max));

    // Shrink the large allocation by half.
    test.IsTrue(r.Shrink(var1, max, half), __LINE__);

    test.IsFalse(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);

    // Attempt to allocate more than remaining capacity — triggers new block.
    var2 = static_cast<char *>(r.Reserve(8 * 1024));
    test.IsEqual(r.TotalBlocks(), SizeT{2}, __LINE__);

    // Release the second block's allocation.
    r.Release(var2, 8 * 1024);
    test.IsFalse(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__); // Block count collapses.

    // Finally release the remaining half of var1.
    r.Release(var1, max - half);

    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);
}

static void TestReserverExpand(QTest &test) {
    ReserverCore<16, (8 * 1024)> r{};
    constexpr SystemIntType      full_bit_region = sizeof(void *) * 8 * 16;

    char *var1;
    char *var2;

    var1 = static_cast<char *>(r.Reserve(16));
    test.IsEqual(r.TryExpand(var1, 16, 32), SystemIntType{32}, __LINE__);
    r.Release(var1, 32);
    test.IsTrue(r.IsEmpty(), __LINE__);

    var1 = static_cast<char *>(r.Reserve(64));
    test.IsEqual(r.TryExpand(var1, 64, 128), SystemIntType{128}, __LINE__);
    r.Release(var1, 128);
    test.IsTrue(r.IsEmpty(), __LINE__);

    var1 = static_cast<char *>(r.Reserve(full_bit_region - 16));
    test.IsEqual(r.TryExpand(var1, (full_bit_region - 16), full_bit_region), full_bit_region, __LINE__);
    test.IsEqual(r.TryExpand(var1, full_bit_region, (full_bit_region * 2)), SystemIntType(full_bit_region * 2),
                 __LINE__);
    r.Release(var1, full_bit_region * 2);
    test.IsTrue(r.IsEmpty(), __LINE__);

    var1 = static_cast<char *>(r.Reserve(64));
    var2 = static_cast<char *>(r.Reserve(64));

    test.IsTrue(r.Shrink(var1, 64, 16), __LINE__);
    test.IsEqual(r.TryExpand(var1, 16, 128), SystemIntType{16}, __LINE__);
    test.IsTrue(r.TryExpand(var1, 16, 64), __LINE__);
    r.Release(var1, 64);
    r.Release(var2, 64);
    test.IsTrue(r.IsEmpty(), __LINE__);

    var1 = static_cast<char *>(r.Reserve(full_bit_region / 2));
    test.IsEqual(r.TryExpand(var1, (full_bit_region / 2), ((full_bit_region / 2) + full_bit_region)),
                 ((full_bit_region / 2) + full_bit_region), __LINE__);
    r.Release(var1, ((full_bit_region / 2) + full_bit_region));
    test.IsTrue(r.IsEmpty(), __LINE__);

    var1 = static_cast<char *>(r.Reserve(2 * 1024));
    var2 = static_cast<char *>(r.Reserve(2 * 1024));

    test.IsTrue(r.Shrink(var1, 2 * 1024, 16), __LINE__);
    test.IsEqual(r.TryExpand(var1, 16, 2 * 1024), SystemIntType{2 * 1024}, __LINE__);

    r.Release(var1, 2 * 1024);
    r.Release(var2, 2 * 1024);

    var1 = static_cast<char *>(r.Reserve(((2 * 1024) + 512)));
    var2 = static_cast<char *>(r.Reserve(1536));

    test.IsTrue(r.Shrink(var1, ((2 * 1024) + 512), 512), __LINE__);
    test.IsEqual(r.TryExpand(var1, 512, ((2 * 1024) + 512)), SystemIntType((2 * 1024) + 512), __LINE__);

    r.Release(var2, 1536);
    r.Release(var1, ((2 * 1024) + 512));

    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);
}

static int RunReserverTests() {
    QTest test{"Reserver.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("Reserver Test 1", TestReserver1);
    test.Test("Reserver Test 2", TestReserver2);
    test.Test("Reserver Test 3", TestReserver3);
    test.Test("Reserver Test Shrink", TestReserverShrink);
    test.Test("Reserver Test Expand", TestReserverExpand);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
