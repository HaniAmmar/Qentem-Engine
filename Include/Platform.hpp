/**
 * @file Platform.hpp
 * @brief Provides platform detection and configuration utilities for Qentem Engine.
 *
 * This header defines macros, types, and helper functions to detect and configure
 * platform-specific features, ensuring consistent behavior and compatibility across
 * different operating systems and compiler environments.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_PLATFORM_H
#define QENTEM_PLATFORM_H

#include "QCommon.hpp"

#if (QENTEM_AVX2 == 1) || (QENTEM_SSE2 == 1) || (QENTEM_MSIMD128 == 1)
#if (QENTEM_MSIMD128 == 1)
#include <wasm_simd128.h>
#else
#include <immintrin.h>
#endif
#endif

#ifdef _MSC_VER
#include <intrin.h>
#ifdef _M_X64
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#else
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#endif // _M_X64
#endif // _MSC_VER

namespace Qentem {

struct Platform {
#if defined(QENTEM_AVX2) && (QENTEM_AVX2 == 1)
    struct SIMD {
        using Number_T = SizeT32;
        using VAR_T    = __m256i;

        static constexpr SizeT32 Shift = 5U;
        static constexpr SizeT32 Size  = 32U;

        QENTEM_INLINE static VAR_T Load(const VAR_T *pointer) noexcept {
            return _mm256_loadu_si256(pointer);
        }

        QENTEM_INLINE static void Store(VAR_T *left, const VAR_T &right) noexcept {
            _mm256_storeu_si256(left, right);
        }

        QENTEM_INLINE static VAR_T Zero() noexcept {
            return _mm256_setzero_si256();
        }

        QENTEM_INLINE static VAR_T SetToOne8Bit(char value) noexcept {
            return _mm256_set1_epi8(value);
        }

        QENTEM_INLINE static VAR_T SetToOne16Bit(short value) noexcept {
            return _mm256_set1_epi16(value);
        }

        QENTEM_INLINE static VAR_T SetToOne32Bit(int value) noexcept {
            return _mm256_set1_epi32(value);
        }

        QENTEM_INLINE static VAR_T SetToOne64Bit(SizeT64I value) noexcept {
            return _mm256_set1_epi64x(value);
        }

        QENTEM_INLINE static Number_T Compare8Bit(const VAR_T &left, const VAR_T &right) noexcept {
            return (Number_T)(_mm256_movemask_epi8(_mm256_cmpeq_epi8(left, right)));
        }

        QENTEM_INLINE static Number_T Compare16Bit(const VAR_T &left, const VAR_T &right) noexcept {
            return (Number_T)(_mm256_movemask_epi8(_mm256_cmpeq_epi16(left, right)));
        }

        QENTEM_INLINE static Number_T Compare32Bit(const VAR_T &left, const VAR_T &right) noexcept {
            return (Number_T)(_mm256_movemask_epi8(_mm256_cmpeq_epi32(left, right)));
        }
    };
#elif defined(QENTEM_SSE2) && (QENTEM_SSE2 == 1)
    struct SIMD {
        using Number_T = SizeT32;
        using VAR_T    = __m128i;

        static constexpr SizeT32 Shift = 4U;
        static constexpr SizeT32 Size  = 16U;

        QENTEM_INLINE static VAR_T Load(const VAR_T *pointer) noexcept {
            return _mm_loadu_si128(pointer);
        }

        QENTEM_INLINE static void Store(VAR_T *left, const VAR_T &right) noexcept {
            _mm_storeu_si128(left, right);
        }

        QENTEM_INLINE static VAR_T Zero() noexcept {
            return _mm_setzero_si128();
        }

        QENTEM_INLINE static VAR_T SetToOne8Bit(char value) noexcept {
            return _mm_set1_epi8(value);
        }

        QENTEM_INLINE static VAR_T SetToOne16Bit(short value) noexcept {
            return _mm_set1_epi16(value);
        }

        QENTEM_INLINE static VAR_T SetToOne32Bit(int value) noexcept {
            return _mm_set1_epi32(value);
        }

        QENTEM_INLINE static VAR_T SetToOne64Bit(SizeT64I value) noexcept {
            return _mm_set1_epi64x(value);
        }

        QENTEM_INLINE static Number_T Compare8Bit(const VAR_T &left, const VAR_T &right) noexcept {
            return (Number_T)(_mm_movemask_epi8(_mm_cmpeq_epi8(left, right)));
        }

        QENTEM_INLINE static Number_T Compare16Bit(const VAR_T &left, const VAR_T &right) noexcept {
            return (Number_T)(_mm_movemask_epi8(_mm_cmpeq_epi16(left, right)));
        }

        QENTEM_INLINE static Number_T Compare32Bit(const VAR_T &left, const VAR_T &right) noexcept {
            return (Number_T)(_mm_movemask_epi8(_mm_cmpeq_epi32(left, right)));
        }
    };
#elif defined(QENTEM_MSIMD128) && (QENTEM_MSIMD128 == 1)
    struct SIMD {
        using Number_T = SizeT32;
        using VAR_T    = v128_t;

        static constexpr SizeT32 Shift = 4U;
        static constexpr SizeT32 Size  = 16U;

        QENTEM_INLINE static VAR_T Load(const VAR_T *pointer) noexcept {
            return wasm_v128_load(pointer);
        }

        QENTEM_INLINE static void Store(VAR_T *left, const VAR_T &right) noexcept {
            wasm_v128_store(left, right);
        }

        QENTEM_INLINE static VAR_T Zero() noexcept {
            return wasm_i64x2_const_splat(0);
        }

        QENTEM_INLINE static VAR_T SetToOne8Bit(char value) noexcept {
            return wasm_i8x16_splat(value);
        }

        QENTEM_INLINE static VAR_T SetToOne16Bit(short value) noexcept {
            return wasm_i16x8_splat(value);
        }

        QENTEM_INLINE static VAR_T SetToOne32Bit(int value) noexcept {
            return wasm_i32x4_splat(value);
        }

        QENTEM_INLINE static VAR_T SetToOne64Bit(SizeT64I value) noexcept {
            return wasm_i64x2_splat(value);
        }

        QENTEM_INLINE static Number_T Compare8Bit(const VAR_T &left, const VAR_T &right) noexcept {
            return (Number_T)(wasm_i8x16_bitmask(wasm_i8x16_eq(left, right)));
        }

        QENTEM_INLINE static Number_T Compare16Bit(const VAR_T &left, const VAR_T &right) noexcept {
            return (Number_T)(wasm_i8x16_bitmask(wasm_i16x8_eq(left, right)));
        }

        QENTEM_INLINE static Number_T Compare32Bit(const VAR_T &left, const VAR_T &right) noexcept {
            return (Number_T)(wasm_i8x16_bitmask(wasm_i32x4_eq(left, right)));
        }
    };
#else
    struct SIMD {
        using Number_T = SizeT32;
        using VAR_T    = SizeT32;

        static constexpr SizeT32 Shift = 0;
        static constexpr SizeT32 Size  = 0;

        static constexpr VAR_T Load(const VAR_T *) noexcept {
            return 0;
        }

        static void Store(const VAR_T *, const VAR_T &) noexcept {
        }

        static constexpr VAR_T Zero() noexcept {
            return 0;
        }

        static constexpr VAR_T SetToOne8Bit(char) noexcept {
            return 0;
        }

        static constexpr VAR_T SetToOne16Bit(short) noexcept {
            return 0;
        }

        static constexpr VAR_T SetToOne32Bit(int) noexcept {
            return 0;
        }

        static constexpr VAR_T SetToOne64Bit(SizeT64I) noexcept {
            return 0;
        }

        static constexpr Number_T Compare8Bit(const VAR_T &, const VAR_T &) noexcept {
            return 0;
        }

        static constexpr Number_T Compare16Bit(const VAR_T &, const VAR_T &) noexcept {
            return 0;
        }

        static constexpr Number_T Compare32Bit(const VAR_T &, const VAR_T &) noexcept {
            return 0;
        }
    };
#endif // QENTEM_AVX2 // QENTEM_SSE2 // QENTEM_SSE2

#ifdef _MSC_VER
    template <typename Number_T>
    QENTEM_INLINE static QENTEM_CONST_EXPRESSION SizeT32 FindFirstBit(Number_T value) noexcept {
        // 'value' should be bigger than zero.
        constexpr bool is_size_8 = (sizeof(Number_T) == 8U);
        unsigned long  index     = 0;

#ifdef _M_X64
        if QENTEM_CONST_EXPRESSION (is_size_8) {
            _BitScanForward64(&index, SizeT64(value));
        } else {
            _BitScanForward(&index, (unsigned long)(value));
        }

        return SizeT32(index);
#else  // _M_X64
        constexpr SizeT32 int_size = (sizeof(int) * 8U);

        if QENTEM_CONST_EXPRESSION (is_size_8) {
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
#endif // _M_X64
    }

    template <typename Number_T>
    QENTEM_INLINE static QENTEM_CONST_EXPRESSION SizeT32 FindLastBit(Number_T value) noexcept {
        // 'value' should be bigger than zero.
        constexpr bool is_size_8 = (sizeof(Number_T) == 8U);
        unsigned long  index     = 0;

#ifdef _M_X64
        if QENTEM_CONST_EXPRESSION (is_size_8) {
            _BitScanReverse64(&index, SizeT64(value));
        } else {
            _BitScanReverse(&index, (unsigned long)(value));
        }

        return SizeT32(index);
#else  // _M_X64
        constexpr SizeT32 int_size = (sizeof(int) * 8U);

        if QENTEM_CONST_EXPRESSION (is_size_8) {
            // 01010101 <---
            const unsigned long lower_bits = (unsigned long)(value);
            value >>= int_size;

            if (value == 0) {
                _BitScanReverse(&index, lower_bits);
                return SizeT32(index);
            }

            _BitScanReverse(&index, (unsigned long)(value));
            return (SizeT32(index) + int_size);
        } else {
            _BitScanReverse(&index, (unsigned long)(value));
            return SizeT32(index);
        }
#endif // _M_X64
    }

#else  // _MSC_VER
    template <typename Number_T>
    QENTEM_INLINE static QENTEM_CONST_EXPRESSION SizeT32 FindFirstBit(Number_T value) noexcept {
        // 'value' should be bigger than zero.
        constexpr bool is_size_8 = (sizeof(Number_T) == 8U);

        if QENTEM_CONST_EXPRESSION (QentemConfig::Is64bit) {
            if QENTEM_CONST_EXPRESSION (is_size_8) {
                return SizeT32(__builtin_ctzl((unsigned long)(value)));
            }

            return SizeT32(__builtin_ctz(SizeT32(value)));
        } else {
            constexpr SizeT32 int_size = (sizeof(int) * 8U);

            if QENTEM_CONST_EXPRESSION (is_size_8) {
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

    template <typename Number_T>
    QENTEM_INLINE static QENTEM_CONST_EXPRESSION SizeT32 FindLastBit(Number_T value) noexcept {
        // 'value' should be bigger than zero.
        constexpr SizeT32 int_size    = (sizeof(int) * 8U);
        constexpr SizeT32 taken_size  = (int_size - 1U);
        constexpr SizeT32 size        = ((sizeof(Number_T) * 8U) - 1U);
        constexpr bool    is_size_63b = (size == 63U);

        if QENTEM_CONST_EXPRESSION (QentemConfig::Is64bit) {
            if QENTEM_CONST_EXPRESSION (is_size_63b) {
                return (size - SizeT32(__builtin_clzl((unsigned long)(value))));
            }

            return (taken_size - SizeT32(__builtin_clz(SizeT32(value))));
        } else {
            if QENTEM_CONST_EXPRESSION (is_size_63b) {
                const SizeT32 lower_bits = SizeT32(value);
                value >>= int_size;

                if (value == 0) {
                    return (taken_size - SizeT32(__builtin_clz(lower_bits)));
                }

                return ((taken_size - SizeT32(__builtin_clz(SizeT32(value)))) + int_size);
            }

            return (taken_size - SizeT32(__builtin_clz(SizeT32(value))));
        }
    }
#endif // _MSC_VER
    ///////////////////////////////////////
    template <typename, typename, SizeT32>
    struct SIMDCompare_T {};

    template <typename Char_T, typename SIMDValue>
    QENTEM_INLINE static constexpr Platform::SIMD::Number_T SIMDCompare(const SIMDValue &val1,
                                                                        const SIMDValue &val2) noexcept {
        return SIMDCompare_T<Char_T, SIMDValue, sizeof(Char_T)>::Compare(val1, val2);
    }

    // char
    template <typename Char_T, typename SIMDValue>
    struct SIMDCompare_T<Char_T, SIMDValue, 1U> {
        QENTEM_INLINE static constexpr Platform::SIMD::Number_T Compare(const SIMDValue &val1,
                                                                        const SIMDValue &val2) noexcept {
            return Platform::SIMD::Compare8Bit(val1, val2);
        }
    };

    // char16
    template <typename Char_T, typename SIMDValue>
    struct SIMDCompare_T<Char_T, SIMDValue, 2U> {
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
    template <typename Char_T, typename SIMDValue>
    struct SIMDCompare_T<Char_T, SIMDValue, 4U> {
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
    struct SIMDSetToOne_T {};

    template <typename Char_T>
    QENTEM_INLINE static constexpr Platform::SIMD::VAR_T SIMDSetToOne(const Char_T value) noexcept {
        return SIMDSetToOne_T<Char_T, sizeof(Char_T)>::Set(value);
    }

    // char
    template <typename Char_T>
    struct SIMDSetToOne_T<Char_T, 1U> {
        QENTEM_INLINE static constexpr Platform::SIMD::VAR_T Set(const Char_T value) noexcept {
            return Platform::SIMD::SetToOne8Bit(value);
        }
    };

    // char16
    template <typename Char_T>
    struct SIMDSetToOne_T<Char_T, 2U> {
        QENTEM_INLINE static constexpr Platform::SIMD::VAR_T Set(const Char_T value) noexcept {
            return Platform::SIMD::SetToOne16Bit(short(value));
        }
    };

    // char32_t
    template <typename Char_T>
    struct SIMDSetToOne_T<Char_T, 4U> {
        QENTEM_INLINE static constexpr Platform::SIMD::VAR_T Set(const Char_T value) noexcept {
            return Platform::SIMD::SetToOne32Bit(int(value));
        }
    };

    template <typename Char_T, typename Number_T>
    QENTEM_INLINE static constexpr Number_T SIMDNextOffset() noexcept {
        return (Platform::SIMD::Size / sizeof(Char_T));
    }
};

} // namespace Qentem

#endif
