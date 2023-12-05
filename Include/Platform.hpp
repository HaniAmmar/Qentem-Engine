/*
 * Copyright (c) 2023 Hani Ammar
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

namespace Qentem::Platform {

#if defined(QENTEM_AVX2) && (QENTEM_AVX2 == 1)
struct SIMD {
    using Number_T = SizeT32;
    using VAR_T    = __m256i;

    static constexpr SizeT32 Shift = 5U;
    static constexpr SizeT32 Size  = 32U;

    static VAR_T Load(const VAR_T *pointer) noexcept {
        return _mm256_loadu_si256(pointer);
    }

    static void Store(VAR_T *left, const VAR_T &right) noexcept {
        _mm256_storeu_si256(left, right);
    }

    static VAR_T Zero() noexcept {
        return _mm256_setzero_si256();
    }

    static VAR_T SetToOne8Bit(char value) noexcept {
        return _mm256_set1_epi8(value);
    }

    static VAR_T SetToOne16Bit(short value) noexcept {
        return _mm256_set1_epi16(value);
    }

    static VAR_T SetToOne32Bit(int value) noexcept {
        return _mm256_set1_epi32(value);
    }

    static VAR_T SetToOne64Bit(SizeT64I value) noexcept {
        return _mm256_set1_epi64x(value);
    }

    static Number_T Compare8Bit(const VAR_T &left, const VAR_T &right) noexcept {
        return (Number_T)(_mm256_movemask_epi8(_mm256_cmpeq_epi8(left, right)));
    }

    static Number_T Compare16Bit(const VAR_T &left, const VAR_T &right) noexcept {
        return (Number_T)(_mm256_movemask_epi8(_mm256_cmpeq_epi16(left, right)));
    }

    static Number_T Compare32Bit(const VAR_T &left, const VAR_T &right) noexcept {
        return (Number_T)(_mm256_movemask_epi8(_mm256_cmpeq_epi32(left, right)));
    }
};
#elif defined(QENTEM_SSE2) && (QENTEM_SSE2 == 1)
struct SIMD {
    using Number_T = SizeT32;
    using VAR_T    = __m128i;

    static constexpr SizeT32 Shift = 4U;
    static constexpr SizeT32 Size  = 16U;

    static VAR_T Load(const VAR_T *pointer) noexcept {
        return _mm_loadu_si128(pointer);
    }

    static void Store(VAR_T *left, const VAR_T &right) noexcept {
        _mm_storeu_si128(left, right);
    }

    static VAR_T Zero() noexcept {
        return _mm_setzero_si128();
    }

    static VAR_T SetToOne8Bit(char value) noexcept {
        return _mm_set1_epi8(value);
    }

    static VAR_T SetToOne16Bit(short value) noexcept {
        return _mm_set1_epi16(value);
    }

    static VAR_T SetToOne32Bit(int value) noexcept {
        return _mm_set1_epi32(value);
    }

    static VAR_T SetToOne64Bit(SizeT64I value) noexcept {
        return _mm_set1_epi64x(value);
    }

    static Number_T Compare8Bit(const VAR_T &left, const VAR_T &right) noexcept {
        return (Number_T)(_mm_movemask_epi8(_mm_cmpeq_epi8(left, right)));
    }

    static Number_T Compare16Bit(const VAR_T &left, const VAR_T &right) noexcept {
        return (Number_T)(_mm_movemask_epi8(_mm_cmpeq_epi16(left, right)));
    }

    static Number_T Compare32Bit(const VAR_T &left, const VAR_T &right) noexcept {
        return (Number_T)(_mm_movemask_epi8(_mm_cmpeq_epi32(left, right)));
    }
};
#elif defined(QENTEM_MSIMD128) && (QENTEM_MSIMD128 == 1)
struct SIMD {
    using Number_T = SizeT32;
    using VAR_T    = v128_t;

    static constexpr SizeT32 Shift = 4U;
    static constexpr SizeT32 Size  = 16U;

    static VAR_T Load(const VAR_T *pointer) noexcept {
        return wasm_v128_load(pointer);
    }

    static void Store(VAR_T *left, const VAR_T &right) noexcept {
        wasm_v128_store(left, right);
    }

    static VAR_T Zero() noexcept {
        return wasm_i64x2_const_splat(0);
    }

    static VAR_T SetToOne8Bit(char value) noexcept {
        return wasm_i8x16_splat(value);
    }

    static VAR_T SetToOne16Bit(short value) noexcept {
        return wasm_i16x8_splat(value);
    }

    static VAR_T SetToOne32Bit(int value) noexcept {
        return wasm_i32x4_splat(value);
    }

    static VAR_T SetToOne64Bit(SizeT64I value) noexcept {
        return wasm_i64x2_splat(value);
    }

    static Number_T Compare8Bit(const VAR_T &left, const VAR_T &right) noexcept {
        return (Number_T)(wasm_i8x16_bitmask(wasm_i8x16_eq(left, right)));
    }

    static Number_T Compare16Bit(const VAR_T &left, const VAR_T &right) noexcept {
        return (Number_T)(wasm_i8x16_bitmask(wasm_i16x8_eq(left, right)));
    }

    static Number_T Compare32Bit(const VAR_T &left, const VAR_T &right) noexcept {
        return (Number_T)(wasm_i8x16_bitmask(wasm_i32x4_eq(left, right)));
    }
};
#else
struct SIMD {
    using Number_T = SizeT32;
    using VAR_T    = SizeT32;

    static constexpr SizeT32 Shift = 0;
    static constexpr SizeT32 Size  = 0;

    static constexpr VAR_T Load(const VAR_T *pointer) noexcept {
        (void)pointer;
        return 0;
    }

    static constexpr void Store(const VAR_T *left, const VAR_T &right) noexcept {
        (void)left;
        (void)right;
    }

    static constexpr VAR_T Zero() noexcept {
        return 0;
    }

    static constexpr VAR_T SetToOne8Bit(char value) noexcept {
        (void)value;
        return 0;
    }

    static constexpr VAR_T SetToOne16Bit(short value) noexcept {
        (void)value;
        return 0;
    }

    static constexpr VAR_T SetToOne32Bit(int value) noexcept {
        (void)value;
        return 0;
    }

    static constexpr VAR_T SetToOne64Bit(SizeT64I value) noexcept {
        (void)value;
        return 0;
    }

    static constexpr Number_T Compare8Bit(const VAR_T &left, const VAR_T &right) noexcept {
        (void)left;
        (void)right;
        return 0;
    }

    static constexpr Number_T Compare16Bit(const VAR_T &left, const VAR_T &right) noexcept {
        (void)left;
        (void)right;
        return 0;
    }

    static constexpr Number_T Compare32Bit(const VAR_T &left, const VAR_T &right) noexcept {
        (void)left;
        (void)right;
        return 0;
    }
};
#endif

#ifdef _MSC_VER
#include <intrin.h>
#if defined(_M_X64)
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#else
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#endif

template <typename Number_T_>
inline static SizeT32 FindFirstBit(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr SizeT32 size  = (sizeof(Number_T_) * 8U);
    unsigned long     index = 0;

    if constexpr (Config::Is64bit) {
        if constexpr (size == 64U) {
            _BitScanForward64(&index, SizeT64(value));
        } else {
            _BitScanForward(&index, (unsigned long)(value));
        }

        return SizeT32(index);
    } else {
        constexpr SizeT32 int_size = (sizeof(int) * 8U);

        if constexpr (size == 64U) {
            // 01010101 <---
            const unsigned long lower_bits = (unsigned long)(value);

            if (lower_bits != 0U) {
                _BitScanForward(&index, lower_bits);
                return SizeT32(index);
            }

            value >>= int_size;
            _BitScanForward(&index, (unsigned long)(value));
            return (SizeT32(index) + int_size);
        } else {
            _BitScanForward(&index, (unsigned long)(value));
            return SizeT32(index);
        }
    }
}

template <typename Number_T_>
inline static SizeT32 FindLastBit(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr SizeT32 size  = (sizeof(Number_T_) * 8U);
    unsigned long     index = 0;

    if constexpr (Config::Is64bit) {
        if constexpr (size == 64U) {
            _BitScanReverse64(&index, SizeT64(value));
        } else {
            _BitScanReverse(&index, (unsigned long)(value));
        }

        return SizeT32(index);
    } else {
        constexpr SizeT32 int_size = (sizeof(int) * 8U);

        if constexpr (size == 64U) {
            // 01010101 <---
            const unsigned long lower_bits = (unsigned long)(value);
            value >>= int_size;

            if (value == Number_T_{0}) {
                _BitScanReverse(&index, lower_bits);
                return SizeT32(index);
            }

            _BitScanReverse(&index, (unsigned long)(value));
            return (SizeT32(index) + int_size);
        } else {
            _BitScanReverse(&index, (unsigned long)(value));
            return SizeT32(index);
        }
    }
}

#else

template <typename Number_T_>
inline static SizeT32 FindFirstBit(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr SizeT32 size = (sizeof(Number_T_) * 8U);

    if constexpr (Config::Is64bit) {
        if constexpr (size == 64U) {
            return SizeT32(__builtin_ctzl((unsigned long)(value)));
        }

        return SizeT32(__builtin_ctz(SizeT32(value)));
    } else {
        constexpr SizeT32 int_size = (sizeof(int) * 8U);

        if constexpr (size == 64U) {
            // 01010101 <---
            const SizeT32 lower_bits = SizeT32(value);

            if (lower_bits != 0U) {
                return SizeT32(__builtin_ctz(lower_bits));
            }

            value >>= int_size;
            return (SizeT32(__builtin_ctz(SizeT32(value))) + int_size);
        }

        return SizeT32(__builtin_ctz(SizeT32(value)));
    }
}

template <typename Number_T_>
inline static SizeT32 FindLastBit(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr SizeT32 int_size   = (sizeof(int) * 8U);
    constexpr SizeT32 taken_size = (int_size - 1U);
    constexpr SizeT32 size       = ((sizeof(Number_T_) * 8U) - 1U);

    if constexpr (Config::Is64bit) {
        if constexpr (size == 63U) {
            return (size - SizeT32(__builtin_clzl((unsigned long)(value))));
        }

        return (taken_size - SizeT32(__builtin_clz(SizeT32(value))));
    } else {
        if constexpr (size == 63U) {
            // ---> 01010101
            const SizeT32 lower_bits = SizeT32(value);
            value >>= int_size;

            if (value == Number_T_{0}) {
                return (taken_size - SizeT32(__builtin_clz(lower_bits)));
            }

            return ((taken_size - SizeT32(__builtin_clz(SizeT32(value)))) + int_size);
        }

        return (taken_size - SizeT32(__builtin_clz(SizeT32(value))));
    }
}
#endif

// These two are used during compiling, so do not bother optimizing them.
template <typename Number_T_>
inline static constexpr SizeT32 FindFirstBitConst(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    SizeT32 index = 0;

    if (value != Number_T_{0}) {
        while ((value & Number_T_{1}) == Number_T_{0}) {
            ++index;
            value >>= 1U;
        }
    }

    return index;
}

template <typename Number_T_>
inline static constexpr SizeT32 FindLastBitConst(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr SizeT32   size  = ((sizeof(Number_T_) * 8U) - 1U);
    constexpr Number_T_ mask  = (Number_T_{1} << size);
    SizeT32             index = size;

    if (value != Number_T_{0}) {
        while ((value & mask) == Number_T_{0}) {
            --index;
            value <<= 1U;
        }
    }

    return index;
}
///////////////////////////////////////
template <typename, typename, SizeT32>
struct SMIDCompare_T {};

template <typename Char_T_, typename SIMDValue>
inline static constexpr Platform::SIMD::Number_T SMIDCompare(const SIMDValue &val1, const SIMDValue &val2) noexcept {
    return SMIDCompare_T<Char_T_, SIMDValue, sizeof(Char_T_)>::Compare(val1, val2);
}

// char
template <typename Char_T_, typename SIMDValue>
struct SMIDCompare_T<Char_T_, SIMDValue, 1U> {
    inline constexpr static Platform::SIMD::Number_T Compare(const SIMDValue &val1, const SIMDValue &val2) noexcept {
        return Platform::SIMD::Compare8Bit(val1, val2);
    }
};

// char16
template <typename Char_T_, typename SIMDValue>
struct SMIDCompare_T<Char_T_, SIMDValue, 2U> {
    inline static Platform::SIMD::Number_T Compare(const SIMDValue &val1, const SIMDValue &val2) noexcept {
        Platform::SIMD::Number_T bits16 = Platform::SIMD::Compare16Bit(val1, val2);
        Platform::SIMD::Number_T bits   = 0;
        SizeT32                  count  = 0U;
        constexpr SizeT32        shift  = 2U;

        while (bits16 != 0) {
            while ((bits16 & Platform::SIMD::Number_T{1}) == 0) {
                bits16 >>= shift;
                ++count;
            }

            bits |= (Platform::SIMD::Number_T{1} << count);
            bits16 >>= shift;
            ++count;
        }

        return bits;
    }
};

// char32_t
template <typename Char_T_, typename SIMDValue>
struct SMIDCompare_T<Char_T_, SIMDValue, 4U> {
    inline static Platform::SIMD::Number_T Compare(const SIMDValue &val1, const SIMDValue &val2) noexcept {
        Platform::SIMD::Number_T bits32 = Platform::SIMD::Compare32Bit(val1, val2);
        Platform::SIMD::Number_T bits   = 0;
        SizeT32                  count  = 0U;
        constexpr SizeT32        shift  = 4U;

        while (bits32 != 0) {
            while ((bits32 & Platform::SIMD::Number_T{1}) == 0) {
                bits32 >>= shift;
                ++count;
            }

            bits |= (Platform::SIMD::Number_T{1} << count);
            bits32 >>= shift;
            ++count;
        }

        return bits;
    }
};

//////////////////////////////////

template <typename, SizeT32>
struct SMIDSetToOne_T {};

template <typename Char_T_>
inline static constexpr Platform::SIMD::VAR_T SMIDSetToOne(const Char_T_ value) noexcept {
    return SMIDSetToOne_T<Char_T_, sizeof(Char_T_)>::Set(value);
}

// char
template <typename Char_T_>
struct SMIDSetToOne_T<Char_T_, 1U> {
    inline static constexpr Platform::SIMD::VAR_T Set(const Char_T_ value) noexcept {
        return Platform::SIMD::SetToOne8Bit(value);
    }
};

// char16
template <typename Char_T_>
struct SMIDSetToOne_T<Char_T_, 2U> {
    inline static constexpr Platform::SIMD::VAR_T Set(const Char_T_ value) noexcept {
        return Platform::SIMD::SetToOne16Bit(short(value));
    }
};

// char32_t
template <typename Char_T_>
struct SMIDSetToOne_T<Char_T_, 4U> {
    inline static constexpr Platform::SIMD::VAR_T Set(const Char_T_ value) noexcept {
        return Platform::SIMD::SetToOne32Bit(int(value));
    }
};

template <typename Char_T_, typename Number_T_>
inline static constexpr Number_T_ SMIDNextOffset() noexcept {
    constexpr Number_T_ offset = (Platform::SIMD::Size / sizeof(Char_T_));
    return offset;
}

} // namespace Qentem::Platform

#endif
