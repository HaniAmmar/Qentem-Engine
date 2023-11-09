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

#if (QENTEM_AVX2 == 1) || (QENTEM_SSE2 == 1) || (QENTEM_MSIMD128 == 1)
#if (QENTEM_MSIMD128 == 1)
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
#define QENTEM_SIMD_SET_TO_ONE_16 _mm256_set1_epi16
#define QENTEM_SIMD_SET_TO_ONE_32 _mm256_set1_epi32
#define QENTEM_SIMD_SET_TO_ONE_64 _mm256_set1_epi64x
#define QENTEM_SIMD_STOREU _mm256_storeu_si256
#define QENTEM_SIMD_COMPARE_8_MASK(a, b)                                                                               \
    static_cast<QENTEM_SIMD_NUMBER_T>(_mm256_movemask_epi8(_mm256_cmpeq_epi8(a, b)))
#define QENTEM_SIMD_COMPARE_16_MASK(a, b)                                                                              \
    static_cast<QENTEM_SIMD_NUMBER_T>(_mm256_movemask_epi8(_mm256_cmpeq_epi16(a, b)))
#define QENTEM_SIMD_COMPARE_32_MASK(a, b)                                                                              \
    static_cast<QENTEM_SIMD_NUMBER_T>(_mm256_movemask_epi8(_mm256_cmpeq_epi32(a, b)))
#elif defined(QENTEM_SSE2) && (QENTEM_SSE2 == 1)
using QENTEM_SIMD_NUMBER_T = unsigned int;
#define QENTEM_SIMD_SIZE 16U
#define QENTEM_SIMD_SHIFT_SIZE 4U
#define QENTEM_SIMD_MAX_NUMBER 0xFFFFU
#define QENTEM_SIMD_VAR __m128i
#define QENTEM_SIMD_LOAD _mm_loadu_si128
#define QENTEM_SIMD_ZERO _mm_setzero_si128()
#define QENTEM_SIMD_SET_TO_ONE_8 _mm_set1_epi8
#define QENTEM_SIMD_SET_TO_ONE_16 _mm_set1_epi16
#define QENTEM_SIMD_SET_TO_ONE_32 _mm_set1_epi32
#define QENTEM_SIMD_SET_TO_ONE_64 _mm_set1_epi64x
#define QENTEM_SIMD_STOREU _mm_storeu_si128
#define QENTEM_SIMD_COMPARE_8_MASK(a, b) static_cast<QENTEM_SIMD_NUMBER_T>(_mm_movemask_epi8(_mm_cmpeq_epi8(a, b)))
#define QENTEM_SIMD_COMPARE_16_MASK(a, b) static_cast<QENTEM_SIMD_NUMBER_T>(_mm_movemask_epi8(_mm_cmpeq_epi16(a, b)))
#define QENTEM_SIMD_COMPARE_32_MASK(a, b) static_cast<QENTEM_SIMD_NUMBER_T>(_mm_movemask_epi8(_mm_cmpeq_epi32(a, b)))
#elif defined(QENTEM_MSIMD128) && (QENTEM_MSIMD128 == 1)
using QENTEM_SIMD_NUMBER_T = unsigned int;
#define QENTEM_SIMD_SIZE 16U
#define QENTEM_SIMD_SHIFT_SIZE 4U
#define QENTEM_SIMD_MAX_NUMBER 0xFFFFU
#define QENTEM_SIMD_VAR v128_t
#define QENTEM_SIMD_LOAD wasm_v128_load
#define QENTEM_SIMD_ZERO wasm_i64x2_const_splat(0)
#define QENTEM_SIMD_SET_TO_ONE_8 wasm_i8x16_splat
#define QENTEM_SIMD_SET_TO_ONE_16 wasm_i16x8_splat
#define QENTEM_SIMD_SET_TO_ONE_32 wasm_i32x4_splat
#define QENTEM_SIMD_SET_TO_ONE_64 wasm_i64x2_splat
#define QENTEM_SIMD_STOREU wasm_v128_store
#define QENTEM_SIMD_COMPARE_8_MASK(a, b) static_cast<QENTEM_SIMD_NUMBER_T>(wasm_i8x16_bitmask(wasm_i8x16_eq(a, b)))
#define QENTEM_SIMD_COMPARE_16_MASK(a, b) static_cast<QENTEM_SIMD_NUMBER_T>(wasm_i8x16_bitmask(wasm_i16x8_eq(a, b)))
#define QENTEM_SIMD_COMPARE_32_MASK(a, b) static_cast<QENTEM_SIMD_NUMBER_T>(wasm_i8x16_bitmask(wasm_i32x4_eq(a, b)))
#endif

#ifdef _MSC_VER
#include <intrin.h>
#ifdef QENTEM_64BIT_ARCH
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
template <typename Number_T_>
inline static unsigned int CTZ(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr unsigned int size  = (sizeof(Number_T_) * 8U);
    unsigned long          index = 0;

    switch (size) {
        case 64U: {
            _BitScanForward64(&index, value);
        }

        default: {
            _BitScanForward(&index, value);
        }
    }

    return static_cast<unsigned int>(index);
}

template <typename Number_T_>
inline static unsigned int CLZ(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr unsigned int size  = (sizeof(Number_T_) * 8U);
    unsigned long          index = 0;

    switch (size) {
        case 64U: {
            _BitScanForward64(&index, value);
        }

        default: {
            _BitScanReverse(&index, value);
        }
    }

    return static_cast<unsigned int>(index);
}
#else
template <typename Number_T_>
inline static unsigned int CTZ(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    unsigned long index = 0;
    _BitScanForward(&index, value);
    return static_cast<unsigned int>(index);
}

template <typename Number_T_>
inline static unsigned int CLZ(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    unsigned long index = 0;
    _BitScanReverse(&index, value);
    return static_cast<unsigned int>(index);
}
#endif
///////////////////////////////////////
#else

#ifdef QENTEM_64BIT_ARCH

template <typename Number_T_>
inline static unsigned int CTZ(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr unsigned int size = (sizeof(Number_T_) * 8U);

    switch (size) {
        case 64U: {
            return static_cast<unsigned int>(__builtin_ctzl(static_cast<unsigned long>(value)));
        }

        default: {
            return static_cast<unsigned int>(__builtin_ctz(static_cast<unsigned int>(value)));
        }
    }
}

template <typename Number_T_>
inline static unsigned int CLZ(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr unsigned int size = (sizeof(Number_T_) * 8U) - 1U;

    switch (size) {
        case 63U: {
            return (size - static_cast<unsigned int>(__builtin_clzl(static_cast<unsigned long>(value))));
        }

        default: {
            return (size - static_cast<unsigned int>(__builtin_clz(static_cast<unsigned int>(value))));
        }
    }
}

#else

template <typename Number_T_>
inline static unsigned int CTZ(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    return static_cast<unsigned int>(__builtin_ctz(static_cast<unsigned int>(value)));
}

template <typename Number_T_>
inline static unsigned int CLZ(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr unsigned int size = (sizeof(int) * 8U) - 1U;
    return (size - static_cast<unsigned int>(__builtin_clz(static_cast<unsigned int>(value))));
}

#endif
#endif
///////////////////////////////////////
#ifdef QENTEM_SIMD_ENABLED

template <typename Char_T_, typename SIMDValue, int CharSize>
struct SMIDCompare_T {};

template <typename Char_T_, typename SIMDValue>
inline static QENTEM_SIMD_NUMBER_T SMIDCompare(const SIMDValue &val1, const SIMDValue &val2) noexcept {
    return SMIDCompare_T<Char_T_, SIMDValue, static_cast<int>(sizeof(Char_T_))>::Compare(val1, val2);
}

// char
template <typename Char_T_, typename SIMDValue>
struct SMIDCompare_T<Char_T_, SIMDValue, 1> {
    inline static QENTEM_SIMD_NUMBER_T Compare(const SIMDValue &val1, const SIMDValue &val2) noexcept {
        return QENTEM_SIMD_COMPARE_8_MASK(val1, val2);
    }
};

// char16
template <typename Char_T_, typename SIMDValue>
struct SMIDCompare_T<Char_T_, SIMDValue, 2> {
    inline static QENTEM_SIMD_NUMBER_T Compare(const SIMDValue &val1, const SIMDValue &val2) noexcept {
        QENTEM_SIMD_NUMBER_T bits16 = QENTEM_SIMD_COMPARE_16_MASK(val1, val2);
        QENTEM_SIMD_NUMBER_T bits   = 0;
        unsigned int         count  = 0;
        const unsigned int   shift  = 2U;

        while (bits16 != 0) {
            bits |= ((bits16 & 1U) << count);
            bits16 >>= shift;
            ++count;
        }

        return bits;
    }
};

// char32_t
template <typename Char_T_, typename SIMDValue>
struct SMIDCompare_T<Char_T_, SIMDValue, 4> {
    inline static QENTEM_SIMD_NUMBER_T Compare(const SIMDValue &val1, const SIMDValue &val2) noexcept {
        QENTEM_SIMD_NUMBER_T bits32 = QENTEM_SIMD_COMPARE_32_MASK(val1, val2);
        QENTEM_SIMD_NUMBER_T bits   = 0;
        unsigned int         count  = 0;
        const unsigned int   shift  = 4U;

        while (bits32 != 0) {
            bits |= ((bits32 & 1U) << count);
            bits32 >>= shift;
            ++count;
        }

        return bits;
    }
};

//////////////////////////////////

template <typename Char_T_, int S>
struct SMIDSetToOne_T {};

template <typename Char_T_>
inline static QENTEM_SIMD_VAR SMIDSetToOne(const Char_T_ value) noexcept {
    return SMIDSetToOne_T<Char_T_, static_cast<int>(sizeof(Char_T_))>::Set(value);
}

// char
template <typename Char_T_>
struct SMIDSetToOne_T<Char_T_, 1> {
    inline static QENTEM_SIMD_VAR Set(const Char_T_ value) noexcept {
        return QENTEM_SIMD_SET_TO_ONE_8(value);
    }
};

// char16
template <typename Char_T_>
struct SMIDSetToOne_T<Char_T_, 2> {
    inline static QENTEM_SIMD_VAR Set(const Char_T_ value) noexcept {
        return QENTEM_SIMD_SET_TO_ONE_16(static_cast<short>(value));
    }
};

// char32_t
template <typename Char_T_>
struct SMIDSetToOne_T<Char_T_, 4> {
    inline static QENTEM_SIMD_VAR Set(const Char_T_ value) noexcept {
        return QENTEM_SIMD_SET_TO_ONE_32(static_cast<int>(value));
    }
};

template <typename Char_T_, typename Number_T_>
inline constexpr Number_T_ SMIDNextOffset() noexcept {
    return static_cast<Number_T_>(QENTEM_SIMD_SIZE / static_cast<Number_T_>(sizeof(Char_T_)));
}
#endif

} // namespace Platform
} // namespace Qentem

#endif
