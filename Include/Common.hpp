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

#include "Platform.hpp"

#ifndef QENTEM_COMMON_H_
#define QENTEM_COMMON_H_

#ifdef _MSC_VER
#define QENTEM_NOINLINE __declspec(noinline)
#else
#define QENTEM_NOINLINE __attribute__((noinline))
#endif

#ifdef _MSC_VER
#define QENTEM_MAYBE_UNUSED_
#else
#define QENTEM_MAYBE_UNUSED_ __attribute__((unused))
#endif

#if !defined(_WIN64)
#ifndef QENTEM_AVX512BW_
#define QENTEM_AVX512BW_ 0
#endif
#endif

#ifndef QENTEM_AVX2_
#define QENTEM_AVX2_ 0
#endif

#ifndef QENTEM_SSE2_
#define QENTEM_SSE2_ 0
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
#define QMM_MAX_NUMBER_ 0xFFFFFFFFFFFFFFFFULL
#define QMM_VAR_ __m512i
#define QMM_LOAD_ _mm512_loadu_si512
#define QMM_SETZERO_ _mm512_setzero_si512
#define QMM_SETONE_8_ _mm512_set1_epi8
#define QMM_SETONE_16_ _mm512_set1_epi16
#define QMM_SETONE_32_ _mm512_set1_epi32
#define QMM_SETONE_64_ _mm512_set1_epi64
#define QMM_STOREU_ _mm512_storeu_si512
#define QMM_COMPARE_8_MASK_ _mm512_cmpeq_epi8_mask
#elif QENTEM_AVX2_ == 1
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

#if QENTEM_AVX512BW_ == 1 || QENTEM_AVX2_ == 1 || QENTEM_SSE2_ == 1
#include <immintrin.h>
#define QENTEM_SIMD_ENABLED_
#else
#define QMM_Number_T unsigned long // See JSON::FindCache_
#endif

namespace Qentem {

#ifndef QENTEM_DOUBLE_PRECISION_
#define QENTEM_DOUBLE_PRECISION_ 14
#endif

#if defined(_WIN64)
using ULong = unsigned long long;
#else
using ULong        = unsigned long;
#endif

using UInt = unsigned int;

} // namespace Qentem

#endif
