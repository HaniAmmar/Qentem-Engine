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

    using Core = ReserverCore<(sizeof(void *) * 2), page_size>;

    char      *var1;
    char      *var2;
    char      *var3;
    char      *var4;
    SystemLong ptr_number;

    var1 = Core::Reserve<char>(size);
    var2 = Core::Reserve<char>(size);
    var3 = Core::Reserve<char>(page_size * 2);
    var4 = Core::Reserve<char>(page_size * 2);
    Core::Release(var3, page_size * 2);
    Core::Release(var1, size);
    Core::Release(var2, size);
    Core::Release(var4, page_size * 2);

    test.IsTrue(Core::IsEmpty(), __LINE__);
    test.IsEqual(Core::TotalBlocks(), SizeT{1}, __LINE__);

    var1       = Core::Reserve<char>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (size - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, size);

    var1       = Core::Reserve<char>(size + 15);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (size - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, size + 15);

    var1       = Core::Reserve<char, 32U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (32U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, size);

    var2       = Core::Reserve<char>(size);
    var1       = Core::Reserve<char, 64U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (64U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, size);
    Core::Release(var2, size);

    var2       = Core::Reserve<char>(size);
    var1       = Core::Reserve<char, 64U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (64U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, size);
    Core::Release(var2, size);

    var2       = Core::Reserve<char, 32U>(size);
    var1       = Core::Reserve<char, 64U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (64U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, size);
    Core::Release(var2, size);

    var1       = Core::Reserve<char, 64U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (64U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, size);

    var2       = Core::Reserve<char>(size);
    var1       = Core::Reserve<char, 128U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (128U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, size);
    Core::Release(var2, size);

    var2       = Core::Reserve<char, 32U>(size);
    var1       = Core::Reserve<char, 128U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (128U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, size);
    Core::Release(var2, size);

    var2       = Core::Reserve<char, 64U>(size);
    var1       = Core::Reserve<char, 128U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (128U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, size);
    Core::Release(var2, size);

    var2       = Core::Reserve<char, 128U>(size);
    var1       = Core::Reserve<char, 128U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (128U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, size);
    Core::Release(var2, size);

    var2       = Core::Reserve<char, 256U>(size);
    var1       = Core::Reserve<char, 128U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (128U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, size);
    Core::Release(var2, size);

    var1       = Core::Reserve<char, 256U>(size);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (256U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, size);

    var1       = Core::Reserve<char, 32U>(1);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (32U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, 1);

    var1       = Core::Reserve<char, 64U>(1);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (64U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, 1);

    var1       = Core::Reserve<char, 128U>(1);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (128U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, 1);

    var1       = Core::Reserve<char, 256U>(1);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (256U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, 1);

    struct Small {
        char Data[3];
    };

    auto var_s = Core::Reserve<Small, 256U>(1);
    ptr_number = reinterpret_cast<SystemLong>(var_s);
    test.IsEqual((ptr_number & (256U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var_s, 1);

    struct alignas(256) Small2 {
        char Data[3];
    };

    test.IsEqual(SizeT32{alignof(Small2)}, SizeT32{256}, __LINE__);
    auto var_s2 = Core::Reserve<Small2>(1);
    ptr_number  = reinterpret_cast<SystemLong>(var_s2);
    test.IsEqual((ptr_number & (256U - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var_s2, 1);

    Core::Reset();
}

static void TestReserver2(QTest &test) {
    constexpr SizeT32 size      = (sizeof(void *) * 2);
    constexpr SizeT32 page_size = 4096;
    SystemLong        ptr_number;

    using Core  = ReserverCore<(sizeof(void *) * 2), page_size>;
    using Core2 = ReserverCore<(sizeof(void *) * 2), page_size * 2U>;

    char *var1;
    char *var2;
    char *var3;
    char *var4;

    var1 = Core::Reserve<char>(size);
    var2 = Core::Reserve<char>(size);
    var3 = Core::Reserve<char>(page_size * 2);
    var4 = Core::Reserve<char>(page_size * 2);
    Core::Release(var3, page_size * 2);
    Core::Release(var1, size);
    Core::Release(var2, size);
    Core::Release(var4, page_size * 2);
    var1 = Core::Reserve<char>(page_size);
    Core::Release(var1, page_size);

    test.IsTrue(Core::IsEmpty(), __LINE__);
    test.IsEqual(Core::TotalBlocks(), SizeT{1}, __LINE__);

    Core::Reset();

    var1 = Core::Reserve<char, page_size>(page_size * 2);

    test.IsNotNull(var1, __LINE__);
    ptr_number = reinterpret_cast<SystemLong>(var1);
    test.IsEqual((ptr_number & (page_size - 1U)), SystemLong{0}, __LINE__);
    Core::Release(var1, page_size * 2);

    test.IsTrue(Core::IsEmpty(), __LINE__);
    test.IsTrue((Core::TotalBlocks() <= SizeT{1}), __LINE__);

    var1 = Core::Reserve<char>(256);
    var2 = Core::Reserve<char>(64);
    Core::Release(var1, 256);
    var3       = Core::Reserve<char, page_size>(page_size);
    ptr_number = reinterpret_cast<SystemLong>(var3);
    Core::Release(var2, 64);
    Core::Release(var3, page_size);

    test.IsEqual((ptr_number & (page_size - 1U)), SystemLong{0}, __LINE__);
    test.IsTrue(Core::IsEmpty(), __LINE__);
    test.IsTrue((Core::TotalBlocks() <= SizeT{1}), __LINE__);

    Core::Reset();

    var1 = Core2::Reserve<char>(256);
    var2 = Core2::Reserve<char>(64);
    Core2::Release(var1, 256);
    var3       = Core2::Reserve<char, page_size>(page_size);
    ptr_number = reinterpret_cast<SystemLong>(var3);
    Core2::Release(var2, 64);
    Core2::Release(var3, page_size);

    test.IsEqual((ptr_number & (page_size - 1U)), SystemLong{0}, __LINE__);
    test.IsTrue(Core2::IsEmpty(), __LINE__);
    test.IsTrue((Core2::TotalBlocks() <= SizeT{1}), __LINE__);

    Core2::Reset();
}

static void TestReserver3(QTest &test) {
    using Core = ReserverCore<>;

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

    var1 = Core::Reserve<char>(16);
    var2 = Core::Reserve<char>(32);
    Core::Release(var1, 16);
    var4 = Core::Reserve<char>(64);
    Core::Release(var2, 32);
    var3 = Core::Reserve<char>(128);
    Core::Release(var4, 64);
    var5 = Core::Reserve<char>(256);
    Core::Release(var3, 128);
    var6 = Core::Reserve<char>(512);
    Core::Release(var5, 256);
    var7 = Core::Reserve<char>(1024);
    Core::Release(var6, 512);
    var8 = Core::Reserve<char>(2048);
    Core::Release(var7, 1024);
    var9 = Core::Reserve<char>(4096);
    Core::Release(var8, 2048);
    var10 = Core::Reserve<char>(8192);
    Core::Release(var9, 4096);
    Core::Release(var10, 8192);

    test.IsTrue(Core::IsEmpty(), __LINE__);
    test.IsEqual(Core::TotalBlocks(), SizeT{1}, __LINE__);

    for (SystemLong i = 0; i < 10000; ++i) {
        auto *ptr = Core::Reserve<char>(17);
        Core::Release(ptr, 17);
    }

    test.IsTrue(Core::IsEmpty(), __LINE__);
    test.IsEqual(Core::TotalBlocks(), SizeT{1}, __LINE__);

    for (SystemLong i = 1; i <= 10000; ++i) {
        auto *ptr = Core::Reserve<char>((i % 128) + 1);
        Core::Release(ptr, (i % 128) + 1);
    }

    test.IsTrue(Core::IsEmpty(), __LINE__);
    test.IsEqual(Core::TotalBlocks(), SizeT{1}, __LINE__);

    char *block[32];

    for (SystemLong i = 0; i < 32; ++i) {
        block[i] = Core::Reserve<char>(354);
    }

    for (SystemLong i = 0; i < 32; ++i) {
        Core::Release(block[i], 354);
    }

    test.IsTrue(Core::IsEmpty(), __LINE__);
    test.IsEqual(Core::TotalBlocks(), SizeT{1}, __LINE__);

    Core::Reset();
}

static void TestReserverShrink(QTest &test) {
    // Initialize Reserver with 4 KiB block size and 16-byte alignment.
    using Core = ReserverCore<16, (4 * 1024)>;

    char *var1;
    char *var2;

    // Allocate 48 bytes and shrink it to 16 bytes.
    var1 = static_cast<char *>(Core::Reserve<char>(48));
    test.IsTrue(Core::Shrink(var1, 48, 16), __LINE__);

    // Allocate 16 bytes — it should reuse the tail of the shrunk region.
    var2 = static_cast<char *>(Core::Reserve<char>(16));
    test.IsEqual(var1 + 16, var2, __LINE__); // Should follow immediately.

    // Clean up both regions.
    Core::Release(var1, 16);
    Core::Release(var2, 16);

    // Confirm memory is fully released, and only one block remains.
    test.IsTrue(Core::IsEmpty(), __LINE__);
    test.IsEqual(Core::TotalBlocks(), SizeT{1}, __LINE__);

    // Full allocation from the top block.
    SystemLong max  = Core::GetActiveBlocks()->First()->UsableSize();
    SystemLong half = max / 2;
    var1            = static_cast<char *>(Core::Reserve<char>(max));

    test.IsEqual(static_cast<void *>(var1), Core::GetActiveBlocks()->First()->Data(), __LINE__);

    // Shrink the large allocation by half.
    test.IsTrue(Core::Shrink(var1, max, half), __LINE__);

    test.IsFalse(Core::IsEmpty(), __LINE__);
    test.IsEqual(Core::TotalBlocks(), SizeT{1}, __LINE__);

    // Attempt to allocate more than remaining capacity — triggers new block.
    var2 = static_cast<char *>(Core::Reserve<char>(8 * 1024));
    test.IsEqual(Core::TotalBlocks(), SizeT{2}, __LINE__);

    // Release the second block's allocation.
    Core::Release(var2, 8 * 1024);
    test.IsFalse(Core::IsEmpty(), __LINE__);

    test.IsEqual(Core::TotalBlocks(), SizeT{1}, __LINE__);

    // Finally release the remaining half of var1.
    Core::Release(var1, max - half);

    test.IsTrue(Core::IsEmpty(), __LINE__);
    test.IsEqual(Core::TotalBlocks(), SizeT{1}, __LINE__);

    Core::Reset();
}

static void TestReserverExpand(QTest &test) {
    using Core = ReserverCore<16, (8 * 1024)>;

    constexpr SystemLong full_bit_region = sizeof(void *) * 8 * 16;

    char *var1;
    char *var2;
    char *var3;
    char *var4;

    var1 = static_cast<char *>(Core::Reserve<char>(16));
    test.IsTrue(Core::TryExpand(var1, 16, 32), __LINE__);
    Core::Release(var1, 32);
    test.IsTrue(Core::IsEmpty(), __LINE__);

    var1 = static_cast<char *>(Core::Reserve<char>(64));
    test.IsTrue(Core::TryExpand(var1, 64, 128), __LINE__);
    Core::Release(var1, 128);
    test.IsTrue(Core::IsEmpty(), __LINE__);

    var1 = static_cast<char *>(Core::Reserve<char>(full_bit_region - 16));
    test.IsTrue(Core::TryExpand(var1, (full_bit_region - 16), full_bit_region), __LINE__);
    test.IsTrue(Core::TryExpand(var1, full_bit_region, (full_bit_region * 2)), __LINE__);
    Core::Release(var1, full_bit_region * 2);
    test.IsTrue(Core::IsEmpty(), __LINE__);

    var1 = static_cast<char *>(Core::Reserve<char>(64));
    var2 = static_cast<char *>(Core::Reserve<char>(64));
    var3 = static_cast<char *>(Core::Reserve<char>(2 * 1024));
    var4 = static_cast<char *>(Core::Reserve<char>(2 * 1024));
    Core::Release(var4, 2 * 1024);

    test.IsTrue(Core::Shrink(var1, 64, 16), __LINE__);
    test.IsFalse(Core::TryExpand(var1, 16, 128), __LINE__);
    test.IsTrue(Core::TryExpand(var1, 16, 64), __LINE__);
    Core::Release(var1, 64);
    Core::Release(var2, 64);
    Core::Release(var3, 2 * 1024);
    test.IsTrue(Core::IsEmpty(), __LINE__);

    var1 = static_cast<char *>(Core::Reserve<char>(full_bit_region / 2));
    test.IsTrue(Core::TryExpand(var1, (full_bit_region / 2), ((full_bit_region / 2) + full_bit_region)), __LINE__);
    Core::Release(var1, ((full_bit_region / 2) + full_bit_region));
    test.IsTrue(Core::IsEmpty(), __LINE__);

    var1 = static_cast<char *>(Core::Reserve<char>(2 * 1024));
    var2 = static_cast<char *>(Core::Reserve<char>(2 * 1024));

    test.IsTrue(Core::Shrink(var1, 2 * 1024, 16), __LINE__);
    test.IsTrue(Core::TryExpand(var1, 16, 2 * 1024), __LINE__);

    Core::Release(var1, 2 * 1024);
    Core::Release(var2, 2 * 1024);

    var1 = static_cast<char *>(Core::Reserve<char>(((2 * 1024) + 512)));
    var2 = static_cast<char *>(Core::Reserve<char>(1536));

    test.IsTrue(Core::Shrink(var1, ((2 * 1024) + 512), 512), __LINE__);
    test.IsTrue(Core::TryExpand(var1, 512, ((2 * 1024) + 512)), __LINE__);

    Core::Release(var2, 1536);
    Core::Release(var1, ((2 * 1024) + 512));

    test.IsTrue(Core::IsEmpty(), __LINE__);
    test.IsEqual(Core::TotalBlocks(), SizeT{1}, __LINE__);

    Core::Reset();
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
