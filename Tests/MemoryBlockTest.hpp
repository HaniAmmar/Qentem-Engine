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

#ifndef QENTEM_MEMORY_BLOCK_TESTS_H
#define QENTEM_MEMORY_BLOCK_TESTS_H

#include "QTest.hpp"
#include "MemoryBlock.hpp"
#include "LiteArray.hpp"

namespace Qentem {
namespace Test {
constexpr SystemIntType MemoryBlockPointerWidth{sizeof(void *) * 8};

template <typename LineNumber_T>
static bool TestMemoryBlockSeedTableEnd(QTest &test, LiteArray<SystemIntType> &table, SizeT size,
                                        SystemIntType bit_count, LineNumber_T line) {
    SizeT index = 0;
    table.Clear();

    while (index < size) {
        table += 0;
        ++index;
    }

    do {
        --index;
        SystemIntType &current_region = table.Storage()[index];

        SizeT offset = 0;

        while ((offset < MemoryBlockPointerWidth) && (bit_count != 0)) {
            current_region |= (SystemIntType{1} << offset);
            ++offset;
            --bit_count;
        }
    } while ((index != 0) && (bit_count != 0));

    test.IsEqual(bit_count, SystemIntType{0}, line);

    return true;
}

static bool TestMemoryBlockVerifyTable(LiteArray<SystemIntType> &table1, SystemIntType *table2) {
    SizeT index = 0;

    while (index < table1.Size()) {
        if (table1.Storage()[index] != table2[index]) {
            return false;
        }

        ++index;
    }

    return true;
}

static bool TestMemoryBlockVerifyAlignment(void *ptr, SystemIntType alignment) {
    const SystemIntType mb_alignment_m1 = alignment - 1;
    const SystemIntType mb_alignment_n  = ~mb_alignment_m1;
    const SystemIntType raw_address     = reinterpret_cast<SystemIntType>(ptr);
    const SystemIntType aligned_address = ((raw_address + mb_alignment_m1) & mb_alignment_n);

    return (aligned_address == raw_address);
}

template <SizeT32 Alignment_T>
static void TestMemoryBlock(QTest &test, LiteArray<SystemIntType> &a_table, SystemIntType capacity,
                            SystemIntType expected_capacity) {
    using MemoryBlockT = MemoryBlock<Alignment_T>;

    MemoryBlockT        mb{capacity};
    const SystemIntType table_size = (mb.Capacity() > (Alignment_T * MemoryBlockPointerWidth))
                                         ? (mb.Capacity() / (Alignment_T * MemoryBlockPointerWidth))
                                         : 1;

    test.IsEqual(mb.Capacity(), SystemIntType{expected_capacity}, __LINE__);
    test.IsNotNull(mb.Base(), __LINE__);
    test.IsNotNull(mb.Data(), __LINE__);
    test.IsEqual(static_cast<const char *>(mb.End()), (static_cast<const char *>(mb.Base()) + mb.Capacity()), __LINE__);
    test.IsNotEqual(mb.TableSize(), SystemIntType{0}, __LINE__);
    test.IsEqual(mb.TableSize(), table_size, __LINE__);
    test.IsTrue(mb.IsEmpty(), __LINE__);
    test.IsNotEqual(mb.Available(), mb.Capacity(), __LINE__);
    test.IsTrue(mb.Capacity() - mb.TableSize() >= mb.Available(), __LINE__);
    test.IsTrue(TestMemoryBlockVerifyAlignment(mb.Base(), MemoryBlockT::MIN_BASE_ALIGNMENT), __LINE__);
    test.IsTrue(TestMemoryBlockVerifyAlignment(mb.Data(), Alignment_T), __LINE__);
    test.IsTrue(TestMemoryBlockVerifyAlignment(mb.Data(), mb.DataAlignment()), __LINE__);

    SizeT32 default_alignment = Alignment_T;
    SizeT32 alignment         = 0;

    while ((default_alignment & 1U) != 1U) {
        alignment += 1U;
        default_alignment >>= 1U;
    }

    test.IsEqual(MemoryBlockT::DefaultAlignmentBit(), alignment, __LINE__);

    mb.ClearTable();
    const SystemIntType unusable =
        static_cast<SystemIntType>(static_cast<char *>(mb.Data()) - static_cast<char *>(mb.Base())) / Alignment_T;

    const SystemIntType chunk_count = (mb.Capacity() >> MemoryBlockT::DefaultAlignmentBit());
    test.IsNotEqual(mb.Available(), (chunk_count - unusable * SystemIntType{8}), __LINE__);

    TestMemoryBlockSeedTableEnd(
        test, a_table, static_cast<SizeT>(mb.TableSize()),
        (unusable + ((chunk_count >= MemoryBlockPointerWidth)
                         ? 0
                         : (MemoryBlockPointerWidth -
                            static_cast<SizeT32>(mb.Capacity() >> MemoryBlockT::DefaultAlignmentBit())))),
        __LINE__);
    test.IsEqual(mb.TableSize(), a_table.Size(), __LINE__);
    test.IsTrue(TestMemoryBlockVerifyTable(a_table, static_cast<SystemIntType *>(mb.Base())), __LINE__);

    mb.DecreaseAvailable(512);
    test.IsFalse(mb.IsEmpty(), __LINE__);
    mb.IncreaseAvailable(512);
    test.IsTrue(mb.IsEmpty(), __LINE__);

    mb.DecreaseAvailable(64);
    test.IsFalse(mb.IsEmpty(), __LINE__);
    mb.DecreaseAvailable(8);
    test.IsFalse(mb.IsEmpty(), __LINE__);
    mb.DecreaseAvailable(16);
    test.IsFalse(mb.IsEmpty(), __LINE__);

    mb.IncreaseAvailable(64);
    test.IsFalse(mb.IsEmpty(), __LINE__);
    mb.IncreaseAvailable(16);
    test.IsFalse(mb.IsEmpty(), __LINE__);
    mb.IncreaseAvailable(8);
    test.IsTrue(mb.IsEmpty(), __LINE__);
    ///////////////////

    void          *region  = mb.ReserveRegion(0, mb.Available() / Alignment_T);
    SystemIntType *table   = static_cast<SystemIntType *>(mb.Base());
    SystemIntType  t_index = 0;

    while (t_index < mb.TableSize()) {
        test.IsEqual(table[t_index], ~SystemIntType{0}, __LINE__);
        ++t_index;
    }

    mb.ReleaseRegion(region, mb.Available() / Alignment_T);
    test.IsTrue(TestMemoryBlockVerifyTable(a_table, static_cast<SystemIntType *>(mb.Base())), __LINE__);

    region             = mb.ReserveRegion(0, 1);
    SystemIntType map0 = table[0];
    test.IsEqual(table[0], (map0 | SystemIntType{1} << (MemoryBlockPointerWidth - 1)), __LINE__);
    mb.ReleaseRegion(region, 1);
    test.IsTrue(TestMemoryBlockVerifyTable(a_table, static_cast<SystemIntType *>(mb.Base())), __LINE__);

    region = mb.ReserveRegion(0, 2);
    map0   = table[0];
    test.IsEqual(table[0], (map0 | SystemIntType{3} << (MemoryBlockPointerWidth - 2)), __LINE__);
    mb.ReleaseRegion(region, 2);
    test.IsTrue(TestMemoryBlockVerifyTable(a_table, static_cast<SystemIntType *>(mb.Base())), __LINE__);

    region = mb.ReserveRegion(0, 3);
    map0   = table[0];
    test.IsEqual(table[0], (map0 | SystemIntType{7} << (MemoryBlockPointerWidth - 3)), __LINE__);
    mb.ReleaseRegion(region, 3);
    test.IsTrue(TestMemoryBlockVerifyTable(a_table, static_cast<SystemIntType *>(mb.Base())), __LINE__);

    const SystemIntType usable_chunks = (chunk_count - unusable);

    if (usable_chunks > MemoryBlockPointerWidth * SystemIntType{2}) {
        region = mb.ReserveRegion(MemoryBlockPointerWidth - 8, 16);
        map0   = table[0];
        test.IsEqual(table[0], (map0 | SystemIntType{255}), __LINE__);
        SystemIntType map1 = table[1];
        test.IsEqual(table[1], (map1 | (SystemIntType{255} << (MemoryBlockPointerWidth - 8))), __LINE__);

        mb.ReleaseRegion(region, 16);
        test.IsTrue(TestMemoryBlockVerifyTable(a_table, static_cast<SystemIntType *>(mb.Base())), __LINE__);

        region        = mb.ReserveRegion(0, 3);
        void *region2 = mb.ReserveRegion(MemoryBlockPointerWidth - 8, 16);

        test.IsNotEqual(region, region2, __LINE__);

        map0 = table[0];
        test.IsEqual(table[0], (map0 | SystemIntType{7} << (MemoryBlockPointerWidth - 3)), __LINE__);

        map0 = table[0];
        test.IsEqual(table[0], (map0 | SystemIntType{255}), __LINE__);
        map1 = table[1];
        test.IsEqual(table[1], (map1 | (SystemIntType{255} << (MemoryBlockPointerWidth - 8))), __LINE__);

        mb.ReleaseRegion(region, 3);
        mb.ReleaseRegion(region2, 16);

        test.IsTrue(TestMemoryBlockVerifyTable(a_table, static_cast<SystemIntType *>(mb.Base())), __LINE__);
    }

    ///////////////////
    MemoryBlockT mb2{QUtility::Move(mb)};
    test.IsEqual(mb2.Capacity(), SystemIntType{expected_capacity}, __LINE__);
    test.IsNotNull(mb2.Base(), __LINE__);
    test.IsNotNull(mb2.Data(), __LINE__);
    test.IsNotEqual(mb2.TableSize(), SystemIntType{0}, __LINE__);
    test.IsTrue(TestMemoryBlockVerifyAlignment(mb2.Base(), MemoryBlockT::MIN_BASE_ALIGNMENT), __LINE__);
    test.IsTrue(TestMemoryBlockVerifyAlignment(mb2.Data(), Alignment_T), __LINE__);
    test.IsTrue(TestMemoryBlockVerifyAlignment(mb2.Data(), mb2.DataAlignment()), __LINE__);

    MemoryBlockT mb3;
    mb3 = QUtility::Move(mb2);
    test.IsEqual(mb3.Capacity(), SystemIntType{expected_capacity}, __LINE__);
    test.IsNotNull(mb3.Base(), __LINE__);
    test.IsNotNull(mb3.Data(), __LINE__);
    test.IsNotEqual(mb3.TableSize(), SystemIntType{0}, __LINE__);
    test.IsTrue(TestMemoryBlockVerifyAlignment(mb3.Base(), MemoryBlockT::MIN_BASE_ALIGNMENT), __LINE__);
    test.IsTrue(TestMemoryBlockVerifyAlignment(mb3.Data(), Alignment_T), __LINE__);
    test.IsTrue(TestMemoryBlockVerifyAlignment(mb3.Data(), mb3.DataAlignment()), __LINE__);
}

static int RunMemoryBlockTests() {
    LiteArray<SystemIntType> a_table{};

    QTest test{"MemoryBlock.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("MemoryBlock Test 4096:8", TestMemoryBlock<8>, false, a_table, SystemIntType{10}, SystemIntType{4096});

    test.Test("MemoryBlock Test 4096:16", TestMemoryBlock<16>, false, a_table, SystemIntType{4096},
              SystemIntType{4096});

    test.Test("MemoryBlock Test 8192:16", TestMemoryBlock<16>, false, a_table, SystemIntType{4096 + 3},
              SystemIntType{8192});

    test.Test("MemoryBlock Test 12288:16", TestMemoryBlock<16>, false, a_table, SystemIntType{4096 + 4096 + 3},
              SystemIntType{12288});

    test.Test("MemoryBlock Test 4096:32", TestMemoryBlock<32>, false, a_table, SystemIntType{4096},
              SystemIntType{4096});

    test.Test("MemoryBlock Test 8192:32", TestMemoryBlock<32>, false, a_table, SystemIntType{4096 + 4095},
              SystemIntType{8192});

    test.Test("MemoryBlock Test 16K:8", TestMemoryBlock<8>, false, a_table, SystemIntType{16 * 1024},
              SystemIntType{16 * 1024});

    test.Test("MemoryBlock Test 16K:16", TestMemoryBlock<16>, false, a_table, SystemIntType{16 * 1024},
              SystemIntType{16 * 1024});

    test.Test("MemoryBlock Test 16K:32", TestMemoryBlock<32>, false, a_table, SystemIntType{16 * 1024},
              SystemIntType{16 * 1024});

    test.Test("MemoryBlock Test 4096:128", TestMemoryBlock<128>, false, a_table, SystemIntType{4096},
              SystemIntType{4096});

    test.Test("MemoryBlock Test 4096:512", TestMemoryBlock<512>, false, a_table, SystemIntType{4096},
              SystemIntType{4096});

    test.Test("MemoryBlock Test 4096:1024", TestMemoryBlock<1024>, false, a_table, SystemIntType{4096},
              SystemIntType{4096});

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
