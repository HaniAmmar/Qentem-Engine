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

#ifndef QENTEM_BIG_INT_TESTS_H
#define QENTEM_BIG_INT_TESTS_H

#include "QTest.hpp"
#include "BigInt.hpp"
#include "StringStream.hpp"

namespace Qentem {
namespace Test {

template <typename BigInt_T>
static void StreamDigits(char *storage, unsigned int &index, BigInt_T &b_int) {
    index = 0;

    while (b_int != 0) {
        storage[index] = (static_cast<char>(b_int.Divide(10)) + '0');
        ++index;
    }
}

template <typename BigInt_T>
static void PrintDigits(BigInt_T b_int, StringStream<char> &stream) {
    constexpr unsigned int max = (((b_int.TotalBitWidth() * 30103U) / 100000U) + 1U);
    char                   storage[max]{0};
    unsigned int           index = 0;

    StreamDigits(&(storage[0]), index, b_int);

    stream.Clear();

    while (index != 0) {
        --index;
        stream << storage[index];
    }
}

/////////////////////////////////////////////////////////

using BigInt_8_128 = BigInt<SizeT8, 128U>;
using BigInt_8_63  = BigInt<SizeT8, 63U>;
using BigInt_8_64  = BigInt_8_63;
using BigInt_8_23  = BigInt<SizeT8, 23U>;

using BigInt_16_128 = BigInt<SizeT16, 128U>;
using BigInt_16_63  = BigInt<SizeT16, 63U>;
using BigInt_16_120 = BigInt<SizeT16, 120U>;

using BigInt_32_128  = BigInt<SizeT32, 128U>;
using BigInt_32_1024 = BigInt<SizeT32, 1024U>;
using BigInt_32_63   = BigInt<SizeT32, 63U>;
using BigInt_32_120  = BigInt<SizeT32, 120U>;

using BigInt_64_128  = BigInt<SizeT64, 128U>;
using BigInt_64_1024 = BigInt<SizeT64, 1024U>;
using BigInt_64_15   = BigInt<SizeT64, 15U>;
using BigInt_64_120  = BigInt<SizeT64, 120U>;

/////////////////////////////////////////////////////////

static void TestBigInt1(QTest &test) {
    test.IsEqual(BigInt_8_128::ByteWidth(), 1U, __LINE__);
    test.IsEqual(BigInt_8_128::BitWidth(), 8U, __LINE__);
    test.IsEqual(BigInt_8_128::TotalBitWidth(), 128U, __LINE__);
    test.IsEqual(BigInt_8_128::MaxIndex(), 15U, __LINE__);

    test.IsEqual(BigInt_8_63::BitWidth(), 8U, __LINE__);
    test.IsEqual(BigInt_8_63::TotalBitWidth(), 64U, __LINE__);
    test.IsEqual(BigInt_8_63::MaxIndex(), 7U, __LINE__);

    test.IsEqual(BigInt_8_23::BitWidth(), 8U, __LINE__);
    test.IsEqual(BigInt_8_23::TotalBitWidth(), 24U, __LINE__);
    test.IsEqual(BigInt_8_23::MaxIndex(), 2U, __LINE__);

    //////////////////////////////

    test.IsEqual(BigInt_16_128::ByteWidth(), 2U, __LINE__);
    test.IsEqual(BigInt_16_128::BitWidth(), 16U, __LINE__);
    test.IsEqual(BigInt_16_128::TotalBitWidth(), 128U, __LINE__);
    test.IsEqual(BigInt_16_128::MaxIndex(), 7U, __LINE__);

    test.IsEqual(BigInt_16_63::BitWidth(), 16U, __LINE__);
    test.IsEqual(BigInt_16_63::TotalBitWidth(), 64U, __LINE__);
    test.IsEqual(BigInt_16_63::MaxIndex(), 3U, __LINE__);

    test.IsEqual(BigInt_16_120::BitWidth(), 16U, __LINE__);
    test.IsEqual(BigInt_16_120::TotalBitWidth(), 128U, __LINE__);
    test.IsEqual(BigInt_16_120::MaxIndex(), 7U, __LINE__);

    //////////////////////////////

    test.IsEqual(BigInt_32_128::ByteWidth(), 4U, __LINE__);
    test.IsEqual(BigInt_32_128::BitWidth(), 32U, __LINE__);
    test.IsEqual(BigInt_32_128::TotalBitWidth(), 128U, __LINE__);
    test.IsEqual(BigInt_32_128::MaxIndex(), 3U, __LINE__);

    test.IsEqual(BigInt_32_1024::ByteWidth(), 4U, __LINE__);
    test.IsEqual(BigInt_32_1024::BitWidth(), 32U, __LINE__);
    test.IsEqual(BigInt_32_1024::TotalBitWidth(), 1024U, __LINE__);
    test.IsEqual(BigInt_32_1024::MaxIndex(), 31U, __LINE__);

    test.IsEqual(BigInt_32_63::BitWidth(), 32U, __LINE__);
    test.IsEqual(BigInt_32_63::TotalBitWidth(), 64U, __LINE__);
    test.IsEqual(BigInt_32_63::MaxIndex(), 1U, __LINE__);

    test.IsEqual(BigInt_32_120::BitWidth(), 32U, __LINE__);
    test.IsEqual(BigInt_32_120::TotalBitWidth(), 128U, __LINE__);
    test.IsEqual(BigInt_32_120::MaxIndex(), 3U, __LINE__);

    //////////////////////////////

    test.IsEqual(BigInt_64_128::ByteWidth(), 8U, __LINE__);
    test.IsEqual(BigInt_64_128::BitWidth(), 64U, __LINE__);
    test.IsEqual(BigInt_64_128::TotalBitWidth(), 128U, __LINE__);
    test.IsEqual(BigInt_64_128::MaxIndex(), 1U, __LINE__);

    test.IsEqual(BigInt_64_1024::ByteWidth(), 8U, __LINE__);
    test.IsEqual(BigInt_64_1024::BitWidth(), 64U, __LINE__);
    test.IsEqual(BigInt_64_1024::TotalBitWidth(), 1024U, __LINE__);
    test.IsEqual(BigInt_64_1024::MaxIndex(), 15U, __LINE__);

    test.IsEqual(BigInt_64_15::BitWidth(), 64U, __LINE__);
    test.IsEqual(BigInt_64_15::TotalBitWidth(), 64U, __LINE__);
    test.IsEqual(BigInt_64_15::MaxIndex(), 0U, __LINE__);

    test.IsEqual(BigInt_64_120::BitWidth(), 64U, __LINE__);
    test.IsEqual(BigInt_64_120::TotalBitWidth(), 128U, __LINE__);
    test.IsEqual(BigInt_64_120::MaxIndex(), 1U, __LINE__);
}

static void TestBigInt2(QTest &test) {
    BigInt_8_64 b_int_1{SizeT64{18446460386757245432ULL}};
    BigInt_8_64 b_int_2{SizeT32{4294901244U}};
    BigInt_8_64 b_int_3{SizeT16{65534U}};
    BigInt_8_64 b_int_4{SizeT8{205U}};

    const SizeT8 *s_64 = b_int_1.Storage();
    test.IsEqual(b_int_1.Index(), 7U, __LINE__);
    test.IsEqual(s_64[7], SizeT8{255U}, __LINE__);
    test.IsEqual(s_64[6], SizeT8{254U}, __LINE__);
    test.IsEqual(s_64[5], SizeT8{253U}, __LINE__);
    test.IsEqual(s_64[4], SizeT8{252U}, __LINE__);
    test.IsEqual(s_64[3], SizeT8{251U}, __LINE__);
    test.IsEqual(s_64[2], SizeT8{250U}, __LINE__);
    test.IsEqual(s_64[1], SizeT8{249U}, __LINE__);
    test.IsEqual(s_64[0], SizeT8{248U}, __LINE__);

    const SizeT8 *s_32 = b_int_2.Storage();
    test.IsEqual(b_int_2.Index(), 3U, __LINE__);
    test.IsEqual(s_32[3], SizeT8{255U}, __LINE__);
    test.IsEqual(s_32[2], SizeT8{254U}, __LINE__);
    test.IsEqual(s_32[1], SizeT8{253U}, __LINE__);
    test.IsEqual(s_32[0], SizeT8{252U}, __LINE__);

    const SizeT8 *s_16 = b_int_3.Storage();
    test.IsEqual(b_int_3.Index(), 1U, __LINE__);
    test.IsEqual(s_16[1], SizeT8{255U}, __LINE__);
    test.IsEqual(s_16[0], SizeT8{254U}, __LINE__);

    const SizeT8 *s_8 = b_int_4.Storage();
    test.IsEqual(b_int_4.Index(), 0U, __LINE__);
    test.IsEqual(s_8[0], SizeT8{205U}, __LINE__);

    //////////////////////////////////////////

    b_int_1 = SizeT8{255U};
    b_int_2 = SizeT32{4294967295U};
    b_int_3 = SizeT16{65535U};
    b_int_4 = SizeT64{18446744073709551615ULL};

    test.IsEqual(b_int_1.Index(), 0U, __LINE__);
    test.IsEqual(b_int_2.Index(), 3U, __LINE__);
    test.IsEqual(b_int_3.Index(), 1U, __LINE__);
    test.IsEqual(b_int_4.Index(), 7U, __LINE__);

    test.IsEqual(SizeT8{b_int_1}, SizeT8{255U}, __LINE__);
    test.IsEqual(SizeT32{b_int_2}, SizeT32{4294967295U}, __LINE__);
    test.IsEqual(SizeT16{b_int_3}, SizeT16{65535U}, __LINE__);
    test.IsEqual(SizeT64{b_int_4}, SizeT64{18446744073709551615ULL}, __LINE__);

    test.IsEqual(SizeT32{b_int_4}, SizeT32{4294967295U}, __LINE__);
    test.IsEqual(SizeT16{b_int_4}, SizeT16{65535U}, __LINE__);
    test.IsEqual(SizeT8{b_int_4}, SizeT64{255U}, __LINE__);

    ///////////////////////////

    b_int_1 = b_int_4;
    b_int_3 = b_int_2;

    test.IsEqual(b_int_1.Index(), 7U, __LINE__);
    test.IsEqual(b_int_3.Index(), 3U, __LINE__);
    test.IsEqual(SizeT64{b_int_3}, SizeT64{4294967295U}, __LINE__);
    test.IsEqual(SizeT64{b_int_1}, SizeT64{18446744073709551615ULL}, __LINE__);

    b_int_1.Clear();
    b_int_3.Clear();

    test.IsEqual(b_int_1.Index(), 0U, __LINE__);
    test.IsEqual(b_int_3.Index(), 0U, __LINE__);
    test.IsEqual(SizeT64{b_int_3}, SizeT64{0}, __LINE__);
    test.IsEqual(SizeT64{b_int_1}, SizeT64{0}, __LINE__);

    b_int_1 = Memory::Move(b_int_4);
    b_int_3 = Memory::Move(b_int_2);

    test.IsEqual(b_int_1.Index(), 7U, __LINE__);
    test.IsEqual(b_int_3.Index(), 3U, __LINE__);
    test.IsEqual(SizeT64{b_int_3}, SizeT64{4294967295U}, __LINE__);
    test.IsEqual(SizeT64{b_int_1}, SizeT64{18446744073709551615ULL}, __LINE__);

    test.IsEqual(b_int_2.Index(), 0U, __LINE__);
    test.IsEqual(b_int_4.Index(), 0U, __LINE__);
    test.IsEqual(SizeT64{b_int_2}, SizeT64{0}, __LINE__);
    test.IsEqual(SizeT64{b_int_4}, SizeT64{0}, __LINE__);
    ///////////////////////////

    const BigInt_8_64 b_int_5{Memory::Move(b_int_1)};
    const BigInt_8_64 b_int_6{Memory::Move(b_int_3)};

    test.IsEqual(b_int_1.Index(), 0U, __LINE__);
    test.IsEqual(b_int_3.Index(), 0U, __LINE__);
    test.IsEqual(b_int_5.Index(), 7U, __LINE__);
    test.IsEqual(b_int_6.Index(), 3U, __LINE__);
    test.IsEqual(SizeT64{b_int_3}, SizeT64{0}, __LINE__);
    test.IsEqual(SizeT64{b_int_1}, SizeT64{0}, __LINE__);
    test.IsEqual(SizeT64{b_int_6}, SizeT64{4294967295U}, __LINE__);
    test.IsEqual(SizeT64{b_int_5}, SizeT64{18446744073709551615ULL}, __LINE__);

    ///////////////////////////

    const BigInt_8_64 b_int_7{b_int_5};
    const BigInt_8_64 b_int_8{b_int_6};

    test.IsEqual(b_int_5.Index(), 7U, __LINE__);
    test.IsEqual(b_int_6.Index(), 3U, __LINE__);
    test.IsEqual(b_int_7.Index(), 7U, __LINE__);
    test.IsEqual(b_int_8.Index(), 3U, __LINE__);
    test.IsEqual(SizeT64{b_int_8}, SizeT64{4294967295U}, __LINE__);
    test.IsEqual(SizeT64{b_int_7}, SizeT64{18446744073709551615ULL}, __LINE__);
    test.IsEqual(SizeT64{b_int_6}, SizeT64{4294967295U}, __LINE__);
    test.IsEqual(SizeT64{b_int_5}, SizeT64{18446744073709551615ULL}, __LINE__);
    test.IsEqual(b_int_5.Number(), SizeT8{255U}, __LINE__);
    ///////////////////////////

    const BigInt_8_64 b_int_9{};
    const BigInt_8_64 b_int_10{b_int_9};
    test.IsEqual(b_int_9.Index(), 0U, __LINE__);
    test.IsEqual(b_int_10.Index(), 0U, __LINE__);
    test.IsEqual(SizeT64{b_int_9}, SizeT64{0}, __LINE__);
    test.IsEqual(SizeT64{b_int_10}, SizeT64{0}, __LINE__);
    ///////////////////////////
    b_int_1 = SizeT8{255U};
    b_int_4 = SizeT64{18446744073709551615ULL};

    b_int_4 = b_int_1;
    test.IsEqual(SizeT64{b_int_4}, SizeT8{255U}, __LINE__);
    test.IsEqual(b_int_4.Number(), SizeT8{255U}, __LINE__);
    test.IsEqual(b_int_4.FindFirstBit(), SizeT32{0U}, __LINE__);
    test.IsEqual(b_int_4.FindLastBit(), SizeT32{7U}, __LINE__);

    b_int_4 <<= 56U;
    test.IsEqual(SizeT64{b_int_4}, SizeT64{18374686479671623680ULL}, __LINE__);
    test.IsEqual(b_int_4.FindFirstBit(), SizeT32{56U}, __LINE__);
    test.IsEqual(b_int_4.FindLastBit(), SizeT32{63U}, __LINE__);

    b_int_4 = SizeT64{18446744073709551615ULL};
    test.IsEqual(b_int_4.FindFirstBit(), SizeT32{0U}, __LINE__);
    b_int_4 <<= 32U;
    test.IsEqual(SizeT64{b_int_4}, SizeT64{18446744069414584320ULL}, __LINE__);

    b_int_4 = SizeT64{18374686479671623935ULL};
    test.IsEqual(b_int_4.Index(), 7U, __LINE__);
    b_int_4 <<= 32U;
    test.IsEqual(b_int_4.Index(), 4U, __LINE__);
    test.IsEqual(SizeT64{b_int_4}, SizeT64{1095216660480ULL}, __LINE__);
    test.IsEqual(b_int_4.Number(), SizeT8{0}, __LINE__);
    test.IsEqual(b_int_4.FindFirstBit(), SizeT32{32U}, __LINE__);
    test.IsEqual(b_int_4.FindLastBit(), SizeT32{39U}, __LINE__);

    b_int_4 = 0;
    b_int_4 |= SizeT64{18374686479671623935ULL};
    test.IsEqual(SizeT64{b_int_4}, SizeT64{18374686479671623935ULL}, __LINE__);
    b_int_4.Clear();
    test.IsTrue(b_int_4.IsZero(), __LINE__);
    b_int_4 |= SizeT64{263882790666240ULL};
    b_int_4 |= SizeT64{158260954923008ULL};
    test.IsEqual(SizeT64{b_int_4}, SizeT64{281406257233920ULL}, __LINE__);
    b_int_4 &= SizeT64{17293839061777121520ULL};
    test.IsEqual(SizeT64{b_int_4}, SizeT64{16492674416640ULL}, __LINE__);

    b_int_4 &= SizeT8{1U};
    test.IsTrue(b_int_4.IsZero(), __LINE__);
    test.IsEqual(b_int_4.Index(), 0U, __LINE__);

    b_int_4.Storage()[5] = SizeT8{3U};
    b_int_4.SetIndex(5U);
    test.IsEqual(b_int_4.Index(), 5U, __LINE__);
    test.IsEqual(SizeT64{b_int_4}, SizeT64{3298534883328}, __LINE__);
}

static void TestBigInt3(QTest &test) {
    BigInt_64_128 b_int_1;

    test.IsTrue(b_int_1.IsZero(), __LINE__);
    test.IsFalse(b_int_1.NotZero(), __LINE__);

    b_int_1 = 18446744073709551615ULL;
    test.IsFalse(b_int_1.IsZero(), __LINE__);
    test.IsTrue(b_int_1.NotZero(), __LINE__);
    test.IsFalse(b_int_1.IsBig(), __LINE__);
    b_int_1 <<= 64U;
    test.IsTrue(b_int_1.IsBig(), __LINE__);
    b_int_1 |= 18446744073709551615ULL;
    test.IsTrue(b_int_1.IsBig(), __LINE__);

    test.IsEqual(b_int_1.Index(), 1U, __LINE__);
    test.IsEqual(SizeT8{b_int_1}, SizeT8{255U}, __LINE__);
    test.IsEqual(SizeT16{b_int_1}, SizeT16{65535U}, __LINE__);
    test.IsEqual(SizeT32{b_int_1}, SizeT32{4294967295U}, __LINE__);
    test.IsEqual(SizeT64{b_int_1}, SizeT64{18446744073709551615ULL}, __LINE__);

    const SizeT64 *s_64 = b_int_1.Storage();
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{18446744073709551615ULL}, __LINE__);
    test.IsEqual(SizeT64{s_64[1]}, SizeT64{18446744073709551615ULL}, __LINE__);

    b_int_1 >>= 96U;
    test.IsEqual(b_int_1.Index(), 0U, __LINE__);
    b_int_1 <<= 32U;
    test.IsEqual(b_int_1.Index(), 0U, __LINE__);
    b_int_1 |= SizeT32{4294967295U};
    test.IsEqual(b_int_1.Index(), 0U, __LINE__);
    b_int_1 <<= 64U;
    test.IsEqual(b_int_1.Index(), 1U, __LINE__);
    b_int_1 |= 18446744073709551615ULL;
    test.IsEqual(b_int_1.Index(), 1U, __LINE__);
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{18446744073709551615ULL}, __LINE__);
    test.IsEqual(SizeT64{s_64[1]}, SizeT64{18446744073709551615ULL}, __LINE__);

    b_int_1 >>= 128U;
    test.IsEqual(b_int_1.Index(), 0U, __LINE__);
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{0}, __LINE__);
    test.IsEqual(SizeT64{s_64[1]}, SizeT64{0}, __LINE__);
    test.IsTrue(b_int_1.IsZero(), __LINE__);
    test.IsFalse(b_int_1.NotZero(), __LINE__);

    b_int_1 = 18446744073709551615ULL;
    b_int_1 <<= 96U;
    test.IsEqual(b_int_1.Index(), 1U, __LINE__);
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{0}, __LINE__);
    test.IsEqual(SizeT64{s_64[1]}, SizeT64{18446744069414584320ULL}, __LINE__);
    b_int_1 >>= 96U;
    test.IsEqual(b_int_1.Index(), 0U, __LINE__);
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{4294967295ULL}, __LINE__);
    test.IsEqual(SizeT64{s_64[1]}, SizeT64{0}, __LINE__);
    test.IsFalse(b_int_1.IsZero(), __LINE__);
    test.IsTrue(b_int_1.NotZero(), __LINE__);

    b_int_1 = 18446744073709551615ULL;
    b_int_1 <<= 32U;
    b_int_1 >>= 16U;
    test.IsEqual(b_int_1.Index(), 1U, __LINE__);
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{18446744073709486080ULL}, __LINE__);
    test.IsEqual(SizeT64{s_64[1]}, SizeT64{65535ULL}, __LINE__);

    b_int_1 = 18446744073709551615ULL;
    b_int_1 <<= 64U;
    b_int_1 |= 18446744073709551615ULL;
    b_int_1 >>= 96U;
    test.IsEqual(b_int_1.Index(), 0U, __LINE__);
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{4294967295ULL}, __LINE__);
    test.IsEqual(SizeT64{s_64[1]}, SizeT64{0}, __LINE__);
    b_int_1 >>= 16U;
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{65535ULL}, __LINE__);
    b_int_1 >>= 13U;
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{7ULL}, __LINE__);
    b_int_1 >>= 2U;
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{1ULL}, __LINE__);
    b_int_1 >>= 1U;
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{0}, __LINE__);
    test.IsTrue(b_int_1.IsZero(), __LINE__);
    test.IsFalse(b_int_1.NotZero(), __LINE__);

    b_int_1 = 18446744073709551615ULL;
    b_int_1 <<= 128U;
    test.IsEqual(b_int_1.Index(), 0U, __LINE__);
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{0}, __LINE__);
    test.IsEqual(SizeT64{s_64[1]}, SizeT64{0}, __LINE__);

    b_int_1 = 18446744073709551615ULL;
    b_int_1 <<= 64U;
    b_int_1 |= 18446744073709551615ULL;
    b_int_1 <<= 96U;
    test.IsEqual(b_int_1.Index(), 1U, __LINE__);
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{0}, __LINE__);
    test.IsEqual(SizeT64{s_64[1]}, SizeT64{18446744069414584320ULL}, __LINE__);

    b_int_1 = 18446744073709551615ULL;
    b_int_1 <<= 64U;
    b_int_1 |= 18446744073709551615ULL;
    b_int_1 <<= 65U;
    test.IsEqual(SizeT64{s_64[1]}, SizeT64{18446744073709551614ULL}, __LINE__);
    b_int_1 >>= 64U;
    b_int_1 &= 65535ULL;
    test.IsEqual(SizeT64{s_64[0]}, SizeT64{65534ULL}, __LINE__);
}

static void TestBigInt4(QTest &test) {
    BigInt_32_128 b_int;

    b_int = 9U;

    test.IsEqual(b_int.Index(), 0U, __LINE__);
    test.IsTrue(b_int == 9U, __LINE__);
    test.IsFalse(b_int != 9U, __LINE__);
    test.IsTrue(b_int >= 9U, __LINE__);
    test.IsTrue(b_int <= 9U, __LINE__);

    test.IsTrue(b_int > 8U, __LINE__);
    test.IsFalse(b_int < 8U, __LINE__);
    test.IsTrue(b_int >= 8U, __LINE__);
    test.IsFalse(b_int <= 8U, __LINE__);

    test.IsFalse(b_int > 10U, __LINE__);
    test.IsTrue(b_int < 10U, __LINE__);
    test.IsFalse(b_int >= 10U, __LINE__);
    test.IsTrue(b_int <= 10U, __LINE__);
    //*****
    test.IsTrue(9U == b_int, __LINE__);
    test.IsFalse(9U != b_int, __LINE__);
    test.IsTrue(9U >= b_int, __LINE__);
    test.IsTrue(9U <= b_int, __LINE__);

    test.IsFalse(8U > b_int, __LINE__);
    test.IsTrue(8U < b_int, __LINE__);
    test.IsFalse(8U >= b_int, __LINE__);
    test.IsTrue(8U <= b_int, __LINE__);

    test.IsTrue(10U > b_int, __LINE__);
    test.IsFalse(10U < b_int, __LINE__);
    test.IsTrue(10U >= b_int, __LINE__);
    test.IsFalse(10U <= b_int, __LINE__);

    b_int = 38654705673ULL;
    test.IsEqual(b_int.Index(), 1U, __LINE__);

    test.IsFalse(b_int == 9U, __LINE__);
    test.IsTrue(b_int != 9U, __LINE__);
    test.IsTrue(b_int >= 9U, __LINE__);
    test.IsFalse(b_int <= 9U, __LINE__);

    test.IsTrue(b_int > 8U, __LINE__);
    test.IsFalse(b_int < 8U, __LINE__);
    test.IsTrue(b_int >= 8U, __LINE__);
    test.IsFalse(b_int <= 8U, __LINE__);

    test.IsTrue(b_int > 10U, __LINE__);
    test.IsFalse(b_int < 10U, __LINE__);
    test.IsTrue(b_int >= 10U, __LINE__);
    test.IsFalse(b_int <= 10U, __LINE__);
    //*****
    test.IsFalse(9U == b_int, __LINE__);
    test.IsTrue(9U != b_int, __LINE__);
    test.IsFalse(9U >= b_int, __LINE__);
    test.IsTrue(9U <= b_int, __LINE__);

    test.IsFalse(8U > b_int, __LINE__);
    test.IsTrue(8U < b_int, __LINE__);
    test.IsFalse(8U >= b_int, __LINE__);
    test.IsTrue(8U <= b_int, __LINE__);

    test.IsFalse(10U > b_int, __LINE__);
    test.IsTrue(10U < b_int, __LINE__);
    test.IsFalse(10U >= b_int, __LINE__);
    test.IsTrue(10U <= b_int, __LINE__);

    //////////////////////////////////////////
    b_int.Clear();
    b_int += SizeT16(10);

    test.IsTrue(b_int == 10U, __LINE__);

    b_int -= SizeT16(2);
    test.IsTrue(b_int == 8U, __LINE__);

    b_int += SizeT32(8388608);
    test.IsTrue(b_int == 8388616U, __LINE__);

    b_int -= SizeT32(8388600U);
    test.IsTrue(b_int == 16U, __LINE__);

    /******************** */
    b_int -= SizeT32(17U);
    test.IsEqual(b_int.Index(), 3U, __LINE__);
    test.IsEqual(b_int.Storage()[0], SizeT32{4294967295U}, __LINE__);
    test.IsEqual(b_int.Storage()[1], SizeT32{4294967295U}, __LINE__);
    test.IsEqual(b_int.Storage()[2], SizeT32{4294967295U}, __LINE__);
    test.IsEqual(b_int.Storage()[3], SizeT32{4294967295U}, __LINE__);

    b_int += SizeT32(1U);
    test.IsEqual(b_int.Index(), 0U, __LINE__);

    b_int -= SizeT64(18446744073709551615ULL);
    test.IsEqual(b_int.Index(), 3U, __LINE__);
    test.IsEqual(b_int.Storage()[0], SizeT32{1U}, __LINE__);
    test.IsEqual(b_int.Storage()[1], SizeT32{0U}, __LINE__);
    test.IsEqual(b_int.Storage()[2], SizeT32{4294967295U}, __LINE__);
    test.IsEqual(b_int.Storage()[3], SizeT32{4294967295U}, __LINE__);
    /******************** */

    b_int = 18446744073709551615ULL;
    b_int += 18446744073709551615ULL;
    test.IsEqual(b_int.Storage()[0], SizeT32(4294967294U), __LINE__);

    b_int += 18446744073709551615ULL;

    test.IsEqual(b_int.Index(), 2U, __LINE__);
    test.IsEqual(b_int.Storage()[0], SizeT32(4294967293U), __LINE__);
    test.IsEqual(b_int.Storage()[1], SizeT32(4294967295U), __LINE__);
    test.IsEqual(b_int.Storage()[2], SizeT32{2U}, __LINE__);
    test.IsEqual(b_int.Storage()[3], SizeT32{0U}, __LINE__);

    b_int -= 18446744073709551613ULL;
    test.IsEqual(b_int.Index(), 2U, __LINE__);
    test.IsEqual(b_int.Storage()[0], SizeT32{0U}, __LINE__);
    test.IsEqual(b_int.Storage()[1], SizeT32{0U}, __LINE__);
    test.IsEqual(b_int.Storage()[2], SizeT32{2U}, __LINE__);
    test.IsEqual(b_int.Storage()[3], SizeT32{0U}, __LINE__);

    b_int -= 1ULL;
    test.IsEqual(b_int.Index(), 2U, __LINE__);
    test.IsEqual(b_int.Storage()[0], SizeT32{4294967295U}, __LINE__);
    test.IsEqual(b_int.Storage()[1], SizeT32{4294967295U}, __LINE__);
    test.IsEqual(b_int.Storage()[2], SizeT32{1U}, __LINE__);
    test.IsEqual(b_int.Storage()[3], SizeT32{0U}, __LINE__);

    //////////////////
    b_int.Storage()[0] = SizeT32{4294967295U};
    b_int.Storage()[1] = SizeT32{4294967295U};
    b_int.Storage()[2] = SizeT32{4294967295U};
    b_int.Storage()[3] = SizeT32{4294967295U};
    b_int.SetIndex(3);
    b_int += 1ULL;

    test.IsEqual(b_int.Index(), 0U, __LINE__);
    test.IsEqual(b_int.Storage()[0], SizeT32{0U}, __LINE__);
    test.IsEqual(b_int.Storage()[1], SizeT32{0U}, __LINE__);
    test.IsEqual(b_int.Storage()[2], SizeT32{0U}, __LINE__);
    test.IsEqual(b_int.Storage()[3], SizeT32{0U}, __LINE__);

    b_int.Storage()[0] = SizeT32{4294967295U};
    b_int.Storage()[1] = SizeT32{4294967295U};
    b_int.Storage()[2] = SizeT32{4294967295U};
    b_int.Storage()[3] = SizeT32{4294967295U};
    b_int.SetIndex(3);
    b_int += 10ULL;

    test.IsEqual(b_int.Index(), 0U, __LINE__);
    test.IsEqual(b_int.Storage()[0], SizeT32{9U}, __LINE__);
    test.IsEqual(b_int.Storage()[1], SizeT32{0U}, __LINE__);
    test.IsEqual(b_int.Storage()[2], SizeT32{0U}, __LINE__);
    test.IsEqual(b_int.Storage()[3], SizeT32{0U}, __LINE__);

    b_int.Storage()[0] = SizeT32{4294967295U};
    b_int.Storage()[1] = SizeT32{4294967295U};
    b_int.Storage()[2] = SizeT32{4294967295U};
    b_int.Storage()[3] = SizeT32{4294967295U};
    b_int.SetIndex(3);
    b_int += SizeT64(18446744073709551615ULL);

    test.IsEqual(b_int.Index(), 1U, __LINE__);
    test.IsEqual(b_int.Storage()[0], SizeT32{4294967294U}, __LINE__);
    test.IsEqual(b_int.Storage()[1], SizeT32{4294967295U}, __LINE__);
    test.IsEqual(b_int.Storage()[2], SizeT32{0U}, __LINE__);
    test.IsEqual(b_int.Storage()[3], SizeT32{0U}, __LINE__);
}

static void TestBigInt5(QTest &test) {
    BigInt_8_64 b_int;

    b_int.Add(10U);
    test.IsEqual(b_int.Number(), SizeT32{10}, __LINE__);

    b_int.Add(10U, 1U);
    test.IsEqual(b_int.Index(), 1U, __LINE__);
    test.IsEqual(SizeT64(b_int), SizeT64{2570ULL}, __LINE__);

    b_int.Add(1U, 7U);
    test.IsEqual(b_int.Index(), 7U, __LINE__);
    test.IsEqual(SizeT64(b_int), SizeT64{72057594037930506ULL}, __LINE__);

    b_int.Subtract(1U);
    test.IsEqual(b_int.Index(), 7U, __LINE__);
    test.IsEqual(SizeT64(b_int), SizeT64{72057594037930505ULL}, __LINE__);

    b_int.Subtract(9U);
    test.IsEqual(b_int.Index(), 7U, __LINE__);
    test.IsEqual(SizeT64(b_int), SizeT64{72057594037930496ULL}, __LINE__);

    b_int.Subtract(1U, 7U);
    test.IsEqual(b_int.Index(), 1U, __LINE__);
    test.IsEqual(SizeT64(b_int), SizeT64{2560ULL}, __LINE__);

    b_int.Subtract(8U, 1U);
    test.IsEqual(b_int.Index(), 1U, __LINE__);
    test.IsEqual(SizeT64(b_int), SizeT64{512ULL}, __LINE__);

    b_int.Add(1U, 0U);
    test.IsEqual(b_int.Index(), 1U, __LINE__);
    test.IsEqual(SizeT64(b_int), SizeT64{513ULL}, __LINE__);

    b_int.Subtract(2U, 1U);
    test.IsEqual(b_int.Index(), 0U, __LINE__);
    test.IsEqual(SizeT64(b_int), SizeT64{1ULL}, __LINE__);
}

static void TestBigInt6(QTest &test) {
    StringStream<char> stream;

    BigInt_8_128   b_int_8_128;
    BigInt_16_128  b_int_16_128;
    BigInt_32_1024 b_int_32_1024;
    BigInt_64_1024 b_int_64_1024;

    b_int_8_128   = 1U;
    b_int_16_128  = 1U;
    b_int_32_1024 = 1U;
    b_int_64_1024 = 1U;

    //////////////////////////////////////

    for (SizeT32 i = 0; i < 38; i++) {
        b_int_8_128 *= 10U;
    }

    PrintDigits(b_int_8_128, stream);

    test.IsEqual(b_int_8_128.Index(), 15U, __LINE__);
    test.IsEqual(stream, "100000000000000000000000000000000000000", __LINE__);

    for (SizeT32 i = 0; i < 38; i++) {
        b_int_8_128 /= 10U;
    }

    test.IsEqual(b_int_8_128.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_8_128), 1U, __LINE__);

    //////////////////////////////////////

    for (SizeT32 i = 0; i < 55; i++) {
        b_int_8_128 *= 5U;
    }

    PrintDigits(b_int_8_128, stream);

    test.IsEqual(b_int_8_128.Index(), 15U, __LINE__);
    test.IsEqual(stream, "277555756156289135105907917022705078125", __LINE__);

    for (SizeT32 i = 0; i < 55; i++) {
        b_int_8_128 /= 5U;
    }

    test.IsEqual(b_int_8_128.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_8_128), 1U, __LINE__);

    //////////////////////////////////////

    for (SizeT32 i = 0; i < 80; i++) {
        b_int_8_128 *= 3U;
    }

    PrintDigits(b_int_8_128, stream);

    test.IsEqual(b_int_8_128.Index(), 15U, __LINE__);
    test.IsEqual(stream, "147808829414345923316083210206383297601", __LINE__);

    for (SizeT32 i = 0; i < 80; i++) {
        b_int_8_128 /= 3U;
    }

    test.IsEqual(b_int_8_128.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_8_128), 1U, __LINE__);

    //////////////////////////////////////

    for (SizeT32 i = 0; i < 45; i++) {
        b_int_8_128 *= 7U;
    }

    PrintDigits(b_int_8_128, stream);

    test.IsEqual(b_int_8_128.Index(), 15U, __LINE__);
    test.IsEqual(stream, "107006904423598033356356300384937784807", __LINE__);

    for (SizeT32 i = 0; i < 45; i++) {
        b_int_8_128 /= 7U;
    }

    test.IsEqual(b_int_8_128.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_8_128), 1U, __LINE__);

    /*************************/

    for (SizeT32 i = 0; i < 38; i++) {
        b_int_16_128 *= 10U;
    }

    PrintDigits(b_int_16_128, stream);

    test.IsEqual(b_int_16_128.Index(), 7U, __LINE__);
    test.IsEqual(stream, "100000000000000000000000000000000000000", __LINE__);

    for (SizeT32 i = 0; i < 38; i++) {
        b_int_16_128 /= 10U;
    }

    test.IsEqual(b_int_16_128.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_16_128), 1U, __LINE__);

    //////////////////////////////////////

    for (SizeT32 i = 0; i < 55; i++) {
        b_int_16_128 *= 5U;
    }

    PrintDigits(b_int_16_128, stream);

    test.IsEqual(b_int_16_128.Index(), 7U, __LINE__);
    test.IsEqual(stream, "277555756156289135105907917022705078125", __LINE__);

    for (SizeT32 i = 0; i < 55; i++) {
        b_int_16_128 /= 5U;
    }

    test.IsEqual(b_int_16_128.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_16_128), 1U, __LINE__);

    //////////////////////////////////////

    for (SizeT32 i = 0; i < 80; i++) {
        b_int_16_128 *= 3U;
    }

    PrintDigits(b_int_16_128, stream);

    test.IsEqual(b_int_16_128.Index(), 7U, __LINE__);
    test.IsEqual(stream, "147808829414345923316083210206383297601", __LINE__);

    for (SizeT32 i = 0; i < 80; i++) {
        b_int_16_128 /= 3U;
    }

    test.IsEqual(b_int_16_128.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_16_128), 1U, __LINE__);

    //////////////////////////////////////

    for (SizeT32 i = 0; i < 45; i++) {
        b_int_16_128 *= 7U;
    }

    PrintDigits(b_int_16_128, stream);

    test.IsEqual(b_int_16_128.Index(), 7U, __LINE__);
    test.IsEqual(stream, "107006904423598033356356300384937784807", __LINE__);

    for (SizeT32 i = 0; i < 45; i++) {
        b_int_16_128 /= 7U;
    }

    test.IsEqual(b_int_16_128.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_16_128), 1U, __LINE__);

    /*************************/

    for (SizeT32 i = 0; i < 38; i++) {
        b_int_32_1024 *= 10U;
    }

    PrintDigits(b_int_32_1024, stream);

    test.IsEqual(b_int_32_1024.Index(), 3U, __LINE__);
    test.IsEqual(stream, "100000000000000000000000000000000000000", __LINE__);

    for (SizeT32 i = 0; i < 38; i++) {
        b_int_32_1024 /= 10U;
    }

    test.IsEqual(b_int_32_1024.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_32_1024), 1U, __LINE__);

    //////////////////////////////////////

    for (SizeT32 i = 0; i < 55; i++) {
        b_int_32_1024 *= 5U;
    }

    PrintDigits(b_int_32_1024, stream);

    test.IsEqual(b_int_32_1024.Index(), 3U, __LINE__);
    test.IsEqual(stream, "277555756156289135105907917022705078125", __LINE__);

    for (SizeT32 i = 0; i < 55; i++) {
        b_int_32_1024 /= 5U;
    }

    test.IsEqual(b_int_32_1024.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_32_1024), 1U, __LINE__);

    //////////////////////////////////////

    for (SizeT32 i = 0; i < 80; i++) {
        b_int_32_1024 *= 3U;
    }

    PrintDigits(b_int_32_1024, stream);

    test.IsEqual(b_int_32_1024.Index(), 3U, __LINE__);
    test.IsEqual(stream, "147808829414345923316083210206383297601", __LINE__);

    for (SizeT32 i = 0; i < 80; i++) {
        b_int_32_1024 /= 3U;
    }

    test.IsEqual(b_int_32_1024.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_32_1024), 1U, __LINE__);

    //////////////////////////////////////

    for (SizeT32 i = 0; i < 45; i++) {
        b_int_32_1024 *= 7U;
    }

    PrintDigits(b_int_32_1024, stream);

    test.IsEqual(b_int_32_1024.Index(), 3U, __LINE__);
    test.IsEqual(stream, "107006904423598033356356300384937784807", __LINE__);

    for (SizeT32 i = 0; i < 45; i++) {
        b_int_32_1024 /= 7U;
    }

    test.IsEqual(b_int_32_1024.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_32_1024), 1U, __LINE__);

    /*************************/

    for (SizeT32 i = 0; i < 38; i++) {
        b_int_64_1024 *= 10U;
    }

    PrintDigits(b_int_64_1024, stream);

    test.IsEqual(b_int_64_1024.Index(), 1U, __LINE__);
    test.IsEqual(stream, "100000000000000000000000000000000000000", __LINE__);

    for (SizeT32 i = 0; i < 38; i++) {
        b_int_64_1024 /= 10U;
    }

    test.IsEqual(b_int_64_1024.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_64_1024), 1U, __LINE__);

    //////////////////////////////////////

    for (SizeT32 i = 0; i < 55; i++) {
        b_int_64_1024 *= 5U;
    }

    PrintDigits(b_int_64_1024, stream);

    test.IsEqual(b_int_64_1024.Index(), 1U, __LINE__);
    test.IsEqual(stream, "277555756156289135105907917022705078125", __LINE__);

    for (SizeT32 i = 0; i < 55; i++) {
        b_int_64_1024 /= 5U;
    }

    test.IsEqual(b_int_64_1024.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_64_1024), 1U, __LINE__);

    //////////////////////////////////////

    for (SizeT32 i = 0; i < 80; i++) {
        b_int_64_1024 *= 3U;
    }

    PrintDigits(b_int_64_1024, stream);

    test.IsEqual(b_int_64_1024.Index(), 1U, __LINE__);
    test.IsEqual(stream, "147808829414345923316083210206383297601", __LINE__);

    for (SizeT32 i = 0; i < 80; i++) {
        b_int_64_1024 /= 3U;
    }

    test.IsEqual(b_int_64_1024.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_64_1024), 1U, __LINE__);

    //////////////////////////////////////

    for (SizeT32 i = 0; i < 45; i++) {
        b_int_64_1024 *= 7U;
    }

    PrintDigits(b_int_64_1024, stream);

    test.IsEqual(b_int_64_1024.Index(), 1U, __LINE__);
    test.IsEqual(stream, "107006904423598033356356300384937784807", __LINE__);

    for (SizeT32 i = 0; i < 45; i++) {
        b_int_64_1024 /= 7U;
    }

    test.IsEqual(b_int_64_1024.Index(), 0U, __LINE__);
    test.IsEqual(SizeT32(b_int_64_1024), 1U, __LINE__);
}

static void TestBigInt7(QTest &test) {
    StringStream<char> stream;
    BigInt_64_1024     b_int_64_1024{1U};

    for (SizeT32 i = 0; i < 16; i++) {
        b_int_64_1024.Multiply(18446744073709551615UL);
    }

    PrintDigits(b_int_64_1024, stream);

    test.IsEqual(b_int_64_1024.Index(), 15U, __LINE__);
    test.IsEqual(
        stream,
        "179769313486231590617005494896502488139538923424507473845653439431848569886227202866765261632299351819569917639009010788373365912036255753178371299382143631760131695224907130882552454362167933328609537509415576609030163673758148226168953269623548572115351901405836315903312675793605327103910016259918212890625",
        __LINE__);

    ////////////////////////////

    for (SizeT32 i = 0; i < 15; i++) {
        b_int_64_1024.Divide(18446744073709551615UL);
    }

    PrintDigits(b_int_64_1024, stream);

    test.IsEqual(b_int_64_1024.Index(), 0U, __LINE__);
    test.IsEqual(stream, "18446744073709551615", __LINE__);

    ////////////////////////////
    b_int_64_1024 = 1U;

    for (SizeT32 i = 0; i < 16; i++) {
        b_int_64_1024.Multiply(7450580596923828125UL);
    }

    PrintDigits(b_int_64_1024, stream);

    test.IsEqual(b_int_64_1024.Index(), 15U, __LINE__);
    test.IsEqual(
        stream,
        "90165806814313825983973933227508139041490368359926875342795693812529005764464055301218345628050495798421500774741975300465031443140291383385712235482720436807778572320717638422232867180676543016238356187158612124626088658407772165881848454717422232232161596041965234693549291478120721876621246337890625",
        __LINE__);
    ////////////////////////////

    for (SizeT32 i = 0; i < 16; i++) {
        b_int_64_1024.Divide(7450580596923828125UL);
    }

    PrintDigits(b_int_64_1024, stream);

    test.IsEqual(b_int_64_1024.Index(), 0U, __LINE__);
    test.IsEqual(stream, "1", __LINE__);
}

static void TestBigInt8(QTest &test) {
    StringStream<char> stream;
    BigInt_64_1024     b_int;

    b_int = 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;

    PrintDigits(b_int, stream);

    test.IsEqual(b_int.Index(), 15U, __LINE__);
    test.IsEqual(
        stream,
        "179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137215",
        __LINE__);
}

static int RunBigIntTests() {
    QTest test{"BigInt.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("BigInt Test 1", TestBigInt1);
    test.Test("BigInt Test 2", TestBigInt2);
    test.Test("BigInt Test 3", TestBigInt3);
    test.Test("BigInt Test 4", TestBigInt4);
    test.Test("BigInt Test 5", TestBigInt5);
    test.Test("BigInt Test 6", TestBigInt6);
    test.Test("BigInt Test 7", TestBigInt7);
    test.Test("BigInt Test 8", TestBigInt8);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
