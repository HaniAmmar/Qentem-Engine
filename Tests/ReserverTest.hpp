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

#ifndef QENTEM_RESERVER_TESTS_H
#define QENTEM_RESERVER_TESTS_H

#include "Qentem/QTest.hpp"
#include "Qentem/Reserver.hpp"

namespace Qentem {
namespace Test {

static void TestReserver1(QTest &test) {
    constexpr SizeT32 size      = (sizeof(void *) * 2);
    constexpr SizeT32 page_size = 4096;

    ReserverCore<(sizeof(void *) * 2), page_size> r{};

    char      *var1;
    char      *var2;
    char      *var3;
    char      *var4;
    SystemLong ptr_number;

    var1 = r.Reserve<char>(size);
    var2 = r.Reserve<char>(size);
    var3 = r.Reserve<char>(page_size * 2);
    var4 = r.Reserve<char>(page_size * 2);
    r.Release(var3, page_size * 2);
    r.Release(var1, size);
    r.Release(var2, size);
    r.Release(var4, page_size * 2);

    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);

    var1       = r.Reserve<char>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (size - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, size);

    var1       = r.Reserve<char>(size + 15);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (size - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, size + 15);

    var1       = r.Reserve<char, 32U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (32U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, size);

    var2       = r.Reserve<char>(size);
    var1       = r.Reserve<char, 64U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (64U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, size);
    r.Release(var2, size);

    var2       = r.Reserve<char>(size);
    var1       = r.Reserve<char, 64U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (64U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, size);
    r.Release(var2, size);

    var2       = r.Reserve<char, 32U>(size);
    var1       = r.Reserve<char, 64U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (64U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, size);
    r.Release(var2, size);

    var1       = r.Reserve<char, 64U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (64U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, size);

    var2       = r.Reserve<char>(size);
    var1       = r.Reserve<char, 128U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (128U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, size);
    r.Release(var2, size);

    var2       = r.Reserve<char, 32U>(size);
    var1       = r.Reserve<char, 128U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (128U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, size);
    r.Release(var2, size);

    var2       = r.Reserve<char, 64U>(size);
    var1       = r.Reserve<char, 128U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (128U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, size);
    r.Release(var2, size);

    var2       = r.Reserve<char, 128U>(size);
    var1       = r.Reserve<char, 128U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (128U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, size);
    r.Release(var2, size);

    var2       = r.Reserve<char, 256U>(size);
    var1       = r.Reserve<char, 128U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (128U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, size);
    r.Release(var2, size);

    var1       = r.Reserve<char, 256U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (256U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, size);

    var1       = r.Reserve<char, 32U>(1);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (32U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, 1);

    var1       = r.Reserve<char, 64U>(1);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (64U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, 1);

    var1       = r.Reserve<char, 128U>(1);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (128U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, 1);

    var1       = r.Reserve<char, 256U>(1);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (256U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, 1);

    struct Small {
        char Data[3];
    };

    auto var_s = r.Reserve<Small, 256U>(1);
    ptr_number = reinterpret_cast<SystemLong>(var_s);
    test.IsEqual((ptr_number & (256U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var_s, 1);

    struct alignas(256) Small2 {
        char Data[3];
    };

    test.IsEqual(SizeT32{alignof(Small2)}, SizeT32{256}, __LINE__);
    auto var_s2 = r.Reserve<Small2>(1);
    ptr_number  = reinterpret_cast<SystemLong>(var_s2);
    test.IsEqual((ptr_number & (256U - 1U)), SystemLong{0}, __LINE__);
    r.Release(var_s2, 1);
}

static void TestReserver2(QTest &test) {
    constexpr SizeT32 size      = (sizeof(void *) * 2);
    constexpr SizeT32 page_size = 4096;
    SystemLong        ptr_number;

    ReserverCore<(sizeof(void *) * 2), page_size>      r{};
    ReserverCore<(sizeof(void *) * 2), page_size * 2U> r2{};

    char *var1;
    char *var2;
    char *var3;
    char *var4;

    var1 = r.Reserve<char>(size);
    var2 = r.Reserve<char>(size);
    var3 = r.Reserve<char>(page_size * 2);
    var4 = r.Reserve<char>(page_size * 2);
    r.Release(var3, page_size * 2);
    r.Release(var1, size);
    r.Release(var2, size);
    r.Release(var4, page_size * 2);
    var1 = r.Reserve<char>(page_size);
    r.Release(var1, page_size);

    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);

    var1 = r.Reserve<char, page_size>(page_size * 2);

    test.IsNotNull(var1, __LINE__);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (page_size - 1U)), SystemLong{0}, __LINE__);
    r.Release(var1, page_size * 2);

    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsTrue((r.TotalBlocks() <= SizeT{1}), __LINE__);

    var1 = r.Reserve<char>(256);
    var2 = r.Reserve<char>(64);
    r.Release(var1, 256);
    var3       = r.Reserve<char, page_size>(page_size);
    ptr_number = reinterpret_cast<SystemLong>(var3);
    r.Release(var2, 64);
    r.Release(var3, page_size);

    test.IsEqual((ptr_number & (page_size - 1U)), SystemLong{0}, __LINE__);
    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsTrue((r.TotalBlocks() <= SizeT{1}), __LINE__);

    var1 = r2.Reserve<char>(256);
    var2 = r2.Reserve<char>(64);
    r2.Release(var1, 256);
    var3       = r2.Reserve<char, page_size>(page_size);
    ptr_number = reinterpret_cast<SystemLong>(var3);
    r2.Release(var2, 64);
    r2.Release(var3, page_size);

    test.IsEqual((ptr_number & (page_size - 1U)), SystemLong{0}, __LINE__);
    test.IsTrue(r2.IsEmpty(), __LINE__);
    test.IsTrue((r2.TotalBlocks() <= SizeT{1}), __LINE__);
}

static void TestReserver3(QTest &test) {
    ReserverCore<> r{};

    char *var1;
    char *var2;
    char *var3;
    char *var4;
    char *var5;
    char *var6;
    char *var7;
    char *var8;
    char *var9;
    char *var10;

    var1 = r.Reserve<char>(16);
    var2 = r.Reserve<char>(32);
    r.Release(var1, 16);
    var4 = r.Reserve<char>(64);
    r.Release(var2, 32);
    var3 = r.Reserve<char>(128);
    r.Release(var4, 64);
    var5 = r.Reserve<char>(256);
    r.Release(var3, 128);
    var6 = r.Reserve<char>(512);
    r.Release(var5, 256);
    var7 = r.Reserve<char>(1024);
    r.Release(var6, 512);
    var8 = r.Reserve<char>(2048);
    r.Release(var7, 1024);
    var9 = r.Reserve<char>(4096);
    r.Release(var8, 2048);
    var10 = r.Reserve<char>(8192);
    r.Release(var9, 4096);
    r.Release(var10, 8192);

    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);

    for (SystemLong i = 0; i < 10000; ++i) {
        auto *ptr = r.Reserve<char>(17);
        r.Release(ptr, 17);
    }

    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);

    for (SystemLong i = 1; i <= 10000; ++i) {
        auto *ptr = r.Reserve<char>((i % 128) + 1);
        r.Release(ptr, (i % 128) + 1);
    }

    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);

    char *block[32];

    for (SystemLong i = 0; i < 32; ++i) {
        block[i] = r.Reserve<char>(354);
    }

    for (SystemLong i = 0; i < 32; ++i) {
        r.Release(block[i], 354);
    }

    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);
}

static void TestReserverShrink(QTest &test) {
    // Initialize Reserver with 4 KiB block size and 16-byte alignment.
    ReserverCore<16, (4 * 1024)> r{};

    char *var1;
    char *var2;

    // Allocate 48 bytes and shrink it to 16 bytes.
    var1 = static_cast<char *>(r.Reserve<char>(48));
    test.IsTrue(r.Shrink(var1, 48, 16), __LINE__);

    // Allocate 16 bytes — it should reuse the tail of the shrunk region.
    var2 = static_cast<char *>(r.Reserve<char>(16));
    test.IsEqual(var1 + 16, var2, __LINE__); // Should follow immediately.

    // Clean up both regions.
    r.Release(var1, 16);
    r.Release(var2, 16);

    // Confirm memory is fully released, and only one block remains.
    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);

    // Full allocation from the top block.
    SystemLong max  = r.GetActiveBlocks()->First()->UsableSize();
    SystemLong half = max / 2;
    var1            = static_cast<char *>(r.Reserve<char>(max));

    test.IsEqual(static_cast<void *>(var1), r.GetActiveBlocks()->First()->Data(), __LINE__);

    // Shrink the large allocation by half.
    test.IsTrue(r.Shrink(var1, max, half), __LINE__);

    test.IsFalse(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);

    // Attempt to allocate more than remaining capacity — triggers new block.
    var2 = static_cast<char *>(r.Reserve<char>(8 * 1024));
    test.IsEqual(r.TotalBlocks(), SizeT{2}, __LINE__);

    // Release the second block's allocation.
    r.Release(var2, 8 * 1024);
    test.IsFalse(r.IsEmpty(), __LINE__);

    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);

    // Finally release the remaining half of var1.
    r.Release(var1, max - half);

    test.IsTrue(r.IsEmpty(), __LINE__);
    test.IsEqual(r.TotalBlocks(), SizeT{1}, __LINE__);
}

static void TestReserverExpand(QTest &test) {
    ReserverCore<16, (8 * 1024)> r{};

    constexpr SystemLong full_bit_region = sizeof(void *) * 8 * 16;

    char *var1;
    char *var2;
    char *var3;
    char *var4;

    var1 = static_cast<char *>(r.Reserve<char>(16));
    test.IsTrue(r.TryExpand(var1, 16, 32), __LINE__);
    r.Release(var1, 32);
    test.IsTrue(r.IsEmpty(), __LINE__);

    var1 = static_cast<char *>(r.Reserve<char>(64));
    test.IsTrue(r.TryExpand(var1, 64, 128), __LINE__);
    r.Release(var1, 128);
    test.IsTrue(r.IsEmpty(), __LINE__);

    var1 = static_cast<char *>(r.Reserve<char>(full_bit_region - 16));
    test.IsTrue(r.TryExpand(var1, (full_bit_region - 16), full_bit_region), __LINE__);
    test.IsTrue(r.TryExpand(var1, full_bit_region, (full_bit_region * 2)), __LINE__);
    r.Release(var1, full_bit_region * 2);
    test.IsTrue(r.IsEmpty(), __LINE__);

    var1 = static_cast<char *>(r.Reserve<char>(64));
    var2 = static_cast<char *>(r.Reserve<char>(64));
    var3 = static_cast<char *>(r.Reserve<char>(2 * 1024));
    var4 = static_cast<char *>(r.Reserve<char>(2 * 1024));
    r.Release(var4, 2 * 1024);

    test.IsTrue(r.Shrink(var1, 64, 16), __LINE__);
    test.IsFalse(r.TryExpand(var1, 16, 128), __LINE__);
    test.IsTrue(r.TryExpand(var1, 16, 64), __LINE__);
    r.Release(var1, 64);
    r.Release(var2, 64);
    r.Release(var3, 2 * 1024);
    test.IsTrue(r.IsEmpty(), __LINE__);

    var1 = static_cast<char *>(r.Reserve<char>(full_bit_region / 2));
    test.IsTrue(r.TryExpand(var1, (full_bit_region / 2), ((full_bit_region / 2) + full_bit_region)), __LINE__);
    r.Release(var1, ((full_bit_region / 2) + full_bit_region));
    test.IsTrue(r.IsEmpty(), __LINE__);

    var1 = static_cast<char *>(r.Reserve<char>(2 * 1024));
    var2 = static_cast<char *>(r.Reserve<char>(2 * 1024));

    test.IsTrue(r.Shrink(var1, 2 * 1024, 16), __LINE__);
    test.IsTrue(r.TryExpand(var1, 16, 2 * 1024), __LINE__);

    r.Release(var1, 2 * 1024);
    r.Release(var2, 2 * 1024);

    var1 = static_cast<char *>(r.Reserve<char>(((2 * 1024) + 512)));
    var2 = static_cast<char *>(r.Reserve<char>(1536));

    test.IsTrue(r.Shrink(var1, ((2 * 1024) + 512), 512), __LINE__);
    test.IsTrue(r.TryExpand(var1, 512, ((2 * 1024) + 512)), __LINE__);

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
