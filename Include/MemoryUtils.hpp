/**
 * @file MemoryUtils.hpp
 * @brief Low-level memory fill, copy, and alignment utilities.
 *
 * Provides fast, reserve-free operations for zeroing memory, filling with a value,
 * copying data, and adjusting sizes to alignment boundaries.
 *
 * This header does *not* manage memory ownership or object construction.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_MEMORY_UTILS_H
#define QENTEM_MEMORY_UTILS_H

#include "Platform.hpp"

#ifndef QENTEM_NO_CUSTOM_PLACEMENT_NEW
inline QENTEM_INLINE void *operator new(Qentem::SystemIntType, void *pointer, bool) noexcept {
    return pointer;
}

inline QENTEM_INLINE void operator delete(void *, void *, bool) noexcept {
    // no-op, only needed to satisfy compiler
}
#endif

namespace Qentem {

struct MemoryUtils {
    template <typename Type_T, typename Number_T = SizeT32>
    QENTEM_INLINE static void SetToZeroByType(Type_T *des, Number_T size = 1) noexcept {
        constexpr Number_T type_size = sizeof(Type_T);
        constexpr SizeT32  shift64   = 3U;
        constexpr SizeT32  shift32   = 2U;
        constexpr bool     is_mul8   = (type_size == ((type_size >> shift64) << shift64));
        constexpr bool     is_mul4   = (type_size == ((type_size >> shift32) << shift32));

        if QENTEM_CONST_EXPRESSION (QentemConfig::Is64bit && is_mul8) {
            Number_T offset = 0;
            SizeT64 *des64  = reinterpret_cast<SizeT64 *>(des);

            size *= type_size >> shift64;

            while (offset < size) {
                des64[offset] = SizeT64{0};
                ++offset;
            }
        } else if QENTEM_CONST_EXPRESSION (is_mul4) {
            Number_T offset = 0;
            SizeT32 *des32  = reinterpret_cast<SizeT32 *>(des);

            size *= type_size >> shift32;

            while (offset < size) {
                des32[offset] = SizeT32{0};
                ++offset;
            }
        } else {
            Number_T offset = 0;
            SizeT8  *des8   = reinterpret_cast<SizeT8 *>(des);

            size *= type_size;

            while (offset < size) {
                des8[offset] = SizeT8{0};
                ++offset;
            }
        }
    }

    // size = the number of bytes
    // template <typename Number_T>
    // QENTEM_INLINE static void SetToZero(void *pointer, Number_T size) noexcept {
    //     Number_T offset = 0;

    //     // if (QentemConfig::IsSIMDEnabled) {
    //     //     const Number_T m_size = (size >> Platform::SIMD::Shift);

    //     //     if (m_size != 0) {
    //     //         offset = m_size;
    //     //         offset <<= Platform::SIMD::Shift;

    //     //         const Platform::SIMD::VAR_T  m_zero    = Platform::SIMD::Zero();
    //     //         Platform::SIMD::VAR_T       *m_pointer = (Platform::SIMD::VAR_T *)(pointer);
    //     //         const Platform::SIMD::VAR_T *end       = (m_pointer + m_size);

    //     //         do {
    //     //             Platform::SIMD::Store(m_pointer, m_zero);
    //     //             ++m_pointer;
    //     //         } while (m_pointer < end);
    //     //     }
    //     // }

    //     char *src = (char *)(pointer);

    //     while (offset < size) {
    //         src[offset] = 0;
    //         ++offset;
    //     }
    // }

    template <typename Type_T, typename Value_T>
    QENTEM_INLINE static void SetToValue(Type_T *src, Value_T value, SizeT size) noexcept {
        SizeT offset = 0;

        while (offset < size) {
            src[offset] = value;
            ++offset;
        }
    }

    // size = the number of bytes
    // template <typename Number_T>
    // QENTEM_INLINE static void Copy(void *to, const void *from, Number_T size) noexcept {
    //     Number_T offset = 0;

    //     // if (QentemConfig::IsSIMDEnabled) {
    //     // const Number_T m_size = (size >> Platform::SIMD::Shift);

    //     //     if (m_size != 0) {
    //     //         offset = m_size;
    //     //         offset <<= Platform::SIMD::Shift;

    //     //         Platform::SIMD::VAR_T       *m_to   = (Platform::SIMD::VAR_T *)(to);
    //     //         const Platform::SIMD::VAR_T *m_form = (const Platform::SIMD::VAR_T *)(from);
    //     //         const Platform::SIMD::VAR_T *end    = (m_form + m_size);

    //     //         do {
    //     //             Platform::SIMD::Store(m_to, Platform::SIMD::Load(m_form));
    //     //             ++m_form;
    //     //             ++m_to;
    //     //         } while (m_form < end);
    //     //     }
    //     // }

    //     char       *des = (char *)(to);
    //     const char *src = (const char *)(from);

    //     while (offset < size) {
    //         des[offset] = src[offset];
    //         ++offset;
    //     }
    // }
    /////////////////////////////////////////////////////////////////////
    template <typename Type_T, typename Number_T>
    QENTEM_INLINE static void CopyTo(Type_T *to, const void *from, Number_T size) noexcept {
        constexpr Number_T type_size = sizeof(Type_T);
        constexpr SizeT32  shift64   = 3U;
        constexpr SizeT32  shift32   = 2U;
        constexpr bool     is_mul8   = (type_size == ((type_size >> shift64) << shift64));
        constexpr bool     is_mul4   = (type_size == ((type_size >> shift32) << shift32));

        if QENTEM_CONST_EXPRESSION (QentemConfig::Is64bit && is_mul8) {
            Number_T       offset = 0;
            SizeT64       *des64  = reinterpret_cast<SizeT64 *>(to);
            const SizeT64 *src64  = static_cast<const SizeT64 *>(from);

            size *= type_size >> shift64;

            while (offset < size) {
                des64[offset] = src64[offset];
                ++offset;
            }
        } else if QENTEM_CONST_EXPRESSION (is_mul4) {
            Number_T       offset = 0;
            SizeT32       *des32  = reinterpret_cast<SizeT32 *>(to);
            const SizeT32 *src32  = static_cast<const SizeT32 *>(from);

            size *= type_size >> shift32;

            while (offset < size) {
                des32[offset] = src32[offset];
                ++offset;
            }
        } else {
            Number_T      offset = 0;
            SizeT8       *des8   = reinterpret_cast<SizeT8 *>(to);
            const SizeT8 *src8   = static_cast<const SizeT8 *>(from);

            size *= type_size;

            while (offset < size) {
                des8[offset] = src8[offset];
                ++offset;
            }
        }
    }
    /////////////////////////////////////////////////////////////////////
    template <typename Number_T>
    QENTEM_INLINE static QENTEM_CONST_EXPRESSION Number_T AlignToPow2(Number_T n_size) noexcept {
        // Ensure scanned n_size is >= 2, so msb >= 1.
        Number_T size = static_cast<Number_T>(Number_T{1} << Platform::FindLastBit(n_size | Number_T{2}));

        // If n_size wasn’t already exactly that power-of-two, bump up
        size <<= SizeT32(size < n_size);

        return size;
    }

    ///////////////////////////////////////////////////////////
    //               Construction / Initialization           //
    ///////////////////////////////////////////////////////////

    /**
     * @brief Constructs an object of type `Type_T` in-place at the given memory address.
     *
     * Uses Qentem's internal placement-new operator unless `QENTEM_NO_CUSTOM_PLACEMENT_NEW` is defined.
     * This safely constructs the object using perfect forwarding of constructor arguments.
     *
     * @tparam Type_T   The type of object to construct.
     * @tparam Values_T Constructor argument types (deduced).
     * @param item      Pointer to the memory location where the object will be constructed.
     * @param values    Arguments to forward to the constructor of `Type_T`.
     *
     * @note This function does not allocate memory. The caller must ensure `item` points to valid storage.
     */
    template <typename Type_T, typename... Values_T>
    QENTEM_INLINE static void Construct(Type_T *item, Values_T &&...values) noexcept {
#ifndef QENTEM_NO_CUSTOM_PLACEMENT_NEW
        new (item, false) Type_T{QUtility::Forward<Values_T>(values)...};
#else
        ::new (item) Type_T{QUtility::Forward<Values_T>(values)...};
#endif
    }

    /**
     * @brief Constructs a range of objects in-place, forwarding arguments to each.
     *
     * Constructs objects of type `Type_T` in the range [`item`, `end`), using the provided
     * arguments for each element. Useful for array initialization or bulk construction.
     *
     * @tparam Type_T   The type of object to construct.
     * @tparam Values_T Constructor argument types (deduced).
     * @param item      Start of the memory range.
     * @param end       One-past-the-end of the memory range.
     * @param values    Arguments to forward to each object's constructor.
     *
     * @note The caller must ensure the range is valid and uninitialized.
     */
    template <typename Type_T, typename... Values_T>
    QENTEM_INLINE static void ConstructRange(Type_T *item, const Type_T *end, Values_T &&...values) {
        while (item < end) {
            Construct(item, QUtility::Forward<Values_T>(values)...);
            ++item;
        }
    }

    ///////////////////////////////////////////////////////////
    //                    Destruction                       //
    ///////////////////////////////////////////////////////////

    /**
     * @brief Explicitly destroys an object by invoking its destructor.
     *
     * Destroys the object pointed to by `item` by directly calling its destructor.
     * Does not deallocate memory or null the pointer.
     *
     * @tparam Type_T The type of the object to destroy.
     * @param item    Pointer to the object to destroy. If `nullptr`, does nothing.
     */
    template <typename Type_T>
    QENTEM_INLINE static void Destruct(Type_T *item) noexcept {
        if (item != nullptr) {
            item->~Type_T();
        }
    }

    /**
     * @brief Destroys a range of objects in-place.
     *
     * Iterates through the range [`item`, `end`) and invokes the destructor on each object.
     * Used when releasing arrays or bulk-managed objects.
     *
     * @tparam Type_T The type of the objects to destroy.
     * @param item    Start of the object range.
     * @param end     One-past-the-end of the range.
     */
    template <typename Type_T>
    QENTEM_INLINE static void Destruct(Type_T *item, const Type_T *end) noexcept {
        while (item < end) {
            Destruct(item);
            ++item;
        }
    }
};

} // namespace Qentem

#endif // QENTEM_MEMORY_UTILS_H
