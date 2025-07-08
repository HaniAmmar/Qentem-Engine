/**
 * @file Memory.hpp
 * @brief Declares memory management utilities for Qentem Engine.
 *
 * This header provides fundamental functions and helpers for memory allocation,
 * deallocation, and manipulation, supporting the efficient and safe operation
 * of core Qentem Engine components.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_MEMORY_H
#define QENTEM_MEMORY_H

#include "Platform.hpp"

namespace Qentem {
struct Memory {
    template <typename Type_T>
    QENTEM_INLINE static void SetToZeroByType(Type_T *des, SizeT size = 1) noexcept {
        constexpr SizeT   type_size = sizeof(Type_T);
        constexpr SizeT32 shift64   = 3U;
        constexpr SizeT32 shift32   = 2U;
        constexpr bool    is_mul8   = (type_size == ((type_size >> shift64) << shift64));
        constexpr bool    is_mul4   = (type_size == ((type_size >> shift32) << shift32));

        if QENTEM_CONST_EXPRESSION (QentemConfig::Is64bit && is_mul8) {
            SizeT    offset = 0;
            SizeT64 *des64  = reinterpret_cast<SizeT64 *>(des);

            size *= type_size >> shift64;

            while (offset < size) {
                des64[offset] = SizeT64{0};
                ++offset;
            }
        } else if QENTEM_CONST_EXPRESSION (is_mul4) {
            SizeT    offset = 0;
            SizeT32 *des32  = reinterpret_cast<SizeT32 *>(des);

            size *= type_size >> shift32;

            while (offset < size) {
                des32[offset] = SizeT32{0};
                ++offset;
            }
        } else {
            SizeT   offset = 0;
            SizeT8 *des8   = reinterpret_cast<SizeT8 *>(des);

            size *= type_size;

            while (offset < size) {
                des8[offset] = SizeT8{0};
                ++offset;
            }
        }
    }

    // size = the number of bytes
    template <typename Number_T>
    QENTEM_INLINE static void SetToZero(void *pointer, Number_T size) noexcept {
        Number_T offset = 0;

        // if (QentemConfig::IsSIMDEnabled) {
        //     const Number_T m_size = (size >> Platform::SIMD::Shift);

        //     if (m_size != 0) {
        //         offset = m_size;
        //         offset <<= Platform::SIMD::Shift;

        //         const Platform::SIMD::VAR_T  m_zero    = Platform::SIMD::Zero();
        //         Platform::SIMD::VAR_T       *m_pointer = (Platform::SIMD::VAR_T *)(pointer);
        //         const Platform::SIMD::VAR_T *end       = (m_pointer + m_size);

        //         do {
        //             Platform::SIMD::Store(m_pointer, m_zero);
        //             ++m_pointer;
        //         } while (m_pointer < end);
        //     }
        // }

        char *src = (char *)(pointer);

        while (offset < size) {
            src[offset] = 0;
            ++offset;
        }
    }

    template <typename Type_T, typename Value_T>
    QENTEM_INLINE static void SetToValue(Type_T *src, Value_T value, SizeT size) noexcept {
        SizeT offset = 0;

        while (offset < size) {
            src[offset] = value;
            ++offset;
        }
    }

    // size = the number of bytes
    template <typename Number_T>
    QENTEM_INLINE static void Copy(void *to, const void *from, Number_T size) noexcept {
        Number_T offset = 0;

        // if (QentemConfig::IsSIMDEnabled) {
        // const Number_T m_size = (size >> Platform::SIMD::Shift);

        //     if (m_size != 0) {
        //         offset = m_size;
        //         offset <<= Platform::SIMD::Shift;

        //         Platform::SIMD::VAR_T       *m_to   = (Platform::SIMD::VAR_T *)(to);
        //         const Platform::SIMD::VAR_T *m_form = (const Platform::SIMD::VAR_T *)(from);
        //         const Platform::SIMD::VAR_T *end    = (m_form + m_size);

        //         do {
        //             Platform::SIMD::Store(m_to, Platform::SIMD::Load(m_form));
        //             ++m_form;
        //             ++m_to;
        //         } while (m_form < end);
        //     }
        // }

        char       *des = (char *)(to);
        const char *src = (const char *)(from);

        while (offset < size) {
            des[offset] = src[offset];
            ++offset;
        }
    }
    /////////////////////////////////////////////////////////////////////
    template <typename Type_T>
    QENTEM_INLINE static void CopyTo(Type_T *to, const Type_T *from, SizeT size) noexcept {
        constexpr SizeT   type_size = sizeof(Type_T);
        constexpr SizeT32 shift64   = 3U;
        constexpr SizeT32 shift32   = 2U;
        constexpr bool    is_mul8   = (type_size == ((type_size >> shift64) << shift64));
        constexpr bool    is_mul4   = (type_size == ((type_size >> shift32) << shift32));

        if QENTEM_CONST_EXPRESSION (QentemConfig::Is64bit && is_mul8) {
            SizeT          offset = 0;
            SizeT64       *des64  = reinterpret_cast<SizeT64 *>(to);
            const SizeT64 *src64  = reinterpret_cast<const SizeT64 *>(from);

            size *= type_size >> shift64;

            while (offset < size) {
                des64[offset] = src64[offset];
                ++offset;
            }
        } else if QENTEM_CONST_EXPRESSION (is_mul4) {
            SizeT          offset = 0;
            SizeT32       *des32  = reinterpret_cast<SizeT32 *>(to);
            const SizeT32 *src32  = reinterpret_cast<const SizeT32 *>(from);

            size *= type_size >> shift32;

            while (offset < size) {
                des32[offset] = src32[offset];
                ++offset;
            }
        } else {
            SizeT         offset = 0;
            SizeT8       *des8   = reinterpret_cast<SizeT8 *>(to);
            const SizeT8 *src8   = reinterpret_cast<const SizeT8 *>(from);

            size *= type_size;

            while (offset < size) {
                des8[offset] = src8[offset];
                ++offset;
            }
        }
    }
    /////////////////////////////////////////////////////////////////////
    QENTEM_INLINE static SizeT AlignToPow2(SizeT n_size) noexcept {
        // Ensure scanned n_size is >= 2, so msb >= 1.
        SizeT size = SizeT(SizeT{1} << Platform::FindLastBit(n_size | SizeT{2}));

        // If n_size wasn’t already exactly that power-of-two, bump up
        size <<= SizeT32(size < n_size);

        return size;
    }
    /////////////////////////////////////////////////////////////////////
};
} // namespace Qentem

#endif
