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

#ifndef QENTEM_COMMON_H_
#define QENTEM_COMMON_H_

#ifndef QENTEM_AVX512BW_
#define QENTEM_AVX512BW_ 1
#endif

#ifndef QENTEM_AVX2_
#define QENTEM_AVX2_ 1
#endif

#ifndef QENTEM_SSE2_
#define QENTEM_SSE2_ 1
#endif

#if !defined(__AVX512BW__)
#undef QENTEM_AVX512BW_
#endif

#if !defined(__AVX2__)
#undef QENTEM_AVX2_
#endif

#if !defined(__SSE2__)
#undef QENTEM_SSE2_
#endif

#if QENTEM_AVX512BW_ == 1
using QMM_Number_T = unsigned long long;
#define QMM_SIZE_ 64U
#define QMM_SHIFTSIZE_ 6U
#define QMM_MAX_NUMBER_ 0XFFFFFFFFFFFFFFFFULL
#define QMM_VAR_ __m512i
#define QMM_LOAD_ _mm512_loadu_si512
#define QMM_SETZERO_ _mm512_setzero_si512
#define QMM_SETONE_8_ _mm512_set1_epi8
#define QMM_SETONE_64_ _mm512_set1_epi64
#define QMM_STOREU_ _mm512_storeu_si512
#define QMM_COMPARE_8_MASK_ _mm512_cmpeq_epi8_mask
#elif QENTEM_AVX2_ == 1
using QMM_Number_T = unsigned int;
#define QMM_SIZE_ 32U
#define QMM_SHIFTSIZE_ 5U
#define QMM_MAX_NUMBER_ 0XFFFFFFFFU
#define QMM_BIT_ONE_ 0x55555555U
#define QMM_BIT_TWO_ 0xAAAAAAAAU
#define QMM_VAR_ __m256i
#define QMM_LOAD_ _mm256_loadu_si256
#define QMM_SETZERO_ _mm256_setzero_si256
#define QMM_SETONE_8_ _mm256_set1_epi8
#define QMM_SETONE_16_ _mm256_set1_epi16
#define QMM_SETONE_64_ _mm256_set1_epi64x
#define QMM_STOREU_ _mm256_storeu_si256
#define QMM_COMPARE_8_MASK_(a, b)                                              \
    static_cast<QMM_Number_T>(_mm256_movemask_epi8(_mm256_cmpeq_epi8(a, b)))
#define QMM_COMPARE_16_MASK_8_(a, b)                                           \
    static_cast<QMM_Number_T>(_mm256_movemask_epi8(_mm256_cmpeq_epi16(a, b)))
#elif QENTEM_SSE2_ == 1
using QMM_Number_T = unsigned int;
#define QMM_SIZE_ 16U
#define QMM_SHIFTSIZE_ 4U
#define QMM_MAX_NUMBER_ 0XFFFFU
#define QMM_BIT_ONE_ 0x5555U
#define QMM_BIT_TWO_ 0xAAAAU
#define QMM_VAR_ __m128i
#define QMM_LOAD_ _mm_loadu_si128
#define QMM_SETZERO_ _mm_setzero_si128
#define QMM_SETONE_8_ _mm_set1_epi8
#define QMM_SETONE_16_ _mm_set1_epi16
#define QMM_SETONE_64_ _mm_set1_epi64x
#define QMM_STOREU_ _mm_storeu_si128
#define QMM_COMPARE_8_MASK_(a, b)                                              \
    static_cast<QMM_Number_T>(_mm_movemask_epi8(_mm_cmpeq_epi8(a, b)))
#define QMM_COMPARE_16_MASK_8_(a, b)                                           \
    static_cast<QMM_Number_T>(_mm_movemask_epi8(_mm_cmpeq_epi16(a, b)))
#endif

#ifdef _MSC_VER
#include <intrin.h>
#if _WIN64
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#else
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#endif
#endif

#if QENTEM_AVX512BW_ == 1 || QENTEM_AVX2_ == 1 || QENTEM_SSE2_ == 1
#include <immintrin.h>
#define QENTEM_SIMD_ENABLED_
#else
#define QMM_Number_T unsigned long // See JSON::FindCache_
#endif

namespace Qentem {
#ifdef _MSC_VER
#if _WIN64
inline unsigned long Q_CTZL(unsigned long long value) noexcept {
    unsigned long index = 0;
    return ((_BitScanForward64(&index, value) != 0) ? index : 64);
}

inline unsigned long Q_CLZL(unsigned long long value) noexcept {
    unsigned long index = 0;
    return ((_BitScanReverse64(&index, value) != 0) ? index : 0);
}
#else
inline unsigned long Q_CTZL(unsigned long value) noexcept {
    unsigned long index = 0;
    return ((_BitScanForward(&index, value) != 0) ? index : 32);
}

inline unsigned long Q_CLZL(unsigned long value) noexcept {
    unsigned long index = 0;
    return ((_BitScanReverse(&index, value) != 0) ? index : 0);
}
#endif
#elif defined(__GNUC__)
inline unsigned long Q_CTZL(unsigned long value) {
    return static_cast<unsigned long>(__builtin_ctzl(value));
}

inline static unsigned int Q_CLZL(unsigned long value) {
    constexpr unsigned int bits = (sizeof(long) * 8) - 1;

    if (value != 0) {
        return (bits - static_cast<unsigned int>(__builtin_clzl(value)));
    }

    return 0;
}
#endif

#ifndef QENTEM_DOUBLE_PRECISION_
#define QENTEM_DOUBLE_PRECISION_ 14
#endif

#if defined(_MSC_VER) && defined(_WIN64)
using ULong = unsigned long long;
#else
using ULong = unsigned long;
#endif

using UInt = unsigned int;

} // namespace Qentem

#endif
