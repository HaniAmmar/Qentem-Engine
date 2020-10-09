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

#include "Common.hpp"

#ifndef QENTEM_PLATFORM_H_
#define QENTEM_PLATFORM_H_

#if QENTEM_AVX2_ == 1
using QMM_Number_T = unsigned int;
#define QMM_SIZE_ 32U
#define QMM_SHIFTSIZE_ 5U
#define QMM_MAX_NUMBER_ 0xFFFFFFFFU
#define QMM_BIT_ONE_ 0x55555555U
#define QMM_BIT_TWO_ 0xAAAAAAAAU
#define QMM_VAR_ __m256i
#define QMM_LOAD_ _mm256_loadu_si256
#define QMM_SETZERO_ _mm256_setzero_si256
#define QMM_SETONE_8_ _mm256_set1_epi8
#define QMM_SETONE_16_ _mm256_set1_epi16
#define QMM_SETONE_32_ _mm256_set1_epi32
#define QMM_SETONE_64_ _mm256_set1_epi64x
#define QMM_STOREU_ _mm256_storeu_si256
#define QMM_COMPARE_8_MASK_(a, b)                                              \
    static_cast<QMM_Number_T>(_mm256_movemask_epi8(_mm256_cmpeq_epi8(a, b)))
#define QMM_COMPARE_16_MASK_(a, b)                                             \
    static_cast<QMM_Number_T>(_mm256_movemask_epi16(_mm256_cmpeq_epi16(a, b)))
#define QMM_COMPARE_16_MASK_8_(a, b)                                           \
    static_cast<QMM_Number_T>(_mm256_movemask_epi8(_mm256_cmpeq_epi16(a, b)))
#elif QENTEM_SSE2_ == 1
using QMM_Number_T = unsigned int;
#define QMM_SIZE_ 16U
#define QMM_SHIFTSIZE_ 4U
#define QMM_MAX_NUMBER_ 0xFFFFU
#define QMM_BIT_ONE_ 0x5555U
#define QMM_BIT_TWO_ 0xAAAAU
#define QMM_VAR_ __m128i
#define QMM_LOAD_ _mm_loadu_si128
#define QMM_SETZERO_ _mm_setzero_si128
#define QMM_SETONE_8_ _mm_set1_epi8
#define QMM_SETONE_16_ _mm_set1_epi16
#define QMM_SETONE_32_ _mm_set1_epi32
#define QMM_SETONE_64_ _mm_set1_epi64x
#define QMM_STOREU_ _mm_storeu_si128
#define QMM_COMPARE_8_MASK_(a, b)                                              \
    static_cast<QMM_Number_T>(_mm_movemask_epi8(_mm_cmpeq_epi8(a, b)))
#define QMM_COMPARE_16_MASK_8_(a, b)                                           \
    static_cast<QMM_Number_T>(_mm_movemask_epi8(_mm_cmpeq_epi16(a, b)))
#endif

#if QENTEM_AVX2_ == 1 || QENTEM_SSE2_ == 1
#include <immintrin.h>
#define QENTEM_SIMD_ENABLED_
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

namespace Qentem {
namespace Platform {

// static bool IsBigEndian() noexcept {
//     union {
//         unsigned short c;
//         unsigned char  n[2];
//     } constexpr nc = {256};

//     return (nc.n[0] == 1);
// }

#ifdef _MSC_VER
#ifdef QENTEM_64BIT_
inline static unsigned long CTZ(unsigned long long value) noexcept {
    unsigned long index = 0;
    return ((_BitScanForward64(&index, value) != 0) ? index : 64UL);
}

inline static unsigned long CLZ(unsigned long long value) noexcept {
    unsigned long index = 0;
    return ((_BitScanReverse64(&index, value) != 0) ? index : 0);
}
#endif

inline static unsigned int CTZ(unsigned long value) noexcept {
    unsigned long index = 0;
    return ((_BitScanForward(&index, value) != 0)
                ? static_cast<unsigned int>(index)
                : 32U);
}

inline static unsigned long CLZ(unsigned long value) noexcept {
    unsigned long index = 0;
    return ((_BitScanReverse(&index, value) != 0) ? index : 0);
}

inline static unsigned int CTZ(unsigned int value) noexcept {
    unsigned long index = 0;
    return ((_BitScanForward(&index, value) != 0)
                ? static_cast<unsigned int>(index)
                : 32U);
}

inline static unsigned long CLZ(unsigned int value) noexcept {
    unsigned long index = 0;
    return ((_BitScanReverse(&index, value) != 0) ? index : 0);
}

#else

#ifdef QENTEM_64BIT_
inline static unsigned long CTZ(unsigned long long value) noexcept {
    return static_cast<unsigned long>(__builtin_ctzl(value));
}

inline static unsigned long CLZ(unsigned long long value) noexcept {
    constexpr unsigned long bits = (sizeof(long) * 8) - 1;
    return (bits - static_cast<unsigned long>(__builtin_clzl(value)));
}
#endif

inline static unsigned int CTZ(unsigned int value) noexcept {
    return static_cast<unsigned int>(__builtin_ctz(value));
}

inline static unsigned int CLZ(unsigned int value) noexcept {
    constexpr unsigned int bits = (sizeof(int) * 8) - 1;
    return (bits - static_cast<unsigned int>(__builtin_clzl(value)));
}
#endif

} // namespace Platform
} // namespace Qentem

#endif
