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

#if defined(QENTEM_AVX2) && (QENTEM_AVX2 == 1)
struct QentemSIMD {
    using Number_T = unsigned int;
    using VAR_T    = __m256i;

    static constexpr unsigned int ShiftWidth = 5U;
    static constexpr unsigned int Size       = 32U;

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

    static VAR_T SetToOne64Bit(long long value) noexcept {
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
struct QentemSIMD {
    using Number_T = unsigned int;
    using VAR_T    = __m128i;

    static constexpr unsigned int ShiftWidth = 4U;
    static constexpr unsigned int Size       = 16U;

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

    static VAR_T SetToOne64Bit(long long value) noexcept {
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
struct QentemSIMD {
    using Number_T = unsigned int;
    using VAR_T    = v128_t;

    static constexpr unsigned int ShiftWidth = 4U;
    static constexpr unsigned int Size       = 16U;

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

    static VAR_T SetToOne64Bit(long long value) noexcept {
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
#endif

namespace Qentem::Platform {

#ifdef _MSC_VER

template <typename Number_T_>
inline static unsigned int FindFirstBit(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr unsigned int size  = (sizeof(Number_T_) * 8U);
    unsigned long          index = 0;

    if constexpr (Config::Is64bit) {
        if constexpr (size == 64U) {
            _BitScanForward64(&index, (unsigned long long)(value));
        } else {
            _BitScanForward(&index, (unsigned long)(value));
        }

        return (unsigned int)(index);
    } else {
        constexpr unsigned int int_size = (sizeof(int) * 8U);

        if constexpr (size == 64U) {
            // 01010101 <---
            const unsigned long lower_bits = (unsigned long)(value);

            if (lower_bits != 0U) {
                _BitScanForward(&index, lower_bits);
                return (unsigned int)(index);
            }

            value >>= int_size;
            _BitScanForward(&index, (unsigned long)(value));
            return ((unsigned int)(index) + int_size);
        } else {
            _BitScanForward(&index, (unsigned long)(value));
            return (unsigned int)(index);
        }
    }
}

template <typename Number_T_>
inline static unsigned int FindLastBit(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr unsigned int size  = (sizeof(Number_T_) * 8U);
    unsigned long          index = 0;

    if constexpr (Config::Is64bit) {
        if constexpr (size == 64U) {
            _BitScanReverse64(&index, (unsigned long long)(value));
        } else {
            _BitScanReverse(&index, (unsigned long)(value));
        }

        return (unsigned int)(index);
    } else {
        constexpr unsigned int int_size = (sizeof(int) * 8U);

        if constexpr (size == 64U) {
            // 01010101 <---
            const unsigned long lower_bits = (unsigned long)(value);
            value >>= int_size;

            if (value == Number_T_{0}) {
                _BitScanReverse(&index, lower_bits);
                return (unsigned int)(index);
            }

            _BitScanReverse(&index, (unsigned long)(value));
            return ((unsigned int)(index) + int_size);
        } else {
            _BitScanReverse(&index, (unsigned long)(value));
            return (unsigned int)(index);
        }
    }
}

#else

template <typename Number_T_>
inline static unsigned int FindFirstBit(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr unsigned int size = (sizeof(Number_T_) * 8U);

    if constexpr (Config::Is64bit) {
        if constexpr (size == 64U) {
            return (unsigned int)(__builtin_ctzl((unsigned long)(value)));
        }

        return (unsigned int)(__builtin_ctz((unsigned int)(value)));
    } else {
        constexpr unsigned int int_size = (sizeof(int) * 8U);

        if constexpr (size == 64U) {
            // 01010101 <---
            const unsigned int lower_bits = (unsigned int)(value);

            if (lower_bits != 0U) {
                return (unsigned int)(__builtin_ctz(lower_bits));
            }

            value >>= int_size;
            return ((unsigned int)(__builtin_ctz((unsigned int)(value))) + int_size);
        }

        return (unsigned int)(__builtin_ctz((unsigned int)(value)));
    }
}

template <typename Number_T_>
inline static unsigned int FindLastBit(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr unsigned int int_size   = (sizeof(int) * 8U);
    constexpr unsigned int taken_size = (int_size - 1U);
    constexpr unsigned int size       = ((sizeof(Number_T_) * 8U) - 1U);

    if constexpr (Config::Is64bit) {
        if constexpr (size == 63U) {
            return (size - (unsigned int)(__builtin_clzl((unsigned long)(value))));
        }

        return (taken_size - (unsigned int)(__builtin_clz((unsigned int)(value))));
    } else {
        if constexpr (size == 63U) {
            // ---> 01010101
            const unsigned int lower_bits = (unsigned int)(value);
            value >>= int_size;

            if (value == Number_T_{0}) {
                return (taken_size - (unsigned int)(__builtin_clz(lower_bits)));
            }

            return ((taken_size - (unsigned int)(__builtin_clz((unsigned int)(value)))) + int_size);
        }

        return (taken_size - (unsigned int)(__builtin_clz((unsigned int)(value))));
    }
}
#endif

// These two are used during compiling, so do not bother optimizing them.
template <typename Number_T_>
inline static constexpr unsigned int FindFirstBitConst(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    unsigned int index = 0;

    if (value != Number_T_{0}) {
        while ((value & Number_T_{1}) == Number_T_{0}) {
            ++index;
            value >>= 1U;
        }
    }

    return index;
}

template <typename Number_T_>
inline static constexpr unsigned int FindLastBitConst(Number_T_ value) noexcept {
    // 'value' should be bigger than zero.
    constexpr unsigned int size  = ((sizeof(Number_T_) * 8U) - 1U);
    constexpr Number_T_    mask  = (Number_T_{1} << size);
    unsigned int           index = size;

    if (value != Number_T_{0}) {
        while ((value & mask) == Number_T_{0}) {
            --index;
            value <<= 1U;
        }
    }

    return index;
}
///////////////////////////////////////
#ifdef QENTEM_SIMD_ENABLED

template <typename, typename, unsigned int CharSize>
struct SMIDCompare_T {};

template <typename Char_T_, typename SIMDValue>
inline static constexpr QentemSIMD::Number_T SMIDCompare(const SIMDValue &val1, const SIMDValue &val2) noexcept {
    return SMIDCompare_T<Char_T_, SIMDValue, sizeof(Char_T_)>::Compare(val1, val2);
}

// char
template <typename Char_T_, typename SIMDValue>
struct SMIDCompare_T<Char_T_, SIMDValue, 1U> {
    inline constexpr static QentemSIMD::Number_T Compare(const SIMDValue &val1, const SIMDValue &val2) noexcept {
        return QentemSIMD::Compare8Bit(val1, val2);
    }
};

// char16
template <typename Char_T_, typename SIMDValue>
struct SMIDCompare_T<Char_T_, SIMDValue, 2U> {
    inline static QentemSIMD::Number_T Compare(const SIMDValue &val1, const SIMDValue &val2) noexcept {
        QentemSIMD::Number_T bits16 = QentemSIMD::Compare16Bit(val1, val2);
        QentemSIMD::Number_T bits   = 0;
        unsigned int         count  = 0U;
        const unsigned int   shift  = 2U;

        while (bits16 != 0) {
            bits |= ((bits16 & QentemSIMD::Number_T{1}) << count);
            bits16 >>= shift;
            ++count;
        }

        return bits;
    }
};

// char32_t
template <typename Char_T_, typename SIMDValue>
struct SMIDCompare_T<Char_T_, SIMDValue, 4U> {
    inline static QentemSIMD::Number_T Compare(const SIMDValue &val1, const SIMDValue &val2) noexcept {
        QentemSIMD::Number_T bits32 = QentemSIMD::Compare32Bit(val1, val2);
        QentemSIMD::Number_T bits   = 0;
        unsigned int         count  = 0U;
        const unsigned int   shift  = 4U;

        while (bits32 != 0) {
            bits |= ((bits32 & QentemSIMD::Number_T{1}) << count);
            bits32 >>= shift;
            ++count;
        }

        return bits;
    }
};

//////////////////////////////////

template <typename, unsigned int S>
struct SMIDSetToOne_T {};

template <typename Char_T_>
inline static constexpr QentemSIMD::VAR_T SMIDSetToOne(const Char_T_ value) noexcept {
    return SMIDSetToOne_T<Char_T_, sizeof(Char_T_)>::Set(value);
}

// char
template <typename Char_T_>
struct SMIDSetToOne_T<Char_T_, 1U> {
    inline static constexpr QentemSIMD::VAR_T Set(const Char_T_ value) noexcept {
        return QentemSIMD::SetToOne8Bit(value);
    }
};

// char16
template <typename Char_T_>
struct SMIDSetToOne_T<Char_T_, 2U> {
    inline static constexpr QentemSIMD::VAR_T Set(const Char_T_ value) noexcept {
        return QentemSIMD::SetToOne16Bit(short(value));
    }
};

// char32_t
template <typename Char_T_>
struct SMIDSetToOne_T<Char_T_, 4U> {
    inline static constexpr QentemSIMD::VAR_T Set(const Char_T_ value) noexcept {
        return QentemSIMD::SetToOne32Bit(int(value));
    }
};

template <typename Char_T_, typename Number_T_>
inline static constexpr Number_T_ SMIDNextOffset() noexcept {
    constexpr Number_T_ offset = (QentemSIMD::Size / sizeof(Char_T_));
    return offset;
}
#endif

} // namespace Qentem::Platform

#endif
