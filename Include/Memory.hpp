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
#include "QTraits.hpp"

#ifndef QENTEM_ALLOCATE
#if defined(_MSC_VER)
#define QENTEM_ALLOCATE(size) ::operator new(size)
#define QENTEM_DEALLOCATE(ptr) ::operator delete(ptr)
#define QENTEM_RAW_ALLOCATE(size) malloc(size)
#define QENTEM_RAW_DEALLOCATE(ptr) free(ptr)
#else
#define QENTEM_ALLOCATE(size) ::operator new(size)
#define QENTEM_DEALLOCATE(ptr) ::operator delete(ptr)
#define QENTEM_RAW_ALLOCATE(size) __builtin_malloc(size)
#define QENTEM_RAW_DEALLOCATE(ptr) __builtin_free(ptr)
#endif
#endif

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
    template <typename Type_T>
    inline static Type_T *AllocateAligned(SystemIntType count, SystemIntType alignment = alignof(Type_T)) noexcept {
        // constexpr SystemIntType min_align = alignof(void *);
        // alignment                         = (alignment < min_align) ? min_align : alignment;

        --alignment;
        SystemIntType padding = (alignment + sizeof(void *));

        void *raw = QENTEM_RAW_ALLOCATE((sizeof(Type_T) * count) + padding);

#ifdef QENTEM_Q_TEST_H
        MemoryRecord::AddAllocation(raw);
#endif
        void *aligned = reinterpret_cast<void *>((reinterpret_cast<SystemIntType>(raw) + sizeof(void *) + alignment) &
                                                 ~(alignment));

        *(reinterpret_cast<void **>(aligned) - 1) = raw;
        return static_cast<Type_T *>(aligned);
    }

    inline static void DeallocateAligned(void *ptr) noexcept {
        if (ptr != nullptr) {
            void *raw = (static_cast<void **>(ptr))[-1];
#ifdef QENTEM_Q_TEST_H
            MemoryRecord::RemoveAllocation(raw);
#endif
            QENTEM_RAW_DEALLOCATE(raw);
        }
    }

    template <typename Type_T>
    inline static Type_T *Allocate(SizeT size) {
        Type_T *pointer = static_cast<Type_T *>(QENTEM_ALLOCATE(SystemIntType(size * sizeof(Type_T))));

#ifdef QENTEM_Q_TEST_H
        MemoryRecord::AddAllocation(pointer);
#endif
        // TODO: Build Allocator
        return pointer;
    }

    template <typename Type_T>
    inline static void Deallocate(Type_T *pointer) noexcept {
#ifdef QENTEM_Q_TEST_H
        if (pointer != nullptr) {
            MemoryRecord::RemoveAllocation(pointer);
        }
#endif

        QENTEM_DEALLOCATE(pointer);
    }

    template <typename Type_T>
    QENTEM_INLINE static Type_T *AllocateInit() {
        Type_T *pointer = Allocate<Type_T>(1);
        Initialize(pointer);
        return pointer;
    }

    // Allocate
    template <typename Type_T, typename... Values_T>
    QENTEM_INLINE static Type_T *AllocateInit(Values_T &&...values) noexcept {
        Type_T *pointer = Allocate<Type_T>(1);
        Initialize(pointer, QUtility::Forward<Values_T>(values)...);
        return pointer;
    }

    // Initializer
    template <typename Type_T>
    QENTEM_INLINE static void Initialize(Type_T *pointer) noexcept {
        ::new (pointer) Type_T{};
    }

    // Range default initializer
    template <typename Type_T>
    QENTEM_INLINE static void InitializeRange(Type_T *pointer, const Type_T *end) noexcept {
        while (pointer < end) {
            ::new (pointer) Type_T{};
            ++pointer;
        }
    }

    // Forward initializer
    template <typename Type_T, typename... Values_T>
    QENTEM_INLINE static void Initialize(Type_T *pointer, Values_T &&...values) noexcept {
        ::new (pointer) Type_T{QUtility::Forward<Values_T>(values)...};
    }

    // Range forward initializer
    template <typename Type_T, typename... Values_T>
    QENTEM_INLINE static void InitializeRange(Type_T *pointer, const Type_T *end, Values_T &&...values) {
        while (pointer < end) {
            ::new (pointer) Type_T{QUtility::Forward<Values_T>(values)...};
            ++pointer;
        }
    }

    template <typename Type_T>
    QENTEM_INLINE static void Dispose(Type_T *item) noexcept {
        if (item != nullptr) {
            item->~Type_T();
        }
    }

    template <typename Type_T>
    QENTEM_INLINE static void Dispose(Type_T *item, const Type_T *end) noexcept {
        while (item < end) {
            Dispose(item);
            ++item;
        }
    }
};
} // namespace Qentem

#endif
