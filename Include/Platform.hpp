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

#if QENTEM_AVX2 == 1 || QENTEM_SSE2 == 1 || QENTEM_MSIMD128 == 1
#if QENTEM_MSIMD128 == 1
#include <wasm_simd128.h>
#else
#include <immintrin.h>
#endif
#define QENTEM_SIMD_ENABLED
#endif

#if defined(QENTEM_AVX2) && (QENTEM_AVX2 == 1)
using QENTEM_SIMD_NUMBER_T = unsigned int;
// #define QMM_TABLE_
#define QENTEM_SIMD_SIZE 32U
#define QENTEM_SIMD_SHIFT_SIZE 5U
#define QENTEM_SIMD_MAX_NUMBER 0xFFFFFFFFU
#define QENTEM_SIMD_VAR __m256i
#define QENTEM_SIMD_LOAD _mm256_loadu_si256
#define QENTEM_SIMD_ZERO _mm256_setzero_si256()
#define QENTEM_SIMD_SET_TO_ONE_8 _mm256_set1_epi8
#define QENTEM_SIMD_SET_TO_ONE_64 _mm256_set1_epi64x
#define QENTEM_SIMD_STOREU _mm256_storeu_si256
#define QENTEM_SIMD_COMPARE_8_MASK(a, b)                                       \
    static_cast<QENTEM_SIMD_NUMBER_T>(                                         \
        _mm256_movemask_epi8(_mm256_cmpeq_epi8(a, b)))
#define QENTEM_SIMD_COMPARE_16_MASK(a, b)                                      \
    static_cast<QENTEM_SIMD_NUMBER_T>(                                         \
        _mm256_movemask_epi16(_mm256_cmpeq_epi16(a, b)))
#define QENTEM_COMPARE_16_MASK_8(a, b)                                         \
    static_cast<QENTEM_SIMD_NUMBER_T>(                                         \
        _mm256_movemask_epi8(_mm256_cmpeq_epi16(a, b)))
#elif defined(QENTEM_SSE2) && (QENTEM_SSE2 == 1)
using QENTEM_SIMD_NUMBER_T = unsigned int;
#define QENTEM_SIMD_SIZE 16U
#define QENTEM_SIMD_SHIFT_SIZE 4U
#define QENTEM_SIMD_MAX_NUMBER 0xFFFFU
#define QENTEM_SIMD_VAR __m128i
#define QENTEM_SIMD_LOAD _mm_loadu_si128
#define QENTEM_SIMD_ZERO _mm_setzero_si128()
#define QENTEM_SIMD_SET_TO_ONE_8 _mm_set1_epi8
#define QENTEM_SIMD_SET_TO_ONE_64 _mm_set1_epi64x
#define QENTEM_SIMD_STOREU _mm_storeu_si128
#define QENTEM_SIMD_COMPARE_8_MASK(a, b)                                       \
    static_cast<QENTEM_SIMD_NUMBER_T>(_mm_movemask_epi8(_mm_cmpeq_epi8(a, b)))
#define QENTEM_COMPARE_16_MASK_8(a, b)                                         \
    static_cast<QENTEM_SIMD_NUMBER_T>(_mm_movemask_epi8(_mm_cmpeq_epi16(a, b)))
#elif defined(QENTEM_MSIMD128) && (QENTEM_MSIMD128 == 1)
using QENTEM_SIMD_NUMBER_T = unsigned int;
#define QENTEM_SIMD_SIZE 16U
#define QENTEM_SIMD_SHIFT_SIZE 4U
#define QENTEM_SIMD_MAX_NUMBER 0xFFFFU
#define QENTEM_SIMD_VAR v128_t
#define QENTEM_SIMD_LOAD wasm_v128_load
#define QENTEM_SIMD_ZERO wasm_i64x2_const_splat(0)
#define QENTEM_SIMD_SET_TO_ONE_8 wasm_i8x16_splat
#define QENTEM_SIMD_SET_TO_ONE_64 wasm_i64x2_splat
#define QENTEM_SIMD_STOREU wasm_v128_store
#define QENTEM_SIMD_COMPARE_8_MASK(a, b)                                       \
    static_cast<QENTEM_SIMD_NUMBER_T>(wasm_i8x16_bitmask(wasm_i8x16_eq(a, b)))
#define QENTEM_COMPARE_16_MASK_8(a, b)                                         \
    static_cast<QENTEM_SIMD_NUMBER_T>(wasm_i8x16_bitmask(wasm_i16x8_eq(a, b)))
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

#ifdef _MSC_VER
#ifdef QENTEM_64BIT_ARCH
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

#ifdef QENTEM_64BIT_ARCH
template <typename Number_T>
inline static unsigned long CTZ(Number_T value) noexcept {
    return static_cast<unsigned long>(__builtin_ctzl(value));
}

template <typename Number_T>
inline static unsigned long CLZ(Number_T value) noexcept {
    constexpr unsigned long bits = (sizeof(long) * 8) - 1;
    return (bits - static_cast<unsigned long>(__builtin_clzl(value)));
}
#endif

inline static unsigned int CTZ(unsigned int value) noexcept {
    return static_cast<unsigned int>(__builtin_ctz(value));
}

inline static unsigned int CLZ(unsigned int value) noexcept {
    constexpr unsigned int bits = (sizeof(int) * 8) - 1;
    return (bits - static_cast<unsigned int>(__builtin_clz(value)));
}
#endif

} // namespace Platform
} // namespace Qentem

#endif
